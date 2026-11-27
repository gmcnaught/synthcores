// Sound bench: snd_ctrl (the core's sound subsystem: write FIFO, sequencer, synthframe_mem_arb /
// synthframe_mem_ddr3, proteus_snd) with a DDR3 model (random BUSY, random read latency).
// Loads a demo image (tools/make_demo.py, tools/snd_randimg.py) through ioctl as the core does,
// then plays its event stream into the CPU write port: the events for output sample k are
// written (one per clock, as the 68000's bus FSM would) after sample k-1 and before tick k, so
// the FIFO drains them in front of sample k, the same timing as tools/proteus_model.py.
// Writes the outputs as raw s16 LE L/R and checks the DDR3 capture ring and the status words.
//
// Usage: Vsnd_ctrl DEMO.bin OUT.raw SAMPLES [seed] [--no-tone] [--status-test]
//   --status-test: for a build with a short sample period (VFLAGS=-GSAMPLE_DIV=128, so most
//   ticks are missed): resets the sound subsystem once at sample 8192 and checks the status
//   words: missed deadlines saturate at 0xFFFF instead of wrapping, the reset epoch counts
//   both resets (review 2026-11-26 proteus1.md#16). No output comparison in this mode.
#include "Vsnd_ctrl.h"
#include "verilated.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <deque>
#include <vector>

static std::vector<uint8_t> mem(0x2000000); // 32 MB window at 0x30000000

static uint64_t ddr_word(uint32_t off) {
	uint64_t v = 0;
	for (int i = 0; i < 8; i++) v |= uint64_t(mem[off + i]) << (8 * i);
	return v;
}

