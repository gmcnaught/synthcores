// license:BSD-3-Clause
// copyright-holders:emu-proteus-xr-MiSTer contributors
/*
    Floating-point reference of p2k_sound.h: the same structure, ramps and time base, with the
    arithmetic in float/double as the MAME research model had it before the integer model
    (mame/patches 0002, emup2k.cpp / emup2k_fx.h). Used only to measure what the integer
    arithmetic changes (p2k_render --float, --compare). Not a definition of anything.
*/
#ifndef P2K_SOUND_FLOAT_H
#define P2K_SOUND_FLOAT_H

#include "p2k_sound.h"

#include <cmath>

namespace p2k {

class fx_float
{
public:
	fx_float()
	{
		static const int refl_len[6] = { 2, 4, 8, 13, 19, 26 };
		for (int i = 0; i < 6; i++)
			resize(m_refl[i].buf, refl_len[i] * STEP);
		for (int c = 0; c < 2; c++)
		{
			resize(m_ap[c][0].buf, 2 * 4 * STEP + 55);
			resize(m_ap[c][1].buf, 1 * 4 * STEP + 55);
			resize(m_tail[c].buf, 27 * STEP);
			m_cbuf[c].assign(CHORUS_LEN, 0.0);
		}
	}

	void configure(const uint16_t (*regs)[64])
	{
		auto init = [&](int n, int ch) -> uint16_t { static const int off[4] = { 0x28, 0x2a, 0x38, 0x3a }; return regs[ch][off[n - 1] / 2]; };
		auto dword = [&](int ch, int lo, int hi) -> uint32_t { return (uint32_t(regs[ch][hi / 2]) << 16) | regs[ch][lo / 2]; };
		m_active = init(1, 0) != 0x03ff;
		m_out_mix = (init(1, 3) & 0xff) / 256.0;
		m_ap_fb = (init(1, 5) & 0xff) / 255.0;
		m_stereo = init(1, 7) == 0x8474;
		static const int gain_slot[6][2] = { { 1, 0x0f }, { 1, 0x17 }, { 1, 0x1f }, { 2, 0x07 }, { 2, 0x0f }, { 2, 0x17 } };
		static const int fb_slot[6][2] = { { 1, 0x09 }, { 1, 0x11 }, { 1, 0x19 }, { 2, 0x01 }, { 2, 0x09 }, { 2, 0x11 } };
		for (int i = 0; i < 6; i++)
		{
			m_refl[i].gain = ((init(gain_slot[i][0], gain_slot[i][1]) >> 4) & 0xf) / 15.0;
			m_refl[i].fb = (init(fb_slot[i][0], fb_slot[i][1]) & 0xf) / 15.0;
		}
		m_damp = (init(2, 0x1d) & 0xff) / 255.0;
		m_refl_in = (init(3, 1) & 0xff) / 256.0;
		m_tail_ret = (init(4, 0x1f) & 0xff) / 256.0;
		const int tl = ((init(2, 0x14) >> 8) & 0xf) + 18;
		resize(m_refl[5].buf, tl * STEP);
		resize(m_tail[0].buf, (tl + 1) * STEP);
		const int tr = m_stereo ? ((init(2, 0x16) >> 8) & 0xf) + 18 : tl;
		resize(m_tail[1].buf, (tr + 1) * STEP);
		m_ch_delay = std::clamp<double>(init(3, 0x1a), 1.0, double(CHORUS_LEN - 4096));
		m_ch_fb = (init(3, 0x17) & 0xff) / 256.0;
		m_ch_depth = (init(4, 0x0b) & 0xff) * m_ch_delay / 256.0;
		// the integer model's phase increment (milli-Hz -> 2^32 / 44100 as (reg * 99731) >> 10, 3.5 ppm
		// fast): the LFO drifts otherwise, which is a definition difference, not arithmetic error
		m_ch_rate = double((uint64_t(regs[0x0a][0x1a / 2]) * 99731) >> 10) / 4294967296.0 * 44100.0;
		for (int b = 0; b < 4; b++)
		{
			const uint32_t ptrx = dword(4 * b, 0x16, 0x14), csl = dword(4 * b, 0x76, 0x74);
			m_rev_send[b] = ((ptrx >> 8) & 0xff) / 255.0;
			m_cho_send[b] = ((csl >> 24) & 0xff) / 255.0;
			const uint32_t psst = dword(4 * b, 0x66, 0x64), ptrx_r = dword(4 * b + 0x10, 0x16, 0x14);
			m_dry[b][0] = (b == 1 || b == 2) ? 1.0 : (psst >> 24) / 255.0;
			m_dry[b][1] = (b == 1 || b == 2) ? 1.0 : (ptrx_r & 0xff) / 255.0;
		}
	}

	double dry(int bus, int side) const { return m_dry[bus & 3][side & 1]; }
	double reverb_send(int bus) const { return m_rev_send[bus & 3]; }
	double chorus_send(int bus) const { return m_cho_send[bus & 3]; }

