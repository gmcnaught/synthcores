#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# synthframe's ROM-free tests, for CI (tests.yml runs every cores/*/tests/ci.sh and
# modules/*/tests/ci.sh). Runs from any directory; exits non-zero if any test fails.
#
#   modules/synthframe/tests/ci.sh
#
# Needs: bash, python3 with PyYAML, pypy3, verilator (5.048), a C++ compiler.
set -euo pipefail
cd "$(dirname "$0")/../../.."
pass=0; fail=0; failed=()
run() {     # NAME CMD...
    local name=$1; shift
    echo "== $name"
    if "$@"; then pass=$((pass + 1)); else fail=$((fail + 1)); failed+=("$name"); echo "FAIL: $name"; fi
}
S=modules/synthframe
run "synthcore check" python3 $S/bin/synthcore check
run "synthcore unit tests" python3 $S/tests/synthcore/test_synthcore.py
run "synth-mister backup/restore (fake SD card)" $S/tests/synth-mister/run.sh
run "mister/build.sh wrappers (fake Quartus)" $S/tests/build/run.sh
run "synthframe_resample 40k->96k (CZ-101)" $S/tests/resample/run.sh
run "synthframe_resample 32k->96k (MT-32)" $S/tests/resample/run.sh 3 1 80 hdl/audio/synthframe_resample_32k_96k.hex
run "synthframe_midi_tx" python3 $S/ver/midi/test_midi_tx.py
echo "synthframe ci: $pass passed, $fail failed${failed:+ (${failed[*]})}"
[ "$fail" = 0 ]
