// SPDX-License-Identifier: GPL-3.0-or-later
// System bench for the Proteus 2000 board (hdl/p2k_board.sv): the CPU runs the OS from a flash
// image, the sound engine runs on a DDR3 model, MIDI comes in on UART1. No ROMs in the repo: the
// flash image and the SIMM images are the user's (tools in cores/proteus/tools).
//
//   p2k_board_tb --flash FLASH.bin [--big N=FILE] [--baby N=FILE] [--seconds S] [--midi EVENTS.txt]
//                [--lcd OUT.txt] [--sndlog OUT.log] [--raw OUT.raw] [--until TEXT] [--vol 0-100]
//                [--ddr-busy N] [--ddr-lat MIN MAX] [--seed N] [--nofilter] [--nofx]
//     --flash      2 MB flash image (big-endian words: tools/p2k_flash_image.py, or a MAME flash NVRAM)
//     --big/--baby SIMM slot N (1-4) Big / Baby images (big-endian words)
//     --seconds    system time to run (default 15)
//     --midi       "seconds hexbytes" lines (cores/sc55/scripts/mister/smf2events.py), seconds from
//                  power-on; sent to UART1 at 31250 baud
//     --lcd        the LCD text every 0.25 s when it changed, as MAME's LOG_LCD lines
//     --sndlog     every sound-chip write as the engine applies it, with its sample index
//                  (P2K_SNDLOG format: p2k/model/p2k_render replays it)
//     --raw        the engine's output, 16-bit stereo little-endian at 44.1 kHz
//     --until      stop 1 s after the LCD's first line contains TEXT
//     --ddr-busy N DDR3 BUSY on a random 1 clock in N (0: never; default 4)
//     --ddr-lat    read latency in clk_sys clocks, random in [MIN, MAX], in order (default 8 24)
// clk_sys 88.2 MHz, clk_cpu 44.1 MHz (rising edges aligned).
#include "Vp2k_board.h"
#include "verilated.h"
#ifdef P2K_TB_PUBLIC
#include "Vp2k_board___024root.h"
#endif

#include <algorithm>
#include <chrono>
#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <fstream>
#include <iterator>
#include <memory>
#include <random>
#include <string>
#include <utility>
#include <vector>

