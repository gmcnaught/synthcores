#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Write a MAME rompath for the mu50 set that runs a generated program.

Usage: mu50_testrom.py PROGRAM.bin ROMPATH [--bios v105]

PROGRAM.bin is the H8 program image as the CPU sees it (big-endian, address 0 first, at most
512 KB). It goes into the file of the chosen BIOS, laid out as MAME's ROM_LOAD16_WORD_SWAP
expects; the two wave ROMs are written as zeros, and the mulcd device's files (LCD artwork and
character ROM) as placeholders of the right length. MAME warns about the wrong hashes and runs.
"""
import argparse
import pathlib

BIOS_FILES = {"v105": "xr174c0.ic7", "v104": "yamaha_mu50.bin", "v102": "xq332e0.bin"}
PROG_SIZE = 0x80000
WAVE_FILES = ("xq057c0.ic18", "xq058c0.ic19")
WAVE_SIZE = 0x200000
MULCD_SVG_SIZE = 525261
MULCD_CGROM = ("hd44780u_b04.bin", 0x1000)


def rom_file_bytes(program: bytes) -> bytes:
    """The file holds each 16-bit word little-endian (ROM_LOAD16_WORD_SWAP swaps it into the
    big-endian CPU's order), so every byte pair of the CPU image is swapped. Checked with a
    reset vector of 00000100: unswapped, MAME read 00000001."""
    if len(program) > PROG_SIZE:
        raise SystemExit(f"program is {len(program)} bytes, more than {PROG_SIZE}")
    img = bytearray(program + bytes(PROG_SIZE - len(program)))
    img[0::2], img[1::2] = img[1::2], img[0::2]
    return bytes(img)


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("program", type=pathlib.Path)
    ap.add_argument("rompath", type=pathlib.Path)
    ap.add_argument("--bios", default="v105", choices=sorted(BIOS_FILES))
    ap.add_argument("--wave", type=pathlib.Path, help="a 4 MB wave ROM image (default zeros)")
    a = ap.parse_args()
    d = a.rompath / "mu50"
    d.mkdir(parents=True, exist_ok=True)
    (d / BIOS_FILES[a.bios]).write_bytes(rom_file_bytes(a.program.read_bytes()))
    wave = a.wave.read_bytes() if a.wave else bytes(2 * WAVE_SIZE)
    if len(wave) != 2 * WAVE_SIZE:
        raise SystemExit(f"{a.wave}: {len(wave)} bytes, want {2 * WAVE_SIZE}")
    for i, w in enumerate(WAVE_FILES):
        (d / w).write_bytes(wave[i * WAVE_SIZE:(i + 1) * WAVE_SIZE])
    lcd = a.rompath / "mulcd"
    lcd.mkdir(exist_ok=True)
    svg = b'<svg xmlns="http://www.w3.org/2000/svg" width="100" height="40"></svg>\n'
    (lcd / "mulcd.svg").write_bytes(svg + b" " * (MULCD_SVG_SIZE - len(svg)))
    (lcd / MULCD_CGROM[0]).write_bytes(bytes(MULCD_CGROM[1]))


if __name__ == "__main__":
    main()
