#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Coefficients and integer model for synthframe_resample (rational L/M polyphase FIR).

    gen_resample.py FS_IN L M TAPS FC OUT.hex [--beta B]

Designs a Kaiser-windowed sinc low-pass of L*TAPS taps at FS_IN*L (cutoff FC Hz), quantises it
to Q1.17 (18-bit, each phase summed to 2**17 so DC passes at unit gain), writes OUT.hex (one
18-bit word a line, index p + L*j for phase p and tap j, as the HDL reads it) and prints the
passband ripple and stopband attenuation of the quantised filter.

model(h, L, M, TAPS, xs) is the HDL's integer arithmetic: after input n, while q < L an output
of phase p = q is made, y = clamp((sum_j h[p + L j] * x[n - j] + 2**16) >> 17), q += M; then
q -= L. Inputs before the first read as 0.
"""
import argparse
import math


def i0(x):
    s, t, k = 1.0, 1.0, 1
    while t > 1e-12 * s:
        t *= (x / (2 * k)) ** 2
        s += t
        k += 1
    return s


def design(fs_in, L, M, taps, fc, beta):
    n = L * taps
    fs = fs_in * L
    c = (n - 1) / 2
    h = []
    for i in range(n):
        t = i - c
        x = 2 * fc / fs
        s = x if t == 0 else math.sin(math.pi * x * t) / (math.pi * t)
        w = i0(beta * math.sqrt(max(0.0, 1 - (t / c) ** 2))) / i0(beta)
        h.append(s * w * L)
    # quantise per phase so each phase sums to exactly 2**17
    q = [0] * n
    for p in range(L):
        idx = [p + L * j for j in range(taps)]
        vals = [h[i] * (1 << 17) for i in idx]
        r = [round(v) for v in vals]
        err = (1 << 17) - sum(r)
        order = sorted(range(taps), key=lambda j: -abs(vals[j]))
        k = 0
        while err:
            r[order[k % taps]] += 1 if err > 0 else -1
            err += -1 if err > 0 else 1
            k += 1
        for j, i in enumerate(idx):
            q[i] = r[j]
    assert all(-(1 << 17) <= v < (1 << 17) for v in q)
    return q


def response(q, fs, f):
    re = sum(v * math.cos(2 * math.pi * f * i / fs) for i, v in enumerate(q))
    im = sum(v * math.sin(2 * math.pi * f * i / fs) for i, v in enumerate(q))
    return math.hypot(re, im)


def model(h, L, M, taps, xs):
    hist = [0] * taps
    out = []
    q = 0
    for x in xs:
        hist = [x] + hist[:-1]
        while q < L:
            acc = sum(h[q + L * j] * hist[j] for j in range(taps))
            y = (acc + (1 << 16)) >> 17
            out.append(max(-32768, min(32767, y)))
            q += M
        q -= L
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("fs_in", type=float)
    ap.add_argument("L", type=int)
    ap.add_argument("M", type=int)
    ap.add_argument("taps", type=int)
    ap.add_argument("fc", type=float)
    ap.add_argument("out")
    ap.add_argument("--beta", type=float, default=9.0)
    a = ap.parse_args()
    q = design(a.fs_in, a.L, a.M, a.taps, a.fc, a.beta)
    with open(a.out, "w") as f:
        for v in q:
            f.write(f"{v & 0x3FFFF:05x}\n")
    fs = a.fs_in * a.L
    dc = response(q, fs, 0)
    pb = [20 * math.log10(response(q, fs, f) / dc) for f in range(0, int(a.fc * 0.9), 500)]
    sb = [20 * math.log10(max(response(q, fs, f), 1e-9) / dc)
          for f in range(int(a.fs_in - a.fc * 0.9), int(fs / 2), 1000)]
    print(f"{a.out}: {len(q)} taps ({a.L} phases x {a.taps}), fs {fs:.0f} Hz, cutoff {a.fc:.0f} Hz")
    print(f"passband 0-{a.fc * 0.9:.0f} Hz: {min(pb):+.3f} .. {max(pb):+.3f} dB")
    print(f"images from {a.fs_in - a.fc * 0.9:.0f} Hz: worst {max(sb):.1f} dB")


if __name__ == "__main__":
    main()
