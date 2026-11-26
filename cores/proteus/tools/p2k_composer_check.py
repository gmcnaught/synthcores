#!/usr/bin/env python3
"""Play Composer presets (ROM 4) in the MAME research machine with a SIMM and report, per preset,
the name on the LCD and the output level of one note (tools/p2k_build_composer.py builds the SIMM).

  p2k_composer_check.py SIMMDIR OUTDIR [--presets 0,1,128,...|--every N] [--note 60]
needs P2K_MAME and P2K_ROMPATH (p2k_corpus.py); SIMMDIR has big.bin, baby.bin and flash_clean.nv
(a P2K_CLEAN=1 boot; any SIMM's will do: it holds the user bank and settings).
Prints one line per preset: number, LCD name, RMS and peak of its note (dBFS), and writes
OUTDIR/check.json. Exit 1 if a preset's name is not on the LCD or a note is silent (< -60 dBFS).
"""
import argparse
import json
import math
import os
import re
import shutil
import struct
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
START, STEP, NOTE_LEN = 2.0, 1.5, 0.8      # MIDI file time; MAME starts the file 10 s after power-on


def script(presets, note):
    lines, t = [], START
    for p in presets:
        lines += [f"{t:.3f} cc 1 0 4", f"{t:.3f} cc 1 32 {p // 128}", f"{t + 0.05:.3f} pc 1 {p % 128}",
                  f"{t + 0.4:.3f} on 1 {note} 110", f"{t + 0.4 + NOTE_LEN:.3f} off 1 {note}"]
        t += STEP
    return lines, t + 1.0


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("simm")
    ap.add_argument("out")
    ap.add_argument("--presets")
    ap.add_argument("--every", type=int, default=64)
    ap.add_argument("--note", type=int, default=60)
    ap.add_argument("--reuse", action="store_true", help="analyse OUTDIR's existing render")
    a = ap.parse_args()
    presets = [int(x) for x in a.presets.split(",")] if a.presets else list(range(0, 1024, a.every))
    mame, rompath = os.environ["P2K_MAME"], os.environ["P2K_ROMPATH"]
    d = os.path.abspath(a.out)
    if not a.reuse:
        shutil.rmtree(d, ignore_errors=True)
    os.makedirs(os.path.join(d, "nvram", "proteus2k"), exist_ok=True)
    shutil.copy(os.path.join(a.simm, "flash_clean.nv"), os.path.join(d, "nvram", "proteus2k", "flash"))
    lines, end = script(presets, a.note)
    with open(os.path.join(d, "in.txt"), "w") as f:
        f.write("\n".join(lines) + "\n")
    subprocess.run([sys.executable, os.path.join(HERE, "mkmidi.py"), os.path.join(d, "in.txt"),
                    os.path.join(d, "in.mid")], check=True, stdout=subprocess.DEVNULL)
    env = dict(os.environ, SDL_VIDEODRIVER="dummy", SDL_AUDIODRIVER="dummy",
               P2K_SIMM1=os.path.abspath(os.path.join(a.simm, "big.bin")),
               P2K_BABY1=os.path.abspath(os.path.join(a.simm, "baby.bin")))
    for k in ("P2K_NOFX", "P2K_NOFILTER", "P2K_CLEAN", "P2K_TRACE", "P2K_SNDLOG", "P2K_SNDOUT"):
        env.pop(k, None)
    args = [mame, "proteus2k", "-rompath", rompath, "-video", "none", "-sound", "none", "-nothrottle",
                    "-skip_gameinfo", "-keyboardprovider", "none", "-mouseprovider", "none",
                    "-joystickprovider", "none", "-log", "-seconds_to_run", f"{10 + end:.3f}",
                    "-wavwrite", os.path.join(d, "out.wav"), "-midiin", os.path.join(d, "in.mid")]
    if not a.reuse:
        subprocess.run(args, cwd=d, env=env, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.STDOUT)
    lcd = []                                    # (emulated seconds, text) from the driver's LCD log
    with open(os.path.join(d, "error.log"), errors="ignore") as f:
        for ln in f:
            if ln.startswith("[:] LCD "):
                m = re.match(r"\[:\] LCD\s+([\d.]+)\s+(.*)", ln.rstrip("\n"))
                if m:
                    lcd.append((float(m[1]), m[2]))
                else:
                    lcd.append((None, ln[8:].rstrip("\n")))
    with open(os.path.join(d, "out.wav"), "rb") as f:
        w = f.read()
    rate = struct.unpack_from("<I", w, 24)[0]
    data0 = w.index(b"data") + 8

    def segment(t0, t1):                        # stereo 16-bit frames between two times
        a, b = int(t0 * rate) * 4, int(t1 * rate) * 4
        b = min(b, len(w) - data0) // 4 * 4
        return struct.unpack_from(f"<{max(0, b - a) // 2}h", w, data0 + a) if b > a else ()
    results, bad = [], 0
    for k, p in enumerate(presets):
        t0 = 10 + START + k * STEP + 0.4
        seg = segment(t0, t0 + NOTE_LEN)
        rms = math.sqrt(sum(x * x for x in seg) / max(1, len(seg)))
        peak = max((abs(x) for x in seg), default=0)
        db = lambda v: round(20 * math.log10(v / 32768), 1) if v > 0 else -120.0
        shown = [txt for t, txt in lcd if t is None or (t0 - 0.4 <= t <= t0 + NOTE_LEN)]
        results.append(dict(preset=p, rms_db=db(rms), peak_db=db(peak), lcd=shown[-1] if shown else None))
        bad += db(rms) < -60
        print(f"{p:5d}  {results[-1]['rms_db']:7.1f} dB rms  {results[-1]['peak_db']:7.1f} dB peak  {results[-1]['lcd']}")
    with open(os.path.join(d, "check.json"), "w") as f:
        json.dump(dict(lcd_lines=len(lcd), results=results), f, indent=1)
    print(f"{len(presets)} presets, {bad} silent")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
