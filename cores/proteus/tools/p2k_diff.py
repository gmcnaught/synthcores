#!/usr/bin/env python3
"""Differential register capture for the proteus2k MAME research machine.

Each run builds a SIMM variant (tests/p2k_simm_demo.json; preset 0 layer 0 gets the given
parameter values by SysEx ID, offsets from tools/p2k_params.py), selects bank MSB 4 / program 0
and plays note 60 vel 100 for 2 s. (SysEx parameter edits were sent and had no effect.) The G20/H writes of the note-on and note-off are extracted from error.log
(driver tracing with P2K_TRACE=1) and compared with a baseline run.

  p2k_diff.py run NAME [ID=VALUE ...] [--note N] [--vel N]
  p2k_diff.py diff BASE NAME
Runs go to build/p2k/diff/NAME. SIMM images: build/p2k/simm/demo (tools/p2k_build_simm.py).
"""
import collections
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(ROOT, "build", "p2k", "diff")


def build_simm(d, edits):
    """SIMM variant: tests/p2k_simm_demo.json with layer-0 parameter edits on preset 0."""
    import json
    spec = json.load(open(os.path.join(ROOT, "tests", "p2k_simm_demo.json")))
    spec["presets"][0]["params"] = {str(k): v for k, v in edits}
    path = os.path.join(d, "spec.json")
    json.dump(spec, open(path, "w"))
    simm = os.path.join(d, "simm")
    subprocess.run([sys.executable, os.path.join(ROOT, "tools", "p2k_build_simm.py"), path, simm],
                   check=True, cwd=ROOT, stdout=subprocess.DEVNULL)
    return simm


def run(name, edits, note=60, vel=100):
    d = os.path.join(OUT, name)
    os.makedirs(d, exist_ok=True)
    simm = build_simm(d, edits)
    script = ["14.0 cc 1 0 4", "14.0 cc 1 32 0", "14.1 pc 1 0", f"15.0 on 1 {note} {vel}", f"17.0 off 1 {note}"]
    open(os.path.join(d, "script.txt"), "w").write("\n".join(script) + "\n")
    mid = os.path.join(d, "run.mid")
    subprocess.run([sys.executable, os.path.join(ROOT, "..", "..", "modules", "synthframe", "tools", "midi", "mkmidi.py"), os.path.join(d, "script.txt"), mid], check=True)
    nv = os.path.join(ROOT, "build", "p2k", "simm", "demo", "flash_clean.nv")
    if os.path.exists(nv):   # factory-default user data (the dump's own has MIDI channel 1 off)
        os.makedirs(os.path.join(d, "nvram", "proteus2k"), exist_ok=True)
        import shutil
        shutil.copy(nv, os.path.join(d, "nvram", "proteus2k", "flash"))
    env = dict(os.environ, P2K_TRACE="1", P2K_SIMM1=os.path.join(simm, "big.bin"), P2K_BABY1=os.path.join(simm, "baby.bin"))
    extra = ["-autoboot_script", os.environ["P2K_DIFF_LUA"]] if os.environ.get("P2K_DIFF_LUA") else []
    subprocess.run([os.path.join(ROOT, "tools", "p2k-run.sh"), "300", d, "-seconds_to_run", "29", "-midiin", mid,
                    "-wavwrite", os.path.join(d, "out.wav")] + extra, env=env, check=True, stdout=subprocess.DEVNULL)
    os.remove(os.path.join(simm, "big.bin"))
    return parse(d)


LINE = re.compile(r"T ([\d.]+) (?:([0-9a-f]{8}) )?(MIDI|G v(\d+) (..) = (....)|H (......) = (....) & (....))")


def parse(d):
    ev, midi = [], []
    for l in open(os.path.join(d, "error.log"), errors="replace"):
        m = LINE.search(l)
        if not m:
            continue
        t = float(m.group(1))
        if m.group(3) == "MIDI":
            midi.append(t)
        elif m.group(4) is not None:
            ev.append((t, m.group(2), "G%03d.%s" % (int(m.group(4)), m.group(5)), int(m.group(6), 16)))
        else:
            addr, val, mask = int(m.group(7), 16), int(m.group(8), 16), int(m.group(9), 16)
            if mask == 0x00FF:
                ev.append((t, m.group(2), "H%06x" % (addr + 1), val & 0xFF))
            elif mask == 0xFF00:
                ev.append((t, m.group(2), "H%06x" % addr, val >> 8))
            else:
                ev.append((t, m.group(2), "H%06x" % addr, val))
    on, off = midi[-2], midi[-1]
    seg = {"on": [e for e in ev if on <= e[0] < on + 0.1],
           "sustain": [e for e in ev if on + 0.1 <= e[0] < off],
           "off": [e for e in ev if off <= e[0] < off + 0.1],
           "after": [e for e in ev if e[0] >= off + 0.1]}
    return seg


def final(seg):
    """Last value written to each register in the segment, with the writer PC."""
    r = collections.OrderedDict()
    for t, pc, reg, v in seg:
        r[reg] = (v, pc)
    return r


def show(name):
    seg = parse(os.path.join(OUT, name))
    for k, v in seg.items():
        print(f"{k}: {len(v)} writes")
        if k in ("on", "off"):
            for reg, (val, pc) in final(v).items():
                print(f"   {reg} = {val:04x}  ({pc})")


def diff(base, name):
    a, b = parse(os.path.join(OUT, base)), parse(os.path.join(OUT, name))
    for k in ("on", "off"):
        fa, fb = final(a[k]), final(b[k])
        for reg in sorted(set(fa) | set(fb)):
            va, vb = fa.get(reg, (None, ""))[0], fb.get(reg, (None, ""))[0]
            if va != vb:
                fmt = lambda x: "----" if x is None else "%04x" % x
                print(f"{k:4s} {reg:10s} {fmt(va)} -> {fmt(vb)}  ({fb.get(reg, fa.get(reg))[1]})")
    for k in ("sustain", "after"):
        if len(a[k]) != len(b[k]):
            print(f"{k}: {len(a[k])} -> {len(b[k])} writes")


def main():
    if sys.argv[1] == "run":
        args = sys.argv[3:]
        opts = {"--note": 60, "--vel": 100}
        edits = []
        i = 0
        while i < len(args):
            if args[i] in opts:
                opts[args[i]] = int(args[i + 1]); i += 2
            else:
                pid, val = args[i].split("="); edits.append((int(pid), int(val))); i += 1
        run(sys.argv[2], edits, opts["--note"], opts["--vel"])
        show(sys.argv[2])
    elif sys.argv[1] == "show":
        show(sys.argv[2])
    else:
        diff(sys.argv[2], sys.argv[3])


if __name__ == "__main__":
    main()
