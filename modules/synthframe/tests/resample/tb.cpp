// SPDX-License-Identifier: MIT
// synthframe_resample bench: IN (one "l r" pair a line, decimal) -> OUT (one "l r" a line).
// An input every GAP clocks (default 1250: 40 kHz at 50 MHz); GAP 0: bursts of 6 inputs on
// consecutive clocks, then 6 x 1250 clocks of nothing (the queue holds 8).
#include "Vsynthframe_resample.h"
#include "verilated.h"
#include <cstdio>
#include <cstdlib>
#include <vector>
int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    FILE *fi = fopen(argv[1], "r"), *fo = fopen(argv[2], "w");
    int gap = argc > 3 ? atoi(argv[3]) : 1250;
    std::vector<std::pair<int, int>> in;
    int l, r;
    while (fscanf(fi, "%d %d", &l, &r) == 2) in.push_back({l, r});
    auto *m = new Vsynthframe_resample;
    auto tick = [&] { m->clk = 0; m->eval(); m->clk = 1; m->eval(); if (m->out_valid) fprintf(fo, "%d %d\n", (int16_t)m->out_l, (int16_t)m->out_r); };
    m->rst = 1; m->in_valid = 0; for (int i = 0; i < 4; i++) tick();
    m->rst = 0; for (int i = 0; i < 200; i++) tick();
    for (size_t k = 0; k < in.size(); k++) {
        m->in_valid = 1; m->in_l = in[k].first; m->in_r = in[k].second; tick();
        m->in_valid = 0;
        int wait = gap ? gap - 1 : (k % 6 == 5 ? 6 * 1250 : 0);
        for (int i = 0; i < wait; i++) tick();
    }
    for (int i = 0; i < 2000; i++) tick();
    fclose(fo); delete m;
    return 0;
}
