// SPDX-License-Identifier: GPL-3.0-or-later
// Unit bench for hdl/p2k_sndbus.sv against MAME's emup2k.cpp sound-chip read/write handlers
// (g20_r/g20_w, h_r/h_w, g05_r/g05_w, snd_read): random CPU writes and reads of the G20, H and
// G05 windows, the sound-memory ports A/B (Big and Baby, loaded and unloaded slots), the encoder
// and the live values. The engine is a stub: it applies every write it takes to its own copy (so
// the bench also checks that each write reaches it, in order) and answers live reads with values
// derived from the voice or block number. Slot 2 (index 1) is a user flash SIMM: MAME's
// p2k::intel_flash (cores/proteus/p2k/model/p2k_flash.h) on its own copy of the slot, driven by
// command sequences through port B (program, erase, buffered program, status, ID, read array, other
// commands); at the end the bench's DDR3 must equal that copy and the dirty regions the regions
// MAME wrote. Exits 1 on the first difference.
//   sndbus_tb [--accesses N] [--seed N]
#include "Vp2k_sndbus.h"
#include "verilated.h"
#ifdef SB_DEBUG
#include "Vp2k_sndbus___024root.h"
#endif

#include "../../proteus/p2k/model/p2k_flash.h"

#include <array>
#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <map>
#include <set>
#include <memory>
#include <random>
#include <string>
#include <vector>

namespace {
constexpr uint64_t SMP_BASE = 0x6000000, BABY_BASE = 0x7100000;
uint32_t live_addr(unsigned v) { return (v * 0x12345u + 0x777u) & 0x3ffffff; }
bool live_sel(unsigned v) { return (v * 7) & 4; }
uint16_t live_ip(unsigned b, unsigned n) { return uint16_t(b * 0x101 + n * 0x3333); }
uint64_t ddr_init(uint64_t a) { return (a * 0x9e3779b97f4a7c15ull) ^ (a >> 7); }
constexpr unsigned FL = 1;                           // the flash slot
}

