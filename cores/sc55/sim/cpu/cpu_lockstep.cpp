// CPU lockstep: hdl/cpu/h8_cpu.sv against the reference model's MCU, in one process.
//
// For every MCU_Step of the model the bench records the bytes the MCU reads
// and writes (ref hook), then runs one HDL step: each HDL bus access must be
// the next recorded access (same direction and address; same data for a
// write) and HDL reads get the recorded data. After the step all CPU state is
// compared. Devices are the model's; the HDL CPU only sees their values.
//
// Usage:
//   cpu_lockstep --roms DIR [--romset mk1-v1.21] [--steps N] [--midi FILE]
//                [--midi-demo SEED]  (GM reset + random MIDI traffic from step 4M)
//                [--random SEED]     (random code in ROM1 instead of firmware)
//   cpu_lockstep --roms none --random SEED   (no ROM files needed)
#include "Vh8_cpu.h"
#include "verilated.h"

#include "emu.h"
#include "mcu.h"
#include "rom_loader.h"
#include "ref_hooks.h"
#include "../common/midi_demo.h"

#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <fstream>
#include <random>
#include <string>
#include <vector>
#include <unordered_set>

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
    if (g_recording)
        g_rec.push_back({write, addr & 0xffffff, data});
}
static void cpu_done_hook(void*)
{
    g_recording = false;
}

struct Bench
{
    Vh8_cpu* top;
    uint64_t clocks = 0;

    void tick()
    {
        top->clk = 0;
        top->eval();
        top->clk = 1;
        top->eval();
        ++clocks;
    }
};

static std::string dump_access(const Access& a)
{
    char b[64];
    snprintf(b, sizeof b, "%s %06x %02x", a.write ? "W" : "R", a.addr, a.data);
    return b;
}

struct History
{
    std::deque<std::string> lines;
    void add(std::string s)
    {
        lines.push_back(std::move(s));
        if (lines.size() > 24)
            lines.pop_front();
    }
    void print() const
    {
        for (auto& l : lines)
            fprintf(stderr, "  %s\n", l.c_str());
    }
};

static int fail(const History& h, const char* what)
{
    fprintf(stderr, "MISMATCH: %s\nlast steps:\n", what);
    h.print();
    return 1;
}

