#!/usr/bin/env python3
"""hdl/board/resample.sv equals tools/gen_resampler.py's integer model; the coefficients are current."""
import math
import os
import pathlib
import random
import subprocess
import sys

from testlib import ROOT, need_file, run

sys.path.insert(0, str(ROOT / "tools"))
import gen_resampler as gr  # noqa: E402

SIM_BUILD = pathlib.Path(os.environ.get("SC55_SIM_BUILD", ROOT / "out" / "sim"))


def test_coefficients_current():
    h = gr.coefficients()
    text = (ROOT / "hdl" / "board" / "resample_coef.svh").read_text()
    for n, c in enumerate(h):
        assert f"8'd{n}: return {'-' if c < 0 else ''}18'sd{abs(c)};" in text, n


def test_hdl_equals_model():
    exe = need_file(SIM_BUILD / "resample_tb", "resampler bench")
    rng = random.Random(5)
    left = [rng.randint(-32768, 32767) for _ in range(600)]
    left += [int(30000 * math.sin(2 * math.pi * 19000 * i / 64000)) for i in range(800)]
    left += [32767] * 200 + [-32768] * 200                    # clamping
    right = [rng.randint(-32768, 32767) for _ in range(len(left))]
    inp = "".join(f"{a} {b}\n" for a, b in zip(left, right))
    out = subprocess.run([str(exe)], input=inp, capture_output=True, text=True, check=True).stdout.split("\n")
    got = [tuple(map(int, l.split())) for l in out if l.strip()]
    h = gr.coefficients()
    want = list(zip(gr.model(left, h), gr.model(right, h)))
    assert len(got) == len(want), (len(got), len(want))
    bad = [i for i, (g, w) in enumerate(zip(got, want)) if g != w]
    assert not bad, f"{len(bad)} outputs differ, first {bad[0]}: {got[bad[0]]} vs {want[bad[0]]}"
    print(f"    {len(got)} outputs identical")


def test_passband_and_stopband():
    h = gr.coefficients()
    assert abs(gr.response(h, 10000)) < 0.1
    assert gr.response(h, 24000) < -80


if __name__ == "__main__":
    run(globals())
