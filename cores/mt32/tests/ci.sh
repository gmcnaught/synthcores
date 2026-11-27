#!/bin/bash
# SPDX-License-Identifier: GPL-2.0-or-later
# MT-32 tests that need no ROMs (CI): run from the repo root or anywhere.
#   test_cpu.py    random 8098 programs: HDL CPU vs Nuked-MT32's CPU (co-simulation)
#   test_board.py  bus decode programs: whole HDL system vs Nuked-MT32, CPU trace
#   test_lcd.py    LCD state and dots vs Nuked-MT32's lcd.cpp
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
echo "mt32 ci: $pass passed, 0 failed"
