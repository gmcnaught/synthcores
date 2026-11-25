#!/usr/bin/env python3
"""MCU system HDL (CPU + memories + peripherals; the mk2's sub-MCU) in lockstep with the reference."""
import os
import pathlib
import subprocess
import tempfile

from testlib import REF_BUILD, ROM_DIR, ROOT, need_file, need_roms, run

SIM_BUILD = pathlib.Path(os.environ.get("SC55_SIM_BUILD", ROOT / "out" / "sim"))


def bench(*args):
    exe = need_file(SIM_BUILD / "mcu_lockstep", "MCU lockstep bench")
    r = subprocess.run([str(exe), "--roms", str(ROM_DIR), *args], capture_output=True, text=True)
    ok = [l for l in r.stdout.splitlines() if l.startswith("OK")]
    assert r.returncode == 0 and ok, f"{args}:\n{r.stdout[-2000:]}\n{r.stderr[-3000:]}"
    print("   ", " ".join(args), "->", ok[0][:120])


def midi_out(*args):
    """Run the bench (which fails unless the HDL's MIDI out bytes and cycles equal the model's) and
    return the model's MIDI out bytes."""
    with tempfile.TemporaryDirectory() as d:
        log = pathlib.Path(d) / "tx.txt"
        bench(*args, "--tx-log", str(log))
        return bytes(int(l.split()[2], 16) for l in log.read_text().splitlines())


def roland_messages(data):
    """The SysEx messages in a byte stream, each checked for a valid Roland checksum."""
    msgs = []
    while (i := data.find(0xF0)) >= 0:
        j = data.index(0xF7, i)
        m = data[i:j + 1]
        assert m[1] == 0x41 and m[4] == 0x12, m.hex(" ")
        assert sum(m[5:-1]) % 128 == 0, f"checksum: {m.hex(' ')}"
        msgs.append(m)
        data = data[j + 1:]
    return msgs


def test_boot():
    need_roms()
    bench("--steps", "6000000")


def test_midi_and_buttons():
    need_roms()
    bench("--midi-demo", "5", "--buttons", "9", "--steps", "12000000")


def test_mk2_midi_and_buttons():
    need_roms("mk2-v1.01")
    bench("--romset", "mk2-v1.01", "--midi-demo", "5", "--buttons", "9", "--steps", "12000000")


def test_sc155_midi_and_buttons():
    need_roms("sc155-rev1")
    bench("--romset", "sc155-rev1", "--midi-demo", "2", "--buttons", "4", "--steps", "6000000")



# MIDI out (MIDI-OUT-PLAN.md). Requests at Roland's default device ID 0x10.
GS_RQ1_MASTER_TUNE = "f0411042114000000000043cf7"           # 40 00 00, 4 bytes
JV_RQ1_SYSTEM = "f041104611000000000000001070f7"            # 00 00 00 00, 16 bytes


def test_mk1_midi_out_active_sensing():
    need_roms()
    out = midi_out("--steps", "2000000")
    assert len(out) >= 4 and set(out) == {0xFE}, out.hex(" ")


def test_mk2_midi_out_rq1():
    need_roms("mk2-v1.01")
    out = midi_out("--romset", "mk2-v1.01", "--steps", "8000000", "--midi-at", f"6000000:{GS_RQ1_MASTER_TUNE}")
    msgs = roland_messages(bytes(b for b in out if b != 0xFE))
    assert len(msgs) == 1 and msgs[0][:8] == bytes.fromhex("f041104212400000") and len(msgs[0]) == 14, msgs


def test_jv880_midi_out_rq1():
    need_roms("jv880-v1.0.1")
    nvram_exe = need_file(REF_BUILD / "jv880-nvram", "jv880-nvram")
    with tempfile.TemporaryDirectory() as d:
        nv = pathlib.Path(d) / "factory.nvram"
        subprocess.run([str(nvram_exe), str(ROM_DIR), str(nv)], check=True, capture_output=True)
        out = midi_out("--romset", "jv880-v1.0.1", "--nvram", str(nv), "--steps", "12000000",
                       "--midi-at", f"9000000:{JV_RQ1_SYSTEM}")
    msgs = roland_messages(out)
    assert len(msgs) == 1 and msgs[0][:9] == bytes.fromhex("f04110461200000000") and len(msgs[0]) == 75, msgs


if __name__ == "__main__":
    run(globals())
