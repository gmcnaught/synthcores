// SPDX-License-Identifier: MIT
// U4 bench: the whole MU50 board (mu50_board.sv: H8/3003, peripherals, RAM, SWP00, LCD) on the
// real ROMs. Writes the samples as MAME's MU_SAMPLES does (int32 L, int32 R per sample, little
// endian) and, optionally, the CPU trace.
//
//   mu50_bench ROMDIR SAMPLES OUT.raw [--rx RXLOG] [--trace OUT.trace] [--bios v105|v104|v102]
//              [--lcd OUT.txt]
//
// ROMDIR is a MAME rompath (mu50/ with the program and wave ROMs). The run ends when SAMPLES
// sample pairs are out. --rx replays MAME's MU_RX_LOG (MIDI in, as mu50sys_bench). --lcd writes
// the LCD's DDRAM text at the end.
#include "Vmu50_board.h"
#include "verilated.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <string>
#include <vector>

static std::vector<uint8_t> load(const std::string &fn, size_t size) {
    std::vector<uint8_t> v(size);
    FILE *f = fopen(fn.c_str(), "rb");
    if (!f) { perror(fn.c_str()); exit(1); }
    size_t got = fread(v.data(), 1, size, f);
    fclose(f);
    if (got != size) { fprintf(stderr, "%s: %zu bytes, expected %zu\n", fn.c_str(), got, size); exit(1); }
    return v;
}

struct RxEv { uint64_t pos, now; int val; };

