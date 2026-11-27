#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# MU50 tests that need no ROMs and no MAME (the CI entry point; runs from any directory):
#   1. u1: the H8/300H core runs the committed random programs (tests/u1/*.hex, made by
#      tools/h8h_randprog.py) and its traces hash to MAME's (tests/u1/expected.txt);
#   2. comparator self-check: a one-character change to a trace must change its hash;
#   3. u2: the H8/3003 with its peripherals (mu50sys_bench) runs tests/u2/*.hex (timer
#      interrupts, SLEEP, A/D scan) and its trace hashes to MAME's (tests/u2/expected.txt);
#   4. u3: the SWP00 HDL on the committed SWP00 access logs (tests/u3/) is identical to the
#      C++ reference (ref/, MAME's code) sample by sample, and its samples hash to MAME's.
# The MAME runs themselves (tests/u1_check.sh, tests/u2_check.sh, tests/u3_run.sh,
# tests/u0_*.sh) are local.
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
        xxd -r -p "$here/u1/$seed.hex" > "$d/$seed.bin"
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

# U3: the SWP00 HDL replays the SWP00 accesses of the committed stimuli (tests/u3/SEED.acc: MAME's
# logs of tools/swp00_stimgen.py runs, tools/swp00_acc.py; the wave ROM is regenerated from the seed) beside the
# C++ reference, and its samples hash to MAME's (tests/u3/expected.txt)
py=python3; command -v pypy3 >/dev/null && py=pypy3
u3() {
    local d="$out/u3ci" ok=0 seed n want args got
    mkdir -p "$d"
    while read -r seed n want args; do
        [ -z "$seed" ] || [ "${seed:0:1}" = "#" ] && continue
        "$py" "$core/tools/swp00_stimgen.py" "$seed" /dev/null "$d/$seed.wave" --wave-only
        cp "$here/u3/$seed.acc" "$d/$seed.acc"
        if ! "$out/swp00_bench" "$d/$seed.wave" "$d/$seed.acc" "$n" --acc --raw "$d/$seed.raw" \
                --quiet > "$d/$seed.txt" 2>&1; then
            echo "u3 seed $seed: HDL differs from the reference: $(tail -1 "$d/$seed.txt")"; ok=1; continue
        fi
        got=$(sha < "$d/$seed.raw")
        ref="MAME"; [[ "$args" == wild* ]] && ref="the wrapping reference"
        if [ "$got" = "$want" ]; then echo "u3 seed $seed: identical to $ref ($n samples${args:+, $args})"
        else echo "u3 seed $seed: DIFFERENT from MAME ($got)"; ok=1; fi
    done < "$here/u3/expected.txt"
    return $ok
}

u2() {
    local d="$out/u2ci" ok=0 name n want got
    mkdir -p "$d"
    while read -r name n want; do
        [ -z "$name" ] || [ "${name:0:1}" = "#" ] && continue
        xxd -r -p "$here/u2/$name.hex" > "$d/$name.bin"
        "$out/mu50sys_bench" "$d/$name.bin" "$n" "$d/$name.trace"
        got=$(cut_trace "$n" < "$d/$name.trace" | sha)
        if [ "$got" = "$want" ]; then echo "u2 $name: identical ($n lines)"
        else echo "u2 $name: DIFFERENT ($got)"; ok=1; fi
    done < "$here/u2/expected.txt"
    return $ok
}

if cmake -Wno-dev -S "$core/sim" -B "$out" "${gen[@]}" -DCMAKE_BUILD_TYPE=Release >/dev/null &&
   cmake --build "$out" -j "$jobs" --target h8h_bench >/dev/null; then
    step u1 u1
    step "inject (a changed trace must not match)" inject
else
    failed=$((failed + 1)); echo "== u1: FAIL (h8h_bench did not build)"
fi
if cmake --build "$out" -j "$jobs" --target swp00_bench >/dev/null; then
    step u3 u3
else
    failed=$((failed + 1)); echo "== u3: FAIL (swp00_bench did not build)"
fi
if cmake --build "$out" -j "$jobs" --target mu50sys_bench >/dev/null; then
    step u2 u2
else
    failed=$((failed + 1)); echo "== u2: FAIL (mu50sys_bench did not build)"
fi
echo "mu50 ci: $pass passed, $failed failed"
[ "$failed" = 0 ]
