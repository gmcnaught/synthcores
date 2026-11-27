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

from testlib import MIDI_DIR, REF_BUILD, ROM_DIR, ROOT, Skip, need_file, need_roms, require_sound, run

SIM_BUILD = pathlib.Path(os.environ.get("SC55_SIM_BUILD", ROOT / "out" / "sim"))
# The benches send MIDI from 4 M steps; the mk1 sounds soon after. The mk2 firmware sets up its PCM only after
# about 10 M steps, so an mk2 run must reach 12 M to compare anything but silence.
MK2_SOUNDING = "12000000"


def ok_line(exe, *args):
    r = subprocess.run([str(exe), *args], capture_output=True, text=True)
    ok = [l for l in r.stdout.splitlines() if l.startswith("OK:")]
    assert r.returncode == 0 and ok, f"{args}:\n{r.stdout[-2000:]}\n{r.stderr[-3000:]}"
    print("   ", ok[0][:160])
    require_sound(ok[0])   # every system and board run must sound, not only match
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
        ok_line(exe, "--roms", str(ROM_DIR), "--romset", "mk2-v1.01", "--pack", str(pack), "--midi-loud", "1", "--steps", MK2_SOUNDING)



def test_board_pack_v3():
    """Pack v3 (wave ROMs as dumped, as the MRAs send them): the loader unscrambles them. The bench
    checks all of DDR3 after the load against the model's wave ROMs, then runs sample for sample."""
    sys.path.insert(0, str(ROOT / "tools"))
    import romset
    exe = need_file(SIM_BUILD / "board_lockstep", "board bench")
    for rs, steps in (("mk1-v1.21", "5000000"), ("mk2-v1.01", MK2_SOUNDING)):
        need_roms(rs)
        with tempfile.TemporaryDirectory() as d:
            pack = pathlib.Path(d) / "boot.rom"
            pack.write_bytes(romset.pack(ROM_DIR, rs, v3=True))
            ok_line(exe, "--roms", str(ROM_DIR), "--romset", rs, "--pack", str(pack), "--midi-loud", "1", "--steps", steps)

def test_mk2_system_loud():
    need_roms("mk2-v1.01")
    exe = need_file(SIM_BUILD / "system_lockstep", "system bench")
    ok_line(exe, "--roms", str(ROM_DIR), "--romset", "mk2-v1.01", "--midi-loud", "1", "--steps", MK2_SOUNDING, "--rom-latency", "10", "--rom-backpressure")



WRAP_48 = (2**48 - 12 * 5_000_000) // (3 << 20) * (3 << 20)   # sim/common/cycle_base.h


def test_cycles_wrap_48():
    """review 2026-11-26 roland.md #10: mcu.cycles and pcm.cycles cross 2^48 at step 5,068,117,
    during loud MIDI; CPU and every sample as the model (64-bit counters)."""
    need_roms()
    exe = need_file(SIM_BUILD / "system_lockstep", "system bench")
    ok_line(exe, "--roms", str(ROM_DIR), "--midi-loud", "1", "--steps", "5500000", "--cycle-base", str(WRAP_48))


def test_mk2_cycles_wrap_48():
    """The mk2 sets up its PCM only after about 10 M steps: cross 2^48 at step 11,621,717, while it sounds."""
    need_roms("mk2-v1.01")
    exe = need_file(SIM_BUILD / "system_lockstep", "system bench")
    base = (2**48 - 12 * 11_500_000) // (3 << 20) * (3 << 20)
    ok_line(exe, "--roms", str(ROM_DIR), "--romset", "mk2-v1.01", "--midi-loud", "1", "--steps", "12000000",
            "--cycle-base", str(base))


def test_board_end_of_run_count():
    """review 2026-11-26 roland.md #3: the board bench fails when the HDL's last samples are
    missing (self-test: --drop-hdl-from discards them)."""
    need_roms()
    exe = need_file(SIM_BUILD / "board_lockstep", "board bench")
    sys.path.insert(0, str(ROOT / "tools"))
    import romset
    with tempfile.TemporaryDirectory() as d:
        pack = pathlib.Path(d) / "boot.rom"
        pack.write_bytes(romset.pack(ROM_DIR, "mk1-v1.21"))
        r = subprocess.run([str(exe), "--roms", str(ROM_DIR), "--pack", str(pack), "--steps", "3000000",
                            "--drop-hdl-from", "2999000"], capture_output=True, text=True)
    assert r.returncode != 0 and "MISMATCH at the end" in r.stderr, r.stdout[-1000:] + r.stderr[-1000:]
    print("    ", r.stderr.strip().splitlines()[-1][:150])

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


DT1_MASTER_TUNE = bytes.fromhex("f0 41 10 42 12 40 00 00 00 04 00 00 3c f7")   # the reference's reply (smoke.sh)


def test_board_serial_rq1():
    """synthcores #52: MIDI in over uart_rxd at 9600 baud, through synthframe_midi_rx's speed
    detection, as the MiSTer smoke sends it (8 x 00, GM reset, then the GS RQ1 for master tune).
    The mk1 firmware starts its DT1 reply by setting SCR.TIE while TDRE is 1; with the model's
    single-buffered SCI no TXI came, and the core sent nothing after the RQ1 (not even Active
    Sensing), and the note after it stayed silent. The SC-155 shares the transmitter; its firmware
    does not answer the RQ1 (before or after the fix). On uart_txd, read at 9600 as the host does:
    the mk1's DT1, then Active Sensing (FE) again; the note sounds; the HDL in lockstep with the
    model throughout."""
    exe = need_file(SIM_BUILD / "board_lockstep", "board bench")
    sys.path.insert(0, str(ROOT / "tools"))
    import romset
    for rs, reply in (("mk1-v1.21", DT1_MASTER_TUNE), ("sc155-rev1", b"")):
        need_roms(rs)
        with tempfile.TemporaryDirectory() as d:
            pack = pathlib.Path(d) / "boot.rom"
            pack.write_bytes(romset.pack(ROM_DIR, rs))
            r = subprocess.run([str(exe), "--roms", str(ROM_DIR), "--romset", rs, "--pack", str(pack), "--steps", "3000000",
                                "--rx-baud", "9600", "--midi-at", "1000000:0000000000000000f07e7f0901f7",
                                "--midi-at", "1300000:f0411042114000000000043cf7", "--midi-at", "1400000:903c7f"],
                               capture_output=True, text=True)
        ok = [l for l in r.stdout.splitlines() if l.startswith("OK:")]
        line = [l for l in r.stdout.splitlines() if l.startswith("serial MIDI in")]
        assert r.returncode == 0 and ok and line, f"{rs}:\n{r.stdout[-2000:]}\n{r.stderr[-3000:]}"
        require_sound(ok[0])
        out = bytes.fromhex(line[0].split("baud:", 2)[2])
        print(f"    {rs}: MIDI out at 9600: {out.hex(' ')}")
        # FE every ~400 k steps from step 760 k; the RQ1 is in at about 1.34 M
        if reply:
            assert reply in out, f"{rs}: no DT1 reply to the RQ1: {out.hex(' ')}"
            assert out[:out.index(reply)] == b"\xfe" and out[out.index(reply) + len(reply):] == b"\xfe" * 4, out.hex(" ")
        else:
            assert out == b"\xfe" * 5, f"{rs}: {out.hex(' ')}"


if __name__ == "__main__":
    run(globals())
