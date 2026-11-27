// ROM-free checks of proteus_core (sim/unit_bench.sv) with a small hand-assembled 68000 program
// instead of the firmware. Usage: Vunit_bench TEST; exit 0 pass, 1 fail. Tests:
//   snd      sound-register write then immediate raw read: returns the written word, as MAME's
//            m_regs (review 2026-11-26 proteus1.md#5); pan byte writes: only those with LDS
//            reach the pan register, as MAME's umask16(0x00ff) (#15)
//   macro    first boot with the preset-initialisation macro: opening the OSD (autosave on)
//            while the macro runs must not start a save (#3)
//   osd      control for 'macro': without the macro, opening the OSD saves
//   factory  OSD "Factory Reset Presets" while a save is in progress: the macro waits for the
//            save to finish instead of running with the CPU paused (#3)
#include "Vunit_bench.h"
#include "verilated.h"
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <deque>
#include <string>
#include <vector>

static const double CLK = 80e6;

// The program: reset vectors (SSP 0xFFFF00, PC 0x100), then at 0x100:
static const uint16_t PROG[] = {
	0x33FC, 0x1234, 0x0080, 0x0026,   // move.w #$1234,$800026   voice 1 word 3
	0x3039, 0x0080, 0x0026,           // move.w $800026,d0        raw read straight after the write
	0x33C0, 0x00FF, 0x0000,           // move.w d0,$FF0000        result A
	0x13FC, 0x0005, 0x0050, 0x0008,   // move.b #5,$500008        UDS only: not a pan write in MAME
	0x13FC, 0x0003, 0x0050, 0x000B,   // move.b #3,$50000B        voice 1 pan (LDS)
	0x33FC, 0x0007, 0x0050, 0x000C,   // move.w #7,$50000C        voice 2 pan (word write)
	0x33FC, 0xABCD, 0x0080, 0x0046,   // move.w #$ABCD,$800046   voice 2 word 3
	0x3039, 0x0080, 0x0046,           // move.w $800046,d0
	0x33C0, 0x00FF, 0x0002,           // move.w d0,$FF0002        result B
	0x33FC, 0x0001, 0x00FF, 0x0004,   // move.w #1,$FF0004        done
	0x60FE,                           // bra.s *
};

