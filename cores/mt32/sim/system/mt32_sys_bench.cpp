// SPDX-License-Identifier: GPL-2.0-or-later
// MT-32 system bench (plan milestone M5): hdl/mt32_core.sv from power-on, with the same options
// and log formats as ref/mt32_ref, so tools/corpus.py and tools/trace_cmp.py compare the two.
//
//   mt32_sys_bench --roms DIR [--cpu-rom FILE] [--midi FILE] [--samples N] [--out FILE]
//                  [--trace FILE] [--out-log FILE] [--lcd-log FILE] [--maxlat N] [--seed S]
//                  [--stats]
//
// The PCM ROM is served by an in-order memory model with random latency 1..maxlat clocks
// (default 40) and random req_ready. MIDI bytes scheduled for sample s are queued while the
// board waits at the start of sample s (`hold`), as Nuked-MT32's MIDI thread posts them before
// clock(). --stats prints clocks per sample (mean, worst).
#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <random>
#include <string>
#include <vector>
#include "Vmt32_core.h"
#include "verilated.h"

static bool load(const std::string& p, std::vector<uint8_t>& b)
{
    FILE* f = fopen(p.c_str(), "rb");
    if (!f) return false;
    bool ok = fread(b.data(), 1, b.size(), f) == b.size();
    fclose(f);
    return ok;
}

struct Ev { uint64_t sample; std::vector<uint8_t> bytes; };
struct Rsp { uint64_t t; uint64_t data; uint8_t tag; };

