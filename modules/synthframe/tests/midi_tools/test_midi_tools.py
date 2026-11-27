#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""The shared MIDI helpers in modules/synthframe/tools (used by every core's smoke test):
mkmidi.py -> smf2events.py gives the script's events at the script's times; play_tty.py and
tty_midi_xfer.py move those bytes through a pseudo-terminal standing in for /dev/ttyS1.

    python3 modules/synthframe/tests/midi_tools/test_midi_tools.py
"""
import os
import pty
import subprocess
import sys
import tempfile
import threading
import time
import tty
from pathlib import Path

TOOLS = Path(__file__).resolve().parents[2] / "tools"
SCRIPT = """# time event
0     pc  1 5
0     on  1 60 100
0.5   cc  2 7 90
0.75  off 1 60
1.0   pb  3 -8192
1.25  syx 41 10 42 12 40 00 7f 00 41
"""
EVENTS = """0.000000 c005
0.000000 903c64
0.500000 b1075a
0.750000 803c40
1.000000 e20000
1.250000 f04110421240007f0041f7
"""


def far_end():
    """A pty pair: (master fd, slave path, bytes the master received, thread)."""
    m, s = pty.openpty()
    tty.setraw(s)
    got = bytearray()

    def rd():
        while True:
            try:
                d = os.read(m, 4096)
            except OSError:
                return
            if not d:
                return
            got.extend(d)
    t = threading.Thread(target=rd, daemon=True)
    t.start()
    return m, os.ttyname(s), got


def main():
    fails = 0
    with tempfile.TemporaryDirectory() as w:
        w = Path(w)
        (w / "s.txt").write_text(SCRIPT)
        subprocess.run([sys.executable, TOOLS / "midi/mkmidi.py", w / "s.txt", w / "s.mid"], check=True)
        subprocess.run([sys.executable, TOOLS / "midi/smf2events.py", w / "s.mid", w / "ev.txt"], check=True,
                       capture_output=True)
        ok = (w / "ev.txt").read_text() == EVENTS
        fails += not ok
        print("mkmidi -> smf2events:", "ok" if ok else "FAIL\n" + (w / "ev.txt").read_text())

        m, dev, got = far_end()
        r = subprocess.run([sys.executable, TOOLS / "mister/play_tty.py", w / "ev.txt", "1.5", dev],
                           capture_output=True, text=True)
        time.sleep(0.2)
        want = bytes.fromhex("".join(l.split()[1] for l in EVENTS.splitlines()))
        want += b"".join(bytes([0xB0 | c, 123, 0]) for c in range(16))
        ok = r.returncode == 0 and bytes(got) == want
        fails += not ok
        print("play_tty:", "ok" if ok else f"FAIL: {bytes(got).hex(' ')} {r.stderr}")

        m, dev, got = far_end()
        os.write(m, b"\x99\x99")                       # buffered before the start: dropped
        time.sleep(0.1)
        threading.Timer(0.3, lambda: os.write(m, b"\xfe")).start()
        r = subprocess.run([sys.executable, TOOLS / "mister/tty_midi_xfer.py", "--dev", dev, "1", "c005", "90407f"],
                           capture_output=True, text=True)
        ok = r.returncode == 0 and r.stdout.strip() == "fe" and bytes(got) == bytes.fromhex("c00590407f")
        fails += not ok
        print("tty_midi_xfer:", "ok" if ok else f"FAIL: printed {r.stdout!r}, sent {bytes(got).hex(' ')} {r.stderr}")
    print(f"midi tools: {3 - fails}/3 ok")
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
