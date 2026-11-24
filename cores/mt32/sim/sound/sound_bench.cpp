// SPDX-License-Identifier: GPL-2.0-or-later
// MT-32 sound section bench (plan milestones M3, M4): replays the reference's logs through
// mt32_sound and compares every sample, every LA32 interrupt and optionally every slot.
//
//   sound_bench --roms DIR --la32-log F --rv-log F --out-log F --irq-log F [--slot-log F]
//               [--samples N] [--seed S] [--maxlat N] [--quiet]
//
// The reference (build/ref/mt32_ref) writes the logs for a MIDI case. A write logged with LA32
// cycle L was made during the CPU turn after the slot ending at L; the bench makes it in the
// same turn. pwm comes from the out-log's vol (pwm = 255 - vol) at slot 31's turn. The PCM ROM
// is served through an in-order memory model with random latency 1..maxlat clocks and random
// req_ready.
#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <random>
#include <string>
#include <vector>
#include "Vmt32_sound.h"
#include "verilated.h"

struct Wr { uint64_t cyc; uint32_t a, d; };
struct Irq { char kind; uint64_t cyc; uint32_t st; };
struct Out { int v[9]; };
struct Slot { char k; uint64_t cyc; uint32_t i; long long v[6]; };

static FILE* open_or_die(const char* p, const char* m)
{
    FILE* f = fopen(p, m);
    if (!f) { fprintf(stderr, "sound_bench: can't open %s\n", p); exit(2); }
    return f;
}

