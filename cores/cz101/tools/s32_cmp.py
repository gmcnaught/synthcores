#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Compare MAME's raw 40 kHz samples (CZ_SAMPLES) with cz101_bench's.

Usage: s32_cmp.py MAME.s32 HDL.s32 BENCH_OUT [LABEL]

BENCH_OUT is cz101_bench's stdout ("N instructions, C cycles, S samples, ..."). Fails when:
  - either file is empty, or the HDL file does not hold the S samples the bench reported;
  - S differs by more than one from the samples due by C cycles (one per 250 ticks of 10 MHz,
    the first at tick 0);
  - MAME has more samples than the HDL, or more than MAX_LAG fewer (MAME's stream is only
    brought up to date by an access or at a frame end, so it stops short of the HDL's count);
  - any sample in the common prefix differs.
Prints "LABEL: identical N (Z non-zero), sha256 H" on success.
"""
import hashlib, re, struct, sys

MAX_LAG = 1000      # samples (25 ms): MAME brings its stream up to date at least once a frame


def ticks(a):       # 10 MHz tick of CPU cycle a: 3a - ceil(3a / 5,000,000)
    t = 3 * a
    return t - (t + 4999999) // 5000000


def main():
    mame_p, hdl_p, bench_p = sys.argv[1:4]
    label = sys.argv[4] if len(sys.argv) > 4 else "samples"
    a, b = open(mame_p, "rb").read(), open(hdl_p, "rb").read()
    m = re.search(r"(\d+) instructions, (\d+) cycles, (\d+) samples", open(bench_p).read())
    fail = []
    if not m:
        fail.append(f"no sample count in {bench_p}")
    if len(a) < 4 or len(b) < 4:
        fail.append(f"empty input: MAME {len(a)} bytes, HDL {len(b)} bytes")
    if len(a) % 4 or len(b) % 4:
        fail.append(f"partial sample: MAME {len(a)} bytes, HDL {len(b)} bytes")
    na, nb = len(a) // 4, len(b) // 4
    if m:
        cycles, reported = int(m[2]), int(m[3])
        due = ticks(cycles) // 250 + 1
        if reported != nb:
            fail.append(f"bench reported {reported} samples, {hdl_p} holds {nb}")
        if abs(reported - due) > 1:
            fail.append(f"bench made {reported} samples in {cycles} cycles, {due} due")
    if na > nb or nb - na > MAX_LAG:
        fail.append(f"sample counts: MAME {na}, HDL {nb} (MAME may lag by up to {MAX_LAG})")
    if fail:
        print(f"{label}: FAIL: " + "; ".join(fail))
        sys.exit(1)
    va = struct.unpack(f"<{na}i", a)
    vb = struct.unpack(f"<{na}i", b[:na * 4])
    d = next((i for i in range(na) if va[i] != vb[i]), None)
    if d is not None:
        print(f"{label}: FAIL at {d} of {na}: MAME {va[d]} HDL {vb[d]}")
        sys.exit(1)
    nz = sum(1 for x in va if x)
    print(f"{label}: identical {na} ({nz} non-zero), sha256 {hashlib.sha256(a).hexdigest()}")


if __name__ == "__main__":
    main()
