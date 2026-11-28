#!/usr/bin/env python3
"""Stuck-note check, run ON the MiSTer with the Proteus core loaded: note-offs sent while the
OSD opens. Opening the OSD starts nv_ctrl's autosave, which pauses the CPU while the RAM is
written to the save file; before midi_in.sv, MIDI bytes that arrived in that pause were lost
in an MFP overrun, so note-offs went missing and notes hung.

Usage: python3 mister_osd_notes.py PRESET TRIALS [osd|none]
Each trial: program change PRESET, a 6-note chord held 1 s, F12 (OSD open, from a virtual
uinput keyboard) unless "none", the 6 note-offs DELAY ms later (0..30 ms, varied), F12 again
(OSD closed), 3 s, then the core's output peak over its last 4096 samples (0x30FFFF08). A peak
above 64 after the release counts as hung notes; CC 123 (all notes off) clears them before the
next trial. Prints one line a trial and "hung: N of TRIALS". Stop MidiLink first.
"""
import fcntl
import os
import struct
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from mister_soak import status  # noqa: E402
from mister_midi import set_baud  # noqa: E402

EV_SYN, EV_KEY, KEY_F12 = 0, 1, 88
UI_SET_EVBIT, UI_SET_KEYBIT, UI_DEV_CREATE, UI_DEV_DESTROY = 0x40045564, 0x40045565, 0x5501, 0x5502


def keyboard():
    fd = os.open("/dev/uinput", os.O_WRONLY | os.O_NONBLOCK)
    fcntl.ioctl(fd, UI_SET_EVBIT, EV_KEY)
    fcntl.ioctl(fd, UI_SET_EVBIT, EV_SYN)
    fcntl.ioctl(fd, UI_SET_KEYBIT, KEY_F12)
    dev = b"osd-test-kbd".ljust(80, b"\0") + struct.pack("<HHHHI", 3, 0x1234, 0x5678, 1, 0) + bytes(4 * 64 * 4)
    os.write(fd, dev)
    fcntl.ioctl(fd, UI_DEV_CREATE)
    return fd


def key(fd, code):
    for v in (1, 0):
        os.write(fd, struct.pack("<llHHi", 0, 0, EV_KEY, code, v))
        os.write(fd, struct.pack("<llHHi", 0, 0, EV_SYN, 0, 0))
        time.sleep(0.03)


def main():
    preset, trials = int(sys.argv[1]), int(sys.argv[2])
    osd = (sys.argv[3] if len(sys.argv) > 3 else "osd") == "osd"
    midi = os.open("/dev/ttyS1", os.O_RDWR | os.O_NOCTTY)
    set_baud(midi, 31250)
    kb = keyboard() if osd else None
    time.sleep(2.0)                                  # Main_MiSTer picks up the new input device
    notes = [48, 52, 55, 60, 64, 67]
    hung = 0
    for t in range(trials):
        os.write(midi, bytes([0xC0, preset]))
        time.sleep(0.3)
        os.write(midi, bytes([0x90] + sum(([n, 100] for n in notes), [])))
        time.sleep(1.0)
        held = status()["peak"]
        delay = (t * 7 % 31) / 1000.0
        if osd:
            key(kb, KEY_F12)
        time.sleep(delay)
        os.write(midi, bytes([0x80] + sum(([n, 64] for n in notes), [])))
        time.sleep(1.0)
        if osd:
            key(kb, KEY_F12)
        time.sleep(3.0)
        after = status()["peak"]
        bad = after > 64
        hung += bad
        print(f"trial {t}: delay {delay * 1000:.0f} ms, peak held {held}, after release {after}"
              + (" HUNG" if bad else ""), flush=True)
        os.write(midi, bytes([0xB0, 123, 0]))
        time.sleep(2.0 if bad else 0.2)
    print(f"hung: {hung} of {trials}")
    if kb is not None:
        fcntl.ioctl(kb, UI_DEV_DESTROY)


if __name__ == "__main__":
    main()
