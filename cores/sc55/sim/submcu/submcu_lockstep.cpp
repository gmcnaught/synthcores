// Sub-MCU lockstep: hdl/submcu/sc55_submcu.sv against the reference's
// submcu.cpp, in one process (SC-55mk2 romset).
//
// Per MCU step of the model: MIDI bytes posted before the step go to both;
// the main MCU's accesses to the sub-MCU window (page 0, EC00-EFFF) during
// the step are recorded and replayed into the HDL's sys port (reads must
// return the model's data); then the HDL runs to the model's cycle count
// (SM_Update) and all sub-MCU state is compared: registers, cycles, sleep,
// RAM, shared RAM, access bits, device registers, ports, timer, UART.
//
// Usage: submcu_lockstep --roms DIR [--romset mk2-v1.01] [--steps N]
//                        [--midi-demo SEED] [--buttons SEED] [--random SEED]
//   --random: random code in rom_sm (implemented opcodes mostly, some
//   unimplemented), all vectors into it
#include "Vsc55_submcu.h"
#include "Vsc55_submcu___024root.h"
#include "verilated.h"

#include "emu.h"
#include "mcu.h"
#include "submcu.h"
#include "rom_loader.h"
#include "ref_hooks.h"
#include "../common/midi_demo.h"

#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <random>
#include <string>
#include <vector>

struct Access
{
    bool     write;
    uint8_t  addr;
    uint8_t  data;
};
static std::vector<Access> g_rec;
static bool g_recording = false;

static void bus_hook(void*, bool write, uint32_t addr, uint8_t data)
{
    if (!g_recording) return;
    uint32_t page = (addr >> 16) & 0xf;
    uint32_t a16  = addr & 0xffff;
    if (page == 0 && a16 >= 0xec00 && a16 < 0xf000)
        g_rec.push_back({write, (uint8_t)(a16 & 0xff), data});
}

