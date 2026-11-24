// SPDX-License-Identifier: GPL-3.0-or-later
// Verilator bench for p2k/rtl/snd/p2k_snd.sv: replays a sample-indexed sound-register log
// (P2K_SNDLOG, as p2k/model/p2k_render does) and writes 16-bit stereo raw audio at 44.1 kHz.
// The output must equal `p2k_render` run with the same log, SIMM and options, bit for bit.
//
//   p2k_snd_tb [--simm N=FILE] [--frames N] [--nofilter] [--nofx] [--lat N] SNDLOG OUT.raw
//     --lat N   sample-memory latency in clocks (default 3)
#include "Vp2k_snd.h"
#include "verilated.h"

#include <algorithm>
#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <memory>
#include <string>
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
	int lat = 3;
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
		else if (a == "--lat" && i + 1 < argc)
			lat = std::atoi(argv[++i]);
		else if (a == "--nofilter")
			filter = false;
		else if (a == "--nofx")
			fx = false;
		else
			pos.push_back(argv[i]);
	}
	if (pos.size() != 2)
	{
		std::fprintf(stderr, "usage: p2k_snd_tb [--simm N=FILE] [--frames N] [--nofilter] [--nofx] [--lat N] SNDLOG OUT.raw\n");
		return 2;
	}
	// sound memory: 4 slots of 16 M words, zero-filled (as p2k_render)
	std::vector<uint16_t> mem(size_t(4) << 24, 0);
	for (int s = 0; s < 4; s++)
	{
		if (simm[s].empty())
			continue;
		std::ifstream f(simm[s], std::ios::binary);
		std::vector<unsigned char> b((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
		const size_t words = std::min<size_t>(b.size() / 2, 1 << 24);
		for (size_t i = 0; i < words; i++)
			mem[(size_t(s) << 24) + i] = uint16_t((b[2 * i] << 8) | b[2 * i + 1]);
	}
	const auto log = read_log(pos[0]);
	if (frames < 0)
		frames = (log.empty() ? 0 : log.back().sample) + 3 * 44100;

	auto ctx = std::make_unique<VerilatedContext>();
	auto top = std::make_unique<Vp2k_snd>(ctx.get());
	int cnt = 0;
	uint64_t cycles = 0, reads = 0;
	auto tick = [&]() {
		// sample memory: ack lat clocks after the request (or the previous ack) with the addressed word
		if (top->mem_req)
		{
			if (cnt >= lat)
			{
				top->mem_ack = 1;
				const uint32_t a = top->mem_addr;
				top->mem_data = a < mem.size() ? mem[a] : 0;
				cnt = 0;
				reads++;
			}
			else
			{
				top->mem_ack = 0;
				cnt++;
			}
		}
		else
		{
			top->mem_ack = 0;
			cnt = 0;
		}
		top->clk = 0;
		top->eval();
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
	uint64_t max_cyc = 0, sum_cyc = 0;
	for (long long n = 0; n < frames; n++)
	{
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
		}
		while (!top->wr_ready)
			tick();
		const uint64_t c0 = cycles;
		top->start = 1;
		tick();
		top->start = 0;
		while (!top->done)
			tick();
		const uint64_t c = cycles - c0;
		max_cyc = std::max(max_cyc, c);
		sum_cyc += c;
		pcm[2 * n] = int16_t(top->out_l);
		pcm[2 * n + 1] = int16_t(top->out_r);
	}
	std::ofstream f(pos[1], std::ios::binary);
	f.write(reinterpret_cast<const char *>(pcm.data()), std::streamsize(pcm.size() * 2));
	std::printf("frames %lld, writes %zu, clocks per sample mean %.1f max %" PRIu64 " (latency %d), "
			"memory reads %.1f per sample, divider clocks %u, chained voice-samples %u\n",
			frames, log.size(), frames ? double(sum_cyc) / frames : 0.0, max_cyc, lat,
			frames ? double(reads) / frames : 0.0, unsigned(top->stat_div_cycles), unsigned(top->stat_chained));
	top->final();
	return 0;
}