int main(int argc, char **argv)
{
	long n_acc = 100000;
	unsigned seed = 1;
	for (int i = 1; i < argc; i++)
	{
		const std::string a = argv[i];
		if (a == "--accesses" && i + 1 < argc) n_acc = std::atol(argv[++i]);
		else if (a == "--seed" && i + 1 < argc) seed = unsigned(std::atoi(argv[++i]));
	}
	std::mt19937 rng(seed);

	// sound memory: slots 0 and 2 loaded (slot 2 short), Baby on slot 0 only
	const uint32_t big_words[4] = { 1u << 24, 0, 0x1000, 0 }, baby_words[4] = { 1u << 20, 0, 0, 0 };
	std::map<uint64_t, uint64_t> ddr;
	auto ddr_word = [&](uint64_t a) -> uint64_t {
		auto it = ddr.find(a);
		return it != ddr.end() ? it->second : ddr_init(a);
	};
	// MAME's flash slot: its own copy of the slot's Big and Baby words
	std::vector<uint16_t> fl_big(1u << 24), fl_baby(1u << 20);
	for (uint32_t i = 0; i < (1u << 24); i++) fl_big[i] = uint16_t(ddr_init(SMP_BASE + (((FL << 24) | i) >> 2)) >> (16 * (i & 3)));
	for (uint32_t i = 0; i < (1u << 20); i++) fl_baby[i] = uint16_t(ddr_init(BABY_BASE + ((FL << 18) | (i >> 2))) >> (16 * (i & 3)));
	p2k::intel_flash big_flash[4], baby_flash;
	auto fl_config = [&]() {
		for (int c = 0; c < 4; c++) big_flash[c].configure(&fl_big[c << 22], 1 << 22, 0x0089, 0x0015, 1 << 16, true);
		baby_flash.configure(fl_baby.data(), 1 << 20, 0x00b0, 0x00d0, 1 << 15, false);
	};
	fl_config();
	auto snd_read = [&](uint32_t a, bool baby) -> uint16_t {         // MAME snd_read
		const unsigned slot = (a >> 24) & 3;
		const uint32_t off = a & 0xffffff;
		if (slot == FL)
			return baby ? (off < (1u << 20) ? baby_flash.read(off) : 0xffff) : big_flash[(off >> 22) & 3].read(off & 0x3fffff);
		if (baby)
			return off < (1u << 20) && off < baby_words[slot] ? uint16_t(ddr_word(BABY_BASE + ((slot << 18) | (off >> 2))) >> (16 * (off & 3))) : 0xffff;
		return off < big_words[slot] ? uint16_t(ddr_word(SMP_BASE + (a >> 2)) >> (16 * (a & 3))) : 0;
	};

	auto snd_write = [&](uint32_t a, bool baby, uint16_t d) {          // MAME snd_write
		const unsigned slot = (a >> 24) & 3;
		const uint32_t off = a & 0xffffff;
		if (slot != FL) return;
		if (baby)
		{
			if (off < (1u << 20)) baby_flash.write(off, d);
		}
		else
			big_flash[(off >> 22) & 3].write(off & 0x3fffff, d);
	};

	// MAME's state
	std::vector<uint16_t> g20(4096, 0), h(0x4000, 0), g05(2048, 0);
	uint16_t g05_index = 0;
	uint32_t snd_addr[2] = {0, 0};
	uint16_t snd_latch[2] = {0, 0};
	bool baby_sel = false;
	int enc = 0;
	// the engine stub's copy, and the writes it should see
	std::deque<std::array<uint32_t, 4>> expect;

	auto ctx = std::make_unique<VerilatedContext>();
	auto top = std::make_unique<Vp2k_sndbus>(ctx.get());
	for (int s = 0; s < 4; s++) { top->big_words[s] = big_words[s]; top->baby_words[s] = baby_words[s]; }
	top->flash_slot = 1u << FL;
	top->flash_reset = 0;
	std::set<unsigned> hdl_regions;                   // fl_dirty regions of the flash slot
	long mem_writes = 0;
	std::deque<std::pair<uint64_t, uint64_t>> rdq;    // (due, data): data as of the read's turn
	uint64_t cyc = 0, last_due = 0;
	int rd_wait = -1;
	unsigned rd_chip = 0, rd_addr = 0;
	bool bad = false;
	long engine_writes = 0;
	auto clock = [&]() {
		top->req_ready = rng() & 1;
		top->rsp_valid = 0;
		if (!rdq.empty() && rdq.front().first <= cyc)
		{
			top->rsp_valid = 1;
			top->rsp_data = rdq.front().second;
			rdq.pop_front();
		}
		top->tick = rng() % 300 == 0;
		top->snd_idle = rd_wait < 0 && (rng() % 4 != 0);
		top->snd_rd_done = 0;
		if (rd_wait == 0)
		{
			top->snd_rd_done = 1;
			top->snd_rd_vaddr = live_addr(rd_addr & 127);
			top->snd_rd_vsel = live_sel(rd_addr & 127);
			for (int n = 0; n < 3; n++) top->snd_rd_ipx[n] = live_ip(rd_addr, n);
		}
		if (rd_wait >= 0) rd_wait--;
		top->enc_count = uint8_t(enc);
		top->clk = 0;
		top->eval();
		if (top->req_valid && top->req_ready)
		{
			const unsigned len = top->req_len ? top->req_len : 1;
			if (top->req_we)
			{
				uint64_t m = 0;
				for (int b = 0; b < 8; b++)
					if (top->req_be >> b & 1) m |= 0xffull << (8 * b);
				for (unsigned i = 0; i < len; i++)
					ddr[top->req_addr + i] = (ddr_word(top->req_addr + i) & ~m) | (top->req_wdata & m);
				mem_writes++;
			}
			else
				for (unsigned i = 0; i < len; i++)
				{
					last_due = std::max<uint64_t>(cyc + 1 + rng() % 20, last_due + 1);
					rdq.push_back({last_due, ddr_word(top->req_addr + i)});
				}
		}
		if (top->fl_dirty)
		{
			if (top->fl_dirty_slot != FL) { std::fprintf(stderr, "dirty mark for slot %u\n", top->fl_dirty_slot); bad = true; }
			hdl_regions.insert(top->fl_dirty_region);
		}
		const bool fire_wr = top->snd_wr_valid && top->snd_idle && !top->snd_start;
		if (fire_wr)
		{
			if (expect.empty()) { std::fprintf(stderr, "engine got an unexpected write\n"); bad = true; }
			else
			{
				const auto e = expect.front();
				expect.pop_front();
				if (e[0] != top->snd_wr_chip || e[1] != top->snd_wr_addr || e[2] != top->snd_wr_data || e[3] != top->snd_wr_mask)
				{
					std::fprintf(stderr, "engine write %u %05x %04x %04x, expected %u %05x %04x %04x\n", top->snd_wr_chip,
					             top->snd_wr_addr, top->snd_wr_data, top->snd_wr_mask, e[0], e[1], e[2], e[3]);
					bad = true;
				}
				engine_writes++;
			}
		}
		if (top->snd_rd_valid && top->snd_idle && !top->snd_start && !top->snd_wr_valid)
		{
			if (!expect.empty()) { std::fprintf(stderr, "live read before the writes were applied\n"); bad = true; }
			rd_chip = top->snd_rd_chip;
			rd_addr = top->snd_rd_addr;
			rd_wait = 1 + rng() % 5;
		}
		if (top->enc_take) enc = 0;
		top->clk = 1;
		top->eval();
		cyc++;
	};
	top->rst = 1;
	for (int i = 0; i < 4; i++) clock();
	top->rst = 0;
	for (int i = 0; i < 4; i++) clock();

	long reads = 0, writes = 0;
	auto access = [&](unsigned chip, uint32_t woff, bool we, uint16_t d, bool u, bool l) -> uint16_t {
		top->go = 1; top->we = we; top->chip = chip; top->woff = woff; top->wdata = d; top->uds = u; top->lds = l;
		clock();
		top->go = 0;
		for (long guard = 0; !top->done; guard++)
		{
			clock();
			if (guard > 100000) { std::fprintf(stderr, "no done\n"); bad = true; return 0; }
		}
		const uint16_t r = top->rdata;
		clock();
		(we ? writes : reads)++;
		return r;
	};

	// G20 writes and port reads of the flash slot, as the OS makes them (voice 0)
	auto g20_w = [&](unsigned w, uint16_t d) {
		const uint32_t off = w;
		g20[off] = w == 0x1f ? (d & 0x03ff) : d;
		expect.push_back({0, off, d, 0xffff});
		if (w == 0x18 || w == 0x19) snd_addr[0] = ((uint32_t(g20[0x18]) << 16) | g20[0x19]) & 0x3ffffff;
		else if (w == 0x1a || w == 0x1b) snd_addr[1] = ((uint32_t(g20[0x1a]) << 16) | g20[0x1b]) & 0x3ffffff;
		else if (w == 0x0f)
		{
			snd_write(snd_addr[1], baby_sel, d);
			snd_addr[1] = (snd_addr[1] + 1) & 0x3ffffff;
		}
		access(0, off, true, d, true, true);
	};
	auto set_b = [&](uint32_t a) { g20_w(0x1a, uint16_t(a >> 16)); g20_w(0x1b, uint16_t(a)); };
	auto port_rd = [&](int p) {
		const uint16_t e = snd_latch[p];
		snd_latch[p] = snd_read(snd_addr[p], baby_sel);
		snd_addr[p] = (snd_addr[p] + 1) & 0x3ffffff;
		const uint16_t got = access(0, 0x0e + p, false, 0, true, true);
		if (got != e)
		{
			std::fprintf(stderr, "flash port %d read = %04x, expected %04x\n", p, got, e);
			bad = true;
		}
	};
	long fl_ops = 0;
	auto flash_op = [&]() {
		fl_ops++;
		const bool bs = rng() % 3 == 0;
		baby_sel = bs;
		top->baby_sel = bs;
		uint32_t off;
		if (bs) off = ((rng() % 3) << 15) | (rng() % 64);
		else off = ((rng() % 4) << 22) | ((rng() % 3) << 16) | (rng() % 64);
		if (bs && rng() % 40 == 0) off = 0x100000 + rng() % 64;      // beyond Baby
		const uint32_t a = (FL << 24) | off;
		const unsigned kind = rng() % 9;
		const uint16_t dbl[] = { 0x4040, 0x1010, 0x2020, 0x7070, 0x9090, 0xffff, 0x5050, 0x0000, 0xaaaa, 0x5555, 0xf0f0 };
		switch (kind)
		{
		case 0: case 1:                               // program
			set_b(a); g20_w(0x0f, kind ? 0x1010 : 0x4040);
			set_b(a); g20_w(0x0f, uint16_t(rng()));
			break;
		case 2:                                       // erase (or a wrong confirm)
			if (rng() % 4) break;
			set_b(a); g20_w(0x0f, 0x2020);
			set_b(a); g20_w(0x0f, rng() % 5 ? 0xd0d0 : 0xffff);
			break;
		case 3: {                                     // buffered program
			const unsigned n = rng() % 8 == 0 ? 33 + rng() % 8 : 1 + rng() % 32;
			set_b(a); g20_w(0x0f, 0xe8e8);
			set_b(a); g20_w(0x0f, uint16_t(n - 1));
			set_b(a);
			for (unsigned i = 0; i < n; i++) g20_w(0x0f, uint16_t(rng()));
			set_b(a); g20_w(0x0f, rng() % 6 ? 0xd0d0 : 0x0000);
			break;
		}
		default: {                                    // a command, then reads through port A
			set_b(a); g20_w(0x0f, dbl[rng() % 11]);
			g20_w(0x18, uint16_t(a >> 16)); g20_w(0x19, uint16_t(a));
			const unsigned n = 1 + rng() % 5;
			for (unsigned i = 0; i < n; i++) port_rd(0);
			break;
		}
		}
	};

	for (long n = 0; n < n_acc && !bad; n++)
	{
		const unsigned k = rng() % 100;
		if (k < 2)
		{
			baby_sel = rng() & 1;
			top->baby_sel = baby_sel;
			continue;
		}
		if (k < 4)
		{
			const int st = int(rng() % 3) - 1;
			enc += st;
			continue;
		}
		if (k < 12)
		{
			flash_op();
			continue;
		}
		if (k == 12 && rng() % 20 == 0)
		{
			// the CPU held: every chip back to read array
			top->flash_reset = 1;
			clock();
			top->flash_reset = 0;
			fl_config();
			continue;
		}
		const unsigned chip = rng() % 3;
		const bool we = rng() % 2;
		const unsigned lanes = 1 + rng() % 3;
		const bool u = lanes & 2, lo = lanes & 1;
		const uint16_t mask = (u ? 0xff00 : 0) | (lo ? 0x00ff : 0);
		uint16_t d = uint16_t(rng());
		if (chip == 0)
		{
			const unsigned v = rng() % 128;
			// favour the port and live registers
			static const unsigned hot[] = { 0x01, 0x0c, 0x0d, 0x0e, 0x0f, 0x18, 0x19, 0x1a, 0x1b, 0x1f };
			const unsigned w = (rng() % 3) ? hot[rng() % 10] : rng() % 32;
			const uint32_t off = v * 32 + w;
			if (w == 0x18 || w == 0x1a) d &= 0x03ff;                    // keep port addresses in range
			if (w == 0x0f && (d & 0xff) == 0xe8) d ^= 1;                // buffered programs only in flash_op
			if (we)
			{
				uint16_t dd = w == 0x1f ? (d & 0x03ff) : d;
				g20[off] = (g20[off] & ~mask) | (dd & mask);
				expect.push_back({0, off, d, mask});
				if (getenv("SB_TRACE") && (w >= 0x18 && w <= 0x1b || w == 0x0f)) std::fprintf(stderr, "write v%u w%02x %04x mask %04x\n", v, w, d, mask);
				if (w == 0x18 || w == 0x19) snd_addr[0] = ((uint32_t(g20[v * 32 + 0x18]) << 16) | g20[v * 32 + 0x19]) & 0x3ffffff;
				else if (w == 0x1a || w == 0x1b) snd_addr[1] = ((uint32_t(g20[v * 32 + 0x1a]) << 16) | g20[v * 32 + 0x1b]) & 0x3ffffff;
				else if (w == 0x0f)
				{
					snd_write(snd_addr[1], baby_sel, d);
					snd_addr[1] = (snd_addr[1] + 1) & 0x3ffffff;
				}
				access(0, off, true, d, u, lo);
			}
			else
			{
				uint16_t e;
				if (w == 0x0e || w == 0x0f)
				{
					const int p = w == 0x0f;
					if (getenv("SB_TRACE")) std::fprintf(stderr, "port %d read: addr %07x baby %d latch %04x\n", p, snd_addr[p], baby_sel, snd_latch[p]);
#ifdef SB_DEBUG
					std::fprintf(stderr, "  hdl p_addr %07x %07x latch %04x %04x\n", top->rootp->p2k_sndbus__DOT__p_addr[0], top->rootp->p2k_sndbus__DOT__p_addr[1], top->rootp->p2k_sndbus__DOT__p_latch[0], top->rootp->p2k_sndbus__DOT__p_latch[1]);
#endif
					e = snd_latch[p];
					snd_latch[p] = snd_read(snd_addr[p], baby_sel);
					snd_addr[p] = (snd_addr[p] + 1) & 0x3ffffff;
				}
				else if (w == 0x0c) e = uint16_t(live_addr(v) >> 16);
				else if (w == 0x0d) e = uint16_t(live_addr(v));
				else if (w == 0x01) e = (g20[off] & ~0x1000) | (live_sel(v) ? 0x1000 : 0);
				else e = g20[off];
				const uint16_t got = access(0, off, false, 0, true, true);
				if (got != e) { std::fprintf(stderr, "G20 read voice %u word %02x = %04x, expected %04x\n", v, w, got, e); bad = true; }
			}
		}
		else if (chip == 1)
		{
			const unsigned b = rng() % 256;
			static const unsigned hot[] = { 0x1a, 0x1b, 0x1d, 0x1f, 0x3d };
			const unsigned w = (rng() % 3) ? hot[rng() % 5] : rng() % 64;
			uint32_t off = b * 64 + w;
			if (rng() % 50 == 0) off = 0x4000 + rng() % 0x7c000;          // above the voice blocks
			if (we)
			{
				if (off < 0x4000) h[off] = (h[off] & ~mask) | (d & mask);
				expect.push_back({1, off, d, mask});
				access(1, off, true, d, u, lo);
			}
			else
			{
				uint16_t e;
				if (off >= 0x4000) e = 0;
				else if (w == 0x1a || w == 0x1b || w == 0x1f) e = live_ip(b, w == 0x1a ? 0 : w == 0x1b ? 1 : 2);
				else if (w == 0x1d) e = h[b * 64 + 0x3d];
				else e = h[off];
				const uint16_t got = access(1, off, false, 0, true, true);
				if (got != e) { std::fprintf(stderr, "H read word %05x = %04x, expected %04x\n", off, got, e); bad = true; }
			}
		}
		else
		{
			static const unsigned hot[] = { 0x07, 0x0c };
			const unsigned w = (rng() % 3) ? hot[rng() % 2] : rng() % 64;
			if (w == 0x07 && we && (rng() % 2)) d = (d & 0xffe0) | ((rng() % 2) ? 0x1b : 0x1e);
			const unsigned idx = g05_index & 31;
			if (we)
			{
				if (w == 0x07) g05_index = (g05_index & ~mask) | (d & mask);
				else
				{
					g05[idx * 64 + w] = (g05[idx * 64 + w] & ~mask) | (d & mask);
					expect.push_back({2, idx * 64 + w, d, mask});
				}
				access(2, w, true, d, u, lo);
			}
			else
			{
				uint16_t e;
				if (w == 0x07) e = g05_index;
				else if (w == 0x0c && idx == 0x1e) e = 0x30;
				else if (w == 0x0c && idx == 0x1b) { e = uint8_t(enc); }
				else e = g05[idx * 64 + w];
				const uint16_t got = access(2, w, false, 0, true, true);
				if (got != e) { std::fprintf(stderr, "G05 read index %02x word %02x = %04x, expected %04x\n", idx, w, got, e); bad = true; }
			}
		}
	}
	for (int i = 0; i < 20000 && !expect.empty(); i++) clock();
	if (!expect.empty()) { std::fprintf(stderr, "%zu writes never reached the engine\n", expect.size()); bad = true; }
	// the flash slot in DDR3 equals MAME's copy; every region that changed was marked dirty
	long diff = 0, changed = 0;
	std::set<unsigned> regions;
	for (uint32_t i = 0; i < (1u << 24) + (1u << 20); i += 4)
	{
		const bool bb = i >= (1u << 24);
		const uint32_t o = bb ? i - (1u << 24) : i;
		const uint64_t da = bb ? BABY_BASE + ((FL << 18) | (o >> 2)) : SMP_BASE + (((FL << 24) | o) >> 2);
		const uint64_t got = ddr_word(da);
		uint64_t want = 0;
		for (int l = 0; l < 4; l++) want |= uint64_t(bb ? fl_baby[o + l] : fl_big[o + l]) << (16 * l);
		if (got != want && diff++ < 5) std::fprintf(stderr, "flash %s word %06x: DDR3 %016" PRIx64 ", MAME %016" PRIx64 "\n", bb ? "Baby" : "Big", o, got, want);
		if (want != ddr_init(da))
		{
			changed++;
			regions.insert(bb ? o >> 17 : 8 + (o >> 17));
		}
	}
	for (unsigned r : regions)
		if (!hdl_regions.count(r)) { std::fprintf(stderr, "region %u changed but not marked dirty\n", r); bad = true; }
	if (diff) bad = true;
	std::printf("flash: %ld operations, %ld DDR3 writes, %ld words changed, %zu regions changed, %zu marked\n", fl_ops,
	            mem_writes, changed, regions.size(), hdl_regions.size());
	std::printf("%ld reads, %ld writes, %ld engine writes, %" PRIu64 " clocks: %s\n", reads, writes, engine_writes, cyc,
	            bad ? "DIFFERENT" : "identical");
	return bad ? 1 : 0;
}