	void process(double rev_in, double cho_in, double &l, double &r)
	{
		if (!m_active)
			return;
		m_damp_state = rev_in * (1.0 - m_damp) + m_damp_state * m_damp;
		const double in = m_damp_state;
		const double in2 = in * m_refl_in;
		double o[6];
		for (int i = 0; i < 6; i++)
			o[i] = comb(m_refl[i], in2);
		double dl, dr;
		if (m_stereo)
		{
			dl = o[2] + o[4];
			dr = o[0] + o[1] + o[3] + o[5];
		}
		else
			dl = dr = o[0] + o[1] + o[2] + o[3] + o[4] + o[5];
		dl += tail(0, in + dl) * m_tail_ret;
		dr += tail(1, in + dr) * m_tail_ret;
		l += dl * m_out_mix;
		r += dr * m_out_mix;
		const double lfo = std::sin(m_phase * 6.283185307179586) * m_ch_depth;
		const double lfo_r = std::sin((m_phase + 0.25) * 6.283185307179586) * m_ch_depth;
		m_phase += m_ch_rate / 44100.0;
		m_phase -= std::floor(m_phase);
		const double cl = tap(0, m_ch_delay + lfo), cr = tap(1, m_ch_delay + lfo_r);
		m_cbuf[0][m_cpos] = cho_in + cl * m_ch_fb;
		m_cbuf[1][m_cpos] = cho_in + cr * m_ch_fb;
		m_cpos = (m_cpos + 1) % CHORUS_LEN;
		l += cl;
		r += cr;
	}

private:
	static constexpr int STEP = 242;
	static constexpr int CHORUS_LEN = 0x8000;
	struct line { std::vector<double> buf; size_t pos = 0; double lp = 0, gain = 0, fb = 0; };
	static void resize(std::vector<double> &b, size_t n) { if (b.size() != n) b.assign(n, 0.0); }

	double comb(line &c, double in)
	{
		if (c.pos >= c.buf.size())
			c.pos = 0;
		const double out = c.buf[c.pos];
		c.lp = out * (1.0 - m_damp) + c.lp * m_damp;
		c.buf[c.pos] = in - c.lp * c.fb;
		c.pos = (c.pos + 1) % c.buf.size();
		return out * c.gain;
	}
	double diffuse(line &a, double in) const
	{
		if (a.pos >= a.buf.size())
			a.pos = 0;
		const double held = a.buf[a.pos];
		const double w = held * m_ap_fb - in;
		a.buf[a.pos] = w;
		a.pos = (a.pos + 1) % a.buf.size();
		return held - w * m_ap_fb;
	}
	double tail(int c, double in)
	{
		line &t = m_tail[c];
		if (t.pos >= t.buf.size())
			t.pos = 0;
		double out = t.buf[t.pos];
		t.buf[t.pos] = in;
		t.pos = (t.pos + 1) % t.buf.size();
		out = diffuse(m_ap[c][0], out);
		return diffuse(m_ap[c][1], out);
	}
	double tap(int c, double delay) const
	{
		const double p = double(m_cpos) - delay + CHORUS_LEN;
		const int i0 = int(p) % CHORUS_LEN, i1 = (i0 + 1) % CHORUS_LEN;
		const double f = p - std::floor(p);
		return m_cbuf[c][i0] * (1.0 - f) + m_cbuf[c][i1] * f;
	}

	bool m_active = false, m_stereo = false;
	double m_out_mix = 0, m_ap_fb = 0, m_damp = 0, m_damp_state = 0, m_refl_in = 0, m_tail_ret = 0;
	line m_refl[6], m_tail[2], m_ap[2][2];
	double m_ch_delay = 1, m_ch_fb = 0, m_ch_depth = 0, m_ch_rate = 0, m_phase = 0;
	std::vector<double> m_cbuf[2];
	size_t m_cpos = 0;
	double m_rev_send[4]{}, m_cho_send[4]{}, m_dry[4][2]{};
};

class sound_float : public sound_base
{
public:
	// the level law of p2k_sound.h: dec((x - (x >> 8)) / 256) / dec(255), 0 at x = 0
	static double gain(uint16_t x) { return x ? hdec((x - (x >> 8)) / 256.0) / 262144.0 : 0.0; }

