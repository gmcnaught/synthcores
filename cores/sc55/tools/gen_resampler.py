#!/usr/bin/env python3
"""Output resamplers to 48 kHz for hdl/board/resample.sv.

Polyphase FIR, up by L, down by M, PH = 64 taps per phase, coefficients
signed Q1.17 (a Kaiser-windowed sinc at fs * L, cut-off 20 kHz, gain L):

    mk1: 64 kHz (48 MHz / 750)      -> 48 kHz: L = 3,  M = 4,  192 taps at 192 kHz
    mk2: 66,207 Hz (48 MHz / 725)   -> 48 kHz: L = 29, M = 40, 1856 taps at 1.92 MHz

Phase tracking: q starts at 0; after input n, if q < L an output is made with
phase p = q (then q += M); then q -= L. The output is
    y = clamp((sum_j h[L j + p] * x[n - j]) >> 17)     (x newest first)
with the integer arithmetic of the HDL (see model()).

  gen_resampler.py            write hdl/board/resample_coef.hex (mk1 at 0, mk2 at 256)
  gen_resampler.py --check    print the responses at a few frequencies
"""
import math
import pathlib
import sys

PH = 64
QBITS = 17
FC = 20_000.0
BETA = 8.0
DESIGNS = {
    "mk1": dict(L=3, M=4, FS=192_000.0, base=0),
    "mk2": dict(L=29, M=40, FS=1_920_000.0, base=256),
}
ROM_WORDS = 4096
ROOT = pathlib.Path(__file__).resolve().parent.parent


def i0(x):
    s, t, k = 1.0, 1.0, 1
    while t > 1e-12 * s:
        t *= (x / (2 * k)) ** 2
        s += t
        k += 1
    return s


def coefficients(model="mk1"):
    d = DESIGNS[model]
    L, FS = d["L"], d["FS"]
    taps = L * PH
    h = []
    for n in range(taps):
        m = n - (taps - 1) / 2
        sinc = 2 * FC / FS if m == 0 else math.sin(2 * math.pi * FC / FS * m) / (math.pi * m)
        w = i0(BETA * math.sqrt(max(0.0, 1 - (2 * n / (taps - 1) - 1) ** 2))) / i0(BETA)
        h.append(L * sinc * w)                     # gain L for the zero-stuffing
    return [round(v * (1 << QBITS)) for v in h]


def model(x, h, L=3, M=4):
    """Integer model: x are 16-bit samples (one channel); returns the 48 kHz outputs."""
    out, hist, q = [], [], 0
    for v in x:
        hist.insert(0, v)
        del hist[PH:]
        if q < L:
            acc = sum(h[L * j + q] * hist[j] for j in range(len(hist)))
            out.append(max(-32768, min(32767, acc >> QBITS)))
            q += M
        q -= L
    return out


def response(h, f, model_name="mk1"):
    d = DESIGNS[model_name]
    FS, L = d["FS"], d["L"]
    re = sum(c * math.cos(2 * math.pi * f / FS * n) for n, c in enumerate(h))
    im = sum(c * math.sin(2 * math.pi * f / FS * n) for n, c in enumerate(h))
    return 20 * math.log10(max(1e-12, math.hypot(re, im) / (L << QBITS)))


def rom():
    words = [0] * ROM_WORDS
    for name, d in DESIGNS.items():
        for n, c in enumerate(coefficients(name)):
            words[d["base"] + n] = c & 0x3ffff
    return words


def main() -> int:
    if "--check" in sys.argv:
        for name in DESIGNS:
            h = coefficients(name)
            print(name, "  ".join(f"{f // 1000}k {response(h, f, name):6.1f}"
                                  for f in (1000, 10000, 18000, 20000, 22000, 24000, 28000, 33000)))
        return 0
    lines = [f"{w:05x}" for w in rom()]
    (ROOT.parent.parent / "modules" / "roland" / "hdl" / "board" / "resample_coef.hex").write_text("\n".join(lines) + "\n")
    print("wrote hdl/board/resample_coef.hex")
    return 0


if __name__ == "__main__":
    sys.exit(main())
