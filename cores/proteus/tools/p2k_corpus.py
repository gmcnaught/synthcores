#!/usr/bin/env python3
"""Proteus 2000 reference corpus: fixed MIDI cases rendered by the MAME research machine
(proteus2k) with the stand-in Composer SIMM. Each case records the audio (-wavwrite) and the
sound-chip register log (P2K_TRACE: G20 voice, H filter and G05 writes with time and PC). The
hashes and summary statistics go to tests/p2k_corpus/manifest.json; a sound core is checked
against the logs and audio of these runs, as the Proteus/1 is checked against MAME.

  p2k_corpus.py gen                                  write tests/p2k_corpus/<case>.txt
  p2k_corpus.py run [--update] [--repeat N] [--jobs N] [--cases a,b] [--out DIR]
  p2k_corpus.py model [--cases a,b] [--out DIR] [--snr]
                                                     replay each case's snd.log through the standalone
                                                     renderer (p2k/model, build.sh) and compare with
                                                     MAME's snd.raw; --snr also reports integer vs float

run needs (environment or options):
  P2K_MAME      the proteus MAME binary built with mame/patches 0001-0003
  P2K_ROMPATH   its rompath (roms/proteus2k/proteus-2k-v124-28f160.bin, p2k226.dli)
  P2K_SIMMDIR   big.bin, baby.bin and flash_clean.nv of the Composer stand-in (mame/README.md:
                p2k_spec_from_dir.py on "Proteus 2000 Samples", p2k_build_simm.py, one
                P2K_CLEAN=1 boot of ~60 s emulated)
Without --update, run compares with the manifest and exits 1 on a difference. Inputs whose
hashes differ from the manifest are reported first (a different SIMM or MAME gives other output).

Times in the case scripts are MIDI file times: MAME's -midiin starts the file 10 s after power-on
(measured: first event at 10.000016 s), the OS main screen is up at ~11.75 s.
"""
import pathlib
import argparse
import hashlib
import json
import os
import re
import resource
import shutil
import struct
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor

HERE = os.path.dirname(os.path.abspath(__file__))
CORE = os.path.dirname(HERE)
CASEDIR = os.path.join(CORE, "tests", "p2k_corpus")
MANIFEST = os.path.join(CASEDIR, "manifest.json")
MIDI_OFFSET = 10.0          # -midiin starts the file 10 s after power-on
START = 2.0                 # first event (file time): after the main screen
TAIL = 3.0                  # seconds rendered after the last event
BANK_MSB = 4                # SIMM ID 4 ("P2Ksy"); preset = LSB * 128 + program

# Stand-in presets used by name (tools/p2k_spec_from_dir.py order on "Proteus 2000 Samples")
B3 = 87                     # rom:B3 Lo Fast: 10 looped zones, roots 42-96 (held notes sustain)
ORGAN = 880                 # org:OrganClassic: one unlooped sample, root 68, 25 kHz
SINE = 967                  # wav:Sine


def sel(t, ch, preset):
    return [f"{t:.3f} cc {ch} 0 {BANK_MSB}", f"{t:.3f} cc {ch} 32 {preset // 128}",
            f"{t + 0.05:.3f} pc {ch} {preset % 128}"]


def case_boot():
    return ["# power-on to the main screen, no MIDI"], 14.0 - MIDI_OFFSET


def case_sweep(bank):
    ev = [f"# presets {bank * 128}-{bank * 128 + 127}: one note each (C4, velocity 100, 0.4 s)"]
    t = START
    for prog in range(128):
        ev += sel(t, 1, bank * 128 + prog)
        ev += [f"{t + 0.15:.3f} on 1 60 100", f"{t + 0.55:.3f} off 1 60"]
        t += 0.7
    return ev, t


def case_poly():
    ev = ["# 150 held notes (looped B3 organ, keys 24-91: the stand-in presets' layer key range ends at 91):",
          "# all 128 voices on both G20 chips, then 22 voice steals"]
    ev += sel(START, 1, B3) + sel(START, 2, B3) + sel(START, 3, B3)
    t = START + 0.5
    notes = [(1, n) for n in range(24, 92)] + [(2, n) for n in range(24, 92)] + [(3, n) for n in range(40, 54)]
    for i, (ch, n) in enumerate(notes):
        ev.append(f"{t + 0.03 * i:.3f} on {ch} {n} {40 + (i * 7) % 88}")
    t_off = t + 0.03 * len(notes) + 2.0
    for ch, n in notes:
        ev.append(f"{t_off:.3f} off {ch} {n}")
    return ev, t_off + 1.0


