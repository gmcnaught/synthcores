// SPDX-License-Identifier: GPL-3.0-or-later
// Unit bench for hdl/p2k_imgsave.sv: the flash save file (image 0) and the flash SIMM files (images
// 1, 2) against hps_io's SD side as Main_MiSTer serves it (one request at a time; a read past the end
// of a file gives zeros; a write past the end of a SIMM file is dropped, the save file grows) and a
// DDR3 model (random ready and read latency, responses in order, fills). The memory client is granted
// after a random delay, as when the loader's copier holds it. Steps, each checked against a model of
// the files and memory:
//   1. a system pack download with the save file mounted: restore, boot_hold falls;
//   2. the OS changes the flash; autosave on OSD open writes the save file;
//   3. flash SIMM files mounted in slots 1 and 2 (one of an odd size, part of the slot past its end):
//      loaded, the rest of the slot erased, simm_loaded;
//   4. the OS changes regions of both slots, some marked dirty (fl_dirty), some not: a save writes back
//      exactly the marked regions, within each file;
//   5. a ROM SIMM download into slot 1 during a reload stops it; the slot is no longer flash;
//   6. a read-only SIMM file is loaded and never written; an empty mount leaves the slot empty;
//   7. a pack download during a save of the save file stops the save.
//   imgsave_tb [--seed N] [--full]   (--full: a 34 MB SIMM file)
#include "Vp2k_imgsave.h"
#include "verilated.h"

#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <memory>
#include <random>
#include <string>
#include <vector>

namespace {
constexpr uint64_t SMP_BASE = 0x6000000, BABY_BASE = 0x7100000, CPU_BASE = 0x7200000;
constexpr uint64_t MEM_ORG = 0x6000000, MEM_WORDS = 0x1280000;   // SMP_BASE .. CPU_BASE + 2 MB
constexpr size_t SIMM_BYTES = 0x2200000;
}

