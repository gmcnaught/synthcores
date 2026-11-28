// SPDX-License-Identifier: MIT
// U5 bench: the MiSTer wrapper (hdl/mu50_top.sv) with a DDR3 model: the ROM pack goes in through
// the ioctl download, the CPU runs from DDR3 through its cache, the SWP00 reads the wave ROM and
// the delay lines there. Pacing is off. Prints the LCD text, a hash of every CPU bus access
// (kind, address, data, time) and the FPGA clocks per CPU state; mu50_bench --bushash gives the
// same hash for the same run on the board alone (memories in C++ arrays).
//
//   mu50_top_bench PACK.rom SAMPLES [--lat N] [--rx RXLOG] [--pace]
// --lat: DDR3 read latency in clocks (default 12). --rx: MAME's MU_RX_LOG; its bytes are sent on
// the UART at 31,250 baud, each when the CPU reaches the time MAME's started. --pace: real-time
// pacing on (prints the audio FIFO underruns). The bytes on the UART's transmit pin (MIDI out,
// 31,250 baud) are printed in hex.
#include "Vmu50_top.h"
#include "Vmu50_top___024root.h"
#include "verilated.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <vector>

int main(int argc, char **argv) {
    if (argc < 3) { fprintf(stderr, "usage: %s PACK SAMPLES [--lat N]\n", argv[0]); return 2; }
    std::vector<uint8_t> pack;
    {
        FILE *f = fopen(argv[1], "rb");
        if (!f) { perror(argv[1]); return 1; }
        int ch;
        while ((ch = fgetc(f)) != EOF) pack.push_back(ch);
        fclose(f);
    }
    const uint64_t want = strtoull(argv[2], nullptr, 0);
    int lat = 12;
    bool pace = false;
    struct Mb { uint64_t t; uint8_t b; };
    std::deque<Mb> mbytes;
    for (int i = 3; i < argc; i++) {
        if (!strcmp(argv[i], "--lat") && i + 1 < argc) lat = atoi(argv[++i]);
        else if (!strcmp(argv[i], "--pace")) pace = true;
        else if (!strcmp(argv[i], "--rx") && i + 1 < argc) {
            // decode MAME's pin changes (320 states a bit) back into bytes and start times
            FILE *g = fopen(argv[++i], "r");
            if (!g) { perror(argv[i]); return 1; }
            int sci, val; unsigned long long p, nw;
            std::vector<std::pair<uint64_t, int>> ev;
            while (fscanf(g, "%d %llu %llu %d", &sci, &p, &nw, &val) == 4)
                if (sci == 1) ev.push_back({nw, val});
            fclose(g);
            int level = 1;
            size_t k = 0;
            auto at = [&](uint64_t t) {       // line level at time t
                while (k < ev.size() && ev[k].first <= t) { level = ev[k].second; k++; }
                return level;
            };
            for (size_t j = 0; j < ev.size(); j++) {
                if (ev[j].second == 0 && (j == 0 || ev[j - 1].second == 1)) {
                    uint64_t t0 = ev[j].first;
                    if (!mbytes.empty() && t0 < mbytes.back().t + 3200) continue;
                    k = 0; level = 1;
                    uint8_t b = 0;
                    for (int bit = 0; bit < 8; bit++) b |= at(t0 + 320 * (bit + 1) + 160) << bit;
                    mbytes.push_back({t0, b});
                }
            }
            fprintf(stderr, "%zu MIDI bytes\n", mbytes.size());
        }
    }

    const uint32_t W0 = 0x30000000 >> 3;
    std::vector<uint64_t> ddr(0x70000, 0xdeadbeefcafef00dULL);   // garbage until written
    struct Rd { uint64_t due; uint32_t addr; int n; };
    std::deque<Rd> rq;
    int wr_left = 0;
    uint32_t wr_addr = 0;

    Verilated::commandArgs(argc, argv);
    auto *c = new Vmu50_top;
    auto *r = c->rootp;
    c->rst = 1; c->user_reset = 0; c->pace_enable = pace; c->gain = 0; c->uart_rxd = 1; c->ps2_key = 0;
    c->img_mounted = 0; c->img_readonly = 0; c->img_size = 0; c->sd_ack = 0; c->save_req = 0;
    c->osd_open = 0; c->autosave = 0; c->DDRAM_BUSY = 0; c->DDRAM_DOUT_READY = 0;
    c->ioctl_download = 0; c->ioctl_wr = 0;
    uint64_t clk = 0;
    auto tick = [&] {
        c->clk = 0; c->clk_vid = 0; c->eval();
        // DDR3 (Avalon, never busy): reads answer `lat` clocks later, one word a clock
        c->DDRAM_DOUT_READY = 0;
        if (!rq.empty() && rq.front().due <= clk) {
            Rd &q = rq.front();
            uint32_t a = q.addr - W0;
            c->DDRAM_DOUT = a < ddr.size() ? ddr[a] : 0;
            c->DDRAM_DOUT_READY = 1;
            q.addr++;
            if (--q.n == 0) rq.pop_front();
        }
        c->eval();
        c->clk = 1; c->clk_vid = 1; c->eval();
        if (c->DDRAM_RD) rq.push_back({clk + (uint64_t)lat, c->DDRAM_ADDR, c->DDRAM_BURSTCNT});
        if (c->DDRAM_WE) {
            if (wr_left == 0) { wr_addr = c->DDRAM_ADDR; wr_left = c->DDRAM_BURSTCNT; }
            uint32_t a = wr_addr - W0;
            if (a < ddr.size()) {
                uint64_t m = 0;
                for (int b = 0; b < 8; b++) if (c->DDRAM_BE >> b & 1) m |= 0xffULL << (8 * b);
                ddr[a] = (ddr[a] & ~m) | (c->DDRAM_DIN & m);
            }
            wr_addr++;
            wr_left--;
        }
        clk++;
    };
    for (int i = 0; i < 8; i++) tick();
    c->rst = 0;
    for (int i = 0; i < 8; i++) tick();

    // the download (hps_io: one byte at a time, waiting while ioctl_wait)
    c->ioctl_download = 1;
    for (size_t a = 0; a < pack.size(); a++) {
        while (c->ioctl_wait) tick();
        c->ioctl_addr = a; c->ioctl_dout = pack[a]; c->ioctl_wr = 1;
        tick();
        c->ioctl_wr = 0;
        tick();
    }
    c->ioctl_download = 0;
    uint64_t t_load = clk;

    uint64_t samples = 0, h = 1469598103934665603ULL, accesses = 0;
    uint64_t run_start = 0;
    // profile: clocks by the bus sequencer's state (h8h_sys b) and, in B_ACC, the board's (x)
    uint64_t prof_b[8] = {0}, prof_x[16] = {0};
    // UART: 31,250 baud at 50 MHz = 1600 clocks a bit
    int u_bit = -1; uint64_t u_next = 0; uint16_t u_frame = 0;
    // MIDI out receiver: 1600 clocks a bit, sampled mid-bit
    std::vector<uint8_t> mout;
    int tdr_n = 0, sci_edges = 0, sci_prev = 1;
    std::vector<uint8_t> rdr;
    int mrx = 0;
    int o_bit = -1; uint64_t o_next = 0; uint8_t o_byte = 0; int o_prev = 1;
    while (samples < want) {
        if (u_bit < 0 && clk >= u_next && !mbytes.empty() && r->mu50_top__DOT__vtime >= mbytes.front().t) {
            u_frame = (1 << 9) | (mbytes.front().b << 1);   // start 0, data, stop 1
            mbytes.pop_front();
            u_bit = 0; u_next = clk;
        }
        if (u_bit >= 0 && clk >= u_next) {
            c->uart_rxd = (u_frame >> u_bit) & 1;
            u_bit++; u_next = clk + 1600;
            if (u_bit == 10) u_bit = -1;
        }
        if (u_bit < 0 && clk >= u_next) c->uart_rxd = 1;
        tick();
        if (!run_start && r->mu50_top__DOT__running) run_start = clk;
        if (r->mu50_top__DOT__board__DOT__bl_stb) {
            uint64_t v[4] = {r->mu50_top__DOT__board__DOT__bl_kind, r->mu50_top__DOT__board__DOT__bl_addr,
                             r->mu50_top__DOT__board__DOT__bl_data, r->mu50_top__DOT__board__DOT__bl_time};
            for (uint64_t x : v) { h ^= x; h *= 1099511628211ULL; }
            accesses++;
            if (r->mu50_top__DOT__board__DOT__bl_kind == 4 && r->mu50_top__DOT__board__DOT__bl_addr == 0xffffbb) tdr_n++;
            if (r->mu50_top__DOT__board__DOT__bl_kind == 2 && r->mu50_top__DOT__board__DOT__bl_addr == 0xffffbd)
                rdr.push_back(r->mu50_top__DOT__board__DOT__bl_data & 0xff);
        }
        if (r->mu50_top__DOT__smp_valid) samples++;
        if (r->mu50_top__DOT__mb_valid) mrx++;
        if (r->mu50_top__DOT__sci_txd != sci_prev) { sci_edges++; sci_prev = r->mu50_top__DOT__sci_txd; }
        {
            int txd = c->uart_txd;
            if (o_bit < 0 && o_prev && !txd) { o_bit = 0; o_next = clk + 800; }
            else if (o_bit >= 0 && clk >= o_next) {
                if (o_bit >= 1 && o_bit <= 8) o_byte = (o_byte >> 1) | (txd << 7);
                if (o_bit == 9) { mout.push_back(o_byte); o_bit = -1; }
                else { o_bit++; o_next += 1600; }
            }
            o_prev = txd;
        }
        if (run_start) {
            int b = r->mu50_top__DOT__board__DOT__cpu__DOT__b;
            prof_b[b & 7]++;
            if (b == 2) prof_x[r->mu50_top__DOT__board__DOT__x & 15]++;
        }
        if (clk - t_load > 4000000000ULL) { fprintf(stderr, "no progress\n"); break; }
    }
    uint64_t vt = r->mu50_top__DOT__vtime;
    printf("loaded in %llu clocks; %llu samples, %llu accesses, bus hash %016llx\n",
           (unsigned long long)t_load, (unsigned long long)samples, (unsigned long long)accesses,
           (unsigned long long)h);
    printf("CPU at %llu states, %.2f clocks a state (DDR3 latency %d), %.1f clocks a sample, underruns %d\n",
           (unsigned long long)vt, (double)(clk - run_start) / (double)vt, lat,
           (double)(clk - run_start) / (double)samples, (int)c->underruns);
    printf("TDR1 writes %d, SCI1 tx edges %d, UART bytes in %d\nRDR1 reads (%zu):", tdr_n, sci_edges, mrx, rdr.size());
    for (uint8_t b : rdr) printf(" %02X", b);
    printf("\n");
    printf("MIDI out (%zu bytes):", mout.size());
    for (uint8_t b : mout) printf(" %02X", b);
    printf("\n");
    const char *bn[8] = {"idle (CPU states)", "pre", "access", "events", "inject", "events2", "ack", "?"};
    const char *xn[16] = {"x_idle", "rom", "ram", "ram2", "swp_t", "swp", "swp_w", "ack", "gen", "", "", "", "", "", "", ""};
    double tot = (double)(clk - run_start);
    for (int i = 0; i < 7; i++) printf("  bus %-18s %5.1f%%\n", bn[i], 100.0 * prof_b[i] / tot);
    for (int i = 0; i < 9; i++) if (prof_x[i]) printf("    access %-8s %5.1f%%\n", xn[i], 100.0 * prof_x[i] / tot);
    for (int line = 0; line < 4; line++) {
        for (int pos = 0; pos < 20; pos++) {
            r->mu50_top__DOT__r_line = line & 1;
            r->mu50_top__DOT__r_pos = pos + (line >> 1) * 20;
            c->eval();
            uint8_t code = r->mu50_top__DOT__r_code;
            putchar(code >= 0x20 && code < 0x7f ? code : '.');
        }
        putchar('\n');
    }
    delete c;
    return 0;
}
