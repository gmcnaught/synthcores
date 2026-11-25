// SPDX-License-Identifier: MIT
// synthframe_midi_tx bench: pushes "CLOCK BYTE" lines from a script (BYTE on that clock; several
// on one clock are pushed on consecutive clocks), decodes txd with a software UART sampling at
// the bit centres (DIV clocks a bit), and prints the decoded bytes and the dropped count.
//   tb_midi_tx SCRIPT ALT DIV
#include "Vsynthframe_midi_tx.h"
#include "verilated.h"
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <utility>

int main(int argc, char** argv)
{
    if (argc != 4) { fprintf(stderr, "usage: tb_midi_tx SCRIPT ALT DIV\n"); return 2; }
    FILE* f = fopen(argv[1], "r");
    if (!f) { perror(argv[1]); return 2; }
    std::deque<std::pair<unsigned long long, unsigned>> in;
    unsigned long long c; unsigned b;
    while (fscanf(f, "%llu %x", &c, &b) == 2) in.push_back({c, b});
    fclose(f);
    const int alt = atoi(argv[2]);
    const long div = atol(argv[3]);

    VerilatedContext ctx;
    Vsynthframe_midi_tx t{&ctx};
    t.clk = 0; t.rst = 1; t.alt = alt; t.valid = 0; t.data = 0;
    auto tick = [&]() { t.clk = 0; t.eval(); t.clk = 1; t.eval(); };
    for (int i = 0; i < 4; ++i) tick();
    t.rst = 0;

    // software UART
    long rx_at = -1;            // clock of the next sample, -1 idle
    int  rx_bit = 0, prev = 1, bad = 0, bytes = 0;
    unsigned rx_sh = 0;
    unsigned long long clk = 0, idle_since = 0;
    while (true)
    {
        t.valid = 0;
        if (!in.empty() && in.front().first <= clk) { t.valid = 1; t.data = in.front().second; in.pop_front(); }
        tick();
        ++clk;
        const int line = t.txd;
        if (rx_at < 0)
        {
            if (prev && !line) { rx_at = (long)clk + div / 2; rx_bit = 0; rx_sh = 0; }
        }
        else if ((long)clk == rx_at)
        {
            if (rx_bit == 0 && line) { printf("E start\n"); ++bad; rx_at = -1; }
            else if (rx_bit >= 1 && rx_bit <= 8) rx_sh |= (unsigned)line << (rx_bit - 1);
            else if (rx_bit == 9)
            {
                if (!line) { printf("E stop\n"); ++bad; }
                else { printf("B %02x %llu\n", rx_sh, clk); ++bytes; }
                rx_at = -1;
            }
            if (rx_at >= 0) { ++rx_bit; rx_at += div; }
        }
        prev = line;
        if (in.empty() && !t.busy && rx_at < 0 && line) { if (++idle_since > (unsigned long long)div * 2) break; }
        else idle_since = 0;
    }
    printf("D %u\n", (unsigned)t.dropped);
    return bad ? 1 : 0;
}
