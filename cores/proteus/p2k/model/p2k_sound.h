// license:BSD-3-Clause
// copyright-holders:emu-proteus-xr-MiSTer contributors
/*
    Proteus 2000 sound model: two G20 voice chips (128 voices), the H filter chips, the G05
    effects processor and the output mixer, in integer arithmetic.

    This file is the bit-exact definition a P2K sound core is checked against
    (docs/p2k-mame-plan.md, step 2). The MAME research machine (mame/patches 0004) and the
    standalone renderer (p2k_render.cpp) both use it. Where the register meaning comes from the
    firmware it is marked [O]; the arithmetic itself is a definition, not a measurement of the
    real chips [I] (no unit was available, 2026-11-23).

    Time base: one call of step() = one output sample at 44.1 kHz. Register writes take effect
    before the next sample; the log (set_log) records the sample index of every write, so a log
    replayed through the same writes gives the same output.

    Register windows (word offsets, MAME COMBINE_DATA semantics: new = (old & ~mask) | (data & mask)):
      G20  0x77F000-0x780FFF: 128 voices x 32 words
      H    0x500000-0x5FFFFF: 0x80000 words (filter coefficients, cutoff, flags, level bytes)
      G05  0x600000-0x60007F: 32 channels x 64 words, channel = the index written to +0x0E

    Number formats (s = signed, Qn = n fraction bits; rsh = shift right rounding half up):
      sample memory    s16 words; 26-bit word address; phase increment 4.12, fraction 12 bits
      interpolation    linear, Q8: (s0 << 8) + ((s1 - s0) * frac >> 4)
      filter           three direct-form-II biquads; signal and state Q8, saturated to +-(2^47 - 1);
                       coefficients Q28 (exact) from the Q8 code C0 + x (C255 - C0), x = the cutoff
                       interpolator: f^2 = dec(F) << 4, B2' = dec(X) << 2; products rsh 28;
                       clipped to 16 bits (+-32767 << 8) before the gain word
      interpolators    H level/cutoff interpolators (16-bit x, 8-bit destination, 32 << k samples)
      level gain       Q22: H code decode of (x - (x >> 8)) / 256, dec(code) >> 4 [I]
      mixer            Q8 per-bus sums of rsh(signal * gain, 22) by H output channel; effect input = bus
                       (L + R) / 2 times the A / B send; output = sum of bus * D (Q15) + effect returns
      effects          Q8 signals saturated to +-2^30, Q15 coefficients (fx_int)
      output           rsh(sum, 9) clamped to s16: Q8 -> samples, -6 dB headroom [I]
*/
#ifndef P2K_SOUND_H
#define P2K_SOUND_H

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

namespace p2k {

// ---------------------------------------------------------------- shared register state

class sound_base
{
public:
	static constexpr int VOICES = 128;
	static constexpr uint32_t H_WORDS = 0x80000;
	static constexpr int64_t CHAIN_WINDOW = 2205;      // 50 ms: 12th-order chaining window [I]

	sound_base() : m_h(H_WORDS, 0) {}
	virtual ~sound_base() = default;

	void set_memory(const uint16_t *mem, uint32_t words) { m_mem = mem; m_words = words; }
	void set_options(bool filter, bool fx) { m_filter_on = filter; m_fx_on = fx; }
	void set_log(FILE *f) { m_log = f; if (f) std::fprintf(f, "# p2k sndlog 1\n"); }

	// offset = word offset in the chip window
	void write_g20(uint32_t offset, uint16_t data, uint16_t mask)
	{
		offset &= 0xfff;
		if (m_log)
			std::fprintf(m_log, "%lld G %04x %04x %04x\n", (long long)m_sample, offset * 2, data, mask);
		const unsigned voice = offset >> 5, reg = (offset & 31) * 2;
		if (reg == 0x3e)
			data &= 0x03ff;     // boot test writes 0x8211, expects 0x0211 [O 0x020011F0; width I]
		uint16_t &r = m_g20[voice][offset & 31];
		r = (r & ~mask) | (data & mask);
		if (reg == 0x02)
			m_sel[voice] = (r >> 12) & 1;   // +0x00 bit 12: end select (step-1 findings 1.2) [I]
		if (reg == 0x1a)
			restart(voice);     // +0x18/+0x1A = start address; the low word is written last [O 0x0103282A]
	}

	void write_h(uint32_t offset, uint16_t data, uint16_t mask)
	{
		offset &= H_WORDS - 1;
		if (m_log)
			std::fprintf(m_log, "%lld H %05x %04x %04x\n", (long long)m_sample, offset * 2, data, mask);
		m_h[offset] = (m_h[offset] & ~mask) | (data & mask);
		const uint32_t blk = (offset * 2) & ~0x7fu, o = (offset * 2) & 0x7f;
		if (blk >= 0x8000)
			return;
		if (o < 0x28)
			m_coef_time[h_voice(blk)] = m_sample;
		interp *ip = m_ip[blk >> 7];
		const int page = blk >> 12;
		switch (o)
		{
		case 0x34: case 0x36: case 0x3e:       // current value x (written c * 0x0101 at note-on; read back live)
		{
			interp &i = ip[o == 0x34 ? IP_R : o == 0x36 ? IP_L : IP_C];
			i.x = (i.x & ~mask) | (data & mask);
			break;
		}
		case 0x30: case 0x32: case 0x3c:       // destination in the high byte (low byte written 0)
			if (mask & 0xff00)
				set_target(ip[o == 0x30 ? IP_R : o == 0x32 ? IP_L : IP_C], data >> 8, o == 0x3c ? m_k_cut[page] : m_k_level[page]);
			break;
		case 0x70: case 0x72:                  // +0x71 / +0x73: live right / left level destination
			if (mask & 0x00ff)
				set_target(ip[o == 0x70 ? IP_R : IP_L], data & 0xff, m_k_level[page]);
			break;
		case 0x7c:                             // +0x7C: live cutoff destination (for the next slot's filter)
			if (mask & 0xff00)
				set_target(ip[IP_C], data >> 8, m_k_cut[page]);
			break;
		case 0x44:                             // approach time index of the cutoff interpolators [I], page-global
			m_k_cut[page] = data & 7;
			break;
		case 0x46:                             // approach time index of the level interpolators [I], page-global
			m_k_level[page] = data & 7;
			break;
		}
	}

