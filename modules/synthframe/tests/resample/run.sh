#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# synthframe_resample against gen_resample.py's integer model: an impulse, full-scale square
# waves (saturation), a sine sweep and random samples, at 40 kHz input (1250 clocks at 50 MHz)
# and in bursts of 6 inputs on consecutive clocks (through the input queue).
#   tests/resample/run.sh [L M TAPS HEX]     default: 12 5 80 hdl/audio/synthframe_resample_40k_96k.hex
set -euo pipefail
cd "$(dirname "$0")/../.."
L=${1:-12}; M=${2:-5}; TAPS=${3:-80}; HEX=${4:-hdl/audio/synthframe_resample_40k_96k.hex}
W=${RESAMPLE_WORK:-out/tests/resample-$L-$M}; mkdir -p "$W/obj"   # kept: rebuilds are incremental
# warnings are waived in cfg/lint.vlt (bin/hdl-lint), not by -Wno-* options
verilator --cc --exe --build --build-jobs "${VERILATOR_JOBS:-1}" -O2 --Mdir "$W/obj" \
    lint/common.vlt ../../cores/*/cfg/lint.vlt ../*/cfg/lint.vlt \
    hdl/audio/synthframe_resample.sv tests/resample/tb.cpp --top-module synthframe_resample \
    -GL=$L -GM=$M -GTAPS=$TAPS "-GCOEF_HEX=\"$PWD/$HEX\"" >/dev/null
"$(command -v pypy3 || command -v python3)" - "$W/in.txt" <<'PY'
import math, random, sys
random.seed(1)
v = [(32767, -32768)] + [(0, 0)] * 200
v += [((32767 if (i // 20) % 2 else -32768), (32767 if (i // 7) % 2 else -32768)) for i in range(400)]
v += [(int(30000 * math.sin(2 * math.pi * (100 + 19000 * i / 3000) * i / 40000)), int(20000 * math.cos(0.3 * i))) for i in range(3000)]
v += [(random.randint(-32768, 32767), random.randint(-32768, 32767)) for _ in range(2000)]
open(sys.argv[1], "w").write("".join(f"{l} {r}\n" for l, r in v))
PY
for gap in 1250 0; do
    "$W/obj/Vsynthframe_resample" "$W/in.txt" "$W/out_$gap.txt" $gap
    pypy3 - "$HEX" "$L" "$M" "$TAPS" "$W/in.txt" "$W/out_$gap.txt" "$gap" <<'PY'
import sys
sys.path.insert(0, "tools")
from gen_resample import model
hexf, L, M, T, fin, fout, gap = sys.argv[1], *map(int, sys.argv[2:5]), sys.argv[5], sys.argv[6], sys.argv[7]
h = [(int(x, 16) ^ 0x20000) - 0x20000 for x in open(hexf).read().split()]
xs = [tuple(map(int, l.split())) for l in open(fin)]
want = list(zip(model(h, L, M, T, [x[0] for x in xs]), model(h, L, M, T, [x[1] for x in xs])))
got = [tuple(map(int, l.split())) for l in open(fout)]
# at least the outputs that L/M of the inputs make, less the filter's start-up
need = len(xs) * L // M - 2 * T
if len(want) < need or not got:
    print(f"gap {gap}: FAIL: {len(got)} outputs from the HDL, {len(want)} from the model, need {need}")
    sys.exit(1)
bad = next((i for i in range(max(len(want), len(got))) if i >= len(want) or i >= len(got) or want[i] != got[i]), None)
if bad is not None:
    print(f"gap {gap}: FAIL at output {bad} of {len(want)}: model {want[bad] if bad < len(want) else None} HDL {got[bad] if bad < len(got) else None}")
    sys.exit(1)
print(f"gap {gap}: {len(got)} outputs identical to the model ({len(xs)} inputs)")
PY
done
