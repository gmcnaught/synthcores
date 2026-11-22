// SPDX-License-Identifier: GPL-2.0-or-later
// jv880-nvram: the factory-preset JV-880 NVRAM that Nuked-SC55's jv880 render test expects
// (doc/building.rst, NUKED_TEST_JV880_NVRAM; SHA-256 d5da7845...d207), made without the GUI.
// It does what the documented key presses do: start with no NVRAM, UTILITY, turn the encoder
// until "Factory preset" shows, ENTER twice, then save the 32 KB NVRAM.
//
//   jv880-nvram ROMDIR OUT.nvram [ROMSET]       (ROMSET default jv880-v1.0.1)
//
// Prints the LCD after each step. Instrument time: 600 ns a step (the renderer's guess).
#include "emu.h"
#include "mcu.h"
#include "lcd.h"
#include "rom_loader.h"

#include <cstdio>
#include <fstream>
#include <filesystem>
#include <string>

// LCD_Write keeps no LCD state unless a backend is attached, so attach one that draws nothing
struct NullLcd : LCD_Backend
{
    bool Start(const lcd_t&) override { return true; }
    void Stop() override {}
    void Render() override {}
};

static const uint64_t STEPS_PER_MS = 1'000'000 / 600;

static void run_ms(Emulator& emu, uint64_t ms)
{
    for (uint64_t i = 0; i < ms * STEPS_PER_MS; i++)
        emu.Step();
}

static std::string lcd_text(Emulator& emu)
{
    // HD44780 display RAM: line 1 at 0x00, line 2 at 0x40 (24 characters each on the JV-880)
    const lcd_t& lcd = emu.GetLCD();
    std::string s;
    for (int line = 0; line < 2; line++)
    {
        for (int i = 0; i < 24; i++)
        {
            uint8_t c = lcd.LCD_Data[line * 40 + i];
            s += (c >= 0x20 && c < 0x7f) ? char(c) : '.';
        }
        if (line == 0) s += " | ";
    }
    return s;
}

static void press(Emulator& emu, int button, uint64_t hold_ms = 100, uint64_t after_ms = 400)
{
    emu.GetMCU().button_pressed |= 1u << button;
    run_ms(emu, hold_ms);
    emu.GetMCU().button_pressed &= ~(1u << button);
    run_ms(emu, after_ms);
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        std::fprintf(stderr, "usage: jv880-nvram ROMDIR OUT.nvram [ROMSET]\n");
        return 2;
    }
    const std::filesystem::path out = argv[2];
    std::filesystem::remove(out);                   // start from no NVRAM, as the procedure does

    common::LoadRomsetResult load;
    common::LoadRomsetError err = common::LoadRomset(
        argv[1], argc > 3 ? argv[3] : "jv880-v1.0.1", common::RomLoader::Hashing, {}, load);
    common::PrintLoadRomsetDiagnostics(err, load);
    if (err != common::LoadRomsetError{})
        return 1;

    Emulator emu;
    NullLcd lcd;
    emu.Init({.lcd_backend = &lcd});                // no NVRAM file: start empty, write it below
    if (!emu.LoadRoms(load.romset, load.romset_info, nullptr))
        return 1;
    emu.Reset();
    emu.StartLCD();

    // boot: wait until the LCD shows something and stays the same for 2 s (at most 60 s)
    std::string last;
    int same = 0;
    for (int t = 0; t < 120 && same < 4; t++)
    {
        run_ms(emu, 500);
        std::string now = lcd_text(emu);
        same = (now == last && now.find_first_not_of(". |") != std::string::npos) ? same + 1 : 0;
        if (now != last || t % 10 == 0) std::printf("boot %4.1f s: pc %02x:%04x %s\n", (t + 1) * 0.5, emu.GetMCU().cp, emu.GetMCU().pc, now.c_str());
        last = now;
    }
    press(emu, MCU_BUTTON_UTILITY);
    std::printf("utility  : %s\n", lcd_text(emu).c_str());
    int turns = 0;
    while (lcd_text(emu).find("Factory preset") == std::string::npos)
    {
        if (++turns > 40)
        {
            std::fprintf(stderr, "no \"Factory preset\" after 40 encoder steps\n");
            return 1;
        }
        MCU_EncoderTrigger(emu.GetMCU(), 1);
        run_ms(emu, 300);
        std::printf("encoder %2d: %s\n", turns, lcd_text(emu).c_str());
    }
    press(emu, MCU_BUTTON_ENTER);
    std::printf("enter    : %s\n", lcd_text(emu).c_str());
    press(emu, MCU_BUTTON_ENTER, 100, 3000);
    std::printf("enter    : %s\n", lcd_text(emu).c_str());
    std::ofstream f(out, std::ios::binary);
    f.write(reinterpret_cast<const char*>(emu.GetMCU().nvram), NVRAM_SIZE);
    f.close();
    std::printf("saved %s\n", out.string().c_str());
    return 0;
}
