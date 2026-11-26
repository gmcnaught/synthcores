#!/usr/bin/env python3
"""Identify SC-55 ROM files by SHA-256 against the reference model's romset table.

The table is read from jcmoyer's doc/supported_romsets.rst (the submodule), so
it is the same table the reference renderer uses.

  romset.py check DIR      list every file in DIR that belongs to a known romset
                           and which romsets are complete
  romset.py pack DIR OUT [--romset mk1-v1.21] [--nvram FILE]
                           write the core's ROM pack (modules/roland/hdl/board/rom_loader.sv):
                           copy it to the MiSTer as games/SC-55/boot.rom (games/JV-880/boot.rom
                           for a jv880 romset; --nvram adds a 32 KB JV-880 NVRAM image, e.g. the
                           factory preset from ref/jv880_nvram.cpp); --v3 keeps the wave ROMs
                           as dumped (pack v3, the layout the MRAs assemble)
  romset.py mra OUTDIR     write the MiSTer MRAs (one per model in MAME_SETS): each assembles a
                           pack v3 from MAME's zip in games/mame, picking each chip by CRC32
"""
import argparse
import hashlib
import zlib
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
TABLE = ROOT / "third_party" / "nuked-sc55" / "doc" / "supported_romsets.rst"

ROW = re.compile(r"^\|\s*(?:``(?P<name>[^`]+)``)?\s*\|\s*(?P<part>\S+)\s*\|\s*``(?P<sha>[0-9a-f]{64})``\s*\|\s*(?P<loc>\S+)\s*\|")


def load_table(path: pathlib.Path = TABLE) -> dict[str, dict[str, str]]:
    """Return {romset: {location: sha256}}."""
    table: dict[str, dict[str, str]] = {}
    current = None
    for line in path.read_text().splitlines():
        m = ROW.match(line)
        if not m:
            continue
        if m.group("name"):
            current = m.group("name")
            table.setdefault(current, {})
        if current is None:
            continue
        table[current][m.group("loc")] = m.group("sha")
    return table


