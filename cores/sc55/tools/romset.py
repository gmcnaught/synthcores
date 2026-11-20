#!/usr/bin/env python3
"""Identify SC-55 ROM files by SHA-256 against the reference model's romset table.

The table is read from jcmoyer's doc/supported_romsets.rst (the submodule), so
it is the same table the reference renderer uses.

  romset.py check DIR      list every file in DIR that belongs to a known romset
                           and which romsets are complete
  romset.py pack DIR OUT [--romset mk1-v1.21]
                           write the core's ROM pack (hdl/board/rom_loader.sv):
                           copy it to the MiSTer as games/SC-55/boot.rom
"""
import argparse
import hashlib
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


PACK_LAYOUT = {  # mk1: offsets in the pack (hdl/board/rom_loader.sv)
    "ROM1": (0x000100, 0x8000),
    "ROM2": (0x008100, 0x40000),
    "WAVEROM1": (0x048100, 0x100000),
    "WAVEROM2": (0x148100, 0x100000),
    "WAVEROM3": (0x248100, 0x100000),
}
PACK_SIZE = 0x348100


def pack(directory: pathlib.Path, romset: str) -> bytes:
    if not romset.startswith("mk1-"):
        raise LookupError("the core supports mk1 romsets only (for now)")
    files = find_romset(directory, romset)
    out = bytearray(PACK_SIZE)
    out[0:5] = b"SC55\x01"
    for loc, (off, size) in PACK_LAYOUT.items():
        data = files[loc].read_bytes()
        if len(data) != size:
            raise LookupError(f"{loc}: {len(data)} bytes, expected {size}")
        if loc.startswith("WAVEROM"):
            data = unscramble(data)
        out[off:off + size] = data
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
    args = ap.parse_args()

    if args.cmd == "pack":
        args.out.write_bytes(pack(args.directory, args.romset))
        print(f"wrote {args.out} ({PACK_SIZE} bytes, {args.romset})")
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
