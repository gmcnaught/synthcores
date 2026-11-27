#!/usr/bin/env bash
# Synclavier ROM-free test suite (CI entry point; runnable from anywhere).
#
# Runs every tests/test_*.py, tests/sim/test_*.py and tests/hdl/test_*.py
# under pypy3 (CLAUDE.md: the simulator and its tests run under pypy3;
# PYTHON=... overrides). Each file uses tests/testlib.py: exit 0 = passed
# (possibly with some tests skipped), 77 = every test skipped (no Verilator,
# no Release L / W0 image), anything else = failed. Image-dependent tests run
# too when SYNCLAVIER_RELEASE_L / SYNCLAVIER_W0 / SYNCLAVIER_W0_RAW are set;
# without them they are counted as skipped, never as passed.
#
# Needs: pypy3, verilator (5.x) and a C++ compiler for tests/hdl (without
# Verilator those files skip), git (test_repo_hygiene). Standard library
# only otherwise.
#
# Exit 1 if any file failed; prints one summary line last.
set -euo pipefail

cd "$(dirname "${BASH_SOURCE[0]}")/.."
PY=${PYTHON:-pypy3}
if ! command -v "$PY" >/dev/null 2>&1; then
    echo "synclavier ci: $PY not found (set PYTHON= to override)" >&2
    exit 1
fi

files_ok=0 files_skip=0 files_fail=0
t_pass=0 t_skip=0 t_fail=0
failed=()
log=$(mktemp)
trap 'rm -f "$log"' EXIT

for t in tests/test_*.py tests/sim/test_*.py tests/hdl/test_*.py; do
    [ -f "$t" ] || continue
    echo "== $t"
    start=$SECONDS
    set +e
    "$PY" "$t" >"$log" 2>&1
    rc=$?
    set -e
    cat "$log"
    # tests/testlib.py's last line: "<file>: N passed, M skipped, K failed"
    if counts=$(grep -E '^[^ ]+: [0-9]+ passed, [0-9]+ skipped, [0-9]+ failed$' "$log" | tail -1); then
        read -r p s f < <(echo "$counts" | sed -E 's/.*: ([0-9]+) passed, ([0-9]+) skipped, ([0-9]+) failed/\1 \2 \3/')
        t_pass=$((t_pass + p)) t_skip=$((t_skip + s)) t_fail=$((t_fail + f))
    elif [ "$rc" -eq 0 ]; then
        echo "   (no tests/testlib.py summary line: tests not counted)"
    elif [ "$rc" -ne 77 ]; then
        t_fail=$((t_fail + 1))                 # failed before its summary line
    fi
    case $rc in
        0)  files_ok=$((files_ok + 1)) ;;
        77) files_skip=$((files_skip + 1)) ;;
        *)  files_fail=$((files_fail + 1)); failed+=("$t") ;;
    esac
    echo "== $t: exit $rc, $((SECONDS - start)) s"
done

if [ "$files_fail" -ne 0 ]; then
    echo "failed: ${failed[*]}"
fi
echo "synclavier ci: files $files_ok passed, $files_skip skipped, $files_fail failed;" \
     "tests $t_pass passed, $t_skip skipped, $t_fail failed"
[ "$files_fail" -eq 0 ]
