#!/usr/bin/env bash
# Run the CPU testbench in MAME replay mode (see p2k_cpu_tb.cpp) and print the result.
#   p2k/sim/replay.sh MAME.bt FLASH.bin OUTDIR [cycles]
# P2K_SIMM1/P2K_BABY1 etc. are passed through (use the ones the MAME run had).
#
# Iteration 11 gate (reset to the OS 2.26 main screen), from the old repo's MAME build with
# patches 0001-0003 and its untracked build/p2k files (flash_init.bin = the MAME NVRAM flash):
#   mkdir -p OUT/nvram/proteus2k && cp flash_init.bin OUT/nvram/proteus2k/flash
#   P2K_BUSTRACE=OUT/mame_nd.bt P2K_BUSTRACE_NODRAM=1 P2K_SIMM1=simm/demo/big.bin \
#     P2K_BABY1=simm/demo/baby.bin tools/p2k-run.sh 300 OUT -seconds_to_run 12
#   p2k/sim/build_cpu.sh
#   P2K_SIMM1=... P2K_BABY1=... p2k/sim/replay.sh OUT/mame_nd.bt flash_init.bin build/p2k/replay
#   tools/p2k_trace_cmp.py --nodram OUT/mame_nd.bt build/p2k/replay/rtl.trace
set -uo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
bt=$1; flash=$2; out=$3; cycles=${4:-4000000000}
mkdir -p "$out"
P2K_TB_NODRAM=1 P2K_TB_LCD="$out/lcd.log" P2K_TB_REPLAY="$bt" \
  "$ROOT/build/p2k/vl/obj/p2k_cpu_tb" "$flash" "$out/rtl.trace" "$cycles" > "$out/out.txt" 2> "$out/err.txt"
rc=$?
cat "$out/out.txt" "$out/err.txt"
tail -2 "$out/lcd.log" 2>/dev/null
exit $rc
