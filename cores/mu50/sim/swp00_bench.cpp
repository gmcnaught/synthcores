// SPDX-License-Identifier: MIT
// SWP00 HDL bench (milestone U3): the HDL (hdl/swp00) and the C++ copy of MAME's model
// (ref/swp00_ref) run the same SWP00 accesses side by side, sample by sample.
//   swp00_bench WAVE_ROM SND_LOG NSAMPLES [options]
//     --rlog FILE     also replay the CPU's reads (MU_SND_RLOG) and check their values
//     --acc           SND_LOG is a compact access list (tools/swp00_acc.py, gunzipped)
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

// a compact access list (tools/swp00_acc.py, gunzipped): "<dn> <addr> <data>" or "<dn> r<addr>"
static std::vector<Cmd> load_acc(const char *fn) {
    FILE *f = fopen(fn, "r");
    if (!f) { perror(fn); exit(1); }
    std::vector<Cmd> c;
    char line[128];
    uint64_t n = 0;
    while (fgets(line, sizeof line, f)) {
        unsigned long long dn;
        char a[32];
        unsigned d = 0;
        int k = sscanf(line, "%llu %31s %x", &dn, a, &d);
        if (k < 2) continue;
        n += dn;
        if (a[0] == 'r') c.push_back({2, n, unsigned(strtoul(a + 1, nullptr, 16)), 0});
        else c.push_back({1, n, unsigned(strtoul(a, nullptr, 16)), d});
    }
    fclose(f);
    return c;
}

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

