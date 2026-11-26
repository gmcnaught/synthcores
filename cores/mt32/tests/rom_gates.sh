#!/bin/bash
# SPDX-License-Identifier: GPL-2.0-or-later
# MT-32 gates that need the ROMs (MT32_ROM_DIR; local only, not in CI):
#   corpus hashes from the reference, the co-simulation and the whole HDL system (tools/corpus.py), the sound
#   replay (tests/sound_check.sh), the uptime wrap (tests/wrap_check.sh) and the reference built
#   without its log hooks (tests/ref_hooks_check.sh).
# Without MT32_ROM_DIR every gate is skipped and the script exits 77.
set -uo pipefail
core=$(cd "$(dirname "$0")/.." && pwd)
py=$(command -v pypy3 || command -v python3)     # pypy3 when present (CLAUDE.md)
pass=0; fail=0; skip=0
run() {
    echo "== mt32 $*"
    "$@"; local rc=$?
    case $rc in 0) pass=$((pass + 1)) ;; 77) skip=$((skip + 1)) ;; *) fail=$((fail + 1)) ;; esac
}
if [ -n "${MT32_ROM_DIR:-}" ]; then
    "$core/ref/build.sh" >/dev/null && "$core/sim/cpu/build.sh" >/dev/null &&
        "$core/sim/system/build.sh" >/dev/null || exit 1
fi
run "$py" "$core/tools/corpus.py" check
run "$py" "$core/tools/corpus.py" check --renderer "$core/build/cosim/mt32_cosim"
run "$py" "$core/tools/corpus.py" check --renderer "$core/build/system/mt32_sys_bench"
run "$core/tests/sound_check.sh"
run "$core/tests/wrap_check.sh"
run "$core/tests/ref_hooks_check.sh"
echo "mt32 rom gates: $pass passed, $fail failed, $skip skipped"
[ "$fail" = 0 ] || exit 1
[ "$pass" -gt 0 ] || exit 77
