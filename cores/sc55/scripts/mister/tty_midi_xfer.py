#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-or-later
"""MIDI out check on the MiSTer (runs there: python3, 32-bit ARM). /dev/ttyS1 must already be raw
at the baud rate the core uses (the smoke scripts set 9600; synthframe_midi_tx then sends at 9600
too). MidiLink must be stopped.

  tty_midi_xfer.py SECONDS [HEX]     drops what is buffered, writes HEX (if given), reads for
                                     SECONDS and prints the bytes received in hex, one line
"""
import os
import select
import sys
import time


def main():
    secs = float(sys.argv[1])
    fd = os.open("/dev/ttyS1", os.O_RDWR | os.O_NOCTTY | os.O_NONBLOCK)
    try:
        while os.read(fd, 4096):
            pass
    except BlockingIOError:
        pass
    if len(sys.argv) > 2:
        os.write(fd, bytes.fromhex(sys.argv[2]))
    got = bytearray()
    end = time.time() + secs
    while time.time() < end:
        r, _, _ = select.select([fd], [], [], max(0.0, end - time.time()))
        if r:
            try:
                got += os.read(fd, 4096)
            except BlockingIOError:
                pass
    os.close(fd)
    print(got.hex(" "))


if __name__ == "__main__":
    main()