	void write_g05(unsigned index, unsigned offset, uint16_t data, uint16_t mask)
	{
		index &= 31;
		offset &= 63;
		if (m_log)
			std::fprintf(m_log, "%lld X %02x %02x %04x %04x\n", (long long)m_sample, index, offset * 2, data, mask);
		m_g05[index][offset] = (m_g05[index][offset] & ~mask) | (data & mask);
	}

	uint16_t g20(uint32_t offset) const { return m_g20[(offset >> 5) & 127][offset & 31]; }
	uint16_t h(uint32_t offset) const { return m_h[offset & (H_WORDS - 1)]; }
	// CPU reads: G20 +0x00 bit 12 is the live end select; H +0x34/+0x36/+0x3E return the live
	// interpolator value and +0x3A the +0x7A word (the OS reads +0x3B before changing +0x7B) [I]
	uint16_t g20_read(uint32_t offset) const
	{
		const unsigned v = (offset >> 5) & 127;
		uint16_t r = g20(offset);
		if ((offset & 31) == 1)
			r = (r & ~0x1000) | (m_sel[v] ? 0x1000 : 0);
		return r;
	}
	uint16_t h_read(uint32_t offset) const
	{
		offset &= H_WORDS - 1;
		const uint32_t blk = (offset * 2) & ~0x7fu, o = (offset * 2) & 0x7f;
		if (blk < 0x8000)
		{
			const interp *ip = m_ip[blk >> 7];
			if (o == 0x34 || o == 0x36 || o == 0x3e)
				return ip[o == 0x34 ? IP_R : o == 0x36 ? IP_L : IP_C].x;
			if (o == 0x3a)
				return m_h[(blk + 0x7a) / 2];
		}
		return m_h[offset];
	}
	uint16_t g05(unsigned index, unsigned offset) const { return m_g05[index & 31][offset & 63]; }
	uint32_t address(int v) const { return m_addr[v & 127]; }
	int64_t sample() const { return m_sample; }

	// H block of voice v (OS table 0x010331D0 [O]) and its inverse
	static uint32_t h_block(int v) { static const uint16_t page[4] = { 0x2000, 0, 0x3000, 0x1000 }; return 0x4000 * (v >> 6) + page[v & 3] + 0x80 * ((((v >> 2) & 15) + 15) & 15); }
	static int h_voice(uint32_t blk)
	{
		static const int lane[4] = { 1, 3, 0, 2 };
		const int slot = (((blk >> 7) & 15) + 1) & 15;
		return (blk >> 14) * 64 + slot * 4 + lane[(blk >> 12) & 3];
	}
	static uint32_t next_block(uint32_t blk) { return (blk & 0x780) == 0x780 ? blk - 0x780 : blk + 0x80; }
	static uint32_t prev_block(uint32_t blk) { return (blk & 0x7ff) == 0 ? blk + 0x780 : blk - 0x80; }

	// ---- H interpolators (US 5,170,369 FIGs. 9-12, as described by the E4 measurement project
	// github.com/ds10fan/emu-hchip-patent, docs/INTERPOLATOR.md; independent implementation).
	// Per H block: right level, left level, and the cutoff of the next slot's filter [O: register
	// offsets from the OS writes; I: that these are the patent's interpolators].
	enum { IP_R = 0, IP_L = 1, IP_C = 2 };
	struct interp { uint16_t x = 0; uint8_t dest = 0, inc = 0; };
	static uint16_t dup(uint8_t b) { return uint16_t(b * 0x0101); }
	// Table I: 8-bit increment code (sign, 3-bit shift, 4-bit data) -> signed value, -2048..1984
	static int32_t inc_decode(uint8_t c)
	{
		const int32_t f = (c >> 4) & 7, d = c & 15;
		if (!(c & 0x80))
			return f ? (16 + d) << (f - 1) : d;
		return f ? (d - 32) << (f - 1) : d - 16;
	}
	// largest Table I value not above v, saturating
	static uint8_t inc_encode(int32_t v)
	{
		v = std::clamp(v, -2048, 1984);
		if (v >= 0)
		{
			if (v < 16)
				return uint8_t(v);
			int f = 1;
			while ((v >> (f - 1)) >= 32)
				f++;
			return uint8_t((f << 4) | ((v >> (f - 1)) - 16));
		}
		if (v >= -16)
			return uint8_t(0x80 | (v + 16));
		int f = 1;
		while ((v >> (f - 1)) < -32)
			f++;
		return uint8_t(0x80 | (f << 4) | ((v >> (f - 1)) + 32));
	}
	// FIGs. 11/12: aim at dest in about 32 << k samples, increment from the current value
	static void set_target(interp &i, uint8_t dest, int k)
	{
		i.dest = dest;
		i.inc = inc_encode((int32_t(dup(dest)) - i.x) >> (5 + (k & 7)));
	}
	// FIG. 10: add the increment (+1 for positive codes), clamp at the destination or on overflow
	static void tick(interp &i)
	{
		const bool pos = !(i.inc & 0x80);
		const int32_t sum = i.x + inc_decode(i.inc) + (pos ? 1 : 0), clamp = dup(i.dest);
		i.x = uint16_t((sum < 0 || sum > 0xffff || (pos ? sum > clamp : sum < clamp)) ? clamp : sum);
	}
	const interp &ip(uint32_t blk, int n) const { return m_ip[(blk >> 7) & 255][n]; }

