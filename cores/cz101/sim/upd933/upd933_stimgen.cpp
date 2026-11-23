// license:BSD-3-Clause
// Synthetic uPD933 bus traffic, generated closed-loop against the C++ reference so that IRQs are
// serviced the way the CZ-101 firmware does (/CS low, read the IRQ source, write the next envelope
// step, /CS high). Random but plausible: note on/off over all 8 voices, every waveform and window,
// ring and pitch modulation (other voice and noise), pitch bends, envelope programs with sustain
// and end steps, /ID toggles, reads with /CS high, unknown registers, rare device resets.
//
//   upd933_stimgen SEED SECONDS OUT.ev [--order] [--mamelog LOG SAMPLES]
//     --order: some accesses get an order tick before their access tick (MAME's instruction
//              overshoot), to exercise the timer-versus-access ordering rule.
//     --mamelog: also write what the patched MAME would log for this traffic (CZ_SND_LOG format
//              with the optional C/I/X/T/N lines) and the int32 samples (CZ_SAMPLES format),
//              to self-test upd933_log2ev.py and upd933_replay.
#include "../../ref/upd933_ref.h"

#include <cstdio>
#include <cstdlib>
#include <random>
#include <string>

using namespace upd933ref;

namespace {

struct EnvProg {
	struct Step { u8 dir, code, sus, level; };
	Step step[8];
	int n = 0, idx = 0;
	bool active = false;
};

struct Gen {
	std::mt19937_64 rng;
	Runner rn;
	std::vector<Event> out;
	u64 t = 1000, prev = 0;
	bool order_games;
	EnvProg env[3][8];       // kind 0 DCA, 1 DCW, 2 DCO
	bool id_low = false;

	Gen(u64 seed, bool og) : rng(seed), order_games(og) {}

	u64 rnd(u64 n) { return rng() % n; }
	bool chance(unsigned pct) { return rnd(1000) < pct * 10; }
	bool chance_permille(unsigned pm) { return rnd(1000) < pm; }

	void emit(char op, s64 arg)
	{
		Event e;
		e.tick = t;
		e.order = t;
		if (order_games && t > prev + 3 && chance(25))
			e.order = t - rnd(std::min<u64>(t - prev, 40));
		e.op = op;
		e.arg = arg;
		rn.apply(e);
		out.push_back(e);
		prev = t;
		t += 12 + 3 * rnd(10);    // uPD7811 instruction spacing, 3-tick cycles
	}
	void wr(u8 reg, u16 v) { emit('W', reg); emit('W', v >> 8); emit('W', v & 0xff); }
	u8 rd() { emit('R', -1); return rn.r.reads.back().data; }

	bool irq_due(u64 &when)
	{
		const Upd933 &c = rn.chip;
		if (c.rq_r()) { when = t; return true; }
		if (c.timer_armed()) { when = std::max(c.timer_deadline(), t); return true; }
		return false;
	}

	u8 env_reg(int kind, int v) { return u8((kind == 0 ? 0x00 : kind == 1 ? 0x20 : 0x10) | v); }

	u16 step_value(int kind, const EnvProg::Step &s)
	{
		return u16((s.dir << 15) | (s.code << 8) | (s.sus << 7) | (kind == 2 ? (s.level & 0x7f) : (s.level & 0x7f)));
	}

	u8 rate_code()
	{
		const u64 r = rnd(100);
		if (r < 10) return u8(rnd(48));          // slow (seconds)
		if (r < 85) return u8(48 + rnd(56));     // normal
		return u8(104 + rnd(24));                // very fast
	}

