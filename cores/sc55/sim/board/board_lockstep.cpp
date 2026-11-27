// Board lockstep (M5): hdl/board/sc55_board.sv from power-on: the ROM pack
// arrives over the ioctl handshake (respecting ioctl_wait), the wave ROMs go
// to a DDR3 model (random BUSY, random in-order read latency), the core
// clears and resets itself; then it runs one step at a time (step_gate)
// against the reference model with the same MIDI bytes, comparing PC after
// every step and the two sample streams in order.
//
// MIDI out: uart_txd, decoded at 31250 baud, must carry the bytes the model sends (ref/patch_ref.py's
// tx hook), in order, with none dropped.
//
// Usage: board_lockstep --roms DIR --pack FILE [--steps N] [--midi-loud S | --midi-demo S] [--seed S]
//                       [--midi-at STEP:HEX]...
//        [--rx-baud B] (mk1, SC-155): --midi-at bytes go out on uart_rxd as 8N1 at B baud, back to back
//        from their step (as `cat > /dev/ttyS1` sends them), through synthframe_midi_rx's speed
//        detection; the model gets each byte in the step in which the HDL MCU reads it, and uart_txd is
//        decoded at B baud, as the host reads it. [--step-clocks N]: each step takes at least N clocks
//        (16 is about real time at 50 MHz for the mk1).
//        [--drop-hdl-from STEP] (bench self-test): discard the HDL's samples from STEP on, as a PCM
//        that stops late in the run would; the end-of-run count check must then fail.
#include "Vsc55_board.h"
#include "Vsc55_board___024root.h"
#include "verilated.h"

#include "../common/midi_demo.h"
#include "../common/uart_decode.h"
#include "ref_hooks.h"
#include "emu.h"
#include "mcu.h"
#include "submcu.h"
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

