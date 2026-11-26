#!/usr/bin/env python3
"""proteus_core with a hand-assembled 68000 program instead of the firmware (no ROMs).

Builds sim/unit_tb.cpp (sim/unit_bench.sv around proteus_core) and runs its tests:
  snd      raw sound-register read straight after a write returns the written word (MAME
           m_regs); pan byte writes without LDS are ignored (MAME umask16(0x00ff))
  macro    autosave does not start while the preset-initialisation macro runs
  osd      control: without the macro, opening the OSD saves
  factory  Factory Reset during a save: the macro waits for the save (CPU not paused under it)
Review 2026-11-26 proteus1.md #3, #5, #15.
Exit status: 0 all passed, 1 a failure, 77 skipped (verilator missing).
"""
import os
import shutil
import subprocess
import sys
import tempfile

CORE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TESTS = ["snd", "macro", "osd", "factory"]


def main():
    if not shutil.which("verilator"):
        print("SKIP: verilator not installed")
        return 77
    tmp = tempfile.mkdtemp(prefix="proteus_unit_")
    try:
        out = os.path.join(CORE, "build", "sim", "unit")   # kept: incremental rebuilds
        tb = subprocess.run([os.path.join(CORE, "sim", "build.sh"), "unit", out],
                            check=True, stdout=subprocess.PIPE, text=True).stdout.strip().splitlines()[-1]
        fails = 0
        for name in TESTS:
            r = subprocess.run([tb, name], cwd=out, stderr=subprocess.PIPE, text=True)
            lines = [ln for ln in r.stderr.splitlines() if "readmem" not in ln]
            if r.returncode:
                fails += 1
                print(f"FAIL {name}:\n  " + "\n  ".join(lines))
            else:
                print(f"ok   {name}: " + "; ".join(lines[:-1]))
        print(f"test_core_unit: {len(TESTS) - fails} passed, {fails} failed")
        return 1 if fails else 0
    finally:
        shutil.rmtree(tmp, ignore_errors=True)


if __name__ == "__main__":
    sys.exit(main())
