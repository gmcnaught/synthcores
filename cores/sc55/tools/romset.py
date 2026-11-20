#!/usr/bin/env python3
"""Identify SC-55 ROM files by SHA-256 against the reference model's romset table.

The table is read from jcmoyer's doc/supported_romsets.rst (the submodule), so
it is the same table the reference renderer uses.

  romset.py check DIR      list every file in DIR that belongs to a known romset
                           and which romsets are complete
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


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    c = sub.add_parser("check")
    c.add_argument("directory", type=pathlib.Path)
    args = ap.parse_args()

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