def case_ctrl():
    ev = ["# controllers on wav:Sine: velocity, pitch bend, mod wheel, volume, pan, sustain"]
    ev += sel(START, 1, SINE)
    t = START + 0.5
    for v in range(1, 128, 14):                       # velocity sweep
        ev += [f"{t:.3f} on 1 69 {v}", f"{t + 0.25:.3f} off 1 69"]
        t += 0.35
    ev.append(f"{t:.3f} on 1 69 100")                 # held note for the controllers
    for i in range(33):                               # pitch bend -8192..8191 and back to 0
        b = max(-8192, min(8191, -8192 + i * 512))
        ev.append(f"{t + 0.05 * i:.3f} pb 1 {b}")
    t += 33 * 0.05
    ev.append(f"{t:.3f} pb 1 0")
    for name, cc, vals in (("mod", 1, range(0, 128, 8)), ("vol", 7, range(127, -1, -8)),
                           ("pan", 10, range(0, 128, 8))):
        for i, v in enumerate(vals):
            ev.append(f"{t + 0.05 * i:.3f} cc 1 {cc} {v}")
        t += 0.05 * len(vals) + 0.2
    ev += [f"{t:.3f} cc 1 1 0", f"{t:.3f} cc 1 7 127", f"{t:.3f} cc 1 10 64", f"{t + 0.1:.3f} off 1 69"]
    t += 0.5
    ev.append(f"{t:.3f} cc 1 64 127")                 # sustain pedal holds released notes
    for i, n in enumerate((60, 64, 67)):
        ev += [f"{t + 0.1 + 0.1 * i:.3f} on 1 {n} 90", f"{t + 0.4 + 0.1 * i:.3f} off 1 {n}"]
    ev.append(f"{t + 1.5:.3f} cc 1 64 0")
    return ev, t + 2.0


def case_multi():
    ev = ["# 16 channels, a different preset each, staggered chords"]
    t = START
    for ch in range(1, 17):
        ev += sel(t + 0.01 * ch, ch, (ch * 61 + 7) % 1024)
    t += 0.5
    for rep in range(3):
        for ch in range(1, 17):
            base = 36 + (ch * 5) % 36
            for k, d in enumerate((0, 4, 7)):
                ev.append(f"{t + 0.04 * ch + 0.01 * k:.3f} on {ch} {base + d} {60 + ch * 3}")
        t += 1.2
        for ch in range(1, 17):
            base = 36 + (ch * 5) % 36
            for d in (0, 4, 7):
                ev.append(f"{t:.3f} off {ch} {base + d}")
        t += 0.4
    return ev, t


def case_range():
    ev = ["# key range on the unlooped organ (root 68): notes 60-100, one at a time (92-100 are above",
          "# the stand-in layer key range and start no voice)"]
    ev += sel(START, 1, ORGAN)
    t = START + 0.5
    for n in range(60, 101):
        ev += [f"{t:.3f} on 1 {n} 100", f"{t + 0.2:.3f} off 1 {n}"]
        t += 0.25
    return ev, t


# ---------------------------------------------------------------- step 4 cases (SysEx edits)
# Parameter edits go to the edit buffer of the preset on the basic channel (channel 1):
# F0 18 0F 00 55 01 <count of 14-bit words> [897 -1] [898 layer] [id value]... F7 (docs/p2k-findings.md).
# IDs and values: the Pistol Instruments Proteus 2000 Editor reference (docs/p2k-findings.md).
FILTERS = [127, 0, 1, 2, 260, 261, 262, 264, 265, 8, 9, 16, 17, 18, 32, 33, 34, 274, 259, 268, 270,
           275, 276, 277, 278, 80, 81, 271, 269, 272, 279, 280, 281, 64, 65, 282, 283, 72, 284, 285,
           263, 267, 273, 286, 287, 288, 289, 290, 266, 66, 291]      # Off + the 50 Z-plane types
