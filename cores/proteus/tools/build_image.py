#!/usr/bin/env python3
"""Build the Proteus core's system image from files you have downloaded yourself.

Inputs (zip files, used as downloaded):
  --protologic  "Proteus 1 XR w Protologic - v2.30.zip": firmware EPROMs IP485/IP486 (v2.30) and
                the ProtoLogic sound ROMs IP505-IP508 / IP705-IP708
  --samples     "Proteus 1.zip": E-mu's Emulator X conversion of the Proteus/1 sound set
                (SamplePool/*.ebl), used to rebuild the 4 MB Proteus/1 sound ROM
  --font        MAME's "hd44780.zip" (hd44780_a00.bin, the HD44780 A00 character set)
Output: proteus_system.bin (copy to games/Proteus/ on the MiSTer, load with "Load System").

Image layout: 0x000000 samples (8 MB: rebuilt Proteus/1 bank, ProtoLogic bank), 0x800000 program
ROM (128 KB), 0x820000 RAM (64 KB, blank), 0x830000 font (4 KB), 0x831000 flags (bit0: initialise
presets on first boot). On first boot without a save file the core runs the firmware's own
diagnostics "Initialize" to load the factory presets, then saves the RAM to the save file.

Every input file is checked by SHA-1 against the known dumps before use.
Usage: build_image.py --protologic ZIP --samples ZIP --font ZIP [-o proteus_system.bin]
"""
import argparse
import hashlib
import os
import sys
import tempfile
import zipfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import rebuild_proteus1_rom as rb  # noqa: E402

# SHA-1 of the known dumps (dbwbp.com "Proteus 1 XR w Protologic - v2.30", MAME hd44780)
KNOWN = {
    "IP486": "7e8f6474545bf24e6af318db4cd573ad05c66cbb",   # v2.30 MSB (even bytes)
    "IP485": "bcd668ac2c5ffbf43dc1c50a3bbd85a87b537845",   # v2.30 LSB (odd bytes)
    "IP505": "e7c587010713bd911b9805c8616e1e920c4307e1",
    "IP506": "4eb67d722c064d66ab1604ffb9ffe02b039dfcd9",
    "IP507": "316df7f76b48f17b49b4397268a9c7021c84ed12",
    "IP508": "54571ee9b4c1992172a94557cf8e9c4b74fef0d4",
    "IP705": "35c2ee0fafa9bf99643a34a610a8ab0b32a0f42d",
    "IP706": "f02a46f2bbfa928356a0c4521b021f9fa38049b1",
    "IP707": "1d10905cad823d340cfa5ceec1b3ade323f2bc31",
    "IP708": "18fc57153bf69e854efa0c48f4fcec0b62c73654",
    "hd44780_a00.bin": "65cf075a988cdcbb316b9afdd0529b374a1a65ec",
}


def zip_member(zf, key):
    """Return the bytes of the member whose base name starts with key (case-insensitive)."""
    for info in zf.infolist():
        base = os.path.basename(info.filename)
        if base.upper().startswith(key.upper()) and not info.is_dir() and not base.startswith("._"):
            if key.upper().startswith("IP") and not base.lower().endswith(".bin"):
                continue
            return zf.read(info)
    raise SystemExit(f"{key} not found in {zf.filename}")


def checked(name, data):
    digest = hashlib.sha1(data).hexdigest()
    if digest != KNOWN[name]:
        raise SystemExit(f"{name}: SHA-1 {digest} does not match the known dump {KNOWN[name]}")
    return data


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--protologic", required=True)
    ap.add_argument("--samples", required=True)
    ap.add_argument("--font", required=True)
    ap.add_argument("-o", "--output", default="proteus_system.bin")
    a = ap.parse_args()

    with zipfile.ZipFile(a.protologic) as zf:
        roms = {k: checked(k, zip_member(zf, k)) for k in KNOWN if k.startswith("IP")}
    with zipfile.ZipFile(a.font) as zf:
        font = checked("hd44780_a00.bin", zip_member(zf, "hd44780_a00"))

    # firmware: even bytes from IP486, odd bytes from IP485
    fw = bytearray(0x20000)
    fw[0::2], fw[1::2] = roms["IP486"], roms["IP485"]

    # ProtoLogic bank: word n = {IP70x[n>>2], IP50x[n>>2]}, x = 5 + (n & 3)
    lo = [roms[f"IP50{c}"] for c in (5, 6, 7, 8)]
    hi = [roms[f"IP70{c}"] for c in (5, 6, 7, 8)]
    bank_b = bytearray(0x400000)
    for n in range(0x200000):
        bank_b[2 * n] = hi[n & 3][n >> 2]
        bank_b[2 * n + 1] = lo[n & 3][n >> 2]
    sine = [bank_b[2 * n] << 8 | bank_b[2 * n + 1] for n in range(0x150, 0x1C0)]

    # Proteus/1 bank: rebuilt from the Emulator X samples, placed by the firmware's sample table
    name, base, count = rb.detect_table(bytes(fw))
    records = rb.load_records(bytes(fw), base, count)
    with tempfile.TemporaryDirectory() as tmp:
        with zipfile.ZipFile(a.samples) as zf:
            members = [i for i in zf.infolist() if i.filename.lower().endswith(".ebl")
                       and not os.path.basename(i.filename).startswith("._")]
            if not members:
                raise SystemExit(f"no .ebl samples in {a.samples}")
            for i in members:
                with open(os.path.join(tmp, os.path.basename(i.filename)), "wb") as out:
                    out.write(zf.read(i))
        samples = rb.load_ebl(tmp)
    rom, placed, unmatched, nwritten = rb.build(records, samples, sine)
    bank_a = rb.to_bytes(rom)

    img = bytearray(bank_a + bytes(bank_b))   # 0x000000
    img += fw                                  # 0x800000
    img += b"\xff" * 0x10000                   # 0x820000 RAM: blank, initialised on first boot
    img += font                                # 0x830000
    img += bytes([0x01]) + bytes(0xFFF)        # 0x831000 flags
    with open(a.output, "wb") as f:
        f.write(img)
    print(f"{a.output}: {len(img)} bytes; firmware table {name}; Proteus/1 bank: "
          f"{len(placed)} of {len(records) - 1} samples placed, {len(unmatched)} silent, "
          f"{nwritten * 100 // (rb.WORDS - 0x1C0)} % of sample words")


if __name__ == "__main__":
    main()
