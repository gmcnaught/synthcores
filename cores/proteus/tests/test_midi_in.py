#!/usr/bin/env python3
"""midi_in in front of the MFP: no MIDI byte is lost while the CPU does not read UDR.

Builds sim/midi_tb.cpp (synthframe_midi_rx + midi_in + mfp68901, as in proteus_sys) and sends
400 bytes back to back while the CPU stops reading for 60 ms (as during nv_ctrl's save pause).
  midi_in   every byte arrives, in order, with no overrun
  bypass    the line straight into the MFP (the wiring before midi_in) must lose bytes, so the
            test can tell the two apart
Exit status: 0 passed, 1 a failure, 77 skipped (verilator missing).
"""
import os
import shutil
import subprocess
import sys

CORE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def main():
    if not shutil.which("verilator"):
        print("SKIP: verilator not installed")
        return 77
    tb = subprocess.run([os.path.join(CORE, "sim", "build.sh"), "midi", os.path.join(CORE, "build", "sim", "midi")],
                        check=True, stdout=subprocess.PIPE, text=True).stdout.strip().splitlines()[-1]
    fails = 0
    for mode, want in (("midi_in", 0), ("bypass", 1)):
        r = subprocess.run([tb] + ([mode] if mode == "bypass" else []), stderr=subprocess.PIPE, text=True)
        line = r.stderr.strip().splitlines()[-1] if r.stderr.strip() else "(no output)"
        if r.returncode != want:
            fails += 1
            print(f"FAIL {mode}: {line} (exit {r.returncode}, want {want})")
        else:
            print(f"ok   {mode}: {line}")
    print(f"test_midi_in: {2 - fails} passed, {fails} failed")
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
