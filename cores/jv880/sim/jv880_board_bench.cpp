// SPDX-License-Identifier: GPL-2.0-or-later
// JV-880 board bench (cores/jv880/docs/plan.md, J4): hdl/jv880_board.sv from power-on against
// the reference model (Nuked-SC55, romset jv880-*). The ROM pack (cores/sc55/tools/romset.py,
// with the NVRAM image) arrives over the ioctl handshake, respecting ioctl_wait; the wave ROMs go
// to a DDR3 model (random BUSY, random in-order read latency); the core clears and resets itself;
// then it runs one step at a time (step_gate) against the model with the same MIDI bytes,
// comparing PC after every step and the two sample streams in order, and the LCD's display RAM
// at the end.
//
// Usage: jv880_board_bench --roms DIR --pack FILE [--romset jv880-v1.0.1] [--steps N]
//            [--smf FILE [--reset-steps N] [--out FILE]] [--midi-demo S] [--seed S]
//            [--ddr-busy N] [--ddr-lat MIN MAX]
//   --smf reproduces the renderer's jv880 test (no reset message, reset steps, then the file's
//   events at 600 ns a step); --out writes the HDL's samples after the reset steps as the
//   renderer's s16 stream (its SHA-256 compares with `sc55-render --stdout`).
#include "Vjv880_board.h"
#include "Vjv880_board___024root.h"
#include "verilated.h"

#include "../../sc55/sim/common/midi_demo.h"
#include "emu.h"
#include "lcd.h"
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

// The model keeps LCD state only when a backend is attached.
struct NullLcd : LCD_Backend
{
    bool Start(const lcd_t&) override { return true; }
    void Stop() override {}
    void Render() override {}
};
static std::vector<Frame> g_model;
static void sample_cb(void*, const AudioFrame<int32_t>& f) { g_model.push_back({f.left, f.right}); }

