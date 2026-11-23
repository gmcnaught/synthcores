// uPD7810 core bench: runs a 32 KB program image and writes a trace in the format of the
// patched MAME (cores/cz101/mame/patches/0001-cz101-logging.patch), so the two diff directly.
//   upd7810_bench ROM.bin N OUT.trace
// Memory: 0000-7FFF ROM (the image), the rest RAM. Special registers are plain storage
// (EOM reads masked with 0x22 as MAME), no interrupts, peripherals advance instantly.
#include "Vupd7810_core.h"
#include "verilated.h"
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <vector>

int main(int argc, char **argv) {
    if (argc < 4) { fprintf(stderr, "usage: %s ROM N OUT\n", argv[0]); return 2; }
    std::vector<uint8_t> mem(0x10000, 0);
    FILE *f = fopen(argv[1], "rb");
    if (!f) { perror(argv[1]); return 1; }
    fread(mem.data(), 1, 0x8000, f);
    fclose(f);
    const uint64_t n = strtoull(argv[2], nullptr, 0);
    FILE *out = fopen(argv[3], "w");

    Verilated::commandArgs(argc, argv);
    auto *c = new Vupd7810_core;
    uint16_t sfr[64] = {0};
    c->reset = 1; c->cyc_ce = 1;
    c->irq_vec = 0; c->irq_nmi = 0;
    auto tick = [&] { c->clk = 0; c->eval(); c->clk = 1; c->eval(); };
    for (int i = 0; i < 4; i++) tick();
    c->reset = 0;

    uint64_t cyc = 0, count = 0;
    bool mem_pend = false, sfr_pend = false, skit_pend = false, adv_pend = false;
    for (uint64_t clk = 0; count < n && clk < n * 200 + 1000; clk++) {
        // respond to requests seen at the last edge
        c->mem_ack = 0; c->sfr_ack = 0; c->skit_ack = 0; c->adv_ack = 0;
        if (mem_pend) {
            c->mem_ack = 1;
            if (c->mem_we) {
                fprintf(out, "W %04X %02X\n", c->mem_addr, c->mem_wdata);
                if (c->mem_addr >= 0x8000) mem[c->mem_addr] = c->mem_wdata;
            } else c->mem_rdata = mem[c->mem_addr];
            mem_pend = false;
        }
        if (sfr_pend) {
            c->sfr_ack = 1;
            if (c->sfr_we) sfr[c->sfr_id] = c->sfr_wdata;
            else c->sfr_rdata = c->sfr_id == 10 ? (sfr[10] & 0x22) : sfr[c->sfr_id];
            sfr_pend = false;
        }
        if (skit_pend) { c->skit_ack = 1; c->skit_flag = 0; skit_pend = false; }
        if (adv_pend) { c->adv_ack = 1; adv_pend = false; }
        tick();
        if (c->insn_done) {
            fprintf(out, "%llu %04X %02X %02X %d %02X%02X%02X%02X%02X%02X%02X%02X %04X %04X %02X %04X\n",
                    (unsigned long long)cyc, c->insn_pc, c->insn_op, c->insn_op2, c->insn_cc,
                    c->r_v, c->r_a, c->r_b, c->r_c, c->r_d, c->r_e, c->r_h, c->r_l, c->r_ea,
                    c->r_sp, c->r_psw, 0);
            cyc += c->insn_cc;
            count++;
        }
        if (c->halt_cycle) cyc++;
        if (c->irq_taken) fprintf(out, "I %04X\n", c->irq_taken_vec);
        if (c->mem_req && !c->mem_ack && !mem_pend) mem_pend = true;
        if (c->sfr_req && !c->sfr_ack && !sfr_pend) sfr_pend = true;
        if (c->skit_req && !c->skit_ack && !skit_pend) skit_pend = true;
        if (c->adv_req && !c->adv_ack && !adv_pend) adv_pend = true;
    }
    fclose(out);
    delete c;
    printf("%llu instructions\n", (unsigned long long)count);
    return count == n ? 0 : 1;
}
