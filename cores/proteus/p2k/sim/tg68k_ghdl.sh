#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-3.0-or-later
# TG68K (VHDL) -> OUTDIR/tg68k_kernel.v for Verilator benches, with GHDL synth in the hdlc/ghdl:yosys
# container (e.g. via colima). Skipped when the output is newer than the kernel sources. Shared by
# cores/p2k/sim/build.sh (board bench) and cores/proteus/p2k/sim/build_cpu.sh (CPU bench).
#   tg68k_ghdl.sh OUTDIR        (OUTDIR inside the repository: the container mounts the repo root)
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../../.." && pwd)"
OUT="$(mkdir -p "$1" && cd "$1" && pwd)"
case "$OUT" in "$ROOT"/*) ;; *) echo "tg68k_ghdl.sh: $OUT is outside $ROOT" >&2; exit 1 ;; esac
TG="cores/proteus/p2k/rtl/cpu/tg68k"
V="$OUT/tg68k_kernel.v"
if [ -f "$V" ] && [ "$V" -nt "$ROOT/$TG/TG68KdotC_Kernel.vhd" ] && [ "$V" -nt "$ROOT/$TG/TG68K_ALU.vhd" ] \
   && [ "$V" -nt "$ROOT/$TG/TG68K_Pack.vhd" ]; then
  exit 0
fi
REL="${OUT#"$ROOT"/}"
docker run --rm -v "$ROOT":/w -w /w hdlc/ghdl:yosys sh -c \
  "ghdl -a --std=08 -fsynopsys --workdir=$REL $TG/TG68K_Pack.vhd $TG/TG68K_ALU.vhd $TG/TG68KdotC_Kernel.vhd && \
   ghdl --synth --std=08 -fsynopsys --latches --workdir=$REL --out=verilog -gColdFire=1 TG68KdotC_Kernel > $REL/tg68k_kernel.v.tmp" \
  > "$OUT/ghdl.log" 2>&1 || { cat "$OUT/ghdl.log"; rm -f "$V.tmp"; exit 1; }
mv "$V.tmp" "$V"
