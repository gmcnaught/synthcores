#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Extract the uPD7810 opcode tables from MAME's upd7810_table.cpp.

Usage: upd7810_tables.py MAME_DIR [OUT.json]

Writes {table: [[handler, oplen, cycles, cycles_skip, lmask], ...256]} for the tables the
uPD7810/7811 uses: s_opXX_7810 (as "XX") and the prefixes 48, 4C, 4D, 60, 64, 70, 74.
lmask: bit 0 = L0, bit 1 = L1 (the flags the opcode clears).
"""
import json, re, sys
from pathlib import Path

TABLES = {"XX": "s_opXX_7810", "48": "s_op48", "4C": "s_op4C", "4D": "s_op4D",
          "60": "s_op60", "64": "s_op64", "70": "s_op70", "74": "s_op74"}
ENTRY = re.compile(r"\{&upd7810_device::(\w+),\s*(\d+),\s*(\d+),\s*(\d+),\s*([\w|]+)\s*\}")


def lmask(s):
    return sum({"L0": 1, "L1": 2, "0": 0}[t.strip()] for t in s.split("|"))


def extract(mame_dir):
    src = (Path(mame_dir) / "src/devices/cpu/upd7810/upd7810_table.cpp").read_text()
    out = {}
    for key, name in TABLES.items():
        start = src.index(f"upd7810_device::{name}[256] =")
        end = src.index("};", start)
        body = "\n".join(l for l in src[start:end].splitlines() if not l.lstrip().startswith("//"))
        rows = [(m[0], int(m[1]), int(m[2]), int(m[3]), lmask(m[4]))
                for m in ENTRY.findall(body)]
        if len(rows) != 256:
            sys.exit(f"{name}: {len(rows)} entries")
        out[key] = rows
    return out


if __name__ == "__main__":
    t = extract(sys.argv[1])
    if len(sys.argv) > 2:
        Path(sys.argv[2]).write_text(json.dumps(t, indent=0))
    names = sorted({r[0] for rows in t.values() for r in rows})
    print(f"{sum(len(v) for v in t.values())} entries, {len(names)} distinct handlers")
