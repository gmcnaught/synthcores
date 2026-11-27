#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-3.0-or-later
# Build the P2K system bench: build/sim/p2k_board_tb (Verilator; hdl/p2k_board.sv, the HDL list
# sim/files.f). TG68K is VHDL: cores/proteus/p2k/sim/tg68k_ghdl.sh converts it to Verilog first
# (GHDL synth in the hdlc/ghdl:yosys container, e.g. via colima).
#   sim/build.sh [OUTDIR]
# VFLAGS="--public-flat-rw -CFLAGS -DP2K_TB_PUBLIC" adds P2K_TB_FIRST=N (the first N CPU accesses) at
# a large speed cost.
set -euo pipefail
CORE="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
ROOT="$(cd "$CORE/../.." && pwd)"
OUT="${1:-$CORE/build/sim}"
mkdir -p "$OUT"
"$ROOT/cores/proteus/p2k/sim/tg68k_ghdl.sh" "$OUT"
# the core's HDL from sim/files.f (generated from cfg/files.yaml by synthcore)
verilator --cc "$OUT/tg68k_kernel.v" -F "$CORE/sim/files.f" \
  --top-module p2k_board --exe "$CORE/sim/p2k_board_tb.cpp" \
  -Mdir "$OUT/obj" -O3 -CFLAGS "-O2 -std=c++17" -Wno-fatal -Wno-lint -Wno-style -Wno-MULTIDRIVEN ${VFLAGS:-} \
  --build -j 8 -o "$OUT/p2k_board_tb" > "$OUT/verilator.log" 2>&1 || { grep -E '%Error' "$OUT/verilator.log" | head -40; tail -20 "$OUT/verilator.log"; exit 1; }
echo "$OUT/p2k_board_tb"
