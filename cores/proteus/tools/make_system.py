#!/usr/bin/env python3
"""Build the system image for the Proteus core (OSD "Load System", ioctl index 1).

Layout (byte offsets):
  0x000000  sample words, big-endian (8 MB: bank A = Proteus/1 base, bank B = ProtoLogic)
  0x800000  program ROM, big-endian words (128 KB): even byte = MSB EPROM, odd = LSB EPROM
  0x820000  RAM 0xFF0000-0xFFFFFF, big-endian words (64 KB): the XR's battery-backed state
  0x830000  HD44780 character generator ROM (4 KB, MAME hd44780_a00.bin layout: 16 bytes/char)

The RAM image comes from a MAME nvram file for proteusxrpl. MAME stores the 16-bit share in
host order (little-endian words on x86/ARM), so each word is byte-swapped here.

Usage: make_system.py BASE_WAVES PROTOLOGIC_DIR FW_MSB FW_LSB MAME_NVRAM FONT OUT.bin
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from make_demo import protologic_image  # noqa: E402


def main():
    if len(sys.argv) != 8:
        raise SystemExit(__doc__)
    base, pl_dir, msb, lsb, nvram, font, out = sys.argv[1:]
    samples = open(base, "rb").read()
    assert len(samples) == 0x400000
    samples += protologic_image(pl_dir)

    hi, lo = open(msb, "rb").read(), open(lsb, "rb").read()
    assert len(hi) == len(lo) == 0x10000
    rom = bytearray(0x20000)
    rom[0::2], rom[1::2] = hi, lo

    nv = open(nvram, "rb").read()
    assert len(nv) == 0x10000, len(nv)
    ram = bytearray(0x10000)
    ram[0::2], ram[1::2] = nv[1::2], nv[0::2]

    fnt = open(font, "rb").read()
    assert len(fnt) == 0x1000

    img = bytearray(samples)        # 0x000000
    img += rom                      # 0x800000
    img += ram                      # 0x820000
    img += fnt                      # 0x830000
    open(out, "wb").write(img)
    ssp = int.from_bytes(rom[0:4], "big")
    pc = int.from_bytes(rom[4:8], "big")
    print(f"image {len(img)} bytes; reset SSP={ssp:08x} PC={pc:08x}; "
          f"RAM word 0xFFE1A2 = {int.from_bytes(ram[0xE1A2:0xE1A4], 'big'):04x} (program map entry 1)")


if __name__ == "__main__":
    main()
