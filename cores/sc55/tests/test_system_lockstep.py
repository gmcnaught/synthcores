#!/usr/bin/env python3
"""Whole core and board against the reference (needs mk1-v1.21):

* system: sc55_core with loud MIDI, sample for sample
* renderer: an avmidi file rendered by the HDL system in jcmoyer's renderer
  schedule gives the same SHA-256 as `sc55-render --stdout` (slow: ~12 min;
  set SC55_LONG=1)
* board: sc55_board from power-on with the ROM pack over ioctl and a DDR3
  model, sample for sample
"""
import hashlib
import os
import pathlib
import subprocess
import sys
import tempfile

from testlib import MIDI_DIR, REF_BUILD, ROM_DIR, ROOT, Skip, need_file, need_roms, run

SIM_BUILD = pathlib.Path(os.environ.get("SC55_SIM_BUILD", ROOT / "out" / "sim"))


def ok_line(exe, *args):
    r = subprocess.run([str(exe), *args], capture_output=True, text=True)
    ok = [l for l in r.stdout.splitlines() if l.startswith("OK:")]
    assert r.returncode == 0 and ok, f"{args}:\n{r.stdout[-2000:]}\n{r.stderr[-3000:]}"
    print("   ", ok[0][:160])
    return ok[0]


def test_system_loud():
    need_roms()
    exe = need_file(SIM_BUILD / "system_lockstep", "system bench")
    ok_line(exe, "--roms", str(ROM_DIR), "--midi-loud", "1", "--steps", "8000000", "--rom-latency", "10", "--rom-backpressure")


def test_board_from_power_on():
    need_roms()
    exe = need_file(SIM_BUILD / "board_lockstep", "board bench")
    sys.path.insert(0, str(ROOT / "tools"))
    import romset
    with tempfile.TemporaryDirectory() as d:
        pack = pathlib.Path(d) / "boot.rom"
        pack.write_bytes(romset.pack(ROM_DIR, "mk1-v1.21"))
        ok_line(exe, "--roms", str(ROM_DIR), "--pack", str(pack), "--midi-loud", "1", "--steps", "10000000")


def test_renderer_hash():
    need_roms()
    if not os.environ.get("SC55_LONG"):
        raise Skip("set SC55_LONG=1 (about 12 minutes)")
    exe = need_file(SIM_BUILD / "system_lockstep", "system bench")
    ref = need_file(REF_BUILD / "sc55-render", "reference renderer")
    midi = MIDI_DIR / "avmidi" / "0A.mid"
    want = hashlib.sha256(subprocess.run([str(ref), "--quiet", "--stdout", "-d", str(ROM_DIR), "--romset", "mk1-v1.21",
                                          "--reset", "gm", str(midi)], capture_output=True, check=True).stdout).hexdigest()
    with tempfile.TemporaryDirectory() as d:
        out = pathlib.Path(d) / "hdl.s16"
        ok_line(exe, "--roms", str(ROM_DIR), "--smf", str(midi), "--out", str(out))
        got = hashlib.sha256(out.read_bytes()).hexdigest()
    assert got == want, (got, want)
    print("    SHA-256", got)


if __name__ == "__main__":
    run(globals())
