// SPDX-License-Identifier: GPL-3.0-or-later
// Unit bench for hdl/p2k_loader.sv: each case downloads one random file (a system pack or a SIMM)
// twice, as bytes on ioctl_wr (hps_io without a load address) and direct (the file staged in DDR3
// at STAGE_BASE, then the copier), and checks that both leave the same DDR3 words, font bytes,
// slot sizes and pack_ok as a reference, with loading and pack_loading high once (no gap the
// flash save would take for a new download), built from the file layout (p2k_loader.sv's header).
// DDR3 starts as a hash of the address, so byte enables are checked too. The memory port is ready
// on a random 1 clock in 2 and returns reads in order after 1-30 clocks; ext_busy is high for a
// random time after the download. Exits 1 on the first difference.
//   loader_tb [--seed N] [--full]   (--full adds a 34 MB SIMM)
#include "Vp2k_loader.h"
#include "verilated.h"

#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <memory>
#include <random>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace {

constexpr uint64_t SMP_BASE = 0x6000000, BABY_BASE = 0x7100000, CPU_BASE = 0x7200000, STAGE_BASE = 0x7800000;

uint64_t hash(uint64_t x)
{
	x += 0x9e3779b97f4a7c15ull;
	x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ull;
	x = (x ^ (x >> 27)) * 0x94d049bb133111ebull;
	return x ^ (x >> 31);
}

struct ddr
{
	std::unordered_map<uint64_t, uint64_t> w;
	uint64_t get(uint64_t a) const { auto i = w.find(a); return i == w.end() ? hash(a) : i->second; }
	void put(uint64_t a, uint64_t d, uint8_t be)
	{
		uint64_t v = get(a);
		for (int b = 0; b < 8; b++)
			if (be >> b & 1) v = (v & ~(0xffull << 8 * b)) | (d & 0xffull << 8 * b);
		w[a] = v;
	}
};

struct result
{
	ddr mem;
	std::set<uint64_t> written;
	std::vector<uint8_t> font = std::vector<uint8_t>(4096, 0xee);
	uint32_t big[4], baby[4];
	bool pack_ok = false;
	int simm_pulses = 0;
	int load_rises = 0, pack_rises = 0;   // rising edges of loading, pack_loading (sampled before each edge)
	bool ld_prev = false, pl_prev = false;
};

// where file byte k goes in the loader's layout: word, bit shift; false if it is dropped
bool place(bool pack, int slot, uint64_t k, uint64_t &word, int &sh, bool &font)
{
	sh = 16 * int((k & 7) / 2) + (k & 1 ? 0 : 8);
	font = false;
	if (pack)
	{
		if (k < 0x200000) { word = CPU_BASE + k / 8; return true; }
		if (k < 0x201000) { font = true; word = k - 0x200000; return true; }
		return false;
	}
	if (k < 0x200000) { word = BABY_BASE + (uint64_t(slot) << 18) + k / 8; return true; }
	if (k < 0x2200000) { word = SMP_BASE + (uint64_t(slot) << 22) + (k - 0x200000) / 8; return true; }
	return false;
}

result reference(bool pack, int slot, const std::vector<uint8_t> &f)
{
	result r;
	for (uint64_t k = 0; k < f.size(); k++)
	{
		uint64_t w; int sh; bool font;
		if (!place(pack, slot, k, w, sh, font)) continue;
		if (font) { r.font[w] = f[k]; continue; }
		r.mem.put(w, uint64_t(f[k]) << sh, uint8_t(1u << (sh / 8)));
		r.written.insert(w);
	}
	const uint64_t n = f.size();
	for (int s = 0; s < 4; s++) r.big[s] = r.baby[s] = 0;
	if (!pack)
	{
		r.baby[slot] = n >= 0x200000 ? 0x100000 : uint32_t(n / 2);
		r.big[slot] = n >= 0x2200000 ? 0x1000000 : n > 0x200000 ? uint32_t((n - 0x200000) / 2) : 0;
	}
	r.pack_ok = pack && n >= 0x201000;
	r.simm_pulses = pack ? 0 : 1;
	r.load_rises = 1;
	r.pack_rises = pack ? 1 : 0;
	return r;
}

