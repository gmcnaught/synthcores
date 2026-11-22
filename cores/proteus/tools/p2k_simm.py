#!/usr/bin/env python3
"""Build a synthetic Proteus 2000 sound ROM SIMM image (research; not a dump).

Header layout as read by OS 2.26 (docs/p2k-findings.md, "Sound ROM SIMMs"):
  +0x000 u32 magic 0x05551212        +0x004 u32 size in bytes
  +0x00E "EOS Sound SIMM"            +0x03E u32 ROM ID (0 = flash formats "EROM ...")
  +0x04E idle-voice silence (the OS points idle voices at word (0x4E/2)+8 .. +28)
  +0x68E u16 block checksums: 16-bit sum of the words of each block (block 0 includes the header);
         size < 8 MB: 4 blocks of 2 MB, else 8 blocks of size/8.

  p2k_simm.py OUT.bin [--id N] [--size BYTES]
"""
import argparse
import struct

MAGIC = 0x05551212
CKSUM = 0x68E
COMP = 0x6AC          # compensation word inside the 0x6AE-byte header (unused by the OS) [I]


def blocks(size):
    return (4, 2 << 20) if size < 8 << 20 else (8, size // 8)


def word_sum(b):
    return sum(struct.unpack(">%dH" % (len(b) // 2), b)) & 0xFFFF


def finish(img: bytearray):
    """Fill the block checksums. Block 0 contains its own checksum word, so it is balanced with
    the compensation word: with comp = -(sum of block 0 with comp = 0 and checksum = 0), the block
    sums to the stored checksum c for any c; c = 0 is stored."""
    n, bs = blocks(len(img))
    for k in range(1, n):
        struct.pack_into(">H", img, CKSUM + 2 * k, word_sum(img[k * bs:(k + 1) * bs]))
    struct.pack_into(">H", img, COMP, 0)
    struct.pack_into(">H", img, CKSUM, 0)
    struct.pack_into(">H", img, COMP, (-word_sum(img[:bs])) & 0xFFFF)
    assert word_sum(img[:bs]) == 0
    return img


def header(size, rom_id):
    img = bytearray(size)
    struct.pack_into(">II", img, 0, MAGIC, size)
    img[0x0E:0x0E + 15] = b"EOS Sound SIMM\0"
    struct.pack_into(">I", img, 0x3E, rom_id)
    return img


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("out")
    ap.add_argument("--id", type=int, default=1)
    ap.add_argument("--size", type=int, default=32 << 20)
    a = ap.parse_args()
    img = finish(header(a.size, a.id))
    with open(a.out, "wb") as _f: _f.write(img)
    n, bs = blocks(a.size)
    print(a.out, "blocks", [hex(struct.unpack_from(">H", img, CKSUM + 2 * k)[0]) for k in range(n)])


if __name__ == "__main__":
    main()
