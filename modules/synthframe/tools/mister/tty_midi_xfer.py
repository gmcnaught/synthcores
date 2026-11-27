#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Send MIDI to the core and read its MIDI out (runs on the MiSTer: python3, 32-bit ARM).

    tty_midi_xfer.py [--baud BAUD] [--dev DEVICE] SECONDS [HEX ...]

DEVICE (default /dev/ttyS1, the core's UART) must not be in use: run synth-mister midi-off after
the core load. --baud sets the line to raw BAUD first (31250 through termios2 BOTHER: the HPS
UART divides 100 MHz by 16 x 200); without it the line stays as it is (e.g. set by
stty -F /dev/ttyS1 9600 raw -echo). Then drops what is buffered, writes each HEX message
0.5 s apart (the first at once), reads until SECONDS after the start, and prints the bytes
received in hex on one line ("" when none).
"""
import argparse
import fcntl
import os
import select
import struct
import subprocess
import time

TCGETS2, TCSETS2 = 0x802C542A, 0x402C542B      # _IOR/_IOW('T', 0x2A/0x2B, struct termios2)
CBAUD, BOTHER = 0o010017, 0o010000
TERMIOS2 = "4I B 19s 2I"                        # 44 bytes on 32-bit ARM


def set_baud(dev, fd, baud):
    subprocess.run(["stty", "-F", dev, str(38400 if baud == 31250 else baud), "raw", "-echo"], check=True)
    if baud == 31250:
        t = struct.unpack(TERMIOS2, fcntl.ioctl(fd, TCGETS2, bytes(struct.calcsize(TERMIOS2))))
        iflag, oflag, cflag, lflag, line, cc = t[:6]
        cflag = (cflag & ~CBAUD) | BOTHER
        fcntl.ioctl(fd, TCSETS2, struct.pack(TERMIOS2, iflag, oflag, cflag, lflag, line, cc, baud, baud))


def read_until(fd, until, got):
    while True:
        left = until - time.time()
        if left <= 0:
            return
        r, _, _ = select.select([fd], [], [], left)
        if r:
            try:
                got += os.read(fd, 4096)
            except BlockingIOError:     # select saw data another reader took
                pass


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--baud", type=int)
    ap.add_argument("--dev", default="/dev/ttyS1")
    ap.add_argument("seconds", type=float)
    ap.add_argument("hex", nargs="*")
    a = ap.parse_args()
    msgs = [bytes.fromhex(h) for h in a.hex]
    fd = os.open(a.dev, os.O_RDWR | os.O_NOCTTY | os.O_NONBLOCK)
    if a.baud:
        set_baud(a.dev, fd, a.baud)
    try:
        while os.read(fd, 4096):        # drop what is already buffered
            pass
    except BlockingIOError:
        pass
    got = bytearray()
    start = time.time()
    for i, m in enumerate(msgs):
        read_until(fd, start + 0.5 * i, got)
        os.write(fd, m)
    read_until(fd, start + a.seconds, got)
    os.close(fd)
    print(got.hex(" "))


if __name__ == "__main__":
    main()
