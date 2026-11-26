// SPDX-License-Identifier: GPL-3.0-or-later
// Unit bench for cores/proteus/p2k/rtl/cpu/mcf5206e_periph.sv (no ROMs): UART1 receive and transmit
// with CPU accesses on every clock phase, timer reference events against TER clears, the timer reset
// (TMR RST 1->0) and reset values as MAME's coldfire_timer_device, chip-select reset values, and the
// interrupt level. Each check prints PASS/FAIL; exit status 1 if any failed.
//   periph_tb [--seed N] [--bytes N]
// The system-clock enable runs either every clock or as on the board (33 MHz of 44.1 MHz clocks).
#include "Vmcf5206e_periph.h"
#include "verilated.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <random>
#include <string>
#include <vector>

static Vmcf5206e_periph *t;
static std::mt19937 rng;
static int fails = 0, checks = 0;
static bool ce_board = false;     // false: ce_sys every clock; true: 33 of 44.1 clocks
static long ce_acc = 0;
static long ce_ticks = 0;         // ce_sys clocks so far

static void check(bool ok, const std::string &what) {
	checks++;
	if (!ok) { fails++; printf("FAIL: %s\n", what.c_str()); }
}

// one clk cycle; the access (if sel) is presented for this clock. Returns dout seen before the edge.
struct Line;
static Line *rx_line = nullptr;
static int access_(bool sel, bool wr, int offs, bool uds, bool lds, int din);

// serial source for u1_rxd, advanced on ce_sys clocks; per = ce_sys clocks per bit
struct Line {
	int per = 32;
	std::deque<int> bytes;     // pending bytes; -1..-N = idle gap of N bits
	std::vector<int> bits;     // current frame
	int bit = 0, cnt = 0;
	int level() const { return bits.empty() ? 1 : bits[bit]; }
	void step() {            // called after a ce_sys clock
		if (bits.empty()) {
			if (bytes.empty()) return;
			int b = bytes.front(); bytes.pop_front();
			if (b < 0) { bits.assign(-b, 1); }
			else { bits.push_back(0); for (int k = 0; k < 8; k++) bits.push_back((b >> k) & 1); bits.push_back(1); }
			bit = 0; cnt = 0;
			return;
		}
		if (++cnt == per) { cnt = 0; if (++bit == (int)bits.size()) bits.clear(); }
	}
	bool idle() const { return bits.empty() && bytes.empty(); }
};

static int access_(bool sel, bool wr, int offs, bool uds, bool lds, int din) {
	bool ce;
	if (ce_board) { ce_acc += 330; ce = ce_acc >= 441; if (ce) ce_acc -= 441; }
	else ce = true;
	t->ce_sys = ce;
	t->u1_rxd = rx_line ? rx_line->level() : 1;
	t->sel = sel; t->wr = wr; t->offs = offs & 0x3fe; t->uds = uds; t->lds = lds; t->din = din & 0xffff;
	t->clk = 0; t->eval();
	int dout = t->dout;
	t->clk = 1; t->eval();
	t->sel = 0; t->wr = 0;
	if (ce) { ce_ticks++; if (rx_line) rx_line->step(); }
	return dout;
}
static void clk() { access_(false, false, 0, false, false, 0); }
// byte access: even offsets on the upper lane, odd on the lower
static int rd8(int a) { int d = access_(true, false, a & ~1, !(a & 1), a & 1, 0); return a & 1 ? d & 0xff : d >> 8; }
static void wr8(int a, int d) { access_(true, true, a & ~1, !(a & 1), a & 1, (a & 1) ? d : d << 8); }
static int rd16(int a) { return access_(true, false, a, true, true, 0); }
static void wr16(int a, int d) { access_(true, true, a, true, true, d); }
// combinational peek (no access, no clock)
static int peek8(int a) { t->sel = 0; t->offs = a & 0x3fe; t->eval(); return a & 1 ? t->dout & 0xff : t->dout >> 8; }