CORDS_OFF = [(1921 + 3 * k + j, 0) for k in range(24) for j in range(3)]   # layer cords 1-24 cleared
A_SENDS, B_SENDS = (1157, 1158, 1159, 1167), (1164, 1165, 1166, 1168)


def ftype(v):
    """Editor filter-type value -> the byte the OS stores (preset +0xC8, range 0..255 in the OS table
    0x010491B4): 12th-order types 259-291 are 0x80 | n (descriptor table 0x01020444); sending the
    editor's 14-bit value stores its low byte, which is not a filter type."""
    return 0x80 | (v - 256) if v >= 256 else v


def edit(t, params, layer=0):
    """SysEx parameter edits (edit buffer, layer `layer`), split into messages of <= 41 values."""
    out, head = [], [(897, -1), (898, layer)]
    for i in range(0, len(params), 39):
        vals = head + params[i:i + 39]
        b = [0x18, 0x0F, 0x00, 0x55, 0x01, 2 * len(vals)]
        for pid, v in vals:
            b += [pid & 0x7F, pid >> 7, v & 0x7F, (v >> 7) & 0x7F]
        out.append(f"{t + 0.01 * (i // 39):.3f} syx " + " ".join(f"{x:02x}" for x in b))
    return out


def case_filt(part):
    types = FILTERS[part::3]
    ev = [f"# filter types {', '.join(map(str, types))} (SysEx 1537) on the looped B3 organ, note 48:",
          "# Q 0 and 127, frequency 0 swept up by the mod wheel (cord 1 ModWhl -> FiltFreq +100)"]
    ev += sel(START, 1, B3)
    t = START + 0.4
    ev += edit(t, CORDS_OFF + [(1921, 17), (1922, 56), (1923, 100)])
    t += 0.2
    for ft in types:
        for q in (0, 127):
            ev += edit(t, [(1537, ftype(ft)), (1538, 0), (1539, q)])
            ev += [f"{t + 0.05:.3f} cc 1 1 0", f"{t + 0.1:.3f} on 1 48 100"]
            ev += [f"{t + 0.15 + 0.05 * i:.3f} cc 1 1 {min(127, 11 * i)}" for i in range(13)]
            ev.append(f"{t + 0.85:.3f} off 1 48")
            t += 1.0
    return ev, t


def case_fxa(part):
    algs = list(range(1, 23)) if part == 0 else list(range(23, 45))
    ev = [f"# FX A algorithms {algs[0]}-{algs[-1]} (SysEx 1153), A send 1 = 100, B sends 0: staccato notes and tails"]
    ev += sel(START, 1, B3)
    t = START + 0.4
    ev += edit(t, [(a, 100 if a == 1157 else 0) for a in A_SENDS] + [(b, 0) for b in B_SENDS] + [(1156, 0)])
    t += 0.2
    runs = [(a, None) for a in algs]
    if part == 0:   # decay / HF damping on Room 1 and Delay
        runs += [(a, dd) for a in (1, 7) for dd in ((0, 0), (90, 0), (90, 127))]
    for alg, dd in runs:
        ev += edit(t, [(1153, alg)] + ([(1154, dd[0]), (1155, dd[1])] if dd else []))
        ev += [f"{t + 0.1:.3f} on 1 60 110", f"{t + 0.3:.3f} off 1 60"]
        t += 1.8
    return ev, t


def case_fxb():
    ev = ["# FX B algorithms 1-32 (SysEx 1160), B send 1 = 100, A sends 0; feedback, LFO rate and delay variants"]
    ev += sel(START, 1, B3)
    t = START + 0.4
    ev += edit(t, [(a, 0) for a in A_SENDS] + [(b, 100 if b == 1164 else 0) for b in B_SENDS] + [(1156, 0)])
    t += 0.2
    runs = [(b, []) for b in range(1, 33)]
    runs += [(1, [(1161, fb), (1162, r)]) for fb, r in ((0, 0), (127, 127), (64, 3))]
    runs += [(18, [(1161, 100), (1163, d)]) for d in (0, 64, 127)]
    for alg, extra in runs:
        ev += edit(t, [(1160, alg)] + extra)
        ev += [f"{t + 0.1:.3f} on 1 60 110", f"{t + 0.9:.3f} off 1 60"]
        t += 1.8
    return ev, t


