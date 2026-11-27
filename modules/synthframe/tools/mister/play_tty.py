#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Play timed MIDI bytes into the core's UART (runs on the MiSTer: python3, 32-bit ARM).

    play_tty.py EVENTS SECONDS [DEVICE]

EVENTS: "SECONDS HEXBYTES" lines (modules/synthframe/tools/midi/smf2events.py). Writes them to
DEVICE (default /dev/ttyS1; set its baud first, e.g. stty -F /dev/ttyS1 9600 raw -echo, and stop
MidiLink: synth-mister midi-stop/midi-off) on time, repeating the song (with a second's gap)
until SECONDS, then all notes off on every channel. Prints the bytes sent and the time taken.
"""
import os
import sys
import time


def main():
    if len(sys.argv) not in (3, 4):
        raise SystemExit(__doc__)
    with open(sys.argv[1]) as f:
        events = [(float(t), bytes.fromhex(h)) for t, h in (line.split() for line in f if line.strip())]
    seconds = float(sys.argv[2])
    song = events[-1][0] + 1.0
    fd = os.open(sys.argv[3] if len(sys.argv) > 3 else "/dev/ttyS1", os.O_WRONLY)
    start = time.monotonic()
    sent = 0
    offset = 0.0
    while True:
        for t, msg in events:
            if offset + t > seconds:
                break
            wait = start + offset + t - time.monotonic()
            if wait > 0:
                time.sleep(wait)
            os.write(fd, msg)
            sent += len(msg)
        else:
            offset += song
            continue
        break
    os.write(fd, b"".join(bytes([0xB0 | ch, 123, 0]) for ch in range(16)))
    print("sent", sent, "bytes in %.1f s" % (time.monotonic() - start))


if __name__ == "__main__":
    main()
