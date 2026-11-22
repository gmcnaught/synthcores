// Proteus 2000 CPU testbench (Verilator): p2k_cpu (TG68K kernel converted to Verilog by
// p2k/sim/build_cpu.sh, + MCF5206e peripherals) + flash + DRAM + I/O stubs mirroring the MAME
// research machine (emup2k.cpp). One bus access per clock (combinational memory).
//
//   p2k_cpu_tb FLASH.bin TRACE.txt [cycles]
// Trace: every data access (opcode fetches excluded):
//   <R|W> <word address> <data16> <lanes 3/2/1> <last fetch address>
#include "Vp2k_cpu.h"
#include "verilated.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <vector>

namespace {

struct board
{
	std::vector<uint16_t> flash = std::vector<uint16_t>(0x100000, 0xffff);
	std::vector<uint16_t> dram = std::vector<uint16_t>(0x400000, 0);
	std::vector<uint16_t> hmem = std::vector<uint16_t>(0x80000, 0);
	std::vector<uint16_t> g20 = std::vector<uint16_t>(0x800, 0);
	std::vector<uint16_t> g05 = std::vector<uint16_t>(32 * 64, 0);
	uint16_t g05_index = 0;
	uint32_t open_bus = 0;
	uint32_t last_fetch = 0;
	uint8_t adc_sel = 0;
	std::vector<uint16_t> sound;                             // SIMM slots 1-4 (word address slot << 24)
	std::vector<uint16_t> baby = std::vector<uint16_t>(4 << 20, 0xffff);
	bool baby_sel = false;                                   // 0xB00000 bit 6 (as MAME)
	uint32_t snd_addr[2] = {0, 0};
	uint16_t snd_latch[2] = {0, 0};
	uint8_t ddram[0x80];                                     // HD44780 display RAM (never busy)
	unsigned ddaddr = 0;
	bool lcd_dirty = false;
	board() { memset(ddram, ' ', sizeof(ddram)); }

	uint16_t read(uint32_t a)
	{
		if (a < 0x200000 || (a >= 0x02000000 && a < 0x02200000))
			return flash[(a & 0x1fffff) >> 1];
		if (a >= 0x01000000 && a < 0x01800000)
			return dram[(a & 0x7fffff) >> 1];
		if (a == 0x400000)
			return (ddaddr & 0x7f) << 8;                     // LCD: busy flag 0, address counter
		if (a == 0x408000)
		{
			const uint16_t v = ddram[ddaddr & 0x7f] << 8;   // data read, address increments
			ddaddr = (ddaddr + 1) & 0x7f;
			return v;
		}
		if (a >= 0x500000 && a < 0x600000)
			return hmem[(a & 0xfffff) >> 1];
		if (a >= 0x600000 && a < 0x600080)
		{
			const unsigned reg = a & 0x7e, idx = g05_index & 0x1f;
			if (reg == 0x0e) return g05_index;
			if (reg == 0x18 && idx == 0x1e) return 0x0030;    // G05 chip ID
			if (reg == 0x18 && idx == 0x1b) return 0x0000;    // encoder
			return g05[idx * 64 + reg / 2];
		}
		if (a >= 0x77f000 && a < 0x781000)
		{
			if ((a & 0x3e) == 0x1c || (a & 0x3e) == 0x1e)
			{
				// sound-memory read ports, pipelined as in MAME: return the latched word, fetch the next
				const int port = (a & 0x3e) == 0x1e;
				const uint32_t sa = snd_addr[port];
				const uint16_t v = snd_latch[port];
				snd_addr[port] = (sa + 1) & 0x3ffffff;
				if (baby_sel)
				{
					const uint32_t off = sa & 0xffffff;
					snd_latch[port] = off < (1u << 20) ? baby[((sa >> 24) & 3) << 20 | off] : 0xffff;
				}
				else
					snd_latch[port] = sa < sound.size() ? sound[sa] : 0;
				return v;
			}
			return g20[(a - 0x77f000) >> 1];    // 0x18/0x1A: MAME returns the live voice address
		}
		if (a == 0x800000)
			return last_fetch >= 0x02000000 ? 0xff7f : 0xffff;  // bit-7 boot workaround (as MAME)
		if (a == 0xc00000)
			return (adc_sel & 7) == 2 ? 0xffff : 0x0000;
		if (a == 0xf00000)
			return 0x0000;
		if (a >= 0x50000000 && a < 0x54000000)
			return (a & 2) ? open_bus & 0xffff : open_bus >> 16;
		return 0xffff;
	}

