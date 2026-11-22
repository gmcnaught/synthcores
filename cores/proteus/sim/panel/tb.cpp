// Unit test for mister/rtl/panel.sv (clock scaled: CLK_HZ = 80 MHz, run cycles directly)
#include "Vpanel.h"
#include "verilated.h"
#include <cstdio>
int main(int argc, char **argv) {
	Verilated::commandArgs(argc, argv);
	Vpanel *p = new Vpanel; unsigned tog = 0; long cyc = 0;
	auto tick = [&]() { p->clk = 0; p->eval(); p->clk = 1; p->eval(); cyc++; };
	auto key = [&](int code, int ext, int pressed) { tog ^= 1; p->ps2_key = (tog << 10) | (pressed << 9) | (ext << 8) | code; };
	p->reset = 1; p->joy = 0; for (int i = 0; i < 10; i++) tick(); p->reset = 0;
	int last = p->gpio, steps = 0, m_low = 0;
	auto run = [&](long n) { for (long i = 0; i < n; i++) { tick(); int g = p->gpio;
		if ((last & 0x10) && !(g & 0x10)) { steps++; printf("step at %.3f s dir_bit5=%d\n", cyc / 80e6, (g >> 5) & 1); }
		if ((last & 1) != (g & 1)) printf("gpio0 -> %d at %.3f s\n", g & 1, cyc / 80e6);
		last = g; } };
	key(0x3A, 0, 1); run(8000000); key(0x3A, 0, 0); run(800000);          // M for 100 ms
	key(0x74, 1, 1); run(40000000); key(0x74, 1, 0); run(4000000);        // Right held 0.5 s
	key(0x6B, 1, 1); run(800000); key(0x6B, 1, 0); run(4000000);          // Left tap
	printf("steps=%d final gpio=%02x\n", steps, p->gpio);
	delete p; return 0;
}
