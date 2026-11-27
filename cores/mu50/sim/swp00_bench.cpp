// SPDX-License-Identifier: MIT
// SWP00 HDL bench (milestone U3): the HDL (hdl/swp00) and the C++ copy of MAME's model
// (ref/swp00_ref) run the same SWP00 accesses side by side, sample by sample.
//   swp00_bench WAVE_ROM SND_LOG NSAMPLES [options]
//     --rlog FILE     also replay the CPU's reads (MU_SND_RLOG) and check their values
//     --raw FILE      write the HDL's samples (MAME's MU_SAMPLES format, int32 L, R)
//     --mix-only      compare the seven voice sums only (no MEG)
//     --wlat N        wave ROM latency in clocks (default 2)
//     --dlat N        delay memory latency in clocks (default 2)
//     --quiet
// Prints "identical N samples" or the first difference; with the clock count per sample.
// WAVE_ROM: 4 MB. SND_LOG / --rlog lines: "<cpu states> <samples before> <byte address> <data>
// <mem_mask>" (MAME's MU_SND_LOG / MU_SND_RLOG); a 16-bit access is two byte accesses, even first.
#include "Vswp00.h"
#include "swp00_ref.h"
#include "verilated.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

struct Cmd { int kind; uint64_t n; unsigned reg, data; };   // kind 0 gen, 1 write, 2 read

static std::vector<Cmd> load_cmds(const char *wlog, const char *rlog) {
    struct Ev { unsigned long long states, before; unsigned adr, data, mask; bool rd; };
    std::vector<Ev> ev;
    for (int k = 0; k < 2; k++) {
        const char *fn = k ? rlog : wlog;
        if (!fn) continue;
        FILE *f = fopen(fn, "r");
        if (!f) { perror(fn); exit(1); }
        char line[256];
        while (fgets(line, sizeof line, f)) {
            Ev e{}; e.rd = k;
            if (sscanf(line, "%llu %llu %x %x %x", &e.states, &e.before, &e.adr, &e.data, &e.mask) == 5)
                ev.push_back(e);
        }
        fclose(f);
    }
    std::stable_sort(ev.begin(), ev.end(), [](const Ev &a, const Ev &b) { return a.states < b.states; });
    std::vector<Cmd> c;
    for (auto &e : ev) {
        if (e.mask & 0xff00) c.push_back({e.rd ? 2 : 1, e.before, (e.adr & 0x7fe), (e.data >> 8) & 0xff});
        if (e.mask & 0x00ff) c.push_back({e.rd ? 2 : 1, e.before, (e.adr & 0x7fe) | 1, e.data & 0xff});
    }
    return c;
}

struct MixCap { std::vector<std::array<int32_t, 7>> v; };
static void mix_hook(void *ctx, const int32_t *m) {
    std::array<int32_t, 7> a;
    for (int i = 0; i < 7; i++) a[i] = m[i];
    static_cast<MixCap *>(ctx)->v.push_back(a);
}

