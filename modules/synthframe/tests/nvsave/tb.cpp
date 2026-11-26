// SPDX-License-Identifier: MIT
// synthframe_nvsave against a model of hps_io's SD slot 0 (reads and writes) and a 1-clock RAM:
//   restore at an image load; a save with the core running (one pass), and with a RAM write
//   during it (a second, paused pass); a user reset (rst) after the load: saves still work and
//   the file is not read again; rst during a restore: the restore finishes after it; a
//   read-only file is never written; a load with no file leaves the RAM alone.
//   tb [SEED]       (AW from the build, -GAW=N)
#include "Vsynthframe_nvsave.h"
#include "verilated.h"

#include <cstdio>
#include <cstdlib>
#include <random>
#include <vector>

static Vsynthframe_nvsave* t;
static std::vector<uint8_t> ram, file;
static std::mt19937 rng;
static long clk_n, fails;
static int rd_reqs, wr_reqs;
static bool core_writes;                  // the core writes the RAM now and then

// hps_io: on sd_rd / sd_wr, ack after a delay, then (sd_blk_cnt + 1) x 512 bytes, a few clocks
// each (reads: sd_buff_wr pulses with the file's bytes; writes: sd_buff_din sampled), then ack low
struct Sd { int phase = 0, wait = 0, dir = 0; uint32_t lba = 0, n = 0, i = 0; } sd;
static void sd_clock()
{
    t->sd_buff_wr = 0;
    switch (sd.phase)
    {
    case 0:
        if (t->sd_rd || t->sd_wr) { sd.dir = t->sd_wr; sd.phase = 1; sd.wait = 10 + (int)(rng() % 40); }
        break;
    case 1:
        if (--sd.wait > 0) break;
        sd.lba = t->sd_lba; sd.n = (t->sd_blk_cnt + 1u) * 512u; sd.i = 0;
        (sd.dir ? wr_reqs : rd_reqs)++;
        t->sd_ack = 1; t->sd_buff_addr = 0; sd.phase = 2; sd.wait = 3;
        break;
    case 2:
        if (--sd.wait > 0) break;
        if (sd.dir)     // sd_buff_din: the byte at sd_buff_addr, set a few clocks ago
        {
            file.at(sd.lba * 512 + sd.i) = t->sd_buff_din;
            t->sd_buff_addr = (sd.i + 1) & 0x3fff;
        }
        else            // a byte to the module with its address
        {
            t->sd_buff_addr = sd.i & 0x3fff;
            t->sd_buff_dout = file.at(sd.lba * 512 + sd.i);
            t->sd_buff_wr = 1;
        }
        sd.wait = 2 + (int)(rng() % 3);
        if (++sd.i == sd.n) { sd.phase = 3; sd.wait = 4; }
        break;
    case 3:
        if (--sd.wait > 0) break;
        t->sd_ack = 0; sd.phase = 0;
        break;
    }
}

static void tick()
{
    sd_clock();
    // the core: idle unless stepping; paused or held, it starts no step and writes nothing
    const bool running = !t->pause && !t->boot_hold && !t->rst;
    t->core_idle = running ? (clk_n % 4 != 0) : 1;
    t->mem_written = 0;
    uint32_t cw = 0;
    if (core_writes && running && rng() % 500 == 0) { cw = rng() % ram.size(); t->mem_written = 1; }
    const uint32_t a = t->mem_addr;
    const bool we = t->mem_we;
    const uint8_t wd = t->mem_wdata;
    t->clk = 0; t->eval();
    t->clk = 1; t->eval();
    if (we) ram.at(a) = wd;
    if (cw || t->mem_written) ram[cw] ^= 0x5a;   // the CPU's write lands with mem_written
    t->mem_rdata = ram.at(t->mem_addr);
    t->eval();
    ++clk_n;
}
static void run(int n) { for (int i = 0; i < n; ++i) tick(); }
static bool until(bool (*f)(), long limit) { for (long i = 0; i < limit; ++i) { if (f()) return true; tick(); } return false; }
static void check(bool ok, const char* what) { printf("  %-62s %s\n", what, ok ? "ok" : "FAIL"); fails += !ok; }
static void fill(std::vector<uint8_t>& v) { for (auto& b : v) b = (uint8_t)rng(); }

