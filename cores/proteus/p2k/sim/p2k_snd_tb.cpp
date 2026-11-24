// SPDX-License-Identifier: GPL-3.0-or-later
// Verilator bench for p2k/rtl/snd/p2k_snd.sv (top p2k_snd_bench: p2k_snd on synthframe_mem_ddr3):
// replays a sample-indexed sound-register log (P2K_SNDLOG, as p2k/model/p2k_render does) and writes
// 16-bit stereo raw audio at 44.1 kHz. The output must equal `p2k_render` run with the same log,
// SIMM and options, bit for bit.
//
//   p2k_snd_tb [--simm N=FILE] [--frames N] [--nofilter] [--nofx] [--ddr-busy N] [--ddr-lat MIN MAX]
//              [--seed N] SNDLOG OUT.raw
//     --ddr-busy N      DDR3 BUSY on a random 1 clock in N (0: never; default 4)
//     --ddr-lat MIN MAX read latency in clocks, random in [MIN, MAX], in order (default 8 24)
//     --seed N          random seed for the DDR3 timing and the effects region's initial contents
//
// DDR3 model: 64-bit words; sample word a at bits [16 (a mod 4) +: 16] of word a / 4 (SMP_BASE 0);
// the effects region (FX_BASE 2^24, 14 x 2^14 words) starts as random data, which the engine must
// never read before writing it.
#include "Vp2k_snd_bench.h"
#include "verilated.h"

#include <algorithm>
#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <fstream>
#include <iterator>
#include <memory>
#include <random>
#include <string>
#include <utility>
#include <vector>

namespace {

struct rec { long long sample; char chip; uint32_t a, b; uint16_t data, mask; };

std::vector<rec> read_log(const char *path)
{
	std::vector<rec> v;
	FILE *f = std::fopen(path, "r");
	if (!f)
	{
		std::perror(path);
		std::exit(2);
	}
	char line[256];
	while (std::fgets(line, sizeof(line), f))
	{
		if (line[0] == '#' || line[0] == '\n')
			continue;
		rec r{};
		long long s;
		char c;
		unsigned x, y, d, m;
		if (std::sscanf(line, "%lld %c %x %x %x %x", &s, &c, &x, &y, &d, &m) == 6 && c == 'X')
			r = { s, c, x, y >> 1, uint16_t(d), uint16_t(m) };
		else if (std::sscanf(line, "%lld %c %x %x %x", &s, &c, &x, &d, &m) == 5 && (c == 'G' || c == 'H'))
			r = { s, c, x >> 1, 0, uint16_t(d), uint16_t(m) };
		else
		{
			std::fprintf(stderr, "bad log line: %s", line);
			std::exit(2);
		}
		v.push_back(r);
	}
	std::fclose(f);
	return v;
}

} // anonymous namespace

