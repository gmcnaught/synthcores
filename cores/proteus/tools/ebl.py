#!/usr/bin/env python3
"""Inspect E-mu Emulator X / Proteus X sample files (.ebl, FORM/E5B0 container).

Layout observed (Proteus 1SL001.ebl):
  FORM <be32 size> E5B0
  TOC2 <be32 size> ...                       table of contents
  E5S1 <be32 size> <chunk body>              one sample
    body+0x06  name: 32 x UTF-16LE (64 bytes)
    body+0x4a  le32 fields: start, start, end, end, loopstart, loopstart, loopend, loopend, ...
               (byte offsets relative to the chunk body; the duplicated pairs are
                presumably left/right channel)
    PCM: 16-bit signed little-endian, mono, from `start` to `end` (bytes)

Usage: ebl.py FILE_OR_DIR [...]   prints one line per sample
"""
import os
import struct
import sys


def chunks(data, off, end):
    while off + 8 <= end:
        cid = data[off:off + 4]
        size = struct.unpack(">I", data[off + 4:off + 8])[0]
        yield cid, off + 8, size
        off += 8 + size + (size & 1)


def parse(path):
    data = open(path, "rb").read()
    assert data[:4] == b"FORM" and data[8:12] == b"E5B0", path
    out = []
    for cid, body, size in chunks(data, 12, len(data)):
        if cid != b"E5S1":
            continue
        name = data[body + 6:body + 6 + 64].decode("utf-16-le").split("\0")[0]
        f = struct.unpack("<10I", data[body + 0x4a:body + 0x4a + 40])
        rest = data[body + 0x4a + 40:body + 0x4a + 40 + 16]
        out.append(dict(file=os.path.basename(path), name=name, body=body, size=size,
                        start=f[0], end=f[2], loop_start=f[4], loop_end=f[6], fields=f, rest=rest.hex()))
    return out


def main():
    paths = []
    for p in sys.argv[1:]:
        if os.path.isdir(p):
            paths += sorted(os.path.join(r, f) for r, _, fs in os.walk(p) for f in fs if f.lower().endswith(".ebl"))
        else:
            paths.append(p)
    total = 0
    for p in paths:
        for s in parse(p):
            n = (s["end"] - s["start"]) // 2
            total += n
            print(f"{s['file']:28s} {s['name']:20s} words={n:7d} loop=({(s['loop_start']-s['start'])//2},{(s['loop_end']-s['start'])//2}) f9={s['fields'][8]:#x} rest={s['rest']}")
    print(f"# total PCM words {total} ({total * 2} bytes)", file=sys.stderr)


if __name__ == "__main__":
    main()
