// SPDX-License-Identifier: GPL-3.0-or-later
// Unit bench for hdl/p2k_mem.sv (the CPU's cache over DDR3 and the 28F160 flash model): random
// CPU accesses (DRAM reads and byte/word writes, flash reads and the command sequences of
// p2k/sim/p2k_cpu_tb.cpp) against a flat model of the same memory, with a memory port that is
// ready on a random 1 clock in 2 and returns reads in order after 1-30 clocks. Exits 1 on the
// first difference.
//   mem_tb [--accesses N] [--seed N] [--tiny] [--gap0]
//     --tiny  most DRAM accesses in 8 words (two 64-bit words): a write hit followed at once by a
//             read of the same word (the cache line updated a clock after the write's ack)
//     --gap0  no idle clocks between accesses (the CPU presents its next access after ack)
//     --inject-error  flip bit 0 of the HDL's data for the read halfway through the run (the
//             comparator self-check: the run must then report DIFFERENT; tests/test_p2k_mem.py)
#include "Vp2k_mem.h"
#include "verilated.h"

#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <memory>
#include <random>
#include <string>
#include <vector>

namespace {

constexpr uint64_t BASE = 0x7200000;

// the reference: P space as 16-bit words, flash commands as the CPU bench's board model
struct model
{
	std::vector<uint16_t> w = std::vector<uint16_t>(0x500000, 0);   // 10 MB of P
	enum { F_NORMAL, F_READID, F_STATUS, F_WRITE1, F_CLEAR1, F_MASTER, F_AMD1, F_AMD2, F_AMD3, F_BYTEPROG } fmode = F_NORMAL;
	bool fmaster = false;
	uint16_t read(uint32_t pw, bool flash)
	{
		if (flash)
			switch (fmode)
			{
			case F_STATUS: return 0x80;
			case F_AMD3: return (pw & 0xff) == 0 ? 0xb0 : (pw & 0xff) == 1 ? 0xd0 : 0;
			case F_READID: switch (pw & 0xff) { case 0: return 0xb0; case 1: return 0xd0; case 3: return fmaster; default: return 0; }
			default: break;
			}
		return w[pw];
	}
	void write(uint32_t pw, bool flash, uint16_t d, bool u, bool l)
	{
		if (!flash)
		{
			if (u) w[pw] = (w[pw] & 0x00ff) | (d & 0xff00);
			if (l) w[pw] = (w[pw] & 0xff00) | (d & 0x00ff);
			return;
		}
		const uint8_t c = d & 0xff;
		switch (fmode)
		{
		case F_NORMAL: case F_STATUS: case F_READID: case F_AMD3:
			switch (c)
			{
			case 0xf0: case 0xff: fmode = F_NORMAL; break;
			case 0x90: fmode = F_READID; break;
			case 0x40: case 0x10: fmode = F_WRITE1; break;
			case 0x50: fmode = F_STATUS; break;
			case 0x20: fmode = F_CLEAR1; break;
			case 0x60: fmode = F_MASTER; break;
			case 0x70: fmode = F_STATUS; break;
			case 0xaa: if ((pw & 0xfff) == 0x555 || (pw & 0xfff) == 0xaaa) fmode = F_AMD1; break;
			default: break;
			}
			break;
		case F_AMD1:
			fmode = c == 0x55 && ((pw & 0xffff) == 0x2aa || (pw & 0xffff) == 0x2aaa || (pw & 0xfff) == 0x555) ? F_AMD2 : F_NORMAL;
			break;
		case F_AMD2:
		{
			const bool cmd = (pw & 0xffff) == 0x555 || (pw & 0xffff) == 0x5555 || (pw & 0xfff) == 0xaaa;
			fmode = F_NORMAL;
			if (cmd && c == 0x90) fmode = F_AMD3;
			else if (cmd && c == 0xa0) fmode = F_BYTEPROG;
			break;
		}
		case F_BYTEPROG: w[pw] &= d; fmode = F_NORMAL; break;
		case F_WRITE1: w[pw] &= d; fmode = F_STATUS; break;
		case F_CLEAR1:
			if (c == 0xd0)
			{
				const uint32_t base = (pw * 2) & ~0xffffu;
				for (uint32_t i = 0; i < 0x8000; i++) w[(base >> 1) + i] = 0xffff;
				fmode = F_STATUS;
			}
			break;
		case F_MASTER:
			if (c == 0xf1) fmaster = true;
			else if (c == 0xd0) fmaster = false;
			fmode = F_STATUS;
			break;
		}
	}
};

} // anonymous namespace

