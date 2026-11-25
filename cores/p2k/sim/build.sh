#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-3.0-or-later
# Build the P2K system bench: build/sim/p2k_board_tb (Verilator; hdl/p2k_board.sv). TG68K is VHDL:
# GHDL synth (hdlc/ghdl:yosys container, e.g. via colima) converts it to Verilog first, as
# cores/proteus/p2k/sim/build_cpu.sh does.
#   sim/build.sh [OUTDIR]
# VFLAGS="--public-flat-rw -CFLAGS -DP2K_TB_PUBLIC" adds P2K_TB_FIRST=N (the first N CPU accesses) at
# a large speed cost.
set -euo pipefail
CORE="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
ROOT="$(cd "$CORE/../.." && pwd)"
OUT="${1:-$CORE/build/sim}"
P2K="$ROOT/cores/proteus/p2k/rtl"
SF="$ROOT/modules/synthframe/hdl"
mkdir -p "$OUT"
TG="$P2K/cpu/tg68k"
if [ ! -f "$OUT/tg68k_kernel.v" ] || [ "$TG/TG68KdotC_Kernel.vhd" -nt "$OUT/tg68k_kernel.v" ] || [ "$TG/TG68K_ALU.vhd" -nt "$OUT/tg68k_kernel.v" ]; then
  REL=$(python3 -c "import os,sys; print(os.path.relpath(sys.argv[1], sys.argv[2]))" "$OUT" "$ROOT")
  docker run --rm -v "$ROOT":/w -w /w hdlc/ghdl:yosys sh -c \
    "ghdl -a --std=08 -fsynopsys --workdir=$REL cores/proteus/p2k/rtl/cpu/tg68k/TG68K_Pack.vhd cores/proteus/p2k/rtl/cpu/tg68k/TG68K_ALU.vhd cores/proteus/p2k/rtl/cpu/tg68k/TG68KdotC_Kernel.vhd && \
     ghdl --synth --std=08 -fsynopsys --latches --workdir=$REL --out=verilog TG68KdotC_Kernel > $REL/tg68k_kernel.v" \
    > "$OUT/ghdl.log" 2>&1 || { cat "$OUT/ghdl.log"; exit 1; }
fi
verilator --cc "$OUT/tg68k_kernel.v" "$P2K/cpu/mcf5206e_periph.sv" "$P2K/cpu/p2k_cpu.sv" \
  "$P2K/snd/p2k_ram.sv" "$P2K/snd/p2k_mul.sv" "$P2K/snd/p2k_snd_filt.sv" "$P2K/snd/p2k_fx_sine_rom.sv" "$P2K/snd/p2k_fx.sv" "$P2K/snd/p2k_snd.sv" \
  "$SF/mem/synthframe_mem_arb.sv" "$SF/mem/synthframe_mem_ddr3.sv" "$SF/panel/synthframe_hd44780.sv" \
  "$CORE/hdl/p2k_x21.sv" "$CORE/hdl/p2k_mem.sv" "$CORE/hdl/p2k_sndbus.sv" "$CORE/hdl/p2k_board.sv" \
  -I"$P2K/snd" --top-module p2k_board --exe "$CORE/sim/p2k_board_tb.cpp" \
  -Mdir "$OUT/obj" -O3 -CFLAGS "-O2 -std=c++17" -Wno-fatal -Wno-lint -Wno-style -Wno-MULTIDRIVEN ${VFLAGS:-} \
  --build -j 8 -o "$OUT/p2k_board_tb" > "$OUT/verilator.log" 2>&1 || { grep -E '%Error' "$OUT/verilator.log" | head -40; tail -20 "$OUT/verilator.log"; exit 1; }
echo "$OUT/p2k_board_tb"