namespace {

constexpr double SYS_HZ = 88.2e6;
constexpr uint64_t DDR_ORG = 0x6000000;             // DDRAM_ADDR of the region's first word
constexpr uint64_t DDR_WORDS = 0x2000000;           // 256 MB
constexpr uint64_t SMP_BASE = 0x6000000, FX_BASE = 0x7000000, BABY_BASE = 0x7100000, CPU_BASE = 0x7200000;

std::vector<unsigned char> load(const std::string &path)
{
	std::ifstream f(path, std::ios::binary);
	if (!f)
	{
		std::fprintf(stderr, "cannot read %s\n", path.c_str());
		std::exit(2);
	}
	return std::vector<unsigned char>((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
}

struct midi_ev { double t; std::vector<uint8_t> b; };

std::vector<midi_ev> load_midi(const std::string &path)
{
	std::vector<midi_ev> v;
	FILE *f = std::fopen(path.c_str(), "r");
	if (!f)
	{
		std::perror(path.c_str());
		std::exit(2);
	}
	char line[4096];
	while (std::fgets(line, sizeof(line), f))
	{
		double t;
		char hex[4000];
		if (line[0] == '#' || std::sscanf(line, "%lf %3999s", &t, hex) != 2)
			continue;
		midi_ev e{t, {}};
		for (size_t i = 0; hex[i] && hex[i + 1]; i += 2)
		{
			unsigned x;
			std::sscanf(hex + i, "%2x", &x);
			e.b.push_back(uint8_t(x));
		}
		v.push_back(e);
	}
	std::fclose(f);
	return v;
}

} // anonymous namespace

int main(int argc, char **argv)
{
	std::string flash, big[4], baby[4], midi_path, lcd_path, log_path, raw_path, until, dram_path;
	double seconds = 15.0, hold_at = -1, hold_for = 0.01;
	int busy_mod = 4, lat_min = 8, lat_max = 24, vol = 100;
	unsigned seed = 1;
	bool filter = true, fx = true, dram_random = false;
	for (int i = 1; i < argc; i++)
	{
		const std::string a = argv[i];
		auto next = [&]() -> std::string {
			if (i + 1 >= argc) { std::fprintf(stderr, "%s needs a value\n", a.c_str()); std::exit(2); }
			return argv[++i];
		};
		if (a == "--flash") flash = next();
		else if (a == "--big" || a == "--baby")
		{
			const std::string s = next();
			const int n = std::clamp(s[0] - '1', 0, 3);
			(a == "--big" ? big : baby)[n] = s.substr(2);
		}
		else if (a == "--seconds") seconds = std::atof(next().c_str());
		else if (a == "--midi") midi_path = next();
		else if (a == "--lcd") lcd_path = next();
		else if (a == "--sndlog") log_path = next();
		else if (a == "--raw") raw_path = next();
		else if (a == "--until") until = next();
		else if (a == "--dram") dram_path = next();
		else if (a == "--vol") vol = std::atoi(next().c_str());
		else if (a == "--ddr-busy") busy_mod = std::atoi(next().c_str());
		else if (a == "--ddr-lat") { lat_min = std::atoi(next().c_str()); lat_max = std::atoi(next().c_str()); }
		else if (a == "--seed") seed = unsigned(std::atoi(next().c_str()));
		else if (a == "--nofilter") filter = false;
		else if (a == "--nofx") fx = false;
		else if (a == "--dram-random") dram_random = true;
		else if (a == "--hold-at") { hold_at = std::atof(next().c_str()); hold_for = std::atof(next().c_str()); }
		else { std::fprintf(stderr, "unknown option %s\n", a.c_str()); return 2; }
	}
	if (flash.empty() || lat_min < 1 || lat_max < lat_min)
	{
		std::fprintf(stderr, "usage: p2k_board_tb --flash FLASH.bin [options] (see the source)\n");
		return 2;
	}

	// ------------------------------------------------------------------ DDR3 contents
	std::vector<uint64_t> ddr(DDR_WORDS, 0);
	std::mt19937_64 rng(seed);
	auto put16 = [&](uint64_t base, size_t i, uint16_t w) {
		uint64_t &d = ddr[base - DDR_ORG + (i >> 2)];
		d = (d & ~(0xffffull << (16 * (i & 3)))) | (uint64_t(w) << (16 * (i & 3)));
	};
	for (size_t i = 0; i < (size_t(14) << 14); i++)          // effect delay lines: random
		ddr[FX_BASE - DDR_ORG + i] = rng();
	if (dram_random)                                         // DRAM as another core left it
		for (size_t i = 0; i < 0x100000; i++)
			ddr[CPU_BASE + 0x40000 - DDR_ORG + i] = rng();
	{
		const auto b = load(flash);
		if (b.size() != 0x200000) { std::fprintf(stderr, "%s: not 2 MB\n", flash.c_str()); return 2; }
		for (size_t i = 0; i < 0x100000; i++)
			put16(CPU_BASE, i, uint16_t((b[2 * i] << 8) | b[2 * i + 1]));
	}
	uint32_t big_words[4] = {0, 0, 0, 0}, baby_words[4] = {0, 0, 0, 0};
	for (int s = 0; s < 4; s++)
	{
		if (!big[s].empty())
		{
			const auto b = load(big[s]);
			big_words[s] = uint32_t(std::min<size_t>(b.size() / 2, 1 << 24));
			for (size_t i = 0; i < big_words[s]; i++)
				put16(SMP_BASE, (size_t(s) << 24) + i, uint16_t((b[2 * i] << 8) | b[2 * i + 1]));
		}
		if (!baby[s].empty())
		{
			const auto b = load(baby[s]);
			baby_words[s] = uint32_t(std::min<size_t>(b.size() / 2, 1 << 20));
			for (size_t i = 0; i < baby_words[s]; i++)
				put16(BABY_BASE, (size_t(s) << 20) + i, uint16_t((b[2 * i] << 8) | b[2 * i + 1]));
		}
	}
	const auto midi = midi_path.empty() ? std::vector<midi_ev>() : load_midi(midi_path);
	FILE *lcd = lcd_path.empty() ? nullptr : std::fopen(lcd_path.c_str(), "w");
	FILE *slog = log_path.empty() ? nullptr : std::fopen(log_path.c_str(), "w");
	FILE *raw = raw_path.empty() ? nullptr : std::fopen(raw_path.c_str(), "wb");
	if (slog) std::fprintf(slog, "# p2k sndlog 1\n");

	// ------------------------------------------------------------------ model
	auto ctx = std::make_unique<VerilatedContext>();
	auto top = std::make_unique<Vp2k_board>(ctx.get());
	for (int s = 0; s < 4; s++)
	{
		top->big_words[s] = big_words[s];
		top->baby_words[s] = baby_words[s];
	}
	top->master_adc = vol * 255 / 100;
	top->filter_on = filter;
	top->fx_on = fx;
	top->buttons = 0;
	top->enc_step = 0;
	top->midi_rxd = 1;
	top->flash_lock = 0;
	top->x_req_valid = 0;
	top->cpu_hold = 0;

	uint64_t cyc = 0, reads = 0, writes = 0, last_due = 0, samples = 0, starts = 0, wr_logged = 0;
	std::deque<std::pair<uint64_t, uint64_t>> rdq;
	bool bad = false;
	auto clock = [&]() {
		top->DDRAM_BUSY = busy_mod > 0 && (rng() % unsigned(busy_mod)) == 0;
		top->DDRAM_DOUT_READY = 0;
		if (!rdq.empty() && rdq.front().first <= cyc)
		{
			top->DDRAM_DOUT_READY = 1;
			top->DDRAM_DOUT = ddr[rdq.front().second];
			rdq.pop_front();
		}
		const bool even = (cyc & 1) == 0;
		top->clk_sys = 0;
		if (even) top->clk_cpu = 0;
		top->eval();
		// this clock's settled signals: DDR3 command, sound log, audio
		if (!top->DDRAM_BUSY && (top->DDRAM_RD || top->DDRAM_WE))
		{
			const uint64_t w = top->DDRAM_ADDR;
			const unsigned n = top->DDRAM_RD ? std::max(1u, unsigned(top->DDRAM_BURSTCNT)) : 1u;
			if (w < DDR_ORG || w + n > DDR_ORG + DDR_WORDS ||
			    (top->DDRAM_WE && !(w >= FX_BASE && w < FX_BASE + (14u << 14)) && !(w >= CPU_BASE && w < CPU_BASE + 0x140000)))
			{
				std::fprintf(stderr, "DDR3 %s outside its region: word %" PRIx64 "\n", top->DDRAM_RD ? "read" : "write", w);
				bad = true;
			}
			else if (top->DDRAM_RD)
				for (unsigned k = 0; k < n; k++)
				{
					const uint64_t due = std::max<uint64_t>(cyc + lat_min + rng() % unsigned(lat_max - lat_min + 1), last_due + 1);
					last_due = due;
					rdq.push_back({due, w - DDR_ORG + k});
					reads++;
				}
			else
			{
				uint64_t &d = ddr[w - DDR_ORG];
				for (int b = 0; b < 8; b++)
					if (top->DDRAM_BE & (1 << b))
						d = (d & ~(0xffull << (8 * b))) | (((top->DDRAM_DIN >> (8 * b)) & 0xff) << (8 * b));
				writes++;
			}
		}
		if (top->dbg_snd_start)
			starts++;
		if (top->dbg_snd_wr && slog)
		{
			const unsigned a = top->dbg_snd_addr;
			if (top->dbg_snd_chip == 0)
				std::fprintf(slog, "%" PRIu64 " G %04x %04x %04x\n", starts, (a & 0xfff) * 2, top->dbg_snd_data, top->dbg_snd_mask);
			else if (top->dbg_snd_chip == 1)
				std::fprintf(slog, "%" PRIu64 " H %05x %04x %04x\n", starts, a * 2, top->dbg_snd_data, top->dbg_snd_mask);
			else
				std::fprintf(slog, "%" PRIu64 " X %02x %02x %04x %04x\n", starts, (a >> 6) & 31, (a & 63) * 2, top->dbg_snd_data, top->dbg_snd_mask);
			wr_logged++;
		}
		if (top->audio_strobe)
		{
			samples++;
			if (raw)
			{
				const int16_t s[2] = { int16_t(top->audio_l), int16_t(top->audio_r) };
				std::fwrite(s, 2, 2, raw);
			}
		}
#ifdef P2K_TB_PUBLIC
		if (even && getenv("P2K_TB_FIRST") && top->rootp->p2k_board__DOT__ext_sel && top->rootp->p2k_board__DOT__ext_ack)
		{
			static int nfirst = 0;
			if (nfirst++ < atoi(getenv("P2K_TB_FIRST")))
				std::fprintf(stderr, "%04x pc %08x a %08x bs %u data %04x\n", nfirst - 1, top->rootp->p2k_board__DOT__pc,
				             top->rootp->p2k_board__DOT__a, top->rootp->p2k_board__DOT__bs,
				             top->rootp->p2k_board__DOT__bs == 3 ? top->rootp->p2k_board__DOT__cpu_dout : top->rootp->p2k_board__DOT__ext_din);
		}
#endif
		top->clk_sys = 1;
		top->clk_cpu = even ? 1 : 0;
		top->eval();
		cyc++;
	};

	// LCD text through the render port (combinational)
	std::string lcd_hex;
	auto lcd_text = [&](int line) {
		std::string s;
		if (line == 0) lcd_hex.clear();
		for (int p = 0; p < 24; p++)
		{
			top->r_line = line;
			top->r_pos = p;
			top->r_row = 0;
			top->eval();
			const uint8_t c = top->r_code;
			s += (c >= 0x20 && c < 0x7f) ? char(c) : '.';
			char h[4];
			std::snprintf(h, sizeof(h), "%02x", c);
			lcd_hex += h;
		}
		return s;
	};

	top->rst = 1;
	for (int i = 0; i < 64; i++) clock();
	top->rst = 0;

	const uint64_t end = uint64_t(seconds * SYS_HZ);
	const uint64_t lcd_every = uint64_t(0.25 * SYS_HZ);
	const double bit_cyc = SYS_HZ / 31250.0;
	size_t mi = 0, mb = 0;                  // MIDI event, byte in it
	int mbit = -1;                          // bit being sent (-1 idle, 0 start, 1-8 data, 9 stop)
	uint64_t mbit_end = 0;
	std::string l1, l2;
	uint64_t stop_at = UINT64_MAX;
	const auto wall0 = std::chrono::steady_clock::now();
	while (cyc < end && cyc < stop_at && !bad)
	{
		// MIDI: 8N1, LSB first
		if (mbit < 0 && mi < midi.size() && double(cyc) >= midi[mi].t * SYS_HZ)
		{
			mbit = 0;
			mbit_end = cyc + uint64_t(bit_cyc);
		}
		if (mbit >= 0)
		{
			const uint8_t byte = midi[mi].b[mb];
			top->midi_rxd = mbit == 0 ? 0 : mbit == 9 ? 1 : (byte >> (mbit - 1)) & 1;
			if (cyc >= mbit_end)
			{
				mbit++;
				mbit_end = cyc + uint64_t(bit_cyc);
				if (mbit == 10)
				{
					mbit = -1;
					if (++mb == midi[mi].b.size()) { mb = 0; mi++; }
					else { mbit = 0; }
				}
			}
		}
		else
			top->midi_rxd = 1;
		if (hold_at >= 0)                    // the CPU held in reset for a while, as a SIMM download does
			top->cpu_hold = double(cyc) >= hold_at * SYS_HZ && double(cyc) < (hold_at + hold_for) * SYS_HZ;
		clock();
		if (cyc % lcd_every == 0)
		{
			const std::string n1 = lcd_text(0), n2 = lcd_text(1);
			const double t = double(cyc) / SYS_HZ;
			if (n1 != l1 || n2 != l2)
			{
				l1 = n1; l2 = n2;
				if (lcd) { std::fprintf(lcd, "LCD %.2f |%s|%s| %s\n", t, l1.c_str(), l2.c_str(), lcd_hex.c_str()); std::fflush(lcd); }
			}
			const double wall = std::chrono::duration<double>(std::chrono::steady_clock::now() - wall0).count();
			std::fprintf(stderr, "t %.2f s  pc %08x  samples %" PRIu64 "  writes %" PRIu64 "  late %d  %.0f kclk/s  |%s|%s|\n",
			             t, top->cpu_pc, samples, wr_logged, int(top->late), cyc / wall / 1e3, l1.c_str(), l2.c_str());
			if (!until.empty() && stop_at == UINT64_MAX && l1.find(until) != std::string::npos)
				stop_at = cyc + uint64_t(SYS_HZ);
		}
	}
	if (lcd) std::fclose(lcd);
	if (!dram_path.empty())                    // DRAM as the MiSTer peek shows it (64-bit little-endian words)
	{
		FILE *f = std::fopen(dram_path.c_str(), "wb");
		std::fwrite(&ddr[CPU_BASE + 0x40000 - DDR_ORG], 8, 0x100000, f);
		std::fclose(f);
	}
	if (slog) std::fclose(slog);
	if (raw) std::fclose(raw);
	std::printf("time %.3f s, samples %" PRIu64 ", sound writes %" PRIu64 ", DDR3 reads %" PRIu64 " writes %" PRIu64
	            ", late %d, LCD |%s|%s|\n", double(cyc) / SYS_HZ, samples, wr_logged, reads, writes, int(top->late),
	            l1.c_str(), l2.c_str());
	return bad ? 1 : 0;
}
