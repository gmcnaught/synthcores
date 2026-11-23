#!/usr/bin/env python3
"""Copy the reference backend, apply FIXES, and add observation hooks for the lockstep benches.

FIXES go into both builds (the plain one with --fixes-only). They change behaviour only where the
fork departs from Nuked-SC55's original:
  - addclip20 saturates at 20 bits again. jcmoyer/Nuked-SC55 7204125 replaced the original
    (nukeykt) saturating add with a plain sign-extended add, noting that if these functions do
    overflow "their outputs simply need to saturate at 20 ... bits". The JV-880 overflows in
    loud chords (the 20-bit mix wrapped from full negative to full positive); the SC-55 and
    JV-880 integration-test hashes are unchanged.

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
     "inline int32_t addclip20(int32_t add1, int32_t add2, int32_t cin)\n"
     "{\n"
     "    // saturate at 20 bits, as Nuked-SC55's original (ref/patch_ref.py FIXES)\n"
     "    const int32_t sum = sx20(add1) + sx20(add2) + cin;\n"
     "    return sum > 0x7ffff ? 0x7ffff : sum < -0x80000 ? -0x80000 : sum;\n"
     "}"),
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