static void do_reset() {
	t->reset = 1;
	for (int i = 0; i < 4; i++) clk();
	t->reset = 0;
	clk();
}
static void new_dut() {
	delete t;
	t = new Vmcf5206e_periph;
	t->u1_rxd = 1; t->u2_rxd = 1; t->warp_level = 0; t->pp_in = 0xff;
	ce_acc = 0; ce_ticks = 0;
	do_reset();
}

enum { USR1 = 0x144, UCR1 = 0x148, RB1 = 0x14c, UBG1 = 0x158, UBG2 = 0x15c,
       TMR1 = 0x100, TRR1 = 0x104, TCN1 = 0x10c, TER1 = 0x111, ICR9 = 0x01c, IMR = 0x036, CSCR0 = 0x06e };

static void uart_setup() {
	wr8(UBG1, 0); wr8(UBG2, 1);      // 32 ce_sys clocks per bit
	wr8(UCR1, 0x05);                 // RX and TX enable
	for (int i = 0; i < 64; i++) clk();
}

// ---- RX: one RB read at a fixed clock relative to the clock byte 2 completes, then drain
static void rx_phase_sweep() {
	// calibrate: the clock (index from start) on which RxRDY count goes 1 -> 2
	auto run = [](long read_at, std::vector<int> &got, int &usr_end) -> long {
		new_dut(); uart_setup();
		Line ln; rx_line = &ln;
		ln.bytes = {0x90, 0x3c};
		long push2 = -1;
		int n = 0;
		got.clear();
		for (long i = 0; i < 20 * 32 * 2 + 400; i++) {
			// calibration run (read_at < 0): read byte 1 as soon as it arrives; push 2 is the
			// clock whose edge sets RxRDY again. Receiver timing does not depend on reads.
			if (read_at < 0 && (peek8(USR1) & 1)) { got.push_back(rd8(RB1)); n++; }
			else if (i == read_at && (peek8(USR1) & 1)) got.push_back(rd8(RB1));
			else clk();
			if (read_at < 0 && push2 < 0 && n == 1 && (peek8(USR1) & 1)) push2 = i;
		}
		while (peek8(USR1) & 1) got.push_back(rd8(RB1));
		usr_end = peek8(USR1);
		rx_line = nullptr;
		return push2;
	};
	for (int board = 0; board < 2; board++) {
		ce_board = board;
		std::vector<int> got; int usr;
		long push2 = run(-1, got, usr);
		check(push2 > 0 && got == std::vector<int>({0x90, 0x3c}), "rx calibrate");
		int bad = 0;
		for (long k = -40; k <= 40; k++) {
			run(push2 + k, got, usr);
			if (got != std::vector<int>({0x90, 0x3c}) || (usr & 0x10)) {
				bad++;
				if (bad <= 3) {
					printf("  rx read at push%+ld (ce %s): got", k, board ? "board" : "every");
					for (int b : got) printf(" %02x", b);
					printf(" USR %02x\n", usr);
				}
			}
		}
		check(bad == 0, std::string("rx RB read at every clock around a byte completing (ce ") + (board ? "board" : "every") + ")");
	}
	ce_board = false;
}

