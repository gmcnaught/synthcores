#!/usr/bin/env bash
# Build the P2K sound-engine bench: build/p2k/snd/p2k_snd_tb (Verilator).
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$HERE/../.."
OUT="${1:-$ROOT/build/p2k/snd}"
mkdir -p "$OUT"
verilator --cc "$ROOT/p2k/rtl/snd/p2k_snd.sv" "$ROOT/p2k/rtl/snd/p2k_fx.sv" -I"$ROOT/p2k/rtl/snd" --top-module p2k_snd --exe "$HERE/p2k_snd_tb.cpp" \
  -Mdir "$OUT/obj" -O3 -CFLAGS "-O2 -std=c++17" -Wall -Wno-fatal -Wno-DECLFILENAME -Wno-UNUSEDSIGNAL \
  --build -j 8 -o "$OUT/p2k_snd_tb" > "$OUT/verilator.log" 2>&1 || { tail -40 "$OUT/verilator.log"; exit 1; }
grep -E '%Warning' "$OUT/verilator.log" | head -40 >&2 || true
echo "$OUT/p2k_snd_tb"
