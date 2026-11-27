#!/usr/bin/env python3
"""Reference model of the Proteus/1 sound chip, driven by a demo image (tools/make_demo.py).

A line-by-line port of third_party/mame/src/mame/emusys/proteus_snd.cpp (the MAME model);
the HDL (mister/rtl/proteus_snd.sv) must match its output sample for sample.

Usage: proteus_model.py DEMO.bin OUT.raw [--ticks N] [--wav OUT.wav] [--no-tone]
OUT.raw: per output sample, two little-endian s16 (left, right).
"""
import pathlib
import argparse
import array
import sys
import wave

from interp_kernels_data import KERNELS

# per tone: K[a] and K[a+1] - K[a] for a = 0..128 (tools/interp_kernels.py)
KERN = [(k, [b - a for a, b in zip(k, k[1:] + [0])]) for k in KERNELS]

ADDR_MASK = 0xFFFFFF
SAMPLE_BYTES = 0x800000


def sext(v, bits):
    return v - (1 << bits) if v & (1 << (bits - 1)) else v


def level_to_gain(level):
    e = (level >> 9) & 7
    m = level & 0x1FF
    return ((0x200 | m) << (e - 1)) if e else m


class Voice:
    __slots__ = ("addr", "frac", "pitch", "env", "loop_start", "loop_end", "level", "pan")

    def __init__(self):
        self.addr = self.frac = self.pitch = self.env = 0
        self.loop_start = self.loop_end = self.level = 0
        self.pan = 0x08


class Chip:
    def __init__(self, rom_words, tone=True):
        self.rom = rom_words  # array('h') of signed words
        self.tone = tone
        self.mask = len(rom_words) - 1
        self.regs = [0] * (32 * 16)
        self.v = [Voice() for _ in range(32)]

    def reg_long(self, v, w):
        return (self.regs[v * 16 + w] << 16) | self.regs[v * 16 + w + 1]

    def write(self, v, w, data, mask=0xFFFF):
        # MAME COMBINE_DATA: only the byte lanes in mask are written; the voice state is
        # rebuilt from the merged words either way
        i = v * 16 + w
        self.regs[i] = (self.regs[i] & ~mask & 0xFFFF) | (data & mask)
        vc = self.v[v]
        if w in (0, 1):
            vc.addr = self.reg_long(v, 0) & ADDR_MASK
            vc.frac = 0
        elif w in (2, 3):
            vc.pitch = self.reg_long(v, 2)
        elif w in (4, 5):
            vc.env = self.reg_long(v, 4)
            if w == 5 and vc.env & 0x200:
                vc.level = (vc.env >> 12) & 0xFFF
        elif w in (8, 9):
            vc.loop_start = self.reg_long(v, 8) & ADDR_MASK
        elif w in (10, 11):
            vc.loop_end = ~self.reg_long(v, 10) & ADDR_MASK

    def pan(self, v, data):
        self.v[v].pan = data

    def step(self, vc):
        # 18-bit signed increment: integer part bits 17..12 (signed), fraction bits 11..0
        reverse = vc.pitch & 0x20000
        whole = sext((vc.pitch >> 12) & 0x3F, 6)
        pos = (vc.frac & 0xFFF) + (vc.pitch & 0xFFF)
        vc.frac = pos & 0xFFF
        vc.addr = (vc.addr + whole + (pos >> 12)) & ADDR_MASK
        if reverse:
            passed = vc.addr < vc.loop_end and vc.loop_end < vc.loop_start
        else:
            passed = vc.addr > vc.loop_end and vc.loop_end >= vc.loop_start
        if passed:
            vc.addr = (vc.addr - (vc.loop_end - vc.loop_start)) & ADDR_MASK

    def sample(self):
        left = right = 0
        rom, mask = self.rom, self.mask
        for vc in self.v:
            target = (vc.env >> 12) & 0xFFF
            rate = sext(vc.env & 0x1FF, 9)
            if vc.level != target:
                delta = abs(rate) if rate else 1
                if vc.level < target:
                    vc.level = min(vc.level + delta, target)
                else:
                    vc.level = max(vc.level - delta, target)
            # 7-point interpolation (patent US 5,111,727) at addr + 4 + frac: words addr+1..addr+8,
            # the tap 3.5+ words away has weight 0; kernel selected by the tone value (pitch bits 23..21)
            k, dk = KERN[(vc.pitch >> 21) & 7 if self.tone else 0]
            f = vc.frac
            acc = 0
            for j in range(8):
                u = ((3 - j) << 12) + f if j <= 3 else ((j - 3) << 12) - f
                a, w = u >> 7, u & 127
                acc += rom[(vc.addr + 1 + j) & mask] * (k[a] + ((dk[a] * w) >> 7))
            smp = max(-65536, min(65535, acc >> 16))
            out = (smp * level_to_gain(vc.level)) >> 16
            p = vc.pan & 0x0F
            left += out * p
            right += out * (15 - p)
            if not vc.pitch & 0x40000:
                self.step(vc)
        clamp = lambda x: max(-32768, min(32767, x))
        return clamp(left >> 6), clamp(right >> 6)


def load(path):
    data = pathlib.Path(path).read_bytes()
    rom = array.array("h", data[:SAMPLE_BYTES])
    if sys.byteorder == "little":
        rom.byteswap()  # big-endian words in the image
    events = array.array("I", data[SAMPLE_BYTES:])
    if sys.byteorder == "big":
        events.byteswap()
    return rom, events


def run(rom, events, ticks, tone=True):
    chip = Chip(rom, tone)
    out = array.array("h")
    ev, wait = 0, 0
    for _ in range(ticks):
        if wait:
            wait -= 1
        else:
            while True:
                e = events[ev]
                if e == 0:
                    ev = 0  # END: restart (the chip state carries over, as in the HDL)
                    continue
                ev += 1
                if not e & 0x80000000:
                    wait = e - 1
                    break
                v = (e >> 25) & 31
                if e & 0x40000000:
                    chip.pan(v, e & 0xFF)
                else:
                    mask = (0 if e & 0x20000 else 0xFF00) | (0 if e & 0x10000 else 0x00FF)
                    chip.write(v, (e >> 21) & 15, e & 0xFFFF, mask)
        out.extend(chip.sample())
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("demo")
    ap.add_argument("out")
    ap.add_argument("--ticks", type=int, default=0)
    ap.add_argument("--wav")
    ap.add_argument("--no-tone", action="store_true", help="tone value ignored, kernel 0 for all voices (OSD option)")
    a = ap.parse_args()
    rom, events = load(a.demo)
    total = a.ticks or sum(e for e in events if e and not e & 0x80000000)
    out = run(rom, events, total, not a.no_tone)
    if sys.byteorder == "big":
        out.byteswap()
    with open(a.out, "wb") as _f: _f.write(out.tobytes())
    if a.wav:
        with wave.open(a.wav, "wb") as w:
            w.setnchannels(2)
            w.setsampwidth(2)
            w.setframerate(39062)
            w.writeframes(out.tobytes())
    print(f"{total} samples")


if __name__ == "__main__":
    main()
