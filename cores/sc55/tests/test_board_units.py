#!/usr/bin/env python3
"""Board units without ROMs: the mk2 ROM2 cache (hdl/board/rom2x_cache.sv) against a memory model;
the step pacer's lag count (modules/roland/hdl/board/step_pacer.sv)."""
import os
import pathlib
import subprocess

from testlib import ROOT, need_file, run

SIM_BUILD = pathlib.Path(os.environ.get("SC55_SIM_BUILD", ROOT / "out" / "sim"))


def test_rom2x_cache():
    exe = need_file(SIM_BUILD / "rom2x_cache_tb", "ROM2 cache bench")
    r = subprocess.run([str(exe)], capture_output=True, text=True)
    assert r.returncode == 0 and r.stdout.startswith("OK"), r.stdout + r.stderr
    print("   ", r.stdout.strip())


def test_step_pacer_lag():
    exe = need_file(SIM_BUILD / "step_pacer_tb", "step pacer bench")
    r = subprocess.run([str(exe)], capture_output=True, text=True)
    assert r.returncode == 0 and r.stdout.startswith("OK"), r.stdout + r.stderr
    print("   ", r.stdout.strip())


if __name__ == "__main__":
    run(globals())
