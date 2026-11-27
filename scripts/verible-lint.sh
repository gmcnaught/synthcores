#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Second HDL front end, advisory: verible-verilog-lint with a small rule set on the HDL every
# core builds (the files of each sim/files.f, once each). CI runs it as a job that cannot fail
# the run (tests.yml, job "verible"); findings are for review, not a gate.
#
#   scripts/verible-lint.sh            exit 1 if verible reports anything
#
# Rules: widths of binary literals, case without default, defparam, begin/end label mismatch,
# a semicolon that ends an if/for/while early, empty statements, always @* (always_comb).
set -uo pipefail
cd "$(dirname "$0")/.." || exit 1
RULES=undersized-binary-literal,case-missing-default,forbid-defparam,mismatched-labels,suspicious-semicolon,forbid-consecutive-null-statements,always-comb
files=$(for f in cores/*/sim/files.f; do
    d=$(dirname "$f")
    grep -vE '^(//|\+|$)' "$f" | while read -r p; do realpath --relative-to=. "$d/$p" 2>/dev/null \
        || python3 -c 'import os,sys; print(os.path.relpath(os.path.realpath(sys.argv[1])))' "$d/$p"; done
done | sort -u)
n=$(echo "$files" | wc -l | tr -d ' ')
out=$(echo "$files" | xargs verible-verilog-lint --ruleset=none --rules="$RULES" 2>&1)
rc=$?
[ -n "$out" ] && echo "$out"
echo "verible: $n files, $(printf '%s' "$out" | grep -c ':[0-9]*:[0-9]*' ) finding(s) (rules: $RULES)"
exit $rc
