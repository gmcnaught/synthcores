#!/usr/bin/env python3
"""Random sound-register log (P2K_SNDLOG format) for the P2K sound engine bench.

Drives G20 voices with random start/loop/end addresses, increments and end-select changes, H level
and cutoff interpolators, routing bytes, approach-time indices and filter coefficient words. It is a
stimulus for comparing p2k/sim/p2k_snd_tb with p2k/model/p2k_render, not firmware behaviour.

  p2k_snd_randlog.py SEED OUT.log [--samples N] [--voices N] [--words W]
"""
import argparse
import random


def h_block(v):
    page = (0x2000, 0, 0x3000, 0x1000)[v & 3]
    return 0x4000 * (v >> 6) + page + 0x80 * ((((v >> 2) & 15) + 15) & 15)


def next_block(blk):
    return blk - 0x780 if (blk & 0x780) == 0x780 else blk + 0x80


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("seed", type=int)
    ap.add_argument("out")
    ap.add_argument("--samples", type=int, default=20000)
    ap.add_argument("--voices", type=int, default=24)
    ap.add_argument("--words", type=int, default=1 << 24, help="sample memory words to address")
    ap.add_argument("--fx", action="store_true", help="also write random G05 effect registers")
    a = ap.parse_args()
    rnd = random.Random(a.seed)
    lines = ["# p2k sndlog 1"]

    def g(s, v, word, data, mask=0xFFFF):
        lines.append(f"{s} G {(v * 32 + word) * 2:04x} {data & 0xFFFF:04x} {mask:04x}")

    def h(s, byte, data, mask=0xFFFF):
        lines.append(f"{s} H {byte:05x} {data & 0xFFFF:04x} {mask:04x}")

    def x(s, ch, word, data, mask=0xFFFF):
        lines.append(f"{s} X {ch:02x} {word * 2:02x} {data & 0xFFFF:04x} {mask:04x}")

    def fx_regs(s, all_regs):
        # the words fx_int::configure reads: INIT1-4 of every channel, PTRX/PSST/CSL of the bus channels,
        # the chorus rate (channel 10 word 13)
        words = [0x14, 0x15, 0x1C, 0x1D]
        for ch in range(32):
            for w in (words if all_regs else [rnd.choice(words)]):
                if all_regs or rnd.random() < 0.1:
                    x(s, ch, w, rnd.getrandbits(16))
        for b in range(4):
            for ch, w in ((4 * b, 0x0A), (4 * b, 0x0B), (4 * b, 0x32), (4 * b, 0x33), (4 * b, 0x3A), (4 * b, 0x3B),
                          (4 * b + 16, 0x0A), (4 * b + 16, 0x0B)):
                if all_regs or rnd.random() < 0.2:
                    x(s, ch, w, rnd.getrandbits(16))
        x(s, 0, 0x14, rnd.choice([0x03FF, 0x0000, rnd.getrandbits(16)]) if not all_regs else rnd.getrandbits(16))
        x(s, 7, 0x14, rnd.choice([0x8474, rnd.getrandbits(16)]))
        x(s, 3, 0x1C, rnd.randrange(0, 40000))         # chorus delay (clamped by configure)
        x(s, 10, 13, rnd.randrange(0, 0x10000))        # chorus rate

    def g32(s, v, word, val):
        g(s, v, word, val >> 16)
        g(s, v, word + 1, val)

    def start_voice(s, v):
        blk = h_block(v)
        start = rnd.randrange(0, a.words - 4096)
        if rnd.random() < 0.05:
            start = rnd.randrange(a.words, 1 << 26)        # reads zeros / far beyond the ends
        length = rnd.choice([1, 2, 3, 7, 16, 100, 1000, 30000])
        loop_end = (start + length) & 0x3FFFFFF
        loop_start = max(0, loop_end - rnd.choice([0, 1, 2, 5, 64, 999, length]))
        if rnd.random() < 0.1:
            loop_start = (loop_end + rnd.randrange(1, 50)) & 0x3FFFFFF   # empty or inverted loop
        other_end = (start + rnd.randrange(1, 5000)) & 0x3FFFFFF
        g(s, v, 1, 0x1800 if rnd.random() < 0.8 else 0x0800)
        g32(s, v, 4, ~other_end & 0x3FFFFFF)                     # +0x08: NOT(end), select 0
        g32(s, v, 6, ~loop_end & 0x3FFFFFF)                      # +0x0C: NOT(end), select 1
        g32(s, v, 10, loop_start | (rnd.getrandbits(1) << 26))   # +0x14: jump address + next select
        route = rnd.choice([0x3F, rnd.randrange(64), rnd.randrange(64), 0x80 | rnd.randrange(64), 0x40 | rnd.randrange(64)])
        h(s, blk + 0x7A, route, 0x00FF)
        for o in (0x34, 0x36, 0x3E):
            h(s, blk + o, rnd.choice([0, 0x0101 * rnd.randrange(256), rnd.getrandbits(16)]))
        for o in (0x30, 0x32, 0x3C):
            h(s, blk + o, rnd.randrange(256) << 8, rnd.choice([0xFF00, 0xFFFF]))
        if rnd.random() < 0.6:       # filter words 0-9 of this block (some neutral)
            for w in range(10):
                val = 0xFFCFFFCF if rnd.random() < 0.4 else rnd.getrandbits(32)
                h(s, blk + 4 * w, val >> 16)
                h(s, blk + 4 * w + 2, val)
        if rnd.random() < 0.3:       # next slot's coefficients just before the start: 12th-order chain
            nb = next_block(blk)
            for w in range(10):
                val = 0xFFCFFFCF if rnd.random() < 0.3 else rnd.getrandbits(32)
                h(s, nb + 4 * w, val >> 16)
                h(s, nb + 4 * w + 2, val)
            h(s, nb + 0x7A, rnd.choice([0x3F, rnd.randrange(256)]), 0x00FF)
        g32(s, v, 12, start)                                     # +0x18/+0x1A: start (restart)
        g(s, v, 2, rnd.choice([0x1000, 0x0800, 0x2000, rnd.randrange(1, 0x10000), rnd.randrange(1, 0x1000)]))

    for page in range(8):
        h(0, page * 0x1000 + 0x44, rnd.randrange(8))
        h(0, page * 0x1000 + 0x46, rnd.randrange(8))
    if a.fx:
        fx_regs(0, True)
    voices = rnd.sample(range(128), a.voices)
    for v in voices:
        start_voice(rnd.randrange(0, 50), v)
    s = 50
    while s < a.samples:
        s += rnd.randrange(1, 400)
        v = rnd.choice(voices)
        r = rnd.random()
        if r < 0.3:
            start_voice(s, v)
        elif r < 0.5:
            g(s, v, 2, rnd.randrange(0, 0x10000))
        elif r < 0.6:
            g(s, v, 1, rnd.getrandbits(16), rnd.choice([0xFFFF, 0xFF00, 0x00FF]))
        elif r < 0.8:
            blk = h_block(v)
            o = rnd.choice([0x30, 0x32, 0x3C, 0x70, 0x72, 0x7C, 0x34, 0x36, 0x3E])
            h(s, blk + o, rnd.getrandbits(16), rnd.choice([0xFFFF, 0xFF00, 0x00FF]))
        elif r < 0.85:
            page = rnd.randrange(8)
            h(s, page * 0x1000 + rnd.choice([0x44, 0x46]), rnd.getrandbits(16))
        elif r < 0.9:
            h(s, h_block(v) + 0x7A, rnd.getrandbits(8), 0x00FF)
        elif a.fx and r < 0.93:
            fx_regs(s, False)
        else:
            g32(s, v, 10, rnd.randrange(0, a.words) | (rnd.getrandbits(1) << 26))
    with open(a.out, "w") as f:
        f.write("\n".join(lines) + "\n")


if __name__ == "__main__":
    main()
