#!/usr/bin/env python3
"""Interpolation kernels of the sound chip (one per tone value) and the tables generated from them.

Source: E-mu's patent US 5,111,727 (Rossum, filed 1990-01-05), [O] in the patent text:
  * 7- or 8-point interpolation: the output is the sum of the surrounding sample words times
    coefficients C_n(f) that depend on the fractional address f. With 7 points the window is
    centred on the nearest word ("conceptually adding 1/2 to the effective address") and two of
    the 16 cycles per output point are left for volume scaling and mixing, which this chip does
    on-chip (32 voices x 16 cycles x 39062.5 Hz = the 20 MHz clock) -> 7 points here [I].
  * The coefficients are one impulse response stored single-sided with 112 points (7-point) or
    128 (8-point), 32 per sample period, mirrored, linearly interpolated by the low fraction bits.
  * Designed with weights: passband 1, transition 1e-6, stopband 2000 at the notches (multiples of
    the sample rate) and 66 at the peaks between them. Table 1 prints a typical 7-point kernel.
  * "Typically a family of six filters" (FIG. 6A-F) of lower cutoff, chosen per voice for upward
    pitch shifts and to darken the timbre; the firmware's tone value (pitch register bits 23..21)
    takes the values 0..5.

Kernels here:
  tone 0: Table 1 of the patent, normalised so every polyphase branch sums to 1. Two printed
          values look misprinted and are corrected (both appear in each mirrored half): entry 0
          -1.76482e-04 -> -1.76482e-05 (neighbours -2.8e-05), entry 32 2.30703e-03 -> 2.12703e-03
          (between 2.00823e-03 and 2.23996e-03 on a smooth rise).
  tones 1..7 [I]: the same design method fitted to Table 1 (weighted least squares, notches imposed
          exactly; fit: passband edge 0.625, stopband edge 1.55 of the source Nyquist, within 1.3 dB
          of Table 1 down to -60 dB). The firmware raises the tone by one per 5 semitones of upward
          transposition, so tone t scales Table 1 by sc = 2^(-5t/12): stopband edge 1.55 * sc, and
          the passband edge chosen so the -6 dB point is Table 1's (1.06) * sc. A 7-point kernel
          cannot stay sharp below about 0.3 of Nyquist: tones 4 and 5 become gentle roll-offs, and
          tones 6 and 7 (not used by the firmware) stop at the narrowest band it allows. FIG. 6 has
          no axis scale, so the real cutoffs of tones 1..5 are unknown.

Table format (all three implementations): K[t][a], a = 0..128, Q16 (65536 = 1.0), a = |x| * 32 for
tap distance x in samples; K[t][a] = 0 for a >= 112 (7-point support). Interpolation
(tools/proteus_model.py), with the 8 words addr+1..addr+8 around the position addr + 4 + f/4096:
    u = |(j - 3) * 4096 - f|, a = u >> 7, w = u & 127, C_j = K[a] + (((K[a + 1] - K[a]) * w) >> 7)
    sample = (sum of word_j * C_j) >> 16
Of the 8 taps the one 3.5 or more samples away has weight 0, which leaves the patent's 7 taps
centred on the nearest word.

Usage (needs numpy): uv run --with numpy python tools/interp_kernels.py
Writes tools/interp_kernels_data.py, mister/rtl/interp_rom.sv and the KERNELS table in
third_party/mame/src/mame/emusys/proteus_snd.cpp (when that file exists; then regenerate
mame/patches/0001 from the MAME tree).
"""

import pathlib
import os

import numpy as np

TONES = 8
OS = 32              # kernel points per sample period
SUP = 112            # 7-point support: K[a] = 0 for a >= 112
NSWT, PSWT = 2000.0, 66.0   # patent: stop weight at a notch / at the peak between notches
FIT = (0.625, 1.55, 0.1)    # [I] passband edge, stopband edge (source Nyquist), stop weight scale,
                            #     fitted so design() reproduces Table 1
TABLE1_6DB = 1.06           # Table 1's -6 dB point (source Nyquist)

