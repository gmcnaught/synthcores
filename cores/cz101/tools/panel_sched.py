#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Panel script -> the key and pitch-wheel schedules of the C4 bench and the patched MAME.

  panel_sched.py SCRIPT KEYS_OUT WHEEL_OUT

SCRIPT lines (times in seconds from power-on; '#' starts a comment):
  <time> key <name> [<seconds held>]     a panel button, MAME's port name (default 0.15 s)
  <time> wheel <0-255>                   the pitch wheel (AN1; 127 = centre)
KEYS_OUT lines "CYCLE ROW BIT DOWN", WHEEL_OUT lines "CYCLE VALUE" (CZ_KEYS, CZ_WHEEL: CPU cycles
at MAME's 3,333,334 a second).
"""
import sys

CPU_HZ = 3_333_334
# MAME cz101 INPUT_PORTS: name -> (row kcN, bit)
KEYS = {
    "Portamento On/Off": (9, 0), "Portamento Time": (9, 1), "Vibrato On/Off": (9, 2),
    "Bend Range": (9, 3), "Preset": (9, 4), "Internal": (9, 5), "Cartridge": (9, 6),
    "Compare/Recall": (9, 7),
    "Solo": (10, 0), "Tone Mix": (10, 1), "Key Transpose": (10, 2), "Write": (10, 3),
    "MIDI": (10, 4), "Select": (10, 6),
    **{f"Tone {i + 1}": (11, i) for i in range(8)},
    "Value Down": (12, 0), "Value Up": (12, 1), "Cursor Left": (12, 2), "Cursor Right": (12, 3),
    "Env Step Down": (12, 4), "Env Step Up": (12, 5),
    "Vibrato": (13, 1), "DCO1 Wave Form": (13, 2), "DCO1 Envelope": (13, 3),
    "DCW1 Key Follow": (13, 4), "DCW1 Envelope": (13, 5), "DCA1 Key Follow": (13, 6),
    "DCA1 Envelope": (13, 7),
    "Initialize": (14, 0), "Octave": (14, 1), "DCO2 Wave Form": (14, 2), "DCO2 Envelope": (14, 3),
    "DCW2 Key Follow": (14, 4), "DCW2 Envelope": (14, 5), "DCA2 Key Follow": (14, 6),
    "DCA2 Envelope": (14, 7),
    "Detune": (15, 0), "Line Select": (15, 1), "Ring Modulation": (15, 2),
    "Noise Modulation": (15, 3), "Master Tune Down": (15, 4), "Master Tune Up": (15, 5),
}


def main():
    src, kout, wout = sys.argv[1:4]
    keys, wheel = [], []
    for n, line in enumerate(open(src), 1):
        line = line.split("#", 1)[0].strip()
        if not line:
            continue
        t, kind, rest = line.split(None, 2)
        cyc = int(float(t) * CPU_HZ)
        if kind == "wheel":
            wheel.append((cyc, int(rest)))
            continue
        if kind != "key":
            raise SystemExit(f"{src}:{n}: unknown event {kind}")
        name, held = rest, 0.15
        parts = rest.rsplit(None, 1)
        if name not in KEYS and len(parts) == 2 and parts[0] in KEYS:
            name, held = parts[0], float(parts[1])
        if name not in KEYS:
            raise SystemExit(f"{src}:{n}: unknown key {name!r}")
        r, b = KEYS[name]
        keys += [(cyc, r, b, 1), (int((float(t) + held) * CPU_HZ), r, b, 0)]
    with open(kout, "w") as f:
        for e in sorted(keys):
            f.write("%d %d %d %d\n" % e)
    with open(wout, "w") as f:
        for e in sorted(wheel):
            f.write("%d %d\n" % e)


if __name__ == "__main__":
    main()