int main(int argc, char** argv)
{
    std::string roms, la32_log, rv_log, out_log, irq_log, slot_log;
    uint64_t samples = UINT64_MAX, seed = 1;
    int maxlat = 40;
    bool quiet = false;
    for (int i = 1; i < argc; i++) {
        std::string a = argv[i];
        auto v = [&]() { if (i + 1 >= argc) exit(2); return std::string(argv[++i]); };
        if (a == "--roms") roms = v();
        else if (a == "--la32-log") la32_log = v();
        else if (a == "--rv-log") rv_log = v();
        else if (a == "--out-log") out_log = v();
        else if (a == "--irq-log") irq_log = v();
        else if (a == "--slot-log") slot_log = v();
        else if (a == "--samples") samples = strtoull(v().c_str(), nullptr, 0);
        else if (a == "--seed") seed = strtoull(v().c_str(), nullptr, 0);
        else if (a == "--maxlat") maxlat = atoi(v().c_str());
        else if (a == "--quiet") quiet = true;
        else { fprintf(stderr, "sound_bench: unknown option %s\n", a.c_str()); return 2; }
    }

    // ---- inputs
    std::vector<uint8_t> pcm(0x80000), rvrom(0x8000);
    {
        FILE* f = open_or_die((roms + "/mt32_pcm.bin").c_str(), "rb");
        if (fread(pcm.data(), 1, pcm.size(), f) != pcm.size()) return 2;
        fclose(f);
        f = open_or_die((roms + "/mt32_reverb.bin").c_str(), "rb");
        if (fread(rvrom.data(), 1, rvrom.size(), f) != rvrom.size()) return 2;
        fclose(f);
    }
    std::vector<Wr> lw, rw;
    std::vector<Irq> irqs;
    std::vector<Out> outs;
    char line[512];
    {
        FILE* f = open_or_die(la32_log.c_str(), "r");
        while (fgets(line, sizeof line, f)) {
            Wr w; if (sscanf(line, "L %" SCNx64 " %x %x", &w.cyc, &w.a, &w.d) == 3) lw.push_back(w);
        }
        fclose(f);
        f = open_or_die(rv_log.c_str(), "r");
        while (fgets(line, sizeof line, f)) {
            Wr w; if (sscanf(line, "W %" SCNx64 " %x %x", &w.cyc, &w.a, &w.d) == 3) rw.push_back(w);
        }
        fclose(f);
        f = open_or_die(irq_log.c_str(), "r");
        while (fgets(line, sizeof line, f)) {
            Irq q; if (sscanf(line, "%c %" SCNx64 " %x", &q.kind, &q.cyc, &q.st) == 3) irqs.push_back(q);
        }
        fclose(f);
        f = open_or_die(out_log.c_str(), "r");
        while (fgets(line, sizeof line, f)) {
            Out o;
            if (sscanf(line, "%d %d %d %d %d %d %d %d %d", &o.v[0], &o.v[1], &o.v[2], &o.v[3], &o.v[4],
                       &o.v[5], &o.v[6], &o.v[7], &o.v[8]) == 9) outs.push_back(o);
        }
        fclose(f);
    }
    FILE* fslot = slot_log.empty() ? nullptr : open_or_die(slot_log.c_str(), "r");
    if (samples > outs.size()) samples = outs.size();

    // ---- model
    Verilated::commandArgs(argc, argv);
    Vmt32_sound* m = new Vmt32_sound;
    std::mt19937_64 rng(seed);
    uint64_t clk_count = 0;
    struct Rsp { uint64_t t; uint64_t data; uint8_t tag; };
    std::deque<Rsp> rq;
    uint64_t last_rsp_t = 0;
    uint64_t pcm_reads = 0;

    auto tick = [&]() {
        // memory model, inputs for this clock
        m->pcm_rsp_valid = 0;
        if (!rq.empty() && rq.front().t <= clk_count) {
            m->pcm_rsp_valid = 1;
            m->pcm_rsp_data = rq.front().data;
            m->pcm_rsp_tag = rq.front().tag;
            rq.pop_front();
        }
        m->pcm_req_ready = (rng() % 4) != 0;
        m->eval();
        if (m->pcm_req_valid && m->pcm_req_ready) {
            uint64_t w = m->pcm_req_addr, data = 0;
            for (int b = 0; b < 8; b++) data |= (uint64_t)pcm[(w * 8 + b) & 0x7ffff] << (8 * b);
            uint64_t t = clk_count + 1 + rng() % maxlat;
            if (t <= last_rsp_t) t = last_rsp_t + 1;
            last_rsp_t = t;
            rq.push_back({t, data, (uint8_t)m->pcm_req_tag});
            pcm_reads++;
        }
        m->clk = 1; m->eval();
        m->clk = 0; m->eval();
        clk_count++;
    };

    m->clk = 0; m->rst = 1; m->run = 0; m->cpu_done = 0;
    m->bus_we = 0; m->bus_addr = 0; m->rv_we = 0; m->pwm = 0; m->rvrom_we = 0;
    for (int k = 0; k < 4; k++) tick();
    m->rst = 0;
    for (int a = 0; a < 0x8000; a++) {
        m->rvrom_we = 1; m->rvrom_addr = a; m->rvrom_data = rvrom[a];
        tick();
    }
    m->rvrom_we = 0;
    m->run = 1;

    size_t li = 0, ri = 0, ii = 0;
    uint64_t sample = 0, slots = 0, slot_clk_max = 0, slot_clk_sum = 0, slot_start = clk_count;
    int prev_ext = 0;
    uint64_t slot_start_cyc = 0;
    int errors = 0;
    auto fail = [&](const char* what) {
        fprintf(stderr, "sound_bench: %s at sample %" PRIu64 " (la32 cycle %" PRIx64 ")\n", what, sample,
                (uint64_t)m->la32_cycle);
        errors++;
    };
    auto check_irq = [&](char kind, uint64_t cyc, uint32_t st) {
        if (ii >= irqs.size() || irqs[ii].kind != kind || irqs[ii].cyc != cyc || irqs[ii].st != st) {
            fprintf(stderr, "sound_bench: IRQ: bench %c %" PRIx64 " %02x, reference ", kind, cyc, st);
            if (ii < irqs.size()) fprintf(stderr, "%c %" PRIx64 " %02x\n", irqs[ii].kind, irqs[ii].cyc, irqs[ii].st);
            else fprintf(stderr, "(none)\n");
            fail("IRQ differs");
        }
        ii++;
    };

    while (sample < samples && !errors) {
        tick();
        if (m->dbg_slot_done) {
            uint64_t n = clk_count - slot_start;
            if (n > slot_clk_max) slot_clk_max = n;
            slot_clk_sum += n;
            slots++;
            if (m->extint && !prev_ext) check_irq('S', slot_start_cyc, m->bus_rdata);
            prev_ext = m->extint;
            if (fslot) {
                for (int k = 0; k < 2 && !errors; k++) {
                    Slot s;
                    if (!fgets(line, sizeof line, fslot)) { fslot = nullptr; break; }
                    sscanf(line, "%c %" SCNx64 " %u %lld %lld %lld %lld %lld %lld", &s.k, &s.cyc, &s.i,
                           &s.v[0], &s.v[1], &s.v[2], &s.v[3], &s.v[4], &s.v[5]);
                    long long h[6];
                    if (s.k == 'G') {
                        h[0] = (int32_t)m->dbg_oo; h[1] = (int32_t)m->dbg_prev; h[2] = m->dbg_w46;
                        h[3] = m->dbg_ram2_0; h[4] = m->dbg_ram2_1; h[5] = m->dbg_ram2_2;
                    } else {
                        h[0] = (int32_t)m->dbg_sacc2; h[1] = (int32_t)m->dbg_sacc3; h[2] = (int32_t)m->dbg_sacc6;
                        h[3] = (int32_t)m->dbg_sacc7; h[4] = ((long long)m->dbg_w186 << 16) | m->dbg_w187;
                        h[5] = m->dbg_w188;
                    }
                    if (memcmp(h, s.v, sizeof h) || s.i != m->slot_idx) {
                        fprintf(stderr, "slot %c i=%u: ref", s.k, s.i);
                        for (long long x : s.v) fprintf(stderr, " %lld", x);
                        fprintf(stderr, "\n          hdl");
                        for (long long x : h) fprintf(stderr, " %lld", x);
                        fprintf(stderr, " (i=%u)\n", m->slot_idx);
                        fail("slot state differs");
                    }
                }
            }
        }
        if (m->cpu_turn) {
            uint64_t L = m->la32_cycle;
            slot_start_cyc = L;
            // writes made in this turn
            while (li < lw.size() && lw[li].cyc <= L) {
                if (lw[li].cyc < L) { fail("LA32 write left over from an earlier turn"); break; }
                m->bus_we = 1; m->bus_addr = lw[li].a; m->bus_wdata = lw[li].d;
                tick();
                m->bus_we = 0; m->bus_addr = 0;
                m->eval();
                if (prev_ext && !m->extint) check_irq('C', L, m->bus_rdata);
                prev_ext = m->extint;
                li++;
            }
            while (ri < rw.size() && rw[ri].cyc <= L) {
                if (rw[ri].cyc < L) { fail("reverb write left over from an earlier turn"); break; }
                m->rv_we = 1; m->rv_sel = rw[ri].a;
                m->rv_data = rw[ri].a == 2 ? (rw[ri].d << 1) : (rw[ri].d >> 1);
                tick();
                m->rv_we = 0;
                ri++;
            }
            if (m->slot_idx == 31) m->pwm = 255 - outs[sample].v[6];
            m->cpu_done = 1;
            tick();
            m->cpu_done = 0;
            slot_start = clk_count;
        }
        if (m->sample_valid) {
            const Out& o = outs[sample];
            int h[9] = { m->dbg_acc2, m->dbg_acc3, m->dbg_acc6, m->dbg_acc7, m->dbg_rv0, m->dbg_rv1,
                         m->dbg_vol, m->out_l, m->out_r };
            for (int k = 0; k < 9; k++) h[k] = k == 6 ? h[k] : (int16_t)h[k];
            if (memcmp(h, o.v, sizeof h)) {
                fprintf(stderr, "sample %" PRIu64 ": ref", sample);
                for (int x : o.v) fprintf(stderr, " %d", x);
                fprintf(stderr, "\n            hdl");
                for (int x : h) fprintf(stderr, " %d", x);
                fprintf(stderr, "\n");
                fail("sample differs");
            }
            sample++;
        }
    }
    bool irq_left = !errors && ii < irqs.size() && irqs[ii].cyc < (uint64_t)m->la32_cycle;
    if (irq_left) fail("reference has more IRQ events");
    if (!quiet || errors)
        printf("%s: %" PRIu64 " samples, %zu IRQ events, %" PRIu64 " slots, clocks/slot max %" PRIu64
               " mean %.2f (CPU turn excluded), PCM reads/sample %.2f, %" PRIu64 " clocks\n",
               errors ? "DIFFERENT" : "identical", sample, ii, slots, slot_clk_max,
               slots ? (double)slot_clk_sum / slots : 0.0, sample ? (double)pcm_reads / sample : 0.0, clk_count);
    delete m;
    return errors ? 1 : 0;
}
