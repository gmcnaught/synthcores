#!/usr/bin/env python3
"""Walk an E-mu Proteus 2000 "lib_" container (flash user banks, SIMM Baby flash).

Layout [O: flash v1.24 dump 0xA0000, OS 2.26 0x1037DEC]:
  +0  "lib_"  +4 4-char library type  +8 u32 size  +12 u16 (<= 64)  +14 u16
  then objects: 4-char type, u32 length (including this 12-byte header), u16 id, u16 flags, data

  p2k_lib.py FILE OFFSET [--swap]   (--swap: file is the byte-swapped 28F160 dump)
"""
import pathlib
import collections
import struct
import sys


def walk(d, off):
    magic, typ, size, n, x = struct.unpack_from(">4s4sIHH", d, off)
    assert magic == b"lib_", magic
    p, end, objs = off + 16, off + size, []
    while p + 12 <= min(end, len(d)):
        t, ln, oid, fl = struct.unpack_from(">4sIHH", d, p)
        if t in (b"\xff\xff\xff\xff", b"\0\0\0\0") or ln < 12:
            break
        objs.append((p, t.decode("latin-1"), ln, oid, fl))
        p += ln
    return typ.decode("latin-1"), size, n, x, objs, p


if __name__ == "__main__":
    d = pathlib.Path(sys.argv[1]).read_bytes()
    if "--swap" in sys.argv:
        d = bytes(b for i in range(0, len(d), 2) for b in (d[i + 1], d[i]))
    typ, size, n, x, objs, p = walk(d, int(sys.argv[2], 0))
    print(f"lib type {typ} size {size:#x} n {n} x {x:#x}; {len(objs)} objects, stop at {p:#x}")
    c = collections.Counter(o[1] for o in objs)
    print(dict(c))
    seen = collections.Counter()
    for o in objs:
        seen[o[1]] += 1
        if seen[o[1]] <= 2:
            print(f"  {o[0]:#08x} {o[1]} len {o[2]:#x} id {o[3]} flags {o[4]:#06x}  {d[o[0]+12:o[0]+44].hex(' ')}")
