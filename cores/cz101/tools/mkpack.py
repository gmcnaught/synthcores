#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Make the CZ-101 MiSTer ROM pack from MAME's cz101 set and the HD44780 character ROM.

    tools/mkpack.py CZ101_DIR HD44780_A00 OUT.rom [--version 1|2]

CZ101_DIR holds MAME's cz101 files: hn613256ps40.bin (program, Version II) or
hn613256pn26.bin (Version I) and init_ram.bin. HD44780_A00 is MAME's hd44780_a00.bin.
Pack (40 KB): 0000-7FFF program ROM, 8000-8FFF initial battery RAM, 9000-9FFF character ROM.
"""
import argparse
import hashlib
import os
import sys

PROGRAM = {
    2: ("hn613256ps40.bin", "2aa5bfb76dc0a56797cf5dd547197816cedfa370"),
    1: ("hn613256pn26.bin", "52ff2c280392e104e84b05288c2b952fc29b50f4"),
}


def read(path, size):
    with open(path, "rb") as f:
        data = f.read()
    if len(data) != size:
        sys.exit(f"{path}: {len(data)} bytes, expected {size}")
    return data


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("cz101_dir")
    ap.add_argument("hd44780_a00")
    ap.add_argument("out")
    ap.add_argument("--version", type=int, choices=(1, 2), default=2)
    a = ap.parse_args()

    name, sha1 = PROGRAM[a.version]
    prog = read(os.path.join(a.cz101_dir, name), 0x8000)
    if hashlib.sha1(prog).hexdigest() != sha1:
        print(f"warning: {name} is not MAME's dump (SHA-1 {hashlib.sha1(prog).hexdigest()})",
              file=sys.stderr)
    ram = read(os.path.join(a.cz101_dir, "init_ram.bin"), 0x1000)
    font = read(a.hd44780_a00, 0x1000)
    pack = prog + ram + font
    with open(a.out, "wb") as f:
        f.write(pack)
    print(f"{a.out}: {len(pack)} bytes, sha1 {hashlib.sha1(pack).hexdigest()}")


if __name__ == "__main__":
    main()