def case_route():
    ev = ["# layer Mix Output Main / Sub 1 / Sub 2 (SysEx 1412) x A and B sends 1-4 (Room 1, Chorus 1), then B -> A"]
    ev += sel(START, 1, B3)
    t = START + 0.4
    ev += edit(t, [(1153, 1), (1160, 1), (1156, 0)])
    t += 0.2
    for out in (0, 1, 2):
        for k in range(4):
            sends = [(a, 100 if i == k else 0) for i, a in enumerate(A_SENDS)] + \
                    [(b, 100 if i == k else 0) for i, b in enumerate(B_SENDS)]
            ev += edit(t, [(1412, out)] + sends)
            ev += [f"{t + 0.1:.3f} on 1 60 110", f"{t + 0.6:.3f} off 1 60"]
            t += 1.5
    ev += edit(t, [(1412, 0), (1156, 127), (1157, 100), (1164, 100)])
    ev += [f"{t + 0.1:.3f} on 1 60 110", f"{t + 0.6:.3f} off 1 60"]
    return ev, t + 1.5


def case_mod():
    ev = ["# CPU-side modulation: LFO 1 -> pitch / filter with shapes, volume and filter envelopes,",
          "# glide in solo mode, coarse tune (the arpeggiator did not play over SysEx: docs/p2k-findings.md)"]
    ev += sel(START, 1, B3)
    t = START + 0.4
    ev += edit(t, CORDS_OFF + [(1537, 0), (1538, 128), (1539, 64)])
    t += 0.2
    for shape in (1, 0, 2, 3, -1):                            # sine, triangle, saw, square, random
        ev += edit(t, [(1665, 90), (1666, shape), (1921, 96), (1922, 48), (1923, 30), (1924, 96), (1925, 56), (1926, 60)])
        ev += [f"{t + 0.1:.3f} on 1 60 100", f"{t + 1.4:.3f} off 1 60"]
        t += 1.8
    ev += edit(t, CORDS_OFF + [(1793, 1), (1794, 60), (1795, 100), (1796, 40), (1797, 50), (1798, 50), (1799, 0)])
    ev += [f"{t + 0.1:.3f} on 1 60 100", f"{t + 1.1:.3f} off 1 60"]
    t += 2.6
    ev += edit(t, [(1806, 2), (1807, 30), (1808, 100), (1809, 50), (1810, 0), (1811, 40), (1812, 0),
                   (1921, 80), (1922, 56), (1923, 100), (1538, 0)])
    ev += [f"{t + 0.1:.3f} on 1 60 100", f"{t + 1.1:.3f} off 1 60"]
    t += 2.6
    ev += edit(t, CORDS_OFF + [(1793, 0), (1806, 1), (1538, 255), (1437, 2), (1432, 60)])   # Melody Last, glide
    for i, n in enumerate((48, 60, 55, 67, 72)):
        ev.append(f"{t + 0.1 + 0.4 * i:.3f} on 1 {n} 100")
        ev.append(f"{t + 0.5 + 0.4 * i:.3f} off 1 {n}")
    t += 2.5
    ev += edit(t, [(1437, 0), (1432, 0), (1425, 12)])
    ev += [f"{t + 0.1:.3f} on 1 48 100", f"{t + 0.6:.3f} off 1 48"]
    t += 1.0
    return ev, t + 0.5


