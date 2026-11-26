#!/usr/bin/env python3
"""tools/compare_writes.py exit status on synthetic MAME / HDL write traces (review 2026-11-26
proteus1.md#10): identical streams pass; a difference in a device that must match (snd, lcd,
pan, other) fails; RAM and MFP differences fail only with --ram (resp. --mfp); empty or
device-less traces fail. Exit status: 0 passed, 1 failed.
"""
import os
import subprocess
import sys
import tempfile

CORE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TOOL = os.path.join(CORE, "tools", "compare_writes.py")

# (addr, data, mask) writes in MAME's format; the HDL file gets the same with ds bits
BASE = [(0x700000, 0x0038, 0x00FF), (0x800020, 0x1234, 0xFFFF), (0x500009, 0x0007, 0x00FF),
        (0xFF1000, 0xABCD, 0xFFFF), (0x900007, 0x0020, 0x00FF), (0x400000, 0x0001, 0x00FF),
        (0x700002, 0x0041, 0x00FF), (0x800022, 0x5678, 0xFFFF)]


def write(path, recs, hdl):
    with open(path, "w") as f:
        for i, (a, d, m) in enumerate(recs):
            last = (2 if m & 0xFF00 else 0) | (1 if m & 0x00FF else 0) if hdl else m
            f.write(f"{i * 0.001:.6f} {a:06x} {d:04x} {last if hdl else format(last, '04x')}\n")


def changed(addr, data):
    return [(a, data if a == addr else d, m) for a, d, m in BASE]


def main():
    cases = [
        ("identical", BASE, BASE, [], 0),
        ("sound write differs", BASE, changed(0x800022, 0x5679), [], 1),
        ("LCD write differs", BASE, changed(0x700002, 0x0042), [], 1),
        ("extra trailing HDL write", BASE, BASE + [(0x800024, 1, 0xFFFF)], [], 1),
        ("RAM differs, default", BASE, changed(0xFF1000, 0xABCE), [], 0),
        ("RAM differs, --ram", BASE, changed(0xFF1000, 0xABCE), ["--ram"], 1),
        ("MFP differs, default", BASE, changed(0x900007, 0x0021), [], 0),
        ("MFP differs, --mfp", BASE, changed(0x900007, 0x0021), ["--mfp"], 1),
        ("empty HDL trace", BASE, [], [], 1),
        ("no sound or LCD writes", [BASE[3]], [BASE[3]], [], 1),
    ]
    fails = 0
    with tempfile.TemporaryDirectory() as tmp:
        ma, hd = os.path.join(tmp, "mame.txt"), os.path.join(tmp, "hdl.txt")
        for name, a, b, opts, want in cases:
            write(ma, a, False)
            write(hd, b, True)
            r = subprocess.run([sys.executable, TOOL, ma, hd] + opts, stdout=subprocess.PIPE,
                               stderr=subprocess.STDOUT, text=True)
            ok = (r.returncode != 0) == bool(want)
            fails += not ok
            print(f"{'ok  ' if ok else 'FAIL'} {name}: exit {r.returncode}")
    print(f"test_compare_writes: {len(cases) - fails} passed, {fails} failed")
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