int main(int argc, char** argv)
{
    std::string rom_dir, romset = "mk2-v1.01";
    uint64_t    steps = 2'000'000;
    int64_t     midi_seed = -1, button_seed = -1, random_seed = -1;
    for (int i = 1; i < argc; ++i)
    {
        std::string a = argv[i];
        if (a == "--roms" && i + 1 < argc) rom_dir = argv[++i];
        else if (a == "--romset" && i + 1 < argc) romset = argv[++i];
        else if (a == "--steps" && i + 1 < argc) steps = std::strtoull(argv[++i], nullptr, 0);
        else if (a == "--midi-demo" && i + 1 < argc) midi_seed = std::strtoll(argv[++i], nullptr, 0);
        else if (a == "--buttons" && i + 1 < argc) button_seed = std::strtoll(argv[++i], nullptr, 0);
        else if (a == "--random" && i + 1 < argc) random_seed = std::strtoll(argv[++i], nullptr, 0);
        else { fprintf(stderr, "unknown argument %s\n", a.c_str()); return 2; }
    }
    if (rom_dir.empty()) { fprintf(stderr, "--roms DIR required\n"); return 2; }

    common::LoadRomsetResult load;
    common::RomOverrides     ov{};
    if (auto err = common::LoadRomset(rom_dir, romset, common::RomLoader::Hashing, ov, load); err != common::LoadRomsetError{})
    {
        fprintf(stderr, "romset load failed: %s\n", common::ToCString(err));
        return 2;
    }
    Emulator emu;
    emu.Init({.lcd_backend = nullptr});
    if (!emu.LoadRoms(load.romset, load.romset_info)) { fprintf(stderr, "LoadRoms failed\n"); return 2; }
    mcu_t&    m  = emu.GetMCU();
    submcu_t& sm = *m.sm;
    if (m.is_mk1) { fprintf(stderr, "%s has no sub-MCU\n", romset.c_str()); return 2; }

    std::mt19937_64 rng(random_seed >= 0 ? (uint64_t)random_seed : 1);
    if (random_seed >= 0)
    {
        // random code: mostly implemented opcodes with random operands; vectors into the ROM
        static const uint8_t ops[] = {
            0x01,0x05,0x09,0x0d,0x11,0x15,0x19,0x1d,0x21,0x25,0x29,0x2d,0x31,0x35,0x39,0x3d,
            0xa1,0xa5,0xa9,0xad,0xb1,0xb5,0xb9,0xbd,0xc1,0xc5,0xc9,0xcd,0xd1,0xd5,0xd9,0xdd,
            0x81,0x85,0x8d,0x91,0x95,0x99,0x9d,0xa2,0xa6,0xae,0xb6,0xbe,0xa0,0xa4,0xac,0xb4,0xbc,
            0x86,0x96,0x8e,0x84,0x94,0x8c,0xe0,0xe4,0xec,0xc0,0xc4,0xcc,0x3a,0xe6,0xf6,0xee,0xfe,
            0x1a,0xc6,0xd6,0xce,0xde,0x3c,0x10,0x80,0x90,0xb0,0xd0,0xf0,0x20,0x02,0x22,0x4c,0x6c,0xb2,
            0x60,0x40,0x48,0x68,0x42,0xea,0x78,0x58,0xd8,0x12,0x38,0x18,0x9a,0x8a,0xaa,0xe8,0xc8,
            0x03,0x07,0x0b,0x0f,0x63,0xa7,0xeb,0xef,0x00,0x32,0x69};
        for (int i = 0; i < 0xfec;)
        {
            sm.rom[i++] = ops[rng() % sizeof ops];
            int n = (int)(rng() % 3);
            for (int j = 0; j < n && i < 0xfec; ++j)
            {
                uint32_t k = (uint32_t)(rng() % 8);
                // operands: zero page / device / shared / ROM addresses, small displacements
                sm.rom[i++] = k < 3 ? (uint8_t)(rng() % 0x80) : k < 4 ? (uint8_t)(0xe0 + rng() % 0x20)
                            : k < 5 ? (uint8_t)(0x10 + rng() % 0x0f) : (uint8_t)rng();
            }
        }
        for (int v = 0; v < 10; ++v)
        {
            uint16_t tgt = (uint16_t)(0x1000 + rng() % 0xfe0);
            sm.rom[0xfec + v * 2] = (uint8_t)tgt;
            sm.rom[0xfec + v * 2 + 1] = (uint8_t)(tgt >> 8);
        }
    }
    emu.Reset();

    std::vector<uint8_t> midi;
    if (midi_seed >= 0) midi = make_midi_demo((uint64_t)midi_seed);
    g_ref_hooks.bus = bus_hook;

    VerilatedContext ctx;
    ctx.commandArgs(argc, argv);
    auto* t = new Vsc55_submcu{&ctx};
    auto* R = t->rootp;
    uint64_t clocks = 0;
    auto tick = [&]() { t->clk = 0; t->eval(); t->clk = 1; t->eval(); ++clocks; };
    std::deque<uint8_t> fifo;
    uint64_t hdl_pops = 0;
    bool     ga5 = false;
    auto drive = [&]() {
        t->rx_avail = !fifo.empty();
        t->rx_data  = fifo.empty() ? 0 : fifo.front();
    };
    std::vector<uint64_t> cov(256, 0);
    uint32_t last_n = 0;
    auto tick_io = [&]() {
        drive();
        tick();
        if (t->st_instructions != last_n) { last_n = t->st_instructions; cov[R->sc55_submcu__DOT__opc]++; }
        if (t->rx_pop) { fifo.pop_front(); ++hdl_pops; }
        if (t->ga5_we) ga5 = t->ga5_val;
    };

    t->ld_en = 0; t->rx_delay_we = 0; t->target5 = 0;
    t->rst = 1; t->run = 0; t->reset_start = 0; t->sys_req = 0; t->ld_we = 0; t->buttons = 0;
    for (int i = 0; i < 4; ++i) tick();
    t->rst = 0;
    for (int i = 0; i < 4096; ++i) { t->ld_we = 1; t->ld_addr = i; t->ld_data = sm.rom[i]; tick(); }
    t->ld_we = 0;
    t->reset_start = 1;
    tick_io();
    t->reset_start = 0;
    for (int g = 0; g < 100 && !t->reset_done; ++g) tick_io();

    uint64_t n_sys = 0, n_reads = 0, max_clk = 0, midi_pos = 0, model_reads0 = m.uart_read_ptr;
    uint64_t model_pops = 0;
    uint32_t buttons = 0;
    std::mt19937_64 brng(button_seed >= 0 ? (uint64_t)button_seed : 0);
    const uint64_t midi_start = 3'000'000;

    auto fail = [&](uint64_t s, const char* what, uint64_t hv, uint64_t mv) {
        fprintf(stderr, "MISMATCH at step %" PRIu64 ": %s HDL %" PRIx64 " model %" PRIx64 " (sm pc %04x, mcu pc %02x:%04x)\n",
                s, what, hv, mv, sm.pc, m.cp, m.pc);
        return 1;
    };

    for (uint64_t s = 0; s < steps; ++s)
    {
        if (random_seed >= 0 && (s % 4096) == 0 && s > 0)
        {
            // new random registers in both: PC into the random code, I mostly clear, T sometimes
            sm.pc = (uint16_t)(0x1000 + rng() % 0xfe0);
            sm.a = (uint8_t)rng(); sm.x = (uint8_t)rng(); sm.y = (uint8_t)rng();
            sm.s = (uint8_t)(rng() % 2 ? 0x7f - rng() % 0x20 : rng());
            sm.sr = (uint8_t)(rng() & 0xeb);
            if (rng() % 4) sm.sr &= ~0x20;
            if (rng() % 2) sm.sr &= ~0x04;
            sm.sleep = 0;
            t->ld_pc = sm.pc; t->ld_a = sm.a; t->ld_x = sm.x; t->ld_y = sm.y; t->ld_s = sm.s; t->ld_sr = sm.sr; t->ld_sleep = 0;
            t->ld_en = 1;
            tick_io();
            t->ld_en = 0;
        }
        if (!midi.empty() && s >= midi_start && (s % 600) == 0 && midi_pos < midi.size())
        {
            emu.PostMIDI(midi[midi_pos]);
            fifo.push_back(midi[midi_pos]);
            ++midi_pos;
        }
        if (button_seed >= 0 && (s % 200000) == 0)
            buttons = (brng() % 3 == 0) ? (uint32_t)(1u << (brng() % 32)) & ~1u : 0;   // not POWER
        m.button_pressed = buttons;
        t->buttons = buttons;

        size_t rp0 = m.uart_read_ptr;
        g_rec.clear();
        g_recording = true;
        emu.Step();
        g_recording = false;
        if (m.uart_read_ptr != rp0) ++model_pops;

        uint64_t c0 = clocks;
        for (auto& acc : g_rec)
        {
            t->sys_req = 1; t->sys_we = acc.write; t->sys_addr = acc.addr; t->sys_wdata = acc.data;
            int g = 0;
            do { tick_io(); } while (!t->sys_ack && ++g < 10);
            t->sys_req = 0;
            if (!acc.write && t->sys_rdata != acc.data) return fail(s, "sys read data", t->sys_rdata, acc.data);
            ++n_sys;
            n_reads += !acc.write;
        }
        t->target5 = m.cycles * 5;
        t->run = 1;
        tick_io();
        for (int g = 0; g < 2000 && !t->caught_up; ++g) tick_io();
        if (!t->caught_up) return fail(s, "not caught up", 0, 0);
        max_clk = std::max<uint64_t>(max_clk, clocks - c0);

        // compare
        if (t->st_pc != sm.pc) return fail(s, "pc", t->st_pc, sm.pc);
        if (t->st_a != sm.a) return fail(s, "a", t->st_a, sm.a);
        if (t->st_x != sm.x) return fail(s, "x", t->st_x, sm.x);
        if (t->st_y != sm.y) return fail(s, "y", t->st_y, sm.y);
        if (t->st_s != sm.s) return fail(s, "s", t->st_s, sm.s);
        if (t->st_sr != sm.sr) return fail(s, "sr", t->st_sr, sm.sr);
        if (t->st_cycles != sm.cycles) return fail(s, "cycles", t->st_cycles, sm.cycles);
        if (t->st_sleep != sm.sleep) return fail(s, "sleep", t->st_sleep, sm.sleep);
        if (t->st_p0_data != m.p0_data) return fail(s, "p0_data", t->st_p0_data, m.p0_data);
        if (t->st_p1_data != m.p1_data) return fail(s, "p1_data", t->st_p1_data, m.p1_data);
        if (t->st_p0_dir != sm.p0_dir) return fail(s, "p0_dir", t->st_p0_dir, sm.p0_dir);
        if (t->st_p1_dir != sm.p1_dir) return fail(s, "p1_dir", t->st_p1_dir, sm.p1_dir);
        if (t->st_prescaler != sm.timer_prescaler) return fail(s, "prescaler", t->st_prescaler, sm.timer_prescaler);
        if (t->st_counter != sm.timer_counter) return fail(s, "counter", t->st_counter, sm.timer_counter);
        if (t->st_timer_cycles != sm.timer_cycles) return fail(s, "timer_cycles", t->st_timer_cycles, sm.timer_cycles);
        if (t->st_gotbyte != sm.uart_rx_gotbyte) return fail(s, "gotbyte", t->st_gotbyte, sm.uart_rx_gotbyte);
        if (t->rx_byte != m.uart_rx_byte) return fail(s, "uart_rx_byte", t->rx_byte, m.uart_rx_byte);
        if (t->st_rx_delay != m.uart_rx_delay) return fail(s, "uart_rx_delay", t->st_rx_delay, m.uart_rx_delay);
        if (hdl_pops != model_pops) return fail(s, "bytes taken", hdl_pops, model_pops);
        if (ga5 != (bool)m.ga_int[5]) return fail(s, "GA int 5", ga5, m.ga_int[5]);
        for (int i = 0; i < 128; ++i)
            if (R->sc55_submcu__DOT__u_ram__DOT__mem[i] != sm.ram[i]) return fail(s, ("ram " + std::to_string(i)).c_str(), R->sc55_submcu__DOT__u_ram__DOT__mem[i], sm.ram[i]);
        for (int i = 0; i < 192; ++i)
            if (R->sc55_submcu__DOT__u_shared__DOT__mem[i] != sm.shared_ram[i]) return fail(s, ("shared " + std::to_string(i)).c_str(), R->sc55_submcu__DOT__u_shared__DOT__mem[i], sm.shared_ram[i]);
        for (int i = 0; i < 24; ++i)
            if (R->sc55_submcu__DOT__access[i] != sm.access[i]) return fail(s, ("access " + std::to_string(i)).c_str(), R->sc55_submcu__DOT__access[i], sm.access[i]);
        for (int i = 0; i < 32; ++i)
            if (R->sc55_submcu__DOT__dm[i] != sm.device_mode[i]) return fail(s, ("device " + std::to_string(i)).c_str(), R->sc55_submcu__DOT__dm[i], sm.device_mode[i]);
    }
    (void)model_reads0;
    {
        int used = 0;
        for (int i = 0; i < 256; ++i) used += cov[i] != 0;
        printf("opcodes executed: %d of 256 values\n", used);
    }
    printf("OK: %" PRIu64 " steps, %u sub-MCU instructions, sub-MCU pc %04x, %" PRIu64 " sys accesses (%" PRIu64 " reads), "
           "MIDI bytes %" PRIu64 " (taken %" PRIu64 "), max %" PRIu64 " HDL clocks a step\n",
           steps, t->st_instructions, sm.pc, n_sys, n_reads, midi_pos, hdl_pops, max_clk);
    delete t;
    return 0;
}
