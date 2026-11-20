// System lockstep (M4): hdl/sc55_core.sv (MCU system + PCM) against the whole
// reference model. Both get the same MIDI bytes at the same steps; every
// output sample and the CPU's PC/SR after every step must match.
//
// --smf FILE reproduces jcmoyer's renderer (nuked-sc55-render) for one
// instance: GM reset, 24 M steps, then the file's events at 600 ns per step
// (mk1), end = cut. --out FILE writes the HDL's samples as the renderer's
// s16 stream, so its SHA-256 can be compared with `sc55-render --stdout`.
//
// Usage: system_lockstep --roms DIR (--smf FILE | --midi-demo S | --midi-loud S --steps N)
//                        [--out FILE] [--reset-steps N] [--rom-latency N]
#include "Vsc55_core.h"
#include "Vsc55_core___024root.h"
#include "verilated.h"

#include "../common/midi_demo.h"
#include "emu.h"
#include "mcu.h"
#include "pcm.h"
#include "rom_loader.h"
#include "smf.h"

#include <algorithm>
#include <cinttypes>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <fstream>
#include <random>
#include <string>
#include <vector>

struct Frame { int32_t l, r; };
static std::vector<Frame> g_model;
static void sample_cb(void*, const AudioFrame<int32_t>& f) { g_model.push_back({f.left, f.right}); }

