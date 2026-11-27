#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Write a format-0 Standard MIDI File from an event script (runs on the build machine).

    mkmidi.py SCRIPT OUT.mid

SCRIPT has one event a line, the time in seconds first; '#' starts a comment:
    T on  CH NOTE VEL         note on (CH 1-16)
    T off CH NOTE             note off, velocity 64
    T pc  CH PROGRAM          program change (0-127)
    T cc  CH CONTROLLER VALUE
    T pb  CH BEND             pitch bend, -8192..8191
    T syx HEX...              system exclusive, the bytes between F0 and F7
The file is 480 ticks a quarter note at 120 bpm (960 ticks a second). Events at the same time
keep the script's order.
"""
import struct
import sys

PPQ = 480
TICKS_PER_SECOND = 960


def varlen(n):
    """MIDI variable-length quantity."""
    groups = [n & 0x7F]
    n >>= 7
    while n:
        groups.insert(0, 0x80 | (n & 0x7F))
        n >>= 7
    return bytes(groups)


def message(kind, args):
    if kind == "syx":
        body = bytes.fromhex("".join(args)) + b"\xf7"
        return b"\xf0" + varlen(len(body)) + body
    ch = int(args[0]) - 1
    if kind == "on":
        return bytes([0x90 | ch, int(args[1]), int(args[2])])
    if kind == "off":
        return bytes([0x80 | ch, int(args[1]), 0x40])
    if kind == "pc":
        return bytes([0xC0 | ch, int(args[1])])
    if kind == "cc":
        return bytes([0xB0 | ch, int(args[1]), int(args[2])])
    if kind == "pb":
        v = int(args[1]) + 8192
        return bytes([0xE0 | ch, v & 0x7F, v >> 7])
    raise SystemExit(f"unknown event {kind}")


def main():
    if len(sys.argv) != 3:
        raise SystemExit(__doc__)
    events = []
    with open(sys.argv[1]) as f:
        for line in f:
            words = line.split("#", 1)[0].split()
            if words:
                events.append((round(float(words[0]) * TICKS_PER_SECOND), message(words[1], words[2:])))
    events.sort(key=lambda e: e[0])                       # stable: same-time events keep order
    track = bytearray(b"\x00\xff\x51\x03" + (500000).to_bytes(3, "big"))   # 120 bpm
    now = 0
    for t, msg in events:
        track += varlen(t - now) + msg
        now = t
    track += b"\x00\xff\x2f\x00"
    with open(sys.argv[2], "wb") as f:
        f.write(b"MThd" + struct.pack(">IHHH", 6, 0, 1, PPQ))
        f.write(b"MTrk" + struct.pack(">I", len(track)) + track)


if __name__ == "__main__":
    main()
