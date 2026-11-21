#!/usr/bin/env python3
"""Make a tools/p2k_build_simm.py spec from a folder of WAV samples (research; not E-mu's layout).

Samples are grouped into instruments by file name with the note/velocity tokens removed
("Piano 1 C#4.wav", "Piano 1 F#5.wav" -> "Piano 1"); each group gets key zones split between
the samples' root keys (smpl chunk). One preset per instrument (up to MAX_PRESETS), named
"<cat>:<name>" with the category taken from the folder.

  p2k_spec_from_dir.py SAMPLE_DIR OUT.json [--id 4] [--name P2Ksy] [--budget BYTES]
"""
import argparse
import collections
import json
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import p2k_wav  # noqa: E402

NOTE = re.compile(r"\s*([A-Ga-g][#b]?\s?-?\d)\s*$")
VEL = re.compile(r"\s*\b(pp|p|mp|mf|f|ff|soft|hard|med|loud)\b\s*$", re.I)
MAX_PRESETS = 1024
CATS = {"piano": "pno", "organ": "org", "bass": "bas", "drum": "drm", "guitar": "gtr", "string": "str",
        "brass": "brs", "vox": "vox", "sax": "wnd", "flute": "wnd", "synth": "syn", "pad": "pad",
        "lead": "led", "fx": "sfx", "hit": "sfx", "bell": "bel", "ep": "kb1", "wav": "wav", "sine": "wav"}


def base_name(fname):
    n = os.path.splitext(fname)[0]
    for _ in range(3):
        n2 = VEL.sub("", NOTE.sub("", n))
        if n2 == n:
            break
        n = n2
    return n.strip() or os.path.splitext(fname)[0]


def category(folder):
    f = folder.lower()
    for k, v in CATS.items():
        if k in f:
            return v
    return "rom"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("dir")
    ap.add_argument("out")
    ap.add_argument("--id", type=int, default=4)
    ap.add_argument("--name", default="P2Ksy")
    ap.add_argument("--budget", type=int, default=(32 << 20) - 0x10000)
    a = ap.parse_args()
    groups = collections.OrderedDict()
    for dp, _, fs in sorted(os.walk(a.dir)):
        for f in sorted(fs):
            if f.lower().endswith(".wav"):
                key = (os.path.basename(dp), base_name(f))
                groups.setdefault(key, []).append(os.path.join(dp, f))
    used, instruments, presets = 0x1000, [], []
    for (folder, name), files in groups.items():
        roots, keep, size = set(), [], 0
        for f in files:
            w = p2k_wav.read_wav(f)
            if w["root"] in roots:
                continue
            roots.add(w["root"])
            keep.append(f)
            size += 2 * (len(w["pcm"]) + 128)
        if not keep or used + size > a.budget:
            continue
        used += size
        stereo = p2k_wav.read_wav(keep[0]).get("channels", 1) == 2
        # instrument names use the same "cat:name" form as presets; the OS builds the Sound Navigator
        # categories from them ("Too Many Inst Categories" otherwise)
        instruments.append({"name": f"{category(folder)}:{name}"[:16], "files": keep, "stereo": stereo})
        if len(presets) < MAX_PRESETS:
            presets.append({"name": f"{category(folder)}:{name}"[:16], "instrument": len(instruments)})
    json.dump({"id": a.id, "name": a.name, "instruments": instruments, "presets": presets},
              open(a.out, "w"), indent=1)
    print(f"{len(instruments)} instruments, {len(presets)} presets, {used / 2**20:.1f} MiB of samples")


if __name__ == "__main__":
    main()
