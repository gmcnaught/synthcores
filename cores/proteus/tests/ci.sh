#!/usr/bin/env bash
# ROM-free tests of the Proteus core directory (Proteus/1 XR and Proteus 2000), for CI.
# Runnable from anywhere; exits non-zero if any test fails. A test that exits 77 is a skip
# (counted separately, never as a pass).
#
#   - P2K sound and flash model unit tests (p2k/model/build.sh)
#   - every tests/test_*.py and tests/*/test_*.py:
#       test_snd_model.py   Proteus/1 sound HDL (snd_ctrl + proteus_snd) vs tools/proteus_model.py
#       test_mfp.py         MC68901 against MAME's behaviour at the corners the firmware can hit
#       test_core_unit.py   proteus_core with a small 68000 program: sound-register read-back,
#                           pan byte lanes, preset macro vs autosave
#       test_compare_writes.py, test_rebuild_checksum.py   Proteus/1 tools
#       p2k_snd/test_p2k_snd.py  P2K sound HDL vs the integer model
#   - smoke_gate.py: scripts/mister/smoke.sh's pass/fail step on synthetic screenshots; needs
#     Pillow, skips without it (not named test_*.py, so a plain loop over test_*.py that
#     treats exit 77 as failure does not run it)
#   - p2k/tests/ci.sh, if present
#   - comparator self-check: test_snd_model.py --inject-error (one HDL output bit flipped) must fail
# Needs: verilator (5.048), a C++17 compiler (g++-14 used when present, as in CI), python3,
# Pillow (pip); pypy3 is used for the Proteus/1 model when present.
set -euo pipefail
CORE="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$CORE"
# Python tests and models run under pypy3 when present (CLAUDE.md); smoke_gate.py needs Pillow
if [ -z "${PYTHON:-}" ] && command -v pypy3 >/dev/null; then PY=pypy3; else PY=${PYTHON:-python3}; fi
PIL_PY=${PIL_PYTHON:-python3}
# Verilator benches build incrementally under build/sim with ccache, one job by default
if [ -z "${OBJCACHE+x}" ] && command -v ccache >/dev/null; then export OBJCACHE="$CORE/../../modules/synthframe/bin/objcache"; fi
pass=0; skip=0; fail=0; failed=()

run() {   # run NAME CMD...
  local name=$1; shift
  echo "== $name"
  local rc=0
  "$@" || rc=$?
  case $rc in
    0)  pass=$((pass + 1)) ;;
    77) skip=$((skip + 1)); echo "-- $name: skipped" ;;
    *)  fail=$((fail + 1)); failed+=("$name"); echo "-- $name: FAILED (exit $rc)" ;;
  esac
}

if [ -z "${CXX:-}" ] && command -v g++-14 >/dev/null; then export CXX=g++-14; fi
run "p2k/model/build.sh" p2k/model/build.sh
for t in tests/test_*.py tests/*/test_*.py; do
  [ -f "$t" ] || continue
  run "$t" "$PY" "$t"
done
run "tests/smoke_gate.py" "$PIL_PY" tests/smoke_gate.py
# comparator self-check: one flipped HDL output bit must fail the Proteus/1 sound comparison
inject_snd() {
  local o rc=0
  o=$("$PY" tests/test_snd_model.py --inject-error 2>&1) || rc=$?
  if [ "$rc" -eq 0 ]; then echo "$o"; echo "test_snd_model.py passed with --inject-error"; return 1; fi
  printf '%s\n' "$o" | grep "output differs from the model" || { echo "$o"; return 1; }
}
run "inject-error (test_snd_model.py --inject-error must fail)" inject_snd
if [ -x p2k/tests/ci.sh ]; then run "p2k/tests/ci.sh" p2k/tests/ci.sh; fi

summary="proteus ci: $pass passed, $skip skipped, $fail failed"
if [ "$fail" -ne 0 ]; then summary="$summary (${failed[*]})"; fi
echo "$summary"
[ "$fail" -eq 0 ]
