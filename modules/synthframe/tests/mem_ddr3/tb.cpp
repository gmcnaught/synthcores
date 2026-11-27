// SPDX-License-Identifier: MIT
// synthframe_mem_ddr3 against a DDR3 (Avalon-MM) model with random BUSY and read latency: random
// reads (1-4 words) and fills, and short resets (1-4 clocks) while reads are outstanding.
// Checks: every response is for a read taken since the last reset, in order, with its tag and
// the right data; no response for a read taken before a reset; req_ready comes back after every
// reset (no stall); DDRAM_RD/WE and the command are held while DDRAM_BUSY (Avalon).
//   tb SEED CLOCKS
#include "Vsynthframe_mem_ddr3.h"
#include "verilated.h"

#include <cstdio>
#include <cstdlib>
#include <deque>
#include <random>

static uint64_t word(uint32_t a) { return 0x9e3779b97f4a7c15ull * (a + 1) ^ ((uint64_t)a << 17); }

int main(int argc, char** argv)
{
    const unsigned seed = argc > 1 ? atoi(argv[1]) : 1;
    const long clocks = argc > 2 ? atol(argv[2]) : 200000;
    VerilatedContext ctx;
    ctx.commandArgs(argc, argv);
    auto* t = new Vsynthframe_mem_ddr3{&ctx};
    std::mt19937 rng(seed);
    auto chance = [&](int pct) { return (int)(rng() % 100) < pct; };

    struct Rd { uint32_t addr; int len; int tag; };
    struct Ddr { uint32_t addr; int len; long ready; };
    std::deque<Rd> want;                 // reads taken since the last reset, oldest first
    int want_beat = 0;
    std::deque<Ddr> ddr;                 // read commands the controller took
    int ddr_beat = 0;
    int wr_left = 0;                     // beats left of the write burst in progress
    long errors = 0, responses = 0, resets = 0, stalled = 0, waiting = 0;
    int rst_left = 0;
    bool prev_cmd = false, prev_busy = false; uint32_t prev_addr = 0;

    auto edge = [&]() { t->clk = 0; t->eval(); t->clk = 1; t->eval(); };
    t->rst = 1; t->req_valid = 0; t->DDRAM_BUSY = 0; t->DDRAM_DOUT_READY = 0;
    for (int i = 0; i < 4; ++i) edge();
    t->rst = 0;

    for (long n = 0; n < clocks; ++n)
    {
        // inputs for this clock
        if (rst_left == 0 && n > 100 && chance(1) && !want.empty()) { rst_left = 1 + rng() % 4; ++resets; }
        t->rst = rst_left > 0;
        if (!t->req_valid && chance(60))
        {
            t->req_valid = 1;
            t->req_we = chance(20);
            t->req_len = 1 + rng() % 4;
            t->req_addr = t->req_we ? (0x10000000u | (rng() & 0xfff)) : (rng() & 0xffff);
            t->req_wdata = ((uint64_t)rng() << 32) | rng();
            t->req_be = 0xff;
            t->req_tag = rng() & 0xf;
        }
        t->DDRAM_BUSY = chance(30);
        t->DDRAM_DOUT_READY = 0;
        if (!ddr.empty() && ddr.front().ready <= n)
        {
            t->DDRAM_DOUT_READY = 1;
            t->DDRAM_DOUT = word(ddr.front().addr + ddr_beat);
            if (++ddr_beat == ddr.front().len) { ddr.pop_front(); ddr_beat = 0; }
        }
        t->clk = 0; t->eval();
        // what the clock edge takes
        const bool fire = t->req_valid && t->req_ready;
        const bool cmd = t->DDRAM_RD || t->DDRAM_WE;
        if (prev_cmd && prev_busy && (!cmd || t->DDRAM_ADDR != prev_addr))
        {
            if (errors++ < 10) printf("clock %ld: command dropped or changed while DDRAM_BUSY\n", n);
        }
        prev_cmd = cmd; prev_busy = t->DDRAM_BUSY; prev_addr = t->DDRAM_ADDR;
        if (t->DDRAM_RD && !t->DDRAM_BUSY)
            ddr.push_back({t->DDRAM_ADDR, t->DDRAM_BURSTCNT, n + 4 + (long)(rng() % 40)});
        if (t->DDRAM_WE && !t->DDRAM_BUSY)
        {
            if (wr_left == 0) wr_left = t->DDRAM_BURSTCNT;
            --wr_left;
        }
        if (fire && !t->req_we) want.push_back({t->req_addr, t->req_len, t->req_tag});
        if (t->req_valid && !t->rst) { if (++waiting == 2000 && stalled++ < 10) printf("clock %ld: req_ready low for 2000 clocks (%d reads outstanding at the controller)\n", n, (int)ddr.size()); }
        else waiting = 0;
        t->clk = 1; t->eval();
        // after the edge
        if (fire) t->req_valid = 0;
        if (t->rst) { want.clear(); want_beat = 0; }
        if (rst_left > 0) --rst_left;
        if (t->rsp_valid)
        {
            ++responses;
            if (want.empty())
            {
                if (errors++ < 10) printf("clock %ld: response with no read outstanding (from before a reset?)\n", n);
            }
            else
            {
                const Rd& r = want.front();
                if (t->rsp_tag != r.tag || t->rsp_data != word(r.addr + want_beat))
                    if (errors++ < 10) printf("clock %ld: response tag %d data %016llx, want tag %d data %016llx\n", n,
                                              t->rsp_tag, (unsigned long long)t->rsp_data, r.tag,
                                              (unsigned long long)word(r.addr + want_beat));
                if (++want_beat == r.len) { want.pop_front(); want_beat = 0; }
            }
        }
    }
    const bool ok = errors == 0 && stalled == 0 && responses > clocks / 20 && resets > 10;
    printf("seed %u: %ld clocks, %ld resets, %ld responses, %ld errors, %ld stalls: %s\n", seed, clocks, resets,
           responses, errors, stalled, ok ? "ok" : "FAIL");
    delete t;
    return ok ? 0 : 1;
}
