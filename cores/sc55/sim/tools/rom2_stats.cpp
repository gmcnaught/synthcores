// ROM2 access statistics from the reference model (for the mk2 ROM2-in-SDRAM
// design): reads per step, the share in the upper 256 KB, and miss rates of
// direct-mapped caches. Model only, no HDL.
//
// Usage: rom2_stats --roms DIR [--romset mk2-v1.01] [--steps N] [--midi-demo SEED] [--midi-loud SEED]
#include "emu.h"
#include "mcu.h"
#include "rom_loader.h"
#include "ref_hooks.h"
#include "../common/midi_demo.h"

#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

struct Cache
{
    int line_bits, sets_bits;
    std::vector<int64_t> tag;
    uint64_t access = 0, miss = 0, miss_upper = 0;
    Cache(int lb, int sb) : line_bits(lb), sets_bits(sb), tag((size_t)1 << sb, -1) {}
    void touch(uint32_t a)
    {
        uint32_t line = a >> line_bits;
        uint32_t set  = line & ((1u << sets_bits) - 1);
        ++access;
        if (tag[set] != (int64_t)line) { tag[set] = line; ++miss; if (a & 0x40000) ++miss_upper; }
    }
};

static std::vector<Cache>* g_caches;
static uint64_t g_reads = 0, g_upper = 0, g_mask = 0x7ffff;
static uint64_t g_step_reads = 0, g_max_step = 0;

static void bus_hook(void*, bool write, uint32_t addr, uint8_t)
{
    if (write) return;
    uint32_t page = (addr >> 16) & 0xf;
    bool rom2 = page == 1 || page == 2 || page == 3 || page == 4 || page == 8 || page == 9 || page == 14 || page == 15;
    if (!rom2) return;
    uint32_t a = (addr & 0x3ffff) | ((addr & 0x80000) ? 0x40000 : 0);
    a &= (uint32_t)g_mask;
    ++g_reads;
    ++g_step_reads;
    if (a & 0x40000) ++g_upper;
    for (auto& c : *g_caches) c.touch(a);
}

int main(int argc, char** argv)
{
    std::string rom_dir, romset = "mk2-v1.01";
    uint64_t steps = 20'000'000;
    int64_t midi_seed = -1, loud_seed = -1;
    for (int i = 1; i < argc; ++i)
    {
        std::string a = argv[i];
        if (a == "--roms" && i + 1 < argc) rom_dir = argv[++i];
        else if (a == "--romset" && i + 1 < argc) romset = argv[++i];
        else if (a == "--steps" && i + 1 < argc) steps = std::strtoull(argv[++i], nullptr, 0);
        else if (a == "--midi-demo" && i + 1 < argc) midi_seed = std::strtoll(argv[++i], nullptr, 0);
        else if (a == "--midi-loud" && i + 1 < argc) loud_seed = std::strtoll(argv[++i], nullptr, 0);
        else { fprintf(stderr, "unknown argument %s\n", a.c_str()); return 2; }
    }
    common::LoadRomsetResult load;
    common::RomOverrides ov{};
    if (auto err = common::LoadRomset(rom_dir, romset, common::RomLoader::Hashing, ov, load); err != common::LoadRomsetError{})
    {
        fprintf(stderr, "romset load failed: %s\n", common::ToCString(err));
        return 2;
    }
    Emulator emu;
    emu.Init({.lcd_backend = nullptr});
    emu.LoadRoms(load.romset, load.romset_info);
    emu.Reset();
    g_mask = emu.GetMCU().rom2_mask;

    std::vector<Cache> caches;
    for (int lb : {3, 4})
        for (int kb : {1, 2, 4, 8, 16})
        {
            int sb = 0;
            while ((1 << (sb + lb)) < kb * 1024) ++sb;
            caches.emplace_back(lb, sb);
        }
    g_caches = &caches;
    g_ref_hooks.bus = bus_hook;

    std::vector<uint8_t> midi;
    if (midi_seed >= 0) midi = make_midi_demo((uint64_t)midi_seed);
    if (loud_seed >= 0) midi = make_midi_loud((uint64_t)loud_seed);
    size_t pos = 0;
    for (uint64_t s = 0; s < steps; ++s)
    {
        if (!midi.empty() && s >= 4'000'000 && (s % 600) == 0 && pos < midi.size()) emu.PostMIDI(midi[pos++]);
        g_step_reads = 0;
        emu.Step();
        if (g_step_reads > g_max_step) g_max_step = g_step_reads;
    }
    printf("%s, %" PRIu64 " steps, MIDI bytes %zu: ROM2 reads %" PRIu64 " (%.2f a step, max %" PRIu64 " in one step), upper 256 KB %.1f %%\n",
           romset.c_str(), steps, pos, g_reads, (double)g_reads / (double)steps, g_max_step, 100.0 * (double)g_upper / (double)std::max<uint64_t>(g_reads, 1));
    for (auto& c : caches)
        printf("  direct-mapped %2d KB, %2d-byte lines: miss %.3f %% of reads (%.4f a step; upper half %.4f a step)\n",
               (int)((1u << (c.sets_bits + c.line_bits)) / 1024), 1 << c.line_bits,
               100.0 * (double)c.miss / (double)std::max<uint64_t>(c.access, 1), (double)c.miss / (double)steps,
               (double)c.miss_upper / (double)steps);
    return 0;
}
