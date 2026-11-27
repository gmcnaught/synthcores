#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-or-later
"""Compare two MT-32 trace/log files line by line; print the first difference with context.

    trace_cmp.py A B [--context N] [--min N] [--inject-error]
Exit 0 if identical, equally long and at least --min lines (default 1); 1 otherwise. Two empty
files are not a pass. --inject-error flips one bit of B's middle line before comparing (the low
bit of its last hex digit): the comparator self-check, which must then fail (tests/ci.sh).
"""
import argparse
import itertools
import sys


def inject(lines):
    """B's lines with the low bit of the last hex digit of the middle line flipped."""
    k = len(lines) // 2
    for i in range(k, len(lines)):
        line = lines[i]
        j = max((p for p, c in enumerate(line) if c in "0123456789abcdefABCDEF"), default=-1)
        if j >= 0:
            lines[i] = line[:j] + format(int(line[j], 16) ^ 1, "x") + line[j + 1:]
            break
    return iter(lines)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("a")
    ap.add_argument("b")
    ap.add_argument("--context", type=int, default=12)
    ap.add_argument("--min", type=int, default=1, help="fewest lines that count as a comparison")
    ap.add_argument("--inject-error", action="store_true", help="flip one bit of B's middle line")
    a = ap.parse_args()
    hist = []
    n = 0
    with open(a.a) as fa, open(a.b) as fb:
        if a.inject_error:
            fb = inject(fb.readlines())
        for n, (x, y) in enumerate(itertools.zip_longest(fa, fb), 1):
            if x == y:
                hist.append((x, y))
                if len(hist) > a.context:
                    hist.pop(0)
                continue
            print(f"first difference at line {n}")
            for i, (p, q) in enumerate(hist):
                print(f"{n - len(hist) + i:10}  {p.rstrip():34} {q.rstrip()}")
            print(f"{n:10}  {(x or '<end>').rstrip():34} {(y or '<end>').rstrip()}   <==")
            for i, (p, q) in enumerate(zip(itertools.islice(fa, 5), itertools.islice(fb, 5))):
                print(f"{n + 1 + i:10}  {p.rstrip():34} {q.rstrip()}")
            sys.exit(1)
    if n < a.min:
        print(f"only {n} lines compared, expected at least {a.min}")
        sys.exit(1)
    print(f"identical, {n} lines")


if __name__ == "__main__":
    main()
