// mfp68901 against MAME's mc68901 behaviour (with patch 0001) at corners the core can reach.
// Usage: Vmfp68901 TEST; exit 0 pass, 1 fail. Tests (review 2026-11-26 proteus1.md):
//   ipr      #4: timer C interrupts while the CPU rewrites IERB / IPRB at every clock offset;
//            every timeout must be acknowledged (MAME applies the event and the write in
//            sequence, so neither is lost)
//   tdr      #11: timer A in pulse-width mode with the input inactive: a TADR write loads the
//            counter (MAME: the timer is disabled); with the input active it does not
//   overrun  #12: USART overrun is held back until UDR is read, then sets OE and raises the
//            receive-error interrupt; while OE is set no character is loaded
#include "Vmfp68901.h"
#include "verilated.h"
#include <cstdio>
#include <cstdlib>
#include <string>

enum { GPIP, AER, DDR, IERA, IERB, IPRA, IPRB, ISRA, ISRB, IMRA, IMRB, VR, TACR, TBCR, TCDCR,
       TADR, TBDR, TCDR, TDDR, SCR, UCR, RSR, TSR, UDR };

int main(int argc, char **argv) {
	if (argc < 2) { fprintf(stderr, "usage: %s ipr|tdr|overrun\n", argv[0]); return 2; }
	std::string test = argv[1];
	Verilated::commandArgs(argc, argv);
	Vmfp68901 *t = new Vmfp68901;
	uint64_t cyc = 0;
	int ser_div = 0;
	bool ce_every_clock = true;
	auto clk = [&]() {
		t->ce_timer = ce_every_clock ? 1 : 0;
		t->ce_ser = (++ser_div % 4) == 0;     // USART x16 clock every 4 clocks (bit = 64 clocks)
		t->clk = 0; t->eval(); t->clk = 1; t->eval();
		t->wr = 0; t->rd = 0; t->iack = 0;
		cyc++;
	};
	auto wr = [&](int r, int v) { t->rs = r; t->din = v; t->wr = 1; clk(); };
	auto rd = [&](int r) { t->rs = r; t->eval(); int v = t->dout; t->rd = 1; clk(); return v; };
	t->cs = 1; t->wr = 0; t->rd = 0; t->iack = 0; t->gpio_in = 0xFF; t->tai = 0; t->tbi = 0; t->si = 1;
	t->reset = 1; for (int i = 0; i < 8; i++) clk(); t->reset = 0; clk();
	int rc = 0;
	auto fail = [&](const char *m) { fprintf(stderr, "FAIL %s: %s\n", test.c_str(), m); rc = 1; };

	if (test == "ipr") {
		// timer C: delay mode, prescale 200 (mode 7), count 1 -> a timeout every 200 clocks
		wr(VR, 0x40); wr(IMRB, 0x20); wr(IERB, 0x20); wr(TCDR, 1); wr(TCDCR, 0x70);
		int timeouts = 0, acks = 0, last_to = t->tout >> 2 & 1, k = 0;
		for (int i = 0; i < 200000; i++) {
			if (t->irq) {
				t->iack = 1; clk();
				if ((t->vector & 0x0F) == 5) acks++;
			}
			else if ((k++ % 7) == 0) {
				// read-modify-write cycles as the firmware's bclr/bset on IERB, and IPRB writes
				// that clear another channel
				if (k % 2) wr(IERB, 0x20); else wr(IPRB, 0xFE);
			}
			else clk();
			int to = t->tout >> 2 & 1;
			if (to != last_to) { timeouts++; last_to = to; }
		}
		while (t->irq) { t->iack = 1; clk(); if ((t->vector & 0x0F) == 5) acks++; }
		fprintf(stderr, "timer C timeouts %d, interrupts acknowledged %d\n", timeouts, acks);
		if (timeouts < 900) fail("timer did not run");
		if (acks != timeouts) fail("timer interrupts were lost");
	}
	else if (test == "tdr") {
		wr(AER, 0x10);               // TAI active high
		t->tai = 0;
		wr(TACR, 0x09);              // pulse width, prescale 4
		wr(TADR, 0x55);
		int v = rd(TADR);
		fprintf(stderr, "pulse mode, input inactive: TADR write 55 -> counter %02x (MAME 55)\n", v);
		if (v != 0x55) fail("counter not loaded while the timer is stopped by its input");
		t->tai = 1;                  // timer counts
		for (int i = 0; i < 40; i++) clk();
		wr(TADR, 0x66);
		v = rd(TADR);
		fprintf(stderr, "pulse mode, input active: TADR write 66 -> counter %02x (MAME: still counting from 55)\n", v);
		if (v == 0x66 || v > 0x55) fail("counter loaded while the timer runs");
		t->tai = 0;
		wr(TACR, 0x00); wr(TADR, 0x77);
		v = rd(TADR);
		if (v != 0x77) fail("counter not loaded while stopped");
	}
	else if (test == "overrun") {
		auto send = [&](int b) {   // 8N1, 64 clocks per bit
			int bits = 0x200 | (b << 1);
			for (int i = 0; i < 10; i++) { t->si = bits >> i & 1; for (int j = 0; j < 64; j++) clk(); }
			t->si = 1; for (int j = 0; j < 128; j++) clk();
		};
		wr(UCR, 0x88); wr(RSR, 0x01);
		wr(IMRA, 0x18); wr(IERA, 0x18);   // receive buffer full (12) and receive error (11)
		send(0x11);
		int ipra = rd(IPRA);
		if (!(ipra & 0x10)) fail("no buffer-full interrupt");
		wr(IPRA, ~0x10 & 0xFF);
		send(0x22);                     // overrun: buffer still full
		int rsr = rd(RSR);
		fprintf(stderr, "after overrun, before UDR read: RSR %02x (MAME: OE not yet set)\n", rsr);
		if (rsr & 0x40) fail("OE set before the UDR read");
		int d = rd(UDR);
		rsr = rd(RSR);                  // reading RSR shows OE, then clears it
		ipra = rd(IPRA);
		fprintf(stderr, "UDR %02x; then RSR %02x, IPRA %02x (MAME: 11, OE set, receive error pending)\n", d, rsr, ipra);
		if (d != 0x11) fail("UDR lost the first character");
		if (!(rsr & 0x40)) fail("OE not set after the UDR read");
		if (!(ipra & 0x08)) fail("no receive-error interrupt");
		wr(IPRA, 0x00);
		// OE held: a character arriving while OE is set is not loaded
		send(0x33); d = rd(UDR); (void)d;   // buffer empty again, OE clear (read above)
		send(0x44);
		d = rd(UDR);
		fprintf(stderr, "next characters: UDR %02x (MAME 44)\n", d);
		if (d != 0x44) fail("receiver did not resume");
		// overrun again, but do not read RSR: OE stays set and blocks the next character
		send(0x55); send(0x66); d = rd(UDR);   // 55 read, OE now set (66 lost)
		send(0x77);
		rsr = rd(RSR);
		fprintf(stderr, "char while OE set: RSR %02x (MAME: OE, buffer empty)\n", rsr);
		if (!(rsr & 0x40) || (rsr & 0x80)) fail("a character was loaded while OE was set");
	}
	else { fprintf(stderr, "unknown test\n"); return 2; }
	fprintf(stderr, "%s: %s\n", test.c_str(), rc ? "FAIL" : "ok");
	delete t;
	return rc;
}
