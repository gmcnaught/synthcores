#!/usr/bin/env bash
# Build the Proteus 2000 CPU testbench: TG68K VHDL -> Verilog (GHDL synth in the hdlc/ghdl:yosys
# container, e.g. via colima) -> Verilator C++ model + p2k_cpu_tb.cpp.
#   p2k/sim/build_cpu.sh            (outputs in build/p2k/vl)
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
OUT="$ROOT/build/p2k/vl"
mkdir -p "$OUT"
cd "$ROOT"
SRC="p2k/rtl/cpu/tg68k"
if [ ! -f "$OUT/tg68k_kernel.v" ] || [ "$SRC/TG68KdotC_Kernel.vhd" -nt "$OUT/tg68k_kernel.v" ] || [ "$SRC/TG68K_ALU.vhd" -nt "$OUT/tg68k_kernel.v" ]; then
  docker run --rm -v "$ROOT":/w -w /w hdlc/ghdl:yosys sh -c \
    "ghdl -a --std=08 -fsynopsys --workdir=build/p2k/vl $SRC/TG68K_Pack.vhd $SRC/TG68K_ALU.vhd $SRC/TG68KdotC_Kernel.vhd && \
     ghdl --synth --std=08 -fsynopsys --latches --workdir=build/p2k/vl --out=verilog -gColdFire=1 TG68KdotC_Kernel > build/p2k/vl/tg68k_kernel.v" \
    > "$OUT/ghdl.log" 2>&1
fi
verilator --cc "$OUT/tg68k_kernel.v" p2k/rtl/cpu/mcf5206e_periph.sv p2k/rtl/cpu/p2k_cpu.sv --top-module p2k_cpu \
  --exe "$ROOT/p2k/sim/p2k_cpu_tb.cpp" \
  -Mdir "$OUT/obj" -O3 -Wno-fatal -Wno-lint -Wno-style --build -j 8 -o p2k_cpu_tb > "$OUT/verilator.log" 2>&1
echo "built $OUT/obj/p2k_cpu_tb"