int main(int argc, char** argv)
{
    std::string rom_dir, pack_path, romset = "jv880-v1.0.1", smf_path, out_path;
    uint64_t    steps = 6'000'000, seed = 1, reset_steps = 24'000'000;
    int64_t     midi_seed = -1;
    int         busy_mod = 4, lat_min = 8, lat_max = 24;
    for (int i = 1; i < argc; ++i)
    {
        std::string a = argv[i];
        if (a == "--roms" && i + 1 < argc) rom_dir = argv[++i];
        else if (a == "--romset" && i + 1 < argc) romset = argv[++i];
        else if (a == "--pack" && i + 1 < argc) pack_path = argv[++i];
        else if (a == "--steps" && i + 1 < argc) steps = std::strtoull(argv[++i], nullptr, 0);
        else if (a == "--smf" && i + 1 < argc) smf_path = argv[++i];
        else if (a == "--reset-steps" && i + 1 < argc) reset_steps = std::strtoull(argv[++i], nullptr, 0);
        else if (a == "--out" && i + 1 < argc) out_path = argv[++i];
        else if (a == "--midi-demo" && i + 1 < argc) midi_seed = std::strtoll(argv[++i], nullptr, 0);
        else if (a == "--seed" && i + 1 < argc) seed = std::strtoull(argv[++i], nullptr, 0);
        else if (a == "--ddr-busy" && i + 1 < argc) busy_mod = std::atoi(argv[++i]);
        else if (a == "--ddr-lat" && i + 2 < argc) { lat_min = std::atoi(argv[++i]); lat_max = std::atoi(argv[++i]); }
        else { fprintf(stderr, "unknown argument %s\n", a.c_str()); return 2; }
    }

    // ---------------------------------------------------------------- the pack
    std::ifstream pf(pack_path, std::ios::binary);
    std::vector<uint8_t> pack((std::istreambuf_iterator<char>(pf)), {});
    const size_t ROM1_AT = 0x100, ROM2_AT = 0x8100, WAVE_AT = 0x89100, NV_AT = 0x489100;
    if (pack.size() != 0x491100 || std::string(pack.begin(), pack.begin() + 4) != "SC55" || pack[4] != 2 || pack[5] != 4)
    {
        fprintf(stderr, "pack %s: %zu bytes, not a JV-880 pack (romset.py pack --romset jv880-...)\n", pack_path.c_str(), pack.size());
        return 2;
    }
    const bool pack_nvram = pack[6] & 1;

    // ---------------------------------------------------------------- the model
    common::LoadRomsetResult load;
    common::RomOverrides     ov{};
    if (auto err = common::LoadRomset(rom_dir, romset, common::RomLoader::Hashing, ov, load); err != common::LoadRomsetError{})
    {
        fprintf(stderr, "romset load failed: %s\n", common::ToCString(err));
        return 2;
    }
    Emulator emu;
    NullLcd  null_lcd;
    emu.Init({.lcd_backend = &null_lcd});
    emu.LoadRoms(load.romset, load.romset_info);
    mcu_t& m = emu.GetMCU();
    pcm_t& pm = emu.GetPCM();
    if (!m.is_jv880) { fprintf(stderr, "%s is not a JV-880 romset\n", romset.c_str()); return 2; }
    if (pack_nvram) std::copy(pack.begin() + NV_AT, pack.begin() + NV_AT + NVRAM_SIZE, m.nvram);
    emu.Reset();
    emu.StartLCD();
    pm.enable_oversampling = true;
    emu.SetSampleCallback(sample_cb, nullptr);
    for (uint32_t i = 0; i < 0x400000; ++i)
    {
        uint8_t want = i < 0x200000 ? pm.waverom1[i] : pm.waverom2[i - 0x200000];
        if (pack[WAVE_AT + i] != want) { fprintf(stderr, "pack wave byte %06x: %02x, model %02x\n", i, pack[WAVE_AT + i], want); return 2; }
    }
    for (uint32_t i = 0; i < 0x8000; ++i) if (pack[ROM1_AT + i] != m.rom1[i]) { fprintf(stderr, "pack ROM1 differs at %x\n", i); return 2; }
    for (uint32_t i = 0; i <= m.rom2_mask; ++i) if (pack[ROM2_AT + i] != m.rom2[i]) { fprintf(stderr, "pack ROM2 differs at %x\n", i); return 2; }

    // ---------------------------------------------------------------- the HDL
    VerilatedContext ctx;
    ctx.commandArgs(argc, argv);
    ctx.randReset(2);                         // random contents in every register and memory
    Vjv880_board* t = new Vjv880_board{&ctx};
    auto* R = t->rootp;
    std::mt19937_64 rng(seed);
    uint64_t clocks = 0, ddr_reads = 0, last_rd_due = 0;

    // DDR3 model: 64-bit words, BUSY on random clocks, reads back in order after lat_min..lat_max clocks
    std::vector<uint64_t> ddr(0x88000, 0);   // 4 MB of wave ROM from WAVE_BASE, then the pack's unused upper ROM2 half (zeros)
    const uint32_t base_w = 0x30000000u >> 3;
    std::deque<std::pair<uint64_t, uint32_t>> rdq;
    std::deque<Frame> hdl;
    auto tick = [&]() {
        t->DDRAM_BUSY = busy_mod > 0 && (rng() % busy_mod) == 0;
        t->DDRAM_DOUT_READY = 0;
        if (!rdq.empty() && rdq.front().first <= clocks)
        {
            t->DDRAM_DOUT_READY = 1;
            t->DDRAM_DOUT = ddr[rdq.front().second];
            rdq.pop_front();
        }
        t->clk = 0; t->clk_vid = 0; t->eval();
        if (!t->DDRAM_BUSY && !t->rst)
        {
            uint32_t w = t->DDRAM_ADDR - base_w;
            if ((t->DDRAM_RD || t->DDRAM_WE) && w >= ddr.size()) { fprintf(stderr, "DDR3 access outside the wave ROM: %x\n", t->DDRAM_ADDR); exit(1); }
            if (t->DDRAM_RD)
            {
                uint64_t due = std::max<uint64_t>(clocks + lat_min + rng() % (lat_max - lat_min + 1), last_rd_due + 1);
                last_rd_due = due;
                rdq.push_back({due, w});
                ++ddr_reads;
            }
            if (t->DDRAM_WE)
                for (int b = 0; b < 8; ++b)
                    if (t->DDRAM_BE & (1 << b))
                        ddr[w] = (ddr[w] & ~(0xffull << (8 * b))) | (((t->DDRAM_DIN >> (8 * b)) & 0xff) << (8 * b));
        }
        t->clk = 1; t->clk_vid = 1; t->eval();
        ++clocks;
        if (t->sample_valid) hdl.push_back({(int32_t)t->sample_l, (int32_t)t->sample_r});
    };

    t->rst = 1; t->user_reset = 0; t->ioctl_download = 0; t->ioctl_wr = 0; t->uart_rxd = 1;
    t->midi_inject = 0; t->buttons = 0; t->enc_we = 0; t->enc_dir = 0; t->pace_enable = 0; t->step_gate = 0; t->gain = 0;
    for (int i = 0; i < 8; ++i) tick();
    t->rst = 0;
    tick();

    // ROM pack download, as hps_io: one ioctl_wr pulse per byte, none while ioctl_wait
    t->ioctl_download = 1;
    tick();
    for (size_t a = 0; a < pack.size(); ++a)
    {
        while (t->ioctl_wait) tick();
        t->ioctl_addr = (uint32_t)a;
        t->ioctl_dout = pack[a];
        t->ioctl_wr = 1;
        tick();
        t->ioctl_wr = 0;
        if (rng() % 3 == 0) tick();
    }
    while (t->ioctl_wait) tick();
    t->ioctl_download = 0;
    const uint64_t load_clocks = clocks;
    for (int g = 0; g < 100000 && !t->running; ++g) tick();
    if (!t->rom_ok || !t->running) { fprintf(stderr, "core did not start: rom_ok %d running %d\n", t->rom_ok, t->running); return 1; }
    {
        size_t bad = 0;
        for (size_t i = 0; i < 0x8000; ++i) bad += R->jv880_board__DOT__core__DOT__mcu__DOT__u_rom1__DOT__mem[i] != m.rom1[i];
        for (size_t i = 0; i < 0x40000; ++i) bad += R->jv880_board__DOT__core__DOT__mcu__DOT__u_rom2__DOT__mem[i] != m.rom2[i];
        for (size_t i = 0; i < NVRAM_SIZE; ++i) bad += R->jv880_board__DOT__core__DOT__mcu__DOT__g_jv880_rams__DOT__u_nvram__DOT__mem[i] != m.nvram[i];
        if (bad) { fprintf(stderr, "after load: %zu bytes of ROM1, ROM2 or NVRAM differ from the model\n", bad); return 1; }
        for (uint32_t w = 0; w < 0x80000; ++w)
        {
            uint64_t want = 0;
            for (int b = 0; b < 8; ++b) want |= (uint64_t)pack[WAVE_AT + (size_t)w * 8 + b] << (8 * b);
            if (ddr[w] != want) { fprintf(stderr, "DDR3 word %x after load: %016" PRIx64 ", pack %016" PRIx64 "\n", w, ddr[w], want); return 1; }
        }
    }

    // ---------------------------------------------------------------- lockstep
    uint64_t compared = 0, step_no = 0, run_clocks = 0, max_step = 0, out_from = ~0ull;
    int64_t  peak = 0;
    std::vector<int16_t> out;
    auto s16 = [](int32_t v) { int32_t s = v >> 15; return (int16_t)std::clamp(s, -32768, 32767); };
    // samples in order: the HDL's PCM runs alongside its CPU, so its samples come up to a frame later
    auto drain = [&]() {
        while (!hdl.empty() && compared < g_model.size())
        {
            const Frame a = hdl.front(), b = g_model[compared];
            if (a.l != b.l || a.r != b.r)
            {
                fprintf(stderr, "sample %" PRIu64 " (step %" PRIu64 "): HDL %d %d, model %d %d\n", compared, step_no, a.l, a.r, b.l, b.r);
                return false;
            }
            if (compared >= out_from) { out.push_back(s16(a.l)); out.push_back(s16(a.r)); }
            peak = std::max<int64_t>(peak, std::max(std::abs((int64_t)a.l), std::abs((int64_t)a.r)));
            hdl.pop_front();
            ++compared;
        }
        return true;
    };
    auto post = [&](uint8_t b) {
        emu.PostMIDI(b);
        t->midi_inject = 1; t->midi_inject_data = b;
        tick();
        t->midi_inject = 0;
    };
    auto step = [&]() {
        emu.Step();
        uint64_t c0 = clocks;
        t->step_gate = 1;
        tick();
        t->step_gate = 0;
        for (int g = 0; g < 50'000'000 && !t->step_done; ++g) tick();
        const bool done = t->step_done;
        max_step = std::max(max_step, clocks - c0);
        run_clocks += clocks - c0;
        ++step_no;
        if (!done || t->dbg_pc != m.pc || t->dbg_cp != m.cp)
        {
            fprintf(stderr, "MISMATCH at step %" PRIu64 ": HDL %02x:%04x (done %d), model %02x:%04x\n", step_no, t->dbg_cp, t->dbg_pc,
                    (int)done, m.cp, m.pc);
            return false;
        }
        return drain();
    };

    size_t midi_bytes = 0;
    if (!smf_path.empty())
    {
        // R_RunReset(NONE) then R_RenderOne: reset steps, then the file's events (600 ns a step)
        for (uint64_t i = 0; i < reset_steps; ++i)
            if (!step()) return 1;
        out_from = g_model.size();
        SMF_Data  data   = SMF_LoadEvents(smf_path.c_str());
        SMF_Track merged = SMF_MergeTracks(data);
        SMF_SetDeltasFromTimestamps(merged);
        uint64_t division = data.header.division, us_per_qn = 500000, ns_sim = 0;
        for (const SMF_Event& ev : merged.events)
        {
            const uint64_t when = ns_sim + 1000 * SMF_TicksToUS(ev.delta_time, us_per_qn, division);
            while (ns_sim < when)
            {
                if (!step()) return 1;
                ns_sim += 600;
            }
            if (ev.IsTempo(data.bytes)) us_per_qn = ev.GetTempoUS(data.bytes);
            if (!ev.IsMetaEvent())
            {
                post(ev.status); ++midi_bytes;
                for (uint8_t b : ev.GetData(data.bytes)) { post(b); ++midi_bytes; }
            }
        }
    }
    else
    {
        std::vector<uint8_t> midi;
        if (midi_seed >= 0) midi = make_midi_demo((uint64_t)midi_seed);
        for (uint64_t s = 0; s < steps; ++s)
        {
            if (!midi.empty() && s >= 4'000'000 && (s % 600) == 0 && midi_bytes < midi.size()) post(midi[midi_bytes++]);
            if (!step()) return 1;
        }
    }
    for (int g = 0; g < 2'000'000 && compared + hdl.size() < g_model.size(); ++g) tick();
    if (!drain()) return 1;
    if (compared + 64 < g_model.size()) { fprintf(stderr, "HDL produced %" PRIu64 " samples, model %zu\n", compared, g_model.size()); return 1; }

    // the LCD's display RAM
    lcd_t& lc = emu.GetLCD();
    for (int i = 0; i < 80; ++i)
        if (R->jv880_board__DOT__lcd_dd[i] != lc.LCD_Data[i]) { fprintf(stderr, "LCD data[%d]: HDL %02x, model %02x\n", i, R->jv880_board__DOT__lcd_dd[i], lc.LCD_Data[i]); return 1; }
    printf("LCD identical: |");
    for (int i = 0; i < 24; ++i) { uint8_t c = lc.LCD_Data[i]; putchar(c >= 32 && c < 127 ? c : '.'); }
    printf("|");
    for (int i = 40; i < 64; ++i) { uint8_t c = lc.LCD_Data[i]; putchar(c >= 32 && c < 127 ? c : '.'); }
    printf("|\n");
    if (!out_path.empty())
    {
        std::ofstream f(out_path, std::ios::binary);
        f.write((const char*)out.data(), (std::streamsize)(out.size() * 2));
    }
    printf("OK: pack loaded in %" PRIu64 " clocks (NVRAM %s), %" PRIu64 " steps, %" PRIu64 " samples identical (peak %.1f dBFS), "
           "%.1f clocks/step (max %" PRIu64 "), %" PRIu64 " DDR3 reads, MIDI bytes %zu\n",
           load_clocks, pack_nvram ? "from the pack" : "empty", step_no, compared,
           peak > 0 ? 20 * std::log10((double)peak / 2147483648.0) : -999.0, (double)run_clocks / (double)std::max<uint64_t>(1, step_no),
           max_step, ddr_reads, midi_bytes);
    delete t;
    return 0;
}
