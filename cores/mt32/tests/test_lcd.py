#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-or-later
"""CI check without ROMs: the LCD against Nuked-MT32's lcd.cpp.

An 8098 program (no firmware) sends LCD commands and data: CG RAM characters, text in both
halves, the cursor with and without blink and with the cursor font, 1- and 2-line mode, display
off, and command 10h (lcd_reset(), which clears the CG RAM).
  1. state: after every command the HDL system bench (sim/system) and ref/mt32_ref log the text,
     CG RAM, cursor and mode bits (--lcd-state-log); the logs must be identical.
  2. dots: the reference's state log is drawn by hdl/mt32_lcd_video.sv (sim/lcd) and compared
     dot for dot with what lcd_render() draws (ref/mt32_ref --lcd-dots).

    python3 tests/test_lcd.py
"""
import os
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from test_board import CORE, SPIN, ld_sp, ldb_imm, rom_of, stb_abs  # noqa: E402


def cmd(v):
    return ldb_imm(0x30, v) + stb_abs(0x30, 0x0380)


def data(*vs):
    out = []
    for v in vs:
        out += ldb_imm(0x30, v) + stb_abs(0x30, 0x0300)
    return out


def program():
    c = ld_sp()
    c += cmd(0x13) + cmd(0x0D)                                   # 2 lines, display on
    c += cmd(0x20) + sum((cmd(0x40 | r) for r in [0x1f, 0x11, 0x0a, 0x04, 0x0a, 0x11, 0x1f, 0x15]), [])
    c += cmd(0x21) + sum((cmd(0x40 | r) for r in [0x04, 0x0e, 0x1f, 0x0e, 0x04, 0x00, 0x1b, 0x1f]), [])
    c += data(0x41, 0x42, 0x00, 0x01, 0xa5) + cmd(0x80)          # text at 0
    c += data(0x78, 0x01, 0x7e) + cmd(0xC5)                      # second half, position 15
    c += data(0xff, 0x7f, 0x05, 0x20) + cmd(0x87)                # codes outside the font
    c += data(0x31, 0x32) + cmd(0x8A)                            # position 10: ignored
    c += cmd(0x83) + cmd(0x0F)                                   # cursor at 3, on, no blink
    c += cmd(0x0B)                                               # blink: the cursor shows
    c += cmd(0x09)                                               # cursor font: the cell is blank
    c += cmd(0x08) + cmd(0x82)                                   # cursor on CG character 0
    c += cmd(0x07) + cmd(0x07) + cmd(0x06)                       # cursor moves
    c += cmd(0x05) + data(0x51, 0x52) + cmd(0x89) + cmd(0x04)    # decrementing writes
    c += cmd(0x0E)                                               # cursor off
    c += cmd(0x12)                                               # 1 line: background only
    c += cmd(0x13)
    c += cmd(0x10)                                               # lcd_reset(): CG RAM cleared
    c += cmd(0x13) + cmd(0x0D) + cmd(0x0F) + cmd(0x0B)           # CG characters now blank
    c += cmd(0x0C)                                               # display off
    c += SPIN
    return c


def main():
    env = dict(os.environ)
    for b in ("ref/build.sh", "sim/system/build.sh", "sim/lcd/build.sh"):
        subprocess.run([str(CORE / b)], check=True, env=env, stdout=subprocess.DEVNULL)
    bad = []
    with tempfile.TemporaryDirectory() as d:
        d = Path(d)
        (d / "mt32_pcm.bin").write_bytes(bytes(0x80000))
        (d / "mt32_reverb.bin").write_bytes(bytes(0x8000))
        (d / "lcd.bin").write_bytes(rom_of(program()))
        args = ["--roms", str(d), "--cpu-rom", str(d / "lcd.bin"), "--samples", "128"]
        subprocess.run([str(CORE / "build/ref/mt32_ref"), *args, "--lcd-state-log", str(d / "ref.st"),
                        "--lcd-dots", str(d / "ref.dots")], check=True, stdout=subprocess.DEVNULL)
        subprocess.run([str(CORE / "build/system/mt32_sys_bench"), *args, "--lcd-state-log", str(d / "hdl.st")],
                       check=True, stdout=subprocess.DEVNULL)
        with open(d / "hdl.dots", "w") as f:
            subprocess.run([str(CORE / "build/lcd/lcd_bench"), str(d / "ref.st")], check=True, stdout=f,
                           stderr=subprocess.DEVNULL)
        n = len((d / "ref.st").read_text().splitlines())
        if n != 45:
            bad.append(f"the reference logged {n} LCD commands, expected 45")
        for what, a, b in (("state", "ref.st", "hdl.st"), ("dots", "ref.dots", "hdl.dots")):
            r = subprocess.run([sys.executable, str(CORE / "tools/trace_cmp.py"), str(d / a), str(d / b)],
                               capture_output=True, text=True)
            print(f"{what}: {r.stdout.strip()}")
            if r.returncode:
                bad.append(what)
    print("PASS" if not bad else f"FAIL ({', '.join(bad)})")
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
