#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# U1: a random H8/300H program (tools/h8h_randprog.py), HDL trace against MAME's.
# Usage: tools/u1_run.sh SEED [COUNT] [randprog options...]
# Needs ../mame-mu50 (mame/patches/) and the bench (cmake -S sim -B out/sim; ninja -C out/sim).
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
ROOT="$(cd "$HERE/../.." && pwd)"
MAME_DIR="${MAME_DIR:-$ROOT/../mame-mu50}"
SEED=$1; COUNT=${2:-2000}; shift; shift || true
W="${U1_WORK:-$HERE/out/u1}/$SEED"; rm -rf "$W"; mkdir -p "$W"
python3 "$HERE/tools/h8h_randprog.py" "$MAME_DIR" "$SEED" "$W/prog.bin" --count "$COUNT" "$@"
python3 "$HERE/tools/mu50_testrom.py" "$W/prog.bin" "$W/roms"
# instruction lines to compare: generous, MAME stops a little after its limit
N=$((COUNT * 4 + 100))
MU_TRACE="$W/mame.trace" MU_TRACE_LIMIT=$N "$HERE/tools/mame-mu50.sh" "$W/roms" "$W/mame" -seconds_to_run 5 >/dev/null 2>&1 || true
"$HERE/out/sim/h8h_bench" "$W/prog.bin" "$N" "$W/hdl.trace"
# both up to the end of the program (the BRA $ loop) or N lines
for t in mame hdl; do
    awk -v n="$N" '/^W/{buf = buf $0 "\n"; next} {printf "%s", buf; buf = ""; print; if (++k == n) exit}' \
        "$W/$t.trace" > "$W/$t.cmp"
done
if cmp -s "$W/mame.cmp" "$W/hdl.cmp"; then
    echo "seed $SEED: PASS $(grep -vc '^W' "$W/hdl.cmp") lines"
else
    echo "seed $SEED: FAIL"; diff "$W/mame.cmp" "$W/hdl.cmp" | head -"${U1_DIFF:-12}"; exit 1
fi
