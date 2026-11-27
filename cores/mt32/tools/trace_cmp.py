#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-or-later
"""Compare two MT-32 trace/log files line by line; print the first difference with context.

    trace_cmp.py A B [--context N] [--min N]
Exit 0 if identical, equally long and at least --min lines (default 1); 1 otherwise. Two empty
files are not a pass.
"""
import argparse
import itertools
import sys


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("a")
    ap.add_argument("b")
    ap.add_argument("--context", type=int, default=12)
    ap.add_argument("--min", type=int, default=1, help="fewest lines that count as a comparison")
    a = ap.parse_args()
    hist = []
    n = 0
    with open(a.a) as fa, open(a.b) as fb:
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
