#!/usr/bin/env python3
"""H8/532 CPU HDL in lockstep with the reference MCU (sim/cpu/cpu_lockstep.cpp).

Random code runs without ROMs (CI); firmware boot and MIDI traffic need mk1-v1.21.
"""
import os
import pathlib
import subprocess

from testlib import ROM_DIR, ROOT, need_file, need_roms, run

SIM_BUILD = pathlib.Path(os.environ.get("SC55_SIM_BUILD", ROOT / "out" / "sim"))


def bench(*args):
    exe = need_file(SIM_BUILD / "cpu_lockstep", "CPU lockstep bench")
    r = subprocess.run([str(exe), *args], capture_output=True, text=True)
    ok = [l for l in r.stdout.splitlines() if l.startswith("OK:")]
    assert r.returncode == 0 and ok, f"{args}:\n{r.stdout[-2000:]}\n{r.stderr[-3000:]}"
    print("   ", " ".join(args[2:]), "->", ok[0][:110])


def test_random_code_without_roms():
    for seed in (11, 12, 13):
        bench("--roms", "none", "--random", str(seed), "--steps", "400000")


def test_random_code_with_firmware_pages():
    need_roms()
    for seed in (1, 2):
        bench("--roms", str(ROM_DIR), "--random", str(seed), "--steps", "400000")


def test_firmware_boot():
    need_roms()
    bench("--roms", str(ROM_DIR), "--steps", "6000000")


def test_firmware_midi_traffic():
    need_roms()
    bench("--roms", str(ROM_DIR), "--midi-demo", "3", "--steps", "10000000")


if __name__ == "__main__":
    run(globals())
