#!/usr/bin/env python3
"""MCU system HDL (CPU + memories + peripherals) in lockstep with the reference (needs mk1-v1.21)."""
import os
import pathlib
import subprocess

from testlib import ROM_DIR, ROOT, need_file, need_roms, run

SIM_BUILD = pathlib.Path(os.environ.get("SC55_SIM_BUILD", ROOT / "out" / "sim"))


def bench(*args):
    exe = need_file(SIM_BUILD / "mcu_lockstep", "MCU lockstep bench")
    r = subprocess.run([str(exe), "--roms", str(ROM_DIR), *args], capture_output=True, text=True)
    ok = [l for l in r.stdout.splitlines() if l.startswith("OK:")]
    assert r.returncode == 0 and ok, f"{args}:\n{r.stdout[-2000:]}\n{r.stderr[-3000:]}"
    print("   ", " ".join(args), "->", ok[0][:120])


def test_boot():
    need_roms()
    bench("--steps", "6000000")


def test_midi_and_buttons():
    need_roms()
    bench("--midi-demo", "5", "--buttons", "9", "--steps", "12000000")


if __name__ == "__main__":
    run(globals())
