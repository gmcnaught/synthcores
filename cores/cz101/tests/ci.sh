#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# CZ-101 tests that need no ROMs, MAME or images (the CI entry point; runs from any directory):
#   1. tests/upd933_test.sh: uPD933 HDL against the C++ copy of MAME's model (tables, synthetic
#      stimuli, slow time base, external timer, 40-bit tick wrap, MAME-log path);
#   2. tests/board_test.sh: cz101_bench with short uPD7811 programs (sample rate, tick wrap,
#      firmware power-down, far IRQ timer deadline, /NMI during interrupts, live-MIDI serializer).
# The ROM and MAME lockstep (tests/c4_run.sh, tests/reload_run.sh, tests/wrap_run.sh) runs locally.
# Needs cmake, a C++20 compiler, Verilator 5, python3 (pypy3 if present); ninja optional.
# SECONDS_PER_STIM (default 5 here) sets the length of each uPD933 stimulus; CZ101_JOBS the build
# and test parallelism (default: all cores); ccache is used when present.
# Prints "cz101 ci: N passed, N failed"; exits 1 on any failure.
set -euo pipefail
here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
core="$(cd "$here/.." && pwd)"
out="${CZ101_SIM_OUT:-$core/out/sim}"
jobs="${CZ101_JOBS:-$(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 4)}"
gen=()
command -v ninja >/dev/null && gen=(-G Ninja)
command -v ccache >/dev/null && gen+=(-DCMAKE_CXX_COMPILER_LAUNCHER=ccache)

pass=0; failed=0
step() {   # NAME COMMAND...
    local name=$1; shift
    if "$@"; then pass=$((pass + 1)); echo "== $name: PASS"
    else failed=$((failed + 1)); echo "== $name: FAIL"; fi
}

SECONDS_PER_STIM="${SECONDS_PER_STIM:-5}" UPD933_OUT="$out" step upd933 "$here/upd933_test.sh"
build_bench() {
    cmake -Wno-dev -S "$core/sim" -B "$out" "${gen[@]}" -DCMAKE_BUILD_TYPE=Release >/dev/null &&
    cmake --build "$out" -j "$jobs" --target cz101_bench >/dev/null
}
if build_bench; then
    CZ101_BENCH="$out/cz101_bench" step board "$here/board_test.sh"
else
    failed=$((failed + 1)); echo "== board: FAIL (cz101_bench did not build)"
fi
echo "cz101 ci: $pass passed, $failed failed"
[ "$failed" = 0 ]
