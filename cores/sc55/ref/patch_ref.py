#!/usr/bin/env python3
"""Copy the reference backend, apply FIXES, and add observation hooks for the lockstep benches.

FIXES go into both builds (the plain one with --fixes-only). They change behaviour only where the
fork departs from Nuked-SC55's original:
  - JV-880: addclip20 saturates at 20 bits again. jcmoyer/Nuked-SC55 7204125 replaced the
    original (nukeykt) saturating add with a plain sign-extended add, noting that if these
    functions do overflow "their outputs simply need to saturate at 20 ... bits". The JV-880
    overflows in loud chords (the 20-bit mix wrapped from full negative to full positive). The
    SC-55 models keep the plain add (none of their tests overflow; saturating them costs the
    SC-55 core its timing). The jv880.mid integration-test hash is unchanged.

The hooks only observe: with no hook installed, the hooked backend behaves exactly like the
plain one (checked by tests/test_ref_patch.py, which renders with both builds and compares the
audio).

Usage: patch_ref.py [--fixes-only] NUKED_SRC_DIR OUT_SRC_DIR   (copies all of src/, patches src/backend)
"""
import pathlib
import shutil
import sys

HOOK_DECLS = """
// ---- sc55-MiSTer lockstep hooks (added by ref/patch_ref.py) ----
#include "ref_hooks.h"
"""

# (file, old, new): every `old` must occur exactly once.
FIXES = [
    ("backend/pcm.cpp",
     "inline int32_t addclip20(int32_t add1, int32_t add2, int32_t cin)\n"
     "{\n"
     "    return sx20(add1) + sx20(add2) + cin;\n"
     "}",
     "// JV-880: saturate at 20 bits, as Nuked-SC55's original (ref/patch_ref.py FIXES)\n"
     "static bool g_addclip20_saturate = false;\n"
     "\n"
     "inline int32_t addclip20(int32_t add1, int32_t add2, int32_t cin)\n"
     "{\n"
     "    const int32_t sum = sx20(add1) + sx20(add2) + cin;\n"
     "    if (!g_addclip20_saturate)\n"
     "        return sum;\n"
     "    return sum > 0x7ffff ? 0x7ffff : sum < -0x80000 ? -0x80000 : sum;\n"
     "}"),
    ("backend/pcm.cpp",
     "void PCM_Update(pcm_t& pcm, uint64_t cycles)\n{\n",
     "void PCM_Update(pcm_t& pcm, uint64_t cycles)\n{\n"
     "    g_addclip20_saturate = pcm.mcu->is_jv880;\n"),
]

EDITS = [
    ("backend/mcu.cpp",
     "uint8_t MCU_Read(mcu_t& mcu, uint32_t address)\n{",
     "static uint8_t MCU_Read_Base(mcu_t& mcu, uint32_t address)\n{"),
    ("backend/mcu.cpp",
     "void MCU_Write(mcu_t& mcu, uint32_t address, uint8_t value)\n{",
     "static void MCU_Write_Base(mcu_t& mcu, uint32_t address, uint8_t value)\n{"),
    ("backend/mcu.cpp",
     'Diag_Printf(Diag_Category::Debug, "%.2x %.4x\\n", mcu.cp, mcu.pc);',
     'Diag_Printf(Diag_Category::Debug, "%.2x %.4x\\n", mcu.cp, mcu.pc);\n'
     '    ++g_ref_hooks.error_traps;'),
    ("backend/mcu.cpp",
     "    mcu.cycles += 12; // FIXME: assume 12 cycles per instruction",
     "    if (g_ref_hooks.cpu_done) g_ref_hooks.cpu_done(g_ref_hooks.ctx);\n"
     "    mcu.cycles += 12; // FIXME: assume 12 cycles per instruction"),
    # MIDI out. The reference sends nothing ("dummy TX"). A byte counts as sent when the CPU
    # writes TDR and then writes SSR with TDRE 0 (the byte is TDR), whether or not the model's
    # TDRE was 1: the H8/532's SCI is double-buffered (TDR moves to the shift register and TDRE
    # sets again at once while the line is idle), and the JV-880 firmware writes two bytes into
    # one of the model's 3000-cycle TDRE periods. On the mk2, a byte is the sub-MCU's write to its
    # UART2 data register (dev 0x08, address 0xe8).
    ("backend/mcu.cpp",
     "void MCU_DeviceWrite(mcu_t& mcu, uint32_t address, uint8_t data)\n{\n    address &= 0x7f;\n",
     "void MCU_DeviceWrite(mcu_t& mcu, uint32_t address, uint8_t data)\n{\n    address &= 0x7f;\n"
     "    if (address == DEV_TDR)\n"
     "        g_ref_hooks.tdr_written = true;\n"
     "    else if (address == DEV_SSR && (data & 0x80) == 0 && g_ref_hooks.tdr_written)\n"
     "    {\n"
     "        g_ref_hooks.tdr_written = false;\n"
     "        if (g_ref_hooks.tx) g_ref_hooks.tx(g_ref_hooks.ctx, 0, mcu.cycles, mcu.dev_register[DEV_TDR]);\n"
     "    }\n"),
    ("backend/submcu.cpp",
     "void SM_Write(submcu_t& sm, uint16_t address, uint8_t data)\n{\n    address &= 0x1fff;\n",
     "void SM_Write(submcu_t& sm, uint16_t address, uint8_t data)\n{\n    address &= 0x1fff;\n"
     "    if (address == 0xe8 && g_ref_hooks.tx) g_ref_hooks.tx(g_ref_hooks.ctx, 1, sm.cycles, data);\n"),
]