int main(int argc, char **argv)
{
	std::vector<std::string> simm(4);
	long long frames = -1;
	bool filter = true, fx = true;
	int busy_mod = 4, lat_min = 8, lat_max = 24;
	unsigned seed = 1;
	std::vector<const char *> pos;
	for (int i = 1; i < argc; i++)
	{
		const std::string a = argv[i];
		if (a == "--simm" && i + 1 < argc)
		{
			const std::string s = argv[++i];
			simm[std::clamp(s[0] - '1', 0, 3)] = s.substr(2);
		}
		else if (a == "--frames" && i + 1 < argc)
			frames = std::atoll(argv[++i]);
		else if (a == "--ddr-busy" && i + 1 < argc)
			busy_mod = std::atoi(argv[++i]);
		else if (a == "--ddr-lat" && i + 2 < argc)
		{
			lat_min = std::atoi(argv[++i]);
			lat_max = std::atoi(argv[++i]);
		}
		else if (a == "--seed" && i + 1 < argc)
			seed = unsigned(std::atoi(argv[++i]));
		else if (a == "--nofilter")
			filter = false;
		else if (a == "--nofx")
			fx = false;
		else
			pos.push_back(argv[i]);
	}
	if (pos.size() != 2 || lat_min < 1 || lat_max < lat_min)
	{
		std::fprintf(stderr, "usage: p2k_snd_tb [--simm N=FILE] [--frames N] [--nofilter] [--nofx] "
				"[--ddr-busy N] [--ddr-lat MIN MAX] [--seed N] SNDLOG OUT.raw\n");
		return 2;
	}
	// DDR3: sample memory (4 slots of 16 M words, zero-filled as p2k_render), then the effects lines
	const size_t SMP_WORDS = size_t(1) << 24, FX_BASE = size_t(1) << 24, FX_WORDS = size_t(14) << 14;
	std::vector<uint64_t> ddr(SMP_WORDS + FX_WORDS, 0);
	for (int s = 0; s < 4; s++)
	{
		if (simm[s].empty())
			continue;
		std::ifstream f(simm[s], std::ios::binary);
		std::vector<unsigned char> b((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
		const size_t words = std::min<size_t>(b.size() / 2, 1 << 24);
		for (size_t i = 0; i < words; i++)
		{
			const size_t a = (size_t(s) << 24) + i;
			ddr[a >> 2] |= uint64_t(uint16_t((b[2 * i] << 8) | b[2 * i + 1])) << (16 * (a & 3));
		}
	}
	std::mt19937_64 rng(seed);
	for (size_t i = 0; i < FX_WORDS; i++)
		ddr[FX_BASE + i] = rng();
	const auto log = read_log(pos[0]);
	if (frames < 0)
		frames = (log.empty() ? 0 : log.back().sample) + 3 * 44100;

	auto ctx = std::make_unique<VerilatedContext>();
	auto top = std::make_unique<Vp2k_snd_bench>(ctx.get());
	uint64_t cycles = 0, reads = 0, writes = 0, last_due = 0;
	std::deque<std::pair<uint64_t, size_t>> rdq;
	auto tick = [&]() {
		top->DDRAM_BUSY = busy_mod > 0 && (rng() % unsigned(busy_mod)) == 0;
		top->DDRAM_DOUT_READY = 0;
		if (!rdq.empty() && rdq.front().first <= cycles)
		{
			top->DDRAM_DOUT_READY = 1;
			top->DDRAM_DOUT = ddr[rdq.front().second];
			rdq.pop_front();
		}
		top->clk = 0;
		top->eval();
		if (!top->DDRAM_BUSY && !top->rst && (top->DDRAM_RD || top->DDRAM_WE))
		{
			const size_t w = top->DDRAM_ADDR;
			const unsigned n = top->DDRAM_RD ? std::max(1u, unsigned(top->DDRAM_BURSTCNT)) : 1u;
			if (w + n > ddr.size() || (top->DDRAM_WE && w < FX_BASE))
			{
				std::fprintf(stderr, "DDR3 %s outside its region: word %zx\n", top->DDRAM_RD ? "read" : "write", w);
				std::exit(1);
			}
			if (top->DDRAM_RD)
				for (unsigned k = 0; k < n; k++)
				{
					const uint64_t due = std::max<uint64_t>(cycles + lat_min + rng() % unsigned(lat_max - lat_min + 1), last_due + 1);
					last_due = due;
					rdq.push_back({due, w + k});
					reads++;
				}
			else
			{
				for (int b = 0; b < 8; b++)
					if (top->DDRAM_BE & (1 << b))
						ddr[w] = (ddr[w] & ~(0xffull << (8 * b))) | (((top->DDRAM_DIN >> (8 * b)) & 0xff) << (8 * b));
				writes++;
			}
		}
		top->clk = 1;
		top->eval();
		cycles++;
	};

	top->filter_on = filter;
	top->fx_on = fx;
	top->rst = 1;
	tick();
	tick();
	top->rst = 0;
	tick();

	std::vector<int16_t> pcm(size_t(frames) * 2);
	size_t li = 0;
	uint64_t max_cyc = 0, sum_cyc = 0, max_busy = 0;
	for (long long n = 0; n < frames; n++)
	{
		while (!top->wr_ready)
			tick();
		const uint64_t c0 = cycles;
		while (li < log.size() && log[li].sample <= n)
		{
			const rec &r = log[li++];
			top->wr_chip = r.chip == 'G' ? 0 : r.chip == 'H' ? 1 : 2;
			top->wr_addr = r.chip == 'X' ? (((r.a & 31) << 6) | (r.b & 63)) : r.a;
			top->wr_data = r.data;
			top->wr_mask = r.mask;
			top->wr_valid = 1;
			top->eval();
			while (!top->wr_ready)
				tick();
			tick();
			top->wr_valid = 0;
			top->eval();
			while (!top->wr_ready)
				tick();
		}
		const uint64_t c1 = cycles;
		top->start = 1;
		tick();
		top->start = 0;
		while (!top->done)
			tick();
		const uint64_t c = cycles - c1;
		max_cyc = std::max(max_cyc, c);
		max_busy = std::max(max_busy, cycles - c0);
		sum_cyc += c;
		pcm[2 * n] = int16_t(top->out_l);
		pcm[2 * n + 1] = int16_t(top->out_r);
	}
	std::ofstream f(pos[1], std::ios::binary);
	f.write(reinterpret_cast<const char *>(pcm.data()), std::streamsize(pcm.size() * 2));
	std::printf("frames %lld, writes %zu, clocks per sample mean %.1f max %" PRIu64 ", with register writes max %" PRIu64
			" (DDR3 busy 1/%d, latency %d-%d), DDR3 reads %.1f writes %.1f per sample, divider clocks %u, "
			"chained voice-samples %u\n",
			frames, log.size(), frames ? double(sum_cyc) / frames : 0.0, max_cyc, max_busy, busy_mod, lat_min, lat_max,
			frames ? double(reads) / frames : 0.0, frames ? double(writes) / frames : 0.0,
			unsigned(top->stat_div_cycles), unsigned(top->stat_chained));
	top->final();
	return 0;
}
