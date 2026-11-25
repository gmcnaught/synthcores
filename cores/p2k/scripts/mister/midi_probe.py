#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
# MIDI out check, run on the MiSTer: sends hex MIDI messages on /dev/ttyS1 at 31250 baud and prints
# what the core sends back for SECS seconds. Stop MidiLink first (synth-mister midi-stop).
#   python3 midi_probe.py SECS HEX [HEX...]   (one message per HEX, 0.5 s apart)
# The identity request F07E7F0601F7 gets F0 7E 00 06 02 18 04 04 03 00 '2.26' F7 from OS 2.26.
# 31250 is not a standard termios rate: set through TCSETS2 with BOTHER (32-bit ARM layout).
import fcntl, os, select, struct, sys, termios, time
fd = os.open("/dev/ttyS1", os.O_RDWR | os.O_NOCTTY)
a = termios.tcgetattr(fd)
a[0] = 0; a[1] = 0; a[3] = 0; a[2] = termios.CS8 | termios.CREAD | termios.CLOCAL
a[6][termios.VMIN] = 0; a[6][termios.VTIME] = 0
termios.tcsetattr(fd, termios.TCSANOW, a)
TCGETS2, TCSETS2, CBAUD, BOTHER = 0x802C542A, 0x402C542B, 0o010017, 0o010000
t2 = bytearray(44); fcntl.ioctl(fd, TCGETS2, t2)
i, o, c, l = struct.unpack_from("4I", t2)
struct.pack_into("4I", t2, 0, i, o, (c & ~CBAUD) | BOTHER, l)
struct.pack_into("2I", t2, 36, 31250, 31250)
fcntl.ioctl(fd, TCSETS2, t2)
fcntl.ioctl(fd, TCGETS2, t2); print("speed", struct.unpack_from("2I", t2, 36))
termios.tcflush(fd, termios.TCIOFLUSH)
for h in sys.argv[2:]:
    os.write(fd, bytes.fromhex(h)); time.sleep(0.5)
end = time.time() + float(sys.argv[1]); got = bytearray()
while time.time() < end:
    r, _, _ = select.select([fd], [], [], 0.2)
    if r: got += os.read(fd, 4096)
print("rx", len(got), got.hex(" "))
