#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# MU50 tests that need no ROMs and no MAME (the CI entry point; runs from any directory):
#   1. u1: the H8/300H core runs the committed random programs (tests/u1/*.bin, made by
#      tools/h8h_randprog.py) and its traces hash to MAME's (tests/u1/expected.txt);
#   2. comparator self-check: a one-character change to a trace must change its hash.
# The MAME lockstep itself (tests/u1_check.sh, tests/u0_check.sh, tests/u0_corpus.sh) runs locally.
# Needs cmake, a C++20 compiler, Verilator 5; ninja optional.
# Prints "mu50 ci: N passed, N failed"; exits 1 on any failure.
set -euo pipefail
here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
core="$(cd "$here/.." && pwd)"
out="${MU50_SIM_OUT:-$core/out/sim}"
jobs="${MU50_JOBS:-$(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 4)}"
gen=()
command -v ninja >/dev/null && gen=(-G Ninja)
command -v ccache >/dev/null && gen+=(-DCMAKE_CXX_COMPILER_LAUNCHER=ccache)
sha() { if command -v sha256sum >/dev/null; then sha256sum | cut -c1-64; else shasum -a 256 | cut -c1-64; fi; }

pass=0; failed=0
step() {   # NAME COMMAND...
    local name=$1; shift
    if "$@"; then pass=$((pass + 1)); echo "== $name: PASS"
    else failed=$((failed + 1)); echo "== $name: FAIL"; fi
}

# the first N instruction lines of a trace with the writes before them (as tools/u1_run.sh)
cut_trace() { awk -v n="$1" '/^W/{buf = buf $0 "\n"; next} {printf "%s", buf; buf = ""; print; if (++k == n) exit}'; }

u1() {
    local d="$out/u1ci" ok=0 seed n want got
    mkdir -p "$d"
    while read -r seed n want; do
        [ -z "$seed" ] || [ "${seed:0:1}" = "#" ] && continue
        gzip -dc "$here/u1/$seed.bin.gz" > "$d/$seed.bin"
        "$out/h8h_bench" "$d/$seed.bin" "$n" "$d/$seed.trace"
        got=$(cut_trace "$n" < "$d/$seed.trace" | sha)
        if [ "$got" = "$want" ]; then echo "u1 seed $seed: identical ($n lines)"
        else echo "u1 seed $seed: DIFFERENT ($got)"; ok=1; fi
    done < "$here/u1/expected.txt"
    return $ok
}
inject() {   # one changed character in a trace must change the hash
    local d="$out/u1ci" seed n want
    read -r seed n want < <(grep -v '^#' "$here/u1/expected.txt" | head -1)
    [ "$(cut_trace "$n" < "$d/$seed.trace" | sed '5s/[0-9A-F]\([0-9A-F]*\)$/X\1/' | sha)" != "$want" ]
}

if cmake -Wno-dev -S "$core/sim" -B "$out" "${gen[@]}" -DCMAKE_BUILD_TYPE=Release >/dev/null &&
   cmake --build "$out" -j "$jobs" --target h8h_bench >/dev/null; then
    step u1 u1
    step "inject (a changed trace must not match)" inject
else
    failed=$((failed + 1)); echo "== u1: FAIL (h8h_bench did not build)"
fi
echo "mu50 ci: $pass passed, $failed failed"
[ "$failed" = 0 ]
