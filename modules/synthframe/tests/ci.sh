#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# synthframe's ROM-free tests, for CI (tests.yml runs every cores/*/tests/ci.sh and
# modules/*/tests/ci.sh). Runs from any directory; exits non-zero if any test fails.
#
#   modules/synthframe/tests/ci.sh
#
# Needs: bash, pypy3 (or python3), PyYAML for synthcore, verilator (5.048), a C++ compiler, ccache
# (optional).
set -euo pipefail
cd "$(dirname "$0")/../../.."
pass=0; fail=0; failed=()
run() {     # NAME CMD...
    local name=$1; shift
    echo "== $name"
    if "$@"; then pass=$((pass + 1)); else fail=$((fail + 1)); failed+=("$name"); echo "FAIL: $name"; fi
}
S=modules/synthframe
# Python tests under pypy3 where it is installed (CLAUDE.md); Verilator builds through ccache with
# one make job unless VERILATOR_JOBS says otherwise; build folders persist in $S/out/tests
PY=$(command -v pypy3 || command -v python3)
PYY=$PY; "$PY" -c 'import yaml' 2>/dev/null || PYY=python3     # synthcore needs PyYAML
command -v ccache >/dev/null && export OBJCACHE=${OBJCACHE:-ccache}
export VERILATOR_JOBS=${VERILATOR_JOBS:-1}
run "no ROMs in the repository" "$PY" $S/tests/hygiene/test_no_roms.py
run "generate regions (Quartus 17.0 needs generate ... endgenerate)" "$PY" $S/tests/hdl_style/test_generate_regions.py
run "synthcore check" "$PYY" $S/bin/synthcore check
run "synthcore unit tests" "$PYY" $S/tests/synthcore/test_synthcore.py
run "synth-mister backup/restore (fake SD card)" $S/tests/synth-mister/run.sh
run "mister/build.sh wrappers (fake Quartus)" $S/tests/build/run.sh
run "synthframe_resample 40k->96k (CZ-101)" $S/tests/resample/run.sh
run "synthframe_resample 32k->96k (MT-32)" $S/tests/resample/run.sh 3 1 80 hdl/audio/synthframe_resample_32k_96k.hex
run "MIDI helpers (mkmidi, smf2events, play_tty, tty_midi_xfer)" "$PY" $S/tests/midi_tools/test_midi_tools.py
run "synthframe_mem_ddr3 (DDR3 model, resets with reads in flight)" $S/tests/mem_ddr3/run.sh
run "synthframe_nvsave (hps_io SD model, user reset)" $S/tests/nvsave/run.sh
run "synthframe_midi_tx" "$PY" $S/ver/midi/test_midi_tx.py
echo "synthframe ci: $pass passed, $fail failed${failed:+ (${failed[*]})}"
[ "$fail" = 0 ]
