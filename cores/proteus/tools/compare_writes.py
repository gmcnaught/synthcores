#!/usr/bin/env python3
"""Compare CPU write streams from MAME (mame/scripts/trace_writes.lua) and the HDL system
simulation (sim/sys_main.cpp), per device, in order.

MAME lines: "<t> <addr> <data> <mask>" (word-aligned address, mask ffff/ff00/00ff).
HDL lines:  "<t> <addr> <data> <ds>"   (ds bit1 = UDS, bit0 = LDS).

Usage: compare_writes.py MAME.txt HDL.txt [--ram]
Prints, per device, the length of the common prefix and the first difference.
"""
import sys

DEVICES = [("ram", 0xFF0000, 0xFFFFFF), ("lcd", 0x700000, 0x700003), ("snd", 0x800000, 0x8003FF),
           ("pan", 0x500000, 0x50004F), ("mfp", 0x900000, 0x90002F), ("other", 0, 0xFFFFFF)]


def norm(addr, data, mask):
    data &= mask
    return (addr & ~1, data, mask)


def load(path, hdl):
    out = {name: [] for name, _, _ in DEVICES}
    for line in open(path):
        p = line.split()
        if len(p) < 4:
            continue
        t, addr, data = float(p[0]), int(p[1], 16), int(p[2], 16)
        if hdl:
            ds = int(p[3])
            mask = (0xFF00 if ds & 2 else 0) | (0x00FF if ds & 1 else 0)
        else:
            mask = int(p[3], 16)
        key = norm(addr, data, mask)
        for name, lo, hi in DEVICES:
            if lo <= addr <= hi:
                out[name].append((t, key))
                break
    return out


def main():
    a = load(sys.argv[1], False)
    b = load(sys.argv[2], True)
    for name, _, _ in DEVICES:
        x, y = a[name], b[name]
        n = 0
        while n < min(len(x), len(y)) and x[n][1] == y[n][1]:
            n += 1
        status = "identical" if n == len(x) == len(y) else f"first {n} match"
        print(f"{name:5s}: MAME {len(x):6d}  HDL {len(y):6d}  {status}")
        if n < min(len(x), len(y)):
            for k in range(max(0, n - 2), min(n + 4, len(x), len(y))):
                (tx, kx), (ty, ky) = x[k], y[k]
                mark = "<<" if kx != ky else "  "
                print(f"   {mark} MAME {tx:.6f} {kx[0]:06x} {kx[1]:04x}/{kx[2]:04x}   HDL {ty:.6f} {ky[0]:06x} {ky[1]:04x}/{ky[2]:04x}")


if __name__ == "__main__":
    main()