int main(int argc, char **argv) {
	if (argc < 2) { fprintf(stderr, "usage: %s snd|macro|osd|factory\n", argv[0]); return 2; }
	std::string test = argv[1];
	Verilated::commandArgs(argc, argv);
	Verilated::assertOn(false);   // fx68kAlu has an unreachable-case assertion, as in core_main.cpp
	Vunit_bench *t = new Vunit_bench;
	std::vector<uint8_t> mem(0x2000000);
	struct rd { uint32_t addr; int beats; int delay; };
	std::deque<rd> reads;
	uint64_t cycle = 0;
	struct wr { uint32_t addr; uint16_t data; int ds; };
	std::vector<wr> writes;
	struct fw { int pan, voice; uint16_t data; };
	std::vector<fw> fifo;
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
		t->clk = 0; t->eval();
		if (!t->DDRAM_BUSY) {
			if (t->DDRAM_RD) reads.push_back({uint32_t(t->DDRAM_ADDR), int(t->DDRAM_BURSTCNT), 12 + rand() % 28});
			if (t->DDRAM_WE) {
				uint32_t off = (uint32_t(t->DDRAM_ADDR) - 0x6000000u) * 8u;
				for (int i = 0; i < 8; i++) if (t->DDRAM_BE >> i & 1) mem[off + i] = uint8_t(t->DDRAM_DIN >> (8 * i));
			}
		}
		t->clk = 1; t->eval();
		if (t->dbg_wr_strobe) writes.push_back({t->dbg_wr_addr, uint16_t(t->dbg_wr_data), t->dbg_wr_ds});
		if (t->fifo_wr) fifo.push_back({t->fifo_pan, t->fifo_voice, uint16_t(t->fifo_data)});
		cycle++;
	};
	auto run_ms = [&](double ms) { uint64_t end = cycle + uint64_t(ms * 1e-3 * CLK); while (cycle < end) step(); };

	bool init = test == "macro";
	bool mount = test != "snd";
	t->reset_in = 1; t->ioctl_download = 0; t->ioctl_index = 1; t->ioctl_wr = 0; t->img_mounted = 0;
	t->img_readonly = 0; t->img_size = 0; t->sd_ack = 0; t->sd_buff_wr = 0; t->save_req = 0;
	t->autosave = 1; t->osd_open = 0; t->factory_reset = 0;
	for (int i = 0; i < 64; i++) step();
	t->reset_in = 0;
	// download: program ROM at 0x800000 (big-endian words), flags at 0x831000
	std::vector<std::pair<uint32_t, uint8_t>> bytes;
	const uint32_t vec[2] = {0x00FFFF00, 0x00000100};
	for (int i = 0; i < 8; i++) bytes.push_back({0x800000u + i, uint8_t(vec[i / 4] >> (8 * (3 - i % 4)))});
	for (size_t i = 0; i < sizeof PROG / 2; i++) {
		bytes.push_back({0x800100u + 2 * i, uint8_t(PROG[i] >> 8)});
		bytes.push_back({0x800101u + 2 * i, uint8_t(PROG[i])});
	}
	bytes.push_back({0x831000u, uint8_t(init)});
	t->ioctl_download = 1;
	for (auto &b : bytes) {
		while (t->ioctl_wait) step();
		t->ioctl_addr = b.first; t->ioctl_dout = b.second; t->ioctl_wr = 1; step(); t->ioctl_wr = 0; step();
	}
	if (mount) { t->img_size = 0; t->img_mounted = 1; step(); t->img_mounted = 0; }   // new, empty save file
	for (int i = 0; i < 64; i++) step();
	t->ioctl_download = 0;

	int rc = 0;
	auto fail = [&](const char *msg) { fprintf(stderr, "FAIL %s: %s\n", test.c_str(), msg); rc = 1; };

	if (test == "snd") {
		run_ms(2);
		uint16_t a = 0, b = 0; bool done = false;
		for (auto &w : writes) {
			if (w.addr == 0xFF0000) a = w.data;
			if (w.addr == 0xFF0002) b = w.data;
			if (w.addr == 0xFF0004) done = true;
		}
		if (!done) fail("program did not finish");
		fprintf(stderr, "raw read-back after write: %04x (want 1234), %04x (want abcd)\n", a, b);
		if (a != 0x1234 || b != 0xABCD) fail("raw sound-register read returned a stale word (#5)");
		std::string pans;
		for (auto &f : fifo) if (f.pan) pans += " v" + std::to_string(f.voice) + "=" + std::to_string(f.data & 0xFF);
		fprintf(stderr, "pan writes:%s (want v1=3 v2=7)\n", pans.c_str());
		if (pans != " v1=3 v2=7") fail("pan writes differ from MAME's umask16(0x00ff) (#15)");
	}
	else {
		// SD slot 0 service; 'hold' keeps sd_ack low on the first write request (a slow SD card)
		bool saw_wr = false, wr_in_macro = false, macro_in_pause = false, macro_after = false;
		auto watch = [&]() {
			if (t->sd_wr) { saw_wr = true; if (t->macro_active) wr_in_macro = true; }
			if (t->macro_active && t->cpu_pause) macro_in_pause = true;
		};
		auto sd_service = [&](bool hold_first) {
			static bool held = false;
			if (!(t->sd_rd || t->sd_wr)) return;
			if (hold_first && !held) {
				held = true;
				// stretch the first request: Factory Reset arrives while the save is under way
				for (int i = 0; i < 40000; i++) { step(); watch(); }
				t->factory_reset = 1;
				for (int i = 0; i < 40000; i++) { step(); watch(); }
			}
			int n = (t->sd_blk_cnt + 1) * 512;
			t->sd_ack = 1; for (int i = 0; i < 4; i++) { step(); watch(); }
			for (int i = 0; i < n; i++) { t->sd_buff_addr = i; step(); step(); watch(); }
			t->sd_ack = 0; for (int i = 0; i < 4; i++) { step(); watch(); }
		};
		auto run_watch = [&](double ms, bool hold_first) {
			uint64_t end = cycle + uint64_t(ms * 1e-3 * CLK);
			while (cycle < end) { step(); watch(); sd_service(hold_first); if (t->macro_active && !t->cpu_pause && t->factory_reset) macro_after = true; }
		};
		if (test == "macro") {
			run_ms(1);
			if (!t->macro_active) fail("the initialisation macro did not start");
			t->osd_open = 1;
			run_watch(30, false);
			if (!t->macro_active) fail("the macro ended early");
			fprintf(stderr, "OSD opened during the macro: save started %s\n", wr_in_macro ? "yes" : "no");
			if (wr_in_macro) fail("autosave started while the macro drives the panel (#3)");
		}
		else if (test == "osd") {
			run_ms(1);
			t->osd_open = 1;
			run_watch(10, false);
			fprintf(stderr, "OSD opened (no macro): save started %s\n", saw_wr ? "yes" : "no");
			if (!saw_wr) fail("autosave did not start");
		}
		else if (test == "factory") {
			run_ms(1);
			t->osd_open = 1;
			run_watch(20, true);
			fprintf(stderr, "Factory Reset during a save: macro ran with the CPU paused %s, macro started after %s\n",
			        macro_in_pause ? "yes" : "no", macro_after ? "yes" : "no");
			if (!saw_wr) fail("autosave did not start");
			if (macro_in_pause) fail("the macro ran while the save paused the CPU (#3)");
			if (!macro_after) fail("the macro did not start after the save");
		}
		else { fprintf(stderr, "unknown test\n"); return 2; }
	}
	fprintf(stderr, "%s: %s (%.1f ms simulated)\n", test.c_str(), rc ? "FAIL" : "ok", cycle / CLK * 1e3);
	delete t;
	return rc;
}