# US 5,111,727 Table 1, centre value and one side (x = 0, 1/32, ... 111/32), as printed
TABLE1 = [
    3.41091e-02, 3.40395e-02, 3.38315e-02, 3.34864e-02, 3.30073e-02, 3.23978e-02,
    3.16629e-02, 3.08081e-02, 2.98404e-02, 2.87672e-02, 2.75974e-02, 2.63387e-02,
    2.50015e-02, 2.35951e-02, 2.21342e-02, 2.06358e-02, 1.90737e-02, 1.74941e-02,
    1.59146e-02, 1.43280e-02, 1.27465e-02, 1.11835e-02, 9.64852e-03, 8.15169e-03,
    6.70091e-03, 5.30482e-03, 3.97084e-03, 2.70624e-03, 1.51650e-03, 4.07193e-04,
    -6.17815e-04, -1.55455e-03, -2.40097e-03, -3.15493e-03, -3.81608e-03, -4.38396e-03,
    -4.85995e-03, -5.24526e-03, -5.54262e-03, -5.75447e-03, -5.88508e-03, -5.93834e-03,
    -5.91962e-03, -5.83258e-03, -5.68381e-03, -5.47788e-03, -5.22598e-03, -4.95827e-03,
    -4.58223e-03, -4.21098e-03, -3.82117e-03, -3.41064e-03, -2.98432e-03, -2.55058e-03,
    -2.11401e-03, -1.68040e-03, -1.25316e-03, -8.37246e-04, -4.35900e-04, -5.33297e-05,
    3.08346e-04, 6.45892e-04, 9.57995e-04, 1.24225e-03, 1.49820e-03, 1.72439e-03,
    1.92109e-03, 2.08762e-03, 2.22505e-03, 2.33337e-03, 2.41417e-03, 2.46781e-03,
    2.49661e-03, 2.50154e-03, 2.48535e-03, 2.44830e-03, 2.39336e-03, 2.32145e-03,
    2.23996e-03, 2.30703e-03, 2.00823e-03, 1.89538e-03, 1.78384e-03, 1.66723e-03,
    1.54694e-03, 1.42666e-03, 1.30705e-03, 1.18975e-03, 1.07495e-03, 9.64024e-04,
    8.57506e-04, 7.56578e-04, 6.61072e-04, 5.72071e-04, 4.89258e-04, 4.13468e-04,
    3.44148e-04, 2.81886e-04, 2.26064e-04, 1.77026e-04, 1.34006e-04, 9.71765e-05,
    6.59547e-05, 4.01338e-05, 1.90793e-05, 2.51618e-06, -1.00873e-05, -1.87968e-05,
    -2.46347e-05, -2.88158e-05, -2.80411e-05, -1.76482e-04,
]
TABLE1[79] = 2.12703e-03     # printed 2.30703e-03 (breaks a smooth rise; same in the mirrored half)
TABLE1[111] = -1.76482e-05   # printed -1.76482e-04 (neighbours -2.8e-05; same in the mirrored half)


def design(fc, fs, stop_scale, grid=8192):
    """Symmetric 7-point kernel (h[n] = 0 for n >= 112) at 32x the sample rate; returns h[0..128].

    Patent targets and weights: 1 up to fc (weight 1), don't care up to fs (weight 1e-6), 0 above
    with weight 2000 at the notches and 66 at the peaks between them (times stop_scale). Weighted
    least squares, with exact zeros at every multiple of the sample rate (2, 4, ... 32 in
    source-Nyquist units) so each polyphase branch sums to exactly 1."""
    w = np.linspace(0, np.pi, grid)
    f = w / (np.pi / OS)                 # frequency in units of the source Nyquist (0..32)
    d = np.where(f <= fc, 1.0, 0.0)
    dist = np.abs(f - 2 * np.round(f / 2))          # distance to the nearest notch (0..1)
    k = np.clip(1 + 0.2 * np.log2(np.maximum(dist, 1e-9)), 0, 1)
    wt = np.where(f <= fc, 1.0, np.where(f < fs, 1e-6, stop_scale * NSWT * (PSWT / NSWT) ** k))
    n = np.arange(SUP)
    cosb = lambda ww: np.cos(np.outer(ww, n)) * np.where(n == 0, 1.0, 2.0)
    sw = np.sqrt(wt)
    A, b = cosb(w) * sw[:, None], d * sw
    C = cosb(np.arange(2, OS + 1, 2) * np.pi / OS)
    M = np.block([[A.T @ A, C.T], [C, np.zeros((len(C), len(C)))]])
    h = np.linalg.lstsq(M, np.concatenate([A.T @ b, np.zeros(len(C))]), rcond=None)[0][:SUP]
    h = np.append(h, np.zeros(129 - SUP))
    return h * OS / (h[0] + 2 * h[1:].sum())


def table1():
    h = np.append(np.array(TABLE1), np.zeros(129 - SUP))
    return h * OS / (h[0] + 2 * h[1:].sum())