int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "usage: %s ROMDIR SAMPLES OUT.raw [--rx F] [--trace F] [--bios v105] [--lcd F]\n", argv[0]);
        return 2;
    }
    std::string dir = std::string(argv[1]) + "/mu50/";
    const uint64_t want = strtoull(argv[2], nullptr, 0);
    FILE *raw = fopen(argv[3], "wb");
    std::string bios = "xr174c0.ic7", rxfn, trfn, lcdfn;
    for (int i = 4; i + 1 < argc; i += 2) {
        if (!strcmp(argv[i], "--rx")) rxfn = argv[i + 1];
        else if (!strcmp(argv[i], "--trace")) trfn = argv[i + 1];
        else if (!strcmp(argv[i], "--lcd")) lcdfn = argv[i + 1];
        else if (!strcmp(argv[i], "--bios")) {
            std::string b = argv[i + 1];
            bios = b == "v104" ? "yamaha_mu50.bin" : b == "v102" ? "xq332e0.bin" : "xr174c0.ic7";
        }
    }
    std::vector<uint8_t> prog = load(dir + bios, 0x80000);
    for (size_t i = 0; i < prog.size(); i += 2) std::swap(prog[i], prog[i + 1]);   // word swap
    std::vector<uint8_t> wave = load(dir + "xq057c0.ic18", 0x200000);
    std::vector<uint8_t> w2 = load(dir + "xq058c0.ic19", 0x200000);
    wave.insert(wave.end(), w2.begin(), w2.end());
    std::vector<uint32_t> dmem(0x48000, 0);

    std::deque<RxEv> rx;
    if (!rxfn.empty()) {
        FILE *g = fopen(rxfn.c_str(), "r");
        if (!g) { perror(rxfn.c_str()); return 1; }
        int sci, val; unsigned long long p, nw;
        while (fscanf(g, "%d %llu %llu %d", &sci, &p, &nw, &val) == 4)
            if (sci == 1) rx.push_back({p, nw, val});
        fclose(g);
    }
    FILE *tr = trfn.empty() ? nullptr : fopen(trfn.c_str(), "w");

    Verilated::commandArgs(argc, argv);
    auto *c = new Vmu50_board;
    c->rst = 1; c->rom_ack = 0; c->wv_ack = 0; c->dm_ack = 0; c->pre_ack = 0; c->rx_req = 0; c->keys = 0;
    for (int i = 0; i < 4; i++) { c->clk = 0; c->eval(); c->clk = 1; c->eval(); }
    c->rst = 0;

    uint64_t samples = 0, clk = 0, bh = 1469598103934665603ULL;   // FNV hash of the bus log
    // MIDI out: the CPU's TDR1 writes, and the board's transmit line read back in virtual time
    std::vector<uint8_t> tdr, line;
    int l_prev = 1, l_k = -1; uint64_t l_t = 0; uint8_t l_byte = 0;
    int rx_busy = 0, wv_pend = 0, dm_pend = 0;
    uint32_t wv_q = 0, dm_q = 0;
    while (samples < want) {
        c->clk = 0;
        c->eval();
        // memories: answer one clock after the request
        c->rom_ack = 0; c->wv_ack = 0; c->dm_ack = 0; c->pre_ack = 0;
        if (!rx_busy) c->rx_req = 0;
        if (c->rom_req) {
            uint32_t a = (uint32_t)c->rom_addr << 1;
            c->rom_data = (prog[a] << 8) | prog[a + 1];
            c->rom_ack = 1;
        }
        if (wv_pend) { c->wv_data = wv_q; c->wv_ack = 1; wv_pend = 0; }
        else if (c->wv_req) {
            uint32_t a = c->wv_addr;
            wv_q = 0;
            for (int i = 0; i < 4; i++) wv_q |= (uint32_t)wave[(a + i) & 0x3fffff] << (8 * i);
            wv_pend = 1;
        }
        if (dm_pend) { c->dm_rdata = dm_q; c->dm_ack = 1; dm_pend = 0; }
        else if (c->dm_req) {
            uint32_t a = c->dm_addr;
            if (a >= dmem.size()) { fprintf(stderr, "delay address %05X\n", a); return 1; }
            if (c->dm_we) dmem[a] = c->dm_wdata;
            dm_q = dmem[a];
            dm_pend = 1;
        }
        c->inj_pending = !rx.empty();
        if (!rx.empty()) { c->inj_pos = rx.front().pos; c->inj_time = rx.front().now; }
        if (c->pre_req && !rx_busy) {
            if (!rx.empty() && rx.front().pos <= c->pre_time) {
                c->rx_req = 1; c->rx_val = rx.front().val; c->rx_now = rx.front().now;
                rx.pop_front();
                rx_busy = 1;
            } else
                c->pre_ack = 1;
        }
        c->eval();
        if (tr && c->tr_stb) {
            if (c->tr_state < 0x10000)
                fprintf(tr, "%llu %06X %04X", (unsigned long long)c->vtime, c->tr_npc, c->tr_state);
            else
                fprintf(tr, "%llu %06X %05X", (unsigned long long)c->vtime, c->tr_npc, c->tr_state);
            for (int r = 0; r < 8; r++) {
                uint32_t hi = (c->tr_r[(r + 8) / 2] >> (((r + 8) & 1) * 16)) & 0xffff;
                uint32_t lo = (c->tr_r[r / 2] >> ((r & 1) * 16)) & 0xffff;
                fprintf(tr, " %04X%04X", hi, lo);
            }
            fprintf(tr, " %02X", c->tr_ccr);
            if (c->tr_state == 0x10001) fprintf(tr, " I %d %d", c->tr_tvec, (int8_t)c->tr_tlvl);
            fprintf(tr, "\n");
        }
        c->clk = 1;
        c->eval();
        clk++;
        if (c->rx_done) rx_busy = 0;
        if (tr && c->bl_stb && (c->bl_kind == 4 || c->bl_kind == 5))
            fprintf(tr, "W%d %06X %0*X %llu\n", c->bl_kind == 4 ? 1 : 2, c->bl_addr,
                    c->bl_kind == 4 ? 2 : 4, c->bl_kind == 4 ? c->bl_data & 0xff : c->bl_data,
                    (unsigned long long)c->bl_time);
        if (c->bl_stb && c->bl_kind == 4 && c->bl_addr == 0xffffbb) tdr.push_back(c->bl_data & 0xff);
        {
            int txd = c->midi_tx;
            uint64_t vt = c->vtime;
            if (l_k < 0 && l_prev && !txd) { l_k = 0; l_t = vt + 160; }
            else if (l_k >= 0 && vt >= l_t) {
                if (l_k >= 1 && l_k <= 8) l_byte = (l_byte >> 1) | (txd << 7);
                if (l_k == 9) { line.push_back(l_byte); l_k = -1; }
                else { l_k++; l_t += 320; }
            }
            l_prev = txd;
        }
        if (c->bl_stb) {
            uint64_t v[4] = {c->bl_kind, c->bl_addr, c->bl_data, (uint64_t)c->bl_time};
            for (uint64_t x : v) { bh ^= x; bh *= 1099511628211ULL; }
        }
        if (c->smp_valid) {
            int32_t l = (int32_t)(c->smp_l << 14) >> 14, r = (int32_t)(c->smp_r << 14) >> 14;
            fwrite(&l, 4, 1, raw);
            fwrite(&r, 4, 1, raw);
            samples++;
        }
        if (c->illegal) { fprintf(stderr, "illegal instruction\n"); break; }
    }
    fclose(raw);
    if (tr) fclose(tr);
    if (!lcdfn.empty()) {
        FILE *o = fopen(lcdfn.c_str(), "w");
        for (int line = 0; line < 2; line++) {
            for (int pos = 0; pos < 20; pos++) {
                c->r_line = line; c->r_pos = pos; c->r_row = 0;
                c->clk = 0; c->eval();
                fputc(c->r_code >= 0x20 && c->r_code < 0x7f ? c->r_code : '.', o);
            }
            fputc('\n', o);
        }
        fclose(o);
    }
    fprintf(stderr, "TDR1 writes (%zu):", tdr.size());
    for (uint8_t b : tdr) fprintf(stderr, " %02X", b);
    fprintf(stderr, "\nMIDI out line (%zu):", line.size());
    for (uint8_t b : line) fprintf(stderr, " %02X", b);
    fprintf(stderr, "\n");
    fprintf(stderr, "%llu samples, %llu clocks, CPU at %llu states, bus hash %016llx\n",
            (unsigned long long)samples, (unsigned long long)clk, (unsigned long long)c->vtime,
            (unsigned long long)bh);
    delete c;
    return 0;
}