int main(int argc, char **argv)
{
	unsigned seed = 1;
	bool full = false;
	for (int i = 1; i < argc; i++)
	{
		const std::string a = argv[i];
		if (a == "--seed" && i + 1 < argc) seed = unsigned(std::atoi(argv[++i]));
		else if (a == "--full") full = true;
	}
	std::mt19937_64 rng(seed);
	std::vector<uint64_t> mem(MEM_WORDS);
	for (auto &w : mem) w = rng();
	auto M = [&](uint64_t a) -> uint64_t & {
		if (a < MEM_ORG || a >= MEM_ORG + MEM_WORDS) { std::fprintf(stderr, "memory word %" PRIx64 " outside the model\n", a); std::exit(1); }
		return mem[a - MEM_ORG];
	};
	// 16-bit word i of a region (lane i mod 4)
	auto get16 = [&](uint64_t base, size_t i) { return uint16_t(M(base + (i >> 2)) >> (16 * (i & 3))); };
	auto put16 = [&](uint64_t base, size_t i, uint16_t v) {
		uint64_t &w = M(base + (i >> 2));
		w = (w & ~(0xffffull << (16 * (i & 3)))) | (uint64_t(v) << (16 * (i & 3)));
	};

	// files (Main_MiSTer's view)
	struct image { std::vector<uint8_t> f; bool grow = false; long writes = 0; };
	image img[3];
	img[0].grow = true;

	auto ctx = std::make_unique<VerilatedContext>();
	auto top = std::make_unique<Vp2k_imgsave>(ctx.get());
	bool bad = false;
	uint64_t cyc = 0;
	bool owned = false;
	std::deque<std::pair<uint64_t, uint64_t>> rdq;    // (due, data)
	uint64_t last_due = 0;
	// SD: one request at a time
	int sd_disk = -1, sd_ph = 0, sd_wait = 0, sd_lag = 0;
	bool sd_write = false;
	uint32_t sd_lba = 0, sd_n = 0, sd_i = 0;
	std::vector<uint8_t> sd_buf;
	long simm_loaded = 0;
	auto clock = [&]() {
		// memory
		top->req_ready = rng() % 3 != 0;
		top->rsp_valid = 0;
		if (!rdq.empty() && rdq.front().first <= cyc)
		{
			top->rsp_valid = 1;
			top->rsp_data = rdq.front().second;
			rdq.pop_front();
		}
		// grant after a random delay; kept while want is high
		if (!owned && top->want && rng() % 8 == 0) owned = true;
		top->grant = owned;
		// SD
		top->sd_buff_wr = 0;
		if (sd_disk < 0)
		{
			top->sd_ack = 0;
			for (int d = 0; d < 3 && sd_disk < 0; d++)
				if (((top->sd_rd | top->sd_wr) >> d) & 1)
				{
					sd_disk = d;
					sd_write = (top->sd_wr >> d) & 1;
					sd_lba = top->sd_lba;
					sd_n = (top->sd_blk_cnt + 1) * 512;
					sd_i = 0;
					sd_ph = 0;
					sd_wait = 1 + rng() % 40;
					sd_buf.assign(sd_n, 0);
					if (!sd_write)
						for (uint32_t k = 0; k < sd_n; k++)
						{
							const size_t p = size_t(sd_lba) * 512 + k;
							sd_buf[k] = p < img[d].f.size() ? img[d].f[p] : 0;
						}
				}
		}
		else if (sd_wait > 0)
			sd_wait--;
		else if (sd_ph == 0)
		{
			top->sd_ack = 1 << sd_disk;
			sd_ph = 1;
		}
		else if (sd_ph == 1)
		{
			// one byte every 1-3 clocks; a write samples sd_buff_din two clocks after the address
			if (!sd_write)
			{
				if (rng() % 2)
				{
					top->sd_buff_addr = sd_i;
					top->sd_buff_dout = sd_buf[sd_i];
					top->sd_buff_wr = 1;
					if (++sd_i == sd_n) sd_ph = 3;
				}
			}
			else
			{
				top->sd_buff_addr = sd_i;
				sd_ph = 2;
				sd_lag = 0;
			}
		}
		else if (sd_ph == 2)
		{
			if (++sd_lag == 2)
			{
				sd_buf[sd_i] = top->sd_buff_din;
				sd_lag = 0;
				sd_ph = ++sd_i == sd_n ? 3 : 1;
			}
		}
		else if (sd_ph == 3)
		{
			top->sd_ack = 0;
			if (sd_write)
			{
				image &m = img[sd_disk];
				m.writes++;
				for (uint32_t k = 0; k < sd_n; k++)
				{
					const size_t p = size_t(sd_lba) * 512 + k;
					if (p >= m.f.size())
					{
						if (!m.grow) break;
						m.f.resize(p + 1, 0);
					}
					m.f[p] = sd_buf[k];
				}
			}
			sd_disk = -1;
			// hps_io drops sd_ack a clock later than the last byte
		}
		top->clk = 0;
		top->eval();
		if (top->req_valid && top->req_ready)
		{
			if (!owned) { std::fprintf(stderr, "memory request without the grant\n"); bad = true; }
			const unsigned len = top->req_len ? top->req_len : 1;
			if (top->req_we)
				for (unsigned k = 0; k < len; k++) M(top->req_addr + k) = top->req_wdata;
			else
				for (unsigned k = 0; k < len; k++)
				{
					last_due = std::max<uint64_t>(cyc + 2 + rng() % 30, last_due + 1);
					rdq.push_back({last_due, M(top->req_addr + k)});
				}
		}
		if (top->simm_loaded) simm_loaded++;
		top->clk = 1;
		top->eval();
		if (owned && !top->want && rdq.empty()) owned = false;
		cyc++;
	};
	auto run = [&](uint64_t n) { for (uint64_t i = 0; i < n && !bad; i++) clock(); };
	auto until = [&](auto cond, uint64_t limit, const char *what) {
		for (uint64_t i = 0; !cond(); i++)
		{
			clock();
			if (i > limit || bad) { std::fprintf(stderr, "timeout: %s\n", what); bad = true; return; }
		}
	};
	auto mount = [&](int d, const std::vector<uint8_t> &f, bool ro) {
		img[d].f = f;
		img[d].writes = 0;
		top->img_readonly = ro;
		top->img_size = f.size();
		top->img_mounted = 1 << d;
		clock();
		top->img_mounted = 0;
		clock();
	};
	auto idle = [&]() { return !top->want && !top->saving && !top->simm_busy && rdq.empty(); };

	// image 0's file <-> the flash region
	auto check_flash = [&](const char *step) {
		for (size_t i = 0; i < 0x100000; i++)
		{
			const uint16_t f = uint16_t(img[0].f[2 * i] << 8 | img[0].f[2 * i + 1]);
			if (get16(CPU_BASE, i) != f)
			{
				std::fprintf(stderr, "%s: flash word %05zx %04x, file %04x\n", step, i, get16(CPU_BASE, i), f);
				bad = true;
				return;
			}
		}
	};
	// a SIMM slot against a file: Baby then Big, erased past the file's end
	auto slot_word = [&](int s, size_t b) -> uint16_t {        // file byte offset b (even)
		return b < 0x200000 ? get16(BABY_BASE + (uint64_t(s) << 18), b / 2) : get16(SMP_BASE + (uint64_t(s) << 22), (b - 0x200000) / 2);
	};
	auto check_slot = [&](int s, const std::vector<uint8_t> &f, const char *step) {
		for (size_t b = 0; b < SIMM_BYTES; b += 2)
		{
			const uint16_t want = uint16_t((b < f.size() ? f[b] : 0xff) << 8 | (b + 1 < f.size() ? f[b + 1] : 0xff));
			if (slot_word(s, b) != want)
			{
				std::fprintf(stderr, "%s: slot %d byte %07zx %04x, expected %04x\n", step, s + 1, b, slot_word(s, b), want);
				bad = true;
				return;
			}
		}
	};
	auto rand_file = [&](size_t n) { std::vector<uint8_t> v(n); for (auto &x : v) x = uint8_t(rng()); return v; };

	top->rst = 1;
	top->loading = 0; top->image_ok = 0; top->rom_load = 0; top->img_mounted = 0; top->sd_ack = 0;
	top->save_req = 0; top->osd_open = 0; top->autosave = 1; top->flash_changed = 0; top->fl_dirty = 0;
	run(8);
	top->rst = 0;
	run(8);

	// 1. pack download, save file mounted at its end, restore
	top->loading = 1;
	run(100);
	mount(0, rand_file(0x200000), false);
	run(50);
	top->loading = 0;
	top->image_ok = 1;
	until([&] { return !top->boot_hold; }, 40000000, "restore");
	check_flash("restore");
	std::printf("1. restore: %s (%" PRIu64 " clocks)\n", bad ? "FAIL" : "ok", cyc);

	// 2. the OS changes the flash; autosave on OSD open
	for (int k = 0; k < 100; k++) put16(CPU_BASE, rng() % 0x100000, uint16_t(rng()));
	top->flash_changed = 1; clock(); top->flash_changed = 0;
	top->osd_open = 1;
	run(4);
	until([&] { return idle(); }, 40000000, "save 0");
	top->osd_open = 0;
	check_flash("autosave");
	std::printf("2. autosave: %s, %ld SD writes\n", bad ? "FAIL" : "ok", img[0].writes);

	// 3. flash SIMM files: slot 1 odd-sized, slot 2 a whole SIMM or a few MB
	const auto f1 = rand_file(0x123457);
	const auto f2 = rand_file(full ? SIMM_BYTES : 0x400000);
	mount(1, f1, false);
	if (!top->simm_busy || !(top->slot_flash & 1)) { std::fprintf(stderr, "slot 1 not busy/flash after the mount\n"); bad = true; }
	mount(2, f2, false);
	until([&] { return idle(); }, 2000000000, "SIMM loads");
	run(4);
	check_slot(0, f1, "load 1");
	check_slot(1, f2, "load 2");
	if (simm_loaded != 2 || top->slot_flash != 3 || top->slot_empty != 0) { std::fprintf(stderr, "loaded %ld, flash %d\n", simm_loaded, top->slot_flash); bad = true; }
	std::printf("3. loads: %s (%" PRIu64 " clocks)\n", bad ? "FAIL" : "ok", cyc);

	// 4. changes in both slots; marked regions are written back, within the file
	std::vector<bool> mk[2] = { std::vector<bool>(136), std::vector<bool>(136) };
	auto change = [&](int s, bool always_mark) {
		const unsigned region = rng() % 136;
		const bool mark = always_mark || rng() % 4 != 0;
		for (int j = 0; j < 20; j++)
		{
			const size_t b = (size_t(region) << 18) + 2 * (rng() % 0x20000);
			const uint16_t v = uint16_t(rng());
			if (b < 0x200000) put16(BABY_BASE + (uint64_t(s) << 18), b / 2, v);
			else put16(SMP_BASE + (uint64_t(s) << 22), (b - 0x200000) / 2, v);
		}
		if (mark)
		{
			top->fl_dirty = 1; top->fl_dirty_slot = s; top->fl_dirty_region = region;
			clock();
			top->fl_dirty = 0;
			mk[s][region] = true;
		}
	};
	for (int k = 0; k < 40; k++) change(k & 1, false);
	change(0, true);                                     // a region inside the odd-sized file's last chunk
	top->fl_dirty = 1; top->fl_dirty_slot = 0; top->fl_dirty_region = 0x123457 >> 18;
	clock();
	top->fl_dirty = 0;
	mk[0][0x123457 >> 18] = true;
	top->fl_dirty = 1; top->fl_dirty_slot = 2; top->fl_dirty_region = 3;   // slots 3-4 are not tracked
	clock();
	top->fl_dirty = 0;
	auto expect = [&](int s, std::vector<uint8_t> e) {
		for (size_t b = 0; b < e.size(); b += 2)
			if (mk[s][b >> 18])
			{
				const uint16_t w = slot_word(s, b);
				e[b] = uint8_t(w >> 8);
				if (b + 1 < e.size()) e[b + 1] = uint8_t(w);
			}
		return e;
	};
	const auto e1 = expect(0, f1), e2 = expect(1, f2);
	top->save_req = 1;
	run(4);
	until([&] { return idle(); }, 2000000000, "SIMM save");
	top->save_req = 0;
	run(4);
	for (int s = 0; s < 2; s++)
	{
		const auto &e = s ? e2 : e1;
		const auto &f = img[s + 1].f;
		if (f.size() != e.size()) { std::fprintf(stderr, "slot %d file size %zu, expected %zu\n", s + 1, f.size(), e.size()); bad = true; }
		for (size_t b = 0; b < e.size() && !bad; b++)
			if (f[b] != e[b]) { std::fprintf(stderr, "slot %d file byte %07zx %02x, expected %02x\n", s + 1, b, f[b], e[b]); bad = true; }
	}
	int nm = 0;
	for (int s = 0; s < 2; s++) for (bool x : mk[s]) nm += x;
	std::printf("4. SIMM save: %s, %d regions marked, %ld + %ld SD writes (save file: %ld)\n",
	            bad ? "FAIL" : "ok", nm, img[1].writes, img[2].writes, img[0].writes);

	// 5. a ROM SIMM download into slot 1 during a reload stops it
	const long loaded0 = simm_loaded;
	mount(1, f1, false);
	run(200000);
	top->rom_load = 1; clock(); top->rom_load = 0;
	until([&] { return idle(); }, 40000000, "stopped load");
	run(4);
	if (simm_loaded != loaded0 || (top->slot_flash & 1) || (top->slot_empty & 1)) { std::fprintf(stderr, "ROM load: loaded %ld, flash %d empty %d\n", simm_loaded - loaded0, top->slot_flash, top->slot_empty); bad = true; }
	std::printf("5. ROM download during a load: %s\n", bad ? "FAIL" : "ok");

	// 6. read-only SIMM: loaded, never written; an empty mount
	const auto f3 = rand_file(0x300000);
	mount(1, f3, true);
	until([&] { return idle(); }, 2000000000, "read-only load");
	check_slot(0, f3, "read-only load");
	for (int k = 0; k < 4; k++) change(0, true);
	top->save_req = 1; run(4); until([&] { return idle(); }, 40000000, "read-only save"); top->save_req = 0; run(4);
	if (img[1].writes != 0 || img[1].f != f3) { std::fprintf(stderr, "read-only file written (%ld)\n", img[1].writes); bad = true; }
	mount(2, std::vector<uint8_t>(), false);
	run(100);
	if ((top->slot_flash & 2) || !(top->slot_empty & 2)) { std::fprintf(stderr, "empty mount: flash %d empty %d\n", top->slot_flash, top->slot_empty); bad = true; }
	std::printf("6. read-only and empty mounts: %s\n", bad ? "FAIL" : "ok");

	// 7. a pack download during a save of the save file stops it
	img[0].writes = 0;
	for (int k = 0; k < 100; k++) put16(CPU_BASE, rng() % 0x100000, uint16_t(rng()));
	top->save_req = 1;
	until([&] { return img[0].writes >= 3; }, 40000000, "save 0 started");
	top->save_req = 0;
	top->loading = 1;
	until([&] { return idle(); }, 40000000, "save 0 stopped");
	const long w7 = img[0].writes;
	run(100000);
	if (w7 >= 128 || img[0].writes != w7 || !top->boot_hold) { std::fprintf(stderr, "save during a download: %ld writes, boot_hold %d\n", w7, top->boot_hold); bad = true; }
	top->loading = 0;
	until([&] { return !top->boot_hold; }, 1000, "boot_hold without a save file");
	std::printf("7. pack download during a save: %s (%ld of 128 chunks written)\n", bad ? "FAIL" : "ok", w7);

	std::printf("%" PRIu64 " clocks: %s\n", cyc, bad ? "DIFFERENT" : "identical");
	return bad ? 1 : 0;
}
