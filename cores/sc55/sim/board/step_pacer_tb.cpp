// modules/roland/hdl/board/step_pacer.sv: a stall at the credit cap must count every lost
// microsecond in lag_us (review 2026-11-26 roland.md #6). At 50 MHz with no steps the credit
// reaches CAP (1000 us) after 50,000 clocks; from then on 1000/1000 clock a clock is lost, so
// lag_us must rise by exactly one every 50 clocks. Then real-time stepping (one step every 30
// clocks, 600 ns) must lose nothing more. Prints "OK: ..." (tests/test_board_units.py).
#include "Vstep_pacer.h"
#include "verilated.h"
#include <cinttypes>
#include <cstdio>

int main(int argc, char** argv)
{
    VerilatedContext ctx;
    ctx.commandArgs(argc, argv);
    Vstep_pacer t{&ctx};
    auto tick = [&]() { t.clk = 0; t.eval(); t.clk = 1; t.eval(); };
    t.rst = 1; t.enable = 1; t.fast = 0; t.step_done = 0;
    for (int i = 0; i < 4; ++i) tick();
    t.rst = 0;
    const uint64_t cap_clocks = 50'000, stall = 1'000'000;
    for (uint64_t c = 0; c < stall; ++c) tick();
    const uint64_t want = (stall - cap_clocks) / 50;
    if (t.lag_us != want)
    {
        printf("FAIL: after a %" PRIu64 "-clock stall lag_us = %u, want %" PRIu64 "\n", stall, t.lag_us, want);
        return 1;
    }
    // real time: a step every 30 clocks keeps the credit where it is
    for (int s = 0; s < 100'000; ++s)
    {
        for (int c = 0; c < 29; ++c) tick();
        t.step_done = 1; tick(); t.step_done = 0;
    }
    if (t.lag_us != want)
    {
        printf("FAIL: lag_us moved to %u while stepping in real time (want %" PRIu64 ")\n", t.lag_us, want);
        return 1;
    }
    printf("OK: %" PRIu64 "-clock stall -> lag_us %u (exact), unchanged over 100000 real-time steps\n", stall, t.lag_us);
    return 0;
}
