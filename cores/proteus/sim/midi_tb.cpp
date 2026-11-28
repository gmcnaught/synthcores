// midi_in in front of the MFP: a full-rate MIDI stream while the CPU does not read UDR for
// PAUSE_MS (nv_ctrl's save pause), then reads every byte as soon as RSR says buffer full.
// Usage: Vmidi_bench [bypass]; prints "received N of M, overruns K"; exit 0 when all M bytes
// arrive in order with no overrun (with bypass, the old direct wiring, bytes are lost).
#include "Vmidi_bench.h"
#include "verilated.h"
#include <cstdio>
#include <cstring>
#include <vector>

enum { RSR = 21, UDR = 23 };
static const int BIT = 2560;           // 80 MHz / 31250
static const double PAUSE_MS = 60.0;

int main(int argc, char **argv) {
	bool bypass = argc > 1 && !strcmp(argv[1], "bypass");
	Verilated::commandArgs(argc, argv);
	Vmidi_bench *t = new Vmidi_bench;
	std::vector<int> sent, got;
	for (int i = 0; i < 400; i++) sent.push_back(i % 3 == 0 ? (0x80 | (i >> 4 & 0x1F)) : (i * 37) & 0x7F);
	// line level per clock: bytes back to back from clock 4000
	auto level = [&](uint64_t c) {
		if (c < 4000) return 1;
		uint64_t b = (c - 4000) / (10 * BIT), k = (c - 4000) / BIT % 10;
		if (b >= sent.size()) return 1;
		if (k == 0) return 0;
		if (k == 9) return 1;
		return (sent[b] >> (k - 1)) & 1;
	};
	uint64_t cyc = 0;
	auto clk = [&]() { t->line = level(cyc); t->clk = 0; t->eval(); t->clk = 1; t->eval(); t->rd = 0; t->wr = 0; cyc++; };
	auto wr = [&](int r, int v) { t->rs = r; t->din = v; t->wr = 1; clk(); };
	t->bypass = bypass; t->reset = 1; t->rd = 0; t->wr = 0; t->line = 1;
	for (int i = 0; i < 16; i++) clk();
	t->reset = 0;
	wr(RSR, 0x01);                                   // receiver enable
	int overruns = 0;
	uint64_t pause_end = 4000 + uint64_t(PAUSE_MS * 80000), end = 4000 + uint64_t(sent.size() + 200) * 10 * BIT + pause_end;
	int poll = 0;
	while (cyc < end) {
		// the CPU: paused from 1/4 of the stream (after some bytes were read normally)
		bool paused = cyc > 4000 + 100 * 10 * BIT && cyc < 4000 + 100 * 10 * BIT + pause_end;
		if (!paused && ++poll >= 400) {              // polls RSR every 5 us
			poll = 0;
			t->rs = RSR; t->eval();
			int rsr = t->dout;
			if (rsr & 0x40) overruns++;
			if (rsr & 0x80) { t->rs = UDR; t->eval(); got.push_back(t->dout); t->rd = 1; }
			else if (rsr & 0x40) { t->rs = RSR; t->rd = 1; }   // reading RSR clears OE
		}
		clk();
	}
	int ok = 0;
	while (ok < (int)got.size() && ok < (int)sent.size() && got[ok] == sent[ok]) ok++;
	fprintf(stderr, "%s: received %zu of %zu (%d in order), overruns %d\n",
	        bypass ? "bypass" : "midi_in", got.size(), sent.size(), ok, overruns);
	delete t;
	return (got.size() == sent.size() && ok == (int)sent.size() && overruns == 0) ? 0 : 1;
}