def sha256(path: pathlib.Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def identify(directory: pathlib.Path, table=None):
    """Return ({sha: path} for known files, [complete romset names])."""
    table = table or load_table()
    known = {sha for rs in table.values() for sha in rs.values()}
    found = {}
    for p in sorted(directory.rglob("*")):
        if p.is_file() and p.stat().st_size <= 16 << 20:
            h = sha256(p)
            if h in known:
                found[h] = p
    complete = [name for name, locs in table.items() if all(h in found for h in locs.values())]
    return found, complete


def find_romset(directory: pathlib.Path, romset: str) -> dict[str, pathlib.Path]:
    """Return {location: path} for one complete romset, or raise LookupError."""
    table = load_table()
    if romset not in table:
        raise LookupError(f"unknown romset {romset}")
    found, _ = identify(directory, table)
    missing = [loc for loc, h in table[romset].items() if h not in found]
    if missing:
        raise LookupError(f"{romset}: missing {', '.join(missing)} in {directory}")
    return {loc: found[h] for loc, h in table[romset].items()}


# Roland wave ROM dumps are address- and data-scrambled; the reference model
# unscrambles them on load (rom_io.cpp unscramble). The pack holds them
# unscrambled, as the model has them in memory.
_ADDR_BITS = [2, 0, 3, 4, 1, 9, 13, 10, 18, 17, 6, 15, 11, 16, 8, 5, 12, 7, 14, 19]
_DATA_BITS = [2, 0, 4, 5, 7, 6, 3, 1]


def unscramble(src: bytes) -> bytes:
    def table(bits):
        return [sum(1 << _ADDR_BITS[b + bits] for b in range(10) if v & (1 << b)) for v in range(1024)]
    lo, hi = table(0), table(10)
    data = bytes(sum(1 << j for j in range(8) if v & (1 << _DATA_BITS[j])) for v in range(256))
    out = bytearray(len(src))
    for i in range(len(src)):
        a = (i & ~0xfffff) | lo[i & 0x3ff] | hi[(i >> 10) & 0x3ff]
        out[i] = data[src[a]]
    return bytes(out)


# Pack v2 (hdl/board/rom_loader.sv), every supported model:
#   0x000000  header: "SC55", 2, model (0 mk1, 1 SC-155, 2 mk2, 3 SC-155mk2, 4 JV-880),
#             flags (bit 0: the JV-880 NVRAM section holds an image), 249 bytes reserved
#   0x000100  ROM1 (32 KB)
#   0x008100  ROM2 (512 KB; a 256 KB ROM2 is followed by zeros)
#   0x088100  sub-MCU ROM (4 KB; zeros on the mk1)
#   0x089100  wave ROMs as the PCM chip addresses them (4 MB): mk1 waverom1..3
#             at 0, 1 MB, 2 MB; mk2 waverom1 (2 MB) at 0, waverom2 at 2 MB; JV-880
#             waverom1 and waverom2 (2 MB each) at 0 and 2 MB
#   0x489100  JV-880 only: NVRAM (32 KB; zeros without --nvram)
# Pack v3: the v2 layout with the wave ROMs as dumped (scrambled); rom_loader.sv unscrambles them
# as they go to DDR3. The MRAs assemble this from MAME's zips, which hold the dumps.
# (Pack v1 was the mk1 layout ROM1 0x100, ROM2 0x8100, waves 0x48100; the
# loader still reads it.)
MODELS = {"mk1": 0, "sc155": 1, "mk2": 2, "sc155mk2": 3, "jv880": 4}
ROM1_AT, ROM2_AT, SMROM_AT, WAVE_AT = 0x000100, 0x008100, 0x088100, 0x089100
NVRAM_AT = WAVE_AT + 0x400000
PACK_SIZE = WAVE_AT + 0x400000
JV880_PACK_SIZE = NVRAM_AT + 0x8000


def model_of(romset: str) -> int:
    for prefix in ("sc155mk2", "sc155", "mk2", "mk1", "jv880"):
        if romset.startswith(prefix + "-"):
            return MODELS[prefix]
    raise LookupError(f"{romset}: the cores support mk1, SC-155, mk2, SC-155mk2 and JV-880 romsets")


def pack(directory: pathlib.Path, romset: str, nvram: bytes | None = None, v3: bool = False) -> bytes:
    model = model_of(romset)
    jv = model == MODELS["jv880"]
    mk2 = model in (MODELS["mk2"], MODELS["sc155mk2"])
    files = find_romset(directory, romset)
    out = bytearray(JV880_PACK_SIZE if jv else PACK_SIZE)
    out[0:7] = b"SC55" + bytes([3 if v3 else 2, model, 1 if (jv and nvram is not None) else 0])
    if nvram is not None:
        if not jv or len(nvram) != 0x8000:
            raise LookupError("--nvram: a 32 KB image, for a jv880 romset only")
        out[NVRAM_AT:NVRAM_AT + 0x8000] = nvram

    def put(loc, off, sizes):
        data = files[loc].read_bytes()
        if len(data) not in sizes:
            raise LookupError(f"{loc}: {len(data)} bytes, expected {sizes}")
        if loc.startswith("WAVEROM") and not v3:
            data = unscramble(data)
        out[off:off + len(data)] = data

    put("ROM1", ROM1_AT, (0x8000,))
    put("ROM2", ROM2_AT, (0x80000,) if mk2 else (0x40000,))
    if jv:
        put("WAVEROM1", WAVE_AT, (0x200000,))
        put("WAVEROM2", WAVE_AT + 0x200000, (0x200000,))
    elif mk2:
        put("SMROM", SMROM_AT, (0x1000,))
        put("WAVEROM1", WAVE_AT, (0x200000,))
        put("WAVEROM2", WAVE_AT + 0x200000, (0x100000,))
    else:
        for k in range(3):
            put(f"WAVEROM{k + 1}", WAVE_AT + k * 0x100000, (0x100000,))
    return bytes(out)


# MAME's sets of the SC-55 models (src/mame/roland/roland_sc55.cpp, roland_sc55mk2.cpp): the zip
# (a clone's zip first, then its parent's, for split sets), and each chip's name and CRC32 there.
# The CRC32s are those of Nuked-SC55's romsets (tests/test_romset.py checks them against local
# dumps found by SHA-256). The MRA picks each chip by CRC32, so the names inside the zip do not
# matter. MAME has no other SC-55 firmware versions; those need tools/romset.py pack.
_MK1_WAVES = {
    "WAVEROM1": ("roland-gss.a_r15209276.ic28", "1ac774d3"),
    "WAVEROM2": ("roland-gss.b_r15209277.ic27", "8dcc592a"),
    "WAVEROM3": ("roland-gss.c_r15209281.ic26", "e21ebc04"),
}
_MK2 = {
    "ROM1": ("r15199858_main_mcu.bin", "9b66631f"),
    "ROM2": ("r00233567_control.bin", "fcee1e8e"),
    "SMROM": ("r15199880_secondary_mcu.bin", "702c0a82"),
    "WAVEROM1": ("r15209359_pcm_1.bin", "1519d3b3"),
    "WAVEROM2": ("r15279813_pcm_2.bin", "0f826c7f"),
}
# romset: (MRA name, MAME setname, zips, {location: (name, crc32)})
MAME_SETS = {
    "mk1-v1.21": ("SC-55", "sc55", "sc55.zip", {
        "ROM1": ("roland_r15199778_6435328a97f.ic30", "4ed0d171"),
        "ROM2": ("roland_r15209363.ic23", "2dc58549"), **_MK1_WAVES}),
    "sc155-rev1": ("SC-155", "sc155", "sc155.zip|sc55.zip", {
        "ROM1": ("roland_r15199799.ic30", "a160da90"),
        "ROM2": ("roland_r15209361.ic15", "e19d4a52"), **_MK1_WAVES}),
    "mk2-v1.01": ("SC-55mk2", "sc55mk2", "sc55mk2.zip", _MK2),
    "sc155mk2-v1.01": ("SC-155mk2", "sc55mk2", "sc55mk2.zip", _MK2),
}
# MD5 of each MRA's assembled pack (checked by romset.py mra --roms and tests/test_romset.py):
# Main_MiSTer reports a mismatch when the zip holds other files.
MRA_MD5 = {
    "mk1-v1.21": "e38ef33b7d925ea49b8c0f3f8e4700a5",
    "sc155-rev1": "2bb38e9147a9493bd35bb6c5797b9073",
    "mk2-v1.01": "207ad7a76b6b043d4b25747cf7cadd59",
    "sc155mk2-v1.01": "a96281f6357c96c64d8b1fc2ae65b2c0",
}


def mra_parts(romset: str):
    """The MRA's parts in order: ("data", bytes) or ("file", location, size)."""
    model = model_of(romset)
    mk2 = model in (MODELS["mk2"], MODELS["sc155mk2"])
    head = b"SC55" + bytes([3, model, 0]) + bytes(ROM1_AT - 7)
    if mk2:
        return [("data", head), ("file", "ROM1", 0x8000), ("file", "ROM2", 0x80000), ("file", "SMROM", 0x1000),
                ("file", "WAVEROM1", 0x200000), ("file", "WAVEROM2", 0x100000), ("data", bytes(0x100000))]
    return [("data", head), ("file", "ROM1", 0x8000), ("file", "ROM2", 0x40000), ("data", bytes(0x40000 + 0x1000)),
            ("file", "WAVEROM1", 0x100000), ("file", "WAVEROM2", 0x100000), ("file", "WAVEROM3", 0x100000),
            ("data", bytes(0x100000))]


def mra(romset: str, md5: str) -> str:
    """The MRA text for one romset. md5: of the assembled pack (romset.py pack --v3)."""
    name, setname, zips, chips = MAME_SETS[romset]
    lines = [
        "<misterromdescription>",
        f"    <name>{name}</name>",
        f"    <setname>{setname}</setname>",
        "    <rbf>SC55</rbf>",
        f"    <!-- Generated by cores/sc55/tools/romset.py mra (gmcnaught/synthcores). Assembles the",
        f"         core's ROM pack v3 for romset {romset} from MAME's {setname} set: put the zip in",
        f"         games/mame/. Each chip is picked by CRC32, whatever its name in the zip. -->",
        f'    <rom index="0" zip="{zips}" md5="{md5}">',
    ]
    for part in mra_parts(romset):
        if part[0] == "file":
            loc = part[1]
            lines.append(f'        <part name="{chips[loc][0]}" crc="{chips[loc][1]}"/> <!-- {loc} -->')
        elif any(part[1]):
            lines.append(f"        <part>{part[1][:7].hex(' ')}</part> <!-- header: SC55, pack v3, model, flags -->")
            lines.append(f'        <part repeat="{len(part[1]) - 7}">00</part>')
        else:
            assert len(part[1]) % 0x100 == 0
            lines.append(f'        <part repeat="{len(part[1]) // 0x100}">{bytes(0x100).hex(" ")}</part>'
                         f" <!-- {len(part[1]):#x} zero bytes -->")
    lines += ["    </rom>", "</misterromdescription>", ""]
    return "\n".join(lines)


def assemble_mra(romset: str, directory: pathlib.Path) -> bytes:
    """What the MRA sends, from the dumps in DIR (found by CRC32, as Main_MiSTer finds them)."""
    by_crc = {}
    for p in sorted(directory.rglob("*")):
        if p.is_file() and p.stat().st_size <= 16 << 20:
            by_crc.setdefault("%08x" % zlib.crc32(p.read_bytes()), p)
    chips = MAME_SETS[romset][3]
    out = bytearray()
    for part in mra_parts(romset):
        if part[0] == "data":
            out += part[1]
        else:
            data = by_crc[chips[part[1]][1]].read_bytes()
            assert len(data) == part[2], (part, len(data))
            out += data
    return bytes(out)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    c = sub.add_parser("check")
    c.add_argument("directory", type=pathlib.Path)
    pk = sub.add_parser("pack")
    pk.add_argument("directory", type=pathlib.Path)
    pk.add_argument("out", type=pathlib.Path)
    pk.add_argument("--romset", default="mk1-v1.21")
    pk.add_argument("--nvram", type=pathlib.Path, help="JV-880: a 32 KB NVRAM image for the pack")
    pk.add_argument("--v3", action="store_true", help="pack v3: the wave ROMs as dumped")
    mr = sub.add_parser("mra")
    mr.add_argument("outdir", type=pathlib.Path)
    mr.add_argument("--roms", type=pathlib.Path, help="dumps to check the MRAs against (by assembling them)")
    args = ap.parse_args()

    if args.cmd == "pack":
        data = pack(args.directory, args.romset, args.nvram.read_bytes() if args.nvram else None, args.v3)
        args.out.write_bytes(data)
        print(f"wrote {args.out} ({len(data)} bytes, {args.romset})")
        return 0

    if args.cmd == "mra":
        args.outdir.mkdir(parents=True, exist_ok=True)
        for rs in MAME_SETS:
            md5 = MRA_MD5[rs]
            if args.roms:
                built = assemble_mra(rs, args.roms)
                assert built == pack(args.roms, rs, v3=True), rs
                assert hashlib.md5(built).hexdigest() == md5, (rs, hashlib.md5(built).hexdigest())
            out = args.outdir / f"{MAME_SETS[rs][0]}.mra"
            out.write_text(mra(rs, md5))
            print(f"wrote {out}")
        return 0

    table = load_table()
    found, complete = identify(args.directory, table)
    for name, locs in table.items():
        for loc, h in locs.items():
            if h in found:
                print(f"{name:40s} {loc:10s} {found[h]}")
    print("complete:", ", ".join(complete) if complete else "none")
    return 0 if complete else 1


if __name__ == "__main__":
    sys.exit(main())
