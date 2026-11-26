#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-or-later
"""Write the MT-32 MIDI corpus as byte schedules (tests/midi/*.txt).

    midi_corpus.py [OUTDIR]

A schedule line is "SAMPLE HEX HEX ...": the bytes are queued before 32 kHz sample SAMPLE.
The MT-32's default parts are on channels 2-9 and the rhythm part on channel 10.
Every case is generated from fixed seeds, so the files are the same on every run. rq1.txt and
panel.txt (buttons and knob through "# args" lines, tools/corpus.py) are written by hand.
"""
import random
import sys
from pathlib import Path

RATE = 32000
BOOT = 2 * RATE  # the firmware shows "1 2 3 4 5 R" after about 1.9 s


class Sched:
    def __init__(self):
        self.lines = []

    def at(self, t, *data):
        self.lines.append((int(t * RATE), list(data)))

    def text(self, title):
        out = [f"# {title}"]
        for s, d in sorted(self.lines, key=lambda x: x[0]):
            out.append(f"{s} " + " ".join(f"{b:02x}" for b in d))
        return "\n".join(out) + "\n"


def dt1(addr, data):
    """Roland DT1 SysEx to the MT-32 (device 10h, model 16h)."""
    a = [(addr >> 16) & 0x7F, (addr >> 8) & 0x7F, addr & 0x7F]
    body = a + list(data)
    cs = (128 - sum(body) % 128) % 128
    return [0xF0, 0x41, 0x10, 0x16, 0x12] + body + [cs, 0xF7]


def note(s, t, ch, key, vel, dur):
    s.at(t, 0x90 | ch, key, vel)
    s.at(t + dur, 0x80 | ch, key, 0x40)


def case_boot():
    return Sched(), 3 * RATE, "power-on, no MIDI"


def case_parts():
    s = Sched()
    t = BOOT / RATE
    for i, ch in enumerate(range(1, 9)):  # channels 2-9
        note(s, t + i * 0.5, ch, 48 + 3 * i, 100, 0.4)
    t += 4.2
    for i, key in enumerate([35, 36, 38, 40, 42, 46, 49, 51, 56, 60, 64, 75]):  # rhythm, channel 10
        note(s, t + i * 0.15, 9, key, 110, 0.1)
    return s, int((t + 3.0) * RATE), "one note on each part 1-8, then rhythm keys"


def case_programs():
    s = Sched()
    t = BOOT / RATE
    for i, prog in enumerate([0, 5, 16, 24, 33, 48, 56, 62, 70, 80, 90, 104, 117, 127]):
        s.at(t, 0xC1, prog)
        note(s, t + 0.02, 1, 60, 100, 0.3)
        note(s, t + 0.02, 1, 64, 90, 0.3)
        t += 0.45
    return s, int((t + 1.5) * RATE), "program changes on part 1 with a two-note chord each"


def case_controllers():
    s = Sched()
    t = BOOT / RATE
    s.at(t, 0x91, 60, 100)
    for k in range(40):  # pitch bend sweep
        v = int(8192 + 8191 * ((k / 20) - 1)) if k < 40 else 8192
        v = max(0, min(16383, v))
        s.at(t + 0.02 + k * 0.02, 0xE1, v & 0x7F, v >> 7)
    s.at(t + 0.9, 0xE1, 0x00, 0x40)
    for k in range(20):  # modulation
        s.at(t + 1.0 + k * 0.03, 0xB1, 1, k * 6)
    s.at(t + 1.7, 0xB1, 1, 0)
    for k in range(16):  # volume and pan
        s.at(t + 1.8 + k * 0.03, 0xB1, 7, 127 - k * 6)
        s.at(t + 1.81 + k * 0.03, 0xB1, 10, k * 8)
    s.at(t + 2.4, 0x81, 60, 0x40)
    s.at(t + 2.5, 0xB1, 7, 100)
    s.at(t + 2.5, 0xB1, 10, 64)
    s.at(t + 2.6, 0xB1, 64, 127)  # hold
    note(s, t + 2.7, 1, 67, 100, 0.1)
    note(s, t + 2.9, 1, 71, 100, 0.1)
    s.at(t + 3.6, 0xB1, 64, 0)
    s.at(t + 3.7, 0xB1, 121, 0)  # reset all controllers
    note(s, t + 3.8, 1, 72, 100, 0.3)
    return s, int((t + 5.0) * RATE), "pitch bend, modulation, volume, pan, hold on part 1"