int main(int argc, char **argv)
{
	long n_acc = 200000;
	unsigned seed = 1;
	bool tiny = false, gap0 = false, inject_error = false;
	for (int i = 1; i < argc; i++)
	{
		const std::string a = argv[i];
		if (a == "--accesses" && i + 1 < argc) n_acc = std::atol(argv[++i]);
		else if (a == "--seed" && i + 1 < argc) seed = unsigned(std::atoi(argv[++i]));
		else if (a == "--tiny") tiny = true;
		else if (a == "--gap0") gap0 = true;
		else if (a == "--inject-error") inject_error = true;
	}
	std::mt19937 rng(seed);
	model ref;
	// DDR3 contents = the model's initial memory (random DRAM, random flash)
	std::vector<uint64_t> ddr(0x140000, 0);
	for (size_t i = 0; i < ref.w.size(); i++)
	{
		ref.w[i] = uint16_t(rng());
		ddr[i >> 2] |= uint64_t(ref.w[i]) << (16 * (i & 3));
	}

	auto ctx = std::make_unique<VerilatedContext>();
	auto top = std::make_unique<Vp2k_mem>(ctx.get());
	std::deque<std::pair<uint64_t, uint64_t>> rdq;   // due clock, DDR3 word
	uint64_t cyc = 0, last_due = 0;
	bool bad = false;
	auto clock = [&]() {
		top->req_ready = rng() & 1;
		top->rsp_valid = 0;
		if (!rdq.empty() && rdq.front().first <= cyc)
		{
			top->rsp_valid = 1;
			top->rsp_data = ddr[rdq.front().second];
			rdq.pop_front();
		}
		top->clk = 0;
		top->eval();
		if (top->req_valid && top->req_ready)
		{
			const uint64_t a = top->req_addr - BASE;
			if (a >= ddr.size()) { std::fprintf(stderr, "request outside: %" PRIx64 "\n", a); bad = true; }
			else if (top->req_we)
			{
				for (int b = 0; b < 8; b++)
					if (top->req_be & (1 << b))
						ddr[a] = (ddr[a] & ~(0xffull << (8 * b))) | (((top->req_wdata >> (8 * b)) & 0xff) << (8 * b));
			}
			else
				for (unsigned k = 0; k < top->req_len; k++)
				{
					last_due = std::max<uint64_t>(cyc + 1 + rng() % 30, last_due + 1);
					rdq.push_back({last_due, a + k});
				}
		}
		top->clk = 1;
		top->eval();
		cyc++;
	};
	top->rst = 1;
	top->flush = 0;
	top->go = 0;
	top->flash_lock = 0;
	for (int i = 0; i < 4; i++) clock();
	top->rst = 0;
	for (int i = 0; i < 1100; i++) clock();       // the reset flush

	// accesses: mostly a few hot regions (hits), flash sequences now and then
	long reads = 0, writes = 0, fcmds = 0;
	auto access = [&](uint32_t pw, bool we, uint16_t d, bool u, bool l) {
		const bool flash = pw < 0x100000;
		top->go = 1; top->we = we; top->flash = flash; top->pa = pw; top->wdata = d; top->uds = u; top->lds = l;
		for (int guard = 0;; guard++)
		{
			top->clk = 0;
			top->eval();
			if (top->ack)
			{
				const uint16_t got = top->rdata ^ uint16_t(inject_error && !we && reads == n_acc / 2);
				uint16_t exp = 0;
				if (!we) exp = ref.read(pw, flash);
				else ref.write(pw, flash, d, u, l);
				if (!we && got != exp)
				{
					std::fprintf(stderr, "access %ld: read P word %06x (%s) = %04x, expected %04x\n",
					             reads + writes, pw, flash ? "flash" : "DRAM", got, exp);
					bad = true;
				}
				clock();
				break;
			}
			clock();
			if (guard > 2000000) { std::fprintf(stderr, "no ack for P word %06x\n", pw); bad = true; break; }
		}
		top->go = 0;
		if (!gap0)
			for (unsigned i = rng() % 3; i; i--) clock();
		(we ? writes : reads)++;
	};
	for (long n = 0; n < n_acc && !bad; n++)
	{
		const unsigned r = rng() % 1000;
		if (r < 6)
		{
			// a flash command sequence
			const uint32_t fw = rng() % 0x100000;
			const unsigned k = rng() % 6;
			fcmds++;
			if (k == 0) { access(fw, true, 0x4040, true, true); access(fw, true, uint16_t(rng()), true, true); access(fw, true, 0x7070, true, true); access(fw, false, 0, true, true); access(fw, true, 0xffff, true, true); }
			else if (k == 1 && rng() % 8 == 0) { access(fw, true, 0x2020, true, true); access(fw, true, 0xd0d0, true, true); access(fw, false, 0, true, true); access(fw, true, 0xffff, true, true); }
			else if (k == 2) { access(fw, true, 0x9090, true, true); access(rng() % 4, false, 0, true, true); access(fw, true, 0xffff, true, true); }
			else if (k == 3) { access(0x555, true, 0xaaaa, true, true); access(0x2aa, true, 0x5555, true, true); access(0x555, true, 0xa0a0, true, true); access(fw, true, uint16_t(rng()), true, true); }
			else if (k == 4) { access(fw, true, 0x5050, true, true); access(fw, false, 0, true, true); access(fw, true, 0xffff, true, true); }
			else { access(fw, true, 0x6060, true, true); access(fw, true, (rng() & 1) ? 0xf1f1 : 0xd0d0, true, true); access(fw, true, 0xffff, true, true); }
			continue;
		}
		uint32_t pw;
		const unsigned region = rng() % 10;
		if (tiny && region < 6) pw = 0x100000 + rng() % 8;                              // DRAM, 2 words
		else if (region < 6) pw = 0x100000 + (rng() % 4) * 0x40000 + rng() % 0x2000;  // DRAM, hot
		else if (region < 8) pw = 0x100000 + rng() % 0x400000;                         // DRAM, anywhere
		else pw = rng() % 0x100000;                                                     // flash
		const bool flash = pw < 0x100000;
		const bool we = !flash && rng() % 3 == 0;
		const unsigned lanes = 1 + rng() % 3;
		access(pw, we, uint16_t(rng()), lanes & 2, lanes & 1);
	}
	std::printf("%ld reads, %ld writes, %ld flash sequences, %" PRIu64 " clocks: %s\n", reads, writes, fcmds, cyc,
	            bad ? "DIFFERENT" : "identical");
	return bad ? 1 : 0;
}
