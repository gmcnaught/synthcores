#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""Build the files the P2K MiSTer core loads (cores/p2k/docs/plan.md), from your own files.

  p2k_pack.py pack OUT.p2k --font hd44780_a00.bin (--dump V124.bin [--dli p2k226.dli] [--keep-user]
                                                     | --flash FLASH.bin)
      System pack, OSD "Load system pack" (index 1): the 2 MB flash image, then the 4 KB HD44780
      character ROM. With --dump the image is built as MAME's proteus2k machine builds it
      (cores/proteus/tools/p2k_flash_image.py: byte swap, the 0x000DFE repair, the OS 2.26 overlay
      with its start-mode patch) with the user area erased, so the OS writes factory defaults on
      the first start (about 45 s; the save file keeps them); --keep-user keeps the dump's user
      data. --flash takes a finished 2 MB image instead (big-endian words, e.g. a MAME flash NVRAM).

  p2k_pack.py simm OUT.sim (--baby baby.bin --big big.bin [--flash] | --blank)
      SIMM slot image, OSD "SIMM slot N" (index 1 + N): Baby padded to 2 MB with 0xFF, then Big
      (up to 32 MB), e.g. the Composer stand-in from cores/proteus/tools/p2k_build_simm.py.
      --flash pads Big to 32 MB with 0xFF (erased flash): a user flash SIMM file for OSD "Flash
      SIMM slot N", 34 MB, so that whatever the OS writes is saved back (Main_MiSTer does not grow a
      mounted file). The OS writes only a SIMM whose ROM ID (Big header +0x3E) is 106-119.
      --blank: an erased flash SIMM (34 MB of 0xFF), e.g. the target of DUPLICATE SLOT 0 FLASH.

Prints each output's size and SHA-256.
"""
import pathlib
import argparse
import hashlib
import os
import sys

# p2k_flash_image.py: in cores/proteus/tools, or next to this file (the release repository's tools/)
HERE = os.path.dirname(os.path.abspath(__file__))
sys.path[:0] = [HERE, os.path.join(HERE, "..", "..", "proteus", "tools")]
import p2k_flash_image  # noqa: E402


def done(path, data):
    with open(path, "wb") as f:
        f.write(data)
    print(f"{path}: {len(data)} bytes, sha256 {hashlib.sha256(data).hexdigest()}")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("pack")
    p.add_argument("out")
    p.add_argument("--font", required=True)
    p.add_argument("--dump")
    p.add_argument("--dli")
    p.add_argument("--keep-user", action="store_true")
    p.add_argument("--flash")
    s = sub.add_parser("simm")
    s.add_argument("out")
    s.add_argument("--baby")
    s.add_argument("--big")
    s.add_argument("--flash", action="store_true")
    s.add_argument("--blank", action="store_true")
    a = ap.parse_args()

    if a.cmd == "pack":
        if bool(a.dump) == bool(a.flash):
            ap.error("pack: give --dump or --flash")
        if a.flash:
            img = pathlib.Path(a.flash).read_bytes()
        else:
            img = bytes(p2k_flash_image.build(a.dump, a.dli, clean=not a.keep_user))
        if len(img) != 0x200000:
            ap.error("the flash image must be 2 MB")
        font = pathlib.Path(a.font).read_bytes()
        if len(font) != 0x1000:
            ap.error("the character ROM must be 4 KB (hd44780_a00.bin)")
        done(a.out, img + font)
    elif a.blank:
        if a.baby or a.big:
            ap.error("simm: --blank takes no --baby/--big")
        done(a.out, b"\xff" * 0x2200000)
    else:
        if not (a.baby and a.big):
            ap.error("simm: give --baby and --big, or --blank")
        baby = pathlib.Path(a.baby).read_bytes()
        big = pathlib.Path(a.big).read_bytes()
        if len(baby) > 0x200000 or len(big) > 0x2000000:
            ap.error("Baby is at most 2 MB, Big at most 32 MB")
        if a.flash:
            big += b"\xff" * (0x2000000 - len(big))
        done(a.out, baby + b"\xff" * (0x200000 - len(baby)) + big)


if __name__ == "__main__":
    main()