def case_sysex():
    s = Sched()
    t = BOOT / RATE
    for mode in range(4):
        s.at(t, *dt1(0x100001, [mode, 5, 7]))  # system: reverb mode, time, level
        note(s, t + 0.05, 1, 60, 110, 0.15)
        note(s, t + 0.05, 9, 38, 110, 0.1)
        t += 1.4
    s.at(t, *dt1(0x100016, [60]))  # master volume
    note(s, t + 0.05, 2, 64, 100, 0.3)
    t += 0.6
    # Patch temp area, part 1: timbre group, timbre number, key shift, fine tune, bender range.
    s.at(t, *dt1(0x030000, [1, 10, 28, 50, 6]))
    note(s, t + 0.05, 1, 62, 100, 0.4)
    t += 0.7
    # Timbre temp, part 1: common area, structure 1&2 = 3, structure 3&4 = 5.
    s.at(t, *dt1(0x04000A, [3, 5]))
    note(s, t + 0.05, 1, 57, 100, 0.4)
    t += 0.8
    s.at(t, *dt1(0x7F0000, [0]))  # all parameters reset
    note(s, t + 0.6, 1, 60, 100, 0.3)
    return s, int((t + 2.0) * RATE), "SysEx: reverb modes 0-3, master volume, patch and timbre edits, reset"


def case_dense(seed=1, seconds=20.0):
    rng = random.Random(seed)
    s = Sched()
    t0 = BOOT / RATE
    held = {}
    t = t0
    for ch in range(1, 9):
        s.at(t0 - 0.1, 0xC0 | ch, rng.randrange(128))
    while t < t0 + seconds:
        t += rng.choice([0.01, 0.02, 0.03, 0.05, 0.08])
        r = rng.random()
        ch = rng.choice(list(range(1, 10)))
        if r < 0.55:
            key = rng.randrange(36, 96) if ch != 9 else rng.randrange(35, 82)
            s.at(t, 0x90 | ch, key, rng.randrange(30, 128))
            held.setdefault(ch, []).append(key)
        elif r < 0.85 and held.get(ch):
            key = held[ch].pop(rng.randrange(len(held[ch])))
            s.at(t, 0x80 | ch, key, 0x40)
        elif r < 0.90:
            s.at(t, 0xC0 | ch, rng.randrange(128))
        elif r < 0.95:
            v = rng.randrange(16384)
            s.at(t, 0xE0 | ch, v & 0x7F, v >> 7)
        else:
            s.at(t, 0xB0 | ch, rng.choice([1, 7, 10, 64]), rng.randrange(128))
    t += 0.1
    for ch in range(1, 10):
        s.at(t, 0xB0 | ch, 123, 0)  # all notes off
    return s, int((t + 2.0) * RATE), f"seeded random traffic on parts 1-8 and rhythm, seed {seed}, more than 32 partials"


CASES = {
    "boot": case_boot,
    "parts": case_parts,
    "programs": case_programs,
    "controllers": case_controllers,
    "sysex": case_sysex,
    "dense": case_dense,
}


def main():
    out = Path(sys.argv[1]) if len(sys.argv) > 1 else Path(__file__).resolve().parent.parent / "tests" / "midi"
    out.mkdir(parents=True, exist_ok=True)
    for name, fn in CASES.items():
        sched, samples, title = fn()
        (out / f"{name}.txt").write_text(f"# samples {samples}\n" + sched.text(title))


if __name__ == "__main__":
    main()