	virtual void step(int16_t &l, int16_t &r) = 0;
	void render(int16_t *out, int frames) { for (int i = 0; i < frames; i++) step(out[2 * i], out[2 * i + 1]); }

protected:
	uint32_t reg32(int v, int r) const { return (uint32_t(m_g20[v][r / 2]) << 16) | m_g20[v][r / 2 + 1]; }
	int32_t mem(uint32_t a) const { return a < m_words ? int16_t(m_mem[a]) : 0; }

	// [I] a 12th-order voice continues through the next slot when that slot's coefficients were written
	// within 50 ms before this voice started and the slot's own voice has not been started since
	bool chained(int v) const
	{
		const int nv = h_voice(next_block(h_block(v)));
		return m_coef_time[nv] + CHAIN_WINDOW >= m_start_time[v] && m_start_time[nv] < m_coef_time[nv];
	}

	// G20 address generator, one output sample [O: +0x04 increment 4.12, +0x0C/+0x08 NOT(end), +0x14
	// jump address; I (step-1 findings 1.2): +0x00 bit 12 selects the live end register (1: +0x0C,
	// 0: +0x08); at each end crossing the address jumps to +0x14[25:0] and bit 12 takes +0x14 bit 26.
	// Staying on the same end register is a loop (the overshoot wraps); changing it is a jump into
	// the other region (overshoot dropped)]
	void advance(int v, uint32_t inc)
	{
		const uint32_t a = m_addr[v];
		const uint32_t f = m_frac[v] + inc;
		uint32_t na = a + (f >> 12);
		m_frac[v] = f & 0xfff;
		const uint32_t le = ~reg32(v, m_sel[v] ? 0x0c : 0x08) & 0x3ffffff;
		if (na > le)
		{
			const uint32_t next = reg32(v, 0x14), ls = next & 0x3ffffff;
			const bool nsel = (next >> 26) & 1;
			const uint32_t over = na - le - 1;
			na = nsel == m_sel[v] ? ls + over % std::max<uint32_t>(1, le + 1 - ls) : ls;
			m_sel[v] = nsel;
		}
		m_addr[v] = na & 0x3ffffff;
	}

	void restart(int v)
	{
		m_addr[v] = reg32(v, 0x18) & 0x3ffffff;
		m_frac[v] = 0;
		m_start_time[v] = m_sample;
		m_sel[v] = true;    // [I] as after the note-on write +0x00 = 0x1800 that precedes every start
		on_restart(v);
	}
	virtual void on_restart(int v) = 0;

	const uint16_t *m_mem = nullptr;
	uint32_t m_words = 0;
	bool m_filter_on = true, m_fx_on = true;
	FILE *m_log = nullptr;
	int64_t m_sample = 0;

	uint16_t m_g20[VOICES][32]{};
	std::vector<uint16_t> m_h;
	uint16_t m_g05[32][64]{};
	uint32_t m_addr[VOICES]{}, m_frac[VOICES]{};
	bool m_sel[VOICES]{};                  // G20 +0x00 bit 12, end select
	int64_t m_start_time[VOICES]{}, m_coef_time[VOICES]{};
	interp m_ip[256][3]{};                 // per H block (0x80 bytes) of the 0x8000 used
	uint8_t m_k_level[8]{}, m_k_cut[8]{};  // approach time index per 0x1000 page (+0x46, +0x44)
	void tick_interps() { for (auto &b : m_ip) for (auto &i : b) tick(i); }
};

// ---------------------------------------------------------------- integer helpers

// arithmetic shift right with rounding (half up): (v + 2^(s-1)) >> s
inline int64_t rsh(int64_t v, int s) { return (v + (int64_t(1) << (s - 1))) >> s; }
inline int64_t rsh(__int128 v, int s) { return int64_t((v + (__int128(1) << (s - 1))) >> s); }
inline int64_t clamp64(int64_t v, int64_t lim) { return std::clamp<int64_t>(v, -lim, lim); }

// ---------------------------------------------------------------- G05 effects (integer)

/*
    Reverb and chorus driven by the EMU8000-style INIT arrays the OS writes (docs/p2k-findings.md).
    A workalike, not the G05 microcode: reverb = the slot meanings 86Box's EMU8000 model uses
    (independent implementation of that interpretation), chorus = the slots measured on the P2K.
    Signals Q8 sample units, saturated to +-2^30; coefficients Q15: byte / 256 = byte << 7,
    byte / 255 = (byte * 257) >> 1, 4-bit n / 15 = (n * 34953) >> 4. Products rounded (rsh).
    Reconfigured from the register file every 256 samples.
*/
class fx_int
{
public:
	static constexpr int32_t LIM = 1 << 30;

	fx_int()
	{
		static const int refl_len[6] = { 2, 4, 8, 13, 19, 26 };
		for (int i = 0; i < 6; i++)
			resize(m_refl[i].buf, refl_len[i] * STEP);
		for (int c = 0; c < 2; c++)
		{
			resize(m_ap[c][0].buf, 2 * 4 * STEP + 55);
			resize(m_ap[c][1].buf, 1 * 4 * STEP + 55);
			resize(m_tail[c].buf, 27 * STEP);
			m_cbuf[c].assign(CHORUS_LEN, 0);
		}
	}

