#!/usr/bin/env python3
"""Build the MiSTer demo image: 8 MB of sample ROM followed by a register-write event stream.

Input: a MAME trace from mame/scripts/trace_io.lua (lines "time kind R/W addr data mask"),
the rebuilt Proteus/1 base ROM (big-endian 16-bit, 4 MB) and the ProtoLogic chip dumps.

Image layout (byte offsets; loaded to DDR3 at 0x30000000 by the core):
  0x000000  sample words, big-endian, 4M words (bank A = Proteus/1, bank B = ProtoLogic)
  0x800000  events, 32-bit little-endian:
              0x00000000                 END (restart from the first event)
              0 nnnnnnn... (bit31 = 0)   WAIT n output samples (n = bits 30..0, n >= 1)
              1 0 vvvvv wwww .... dddd   sound register write: voice v, word w (0..15), data d
              1 1 vvvvv ........ pppp    pan write: voice v, byte p (bits 7..0)

A write at time t affects output samples k >= ceil(t * 39062.5): MAME's stream generates
samples with times < t before applying it.

Usage: make_demo.py TRACE REBUILT_BASE PROTOLOGIC_DIR OUT.bin [--from SEC] [--to SEC]
"""
import argparse
import math
import struct

FS = 39062.5


def protologic_image(d):
    lo = [open(f"{d}/ip50{c}.bin", "rb").read() for c in (5, 6, 7, 8)]
    hi = [open(f"{d}/ip70{c}.bin", "rb").read() for c in (5, 6, 7, 8)]
    out = bytearray(0x400000)
    for n in range(0x200000):
        out[2 * n] = hi[n & 3][n >> 2]
        out[2 * n + 1] = lo[n & 3][n >> 2]
    return bytes(out)


def parse_trace(path, t_from, t_to):
    """Yield (tick, event) in trace order; sound/pan writes only."""
    for line in open(path):
        p = line.split()
        if len(p) < 6 or p[2] != "W" or p[1] not in ("snd", "pan"):
            continue
        t = float(p[0])
        if t < t_from or t > t_to:
            continue
        addr, data = int(p[3], 16), int(p[4], 16)
        tick = math.ceil((t - t_from) * FS)
        if p[1] == "snd":
            off = (addr - 0x800000) >> 1
            ev = 0x80000000 | ((off >> 4) << 25) | ((off & 15) << 21) | (data & 0xFFFF)
        else:
            voice = (((addr - 0x500000) >> 1) - 4) & 31
            ev = 0xC0000000 | (voice << 25) | (data & 0xFF)
        yield tick, ev


def build_events(writes, end_tick):
    events, cur = [], 0
    for tick, ev in writes:
        if tick > cur:
            events.append(tick - cur)
            cur = tick
        events.append(ev)
    if end_tick > cur:
        events.append(end_tick - cur)
    events.append(0)  # END
    return events


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("trace")
    ap.add_argument("base")
    ap.add_argument("protologic")
    ap.add_argument("out")
    ap.add_argument("--from", dest="t_from", type=float, default=0.0)
    ap.add_argument("--to", dest="t_to", type=float, default=1e9)
    a = ap.parse_args()

    base = open(a.base, "rb").read()
    assert len(base) == 0x400000
    samples = base + protologic_image(a.protologic)
    writes = list(parse_trace(a.trace, a.t_from, a.t_to))
    last = max(t for t, _ in writes)
    events = build_events(writes, last + int(2 * FS))
    with open(a.out, "wb") as f:
        f.write(samples)
        for e in events:
            f.write(struct.pack("<I", e))
    ticks = sum(e for e in events if e and not e & 0x80000000)
    print(f"{len(writes)} writes, {len(events)} events, {ticks} ticks ({ticks / FS:.1f} s), "
          f"image {len(samples) + 4 * len(events)} bytes")


if __name__ == "__main__":
    main()
