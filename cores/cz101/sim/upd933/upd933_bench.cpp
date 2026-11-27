// SPDX-License-Identifier: MIT
// license:BSD-3-Clause
// uPD933 HDL against the C++ reference (MAME's model) on the same event lists.
//
//   upd933_bench [--real N] [--lead L] [--ext] [--base B] [--out DIR] [--inject-error] FILE.ev...
//     default ("fast"): now_tick jumps to each request's order tick (the CPU's logical time).
//     --real N: now_tick advances one tick every N clocks (a 10 MHz counter at N x 10 MHz),
//               clamped to the next request's order tick; requests are presented up to L ticks
//               early (default 30).
//     --out DIR: write DIR/<name>.s32 (HDL samples, int32 LE) for hashing.
//     --base B: add B (mod 2^40) to every tick the HDL sees, from reset on (now_tick, req_tick,
//               req_order, timer_fire_tick), and subtract it from the IRQ ticks it reports. With
//               B close to 2^40 the 40-bit tick counter wraps during the run (after about
//               30.5 h on the board); the results must not change.
//     --ext: ext_timer = 1; the bench plays the board: on timer_arm it computes the expiry as the
//            reference does (arming request's tick + timer_n * 250), forgets it on timer_cancel,
//            and pulses timer_fire before the first request whose order tick reaches it.
//     --inject-error: flip bit 0 of the HDL's sample 1000 (the comparator self-check: the run
//                     must then report DIFF; tests/ci.sh).
// Exit status 0 when every file matches in samples, IRQ changes (tick and level) and read data.
#include "../../ref/upd933_ref.h"
#include "Vupd933.h"
#include "verilated.h"

#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

using namespace upd933ref;

namespace {

bool inject_error = false;

struct HdlResult {
	std::vector<s32> samples;
	std::vector<IrqChange> irq;
	std::vector<ReadData> reads;
	u64 cycles = 0;
	bool late = false;
	bool hung = false;
};

int op_code(char op)
{
	switch (op)
	{
	case 'W': return 0; case 'R': return 1; case 'C': return 2; case 'I': return 3;
	case 'X': return 4; case 'S': return 5; default: return -1;
	}
}

HdlResult run_hdl(const std::vector<Event> &ev, unsigned real_n, u64 lead, bool ext, u64 base)
{
	constexpr u64 TICK_MASK = (u64(1) << 40) - 1;
	auto at = [&](u64 t) { return (t + base) & TICK_MASK; };
	HdlResult h;
	auto ctx = std::make_unique<VerilatedContext>();
	auto top = std::make_unique<Vupd933>(ctx.get());

	auto clock = [&]() {
		top->clk = 0; top->eval();
		top->clk = 1; top->eval();
		h.cycles++;
		if (top->sample_valid)
			h.samples.push_back((s32(top->sample_sum << 13) >> 13) ^ s32(inject_error && h.samples.size() == 1000));
		if (top->irq_evt) h.irq.push_back({(top->irq_evt_tick - base) & TICK_MASK, u8(top->irq), 0});
	};

	top->rst = 1; top->req_valid = 0; top->now_tick = at(0);
	top->ext_timer = ext; top->timer_fire = 0; top->timer_fire_tick = at(0);
	bool b_armed = false;       // --ext: the board's copy of the timer
	u64 b_deadline = 0;
	for (int i = 0; i < 16; i++) clock();
	top->rst = 0;

	// bus events only
	std::vector<const Event *> bus;
	u64 end_tick = 0;
	for (const Event &e : ev)
	{
		if (op_code(e.op) >= 0) bus.push_back(&e);
		if (e.op == 'E') end_tick = e.tick;
		end_tick = std::max(end_tick, e.tick);
	}

	size_t idx = 0;
	bool pending = false;
	u64 rc = 0;                 // --real: 10 MHz counter
	unsigned div = 0;
	u64 idle = 0;
	while (true)
	{
		const Event *e = idx < bus.size() ? bus[idx] : nullptr;
		u64 limit = e ? e->order : end_tick;
		u64 now;
		if (real_n)
		{
			if (++div >= real_n) { div = 0; if (rc < limit) rc++; }
			now = std::min(rc, limit);
		}
		else
			now = limit;
		top->now_tick = at(now);

		const bool fire_due = ext && b_armed && !pending && b_deadline <= (e ? e->order : end_tick);
		if (fire_due)
		{
			top->timer_fire = 1;
			top->timer_fire_tick = at(b_deadline);
			b_armed = false;
		}
		else if (!pending && e && (!real_n || rc + lead >= e->order))
		{
			top->req_valid = 1;
			top->req_op = op_code(e->op);
			top->req_data = u8(e->arg);
			top->req_tick = at(e->tick);
			top->req_order = at(e->order);
			pending = true;
		}
		clock();
		top->timer_fire = 0;
		if (top->timer_arm) { b_armed = true; b_deadline = e->tick + u64(top->timer_n) * TICKS_PER_SAMPLE; }
		if (top->timer_cancel) b_armed = false;
		if (pending && top->req_ack)
		{
			if (e->op == 'R') h.reads.push_back({e->tick, top->rd_data});
			top->req_valid = 0;
			pending = false;
			idx++;
			idle = 0;
		}
		if (!e && top->settled && now == end_tick && !(ext && b_armed && b_deadline <= end_tick)) break;
		if (++idle > 200'000'000ull) { h.hung = true; break; }
	}
	for (int i = 0; i < 4; i++) clock();
	h.late = top->late;
	top->final();
	return h;
}

std::string basename_noext(const std::string &p)
{
	std::string b = p.substr(p.find_last_of('/') + 1);
	const auto dot = b.rfind('.');
	return dot == std::string::npos ? b : b.substr(0, dot);
}

} // namespace