def response_db(h, f_nyq):
    """|H| in dB at frequencies given in source-Nyquist units, relative to DC."""
    w = np.asarray(f_nyq) * np.pi / OS
    n = np.arange(len(h))
    H = (np.cos(np.outer(w, n)) * np.where(n == 0, 1.0, 2.0)) @ h
    return 20 * np.log10(np.maximum(np.abs(H) / OS, 1e-12))


def scaled(t):
    """Tone t >= 1: stopband edge 1.55 * sc; passband edge found so that -6 dB falls at 1.06 * sc."""
    sc = 2 ** (-5 * t / 12)
    fs, target = FIT[1] * sc, TABLE1_6DB * sc
    ff = np.linspace(0, 2, 4001)
    lo, hi = 0.0, fs * 0.98
    for _ in range(30):
        fc = (lo + hi) / 2
        h = design(fc, fs, FIT[2])
        if ff[np.argmax(response_db(h, ff) < -6)] < target:
            lo = fc
        else:
            hi = fc
    return h


def tables():
    out = []
    for t in range(TONES):
        h = table1() if t == 0 else scaled(t)
        out.append([int(round(v * 65536)) for v in h])
    return out


def write_py(path, K):
    with open(path, "w") as f:
        f.write("# Generated by tools/interp_kernels.py - do not edit.\n")
        f.write("# Interpolation kernels K[tone][a], a = 0..128 (32 per sample period), Q16.\n")
        f.write("KERNELS = [\n")
        for row in K:
            f.write("    [" + ", ".join(map(str, row)) + "],\n")
        f.write("]\n")


def write_mame(path, K):
    """Replace the rows of `constexpr s32 KERNELS[8][129] = { ... };` in proteus_snd.cpp."""
    src = pathlib.Path(path).read_text()
    head = "constexpr s32 KERNELS[8][129] = {\n"
    start = src.index(head) + len(head)
    end = src.index("};\n", start)
    rows = "".join("\t{ " + ", ".join(map(str, row)) + " },\n" for row in K)
    with open(path, "w") as f:
        f.write(src[:start] + rows + src[end:])


def write_sv(path, K):
    lines = [
        "// Generated by tools/interp_kernels.py - do not edit.",
        "// Interpolation kernel ROM: idx = {tone[2:0], a[7:0]} -> {K[a+1] - K[a], K[a]}, 18-bit signed each.",
        "module interp_rom",
        "(",
        "\tinput             clk,",
        "\tinput      [10:0] idx,",
        "\toutput reg [35:0] q",
        ");",
        "always @(posedge clk) begin",
        "\tcase (idx)",
    ]
    for t, row in enumerate(K):
        ext = row + [0]
        for a in range(129):
            k, dk = ext[a], ext[a + 1] - ext[a]
            if k or dk:
                lines.append(f"\t11'd{(t << 8) | a}: q <= {{18'sd{dk & 0x3FFFF}, 18'sd{k & 0x3FFFF}}};"
                             .replace("18'sd", "18'd"))
    lines += ["\tdefault: q <= 36'd0;", "\tendcase", "end", "endmodule", ""]
    with open(path, "w") as _f: _f.write("\n".join(lines))


def main():
    root = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..")
    K = tables()
    for row in K:
        assert all(v == 0 for v in row[SUP:]) and max(map(abs, row)) < 1 << 17
    write_py(os.path.join(root, "tools", "interp_kernels_data.py"), K)
    write_sv(os.path.join(root, "mister", "rtl", "interp_rom.sv"), K)
    mame = os.path.join(root, "third_party", "mame", "src", "mame", "emusys", "proteus_snd.cpp")
    if os.path.isfile(mame):
        write_mame(mame, K)
    # summary: response of the stored (quantised) kernels
    ff = np.linspace(0, 2, 2001)
    for t, row in enumerate(K):
        h = np.array(row, float) / 65536
        r = response_db(h, ff)
        pts = " ".join(f"{lv} dB {ff[np.argmax(r < lv)]:5.3f}" for lv in (-1, -3, -6, -20, -40))
        notch = response_db(h, [2.0, 4.0, 8.0])
        peaks = [response_db(h, np.linspace(c - 0.8, c + 0.8, 400)).max() for c in (3, 5, 9)]
        print(f"tone {t}{' (Table 1)' if t == 0 else ''}: {pts} (x source Nyquist); "
              f"notches {' '.join(f'{v:4.0f}' for v in notch)} dB; "
              f"peaks at 1.5/2.5/4.5x fs {' '.join(f'{v:5.1f}' for v in peaks)} dB")


if __name__ == "__main__":
    main()
