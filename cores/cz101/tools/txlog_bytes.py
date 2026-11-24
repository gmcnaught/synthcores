#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""MIDI-out line log -> bytes: "CYCLE BYTE" per byte (CYCLE: the start bit's falling edge).

  txlog_bytes.py [--mame] LOG

LOG: the HDL bench's CZ_TX_LOG ("CYCLE LEVEL"), or with --mame MAME's CZ_TX_LOG ("SECONDS
ATTOSECONDS LEVEL", converted to CPU cycles as floor(T / P)). 31,250 baud, 8N1: each bit is
sampled in its middle; a byte with a low stop bit prints as "CYCLE FE" (framing error).
"""
import bisect
import sys

P = 299_999_940_000          # attoseconds per CPU cycle (3,333,334 Hz)
BIT = 3_333_334 / 31_250     # CPU cycles a bit


def main():
    mame = sys.argv[1] == "--mame"
    path = sys.argv[-1]
    ch = []
    for line in open(path):
        f = line.split()
        if mame:
            t = int(f[0]) * 10**18 + int(f[1]); ch.append((t // P, int(f[2])))
        else:
            ch.append((int(f[0]), int(f[1])))
    cyc = [c for c, _ in ch]
    def level(t):
        i = bisect.bisect_right(cyc, t) - 1
        return 1 if i < 0 else ch[i][1]
    t_end = cyc[-1] if cyc else 0
    i = 0
    while i < len(ch):
        c, lv = ch[i]
        if i == 0 or lv != 0 or ch[i - 1][1] != 1:   # the first record is the reset level
            i += 1
            continue
        bits = [level(c + (k + 1.5) * BIT) for k in range(8)]
        stop = level(c + 9.5 * BIT)
        v = sum(b << k for k, b in enumerate(bits))
        print(f"{c} {v:02X}" if stop else f"{c} FE")
        nxt = c + 9.75 * BIT
        while i < len(ch) and ch[i][0] < nxt:
            i += 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