	void make_prog(int kind, int v, bool release)
	{
		EnvProg &p = env[kind][v];
		p.n = release ? 1 : 1 + int(rnd(8));
		p.idx = 0;
		p.active = true;
		const int sus_at = release ? -1 : (chance(70) ? int(rnd(p.n)) : -1);
		for (int i = 0; i < p.n; i++)
		{
			EnvProg::Step &s = p.step[i];
			s.code = rate_code();
			s.level = release ? 0 : u8(kind == 2 ? (rnd(64) | (chance(20) ? 0x40 : 0)) : rnd(128));
			s.dir = release ? 1 : u8(chance(85) ? (i == 0 ? 0 : rnd(2)) : rnd(2));
			s.sus = (i == sus_at || i == p.n - 1) ? 1 : 0;
			if (i == p.n - 1 && chance(50)) s.sus = 0;   // end step that still interrupts
		}
	}

	void write_step(int kind, int v)
	{
		EnvProg &p = env[kind][v];
		if (!p.active || p.idx >= p.n) { p.active = false; return; }
		wr(env_reg(kind, v), step_value(kind, p.step[p.idx]));
	}

	u16 rand_pitch()
	{
		if (chance(5)) return u16(rnd(0x10000));
		return u16(((12 + rnd(90)) << 9) | (chance(50) ? rnd(512) : 0));
	}

	u16 rand_wave()
	{
		const u16 w0 = u16(rnd(8)), w1 = u16(rnd(8));
		const u16 en2 = chance(40);
		const u16 win = u16(chance(50) ? 0 : rnd(8));
		const u16 ring = chance(15), pm = u16(chance(80) ? 0 : rnd(4)), mute = chance(10);
		return u16((w0 << 13) | (w1 << 10) | (en2 << 9) | (win << 6) | (ring << 5) | (pm << 3) | (mute << 2) | rnd(4));
	}

	void note_on()
	{
		const int v = int(rnd(8));
		emit('C', 0);
		wr(u8(0x68 | v), rand_wave());
		if (chance(30)) wr(u8(0x68 | (v ^ 1)), rand_wave());
		wr(u8(0x60 | v), rand_pitch());
		if (chance(80)) wr(u8(0x98 | v), chance(80) ? 0 : u16(rnd(0x10000)));
		for (int k = 0; k < 3; k++)
		{
			make_prog(k, v, false);
			write_step(k, v);
		}
		emit('C', 1);
	}

	void note_off()
	{
		const int v = int(rnd(8));
		emit('C', 0);
		for (int k = 0; k < 3; k++)
		{
			make_prog(k, v, true);
			write_step(k, v);
		}
		emit('C', 1);
	}

	void service_irq()
	{
		emit('C', 0);
		const int reads = chance(80) ? 1 : 2 + int(rnd(2));
		for (int i = 0; i < reads; i++)
		{
			const u8 d = rd();
			int kind = -1, v = 0;
			if (d & 4) { kind = 2; v = (d >> 3) & 7; }
			else if (d & 2) { kind = 1; v = (d >> 2) & 7; }
			else if (d & 1) { kind = 0; v = (d >> 1) & 7; }
			if (kind < 0) break;
			EnvProg &p = env[kind][v];
			if (p.active)
			{
				p.idx++;
				write_step(kind, v);
			}
		}
		emit('C', 1);
	}

	void misc()
	{
		const u64 r = rnd(100);
		if (r < 30) { emit('C', 0); wr(u8(0x60 | rnd(8)), rand_pitch()); emit('C', 1); }                       // bend
		else if (r < 45) { emit('C', 0); wr(u8(0x68 | (rnd(4) * 2)), rand_wave()); emit('C', 1); }             // modulation
		else if (r < 55) { emit('C', 0); wr(u8(0xb8 | rnd(8)), u16(chance(50) ? 0 : rnd(0x10000))); emit('C', 1); }
		else if (r < 62) { emit('C', 0); wr(u8(rnd(256)), u16(rnd(0x10000))); emit('C', 1); }                  // any register
		else if (r < 68) { emit('C', 0); wr(u8(0x98 | rnd(8)), u16(rnd(0x10000))); emit('C', 1); }
		else if (r < 74) { emit('R', -1); }                                                                     // /CS high read
		else if (r < 78) { emit('W', s64(rnd(256))); }                                                          // /CS high write
		else if (r < 84) { id_low = !id_low; emit('I', id_low ? 0 : 1); }
		else if (r < 90) { emit('C', 0); emit('C', 1); }
		else if (r < 95) { emit('C', 0); if (chance(50)) rd(); emit('C', 1); }
		else { emit('S', 0); }
	}

