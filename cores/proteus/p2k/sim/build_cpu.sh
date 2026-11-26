#!/usr/bin/env bash
# Build the Proteus 2000 CPU testbench: TG68K VHDL -> Verilog (p2k/sim/tg68k_ghdl.sh: GHDL synth in
# the hdlc/ghdl:yosys container, e.g. via colima) -> Verilator C++ model + p2k_cpu_tb.cpp.
#   p2k/sim/build_cpu.sh            (outputs in build/p2k/vl)
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
OUT="$ROOT/build/p2k/vl"
mkdir -p "$OUT"
cd "$ROOT"
"$ROOT/p2k/sim/tg68k_ghdl.sh" "$OUT"
verilator --cc "$OUT/tg68k_kernel.v" p2k/rtl/cpu/mcf5206e_periph.sv p2k/rtl/cpu/p2k_cpu.sv --top-module p2k_cpu \
  --exe "$ROOT/p2k/sim/p2k_cpu_tb.cpp" \
  -Mdir "$OUT/obj" -O3 -CFLAGS -g -Wno-fatal -Wno-lint -Wno-style --build -j 8 -o p2k_cpu_tb > "$OUT/verilator.log" 2>&1
echo "built $OUT/obj/p2k_cpu_tb"
