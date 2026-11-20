// Feeds hdl/board/resample.sv the 16-bit stereo samples read from stdin
// ("l r" per line), two at a time like the PCM's bursts, and prints each
// output "l r" (tests/test_resample.py compares with the Python model).
#include "Vresample.h"
#include "verilated.h"
#include <cstdio>
#include <vector>

int main(int argc, char** argv)
{
    VerilatedContext ctx;
    ctx.commandArgs(argc, argv);
    Vresample t{&ctx};
    auto tick = [&]() {
        t.clk = 0; t.eval(); t.clk = 1; t.eval();
        if (t.out_valid) printf("%d %d\n", (int16_t)t.out_l, (int16_t)t.out_r);
    };
    t.rst = 1; t.in_valid = 0;
    for (int i = 0; i < 4; ++i) tick();
    t.rst = 0;
    std::vector<std::pair<int, int>> in;
    int l, r;
    while (scanf("%d %d", &l, &r) == 2) in.push_back({l, r});
    for (size_t i = 0; i < in.size(); i += 2)
    {
        for (size_t k = i; k < i + 2 && k < in.size(); ++k)
        {
            t.in_valid = 1; t.in_l = (int16_t)in[k].first; t.in_r = (int16_t)in[k].second;
            tick();
        }
        t.in_valid = 0;
        for (int g = 0; g < 300; ++g) tick();     // a frame's worth of clocks
    }
    for (int g = 0; g < 400; ++g) tick();
    return 0;
}
