// license:BSD-3-Clause
// copyright-holders:emu-proteus-xr-MiSTer contributors
// Unit checks of p2k_sound.h against documented behaviour: the H interpolator (US 5,170,369 Table I,
// FIGs. 10-12 as measured/derived in github.com/ds10fan/emu-hchip-patent docs/INTERPOLATOR.md) and the
// coefficient decode table (COEF_CODES decode in VOICE_INTERFACE.md).
#include "p2k_sound.h"
#include "p2k_flash.h"
#include <cstdio>
#include <vector>

using p2k::sound_base;
static int fails = 0;
#define CHECK(c, ...) do { if (!(c)) { fails++; std::printf("FAIL %s:%d: ", __FILE__, __LINE__); std::printf(__VA_ARGS__); std::printf("\n"); } } while (0)

static int sweep(uint8_t from, uint8_t to, int k)
{
	sound_base::interp i;
	i.x = sound_base::dup(from);
	sound_base::set_target(i, to, k);
	int n = 0;
	while (i.x != sound_base::dup(to) && n < 100000)
	{
		sound_base::tick(i);
		n++;
	}
	return n;
}

int main()
{
	// Table I round trip and lattice floor
	for (int c = 0; c < 256; c++)
		CHECK(sound_base::inc_encode(sound_base::inc_decode(uint8_t(c))) == c, "code %02x", c);
	for (int v = -2048; v <= 1984; v++)
	{
		const int d = sound_base::inc_decode(sound_base::inc_encode(v));
		CHECK(d <= v, "floor %d -> %d", v, d);
	}
	CHECK(sound_base::inc_decode(0x7f) == 1984 && sound_base::inc_decode(0xf0) == -2048 && sound_base::inc_decode(0x8f) == -1 &&
			sound_base::inc_decode(0x90) == -32, "Table I end points");
	// documented sweep times: 0->255 at k=0 34 samples, k=5 1041 samples; 0x10->0x11 at k=7 257 samples
	CHECK(sweep(0, 255, 0) == 34, "0->255 k0: %d", sweep(0, 255, 0));
	CHECK(sweep(0, 255, 5) == 1041, "0->255 k5: %d", sweep(0, 255, 5));
	CHECK(sweep(0x10, 0x11, 7) == 257, "0x10->0x11 k7: %d", sweep(0x10, 0x11, 7));
	CHECK(sweep(255, 0, 0) > 30 && sweep(255, 0, 0) < 40, "255->0 k0: %d", sweep(255, 0, 0));
	// coefficient decode at integer codes: dec(c) * 256 (Q24 of f^2 = dec / 2^16)
	auto dec = [](unsigned code) { unsigned c = code + 1, e = c >> 4, m = c & 15; return c >= 224 ? (c - 192) << 12 : e ? (16 + m) << (e - 1) : m; };
	for (int c = 0; c < 256; c++)
		CHECK(p2k::sound::hdec(c << 8) == int32_t(dec(c) * 256), "hdec %d", c);
	// level gain: 255 = 1.0 (Q22), 0 = off, x = b * 0x0101 gives the table value of b
	// level gain: x = 0xFFFF (byte 255) = 1.0 in Q22, x = 0 off, byte b * 0x0101 = dec(b) / dec(255)
	CHECK(p2k::sound::interp_gain(0xffff) == 1 << 22 && p2k::sound::interp_gain(0) == 0, "gain ends");
	for (int b = 1; b < 256; b++)
		CHECK(p2k::sound::interp_gain(uint16_t(b * 0x0101)) == int64_t(dec(b)) * 16, "gain %d", b);
	// flash command subset (p2k_flash.h), the sequences OS 2.26 sends (docs/p2k-findings.md)
	{
		std::vector<uint16_t> mem(1 << 16, 0xffff);
		p2k::intel_flash f;
		f.configure(mem.data(), uint32_t(mem.size()), 0x0089, 0x0015, 1 << 12, true);
		f.write(0x5555, 0xaaaa); f.write(0x2aaa, 0x5555); f.write(0x5555, 0x9090);
		CHECK(f.read(0) == 0x0089 && f.read(1) == 0x0015, "ID");
		f.write(0x5555, 0xaaaa); f.write(0x2aaa, 0x5555); f.write(0x5555, 0xf0f0); f.write(0x5555, 0xffff);
		CHECK(f.read(0) == 0xffff && !f.dirty(), "back to read array");
		f.write(0, 0x5050); f.write(0x10, 0x4040); f.write(0x10, 0x1234);
		CHECK(f.read(0) == 0x0080, "status after program");
		f.write(0, 0xffff);
		CHECK(mem[0x10] == 0x1234 && f.read(0x10) == 0x1234 && f.dirty(), "word program");
		f.write(0x10, 0x4040); f.write(0x10, 0xff00);
		f.write(0, 0xffff);
		CHECK(mem[0x10] == 0x1200, "program ANDs");
		f.write(0x1000, 0xe8e8);
		CHECK(f.read(0x1000) == 0x0080, "XSR");
		f.write(0x1000, 3); f.write(0x1000, 1); f.write(0x1001, 2); f.write(0x1002, 3); f.write(0x1003, 4); f.write(0x1000, 0xd0d0);
		f.write(0, 0xffff);
		CHECK(mem[0x1000] == 1 && mem[0x1003] == 4 && mem[0x1004] == 0xffff, "buffered program");
		f.write(0x1002, 0x2020); f.write(0x1002, 0xd0d0); f.write(0, 0xffff);
		CHECK(mem[0x1000] == 0xffff && mem[0x1003] == 0xffff && mem[0x10] == 0x1200, "block erase");
	}
	std::printf("%s (%d failures)\n", fails ? "FAILED" : "ok", fails);
	return fails ? 1 : 0;
}
