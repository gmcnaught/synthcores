#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-3.0-or-later
# ROM-free P2K tests for CI: every cores/p2k/tests/test_*.py (Verilator unit benches for the
# ColdFire peripherals, p2k_mem, p2k_loader, p2k_imgsave, p2k_sndbus). Runnable from any directory.
# Needs: verilator 5.x, a C++17 compiler, python3. A test that exits 77 counts as a skip, not a pass.
set -euo pipefail
CORE="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$CORE"
mkdir -p build
pass=0; fail=0; skip=0; failed=()
for t in tests/test_*.py; do
  name=$(basename "$t" .py)
  log="build/ci-$name.log"
  rc=0
  python3 "$t" > "$log" 2>&1 || rc=$?
  case $rc in
    0)  pass=$((pass + 1)); echo "PASS $name" ;;
    77) skip=$((skip + 1)); echo "SKIP $name: $(grep -m1 '^SKIP' "$log" || true)" ;;
    *)  fail=$((fail + 1)); failed+=("$name"); echo "FAIL $name (log $CORE/$log)"; tail -20 "$log" ;;
  esac
done
echo "p2k ci: $pass passed, $skip skipped, $fail failed${failed:+ (${failed[*]})}"
[ "$fail" -eq 0 ] && [ "$pass" -gt 0 ]
