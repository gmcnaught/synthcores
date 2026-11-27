#!/usr/bin/env python3
"""Build the initial 2 MB flash image the MAME proteus2k machine boots (for the RTL testbench).

Same steps as emup2k_state::init_proteus2k:
  - the v1.24 dump is stored byte-swapped (28F160 read as x16 little-endian): swap to big-endian;
  - repair 0x000DFE 4E38 -> 4EB9 (jsr, see docs/p2k-findings.md);
  - with an OS 2.26 .dli: copy its image to 0x010100 and patch the start-mode read at
    OS + 0x9B0 (move.l (1020,a3),d7 -> moveq #0,d7; nop);
  - --clean: erase 0x0A0000-0x1FFFFF (user data) as P2K_CLEAN=1 does.

  p2k_flash_image.py DUMP.bin [--dli p2k226.dli] [--clean] OUT.bin [--hex OUT.hex]
"""
import pathlib
import argparse
import struct


def build(dump, dli=None, clean=False):
    raw = pathlib.Path(dump).read_bytes()
    assert len(raw) == 0x200000
    img = bytearray(b for i in range(0, len(raw), 2) for b in (raw[i + 1], raw[i]))
    if img[0xDFE:0xE00] == b"\x4e\x38":
        img[0xDFE:0xE00] = b"\x4e\xb9"
    if clean:
        img[0xA0000:] = b"\xff" * (0x200000 - 0xA0000)
    if dli:
        d = pathlib.Path(dli).read_bytes()
        off, length = struct.unpack_from(">II", d, 0x24)
        img[0x10100:0x10100 + length] = d[off:off + length]
        p = 0x10100 + 0x9B0
        if img[p:p + 4] == bytes.fromhex("2e2b03fc"):
            img[p:p + 4] = bytes.fromhex("7e004e71")
    return img


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("dump")
    ap.add_argument("out")
    ap.add_argument("--dli")
    ap.add_argument("--clean", action="store_true")
    ap.add_argument("--hex", help="also write one 16-bit word per line (hex) for VHDL textio")
    a = ap.parse_args()
    img = build(a.dump, a.dli, a.clean)
    with open(a.out, "wb") as _f: _f.write(img)
    if a.hex:
        with open(a.hex, "w") as f:
            for i in range(0, len(img), 2):
                f.write("%02x%02x\n" % (img[i], img[i + 1]))


if __name__ == "__main__":
    main()
