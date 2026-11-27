#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Which H8/300H opcodes (MAME handler names) the U1 traces executed.

Usage: u1_coverage.py MAME_DIR WORKDIR...   (each WORKDIR holds prog.bin and hdl.cmp)
Prints the executed count per handler and the enabled handlers never executed."""
import collections
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent))
from h8h_gen import decode_words, dispatch_tables, load_opcodes  # noqa: E402

ol = load_opcodes(sys.argv[1])
tabs = dispatch_tables(ol)
seen = collections.Counter()
for d in sys.argv[2:]:
    img = (Path(d) / "prog.bin").read_bytes() + bytes(16)
    for line in (Path(d) / "hdl.cmp").read_text().splitlines():
        f = line.split()
        if line.startswith("W") or len(f[2]) != 4:
            continue
        pc = int(f[1], 16)
        words = [(img[pc + i] << 8) | img[pc + i + 1] for i in range(0, 12, 2)]
        o = decode_words(ol, tabs, words)
        seen[o.function_name() if o else "illegal"] += 1
names = sorted({o.function_name() for o in ol.opcode_info if o.enabled})
missing = [n for n in names if n not in seen]
print("%d of %d handlers executed, %d instructions" % (len(names) - len(missing), len(names), sum(seen.values())))
if missing:
    print("never executed:", " ".join(missing))
