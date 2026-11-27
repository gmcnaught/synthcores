#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# U1 check (local, needs ../mame-mu50): the generated CPU is up to date with MAME's h8.lst and
# h8.cpp, and random programs (tools/u1_run.sh) give HDL traces identical to MAME's.
# Usage: tests/u1_check.sh [FIRST_SEED [NSEEDS [COUNT]]]    (defaults 1 50 2000)
# Exits 77 (skip) without the MAME build.
set -euo pipefail
HERE=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
ROOT=$(cd "$HERE/../.." && pwd)
MAME_DIR="${MAME_DIR:-$ROOT/../mame-mu50}"
FIRST=${1:-1}; NSEEDS=${2:-50}; COUNT=${3:-2000}
[ -x "$MAME_DIR/mu50" ] || { echo "u1_check: SKIP (no $MAME_DIR/mu50)"; exit 77; }

# 1. generated files current
T=$(mktemp -d); trap 'rm -rf "$T"' EXIT
python3 "$HERE/tools/h8h_gen.py" "$MAME_DIR" "$T" >/dev/null
for f in h8h_gen.svh h8h_gen_states.svh h8h_gen.txt; do
    cmp -s "$T/$f" "$HERE/hdl/h8h/$f" || { echo "u1_check: FAIL: hdl/h8h/$f is not what h8h_gen.py makes"; exit 1; }
done

# 2. bench, random programs, coverage
cmake -Wno-dev -S "$HERE/sim" -B "$HERE/out/sim" -G Ninja -DCMAKE_BUILD_TYPE=Release >/dev/null
ninja -C "$HERE/out/sim" h8h_bench >/dev/null
seq "$FIRST" $((FIRST + NSEEDS - 1)) | xargs -P "${U1_JOBS:-8}" -I{} "$HERE/tools/u1_run.sh" {} "$COUNT" > "$T/runs.txt" 2>&1 || true
fails=$(grep -c FAIL "$T/runs.txt" || true)
passes=$(grep -c PASS "$T/runs.txt" || true)
python3 "$HERE/tools/u1_coverage.py" "$MAME_DIR" $(seq -f "$HERE/out/u1/%g" "$FIRST" $((FIRST + NSEEDS - 1)))
if [ "$fails" != 0 ] || [ "$passes" != "$NSEEDS" ]; then
    grep -A12 FAIL "$T/runs.txt" | head -40
    echo "u1_check: FAIL ($passes of $NSEEDS seeds pass)"; exit 1
fi
echo "u1_check: ok ($NSEEDS seeds of $COUNT random instructions identical to MAME)"