// ---- RX/TX stress: random bytes back to back, the CPU reads RB after a random delay, writes THR,
// and (optionally) writes UART commands that must not disturb the receiver or transmitter
static void uart_stress(int nbytes, bool with_cmds, bool board) {
	ce_board = board;
	new_dut(); uart_setup();
	Line ln; rx_line = &ln;
	std::vector<int> sent, got, tx_sent, tx_got;
	for (int i = 0; i < nbytes; i++) {
		if (rng() % 8 == 0) ln.bytes.push_back(-1 - (int)(rng() % 3));
		int b = rng() & 0xff; ln.bytes.push_back(b); sent.push_back(b);
	}
	// TX decoder, in ce_sys ticks
	int per = 32;
	long tx_start = -1; int tx_prev = 1; int tx_shift = 0; bool tx_bad = false;
	long read_wait = -1, thr_wait = rng() % 200;
	int overrun = 0;
	long limit = (long)nbytes * 14 * per * 2 + 10000;
	for (long i = 0; i < limit && !(ln.idle() && tx_got.size() == (size_t)nbytes && !(peek8(USR1) & 1)); i++) {
		int usr = peek8(USR1);
		if (usr & 0x10) overrun = 1;
		bool did = false;
		if ((usr & 1) && read_wait < 0) read_wait = rng() % 400;   // mean 200 clocks < one byte (320+ clocks): no overrun
		if (read_wait == 0) { got.push_back(rd8(RB1)); did = true; read_wait = -1; }
		else if (read_wait > 0) read_wait--;
		if (!did && (int)tx_sent.size() < nbytes && (usr & 4)) {
			if (thr_wait == 0) { int b = rng() & 0xff; wr8(RB1, b); tx_sent.push_back(b); did = true; thr_wait = rng() % 400; }
			else thr_wait--;
		}
		if (!did && with_cmds && rng() % 40 == 0) { wr8(UCR1, 0x40); did = true; }   // reset error status
		if (!did) clk();
		// sample txd after the edge, once per ce_sys tick
		static long last_ce = -1;
		if (ce_ticks != last_ce) {
			last_ce = ce_ticks;
			int lv = t->u1_txd;
			if (tx_start < 0) { if (tx_prev == 1 && lv == 0) { tx_start = ce_ticks; tx_shift = 0; } }
			if (tx_start >= 0) {
				long rel = ce_ticks - tx_start;          // 0 = first tick of the start bit
				int bitn = rel / per, phase = rel % per;
				// the line must hold each bit for exactly per ticks
				int expect;
				if (bitn == 0) expect = 0;
				else if (bitn <= 8) expect = -1;        // data: constant within the bit
				else expect = 1;
				static int bitval = 0;
				if (phase == 0) bitval = lv;
				if ((expect >= 0 && lv != expect) || lv != bitval) tx_bad = true;
				if (bitn >= 1 && bitn <= 8 && phase == per / 2) tx_shift |= lv << (bitn - 1);
				if (bitn == 9 && phase == per - 1) { tx_got.push_back(tx_shift); tx_start = -1; }
			}
			tx_prev = lv;
		}
	}
	while (peek8(USR1) & 1) got.push_back(rd8(RB1));
	rx_line = nullptr;
	std::string tag = std::string(with_cmds ? " with commands" : "") + " (ce " + (board ? "board" : "every") + ")";
	size_t same = 0;
	while (same < sent.size() && same < got.size() && sent[same] == got[same]) same++;
	if (got != sent || overrun) printf("  rx: sent %zu got %zu, first difference at %zu, overrun %d\n", sent.size(), got.size(), same, overrun);
	check(got == sent && !overrun, "uart rx stress, " + std::to_string(nbytes) + " bytes" + tag);
	check(tx_got == tx_sent && !tx_bad, "uart tx stress, exact bit timing" + tag);
	ce_board = false;
}

