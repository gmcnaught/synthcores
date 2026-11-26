#!/usr/bin/env python3
"""JV-880 HDL against the reference model (review 2026-11-26 roland.md #1, #2, #10).

ROM-free:
* nvsave: cores/jv880/out/sim/nvsave_tb (synthframe_nvsave: a save with the core running, and the
  paused second pass after a write)

Need the jv880-v1.0.1 ROMs (JV880_ROM_DIR or SC55_ROM_DIR; skip otherwise):
* mcu: sc55_mcu with the jv880 model through boot, ending in a P9DR read (cfg bit 1 as the model)
* pcm: sc55_pcm with the jv880 bank map, 25/29 clock and filter, sample for sample
* system: sc55_core from the factory NVRAM with MIDI, sample for sample; and the same with the
  time counters started 9 M steps before 2^48 (the 48-bit wrap of #10), during the MIDI
* board: jv880_board from power-on (pack over ioctl, DDR3 model), sample for sample, with a
  "Save NVRAM" in the middle that must write the model's NVRAM through the SD model
* render (SC55_LONG=1, about an hour): the system bench renders jv880.mid from the factory NVRAM
  to the reference's integration-test hash

The MCU, PCM and system benches are the SC-55's (cores/sc55/out/sim, SC55_SIM_BUILD); the board
bench and nvsave_tb are cores/jv880/out/sim (JV880_SIM_BUILD).
"""
import functools
import hashlib
import os
import pathlib
import subprocess
import sys
import tempfile

HERE = pathlib.Path(__file__).resolve().parent
JV_ROOT = HERE.parent
SC55_ROOT = JV_ROOT.parent / "sc55"
sys.path.insert(0, str(SC55_ROOT / "tests"))
sys.path.insert(0, str(SC55_ROOT / "tools"))

from testlib import REF_BUILD, Skip, need_file, run  # noqa: E402

SC55_SIM = pathlib.Path(os.environ.get("SC55_SIM_BUILD", SC55_ROOT / "out" / "sim"))
JV_SIM = pathlib.Path(os.environ.get("JV880_SIM_BUILD", JV_ROOT / "out" / "sim"))
ROMSET = "jv880-v1.0.1"
NVRAM_SHA = "d5da784546f9fd482c82beb366c527f313e8ea81bc9039dbb8c531197aa6d207"
RENDER_SHA = "73feda3c5d7153e2ae04eb3a12e3b4a08e0a0e8eeebb39a77159b45385d3bbbc"
MIDI = SC55_ROOT / "third_party" / "nuked-sc55" / "test" / "integration" / "jv880" / "jv880.mid"
# the unit takes MIDI from about 8 M steps on (before that the firmware is still starting up)
MIDI_START = "8000000"
# a multiple of 3 * 2^20 (sim/common/cycle_base.h): cycles crosses 2^48 at step 9,068,117
WRAP_48 = (2**48 - 12 * 9_000_000) // (3 << 20) * (3 << 20)
SILENT = "peak -999.0 dBFS"


def rom_dir() -> pathlib.Path:
    import romset
    for var in ("JV880_ROM_DIR", "SC55_ROM_DIR"):
        d = os.environ.get(var)
        if d and pathlib.Path(d).is_dir():
            try:
                romset.find_romset(pathlib.Path(d), ROMSET)
                return pathlib.Path(d)
            except LookupError:
                pass
    raise Skip(f"no {ROMSET} ROMs in JV880_ROM_DIR or SC55_ROM_DIR")


_TMP = tempfile.TemporaryDirectory()


@functools.cache
def factory_nvram() -> pathlib.Path:
    """The factory-preset NVRAM, made by the reference (cores/sc55/ref/jv880_nvram.cpp)."""
    exe = need_file(REF_BUILD / "jv880-nvram", "jv880-nvram")
    out = pathlib.Path(_TMP.name) / "factory.nvram"
    r = subprocess.run([str(exe), str(rom_dir()), str(out), ROMSET], capture_output=True, text=True)
    assert r.returncode == 0, r.stdout[-2000:] + r.stderr[-2000:]
    sha = hashlib.sha256(out.read_bytes()).hexdigest()
    assert sha == NVRAM_SHA, f"factory NVRAM {sha}"
    return out


