#!/usr/bin/env bash
# C1: random uPD7810 programs, HDL trace against MAME's. Usage: tools/c1_run.sh SEED [N] [--sfr]
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SEED=$1; N=${2:-20000}; shift; shift || true
W="${C1_WORK:-$HERE/out/c1}/$SEED"; mkdir -p "$W/rom/cz101" "$W/rom/hd44780"
python3 "$HERE/tools/upd7810_randprog.py" "$HERE/hdl/upd7810/upd7810_decode.txt" "$SEED" "$W/rom/cz101/hn613256ps40.bin" "$@"
head -c 4096 "$W/rom/cz101/hn613256ps40.bin" > "$W/rom/cz101/upd7811g-120.bin"
[ -f "$W/rom/cz101/init_ram.bin" ] || head -c 4096 /dev/zero > "$W/rom/cz101/init_ram.bin"
cp "${HD44780_ROM:?set HD44780_ROM to hd44780_a00.bin}" "$W/rom/hd44780/"
rm -rf "$W/mame"
CZ_TRACE="$W/mame.trace" CZ_TRACE_LIMIT=$N "$HERE/tools/mame-cz101.sh" "$W/rom" "$W/mame" -seconds_to_run 60 >/dev/null 2>&1 || true
"$HERE/out/sim/upd7810_bench" "$W/rom/cz101/hn613256ps40.bin" "$N" "$W/hdl.trace" >/dev/null
if cmp -s "$W/mame.trace" "$W/hdl.trace"; then
    echo "seed $SEED: PASS $(grep -vc '^[WI]' "$W/hdl.trace") instructions"
else
    echo "seed $SEED: FAIL"; diff "$W/mame.trace" "$W/hdl.trace" | head -${C1_DIFF:-12}; exit 1
fi
