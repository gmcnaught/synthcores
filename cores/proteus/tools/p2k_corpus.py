#!/usr/bin/env python3
"""Proteus 2000 reference corpus: fixed MIDI cases rendered by the MAME research machine
(proteus2k) with the stand-in Composer SIMM. Each case records the audio (-wavwrite) and the
sound-chip register log (P2K_TRACE: G20 voice, H filter and G05 writes with time and PC). The
hashes and summary statistics go to tests/p2k_corpus/manifest.json; a sound core is checked
against the logs and audio of these runs, as the Proteus/1 is checked against MAME.

  p2k_corpus.py gen                                  write tests/p2k_corpus/<case>.txt
  p2k_corpus.py run [--update] [--repeat N] [--jobs N] [--cases a,b] [--out DIR]

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


CASES = {"boot": case_boot, "poly": case_poly, "ctrl": case_ctrl, "multi": case_multi, "range": case_range}
CASES.update({f"sweep{b}": (lambda b=b: case_sweep(b)) for b in range(8)})


def gen():
    os.makedirs(CASEDIR, exist_ok=True)
    for name, fn in CASES.items():
        ev, end = fn()
        with open(os.path.join(CASEDIR, name + ".txt"), "w") as f:
            f.write(f"# p2k_corpus case {name}; render to {end + TAIL:.3f} s (file time)\n")
            f.write("\n".join(ev) + "\n")
    print(f"wrote {len(CASES)} cases to {CASEDIR}")


def case_seconds(path):
    m = re.search(r"render to ([\d.]+) s", open(path).readline())
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
    if any(ln.strip() and not ln.startswith("#") for ln in open(script)):
        subprocess.run([sys.executable, os.path.join(HERE, "mkmidi.py"), script, os.path.join(d, "in.mid")], check=True)
        args += ["-midiin", os.path.join(d, "in.mid")]
    env = dict(os.environ, SDL_VIDEODRIVER="dummy", SDL_AUDIODRIVER="dummy", P2K_TRACE="1",
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
            "wav": wav_stats(os.path.join(d, "out.wav")), "trace": trace_stats(trace),
            "lcd_last": lcd[-1] if lcd else None}


def inputs(simm):
    patches = os.path.join(CORE, "mame", "patches")
    return {"simm": {f: sha(os.path.join(simm, f)) for f in ("big.bin", "baby.bin", "flash_clean.nv")},
            "mame_base": open(os.path.join(patches, "BASE")).read().split()[-1],
            "mame_patches": {f: sha(os.path.join(patches, f)) for f in sorted(os.listdir(patches)) if f.endswith(".patch")}}


def run(a):
    mame, rompath, simm = a.mame or os.environ.get("P2K_MAME"), a.rompath or os.environ.get("P2K_ROMPATH"), \
        a.simm or os.environ.get("P2K_SIMMDIR")
    if not (mame and rompath and simm):
        raise SystemExit("set P2K_MAME, P2K_ROMPATH and P2K_SIMMDIR (or --mame/--rompath/--simm)")
    names = a.cases.split(",") if a.cases else list(CASES)
    man = json.load(open(MANIFEST)) if os.path.exists(MANIFEST) else {"inputs": None, "cases": {}}
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
            if (out[n]["wav_sha256"], out[n]["trace_sha256"]) != (v["wav_sha256"], v["trace_sha256"]):
                print(f"NONDETERMINISTIC {n}: run {r} differs from run 0")
                bad += 1
            continue
        out[n] = v
    for n in names:
        v, ref = out[n], man["cases"].get(n)
        same = ref and ref["wav_sha256"] == v["wav_sha256"] and ref["trace_sha256"] == v["trace_sha256"]
        tag = "same" if same else ("new" if not ref else "DIFFERENT")
        if not same and ref and not a.update:
            bad += 1
        print(f"{tag:9} {n:8} {v['seconds']:7.2f}s wav {v['wav_sha256'][:12]} trace {v['trace_sha256'][:12]} "
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
    a = ap.parse_args()
    if a.cmd == "gen":
        gen()
        return 0
    return run(a)


if __name__ == "__main__":
    raise SystemExit(main())
