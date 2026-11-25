// SPDX-License-Identifier: GPL-2.0-or-later
// Headless Nuked-MT32: the reference renderer and logger for the MT-32 core (plan milestone M0).
//
//   mt32_ref --roms DIR [--cpu-rom FILE] [--midi FILE] [--samples N] [--out FILE]
//            [--trace FILE] [--la32-log FILE] [--rv-log FILE] [--out-log FILE] [--lcd-log FILE]
//
// --roms     mt32_cpu.bin (128 KB), mt32_pcm.bin (512 KB), mt32_reverb.bin (32 KB)
// --cpu-rom  a different 128 KB control ROM (generated CPU test programs)
// --midi     byte schedule: "SAMPLE HEX HEX ..." per line, '#' comments; the bytes are queued
//            before sample SAMPLE is rendered, as Nuked-MT32's MIDI thread posts them
// --samples  32 kHz stereo samples to render (default 96000 = 3 s)
// --out      raw output, s16le stereo
// Logs (text, numbers in hex; the HDL benches print the same lines):
//   --trace    F cycle pc psw | I cycle level pc | R cycle addr data | W cycle addr data |
//              X cycle line state | M cycle byte | T cycle byte (MIDI out)   (cycle = CPU cycles, 12 MHz)
//   --la32-log L la32cycle addr data
//   --rv-log   W la32cycle reg value | I la32cycle which value
//   --out-log  one line a sample: acc2 acc3 acc6 acc7 rv0 rv1 vol l r (signed decimal)
//   --lcd-log  cycle "20 characters" (only when the text changes)
// --button SAMPLE:MASK (repeatable) sets the panel buttons (bit = MT32_BUTTTON_*) from SAMPLE on
//   --irq-log  S la32cycle status (LA32 interrupt set, slot start) | C la32cycle status (cleared by a write)
//   --slot-log debug, per slot: G la32cycle i oo prev w46 ram2_0 ram2_1 ram2_2 (after the wave generator)
//              and E la32cycle i acc2 acc3 acc6 acc7 w186<<16|w187 w188 (slot end; decimal)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "mt32.h"

void (*mt32h_fetch)(uint16_t, uint16_t, uint64_t);
void (*mt32h_irq)(int, uint16_t, uint64_t);
void (*mt32h_regw)(uint8_t, uint8_t, uint64_t);
void (*mt32h_input)(int, int, uint64_t);
void (*mt32h_busw)(uint16_t, uint8_t, uint64_t);
void (*mt32h_midi)(uint8_t, uint64_t);
void (*mt32h_tx)(uint8_t, uint64_t);
void (*mt32h_lcd)(const uint8_t*, uint64_t);
void (*mt32h_la32w)(uint16_t, uint8_t, uint64_t);
void (*mt32h_rvw)(int, uint32_t, uint64_t);
void (*mt32h_rvin)(int, int16_t, uint64_t);
void (*mt32h_out)(const int16_t*, const int16_t*, int32_t, int16_t, int16_t, uint64_t);
void (*mt32h_la32irq)(int, uint8_t, uint64_t);
void (*mt32h_slot)(int, uint32_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, uint64_t);

static FILE *f_trace, *f_la32, *f_rv, *f_out, *f_lcd, *f_irq, *f_slot;
static char lcd_last[21];

static mt32_t* mt;

static bool load(const std::string& path, uint8_t* buf, size_t size)
{
    FILE* f = fopen(path.c_str(), "rb");
    if (!f) return false;
    bool ok = fread(buf, 1, size, f) == size;
    fclose(f);
    return ok;
}

struct MidiEvent { uint64_t sample; std::vector<uint8_t> bytes; };

