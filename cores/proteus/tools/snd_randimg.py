#!/usr/bin/env python3
"""Random demo image for the Proteus/1 sound-chip bench (no ROMs needed).

Writes a tools/make_demo.py image: 8 MB of random sample words, then an event stream that
programs all 32 voices several times with random start, pitch (forward and reverse, all 8 tone
values, step-on-read bit 18), loops, envelope target/rate/mode and pan, partly as byte writes
(one byte lane, MAME COMBINE_DATA), with random waits between blocks. At most 60 writes go into
one output sample, so the core's 64-entry write FIFO never fills (sim/snd_tb.cpp checks it).

Usage: snd_randimg.py OUT.bin SEED [--edge] [--blocks N]
  --edge: voices 0-3 start in the last 16 words of the 4M-word sample space, so their
          8-word windows wrap to word 0 (MAME mirrors the sample ROM; review 2026-11-26 #6).
Prints the number of output samples the event stream covers.
"""
import argparse
import random
import struct

MAX_PER_TICK = 60


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("out")
    ap.add_argument("seed", type=int)
    ap.add_argument("--edge", action="store_true")
    ap.add_argument("--blocks", type=int, default=6)
    a = ap.parse_args()
    rnd = random.Random(a.seed)
    rom = bytes(rnd.getrandbits(8) for _ in range(0x800000)) if not hasattr(rnd, "randbytes") \
        else rnd.randbytes(0x800000)

    ev = []
    n_tick = [0]

    def push(e):
        if n_tick[0] == MAX_PER_TICK:
            ev.append(1)  # WAIT 1: continue in the next output sample
            n_tick[0] = 0
        ev.append(e)
        n_tick[0] += 1

    def w(v, word, d):
        skip = 0
        if rnd.random() < 0.08:
            skip = rnd.choice((0x10000, 0x20000))  # byte write: one lane not written
        push(0x80000000 | (v << 25) | (word << 21) | skip | (d & 0xFFFF))

    def wl(v, word, d):
        order = (0, 1) if rnd.random() < 0.8 else (1, 0)
        for k in order:
            w(v, word + k, d >> 16 if k == 0 else d)

    def pan(v, p):
        push(0xC0000000 | (v << 25) | (p & 0xFF))

    for blk in range(a.blocks):
        for v in range(32):
            if blk and rnd.random() < 0.5:
                continue
            if a.edge and v < 4:
                addr = 0x3FFFF0 + rnd.randrange(16)
            else:
                addr = rnd.randrange(0, 0x3FFF00)
            rev = rnd.random() < 0.3
            inc = rnd.randrange(0, 0x20000)
            if rev:
                inc = (-inc) & 0x3FFFF
            tone = rnd.randrange(8)
            pitch = (tone << 21) | inc | (0x40000 if rnd.random() < 0.05 else 0)
            ls = (addr + rnd.randrange(-2000, 2000)) & 0xFFFFFF
            le = (addr + rnd.randrange(-2000, 2000)) & 0xFFFFFF
            if a.edge and v < 4:
                ls, le = addr - rnd.randrange(64), addr + 8 + rnd.randrange(32)  # loop across the end
                le &= 0xFFFFFF
            lvl, rate = rnd.randrange(4096), rnd.randrange(512)
            wl(v, 0, addr)
            wl(v, 2, pitch)
            wl(v, 8, ls)
            wl(v, 10, ~le & 0xFFFFFFFF)
            wl(v, 4, (lvl << 12) | 0x200 | rate)
            wl(v, 4, (rnd.randrange(4096) << 12) | rnd.randrange(512))
            pan(v, rnd.randrange(256))
        n = rnd.randrange(100, 400)
        ev.append(n)
        n_tick[0] = 0
        # a few writes spread over the following samples (envelope changes, pan moves)
        for _ in range(rnd.randrange(0, 6)):
            v = rnd.randrange(32)
            if rnd.random() < 0.5:
                pan(v, rnd.randrange(256))
            else:
                wl(v, 4, (rnd.randrange(4096) << 12) | rnd.randrange(512))
            ev.append(rnd.randrange(1, 20))
            n_tick[0] = 0
    ev.append(0)  # END
    with open(a.out, "wb") as f:
        f.write(rom)
        f.write(b"".join(struct.pack("<I", e) for e in ev))
    print(sum(e for e in ev if e and not e & 0x80000000))


if __name__ == "__main__":
    main()
