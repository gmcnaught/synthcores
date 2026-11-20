#!/usr/bin/env python3
"""PCM chip HDL in lockstep with the reference PCM, sample for sample (needs mk1-v1.21)."""
import os
import pathlib
import subprocess

from testlib import ROM_DIR, ROOT, need_file, need_roms, run

SIM_BUILD = pathlib.Path(os.environ.get("SC55_SIM_BUILD", ROOT / "out" / "sim"))


def bench(*args):
    exe = need_file(SIM_BUILD / "pcm_lockstep", "PCM lockstep bench")
    r = subprocess.run([str(exe), "--roms", str(ROM_DIR), *args], capture_output=True, text=True)
    ok = [l for l in r.stdout.splitlines() if l.startswith("OK:")]
    assert r.returncode == 0 and ok, f"{args}:\n{r.stdout[-2000:]}\n{r.stderr[-3000:]}"
    print("   ", " ".join(args), "->", ok[0][:140])
    return ok[0]


def test_notes_sound_and_match():
    need_roms()
    line = bench("--midi-demo", "1", "--steps", "9000000")
    assert "non-zero" in line and " 0 non-zero" not in line, line


def test_rom_latency():
    need_roms()
    bench("--midi-demo", "2", "--steps", "7000000", "--rom-latency", "11")


if __name__ == "__main__":
    run(globals())
