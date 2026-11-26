// SPDX-License-Identifier: MIT
// CZ-101 board bench: the real firmware from power-on, trace in the patched MAME's format.
//   cz101_bench ROMDIR N OUT.trace [SAMPLES.s32 [RXLOG]]
// RXLOG: MAME's CZ_RX_LOG (seconds attoseconds level per MIDI-in line change).
// ROMDIR holds MAME's cz101 set (hn613256ps40.bin, init_ram.bin). Power switch on from time 0
// (MAME: CZ_POWER_ON=1), no keys, pitch wheel centred, MIDI in idle.
// CZ_MIDI_BYTES=FILE: live MIDI as the MiSTer core feeds it, lines "CYCLE BYTE" (decimal, hex):
// each byte is offered to the board's serializer once vcyc reaches CYCLE.
// CZ_KEYS=FILE: panel and music keys, lines "CYCLE ROW BIT DOWN" (decimal): from vcyc CYCLE, key
// matrix row ROW (kc0-kc15) bit BIT is pressed (DOWN 1) or released (0).
// CZ_NV_OUT=FILE: the battery RAM and cartridge (8 KB, as the MiSTer save file) at the end.
// CZ_LCD_LOG=FILE: "CYCLE [line 1] [line 2]" each time the LCD text changes.
// CZ_WHEEL=FILE: pitch wheel (AN1) changes, lines "CYCLE VALUE" (decimal; 127 = centre).
// CZ_TX_LOG=FILE: MIDI out (the uPD7811's TxD), "CYCLE LEVEL" per line change.
// CZ_CLOCKS=K: run K clocks after reset instead of stopping after N instructions (the CPU may
// halt or be powered down); exit status 0.
// CZ_TICK_BASE=B: start the board's 10 MHz tick time at B instead of 0 (the 40-bit tick counter
// wraps after 2^40 ticks, about 30.5 h); trace and samples must not change.
// CZ_RX_LINE_LOG=FILE: the CPU's serial input line, "EDGE LEVEL" per change (EDGE = index of
// the next 2 MHz SCK edge).
// CZ_POWER=FILE: power switch changes, lines "CYCLE LEVEL" (decimal; 1 = on, as at time 0).
// Last line: "board: ..." with the final time (cycles), the power-down state, the samples made
// after a power-down, the uPD933 IRQ rises and the last IRQ timer arming.
#include "Vcz101_board.h"
#include "verilated.h"
#include "Vcz101_board___024root.h"
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <string>
#include <vector>

static std::vector<uint8_t> load(const std::string &p, size_t n) {
    std::vector<uint8_t> v(n, 0);
    FILE *f = fopen(p.c_str(), "rb");
    if (!f) { perror(p.c_str()); exit(1); }
    if (fread(v.data(), 1, n, f) != n) { fprintf(stderr, "%s: short\n", p.c_str()); exit(1); }
    fclose(f);
    return v;
}

