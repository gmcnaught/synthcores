#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""A Standard MIDI File as timed MIDI bytes (runs on the build machine), for
modules/synthframe/tools/mister/play_tty.py and the Verilator board benches.

    smf2events.py IN.mid OUT.txt

OUT.txt has one message a line, "SECONDS HEXBYTES" (seconds with 6 decimals, from the tempo
map; 120 bpm until the first tempo event). All tracks are merged; events at the same tick go
in track order, then file order. System exclusive messages start with F0; meta events other
than tempo are dropped. Prints the event count and the length.
"""
import pathlib
import struct
import sys


def read_track(data, p, end, track_no, out):
    tick = order = status = 0

    def varlen():
        nonlocal p
        v = 0
        while True:
            b = data[p]
            p += 1
            v = (v << 7) | (b & 0x7F)
            if not b & 0x80:
                return v

    while p < end:
        tick += varlen()
        b = data[p]
        if b == 0xFF:                                     # meta: keep tempo only
            kind = data[p + 1]
            p += 2
            n = varlen()
            if kind == 0x51:
                out.append((tick, track_no, order, int.from_bytes(data[p:p + 3], "big")))
            p += n
        elif b in (0xF0, 0xF7):                           # sysex (F7: escaped bytes as they are)
            p += 1
            n = varlen()
            body = data[p:p + n]
            p += n
            out.append((tick, track_no, order, (b"\xf0" + body) if b == 0xF0 else body))
        else:
            if b & 0x80:
                status = b
                p += 1
            n = 1 if (status & 0xF0) in (0xC0, 0xD0) else 2
            out.append((tick, track_no, order, bytes([status]) + data[p:p + n]))
            p += n
        order += 1


def main():
    if len(sys.argv) != 3:
        raise SystemExit(__doc__)
    data = pathlib.Path(sys.argv[1]).read_bytes()
    if data[:4] != b"MThd":
        raise SystemExit(f"{sys.argv[1]}: not a Standard MIDI File")
    _, ntracks, division = struct.unpack(">HHH", data[8:14])
    p = 8 + struct.unpack(">I", data[4:8])[0]
    events = []
    for n in range(ntracks):
        if data[p:p + 4] != b"MTrk":
            raise SystemExit(f"{sys.argv[1]}: track {n}: no MTrk at {p}")
        length = struct.unpack(">I", data[p + 4:p + 8])[0]
        read_track(data, p + 8, p + 8 + length, n, events)
        p += 8 + length
    events.sort(key=lambda e: e[:3])
    us_per_quarter, last, seconds = 500000, 0, 0.0
    with open(sys.argv[2], "w") as f:
        for tick, _, _, e in events:
            seconds += (tick - last) * us_per_quarter / 1e6 / division
            last = tick
            if isinstance(e, int):
                us_per_quarter = e
            else:
                f.write("%.6f %s\n" % (seconds, e.hex()))
    print("events", len(events), "duration %.1f s" % seconds)


if __name__ == "__main__":
    main()