// --vcheck: the HDL's channel records (swp00_pkg::voice_t, first field most significant) against
// the reference's blocks after every sample
#include "Vswp00___024root.h"
struct Field { const char *name; int width; };
static const Field VFIELDS[] = {
    {"phase", 16}, {"start", 16}, {"loop", 16}, {"pitch", 16}, {"address", 24}, {"format", 8},
    {"pos", 32}, {"pos_dec", 32}, {"dpcm_pos", 32}, {"dpcm_delta", 32}, {"dpcm_s0", 16}, {"dpcm_s1", 16},
    {"last", 16}, {"first", 1}, {"done", 1},
    {"att_speed", 8}, {"att_level", 8}, {"dec_speed", 8}, {"dec_level", 8}, {"env_level", 16}, {"env_mode", 2},
    {"f_q", 16}, {"f_b", 32}, {"f_l", 32}, {"f_k", 16}, {"f_kt", 16}, {"f_info", 16}, {"f_speed", 8}, {"f_sweep", 2},
    {"lfo_counter", 22}, {"lfo_speed", 8}, {"lfo_lamod", 8}, {"lfo_fmod", 8},
    {"cglo", 12}, {"cpanl", 12}, {"cpanr", 12}, {"tglo", 12}, {"tpanl", 12}, {"tpanr", 12},
    {"glo", 8}, {"pan", 8}, {"dry", 8}, {"rev", 8}, {"cho", 8}, {"var", 8},
};
static uint64_t ref_field(swp00ref::swp00_ref *r, int ch, const char *n) {
    auto &s = r->m_streaming[ch]; auto &e = r->m_envelope[ch]; auto &f = r->m_filter[ch];
    auto &l = r->m_lfo[ch]; auto &m = r->m_mixer[ch];
    std::string k = n;
    if (k == "phase") return s.m_phase; if (k == "start") return s.m_start; if (k == "loop") return s.m_loop;
    if (k == "pitch") return s.m_pitch; if (k == "address") return s.m_address; if (k == "format") return s.m_format;
    if (k == "pos") return uint32_t(s.m_pos); if (k == "pos_dec") return uint32_t(s.m_pos_dec);
    if (k == "dpcm_pos") return s.m_dpcm_pos; if (k == "dpcm_delta") return uint32_t(s.m_dpcm_delta);
    if (k == "dpcm_s0") return uint16_t(s.m_dpcm_s0); if (k == "dpcm_s1") return uint16_t(s.m_dpcm_s1);
    if (k == "last") return uint16_t(s.m_last); if (k == "first") return s.m_first; if (k == "done") return s.m_done;
    if (k == "att_speed") return e.m_attack_speed; if (k == "att_level") return e.m_attack_level;
    if (k == "dec_speed") return e.m_decay_speed; if (k == "dec_level") return e.m_decay_level;
    if (k == "env_level") return uint16_t(e.m_envelope_level); if (k == "env_mode") return e.m_envelope_mode;
    if (k == "f_q") return uint16_t(f.m_q); if (k == "f_b") return uint32_t(f.m_b); if (k == "f_l") return uint32_t(f.m_l);
    if (k == "f_k") return f.m_k; if (k == "f_kt") return f.m_k_target; if (k == "f_info") return f.m_info;
    if (k == "f_speed") return f.m_speed; if (k == "f_sweep") return f.m_sweep;
    if (k == "lfo_counter") return l.m_counter; if (k == "lfo_speed") return l.m_speed;
    if (k == "lfo_lamod") return l.m_lamod; if (k == "lfo_fmod") return l.m_fmod;
    if (k == "cglo") return m.m_cglo; if (k == "cpanl") return m.m_cpanl; if (k == "cpanr") return m.m_cpanr;
    if (k == "tglo") return m.m_tglo; if (k == "tpanl") return m.m_tpanl; if (k == "tpanr") return m.m_tpanr;
    if (k == "glo") return m.m_glo; if (k == "pan") return m.m_pan; if (k == "dry") return m.m_dry;
    if (k == "rev") return m.m_rev; if (k == "cho") return m.m_cho; if (k == "var") return m.m_var;
    return 0;
}
template<typename W> static uint64_t bits(const W &w, int lsb, int width) {
    uint64_t v = 0;
    for (int i = 0; i < width; i++) v |= uint64_t((w[(lsb + i) / 32] >> ((lsb + i) % 32)) & 1) << i;
    return v;
}
template<typename H> static uint64_t hfield(H *h, int ch, const char *name) {
    int total = 0;
    for (auto &f : VFIELDS) total += f.width;
    for (auto &f : VFIELDS) {
        total -= f.width;
        if (!strcmp(f.name, name)) return bits(h->rootp->swp00__DOT__vram[ch], total, f.width);
    }
    return 0;
}
// --vcheck: the MEG state registers (swp00_meg_regs.inc) against the reference's members
template<typename H> static bool mcheck(H *h, swp00ref::swp00_ref *r, uint64_t smp) {
    bool ok = true;
#define R(name, idx)                                                                                    \
    if (uint32_t(h->rootp->swp00__DOT__meg__DOT__rfa[idx]) != uint32_t(r->name)) {                       \
        fprintf(stderr, "after sample %llu: MEG %s HDL %08x, model %08x\n", (unsigned long long)smp, #name,  \
                uint32_t(h->rootp->swp00__DOT__meg__DOT__rfa[idx]), uint32_t(r->name));                  \
        ok = false;                                                                                     \
    }
#include "swp00_meg_regs.inc"
#undef R
    return ok;
}
// the delay lines (bench memory) against the reference's buffers
static bool dcheck(const std::vector<uint32_t> &dm, swp00ref::swp00_ref *r, uint64_t smp) {
    int bad = 0;
    for (uint32_t i = 0; i < 0x20000; i++) {
        if (dm[i] != uint32_t(r->m_rev_buffer[i]) && bad++ < 5)
            fprintf(stderr, "after sample %llu: rev[%05x] HDL %08x, model %08x\n", (unsigned long long)smp, i, dm[i], r->m_rev_buffer[i]);
        if (i < 0x8000 && dm[0x20000 + i] != uint32_t(r->m_cho_buffer[i]) && bad++ < 5)
            fprintf(stderr, "after sample %llu: cho[%04x] HDL %08x, model %08x\n", (unsigned long long)smp, i, dm[0x20000 + i], r->m_cho_buffer[i]);
        if (dm[0x28000 + i] != uint32_t(r->m_var_buffer[i]) && bad++ < 5)
            fprintf(stderr, "after sample %llu: var[%05x] HDL %08x, model %08x\n", (unsigned long long)smp, i, dm[0x28000 + i], r->m_var_buffer[i]);
    }
    return bad == 0;
}
// returns false and prints the differences if a channel record differs
template<typename H> static bool vcheck(H *h, swp00ref::swp00_ref *r, uint64_t smp) {
    int total = 0;
    for (auto &f : VFIELDS) total += f.width;
    bool ok = true;
    for (int ch = 0; ch < 32; ch++) {
        auto &w = h->rootp->swp00__DOT__vram[ch];
        int top = total;
        for (auto &f : VFIELDS) {
            top -= f.width;
            uint64_t hv = bits(w, top, f.width), rv = ref_field(r, ch, f.name) & ((1ull << f.width) - 1);
            if (hv != rv) {
                fprintf(stderr, "after sample %llu: channel %d %s HDL %llx, model %llx\n", (unsigned long long)smp, ch,
                        f.name, (unsigned long long)hv, (unsigned long long)rv);
                if (ok) {
                    int t2 = total;
                    fprintf(stderr, "  channel %d (model):", ch);
                    for (auto &g : VFIELDS) {
                        t2 -= g.width;
                        fprintf(stderr, " %s=%llx", g.name, (unsigned long long)(ref_field(r, ch, g.name) & ((1ull << g.width) - 1)));
                    }
                    fprintf(stderr, "\n");
                }
                ok = false;
            }
        }
    }
    return ok;
}

