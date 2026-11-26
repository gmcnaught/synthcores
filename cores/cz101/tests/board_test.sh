#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Board checks that need no CZ-101 firmware: short uPD7811 programs run on cz101_bench.
#   tests/board_test.sh            (cz101_bench from out/sim, or CZ101_BENCH=path)
#   loop       JR $ from reset: the uPD933 makes one (silent) sample per 250 ticks of CPU time.
#   wrap       the same with the tick time started 10,000 cycles before the 40-bit tick counter
#              wraps (CZ_TICK_BASE; on the board after about 30.5 h): samples continue.
#   power      PC3 = 0 (firmware power-down) with the power switch on: the CPU is held in reset,
#              the LCD is dark (MAME m_power = 0) and the uPD933 keeps making samples.
# Prints one line per case and "board: N passed"; exits 1 on any failure.
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BENCH="${CZ101_BENCH:-$HERE/out/sim/cz101_bench}"
W="${BOARD_WORK:-$HERE/out/board}"
[ -x "$BENCH" ] || { echo "board: FAIL: no $BENCH (build cores/cz101/sim)"; exit 1; }
rm -rf "$W"; mkdir -p "$W"
python3 - "$BENCH" "$W" <<'PY'
import os, re, subprocess, sys
bench, w = sys.argv[1:3]
CLOCKS = 300000

def prog(name, code):
    d = os.path.join(w, name)
    os.makedirs(d, exist_ok=True)
    img = bytearray(0x8000)
    img[:len(code)] = bytes(code)
    open(os.path.join(d, "hn613256ps40.bin"), "wb").write(img)
    open(os.path.join(d, "init_ram.bin"), "wb").write(b"\xff" * 0x1000)
    return d

def run(name, rom, **env):
    e = dict(os.environ, CZ_CLOCKS=str(CLOCKS), **{k: str(v) for k, v in env.items()})
    p = subprocess.run([bench, rom, "0", os.devnull], env=e, capture_output=True, text=True)
    out = p.stdout + p.stderr
    if p.returncode != 0:
        return None, f"bench exit {p.returncode}: {out.strip()[-300:]}"
    m = re.search(r"(\d+) instructions, \d+ cycles, (\d+) samples", out)
    b = re.search(r"board: cycles (\d+), powered_down (\d) at (-?\d+), samples after power-down (\d+), "
                  r"lcd_power (\d), uPD933 IRQ rises (\d+) \(first at (-?\d+)\), timer armed (\d+) \(last n (\d+)\)", out)
    if not (m and b):
        return None, "no result line: " + out.strip()[-300:]
    r = dict(insns=int(m[1]), samples=int(m[2]), cycles=int(b[1]), down=int(b[2]), down_at=int(b[3]),
             down_samples=int(b[4]), lcd_power=int(b[5]), irq_rises=int(b[6]), irq_first=int(b[7]),
             arms=int(b[8]), arm_n=int(b[9]))
    return r, ""

def near(got, want, tol=3):
    return abs(got - want) <= tol

JR_SELF = [0xFF]                                   # JR $
MVI_A = lambda v: [0x69, v]
MOV_MC_A, MOV_PC_A = [0x4D, 0xD4], [0x4D, 0xC2]

results = []
def check(name, ok, detail):
    results.append(ok)
    print(f"{'PASS' if ok else 'FAIL'} {name}: {detail}")

# loop / wrap
rom = prog("loop", JR_SELF)
for name, base in (("loop", 0), ("wrap", (1 << 40) - 30000)):
    r, err = run(name, rom, **({"CZ_TICK_BASE": base} if base else {}))
    if r is None:
        check(name, False, err); continue
    want = r["cycles"] * 3 // 250 + 1          # samples at ticks 0, 250, ... up to ticks(cycles)
    check(name, r["cycles"] > CLOCKS // 10 and near(r["samples"], want),
          f"{r['samples']} samples in {r['cycles']} cycles (expected {want})")

# power
rom = prog("power", MVI_A(0x00) + MOV_MC_A + MOV_PC_A + JR_SELF)
r, err = run("power", rom)
if r is None:
    check("power", False, err)
else:
    span = r["cycles"] - r["down_at"] if r["down"] else 0
    want = span * 3 // 250
    check("power", r["down"] == 1 and r["lcd_power"] == 0 and span > CLOCKS // 10
          and near(r["down_samples"], want),
          f"powered down at cycle {r['down_at']}, lcd_power {r['lcd_power']}, {r['down_samples']} samples "
          f"in the {span} cycles after (expected {want})")

npass = sum(results)
print(f"board: {npass} passed, {len(results) - npass} failed")
sys.exit(0 if all(results) else 1)
PY
