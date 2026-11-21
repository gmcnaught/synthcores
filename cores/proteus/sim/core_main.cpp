// Core-level testbench (top = proteus_core, the same wiring as the FPGA).
// Usage: Vproteus_core IMAGE SECONDS WRITES.txt SAVE_IN|- SAVE_OUT [MIDI.txt]
//   SAVE_IN: existing 64 KB save file to mount, or "-" for a new (empty) save file.
//   SAVE_OUT: the save file contents after the run (only written if the core saved).
// Emulates the HPS: ioctl download of IMAGE (index 1; the sample part goes straight into the
// DDR3 model), save-file mount during the download (as Main_MiSTer does for FS options), and
// SD block reads/writes (16 KB requests, byte by byte through sd_buff_*).
#include "Vproteus_core.h"
#include "verilated.h"
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <deque>
#include <vector>

static std::vector<uint8_t> mem(0x2000000);
static const double CLK = 80e6;

int main(int argc, char **argv) {
	if (argc < 6) { fprintf(stderr, "usage\n"); return 2; }
	FILE *f = fopen(argv[1], "rb"); if (!f) return 2;
	std::vector<uint8_t> img; int c; while ((c = fgetc(f)) != EOF) img.push_back(uint8_t(c)); fclose(f);
	double secs = atof(argv[2]);
	FILE *wlog = fopen(argv[3], "w");
	std::vector<uint8_t> save;
	if (strcmp(argv[4], "-")) { FILE *s = fopen(argv[4], "rb"); while ((c = fgetc(s)) != EOF) save.push_back(uint8_t(c)); fclose(s); }
	std::deque<std::pair<uint64_t, int>> midi;
	if (argc > 6) {
		FILE *m = fopen(argv[6], "r"); char line[512];
		while (m && fgets(line, sizeof line, m)) {
			char *p = line; double t = strtod(p, &p); uint64_t cyc = uint64_t(t * CLK); unsigned b; int n;
			while (sscanf(p, " %x%n", &b, &n) == 1) { p += n;
				midi.push_back({cyc, 0}); cyc += 2560;
				for (int i = 0; i < 8; i++) { midi.push_back({cyc, int(b >> i & 1)}); cyc += 2560; }
				midi.push_back({cyc, 1}); cyc += 2560; }
		}
	}
	memcpy(mem.data(), img.data(), 0x800000);

	Verilated::commandArgs(argc, argv);
	Verilated::assertOn(false);
	Vproteus_core *t = new Vproteus_core;
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
		if (t->dbg_wr_strobe) fprintf(wlog, "%.6f %06x %04x %d\n", cycle / CLK, t->dbg_wr_addr, t->dbg_wr_data, t->dbg_wr_ds);
		cycle++;
	};
	// SD slot service (HPS side)
	int saved_chunks = 0, restored_chunks = 0;
	auto sd_service = [&]() {
		if (t->sd_rd || t->sd_wr) {
			bool wr = t->sd_wr; uint32_t base = t->sd_lba * 512; int n = (t->sd_blk_cnt + 1) * 512;
			t->sd_ack = 1; for (int i = 0; i < 4; i++) step();
			for (int i = 0; i < n; i++) {
				t->sd_buff_addr = i;
				if (wr) { step(); step(); step();
					if (save.size() < base + i + 1) save.resize(base + i + 1);
					save[base + i] = t->sd_buff_din; }
				else { t->sd_buff_dout = base + i < save.size() ? save[base + i] : 0; t->sd_buff_wr = 1; step(); t->sd_buff_wr = 0; step(); }
			}
			t->sd_ack = 0; for (int i = 0; i < 4; i++) step();
			if (wr) saved_chunks++; else restored_chunks++;
			fprintf(stderr, "%.3f SD %s lba %u (%d bytes)\n", cycle / CLK, wr ? "write" : "read", t->sd_lba, n);
		}
	};

	t->reset_in = 1; t->midi_in = 1; t->ioctl_wr = 0; t->ioctl_download = 0; t->ioctl_index = 1;
	t->volume_sel = 0; t->gain_sel = 0; t->save_req = 0; t->autosave = 1; t->osd_open = 0; t->factory_reset = 0;
	t->ps2_key = 0; t->joy = 0; t->sd_ack = 0; t->sd_buff_wr = 0; t->img_mounted = 0;
	for (int i = 0; i < 64; i++) step();
	t->reset_in = 0;
	t->ioctl_download = 1;
	for (size_t a = 0x800000; a < img.size(); a++) {
		while (t->ioctl_wait) step();
		t->ioctl_addr = a; t->ioctl_dout = img[a]; t->ioctl_wr = 1; step(); t->ioctl_wr = 0; step();
	}
	// Main_MiSTer mounts the save file before ending the download
	t->img_size = save.size(); t->img_readonly = 0; t->img_mounted = 1; step(); t->img_mounted = 0;
	for (int i = 0; i < 64; i++) step();
	t->ioctl_download = 0;
	uint64_t end = cycle + uint64_t(secs * CLK);
	while (cycle < end) { step(); sd_service(); }
	fclose(wlog);
	fprintf(stderr, "%.3f s; restored %d chunks, saved %d chunks\n", cycle / CLK, restored_chunks, saved_chunks);
	{
		uint64_t st = 0; for (int i = 0; i < 8; i++) st |= uint64_t(mem[0xFFFF00 + i]) << (8 * i);
		fprintf(stderr, "status: samples %u, missed %u, worst %u clocks of 2048\n",
		        unsigned(st & 0xFFFFFFFF), unsigned(st >> 32 & 0xFFFF), unsigned(st >> 48));
	}
	if (saved_chunks) { FILE *s = fopen(argv[5], "wb"); fwrite(save.data(), 1, save.size(), s); fclose(s); }
	delete t;
	return 0;
}
