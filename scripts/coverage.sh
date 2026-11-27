#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Verilator line and toggle coverage of two ROM-free lockstep benches. Report only: CI uploads
# OUT as an artifact and never fails on the numbers (tests.yml, job "coverage").
#   CZ-101 uPD933 against MAME's model: upd933_bench (cores/cz101/sim, UPD933_COVERAGE=ON) on
#     the synthetic stimuli of tests/upd933_test.sh, fast and slow time base, external timer,
#     40-bit tick wrap
#   Proteus/1 sound subsystem against tools/proteus_model.py: snd_tb (cores/proteus/sim/build.sh
#     snd) on the random demo images of tests/test_snd_model.py
# Each bench still compares with its model; a mismatch fails the script.
#
#   scripts/coverage.sh [OUT]      (default out/coverage)
#
# OUT/<bench>.dat (verilator_coverage data), OUT/annotated/ (the HDL with a hit count on each
# line; "%00" marks points never reached), OUT/summary.txt (line, toggle and branch totals).
# Needs cmake, a C++20 compiler, Verilator 5, pypy3 or python3. SECONDS_PER_STIM (default 5)
# sets the uPD933 stimulus length; JOBS the build jobs (default 4).
set -euo pipefail
root=$(cd "$(dirname "$0")/.." && pwd)
out=$(mkdir -p "${1:-$root/out/coverage}" && cd "${1:-$root/out/coverage}" && pwd)
jobs=${JOBS:-4}
secs=${SECONDS_PER_STIM:-5}
py=$(command -v pypy3 || command -v python3)
COV=(--coverage-line --coverage-toggle)
mkdir -p "$out/upd933" "$out/snd"

# CZ-101 uPD933
cz=$root/cores/cz101
b=$out/build-cz101
gen=(); command -v ninja >/dev/null && gen=(-G Ninja)
cmake -Wno-dev -S "$cz/sim" -B "$b" "${gen[@]}" -DCMAKE_BUILD_TYPE=Release -DUPD933_COVERAGE=ON >/dev/null
cmake --build "$b" -j "$jobs" --target upd933_stimgen upd933_bench >/dev/null
stims=()
for seed in 1 2 3 4 5 6; do
    opt=(); [ $seed -ge 5 ] && opt=(--order)
    "$b/upd933_stimgen" $seed "$secs" "$out/upd933/s$seed.ev" "${opt[@]}" >/dev/null
    stims+=("$out/upd933/s$seed.ev")
done
"$b/upd933_stimgen" 9 2 "$out/upd933/r9.ev" >/dev/null
"$b/upd933_stimgen" 11 2 "$out/upd933/w11.ev" >/dev/null
n=0
run_upd933() {   # a DIFF (or a bench error) fails the script
    n=$((n + 1))
    UPD933_COV="$out/upd933/cov$n-" "$b/upd933_bench" "$@" > "$out/upd933/run$n.txt" 2>&1 \
        || { cat "$out/upd933/run$n.txt"; echo "upd933_bench $*: HDL differs from the model"; exit 1; }
}
run_upd933 "${stims[@]}"
run_upd933 --real 5 --lead 60 "$out/upd933/r9.ev"
run_upd933 --ext "$out/upd933/s1.ev" "$out/upd933/s5.ev"
run_upd933 --base $(( (1 << 40) - 5000000 )) "$out/upd933/w11.ev"
verilator_coverage --write "$out/upd933.dat" "$out"/upd933/cov*.dat >/dev/null

# Proteus/1 sound subsystem
pr=$root/cores/proteus
tb=$(VFLAGS="${COV[*]}" BUILD_JOBS=$jobs "$pr/sim/build.sh" snd "$out/build-snd" | tail -1)
k=0
for c in "1" "2" "5 --blocks 10" "3 --edge"; do
    read -ra a <<< "$c"
    seed=${a[0]}
    k=$((k + 1))
    "$py" "$pr/tools/snd_randimg.py" "$out/snd/rand$seed.bin" "$seed" "${a[@]:1}" >/dev/null
    bopt=(); [ "$seed" = 5 ] && bopt=(--no-tone)
    (cd "$out/snd" && "$tb" "rand$seed.bin" "hdl$seed.raw" 5000 "$seed" "${bopt[@]}" \
        "+verilator+coverage+file+$out/snd/cov$k.dat" 2>/dev/null)
    "$py" "$pr/tools/proteus_model.py" "$out/snd/rand$seed.bin" "$out/snd/model$seed.raw" --ticks 5000 \
        "${bopt[@]}" >/dev/null
    cmp -s "$out/snd/hdl$seed.raw" "$out/snd/model$seed.raw" || { echo "snd seed $seed: HDL differs from the model"; exit 1; }
done
verilator_coverage --write "$out/snd.dat" "$out"/snd/cov*.dat >/dev/null

# report
: > "$out/summary.txt"
for d in upd933 snd; do
    mkdir -p "$out/annotated/$d"
    { echo "== $d"
      verilator_coverage --annotate "$out/annotated/$d" --annotate-min 1 "$out/$d.dat" \
          | grep -E '^ *(line|toggle|branch) *:|lines with all'; } | tee -a "$out/summary.txt"
done
echo "coverage: $out (annotated HDL in annotated/; '%00' marks points never reached)"