// ---- timers
static void timer_start(int trr, int tmr) { wr16(TRR1, trr); wr16(TMR1, tmr); }
static void timer_tests() {
	// reset values (MAME coldfire_timer_device::device_reset): TMR 0, TRR FFFF, TCN 0, TER 0
	new_dut();
	check(rd16(TMR1) == 0 && rd16(TRR1) == 0xffff && rd16(TCN1) == 0 && rd8(TER1) == 0, "timer reset values");
	// REF period: TMR = ORI | FRR | CLK sysclk | RST, prescale 1 -> TRR+1 ce_sys clocks
	const int TMR_RUN = 0x001b, TRR = 20;
	int lost = 0;
	// count events: CPU clears TER at a fixed delay d after each event; every period must be seen
	for (int d = 0; d <= TRR; d++) {   // d = TRR: the clear lands on the clock of the next event
		new_dut();
		timer_start(TRR, TMR_RUN);
		int events = 0;
		long clocks = 0;
		while (!(peek8(TER1) & 2)) { clk(); clocks++; }
		long t0 = ce_ticks;
		for (int n = 0; n < 12; n++) {
			// wait d clocks, then clear (the next event sets on that very clock when d == TRR; later than that two events share the one TER bit)
			for (int i = 0; i < d; i++) clk();
			wr8(TER1, 0x02);
			events++;
			long w = 0;
			while (!(peek8(TER1) & 2) && w < 3 * (TRR + 1)) { clk(); w++; }
			if (!(peek8(TER1) & 2)) break;
		}
		long span = ce_ticks - t0;
		// 12 events handled; with no event lost the 12th is found within 12 periods + slack
		if (events != 12 || span > 12L * (TRR + 1) + d + 4) lost++;
		if (events != 12 || span > 12L * (TRR + 1) + d + 4)
			if (lost <= 3) printf("  timer clear delay %d: %d events in %ld ticks\n", d, events, span);
	}
	check(lost == 0, "timer TER clear on the clock of a reference event keeps the event");

	// RST 1 -> 0 resets the whole timer (MAME tmr_w): TER cleared, TRR FFFF, TMR 0, TCN 0
	new_dut();
	wr8(ICR9, 0x14);                  // timer 1 level 5
	wr16(IMR, 0x3ffe & ~(1 << 9));
	timer_start(TRR, TMR_RUN);
	while (!(peek8(TER1) & 2)) clk();
	check(t->ipl == 5, "timer 1 REF raises IPL 5");
	wr16(TMR1, TMR_RUN & ~1);         // stop with REF pending
	check(rd16(TMR1) == 0 && rd16(TRR1) == 0xffff && rd16(TCN1) == 0 && rd8(TER1) == 0 && t->ipl == 0,
	      "timer RST 1->0 resets TMR/TRR/TCN/TER and the interrupt");
	wr16(TRR1, TRR);
	wr16(TMR1, TMR_RUN);              // restart with ORI: no spurious interrupt
	clk();
	check(t->ipl == 0 && rd8(TER1) == 0, "timer restart after RST 1->0: no stale REF interrupt");
	// a TMR write that keeps RST=0 (timer stopped) does not reset TRR
	new_dut();
	wr16(TRR1, 0x1234);
	wr16(TMR1, 0x0010);
	check(rd16(TRR1) == 0x1234 && rd16(TMR1) == 0x0010, "TMR write with RST 0->0 stores TMR, keeps TRR");
}

// ---- latch-only registers return to their reset values on reset (MAME init_regs)
static void reset_regs() {
	new_dut();
	check(rd16(CSCR0) == 0x3c1f, "CSCR0 power-on value");
	wr16(CSCR0, 0x1234); wr16(0x064, 0x5678); wr16(0x0c6, 0x9abc);
	check(rd16(CSCR0) == 0x1234, "CSCR0 write");
	do_reset();
	check(rd16(CSCR0) == 0x3c1f && rd16(0x064) == 0 && rd16(0x0c6) == 0, "CSCR0/CSAR0/DMCR after reset");
}

int main(int argc, char **argv) {
	Verilated::commandArgs(argc, argv);
	int seed = 1, nbytes = 3000;
	for (int i = 1; i + 1 < argc; i += 2) {
		if (!strcmp(argv[i], "--seed")) seed = atoi(argv[i + 1]);
		else if (!strcmp(argv[i], "--bytes")) nbytes = atoi(argv[i + 1]);
	}
	rng.seed(seed);
	rx_phase_sweep();
	for (int board = 0; board < 2; board++) {
		uart_stress(nbytes, false, board);
		uart_stress(nbytes, true, board);
	}
	timer_tests();
	reset_regs();
	delete t;
	printf("periph_tb seed %d: %d checks, %d failed%s\n", seed, checks, fails, fails ? "" : " - all passed");
	return fails ? 1 : 0;
}
