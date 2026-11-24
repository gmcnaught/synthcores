#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-or-later
"""Make the MT-32 core's ROM pack.

    mkpack.py CONTROL_ROM REVERB_ROM PCM_ROM OUT
    mkpack.py --dir DIR OUT          (DIR holds mt32_cpu.bin, mt32_reverb.bin, mt32_pcm.bin)

Pack: 00000-1FFFF control ROM (second generation, firmware 2.0x, 128 KB), 20000-27FFF reverb
ROM (R15179857, 32 KB), 28000-A7FFF PCM ROM (R15449121, 512 KB). Warns when a file's SHA-1 is
not one MAME lists.
"""
import argparse
import hashlib
import sys
from pathlib import Path

KNOWN = {
    "control": {"2c16432b6c73dd2a3947cba950a0f4c19d6180eb": "2.04",
                "47b52adefedaec475c925e54340e37673c11707c": "2.07"},
    "reverb": {"c2933cb7ad86e51904aa1c3bc12fa234e73a337f": "R15179857"},
    "pcm": {"f6b1eebc4b2d200ec6d3d21d51325d5b48c60252": "R15449121"},
}
SIZES = {"control": 0x20000, "reverb": 0x8000, "pcm": 0x80000}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("files", nargs="+")
    ap.add_argument("--dir")
    a = ap.parse_args()
    if a.dir:
        d = Path(a.dir)
        srcs, out = [d / "mt32_cpu.bin", d / "mt32_reverb.bin", d / "mt32_pcm.bin"], a.files[0]
    else:
        if len(a.files) != 4:
            sys.exit("mkpack.py CONTROL_ROM REVERB_ROM PCM_ROM OUT")
        srcs, out = [Path(f) for f in a.files[:3]], a.files[3]
    pack = b""
    for kind, p in zip(("control", "reverb", "pcm"), srcs):
        data = p.read_bytes()
        if len(data) != SIZES[kind]:
            sys.exit(f"mkpack.py: {p}: {len(data)} bytes, the {kind} ROM is {SIZES[kind]}")
        h = hashlib.sha1(data).hexdigest()
        print(f"{kind:8} {p.name}: {KNOWN[kind].get(h, 'UNKNOWN (not in MAME)')} {h[:12]}")
        pack += data
    Path(out).write_bytes(pack)
    print(f"{out}: {len(pack)} bytes")


if __name__ == "__main__":
    main()