	void configure(const uint16_t (*regs)[64])
	{
		auto init = [&](int n, int ch) -> int32_t { static const int off[4] = { 0x28, 0x2a, 0x38, 0x3a }; return regs[ch][off[n - 1] / 2]; };
		auto dword = [&](int ch, int lo, int hi) -> uint32_t { return (uint32_t(regs[ch][hi / 2]) << 16) | regs[ch][lo / 2]; };
		m_active = init(1, 0) != 0x03ff;                  // the boot init sets still in place
		m_out_mix = q256(init(1, 3) & 0xff);
		m_ap_fb = q255(init(1, 5) & 0xff);
		m_stereo = init(1, 7) == 0x8474;
		static const int gain_slot[6][2] = { { 1, 0x0f }, { 1, 0x17 }, { 1, 0x1f }, { 2, 0x07 }, { 2, 0x0f }, { 2, 0x17 } };
		static const int fb_slot[6][2] = { { 1, 0x09 }, { 1, 0x11 }, { 1, 0x19 }, { 2, 0x01 }, { 2, 0x09 }, { 2, 0x11 } };
		for (int i = 0; i < 6; i++)
		{
			m_refl[i].gain = q15((init(gain_slot[i][0], gain_slot[i][1]) >> 4) & 0xf);
			m_refl[i].fb = q15(init(fb_slot[i][0], fb_slot[i][1]) & 0xf);
		}
		m_damp = q255(init(2, 0x1d) & 0xff);
		m_refl_in = q256(init(3, 1) & 0xff);
		m_tail_ret = q256(init(4, 0x1f) & 0xff);
		const int tl = ((init(2, 0x14) >> 8) & 0xf) + 18;
		resize(m_refl[5].buf, tl * STEP);
		resize(m_tail[0].buf, (tl + 1) * STEP);
		const int tr = m_stereo ? ((init(2, 0x16) >> 8) & 0xf) + 18 : tl;
		resize(m_tail[1].buf, (tr + 1) * STEP);
		m_ch_delay = std::clamp<int32_t>(init(3, 0x1a), 1, CHORUS_LEN - 4096);
		m_ch_fb = q256(init(3, 0x17) & 0xff);
		m_ch_depth = int64_t(init(4, 0x0b) & 0xff) * m_ch_delay << 8;      // Q16 samples: byte * delay / 256
		m_ch_inc = uint32_t((uint64_t(regs[0x0a][0x1a / 2]) * 99731) >> 10);   // milli-Hz -> 2^32 / 44100 [I]
		for (int b = 0; b < 4; b++)
		{
			const uint32_t ptrx = dword(4 * b, 0x16, 0x14), csl = dword(4 * b, 0x76, 0x74);
			m_rev_send[b] = q255((ptrx >> 8) & 0xff);
			m_cho_send[b] = q255((csl >> 24) & 0xff);
			// dry level D of the bus into the stereo output [O: step-1 findings 3.1; I: dry, linear]: left
			// = PSST bits 31-24 of channel 4b, right = PTRX bits 7-0 of channel 4b + 0x10. Sub 1 and Sub 2
			// (D forced 0 by the OS: separate jacks) reach the mixdown directly at unity [I]
			const uint32_t psst = dword(4 * b, 0x66, 0x64), ptrx_r = dword(4 * b + 0x10, 0x16, 0x14);
			m_dry[b][0] = (b == 1 || b == 2) ? 32768 : q255(psst >> 24);
			m_dry[b][1] = (b == 1 || b == 2) ? 32768 : q255(ptrx_r & 0xff);
		}
	}

	int32_t reverb_send(int bus) const { return m_rev_send[bus & 3]; }   // Q15
	int32_t dry(int bus, int side) const { return m_dry[bus & 3][side & 1]; }   // Q15
	int32_t chorus_send(int bus) const { return m_cho_send[bus & 3]; }

	// one sample: rev_in / cho_in Q8; adds the wet outputs (Q8) to l / r
	void process(int32_t rev_in, int32_t cho_in, int64_t &l, int64_t &r)
	{
		if (!m_active)
			return;
		m_damp_state = sat(rsh(int64_t(rev_in) * (32768 - m_damp) + int64_t(m_damp_state) * m_damp, 15));
		const int32_t in = m_damp_state;
		const int32_t in2 = mul(in, m_refl_in);
		int32_t o[6];
		for (int i = 0; i < 6; i++)
			o[i] = comb(m_refl[i], in2);
		int32_t dl, dr;
		if (m_stereo)
		{
			dl = sat(int64_t(o[2]) + o[4]);
			dr = sat(int64_t(o[0]) + o[1] + o[3] + o[5]);
		}
		else
			dl = dr = sat(int64_t(o[0]) + o[1] + o[2] + o[3] + o[4] + o[5]);
		dl = sat(int64_t(dl) + mul(tail(0, sat(int64_t(in) + dl)), m_tail_ret));
		dr = sat(int64_t(dr) + mul(tail(1, sat(int64_t(in) + dr)), m_tail_ret));
		l += mul(dl, m_out_mix);
		r += mul(dr, m_out_mix);

		const int64_t lfo = rsh(lfo_sine(m_phase) * m_ch_depth, 16);
		const int64_t lfo_r = rsh(lfo_sine(m_phase + 0x40000000u) * m_ch_depth, 16);   // [I] quadrature right tap
		m_phase += m_ch_inc;
		const int32_t cl = tap(0, (int64_t(m_ch_delay) << 16) + lfo), cr = tap(1, (int64_t(m_ch_delay) << 16) + lfo_r);
		m_cbuf[0][m_cpos] = sat(int64_t(cho_in) + mul(cl, m_ch_fb));
		m_cbuf[1][m_cpos] = sat(int64_t(cho_in) + mul(cr, m_ch_fb));
		m_cpos = (m_cpos + 1) & (CHORUS_LEN - 1);
		l += cl;
		r += cr;
	}

