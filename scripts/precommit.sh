#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# shellcheck disable=SC2329  # (whole file) the functions are called through check()
# Repository hygiene checks, as CI runs them. Run before committing, or install as the hook:
#   ln -s ../../scripts/precommit.sh .git/hooks/pre-commit
#
#   scripts/precommit.sh            everything
#   scripts/precommit.sh --fast     no Verilator (shellcheck, ruff and synthcore check only)
#
#   synthcore check   generated Quartus/Verilator file lists match cfg/ (PyYAML)
#   hdl-lint          verilator --lint-only -Wall, waivers in cfg/lint.vlt (Verilator 5.048)
#   counter-audit     every uptime counter has a row in cfg/counters.yaml (Verilator, PyYAML)
#   ShellCheck        scripts/, modules/synthframe/bin and cores/*/scripts (0.10 or later)
#   ruff              Python tools and tests, rules in ruff.toml (pip install ruff)
# Exit 1 if any check fails; a missing tool is a failure too (CI installs them all).
set -uo pipefail
cd "$(dirname "$0")/.." || exit 1
fast=0; [ "${1:-}" = --fast ] && fast=1
py=$(command -v python3)
fail=0
check() {   # NAME CMD...
    local name=$1; shift
    if "$@"; then echo "precommit: $name ok"; else echo "precommit: $name FAILED"; fail=1; fi
}
need() { command -v "$1" >/dev/null || { echo "precommit: $1 not found"; return 1; }; }

# shell scripts in scope: *.sh, and files with a sh/bash shebang
shell_files() {
    git ls-files scripts modules/synthframe/bin 'cores/*/scripts/*' | while read -r f; do
        case $f in
            *.sh) echo "$f" ;;
            *.*) ;;
            *) head -1 "$f" | grep -qE '^#!.*[/ ](ba)?sh$' && echo "$f" ;;
        esac
    done
}
run_shellcheck() { need shellcheck && shell_files | xargs shellcheck -f gcc; }
run_ruff() { need ruff && ruff check --output-format concise .; }

check "synthcore check" "$py" modules/synthframe/bin/synthcore check
if [ $fast = 0 ]; then
    check "hdl-lint" "$py" modules/synthframe/bin/hdl-lint
    check "counter-audit" "$py" modules/synthframe/bin/counter-audit
fi
check "shellcheck" run_shellcheck
check "ruff" run_ruff
exit $fail
