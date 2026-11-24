#!/usr/bin/env bash
# Build the P2K sound-engine bench: build/p2k/snd/p2k_snd_tb (Verilator; p2k_snd on synthframe_mem_ddr3).
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$HERE/../.."
MEM="$ROOT/../../modules/synthframe/hdl/mem"
SND="$ROOT/p2k/rtl/snd"
OUT="${1:-$ROOT/build/p2k/snd}"
mkdir -p "$OUT"
verilator --cc "$SND/p2k_ram.sv" "$SND/p2k_mul.sv" "$SND/p2k_snd_filt.sv" "$SND/p2k_fx_sine_rom.sv" "$SND/p2k_fx.sv" "$SND/p2k_snd.sv" \
  "$MEM/synthframe_mem_arb.sv" "$MEM/synthframe_mem_ddr3.sv" "$HERE/p2k_snd_bench.sv" \
  -I"$SND" --top-module p2k_snd_bench --exe "$HERE/p2k_snd_tb.cpp" \
  -Mdir "$OUT/obj" -O3 -CFLAGS "-O2 -std=c++17" -Wall -Wno-fatal -Wno-DECLFILENAME -Wno-UNUSEDSIGNAL ${VFLAGS:-} \
  --build -j 8 -o "$OUT/p2k_snd_tb" > "$OUT/verilator.log" 2>&1 || { grep -E '%Error' "$OUT/verilator.log" | head -40; tail -20 "$OUT/verilator.log"; exit 1; }
grep -E '%Warning' "$OUT/verilator.log" | head -40 >&2 || true
echo "$OUT/p2k_snd_tb"
