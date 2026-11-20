// Verilator testbench: load the demo image through ioctl, run N output samples with a DDR3
// model (random BUSY, random read latency), write the outputs as raw s16 LE L/R.
// Usage: Vsim_top DEMO.bin OUT.raw SAMPLES [seed]
#include "Vsim_top.h"
#include "verilated.h"
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <deque>
#include <vector>

static std::vector<uint8_t> mem(0x2000000); // 32 MB window at 0x30000000

int main(int argc, char **argv) {
	if (argc < 4) { fprintf(stderr, "usage: %s DEMO.bin OUT.raw SAMPLES [seed]\n", argv[0]); return 2; }
	FILE *f = fopen(argv[1], "rb"); if (!f) return 2;
	std::vector<uint8_t> img; int c; while ((c = fgetc(f)) != EOF) img.push_back(uint8_t(c)); fclose(f);
	FILE *out = fopen(argv[2], "wb");
	long want = atol(argv[3]);
	srand(argc > 4 ? atoi(argv[4]) : 1);

	Verilated::commandArgs(argc, argv);
	Vsim_top *t = new Vsim_top;
	auto tick = [&]() { t->clk = 0; t->eval(); t->clk = 1; t->eval(); };

	// DDR model state
	struct rd { uint32_t addr; int beats; int delay; };
	std::deque<rd> reads;
	auto ddr_cycle = [&]() {
		// outputs for this cycle are computed before the rising edge
		t->DDRAM_DOUT_READY = 0;
		if (!reads.empty()) {
			rd &r = reads.front();
			if (r.delay > 0) r.delay--;
			else {
				uint64_t v = 0; uint32_t off = (r.addr - 0x6000000u) * 8u;
				for (int i = 0; i < 8; i++) v |= uint64_t(mem[off + i]) << (8 * i);
				t->DDRAM_DOUT = v; t->DDRAM_DOUT_READY = 1;
				r.addr++; if (--r.beats == 0) reads.pop_front();
			}
		}
		t->DDRAM_BUSY = (rand() % 8) == 0;
		t->eval();
		// requests accepted at this edge
		if (!t->DDRAM_BUSY) {
			if (t->DDRAM_RD) reads.push_back({uint32_t(t->DDRAM_ADDR), int(t->DDRAM_BURSTCNT), 12 + rand() % 28});
			if (t->DDRAM_WE) {
				uint32_t off = (uint32_t(t->DDRAM_ADDR) - 0x6000000u) * 8u;
				for (int i = 0; i < 8; i++) if (t->DDRAM_BE >> i & 1) mem[off + i] = uint8_t(t->DDRAM_DIN >> (8 * i));
			}
		}
		tick();
	};

	// reset, then download through ioctl
	t->reset = 1; t->run = 0; t->ioctl_download = 0; t->ioctl_wr = 0;
	for (int i = 0; i < 16; i++) ddr_cycle();
	t->ioctl_download = 1;
	for (size_t a = 0; a < img.size(); a++) {
		while (t->ioctl_wait) ddr_cycle();
		t->ioctl_addr = a; t->ioctl_dout = img[a]; t->ioctl_wr = 1; ddr_cycle(); t->ioctl_wr = 0; ddr_cycle();
	}
	for (int i = 0; i < 64; i++) ddr_cycle();
	t->ioctl_download = 0;
	for (size_t a = 0; a < img.size(); a++) if (mem[a] != img[a]) { fprintf(stderr, "load mismatch at %zx\n", a); return 1; }
	fprintf(stderr, "loaded %zu bytes ok\n", img.size());

	t->reset = 0; t->run = 1;
	long got = 0; uint64_t cycles = 0;
	while (got < want) {
		ddr_cycle(); cycles++;
		if (t->snd_done) {
			int16_t s[2] = { int16_t(t->snd_l), int16_t(t->snd_r) };
			fwrite(s, 2, 2, out); got++;
		}
	}
	for (int i = 0; i < 4096; i++) ddr_cycle(); // let the last capture write land
	fclose(out);
	fprintf(stderr, "%ld samples, %llu cycles, overruns %u\n", got, (unsigned long long)cycles, t->overruns);
	// capture ring must hold the same samples (pairs are written every 2 samples)
	long cap = (got / 2) * 2;
	FILE *chk = fopen(argv[2], "rb");
	std::vector<uint8_t> o(cap * 4); fread(o.data(), 1, o.size(), chk); fclose(chk);
	for (long i = 0; i < cap * 4; i++) if (mem[0x1000000 + i] != o[i]) { fprintf(stderr, "capture mismatch at sample %ld\n", i / 4); return 1; }
	fprintf(stderr, "capture ring matches %ld samples\n", cap);
	delete t;
	return 0;
}
