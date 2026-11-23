// Proteus 2000 CPU testbench (Verilator): p2k_cpu (TG68K kernel converted to Verilog by
// p2k/sim/build_cpu.sh, + MCF5206e peripherals) + flash + DRAM + I/O stubs mirroring the MAME
// research machine (emup2k.cpp). One bus access per clock (combinational memory).
//
//   p2k_cpu_tb FLASH.bin TRACE.txt [cycles]
// Trace: every data access (opcode fetches excluded):
//   <R|W> <word address> <data16> <lanes 3/2/1> <last fetch address> [V]
// V marks a read of the vector table (VBR..VBR+0x3FF); it is written even with P2K_TB_NODRAM.
//
// P2K_TB_REPLAY=MAME.bt (a P2K_BUSTRACE_NODRAM trace): compare against MAME while running and
// stop at the first divergence (same filters as tools/p2k_trace_cmp.py). Interrupts are taken
// where MAME took them: the SIM's interrupt level is hidden from the kernel until MAME's next
// access is an interrupt-vector read and the kernel would stack MAME's PC at that read (the
// interrupted instruction); then a timer at that level is made to reach its reference (warp_level). Exit 0 at the
// end of the MAME trace, 1 on divergence, a stall or no progress.
#include "Vp2k_cpu.h"
#include "verilated.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
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

	// Flash: Sharp LH28F160S3 command set as MAME's intelfsh16 (maker 0xB0, device 0xD0), the
	// subset the OS reaches. Erase and program complete at once (MAME: status busy for a while;
	// replay mode takes MAME's status reads).
	enum { F_NORMAL, F_READID, F_STATUS, F_WRITE1, F_CLEAR1, F_MASTER, F_AMD1, F_AMD2, F_AMD3, F_BYTEPROG } fmode = F_NORMAL;
	uint16_t fstatus = 0x80;
	bool fmaster = false;

	uint16_t flash_read(uint32_t w)
	{
		switch (fmode)
		{
		case F_STATUS: return fstatus;
		case F_AMD3: return (w & 0xff) == 0 ? 0xb0 : (w & 0xff) == 1 ? 0xd0 : 0;
		case F_READID:
			switch (w & 0xff) { case 0: return 0xb0; case 1: return 0xd0; case 3: return fmaster; default: return 0; }
		default: return flash[w];
		}
	}

	void flash_write(uint32_t w, uint16_t d)
	{
		const uint8_t c = d & 0xff;
		switch (fmode)
		{
		case F_NORMAL: case F_STATUS: case F_READID: case F_AMD3:
			switch (c)
			{
			case 0xf0: case 0xff: fmode = F_NORMAL; break;
			case 0x90: fmode = F_READID; break;
			case 0x40: case 0x10: fmode = F_WRITE1; break;
			case 0x50: fstatus = 0x80; fmode = F_STATUS; break;
			case 0x20: fmode = F_CLEAR1; break;
			case 0x60: fmode = F_MASTER; break;
			case 0x70: fmode = F_STATUS; break;
			case 0xaa: if ((w & 0xfff) == 0x555 || (w & 0xfff) == 0xaaa) fmode = F_AMD1; break;
			default: fprintf(stderr, "flash: unknown command %02X at word %06X\n", c, w); break;
			}
			break;
		case F_AMD1:
			fmode = c == 0x55 && ((w & 0xffff) == 0x2aa || (w & 0xffff) == 0x2aaa || (w & 0xfff) == 0x555) ? F_AMD2 : F_NORMAL;
			break;
		case F_AMD2:
			{
				const bool cmd = (w & 0xffff) == 0x555 || (w & 0xffff) == 0x5555 || (w & 0xfff) == 0xaaa;
				fmode = F_NORMAL;
				if (cmd && c == 0x90) fmode = F_AMD3;
				else if (cmd && c == 0xa0) fmode = F_BYTEPROG;
				else if (cmd && c == 0x80) fprintf(stderr, "flash: AMD erase sequence not modelled (word %06X)\n", w);
				else if (!(cmd && c == 0xf0) && !(((w & 0xfff) == 0xaaa || (w & 0xfff) == 0x555) && c == 0x20))
					fprintf(stderr, "flash: unexpected %02X at word %06X after AA/55\n", c, w);
			}
			break;
		case F_BYTEPROG: flash[w] &= d; fmode = F_NORMAL; break;
		case F_WRITE1: flash[w] &= d; fstatus = 0x80; fmode = F_STATUS; break;
		case F_CLEAR1:
			if (c == 0xd0)
			{
				const uint32_t base = (w * 2) & ~0xffffu;            // 64 KB block, as MAME
				for (uint32_t i = 0; i < 0x8000; i++) flash[(base >> 1) + i] = 0xffff;
				fstatus = 0x80;
				fmode = F_STATUS;
			}
			else
				fprintf(stderr, "flash: unexpected %02X in block erase\n", c);
			break;
		case F_MASTER:
			if (c == 0xf1) fmaster = true;
			else if (c == 0xd0) fmaster = false;
			fmode = F_STATUS;
			break;
		}
	}

	uint16_t read(uint32_t a)
	{
		if (a < 0x200000 || (a >= 0x02000000 && a < 0x02200000))
			return flash_read((a & 0x1fffff) >> 1);
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
		else if (a < 0x200000 || (a >= 0x02000000 && a < 0x02200000)) flash_write((a & 0x1fffff) >> 1, d);
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


// ---- MAME lockstep (port of tools/p2k_trace_cmp.py) ----
struct word
{
	char k;          // 'R' or 'W'
	uint32_t a;      // word address
	uint16_t d;
	unsigned lanes;  // 2 upper, 1 lower
	uint32_t pc;     // MAME: PC at the access; RTL: last fetch address
};

bool flash_area(uint32_t a) { return a < 0x200000 || (a >= 0x02000000 && a < 0x02200000); }
uint32_t adiff(uint32_t x, uint32_t y) { return x > y ? x - y : y - x; }
bool near_fetch(const word &t) { return t.k == 'R' && flash_area(t.a) && adiff(t.a, t.pc) <= 32; }
bool polled(const word &t) { return t.k == 'R' && t.a == 0x00400000; }   // LCD busy flag

bool same(const word &x, const word &y)
{
	if (x.k != y.k || x.a != y.a)
		return false;
	const unsigned l = x.lanes & y.lanes;
	const uint16_t m = (l & 2 ? 0xff00 : 0) | (l & 1 ? 0x00ff : 0);
	return (x.d & m) == (y.d & m);
}

struct replay
{
	FILE *f = nullptr;
	std::deque<word> m, r, hist;
	uint64_t matched = 0, mame_records = 0;
	bool m_eof = false;
	enum { RUN, END, DIVERGED } state = RUN;

	bool fill_m(size_t n)
	{
		while (m.size() < n && !m_eof)
		{
			uint32_t rec[4];
			if (fread(rec, 4, 4, f) != 4) { m_eof = true; break; }
			mame_records++;
			const bool w = rec[3] & 1;
			const uint32_t pc = rec[3] & ~1u;
			for (int half = 0; half < 2; half++)
			{
				const uint32_t mk = half ? rec[2] & 0xffff : rec[2] >> 16;
				if (!mk) continue;
				const uint32_t wa = rec[0] + 2 * half;
				const unsigned lanes = (mk & 0xff00 ? 2 : 0) | (mk & 0x00ff ? 1 : 0);
				if (!w && flash_area(wa) && adiff(wa, pc) <= 16) continue;     // opcode fetch from flash
				m.push_back({w ? 'W' : 'R', wa, uint16_t(half ? rec[1] & 0xffff : rec[1] >> 16), lanes, pc});
			}
		}
		return m.size() >= n;
	}

	void ok(const word &t)
	{
		hist.push_back(t);
		if (hist.size() > 16) hist.pop_front();
		matched++;
	}

	static void fmt(const char *tag, const word &t)
	{
		fprintf(stderr, "   %-5s %c %08X %04X l%u pc %08X\n", tag, t.k, t.a, t.d, t.lanes, t.pc);
	}

	void diverge()
	{
		state = DIVERGED;
		fprintf(stderr, "DIVERGENCE after %llu matched word accesses (MAME record %llu)\n",
		        (unsigned long long)matched, (unsigned long long)mame_records);
		for (const word &t : hist) fmt("ok", t);
		fmt("MAME", m[0]);
		fmt("RTL", r[0]);
	}

	void step()
	{
		while (state == RUN && !r.empty())
		{
			fill_m(2);
			if (m.empty()) { state = END; return; }
			if (same(m[0], r[0])) { ok(m[0]); m.pop_front(); r.pop_front(); continue; }
			if (r.size() < 2 && m.size() > 1 && same(m[1], r[0]))
				return;                                   // maybe a swapped pair: wait for the next RTL word
			if (r.size() > 1 && m.size() > 1 && same(m[0], r[1]) && same(m[1], r[0]))
			{
				ok(m[0]); ok(m[1]);
				m.pop_front(); m.pop_front(); r.pop_front(); r.pop_front();
				continue;
			}
			if (polled(r[0])) r.pop_front();
			else if (polled(m[0])) m.pop_front();
			else if (near_fetch(r[0])) r.pop_front();
			else if (near_fetch(m[0])) m.pop_front();
			else diverge();
		}
	}

	// MAME's next significant access is an interrupt-vector read: -1 if not, else the level for an
	// autovector (25-31), 0 for vector 24 or a vector >= 64 (level not known from the vector)
	// (due_pc: MAME's PC at the vector read, the interrupted instruction's address)
	uint32_t due_pc = 0;
	int irq_due(uint32_t vbr)
	{
		if (state != RUN || !r.empty())
			return -1;
		for (size_t i = 0;; i++)
		{
			if (!fill_m(i + 1)) return -1;
			const word &t = m[i];
			if (polled(t) || near_fetch(t)) continue;
			const uint32_t o = t.a - vbr;
			if (t.k != 'R' || t.a < vbr) return -1;
			due_pc = t.pc;
			if (o >= 0x60 && o < 0x80) return (o - 0x60) / 4;
			return o >= 0x100 && o < 0x400 ? 0 : -1;
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
	replay rp;
	if (const char *bt = getenv("P2K_TB_REPLAY"))
	{
		rp.f = fopen(bt, "rb");
		if (!rp.f) { perror(bt); return 1; }
	}
	const uint64_t stall_limit = 33000000ULL * 4 * ratio;    // 4 s of system time without the interrupt
	uint64_t stalled = 0, idle = 0;
	FILE *mbarlog = getenv("P2K_TB_MBARLOG") ? fopen(getenv("P2K_TB_MBARLOG"), "w") : nullptr;
	if (mbarlog) setvbuf(mbarlog, nullptr, _IOLBF, 0);

	auto cpu = std::make_unique<Vp2k_cpu>();
	cpu->u1_rxd = 1;
	cpu->u2_rxd = 1;
	cpu->pp_in = 0xff;
	cpu->cpu_ena = 1;
	cpu->ipl_mask = rp.f != nullptr;
	cpu->warp_level = 0;
	cpu->reset_n = 0;
	for (int i = 0; i < 10; i++)
	{
		cpu->clk = 0; cpu->eval();
		cpu->clk = 1; cpu->eval();
	}
	cpu->reset_n = 1;

	// Interrupt gate (replay): MAME's next access is an interrupt vector and the kernel would stack
	// MAME's interrupted PC at this boundary: TG68_PC while the kernel is in a fetch state (state
	// 00), else the prefetched opcode's address (measured over 30 interrupts in 6 state
	// combinations of the unreplayed boot). Then the SIM's level goes through, and a timer at that
	// level counts as at its reference (warp_level). MAME may run DRAM-only code before the
	// interrupt, which the trace does not show, so the CPU keeps running until then.
	int due = -1;
	auto gate = [&]() {
		due = rp.irq_due(cpu->vbr_out);
		const uint32_t stack_pc = (cpu->kdbg & 3) == 0 ? cpu->pc_out : cpu->opc_pc;
		const bool at = due >= 0 && stack_pc == rp.due_pc;
		cpu->ipl_mask = !at;
		cpu->warp_level = at && due > 0 ? due : 0;
	};
	const int bdbg_n = getenv("P2K_TB_BDBG") ? atoi(getenv("P2K_TB_BDBG")) : 0;
	int bdbg = bdbg_n;
	for (uint64_t c = 0; c < cycles; c++)
	{
		if (rp.f)
		{
			gate();
			if (due < 0)
				stalled = 0;
			else if (++stalled > stall_limit)
			{
				fprintf(stderr, "STALL: MAME takes an interrupt after %llu matched word accesses; the RTL has not taken it in 4 s (SIM level %u, last fetch %08X)\n",
				        (unsigned long long)rp.matched, cpu->ipl_raw, b.last_fetch);
				rp.fmt("MAME", rp.m[0]);
				rp.state = replay::DIVERGED;
				break;
			}
		}
		if (rp.f && rp.r.empty() && !rp.fill_m(1))
		{
			rp.state = replay::END;                  // every MAME access matched
			break;
		}
		if (rp.f && ++idle > stall_limit)
		{
			fprintf(stderr, "NO PROGRESS: no RTL access for 4 s of system time after %llu matched word accesses (SIM level %u, mask %u, last fetch %08X)\n",
			        (unsigned long long)rp.matched, cpu->ipl_raw, cpu->ipl_mask, b.last_fetch);
			rp.fill_m(4);
			for (size_t i = 0; i < rp.m.size() && i < 4; i++) rp.fmt("MAME", rp.m[i]);
			for (const word &t : rp.r) rp.fmt("RTL", t);
			// P2K_TB_PEEK=addr[,addr...]: long words to show (DRAM or flash)
			if (const char *pk = getenv("P2K_TB_PEEK"))
				for (const char *q = pk; *q;)
				{
					char *e;
					const uint32_t pa = strtoul(q, &e, 0) & ~1u;
					fprintf(stderr, "   PEEK %08X = %04X%04X\n", pa, b.read(pa), b.read(pa + 2));
					q = *e ? e + 1 : e;
				}
			rp.state = replay::DIVERGED;
			break;
		}
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
		if (getenv("P2K_TB_IRQDBG") && cpu->irq_take)
			fprintf(stderr, "IRQ take: pc_out %08X opc_pc %08X last fetch %08X kdbg %02X busstate %u\n", cpu->pc_out, cpu->opc_pc, b.last_fetch, cpu->kdbg, cpu->busstate);
		const uint32_t a = cpu->addr;
		const unsigned bs = cpu->busstate;
		if (mbarlog && cpu->cpu_ena && bs == 3 && (cpu->mbar_out & 1) && (a >> 10) == (cpu->mbar_out >> 10))
			fprintf(mbarlog, "MW %03X %04X %d%d pc %08X matched %llu\n", a & 0x3ff, cpu->dout, cpu->uds, cpu->lds,
			        b.last_fetch, (unsigned long long)rp.matched);
		if (cpu->ext_req)
		{
			const bool upper = cpu->uds, lower = cpu->lds;
			uint16_t d = bs == 3 ? 0 : b.read(a & ~1u);
			// replay: LCD status polls return MAME's value (its HD44780 model reports busy; the
			// number of polls decides where a timer interrupt lands)
			if (rp.f && bs == 2 && (a & ~1u) == 0x400000 && rp.r.empty() && rp.fill_m(1) && rp.m[0].k == 'R'
			    && rp.m[0].a == 0x400000)
				d = rp.m[0].d;
			// replay: flash reads outside read-array mode (status, IDs) return MAME's value (its
			// erase/program take emulated time; the status polls follow it)
			if (rp.f && bs == 2 && b.fmode != board::F_NORMAL && flash_area(a & ~1u) && rp.r.empty() && rp.fill_m(1)
			    && rp.m[0].k == 'R' && rp.m[0].a == (a & ~1u))
				d = rp.m[0].d;
			cpu->ext_din = d;
			cpu->eval();
			if (bs == 0)
				b.last_fetch = a;
			else
			{
				const uint16_t v = bs == 3 ? cpu->dout : d;
				const uint32_t wa = a & ~1u, vbr = cpu->vbr_out;
				const bool vec = bs != 3 && wa >= vbr && wa < vbr + 0x400;
				const bool dram = wa >= 0x01000000 && wa < 0x01800000;
				const unsigned lanes = (upper ? 2 : 0) | (lower ? 1 : 0);
				if (!(nodram && dram) || vec)
					fprintf(tf, "%c %08X %04X %u %08X%s\n", bs == 3 ? 'W' : 'R', wa, v, lanes, b.last_fetch, vec ? " V" : "");
				// replay: same RTL-side filter as p2k_trace_cmp.py (MBAR never reaches the board)
				if (rp.f && !(bs != 3 && wa < 8) && !(dram && !vec)
				    && !(bs != 3 && flash_area(wa) && adiff(wa, b.last_fetch) <= 16))
				{
					rp.r.push_back({bs == 3 ? 'W' : 'R', wa, v, lanes, b.last_fetch});
					idle = 0;
					rp.step();
					if (rp.state != replay::RUN)
					{
						b.write(wa, cpu->dout, upper && bs == 3, lower && bs == 3);
						break;
					}
				}
				if (bs == 3)
				{
					b.write(a & ~1u, cpu->dout, upper, lower);
				}
			}
		}
		else if (bs == 0)
			b.last_fetch = a;
		if (rp.f)
		{
			gate();                                  // an access matched this cycle can make the interrupt due
			cpu->eval();
			if (bdbg > 0 && due >= 0 && cpu->endopc)
			{
				bdbg--;
				fprintf(stderr, "BOUNDARY due %08X pc_out %08X opc_pc %08X kdbg %02X bus %u mask %u take %u\n",
				        rp.due_pc, cpu->pc_out, cpu->opc_pc, cpu->kdbg, cpu->busstate, cpu->ipl_mask, cpu->irq_take);
			}
		}
		cpu->clk = 1;
		cpu->eval();
	}
	fclose(tf);
	cpu->final();
	if (rp.f)
	{
		if (rp.state == replay::RUN && rp.m_eof && rp.m.empty()) rp.state = replay::END;
		if (rp.state == replay::END)
			printf("REPLAY END: matched %llu word accesses; end of the MAME trace (%llu records)\n",
			       (unsigned long long)rp.matched, (unsigned long long)rp.mame_records);
		else if (rp.state == replay::RUN)
			printf("REPLAY RUNNING: matched %llu word accesses at the cycle limit (MAME record %llu)\n",
			       (unsigned long long)rp.matched, (unsigned long long)rp.mame_records);
		return rp.state == replay::DIVERGED;
	}
	return 0;
}
