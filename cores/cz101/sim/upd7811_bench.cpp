// uPD7811 bench (core + peripherals + internal memory) on a stub CZ-101 board: writes a trace
// in the patched MAME's format (cores/cz101/mame/patches/0001-cz101-logging.patch).
//   upd7811_bench ROM.bin N OUT.trace
// Board stub, as MAME's cz101 machine with nothing pressed and the uPD933 idle:
//   0000-7FFF program ROM (the image; its first 4 KB is also the internal ROM),
//   8000-8FFF RAM, other external reads FF. Port B input bit 4 = uPD933 /RQ (idle high),
//   AN1 = 7F (pitch wheel centre), AN2 = FF (battery normal), RxD idle high.
#include "Vupd7811.h"
#include "verilated.h"
#include "Vupd7811___024root.h"
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <vector>

int main(int argc, char **argv) {
    if (argc < 4) { fprintf(stderr, "usage: %s ROM N OUT\n", argv[0]); return 2; }
    std::vector<uint8_t> mem(0x10000, 0xFF);
    FILE *f = fopen(argv[1], "rb");
    if (!f) { perror(argv[1]); return 1; }
    fread(mem.data(), 1, 0x8000, f);
    fclose(f);
    for (int a = 0x8000; a < 0x9000; a++) mem[a] = 0;
    const uint64_t n = strtoull(argv[2], nullptr, 0);
    FILE *out = fopen(argv[3], "w");

    Verilated::commandArgs(argc, argv);
    auto *c = new Vupd7811;
    auto tick = [&] { c->clk = 0; c->eval(); c->clk = 1; c->eval(); };
    c->reset = 1; c->cyc_ce = 1;
    c->pa_in = 0; c->pb_in = 0x10; c->pc_in = 0; c->pd_in = 0; c->pf_in = 0;
    for (int i = 0; i < 8; i++) c->an[i] = 0;
    c->an[1] = 0x7F; c->an[2] = 0xFF;
    c->rxd = 1; c->sck_edge = 0; c->sck_level = 0;
    c->nmi_line = 0; c->int1_line = 0; c->int2_line = 0;
    for (int a = 0; a < 4096; a++) { c->rom_wr = 1; c->rom_addr = a; c->rom_data = mem[a]; tick(); }
    c->rom_wr = 0;
    for (int i = 0; i < 4; i++) tick();
    c->reset = 0;

    uint64_t cyc = 0, count = 0;
    bool pend = false;
    for (uint64_t clk = 0; count < n && clk < n * 400 + 100000; clk++) {
        c->ext_ack = 0;
        if (pend) {
            c->ext_ack = 1;
            if (c->ext_we) { if (c->ext_addr >= 0x8000 && c->ext_addr < 0x9000) mem[c->ext_addr] = c->ext_wdata; }
            else c->ext_rdata = mem[c->ext_addr];
            pend = false;
        }
        tick();
        if (c->wr_done) fprintf(out, "W %04X %02X\n", c->wr_addr, c->wr_data);
        if (c->insn_done) {
            fprintf(out, "%llu %04X %02X %02X %d %02X%02X%02X%02X%02X%02X%02X%02X %04X %04X %02X %04X\n",
                    (unsigned long long)cyc, c->insn_pc, c->insn_op, c->insn_op2, c->insn_cc,
                    c->r_v, c->r_a, c->r_b, c->r_c, c->r_d, c->r_e, c->r_h, c->r_l, c->r_ea,
                    c->r_sp, c->r_psw, c->r_irr);
            if (getenv("CZ_TRACE_AD")) {
                auto *r = c->rootp;
#define P(x) r->upd7811__DOT__periph__DOT__##x
                fprintf(out, "AD %u %u %u %u %02X %02X T %02X %d %02X %02X %02X %d %02X\n", P(adcnt), P(adtot), P(adout),
                        P(shdone), P(ANM), P(PANM), P(CNT0), P(ovc0), P(TM0), P(TMM), P(CNT1), P(ovc1), P(TM1));
                fprintf(out, "SIO %02X %02X %d %d %d %d %04X %04X %d\n", P(SMH), P(SML), P(ovcs), P(edges), P(rxcnt),
                        P(txcnt), P(rxs), P(txs), P(txbuf));
            }
            cyc += c->insn_cc;
            count++;
        }
        if (c->halt_cycle) cyc++;
        if (c->irq_taken) fprintf(out, "I %04X\n", c->irq_taken_vec);
        if (c->ext_req && !c->ext_ack && !pend) pend = true;
    }
    fclose(out);
    delete c;
    printf("%llu instructions\n", (unsigned long long)count);
    return count == n ? 0 : 1;
}