	void step(int16_t &out_l, int16_t &out_r) override
	{
		double dry[4][2]{};
		for (int v = 0; v < VOICES; v++)
		{
			const uint32_t inc = reg32(v, 0x04) >> 16;
			const uint32_t hblk = m_filter_on && chained(v) ? next_block(h_block(v)) : h_block(v), hb = hblk / 2;
			if (!inc)
				continue;
			const uint32_t a = m_addr[v];
			const double s0 = mem(a), s1 = mem(a + 1);
			double smp = s0 + (s1 - s0) * m_frac[v] / 4096.0;
			if (m_filter_on)
				smp = filter(v, smp);
			advance(v, inc);
			const uint8_t route = m_h[hb + 0x7a / 2] & 0x3f;
			if (route == 0x3f)
				continue;
			dry[sound::channel_bus(route >> 3)][0] += smp * gain(ip(hblk, IP_L).x);
			dry[sound::channel_bus(route)][1] += smp * gain(ip(hblk, IP_R).x);
		}
		double suml = 0, sumr = 0;
		if (m_fx_on)
		{
			if ((m_sample & 255) == 0)
				m_fx.configure(m_g05);
			double rin = 0, cin = 0;
			for (int b = 0; b < 4; b++)
			{
				const double mono = 0.5 * (dry[b][0] + dry[b][1]);
				rin += mono * m_fx.reverb_send(b);
				cin += mono * m_fx.chorus_send(b);
				suml += dry[b][0] * m_fx.dry(b, 0);
				sumr += dry[b][1] * m_fx.dry(b, 1);
			}
			m_fx.process(rin, cin, suml, sumr);
		}
		else
			for (int b = 0; b < 4; b++)
			{
				suml += dry[b][0];
				sumr += dry[b][1];
			}
		tick_interps();
		out_l = int16_t(std::clamp<double>(std::floor(suml * 0.5), -32768, 32767));
		out_r = int16_t(std::clamp<double>(std::floor(sumr * 0.5), -32768, 32767));
		m_last[0] = suml * 0.5;
		m_last[1] = sumr * 0.5;
		m_sample++;
	}
	double m_last[2]{};      // unquantised output of the last sample
	int64_t m_resets = 0;    // filter states reset after an overflow (|x| > 1e15)

protected:
	void on_restart(int v) override { std::memset(m_fst[v], 0, sizeof(m_fst[v])); }

private:

	static double hdec(double code)
	{
		const double c = code + 1.0;
		if (c >= 224.0)
			return (c - 192.0) * 4096.0;
		const int e = int(c) >> 4;
		const double m = c - 16.0 * e;
		return e ? (16.0 + m) * double(1 << (e - 1)) : m;
	}

	double filter(int v, double x)
	{
		const uint32_t blk = h_block(v);
		double y = stage(v, 0, blk, x);
		if (chained(v))
			y = stage(v, 1, next_block(blk), y);
		return y;
	}

	double stage(int v, int st, uint32_t blk, double x)
	{
		const double cut = ip(prev_block(blk), IP_C).x / 65536.0;
		const bool unnorm = m_h[(prev_block(blk) + 0x7a) / 2] & 0x80;
		const bool gain_mode = m_h[(blk + 0x7a) / 2] & 0x40;
		// the code is kept with 8 fraction bits, as in p2k_sound.h (and the patent's coefficient math unit)
		auto code = [&](uint16_t hi, uint16_t lo, int shift) {
			const int32_t a = (hi >> shift) & 0xff, b = (lo >> shift) & 0xff;
			return (((a << 16) + (b - a) * int32_t(cut * 65536.0) + 128) >> 8) / 256.0;
		};
		auto word = [&](int w, double &f2, double &b2) {
			const uint16_t hi = m_h[blk / 2 + 2 * w], lo = m_h[blk / 2 + 2 * w + 1];
			if (hi == 0xffcf && lo == 0xffcf)
				return false;
			f2 = hdec(code(hi, lo, 0)) / 65536.0;
			b2 = hdec(code(hi, lo, 8)) / 262144.0;
			return true;
		};
		static const int pole_w[3] = { 0, 4, 8 };
		const int zero_w[3] = { 1, 5, gain_mode ? -1 : 9 };
		for (int sct = 0; sct < 3; sct++)
		{
			double pf2 = 1, pb2 = 1, zf2 = 1, zb2 = 1;
			const bool p = word(pole_w[sct], pf2, pb2);
			const bool z = zero_w[sct] >= 0 && word(zero_w[sct], zf2, zb2);
			if (!p && !z)
				continue;
			const double g = (sct != 0 && unnorm) ? 1.0 : pf2;
			double *s = m_fst[v][st][sct];
			const double w = g * x + (2 - pf2 - pb2) * s[0] - (1 - pb2) * s[1];
			x = w - (2 - zf2 - zb2) * s[0] + (1 - zb2) * s[1];
			s[1] = s[0];
			s[0] = w;
		}
		x = std::clamp(x, -32767.0, 32767.0);      // 16-bit latch before the gain, as p2k_sound.h
		const uint16_t ghi = m_h[blk / 2 + 18], glo = m_h[blk / 2 + 19];
		if (gain_mode && !(ghi == 0xffcf && glo == 0xffcf))
			x *= hdec(code(ghi, glo, 0)) / hdec(254.0);
		if (!std::isfinite(x) || std::fabs(x) > 1e15)
		{
			m_resets++;
			std::memset(m_fst[v][st], 0, sizeof(m_fst[v][st]));
			x = 0;
		}
		return x;
	}

	fx_float m_fx;
	double m_fst[VOICES][2][3][2]{};
};

} // namespace p2k

#endif // P2K_SOUND_FLOAT_H
