#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Make the MU50 MiSTer ROM pack from MAME's mu50 and mulcd sets.

    tools/mkpack.py ROMPATH OUT.rom [--bios v105|v104|v102]

ROMPATH is a MAME rompath holding mu50/ (program and wave ROMs) and mulcd/ (hd44780u_b04.bin).
Pack (4,722,688 bytes):
  000000-07FFFF  program ROM in CPU byte order (MAME's file is word-swapped)
  080000-47FFFF  wave ROM: xq057c0.ic18 then xq058c0.ic19
  480000-480FFF  LCD character ROM (HD44780 ROM B04)
"""
import argparse
import hashlib
import os
import sys

BIOS = {
    "v105": ("xr174c0.ic7", "9ca892920598f9fdf08544dac4c0e54e7d46ee3c"),
    "v104": ("yamaha_mu50.bin", "58c41f10d292cac35ef0e8f93029fbc4685df586"),
    "v102": ("xq332e0.bin", "74bb630f8cc575e059cc52cd43b3310e09f26a49"),
}
WAVE = (("xq057c0.ic18", "32f653c7644d060f5a6d63a435ae3a7412386d92"),
        ("xq058c0.ic19", "adf68689b4842ec5bc9b0ea1bb99cf66d2dec4de"))
CGROM = ("hd44780u_b04.bin", "2ff0899bfee7795ba52a3d56c96edf31d9e6a3f9")


def read(path, size, sha1):
    with open(path, "rb") as f:
        data = f.read()
    if len(data) != size:
        sys.exit(f"{path}: {len(data)} bytes, expected {size}")
    if hashlib.sha1(data).hexdigest() != sha1:
        print(f"warning: {path} is not MAME's dump (SHA-1 {hashlib.sha1(data).hexdigest()})",
              file=sys.stderr)
    return data


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("rompath")
    ap.add_argument("out")
    ap.add_argument("--bios", choices=sorted(BIOS), default="v105")
    a = ap.parse_args()

    name, sha1 = BIOS[a.bios]
    prog = bytearray(read(os.path.join(a.rompath, "mu50", name), 0x80000, sha1))
    prog[0::2], prog[1::2] = prog[1::2], prog[0::2]     # ROM_LOAD16_WORD_SWAP
    wave = b"".join(read(os.path.join(a.rompath, "mu50", n), 0x200000, s) for n, s in WAVE)
    cg = read(os.path.join(a.rompath, "mulcd", CGROM[0]), 0x1000, CGROM[1])
    pack = bytes(prog) + wave + cg
    with open(a.out, "wb") as f:
        f.write(pack)
    print(f"{a.out}: {len(pack)} bytes, sha1 {hashlib.sha1(pack).hexdigest()}")


if __name__ == "__main__":
    main()
