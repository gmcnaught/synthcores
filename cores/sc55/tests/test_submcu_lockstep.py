#!/usr/bin/env python3
"""SC-55mk2 sub-MCU HDL in lockstep with the reference's submcu.cpp (needs mk2-v1.01)."""
import os
import pathlib
import subprocess

from testlib import ROM_DIR, ROOT, need_file, need_roms, run

SIM_BUILD = pathlib.Path(os.environ.get("SC55_SIM_BUILD", ROOT / "out" / "sim"))


def bench(*args):
    exe = need_file(SIM_BUILD / "submcu_lockstep", "sub-MCU lockstep bench")
    r = subprocess.run([str(exe), "--roms", str(ROM_DIR), *args], capture_output=True, text=True)
    ok = [l for l in r.stdout.splitlines() if l.startswith("OK")]
    assert r.returncode == 0 and ok, f"{args}:\n{r.stdout[-2000:]}\n{r.stderr[-3000:]}"
    print("   ", " ".join(args), "->", ok[0][:120])


def test_firmware_midi_buttons():
    need_roms("mk2-v1.01")
    bench("--steps", "8000000", "--midi-demo", "1", "--buttons", "3")


def test_random_code():
    need_roms("mk2-v1.01")
    for seed in ("1", "2", "3"):
        bench("--steps", "1000000", "--random", seed)


if __name__ == "__main__":
    run(globals())
