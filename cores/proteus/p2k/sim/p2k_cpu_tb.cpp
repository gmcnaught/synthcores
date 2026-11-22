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

	uint16_t read(uint32_t a)
	{
		if (a < 0x200000 || (a >= 0x02000000 && a < 0x02200000))
			return flash[(a & 0x1fffff) >> 1];
		if (a >= 0x01000000 && a < 0x01800000)
			return dram[(a & 0x7fffff) >> 1];
		if (a == 0x400000)
			return 0x0000;                                   // LCD never busy
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
			if ((a & 0x3e) == 0x1c || (a & 0x3e) == 0x1e) return 0;   // sound-memory ports (no SIMM)
			return g20[(a - 0x77f000) >> 1];
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
		else if (a >= 0x77f000 && a < 0x781000) merge(g20[(a - 0x77f000) >> 1]);
		else if (a == 0xc00000) adc_sel = d >> 8;
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
	FILE *tf = fopen(argv[2], "w");
	const uint64_t cycles = argc > 3 ? strtoull(argv[3], nullptr, 0) : 1000000;
	const bool nodram = getenv("P2K_TB_NODRAM") != nullptr;   // leave DRAM accesses out of the trace

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
		cpu->clk = 0;
		cpu->eval();
		const uint32_t a = cpu->addr;
		const unsigned bs = cpu->busstate;
		if (cpu->ext_req)
		{
			const bool upper = cpu->uds, lower = cpu->lds;
			const uint16_t d = b.read(a & ~1u);
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
					b.write(a & ~1u, cpu->dout, upper, lower);
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
