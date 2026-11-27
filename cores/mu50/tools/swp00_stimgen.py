#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Random SWP00 stimulus for the U3 lockstep: an H8 program that drives the SWP00, and a wave ROM.

Usage: swp00_stimgen.py SEED PROGRAM.bin WAVE.bin [--seconds S] [--variant V]

PROGRAM.bin runs on MAME's mu50 machine in test-ROM mode (tools/mu50_testrom.py --wave WAVE.bin):
straight-line H8/300H code that writes SWP00 registers (byte and word writes), reads them back
(state_r, waverom_val, channel and MEG registers) and waits in delay loops between events, then
loops forever. MAME's MU_SND_LOG / MU_SND_RLOG / MU_SAMPLES of that run are the reference.

WAVE.bin is 4 MB of seeded pseudo-random bytes, so every sample format decodes something.

The events, all with seeded random values: the MEG variant (V, default SEED % 4) and all MEG
constants and offsets at the start, some changed later; notes on random channels with random
format (16-bit, 12-bit, 8-bit, compressed with any scale and mode, loop or one-shot), pitch,
phase, envelope, filter (with and without sweep), LFO and mixer levels; parameter changes while
notes play; releases; state and register reads.
"""
import argparse
import random
import struct
from pathlib import Path

SWP = 0x400000
STATES_PER_ITER = 6          # dec.w #1 + bne: 2 + 4 states (MAME)
SAMPLE_STATES = 1e7 / 44100


class Prog:
    def __init__(self):
        self.code = bytearray()
        self.states = 0.0        # rough time, for the length of the run

    def wb(self, reg, val):     # mov.b #val, r0l ; mov.b r0l, @SWP+reg
        self.code += bytes([0xF8, val & 0xff]) + bytes([0x6A, 0xA8]) + struct.pack(">I", SWP + reg)
        self.states += 2 + 8

    def ww(self, reg, val):     # mov.w #val, r0 ; mov.w r0, @SWP+reg (reg even)
        self.code += bytes([0x79, 0x00]) + struct.pack(">H", val & 0xffff)
        self.code += bytes([0x6B, 0xA0]) + struct.pack(">I", SWP + (reg & 0x7fe))
        self.states += 4 + 8

    def rb(self, reg):          # mov.b @SWP+reg, r0l
        self.code += bytes([0x6A, 0x28]) + struct.pack(">I", SWP + reg)
        self.states += 8

    def wait(self, iters):      # mov.w #n, r1 ; 1: dec.w #1, r1 ; bne 1b
        if iters <= 0:
            return
        self.code += bytes([0x79, 0x01]) + struct.pack(">H", iters) + bytes([0x1B, 0x51, 0x46, 0xFC])
        self.states += 4 + STATES_PER_ITER * iters


def chan_reg(idx, ch):
    """Register address of channel register idx (swp00_device::map rchan) for channel ch."""
    return ((idx & 0x3e) << 5) | (idx & 1) | (ch << 1)


class Gen:
    def __init__(self, seed, variant, wild=False):
        self.r = random.Random(seed)
        self.p = Prog()
        self.variant = variant
        self.wild = wild

    def const_val(self):
        """A MEG constant. Moderate by default: with large gains the effects' feedback overflows
        s32, where MAME's C++ is undefined (U3 compares those runs with the wrapping model only)."""
        r = self.r
        if self.wild:
            k = r.random()
            if k < 0.6:
                return r.randrange(-0x120, 0x120) & 0x3ff
            return r.randrange(0x400) if k < 0.9 else r.randrange(0x10000)
        return r.randrange(-0x50, 0x50) & 0x3ff

    def meg_setup(self):
        p, r = self.p, self.r
        p.wb(0x004, (self.variant << 6) | (r.randrange(4) & 0x3c))
        for i in range(0x40):              # offsets 180-1FF, word writes
            p.ww(0x180 + 2 * i, r.randrange(0x10000))
        for i in range(0xc0):              # constants 200-37F, byte writes, high then low
            v = self.const_val()
            p.wb(0x200 + 2 * i, v >> 8)
            p.wb(0x201 + 2 * i, v)

    def note(self, ch):
        p, r = self.p, self.r
        fmt = r.choice([0x00, 0x40, 0x80, 0xc0]) | r.randrange(0x40)
        start = r.choice([0, r.randrange(0x40), r.randrange(0x1000)])
        loop = r.choice([0, r.randrange(1, 0x100), r.randrange(0x100, 0x8000)])
        e = r.choice([0xd, 0xe, 0xf, 0x0, 0x0, 0x1, 0x2]) if r.random() < 0.9 else r.randrange(16)
        pitch = (e << 12) | r.randrange(0x1000)
        regs = [
            (0x08, r.randrange(256)), (0x09, r.randrange(256)),            # phase
            (0x0a, start >> 8), (0x0b, start & 0xff),
            (0x30, fmt),
            (0x31, r.randrange(256)), (0x32, r.randrange(256)), (0x33, r.randrange(256)),
            (0x34, pitch >> 8), (0x35, pitch & 0xff),
            (0x36, loop >> 8), (0x37, loop & 0xff),
            (0x20, r.randrange(256)), (0x21, r.randrange(256)),            # lpf q, k
            (0x22, r.randrange(256)),                                      # lpf sweep, speed
            (0x23, r.randrange(256)),                                      # lfo lpf/amp levels
            (0x24, r.choice([0x80, r.randrange(256)])),                    # lfo speed/shape
            (0x25, r.randrange(256)),                                      # lfo pitch level
            (0x26, r.randrange(256)), (0x27, r.randrange(256)),            # attack speed, level
            (0x28, r.randrange(0x80) if r.random() < 0.85 else r.randrange(256)),
            (0x29, r.randrange(256)),                                      # decay level
            (0x2a, r.randrange(0x80)), (0x2b, r.randrange(0x60)),          # rev, dry
            (0x2c, r.randrange(0x80)), (0x2d, r.randrange(0x80)),          # cho, var
            (0x2e, r.randrange(0x60)), (0x2f, r.randrange(256)),           # glo, pan
        ]
        r.shuffle(regs)
        for idx, v in regs:
            p.wb(chan_reg(idx, ch), v)
        p.wb(0x008 + (3 - ch // 8), 1 << (ch % 8))                         # keyon

    def change(self, ch):
        p, r = self.p, self.r
        idx = r.choice([0x34, 0x35, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x29, 0x2a, 0x2b, 0x2c, 0x2d,
                        0x2e, 0x2f, 0x36, 0x37, 0x28])
        if idx == 0x28:     # release: decay with bit 7, target ff
            p.wb(chan_reg(0x29, ch), 0xff)
            p.wb(chan_reg(0x28, ch), 0x80 | r.randrange(0x80))
        else:
            p.wb(chan_reg(idx, ch), r.randrange(256))

    def reads(self):
        p, r = self.p, self.r
        k = r.random()
        if k < 0.5:
            p.wb(0x001, r.randrange(256))       # state_adr
            for _ in range(r.randrange(1, 4)):
                p.rb(0x001)
        elif k < 0.7:
            for i, v in ((0x31, r.randrange(256)), (0x32, r.randrange(256)), (0x33, r.randrange(256))):
                p.wb(chan_reg(i, 31), v)
            for _ in range(r.randrange(1, 4)):
                p.rb(0x003)                     # waverom_val: channel 1f's address counts up
        elif k < 0.85:
            p.rb(chan_reg(r.choice([0x08, 0x0b, 0x20, 0x21, 0x26, 0x2f, 0x30, 0x33, 0x34, 0x37]),
                          r.randrange(32)))
        else:
            p.rb(r.choice([0x180, 0x1ff, 0x200, 0x37f, 0x004, 0x002]) + r.randrange(2) * 0)

    def run(self, seconds):
        p, r = self.p, self.r
        self.meg_setup()
        p.wait(200)
        end = seconds * 1e7
        while p.states < end and len(p.code) < 0x7f000:
            k = r.random()
            if k < 0.30:
                self.note(r.randrange(32))
            elif k < 0.70:
                self.change(r.randrange(32))
            elif k < 0.85:
                self.reads()
            elif k < 0.88:
                i = r.randrange(0xc0)
                v = self.const_val()
                p.ww(0x200 + 2 * i, v)
            elif k < 0.89:
                p.ww(0x180 + 2 * r.randrange(0x40), r.randrange(0x10000))
            p.wait(r.choice([0, 20, 40, 150, 500, r.randrange(3000)]))
        p.code += bytes([0x40, 0xFE])           # bra $
        return p


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("seed", type=int)
    ap.add_argument("program", type=Path)
    ap.add_argument("wave", type=Path)
    ap.add_argument("--seconds", type=float, default=1.0)
    ap.add_argument("--variant", type=int, default=None)
    ap.add_argument("--wild", action="store_true", help="any MEG constants (s32 overflow likely)")
    ap.add_argument("--wave-only", action="store_true", help="write only WAVE.bin")
    a = ap.parse_args()
    if a.wave_only:
        a.wave.write_bytes(random.Random(a.seed ^ 0x5a5a5a).randbytes(0x400000))
        return
    g = Gen(a.seed, a.seed % 4 if a.variant is None else a.variant, a.wild)
    p = g.run(a.seconds)
    img = bytearray(0x100)
    struct.pack_into(">I", img, 0, 0x100)       # reset vector
    img += p.code
    a.program.write_bytes(bytes(img))
    a.wave.write_bytes(random.Random(a.seed ^ 0x5a5a5a).randbytes(0x400000))


if __name__ == "__main__":
    main()
