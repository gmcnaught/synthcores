#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
# synthframe_midi_tx: the serial line decodes back to the bytes pushed, at 31250 and at the
# 9600 fallback; a burst faster than the wire waits in the FIFO; a burst past the FIFO drops
# the bytes that do not fit and counts them. Clock 3.125 MHz (100 clocks a bit at 31250).
import contextlib
import os
import pathlib
import random
import subprocess
import sys

here = pathlib.Path(__file__).resolve().parent
hdl = here.parent.parent / "hdl" / "midi" / "synthframe_midi_tx.sv"
CLK, DEPTH_BITS = 3_125_000, 4          # 16-byte FIFO keeps the overflow case short
DIV, ALT_DIV = CLK // 31250, CLK // 9600


def run(work, name, pushes, alt):
    script = work / f"{name}.txt"
    script.write_text("".join(f"{c} {b:02x}\n" for c, b in pushes))
    r = subprocess.run([str(work / "obj" / "Vsynthframe_midi_tx"), str(script), str(alt), str(ALT_DIV if alt else DIV)],
                       capture_output=True, text=True)
    got = [int(l.split()[1], 16) for l in r.stdout.splitlines() if l.startswith("B ")]
    dropped = int(next(l for l in r.stdout.splitlines() if l.startswith("D ")).split()[1])
    assert r.returncode == 0, f"{name}: framing errors\n{r.stdout[-2000:]}"
    return got, dropped


def lint_vlt():
    """bin/hdl-lint's waiver files"""
    repo = here.parent.parent.parent.parent
    return [str(repo / "modules/synthframe/lint/common.vlt")] + sorted(
        str(p) for p in [*repo.glob("cores/*/cfg/lint.vlt"), *repo.glob("modules/*/cfg/lint.vlt")])


def main():
    random.seed(3)
    # kept between runs (out/ is ignored): Verilator and ccache rebuild only what changed
    w = os.environ.get("MIDI_TX_WORK", str(here.parent.parent / "out" / "tests" / "midi_tx"))
    os.makedirs(w, exist_ok=True)
    with contextlib.nullcontext(w):
        work = pathlib.Path(w)
        # the 32-bit -G value for the longint CLK_HZ is waived in cfg/lint.vlt (bin/hdl-lint)
        b = subprocess.run(["verilator", "--cc", "--exe", "--build", "--build-jobs", os.environ.get("VERILATOR_JOBS", "1"), "-O2", "-Wall", "-Wno-DECLFILENAME",
                            *lint_vlt(),
                            "--Mdir", str(work / "obj"), str(hdl), str(here / "tb_midi_tx.cpp"),
                            "--top-module", "synthframe_midi_tx", f"-GCLK_HZ={CLK}", f"-GDEPTH_BITS={DEPTH_BITS}"],
                           capture_output=True, text=True)
        if b.returncode:
            print(b.stdout[-3000:] + b.stderr[-3000:])
            sys.exit(1)
        fails = 0
        # spaced bytes (one every 1.5 byte times), all 256 values
        vals = list(range(256))
        got, d = run(work, "spaced", [(i * DIV * 15, b) for i, b in enumerate(vals)], 0)
        fails += check("spaced 31250", got, vals, d, 0)
        # the 9600 fallback
        vals = [random.randrange(256) for _ in range(40)]
        got, d = run(work, "alt", [(i * ALT_DIV * 15, b) for i, b in enumerate(vals)], 1)
        fails += check("spaced 9600", got, vals, d, 0)
        # a burst on consecutive clocks that fits the FIFO (16 + the byte already on the wire)
        vals = [random.randrange(256) for _ in range(16)]
        got, d = run(work, "burst", [(i, b) for i, b in enumerate(vals)], 0)
        fails += check("burst 16", got, vals, d, 0)
        # SC-55 pace: a byte every 0.4 byte times (faster than the wire), 12 bytes
        vals = [random.randrange(256) for _ in range(12)]
        got, d = run(work, "fast", [(i * DIV * 4, b) for i, b in enumerate(vals)], 0)
        fails += check("faster than the wire", got, vals, d, 0)
        # overflow: 40 bytes on consecutive clocks; the first byte leaves the FIFO 2 clocks after
        # it arrives, so 17 fit (16 queued + 1 shifting) and 23 are dropped
        vals = [random.randrange(256) for _ in range(40)]
        got, d = run(work, "overflow", [(i, b) for i, b in enumerate(vals)], 0)
        fails += check("overflow", got, vals[:17], d, 23)
    sys.exit(1 if fails else 0)


def check(name, got, want, dropped, want_dropped):
    if got == want and dropped == want_dropped:
        print(f"{name}: {len(got)} bytes decoded, {dropped} dropped")
        return 0
    print(f"{name}: FAIL: got {len(got)} bytes {bytes(got[:24]).hex()}, want {len(want)} {bytes(want[:24]).hex()}; "
          f"dropped {dropped}, want {want_dropped}")
    return 1


if __name__ == "__main__":
    main()
