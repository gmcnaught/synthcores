#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Decode MAME's CZ_RX_LOG (MIDI-in line changes) into bytes with their start cycles.

    tools/rxlog_bytes.py RXLOG > BYTES      lines "CYCLE BYTE": CYCLE = start bit time / P

Used to feed the same MIDI stream to the board's live-MIDI serializer (CZ_MIDI_BYTES).
"""
import sys

P = 299_999_940_000          # attoseconds per CPU cycle
BIT = 32_000_000_000_000     # 32 us at 31,250 baud, in attoseconds

ch = []
for line in open(sys.argv[1]):
    s, a, lv = line.split()
    ch.append((int(s) * 10**18 + int(a), int(lv)))


def level(t):
    lv = 1
    for tc, l in ch:
        if tc > t:
            break
        lv = l
    return lv


i = 0
while i < len(ch):
    t0, lv = ch[i]
    if lv != 0:
        i += 1
        continue
    b = 0
    for k in range(8):
        b |= level(t0 + BIT * (k + 1) + BIT // 2) << k
    print(t0 // P, f"{b:02X}")
    end = t0 + BIT * 9 + BIT // 2           # middle of the stop bit
    while i < len(ch) and ch[i][0] <= end:
        i += 1
