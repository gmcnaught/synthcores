#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# An edited tone survives a power cycle (the MiSTer core's reload), on the board bench:
#   tests/reload_run.sh
# 1. From power-on with MAME's init_ram, tests/panel/write.txt writes preset 3 into internal
#    tone 2; the battery RAM is dumped (CZ_NV_OUT).
# 2. The bench starts again from that RAM, as the core does after a reload with its save file,
#    and tests/panel/recall.txt selects internal tone 2.
# 3. A third run selects preset 3 from MAME's init_ram (tests/panel/preset3.txt).
# Pass: internal tone 2 (RAM 0880-08FF) was written and is kept, and the tone buffers the
# firmware plays from (0300-03FF) are the same after the recall as with preset 3 selected.
# Needs CZ101_ROM_DIR (MAME's cz101 set) and out/sim/cz101_bench. Panel keys reach the board
# at the same cycles as in MAME (tests/c4_run.sh with a panel script checks that).
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
W="${RELOAD_WORK:-$HERE/out/reload}"; rm -rf "$W"; mkdir -p "$W/rom" "$W/rom2"
cp "${CZ101_ROM_DIR:?}"/*.bin "$W/rom/"
cp "$W/rom/"*.bin "$W/rom2/"
run() {   # ROMDIR PANEL NAME INSTRUCTIONS
    python3 "$HERE/tools/panel_sched.py" "$2" "$W/$3.keys" "$W/$3.wheel"
    CZ_KEYS="$W/$3.keys" CZ_NV_OUT="$W/$3.nv" "$HERE/out/sim/cz101_bench" "$1" "$4" /dev/null > "$W/$3.txt"
    sed -n 2,3p "$W/$3.txt"
}
run "$W/rom" "$HERE/tests/panel/write.txt" write 2500000 &
run "$W/rom" "$HERE/tests/panel/preset3.txt" preset3 1700000 &
wait
head -c 4096 "$W/write.nv" > "$W/rom2/init_ram.bin"
run "$W/rom2" "$HERE/tests/panel/recall.txt" recall 1700000
python3 - "$W" "$W/rom/init_ram.bin" <<'PY'
import sys
w = sys.argv[1]
init = open(sys.argv[2], "rb").read()
wr, rc, p3 = (open(f"{w}/{n}.nv", "rb").read() for n in ("write", "recall", "preset3"))
written = wr[0x880:0x900] != init[0x880:0x900]
kept = rc[0x880:0x900] == wr[0x880:0x900]
plays = rc[0x300:0x400] == p3[0x300:0x400]
print(f"internal 2 written: {written}; kept after the power cycle: {kept}; "
      f"recalled tone = preset 3: {plays}")
sys.exit(0 if written and kept and plays else 1)
PY
echo "reload: PASS"
