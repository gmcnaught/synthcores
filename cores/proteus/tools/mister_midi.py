#!/usr/bin/env python3
"""Send MIDI to the core from the MiSTer's Linux side (run ON the MiSTer).

The core's MIDI input is the FPGA UART, wired to the HPS UART /dev/ttyS1. This sets
/dev/ttyS1 to 31250 baud (termios2 / BOTHER custom rate) and plays a script:
  lines "<seconds> <hex bytes...>"  e.g. "0.5 90 3c 64" (times from start)
Usage: python3 mister_midi.py SCRIPT.txt
Stop MidiLink first if it is running (it also opens /dev/ttyS1).
"""
import pathlib
import array
import fcntl
import os
import sys
import time

TCGETS2, TCSETS2, BOTHER, CBAUD = 0x802C542A, 0x402C542B, 0o010000, 0o010017


def set_baud(fd, rate):
    buf = array.array("i", [0] * 11)          # struct termios2 (44 bytes on ARM)
    fcntl.ioctl(fd, TCGETS2, buf, True)
    buf[0] = 0                               # c_iflag
    buf[1] = 0                               # c_oflag
    buf[2] = (buf[2] & ~CBAUD) | BOTHER | 0o60 | 0o200 | 0o4000   # CS8 | CREAD | CLOCAL
    buf[3] = 0                               # c_lflag
    buf[9] = rate                            # c_ispeed
    buf[10] = rate                           # c_ospeed
    fcntl.ioctl(fd, TCSETS2, buf)


def main():
    events = []
    for line in pathlib.Path(sys.argv[1]).read_text().splitlines(keepends=True):
        p = line.split("#")[0].split()
        if p:
            events.append((float(p[0]), bytes(int(x, 16) for x in p[1:])))
    fd = os.open("/dev/ttyS1", os.O_RDWR | os.O_NOCTTY)
    set_baud(fd, 31250)
    t0 = time.monotonic()
    for t, data in events:
        delay = t0 + t - time.monotonic()
        if delay > 0:
            time.sleep(delay)
        os.write(fd, data)
    time.sleep(0.05)
    os.close(fd)
    print(f"sent {len(events)} events")


if __name__ == "__main__":
    main()