result run(bool pack, int slot, const std::vector<uint8_t> &f, bool direct, std::mt19937 &rng)
{
	result r;
	auto top = std::make_unique<Vp2k_loader>();
	std::deque<std::pair<uint64_t, uint64_t>> rsp;   // (due clock, data)
	uint64_t clk = 0;
	if (direct)
		for (uint64_t k = 0; k < f.size(); k++)
			r.mem.put(STAGE_BASE + k / 8, uint64_t(f[k]) << 8 * (k & 7), uint8_t(1u << (k & 7)));
	auto tick = [&]() {
		top->clk = 0;
		top->req_ready = rng() & 1;
		top->rsp_valid = 0;
		if (!rsp.empty() && rsp.front().first <= clk) { top->rsp_valid = 1; top->rsp_data = rsp.front().second; rsp.pop_front(); }
		top->eval();
		if (top->req_valid && top->req_ready)
		{
			const unsigned len = top->req_len ? top->req_len : 1;
			uint64_t due = std::max<uint64_t>(rsp.empty() ? 0 : rsp.back().first, clk + 1 + rng() % 30);
			for (unsigned i = 0; i < len; i++)
			{
				const uint64_t a = top->req_addr + i;
				if (top->req_we)
				{
					if (a >= STAGE_BASE && a < STAGE_BASE + 0x440000)
					{ std::printf("write into the staging region %" PRIx64 "\n", a); std::exit(1); }
					r.mem.put(a, top->req_wdata, top->req_be);
					r.written.insert(a);
				}
				else
					rsp.emplace_back(due + i, r.mem.get(a));
			}
		}
		if (top->font_we) r.font[top->font_addr] = top->font_data;
		// as a register on this clock sees them: the values before the edge
		r.load_rises += top->loading && !r.ld_prev;
		r.pack_rises += top->pack_loading && !r.pl_prev;
		r.ld_prev = top->loading;
		r.pl_prev = top->pack_loading;
		top->clk = 1;
		top->eval();
		if (top->simm_loaded) { r.simm_pulses++; if (getenv("LDBG")) std::printf("simm_loaded at %" PRIu64 " loading %d\n", clk, top->loading); }
		clk++;
	};
	top->rst = 1;
	for (int i = 0; i < 4; i++) tick();
	top->rst = 0;
	top->ioctl_index = pack ? 1 : 2 + slot;
	top->ext_busy = 0;
	tick();
	if (direct)
	{
		top->ioctl_addr = uint32_t(f.size());
		top->ioctl_download = 1;
		for (int i = 0, n = 10 + rng() % 100; i < n; i++) tick();
	}
	else
	{
		top->ioctl_addr = 0;
		top->ioctl_download = 1;
		tick();
		for (uint64_t k = 0; k < f.size(); k++)
		{
			do tick(); while (top->ioctl_wait);
			for (int i = 0, n = rng() % 3; i < n; i++) tick();
			top->ioctl_addr = uint32_t(k);
			top->ioctl_dout = f[k];
			top->ioctl_wr = 1;
			tick();
			top->ioctl_wr = 0;
		}
		do tick(); while (top->ioctl_wait);
	}
	top->ioctl_download = 0;
	top->ioctl_addr = uint32_t(f.size()) + 1;
	top->ext_busy = rng() & 1;
	const int busy_for = rng() % 200;
	tick();
	for (int i = 0; top->loading || top->req_valid || !rsp.empty(); i++)
	{
		if (i == busy_for) top->ext_busy = 0;
		if (i > 100000000) { std::printf("the copy does not end\n"); std::exit(1); }
		tick();
	}
	if (top->hold || top->pack_loading) { std::printf("hold/pack_loading high after loading\n"); std::exit(1); }
	for (int i = 0; i < 20; i++) tick();
	for (int s = 0; s < 4; s++) { r.big[s] = top->big_words[s]; r.baby[s] = top->baby_words[s]; }
	r.pack_ok = top->pack_ok;
	std::printf("  %s: %" PRIu64 " clocks\n", direct ? "direct" : "bytes ", clk);
	return r;
}

bool same(const char *what, const result &a, const result &ref)
{
	bool ok = true;
	if (a.written != ref.written)
	{
		std::printf("%s: wrote %zu words, the reference %zu\n", what, a.written.size(), ref.written.size());
		ok = false;
	}
	for (uint64_t w : ref.written)
		if (a.mem.get(w) != ref.mem.get(w))
		{
			std::printf("%s: word %" PRIx64 " = %016" PRIx64 ", the reference %016" PRIx64 "\n", what, w, a.mem.get(w), ref.mem.get(w));
			return false;
		}
	if (a.font != ref.font) { std::printf("%s: font differs\n", what); ok = false; }
	for (int s = 0; s < 4; s++)
		if (a.big[s] != ref.big[s] || a.baby[s] != ref.baby[s])
		{
			std::printf("%s: slot %d big %x baby %x, the reference %x %x\n", what, s, a.big[s], a.baby[s], ref.big[s], ref.baby[s]);
			ok = false;
		}
	if (a.pack_ok != ref.pack_ok) { std::printf("%s: pack_ok %d\n", what, a.pack_ok); ok = false; }
	if (a.load_rises != ref.load_rises || a.pack_rises != ref.pack_rises)
	{
		std::printf("%s: loading rose %d times, pack_loading %d (one load: 1, %d)\n", what, a.load_rises, a.pack_rises, ref.pack_rises);
		ok = false;
	}
	if (a.simm_pulses != ref.simm_pulses) { std::printf("%s: %d simm_loaded pulses\n", what, a.simm_pulses); ok = false; }
	return ok;
}

}   // namespace

int main(int argc, char **argv)
{
	Verilated::commandArgs(argc, argv);
	unsigned seed = 1;
	bool full = false;
	for (int i = 1; i < argc; i++)
	{
		if (!std::strcmp(argv[i], "--seed") && i + 1 < argc) seed = unsigned(std::atoi(argv[++i]));
		else if (!std::strcmp(argv[i], "--full")) full = true;
	}
	std::mt19937 rng(seed);
	struct tcase { bool pack; int slot; size_t size; };
	std::vector<tcase> cases = {
		{ true, 0, 0x201000 }, { true, 0, 0x100003 },
		{ false, 0, 0x12345 }, { false, 2, 0x230001 }, { false, 3, 0x200000 }, { false, 1, 0x20000e },
	};
	if (full) cases.push_back({ false, 1, 0x2200000 });
	for (const tcase &c : cases)
	{
		std::printf("%s slot %d, %zu bytes\n", c.pack ? "pack" : "SIMM", c.slot, c.size);
		std::vector<uint8_t> f(c.size);
		for (auto &b : f) b = uint8_t(rng());
		const result ref = reference(c.pack, c.slot, f);
		if (!same("direct", run(c.pack, c.slot, f, true, rng), ref)) return 1;
		if (!full || c.size < 0x1000000)
			if (!same("bytes", run(c.pack, c.slot, f, false, rng), ref)) return 1;
	}
	std::printf("identical\n");
	return 0;
}
