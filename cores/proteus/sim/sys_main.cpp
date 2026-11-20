// Full-system testbench. Usage: Vsys_top IMAGE SECONDS WRITES.txt [AUDIO.raw] [MIDI.txt]
// IMAGE: tools/make_system.py output. Samples go straight into the DDR3 model; the rest
// (ROM, RAM, font) goes through ioctl. Logs every CPU write: "<seconds> <addr> <data> <ds>".
// MIDI.txt: lines "<seconds> <hex bytes...>" sent to the MFP serial input at 31250 baud.
#include "Vsys_top.h"
#include "verilated.h"
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <deque>
#include <vector>
#include <string>

static std::vector<uint8_t> mem(0x2000000);
static const double CLK = 80e6;

int main(int argc, char **argv) {
	if (argc < 4) { fprintf(stderr, "usage\n"); return 2; }
	FILE *f = fopen(argv[1], "rb"); if (!f) return 2;
	std::vector<uint8_t> img; int c; while ((c = fgetc(f)) != EOF) img.push_back(uint8_t(c)); fclose(f);
	double secs = atof(argv[2]);
	FILE *wlog = fopen(argv[3], "w");
	FILE *aud = argc > 4 ? fopen(argv[4], "wb") : nullptr;
	// MIDI schedule -> bit stream events (cycle, level)
	std::deque<std::pair<uint64_t, int>> midi;
	if (argc > 5) {
		FILE *m = fopen(argv[5], "r"); char line[512];
		while (m && fgets(line, sizeof line, m)) {
			char *p = line; double t = strtod(p, &p); uint64_t cyc = uint64_t(t * CLK);
			unsigned b; int n;
			while (sscanf(p, " %x%n", &b, &n) == 1) {
				p += n;
				midi.push_back({cyc, 0}); cyc += 2560;                       // start bit
				for (int i = 0; i < 8; i++) { midi.push_back({cyc, int(b >> i & 1)}); cyc += 2560; }
				midi.push_back({cyc, 1}); cyc += 2560;                       // stop bit
			}
		}
		if (m) fclose(m);
	}
	memcpy(mem.data(), img.data(), 0x800000);

	Verilated::commandArgs(argc, argv);
	Verilated::assertOn(false);  // fx68k "unique case" checks fire on X-free but undecoded states
	Vsys_top *t = new Vsys_top;
	struct rd { uint32_t addr; int beats; int delay; };
	std::deque<rd> reads;
	uint64_t cycle = 0;
	auto step = [&]() {
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
		while (!midi.empty() && midi.front().first <= cycle) { t->midi_in = midi.front().second; midi.pop_front(); }
		t->clk = 0; t->eval();
		if (!t->DDRAM_BUSY) {
			if (t->DDRAM_RD) reads.push_back({uint32_t(t->DDRAM_ADDR), int(t->DDRAM_BURSTCNT), 12 + rand() % 28});
			if (t->DDRAM_WE) {
				uint32_t off = (uint32_t(t->DDRAM_ADDR) - 0x6000000u) * 8u;
				for (int i = 0; i < 8; i++) if (t->DDRAM_BE >> i & 1) mem[off + i] = uint8_t(t->DDRAM_DIN >> (8 * i));
			}
		}
		t->clk = 1; t->eval();
		cycle++;
	};

	t->reset = 1; t->loading = 1; t->midi_in = 1; t->ioctl_wr = 0;
	for (int i = 0; i < 64; i++) step();
	for (size_t a = 0x800000; a < img.size(); a++) {
		t->ioctl_addr = a; t->ioctl_dout = img[a]; t->ioctl_wr = 1; step(); t->ioctl_wr = 0; step();
	}
	t->loading = 0;
	for (int i = 0; i < 256; i++) step();
	t->reset = 0;
	uint64_t start = cycle, end = cycle + uint64_t(secs * CLK);
	long nwr = 0, nsmp = 0;
	while (cycle < end) {
		step();
		if (t->bus_wr_strobe) {
			fprintf(wlog, "%.6f %06x %04x %d\n", (cycle - start) / CLK, t->bus_wr_addr, t->bus_wr_data, t->bus_wr_ds);
			nwr++;
		}
		if (t->sample_strobe && aud) { int16_t s[2] = { int16_t(t->out_l), int16_t(t->out_r) }; fwrite(s, 2, 2, aud); nsmp++; }
	}
	// LCD contents
	for (int line = 0; line < 2; line++) {
		std::string s;
		for (int pos = 0; pos < 16; pos++) { t->lcd_line = line; t->lcd_pos = pos; t->clk = 0; t->eval(); s += char(t->lcd_code); }
		fprintf(stderr, "LCD %d: [%s]\n", line, s.c_str());
	}
	fprintf(stderr, "%ld writes, %ld samples, %.3f s\n", nwr, nsmp, (cycle - start) / CLK);
	fclose(wlog); if (aud) fclose(aud);
	delete t;
	return 0;
}
