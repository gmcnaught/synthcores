#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Read MIDI out from the core on the MiSTer (runs on the MiSTer: python3, 32-bit ARM).

  tty_midi_read.py SECONDS [DEVICE]      prints the bytes received in hex, one line

Sets DEVICE (default /dev/ttyS1, the core's UART; MidiLink must be stopped) to raw 31,250 baud
with termios2 BOTHER (the HPS UART divides its 100 MHz clock by 16 x 200), then reads for
SECONDS.
"""
import fcntl
import os
import select
import struct
import sys
import time

TCGETS2, TCSETS2 = 0x802C542A, 0x402C542B      # _IOR/_IOW('T', 0x2A/0x2B, struct termios2), 44 bytes
CBAUD, BOTHER = 0o010017, 0o010000
FMT = "4I B 19s 2I"


def main():
    secs = float(sys.argv[1])
    dev = sys.argv[2] if len(sys.argv) > 2 else "/dev/ttyS1"
    os.system(f"stty -F {dev} 38400 raw -echo")
    fd = os.open(dev, os.O_RDWR | os.O_NOCTTY | os.O_NONBLOCK)
    buf = bytearray(fcntl.ioctl(fd, TCGETS2, bytes(struct.calcsize(FMT))))
    iflag, oflag, cflag, lflag, line, cc, ispeed, ospeed = struct.unpack(FMT, buf)
    cflag = (cflag & ~CBAUD) | BOTHER
    fcntl.ioctl(fd, TCSETS2, struct.pack(FMT, iflag, oflag, cflag, lflag, line, cc, 31250, 31250))
    try:
        while os.read(fd, 4096):        # drop what is already buffered
            pass
    except BlockingIOError:
        pass
    got = bytearray()
    end = time.time() + secs
    while time.time() < end:
        r, _, _ = select.select([fd], [], [], max(0.0, end - time.time()))
        if r:
            got += os.read(fd, 4096)
    os.close(fd)
    print(got.hex(" "))


if __name__ == "__main__":
    main()