def case_soak():
    """10 minutes of dense, deterministic pseudo-random playing on 16 channels."""
    ev = ["# soak: 600 s of pseudo-random notes, controllers, program changes and SysEx edits on 16 channels"]
    seed = 12345

    def rnd(n):
        nonlocal seed
        seed = (seed * 1103515245 + 12345) & 0x7FFFFFFF
        return (seed >> 8) % n
    t, end, held = START, START + 600.0, []
    for ch in range(1, 17):
        ev += sel(t + 0.01 * ch, ch, rnd(1024))
    t += 0.5
    while t < end:
        t += 0.02 + rnd(100) / 1000.0
        k = rnd(100)
        if k < 55 or not held:
            ch, n = 1 + rnd(16), 30 + rnd(60)
            ev.append(f"{t:.3f} on {ch} {n} {20 + rnd(108)}")
            held.append((ch, n))
        elif k < 85:
            ch, n = held.pop(rnd(len(held)))
            ev.append(f"{t:.3f} off {ch} {n}")
        elif k < 93:
            ev.append(f"{t:.3f} cc {1 + rnd(16)} {(1, 7, 10, 11, 64)[rnd(5)]} {rnd(128)}")
        elif k < 96:
            ev.append(f"{t:.3f} pb {1 + rnd(16)} {rnd(16384) - 8192}")
        elif k < 99:
            ev += sel(t, 1 + rnd(16), rnd(1024))
        else:
            ev += edit(t, [(1537, ftype(FILTERS[rnd(len(FILTERS))])), (1538, rnd(256)), (1153, 1 + rnd(44)), (1160, 1 + rnd(32))])
    for ch, n in held:
        ev.append(f"{end:.3f} off {ch} {n}")
    for ch in range(1, 17):
        ev.append(f"{end:.3f} cc {ch} 64 0")
    return ev, end


CASES = {"boot": case_boot, "poly": case_poly, "ctrl": case_ctrl, "multi": case_multi, "range": case_range}
CASES.update({f"sweep{b}": (lambda b=b: case_sweep(b)) for b in range(8)})
CASES.update({f"filt{p}": (lambda p=p: case_filt(p)) for p in range(3)})
CASES.update({"fxa0": lambda: case_fxa(0), "fxa1": lambda: case_fxa(1), "fxb": case_fxb, "route": case_route,
              "mod": case_mod, "soak": case_soak})


def gen():
    os.makedirs(CASEDIR, exist_ok=True)
    for name, fn in CASES.items():
        ev, end = fn()
        with open(os.path.join(CASEDIR, name + ".txt"), "w") as f:
            f.write(f"# p2k_corpus case {name}; render to {end + TAIL:.3f} s (file time)\n")
            f.write("\n".join(ev) + "\n")
    print(f"wrote {len(CASES)} cases to {CASEDIR}")


def case_seconds(path):
    m = re.search(r"render to ([\d.]+) s", pathlib.Path(path).read_text().split("\n", 1)[0])
    return float(m.group(1)) + MIDI_OFFSET


