#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-or-later
"""CI check without ROMs: random MCS-96 programs (tools/mcs96_randprog.py) give the same trace on
Nuked-MT32's CPU (ref/build.sh) and on the HDL 8098 in the co-simulation (sim/cpu/build.sh).
The PCM and reverb ROMs are zero-filled stand-ins: the programs never use the LA32 or reverb.

    python3 tests/test_cpu.py [SEEDS]      (default 4: two plain, two with SFRs and interrupts)
"""
import os
import subprocess
import sys
import tempfile
from pathlib import Path

CORE = Path(__file__).resolve().parent.parent


def main():
    n = int(sys.argv[1]) if len(sys.argv) > 1 else 4
    env = dict(os.environ)
    subprocess.run([str(CORE / "ref/build.sh")], check=True, env=env)
    subprocess.run([str(CORE / "sim/cpu/build.sh")], check=True, env=env)
    with tempfile.TemporaryDirectory() as d:
        (Path(d) / "mt32_pcm.bin").write_bytes(bytes(0x80000))
        (Path(d) / "mt32_reverb.bin").write_bytes(bytes(0x8000))
        env["MT32_ROM_DIR"] = d
        bad = 0
        for i in range(n):
            sfr = i >= n // 2
            seed = 1000 + i
            args = [str(CORE / "tools/c1_run.sh"), str(seed), "1600"] + (["--sfr"] if sfr else [])
            r = subprocess.run(args, env=env, capture_output=True, text=True)
            print(r.stdout.strip())
            bad += r.returncode != 0
    print("PASS" if not bad else f"FAIL ({bad})")
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
