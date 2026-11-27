#!/bin/bash
# SPDX-License-Identifier: GPL-2.0-or-later
# MT-32 tests that need no ROMs (CI): run from the repo root or anywhere.
#   test_cpu.py    random 8098 programs: HDL CPU vs Nuked-MT32's CPU (co-simulation)
#   test_board.py  bus decode programs: whole HDL system vs Nuked-MT32, CPU trace
#   test_lcd.py    LCD state and dots vs Nuked-MT32's lcd.cpp
#   inject-error   tools/c1_run.sh with INJECT_ERROR=1 (one HDL trace bit flipped) must fail
# Needs verilator, a C++17 compiler and pypy3 or python3. The ROM gates (corpus hashes, sound replay,
# uptime wrap) are local-only: tests/rom_gates.sh.
set -euo pipefail
core=$(cd "$(dirname "$0")/.." && pwd)
py=$(command -v pypy3 || command -v python3)     # pypy3 when present (CLAUDE.md)
pass=0
for t in test_cpu.py test_board.py test_lcd.py; do
    echo "== mt32 $t"
    "$py" "$core/tests/$t"
    pass=$((pass + 1))
done
# comparator self-check: with one HDL trace bit flipped (INJECT_ERROR), the CPU comparison
# (tools/c1_run.sh -> trace_cmp.py) must report DIFFERENT
echo "== mt32 inject-error (c1_run.sh must report DIFFERENT)"
rom=$(mktemp -d); trap 'rm -rf "$rom"' EXIT
head -c 524288 /dev/zero > "$rom/mt32_pcm.bin"; head -c 32768 /dev/zero > "$rom/mt32_reverb.bin"
if INJECT_ERROR=1 MT32_ROM_DIR="$rom" "$core/tools/c1_run.sh" 1000 400 > "$rom/out.txt" 2>&1; then
    cat "$rom/out.txt"; echo "mt32 ci: c1_run.sh passed with INJECT_ERROR=1"; exit 1
fi
grep -m1 DIFFERENT "$rom/out.txt" || { cat "$rom/out.txt"; echo "mt32 ci: no DIFFERENT"; exit 1; }
pass=$((pass + 1))
echo "mt32 ci: $pass passed, 0 failed"