	// quarter-wave table, 1025 entries, amplitude 65535 (Q16)
	static int32_t sine(uint32_t i)   // i = 12-bit phase
	{
		static const uint16_t q[1025] = {
			0, 101, 201, 302, 402, 503, 603, 704, 804, 905, 1005, 1106, 1206, 1307, 1407, 1508,
			1608, 1709, 1809, 1910, 2010, 2111, 2211, 2312, 2412, 2513, 2613, 2714, 2814, 2914, 3015, 3115,
			3216, 3316, 3416, 3517, 3617, 3718, 3818, 3918, 4019, 4119, 4219, 4320, 4420, 4520, 4621, 4721,
			4821, 4921, 5022, 5122, 5222, 5322, 5422, 5523, 5623, 5723, 5823, 5923, 6023, 6123, 6223, 6324,
			6424, 6524, 6624, 6724, 6824, 6924, 7024, 7123, 7223, 7323, 7423, 7523, 7623, 7723, 7823, 7922,
			8022, 8122, 8222, 8321, 8421, 8521, 8620, 8720, 8820, 8919, 9019, 9118, 9218, 9318, 9417, 9517,
			9616, 9715, 9815, 9914, 10014, 10113, 10212, 10312, 10411, 10510, 10609, 10708, 10808, 10907, 11006, 11105,
			11204, 11303, 11402, 11501, 11600, 11699, 11798, 11897, 11996, 12094, 12193, 12292, 12391, 12489, 12588, 12687,
			12785, 12884, 12982, 13081, 13179, 13278, 13376, 13475, 13573, 13671, 13770, 13868, 13966, 14064, 14163, 14261,
			14359, 14457, 14555, 14653, 14751, 14849, 14947, 15045, 15142, 15240, 15338, 15436, 15533, 15631, 15729, 15826,
			15924, 16021, 16119, 16216, 16313, 16411, 16508, 16605, 16703, 16800, 16897, 16994, 17091, 17188, 17285, 17382,
			17479, 17576, 17673, 17769, 17866, 17963, 18060, 18156, 18253, 18349, 18446, 18542, 18639, 18735, 18831, 18928,
			19024, 19120, 19216, 19312, 19408, 19504, 19600, 19696, 19792, 19888, 19984, 20079, 20175, 20271, 20366, 20462,
			20557, 20653, 20748, 20843, 20939, 21034, 21129, 21224, 21319, 21414, 21509, 21604, 21699, 21794, 21889, 21983,
			22078, 22173, 22267, 22362, 22456, 22551, 22645, 22739, 22834, 22928, 23022, 23116, 23210, 23304, 23398, 23492,
			23586, 23679, 23773, 23867, 23960, 24054, 24147, 24241, 24334, 24428, 24521, 24614, 24707, 24800, 24893, 24986,
			25079, 25172, 25265, 25358, 25450, 25543, 25635, 25728, 25820, 25913, 26005, 26097, 26189, 26281, 26374, 26466,
			26557, 26649, 26741, 26833, 26925, 27016, 27108, 27199, 27291, 27382, 27473, 27565, 27656, 27747, 27838, 27929,
			28020, 28111, 28201, 28292, 28383, 28473, 28564, 28654, 28745, 28835, 28925, 29015, 29106, 29196, 29286, 29375,
			29465, 29555, 29645, 29734, 29824, 29913, 30003, 30092, 30181, 30271, 30360, 30449, 30538, 30627, 30716, 30804,
			30893, 30982, 31070, 31159, 31247, 31335, 31424, 31512, 31600, 31688, 31776, 31864, 31952, 32039, 32127, 32215,
			32302, 32390, 32477, 32564, 32651, 32738, 32826, 32912, 32999, 33086, 33173, 33260, 33346, 33433, 33519, 33605,
			33692, 33778, 33864, 33950, 34036, 34122, 34208, 34293, 34379, 34465, 34550, 34635, 34721, 34806, 34891, 34976,
			35061, 35146, 35231, 35315, 35400, 35485, 35569, 35654, 35738, 35822, 35906, 35990, 36074, 36158, 36242, 36326,
			36409, 36493, 36576, 36660, 36743, 36826, 36909, 36992, 37075, 37158, 37241, 37324, 37406, 37489, 37571, 37653,
			37736, 37818, 37900, 37982, 38064, 38145, 38227, 38309, 38390, 38472, 38553, 38634, 38715, 38797, 38877, 38958,
			39039, 39120, 39200, 39281, 39361, 39442, 39522, 39602, 39682, 39762, 39842, 39922, 40001, 40081, 40161, 40240,
			40319, 40398, 40478, 40557, 40635, 40714, 40793, 40872, 40950, 41029, 41107, 41185, 41263, 41341, 41419, 41497,
			41575, 41653, 41730, 41808, 41885, 41962, 42039, 42117, 42194, 42270, 42347, 42424, 42500, 42577, 42653, 42730,
			42806, 42882, 42958, 43034, 43109, 43185, 43261, 43336, 43411, 43487, 43562, 43637, 43712, 43787, 43861, 43936,
			44011, 44085, 44159, 44234, 44308, 44382, 44456, 44529, 44603, 44677, 44750, 44824, 44897, 44970, 45043, 45116,
			45189, 45262, 45334, 45407, 45479, 45552, 45624, 45696, 45768, 45840, 45912, 45983, 46055, 46126, 46198, 46269,
			46340, 46411, 46482, 46553, 46624, 46694, 46765, 46835, 46905, 46976, 47046, 47116, 47185, 47255, 47325, 47394,
			47464, 47533, 47602, 47671, 47740, 47809, 47877, 47946, 48014, 48083, 48151, 48219, 48287, 48355, 48423, 48491,
			48558, 48626, 48693, 48760, 48827, 48894, 48961, 49028, 49095, 49161, 49228, 49294, 49360, 49426, 49492, 49558,
			49624, 49689, 49755, 49820, 49885, 49951, 50016, 50080, 50145, 50210, 50274, 50339, 50403, 50467, 50531, 50595,
			50659, 50723, 50787, 50850, 50913, 50977, 51040, 51103, 51166, 51228, 51291, 51354, 51416, 51478, 51540, 51602,
			51664, 51726, 51788, 51849, 51911, 51972, 52033, 52094, 52155, 52216, 52277, 52337, 52398, 52458, 52518, 52578,
			52638, 52698, 52758, 52817, 52877, 52936, 52995, 53054, 53113, 53172, 53231, 53289, 53348, 53406, 53464, 53522,
			53580, 53638, 53696, 53754, 53811, 53868, 53925, 53983, 54039, 54096, 54153, 54210, 54266, 54322, 54378, 54434,
			54490, 54546, 54602, 54657, 54713, 54768, 54823, 54878, 54933, 54988, 55042, 55097, 55151, 55206, 55260, 55314,
			55367, 55421, 55475, 55528, 55582, 55635, 55688, 55741, 55794, 55846, 55899, 55951, 56003, 56056, 56108, 56159,
			56211, 56263, 56314, 56366, 56417, 56468, 56519, 56570, 56620, 56671, 56721, 56772, 56822, 56872, 56922, 56972,
			57021, 57071, 57120, 57169, 57218, 57267, 57316, 57365, 57413, 57462, 57510, 57558, 57606, 57654, 57702, 57749,
			57797, 57844, 57891, 57938, 57985, 58032, 58079, 58125, 58171, 58218, 58264, 58310, 58356, 58401, 58447, 58492,
			58537, 58583, 58628, 58672, 58717, 58762, 58806, 58850, 58895, 58939, 58983, 59026, 59070, 59113, 59157, 59200,
			59243, 59286, 59329, 59371, 59414, 59456, 59498, 59540, 59582, 59624, 59666, 59707, 59749, 59790, 59831, 59872,
			59913, 59953, 59994, 60034, 60075, 60115, 60155, 60194, 60234, 60274, 60313, 60352, 60391, 60430, 60469, 60508,
			60546, 60585, 60623, 60661, 60699, 60737, 60775, 60812, 60850, 60887, 60924, 60961, 60998, 61034, 61071, 61107,
			61144, 61180, 61216, 61252, 61287, 61323, 61358, 61393, 61429, 61463, 61498, 61533, 61567, 61602, 61636, 61670,
			61704, 61738, 61772, 61805, 61838, 61872, 61905, 61938, 61970, 62003, 62035, 62068, 62100, 62132, 62164, 62196,
			62227, 62259, 62290, 62321, 62352, 62383, 62414, 62444, 62475, 62505, 62535, 62565, 62595, 62625, 62654, 62684,
			62713, 62742, 62771, 62800, 62829, 62857, 62886, 62914, 62942, 62970, 62998, 63025, 63053, 63080, 63107, 63134,
			63161, 63188, 63214, 63241, 63267, 63293, 63319, 63345, 63371, 63396, 63422, 63447, 63472, 63497, 63522, 63546,
			63571, 63595, 63620, 63644, 63668, 63691, 63715, 63738, 63762, 63785, 63808, 63831, 63853, 63876, 63898, 63921,
			63943, 63965, 63986, 64008, 64030, 64051, 64072, 64093, 64114, 64135, 64155, 64176, 64196, 64216, 64236, 64256,
			64276, 64295, 64315, 64334, 64353, 64372, 64391, 64409, 64428, 64446, 64464, 64482, 64500, 64518, 64535, 64553,
			64570, 64587, 64604, 64621, 64638, 64654, 64671, 64687, 64703, 64719, 64734, 64750, 64765, 64781, 64796, 64811,
			64826, 64840, 64855, 64869, 64883, 64898, 64911, 64925, 64939, 64952, 64966, 64979, 64992, 65005, 65017, 65030,
			65042, 65054, 65066, 65078, 65090, 65102, 65113, 65125, 65136, 65147, 65158, 65168, 65179, 65189, 65199, 65210,
			65219, 65229, 65239, 65248, 65258, 65267, 65276, 65285, 65293, 65302, 65310, 65319, 65327, 65335, 65342, 65350,
			65357, 65365, 65372, 65379, 65386, 65392, 65399, 65405, 65412, 65418, 65424, 65429, 65435, 65441, 65446, 65451,
			65456, 65461, 65466, 65470, 65475, 65479, 65483, 65487, 65491, 65494, 65498, 65501, 65504, 65507, 65510, 65513,
			65515, 65518, 65520, 65522, 65524, 65526, 65527, 65529, 65530, 65531, 65532, 65533, 65534, 65534, 65535, 65535,
			65535,
		};
		const uint32_t k = i & 1023;
		switch ((i >> 10) & 3)
		{
		case 0: return q[k];
		case 1: return q[1024 - k];
		case 2: return -int32_t(q[k]);
		default: return -int32_t(q[1024 - k]);
		}
	}
	// 32-bit phase: 12-bit table index, 10-bit linear interpolation; Q16
	static int64_t lfo_sine(uint32_t phase)
	{
		const uint32_t i = phase >> 20;
		const int32_t a = sine(i), b = sine((i + 1) & 4095), f = (phase >> 10) & 0x3ff;
		return a + rsh(int64_t(b - a) * f, 10);
	}

private:
	static constexpr int STEP = 242;
	static constexpr int CHORUS_LEN = 0x8000;
	struct line { std::vector<int32_t> buf; size_t pos = 0; int32_t lp = 0, gain = 0, fb = 0; };

