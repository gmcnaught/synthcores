// license:BSD-3-Clause
// copyright-holders:Devin Acker
//
// NEC/Casio uPD933 "Phase Distortion" synthesis chip: MAME's model without the MAME framework.
// Everything inside class Upd933 follows src/devices/sound/upd933.cpp line for line; see the
// header for what replaced the scheduler and the sound stream.

#include "upd933_ref.h"

#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <numbers>
#include <sstream>

namespace upd933ref {

template <typename T> static constexpr T BIT(T x, unsigned n) { return (x >> n) & 1; }
template <typename T> static constexpr T BIT(T x, unsigned n, unsigned w) { return (x >> n) & ((T(1) << w) - 1); }

// ---------------------------------------------------------------------------------------------
Tables::Tables()
{
	// device_start(), verbatim arithmetic
	for (int i = 0; i < 0x800; i++)
		cosine[i] = 0xfff * (1 - cos(2.0 * std::numbers::pi * i / 0x7ff)) / 2;

	for (int i = 0; i < 0x80; i++)
	{
		// A4 is note 62, 442 Hz
		const double freq = 442.0 * pow(2, (i - 62) / 12.0);
		pitch[i] = (1 << 20) * (freq * 0x800 / 40000);
	}

	for (int i = 0; i < 0x200; i++)
		pitch_fine[i] = (1 << 12) * (pow(2, (double)i / (12.0 * 0x200)) - 1);

	for (int i = 1; i < 0x200; i++)
		volume[i] = pow(2 << 12, (double)i / 0x1ff);
	volume[0] = 0;
}

const Tables &tables()
{
	static const Tables t;
	return t;
}

// ---------------------------------------------------------------------------------------------
void Upd933::device_reset_state()
{
	m_irq_pending = m_irq_state = 0;
	m_sound_data[0] = m_sound_data[1] = 0;
	m_sound_data_pos = 0;
	std::fill(m_sound_regs.begin(), m_sound_regs.end(), 0);
	std::fill(m_voice.begin(), m_voice.end(), voice_t());
	std::fill(m_dca.begin(), m_dca.end(), env_t());
	std::fill(m_dco.begin(), m_dco.end(), env_t());
	std::fill(m_dcw.begin(), m_dcw.end(), env_t());
	m_sample_count = 0;
	m_last_sample = 0;
	m_timer_armed = false;
}

void Upd933::device_reset(u64 tick)
{
	const u8 old = m_irq_state;
	if (mamelog) fprintf(mamelog, "%llu %u X\n", (unsigned long long)tick, m_sample_count);
	device_reset_state();
	// m_irq_cb(0): not through update_irq, so MAME's log has no Q line for it
	if (old)
		irq_log.push_back({tick, 0, 'X'});
}

void Upd933::timer_fire()
{
	const u64 t = m_timer_deadline;
	m_timer_armed = false;
	if (mamelog) fprintf(mamelog, "%llu %u T\n", (unsigned long long)t, m_sample_count);
	m_irq_pending = 1;
	update_irq(t, 'T');
}

void Upd933::cs_w(int state, u64 tick)
{
	if (mamelog) fprintf(mamelog, "%llu %u C %d\n", (unsigned long long)tick, m_sample_count, state);
	if (!m_cs && state)
		update_pending_irq(tick);
	m_cs = state;
	update_irq(tick, 'C');
}

void Upd933::id_w(int state, u64 tick)
{
	if (mamelog) fprintf(mamelog, "%llu %u I %d\n", (unsigned long long)tick, m_sample_count, state);
	m_id = state;
	update_irq(tick, 'I');
}

u8 Upd933::irq_data()
{
	for (int i = 0; i < 8; i++)
		if (m_dco[i].m_irq) { m_dco[i].m_irq = false; return 4 | (i << 3); }
	for (int i = 0; i < 8; i++)
		if (m_dcw[i].m_irq) { m_dcw[i].m_irq = false; return 2 | (i << 2); }
	for (int i = 0; i < 8; i++)
		if (m_dca[i].m_irq) { m_dca[i].m_irq = false; return 1 | (i << 1); }
	return 0;
}

void Upd933::update_pending_irq(u64 tick)
{
	m_irq_pending = 0;
	bool env_active = false;
	unsigned new_time = UINT_MAX;

	for (int i = 0; i < 8; i++)
	{
		env_active |= m_dca[i].calc_timeout(new_time);
		env_active |= m_dco[i].calc_timeout(new_time);
		env_active |= m_dcw[i].calc_timeout(new_time);
	}

	if (env_active)
	{
		cov.timer_armed++;
		if (new_time == 0) cov.timer_zero++;
		// clocks_to_attotime(new_time * 112) at 4.48 MHz is new_time * 25 us less
		// 32 attoseconds per sample (m_attoseconds_per_clock is truncated), except for whole
		// seconds. Modelled as new_time * 250 ticks; m_timer_exact records whether MAME's
		// expiry is exactly on that tick (its logged tick is one lower otherwise).
		m_timer_armed = true;
		m_timer_deadline = tick + u64(new_time) * TICKS_PER_SAMPLE;
		m_timer_exact = (u64(new_time) % 40000) == 0;
	}
	else
	{
		cov.timer_idle++;
		m_timer_armed = false;
	}
}

void Upd933::update_irq(u64 tick, u8 cause)
{
	const u8 irq_state = m_cs & m_id & m_irq_pending;
	if (irq_state != m_irq_state)
	{
		m_irq_state = irq_state;
		if (mamelog) fprintf(mamelog, "%llu %u Q %u\n", (unsigned long long)tick, m_sample_count, irq_state);
		irq_log.push_back({tick, irq_state, cause});
	}
}

u8 Upd933::read()
{
	const u8 d = m_cs ? 0xff : irq_data();
	if (mamelog) fprintf(mamelog, "%llu %u R %02X\n", (unsigned long long)now, m_sample_count, d);
	return d;
}

void Upd933::write(u8 data)
{
	if (m_cs) return;

	if (m_sound_data_pos >= 2)
	{
		const u8 reg = m_sound_data[0];
		const u16 value = m_sound_regs[reg] = (m_sound_data[1] << 8) | data;
		if (mamelog) fprintf(mamelog, "%llu %u W %02X %04X\n", (unsigned long long)now, m_sample_count, reg, value);
		const int vnum = reg & 7;
		voice_t &voice = m_voice[vnum];
		voice_t &mod_voice = m_voice[(vnum + 6) & 7];

		m_sound_data_pos = 0;
		switch (reg >> 3)
		{
		case 0x0: // 00-07: DCA step
		{
			env_t &dca = m_dca[vnum];
			dca.m_direction = BIT<u32>(value, 15);
			dca.m_rate      = env_rate(BIT<u32>(value, 8, 7));
			dca.m_sustain   = BIT<u32>(value, 7);
			dca.m_target    = BIT<u32>(value, 0, 7) << (ENV_DCA_SHIFT + 2);
			dca.m_irq = false;
		}
			break;

		case 0x2: // 10-17: DCO step
		{
			env_t &dco = m_dco[vnum];
			dco.m_direction = BIT<u32>(value, 15);
			dco.m_rate      = env_rate(BIT<u32>(value, 8, 7));
			dco.m_sustain   = BIT<u32>(value, 7);
			dco.m_target    = BIT<u32>(value, 0, 6) << (ENV_DCO_SHIFT + 5);
			if (BIT<u32>(value, 6))
				dco.m_target <<= 5;
			dco.m_irq = false;
		}
			break;

		case 0x4: // 20-27: DCW step
		{
			env_t &dcw = m_dcw[vnum];
			dcw.m_direction = BIT<u32>(value, 15);
			dcw.m_rate      = env_rate(BIT<u32>(value, 8, 7));
			dcw.m_sustain   = BIT<u32>(value, 7);
			dcw.m_target    = BIT<u32>(value, 0, 7) << (ENV_DCW_SHIFT + 3);
			dcw.m_irq = false;
		}
			break;

		case 0xc: // 60-67: pitch
			voice.m_pitch = value;
			update_pitch_step(vnum);
			break;

		case 0xd: // 68-6f: waveform
			voice.m_wave[0]         = BIT<u32>(value, 13, 3);
			if (BIT<u32>(value, 9))
				voice.m_wave[1]     = BIT<u32>(value, 10, 3);
			else
				voice.m_wave[1]     = voice.m_wave[0];
			voice.m_window          = BIT<u32>(value, 6, 3);
			if (!BIT(vnum, 0))
			{
				mod_voice.m_ring_mod    = BIT<u32>(value, 5);
				mod_voice.m_pitch_mod   = BIT<u32>(value, 3, 2);
				mod_voice.m_mute_other  = BIT<u32>(value, 2);
			}
			break;

		case 0x13: // 98-9f: phase counter
			voice.m_position = u32(value) << (PITCH_SHIFT - 4);
			break;

		case 0x17: // b8-bb: pitch modulator
			if (vnum < 4)
				m_voice[vnum << 1].m_pm_level = (s16)value;
			break;

		default:
			break;
		}
	}
	else
	{
		m_sound_data[m_sound_data_pos++] = data;
	}
}

s32 Upd933::generate()
{
	s32 sample = 0;
	static const int voice_map[] = {5, 0, 7, 2, 1, 4, 3, 6};
	for (int j : voice_map)
		sample += update(j);
	m_sample_count++;
	return sample;
}

u32 Upd933::machine_rand()
{
	if (!noise_override.empty())
	{
		const u32 v = noise_override.front();
		noise_override.pop_front();
		return v;
	}
	m_rand_seed = 1664525 * m_rand_seed + 1013904223;
	const u32 v = (m_rand_seed >> 16) | (m_rand_seed << 16);
	if (mamelog) fprintf(mamelog, "%llu %u N %08X\n", (unsigned long long)now, m_sample_count, v);
	return v;
}

s16 Upd933::update(int vnum)
{
	const Tables &t = tables();
	voice_t &voice = m_voice[vnum];
	s16 sample = 0;

	const u16 pos = BIT<u32>(voice.m_position, PITCH_SHIFT, 11);
	const u8 wave = BIT<u32>(voice.m_position, PITCH_SHIFT + 11);

	const u16 dcw = std::min(u16(m_dcw[vnum].m_current >> ENV_DCW_SHIFT), voice.m_dcw_limit);
	const u16 pivot = 0x400 - dcw;
	cov.wave[voice.m_wave[wave] & 7]++;
	cov.window[voice.m_window & 7]++;
	if ((m_dcw[vnum].m_current >> ENV_DCW_SHIFT) > voice.m_dcw_limit) cov.dcw_limited++;
	u16 phase = 0;
	u16 window = 0;

	switch (voice.m_wave[wave] & 7)
	{
	case 0:
		if (pos < pivot)
			phase = pos * 0x400 / pivot;
		else
			phase = 0x400 + (pos - pivot) * 0x400 / (0x800 - pivot);
		break;
	case 1:
		if ((pos & 0x3ff) < pivot)
			phase = (pos & 0x3ff) * 0x400 / pivot;
		else
			phase = 0x3ff;
		phase |= (pos & 0x400);
		break;
	case 2:
		if (pos < pivot * 2)
			phase = pos * 0x800 / (pivot * 2);
		else
			phase = 0x7ff;
		break;
	case 3:
		break;
	case 4:
		if (pos < pivot)
			phase = pos * 0x800 / pivot;
		else
			phase = (pos - pivot) * 0x800 / (0x800 - pivot);
		break;
	case 5:
		if (pos < 0x400)
			phase = pos;
		else if (pos < (pivot + 0x400))
			phase = 0x400 + (pos & 0x3ff) * 0x400 / pivot;
		else
			phase = 0x7ff;
		break;
	case 6:
		phase = pos + ((pos * dcw) >> 6);
		phase &= 0x7ff;
		break;
	case 7:
		if ((pos & 0x3ff) < pivot)
			phase = (pos & 0x3ff) * 0x400 / pivot;
		else
			phase = 0x7ff;
		break;
	}

	switch (voice.m_window & 7)
	{
	case 0: break;
	case 1: window = pos; break;
	case 2:
		window = (pos & 0x3ff) * 2;
		if (pos < 0x400)
			window ^= 0x7fe;
		break;
	case 3:
		if (pos >= 0x400)
			window = (pos & 0x3ff) * 2;
		break;
	case 4:
		if (pos < 0x400)
			window = pos * 2;
		else
			window = 0x7ff;
		break;
	default:
		window = (0x3ff ^ (pos & 0x3ff)) * 2;
		break;
	}

	sample = t.cosine[phase];
	if (window)
		sample = ((s32)sample * (0x800 - window)) / 0x800;

	const u16 volume = m_dca[vnum].m_current >> ENV_DCA_SHIFT;
	sample = ((s32)sample * t.volume[volume]) >> VOLUME_SHIFT;
	sample -= t.volume[volume] / 2;

	if (voice.m_ring_mod)
	{
		cov.ring++;
		const s32 q = ((s32)sample * m_last_sample) / 0x1000;
		if (q != s16(q)) cov.ring_wrap++;
		sample = q;
	}

	if (voice.m_mute_other)
	{
		cov.mute++;
		sample -= m_last_sample;
	}

	const u32 old_dco = m_dco[vnum].m_current;
	const s16 old_pm = voice.m_pm_level;

	for (env_t *e : {&m_dca[vnum], &m_dcw[vnum], &m_dco[vnum]})
	{
		if (e->m_current != e->m_target && (e->m_direction ? e->m_current < e->m_target : e->m_current > e->m_target))
			cov.env_wrong_side++;
		const u8 irq0 = e->m_irq;
		e->update();
		if (e->m_irq && !irq0) cov.env_irq++;
	}

	if (!(m_sample_count & 7))
	{
		switch (voice.m_pitch_mod & 3)
		{
		default:
			voice.m_pm_level = 0;
			break;
		case 2:
			cov.pm_other++;
			voice.m_pm_level = m_last_sample;
			break;
		case 3:
			cov.pm_noise++;
			voice.m_pm_level = machine_rand() & (32 << NOTE_SHIFT);
			break;
		}
	}

	if ((old_dco ^ m_dco[vnum].m_current) >> ENV_DCO_SHIFT
		|| old_pm != voice.m_pm_level)
	{
		cov.pitch_recalc++;
		update_pitch_step(vnum);
	}

	voice.m_position += voice.m_pitch_step;

	m_last_sample = sample;
	return sample;
}

void Upd933::env_t::update()
{
	if (m_current != m_target)
	{
		if (!m_direction)
		{
			if (m_current > m_target || m_target - m_current <= m_rate)
				m_current = m_target;
			else
				m_current += m_rate;
		}
		else
		{
			if (m_current < m_target || m_current - m_target <= m_rate)
				m_current = m_target;
			else
				m_current -= m_rate;
		}
	}

	if (!m_sustain && (m_current == m_target))
		m_irq = m_sustain = true;
}

bool Upd933::env_t::calc_timeout(unsigned &samples)
{
	if (m_irq)
	{
		samples = 0;
	}
	else if (m_sustain || !m_rate)
	{
		return false;
	}
	else
	{
		const unsigned remaining = m_direction ? (m_current - m_target) : (m_target - m_current);
		unsigned new_time = remaining / m_rate;
		if (remaining % m_rate)
			new_time++;
		if (new_time < samples)
			samples = new_time;
	}
	return true;
}

void Upd933::update_pitch_step(int vnum)
{
	const Tables &t = tables();
	voice_t &voice = m_voice[vnum];
	const s32 pitch = s32(voice.m_pitch + (m_dco[vnum].m_current >> ENV_DCO_SHIFT)) + voice.m_pm_level;
	u32 step = 0;

	if (!(pitch > 0 && pitch < (1 << 16)))
		cov.pitch_out_of_range++;
	if (pitch > 0 && pitch < (1 << 16))
	{
		const u8 note = pitch >> NOTE_SHIFT;
		const u16 fine = pitch & ((1 << NOTE_SHIFT) - 1);
		step = t.pitch[note];
		if (fine)
			step += (step >> PITCH_FINE_SHIFT) * t.pitch_fine[fine];
	}

	voice.m_pitch_step = step;
	voice.m_dcw_limit = 0x400 - std::min(0x400U, (step >> (PITCH_SHIFT - 2)));
}

// ---------------------------------------------------------------------------------------------
// event files: one event per line, '#' comments
//   <tick> <op> [arg] [o=<order tick>] [s=<sample count>]
bool load_events(const std::string &path, std::vector<Event> &ev, std::string &err)
{
	std::ifstream f(path);
	if (!f) { err = "cannot open " + path; return false; }
	std::string line;
	unsigned lineno = 0;
	while (std::getline(f, line))
	{
		lineno++;
		const auto hash = line.find('#');
		if (hash != std::string::npos) line.resize(hash);
		std::istringstream is(line);
		std::string tok;
		if (!(is >> tok)) continue;
		Event e;
		e.tick = std::stoull(tok);
		std::string op;
		if (!(is >> op) || op.size() != 1) { err = path + ":" + std::to_string(lineno) + ": bad op"; return false; }
		e.op = op[0];
		e.order = e.tick;
		e.arg = (e.op == 'R') ? -1 : 0;
		while (is >> tok)
		{
			if (tok.rfind("o=", 0) == 0) e.order = std::stoull(tok.substr(2));
			else if (tok.rfind("s=", 0) == 0) e.sidx = std::stoll(tok.substr(2));
			else e.arg = std::stoll(tok, nullptr, 16);
		}
		ev.push_back(e);
	}
	return true;
}

bool save_events(const std::string &path, const std::vector<Event> &ev)
{
	FILE *f = fopen(path.c_str(), "w");
	if (!f) return false;
	for (const Event &e : ev)
	{
		fprintf(f, "%llu %c", (unsigned long long)e.tick, e.op);
		if (e.op == 'W' || e.op == 'C' || e.op == 'I' || e.op == 'N' || e.op == 'Q' || (e.op == 'R' && e.arg >= 0))
			fprintf(f, " %llx", (unsigned long long)e.arg);
		if (e.order != e.tick) fprintf(f, " o=%llu", (unsigned long long)e.order);
		if (e.sidx >= 0) fprintf(f, " s=%lld", (long long)e.sidx);
		fputc('\n', f);
	}
	return fclose(f) == 0;
}

// ---------------------------------------------------------------------------------------------
void Runner::apply(const Event &e)
{
	char buf[256];
	if (!m_log_timers && e.op != 'N' && e.op != 'Q')
		while (chip.timer_armed() && timer_fires_before(chip.timer_deadline(), e.order))
			chip.timer_fire();

	switch (e.op)
	{
	case 'N': chip.noise_override.push_back(u32(e.arg)); return;
	case 'Q': m_expect_q.push_back({e.tick, u8(e.arg), 0}); return;
	case 'T':
		r.timer_checked++;
		if (!chip.timer_armed() || !(chip.timer_deadline() == e.tick || chip.timer_deadline() == e.tick + 1))
		{
			if (!r.timer_mismatch++)
			{
				snprintf(buf, sizeof buf, "log timer at %llu, model %s %llu", (unsigned long long)e.tick,
					chip.timer_armed() ? "armed for" : "not armed, last", (unsigned long long)chip.timer_deadline());
				r.timer_first = buf;
			}
		}
		chip.timer_fire();
		return;
	default: break;
	}

	// samples MAME has generated before this access
	u64 target = samples_by_tick(e.tick);
	if (e.sidx >= 0)
	{
		const u64 logged = m_base + u64(e.sidx);
		r.rule_checked++;
		if (logged != target && !r.rule_mismatch++)
		{
			snprintf(buf, sizeof buf, "tick %llu op %c: rule gives %llu samples, log %llu",
				(unsigned long long)e.tick, e.op, (unsigned long long)target, (unsigned long long)logged);
			r.rule_first = buf;
		}
		target = logged;
	}
	gen_to(target);
	chip.now = e.tick;

	switch (e.op)
	{
	case 'W': chip.write(u8(e.arg)); break;
	case 'R':
	{
		const u8 d = chip.read();
		r.reads.push_back({e.tick, d});
		if (e.arg >= 0)
		{
			r.read_checked++;
			if (d != u8(e.arg) && !r.read_mismatch++)
			{
				snprintf(buf, sizeof buf, "tick %llu: read %02X, log %02X", (unsigned long long)e.tick, d, unsigned(e.arg));
				r.read_first = buf;
			}
		}
		break;
	}
	case 'C': chip.cs_w(int(e.arg & 1), e.tick); break;
	case 'I': chip.id_w(int(e.arg & 1), e.tick); break;
	case 'X': chip.device_reset(e.tick); m_base = r.samples.size(); break;
	case 'S': break;
	case 'E':
		if (!m_log_timers)
			while (chip.timer_armed() && timer_fires_before(chip.timer_deadline(), e.tick))
				chip.timer_fire();
		break;
	default: break;
	}
}

RunResult Runner::finish()
{
	char buf[256];
	r.irq = chip.irq_log;
	if (!m_expect_q.empty())
	{
		std::vector<IrqChange> got;
		for (const IrqChange &q : r.irq) if (q.cause != 'X') got.push_back(q);
		const size_t n = std::max(got.size(), m_expect_q.size());
		for (size_t i = 0; i < n; i++)
		{
			r.q_checked++;
			bool ok = i < got.size() && i < m_expect_q.size() && got[i].state == m_expect_q[i].state;
			// MAME's timer expiry is 32 as per sample early: its logged tick may be one lower
			if (ok) ok = got[i].tick == m_expect_q[i].tick || (got[i].cause == 'T' && got[i].tick == m_expect_q[i].tick + 1);
			if (!ok && !r.q_mismatch++)
			{
				const std::string g = i < got.size() ? std::to_string(got[i].tick) + "=" + std::to_string(got[i].state) + "/" + char(got[i].cause) : "none";
				const std::string x = i < m_expect_q.size() ? std::to_string(m_expect_q[i].tick) + "=" + std::to_string(m_expect_q[i].state) : "none";
				snprintf(buf, sizeof buf, "IRQ change #%zu: model %s, log %s", i, g.c_str(), x.c_str());
				r.q_first = buf;
			}
		}
	}
	return std::move(r);
}

RunResult run(const std::vector<Event> &ev, const RunOptions &opt)
{
	const bool log_timers = std::any_of(ev.begin(), ev.end(), [](const Event &e) { return e.op == 'T'; });
	Runner rn(opt, log_timers);
	for (const Event &e : ev)
		rn.apply(e);
	return rn.finish();
}

} // namespace upd933ref