def ok_line(exe: pathlib.Path, *args: str) -> str:
    r = subprocess.run([str(exe), *args], capture_output=True, text=True)
    ok = [l for l in r.stdout.splitlines() if l.startswith("OK")]
    assert r.returncode == 0 and ok, f"{exe.name} {' '.join(args)}:\n{r.stdout[-2000:]}\n{r.stderr[-3000:]}"
    print("   ", exe.name, ok[0][:150])
    return r.stdout


def test_nvsave_tb():
    exe = need_file(JV_SIM / "nvsave_tb", "nvsave_tb")
    r = subprocess.run([str(exe)], capture_output=True, text=True)
    assert r.returncode == 0, r.stdout[-2000:] + r.stderr[-2000:]
    print("   ", r.stdout.strip().splitlines()[-1][:150] if r.stdout.strip() else "nvsave_tb OK")


def test_mcu_boot_and_p9dr():
    roms = rom_dir()
    exe = need_file(SC55_SIM / "mcu_lockstep", "MCU lockstep bench")
    ok_line(exe, "--roms", str(roms), "--romset", ROMSET, "--nvram", str(factory_nvram()),
            "--steps", "3000000", "--probe-p9dr")


def test_pcm_loud():
    roms = rom_dir()
    exe = need_file(SC55_SIM / "pcm_lockstep", "PCM lockstep bench")
    out = ok_line(exe, "--roms", str(roms), "--romset", ROMSET, "--nvram", str(factory_nvram()),
                  "--midi-demo", "1", "--midi-start", MIDI_START, "--steps", "10000000", "--rom-latency", "7")
    assert "(0 non-zero" not in out, out


def test_system_loud():
    roms = rom_dir()
    exe = need_file(SC55_SIM / "system_lockstep", "system bench")
    out = ok_line(exe, "--roms", str(roms), "--romset", ROMSET, "--nvram", str(factory_nvram()),
                  "--midi-demo", "1", "--midi-start", MIDI_START, "--steps", "10000000", "--rom-latency", "10",
                  "--rom-backpressure")
    assert SILENT not in out, out


def test_system_cycle_wrap():
    """#10: mcu.cycles and pcm.cycles cross 2^48 at step 9,068,117, during the MIDI."""
    roms = rom_dir()
    exe = need_file(SC55_SIM / "system_lockstep", "system bench")
    out = ok_line(exe, "--roms", str(roms), "--romset", ROMSET, "--nvram", str(factory_nvram()),
                  "--midi-demo", "1", "--midi-start", MIDI_START, "--steps", "9500000", "--cycle-base", str(WRAP_48))
    assert SILENT not in out, out


def test_board_from_power_on_with_save():
    import romset
    roms = rom_dir()
    exe = need_file(JV_SIM / "jv880_board_bench", "JV-880 board bench")
    with tempfile.TemporaryDirectory() as d:
        pack = pathlib.Path(d) / "jv880.rom"
        pack.write_bytes(romset.pack(roms, ROMSET, factory_nvram().read_bytes()))
        sav = pathlib.Path(d) / "jv880.sav"
        out = ok_line(exe, "--roms", str(roms), "--pack", str(pack), "--midi-demo", "1", "--midi-start", MIDI_START,
                      "--steps", "10000000", "--sav", str(sav), "--save-at", "9500000")
        assert "1 saves identical" in out and SILENT not in out, out


def test_render_hash():
    roms = rom_dir()
    if not os.environ.get("SC55_LONG"):
        raise Skip("set SC55_LONG=1 (about an hour)")
    exe = need_file(SC55_SIM / "system_lockstep", "system bench")
    with tempfile.TemporaryDirectory() as d:
        out = pathlib.Path(d) / "hdl.s16"
        ok_line(exe, "--roms", str(roms), "--romset", ROMSET, "--nvram", str(factory_nvram()),
                "--smf", str(MIDI), "--out", str(out))
        got = hashlib.sha256(out.read_bytes()).hexdigest()
    assert got == RENDER_SHA, got
    print("    SHA-256", got)


if __name__ == "__main__":
    run(globals())