	static int32_t sat(int64_t v) { return int32_t(std::clamp<int64_t>(v, -LIM, LIM - 1)); }
	static int32_t mul(int32_t x, int32_t c) { return sat(rsh(int64_t(x) * c, 15)); }
	static int32_t q256(uint32_t b) { return int32_t(b << 7); }
	static int32_t q255(uint32_t b) { return int32_t((b * 257) >> 1); }
	static int32_t q15(uint32_t n) { return int32_t((n * 34953) >> 4); }
	static void resize(std::vector<int32_t> &b, size_t n) { if (b.size() != n) b.assign(n, 0); }

	int32_t comb(line &c, int32_t in)
	{
		if (c.pos >= c.buf.size())
			c.pos = 0;
		const int32_t out = c.buf[c.pos];
		c.lp = sat(rsh(int64_t(out) * (32768 - m_damp) + int64_t(c.lp) * m_damp, 15));
		c.buf[c.pos] = sat(int64_t(in) - mul(c.lp, c.fb));
		c.pos = (c.pos + 1) % c.buf.size();
		return mul(out, c.gain);
	}

	int32_t diffuse(line &a, int32_t in) const
	{
		if (a.pos >= a.buf.size())
			a.pos = 0;
		const int32_t held = a.buf[a.pos];
		const int32_t w = sat(int64_t(mul(held, m_ap_fb)) - in);
		a.buf[a.pos] = w;
		a.pos = (a.pos + 1) % a.buf.size();
		return sat(int64_t(held) - mul(w, m_ap_fb));
	}