int main(int argc, char** argv)
{
    std::string rom_dir, smf_path, out_path;
    uint64_t    steps = 0, reset_steps = 24'000'000;
    int64_t     midi_seed = -1, loud_seed = -1;
    int         rom_latency = 3;
    bool        rom_backpressure = false;
    std::mt19937_64 bp_rng(7);
    for (int i = 1; i < argc; ++i)
    {
        std::string a = argv[i];
        if (a == "--roms" && i + 1 < argc) rom_dir = argv[++i];
        else if (a == "--smf" && i + 1 < argc) smf_path = argv[++i];
        else if (a == "--out" && i + 1 < argc) out_path = argv[++i];
        else if (a == "--steps" && i + 1 < argc) steps = std::strtoull(argv[++i], nullptr, 0);
        else if (a == "--reset-steps" && i + 1 < argc) reset_steps = std::strtoull(argv[++i], nullptr, 0);
        else if (a == "--midi-demo" && i + 1 < argc) midi_seed = std::strtoll(argv[++i], nullptr, 0);
        else if (a == "--midi-loud" && i + 1 < argc) loud_seed = std::strtoll(argv[++i], nullptr, 0);
        else if (a == "--rom-latency" && i + 1 < argc) rom_latency = std::atoi(argv[++i]);
        else if (a == "--rom-backpressure") rom_backpressure = true;
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
    emu.GetPCM().enable_oversampling = true;
    emu.SetSampleCallback(sample_cb, nullptr);
    mcu_t& m = emu.GetMCU();
    pcm_t& p = emu.GetPCM();

    VerilatedContext ctx;
    ctx.commandArgs(argc, argv);
    Vsc55_core* t = new Vsc55_core{&ctx};
    uint64_t    clocks = 0;
    std::vector<Frame> hdl;
    std::deque<uint8_t> midi_q;
    std::deque<std::pair<uint64_t, uint32_t>> romq;
    auto rom_byte = [&](uint32_t a) -> uint8_t {
        uint32_t bank = a >> 20, off = a & 0xfffff;
        return bank == 0 ? p.waverom1[off] : bank == 1 ? p.waverom2[off] : p.waverom3[off];
    };
    auto tick = [&]() {
        t->rom_ready = rom_backpressure ? (bp_rng() % 3) != 0 : 1;
        if (t->rom_req && t->rom_ready) romq.push_back({clocks + (uint64_t)rom_latency, (uint32_t)t->rom_addr});
        t->rom_rvalid = 0;
        if (!romq.empty() && romq.front().first <= clocks)
        {
            t->rom_rvalid = 1;
            t->rom_data = rom_byte(romq.front().second);
            romq.pop_front();
        }
        t->midi_valid = 0;
        if (!midi_q.empty() && !t->midi_full)
        {
            t->midi_valid = 1;
            t->midi_data = midi_q.front();
            midi_q.pop_front();
        }
        t->clk = 0; t->eval(); t->clk = 1; t->eval();
        ++clocks;
        if (t->sample_valid) hdl.push_back({(int32_t)t->sample_l, (int32_t)t->sample_r});
    };

    t->rst = 1; t->run_step = 0; t->reset_start = 0; t->ld_we = 0; t->midi_valid = 0; t->buttons = 0;
    t->rom_rvalid = 0; t->rom_ready = 1; t->enable_oversampling = 1;
    for (int i = 0; i < 4; ++i) tick();
    t->rst = 0;
    for (int g = 0; g < 20000 && !t->ready; ++g) tick();
    auto load_mem = [&](int sel, const uint8_t* src, size_t n) {
        t->ld_we = 1; t->ld_sel = sel;
        for (size_t i = 0; i < n; ++i) { t->ld_addr = (uint32_t)i; t->ld_data = src[i]; tick(); }
        t->ld_we = 0;
    };
    load_mem(0, m.rom1, sizeof m.rom1);
    load_mem(1, m.rom2, 0x40000);
    load_mem(2, m.sram, sizeof m.sram);
    load_mem(3, m.ram, sizeof m.ram);
    t->reset_start = 1; tick(); t->reset_start = 0;
    for (int g = 0; g < 1000 && !t->reset_done; ++g) tick();
    if (!t->reset_done) { fprintf(stderr, "HDL reset did not finish\n"); return 1; }

    auto post = [&](uint8_t b) { emu.PostMIDI(b); midi_q.push_back(b); };

    std::vector<int16_t> out;
    int64_t peak = 0;
    uint64_t step_no = 0, max_step_clocks = 0, total_samples = 0;
    // The HDL's PCM runs alongside its CPU, so its samples come up to a frame
    // after the model's: compare the two streams in order, not per step.
    std::deque<Frame> mq, hq;
    uint64_t m_index = 0, h_index = 0, out_from = ~0ull;

    auto drain = [&]() -> bool {
        for (auto& f : g_model) mq.push_back(f);
        for (auto& f : hdl) hq.push_back(f);
        g_model.clear();
        hdl.clear();
        while (!mq.empty() && !hq.empty())
        {
            Frame a = mq.front(), b = hq.front();
            if (a.l != b.l || a.r != b.r)
            {
                fprintf(stderr, "MISMATCH sample %" PRIu64 " (step %" PRIu64 "): HDL %d,%d model %d,%d\n", h_index, step_no, b.l, b.r, a.l, a.r);
                return false;
            }
            peak = std::max<int64_t>(peak, std::max(std::abs((int64_t)b.l), std::abs((int64_t)b.r)));
            if (h_index >= out_from)
            {
                out.push_back((int16_t)std::clamp<int32_t>(b.l >> 15, INT16_MIN, INT16_MAX));
                out.push_back((int16_t)std::clamp<int32_t>(b.r >> 15, INT16_MIN, INT16_MAX));
            }
            mq.pop_front();
            hq.pop_front();
            ++h_index;
            ++total_samples;
        }
        return true;
    };

    auto step = [&]() -> bool {
        emu.Step();
        m_index += g_model.size();
        // let queued MIDI bytes into the HDL buffer while the CPU is idle
        while (!midi_q.empty() && !t->midi_full) tick();
        uint64_t c0 = clocks;
        t->run_step = 1;
        tick();
        t->run_step = 0;
        for (int g = 0; !t->step_done && g < 50000000; ++g) tick();
        max_step_clocks = std::max(max_step_clocks, clocks - c0);
        if (!t->step_done || t->dbg_pc != m.pc || t->dbg_cp != m.cp || t->dbg_sr != m.sr)
        {
            fprintf(stderr, "MISMATCH at step %" PRIu64 ": HDL pc %02x:%04x sr %04x, model %02x:%04x sr %04x, step_done %d\n",
                    step_no, t->dbg_cp, t->dbg_pc, t->dbg_sr, m.cp, m.pc, m.sr, t->step_done);
            auto* R = t->rootp;
            fprintf(stderr, "  mcu q=%d cpu state=%d int_hold=%d pcm ps=%d irq_known=%d irq0=%d pcm_req=%d cycles=%" PRIu64 " pcm_cycles=%" PRIu64 "\n",
                    (int)R->sc55_core__DOT__mcu__DOT__q, (int)R->sc55_core__DOT__mcu__DOT__cpu__DOT__state,
                    (int)R->sc55_core__DOT__mcu__DOT__int_hold, (int)R->sc55_core__DOT__pcm__DOT__ps,
                    (int)R->sc55_core__DOT__pcm__DOT__irq_known, (int)R->sc55_core__DOT__pcm__DOT__irq0,
                    (int)R->sc55_core__DOT__mcu__DOT__pcm_req, (uint64_t)R->sc55_core__DOT__mcu__DOT__cycles,
                    (uint64_t)R->sc55_core__DOT__pcm__DOT__pcm_cycles);
            return false;
        }
        ++step_no;
        return drain();
    };

    if (!smf_path.empty())
    {
        // R_RunReset(GM): post the reset, 24 M steps, samples not output
        for (uint8_t b : {0xF0, 0x7E, 0x7F, 0x09, 0x01, 0xF7}) post(b);
        for (uint64_t i = 0; i < reset_steps; ++i)
            if (!step()) return 1;
        // R_RenderOne for one instance: output starts with the first sample after the reset
        out_from = m_index;
        SMF_Data  data   = SMF_LoadEvents(smf_path.c_str());
        SMF_Track merged = SMF_MergeTracks(data);
        SMF_SetDeltasFromTimestamps(merged);
        uint64_t division = data.header.division, us_per_qn = 500000, ns_sim = 0;
        const uint64_t ns_per_step = 600;
        for (const SMF_Event& ev : merged.events)
        {
            const uint64_t when = ns_sim + 1000 * SMF_TicksToUS(ev.delta_time, us_per_qn, division);
            while (ns_sim < when)
            {
                if (!step()) return 1;
                ns_sim += ns_per_step;
            }
            if (ev.IsTempo(data.bytes)) us_per_qn = ev.GetTempoUS(data.bytes);
            if (!ev.IsMetaEvent())
            {
                post(ev.status);
                for (uint8_t b : ev.GetData(data.bytes)) post(b);
            }
        }
    }
    else
    {
        std::vector<uint8_t> midi;
        if (midi_seed >= 0) midi = make_midi_demo((uint64_t)midi_seed);
        if (loud_seed >= 0) midi = make_midi_loud((uint64_t)loud_seed);
        out_from = out_path.empty() ? ~0ull : 0;
        size_t pos = 0;
        for (uint64_t s = 0; s < steps; ++s)
        {
            if (!midi.empty() && s >= 4'000'000 && (s % 600) == 0 && pos < midi.size()) post(midi[pos++]);
            if (!step()) return 1;
        }
    }

    // let the HDL finish the frames already due, then everything must have been compared
    for (int g = 0; g < 200000 && h_index + hq.size() + hdl.size() < m_index; ++g) tick();
    if (!drain()) return 1;
    if (h_index != m_index)
    {
        fprintf(stderr, "MISMATCH: HDL produced %" PRIu64 " samples, model %" PRIu64 "\n", h_index, m_index);
        return 1;
    }
    if (!out_path.empty())
    {
        std::ofstream f(out_path, std::ios::binary);
        f.write((const char*)out.data(), (std::streamsize)(out.size() * sizeof(int16_t)));
    }
    printf("OK: %" PRIu64 " steps, %" PRIu64 " samples identical (peak %.1f dBFS), %zu output frames, %" PRIu64
           " HDL clocks (%.1f/step, max %" PRIu64 ")\n",
           step_no, total_samples, peak > 0 ? 20.0 * std::log10((double)peak / 2147483648.0) : -999.0, out.size() / 2, clocks,
           (double)clocks / (double)std::max<uint64_t>(step_no, 1), max_step_clocks);
    delete t;
    return 0;
}
