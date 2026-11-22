#!/usr/bin/env python3
"""Write a format-0 Standard MIDI File from a simple event script.

Script lines (times in seconds, '#' comments):
  <time> on  <chan 1-16> <note> <vel>
  <time> off <chan> <note>
  <time> pc  <chan> <program 0-127>
  <time> cc  <chan> <controller> <value>
  <time> syx <hex bytes without F0/F7>

Usage: mkmidi.py SCRIPT OUT.mid
Tempo is fixed at 120 bpm with 480 ticks per quarter (960 ticks per second).
"""
import struct
import sys

TPS = 960  # ticks per second at 120 bpm, 480 PPQ


def vlq(n):
    out = [n & 0x7F]
    n >>= 7
    while n:
        out.append(0x80 | (n & 0x7F))
        n >>= 7
    return bytes(reversed(out))


def main():
    events = []
    for line in open(sys.argv[1]):
        line = line.split("#", 1)[0].split()
        if not line:
            continue
        t = int(round(float(line[0]) * TPS))
        kind = line[1]
        if kind == "syx":
            body = bytes.fromhex("".join(line[2:])) + b"\xf7"
            events.append((t, b"\xf0" + vlq(len(body)) + body))
            continue
        ch = int(line[2]) - 1
        if kind == "on":
            events.append((t, bytes([0x90 | ch, int(line[3]), int(line[4])])))
        elif kind == "off":
            events.append((t, bytes([0x80 | ch, int(line[3]), 0x40])))
        elif kind == "pc":
            events.append((t, bytes([0xC0 | ch, int(line[3])])))
        elif kind == "cc":
            events.append((t, bytes([0xB0 | ch, int(line[3]), int(line[4])])))
        else:
            raise SystemExit(f"unknown event {kind}")
    events.sort(key=lambda e: e[0])
    track = bytearray(b"\x00\xff\x51\x03\x07\xa1\x20")  # tempo 500000 us/qn
    last = 0
    for t, data in events:
        track += vlq(t - last) + data
        last = t
    track += b"\x00\xff\x2f\x00"
    with open(sys.argv[2], "wb") as f:
        f.write(b"MThd" + struct.pack(">IHHH", 6, 0, 1, 480))
        f.write(b"MTrk" + struct.pack(">I", len(track)) + track)


if __name__ == "__main__":
    main()