int main(int argc, char** argv)
{
    std::string rom_dir, romset = "mk1-v1.21", midi_path;
    uint64_t    steps = 2'000'000;
    int64_t     random_seed = -1, midi_seed = -1;
    bool        verbose = false;
    for (int i = 1; i < argc; ++i)
    {
        std::string a = argv[i];
        if (a == "--roms" && i + 1 < argc) rom_dir = argv[++i];
        else if (a == "--romset" && i + 1 < argc) romset = argv[++i];
        else if (a == "--steps" && i + 1 < argc) steps = std::strtoull(argv[++i], nullptr, 0);
        else if (a == "--midi" && i + 1 < argc) midi_path = argv[++i];
        else if (a == "--random" && i + 1 < argc) random_seed = std::strtoll(argv[++i], nullptr, 0);
        else if (a == "--midi-demo" && i + 1 < argc) midi_seed = std::strtoll(argv[++i], nullptr, 0);
        else if (a == "-v") verbose = true;
        else { fprintf(stderr, "unknown argument %s\n", a.c_str()); return 2; }
    }
    if (rom_dir.empty()) { fprintf(stderr, "--roms DIR required\n"); return 2; }

    common::LoadRomsetResult load;
    Emulator                 emu;
    emu.Init({.lcd_backend = nullptr});
    if (rom_dir == "none")
    {
        // no ROM files (CI): an mk1 with empty ROMs; only useful with --random
        if (random_seed < 0) { fprintf(stderr, "--roms none needs --random\n"); return 2; }
        RomsetInfo empty{};
        emu.LoadRoms(Romset::MK1, empty);
    }
    else
    {
        common::RomOverrides overrides{};
        auto err = common::LoadRomset(rom_dir, romset, common::RomLoader::Hashing, overrides, load);
        if (err != common::LoadRomsetError{}) { fprintf(stderr, "romset load failed: %s\n", common::ToCString(err)); return 2; }
        if (!emu.LoadRoms(load.romset, load.romset_info)) { fprintf(stderr, "LoadRoms failed\n"); return 2; }
    }
    mcu_t& m = emu.GetMCU();

    std::mt19937_64 rng(random_seed >= 0 ? (uint64_t)random_seed : 0);
    if (random_seed >= 0)
    {
        // Random code: instruction-sized chunks whose first byte is drawn from the
        // real formats (general, short, branch, control), followed by random bytes.
        for (size_t i = 0x100; i < sizeof m.rom1;)
        {
            uint32_t k = (uint32_t)(rng() % 100);
            uint8_t  op;
            if (k < 45)      op = (uint8_t)(0xa0 + rng() % 0x60);                 // general Rn/@Rn/@-Rn/@Rn+/@(d,Rn)
            else if (k < 55) { static const uint8_t g[] = {0x04, 0x05, 0x0c, 0x0d, 0x15, 0x1d}; op = g[rng() % 6]; }
            else if (k < 80) op = (uint8_t)(0x40 + rng() % 0x60);                 // short formats
            else if (k < 92) op = (uint8_t)(0x20 + rng() % 0x20);                 // Bcc
            else             op = (uint8_t)(rng() % 0x20);                        // control
            m.rom1[i++] = op;
            int extra = 1 + (int)(rng() % 4);
            for (int j = 0; j < extra && i < sizeof m.rom1; ++j)
                m.rom1[i++] = (rng() % 4 == 0) ? 0x00 : (uint8_t)rng();
        }
        for (size_t i = 4; i < 0x100; ++i)
            m.rom1[i] = (uint8_t)rng();
        // vectors 1..63 into ROM1 so exceptions and traps land in random code too
        for (int v = 1; v < 64; ++v)
        {
            uint16_t target = (uint16_t)(0x0200 + (rng() % 0x7c00));
            m.rom1[v * 4 + 0] = 0;
            m.rom1[v * 4 + 1] = 0;
            m.rom1[v * 4 + 2] = (uint8_t)(target >> 8);
            m.rom1[v * 4 + 3] = (uint8_t)target;
        }
        m.rom1[0] = 0; m.rom1[1] = 0; m.rom1[2] = 0x02; m.rom1[3] = 0x00;
    }

    emu.Reset();

    std::vector<uint8_t> midi;
    if (!midi_path.empty())
    {
        std::ifstream f(midi_path, std::ios::binary);
        midi.assign(std::istreambuf_iterator<char>(f), {});
    }
    if (midi_seed >= 0)
        midi = make_midi_demo((uint64_t)midi_seed);

    g_ref_hooks.bus      = bus_hook;
    g_ref_hooks.cpu_done = cpu_done_hook;

    VerilatedContext ctx;
    ctx.commandArgs(argc, argv);
    Bench b{new Vh8_cpu{&ctx}};
    Vh8_cpu* t = b.top;
    t->rst = 1; t->step = 0; t->reset_start = 0; t->bus_ack = 0; t->ld_en = 0; t->int_hold = 0;
    for (int i = 0; i < 4; ++i) b.tick();
    t->rst = 0;
    b.tick();

    // HDL reset vector load from the model's ROM1
    t->reset_start = 1;
    for (int guard = 0; guard < 1000; ++guard)
    {
        if (t->bus_req && !t->bus_ack)
        {
            t->bus_rdata = m.rom1[t->bus_addr & 0x7fff];
            t->bus_ack   = 1;
        }
        else
            t->bus_ack = 0;
        b.tick();
        t->reset_start = 0;
        if (t->step_done) break;
    }
    t->bus_ack = 0;

    History  hist;
    uint64_t traps_ref0 = g_ref_hooks.error_traps;
    uint64_t traps_hdl0 = t->traps;
    uint64_t midi_pos = 0, n_accesses = 0, n_interrupts = 0;
    const uint64_t midi_start = 4'000'000;   // after the boot delay loops
    std::unordered_set<uint32_t> pcs;
    // coverage of general-format (mode class, size, opcode byte) and other first bytes
    std::vector<uint64_t> cov_general(9 * 2 * 256, 0), cov_other(256, 0);
    auto code_byte = [&](uint16_t pc) -> int { return (m.cp == 0 && pc < 0x8000) ? m.rom1[pc] : -1; };

    auto reseed = [&]() {
        // random architectural state in both, then continue lockstep
        for (int i = 0; i < 8; ++i) m.r[i] = (uint16_t)rng();
        if (rng() % 2) m.r[7] = (uint16_t)(0x8000 + (rng() % 0x5000) * 2);   // stack in SRAM
        for (int i = 0; i < 6; ++i)
            if (rng() % 2) m.r[i] = (uint16_t)(0x8000 + rng() % 0x6000);     // pointers into SRAM
        m.pc = (uint16_t)(0x0100 + rng() % 0x7e00);
        m.cp = 0;
        m.sr = (uint16_t)(rng() & 0x870f);
        if (rng() % 8) m.sr &= ~0x8000;                                       // trace mostly off
        m.dp = (uint8_t)(rng() % 3 == 0 ? rng() % 16 : 0);
        m.ep = (uint8_t)(rng() % 3 == 0 ? rng() % 16 : 0);
        m.tp = (uint8_t)(rng() % 3 == 0 ? rng() % 16 : 0);
        m.br = (uint8_t)(rng() % 2 ? 0xff : rng());
        m.sleep = 0;
        m.ex_ignore = 0;
        m.exception_pending = (MCU_Exception_Source)-1;
        for (int i = 0; i < 16; ++i) m.trapa_pending.Exclude(i);
        for (int i = 0; i < 8; ++i) t->ld_r[i] = m.r[i];
        t->ld_pc = m.pc; t->ld_sr = m.sr; t->ld_cp = m.cp; t->ld_dp = m.dp;
        t->ld_ep = m.ep; t->ld_tp = m.tp; t->ld_br = m.br;
        t->ld_en = 1;
        b.tick();
        t->ld_en = 0;
    };

    for (uint64_t s = 0; s < steps; ++s)
    {
        if (random_seed >= 0 && (s % 50) == 0)
            reseed();

        // feed MIDI slowly so the firmware's receive path runs
        if (!midi.empty() && s >= midi_start && (s % 600) == 0 && midi_pos < midi.size())
            emu.PostMIDI(midi[midi_pos++]);

        // inputs the HDL needs at the start of the step
        uint32_t pend = 0;
        for (int i = 0; i < INTERRUPT_SOURCE_MAX; ++i)
            if (m.interrupt_pending.Contains((MCU_Interrupt_Source)i))
                pend |= 1u << i;
        t->int_pending = pend;
        t->ipra = m.dev_register[DEV_IPRA];
        t->iprb = m.dev_register[DEV_IPRB];
        t->iprc = m.dev_register[DEV_IPRC];
        t->iprd = m.dev_register[DEV_IPRD];
        t->p1cr = m.dev_register[DEV_P1CR];
        const uint16_t pc0 = m.pc;
        const uint8_t  cp0 = m.cp;
        if (!m.sleep)
        {
            int op1 = code_byte(pc0);
            if (op1 >= 0)
            {
                int mode = -1, len = 0;
                switch (op1 >> 4)
                {
                case 0xa: mode = 0; len = 0; break;
                case 0xd: mode = 1; len = 0; break;
                case 0xe: mode = 2; len = 1; break;
                case 0xf: mode = 3; len = 2; break;
                case 0xb: mode = 4; len = 0; break;
                case 0xc: mode = 5; len = 0; break;
                case 0x0: if ((op1 & 7) == 4) { mode = 6; len = (op1 & 8) ? 2 : 1; } else if ((op1 & 7) == 5) { mode = 7; len = 1; } break;
                case 0x1: if ((op1 & 7) == 5) { mode = 8; len = 2; } break;
                }
                if (mode >= 0 && !(op1 == 0x0d && false))
                {
                    int opc = code_byte((uint16_t)(pc0 + 1 + len));
                    if (opc == 0) opc = code_byte((uint16_t)(pc0 + 2 + len));
                    if (opc >= 0) cov_general[(mode * 2 + ((op1 >> 3) & 1)) * 256 + opc]++;
                }
                else
                    cov_other[op1]++;
            }
        }

        // model step
        g_rec.clear();
        g_recording = true;
        emu.Step();
        g_recording = false;

        // HDL step
        size_t k = 0;
        t->step = 1;
        b.tick();
        t->step = 0;
        bool done = t->step_done;
        for (int guard = 0; !done && guard < 20000; ++guard)
        {
            if (t->bus_req && !t->bus_ack)
            {
                char msg[256];
                if (k >= g_rec.size())
                {
                    snprintf(msg, sizeof msg, "step %" PRIu64 " at %02x:%04x: HDL made extra access %s %06x (model made %zu)",
                             s, cp0, pc0, t->bus_we ? "W" : "R", t->bus_addr, g_rec.size());
                    return fail(hist, msg);
                }
                const Access& a = g_rec[k];
                if ((bool)t->bus_we != a.write || t->bus_addr != a.addr || (a.write && t->bus_wdata != a.data))
                {
                    snprintf(msg, sizeof msg, "step %" PRIu64 " at %02x:%04x access %zu: HDL %s %06x %02x, model %s",
                             s, cp0, pc0, k, t->bus_we ? "W" : "R", t->bus_addr, t->bus_wdata, dump_access(a).c_str());
                    std::string all;
                    for (auto& x : g_rec) all += dump_access(x) + "; ";
                    hist.add(std::string("model accesses: ") + all);
                    return fail(hist, msg);
                }
                if (!a.write)
                    t->bus_rdata = a.data;
                t->bus_ack = 1;
                ++k;
            }
            else
                t->bus_ack = 0;
            b.tick();
            done = t->step_done;
        }
        t->bus_ack = 0;
        if (done) b.tick();          // step_done comes in S_END's clock: let S_END finish (trace check)
        n_accesses += k;

        char line[256];
        snprintf(line, sizeof line, "step %" PRIu64 " %02x:%04x -> %02x:%04x sr=%04x acc=%zu pend=%06x",
                 s, cp0, pc0, m.cp, m.pc, m.sr, g_rec.size(), pend);
        hist.add(line);
        if (verbose)
        {
            std::string all;
            for (auto& x : g_rec) all += dump_access(x) + "; ";
            fprintf(stderr, "%s | %s\n", line, all.c_str());
        }

        if (!done)
            return fail(hist, "HDL step did not finish");
        if (k != g_rec.size())
        {
            char msg[128];
            snprintf(msg, sizeof msg, "HDL made %zu accesses, model %zu", k, g_rec.size());
            return fail(hist, msg);
        }

        // state comparison
        char msg[512];
        int  n = 0;
        for (int i = 0; i < 8; ++i)
            if (t->r_out[i] != m.r[i])
                n += snprintf(msg + n, sizeof msg - n, "r%d hdl=%04x ref=%04x; ", i, t->r_out[i], m.r[i]);
        auto cmp = [&](const char* name, uint32_t h, uint32_t r) {
            if (h != r) n += snprintf(msg + n, sizeof msg - n, "%s hdl=%x ref=%x; ", name, h, r);
        };
        cmp("pc", t->pc_out, m.pc);
        cmp("sr", t->sr_out, m.sr);
        cmp("cp", t->cp_out, m.cp);
        cmp("dp", t->dp_out, m.dp);
        cmp("ep", t->ep_out, m.ep);
        cmp("tp", t->tp_out, m.tp);
        cmp("br", t->br_out, m.br);
        cmp("sleep", t->sleep_out, m.sleep);
        cmp("ex_ignore", t->ex_ignore_out, m.ex_ignore);
        {
            int8_t   e   = (int8_t)m.exception_pending;
            uint32_t ref = e < 0 ? 0 : (4u | (uint32_t)e);
            cmp("exception", (t->exc_out & 4) ? t->exc_out : 0, ref);
        }
        {
            uint32_t tr = 0;
            for (int i = 0; i < 16; ++i)
                if (m.trapa_pending.Contains(i)) tr |= 1u << i;
            cmp("trapa", t->trapa_out, tr);
        }
        cmp("traps", (uint32_t)(t->traps - traps_hdl0), (uint32_t)(g_ref_hooks.error_traps - traps_ref0));
        if (n)
            return fail(hist, msg);
        pcs.insert(((uint32_t)cp0 << 16) | pc0);
        if (!g_rec.empty() && !g_rec[0].write && g_rec[0].addr < 0x100 && g_rec.size() >= 4 && g_rec[3].addr == (g_rec[0].addr | 3) && (g_rec[0].addr & 3) == 0 && !(cp0 == 0 && pc0 == g_rec[0].addr))
            ++n_interrupts;
    }

    printf("OK: %" PRIu64 " steps, %zu distinct PCs, %" PRIu64 " exception/interrupt entries, %" PRIu64 " bus accesses, %" PRIu64 " HDL clocks (%.1f/step), %" PRIu64 " error traps, ended at %02x:%04x\n",
           steps, pcs.size(), n_interrupts, n_accesses, b.clocks, (double)b.clocks / (double)steps,
           (uint64_t)(g_ref_hooks.error_traps - traps_ref0), m.cp, m.pc);
    if (random_seed >= 0)
    {
        size_t g = 0, o = 0;
        for (auto c : cov_general) g += c != 0;
        for (auto c : cov_other) o += c != 0;
        printf("coverage: general (mode x size x opcode byte) %zu/%zu, other first bytes %zu/256\n", g, cov_general.size(), o);
        // per opcode group (opcode byte >> 3) and mode: list never-run combinations
        static const char* modes[] = {"Rn", "@Rn", "@(d8,Rn)", "@(d16,Rn)", "@-Rn", "@Rn+", "#imm", "@aa8", "@aa16"};
        for (int grp = 0; grp < 32; ++grp)
            for (int md = 0; md < 9; ++md)
                for (int sz = 0; sz < 2; ++sz)
                {
                    uint64_t sum = 0;
                    for (int r = 0; r < 8; ++r) sum += cov_general[(md * 2 + sz) * 256 + grp * 8 + r];
                    if (!sum) printf("  never: opcode group %02x mode %s %s\n", grp * 8, modes[md], sz ? "W" : "B");
                }
    }
    delete b.top;
    return 0;
}
