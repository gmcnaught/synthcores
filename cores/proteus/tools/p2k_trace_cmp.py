#!/usr/bin/env python3
"""Compare the MAME bus trace (P2K_BUSTRACE, binary) with the RTL testbench trace (text).

MAME records: u32 address (4-byte aligned), u32 data, u32 mask, u32 PC|write. Expanded to 16-bit
word accesses (high half first). Dropped: MAME opcode fetches from flash (reads within 16 bytes of
the PC; MAME's DRAM fetches are not traced) and RTL accesses to the MCF5206e registers at MBAR
(MAME logs those in another address space). Long accesses may be split in either word order; the
comparison accepts a swapped adjacent pair.

  p2k_trace_cmp.py MAME.bt RTL.trace [--context N] [--max-mame N] [--nodram]
  --nodram: ignore DRAM accesses on the RTL side (MAME trace made with P2K_BUSTRACE_NODRAM=1)
"""
import argparse
import struct


def mame_words(path, limit):
    with open(path, "rb") as f:
        n = 0
        while True:
            rec = f.read(16)
            if len(rec) < 16 or (limit and n >= limit):
                return
            n += 1
            a, d, m, pc = struct.unpack("<4I", rec)
            w = pc & 1
            pc &= ~1
            for half, shift, mk in ((0, 16, m >> 16), (2, 0, m & 0xFFFF)):
                if not mk:
                    continue
                wa = a + half
                lanes = (2 if mk & 0xFF00 else 0) | (1 if mk & 0x00FF else 0)
                if not w and (wa < 0x200000 or 0x02000000 <= wa < 0x02200000) and abs(wa - pc) <= 16:
                    continue                                  # opcode fetch from flash
                yield ("W" if w else "R", wa, (d >> shift) & 0xFFFF, lanes, pc)


NODRAM = False


def rtl_words(path):
    with open(path) as f:
        for line in f:
            p = line.split()
            wa = int(p[1], 16)
            if 0x20000000 <= wa < 0x20000400:
                continue                                      # MBAR registers
            if wa < 8 and p[0] == "R":
                continue                                      # reset vectors (MAME: treated as fetches)
            if NODRAM and 0x01000000 <= wa < 0x01800000:
                continue
            lf = int(p[4], 16)
            if p[0] == "R" and (wa < 0x200000 or 0x02000000 <= wa < 0x02200000) and abs(wa - lf) <= 16:
                continue                                      # same filter as MAME's fetch heuristic
            yield (p[0], wa, int(p[2], 16), int(p[3]), int(p[4], 16))


def near_fetch(t):
    """Flash read within 32 bytes of the PC: an opcode fetch or a PC-relative table read, which the
    two traces' fetch filters can classify differently."""
    return t[0] == "R" and (t[1] < 0x200000 or 0x02000000 <= t[1] < 0x02200000) and abs(t[1] - t[4]) <= 32


POLLED = {0x00400000}     # LCD busy flag: polled a timing-dependent number of times


def same(x, y):
    if x[0] != y[0] or x[1] != y[1]:
        return False
    lanes = x[3] & y[3]
    mask = (0xFF00 if lanes & 2 else 0) | (0x00FF if lanes & 1 else 0)
    return (x[2] & mask) == (y[2] & mask)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("mame")
    ap.add_argument("rtl")
    ap.add_argument("--context", type=int, default=8)
    ap.add_argument("--max-mame", type=int, default=0)
    ap.add_argument("--nodram", action="store_true")
    a = ap.parse_args()
    global NODRAM
    NODRAM = a.nodram
    M = mame_words(a.mame, a.max_mame)
    R = rtl_words(a.rtl)
    hist = []
    i = 0
    pend_m, pend_r = [], []
    fmt = lambda t: f"{t[0]} {t[1]:08X} {t[2]:04X} l{t[3]} pc {t[4]:08X}"
    while True:
        while len(pend_m) < 2:
            x = next(M, None)
            if x is None:
                break
            pend_m.append(x)
        while len(pend_r) < 2:
            y = next(R, None)
            if y is None:
                break
            pend_r.append(y)
        if not pend_m or not pend_r:
            print(f"matched {i} word accesses; end of {'MAME' if not pend_m else 'RTL'} trace")
            return 0
        if same(pend_m[0], pend_r[0]):
            hist.append(pend_m[0]); pend_m.pop(0); pend_r.pop(0); i += 1
        elif len(pend_m) > 1 and len(pend_r) > 1 and same(pend_m[0], pend_r[1]) and same(pend_m[1], pend_r[0]):
            hist += pend_m[:2]; pend_m = pend_m[2:]; pend_r = pend_r[2:]; i += 2
        elif pend_r[0][0] == "R" and pend_r[0][1] in POLLED:
            pend_r.pop(0)
        elif pend_m[0][0] == "R" and pend_m[0][1] in POLLED:
            pend_m.pop(0)
        elif near_fetch(pend_r[0]):
            pend_r.pop(0)                                     # PC-relative read the other side filtered
        elif near_fetch(pend_m[0]):
            pend_m.pop(0)
        else:
            print(f"DIVERGENCE after {i} matched word accesses")
            for t in hist[-a.context:]:
                print("   ok   ", fmt(t))
            print("   MAME ", fmt(pend_m[0]))
            print("   RTL  ", fmt(pend_r[0]))
            return 1
        if len(hist) > 1000:
            hist = hist[-a.context:]


if __name__ == "__main__":
    raise SystemExit(main())
