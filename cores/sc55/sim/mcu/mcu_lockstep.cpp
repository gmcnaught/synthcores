// MCU system lockstep (M2): modules/roland/hdl/mcu/sc55_mcu.sv (CPU + memories + peripherals)
// against the reference model, in one process.
//
// The HDL serves its own memories and devices. The bench supplies only what
// belongs to the PCM chip (still the model's in M2): the data of PCM register
// reads and the IRQ0 request level. Every CPU bus access must equal the
// model's (direction, address, and data for reads and writes); after each
// step CPU, interrupt, device-register, timer, UART, A/D and gate-array state
// must equal the model's.
//
// Usage: mcu_lockstep --roms DIR [--romset mk1-v1.21] [--steps N] [--midi-demo SEED]
//                     [--buttons SEED]
//        JV-880 (--romset jv880-v1.0.1): [--nvram FILE] loads the NVRAM into both;
//        [--jv880-factory] plays the factory-preset key presses (cores/sc55/ref/jv880_nvram.cpp:
//        UTILITY, nine encoder detents, ENTER twice) from 6 s; [--nvram-out FILE] writes the HDL's
//        NVRAM at the end, after checking it equals the model's.
#include "Vsc55_mcu.h"
#include "Vsc55_mcu___024root.h"
#include "verilated.h"

#include "emu.h"
#include "mcu.h"
#include "mcu_timer.h"
#include "pcm.h"
#include "submcu.h"
#include "ref_hooks.h"
#include "rom_loader.h"
#include "../common/midi_demo.h"

#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <fstream>
#include <map>
#include <random>
#include <string>
#include <vector>

struct Access
{
    bool     write;
    uint32_t addr;
    uint8_t  data;
};
static std::vector<Access> g_rec;
static bool                g_recording = false;
static void bus_hook(void*, bool write, uint32_t addr, uint8_t data)
{
    if (g_recording) g_rec.push_back({write, addr & 0xffffff, data});
}
static void cpu_done_hook(void*) { g_recording = false; }

static std::string acc_str(const Access& a)
{
    char b[48];
    snprintf(b, sizeof b, "%s %06x %02x", a.write ? "W" : "R", a.addr, a.data);
    return b;
}

