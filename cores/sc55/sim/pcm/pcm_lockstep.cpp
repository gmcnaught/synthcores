// PCM lockstep (M3): hdl/pcm/sc55_pcm.sv against the reference model's PCM.
//
// The full model runs (firmware, MIDI). Each step's PCM register accesses are
// replayed into the HDL PCM in order (reads must return the model's data),
// then the HDL runs PCM_Update up to the model's cycle count. Every output
// sample must match; after each step the IRQ line, the latches, accumulators
// and all of ram1/ram2 must match, and eram every 1000 steps.
//
// Usage: pcm_lockstep --roms DIR [--steps N] [--midi-demo SEED] [--rom-latency N]
#include "Vsc55_pcm.h"
#include "Vsc55_pcm___024root.h"
#include "verilated.h"

#include "../common/midi_demo.h"
#include "emu.h"
#include "mcu.h"
#include "pcm.h"
#include "ref_hooks.h"
#include "rom_loader.h"

#include <cinttypes>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <string>
#include <vector>

struct Access
{
    bool     write;
    uint8_t  addr;
    uint8_t  data;
};
static std::vector<Access> g_pcm;
static bool                g_recording = false;
static void bus_hook(void*, bool write, uint32_t addr, uint8_t data)
{
    addr &= 0xffffff;
    if (g_recording && addr >= 0xe000 && addr < 0xe040)
        g_pcm.push_back({write, (uint8_t)(addr & 0x3f), data});
}
static void cpu_done_hook(void*) { g_recording = false; }

struct Frame { int32_t l, r; };
static std::vector<Frame> g_samples;
static void sample_cb(void*, const AudioFrame<int32_t>& f) { g_samples.push_back({f.left, f.right}); }