APPEND = {
    "backend/mcu.cpp": """
// ---- sc55-MiSTer lockstep hooks (added by ref/patch_ref.py) ----
RefHooks g_ref_hooks;

uint8_t MCU_Read(mcu_t& mcu, uint32_t address)
{
    uint8_t v = MCU_Read_Base(mcu, address);
    if (g_ref_hooks.bus)
        g_ref_hooks.bus(g_ref_hooks.ctx, false, address, v);
    return v;
}

void MCU_Write(mcu_t& mcu, uint32_t address, uint8_t value)
{
    if (g_ref_hooks.bus)
        g_ref_hooks.bus(g_ref_hooks.ctx, true, address, value);
    MCU_Write_Base(mcu, address, value);
}
""",
    "backend/submcu.cpp": "",
}

HOOKS_H = """#pragma once
// Observation hooks for the sc55-MiSTer lockstep benches (ref/patch_ref.py).
#include <cstdint>

struct RefHooks
{
    void* ctx = nullptr;
    // Every byte the MCU reads or writes through MCU_Read/MCU_Write, in order.
    void (*bus)(void* ctx, bool write, uint32_t address, uint8_t data) = nullptr;
    // Called in MCU_Step after the instruction, before cycles += 12 and the devices.
    void (*cpu_done)(void* ctx) = nullptr;
    // MIDI out: a byte sent (src 0 main MCU SCI, 1 mk2 sub-MCU UART2; cycle = mcu.cycles / sm.cycles).
    void (*tx)(void* ctx, int src, uint64_t cycle, uint8_t data) = nullptr;
    bool tdr_written = false;   // TDR written since the last byte sent
    uint64_t error_traps = 0;
};

extern RefHooks g_ref_hooks;
"""


def main() -> int:
    args = sys.argv[1:]
    fixes_only = "--fixes-only" in args
    args = [a for a in args if a != "--fixes-only"]
    src = pathlib.Path(args[0])
    out = pathlib.Path(args[1])
    if out.exists():
        shutil.rmtree(out)
    shutil.copytree(src, out)
    for name, old, new in FIXES + ([] if fixes_only else EDITS):
        path = out / name
        text = path.read_text()
        count = text.count(old)
        if count != 1:
            print(f"patch_ref: {name}: expected 1 match, found {count}:\n{old}", file=sys.stderr)
            return 1
        path.write_text(text.replace(old, new))
    if fixes_only:
        return 0
    for name, extra in APPEND.items():
        path = out / name
        text = path.read_text()
        first_include = text.index("#include")
        text = text[:first_include] + HOOK_DECLS + text[first_include:]
        path.write_text(text + extra)
    (out / "backend" / "ref_hooks.h").write_text(HOOKS_H)
    return 0


if __name__ == "__main__":
    sys.exit(main())