// ram1/ram2 live in one LUT RAM per word index (sc55_pcm.sv g_r1/g_r2)
static uint32_t pcm_ram1(Vsc55_board___024root* R, int r, int i)
{
    switch (i)
    {
    case 0: return R->sc55_board__DOT__core__DOT__pcm__DOT__g_r1__BRA__0__KET____DOT__mem[r];
    case 1: return R->sc55_board__DOT__core__DOT__pcm__DOT__g_r1__BRA__1__KET____DOT__mem[r];
    case 2: return R->sc55_board__DOT__core__DOT__pcm__DOT__g_r1__BRA__2__KET____DOT__mem[r];
    case 3: return R->sc55_board__DOT__core__DOT__pcm__DOT__g_r1__BRA__3__KET____DOT__mem[r];
    case 4: return R->sc55_board__DOT__core__DOT__pcm__DOT__g_r1__BRA__4__KET____DOT__mem[r];
    case 5: return R->sc55_board__DOT__core__DOT__pcm__DOT__g_r1__BRA__5__KET____DOT__mem[r];
    case 6: return R->sc55_board__DOT__core__DOT__pcm__DOT__g_r1__BRA__6__KET____DOT__mem[r];
    case 7: return R->sc55_board__DOT__core__DOT__pcm__DOT__g_r1__BRA__7__KET____DOT__mem[r];
    }
    return 0;
}
static uint32_t pcm_ram2(Vsc55_board___024root* R, int r, int i)
{
    switch (i)
    {
    case 0: return R->sc55_board__DOT__core__DOT__pcm__DOT__g_r2__BRA__0__KET____DOT__mem[r];
    case 1: return R->sc55_board__DOT__core__DOT__pcm__DOT__g_r2__BRA__1__KET____DOT__mem[r];
    case 2: return R->sc55_board__DOT__core__DOT__pcm__DOT__g_r2__BRA__2__KET____DOT__mem[r];
    case 3: return R->sc55_board__DOT__core__DOT__pcm__DOT__g_r2__BRA__3__KET____DOT__mem[r];
    case 4: return R->sc55_board__DOT__core__DOT__pcm__DOT__g_r2__BRA__4__KET____DOT__mem[r];
    case 5: return R->sc55_board__DOT__core__DOT__pcm__DOT__g_r2__BRA__5__KET____DOT__mem[r];
    case 6: return R->sc55_board__DOT__core__DOT__pcm__DOT__g_r2__BRA__6__KET____DOT__mem[r];
    case 7: return R->sc55_board__DOT__core__DOT__pcm__DOT__g_r2__BRA__7__KET____DOT__mem[r];
    case 8: return R->sc55_board__DOT__core__DOT__pcm__DOT__g_r2__BRA__8__KET____DOT__mem[r];
    case 9: return R->sc55_board__DOT__core__DOT__pcm__DOT__g_r2__BRA__9__KET____DOT__mem[r];
    case 10: return R->sc55_board__DOT__core__DOT__pcm__DOT__g_r2__BRA__10__KET____DOT__mem[r];
    case 11: return R->sc55_board__DOT__core__DOT__pcm__DOT__g_r2__BRA__11__KET____DOT__mem[r];
    case 12: return R->sc55_board__DOT__core__DOT__pcm__DOT__g_r2__BRA__12__KET____DOT__mem[r];
    case 13: return R->sc55_board__DOT__core__DOT__pcm__DOT__g_r2__BRA__13__KET____DOT__mem[r];
    case 14: return R->sc55_board__DOT__core__DOT__pcm__DOT__g_r2__BRA__14__KET____DOT__mem[r];
    case 15: return R->sc55_board__DOT__core__DOT__pcm__DOT__g_r2__BRA__15__KET____DOT__mem[r];
    }
    return 0;
}

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
    std::string rom_dir, pack_path, romset = "mk1-v1.21";
    uint64_t    steps = 6'000'000, seed = 1;
    uint64_t    drop_hdl_from = ~0ull, cur_step = 0;
    int64_t     midi_seed = -1, loud_seed = -1;
    int         busy_mod = 4, lat_min = 8, lat_max = 24;
    bool        zero_init = false, serial = false;
    int64_t     rx_baud = 0, step_clocks = 0;
    bool        rx_trace = false;
    std::string lcd_ppm;
    uint64_t    free_run = 0, stall_at = 0, stall_len = 0;   // free run: close step_gate for stall_len clocks at stall_at
    std::vector<uint8_t> frame(741 * 268 * 3, 0);
    std::deque<std::pair<uint64_t, uint8_t>> sched;   // --midi-at
    for (int i = 1; i < argc; ++i)
    {
        std::string a = argv[i];
        if (a == "--roms" && i + 1 < argc) rom_dir = argv[++i];
        else if (a == "--romset" && i + 1 < argc) romset = argv[++i];
        else if (a == "--pack" && i + 1 < argc) pack_path = argv[++i];
        else if (a == "--steps" && i + 1 < argc) steps = std::strtoull(argv[++i], nullptr, 0);
        else if (a == "--midi-demo" && i + 1 < argc) midi_seed = std::strtoll(argv[++i], nullptr, 0);
        else if (a == "--midi-loud" && i + 1 < argc) loud_seed = std::strtoll(argv[++i], nullptr, 0);
        else if (a == "--seed" && i + 1 < argc) seed = std::strtoull(argv[++i], nullptr, 0);
        else if (a == "--midi-at" && i + 1 < argc)
        {
            std::string v = argv[++i];
            size_t c = v.find(':');
            if (c == std::string::npos || (v.size() - c - 1) % 2) { fprintf(stderr, "--midi-at STEP:HEX\n"); return 2; }
            uint64_t at = std::strtoull(v.substr(0, c).c_str(), nullptr, 0);
            for (size_t k = c + 1; k < v.size(); k += 2) sched.push_back({at, (uint8_t)std::stoul(v.substr(k, 2), nullptr, 16)});
        }
        else if (a == "--zero-init") zero_init = true;
        else if (a == "--serial") serial = true;
        else if (a == "--rx-baud" && i + 1 < argc) rx_baud = std::strtoll(argv[++i], nullptr, 0);
        else if (a == "--rx-trace") rx_trace = true;
        else if (a == "--step-clocks" && i + 1 < argc) step_clocks = std::strtoll(argv[++i], nullptr, 0);
        else if (a == "--drop-hdl-from" && i + 1 < argc) drop_hdl_from = std::strtoull(argv[++i], nullptr, 0);
        else if (a == "--lcd-ppm" && i + 1 < argc) lcd_ppm = argv[++i];
        else if (a == "--free-run" && i + 1 < argc) free_run = std::strtoull(argv[++i], nullptr, 0);
        else if (a == "--stall-at" && i + 2 < argc) { stall_at = std::strtoull(argv[++i], nullptr, 0); stall_len = std::strtoull(argv[++i], nullptr, 0); }
        else if (a == "--ddr-busy" && i + 1 < argc) busy_mod = std::atoi(argv[++i]);
        else if (a == "--ddr-lat" && i + 2 < argc) { lat_min = std::atoi(argv[++i]); lat_max = std::atoi(argv[++i]); }
        else { fprintf(stderr, "unknown argument %s\n", a.c_str()); return 2; }
    }
    std::ifstream pf(pack_path, std::ios::binary);
    std::vector<uint8_t> pack((std::istreambuf_iterator<char>(pf)), {});
    // pack v2, or v3 (wave ROMs as dumped, unscrambled by the loader) (tools/romset.py, hdl/board/rom_loader.sv)
    const size_t ROM1_AT = 0x100, ROM2_AT = 0x8100, SM_AT = 0x88100, WAVE_AT = 0x89100;
    if (pack.size() != 0x489100 || (pack[4] != 2 && pack[4] != 3)) { fprintf(stderr, "pack %s: %zu bytes, not a v2 or v3 pack\n", pack_path.c_str(), pack.size()); return 2; }
    const bool v3 = pack[4] == 3;
    // the wave ROM image DDR3 must hold after the load: the pack's (v2), or the model's unscrambled ROMs (v3)
    std::vector<uint8_t> wave(pack.begin() + WAVE_AT, pack.begin() + WAVE_AT + 0x400000);

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
    emu.Reset();
    emu.StartLCD();
    emu.GetPCM().enable_oversampling = true;
    emu.SetSampleCallback(sample_cb, nullptr);
    mcu_t& m = emu.GetMCU();
    if (rx_baud && !m.is_mk1) { fprintf(stderr, "--rx-baud: mk1 and SC-155 only (the mk2's sub-MCU reads the buffer)\n"); return 2; }
    {
        // a v2 pack must hold the wave ROMs as the model has them (unscrambled); for a v3 pack
        // the loader must put them in DDR3 that way
        pcm_t& pm = emu.GetPCM();
        const bool mk2p = !m.is_mk1;
        if (pack[5] != (uint8_t)((mk2p ? 2 : 0) | (m.is_sc155 ? 1 : 0))) { fprintf(stderr, "pack model %d does not match %s\n", pack[5], romset.c_str()); return 2; }
        for (uint32_t i = 0; i < 0x300000; ++i)
        {
            uint8_t want = mk2p ? (i < 0x200000 ? pm.waverom1[i] : pm.waverom2[i - 0x200000])
                                : (i < 0x100000 ? pm.waverom1[i] : i < 0x200000 ? pm.waverom2[i - 0x100000] : pm.waverom3[i - 0x200000]);
            if (v3) wave[i] = want;
            else if (pack[WAVE_AT + i] != want) { fprintf(stderr, "pack wave byte %06x: %02x, model %02x\n", i, pack[WAVE_AT + i], want); return 2; }
        }
        for (uint32_t i = 0; i < 0x8000; ++i) if (pack[ROM1_AT + i] != m.rom1[i]) { fprintf(stderr, "pack ROM1 differs at %x\n", i); return 2; }
        for (uint32_t i = 0; i <= m.rom2_mask; ++i) if (pack[ROM2_AT + i] != m.rom2[i]) { fprintf(stderr, "pack ROM2 differs at %x\n", i); return 2; }
        if (mk2p) for (uint32_t i = 0; i < 0x1000; ++i) if (pack[SM_AT + i] != m.sm->rom[i]) { fprintf(stderr, "pack sub-MCU ROM differs at %x\n", i); return 2; }
    }

    VerilatedContext ctx;
    ctx.commandArgs(argc, argv);
    ctx.randReset(zero_init ? 0 : 2);       // random contents in every register and memory (default)
    Vsc55_board* t = new Vsc55_board{&ctx};
    std::mt19937_64 rng(seed);
    uint64_t clocks = 0;

    // DDR3 model: 64-bit words, BUSY on random clocks, reads back in order after 8..24 clocks
    std::vector<uint64_t> ddr(0x88000, 0);      // 4 MB of wave ROM, then ROM2's upper 256 KB, from WAVE_BASE
    const uint32_t base_w = 0x30000000u >> 3;
    std::deque<std::pair<uint64_t, uint32_t>> rdq;
    std::vector<Frame> hdl;
    std::deque<uint32_t> romcheck;
    std::string last_pcm;
    uint32_t pre_vm = 0, pre_vmp = 0; bool pre_upd = false; uint64_t pre_cyc = 0; uint16_t pre_r2[16] = {};
    uint64_t last_rd_due = 0, ddr_reads = 0;
    const int64_t tx_bit = 50'000'000 / (rx_baud ? rx_baud : 31'250);   // the board at its default CLK_HZ
    UartDecode txdec(tx_bit);
    // --rx-baud: the serial line into uart_rxd, and the bytes synthframe_midi_rx delivered to the MCU
    std::deque<uint8_t> line_q;
    int64_t  line_bit = -1, line_left = 0;   // bit of the frame on the line (-1 idle), clocks left in it
    uint16_t line_frame = 0;
    std::deque<uint8_t> hdl_rx;              // in the HDL's receive buffer, not yet posted to the model
    uint64_t rx_line_bytes = 0, rx_hdl_bytes = 0;
    // MIDI out is checked from the speed match on: before it the board sends at 31250, which a host
    // at another speed cannot read (the bytes the model and the host had by then are not compared)
    bool   speed_matched = rx_baud == 0;
    size_t tx_skip_ref = 0, tx_skip_line = 0;
    int    tx_skip_err = 0;
    static std::vector<uint8_t> tx_ref;
    g_ref_hooks.tx = [](void*, int, uint64_t, uint8_t d) { tx_ref.push_back(d); };
    auto tick = [&]() {
        t->DDRAM_BUSY = busy_mod > 0 && (rng() % busy_mod) == 0;
        t->DDRAM_DOUT_READY = 0;
        if (!rdq.empty() && rdq.front().first <= clocks)
        {
            t->DDRAM_DOUT_READY = 1;
            t->DDRAM_DOUT = ddr[rdq.front().second];
            rdq.pop_front();
        }
        if (rx_baud)
        {
            if (line_bit < 0 && !line_q.empty())
            {
                line_frame = (uint16_t)(0x200 | (line_q.front() << 1));   // start 0, data LSB first, stop 1
                line_q.pop_front();
                line_bit = 0;
                line_left = 50'000'000 / rx_baud;
                ++rx_line_bytes;
            }
            t->uart_rxd = line_bit < 0 ? 1 : (line_frame >> line_bit) & 1;
            if (line_bit >= 0 && --line_left == 0)
            {
                if (++line_bit == 10) line_bit = -1;
                else line_left = 50'000'000 / rx_baud;
            }
        }
        t->clk = 0; t->clk_vid = 0; t->eval();
        // a byte from synthframe_midi_rx goes into the MCU's buffer at this edge
        if (rx_baud && t->rootp->sc55_board__DOT__rx_valid && t->running && !t->rootp->sc55_board__DOT__core__DOT__midi_full)
        {
            hdl_rx.push_back(t->rootp->sc55_board__DOT__rx_data);
            ++rx_hdl_bytes;
            if (rx_trace) printf("step %" PRIu64 ": MIDI in %02x (speed %s)\n", cur_step, t->rootp->sc55_board__DOT__rx_data, t->midi_alt ? "alt" : "standard");
        }
        if (rx_trace)
        {
            static int alt_was = 0;
            static size_t tx_seen = 0;
            static int scr_was = -1;
            const int scr = t->rootp->sc55_board__DOT__core__DOT__mcu__DOT__dev[0x5a];
            if (scr != scr_was) { printf("step %" PRIu64 ": SCR %02x (SSR %02x)\n", cur_step, scr, t->rootp->sc55_board__DOT__core__DOT__mcu__DOT__dev[0x5c]); scr_was = scr; }
            if (t->midi_alt != alt_was) { printf("step %" PRIu64 ": midi_alt %d\n", cur_step, t->midi_alt); alt_was = t->midi_alt; }
            if (t->rootp->sc55_board__DOT__tx_valid) printf("step %" PRIu64 ": MCU sent %02x (midi_alt %d)\n", cur_step, t->rootp->sc55_board__DOT__tx_data, t->midi_alt);
            if (txdec.bytes.size() != tx_seen) { printf("step %" PRIu64 ": host got %02x\n", cur_step, txdec.bytes.back()); tx_seen = txdec.bytes.size(); }
        }
        if (!speed_matched && (t->midi_alt != 0) == (rx_baud != 31'250) && txdec.idle())
        {
            speed_matched = true;
            tx_skip_ref = tx_ref.size();
            tx_skip_line = txdec.bytes.size();
            tx_skip_err = txdec.errors;
        }
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
            if (R->sc55_board__DOT__rom_data != wave[a])
            {
                fprintf(stderr, "wave ROM byte %06x: got %02x, want %02x (clock %" PRIu64 ")\n", a, R->sc55_board__DOT__rom_data,
                        wave[a], clocks);
                exit(1);
            }
        }
        t->clk = 1; t->clk_vid = 1; t->eval();
        t->clk_vid = 0;
        ++clocks;
        if (t->sample_valid && cur_step < drop_hdl_from) hdl.push_back({(int32_t)t->sample_l, (int32_t)t->sample_r});
        txdec.clock(t->uart_txd);
    };

    t->clk_vid = 0;
    t->rst = 1; t->user_reset = 0; t->ioctl_download = 0; t->ioctl_wr = 0; t->uart_rxd = 1;
    t->midi_inject = 0; t->buttons = 0; t->pace_enable = 0; t->step_gate = 0; t->gain = 0;
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
        for (size_t i = 0; i < 0x8000; ++i) if (R->sc55_board__DOT__core__DOT__mcu__DOT__u_rom1__DOT__mem[i] != m.rom1[i]) { if (!bad1) first = i; ++bad1; }
        for (size_t i = 0; i < 0x40000; ++i) if (R->sc55_board__DOT__core__DOT__mcu__DOT__u_rom2__DOT__mem[i] != m.rom2[i]) { if (!bad2 && first == ~0ull) first = i; ++bad2; }
        if (bad1 || bad2) { fprintf(stderr, "after load: ROM1 %zu bytes differ, ROM2 %zu (first at %zx)\n", bad1, bad2, first); return 1; }
    }
    for (uint32_t w = 0; w < 0x88000; ++w)
    {
        uint64_t want = 0;
        for (int b = 0; b < 8; ++b)
            want |= (uint64_t)(w < 0x80000 ? wave[(size_t)w * 8 + b] : pack[ROM2_AT + 0x40000 + (size_t)(w - 0x80000) * 8 + b]) << (8 * b);
        if (ddr[w] != want) { fprintf(stderr, "DDR3 word %x after load: %016" PRIx64 ", pack %016" PRIx64 "\n", w, ddr[w], want); return 1; }
    }

    std::vector<uint8_t> midi;
    if (free_run)
    {
        // throughput: the gate open, no model; loud MIDI paced at 3125 bytes/s of real time (50 MHz)
        std::vector<uint8_t> fm = make_midi_loud(1);
        size_t fp = 0;
        uint64_t c0 = clocks, steps0 = 0, worst = 0, last = clocks;
        t->step_gate = 1;
        t->pace_enable = stall_len ? 1 : 0;   // throughput: as fast as possible; stall test: paced
        auto* R = t->rootp;
        uint64_t prof[5] = {0, 0, 0, 0, 0};
        uint64_t hold_sm = 0, hold_pcm = 0, prof_steps = 0, q_sm = 0;
        const uint64_t prof_from = std::getenv("PROF_FROM") ? std::strtoull(std::getenv("PROF_FROM"), nullptr, 0) : 0;
        uint64_t win_c0 = clocks, win_max = 0, win_at = 0;
        int lag_reports = 0;
        uint64_t per_steps0 = 0;
        uint64_t cst[64] = {};   // clocks: Q_IDLE, Q_CPU (PCM access pending), Q_CPU (other), Q_TIMERS, other
        bool lag_before = false, ur_before = false;
        for (uint64_t i = 0; i < free_run; ++i)
        {
            if (stall_len && i == stall_at) { lag_before = R->sc55_board__DOT__lag_seen; ur_before = t->underruns != 0; }
            t->step_gate = !(stall_len && i >= stall_at && i < stall_at + stall_len);
            if ((i % 16000) == 0 && fp < fm.size() && clocks - c0 > 40'000'000) { t->midi_inject = 1; t->midi_inject_data = fm[fp++]; }
            {
                int q = R->sc55_board__DOT__core__DOT__mcu__DOT__q;
                bool preq = R->sc55_board__DOT__core__DOT__p_req;
                bool hold = R->sc55_board__DOT__core__DOT__mcu__DOT__int_hold && R->sc55_board__DOT__core__DOT__mcu__DOT__cpu__DOT__state == 2;   // S_INT
                if (clocks - c0 >= prof_from)
                {
                    prof[q == 0 ? 0 : q == 1 ? (preq ? 1 : hold ? 4 : 2) : q == 2 ? 3 : 4]++;
                    if (hold)
                    {
                        auto* M = R;
                        bool irq1 = M->sc55_board__DOT__core__DOT__mcu__DOT__mk2 && !M->sc55_board__DOT__core__DOT__mcu__DOT__sm_caught_up
                                    && !((M->sc55_board__DOT__core__DOT__mcu__DOT__int_req >> 2) & 1);
                        bool irq0 = !M->sc55_board__DOT__core__DOT__p_known && !M->sc55_board__DOT__core__DOT__p_irq0;
                        hold_sm += irq1; hold_pcm += irq0;
                    }
                    if (t->step_done) ++prof_steps;
                    if (q == 7) ++q_sm;
                }
                if (q == 1 && !preq && !hold && clocks - c0 >= prof_from) cst[R->sc55_board__DOT__core__DOT__mcu__DOT__cpu__DOT__state & 63]++;
            }
            if (((clocks - c0) % 10'000'000) == 0 && clocks > c0)
            {
                printf("t %.1f s: %.2f clocks/step over the last 10 M clocks, underruns %u, lag_us %u\n", (double)(clocks - c0) / 50e6,
                       10e6 / (double)std::max<uint64_t>(1, steps0 - per_steps0), (unsigned)t->underruns, (unsigned)t->lag_us);
                per_steps0 = steps0;
            }
            uint32_t lag_before = t->lag_us;
            tick();
            t->midi_inject = 0;
            if (stall_len && t->lag_us != lag_before && clocks - c0 > 50'000'000 && lag_reports < 8)
            {
                ++lag_reports;
                printf("lag +%u us at clock %" PRIu64 " (step %" PRIu64 "), %.2f clocks/step over the last 2,000-step window start, credit %" PRId64 "\n",
                       t->lag_us - lag_before, clocks - c0, steps0, (double)(clocks - win_c0) / (double)std::max<uint64_t>(1, steps0 % 2000),
                       (int64_t)R->sc55_board__DOT__pacer__DOT__credit);
            }
            if (t->step_done && (steps0 % 2000) == 0)
            {
                // the slowest 2,000-step window (pacing has 200 us of credit)
                if (steps0 > 0 && clocks - win_c0 > win_max) { win_max = clocks - win_c0; win_at = steps0; }
                win_c0 = clocks;
            }
            if (t->step_done) { ++steps0; worst = std::max(worst, clocks - last); last = clocks; }
        }
        if (stall_len)
            printf("stall of %" PRIu64 " clocks at %" PRIu64 ": lag_seen %d -> %d, underruns %d -> %u, lag_us %u\n", stall_len, stall_at,
                   lag_before, (int)R->sc55_board__DOT__lag_seen, ur_before, (unsigned)t->underruns, (unsigned)t->lag_us);
        {
            const char* nm[5] = {"idle", "cpu, PCM access pending", "cpu, other", "timers", "cpu, int_hold (S_INT)"};
            const double ns = (double)std::max<uint64_t>(1, prof_from ? prof_steps : steps0);
            for (int k = 0; k < 5; ++k) printf("  %-24s %6.2f clocks/step\n", nm[k], (double)prof[k] / ns);
            printf("  int_hold: sub-MCU (IRQ1) %.2f, PCM (IRQ0) %.2f; waiting for the sub-MCU at the device phase %.2f clocks/step\n",
                   (double)hold_sm / ns, (double)hold_pcm / ns, (double)q_sm / ns);
            for (int k = 0; k < 64; ++k) if (cst[k] * 100 > prof[2]) printf("    cpu state %2d %6.2f clocks/step\n", k, (double)cst[k] / ns);
        }
        printf("slowest 2,000-step window: %.2f clocks/step, ending at step %" PRIu64 "\n", (double)win_max / 2000.0, win_at);
        printf("free run: %" PRIu64 " clocks, %" PRIu64 " steps: %.2f clocks/step (real time at 50 MHz needs <= 30), longest step %" PRIu64 " clocks, MIDI bytes %zu\n",
               clocks - c0, steps0, (double)(clocks - c0) / (double)steps0, worst, fp);
        return 0;
    }
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
            for (int i = 0; i < 8; ++i) c("ram1[" + std::to_string(r) + "][" + std::to_string(i) + "]", pcm_ram1(R, r, i), pm.ram1[r][i]);
            for (int i = 0; i < 16; ++i) c("ram2[" + std::to_string(r) + "][" + std::to_string(i) + "]", pcm_ram2(R, r, i), pm.ram2[r][i]);
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
            for (int i = 0; i < 0x4000; ++i) c("eram[" + std::to_string(i) + "]", R->sc55_board__DOT__core__DOT__pcm__DOT__u_eram__DOT__mem[i], pm.eram[i]);
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
            for (int i = 0; i < 16; ++i) fprintf(stderr, " %x", pcm_ram2(R, 23, i));
            fprintf(stderr, "\n");
            return false;
        }
        if (force && !serial) fprintf(stderr, "PCM state identical at step %" PRIu64 "\n", s);
        return true;
    };
    for (uint64_t s = 0; s < steps; ++s)
    {
        cur_step = s;
        if (!midi.empty() && s >= 4'000'000 && (s % 600) == 0 && pos < midi.size())
        {
            uint8_t b = midi[pos++];
            emu.PostMIDI(b);
            t->midi_inject = 1; t->midi_inject_data = b;
            tick();
            t->midi_inject = 0;
        }
        else if (rx_baud)
            while (!sched.empty() && sched.front().first <= s) { line_q.push_back(sched.front().second); sched.pop_front(); }
        else if (!sched.empty() && sched.front().first <= s && (s % 600) == 0)
        {
            uint8_t b = sched.front().second;
            sched.pop_front();
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
        // --rx-baud: the HDL steps first; the model gets the bytes the HDL MCU read in this step
        if (!rx_baud) emu.Step();
        last_pcm.clear();
        uint64_t c0 = clocks;
        const uint32_t rp0 = t->rootp->sc55_board__DOT__core__DOT__mcu__DOT__rx_rp;
        t->step_gate = 1;
        tick();
        t->step_gate = 0;
        for (int g = 0; g < 50'000'000 && !t->step_done; ++g) tick();
        const bool step_finished = t->step_done;
        if (rx_baud)
        {
            for (uint32_t k = (t->rootp->sc55_board__DOT__core__DOT__mcu__DOT__rx_rp - rp0) & 2047; k > 0; --k)
            {
                if (hdl_rx.empty()) { fprintf(stderr, "step %" PRIu64 ": the HDL MCU read a byte the bench did not see arrive\n", s); return 1; }
                emu.PostMIDI(hdl_rx.front());
                hdl_rx.pop_front();
            }
            emu.Step();
            while ((int64_t)(clocks - c0) < step_clocks) tick();
        }
        max_step = std::max(max_step, clocks - c0);
        // mk2: the sub-MCU runs this step's SM_Update alongside the next step; let it finish before comparing
        if (!m.is_mk1) for (int g = 0; g < 10000 && !t->rootp->sc55_board__DOT__core__DOT__mcu__DOT__sm_caught_up; ++g) tick();
        run_clocks += clocks - c0;
        {
            auto* R = t->rootp;
            std::string d;
            if ((m.is_mk1 ? R->sc55_board__DOT__core__DOT__mcu__DOT__uart_rx_byte : R->sc55_board__DOT__core__DOT__mcu__DOT__sm__DOT__rx_byte) != m.uart_rx_byte) d += "uart_rx_byte ";
            uint32_t hc = (uint32_t)(R->sc55_board__DOT__core__DOT__mcu__DOT__rx_wp - R->sc55_board__DOT__core__DOT__mcu__DOT__rx_rp) & 2047;
            const uint32_t rc = (m.uart_write_ptr - m.uart_read_ptr) % uart_buffer_size + (uint32_t)hdl_rx.size();
            if (hc != rc) d += "rx_count(hdl " + std::to_string(hc) + " ref " + std::to_string(rc) + ") ";
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
        if (!step_finished || t->dbg_pc != m.pc || t->dbg_cp != m.cp)
        {
            fprintf(stderr, "MISMATCH at step %" PRIu64 ": HDL %02x:%04x (done %d), model %02x:%04x\n", s, t->dbg_cp, t->dbg_pc,
                    (int)step_finished, m.cp, m.pc);
            {
                auto* R = t->rootp;
                fprintf(stderr, "  mcu q=%d cpu state=%d c_req=%d sm_wait=%d sm_caught_up=%d sm q=%d sm cycles=%" PRIu64 " tgt5=%" PRIu64 " rom2x_req=%d cache st=%d ln_req=%d mk2=%d model=%d running=%d\n",
                        (int)R->sc55_board__DOT__core__DOT__mcu__DOT__q, (int)R->sc55_board__DOT__core__DOT__mcu__DOT__cpu__DOT__state,
                        (int)R->sc55_board__DOT__core__DOT__mcu__DOT__c_req, (int)R->sc55_board__DOT__core__DOT__mcu__DOT__sm_wait,
                        (int)R->sc55_board__DOT__core__DOT__mcu__DOT__sm_caught_up, (int)R->sc55_board__DOT__core__DOT__mcu__DOT__sm__DOT__q,
                        (uint64_t)R->sc55_board__DOT__core__DOT__mcu__DOT__sm__DOT__cycles, (uint64_t)R->sc55_board__DOT__core__DOT__mcu__DOT__sm_tgt5,
                        (int)R->sc55_board__DOT__core__DOT__mcu__DOT__rom2x_req, (int)R->sc55_board__DOT__cache__DOT__st, (int)R->sc55_board__DOT__ln_req,
                        (int)R->sc55_board__DOT__mk2, (int)R->sc55_board__DOT__model, (int)t->running);
            }
            return 1;
        }
        if (!drain()) { pcm_check(s, true); return 1; }
        // PCM state, whenever the HDL PCM has caught up with the model
        if (!pcm_check(s, serial)) return 1;
    }
    // end of run: let the HDL's PCM finish the frames already due, then every model sample must have
    // been compared with an HDL one (no trailing samples left on either side)
    for (int g = 0; g < 2'000'000 && h_total + hdl.size() < m_total + g_model.size(); ++g) tick();
    if (!drain()) return 1;
    if (!mq.empty() || !hq.empty() || m_total != h_total || compared != m_total)
    {
        fprintf(stderr, "MISMATCH at the end: HDL produced %" PRIu64 " samples, model %" PRIu64 " (%" PRIu64 " compared)\n", h_total, m_total, compared);
        return 1;
    }
    if (m_total == 0) { fprintf(stderr, "no samples compared\n"); return 1; }

    // MIDI out: let the line finish, then the bytes must be the model's
    for (uint64_t g = 0; g < (tx_ref.size() - std::min(tx_ref.size(), txdec.bytes.size()) + 2) * 20 * (uint64_t)tx_bit &&
                         (txdec.bytes.size() < tx_ref.size() || !txdec.idle()); ++g)
        tick();
    {
        const unsigned dropped = t->rootp->sc55_board__DOT__midi_out__DOT__dropped;
        if (!speed_matched) { fprintf(stderr, "MIDI in: synthframe_midi_rx never matched the line's %" PRId64 " baud\n", rx_baud); return 1; }
        if (tx_skip_ref || tx_skip_line)
        {
            printf("MIDI out before the speed match (not compared): model %zu bytes, line %zu\n", tx_skip_ref, tx_skip_line);
            tx_ref.erase(tx_ref.begin(), tx_ref.begin() + tx_skip_ref);
            txdec.bytes.erase(txdec.bytes.begin(), txdec.bytes.begin() + tx_skip_line);
            txdec.errors -= tx_skip_err;
        }
        if (txdec.bytes != tx_ref || txdec.errors || dropped)
        {
            fprintf(stderr, "MIDI out: uart_txd carried %zu bytes (%d framing errors, %u dropped), model sent %zu\n",
                    txdec.bytes.size(), txdec.errors, dropped, tx_ref.size());
            for (size_t i = 0; i < std::max(txdec.bytes.size(), tx_ref.size()) && i < 64; ++i)
                fprintf(stderr, "  %zu: line %s model %s\n", i, i < txdec.bytes.size() ? std::to_string(txdec.bytes[i]).c_str() : "-",
                        i < tx_ref.size() ? std::to_string(tx_ref[i]).c_str() : "-");
            return 1;
        }
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
    // the core held still after the last step: no further samples
    if (!drain() || !hq.empty() || m_total != h_total)
    {
        fprintf(stderr, "MISMATCH: HDL produced %" PRIu64 " samples after the last step, model %" PRIu64 "\n", h_total, m_total);
        return 1;
    }
    printf("LCD frame identical (%" PRIu64 " pixels, %" PRIu64 " black); text: ", lcd_pixels, lcd_lit);
    for (int i = 0; i < 80; ++i) { uint8_t c = emu.GetLCD().LCD_Data[i]; putchar(c >= 32 && c < 127 ? c : '.'); }
    printf("\n");
    printf("OK: pack loaded in %" PRIu64 " clocks, %" PRIu64 " steps, %" PRIu64 " samples identical (peak %.1f dBFS), "
           "%.1f clocks/step (max %" PRIu64 "), %" PRIu64 " DDR3 reads, MIDI bytes %zu, MIDI out bytes %zu on uart_txd\n",
           load_clocks, steps, compared, peak > 0 ? 20 * std::log10((double)peak / 2147483648.0) : -999.0,
           (double)run_clocks / (double)steps, max_step, ddr_reads, pos, tx_ref.size());
    if (rx_baud)
        printf("serial MIDI in at %" PRId64 " baud: %" PRIu64 " bytes on uart_rxd, %" PRIu64 " to the MCU, speed %s; MIDI out at %" PRId64 " baud:%s\n",
               rx_baud, rx_line_bytes, rx_hdl_bytes, t->midi_alt ? "alt" : "standard", rx_baud,
               [&] { std::string o; for (uint8_t b : txdec.bytes) { char h[4]; snprintf(h, sizeof h, " %02x", b); o += h; } return o; }().c_str());
    delete t;
    return 0;
}