	void run(u64 end_tick)
	{
		u64 plan = t + 2000;
		while (t < end_tick)
		{
			u64 when;
			const bool irq = irq_due(when) && !id_low;
			const u64 lat = 30 + rnd(400);
			if (irq && when + lat <= plan)
			{
				t = std::max(t, when + lat);
				service_irq();
				continue;
			}
			t = std::max(t, plan);
			const u64 r = rnd(1000);
			if (r < 300) note_on();
			else if (r < 500) note_off();
			else if (r < 999) misc();
			else
			{
				emit('X', 0);
				for (auto &k : env) for (auto &p : k) p.active = false;
				id_low = false;
				emit('I', 1);
			}
			plan = t + 200 + rnd(chance(70) ? 4000 : 60000);
		}
		Event e;
		e.tick = e.order = t;
		e.op = 'E';
		rn.apply(e);
		out.push_back(e);
	}
};

} // namespace

int main(int argc, char **argv)
{
	if (argc < 4)
	{
		fprintf(stderr, "usage: %s SEED SECONDS OUT.ev [--order]\n", argv[0]);
		return 2;
	}
	const u64 seed = std::strtoull(argv[1], nullptr, 0);
	const double secs = std::atof(argv[2]);
	bool og = false;
	std::string mlog, msamples;
	for (int i = 4; i < argc; i++)
	{
		const std::string a = argv[i];
		if (a == "--order") og = true;
		else if (a == "--mamelog" && i + 2 < argc) { mlog = argv[++i]; msamples = argv[++i]; }
	}
	Gen g(seed, og);
	g.run(u64(secs * 10'000'000));
	if (!save_events(argv[3], g.out)) { fprintf(stderr, "cannot write %s\n", argv[3]); return 1; }
	const RunResult r = g.rn.finish();
	if (!mlog.empty())
	{
		Runner rn;
		rn.chip.mamelog = fopen(mlog.c_str(), "w");
		for (const Event &e : g.out) rn.apply(e);
		fclose(rn.chip.mamelog);
		const RunResult r2 = rn.finish();
		if (FILE *o = fopen(msamples.c_str(), "wb")) { fwrite(r2.samples.data(), 4, r2.samples.size(), o); fclose(o); }
	}
	unsigned long irqs = 0;
	for (const auto &q : r.irq) irqs += q.state;
	printf("%s: %zu events, %zu samples, %lu IRQs, %zu reads\n", argv[3], g.out.size(), r.samples.size(), irqs, r.reads.size());
	const auto &c = g.rn.chip.cov;
	printf("  voice-samples by wave:");
	for (u64 w : c.wave) printf(" %llu", (unsigned long long)w);
	printf("\n  by window:");
	for (u64 w : c.window) printf(" %llu", (unsigned long long)w);
	printf("\n  ring %llu (wrap %llu), mute %llu, pm-other latches %llu, noise latches %llu, pitch recalcs %llu (out of range %llu)\n",
		(unsigned long long)c.ring, (unsigned long long)c.ring_wrap, (unsigned long long)c.mute, (unsigned long long)c.pm_other,
		(unsigned long long)c.pm_noise, (unsigned long long)c.pitch_recalc, (unsigned long long)c.pitch_out_of_range);
	printf("  dcw limited %llu, env wrong-side steps %llu, env irqs %llu, timer armed %llu (zero %llu), idle %llu\n",
		(unsigned long long)c.dcw_limited, (unsigned long long)c.env_wrong_side, (unsigned long long)c.env_irq,
		(unsigned long long)c.timer_armed, (unsigned long long)c.timer_zero, (unsigned long long)c.timer_idle);
	return 0;
}
