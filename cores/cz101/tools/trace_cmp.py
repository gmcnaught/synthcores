#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Compare two CZ-101 traces (patched MAME format) up to N instruction lines.
Usage: trace_cmp.py A B N   -> 'identical K' or the first difference with context."""
import sys
from itertools import zip_longest


def lines(path, n):
    k, buf = 0, []
    with open(path) as f:
        for l in f:
            if l[0] in "WI" or l.startswith(("AD ", "SIO ")):
                buf.append(l)
                continue
            yield from buf
            buf = []
            yield l
            k += 1
            if k == n:
                return


a, b, n = sys.argv[1], sys.argv[2], int(sys.argv[3])
ctx = []
for i, (x, y) in enumerate(zip_longest(lines(a, n), lines(b, n))):
    if x != y:
        print(f"first difference at line {i}")
        print("".join(ctx[-6:]) + f"A: {x}B: {y}", end="")
        sys.exit(1)
    ctx.append(x)
    if len(ctx) > 50:
        ctx = ctx[-10:]
print(f"identical {i + 1} lines")