int main(int argc, char** argv)
{
    std::string rom_dir;
    uint64_t    steps = 2'000'000, midi_start = 4'000'000;
    int64_t     midi_seed = -1, loud_seed = -1;
    int         rom_latency = 3;
    for (int i = 1; i < argc; ++i)
    {
        std::string a = argv[i];
        if (a == "--roms" && i + 1 < argc) rom_dir = argv[++i];
        else if (a == "--steps" && i + 1 < argc) steps = std::strtoull(argv[++i], nullptr, 0);
        else if (a == "--midi-demo" && i + 1 < argc) midi_seed = std::strtoll(argv[++i], nullptr, 0);
        else if (a == "--midi-loud" && i + 1 < argc) loud_seed = std::strtoll(argv[++i], nullptr, 0);
        else if (a == "--midi-start" && i + 1 < argc) midi_start = std::strtoull(argv[++i], nullptr, 0);
        else if (a == "--rom-latency" && i + 1 < argc) rom_latency = std::atoi(argv[++i]);
        else { fprintf(stderr, "unknown argument %s\n", a.c_str()); return 2; }
    }

    common::LoadRomsetResult load;
    common::RomOverrides     ov{};
    if (auto err = common::LoadRomset(rom_dir, "mk1-v1.21", common::RomLoader::Hashing, ov, load); err != common::LoadRomsetError{})
    {
        fprintf(stderr, "romset load failed: %s\n", common::ToCString(err));
        return 2;
    }
    Emulator emu;
    emu.Init({.lcd_backend = nullptr});
    emu.LoadRoms(load.romset, load.romset_info);
    emu.Reset();
    emu.SetSampleCallback(sample_cb, nullptr);
    mcu_t& m = emu.GetMCU();
    pcm_t& p = emu.GetPCM();
    g_ref_hooks.bus = bus_hook;
    g_ref_hooks.cpu_done = cpu_done_hook;

    VerilatedContext ctx;
    ctx.commandArgs(argc, argv);
    Vsc55_pcm* t = new Vsc55_pcm{&ctx};
    auto*      R = t->rootp;
    uint64_t   clocks = 0;
    std::deque<Frame> hdl_samples;
    // wave ROM: a request is taken when rom_req && rom_ready; data returns in
    // order rom_latency clocks later (reads pipeline, like the DDR3 stream)
    std::deque<std::pair<uint64_t, uint32_t>> romq;
    auto rom_byte = [&](uint32_t a) -> uint8_t {
        uint32_t bank = a >> 20, off = a & 0xfffff;
        return bank == 0 ? p.waverom1[off] : bank == 1 ? p.waverom2[off] : p.waverom3[off];
    };
    auto tick = [&]() {
        t->rom_ready = 1;
        if (t->rom_req && t->rom_ready) romq.push_back({clocks + (uint64_t)rom_latency, (uint32_t)t->rom_addr});
        t->rom_rvalid = 0;
        if (!romq.empty() && romq.front().first <= clocks)
        {
            t->rom_rvalid = 1;
            t->rom_data = rom_byte(romq.front().second);
            romq.pop_front();
        }
        t->clk = 0; t->eval(); t->clk = 1; t->eval();
        ++clocks;
        if (t->sample_valid) hdl_samples.push_back({(int32_t)t->sample_l, (int32_t)t->sample_r});
    };
    t->rst = 1; t->cpu_req = 0; t->run = 0; t->enable_oversampling = p.enable_oversampling; t->rom_rvalid = 0; t->rom_ready = 1;
    for (int i = 0; i < 4; ++i) tick();
    t->rst = 0;
    for (int g = 0; g < 20000 && !t->ready; ++g) tick();   // power-on eram clear

    std::vector<uint8_t> midi;
    if (midi_seed >= 0) midi = make_midi_demo((uint64_t)midi_seed);
    if (loud_seed >= 0) midi = make_midi_loud((uint64_t)loud_seed);
    size_t midi_pos = 0;

    std::deque<std::string> hist;
    auto fail = [&](const std::string& what) {
        fprintf(stderr, "MISMATCH: %s\nlast steps:\n", what.c_str());
        for (auto& h : hist) fprintf(stderr, "  %s\n", h.c_str());
        return 1;
    };

    uint64_t n_acc = 0, n_samples = 0, n_frames = 0, max_frame_clocks = 0, nonzero = 0;
    int64_t  peak = 0;
    for (uint64_t s = 0; s < steps; ++s)
    {
        if (!midi.empty() && s >= midi_start && (s % 600) == 0 && midi_pos < midi.size())
            emu.PostMIDI(midi[midi_pos++]);

        g_pcm.clear();
        g_samples.clear();
        uint64_t frames_before = p.cycles;
        g_recording = true;
        emu.Step();
        g_recording = false;
        if (p.config.reg_slots > 28) return fail("reg_slots > 28 is outside the HDL's assumption");

        // replay register accesses
        for (size_t k = 0; k < g_pcm.size(); ++k)
        {
            const Access& a = g_pcm[k];
            t->cpu_req = 1; t->cpu_we = a.write; t->cpu_addr = a.addr; t->cpu_wdata = a.data;
            int g = 0;
            do { tick(); } while (!t->cpu_ack && ++g < 1000);
            t->cpu_req = 0;
            if (!t->cpu_ack) return fail("step " + std::to_string(s) + ": PCM access not acknowledged");
            if (!a.write && t->cpu_rdata != a.data)
            {
                char b[128];
                snprintf(b, sizeof b, "step %" PRIu64 ": read %02x HDL %02x model %02x", s, a.addr, t->cpu_rdata, a.data);
                return fail(b);
            }
            tick();
            ++n_acc;
        }
        // PCM_Update(cycles)
        uint64_t c0 = clocks;
        t->run = 1;
        t->target = m.cycles;
        tick();
        t->run = 0;
        for (int g = 0; !t->done && g < 200000; ++g) tick();
        if (!t->done) return fail("step " + std::to_string(s) + ": PCM update did not finish");
        if (p.cycles != frames_before)
        {
            ++n_frames;
            max_frame_clocks = std::max(max_frame_clocks, clocks - c0);
        }

        char line[160];
        snprintf(line, sizeof line, "step %" PRIu64 " pcm_acc=%zu samples=%zu cycles=%" PRIu64, s, g_pcm.size(), g_samples.size(), p.cycles);
        hist.push_back(line);
        if (hist.size() > 12) hist.pop_front();

        // samples
        if (hdl_samples.size() != g_samples.size())
            return fail("step " + std::to_string(s) + ": HDL produced " + std::to_string(hdl_samples.size()) +
                        " samples, model " + std::to_string(g_samples.size()));
        for (size_t i = 0; i < g_samples.size(); ++i)
        {
            if (hdl_samples[i].l != g_samples[i].l || hdl_samples[i].r != g_samples[i].r)
            {
                char b[160];
                snprintf(b, sizeof b, "step %" PRIu64 " sample %zu: HDL %d,%d model %d,%d", s, i, hdl_samples[i].l,
                         hdl_samples[i].r, g_samples[i].l, g_samples[i].r);
                return fail(b);
            }
        }
        n_samples += g_samples.size();
        for (auto& f : g_samples)
        {
            peak = std::max<int64_t>(peak, std::max(std::abs((int64_t)f.l), std::abs((int64_t)f.r)));
            nonzero += (f.l != 0 || f.r != 0);
        }
        hdl_samples.clear();

        // state
        std::string diff;
        auto cmp = [&](const std::string& name, uint64_t h, uint64_t r) {
            if (h != r && diff.size() < 600) { char b[96]; snprintf(b, sizeof b, "%s hdl=%" PRIx64 " ref=%" PRIx64 "; ", name.c_str(), h, r); diff += b; }
        };
        cmp("irq", t->irq0, p.irq_assert);
        cmp("cycles", R->sc55_pcm__DOT__pcm_cycles, p.cycles);
        cmp("tv_counter", R->sc55_pcm__DOT__tv_counter, p.tv_counter);
        cmp("voice_mask", R->sc55_pcm__DOT__voice_mask, p.voice_mask);
        cmp("voice_mask_pending", R->sc55_pcm__DOT__voice_mask_pending, p.voice_mask_pending);
        cmp("write_latch", R->sc55_pcm__DOT__write_latch, p.write_latch);
        cmp("read_latch", R->sc55_pcm__DOT__read_latch, p.read_latch);
        cmp("wave_byte_latch", R->sc55_pcm__DOT__wave_byte_latch, p.wave_byte_latch);
        cmp("irq_channel", R->sc55_pcm__DOT__irq_channel, p.irq_channel);
        cmp("nfs", R->sc55_pcm__DOT__nfs, p.nfs);
        cmp("accum_l", (uint32_t)R->sc55_pcm__DOT__accum_l, (uint32_t)p.accum_l);
        cmp("accum_r", (uint32_t)R->sc55_pcm__DOT__accum_r, (uint32_t)p.accum_r);
        cmp("rcsum0", (uint32_t)R->sc55_pcm__DOT__rcsum[0], (uint32_t)p.rcsum[0]);
        cmp("rcsum1", (uint32_t)R->sc55_pcm__DOT__rcsum[1], (uint32_t)p.rcsum[1]);
        cmp("orval", R->sc55_pcm__DOT__cfg_orval, p.config.orval);
        cmp("noise_mask", R->sc55_pcm__DOT__cfg_noise_mask, p.config.noise_mask);
        cmp("write_mask", R->sc55_pcm__DOT__cfg_write_mask, p.config.write_mask);
        cmp("reg_slots", R->sc55_pcm__DOT__reg_slots, p.config.reg_slots);
        for (int r = 0; r < 32; ++r)
        {
            for (int i = 0; i < 8; ++i)
                if (R->sc55_pcm__DOT__ram1[r][i] != p.ram1[r][i])
                    cmp("ram1[" + std::to_string(r) + "][" + std::to_string(i) + "]", R->sc55_pcm__DOT__ram1[r][i], p.ram1[r][i]);
            for (int i = 0; i < 16; ++i)
                if (R->sc55_pcm__DOT__ram2[r][i] != p.ram2[r][i])
                    cmp("ram2[" + std::to_string(r) + "][" + std::to_string(i) + "]", R->sc55_pcm__DOT__ram2[r][i], p.ram2[r][i]);
        }
        if ((s % 1000) == 0 || s + 1 == steps)
            for (int i = 0; i < 0x4000; ++i)
                if (R->sc55_pcm__DOT__u_eram__DOT__mem[i] != p.eram[i])
                    cmp("eram[" + std::to_string(i) + "]", R->sc55_pcm__DOT__u_eram__DOT__mem[i], p.eram[i]);
        if (!diff.empty())
            return fail("step " + std::to_string(s) + ": " + diff);
    }
    printf("OK: %" PRIu64 " steps, %" PRIu64 " frames, %" PRIu64 " samples (%" PRIu64 " non-zero, peak %.1f dBFS), %" PRIu64
           " PCM register accesses, MIDI bytes %zu, max %" PRIu64 " clocks per update with ROM latency %d\n",
           steps, n_frames, n_samples, nonzero, peak > 0 ? 20.0 * std::log10((double)peak / 2147483648.0) : -999.0, n_acc,
           midi_pos, max_frame_clocks, rom_latency);
    delete t;
    return 0;
}
