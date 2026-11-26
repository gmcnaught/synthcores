#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Compare two CZ-101 traces (patched MAME format) up to N instruction lines.
Usage: trace_cmp.py A B N   -> 'identical K' or the first difference with context.
Fails when the traces differ, or when they hold fewer than N instruction lines."""
import sys
from itertools import zip_longest


count = {}


def lines(path, n):
    k, buf = 0, []
    count[path] = 0
    with open(path) as f:
        for l in f:
            if l[0] in "WI" or l.startswith(("AD ", "SIO ")):
                buf.append(l)
                continue
            yield from buf
            buf = []
            yield l
            k += 1
            count[path] = k
            if k == n:
                return


a, b, n = sys.argv[1], sys.argv[2], int(sys.argv[3])
ctx = []
i = -1
for i, (x, y) in enumerate(zip_longest(lines(a, n), lines(b, n))):
    if x != y:
        print(f"first difference at line {i}")
        print("".join(ctx[-6:]) + f"A: {x}B: {y}", end="")
        sys.exit(1)
    ctx.append(x)
    if len(ctx) > 50:
        ctx = ctx[-10:]
short = {p: k for p, k in count.items() if k < n}
if short:
    print("too short: " + ", ".join(f"{p} has {k} of {n} instructions" for p, k in short.items()))
    sys.exit(1)
print(f"identical {i + 1} lines")