	void write(uint32_t a, uint16_t d, bool upper, bool lower)
	{
		auto merge = [&](uint16_t &m) {
			if (upper) m = (m & 0x00ff) | (d & 0xff00);
			if (lower) m = (m & 0xff00) | (d & 0x00ff);
		};
		if (a >= 0x01000000 && a < 0x01800000) merge(dram[(a & 0x7fffff) >> 1]);
		else if (a >= 0x500000 && a < 0x600000) merge(hmem[(a & 0xfffff) >> 1]);
		else if (a >= 0x600000 && a < 0x600080)
		{
			if ((a & 0x7e) == 0x0e) merge(g05_index);
			else merge(g05[(g05_index & 0x1f) * 64 + (a & 0x7e) / 2]);
		}
		else if (a >= 0x77f000 && a < 0x781000)
		{
			const unsigned v = (a - 0x77f000) >> 6, reg = a & 0x3e;
			uint16_t *r = &g20[(a - 0x77f000) >> 1];
			merge(*r);
			if (reg == 0x3e) *r &= 0x03ff;                  // as MAME (boot test writes 0x8211, expects 0x0211)
			const uint16_t *blk = &g20[v * 32];
			if (reg == 0x30 || reg == 0x32)
				snd_addr[0] = ((blk[0x30 / 2] << 16) | blk[0x32 / 2]) & 0x3ffffff;
			else if (reg == 0x34 || reg == 0x36)
				snd_addr[1] = ((blk[0x34 / 2] << 16) | blk[0x36 / 2]) & 0x3ffffff;
		}
		else if (a == 0xb00000 && upper) baby_sel = (d >> 8) & 0x40;
		else if (a == 0xc00000) adc_sel = d >> 8;
		else if (a == 0x400000 && upper)                     // HD44780 instruction
		{
			const uint8_t v = d >> 8;
			if (v & 0x80) ddaddr = v & 0x7f;
			else if (v == 0x01) { memset(ddram, ' ', sizeof(ddram)); ddaddr = 0; lcd_dirty = true; }
			else if ((v & 0xfe) == 0x02) ddaddr = 0;
		}
		else if (a == 0x408000 && upper)                     // HD44780 data
		{
			ddram[ddaddr & 0x7f] = d >> 8;
			ddaddr = (ddaddr + 1) & 0x7f;
			lcd_dirty = true;
		}
		else if (a >= 0x50000000 && a < 0x54000000)
		{
			if (a & 2) open_bus = (open_bus & 0xffff0000) | d;
			else open_bus = (open_bus & 0x0000ffff) | (uint32_t(d) << 16);
		}
	}
};

} // namespace

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		fprintf(stderr, "usage: %s FLASH.bin TRACE.txt [cycles]\n", argv[0]);
		return 2;
	}
	Verilated::commandArgs(argc, argv);
	board b;
	setvbuf(stdout, nullptr, _IOLBF, 0);
	{
		FILE *f = fopen(argv[1], "rb");
		if (!f) { perror(argv[1]); return 1; }
		std::vector<uint8_t> raw(0x200000);
		fread(raw.data(), 1, raw.size(), f);
		fclose(f);
		for (size_t i = 0; i < b.flash.size(); i++)
			b.flash[i] = (raw[2 * i] << 8) | raw[2 * i + 1];
	}
	// SIMM / Baby images as MAME (P2K_SIMM1..4, P2K_BABY1..4; big-endian byte files)
	for (int slot = 0; slot < 4; slot++)
	{
		char n[16];
		for (int kind = 0; kind < 2; kind++)
		{
			snprintf(n, sizeof(n), kind ? "P2K_SIMM%d" : "P2K_BABY%d", slot + 1);
			const char *path = getenv(n);
			if (!path || !*path) continue;
			FILE *f = fopen(path, "rb");
			if (!f) { perror(path); return 1; }
			std::vector<uint8_t> raw(kind ? 0x2000000 : 0x200000);
			const size_t words = fread(raw.data(), 1, raw.size(), f) / 2;
			fclose(f);
			if (kind && b.sound.size() < ((size_t(slot) + 1) << 24)) b.sound.resize(size_t(slot + 1) << 24, 0);
			for (size_t i = 0; i < words; i++)
				(kind ? b.sound[(size_t(slot) << 24) + i] : b.baby[(slot << 20) + i]) = (raw[2 * i] << 8) | raw[2 * i + 1];
		}
	}
	FILE *tf = fopen(argv[2], "w");
	const uint64_t cycles = argc > 3 ? strtoull(argv[3], nullptr, 0) : 1000000;
	const bool nodram = getenv("P2K_TB_NODRAM") != nullptr;   // leave DRAM accesses out of the trace
	// CPU clocks per MCF5206e system clock (TG68K needs several clocks per instruction where the
	// ColdFire is pipelined); peripherals tick on the system clock enable
	const unsigned ratio = getenv("P2K_TB_RATIO") ? atoi(getenv("P2K_TB_RATIO")) : 4;
	FILE *lcd = getenv("P2K_TB_LCD") ? fopen(getenv("P2K_TB_LCD"), "w") : nullptr;

	auto cpu = std::make_unique<Vp2k_cpu>();
	cpu->u1_rxd = 1;
	cpu->u2_rxd = 1;
	cpu->pp_in = 0xff;
	cpu->reset_n = 0;
	for (int i = 0; i < 10; i++)
	{
		cpu->clk = 0; cpu->eval();
		cpu->clk = 1; cpu->eval();
	}
	cpu->reset_n = 1;

	for (uint64_t c = 0; c < cycles; c++)
	{
		cpu->ce_sys = (c % ratio) == 0;
		if (lcd && b.lcd_dirty && (c % (33000000ULL * ratio / 4)) == 0)    // LCD text every 0.25 s of system time
		{
			char l[2][25];
			for (int r = 0; r < 2; r++)
			{
				for (int i = 0; i < 24; i++)
				{
					const uint8_t ch = b.ddram[r * 0x40 + i];
					l[r][i] = (ch >= 0x20 && ch < 0x7f) ? ch : '.';
				}
				l[r][24] = 0;
			}
			fprintf(lcd, "LCD %.2f |%s|%s|\n", double(c) / (33e6 * ratio), l[0], l[1]);
			fflush(lcd);
			b.lcd_dirty = false;
			if (strstr(l[0], "Err:"))                                   // boot self-test failure: stop
			{
				fprintf(stderr, "stopped on LCD error at %.2f s: %s\n", double(c) / (33e6 * ratio), l[0]);
				break;
			}
		}
		cpu->clk = 0;
		cpu->eval();
		const uint32_t a = cpu->addr;
		const unsigned bs = cpu->busstate;
		if (cpu->ext_req)
		{
			const bool upper = cpu->uds, lower = cpu->lds;
			const uint16_t d = bs == 3 ? 0 : b.read(a & ~1u);
			cpu->ext_din = d;
			cpu->eval();
			if (bs == 0)
				b.last_fetch = a;
			else
			{
				const uint16_t v = bs == 3 ? cpu->dout : d;
				if (!(nodram && a >= 0x01000000 && a < 0x01800000))
					fprintf(tf, "%c %08X %04X %d %08X\n", bs == 3 ? 'W' : 'R', a & ~1u, v, (upper ? 2 : 0) | (lower ? 1 : 0), b.last_fetch);
				if (bs == 3)
				{
					b.write(a & ~1u, cpu->dout, upper, lower);
				}
			}
		}
		else if (bs == 0)
			b.last_fetch = a;
		cpu->clk = 1;
		cpu->eval();
	}
	fclose(tf);
	cpu->final();
	return 0;
}
