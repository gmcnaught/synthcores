// SPDX-License-Identifier: MIT
// H8/300H core bench on the MU50 memory map, no peripherals: writes a trace in the patched
// MAME's format (cores/mu50/mame/patches/0001-mu50-logging.patch, MU_TRACE without MU_BUS).
//   h8h_bench PROGRAM.bin N OUT.trace
// Map, as MAME's mu50: 000000-07FFFF program ROM (the image, big-endian), 200000-20FFFF RAM,
// FFFD10-FFFF0F on-chip RAM, everything else reads 0 and ignores writes. RAM starts zeroed.
#include "Vh8h_core.h"
#include "verilated.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>

enum { OP_F16 = 1, OP_R8 = 2, OP_R16 = 3, OP_W8 = 4, OP_W16 = 5 };

static std::vector<uint8_t> rom(0x80000), ram(0x10000), iram(0x200);

static uint8_t *loc(uint32_t a, bool wr) {
    a &= 0xffffff;
    if (a < 0x80000) return wr ? nullptr : &rom[a];
    if (a >= 0x200000 && a < 0x210000) return &ram[a - 0x200000];
    if (a >= 0xfffd10 && a < 0xffff10) return &iram[a - 0xfffd10];
    return nullptr;
}
static uint8_t rd8(uint32_t a) { uint8_t *p = loc(a, false); return p ? *p : 0; }
static void wr8(uint32_t a, uint8_t v) { if (uint8_t *p = loc(a, true)) *p = v; }

int main(int argc, char **argv) {
    if (argc < 4) { fprintf(stderr, "usage: %s PROGRAM N OUT\n", argv[0]); return 2; }
    FILE *f = fopen(argv[1], "rb");
    if (!f) { perror(argv[1]); return 1; }
    size_t got = fread(rom.data(), 1, rom.size(), f);
    fclose(f);
    (void)got;
    const uint64_t n = strtoull(argv[2], nullptr, 0);
    FILE *out = fopen(argv[3], "w");
    if (!out) { perror(argv[3]); return 1; }

    Verilated::commandArgs(argc, argv);
    auto *c = new Vh8h_core;
    auto tick = [&] { c->clk = 0; c->eval(); c->clk = 1; c->eval(); };
    c->rst = 1; c->ce = 1; c->bus_ack = 0; c->irq_vector = 0; c->irq_level = 0;
    c->syscr_ue = 1; c->standby_pending = 0;
    for (int i = 0; i < 4; i++) tick();
    c->rst = 0;

    uint64_t count = 0;
    for (uint64_t clk = 0; count < n && clk < n * 200 + 100000; clk++) {
        c->clk = 0;
        c->eval();
        // combinational memory: answer in the same clock
        c->bus_ack = c->bus_req;
        if (c->bus_req) {
            uint32_t a = c->bus_addr;
            switch (c->bus_kind) {
            case OP_R8: c->bus_rdata = rd8(a); break;
            case OP_F16: case OP_R16: c->bus_rdata = (rd8(a) << 8) | rd8(a + 1); break;
            default: break;
            }
        }
        c->eval();
        const bool wr = c->bus_req && (c->bus_kind == OP_W8 || c->bus_kind == OP_W16);
        const uint32_t wa = c->bus_addr, wd = c->bus_wdata, wk = c->bus_kind;
        const bool stb = c->tr_stb;
        if (stb) {
            if (c->tr_state < 0x10000)
                fprintf(out, "%llu %06X %04X", (unsigned long long)c->vtime, c->tr_npc, c->tr_state);
            else
                fprintf(out, "%llu %06X %05X", (unsigned long long)c->vtime, c->tr_npc, c->tr_state);
            for (int r = 0; r < 8; r++) {
                uint32_t hi = (c->tr_r[(r + 8) / 2] >> (((r + 8) & 1) * 16)) & 0xffff;
                uint32_t lo = (c->tr_r[r / 2] >> ((r & 1) * 16)) & 0xffff;
                fprintf(out, " %04X%04X", hi, lo);
            }
            fprintf(out, " %02X", c->tr_ccr);
            if (c->tr_state == 0x10001) fprintf(out, " I %d %d", c->tr_tvec, (int8_t)c->tr_tlvl);
            fprintf(out, "\n");
            count++;
        }
        c->clk = 1;
        c->eval();
        if (wr) {
            if (wk == OP_W8) wr8(wa, wd);
            else { wr8(wa, wd >> 8); wr8(wa + 1, wd & 0xff); }
            fprintf(out, "W%d %06X %0*X %llu\n", wk == OP_W8 ? 1 : 2, wa, wk == OP_W8 ? 2 : 4, wd,
                    (unsigned long long)c->vtime);
        }
        if (c->illegal) { fprintf(stderr, "illegal instruction\n"); break; }
    }
    fclose(out);
    delete c;
    return 0;
}
