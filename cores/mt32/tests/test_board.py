#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-or-later
"""CI check without ROMs: small 8098 programs that exercise the HDL board's bus decode run on
Nuked-MT32 (ref/build.sh) and on the whole HDL system (sim/system/build.sh); the CPU traces must
be identical. The PCM and reverb ROMs are zero-filled stand-ins.

    python3 tests/test_board.py [PROGRAM ...]      (default: all)

Programs:
  la32_read  reads of LA32 status (0C00) and of 0DC0 right after a write to the other range:
             the read decodes its own address, not the last write's (review 2026-11-26 mt32.md#2)
Bench checks (the system bench must stop with an error instead of passing or hanging):
  hso_0f     HSO command 0F (A/D start without MAME's nested update): exit 4 (mt32.md#7)
  midi_burst 300 MIDI bytes at one sample, more than the board queue holds: exit 6 (mt32.md#12)
"""
import os
import subprocess
import sys
import tempfile
from pathlib import Path

CORE = Path(__file__).resolve().parent.parent
RESET = 0x2080


def ld_sp():
    return [0xa1, 0x00, 0xff, 0x18]                      # ld 18, #ff00


def stb_abs(reg, addr):
    return [0xc7, 0x01, addr & 0xff, addr >> 8, reg]     # stb reg, [0 + addr]


def ldb_abs(reg, addr):
    return [0xb3, 0x01, addr & 0xff, addr >> 8, reg]     # ldb reg, [0 + addr]


def ldb_imm(reg, v):
    return [0xb1, v, reg]


SPIN = [0x27, 0xfe]                                      # sjmp .


def la32_read():
    code = ld_sp() + ldb_imm(0x34, 0x5a)
    code += stb_abs(0x34, 0x0DC0)          # LA32 1C0 (reg_1c0)
    code += ldb_abs(0x36, 0x0C00)          # status: int_status, not FF
    code += stb_abs(0x34, 0x0C00)          # LA32 000 (low data byte)
    code += ldb_abs(0x38, 0x0DC0)          # 1C0-1FF: FF, not int_status
    code += ldb_abs(0x3A, 0x0C00)          # status after a read of the other range
    code += ldb_abs(0x3C, 0x0FFF)
    code += SPIN
    # registers whose final R line must show the reference's value
    return code, {0x36: None, 0x38: 0xff, 0x3a: None, 0x3c: 0xff}


PROGRAMS = {"la32_read": la32_read}


def hso_0f():
    code = ld_sp()
    code += [0xa0, 0x0a, 0x30]                           # ld 30, timer1
    code += [0x65, 0x20, 0x00, 0x30]                     # add 30, #20
    code += ldb_imm(0x06, 0x0F)                          # hso_command = 0F (A/D start)
    code += [0xc4, 0x04, 0x30, 0xc4, 0x05, 0x31]         # stb hso_time lo, hi
    return code + SPIN, None


def midi_burst():
    return ld_sp() + SPIN, "1 " + " ".join(["f8"] * 300) + "\n"


# name -> (program, expected system bench exit code)
EXITS = {"hso_0f": (hso_0f, 4), "midi_burst": (midi_burst, 6)}


def rom_of(code):
    rom = bytearray(0x20000)
    for i in range(9):                                   # vectors -> a RET at 7F00
        rom[0x2000 + 2 * i] = 0x00
        rom[0x2001 + 2 * i] = 0x7f
    rom[0x7f00] = 0xf0
    rom[RESET:RESET + len(code)] = bytes(code)
    return bytes(rom)


def reg_writes(trace):
    w = {}
    for line in Path(trace).read_text().splitlines():
        f = line.split()
        if f and f[0] == "R":
            w[int(f[2], 16)] = int(f[3], 16)
    return w


def run(name, d, env):
    code, expect = PROGRAMS[name]()
    rom = Path(d) / f"{name}.bin"
    rom.write_bytes(rom_of(code))
    args = ["--roms", env["MT32_ROM_DIR"], "--cpu-rom", str(rom), "--samples", "64"]
    tr_r, tr_h = Path(d) / f"{name}.ref.tr", Path(d) / f"{name}.hdl.tr"
    subprocess.run([str(CORE / "build/ref/mt32_ref"), *args, "--trace", str(tr_r)],
                   check=True, stdout=subprocess.DEVNULL)
    subprocess.run([str(CORE / "build/system/mt32_sys_bench"), *args, "--trace", str(tr_h)],
                   check=True, stdout=subprocess.DEVNULL)
    ref = reg_writes(tr_r)
    bad = [f"register {r:02x} not written by the reference" for r in expect if r not in ref]
    bad += [f"reference wrote {ref[r]:02x} to {r:02x}, test expects {v:02x}"
            for r, v in expect.items() if v is not None and r in ref and ref[r] != v]
    if bad:
        return False, "; ".join(bad)
    r = subprocess.run([sys.executable, str(CORE / "tools/trace_cmp.py"), str(tr_r), str(tr_h)],
                       capture_output=True, text=True)
    return r.returncode == 0, r.stdout.strip()


def run_exit(name, d, env):
    prog, want = EXITS[name]
    code, midi = prog()
    rom = Path(d) / f"{name}.bin"
    rom.write_bytes(rom_of(code))
    args = ["--roms", env["MT32_ROM_DIR"], "--cpu-rom", str(rom), "--samples", "64"]
    if midi:
        (Path(d) / f"{name}.txt").write_text(midi)
        args += ["--midi", str(Path(d) / f"{name}.txt")]
    try:
        r = subprocess.run([os.environ.get("SYS_BENCH", str(CORE / "build/system/mt32_sys_bench")), *args], capture_output=True,
                           text=True, timeout=120)
    except subprocess.TimeoutExpired:
        return False, "the bench hung (killed after 120 s)"
    msg = r.stderr.strip().splitlines()[-1] if r.stderr.strip() else ""
    return r.returncode == want, f"exit {r.returncode} (want {want}) {msg}"


def main():
    names = sys.argv[1:] or list(PROGRAMS) + list(EXITS)
    env = dict(os.environ)
    subprocess.run([str(CORE / "ref/build.sh")], check=True, env=env, stdout=subprocess.DEVNULL)
    subprocess.run([str(CORE / "sim/system/build.sh")], check=True, env=env, stdout=subprocess.DEVNULL)
    bad = 0
    with tempfile.TemporaryDirectory() as d:
        (Path(d) / "mt32_pcm.bin").write_bytes(bytes(0x80000))
        (Path(d) / "mt32_reverb.bin").write_bytes(bytes(0x8000))
        env["MT32_ROM_DIR"] = d
        for n in names:
            if n in EXITS:
                ok, msg = run_exit(n, d, env)
                print(f"{n}: {'ok' if ok else 'FAIL'}: {msg}")
            else:
                ok, msg = run(n, d, env)
                print(f"{n}: {'identical' if ok else 'DIFFERENT'}: {msg}")
            bad += not ok
    print("PASS" if not bad else f"FAIL ({bad})")
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
