#!/usr/bin/env python3
"""Proteus/1 sound subsystem HDL against tools/proteus_model.py on random stimuli (no ROMs).

Builds sim/snd_tb.cpp (snd_ctrl: the core's write FIFO and sequencer, synthframe_mem_arb /
synthframe_mem_ddr3 and proteus_snd), makes random demo images with tools/snd_randimg.py
(random sample data; all 32 voices with random start, pitch, tone, loops, envelopes, pan and
byte-lane writes) and checks that the bench output equals the model's bit for bit. The bench
itself also checks the DDR3 capture ring, missed sample deadlines and the status words
(sample count, reset epoch, peak level). A second build with a 128-clock sample period checks
that the missed-deadline counter saturates and the epoch counts a reset (--status-test).

The model runs under pypy3 (CLAUDE.md); without pypy3 it falls back to python3 (slower).
--inject-error runs the first case only, with one HDL output bit flipped (snd_tb --inject-error):
the comparator self-check, which must fail (tests/ci.sh).
Exit status: 0 all passed, 1 a failure, 77 skipped (verilator missing).
"""
import pathlib
import os
import shutil
import subprocess
import sys
import tempfile

CORE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SAMPLES = 5000
# (seed, generator options, bench options)
CASES = [
    (1, [], []),
    (2, [], []),
    (5, ["--blocks", "10"], ["--no-tone"]),
    (3, ["--edge"], []),        # windows that wrap past word 0x3FFFFF (review 2026-11-26 #6)
]


def main():
    if not shutil.which("verilator"):
        print("SKIP: verilator not installed")
        return 77
    py = "pypy3" if shutil.which("pypy3") else sys.executable
    tmp = tempfile.mkdtemp(prefix="proteus_snd_")
    try:
        tb = subprocess.run([os.path.join(CORE, "sim", "build.sh"), "snd", os.path.join(CORE, "build", "sim", "snd")],
                            check=True, stdout=subprocess.PIPE, text=True).stdout.strip().splitlines()[-1]
        fails = 0
        inject = "--inject-error" in sys.argv[1:]
        for seed, gen, bench in (CASES[:1] if inject else CASES):
            bench = bench + (["--inject-error"] if inject else [])
            img = os.path.join(tmp, f"rand{seed}.bin")
            hdl, ref = os.path.join(tmp, f"hdl{seed}.raw"), os.path.join(tmp, f"model{seed}.raw")
            subprocess.run([py, os.path.join(CORE, "tools", "snd_randimg.py"), img, str(seed)] + gen,
                           check=True, stdout=subprocess.DEVNULL)
            r = subprocess.run([tb, img, hdl, str(SAMPLES), str(seed)] + bench, cwd=tmp,
                               stderr=subprocess.PIPE, text=True)
            model_opts = ["--no-tone"] if "--no-tone" in bench else []
            subprocess.run([py, os.path.join(CORE, "tools", "proteus_model.py"), img, ref,
                            "--ticks", str(SAMPLES)] + model_opts, check=True, stdout=subprocess.DEVNULL)
            a = pathlib.Path(hdl).read_bytes() if os.path.exists(hdl) else b""
            b = pathlib.Path(ref).read_bytes()
            name = f"seed {seed} {' '.join(gen + bench)}".strip()
            if r.returncode != 0:
                print(f"FAIL {name}: bench exit {r.returncode}\n{r.stderr}")
                fails += 1
            elif len(b) != 4 * SAMPLES or len(a) != len(b):
                print(f"FAIL {name}: {len(a) // 4} bench samples, {len(b) // 4} model samples, want {SAMPLES}")
                fails += 1
            elif a != b:
                k = next(i for i in range(0, len(a), 4) if a[i:i + 4] != b[i:i + 4]) // 4
                print(f"FAIL {name}: output differs from the model from sample {k}")
                fails += 1
            else:
                print(f"ok   {name}: {SAMPLES} samples bit-identical")
        if inject:
            return 1 if fails else 0
        # status words under overload: SAMPLE_DIV=128 misses most ticks (review #16)
        env = dict(os.environ, VFLAGS="-GSAMPLE_DIV=128")
        tb2 = subprocess.run([os.path.join(CORE, "sim", "build.sh"), "snd", os.path.join(CORE, "build", "sim", "snd128")],
                             check=True, stdout=subprocess.PIPE, text=True, env=env).stdout.strip().splitlines()[-1]
        img = os.path.join(tmp, f"rand{CASES[0][0]}.bin")
        r = subprocess.run([tb2, img, os.path.join(tmp, "st.raw"), "24576", "1", "--status-test"], cwd=tmp,
                           stderr=subprocess.PIPE, text=True)
        if r.returncode != 0:
            print(f"FAIL status words under overload:\n{r.stderr}")
            fails += 1
        else:
            print("ok   status words: missed deadlines saturate, reset epoch counts")
        print(f"test_snd_model: {len(CASES) + 1 - fails} passed, {fails} failed")
        return 1 if fails else 0
    finally:
        shutil.rmtree(tmp, ignore_errors=True)


if __name__ == "__main__":
    sys.exit(main())
