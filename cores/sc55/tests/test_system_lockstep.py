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


def test_mk2_board_from_power_on():
    need_roms("mk2-v1.01")
    exe = need_file(SIM_BUILD / "board_lockstep", "board bench")
    sys.path.insert(0, str(ROOT / "tools"))
    import romset
    with tempfile.TemporaryDirectory() as d:
        pack = pathlib.Path(d) / "boot.rom"
        pack.write_bytes(romset.pack(ROM_DIR, "mk2-v1.01"))
        ok_line(exe, "--roms", str(ROM_DIR), "--romset", "mk2-v1.01", "--pack", str(pack), "--midi-loud", "1", "--steps", "8000000")



def test_board_pack_v3():
    """Pack v3 (wave ROMs as dumped, as the MRAs send them): the loader unscrambles them. The bench
    checks all of DDR3 after the load against the model's wave ROMs, then runs sample for sample."""
    sys.path.insert(0, str(ROOT / "tools"))
    import romset
    exe = need_file(SIM_BUILD / "board_lockstep", "board bench")
    for rs, steps in (("mk1-v1.21", "3000000"), ("mk2-v1.01", "3000000")):
        need_roms(rs)
        with tempfile.TemporaryDirectory() as d:
            pack = pathlib.Path(d) / "boot.rom"
            pack.write_bytes(romset.pack(ROM_DIR, rs, v3=True))
            ok_line(exe, "--roms", str(ROM_DIR), "--romset", rs, "--pack", str(pack), "--midi-loud", "1", "--steps", steps)

def test_mk2_system_loud():
    need_roms("mk2-v1.01")
    exe = need_file(SIM_BUILD / "system_lockstep", "system bench")
    ok_line(exe, "--roms", str(ROM_DIR), "--romset", "mk2-v1.01", "--midi-loud", "1", "--steps", "6000000", "--rom-latency", "10", "--rom-backpressure")



WRAP_48 = (2**48 - 12 * 5_000_000) // (3 << 20) * (3 << 20)   # sim/common/cycle_base.h


def test_cycles_wrap_48():
    """review 2026-11-26 roland.md #10: mcu.cycles and pcm.cycles cross 2^48 at step 5,068,117,
    during loud MIDI; CPU and every sample as the model (64-bit counters)."""
    need_roms()
    exe = need_file(SIM_BUILD / "system_lockstep", "system bench")
    ok_line(exe, "--roms", str(ROM_DIR), "--midi-loud", "1", "--steps", "5500000", "--cycle-base", str(WRAP_48))


def test_mk2_cycles_wrap_48():
    need_roms("mk2-v1.01")
    exe = need_file(SIM_BUILD / "system_lockstep", "system bench")
    ok_line(exe, "--roms", str(ROM_DIR), "--romset", "mk2-v1.01", "--midi-loud", "1", "--steps", "5500000",
            "--cycle-base", str(WRAP_48))

def test_mk2_upstream_hash():
    """avmidi 0A.mid through the mk2 HDL = jcmoyer's own integration hash (test/integration/CMakeLists.txt)."""
    need_roms("mk2-v1.01")
    if not os.environ.get("SC55_LONG"):
        raise Skip("set SC55_LONG=1 (about 40 minutes)")
    exe = need_file(SIM_BUILD / "system_lockstep", "system bench")
    midi = MIDI_DIR / "avmidi" / "0A.mid"
    want = "222e0892c0afad047c9ca00a14dae2dd1f124cc36e81073659aa474ceaa30f59"
    with tempfile.TemporaryDirectory() as d:
        out = pathlib.Path(d) / "hdl.s16"
        ok_line(exe, "--roms", str(ROM_DIR), "--romset", "mk2-v1.01", "--smf", str(midi), "--out", str(out))
        got = hashlib.sha256(out.read_bytes()).hexdigest()
    assert got == want, (got, want)
    print("    SHA-256", got)


if __name__ == "__main__":
    run(globals())