int main(int argc, char **argv) {
    if (argc < 4) { fprintf(stderr, "usage: %s WAVE_ROM SND_LOG NSAMPLES [options]\n", argv[0]); return 2; }
    const char *rlog = nullptr, *rawfn = nullptr;
    bool mix_only = false, quiet = false;
    int wlat = 2, dlat = 2;
    for (int i = 4; i < argc; i++) {
        std::string a = argv[i];
        if (a == "--rlog" && i + 1 < argc) rlog = argv[++i];
        else if (a == "--raw" && i + 1 < argc) rawfn = argv[++i];
        else if (a == "--mix-only") mix_only = true;
        else if (a == "--wlat" && i + 1 < argc) wlat = atoi(argv[++i]);
        else if (a == "--dlat" && i + 1 < argc) dlat = atoi(argv[++i]);
        else if (a == "--quiet") quiet = true;
    }
    std::vector<uint8_t> rom(0x400000, 0);
    FILE *f = fopen(argv[1], "rb");
    if (!f) { perror(argv[1]); return 1; }
    size_t got = fread(rom.data(), 1, rom.size(), f);
    fclose(f);
    if (got != rom.size()) { fprintf(stderr, "%s: want 4 MB\n", argv[1]); return 1; }
    const uint64_t total = strtoull(argv[3], nullptr, 0);
    std::vector<Cmd> cmds = load_cmds(argv[2], rlog);
    cmds.push_back({0, total, 0, 0});
    FILE *raw = rawfn ? fopen(rawfn, "wb") : nullptr;

    // reference: run command by command, one sample at a time
    auto *ref = new swp00ref::swp00_ref(rom.data());
    MixCap cap;
    ref->m_mix_hook = mix_hook; ref->m_mix_ctx = &cap;
    std::vector<int32_t> ref_out;
    std::vector<uint8_t> ref_reads;
    for (auto &c : cmds) {
        while (ref->sample_counter() < c.n) {
            int32_t o[2];
            ref->generate(1, o);
            ref_out.push_back(o[0]); ref_out.push_back(o[1]);
        }
        if (c.kind == 1) ref->write(uint16_t(c.reg), uint8_t(c.data));
        else if (c.kind == 2) ref_reads.push_back(ref->read(uint16_t(c.reg)));
    }

    // HDL
    std::vector<uint32_t> dmem(0x48000, 0);
    Verilated::commandArgs(argc, argv);
    auto *h = new Vswp00;
    h->rst = 1; h->cmd_valid = 0; h->wv_ack = 0; h->dm_ack = 0;
    for (int i = 0; i < 4; i++) { h->clk = 0; h->eval(); h->clk = 1; h->eval(); }
    h->rst = 0;
    size_t ci = 0, nread = 0, bad_reads = 0;
    uint64_t nsmp = 0, nmix = 0, clk = 0, last_smp_clk = 0, max_clks = 0, sum_clks = 0;
    int wcount = -1, dcount = -1;
    bool fail = false;
    auto mismatch = [&](const char *what) { fail = true; if (!quiet) fprintf(stderr, "%s\n", what); };
    while (!fail && (ci < cmds.size()) && clk < total * 20000ull + 2000000ull) {
        h->clk = 0;
        h->eval();
        // command
        h->cmd_valid = ci < cmds.size();
        if (ci < cmds.size()) {
            h->cmd_kind = cmds[ci].kind; h->cmd_n = uint32_t(cmds[ci].n);
            h->cmd_reg = cmds[ci].reg; h->cmd_data = cmds[ci].data;
        }
        // wave ROM: a request seen now is answered wlat clocks later
        h->wv_ack = 0;
        if (wcount > 0 && --wcount == 0) {
            uint32_t a = h->wv_addr, d = 0;
            for (int b = 0; b < 4; b++) d |= uint32_t(rom[(a + b) & 0x3fffff]) << (8 * b);
            h->wv_data = d; h->wv_ack = 1; wcount = -1;
        } else if (wcount < 0 && h->wv_req) wcount = wlat;
        h->dm_ack = 0;
        if (dcount > 0 && --dcount == 0) {
            uint32_t a = h->dm_addr;
            if (h->dm_we) dmem[a] = h->dm_wdata; else h->dm_rdata = dmem[a];
            h->dm_ack = 1; dcount = -1;
        } else if (dcount < 0 && h->dm_req) dcount = dlat;
        h->eval();
        h->clk = 1;
        h->eval();
        clk++;
        if (h->cmd_ready) {
            if (cmds[ci].kind == 2) {
                uint8_t want = ref_reads[nread++];
                if (h->rd_data != want && bad_reads++ < 10)
                    fprintf(stderr, "read %03X after %llu samples: HDL %02X, model %02X\n", cmds[ci].reg,
                            (unsigned long long)cmds[ci].n, h->rd_data, want);
            }
            ci++;
        }
        if (h->mix_valid) {
            if (nmix < cap.v.size()) {
                for (int i = 0; i < 7; i++)
                    if (int32_t(h->mix[i]) != cap.v[nmix][i]) {
                        char m[256];
                        snprintf(m, sizeof m, "sample %llu: mix[%d] HDL %d, model %d", (unsigned long long)nmix, i,
                                 int32_t(h->mix[i]), cap.v[nmix][i]);
                        mismatch(m);
                        break;
                    }
            }
            nmix++;
        }
        if (h->smp_valid) {
            int32_t l = int32_t(h->smp_l << 14) >> 14, r = int32_t(h->smp_r << 14) >> 14;
            if (raw) { int32_t o[2] = {l, r}; fwrite(o, sizeof o, 1, raw); }
            if (!mix_only && 2 * nsmp + 1 < ref_out.size() && (l != ref_out[2 * nsmp] || r != ref_out[2 * nsmp + 1])) {
                char m[256];
                snprintf(m, sizeof m, "sample %llu: HDL %d %d, model %d %d", (unsigned long long)nsmp, l, r,
                         ref_out[2 * nsmp], ref_out[2 * nsmp + 1]);
                mismatch(m);
            }
            uint64_t d = clk - last_smp_clk;
            if (nsmp > 0) { max_clks = std::max(max_clks, d); sum_clks += d; }
            last_smp_clk = clk;
            nsmp++;
        }
    }
    if (raw) fclose(raw);
    printf("%s %llu samples (model %zu), %zu reads (%zu different); clocks a sample: max %llu, mean %.1f\n",
           fail || bad_reads || nsmp != total ? "DIFFERENT" : "identical", (unsigned long long)nsmp, ref_out.size() / 2,
           nread, bad_reads, (unsigned long long)max_clks, nsmp > 1 ? double(sum_clks) / double(nsmp - 1) : 0.0);
    delete h;
    return fail || bad_reads || nsmp != total ? 1 : 0;
}