	int32_t tail(int c, int32_t in)
	{
		line &t = m_tail[c];
		if (t.pos >= t.buf.size())
			t.pos = 0;
		int32_t out = t.buf[t.pos];
		t.buf[t.pos] = in;
		t.pos = (t.pos + 1) % t.buf.size();
		out = diffuse(m_ap[c][0], out);
		return diffuse(m_ap[c][1], out);
	}

	// fractional delay tap, delay in Q16 samples, linear interpolation
	int32_t tap(int c, int64_t delay_q16) const
	{
		const int64_t p = (int64_t(m_cpos) << 16) - delay_q16 + (int64_t(CHORUS_LEN) << 16);
		const int32_t i0 = int32_t(p >> 16) & (CHORUS_LEN - 1), i1 = (i0 + 1) & (CHORUS_LEN - 1);
		const int64_t f = p & 0xffff;
		return sat(rsh(int64_t(m_cbuf[c][i0]) * (65536 - f) + int64_t(m_cbuf[c][i1]) * f, 16));
	}

	bool m_active = false, m_stereo = false;
	int32_t m_out_mix = 0, m_ap_fb = 0, m_damp = 0, m_damp_state = 0, m_refl_in = 0, m_tail_ret = 0;
	line m_refl[6], m_tail[2], m_ap[2][2];
	int32_t m_ch_delay = 1, m_ch_fb = 0;
	int64_t m_ch_depth = 0;
	uint32_t m_ch_inc = 0, m_phase = 0;
	std::vector<int32_t> m_cbuf[2];
	uint32_t m_cpos = 0;
	int32_t m_rev_send[4]{}, m_cho_send[4]{}, m_dry[4][2]{};
};

// ---------------------------------------------------------------- voices, filter, mixer (integer)

class sound : public sound_base
{
public:
	static constexpr int64_t SIG_LIM = (int64_t(1) << 47) - 1;    // filter state, Q8
	static constexpr int64_t OUT_LIM = (int64_t(1) << 31) - 1;    // voice signal after the filter, Q8

	// H code (8.8: one byte interpolated by cutoff) -> f^2 in Q24. dec(c) of the E4 measurement
	// (c+1 >= 224 ? (c+1-192) << 12 : e ? (16 + m) << (e - 1) : m; e, m = (c+1) >> 4, & 15) times 256
	static int32_t hdec(int32_t code_q8)
	{
		const int32_t c = code_q8 + 256;
		if (c >= 224 * 256)
			return (c - 192 * 256) << 12;
		const int32_t e = c >> 12, m = c & 0xfff;
		return e ? (4096 + m) << (e - 1) : m;
	}

	// level interpolator x -> Q22 gain by the H code decode [I: step-1 findings 2.1, the OS builds level
	// bytes with the inverse of this decode]: gain = dec(code) / dec(255), code = (x - (x >> 8)) / 256
	// (exactly the byte b at x = b * 0x0101, piecewise linear between); x = 0 is off
	static int64_t interp_gain(uint16_t x)
	{
		return x ? hdec(x - (x >> 8)) >> 4 : 0;
	}

	// H output channel (3 bits) -> bus: route pairs {6,2} Main, {7,3} Sub 1, {0,4} Sub 2, {1,5} bus 4
	// [O: OS route table 0x01001F54, step-1 findings 2.4]
	static int channel_bus(int c) { static const int b[8] = { 2, 3, 0, 1, 2, 3, 0, 1 }; return b[c & 7]; }

	void step(int16_t &out_l, int16_t &out_r) override
	{
		int64_t dry[4][2]{};                          // per bus, left / right, Q8
		for (int v = 0; v < VOICES; v++)
		{
			const uint32_t inc = reg32(v, 0x04) >> 16;
			const uint32_t hblk = m_filter_on && chained(v) ? next_block(h_block(v)) : h_block(v), hb = hblk / 2;
			if (!inc)
				continue;
			const uint32_t a = m_addr[v];
			const int64_t s0 = mem(a), s1 = mem(a + 1);
			int64_t y = (s0 << 8) + (((s1 - s0) * m_frac[v]) >> 4);   // linear interpolation, Q8
			if (m_filter_on)
				y = filter(v, y);
			advance(v, inc);
			// routing byte of the chain-end block +0x7B bits 5-3 / 2-0: left / right H output channel;
			// 0x3F = not output (first block of a 12th-order chain) [O 0x01033434]
			const uint8_t route = m_h[hb + 0x7a / 2] & 0x3f;
			if (route == 0x3f)
				continue;
			// level interpolators of the chain-end block [O: +0x30-0x37, +0x71/+0x73]
			const int64_t gl = interp_gain(ip(hblk, IP_L).x), gr = interp_gain(ip(hblk, IP_R).x);   // Q22
			dry[channel_bus(route >> 3)][0] += rsh(y * gl, 22);
			dry[channel_bus(route)][1] += rsh(y * gr, 22);
		}
		int64_t suml = 0, sumr = 0;
		if (m_fx_on)
		{
			if ((m_sample & 255) == 0)
				m_fx.configure(m_g05);
			int64_t rin = 0, cin = 0;
			for (int b = 0; b < 4; b++)
			{
				const int64_t mono = rsh(dry[b][0] + dry[b][1], 1);
				rin += mono * m_fx.reverb_send(b);
				cin += mono * m_fx.chorus_send(b);
				suml += rsh(dry[b][0] * m_fx.dry(b, 0), 15);
				sumr += rsh(dry[b][1] * m_fx.dry(b, 1), 15);
			}
			m_fx.process(int32_t(clamp64(rsh(rin, 15), fx_int::LIM - 1)), int32_t(clamp64(rsh(cin, 15), fx_int::LIM - 1)), suml, sumr);
		}
		else
			for (int b = 0; b < 4; b++)
			{
				suml += dry[b][0];
				sumr += dry[b][1];
			}
		tick_interps();
		// Main + Sub 1 + Sub 2 (+ bus 4) mixed to one stereo pair; -6 dB headroom [I]
		out_l = int16_t(std::clamp<int64_t>(rsh(suml, 9), -32768, 32767));
		out_r = int16_t(std::clamp<int64_t>(rsh(sumr, 9), -32768, 32767));
		m_sample++;
	}

