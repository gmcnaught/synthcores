#!/usr/bin/env python3
"""E-mu .dli (E-Loader OS update) files: show the header, check the CRC, extract the image.

Layout (observed on Proteus 1000/2000 OS 2.26):
  0x00  "Copyright E-mu Systems, Inc." (NUL padded)
  0x1C  u32 0, u32 1, u32 image offset (0x1A0), u32 image length, u32 CRC-32 of the image
  0x30  name fields of 16/32 bytes: "none", "<Model>Loader", "EXE ", "Version x.yy",
        "GenericOS", "dli.os.version=x.yy"
  0x1A0 the image: a ColdFire (MCF5206e) program linked at 0x01000000 (DRAM);
        long 0 = initial SSP, long 1 = reset PC.

  dli.py FILE.dli [OUT.bin]     print the header; with OUT, write the image
"""
import struct
import sys
import zlib


def parse(data: bytes) -> dict:
    off, length, crc = struct.unpack(">III", data[0x24:0x30])
    image = data[off:off + length]
    names = [data[o:o + 32].split(b"\0")[0].decode("latin-1") for o in (0x30, 0x40, 0x60, 0x70, 0x80, 0xA0)]
    ssp, pc = struct.unpack(">II", image[:8])
    return dict(copyright=data[:0x1C].split(b"\0")[0].decode("latin-1"), offset=off, length=length,
                crc=crc, crc_ok=zlib.crc32(image) == crc and off + length == len(data),
                names=names, ssp=ssp, pc=pc, image=image)


def main() -> int:
    if len(sys.argv) < 2:
        print(__doc__)
        return 2
    d = parse(open(sys.argv[1], "rb").read())
    print(f"{d['copyright']}\n  names {d['names']}\n  image at {d['offset']:#x}, {d['length']:#x} bytes, "
          f"CRC-32 {d['crc']:08x} {'OK' if d['crc_ok'] else 'BAD'}\n  SSP {d['ssp']:#010x}  reset PC {d['pc']:#010x}")
    if len(sys.argv) > 2:
        with open(sys.argv[2], "wb") as _f: _f.write(d["image"])
    return 0 if d["crc_ok"] else 1


if __name__ == "__main__":
    sys.exit(main())
