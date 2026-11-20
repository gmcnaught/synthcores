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
    lines = (ROOT / "hdl" / "board" / "resample_coef.hex").read_text().split()
    assert [int(w, 16) for w in lines] == gr.rom()


def hdl_equals_model(name):
    exe = need_file(SIM_BUILD / "resample_tb", "resampler bench")
    d = gr.DESIGNS[name]
    fs_in = d["FS"] / d["L"]
    rng = random.Random(5)
    left = [rng.randint(-32768, 32767) for _ in range(600)]
    left += [int(30000 * math.sin(2 * math.pi * 19000 * i / fs_in)) for i in range(800)]
    left += [32767] * 200 + [-32768] * 200                    # clamping
    right = [rng.randint(-32768, 32767) for _ in range(len(left))]
    inp = "".join(f"{a} {b}\n" for a, b in zip(left, right))
    out = subprocess.run([str(exe), name], input=inp, capture_output=True, text=True, check=True).stdout.split("\n")
    got = [tuple(map(int, l.split())) for l in out if l.strip()]
    h = gr.coefficients(name)
    want = list(zip(gr.model(left, h, d["L"], d["M"]), gr.model(right, h, d["L"], d["M"])))
    assert len(got) == len(want), (len(got), len(want))
    bad = [i for i, (g, w) in enumerate(zip(got, want)) if g != w]
    assert not bad, f"{len(bad)} outputs differ, first {bad[0]}: {got[bad[0]]} vs {want[bad[0]]}"
    print(f"    {name}: {len(got)} outputs identical")


def test_hdl_equals_model_mk1():
    hdl_equals_model("mk1")


def test_hdl_equals_model_mk2():
    hdl_equals_model("mk2")


def test_passband_and_stopband():
    for name in gr.DESIGNS:
        h = gr.coefficients(name)
        assert abs(gr.response(h, 10000, name)) < 0.1, name
        assert gr.response(h, 24000, name) < -80, name


if __name__ == "__main__":
    run(globals())