static std::vector<MidiEvent> read_schedule(const char* path)
{
    std::vector<MidiEvent> ev;
    FILE* f = fopen(path, "r");
    if (!f) { fprintf(stderr, "mt32_ref: can't open %s\n", path); exit(1); }
    char line[4096];
    uint64_t last = 0;
    while (fgets(line, sizeof line, f)) {
        char* p = strchr(line, '#');
        if (p) *p = 0;
        char* tok = strtok(line, " \t\r\n");
        if (!tok) continue;
        MidiEvent e;
        e.sample = strtoull(tok, nullptr, 10);
        if (e.sample < last) { fprintf(stderr, "mt32_ref: %s: samples must not decrease\n", path); exit(1); }
        last = e.sample;
        while ((tok = strtok(nullptr, " \t\r\n")))
            e.bytes.push_back((uint8_t)strtoul(tok, nullptr, 16));
        ev.push_back(e);
    }
    fclose(f);
    return ev;
}

int main(int argc, char** argv)
{
    std::vector<std::pair<uint64_t, uint32_t>> buttons;
    size_t next_button = 0;
    std::string roms, cpu_rom, midi, out_path;
    uint64_t samples = 96000;
    for (int i = 1; i < argc; i++) {
        std::string a = argv[i];
        auto val = [&]() -> const char* {
            if (i + 1 >= argc) { fprintf(stderr, "mt32_ref: %s needs a value\n", a.c_str()); exit(1); }
            return argv[++i];
        };
        auto open_log = [&](FILE** f) { *f = fopen(val(), "w"); if (!*f) { perror(argv[i]); exit(1); } };
        if (a == "--roms") roms = val();
        else if (a == "--cpu-rom") cpu_rom = val();
        else if (a == "--midi") midi = val();
        else if (a == "--samples") samples = strtoull(val(), nullptr, 0);
        else if (a == "--out") out_path = val();
        else if (a == "--trace") open_log(&f_trace);
        else if (a == "--la32-log") open_log(&f_la32);
        else if (a == "--rv-log") open_log(&f_rv);
        else if (a == "--out-log") open_log(&f_out);
        else if (a == "--lcd-log") open_log(&f_lcd);
        else if (a == "--irq-log") open_log(&f_irq);
        else if (a == "--slot-log") open_log(&f_slot);
        else if (a == "--button") {
            char* e; uint64_t at = strtoull(val(), &e, 0);
            if (*e != ':') { fprintf(stderr, "mt32_ref: --button SAMPLE:MASK\n"); return 1; }
            buttons.push_back({ at, (uint32_t)strtoul(e + 1, nullptr, 0) });
        }
        else { fprintf(stderr, "mt32_ref: unknown option %s\n", a.c_str()); return 1; }
    }
    if (roms.empty()) { fprintf(stderr, "mt32_ref: --roms DIR is required\n"); return 1; }

    if (f_trace) {
        mt32h_fetch = [](uint16_t pc, uint16_t psw, uint64_t c) { fprintf(f_trace, "F %llx %04x %04x\n", (unsigned long long)c, pc, psw); };
        mt32h_irq = [](int lv, uint16_t pc, uint64_t c) { fprintf(f_trace, "I %llx %x %04x\n", (unsigned long long)c, lv, pc); };
        mt32h_regw = [](uint8_t a, uint8_t d, uint64_t c) { fprintf(f_trace, "R %llx %02x %02x\n", (unsigned long long)c, a, d); };
        mt32h_input = [](int l, int s, uint64_t c) { fprintf(f_trace, "X %llx %x %x\n", (unsigned long long)c, l, s); };
        mt32h_busw = [](uint16_t a, uint8_t d, uint64_t c) { fprintf(f_trace, "W %llx %04x %02x\n", (unsigned long long)c, a, d); };
        mt32h_midi = [](uint8_t b, uint64_t c) { fprintf(f_trace, "M %llx %02x\n", (unsigned long long)c, b); };
        mt32h_tx = [](uint8_t b, uint64_t c) { fprintf(f_trace, "T %llx %02x\n", (unsigned long long)c, b); };
    }
    if (f_la32)
        mt32h_la32w = [](uint16_t a, uint8_t d, uint64_t c) { fprintf(f_la32, "L %llx %03x %02x\n", (unsigned long long)c, a, d); };
    if (f_rv) {
        mt32h_rvw = [](int r, uint32_t v, uint64_t c) { fprintf(f_rv, "W %llx %x %x\n", (unsigned long long)c, r, v); };
        mt32h_rvin = [](int w, int16_t v, uint64_t c) { fprintf(f_rv, "I %llx %x %d\n", (unsigned long long)c, w, v); };
    }
    if (f_out)
        mt32h_out = [](const int16_t* a, const int16_t* rv, int32_t vol, int16_t l, int16_t r, uint64_t) {
            fprintf(f_out, "%d %d %d %d %d %d %d %d %d\n", a[0], a[1], a[2], a[3], rv[0], rv[1], vol, l, r);
        };
    if (f_irq)
        mt32h_la32irq = [](int set, uint8_t st, uint64_t c) { fprintf(f_irq, "%c %llx %02x\n", set ? 'S' : 'C', (unsigned long long)c, st); };
    if (f_slot)
        mt32h_slot = [](int k, uint32_t i, int64_t a, int64_t b, int64_t c, int64_t d, int64_t e, int64_t f, uint64_t cy) {
            fprintf(f_slot, "%c %llx %u %lld %lld %lld %lld %lld %lld\n", k ? 'E' : 'G', (unsigned long long)cy, i,
                    (long long)a, (long long)b, (long long)c, (long long)d, (long long)e, (long long)f);
        };
    if (f_lcd)
        mt32h_lcd = [](const uint8_t* t, uint64_t c) {
            char s[21];
            for (int i = 0; i < 20; i++) s[i] = (t[i] >= 0x20 && t[i] < 0x7f) ? (char)t[i] : '?';
            s[20] = 0;
            if (strcmp(s, lcd_last)) { strcpy(lcd_last, s); fprintf(f_lcd, "%llx \"%s\"\n", (unsigned long long)c, s); }
        };

    // mt32_t starts the CPU in its constructor, so the ROMs go in through a staging copy.
    static uint8_t rom[0x20000], pcm[0x80000], rvrom[0x8000];
    std::string cpu = cpu_rom.empty() ? roms + "/mt32_cpu.bin" : cpu_rom;
    if (!load(cpu, rom, sizeof rom) || !load(roms + "/mt32_pcm.bin", pcm, sizeof pcm) ||
        !load(roms + "/mt32_reverb.bin", rvrom, sizeof rvrom)) {
        fprintf(stderr, "mt32_ref: can't read the ROMs (%s, %s)\n", cpu.c_str(), roms.c_str());
        return 1;
    }
    mt = new mt32_t();
    memcpy(mt->rom, rom, sizeof rom);
    memcpy(mt->pcm, pcm, sizeof pcm);
    memcpy(mt->rvrom, rvrom, sizeof rvrom);

    std::vector<MidiEvent> sched;
    if (!midi.empty()) sched = read_schedule(midi.c_str());
    size_t next = 0;

    FILE* out = out_path.empty() ? nullptr : fopen(out_path.c_str(), "wb");
    // Render in chunks up to the next scheduled byte: the output does not depend on the chunk
    // size (the reverb chip's sync() at each call falls on its own sample boundary), and
    // mt32_t::clock() redraws the LCD bitmap on every call.
    for (uint64_t s = 0; s < samples;) {
        while (next < sched.size() && sched[next].sample <= s) {
            for (uint8_t b : sched[next].bytes) mt->post_midi(b);
            next++;
        }
        while (next_button < buttons.size() && buttons[next_button].first <= s)
            mt->button = buttons[next_button++].second;
        uint64_t end = samples;
        if (next < sched.size() && sched[next].sample < end) end = sched[next].sample;
        if (next_button < buttons.size() && buttons[next_button].first < end) end = buttons[next_button].first;
        uint64_t n = end - s;
        if (n > 4096) n = 4096;
        mt->clock(n);
        if (out) fwrite(mt->samples, 4, n, out);
        s += n;
    }
    if (out) fclose(out);
    for (FILE* f : { f_trace, f_la32, f_rv, f_out, f_lcd, f_irq, f_slot })
        if (f) fclose(f);
    return 0;
}
