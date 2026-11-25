#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-or-later
"""Copy Nuked-MT32 (modules/nuked-mt32) into a build folder and add the reference's log hooks.

    prepare_src.py SRC OUT

Every edit is anchored on text from the pinned commit; a missing anchor stops the build, so an
upstream change can't silently drop a hook. The hooks are calls through null function pointers
(mt32_hooks.h): with no logging asked for, the audio is the upstream audio (checked by
tests/test_ref.py against a build without hooks).
"""
import shutil
import sys
from pathlib import Path

FILES = ["mt32.cpp", "mt32.h", "la32.cpp", "la32.h", "rv2.cpp", "rv2.h", "lcd.cpp", "lcd_font.h",
         "mame/emu.h", "mame/mcs96.cpp", "mame/mcs96.h", "mame/mcs96.hxx", "mame/i8x9x.cpp",
         "mame/i8x9x.h", "mame/i8x9x.hxx"]

# (file, anchor, replacement). The anchor must occur exactly `count` times.
EDITS = [
    # Windows include path.
    ("mame/emu.h", '#include "..\\mt32.h"', '#include "../mt32.h"\n#include "../mt32_hooks.h"', 1),
    ("mt32.h", "#include <mutex>", "#include <mutex>\n#include \"mt32_hooks.h\"", 1),
    # CPU: every instruction fetch, and every interrupt taken.
    ("mame/mcs96.hxx", "\t\tOP1 = level;\n",
     "\t\tOP1 = level;\n\t\tif (mt32h_irq) mt32h_irq(level, PC, total_cycles());\n", 1),
    ("mame/mcs96.hxx", "\tPPC = PC;\n",
     "\tPPC = PC;\n\tif (mt32h_fetch) mt32h_fetch(PC, PSW, total_cycles());\n", 2),
    # CPU: every write to the register file and SFRs, and every external input change.
    ("mame/i8x9x.cpp", "void i8x9x_device::reg_write(uint8_t addr, uint8_t data)\n{\n",
     "void i8x9x_device::reg_write(uint8_t addr, uint8_t data)\n{\n"
     "\tif (mt32h_regw) mt32h_regw(addr, data, total_cycles());\n", 1),
    ("mame/i8x9x.cpp", "void i8x9x_device::execute_set_input(int linenum, int state)\n{\n",
     "void i8x9x_device::execute_set_input(int linenum, int state)\n{\n"
     "\tif (mt32h_input) mt32h_input(linenum, state, total_cycles());\n", 1),
    # MIDI out: every byte the CPU sends (sbuf_w; MAME's serial_tx_cb is commented out).
    ("mame/i8x9x.cpp", "void i8x9x_device::sbuf_w(u8 data)\n{\n",
     "void i8x9x_device::sbuf_w(u8 data)\n{\n"
     "\tif (mt32h_tx) mt32h_tx(data, total_cycles());\n", 1),
    # Board: every bus write, every MIDI byte handed to the CPU, the LCD text after a command.
    ("mt32.cpp", "void mt32_t::cpu_write(uint16_t address, uint8_t data)\n{\n",
     "void mt32_t::cpu_write(uint16_t address, uint8_t data)\n{\n"
     "    if (mt32h_busw) mt32h_busw(address, data, mcu->total_cycles());\n", 1),
    ("mt32.cpp", "            mcu->serial_w(b);\n",
     "            mcu->serial_w(b);\n            if (mt32h_midi) mt32h_midi(b, mcu->total_cycles());\n", 1),
    ("mt32.cpp", "                ga_lcd_queue_cnt = 0;\n",
     "                ga_lcd_queue_cnt = 0;\n"
     "                if (mt32h_lcd) mt32h_lcd(lcd_data, mcu->total_cycles());\n", 1),
    # LA32: register writes with the LA32 cycle; the reverb chip's register writes.
    ("la32.h", "public:\n", "public:\n    uint64_t cycles() const { return la32_cycles; }\n", 1),
    ("la32.cpp", "void la32_t::write(uint16_t address, uint8_t data)\n{\n",
     "void la32_t::write(uint16_t address, uint8_t data)\n{\n"
     "    if (mt32h_la32w) mt32h_la32w(address, data, la32_cycles);\n", 1),
    ("mt32.cpp", "                rv2->set_hiaddr((data >> 1) & 3);\n",
     "                rv2->set_hiaddr((data >> 1) & 3);\n"
     "                if (mt32h_rvw) mt32h_rvw(2, (data >> 1) & 3, la32->cycles());\n", 1),
    ("mt32.cpp", "                rv2->write(1, data << 1);\n",
     "                rv2->write(1, data << 1);\n"
     "                if (mt32h_rvw) mt32h_rvw(1, data << 1, la32->cycles());\n", 1),
    ("mt32.cpp", "                rv2->write(0, data << 1);\n",
     "                rv2->write(0, data << 1);\n"
     "                if (mt32h_rvw) mt32h_rvw(0, data << 1, la32->cycles());\n", 1),
    # Reverb input (twice a sample) and the mixed sample: accumulators, reverb out, volume, result.
    ("la32.cpp", "                mt32->rv2->set_input(output_expand(accum[oddeven ^ 1][6]));\n",
     "                mt32->rv2->set_input(output_expand(accum[oddeven ^ 1][6]));\n"
     "                if (mt32h_rvin) mt32h_rvin(0, output_expand(accum[oddeven ^ 1][6]), la32_cycles);\n", 1),
    ("la32.cpp", "                mt32->rv2->set_input(output_expand(accum[oddeven ^ 1][2]));\n",
     "                mt32->rv2->set_input(output_expand(accum[oddeven ^ 1][2]));\n"
     "                if (mt32h_rvin) mt32h_rvin(1, output_expand(accum[oddeven ^ 1][2]), la32_cycles);\n", 1),
    ("la32.cpp", "        mt32->samples[sample_dst][0] = s_l;\n",
     "        if (mt32h_out) {\n"
     "            int16_t acc[4] = { output_expand(accum[oddeven^1][2]), output_expand(accum[oddeven^1][3]),\n"
     "                               output_expand(accum[oddeven^1][6]), output_expand(accum[oddeven^1][7]) };\n"
     "            mt32h_out(acc, reverb, vol, (int16_t)s_l, (int16_t)s_r, la32_cycles);\n"
     "        }\n"
     "        mt32->samples[sample_dst][0] = s_l;\n", 1),
    # LA32 interrupt set (slot start cycle, status) and clear (CPU write); per-slot debug state.
    ("la32.cpp", "                    int_status = 0x80 | i | (tv << 5);\n",
     "                    int_status = 0x80 | i | (tv << 5);\n"
     "                    if (mt32h_la32irq) mt32h_la32irq(1, int_status, la32_cycles);\n", 1),
    ("la32.cpp", "                        int_status = (i + 1) & 31;\n",
     "                        int_status = (i + 1) & 31;\n"
     "                        if (mt32h_la32irq) mt32h_la32irq(1, int_status, la32_cycles);\n", 1),
    ("la32.cpp", "                        int_status = ((i + 1) & 31) | 32;\n",
     "                        int_status = ((i + 1) & 31) | 32;\n"
     "                        if (mt32h_la32irq) mt32h_la32irq(1, int_status, la32_cycles);\n", 1),
    ("la32.cpp", "                mt32->mcu->execute_set_input(i8x9x_device::EXTINT_LINE, 0);\n",
     "                mt32->mcu->execute_set_input(i8x9x_device::EXTINT_LINE, 0);\n"
     "                if (mt32h_la32irq) mt32h_la32irq(0, int_status, la32_cycles);\n", 1),
    ("la32.cpp", "                if (!(ring && (i & 1) == 0))\n",
     "                if (mt32h_slot) mt32h_slot(0, i, oo, prev, w46, ram2[0][i], ram2[1][i], ram2[2][i], la32_cycles);\n"
     "                if (!(ring && (i & 1) == 0))\n", 1),
    ("la32.cpp", "            la32_cycles += 8;\n",
     "            if (mt32h_slot) mt32h_slot(1, i, accum[oddeven][2], accum[oddeven][3], accum[oddeven][6],\n"
     "                                       accum[oddeven][7], (w186 << 16) | w187, w188, la32_cycles);\n"
     "            la32_cycles += 8;\n", 1),
]


def main():
    src, out = Path(sys.argv[1]), Path(sys.argv[2])
    (out / "mame").mkdir(parents=True, exist_ok=True)
    texts = {f: (src / f).read_text() for f in FILES}
    for f, anchor, repl, count in EDITS:
        n = texts[f].count(anchor)
        if n != count:
            sys.exit(f"prepare_src: {f}: anchor found {n} times, expected {count}: {anchor!r}")
        texts[f] = texts[f].replace(anchor, repl)
    for f, t in texts.items():
        p = out / f
        if not p.exists() or p.read_text() != t:
            p.write_text(t)
    shutil.copyfile(Path(__file__).with_name("mt32_hooks.h"), out / "mt32_hooks.h")


if __name__ == "__main__":
    main()
