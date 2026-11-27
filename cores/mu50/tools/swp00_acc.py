#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Compact SWP00 access list from MAME's logs, for the committed U3 cases.

Usage: swp00_acc.py SND_LOG RD_LOG NSAMPLES OUT.acc

Merges MU_SND_LOG (writes) and MU_SND_RLOG (reads) in CPU order, splits 16-bit accesses into
bytes (the even address first) and keeps those that act before sample NSAMPLES. One line per
byte access:  "<samples since the previous line> <address hex> <data hex>"  for a write,
"<samples since the previous line> r<address hex>" for a read. swp00_bench --acc reads it.
"""
import sys


def main():
    snd, rd, n, out = sys.argv[1], sys.argv[2], int(sys.argv[3]), sys.argv[4]
    ev = []
    for kind, fn in ((0, snd), (1, rd)):
        with open(fn) as f:
            for line in f:
                p = line.split()
                if len(p) == 5:
                    ev.append((int(p[0]), kind, int(p[1]), int(p[2], 16), int(p[3], 16), int(p[4], 16)))
    ev.sort(key=lambda e: e[0])
    prev, lines = 0, []
    for _, kind, before, adr, data, mask in ev:
        if before >= n:
            break
        for lane in (0, 1):
            if mask & (0xff00 if lane == 0 else 0x00ff):
                a = (adr & 0x7fe) | lane
                if kind == 0:
                    lines.append("%d %x %02x" % (before - prev, a, (data >> (8 if lane == 0 else 0)) & 0xff))
                else:
                    lines.append("%d r%x" % (before - prev, a))
                prev = before
    with open(out, "w") as f:
        f.write("\n".join(lines) + "\n")


if __name__ == "__main__":
    main()
