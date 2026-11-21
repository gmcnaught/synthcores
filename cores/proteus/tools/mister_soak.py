#!/usr/bin/env python3
"""Soak test, run ON the MiSTer with the Proteus core loaded: plays dense random MIDI into
/dev/ttyS1 for SECONDS and reports the core's sound-engine status word (DDR3 0x30FFFF00:
samples produced, missed sample deadlines, worst clocks per sample of 2048) before and after.

Usage: python3 mister_soak.py SECONDS [seed]
Load: up to 16 channels with random programs, 6-note chords every 120 ms plus fast single
notes (more than 32 voices requested, so voice stealing is exercised), pitch bend and CC7/CC10.
Stop MidiLink first (it shares /dev/ttyS1).
"""
import mmap
import os
import random
import struct
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from mister_midi import set_baud  # noqa: E402

FS = 39062.5


def status():
    fd = os.open("/dev/mem", os.O_RDONLY | os.O_SYNC)
    m = mmap.mmap(fd, 4096, mmap.MAP_SHARED, mmap.PROT_READ, offset=0x30FFF000)
    v = struct.unpack("<Q", m[0xF00:0xF08])[0]
    m.close()
    os.close(fd)
    return v & 0xFFFFFFFF, (v >> 32) & 0xFFFF, v >> 48


def main():
    secs = float(sys.argv[1])
    random.seed(int(sys.argv[2]) if len(sys.argv) > 2 else 1)
    fd = os.open("/dev/ttyS1", os.O_RDWR | os.O_NOCTTY)
    set_baud(fd, 31250)
    s0, m0, w0 = status()
    t0 = time.monotonic()
    for ch in range(16):
        os.write(fd, bytes([0xC0 | ch, random.randrange(128)]))
    held = []
    n_events = 0
    while time.monotonic() - t0 < secs:
        out = bytearray()
        ch = random.randrange(16)
        root = random.randrange(36, 84)
        for k in (0, 4, 7, 12, 16, 19):
            out += bytes([0x90 | ch, min(127, root + k), random.randrange(40, 128)])
            held.append((ch, min(127, root + k)))
        for _ in range(4):
            c2 = random.randrange(16)
            n = random.randrange(24, 108)
            out += bytes([0x90 | c2, n, 100, 0x80 | c2, n, 0])
        if random.random() < 0.2:
            out += bytes([0xE0 | ch, 0, random.randrange(128)])
        if random.random() < 0.1:
            out += bytes([0xB0 | ch, random.choice((7, 10)), random.randrange(128)])
        if random.random() < 0.05:
            out += bytes([0xC0 | ch, random.randrange(128)])
        while len(held) > 40:
            c3, n3 = held.pop(0)
            out += bytes([0x80 | c3, n3, 0])
        os.write(fd, bytes(out))
        n_events += 1
        time.sleep(0.12)
    for c3, n3 in held:
        os.write(fd, bytes([0x80 | c3, n3, 0]))
    for ch in range(16):
        os.write(fd, bytes([0xB0 | ch, 123, 0]))   # all notes off
    elapsed = time.monotonic() - t0
    time.sleep(0.5)
    s1, m1, w1 = status()
    os.close(fd)
    produced = (s1 - s0) & 0xFFFFFFFF
    print(f"elapsed {elapsed:.1f} s, {n_events} MIDI bursts")
    print(f"samples produced {produced} (expected ~{elapsed * FS:.0f}, status updates every 4096)")
    print(f"missed deadlines: before {m0}, after {m1}; worst clocks per sample: {w1} of 2048")


if __name__ == "__main__":
    main()
