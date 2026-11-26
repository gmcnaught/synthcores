#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Board checks that need no CZ-101 firmware: short uPD7811 programs run on cz101_bench.
#   tests/board_test.sh            (cz101_bench from out/sim, or CZ101_BENCH=path)
#   loop       JR $ from reset: the uPD933 makes one (silent) sample per 250 ticks of CPU time.
#   wrap       the same with the tick time started 10,000 cycles before the 40-bit tick counter
#              wraps (CZ_TICK_BASE; on the board after about 30.5 h): samples continue.
#   power      PC3 = 0 (firmware power-down) with the power switch on: the CPU is held in reset,
#              the LCD is dark (MAME m_power = 0) and the uPD933 keeps making samples.
#   irqtimer   a DCA step with direction and target on the wrong side (MAME's remaining time
#              wraps: 390,427,742 samples, 3.25e10 CPU cycles), then /CS rises: the IRQ timer is
#              more than 2^31 cycles ahead and must not fire during the run.
#   nmi        timer 0 interrupts every 20 cycles while the Power switch toggles every 13 cycles:
#              /NMI reaches the CPU between instructions, so no interrupt's vector changes while
#              the CPU pushes PSW and PC for it (MAME picks the vector once, before the pushes).
#   serial     three MIDI bytes queued for the live-MIDI serializer: each byte (start, 8 data,
#              stop) takes 1,280 SCK edges, back to back.
# Prints one line per case and "board: N passed"; exits 1 on any failure.
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
PY="$(command -v pypy3 || command -v python3)"   # comparison scripts run under pypy3 when present
BENCH="${CZ101_BENCH:-$HERE/out/sim/cz101_bench}"
W="${BOARD_WORK:-$HERE/out/board}"
[ -x "$BENCH" ] || { echo "board: FAIL: no $BENCH (build cores/cz101/sim)"; exit 1; }
rm -rf "$W"; mkdir -p "$W"
"$PY" - "$BENCH" "$W" <<'PY'
import os, re, subprocess, sys
bench, w = sys.argv[1:3]
CLOCKS = 300000

def prog(name, code):
    d = os.path.join(w, name)
    os.makedirs(d, exist_ok=True)
    img = bytearray(0x8000)
    img[:len(code)] = bytes(code)
    with open(os.path.join(d, "hn613256ps40.bin"), "wb") as f:
        f.write(img)
    with open(os.path.join(d, "init_ram.bin"), "wb") as f:
        f.write(b"\xff" * 0x1000)
    return d

def run(name, rom, **env):
    e = dict(os.environ, CZ_CLOCKS=str(CLOCKS), **{k: str(v) for k, v in env.items()})
    p = subprocess.run([bench, rom, "0", os.devnull], env=e, capture_output=True, text=True)
    out = p.stdout + p.stderr
    if p.returncode != 0:
        return None, f"bench exit {p.returncode}: {out.strip()[-300:]}"
    c = re.search(r"cpu: interrupts (\d+), NMI (\d+), vector changed during the push (\d+)", out)
    m = re.search(r"(\d+) instructions, \d+ cycles, (\d+) samples", out)
    b = re.search(r"board: cycles (\d+), powered_down (\d) at (-?\d+), samples after power-down (\d+), "
                  r"lcd_power (\d), uPD933 IRQ rises (\d+) \(first at (-?\d+)\), timer armed (\d+) \(last n (\d+)\)", out)
    if not (m and b):
        return None, "no result line: " + out.strip()[-300:]
    r = dict(insns=int(m[1]), samples=int(m[2]), cycles=int(b[1]), down=int(b[2]), down_at=int(b[3]),
             down_samples=int(b[4]), lcd_power=int(b[5]), irq_rises=int(b[6]), irq_first=int(b[7]),
             arms=int(b[8]), arm_n=int(b[9]), irqs=int(c[1]) if c else -1, nmis=int(c[2]) if c else -1,
             vec_changes=int(c[3]) if c else -1)
    return r, ""

def near(got, want, tol=3):
    return abs(got - want) <= tol

