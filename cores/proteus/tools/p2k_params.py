#!/usr/bin/env python3
"""Proteus 2000 preset parameter table, read from the OS 2.26 image (not shipped).

OS 2.26 holds 12-byte entries {u16 SysEx parameter ID, u32 0, u16 offset in the 980-byte preset
record, s8 min, s8 max, u16 flags} at 0x010491B4-0x01049EE0 [O]. Layer parameters (0x581-0x7FF)
address layer 0 (record +0xA4); layer n adds n * 0xCC.

  p2k_params.py [OS.bin]      print the table
"""
import struct
import sys

OS_BASE = 0x01000000
TABLE = (0x010491B4, 0x01049EE0)
LAYER0, LAYER_SIZE = 0xA4, 0xCC


def table(os_image="build/p2k/p2k226.bin"):
    d = open(os_image, "rb").read()
    t = {}
    for o in range(TABLE[0] - OS_BASE, TABLE[1] - OS_BASE, 12):
        pid, off = struct.unpack_from(">H4xH", d, o)
        mn, mx = struct.unpack_from("bb", d, o + 8)
        t[pid] = (off, mn, mx)
    return t


def apply(record: bytearray, params, layer=0, os_image="build/p2k/p2k226.bin"):
    """Set {SysEx ID: value} in a 980-byte preset record (layer parameters on `layer`)."""
    t = table(os_image)
    for pid, val in params.items():
        off = t[int(pid)][0]
        if 0x581 <= int(pid) < 0x800:
            off += layer * LAYER_SIZE
        record[off] = val & 0xFF
    return record


if __name__ == "__main__":
    for pid, (off, mn, mx) in sorted(table(*sys.argv[1:]).items()):
        print(f"{pid:5d} 0x{pid:03X} off 0x{off:03X} {mn:4d}..{mx:4d}")
