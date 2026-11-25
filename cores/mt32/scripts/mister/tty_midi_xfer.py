#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-or-later
"""Send MIDI messages to the core and read its MIDI out (runs on the MiSTer: python3, 32-bit ARM).

  tty_midi_xfer.py BAUD SECONDS HEX [HEX ...]     prints the bytes received in hex, one line

Sets /dev/ttyS1 (the core's UART; MidiLink must be stopped) to raw BAUD (31250 through termios2
BOTHER, as cores/cz101/scripts/mister/tty_midi_read.py), drops what is buffered, writes each HEX
message 0.5 s apart, and reads for SECONDS after the first.
"""
import fcntl
import os
import select
import struct
import sys
import time


def read(fd, got):
    try:
        got += os.read(fd, 4096)
    except BlockingIOError:     # select saw data another reader took
        pass

TCGETS2, TCSETS2 = 0x802C542A, 0x402C542B      # _IOR/_IOW('T', 0x2A/0x2B, struct termios2), 44 bytes
CBAUD, BOTHER = 0o010017, 0o010000
FMT = "4I B 19s 2I"
DEV = "/dev/ttyS1"


def main():
    baud, secs, msgs = int(sys.argv[1]), float(sys.argv[2]), [bytes.fromhex(m) for m in sys.argv[3:]]
    os.system(f"stty -F {DEV} {baud if baud != 31250 else 38400} raw -echo")
    fd = os.open(DEV, os.O_RDWR | os.O_NOCTTY | os.O_NONBLOCK)
    if baud == 31250:
        buf = bytearray(fcntl.ioctl(fd, TCGETS2, bytes(struct.calcsize(FMT))))
        iflag, oflag, cflag, lflag, line, cc, _, _ = struct.unpack(FMT, buf)
        cflag = (cflag & ~CBAUD) | BOTHER
        fcntl.ioctl(fd, TCSETS2, struct.pack(FMT, iflag, oflag, cflag, lflag, line, cc, 31250, 31250))
    try:
        while os.read(fd, 4096):        # drop what is already buffered
            pass
    except BlockingIOError:
        pass
    got = bytearray()
    start = time.time()
    end = start + secs
    for i, m in enumerate(msgs):
        due = start + 0.5 * i
        while time.time() < due:
            r, _, _ = select.select([fd], [], [], max(0.0, due - time.time()))
            if r:
                read(fd, got)
        os.write(fd, m)
    while time.time() < end:
        r, _, _ = select.select([fd], [], [], max(0.0, end - time.time()))
        if r:
            read(fd, got)
    os.close(fd)
    print(got.hex(" "))


if __name__ == "__main__":
    main()