int main(int argc, char **argv) {
    if (argc < 4) { fprintf(stderr, "usage: %s WAVE_ROM SND_LOG NSAMPLES [options]\n", argv[0]); return 2; }
    const char *rlog = nullptr, *rawfn = nullptr;
    bool acc = false;
    bool mix_only = false, quiet = false, vchk = false;
    int wlat = 2, dlat = 2;
    for (int i = 4; i < argc; i++) {
        std::string a = argv[i];
        if (a == "--rlog" && i + 1 < argc) rlog = argv[++i];
        else if (a == "--raw" && i + 1 < argc) rawfn = argv[++i];
        else if (a == "--mix-only") mix_only = true;
        else if (a == "--wlat" && i + 1 < argc) wlat = atoi(argv[++i]);
        else if (a == "--dlat" && i + 1 < argc) dlat = atoi(argv[++i]);
        else if (a == "--quiet") quiet = true;
        else if (a == "--vcheck") vchk = true;
        else if (a == "--acc") acc = true;
    }
    std::vector<uint8_t> rom(0x400000, 0);
    FILE *f = fopen(argv[1], "rb");
    if (!f) { perror(argv[1]); return 1; }
    size_t got = fread(rom.data(), 1, rom.size(), f);
    fclose(f);
    if (got != rom.size()) { fprintf(stderr, "%s: want 4 MB\n", argv[1]); return 1; }
    const uint64_t total = strtoull(argv[3], nullptr, 0);
    std::vector<Cmd> cmds = acc ? load_acc(argv[2]) : load_cmds(argv[2], rlog);
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
    auto *ref2 = new swp00ref::swp00_ref(rom.data());
    size_t r2ci = 0;
    bool fail_mix = false;
    auto mismatch = [&](const char *what) {
        if (vchk) fail_mix = true; else fail = true;
        if (!quiet) fprintf(stderr, "%s\n", what);
    };
    while (!fail && (ci < cmds.size()) && clk < total * 20000ull + 2000000ull) {
        if (fail_mix && h->smp_valid) fail = true;
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
            if (vchk) {   // the channel records after this sample's channels
                // second reference, in lockstep: the commands up to this sample, then the sample
                while (r2ci < cmds.size() && cmds[r2ci].n <= nmix) {
                    while (ref2->sample_counter() < cmds[r2ci].n) { int32_t o[2]; ref2->generate(1, o); }
                    if (cmds[r2ci].kind == 1) ref2->write(uint16_t(cmds[r2ci].reg), uint8_t(cmds[r2ci].data));
                    else if (cmds[r2ci].kind == 2) ref2->read(uint16_t(cmds[r2ci].reg));
                    r2ci++;
                }
                while (ref2->sample_counter() < nmix + 1) { int32_t o[2]; ref2->generate(1, o); }
                if (const char *vd = getenv("VDUMP")) {      // one channel's position, each sample
                    int ch = atoi(vd);
                    auto &s = ref2->m_streaming[ch];
                    fprintf(stderr, "sample %llu ch %d model pos %d pos_dec %x start %x fmt %x env %d/%x | HDL pos %d pos_dec %x env %d/%x\n",
                            (unsigned long long)nmix, ch, s.m_pos, s.m_pos_dec, s.m_start, s.m_format,
                            ref2->m_envelope[ch].m_envelope_mode, ref2->m_envelope[ch].m_envelope_level,
                            int32_t(hfield(h, ch, "pos")), unsigned(hfield(h, ch, "pos_dec")),
                            int(hfield(h, ch, "env_mode")), unsigned(hfield(h, ch, "env_level")));
                }
                if (!vcheck(h, ref2, nmix)) fail = true;
            }
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
        if (h->smp_valid && vchk && !mcheck(h, ref2, nsmp)) {   // the MEG state after this sample
            fail = true;
            dcheck(dmem, ref2, nsmp);
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
