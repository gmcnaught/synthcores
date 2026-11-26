#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# The real firmware across the 40-bit tick wrap (on the board after about 30.5 h of uptime):
#   tests/wrap_run.sh [N_INSTRUCTIONS]      (default 2,000,000: boot, about 5.5 s)
# Runs cz101_bench from power-on twice: tick time from 0, and from 2 s before 2^40
# (CZ_TICK_BASE), so the counter wraps 2 s into the second run. The CPU trace and the samples
# must be identical. Needs CZ101_ROM_DIR (MAME's cz101 set; without it "SKIP: ..." and exit 77)
# and out/sim/cz101_bench (or CZ101_BENCH).
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
PY="$(command -v pypy3 || command -v python3)"   # comparison scripts run under pypy3 when present
BENCH="${CZ101_BENCH:-$HERE/out/sim/cz101_bench}"
N="${1:-2000000}"
[ -n "${CZ101_ROM_DIR:-}" ] && [ -f "$CZ101_ROM_DIR/hn613256ps40.bin" ] || { echo "SKIP: CZ101_ROM_DIR (MAME's cz101 set) not set or empty"; exit 77; }
[ -x "$BENCH" ] || { echo "FAIL: no $BENCH (build cores/cz101/sim)"; exit 1; }
W="${WRAP_WORK:-$HERE/out/wrap}"; rm -rf "$W"; mkdir -p "$W"
"$BENCH" "$CZ101_ROM_DIR" "$N" "$W/base.trace" "$W/base.s32" > "$W/base.txt" & p1=$!
CZ_TICK_BASE=$(( (1 << 40) - 20000000 )) "$BENCH" "$CZ101_ROM_DIR" "$N" "$W/wrap.trace" "$W/wrap.s32" > "$W/wrap.txt" & p2=$!
wait $p1
wait $p2
head -1 "$W/base.txt"; head -1 "$W/wrap.txt"
"$PY" - "$W" <<'PY'
import hashlib, sys
w = sys.argv[1]
ta, tb = (open(f"{w}/{k}.trace", "rb").read() for k in ("base", "wrap"))
sa, sb = (open(f"{w}/{k}.s32", "rb").read() for k in ("base", "wrap"))
ok = len(sa) >= 4 * 100000 and sa == sb and ta == tb
lines = ta.count(b"\n")
print(f"trace {'identical' if ta == tb else 'DIFFERS'} ({lines} lines); samples "
      f"{'identical' if sa == sb else 'DIFFER'} ({len(sa) // 4} / {len(sb) // 4}), "
      f"sha256 {hashlib.sha256(sb).hexdigest()}")
sys.exit(0 if ok else 1)
PY
echo "wrap: PASS"