	// statistics for the renderer: peak |filter state| (Q8) and saturation count
	int64_t m_peak_state = 0, m_sat_count = 0;

protected:
	void on_restart(int v) override { std::memset(m_fst[v], 0, sizeof(m_fst[v])); }

private:

	int64_t sat(int64_t v)
	{
		if (v > SIG_LIM || v < -SIG_LIM)
		{
			m_sat_count++;
			return v > 0 ? SIG_LIM : -SIG_LIM;
		}
		return v;
	}

	int64_t filter(int v, int64_t x)
	{
		const uint32_t blk = h_block(v);
		int64_t y = stage(v, 0, blk, x);
		if (chained(v))
			y = stage(v, 1, next_block(blk), y);
		return clamp64(y, OUT_LIM);
	}

	// One H voice: f0^2/D0 * N1 * N5 * P4 * P8 * g9 (E4 measurement, docs/p2k-findings.md), computed as the
	// patent's datapath does it (ds10fan/emu-hchip-patent docs/VOICE.md): three biquad sections in series,
	// each a pole word and a zero word in direct form II sharing one delay line:
	//   section 0: pole word 0 (normalised f^2/D), zero word 1
	//   section 1: pole word 4 (normalised, or 1/D when the flags byte has bit 7), zero word 5
	//   section 2: pole word 8 (as word 4), then the gain word 9 (gain-word mode always on [I])
	//   w = g x + (2 - f^2 - B2') w1 - (1 - B2') w2;  y = w - (2 - f^2z - B2'z) w1 + (1 - B2'z) w2
	// The neutral word FFCFFFCF is 1 in every role; a section with both words neutral is skipped.
	int64_t stage(int v, int st, uint32_t blk, int64_t x)
	{
		// cutoff = the interpolator in the previous slot's block [O: +0x3C/+0x3E/+0x7C written there]
		const int32_t t = ip(prev_block(blk), IP_C).x;
		// flags [O 0x01033434]: bit 7 of the previous block's +0x7B = unnormalised poles (words 4, 8);
		// bit 6 of this block's +0x7B = gain-word mode (word 9 = gain, else a third zero word)
		const bool unnorm = m_h[(prev_block(blk) + 0x7a) / 2] & 0x80;
		const bool gain_mode = m_h[(blk + 0x7a) / 2] & 0x40;
		auto code = [t](uint16_t hi, uint16_t lo, int shift) {     // Q8 code: C0 + x (C255 - C0), rounded
			const int32_t a = (hi >> shift) & 0xff, b = (lo >> shift) & 0xff;
			return ((a << 16) + (b - a) * t + 128) >> 8;
		};
		auto word = [&](int w, int64_t &f2, int64_t &b2) {       // Q28; false for the neutral word
			const uint16_t hi = m_h[blk / 2 + 2 * w], lo = m_h[blk / 2 + 2 * w + 1];
			if (hi == 0xffcf && lo == 0xffcf)
				return false;
			f2 = int64_t(hdec(code(hi, lo, 0))) << 4;
			b2 = int64_t(hdec(code(hi, lo, 8))) << 2;
			return true;
		};
		static const int pole_w[3] = { 0, 4, 8 };
		const int zero_w[3] = { 1, 5, gain_mode ? -1 : 9 };
		for (int sct = 0; sct < 3; sct++)
		{
			int64_t pf2 = ONE, pb2 = ONE, zf2 = ONE, zb2 = ONE;
			const bool p = word(pole_w[sct], pf2, pb2);
			const bool z = zero_w[sct] >= 0 && word(zero_w[sct], zf2, zb2);
			if (!p && !z)
				continue;
			const int64_t g = (sct != 0 && unnorm) ? ONE : pf2;
			const int64_t a1 = 2 * ONE - pf2 - pb2, a2 = ONE - pb2;     // denominator
			const int64_t n1 = 2 * ONE - zf2 - zb2, n2 = ONE - zb2;     // numerator (0, 0 when neutral)
			int64_t *s = m_fst[v][st][sct];
			const int64_t w = sat(rsh(__int128(g) * x + __int128(a1) * s[0] - __int128(a2) * s[1], 28));
			x = sat(rsh((__int128(w) << 28) - __int128(n1) * s[0] + __int128(n2) * s[1], 28));
			s[1] = s[0];
			s[0] = w;
			const int64_t m = w < 0 ? -w : w;
			if (m > m_peak_state)
				m_peak_state = m;
		}
		// the voice output passes a 16-bit latch before the gain [I: ds10fan/emu-hchip-patent docs/VOICE.md]
		x = clamp64(x, int64_t(32767) << 8);
		const uint16_t ghi = m_h[blk / 2 + 18], glo = m_h[blk / 2 + 19];
		if (gain_mode && !(ghi == 0xffcf && glo == 0xffcf))
		{
			const int64_t g = (int64_t(hdec(code(ghi, glo, 0))) * 16644) >> 12;   // dec / dec(0xFE), Q28
			x = sat(rsh(__int128(x) * g, 28));
		}
		return x;
	}

	fx_int m_fx;
	static constexpr int64_t ONE = int64_t(1) << 28;
	int64_t m_fst[VOICES][2][3][2]{};  // [voice][stage][section][w1, w2], Q8
};

} // namespace p2k

#endif // P2K_SOUND_H
