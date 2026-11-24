#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Opcode forms and interrupt entries a reference trace executed.

    mcs96_coverage.py ROM TRACE [TRACE ...]   (ROM: the 128 KB control ROM the traces ran)
Prints how many of the decode table's valid inst_states ran, and the ones that did not.
"""
import re
import sys
from pathlib import Path

CORE = Path(__file__).resolve().parent.parent
tab = {}
for l in (CORE / "hdl/mcs96/mcs96_decode.txt").read_text().splitlines():
    m = re.match(r"([0-9a-f]{3})\s+(\w+)\s+(\w+)", l)
    if m and "INVALID" not in l:
        tab[int(m.group(1), 16)] = f"{m.group(2)} {m.group(3)}"
seen, irq = set(), {}
for rom_p, tr in zip(sys.argv[1::2], sys.argv[2::2]):
    rom = Path(rom_p).read_bytes()
    for line in open(tr):
        if line[0] == "F":
            pc = int(line.split()[2], 16)
            b = rom[pc] if pc < len(rom) else 0
            seen.add(0x100 | rom[pc + 1] if b == 0xFE else b)
        elif line[0] == "I":
            lv = line.split()[2]
            irq[lv] = irq.get(lv, 0) + 1
hit = sorted(s for s in seen if s in tab)
print(f"{len(hit)} of {len(tab)} opcode forms; interrupts by level: {dict(sorted(irq.items()))}")
print("not run:", " ".join(f"{s:03x}({tab[s]})" for s in sorted(set(tab) - seen)))