int main(int argc, char **argv) {
	if (argc < 4) { fprintf(stderr, "usage: %s DEMO.bin OUT.raw SAMPLES [seed] [--no-tone]\n", argv[0]); return 2; }
	FILE *f = fopen(argv[1], "rb"); if (!f) { fprintf(stderr, "cannot open %s\n", argv[1]); return 2; }
	std::vector<uint8_t> img; int c; while ((c = fgetc(f)) != EOF) img.push_back(uint8_t(c)); fclose(f);
	if (img.size() < 0x800004) { fprintf(stderr, "image too short\n"); return 2; }
	std::vector<uint32_t> events;
	for (size_t a = 0x800000; a + 4 <= img.size(); a += 4)
		events.push_back(img[a] | img[a + 1] << 8 | img[a + 2] << 16 | uint32_t(img[a + 3]) << 24);
	FILE *out = fopen(argv[2], "wb"); if (!out) return 2;
	long want = atol(argv[3]);
	srand(argc > 4 && argv[4][0] != '-' ? atoi(argv[4]) : 1);
	bool tone = true, status_test = false;
	for (int i = 4; i < argc; i++) {
		if (!strcmp(argv[i], "--no-tone")) tone = false;
		if (!strcmp(argv[i], "--status-test")) status_test = true;
	}

	Verilated::commandArgs(argc, argv);
	Vsnd_ctrl *t = new Vsnd_ctrl;
	struct rd { uint32_t addr; int beats; int delay; };
	std::deque<rd> reads;
	uint64_t cycles = 0;
	auto cycle = [&]() {
		t->DDRAM_DOUT_READY = 0;
		if (!reads.empty()) {
			rd &r = reads.front();
			if (r.delay > 0) r.delay--;
			else {
				t->DDRAM_DOUT = ddr_word((r.addr - 0x6000000u) * 8u); t->DDRAM_DOUT_READY = 1;
				r.addr++; if (--r.beats == 0) reads.pop_front();
			}
		}
		t->DDRAM_BUSY = (rand() % 8) == 0;
		t->clk = 0; t->eval();
		if (!t->DDRAM_BUSY) {
			if (t->DDRAM_RD) reads.push_back({uint32_t(t->DDRAM_ADDR), int(t->DDRAM_BURSTCNT), 12 + rand() % 28});
			if (t->DDRAM_WE) {
				uint32_t off = (uint32_t(t->DDRAM_ADDR) - 0x6000000u) * 8u;
				for (int i = 0; i < 8; i++) if (t->DDRAM_BE >> i & 1) mem[off + i] = uint8_t(t->DDRAM_DIN >> (8 * i));
			}
		}
		t->clk = 1; t->eval();
		cycles++;
	};

	t->reset = 1; t->loading = 0; t->tone_en = tone; t->ioctl_wr = 0; t->cpu_wr = 0; t->cpu_rb = 0;
	t->cpu_pan = 0; t->cpu_mask = 0xFFFF; t->disp_voice = 0;
	for (int i = 0; i < 16; i++) cycle();
	// download: the sample part (< 8 MB) goes to DDR3 through snd_ctrl's loader
	t->loading = 1;
	for (int i = 0; i < 4; i++) cycle();
	for (uint32_t a = 0; a < 0x800000; a++) {
		while (t->ioctl_wait) cycle();
		t->ioctl_addr = a; t->ioctl_dout = img[a]; t->ioctl_wr = 1; cycle(); t->ioctl_wr = 0; cycle();
	}
	for (int i = 0; i < 256; i++) cycle();
	t->loading = 0;
	if (memcmp(mem.data(), img.data(), 0x800000)) { fprintf(stderr, "FAIL: load mismatch\n"); return 1; }
	for (int i = 0; i < 16; i++) cycle();

	// play
	t->reset = 0;
	size_t ev = 0; long wait = 0;
	long got = 0, writes = 0; int full_stalls = 0;
	std::vector<uint16_t> mags;   // max(|L|, |R|) per sample since the last reset
	auto push_events = [&]() {
		// the events for the next output sample (tools/proteus_model.py run())
		if (wait) { wait--; return; }
		for (;;) {
			uint32_t e = events[ev];
			if (e == 0) { ev = 0; continue; }
			ev++;
			if (!(e & 0x80000000u)) { wait = long(e) - 1; return; }
			while (t->cpu_wr_full) { full_stalls++; cycle(); }
			t->cpu_wr = 1;
			t->cpu_pan = (e >> 30) & 1;
			t->cpu_voice = (e >> 25) & 31;
			t->cpu_word = (e >> 21) & 15;
			t->cpu_data = e & 0xFFFF;
			t->cpu_mask = (e & 0x20000 ? 0 : 0xFF00) | (e & 0x10000 ? 0 : 0x00FF);
			cycle();
			t->cpu_wr = 0;
			writes++;
		}
	};
	push_events();
	while (got < want) {
		cycle();
		if (t->sample_strobe) {
			int16_t s[2] = { int16_t(t->out_l), int16_t(t->out_r) };
			fwrite(s, 2, 2, out); got++;
			mags.push_back(uint16_t(std::max(std::abs(int(s[0])), std::abs(int(s[1])))));
			if (status_test && got == 8192) {
				t->reset = 1; for (int i = 0; i < 16; i++) cycle(); t->reset = 0;
				mags.clear();
			}
			if (got < want) push_events();
		}
	}
	for (int i = 0; i < 4096; i++) cycle(); // let the last capture and status writes land
	fclose(out);
	uint64_t st0 = ddr_word(0xFFFF00), st1 = ddr_word(0xFFFF08);
	fprintf(stderr, "%ld samples, %ld writes, %llu cycles, FIFO-full stalls %d\n", got, writes,
	        (unsigned long long)cycles, full_stalls);
	fprintf(stderr, "status: samples %u, missed %u, worst %u clocks of 2048; epoch %u, peak %u, magic %04x\n",
	        unsigned(st0 & 0xFFFFFFFF), unsigned(st0 >> 32 & 0xFFFF), unsigned(st0 >> 48),
	        unsigned(st1 & 0xFFFF), unsigned(st1 >> 16 & 0xFFFF), unsigned(st1 >> 32 & 0xFFFF));
	int rc = 0;
	unsigned missed = st0 >> 32 & 0xFFFF, epoch = st1 & 0xFFFF, peak = st1 >> 16 & 0xFFFF, magic = st1 >> 32 & 0xFFFF;
	if (mags.size() >= 4096) {
		size_t end = mags.size() / 4096 * 4096;
		unsigned want_peak = *std::max_element(mags.begin() + (end - 4096), mags.begin() + end);
		if (magic != 0x5031) { fprintf(stderr, "FAIL: second status word missing (magic %04x)\n", magic); rc = 1; }
		if (epoch != (status_test ? 2u : 1u)) { fprintf(stderr, "FAIL: reset epoch %u\n", epoch); rc = 1; }
		if (peak != want_peak) { fprintf(stderr, "FAIL: status peak %u, output peak %u\n", peak, want_peak); rc = 1; }
		if ((st0 & 0xFFFFFFFF) != end) { fprintf(stderr, "FAIL: status samples %u, want %zu\n", unsigned(st0 & 0xFFFFFFFF), end); rc = 1; }
	}
	if (status_test) {
		if (missed != 0xFFFF) { fprintf(stderr, "FAIL: missed deadlines %u, want saturated 65535\n", missed); rc = 1; }
		else fprintf(stderr, "status test: missed saturated, epoch %u, peak %u\n", epoch, peak);
		delete t;
		return rc;
	}
	if (full_stalls) { fprintf(stderr, "FAIL: the write FIFO filled (more than 63 writes in one sample)\n"); rc = 1; }
	if (got >= 4096 && missed) { fprintf(stderr, "FAIL: missed sample deadlines\n"); rc = 1; }
	// the capture ring must hold the same samples (pairs are written every 2 samples)
	long cap = (got / 2) * 2;
	std::vector<uint8_t> o(cap * 4);
	FILE *chk = fopen(argv[2], "rb");
	if (!chk || fread(o.data(), 1, o.size(), chk) != o.size()) { fprintf(stderr, "FAIL: cannot reread output\n"); return 1; }
	fclose(chk);
	if (memcmp(mem.data() + 0x1000000, o.data(), o.size())) { fprintf(stderr, "FAIL: capture ring differs from the output\n"); rc = 1; }
	else fprintf(stderr, "capture ring matches %ld samples\n", cap);
	delete t;
	return rc;
}
