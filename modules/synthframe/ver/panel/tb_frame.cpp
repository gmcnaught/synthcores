// SPDX-License-Identifier: MIT
// Panel frame bench driver: for each seed, load the bench's contents, run WARM clocks, then
// hash every clock's video outputs for CLKS clocks (FNV-1a 64). Prints "<bench> <seed> <hash>".
// PPM=<prefix>: also writes the last complete frame of each seed as <prefix><seed>.ppm (visible
// pixels, the ones outside the blanking intervals; Proteus counts one pixel per ce_pix).
// Build with -DBENCH_TOP=<Verilated class> -DBENCH_H=<header> -DNAME=<bench name>.
#include BENCH_H
#include "verilated.h"
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <vector>

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    if (argc < 4) { fprintf(stderr, "usage: %s WARM CLKS SEED...\n", argv[0]); return 2; }
    uint64_t warm = strtoull(argv[1], nullptr, 0), clks = strtoull(argv[2], nullptr, 0);
    for (int a = 3; a < argc; ++a) {
        uint32_t seed = (uint32_t)strtoul(argv[a], nullptr, 0);
        BENCH_TOP *t = new BENCH_TOP;
        auto tick = [&] { t->clk = 0; t->eval(); t->clk = 1; t->eval(); };
        t->seed = seed;
        t->load = 1;
        for (int i = 0; i < 4; ++i) tick();
        t->load = 0;
        for (uint64_t i = 0; i < warm; ++i) tick();
        uint64_t h = 1469598103934665603ull;
        auto mix = [&](uint64_t v, int bytes) { for (int i = 0; i < bytes; ++i) { h ^= (v >> (8 * i)) & 0xff; h *= 1099511628211ull; } };
        const char *ppm = getenv("PPM");
        std::vector<uint8_t> line, frame, last;
        int w = 0, hgt = 0, lw = 0, lh = 0, cur_w = 0;
        bool pv = false, pb = true;
        for (uint64_t i = 0; i < clks; ++i) {
            tick();
            if (ppm && (NAME[0] != 'p' || (t->aux & 1))) {
                bool vis = !t->hblank && !t->vblank;
                if (vis) { line.push_back(t->r); line.push_back(t->g); line.push_back(t->b); ++cur_w; }
                if (!vis && pv && !t->vblank) { frame.insert(frame.end(), line.begin(), line.end()); w = cur_w; ++hgt; line.clear(); cur_w = 0; }
                if (t->vblank && !pb) { if (hgt) { last = frame; lw = w; lh = hgt; } frame.clear(); hgt = 0; line.clear(); cur_w = 0; }
                pv = vis; pb = t->vblank;
            }
            mix(t->hs | t->vs << 1 | t->hblank << 2 | t->vblank << 3, 1);
            mix((uint64_t)t->r << 16 | (uint64_t)t->g << 8 | t->b, 3);
            mix(t->aux, 4);
        }
        if (ppm && lh) {
            char fn[512]; snprintf(fn, sizeof fn, "%s%08x.ppm", ppm, seed);
            FILE *f = fopen(fn, "wb"); fprintf(f, "P6\n%d %d\n255\n", lw, lh); fwrite(last.data(), 1, last.size(), f); fclose(f);
        }
        printf("%s 0x%08x %016llx\n", NAME, seed, (unsigned long long)h);
        delete t;
    }
    return 0;
}
