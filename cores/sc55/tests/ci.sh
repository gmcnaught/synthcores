#!/usr/bin/env bash
# SC-55 tests for CI (review 2026-11-26): builds cores/sc55/sim (Verilator benches + reference model)
# and runs every tests/test_*.py. Tests that need ROMs (SC55_ROM_DIR) print SKIP and do not count
# as passes. Run from anywhere; exits non-zero if any test fails or nothing passed.
# Needs: cmake, ninja, a C++20 compiler, verilator (5.x), python3; git submodule modules/nuked-sc55.
set -euo pipefail
CORE="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD="${SC55_SIM_BUILD:-$CORE/out/sim}"
export SC55_SIM_BUILD="$BUILD"
export SC55_REF_BUILD="${SC55_REF_BUILD:-$BUILD/ref}"

cmake -S "$CORE/sim" -B "$BUILD" -G Ninja -DCMAKE_BUILD_TYPE=Release >/dev/null
ninja -C "$BUILD" >/dev/null

files=0 failed=0 skipped_files=0 tp=0 ts=0 tf=0
for t in "$CORE"/tests/test_*.py; do
    files=$((files + 1))
    echo "== $(basename "$t")"
    set +e
    out="$(cd "$CORE/tests" && python3 "$t" 2>&1)"
    rc=$?
    set -e
    printf '%s\n' "$out"
    if summary="$(printf '%s\n' "$out" | grep -E '^[0-9]+ passed, [0-9]+ skipped' | tail -1)"; then
        read -r p _ s _ f _ <<<"$(printf '%s' "$summary" | tr -d ',')"
        tp=$((tp + p)); ts=$((ts + s)); tf=$((tf + ${f:-0}))
    fi
    case $rc in
        0) ;;
        77) skipped_files=$((skipped_files + 1)) ;;
        *) failed=$((failed + 1)); echo "FAILED: $(basename "$t") (exit $rc)" ;;
    esac
done
echo "sc55 ci: $files files, $tp tests passed, $ts skipped, $tf failed ($failed files failed, $skipped_files all-skip)"
[ "$failed" -eq 0 ] && [ "$tp" -gt 0 ]