int main(int argc, char** argv)
{
    std::string roms, cpu_rom, midi, out_path, trace_p, outlog_p, lcd_p;
    uint64_t samples = 96000, seed = 1;
    unsigned maxlat = 40;
    bool stats = false;
    for (int i = 1; i < argc; i++) {
        std::string a = argv[i];
        auto v = [&]() -> const char* { if (i + 1 >= argc) exit(1); return argv[++i]; };
        if (a == "--roms") roms = v();
        else if (a == "--cpu-rom") cpu_rom = v();
        else if (a == "--midi") midi = v();
        else if (a == "--samples") samples = strtoull(v(), nullptr, 0);
        else if (a == "--out") out_path = v();
        else if (a == "--trace") trace_p = v();
        else if (a == "--out-log") outlog_p = v();
        else if (a == "--lcd-log") lcd_p = v();
        else if (a == "--maxlat") maxlat = (unsigned)strtoul(v(), nullptr, 0);
        else if (a == "--seed") seed = strtoull(v(), nullptr, 0);
        else if (a == "--stats") stats = true;
        else { fprintf(stderr, "mt32_sys_bench: unknown option %s\n", a.c_str()); return 1; }
    }
    std::vector<uint8_t> rom(0x20000), pcm(0x80000), rvrom(0x8000);
    if (!load(cpu_rom.empty() ? roms + "/mt32_cpu.bin" : cpu_rom, rom) ||
        !load(roms + "/mt32_pcm.bin", pcm) || !load(roms + "/mt32_reverb.bin", rvrom)) {
        fprintf(stderr, "mt32_sys_bench: can't read the ROMs\n");
        return 1;
    }
    std::vector<Ev> sched;
    if (!midi.empty()) {
        FILE* f = fopen(midi.c_str(), "r");
        if (!f) return 1;
        char line[4096];
        while (fgets(line, sizeof line, f)) {
            char* c = strchr(line, '#');
            if (c) *c = 0;
            char* t = strtok(line, " \t\r\n");
            if (!t) continue;
            Ev e{strtoull(t, nullptr, 10), {}};
            while ((t = strtok(nullptr, " \t\r\n"))) e.bytes.push_back((uint8_t)strtoul(t, nullptr, 16));
            sched.push_back(e);
        }
        fclose(f);
    }
    FILE* out = out_path.empty() ? nullptr : fopen(out_path.c_str(), "wb");
    FILE* tr = trace_p.empty() ? nullptr : fopen(trace_p.c_str(), "w");
    FILE* ol = outlog_p.empty() ? nullptr : fopen(outlog_p.c_str(), "w");
    FILE* lc = lcd_p.empty() ? nullptr : fopen(lcd_p.c_str(), "w");

    auto* m = new Vmt32_core;
    std::mt19937_64 rng(seed);
    std::deque<Rsp> rq;
    uint64_t clk = 0, last_t = 0;
    std::deque<uint8_t> midi_q;
    int extint_q = 0;
    char lcd_last[21] = "";

    auto tick = [&]() {
        m->pcm_rsp_valid = 0;
        if (!rq.empty() && rq.front().t <= clk) {
            m->pcm_rsp_valid = 1;
            m->pcm_rsp_data = rq.front().data;
            m->pcm_rsp_tag = rq.front().tag;
            rq.pop_front();
        }
        m->pcm_req_ready = (rng() % 4) != 0;
        m->midi_valid = 0;
        if (!midi_q.empty() && !m->midi_full) { m->midi_valid = 1; m->midi_byte = midi_q.front(); }
        m->eval();
        if (m->pcm_req_valid && m->pcm_req_ready) {
            uint64_t w = m->pcm_req_addr, d = 0;
            for (int b = 0; b < 8; b++) d |= (uint64_t)pcm[(w * 8 + b) & 0x7ffff] << (8 * b);
            uint64_t t = clk + 1 + rng() % maxlat;
            if (t <= last_t) t = last_t + 1;
            last_t = t;
            rq.push_back({t, d, (uint8_t)m->pcm_req_tag});
        }
        if (m->midi_valid) midi_q.pop_front();
        m->clk = 1; m->eval();
        clk++;
        if (tr) {
            unsigned long long c = m->cpu_cycles;
            if (m->dbg_irq) fprintf(tr, "I %llx %x %04x\n", c, m->dbg_level, m->dbg_pc);
            if (m->dbg_fetch) fprintf(tr, "F %llx %04x %04x\n", c, m->dbg_fpc, m->dbg_psw);
            if (m->dbg_regw) fprintf(tr, "R %llx %02x %02x\n", c, m->dbg_addr, m->dbg_data);
            if (m->dbg_busw) fprintf(tr, "W %llx %04x %02x\n", c, m->dbg_waddr, m->dbg_wdata);
            if (m->extint != extint_q) fprintf(tr, "X %llx 0 %x\n", c, m->extint);
            if (m->dbg_midi) fprintf(tr, "M %llx %02x\n", c, m->dbg_mbyte);
        }
        extint_q = m->extint;
        if (lc && m->dbg_lcd) {
            char s[21];
            for (int i = 0; i < 20; i++) {
                m->lcd_rd_addr = i;
                m->eval();
                uint8_t t = m->lcd_rd_text;
                s[i] = (t >= 0x20 && t < 0x7f) ? (char)t : '?';
            }
            s[20] = 0;
            if (strcmp(s, lcd_last)) { strcpy(lcd_last, s); fprintf(lc, "%llx \"%s\"\n", (unsigned long long)m->cpu_cycles, s); }
        }
        m->clk = 0; m->eval();
    };

    m->clk = 0; m->rst = 1; m->hold = 1; m->buttons = 0; m->knob = 1020; m->rom_we = 0; m->rvrom_we = 0;
    m->eval();
    for (uint32_t a = 0; a < rom.size(); a++) { m->rom_we = 1; m->rom_addr = a; m->rom_data = rom[a]; tick(); }
    m->rom_we = 0;
    for (uint32_t a = 0; a < rvrom.size(); a++) { m->rvrom_we = 1; m->rvrom_addr = a; m->rvrom_data = rvrom[a]; tick(); }
    m->rvrom_we = 0;
    for (int i = 0; i < 4; i++) tick();
    m->rst = 0;
    while (!m->ready) tick();

    uint64_t s = 0, next = 0, last_clk = clk, worst = 0, start_clk = clk;
    while (s < samples) {
        if (m->at_hold) {
            while (next < sched.size() && sched[next].sample <= s) {
                for (uint8_t b : sched[next].bytes) midi_q.push_back(b);
                next++;
            }
            while (!midi_q.empty()) tick();
            m->hold = 0;
            tick();
            m->hold = 1;
        }
        tick();
        if (m->bad_op) { fprintf(stderr, "mt32_sys_bench: opcode without a handler at %04x\n", m->dbg_pc); return 3; }
        if (clk - last_clk > 200000) {
            fprintf(stderr, "mt32_sys_bench: no sample for 200000 clocks at sample %llu: cpu pc %04x cycles %llu\n",
                    (unsigned long long)s, m->dbg_pc, (unsigned long long)m->cpu_cycles);
            return 5;
        }
        if (m->sample_valid) {
            int16_t lr[2] = { (int16_t)m->out_l, (int16_t)m->out_r };
            if (out) fwrite(lr, 2, 2, out);
            if (ol) fprintf(ol, "%d %d %d %d %d %d %d %d %d\n", (int16_t)m->dbg_acc2, (int16_t)m->dbg_acc3,
                            (int16_t)m->dbg_acc6, (int16_t)m->dbg_acc7, (int16_t)m->dbg_rv0, (int16_t)m->dbg_rv1,
                            (int)m->dbg_vol, lr[0], lr[1]);
            if (clk - last_clk > worst && s > 0) worst = clk - last_clk;
            last_clk = clk;
            s++;
        }
    }
    if (stats)
        fprintf(stderr, "clocks per sample: mean %.1f, worst %llu (maxlat %u)\n",
                (double)(clk - start_clk) / samples, (unsigned long long)worst, maxlat);
    for (FILE* f : { out, tr, ol, lc }) if (f) fclose(f);
    delete m;
    return 0;
}
