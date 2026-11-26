#!/usr/bin/env bash
# JV-880 tests for CI (review 2026-11-26 roland.md #1): builds cores/jv880/sim (board bench,
# nvsave_tb) and runs tests/test_*.py. nvsave_tb needs no ROMs and always runs. The lockstep tests
# need the jv880-v1.0.1 ROMs (JV880_ROM_DIR or SC55_ROM_DIR) and print SKIP without them; with ROMs
# this also builds the SC-55 benches they use (cores/sc55/out/sim). Run from anywhere; exits
# non-zero if any test fails or nothing passed.
# Needs: cmake, ninja, a C++20 compiler, verilator (5.x), python3; git submodule modules/nuked-sc55.
set -euo pipefail
CORE="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SC55="$CORE/../sc55"
export JV880_SIM_BUILD="${JV880_SIM_BUILD:-$CORE/out/sim}"
export SC55_SIM_BUILD="${SC55_SIM_BUILD:-$SC55/out/sim}"
export SC55_REF_BUILD="${SC55_REF_BUILD:-$JV880_SIM_BUILD/ref}"

cmake -S "$CORE/sim" -B "$JV880_SIM_BUILD" -G Ninja -DCMAKE_BUILD_TYPE=Release >/dev/null
if [ -n "${JV880_ROM_DIR:-}${SC55_ROM_DIR:-}" ]; then
    ninja -C "$JV880_SIM_BUILD" >/dev/null
    cmake -S "$SC55/sim" -B "$SC55_SIM_BUILD" -G Ninja -DCMAKE_BUILD_TYPE=Release >/dev/null
    ninja -C "$SC55_SIM_BUILD" mcu_lockstep pcm_lockstep system_lockstep >/dev/null
else
    ninja -C "$JV880_SIM_BUILD" nvsave_tb >/dev/null
fi

set +e
out="$(cd "$CORE/tests" && python3 "$CORE/tests/test_jv880_lockstep.py" 2>&1)"
rc=$?
set -e
printf '%s\n' "$out"
summary="$(printf '%s\n' "$out" | grep -E '^[0-9]+ passed, [0-9]+ skipped' | tail -1 || true)"
echo "jv880 ci: ${summary:-no summary} (exit $rc)"
[ "$rc" -eq 0 ]
