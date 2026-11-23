// SPDX-License-Identifier: GPL-2.0-or-later
// synthframe_nvsave unit bench (AW 15, the JV-880's 32 KB NVRAM): a save runs with the core
// running; a RAM write during that pass makes a second pass with pause up, which must hold the
// RAM as it is after the write.
//   cases: no write (one pass, pause never raised); a write behind the save's position; a write
//   ahead of it. Each checks the file against the RAM and the pause/pass counts.
#include "Vsynthframe_nvsave.h"
#include "verilated.h"

#include <cstdio>
#include <random>
#include <vector>

static Vsynthframe_nvsave* t;
static std::vector<uint8_t> ram(1 << 15), file(1 << 15);
static uint64_t clk_n;

static void tick()
{
    // the RAM: byte at mem_addr one clock later; restores write through mem_we
    const uint32_t a = t->mem_addr;
    const bool we = t->mem_we;
    const uint8_t wd = t->mem_wdata;
    t->clk = 0; t->eval();
    t->clk = 1; t->eval();
    if (we) ram[a] = wd;
    t->mem_rdata = ram[a];
    t->eval();
    ++clk_n;
}

// hps_io's SD write side, as jv880_board_bench: ack after a delay, sd_buff_din sampled every few
// clocks at the current address, then the next address; (sd_blk_cnt + 1) x 512 bytes a request
struct Sd { int phase = 0, wait = 0; uint32_t lba = 0, n = 0, i = 0; } sd;
static int requests;
static void sd_clock(std::mt19937& rng)
{
    switch (sd.phase)
    {
    case 0:
        if (t->sd_wr) { sd.phase = 1; sd.wait = 20 + (int)(rng() % 50); }
        break;
    case 1:
        if (--sd.wait > 0) break;
        sd.lba = t->sd_lba; sd.n = (t->sd_blk_cnt + 1u) * 512u; sd.i = 0;
        t->sd_ack = 1; t->sd_buff_addr = 0; sd.phase = 2; sd.wait = 3; ++requests;
        break;
    case 2:
        if (--sd.wait > 0) break;
        file[sd.lba * 512 + sd.i] = t->sd_buff_din;
        t->sd_buff_addr = (t->sd_buff_addr + 1) & 0x3fff;
        sd.wait = 3 + (int)(rng() % 3);
        if (++sd.i == sd.n) { sd.phase = 3; sd.wait = 4; }
        break;
    case 3:
        if (--sd.wait > 0) break;
        t->sd_ack = 0; sd.phase = 0;
        break;
    }
}

// write_at: -1 none; else the byte offset (of the file) the save has reached when the core writes
// RAM[target]
static bool run_case(const char* name, int write_at, uint32_t target, std::mt19937& rng)
{
    for (auto& b : ram) b = (uint8_t)rng();
    std::fill(file.begin(), file.end(), 0);
    requests = 0;
    bool paused = false, wrote = false;
    t->save_req = 1;                               // rising edge (save_req was 0)
    int guard = 0, saw_saving = 0;
    while (guard++ < 5'000'000)
    {
        sd_clock(rng);
        // the core: idle except one clock in four; while paused it starts no step (idle)
        t->core_idle = t->pause ? 1 : (clk_n % 4 != 0);
        t->mem_written = 0;
        const uint32_t pos = (uint32_t)(sd.lba * 512 + sd.i);
        if (write_at >= 0 && !wrote && !t->pause && t->saving && sd.phase == 2 && pos >= (uint32_t)write_at)
        {
            ram[target] ^= 0x5a;                    // the CPU writes the NVRAM (and its copy)
            t->mem_written = 1;
            wrote = true;
        }
        paused |= t->pause;
        tick();
        if (t->saving) saw_saving = 1;
        if (saw_saving && !t->saving && sd.phase == 0) break;
    }
    t->save_req = 0;
    for (int i = 0; i < 10; ++i) tick();
    size_t bad = 0;
    for (size_t i = 0; i < ram.size(); ++i) bad += ram[i] != file[i];
    const int passes = requests / 2;               // 32 KB = 2 requests of 16 KB
    const bool want_pause = write_at >= 0;
    const bool ok = saw_saving && bad == 0 && paused == want_pause && passes == (want_pause ? 2 : 1);
    printf("%-26s passes %d, paused %d, file bytes differing from the RAM %zu: %s\n", name, passes, (int)paused, bad,
           ok ? "ok" : "FAIL");
    return ok;
}

int main(int argc, char** argv)
{
    VerilatedContext ctx;
    ctx.commandArgs(argc, argv);
    t = new Vsynthframe_nvsave{&ctx};
    std::mt19937 rng(1);
    // reset, then mount a 32 KB file and finish a (fileless) load: the core runs, the file is writable
    t->rst = 1; t->loading = 0; t->image_ok = 0; t->save_req = 0; t->osd_open = 0; t->autosave = 0;
    t->mem_written = 0; t->core_idle = 1; t->img_mounted = 0; t->img_readonly = 0; t->img_size = 0;
    for (int i = 0; i < 5; ++i) tick();
    t->rst = 0; t->loading = 1; tick(); tick();
    t->img_mounted = 1; t->img_size = 1 << 15; tick(); t->img_mounted = 0;
    // no restore: the bench does not answer reads, so mount a short file instead for the load
    t->img_mounted = 1; t->img_size = 0; tick(); t->img_mounted = 0;
    t->loading = 0; t->image_ok = 1;
    for (int i = 0; i < 10; ++i) tick();
    if (t->boot_hold) { printf("boot_hold still up\n"); return 1; }
    // the save path needs `mounted`; a short file is still a writable mount
    bool ok = true;
    ok &= run_case("no write during the save", -1, 0, rng);
    ok &= run_case("write behind the position", 20000, 100, rng);
    ok &= run_case("write ahead of it", 100, 30000, rng);
    printf("%s\n", ok ? "PASS" : "FAIL");
    delete t;
    return ok ? 0 : 1;
}