// an image download; the save file (size bytes, read-only or not; 0 = none) is mounted near its end
static void load(uint64_t size, bool ro)
{
    t->loading = 1; t->image_ok = 0; run(20);
    if (size) { t->img_mounted = 1; t->img_size = size; t->img_readonly = ro; tick(); t->img_mounted = 0; }
    run(5);
    t->loading = 0; t->image_ok = 1;
}
static bool done_hold() { return !t->boot_hold && sd.phase == 0; }
static bool save_done() { return !t->saving && sd.phase == 0; }
static int save()   // returns the write requests the save made
{
    const int before = wr_reqs;
    t->save_req = 1; run(2);
    until([] { return (bool)t->saving; }, 100);
    until(save_done, 20'000'000);
    t->save_req = 0; run(10);
    return wr_reqs - before;
}
static bool same() { return ram == file; }

int main(int argc, char** argv)
{
    VerilatedContext ctx;
    ctx.commandArgs(argc, argv);
    t = new Vsynthframe_nvsave{&ctx};
    rng.seed(argc > 1 ? atoi(argv[1]) : 1);
    t->rst = 1; t->loading = 0; t->image_ok = 0; t->save_req = 0; t->osd_open = 0; t->autosave = 0;
    t->mem_written = 0; t->core_idle = 1; t->img_mounted = 0; t->img_readonly = 0; t->img_size = 0;
    t->sd_ack = 0; t->sd_buff_wr = 0; t->sd_buff_addr = 0; t->sd_buff_dout = 0;
    ram.assign(1u << AW, 0); file.assign(1u << AW, 0);
    run(5); t->rst = 0;
    const int chunks = AW > 14 ? 1 << (AW - 14) : 1;
    printf("AW %d (%d bytes, %d request(s) a pass)\n", AW, 1 << AW, chunks);

    puts("load with a save file");
    fill(file); fill(ram);
    load(1u << AW, false);
    check(until(done_hold, 20'000'000) && same() && t->restored, "RAM = file, restored, boot_hold down");

    puts("save with the core running");
    fill(ram);
    check(save() == chunks && same(), "one pass, file = RAM");
    core_writes = true;
    int n = 0, tries = 0;
    do { fill(ram); n = save(); } while (n == chunks && ++tries < 20);
    core_writes = false; run(100);
    check(n == 2 * chunks && same(), "RAM written during the pass: a second pass, file = RAM");

    puts("user reset (rst) after the load");
    fill(ram);
    std::vector<uint8_t> kept = ram;
    const int rd_before = rd_reqs;
    t->rst = 1; run(3); t->rst = 0;
    check(until(done_hold, 1000) && rd_reqs == rd_before && ram == kept, "no second restore, the RAM kept, boot_hold down");
    check(save() == chunks && same(), "save after the reset writes the file");
    t->autosave = 1; fill(ram);
    { const int w = wr_reqs; t->osd_open = 1; run(2); until([] { return (bool)t->saving; }, 100); until(save_done, 20'000'000);
      t->osd_open = 0; t->autosave = 0; run(10);
      check(wr_reqs - w == chunks && same(), "autosave on OSD open after the reset"); }

    puts("rst during a restore");
    fill(file); fill(ram);
    load(1u << AW, false);
    until([] { return sd.phase == 2 && !sd.dir; }, 100000);
    run(200);
    t->rst = 1; run(3); t->rst = 0;
    check(until(done_hold, 40'000'000) && same() && t->restored, "the restore runs again after rst: RAM = file");

    puts("read-only save file");
    fill(file); load(1u << AW, true);
    until(done_hold, 20'000'000);
    fill(ram);
    check(save() == 0 && ram != file, "no write");

    puts("load with no save file");
    fill(ram); kept = ram;
    { const int r = rd_reqs; load(0, false);
      check(until(done_hold, 1000) && rd_reqs == r && ram == kept && !t->restored, "no read, RAM kept, not restored"); }
    check(save() == 0, "no save without a mounted file");

    printf("%s\n", fails ? "FAIL" : "PASS");
    delete t;
    return fails ? 1 : 0;
}
