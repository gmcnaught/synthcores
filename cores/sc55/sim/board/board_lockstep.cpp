// Board lockstep (M5): hdl/board/sc55_board.sv from power-on: the ROM pack
// arrives over the ioctl handshake (respecting ioctl_wait), the wave ROMs go
// to a DDR3 model (random BUSY, random in-order read latency), the core
// clears and resets itself; then it runs one step at a time (step_gate)
// against the reference model with the same MIDI bytes, comparing PC after
// every step and the two sample streams in order.
//
// Usage: board_lockstep --roms DIR --pack FILE [--steps N] [--midi-loud S | --midi-demo S] [--seed S]
#include "Vsc55_board.h"
#include "Vsc55_board___024root.h"
#include "verilated.h"

#include "../common/midi_demo.h"
#include "emu.h"
#include "mcu.h"
#include "lcd.h"
#include "pcm.h"
#include "rom_loader.h"

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

uint8_t PCM_ReadROM(pcm_t& pcm, uint32_t address);
struct Frame { int32_t l, r; };

// The model only keeps LCD state when a backend is attached.
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
    std::string rom_dir, pack_path;
    uint64_t    steps = 6'000'000, seed = 1;
    int64_t     midi_seed = -1, loud_seed = -1;
    int         busy_mod = 4, lat_min = 8, lat_max = 24;
    bool        zero_init = false, serial = false;
    std::string lcd_ppm;
    std::vector<uint8_t> frame(741 * 268 * 3, 0);
    for (int i = 1; i < argc; ++i)
    {
        std::string a = argv[i];
        if (a == "--roms" && i + 1 < argc) rom_dir = argv[++i];
        else if (a == "--pack" && i + 1 < argc) pack_path = argv[++i];
        else if (a == "--steps" && i + 1 < argc) steps = std::strtoull(argv[++i], nullptr, 0);
        else if (a == "--midi-demo" && i + 1 < argc) midi_seed = std::strtoll(argv[++i], nullptr, 0);
        else if (a == "--midi-loud" && i + 1 < argc) loud_seed = std::strtoll(argv[++i], nullptr, 0);
        else if (a == "--seed" && i + 1 < argc) seed = std::strtoull(argv[++i], nullptr, 0);
        else if (a == "--zero-init") zero_init = true;
        else if (a == "--serial") serial = true;
        else if (a == "--lcd-ppm" && i + 1 < argc) lcd_ppm = argv[++i];
        else if (a == "--ddr-busy" && i + 1 < argc) busy_mod = std::atoi(argv[++i]);
        else if (a == "--ddr-lat" && i + 2 < argc) { lat_min = std::atoi(argv[++i]); lat_max = std::atoi(argv[++i]); }
        else { fprintf(stderr, "unknown argument %s\n", a.c_str()); return 2; }
    }
    std::ifstream pf(pack_path, std::ios::binary);
    std::vector<uint8_t> pack((std::istreambuf_iterator<char>(pf)), {});
    if (pack.size() != 0x348100) { fprintf(stderr, "pack %s: %zu bytes\n", pack_path.c_str(), pack.size()); return 2; }

    common::LoadRomsetResult load;
    common::RomOverrides     ov{};
    if (auto err = common::LoadRomset(rom_dir, "mk1-v1.21", common::RomLoader::Hashing, ov, load); err != common::LoadRomsetError{})
    {
        fprintf(stderr, "romset load failed: %s\n", common::ToCString(err));
        return 2;
    }
    Emulator emu;
    NullLcd  null_lcd;
    emu.Init({.lcd_backend = &null_lcd});
    emu.LoadRoms(load.romset, load.romset_info);
    emu.Reset();
    emu.StartLCD();
    emu.GetPCM().enable_oversampling = true;
    emu.SetSampleCallback(sample_cb, nullptr);
    mcu_t& m = emu.GetMCU();
    {
        // the pack must hold the wave ROMs as the model has them (unscrambled)
        pcm_t& pm = emu.GetPCM();
        for (uint32_t i = 0; i < 0x300000; ++i)
        {
            uint8_t want = i < 0x100000 ? pm.waverom1[i] : i < 0x200000 ? pm.waverom2[i - 0x100000] : pm.waverom3[i - 0x200000];
            if (pack[0x48100 + i] != want) { fprintf(stderr, "pack wave byte %06x: %02x, model %02x\n", i, pack[0x48100 + i], want); return 2; }
        }
        for (uint32_t i = 0; i < 0x8000; ++i) if (pack[0x100 + i] != m.rom1[i]) { fprintf(stderr, "pack ROM1 differs at %x\n", i); return 2; }
        for (uint32_t i = 0; i < 0x40000; ++i) if (pack[0x8100 + i] != m.rom2[i]) { fprintf(stderr, "pack ROM2 differs at %x\n", i); return 2; }
    }

    VerilatedContext ctx;
    ctx.commandArgs(argc, argv);
    ctx.randReset(zero_init ? 0 : 2);       // random contents in every register and memory (default)
    Vsc55_board* t = new Vsc55_board{&ctx};
    std::mt19937_64 rng(seed);
    uint64_t clocks = 0;

    // DDR3 model: 64-bit words, BUSY on random clocks, reads back in order after 8..24 clocks
    std::vector<uint64_t> ddr(1 << 19, 0);      // 4 MB from WAVE_BASE
    const uint32_t base_w = 0x30000000u >> 3;
    std::deque<std::pair<uint64_t, uint32_t>> rdq;
    std::vector<Frame> hdl;
    std::deque<uint32_t> romcheck;
    std::string last_pcm;
    uint32_t pre_vm = 0, pre_vmp = 0; bool pre_upd = false; uint64_t pre_cyc = 0; uint16_t pre_r2[16] = {};
    uint64_t last_rd_due = 0, ddr_reads = 0;
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
        // commands are taken at this edge when not busy
        if (!t->DDRAM_BUSY && !t->rst)
        {
            uint32_t w = t->DDRAM_ADDR - base_w;
            if (t->DDRAM_RD)
            {
                if (w >= ddr.size()) { fprintf(stderr, "DDR3 read outside the wave ROM: %x\n", t->DDRAM_ADDR); exit(1); }
                uint64_t due = std::max<uint64_t>(clocks + lat_min + rng() % (lat_max - lat_min + 1), last_rd_due + 1);
                last_rd_due = due;
                rdq.push_back({due, w});
                ++ddr_reads;
            }
            if (t->DDRAM_WE)
            {
                if (w >= ddr.size()) { fprintf(stderr, "DDR3 write outside the wave ROM: %x\n", t->DDRAM_ADDR); exit(1); }
                for (int b = 0; b < 8; ++b)
                    if (t->DDRAM_BE & (1 << b))
                        ddr[w] = (ddr[w] & ~(0xffull << (8 * b))) | (((t->DDRAM_DIN >> (8 * b)) & 0xff) << (8 * b));
            }
        }
        // wave ROM stream monitor: every byte the PCM gets must be the pack's
        auto* R = t->rootp;
        if (R->sc55_board__DOT__core__DOT__p_req && R->sc55_board__DOT__core__DOT__p_ack)
        {
            char b[32];
            snprintf(b, sizeof b, "%s%02x=%02x ", R->sc55_board__DOT__core__DOT__p_we ? "W" : "R", R->sc55_board__DOT__core__DOT__p_addr,
                     R->sc55_board__DOT__core__DOT__p_we ? R->sc55_board__DOT__core__DOT__p_wdata : R->sc55_board__DOT__core__DOT__p_rdata);
            last_pcm += b;
        }
        if (R->sc55_board__DOT__rom_req && R->sc55_board__DOT__rom_ready)
            romcheck.push_back(R->sc55_board__DOT__rom_addr);
        if (R->sc55_board__DOT__rom_rvalid && !t->rst)
        {
            if (romcheck.empty()) { fprintf(stderr, "wave ROM: data without a request at clock %" PRIu64 " (rst %d core_rst %d running %d)\n", clocks, t->rst, R->sc55_board__DOT__core_rst, t->running); exit(1); }
            uint32_t a = romcheck.front();
            romcheck.pop_front();
            if (R->sc55_board__DOT__rom_data != pack[0x48100 + a])
            {
                fprintf(stderr, "wave ROM byte %06x: got %02x, pack %02x (clock %" PRIu64 ")\n", a, R->sc55_board__DOT__rom_data,
                        pack[0x48100 + a], clocks);
                exit(1);
            }
        }
        t->clk = 1; t->clk_vid = 1; t->eval();
        t->clk_vid = 0;
        ++clocks;
        if (t->sample_valid) hdl.push_back({(int32_t)t->sample_l, (int32_t)t->sample_r});
    };

    t->clk_vid = 0;
    t->rst = 1; t->user_reset = 0; t->ioctl_download = 0; t->ioctl_wr = 0; t->uart_rxd = 1;
    t->midi_inject = 0; t->buttons = 0; t->pace_enable = 0; t->step_gate = 0;
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
    uint64_t load_clocks = clocks;
    for (int g = 0; g < 100000 && !t->running; ++g) tick();
    if (!t->rom_ok || !t->running) { fprintf(stderr, "core did not start: rom_ok %d running %d\n", t->rom_ok, t->running); return 1; }
    {
        auto* R = t->rootp;
        size_t bad1 = 0, bad2 = 0, first = ~0ull;
        for (size_t i = 0; i < 0x8000; ++i) if (R->sc55_board__DOT__core__DOT__mcu__DOT__rom1[i] != m.rom1[i]) { if (!bad1) first = i; ++bad1; }
        for (size_t i = 0; i < 0x40000; ++i) if (R->sc55_board__DOT__core__DOT__mcu__DOT__rom2[i] != m.rom2[i]) { if (!bad2 && first == ~0ull) first = i; ++bad2; }
        if (bad1 || bad2) { fprintf(stderr, "after load: ROM1 %zu bytes differ, ROM2 %zu (first at %zx)\n", bad1, bad2, first); return 1; }
    }
    for (uint32_t w = 0; w < 0x60000; ++w)
    {
        uint64_t want = 0;
        for (int b = 0; b < 8; ++b) want |= (uint64_t)pack[0x48100 + w * 8 + b] << (8 * b);
        if (ddr[w] != want) { fprintf(stderr, "DDR3 word %x after load: %016" PRIx64 ", pack %016" PRIx64 "\n", w, ddr[w], want); return 1; }
    }

    std::vector<uint8_t> midi;
    if (midi_seed >= 0) midi = make_midi_demo((uint64_t)midi_seed);
    if (loud_seed >= 0) midi = make_midi_loud((uint64_t)loud_seed);
    size_t pos = 0;

    std::deque<Frame> mq, hq;
    uint64_t compared = 0, max_step = 0, run_clocks = 0;
    int64_t  peak = 0;
    uint64_t m_total = 0, h_total = 0, m_first = ~0ull, h_first = ~0ull, state_checks = 0;
    auto drain = [&]() -> bool {
        for (auto& f : g_model) { if (m_first == ~0ull && f.l != 16777216) m_first = m_total; ++m_total; mq.push_back(f); }
        for (auto& f : hdl) { if (h_first == ~0ull && f.l != 16777216) h_first = h_total; ++h_total; hq.push_back(f); }
        g_model.clear();
        hdl.clear();
        while (!mq.empty() && !hq.empty())
        {
            if (mq.front().l != hq.front().l || mq.front().r != hq.front().r)
            {
                fprintf(stderr, "MISMATCH sample %" PRIu64 ": HDL %d,%d model %d,%d (first non-DC sample: HDL %" PRIu64 ", model %" PRIu64
                        "; state checks %" PRIu64 ")\n", compared, hq.front().l, hq.front().r, mq.front().l, mq.front().r, h_first, m_first, state_checks);
                return false;
            }
            peak = std::max<int64_t>(peak, std::abs((int64_t)hq.front().l));
            mq.pop_front();
            hq.pop_front();
            ++compared;
        }
        return true;
    };

    auto pcm_check = [&](uint64_t s, bool force) -> bool {
        auto* R = t->rootp;
        pcm_t& pm = emu.GetPCM();
        if (force)
            for (int g = 0; g < 10'000'000 && !(R->sc55_board__DOT__core__DOT__pcm__DOT__ps == 0 &&
                                               R->sc55_board__DOT__core__DOT__pcm__DOT__pcm_cycles >= pm.cycles); ++g) tick();
        if (!(R->sc55_board__DOT__core__DOT__pcm__DOT__ps == 0 && R->sc55_board__DOT__core__DOT__pcm__DOT__pcm_cycles == pm.cycles))
            return true;
        std::string diff;
        ++state_checks;
        auto c = [&](const std::string& n, int64_t h, int64_t r) { if (h != r && diff.size() < 600) diff += n + "(hdl " + std::to_string(h) + " ref " + std::to_string(r) + ") "; };
        for (int r = 0; r < 32; ++r)
        {
            for (int i = 0; i < 8; ++i) c("ram1[" + std::to_string(r) + "][" + std::to_string(i) + "]", R->sc55_board__DOT__core__DOT__pcm__DOT__ram1[r][i], pm.ram1[r][i]);
            for (int i = 0; i < 16; ++i) c("ram2[" + std::to_string(r) + "][" + std::to_string(i) + "]", R->sc55_board__DOT__core__DOT__pcm__DOT__ram2[r][i], pm.ram2[r][i]);
        }
        c("voice_mask", R->sc55_board__DOT__core__DOT__pcm__DOT__voice_mask, pm.voice_mask);
        c("voice_mask_pending", R->sc55_board__DOT__core__DOT__pcm__DOT__voice_mask_pending, pm.voice_mask_pending);
        c("tv_counter", R->sc55_board__DOT__core__DOT__pcm__DOT__tv_counter, pm.tv_counter);
        c("accum_l", (int32_t)R->sc55_board__DOT__core__DOT__pcm__DOT__accum_l, pm.accum_l);
        c("accum_r", (int32_t)R->sc55_board__DOT__core__DOT__pcm__DOT__accum_r, pm.accum_r);
        c("rcsum0", (int32_t)R->sc55_board__DOT__core__DOT__pcm__DOT__rcsum[0], pm.rcsum[0]);
        c("rcsum1", (int32_t)R->sc55_board__DOT__core__DOT__pcm__DOT__rcsum[1], pm.rcsum[1]);
        c("irq", R->sc55_board__DOT__core__DOT__pcm__DOT__irq_assert, pm.irq_assert);
        c("write_latch", R->sc55_board__DOT__core__DOT__pcm__DOT__write_latch, pm.write_latch);
        c("wave_byte_latch", R->sc55_board__DOT__core__DOT__pcm__DOT__wave_byte_latch, pm.wave_byte_latch);
        if ((force && !serial) || (s % 4096) == 0)
            for (int i = 0; i < 0x4000; ++i) c("eram[" + std::to_string(i) + "]", R->sc55_board__DOT__core__DOT__pcm__DOT__eram[i], pm.eram[i]);
        if (!diff.empty())
        {
            fprintf(stderr, "PCM state differs at step %" PRIu64 " (pcm cycles %" PRIu64 "): %s\n", s, pm.cycles, diff.c_str());
            fprintf(stderr, "  voice_mask %08x pending %08x updating %d / hdl %08x %08x %d; reg_slots %d\n", pm.voice_mask, pm.voice_mask_pending,
                    pm.voice_mask_updating, (uint32_t)R->sc55_board__DOT__core__DOT__pcm__DOT__voice_mask,
                    (uint32_t)R->sc55_board__DOT__core__DOT__pcm__DOT__voice_mask_pending, (int)R->sc55_board__DOT__core__DOT__pcm__DOT__voice_mask_updating,
                    pm.config.reg_slots);
            fprintf(stderr, "  slot 23 ram1:"); for (int i = 0; i < 8; ++i) fprintf(stderr, " %x", pm.ram1[23][i]);
            fprintf(stderr, "\n  slot 23 ram2:"); for (int i = 0; i < 16; ++i) fprintf(stderr, " %x", pm.ram2[23][i]);
            fprintf(stderr, "\n  last PCM accesses this step: %s\n", last_pcm.c_str());
            fprintf(stderr, "  model before the step: voice_mask %08x pending %08x updating %d pcm cycles %" PRIu64 "; slot 23 ram2:", pre_vm, pre_vmp, pre_upd, pre_cyc);
            for (int i = 0; i < 16; ++i) fprintf(stderr, " %x", pre_r2[i]);
            fprintf(stderr, "\n  model PCM_ReadROM(0x7c32) = %02x, 3d = %02x, waverom1[0x7c32] = %02x, is_mk1 %d",
                    PCM_ReadROM(pm, 0x7c32), pm.config_reg_3d, pm.waverom1[0x7c32], emu.GetMCU().is_mk1);
            fprintf(stderr, "\n  hdl voice_active %08x, slot 23 ram2:", (uint32_t)R->sc55_board__DOT__core__DOT__pcm__DOT__voice_active);
            for (int i = 0; i < 16; ++i) fprintf(stderr, " %x", R->sc55_board__DOT__core__DOT__pcm__DOT__ram2[23][i]);
            fprintf(stderr, "\n");
            return false;
        }
        if (force && !serial) fprintf(stderr, "PCM state identical at step %" PRIu64 "\n", s);
        return true;
    };
    for (uint64_t s = 0; s < steps; ++s)
    {
        if (!midi.empty() && s >= 4'000'000 && (s % 600) == 0 && pos < midi.size())
        {
            uint8_t b = midi[pos++];
            emu.PostMIDI(b);
            t->midi_inject = 1; t->midi_inject_data = b;
            tick();
            t->midi_inject = 0;
        }
        {
            pcm_t& pm = emu.GetPCM();
            pre_vm = pm.voice_mask; pre_vmp = pm.voice_mask_pending; pre_upd = pm.voice_mask_updating; pre_cyc = pm.cycles;
            for (int i = 0; i < 16; ++i) pre_r2[i] = pm.ram2[23][i];
        }
        emu.Step();
        last_pcm.clear();
        uint64_t c0 = clocks;
        t->step_gate = 1;
        tick();
        t->step_gate = 0;
        for (int g = 0; g < 50'000'000 && !t->step_done; ++g) tick();
        max_step = std::max(max_step, clocks - c0);
        run_clocks += clocks - c0;
        {
            auto* R = t->rootp;
            std::string d;
            if (R->sc55_board__DOT__core__DOT__mcu__DOT__uart_rx_byte != m.uart_rx_byte) d += "uart_rx_byte ";
            uint32_t hc = (uint32_t)(R->sc55_board__DOT__core__DOT__mcu__DOT__rx_wp - R->sc55_board__DOT__core__DOT__mcu__DOT__rx_rp) & 2047;
            if (hc != (m.uart_write_ptr - m.uart_read_ptr) % uart_buffer_size) d += "rx_count(hdl " + std::to_string(hc) + " ref " + std::to_string((m.uart_write_ptr - m.uart_read_ptr) % uart_buffer_size) + ") ";
            for (int a = 0; a < 0x80; ++a)
                if (!((a >= 0x10 && a < 0x40) || (a >= 0x50 && a < 0x55)) && R->sc55_board__DOT__core__DOT__mcu__DOT__dev[a] != m.dev_register[a])
                    d += "dev[" + std::to_string(a) + "] ";
            lcd_t& lc = emu.GetLCD();
            for (int i = 0; i < 80; ++i)
                if (R->sc55_board__DOT__dd[i] != lc.LCD_Data[i]) { d += "lcd data[" + std::to_string(i) + "] "; break; }
            for (int i = 0; i < 64; ++i)
                if (R->sc55_board__DOT__cg[i] != lc.LCD_CG[i]) { d += "lcd cg[" + std::to_string(i) + "] "; break; }
            if (R->sc55_board__DOT__lcd__DOT__dd_ram != lc.LCD_DD_RAM) d += "lcd dd_ram ";
            if (!d.empty()) { fprintf(stderr, "MCU state differs at step %" PRIu64 ": %s\n", s, d.c_str()); return 1; }
        }
        if (!t->step_done || t->dbg_pc != m.pc || t->dbg_cp != m.cp)
        {
            fprintf(stderr, "MISMATCH at step %" PRIu64 ": HDL %02x:%04x (done %d), model %02x:%04x\n", s, t->dbg_cp, t->dbg_pc,
                    t->step_done, m.cp, m.pc);
            return 1;
        }
        if (!drain()) { pcm_check(s, true); return 1; }
        // PCM state, whenever the HDL PCM has caught up with the model
        if (!pcm_check(s, serial)) return 1;
    }
    // one video frame against the model's LCD_Render (741 x 268, ARGB; compare RGB)
    uint64_t lcd_pixels = 0, lcd_lit = 0;
    {
        lcd_t& lc = emu.GetLCD();
        LCD_Render(lc);
        // the frame must not change while it is drawn: hold the core still
        bool seen_start = false;
        for (uint64_t g = 0; g < 4'000'000; ++g)
        {
            tick();
            if (!t->lcd_pix) continue;
            if (t->lcd_x == 0 && t->lcd_y == 0) { if (seen_start) break; seen_start = true; }
            if (!seen_start) continue;
            uint32_t want = lc.buffer[t->lcd_y][t->lcd_x] & 0xffffff;
            uint32_t got = ((uint32_t)t->vid_r << 16) | ((uint32_t)t->vid_g << 8) | t->vid_b;
            if (got != want)
            {
                fprintf(stderr, "LCD pixel (%d, %d): HDL %06x, model %06x\n", t->lcd_x, t->lcd_y, got, want);
                return 1;
            }
            ++lcd_pixels;
            lcd_lit += (want == 0x000000);
            size_t o = ((size_t)t->lcd_y * 741 + t->lcd_x) * 3;
            frame[o] = t->vid_r; frame[o + 1] = t->vid_g; frame[o + 2] = t->vid_b;
        }
        if (lcd_pixels != 741ull * 268) { fprintf(stderr, "LCD frame: %" PRIu64 " pixels compared\n", lcd_pixels); return 1; }
    }
    if (!lcd_ppm.empty())
    {
        std::ofstream f(lcd_ppm, std::ios::binary);
        f << "P6\n741 268\n255\n";
        f.write((const char*)frame.data(), (std::streamsize)frame.size());
    }
    printf("LCD frame identical (%" PRIu64 " pixels, %" PRIu64 " black); text: ", lcd_pixels, lcd_lit);
    for (int i = 0; i < 80; ++i) { uint8_t c = emu.GetLCD().LCD_Data[i]; putchar(c >= 32 && c < 127 ? c : '.'); }
    printf("\n");
    printf("OK: pack loaded in %" PRIu64 " clocks, %" PRIu64 " steps, %" PRIu64 " samples identical (peak %.1f dBFS), "
           "%.1f clocks/step (max %" PRIu64 "), %" PRIu64 " DDR3 reads, MIDI bytes %zu\n",
           load_clocks, steps, compared, peak > 0 ? 20 * std::log10((double)peak / 2147483648.0) : -999.0,
           (double)run_clocks / (double)steps, max_step, ddr_reads, pos);
    delete t;
    return 0;
}
