#!/usr/bin/env python3
"""mfp68901 against MAME's mc68901 behaviour (patch 0001) at corners the Proteus can reach.

Builds sim/mfp_tb.cpp (the MFP alone) and runs its tests (review 2026-11-26 proteus1.md):
  ipr      timer interrupts are not lost when IERB/IPRB are written in the same clock (#4)
  tdr      TxDR loads the counter while a pulse-width timer's input is inactive (#11)
  overrun  USART overrun held back until the UDR read, then OE + receive-error interrupt (#12)
Exit status: 0 all passed, 1 a failure, 77 skipped (verilator missing).
"""
import os
import shutil
import subprocess
import sys
import tempfile

CORE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TESTS = ["ipr", "tdr", "overrun"]


def main():
    if not shutil.which("verilator"):
        print("SKIP: verilator not installed")
        return 77
    tmp = tempfile.mkdtemp(prefix="proteus_mfp_")
    try:
        tb = subprocess.run([os.path.join(CORE, "sim", "build.sh"), "mfp", os.path.join(tmp, "build")],
                            check=True, stdout=subprocess.PIPE, text=True).stdout.strip().splitlines()[-1]
        fails = 0
        for name in TESTS:
            r = subprocess.run([tb, name], cwd=tmp, stderr=subprocess.PIPE, text=True)
            lines = r.stderr.strip().splitlines()
            if r.returncode:
                fails += 1
                print(f"FAIL {name}:\n  " + "\n  ".join(lines))
            else:
                print(f"ok   {name}: " + "; ".join(lines[:-1]))
        print(f"test_mfp: {len(TESTS) - fails} passed, {fails} failed")
        return 1 if fails else 0
    finally:
        shutil.rmtree(tmp, ignore_errors=True)


if __name__ == "__main__":
    sys.exit(main())