JR_SELF = [0xFF]                                   # JR $
MVI_A = lambda v: [0x69, v]
MOV_MB_A, MOV_MC_A, MOV_PB_A, MOV_PC_A = [0x4D, 0xD3], [0x4D, 0xD4], [0x4D, 0xC1], [0x4D, 0xC2]
STA = lambda a: [0x70, 0x79, a & 0xFF, a >> 8]       # MOV (word),A

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

# irqtimer: DCA voice 0 step 0x8301 (direction down, rate code 3 = 11, target 1 << 18; current 0)
N_EXPECT = 390427742
rom = prog("irqtimer", MVI_A(0x00) + MOV_MB_A + MOV_PB_A                 # port B out, /CS low
           + MVI_A(0x00) + STA(0xC000) + MVI_A(0x83) + STA(0xC000) + MVI_A(0x01) + STA(0xC000)
           + MVI_A(0x20) + MOV_PB_A + JR_SELF)                          # /CS high: arm the timer
r, err = run("irqtimer", rom)
if r is None:
    check("irqtimer", False, err)
else:
    check("irqtimer", r["arms"] >= 1 and r["arm_n"] == N_EXPECT and r["irq_rises"] == 0,
          f"timer armed {r['arms']} time(s), n {r['arm_n']} (expected {N_EXPECT}); "
          f"uPD933 IRQ rises {r['irq_rises']} in {r['cycles']} cycles (expected 0)")

# nmi: vectors 0004 (NMI), 0008 (T0) and the rest RETI; T0 every 5 x 4 cycles, EI, JR $
code = [0x54, 0x00, 0x01] + [0x00] * (0x100 - 3)       # JMP 0100
for v in (0x04, 0x08, 0x10, 0x18, 0x20, 0x28):
    code[v] = 0x62                                     # RETI
code += MVI_A(0x05) + [0x4D, 0xDA]                     # MOV TM0,A
code += MVI_A(0x00) + [0x4D, 0xC7]                     # MOV MKL,A: all unmasked
code += MVI_A(0x80) + [0x4D, 0xCD]                     # MOV TMM,A: T0 counts cycles / 4, T1 stopped
code += [0xAA] + JR_SELF                               # EI; JR $
rom = prog("nmi", code)
with open(os.path.join(w, "nmi.power"), "w") as f:
    for k in range(1, 15000):
        f.write(f"{100 + 13 * k} {k & 1 ^ 1}\n")
r, err = run("nmi", rom, CZ_POWER=os.path.join(w, "nmi.power"))
if r is None:
    check("nmi", False, err)
else:
    check("nmi", r["nmis"] > 100 and r["irqs"] > 2 * r["nmis"] and r["vec_changes"] == 0,
          f"{r['irqs']} interrupts, {r['nmis']} NMI, {r['vec_changes']} vector changes during the push (expected 0)")

# serial: 00 55 00 offered at cycle 0; the line falls at each start bit
rom = prog("serial", JR_SELF)
with open(os.path.join(w, "serial.bytes"), "w") as f:
    f.write("0 00\n0 55\n0 00\n")
log = os.path.join(w, "serial.rxline")
r, err = run("serial", rom, CZ_MIDI_BYTES=os.path.join(w, "serial.bytes"), CZ_RX_LINE_LOG=log)
if r is None:
    check("serial", False, err)
else:
    with open(log) as f:
        ch = [tuple(map(int, l.split())) for l in f]
    falls = [k for k, lv in ch if lv == 0]
    # start bits: byte 0 and 2 have one fall each; 0x55 = 1,0,1,0,... LSB first falls 4 more times
    starts = [falls[0], falls[1], falls[6]] if len(falls) == 7 else []
    gaps = [b - a for a, b in zip(starts, starts[1:])]
    check("serial", gaps == [1280, 1280], f"{len(falls)} falls, start-bit spacing {gaps} SCK edges (expected [1280, 1280])")

npass = sum(results)
print(f"board: {npass} passed, {len(results) - npass} failed")
sys.exit(0 if all(results) else 1)
PY
