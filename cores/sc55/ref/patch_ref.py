#!/usr/bin/env python3
"""Copy the reference backend and add observation hooks for the lockstep benches.

The hooks only observe: with no hook installed, the patched backend behaves
exactly like the original (checked by tests/test_ref_patch.py, which renders
with both builds and compares the audio).

Usage: patch_ref.py NUKED_SRC_DIR OUT_SRC_DIR   (copies all of src/, patches src/backend)
"""
import pathlib
import shutil
import sys

HOOK_DECLS = """
// ---- sc55-MiSTer lockstep hooks (added by ref/patch_ref.py) ----
#include "ref_hooks.h"
"""

# (file, old, new): every `old` must occur exactly once.
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
    src = pathlib.Path(sys.argv[1])
    out = pathlib.Path(sys.argv[2])
    if out.exists():
        shutil.rmtree(out)
    shutil.copytree(src, out)
    for name, old, new in EDITS:
        path = out / name
        text = path.read_text()
        count = text.count(old)
        if count != 1:
            print(f"patch_ref: {name}: expected 1 match, found {count}:\n{old}", file=sys.stderr)
            return 1
        path.write_text(text.replace(old, new))
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