int main(int argc, char **argv)
{
	unsigned real_n = 0;
	u64 lead = 30;
	std::string outdir;
	bool ext = false;
	u64 base = 0;
	std::vector<std::string> files;
	for (int i = 1; i < argc; i++)
	{
		const std::string a = argv[i];
		if (a == "--real" && i + 1 < argc) real_n = unsigned(std::atoi(argv[++i]));
		else if (a == "--lead" && i + 1 < argc) lead = std::strtoull(argv[++i], nullptr, 0);
		else if (a == "--out" && i + 1 < argc) outdir = argv[++i];
		else if (a == "--ext") ext = true;
		else if (a == "--base" && i + 1 < argc) base = std::strtoull(argv[++i], nullptr, 0);
		else if (a == "--inject-error") inject_error = true;
		else files.push_back(a);
	}
	if (files.empty()) { fprintf(stderr, "usage: %s [--real N] [--lead L] [--ext] [--base B] [--out DIR] [--inject-error] FILE.ev...\n", argv[0]); return 2; }

	int fails = 0;
	for (const std::string &f : files)
	{
		std::vector<Event> ev;
		std::string err;
		if (!load_events(f, ev, err)) { fprintf(stderr, "%s\n", err.c_str()); return 2; }
		const RunResult ref = run(ev);
		const HdlResult hdl = run_hdl(ev, real_n, lead, ext, base);

		std::string why;
		char buf[512];
		// samples
		const size_t n = std::min(ref.samples.size(), hdl.samples.size());
		for (size_t i = 0; i < n && why.empty(); i++)
			if (ref.samples[i] != hdl.samples[i])
			{
				snprintf(buf, sizeof buf, "sample %zu (tick %zu): ref %d, hdl %d", i, i * 250, ref.samples[i], hdl.samples[i]);
				why = buf;
			}
		if (why.empty() && ref.samples.size() != hdl.samples.size())
		{
			snprintf(buf, sizeof buf, "sample count: ref %zu, hdl %zu", ref.samples.size(), hdl.samples.size());
			why = buf;
		}
		// IRQ changes
		for (size_t i = 0; i < std::max(ref.irq.size(), hdl.irq.size()) && why.empty(); i++)
		{
			const bool ok = i < ref.irq.size() && i < hdl.irq.size()
				&& ref.irq[i].tick == hdl.irq[i].tick && ref.irq[i].state == hdl.irq[i].state;
			if (!ok)
			{
				snprintf(buf, sizeof buf, "IRQ change #%zu: ref %s, hdl %s", i,
					i < ref.irq.size() ? (std::to_string(ref.irq[i].tick) + "=" + std::to_string(ref.irq[i].state) + "/" + char(ref.irq[i].cause)).c_str() : "none",
					i < hdl.irq.size() ? (std::to_string(hdl.irq[i].tick) + "=" + std::to_string(hdl.irq[i].state)).c_str() : "none");
				why = buf;
			}
		}
		// reads
		for (size_t i = 0; i < std::max(ref.reads.size(), hdl.reads.size()) && why.empty(); i++)
		{
			const bool ok = i < ref.reads.size() && i < hdl.reads.size() && ref.reads[i].data == hdl.reads[i].data;
			if (!ok)
			{
				snprintf(buf, sizeof buf, "read #%zu (tick %llu): ref %02X, hdl %02X", i,
					(unsigned long long)(i < ref.reads.size() ? ref.reads[i].tick : 0),
					i < ref.reads.size() ? ref.reads[i].data : 0x100, i < hdl.reads.size() ? hdl.reads[i].data : 0x100);
				why = buf;
			}
		}
		if (why.empty() && hdl.late) why = "HDL flagged a late request";
		if (hdl.hung) why = "HDL bench hung (" + why + ")";

		u64 irqs = 0, nonzero = 0;
		for (const auto &q : ref.irq) irqs += q.state;
		for (s32 s : ref.samples) nonzero += s != 0;
		printf("%s %s%s%s: %zu samples (%llu non-zero), %zu IRQ changes (%llu rising), %zu reads, %llu clk%s%s\n",
			why.empty() ? "MATCH" : "DIFF ", f.c_str(), ext ? " (ext timer)" : "", base ? (" (tick base " + std::to_string(base) + ")").c_str() : "", ref.samples.size(), (unsigned long long)nonzero,
			ref.irq.size(), (unsigned long long)irqs, ref.reads.size(), (unsigned long long)hdl.cycles,
			why.empty() ? "" : "\n      first divergence: ", why.c_str());
		if (!why.empty()) fails++;

		if (!outdir.empty())
		{
			const std::string p = outdir + "/" + basename_noext(f) + ".s32";
			if (FILE *o = fopen(p.c_str(), "wb")) { fwrite(hdl.samples.data(), 4, hdl.samples.size(), o); fclose(o); }
		}
	}
	return fails ? 1 : 0;
}