int main(int argc, char **argv) {
    if (argc < 4) { fprintf(stderr, "usage: %s ROMDIR N OUT [SAMPLES]\n", argv[0]); return 2; }
    std::string dir = argv[1];
    auto prog = load(dir + "/hn613256ps40.bin", 0x8000);
    auto ram = load(dir + "/init_ram.bin", 0x1000);
    const uint64_t n = strtoull(argv[2], nullptr, 0);
    FILE *out = fopen(argv[3], "w");
    FILE *smp = argc > 4 ? fopen(argv[4], "wb") : nullptr;
    // MIDI-in changes: due cycle floor(T / P) and SCK edges before T (ceil(T / E) - 1)
    struct rx_t { uint32_t cycle, edge; int level; };
    std::vector<rx_t> rx;
    if (argc > 5) {
        FILE *f = fopen(argv[5], "r");
        unsigned sec; unsigned long long as; int lv;
        const unsigned __int128 P = 299999940000ULL, E = 250000000000ULL;
        while (f && fscanf(f, "%u %llu %d", &sec, &as, &lv) == 3) {
            unsigned __int128 t = (unsigned __int128)sec * 1000000000000000000ULL + as;
            rx.push_back({(uint32_t)(t / P), (uint32_t)((t + E - 1) / E - 1), lv});
        }
        if (f) fclose(f);
    }
    size_t rxi = 0;
    struct mb_t { uint32_t cycle; uint8_t b; };
    std::vector<mb_t> mb;
    if (const char *mf = getenv("CZ_MIDI_BYTES")) {
        FILE *f = fopen(mf, "r");
        if (!f) { perror(mf); exit(1); }
        unsigned cy, b;
        while (fscanf(f, "%u %x", &cy, &b) == 2) mb.push_back({cy, (uint8_t)b});
        fclose(f);
    }
    size_t mbi = 0;
    struct key_t { uint32_t cycle; int row, bit, down; };
    std::vector<key_t> keys;
    if (const char *kf = getenv("CZ_KEYS")) {
        FILE *f = fopen(kf, "r");
        if (!f) { perror(kf); exit(1); }
        key_t k;
        while (fscanf(f, "%u %d %d %d", &k.cycle, &k.row, &k.bit, &k.down) == 4) keys.push_back(k);
        fclose(f);
    }
    size_t ki = 0;
    struct wheel_t { uint32_t cycle; int v; };
    std::vector<wheel_t> wheel;
    if (const char *wf = getenv("CZ_WHEEL")) {
        FILE *f = fopen(wf, "r");
        if (!f) { perror(wf); exit(1); }
        wheel_t w;
        while (fscanf(f, "%u %d", &w.cycle, &w.v) == 2) wheel.push_back(w);
        fclose(f);
    }
    size_t wi = 0;
    struct power_t { uint32_t cycle; int on; };
    std::vector<power_t> power;
    if (const char *pf = getenv("CZ_POWER")) {
        FILE *f = fopen(pf, "r");
        if (!f) { perror(pf); exit(1); }
        power_t w;
        while (fscanf(f, "%u %d", &w.cycle, &w.on) == 2) power.push_back(w);
        fclose(f);
    }
    size_t pi = 0;
    const uint64_t clocks = getenv("CZ_CLOCKS") ? strtoull(getenv("CZ_CLOCKS"), nullptr, 0) : 0;
    const uint64_t tick_base = getenv("CZ_TICK_BASE") ? strtoull(getenv("CZ_TICK_BASE"), nullptr, 0) : 0;
    FILE *txlog = getenv("CZ_TX_LOG") ? fopen(getenv("CZ_TX_LOG"), "w") : nullptr;
    int tx_last = 1;
    FILE *lcdlog = getenv("CZ_LCD_LOG") ? fopen(getenv("CZ_LCD_LOG"), "w") : nullptr;
    std::string lcd_last;
    FILE *rxline = getenv("CZ_RX_LINE_LOG") ? fopen(getenv("CZ_RX_LINE_LOG"), "w") : nullptr;
    int rxline_last = 1;

    Verilated::commandArgs(argc, argv);
    auto *c = new Vcz101_board;
    auto tick = [&] { c->clk = 0; c->eval(); c->clk = 1; c->eval(); };
    c->reset = 1; c->cyc_ce = 1; c->power_on = 1; c->pitch_wheel = 0x7F; c->rx_valid = 0;
    for (int i = 0; i < 16; i++) c->keys[i] = 0xFF;
    c->r_line = 0; c->r_pos = 0; c->r_row = 0;
    for (int a = 0; a < 0x9000; a++) {
        c->load_wr = 1; c->load_addr = a; c->load_data = a < 0x8000 ? prog[a] : ram[a - 0x8000];
        tick();
    }
    c->load_wr = 0;
    for (int i = 0; i < 300; i++) tick();   // LCD clear loop
    c->reset = 0;
    if (tick_base) {   // as if the board had reset at tick B: tick state and the uPD933's sample grid
        auto *r = c->rootp;
        r->cz101_board__DOT__t3 = tick_base & ((1ull << 40) - 1);
        r->cz101_board__DOT__snd__DOT__next_bnd = tick_base & ((1ull << 40) - 1);
        r->cz101_board__DOT__sim_tick_base = tick_base & ((1ull << 40) - 1);
    }
    uint64_t down_samples = 0, irq_rises = 0, arms = 0, irqs_taken = 0, nmis_taken = 0;
    int64_t down_at = -1, irq_first = -1;
    uint32_t arm_n = 0;
    int irq_last = 0;

    auto lcd_text = [&] {
        std::string t;
        for (int line = 0; line < 2; line++) {
            t += '[';
            for (int p = 0; p < 16; p++) {
                c->r_line = line; c->r_pos = p; c->eval();
                uint8_t ch = c->r_code; t += (ch >= 0x20 && ch < 0x7F) ? (char)ch : '.';
            }
            t += ']';
        }
        return t;
    };
    uint64_t cyc = 0, count = 0, samples = 0;
    for (uint64_t clk = 0; clocks ? clk < clocks : (count < n && clk < n * 2000 + 100000); clk++) {
        if (rxi < rx.size()) {
            c->rx_valid = 1; c->rx_cycle = rx[rxi].cycle; c->rx_edge = rx[rxi].edge; c->rx_level = rx[rxi].level;
        } else c->rx_valid = 0;
        if (mbi < mb.size() && mb[mbi].cycle <= c->cycles) { c->midi_valid = 1; c->midi_byte = mb[mbi].b; }
        else c->midi_valid = 0;
        while (ki < keys.size() && keys[ki].cycle <= c->cycles) {
            if (keys[ki].down) c->keys[keys[ki].row] &= ~(1u << keys[ki].bit);
            else c->keys[keys[ki].row] |= 1u << keys[ki].bit;
            ki++;
        }
        while (wi < wheel.size() && wheel[wi].cycle <= c->cycles) c->pitch_wheel = wheel[wi++].v;
        while (pi < power.size() && power[pi].cycle <= c->cycles) c->power_on = power[pi++].on;
        tick();
        {
            auto *r = c->rootp;
            if (down_at < 0 && c->powered_down) down_at = c->cycles;
            const int irq = r->cz101_board__DOT__s_irq;
            if (irq && !irq_last) { if (irq_first < 0) irq_first = c->cycles; irq_rises++; }
            irq_last = irq;
            if (r->cz101_board__DOT__t_arm) { arms++; arm_n = r->cz101_board__DOT__t_n; }
            if (c->sample_valid && c->powered_down) down_samples++;
            if (rxline && r->cz101_board__DOT__rx_line != rxline_last) {
                rxline_last = r->cz101_board__DOT__rx_line;
                fprintf(rxline, "%u %d\n", r->cz101_board__DOT__sck_k, rxline_last);
            }
        }
        if (txlog && c->midi_tx != tx_last) { tx_last = c->midi_tx; fprintf(txlog, "%u %d\n", c->cycles, tx_last); }
        if (lcdlog && (clk & 4095) == 0) {
            std::string t = lcd_text();
            if (t != lcd_last) { fprintf(lcdlog, "%u %s\n", c->cycles, t.c_str()); lcd_last = t; }
        }
        if (c->rx_pop) rxi++;
        if (c->midi_pop) mbi++;
        if (getenv("CZ_DEBUG_TIMER")) {
            auto *r = c->rootp;
            if (r->cz101_board__DOT__t_arm) fprintf(stderr, "arm a=%u n=%u vcyc=%u\n", r->cz101_board__DOT__arm_time, r->cz101_board__DOT__t_n, r->cz101_board__DOT__vcyc);
            if (r->cz101_board__DOT__t_cancel) fprintf(stderr, "cancel vcyc=%u\n", r->cz101_board__DOT__vcyc);
            if (r->cz101_board__DOT__t_fire) fprintf(stderr, "fire at=%u vcyc=%u\n", r->cz101_board__DOT__tmr_cycle, r->cz101_board__DOT__vcyc);
        }
        if (c->wr_done) fprintf(out, "W %04X %02X\n", c->wr_addr, c->wr_data);
        if (c->insn_done) {
            fprintf(out, "%llu %04X %02X %02X %d %02X%02X%02X%02X%02X%02X%02X%02X %04X %04X %02X %04X\n",
                    (unsigned long long)cyc, c->insn_pc, c->insn_op, c->insn_op2, c->insn_cc,
                    c->r_v, c->r_a, c->r_b, c->r_c, c->r_d, c->r_e, c->r_h, c->r_l, c->r_ea,
                    c->r_sp, c->r_psw, c->r_irr);
            cyc += c->insn_cc;
            count++;
        }
        if (c->halt_cycle) cyc++;
        if (c->irq_taken) { fprintf(out, "I %04X\n", c->irq_taken_vec); irqs_taken++; nmis_taken += c->irq_taken_vec == 0x0004; }
        if (c->sample_valid) { samples++; if (smp) { int32_t s = ((int32_t)(c->sample_sum << 13)) >> 13; fwrite(&s, 4, 1, smp); } }
    }
    fclose(out);
    if (smp) fclose(smp);
    if (lcdlog) fclose(lcdlog);
    if (txlog) fclose(txlog);
    if (rxline) fclose(rxline);
    if (const char *nf = getenv("CZ_NV_OUT")) {
        FILE *f = fopen(nf, "wb");
        if (!f) { perror(nf); exit(1); }
        c->nv_we = 0;
        for (int a = 0; a < 0x2000; a++) { c->nv_addr = a; tick(); fputc(c->nv_rdata, f); }
        fclose(f);
    }
    // LCD text
    std::string l[2];
    for (int line = 0; line < 2; line++)
        for (int p = 0; p < 16; p++) {
            c->r_line = line; c->r_pos = p; c->eval();
            uint8_t ch = c->r_code; l[line] += (ch >= 0x20 && ch < 0x7F) ? (char)ch : '.';
        }
    printf("%llu instructions, %llu cycles, %llu samples, %zu/%zu MIDI-in changes, %zu/%zu MIDI bytes, late %d\nLCD [%s]\n    [%s]\n",
           (unsigned long long)count, (unsigned long long)cyc, (unsigned long long)samples, rxi, rx.size(),
           mbi, mb.size(), c->upd933_late, l[0].c_str(), l[1].c_str());
    printf("board: cycles %u, powered_down %d at %lld, samples after power-down %llu, lcd_power %d, "
           "uPD933 IRQ rises %llu (first at %lld), timer armed %llu (last n %u)\n",
           c->cycles, c->powered_down, (long long)down_at, (unsigned long long)down_samples, c->lcd_power,
           (unsigned long long)irq_rises, (long long)irq_first, (unsigned long long)arms, arm_n);
    printf("cpu: interrupts %llu, NMI %llu, vector changed during the push %u\n",
           (unsigned long long)irqs_taken, (unsigned long long)nmis_taken,
           c->rootp->cz101_board__DOT__cpu__DOT__core__DOT__sim_vec_changes);
    delete c;
    return (clocks || count == n) ? 0 : 1;
}