def sha(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for blk in iter(lambda: f.read(1 << 20), b""):
            h.update(blk)
    return h.hexdigest()


def wav_stats(path):
    with open(path, "rb") as f:
        d = f.read()
    pos, fmt, data = 12, None, b""
    while pos + 8 <= len(d):
        cid, n = d[pos:pos + 4], struct.unpack("<I", d[pos + 4:pos + 8])[0]
        if cid == b"fmt ":
            fmt = struct.unpack("<HHIIHH", d[pos + 8:pos + 24])
        elif cid == b"data":
            data = d[pos + 8:pos + 8 + n]
        pos += 8 + n + (n & 1)
    chans, rate = fmt[1], fmt[2]
    n = len(data) // (2 * chans)
    smp = struct.unpack(f"<{n * chans}h", data[:n * chans * 2])
    peak = max((abs(x) for x in smp), default=0)
    rms = [(sum(x * x for x in smp[c::chans]) / max(n, 1)) ** 0.5 for c in range(chans)]
    blk = rate // 10                                  # 100 ms blocks above -60 dBFS
    loud = sum(1 for i in range(0, n, blk) if max((abs(x) for x in smp[i * chans:(i + blk) * chans]), default=0) > 32)
    return {"rate": rate, "channels": chans, "frames": n, "peak": peak,
            "rms": [round(r, 1) for r in rms], "audible_100ms_blocks": loud}


def trace_stats(lines):
    """Counts by kind (G voice, H filter, HR filter read, X G05, MIDI byte) and note starts: G20
    +0x18 writes (sample start, the last write of the start sequence) after the first MIDI byte."""
    kinds, started, starts, midi = {}, set(), 0, False
    for ln in lines:
        p = ln.split()
        k = "MIDI" if p[2] == "MIDI" else p[3]
        kinds[k] = kinds.get(k, 0) + 1
        midi = midi or k == "MIDI"
        if k == "G" and midi and p[5] == "18":
            starts += 1
            started.add(int(p[4][1:]))
    return {"lines": len(lines), "by_kind": dict(sorted(kinds.items())), "note_starts": starts,
            "voices_started": len(started), "voices_started_64_127": sum(1 for v in started if v >= 64)}


def sndlog_stats(path):
    """Features a case exercises, from its sound-register log (p2k_sound.h set_log format)."""
    n = {"G": 0, "H": 0, "X": 0}
    starts, coef, cut, rb, x, live = 0, set(), set(), set(), set(), set()
    with open(path) as f:
        for ln in f:
            p = ln.split()
            if len(p) < 5 or p[0] == "#":
                continue
            n[p[1]] += 1
            if p[1] == "G" and int(p[2], 16) & 0x3f == 0x1a:
                starts += 1
            elif p[1] == "H":
                a, d = int(p[2], 16), int(p[3], 16)
                o = a & 0x7f
                if o < 0x28 and d:
                    coef.add(d)
                elif o == 0x3e:
                    cut.add(d & 0xff)
                elif o == 0x7a and p[4] == "00ff":
                    rb.add(d & 0xff)
                elif o == 0x7c and p[4] == "ff00":
                    live.add(d >> 8)
            elif p[1] == "X":
                x.add((p[2], p[3], p[4]))
    return {"writes": n, "voice_starts": starts, "filter_words": len(coef), "cutoff_values": len(cut),
            "cutoff_live_values": len(live),
            "route_flag_bytes": sorted(rb), "g05_distinct_writes": len(x)}


def render(name, mame, rompath, simm, out, rep):
    d = os.path.join(out, f"{name}.{rep}")
    if os.path.isdir(d):
        shutil.rmtree(d)
    os.makedirs(os.path.join(d, "nvram", "proteus2k"))
    shutil.copy(os.path.join(simm, "flash_clean.nv"), os.path.join(d, "nvram", "proteus2k", "flash"))
    script = os.path.join(CASEDIR, name + ".txt")
    secs = case_seconds(script)
    args = [mame, "proteus2k", "-rompath", rompath, "-video", "none", "-sound", "none", "-nothrottle",
            "-skip_gameinfo", "-keyboardprovider", "none", "-mouseprovider", "none",
            "-joystickprovider", "none", "-log", "-seconds_to_run", f"{secs:.3f}",
            "-wavwrite", os.path.join(d, "out.wav")]
    if any(ln.strip() and not ln.startswith("#") for ln in pathlib.Path(script).read_text().splitlines()):
        subprocess.run([sys.executable, os.path.join(HERE, "..", "..", "..", "modules", "synthframe", "tools", "midi", "mkmidi.py"), script, os.path.join(d, "in.mid")], check=True)
        args += ["-midiin", os.path.join(d, "in.mid")]
    env = dict(os.environ, SDL_VIDEODRIVER="dummy", SDL_AUDIODRIVER="dummy", P2K_TRACE="1",
               P2K_SNDLOG=os.path.join(d, "snd.log"), P2K_SNDOUT=os.path.join(d, "snd.raw"),
               P2K_SIMM1=os.path.join(simm, "big.bin"), P2K_BABY1=os.path.join(simm, "baby.bin"))
    for k in ("P2K_NOFX", "P2K_NOFILTER", "P2K_CLEAN", "P2K_BUSTRACE"):
        env.pop(k, None)
    lim = lambda: resource.setrlimit(resource.RLIMIT_FSIZE, (2 << 30, 2 << 30))
    r = subprocess.run(args, cwd=d, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, preexec_fn=lim)
    if r.returncode != 0:
        raise RuntimeError(f"{name}: MAME exit {r.returncode}: {r.stdout.decode(errors='ignore')[-500:]}")
    trace, lcd = [], []
    with open(os.path.join(d, "error.log"), errors="ignore") as f:
        for ln in f:
            if ln.startswith("[:] T "):
                trace.append(ln[4:].rstrip("\n"))
            elif ln.startswith("[:] LCD "):
                lcd.append(ln[4:].rstrip("\n"))
    with open(os.path.join(d, "sound.trace"), "w") as f:
        f.write("\n".join(trace) + "\n")
    return {"seconds": round(secs, 3), "midi_sha256": sha(script),
            "wav_sha256": sha(os.path.join(d, "out.wav")), "trace_sha256": sha(os.path.join(d, "sound.trace")),
            "sndlog_sha256": sha(os.path.join(d, "snd.log")), "sndout_sha256": sha(os.path.join(d, "snd.raw")),
            "wav": wav_stats(os.path.join(d, "out.wav")), "trace": trace_stats(trace),
            "sndlog": sndlog_stats(os.path.join(d, "snd.log")),
            "lcd_last": lcd[-1] if lcd else None}


HASHES = ("wav_sha256", "trace_sha256", "sndlog_sha256", "sndout_sha256")

# Step 4 gate: each case's register log exercises the feature it names. The boot alone writes
# 8196 G05 words, 7 distinct filter words and route/flag bytes {0, 1, 0x72} (case boot, sweep0).
FEATURES = {
    "filt": lambda st, tr: st["filter_words"] >= 40 and st["cutoff_live_values"] >= 12
    and any(b & 0x80 for b in st["sndlog"]["route_flag_bytes"]),
    "fxa": lambda st, tr: st["writes"]["X"] > 30000,
    "fxb": lambda st, tr: st["writes"]["X"] > 30000,
    "route": lambda st, tr: {0x72, 0x7B, 0x44} <= set(st["route_flag_bytes"]),
    "mod": lambda st, tr: st["cutoff_live_values"] >= 20 and tr["note_starts"] >= 13,
    "soak": lambda st, tr: tr["note_starts"] >= 4000 and tr["voices_started"] >= 112 and st["filter_words"] >= 100
    and st["writes"]["X"] > 100000,
}


def feature_ok(name, v):
    for k, fn in FEATURES.items():
        if name.startswith(k):
            st = dict(v["sndlog"], sndlog=v["sndlog"])
            return fn(st, v["trace"])
    return None


def inputs(simm):
    patches = os.path.join(CORE, "mame", "patches")
    return {"simm": {f: sha(os.path.join(simm, f)) for f in ("big.bin", "baby.bin", "flash_clean.nv")},
            "mame_base": pathlib.Path(os.path.join(patches, "BASE")).read_text().split()[-1],
            "mame_patches": {f: sha(os.path.join(patches, f)) for f in sorted(os.listdir(patches)) if f.endswith(".patch")}}


def run(a):
    mame, rompath, simm = a.mame or os.environ.get("P2K_MAME"), a.rompath or os.environ.get("P2K_ROMPATH"), \
        a.simm or os.environ.get("P2K_SIMMDIR")
    if not (mame and rompath and simm):
        raise SystemExit("set P2K_MAME, P2K_ROMPATH and P2K_SIMMDIR (or --mame/--rompath/--simm)")
    names = a.cases.split(",") if a.cases else list(CASES)
    man = json.loads(pathlib.Path(MANIFEST).read_text()) if os.path.exists(MANIFEST) else {"inputs": None, "cases": {}}
    inp = inputs(simm)
    if man["inputs"] and man["inputs"] != inp and not a.update:
        print("INPUTS differ from the manifest:")
        for k in inp:
            if inp[k] != man["inputs"].get(k):
                print(f"  {k}: {man['inputs'].get(k)} -> {inp[k]}")
    jobs = [(n, r) for n in names for r in range(a.repeat)]
    with ThreadPoolExecutor(a.jobs) as ex:
        res = list(ex.map(lambda j: (j, render(j[0], mame, rompath, simm, a.out, j[1])), jobs))
    out, bad = {}, 0
    for (n, r), v in res:
        if n in out:
            if any(out[n][k] != v[k] for k in HASHES):
                print(f"NONDETERMINISTIC {n}: run {r} differs from run 0")
                bad += 1
            continue
        out[n] = v
    for n in names:
        v, ref = out[n], man["cases"].get(n)
        same = ref and all(ref.get(k) == v[k] for k in HASHES)
        tag = "same" if same else ("new" if not ref else "DIFFERENT")
        if not same and ref and not a.update:
            bad += 1
        fok = feature_ok(n, v)
        if fok is False:
            print(f"FEATURE   {n}: its register log does not exercise the feature it names: {v['sndlog']}")
            bad += 1
        print(f"{tag:9} {n:8} {v['seconds']:7.2f}s out {v['sndout_sha256'][:12]} log {v['sndlog_sha256'][:12]} "
              f"starts {v['trace']['note_starts']:4} voices {v['trace']['voices_started']:3} peak {v['wav']['peak']:5} "
              f"| {v['lcd_last']}")
    if a.update:
        man["inputs"] = inp
        man["cases"].update(out)
        with open(MANIFEST, "w") as f:
            json.dump(man, f, indent=1, sort_keys=True)
            f.write("\n")
        print(f"updated {MANIFEST}")
    return 1 if bad else 0


def patch_copy_ok():
    """The model headers that mame/patches/0004 adds are the ones in p2k/model."""
    files, cur = {}, None
    for ln in pathlib.Path(os.path.join(CORE, "mame", "patches", "0004-p2k-integer-sound-model.patch")).read_text().splitlines(keepends=True):
        if ln.startswith("diff --git"):
            name = ln.rstrip().rsplit("/", 1)[-1]
            cur = name if name in ("p2k_sound.h", "p2k_flash.h") else None
            if cur:
                files[cur] = []
        elif cur and ln.startswith("+") and not ln.startswith("+++"):
            files[cur].append(ln[1:])
    return all("".join(files.get(h, [])) == pathlib.Path(os.path.join(CORE, "p2k", "model", h)).read_text()
               for h in ("p2k_sound.h", "p2k_flash.h"))


def model(a):
    """Step 3 gate: p2k_render fed only snd.log and the SIMM reproduces MAME's model output."""
    if not patch_copy_ok():
        print("p2k/model headers differ from the copies in mame/patches/0004 (mame/scripts/mkpatch0004.sh)")
        return 1
    simm = a.simm or os.environ.get("P2K_SIMMDIR")
    if not simm:
        raise SystemExit("set P2K_SIMMDIR (or --simm)")
    exe = subprocess.run([os.path.join(CORE, "p2k", "model", "build.sh")], check=True,
                         stdout=subprocess.PIPE, text=True).stdout.strip()
    names = a.cases.split(",") if a.cases else list(CASES)
    man = json.loads(pathlib.Path(MANIFEST).read_text())

    def one(n):
        d = os.path.join(a.out, f"{n}.0")
        raw = os.path.join(d, "snd.raw")
        frames = os.path.getsize(raw) // 4
        args = [exe, "--simm", "1=" + os.path.join(simm, "big.bin"), "--frames", str(frames)]
        r = subprocess.run(args + (["--snr"] if a.snr else []) + [os.path.join(d, "snd.log"), os.path.join(d, "render.raw")],
                           check=True, stdout=subprocess.PIPE, text=True).stdout.strip()
        same = sha(os.path.join(d, "render.raw")) == sha(raw)
        ref = man["cases"].get(n, {}).get("sndout_sha256")
        return n, same, ref == sha(raw), r

    bad = 0
    with ThreadPoolExecutor(a.jobs) as ex:
        for n, same, inman, r in ex.map(one, names):
            bad += not (same and inman)
            print(f"{'same' if same else 'DIFFERENT':9} {n:8} {'' if inman else '(snd.raw not the manifest hash) '}{r}")
    return 1 if bad else 0


def main():
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest="cmd", required=True)
    sub.add_parser("gen")
    r = sub.add_parser("run")
    r.add_argument("--mame"); r.add_argument("--rompath"); r.add_argument("--simm")
    r.add_argument("--out", default=os.path.join(CORE, "build", "p2k", "corpus"))
    r.add_argument("--cases")
    r.add_argument("--repeat", type=int, default=1)
    r.add_argument("--jobs", type=int, default=4)
    r.add_argument("--update", action="store_true")
    m = sub.add_parser("model")
    m.add_argument("--simm")
    m.add_argument("--out", default=os.path.join(CORE, "build", "p2k", "corpus"))
    m.add_argument("--cases")
    m.add_argument("--jobs", type=int, default=4)
    m.add_argument("--snr", action="store_true")
    a = ap.parse_args()
    if a.cmd == "model":
        return model(a)
    if a.cmd == "gen":
        gen()
        return 0
    return run(a)


if __name__ == "__main__":
    raise SystemExit(main())
