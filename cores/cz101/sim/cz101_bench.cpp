// CZ-101 board bench: the real firmware from power-on, trace in the patched MAME's format.
//   cz101_bench ROMDIR N OUT.trace [SAMPLES.s16]
// ROMDIR holds MAME's cz101 set (hn613256ps40.bin, init_ram.bin). Power switch on from time 0
// (MAME: CZ_POWER_ON=1), no keys, pitch wheel centred, MIDI in idle.
#include "Vcz101_board.h"
#include "verilated.h"
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

    Verilated::commandArgs(argc, argv);
    auto *c = new Vcz101_board;
    auto tick = [&] { c->clk = 0; c->eval(); c->clk = 1; c->eval(); };
    c->reset = 1; c->cyc_ce = 1; c->power_on = 1; c->pitch_wheel = 0x7F; c->midi_rx = 1;
    for (int i = 0; i < 16; i++) c->keys[i] = 0xFF;
    c->r_line = 0; c->r_pos = 0; c->r_cg_code = 0; c->r_cg_row = 0;
    for (int a = 0; a < 0x9000; a++) {
        c->load_wr = 1; c->load_addr = a; c->load_data = a < 0x8000 ? prog[a] : ram[a - 0x8000];
        tick();
    }
    c->load_wr = 0;
    for (int i = 0; i < 300; i++) tick();   // LCD clear loop
    c->reset = 0;

    uint64_t cyc = 0, count = 0, samples = 0;
    for (uint64_t clk = 0; count < n && clk < n * 2000 + 100000; clk++) {
        tick();
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
        if (c->irq_taken) fprintf(out, "I %04X\n", c->irq_taken_vec);
        if (c->sample_valid) { samples++; if (smp) { int16_t s = c->sample; fwrite(&s, 2, 1, smp); } }
    }
    fclose(out);
    if (smp) fclose(smp);
    // LCD text
    std::string l[2];
    for (int line = 0; line < 2; line++)
        for (int p = 0; p < 16; p++) {
            c->r_line = line; c->r_pos = p; c->eval();
            uint8_t ch = c->r_code; l[line] += (ch >= 0x20 && ch < 0x7F) ? (char)ch : '.';
        }
    printf("%llu instructions, %llu cycles, %llu samples, late %d\nLCD [%s]\n    [%s]\n",
           (unsigned long long)count, (unsigned long long)cyc, (unsigned long long)samples,
           c->upd933_late, l[0].c_str(), l[1].c_str());
    delete c;
    return count == n ? 0 : 1;
}
