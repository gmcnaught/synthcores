// SPDX-License-Identifier: MIT
// Replay a MAME SWP00 write log through the C++ reference model (ref/swp00_ref, MAME's code) and
// write the samples in MAME's MU_SAMPLES format (int32 L, R per sample pair).
//   swp00_replay WAVE_ROM SND_LOG NSAMPLES OUT.raw [READ_LOG]
// WAVE_ROM: the 4 MB wave ROM (xq057c0 then xq058c0), or a generated one.
// SND_LOG: MU_SND_LOG lines "<cpu states> <samples before> <byte address> <data> <mem_mask>";
// each line's bytes land after <samples before> samples. A word write (mask FFFF) is two
// byte writes, the even address (high byte) first. READ_LOG (MU_SND_RLOG, same format, the data
// MAME returned): each read is replayed in time order with the writes, and its value checked.
#include "swp00_ref.h"

#include <algorithm>
#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

int main(int argc, char **argv) {
    if (argc < 5) { fprintf(stderr, "usage: %s WAVE_ROM SND_LOG NSAMPLES OUT.raw\n", argv[0]); return 2; }
    std::vector<uint8_t> rom(0x400000, 0);
    FILE *f = fopen(argv[1], "rb");
    if (!f) { perror(argv[1]); return 1; }
    size_t got = fread(rom.data(), 1, rom.size(), f);
    fclose(f);
    if (got != rom.size()) { fprintf(stderr, "%s: %zu bytes, want 4 MB\n", argv[1], got); return 1; }
    FILE *log = fopen(argv[2], "r");
    if (!log) { perror(argv[2]); return 1; }
    const uint64_t total = strtoull(argv[3], nullptr, 0);
    FILE *out = fopen(argv[4], "wb");
    if (!out) { perror(argv[4]); return 1; }

    auto *swp = new swp00ref::swp00_ref(rom.data());
    uint64_t made = 0;
    std::vector<int32_t> buf;
    auto run_to = [&](uint64_t n) {
        if (n <= made) return;
        buf.resize(2 * (n - made));
        swp->generate(int(n - made), buf.data());
        fwrite(buf.data(), sizeof(int32_t), buf.size(), out);
        made = n;
    };
    FILE *rlog = argc > 5 ? fopen(argv[5], "r") : nullptr;
    struct ev { unsigned long long states, before; unsigned adr, data, mask; bool ok; };
    auto next = [](FILE *fp, ev &e) {
        char line[256];
        e.ok = false;
        while (fp && fgets(line, sizeof line, fp))
            if (sscanf(line, "%llu %llu %x %x %x", &e.states, &e.before, &e.adr, &e.data, &e.mask) == 5)
                return e.ok = true;
        return false;
    };
    ev w, r;
    next(log, w);
    next(rlog, r);
    uint64_t nreads = 0, bad = 0;
    while (w.ok || r.ok) {
        // in CPU time order (a read and a write never share a state)
        if (w.ok && (!r.ok || w.states < r.states)) {
            run_to(w.before);
            if (w.mask & 0xff00) swp->write(uint16_t(w.adr & 0x7fe), uint8_t(w.data >> 8));
            if (w.mask & 0x00ff) swp->write(uint16_t((w.adr & 0x7fe) | 1), uint8_t(w.data));
            next(log, w);
        } else {
            run_to(r.before);
            unsigned v = 0;
            if (r.mask & 0xff00) v |= swp->read(uint16_t(r.adr & 0x7fe)) << 8;
            if (r.mask & 0x00ff) v |= swp->read(uint16_t((r.adr & 0x7fe) | 1));
            nreads++;
            if ((v & r.mask) != (r.data & r.mask) && bad++ < 10)
                fprintf(stderr, "read %03X at state %llu: model %04X, MAME %04X\n", r.adr, r.states, v, r.data);
            next(rlog, r);
        }
    }
    run_to(total);
    if (rlog) fprintf(stderr, "%llu reads, %llu different\n", (unsigned long long)nreads, (unsigned long long)bad);
    if (getenv("SWP00_PEAK")) {
        // widest value now in each delay line (every value lives at least 0x8000 samples there)
        auto bitsof = [](int32_t v) { int b = 1; while (b < 32 && (v < -(1 << (b - 1)) || v >= (1 << (b - 1)))) b++; return b; };
        int br = 0, bc = 0, bv = 0;
        for (auto v : swp->m_rev_buffer) br = std::max(br, bitsof(v));
        for (auto v : swp->m_cho_buffer) bc = std::max(bc, bitsof(v));
        for (auto v : swp->m_var_buffer) bv = std::max(bv, bitsof(v));
        fprintf(stderr, "delay line widths (signed bits) at the end: reverb %d, chorus %d, variation %d\n", br, bc, bv);
    }
    fclose(out);
    return 0;
}