int main(int argc, char** argv)
{
    std::string rom_dir, romset = "mk1-v1.21";
    uint64_t    steps = 2'000'000;
    int64_t     midi_seed = -1, button_seed = -1;
    std::string nvram_in, nvram_out;
    bool        jv_factory = false;
    uint64_t    trace_from = ~0ull;
    for (int i = 1; i < argc; ++i)
    {
        std::string a = argv[i];
        if (a == "--roms" && i + 1 < argc) rom_dir = argv[++i];
        else if (a == "--romset" && i + 1 < argc) romset = argv[++i];
        else if (a == "--steps" && i + 1 < argc) steps = std::strtoull(argv[++i], nullptr, 0);
        else if (a == "--midi-demo" && i + 1 < argc) midi_seed = std::strtoll(argv[++i], nullptr, 0);
        else if (a == "--buttons" && i + 1 < argc) button_seed = std::strtoll(argv[++i], nullptr, 0);
        else if (a == "--nvram" && i + 1 < argc) nvram_in = argv[++i];
        else if (a == "--nvram-out" && i + 1 < argc) nvram_out = argv[++i];
        else if (a == "--jv880-factory") jv_factory = true;
        else if (a == "--trace-from" && i + 1 < argc) trace_from = std::strtoull(argv[++i], nullptr, 0);
        else { fprintf(stderr, "unknown argument %s\n", a.c_str()); return 2; }
    }

    common::LoadRomsetResult load;
    common::RomOverrides     overrides{};
    if (auto err = common::LoadRomset(rom_dir, romset, common::RomLoader::Hashing, overrides, load);
        err != common::LoadRomsetError{})
    {
        fprintf(stderr, "romset load failed: %s\n", common::ToCString(err));
        return 2;
    }
    Emulator emu;
    emu.Init({.lcd_backend = nullptr});
    if (!emu.LoadRoms(load.romset, load.romset_info)) { fprintf(stderr, "LoadRoms failed\n"); return 2; }
    mcu_t&       m  = emu.GetMCU();
    mcu_timer_t& tm = *m.timer;
    const bool jv  = m.is_jv880;
    const bool mk2 = !m.is_mk1 && !jv;
    if (m.is_scb55 || m.is_cm300) { fprintf(stderr, "only SC-55 / SC-155 (mk1, mk2) and JV-880 romsets\n"); return 2; }
    pcm_t& pcm = emu.GetPCM();
    if (!nvram_in.empty())
    {
        std::ifstream f(nvram_in, std::ios::binary);
        if (!f.read((char*)m.nvram, NVRAM_SIZE)) { fprintf(stderr, "cannot read %s\n", nvram_in.c_str()); return 2; }
    }

    VerilatedContext ctx;
    ctx.commandArgs(argc, argv);
    Vsc55_mcu* t = new Vsc55_mcu{&ctx};
    auto*      R = t->rootp;
    uint64_t   clocks = 0;
    auto tick = [&]() { t->clk = 0; t->eval(); t->clk = 1; t->eval(); ++clocks; };

    t->rst = 1; t->run_step = 0; t->reset_start = 0; t->ld_we = 0; t->midi_valid = 0;
    t->mk2 = mk2; t->sc155 = m.is_sc155; t->jv880 = jv; t->enc_we = 0; t->enc_dir = 0; t->rom2x_ack = 0;
    t->buttons = 0; t->pcm_ack = 0; t->pcm_irq0 = 0; t->pcm_irq_known = 1; t->cpu_ld_en = 0;
    for (int i = 0; i < 4; ++i) tick();
    t->rst = 0;
    for (int g = 0; g < 40000 && !t->ready; ++g) tick();

    // memories: ROMs from the model, RAMs as the model's (zero)
    auto load_mem = [&](int sel, const uint8_t* src, size_t n) {
        t->ld_we = 1; t->ld_sel = sel;
        for (size_t i = 0; i < n; ++i) { t->ld_addr = (uint32_t)i; t->ld_data = src[i]; tick(); }
        t->ld_we = 0;
    };
    load_mem(0, m.rom1, sizeof m.rom1);
    load_mem(1, m.rom2, (size_t)m.rom2_mask + 1);
    if (mk2) load_mem(4, m.sm->rom, 4096);
    load_mem(2, m.sram, sizeof m.sram);
    load_mem(3, m.ram, sizeof m.ram);
    if (jv) { load_mem(5, m.nvram, NVRAM_SIZE); load_mem(6, m.cardram, CARDRAM_SIZE); }

    emu.Reset();
    t->reset_start = 1; tick(); t->reset_start = 0;
    for (int g = 0; g < 1000 && !t->reset_done; ++g) tick();
    if (!t->reset_done) { fprintf(stderr, "HDL reset did not finish\n"); return 1; }

    g_ref_hooks.bus = bus_hook;
    g_ref_hooks.cpu_done = cpu_done_hook;

    std::vector<uint8_t> midi;
    if (midi_seed >= 0) midi = make_midi_demo((uint64_t)midi_seed);
    size_t midi_pos = 0;
    std::mt19937_64 brng(button_seed >= 0 ? (uint64_t)button_seed : 0);
    uint32_t buttons = 0;

    // JV-880 factory preset (jv880_nvram.cpp's timing at 600 ns a step): step -> action
    // (b<n>: button n down, u: all up, e: encoder +1)
    std::map<uint64_t, std::string> script;
    if (jv_factory)
    {
        const uint64_t ms = 1'000'000 / 600;
        uint64_t at = 6000 * ms;
        auto press = [&](int b, uint64_t after) { script[at] = "b" + std::to_string(b); at += 100 * ms; script[at] = "u"; at += after * ms; };
        press(MCU_BUTTON_UTILITY, 400);
        for (int i = 0; i < 9; ++i) { script[at] = "e"; at += 300 * ms; }
        press(MCU_BUTTON_ENTER, 400);
        press(MCU_BUTTON_ENTER, 3000);
        if (steps < at) steps = at;
    }

    std::deque<std::string> hist;
    auto fail = [&](const std::string& what) {
        fprintf(stderr, "MISMATCH: %s\nlast steps:\n", what.c_str());
        for (auto& h : hist) fprintf(stderr, "  %s\n", h.c_str());
        return 1;
    };

    uint64_t n_acc = 0, n_pcm = 0;
    for (uint64_t s = 0; s < steps; ++s)
    {
        if (!midi.empty() && s >= 4'000'000 && (s % 600) == 0 && midi_pos < midi.size() && !t->midi_full)
        {
            uint8_t b = midi[midi_pos++];
            emu.PostMIDI(b);
            t->midi_data = b; t->midi_valid = 1;
        }
        if (button_seed >= 0 && (s % 50000) == 0 && s >= 4'000'000)
        {
            buttons = (brng() % 3 == 0) ? (1u << (1 + brng() % 22)) : 0;   // never POWER (bit 0): it puts the unit in standby
            m.button_pressed = buttons;
        }
        if (auto it = script.find(s); it != script.end())
        {
            const std::string& act = it->second;
            if (act[0] == 'b') buttons |= 1u << std::stoi(act.substr(1));
            else if (act == "u") buttons = 0;
            else if (act == "e")
            {
                MCU_EncoderTrigger(m, 1);
                t->enc_we = 1; t->enc_dir = 1; tick(); t->enc_we = 0;
            }
            m.button_pressed = buttons;
        }
        t->buttons = buttons;

        uint32_t pend0 = 0;
        for (int i = 0; i < INTERRUPT_SOURCE_MAX; ++i)
            if (m.interrupt_pending.Contains((MCU_Interrupt_Source)i)) pend0 |= 1u << i;
        // JV-880: the PCM's own request (pcm.irq_assert) goes to gate-array line 5; it can fall in
        // the step's status read and rise in its PCM update (set at the HDL's pcm_tick below)
        t->pcm_irq0 = jv ? pcm.irq_assert : (pend0 >> INTERRUPT_SOURCE_IRQ0) & 1;
        const bool jv_irq_before = pcm.irq_assert;
        const uint16_t pc0 = m.pc;
        const uint8_t  cp0 = m.cp;

        g_rec.clear();
        g_recording = true;
        emu.Step();
        g_recording = false;
        const uint32_t irq0_after = m.interrupt_pending.Contains(INTERRUPT_SOURCE_IRQ0);
        const bool jv_irq_after = pcm.irq_assert;

        size_t k = 0;
        t->run_step = 1;
        tick();
        t->run_step = 0;
        t->midi_valid = 0;
        bool done = t->step_done;
        bool pcm_acked = false;
        for (int g = 0; !done && g < 20000; ++g)
        {
            // PCM: answer from the model's record
            t->pcm_ack = 0;
            if (t->pcm_req && !pcm_acked)
            {
                if (k >= g_rec.size() || (g_rec[k].addr & ((mk2 || jv) ? 0xfffc00u : 0xffffc0u)) != (jv ? 0xf000u : 0xe000u) || g_rec[k].write != (bool)t->pcm_we ||
                    (g_rec[k].addr & 0x3f) != t->pcm_addr)
                {
                    char msg[160];
                    snprintf(msg, sizeof msg, "step %" PRIu64 " %02x:%04x: HDL PCM access %s %02x not expected (model next: %s)",
                             s, cp0, pc0, t->pcm_we ? "W" : "R", t->pcm_addr, k < g_rec.size() ? acc_str(g_rec[k]).c_str() : "none");
                    return fail(msg);
                }
                t->pcm_rdata = g_rec[k].data;
                t->pcm_ack = 1;
                // JV-880: reading the status (3e) clears an asserted request (MCU_GA_SetGAInt(5, 0))
                if (jv && jv_irq_before && !t->pcm_we && t->pcm_addr == 0x3e) t->pcm_irq0 = 0;
                pcm_acked = true;
                ++n_pcm;
            }
            if (!t->pcm_req) pcm_acked = false;
            tick();
            if (jv && t->pcm_tick) t->pcm_irq0 = jv_irq_after;   // PCM_Update after the instruction
            if (t->mon_ack)
            {
                if (k >= g_rec.size())
                    return fail("step " + std::to_string(s) + ": HDL made an extra access");
                const Access& a = g_rec[k];
                uint8_t d = t->mon_we ? t->mon_wdata : t->mon_rdata;
                if ((bool)t->mon_we != a.write || t->mon_addr != a.addr || d != a.data)
                {
                    char msg[200];
                    snprintf(msg, sizeof msg, "step %" PRIu64 " %02x:%04x access %zu: HDL %s %06x %02x, model %s",
                             s, cp0, pc0, k, t->mon_we ? "W" : "R", t->mon_addr, d, acc_str(a).c_str());
                    return fail(msg);
                }
                ++k;
            }
            done = t->step_done;
        }
        t->pcm_ack = 0;
        if (mk2) for (int g = 0; g < 4000 && !R->sc55_mcu__DOT__sm_caught_up; ++g) tick();   // the sub-MCU runs alongside the next step
        n_acc += k;
        if (!done) return fail("HDL step did not finish");
        if (k != g_rec.size())
            return fail("step " + std::to_string(s) + ": HDL made " + std::to_string(k) + " accesses, model " + std::to_string(g_rec.size()));

        char line[160];
        snprintf(line, sizeof line, "step %" PRIu64 " %02x:%04x -> %02x:%04x acc=%zu pend=%06x", s, cp0, pc0, m.cp, m.pc, g_rec.size(), pend0);
        hist.push_back(line);
        if (s >= trace_from)
            fprintf(stderr, "%s | adcsr hdl=%02x ref=%02x end hdl=%" PRIu64 " ref=%" PRIu64 " cyc=%" PRIu64 " d66 hdl=%02x ref=%02x\n", line,
                    R->sc55_mcu__DOT__dev[0x68], m.dev_register[0x68], (uint64_t)R->sc55_mcu__DOT__analog_end, m.analog_end_time,
                    m.cycles, R->sc55_mcu__DOT__dev[0x66], m.dev_register[0x66]);
        if (hist.size() > 16) hist.pop_front();

        // ---- state comparison
        std::string diff;
        auto cmp = [&](const char* name, uint64_t h, uint64_t r) {
            if (h != r) { char b[96]; snprintf(b, sizeof b, "%s hdl=%" PRIx64 " ref=%" PRIx64 "; ", name, h, r); diff += b; }
        };
        for (int i = 0; i < 8; ++i) cmp(("r" + std::to_string(i)).c_str(), t->dbg_r[i], m.r[i]);
        cmp("pc", t->dbg_pc, m.pc); cmp("sr", t->dbg_sr, m.sr); cmp("cp", t->dbg_cp, m.cp);
        cmp("dp", t->dbg_dp, m.dp); cmp("ep", t->dbg_ep, m.ep); cmp("tp", t->dbg_tp, m.tp); cmp("br", t->dbg_br, m.br);
        cmp("sleep", t->dbg_sleep, m.sleep); cmp("ex_ignore", t->dbg_exign, m.ex_ignore);
        {
            int8_t e = (int8_t)m.exception_pending;
            cmp("exception", (t->dbg_exc & 4) ? t->dbg_exc : 0, e < 0 ? 0 : (4u | (uint32_t)e));
        }
        uint32_t pend = 0;
        for (int i = 0; i < INTERRUPT_SOURCE_MAX; ++i)
            if (m.interrupt_pending.Contains((MCU_Interrupt_Source)i)) pend |= 1u << i;
        // IRQ0 is the PCM's; the HDL sees it from the next step on
        cmp("int_pending", t->dbg_int & ~2u, pend & ~2u);
        (void)irq0_after;
        for (int a = 0; a < 0x80; ++a)
        {
            if ((a >= 0x10 && a < 0x40) || (a >= 0x50 && a < 0x55)) continue;   // timers keep their own state
            if (R->sc55_mcu__DOT__dev[a] != m.dev_register[a]) { char b[48]; snprintf(b, sizeof b, "dev[%02x]", a); cmp(b, R->sc55_mcu__DOT__dev[a], m.dev_register[a]); }
        }
        for (int i = 0; i < 3; ++i)
        {
            std::string p = "frt" + std::to_string(i) + ".";
            cmp((p + "tcr").c_str(), R->sc55_mcu__DOT__frt_tcr[i], tm.frt[i].tcr);
            cmp((p + "tcsr").c_str(), R->sc55_mcu__DOT__frt_tcsr[i], tm.frt[i].tcsr);
            cmp((p + "frc").c_str(), R->sc55_mcu__DOT__frt_frc[i], tm.frt[i].frc);
            cmp((p + "ocra").c_str(), R->sc55_mcu__DOT__frt_ocra[i], tm.frt[i].ocra);
            cmp((p + "ocrb").c_str(), R->sc55_mcu__DOT__frt_ocrb[i], tm.frt[i].ocrb);
            cmp((p + "icr").c_str(), R->sc55_mcu__DOT__frt_icr[i], tm.frt[i].icr);
            cmp((p + "status_rd").c_str(), R->sc55_mcu__DOT__frt_status_rd[i], tm.frt[i].status_rd);
            cmp((p + "deadline").c_str(), R->sc55_mcu__DOT__frt_deadline[i], tm.frt[i].deadline);
            cmp((p + "stride").c_str(), R->sc55_mcu__DOT__frt_stride[i], tm.frt[i].stride);
        }
        cmp("tmr.tcr", R->sc55_mcu__DOT__tmr_tcr, tm.tmr.tcr);
        cmp("tmr.tcsr", R->sc55_mcu__DOT__tmr_tcsr, tm.tmr.tcsr);
        cmp("tmr.tcora", R->sc55_mcu__DOT__tmr_tcora, tm.tmr.tcora);
        cmp("tmr.tcorb", R->sc55_mcu__DOT__tmr_tcorb, tm.tmr.tcorb);
        cmp("tmr.tcnt", R->sc55_mcu__DOT__tmr_tcnt, tm.tmr.tcnt);
        cmp("tmr.status_rd", R->sc55_mcu__DOT__tmr_status_rd, tm.tmr.status_rd);
        cmp("tmr.deadline", R->sc55_mcu__DOT__tmr_deadline, tm.tmr.deadline & 0xffffffffffffull);
        cmp("tmr.stride", R->sc55_mcu__DOT__tmr_stride, tm.tmr.stride);
        cmp("tempreg", R->sc55_mcu__DOT__tempreg, tm.tempreg);
        if (!mk2)
        {
            cmp("uart_rx_byte", R->sc55_mcu__DOT__uart_rx_byte, m.uart_rx_byte);
            cmp("uart_rx_delay", R->sc55_mcu__DOT__uart_rx_delay, m.uart_rx_delay);
        }
        else
        {
            // the sub-MCU (shared uart_rx_byte / uart_rx_delay)
            submcu_t& sm = *m.sm;
            cmp("uart_rx_byte", R->sc55_mcu__DOT__sm__DOT__rx_byte, m.uart_rx_byte);
            cmp("uart_rx_delay", R->sc55_mcu__DOT__sm__DOT__rx_delay, m.uart_rx_delay);
            cmp("sm.pc", R->sc55_mcu__DOT__sm__DOT__pc, sm.pc);
            cmp("sm.a", R->sc55_mcu__DOT__sm__DOT__a, sm.a);
            cmp("sm.x", R->sc55_mcu__DOT__sm__DOT__x, sm.x);
            cmp("sm.y", R->sc55_mcu__DOT__sm__DOT__y, sm.y);
            cmp("sm.s", R->sc55_mcu__DOT__sm__DOT__s, sm.s);
            cmp("sm.sr", R->sc55_mcu__DOT__sm__DOT__sr, sm.sr);
            cmp("sm.cycles", R->sc55_mcu__DOT__sm__DOT__cycles, sm.cycles);
            cmp("sm.sleep", R->sc55_mcu__DOT__sm__DOT__sleep, sm.sleep);
            cmp("sm.timer_cycles", R->sc55_mcu__DOT__sm__DOT__timer_cycles, sm.timer_cycles);
            cmp("sm.gotbyte", R->sc55_mcu__DOT__sm__DOT__gotbyte, sm.uart_rx_gotbyte);
            cmp("p0_data", R->sc55_mcu__DOT__sm__DOT__p0_data, m.p0_data);
            for (int i = 0; i < 32; ++i) if (R->sc55_mcu__DOT__sm__DOT__dm[i] != sm.device_mode[i]) cmp(("sm.dev" + std::to_string(i)).c_str(), R->sc55_mcu__DOT__sm__DOT__dm[i], sm.device_mode[i]);
            for (int i = 0; i < 128; ++i) if (R->sc55_mcu__DOT__sm__DOT__u_ram__DOT__mem[i] != sm.ram[i]) cmp(("sm.ram" + std::to_string(i)).c_str(), R->sc55_mcu__DOT__sm__DOT__u_ram__DOT__mem[i], sm.ram[i]);
            for (int i = 0; i < 192; ++i) if (R->sc55_mcu__DOT__sm__DOT__u_shared__DOT__mem[i] != sm.shared_ram[i]) cmp(("sm.shared" + std::to_string(i)).c_str(), R->sc55_mcu__DOT__sm__DOT__u_shared__DOT__mem[i], sm.shared_ram[i]);
        }
        if (mk2 || jv)
        {
            for (int l = 3; l <= 5; ++l)
                cmp(("ga_int" + std::to_string(l)).c_str(), (R->sc55_mcu__DOT__ga_line >> l) & 1, (uint32_t)m.ga_int[l]);
            cmp("ga_enable", R->sc55_mcu__DOT__ga_en, m.ga_int_enable);
        }
        cmp("uart_tx_delay", R->sc55_mcu__DOT__uart_tx_delay, m.uart_tx_delay);
        cmp("ssr_rd", R->sc55_mcu__DOT__ssr_rd, (uint8_t)m.ssr_rd);
        cmp("rx_count", (uint32_t)(R->sc55_mcu__DOT__rx_wp - R->sc55_mcu__DOT__rx_rp) & 2047,
            (m.uart_write_ptr - m.uart_read_ptr) % uart_buffer_size);
        cmp("analog_end", R->sc55_mcu__DOT__analog_end, m.analog_end_time);
        cmp("adf_rd", R->sc55_mcu__DOT__adf_rd, (uint32_t)m.adf_rd);
        cmp("ga_lcd_counter", R->sc55_mcu__DOT__ga_lcd_counter, (uint32_t)m.ga_lcd_counter);
        cmp("ga_trigger", R->sc55_mcu__DOT__ga_trigger, m.ga_int_trigger);
        cmp("io_sd", R->sc55_mcu__DOT__io_sd, m.io_sd);
        cmp("cycles", t->cycles, m.cycles);
        if (!diff.empty())
            return fail("step " + std::to_string(s) + ": " + diff);
    }

    // memories at the end
    size_t bad = 0;
    for (size_t i = 0; i < sizeof m.sram; ++i) bad += R->sc55_mcu__DOT__u_sram__DOT__mem[i] != m.sram[i];
    for (size_t i = 0; i < sizeof m.ram; ++i) bad += R->sc55_mcu__DOT__u_ram__DOT__mem[i] != m.ram[i];
    if (bad) return fail(std::to_string(bad) + " SRAM/RAM bytes differ at the end");

    if (jv)
    {
        // the NVRAM (factory preset etc.): HDL = model, byte for byte; optionally saved
        for (int i = 0; i < NVRAM_SIZE; ++i)
            if (R->sc55_mcu__DOT__g_jv880_rams__DOT__u_nvram__DOT__mem[i] != m.nvram[i])
            {
                fprintf(stderr, "MISMATCH: NVRAM byte %04x: HDL %02x, model %02x\n", i,
                        R->sc55_mcu__DOT__g_jv880_rams__DOT__u_nvram__DOT__mem[i], m.nvram[i]);
                return 1;
            }
        if (!nvram_out.empty())
        {
            std::ofstream f(nvram_out, std::ios::binary);
            for (int i = 0; i < NVRAM_SIZE; ++i) f.put((char)R->sc55_mcu__DOT__g_jv880_rams__DOT__u_nvram__DOT__mem[i]);
        }
        printf("NVRAM: HDL = model (%d bytes)\n", NVRAM_SIZE);
    }
    printf("OK (%s): %" PRIu64 " steps, %" PRIu64 " bus accesses (%" PRIu64 " PCM), %" PRIu64 " HDL clocks (%.1f/step), MIDI bytes %zu, ended at %02x:%04x\n",
           romset.c_str(), steps, n_acc, n_pcm, clocks, (double)clocks / (double)steps, midi_pos, m.cp, m.pc);
    delete t;
    return 0;
}
