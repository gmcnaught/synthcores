// hdl/board/rom2x_cache.sv against a memory model: random byte reads (with
// locality and reuse, so lines hit, miss and get replaced), the line port
// answered after a random latency, flushes now and then. Every byte returned
// must be the memory's. Prints "OK: ..." (tests/test_board_units.py).
#include "Vrom2x_cache.h"
#include "verilated.h"
#include <cinttypes>
#include <cstdio>
#include <random>
#include <vector>

int main(int argc, char** argv)
{
    VerilatedContext ctx;
    ctx.commandArgs(argc, argv);
    Vrom2x_cache t{&ctx};
    std::mt19937_64 rng(3);
    std::vector<uint8_t> mem(1 << 18);
    for (auto& b : mem) b = (uint8_t)rng();
    auto tick = [&]() { t.clk = 0; t.eval(); t.clk = 1; t.eval(); };
    t.flush = 1; t.req = 0; t.lvalid = 0;
    for (int i = 0; i < 4; ++i) tick();
    t.flush = 0;
    uint64_t reads = 0, fills = 0, clocks = 0;
    uint32_t base = 0;
    int pending = -1;          // clocks until the line read answers
    for (int n = 0; n < 200000; ++n)
    {
        if (rng() % 64 == 0) base = (uint32_t)(rng() % (1 << 18));
        uint32_t a = (base + (uint32_t)(rng() % 512)) & 0x3ffff;
        if (rng() % 5000 == 0) { t.flush = 1; tick(); t.flush = 0; }
        t.req = 1; t.addr = a;
        int guard = 0;
        for (;;)
        {
            t.lvalid = 0;
            if (t.lreq && pending < 0) pending = 3 + (int)(rng() % 40);
            if (pending == 0)
            {
                uint64_t w = 0;
                for (int b = 0; b < 8; ++b) w |= (uint64_t)mem[(t.laddr * 8 + b) & 0x3ffff] << (8 * b);
                t.ldata = w; t.lvalid = 1; pending = -1; ++fills;
            }
            else if (pending > 0) --pending;
            tick(); ++clocks;
            if (t.ack) break;
            if (++guard > 1000) { fprintf(stderr, "no ack for %05x\n", a); return 1; }
        }
        t.req = 0; t.lvalid = 0;
        if (t.data != mem[a]) { fprintf(stderr, "MISMATCH %05x: %02x, memory %02x\n", a, t.data, mem[a]); return 1; }
        ++reads;
        tick(); ++clocks;
    }
    printf("OK: %" PRIu64 " reads, %" PRIu64 " line fills (%.1f %% misses), %.2f clocks a read\n", reads, fills,
           100.0 * (double)fills / (double)reads, (double)clocks / (double)reads);
    return 0;
}
