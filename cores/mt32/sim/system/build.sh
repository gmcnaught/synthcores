#!/bin/sh
# SPDX-License-Identifier: GPL-2.0-or-later
# Build the MT-32 system bench (hdl/mt32_core.sv): sim/system/build.sh [OUTDIR] (default build/system)
# VDEFS adds Verilator options, such as the sim-only time presets (tests/wrap_check.sh).
set -eu
here=$(cd "$(dirname "$0")" && pwd)
core=$(cd "$here/../.." && pwd)
out=${1:-$core/build/system}
mkdir -p "$out"
out=$(cd "$out" && pwd)
h=$core/hdl
# shellcheck disable=SC2086
verilator --cc --exe --build -O3 -j "${JOBS:-4}" --top-module mt32_core ${VDEFS:-} \
    -Wno-fatal -Wno-WIDTH -Wno-UNUSED -Wno-DECLFILENAME -Wno-BLKSEQ \
    "-DMCS96_DECODE_HEX=\"$h/mcs96/mcs96_decode.hex\"" \
    -GPOW_HEX="\"$h/la32/mt32_la32_pow.hex\"" -GLOGSIN_HEX="\"$h/la32/mt32_la32_logsin.hex\"" \
    -F "$core/sim/files.f" "$here/mt32_sys_bench.cpp" \
    -CFLAGS "-O2 -std=c++17" -MAKEFLAGS "OPT_FAST=-O2 OPT_SLOW=-O1" \
    --Mdir "$out/obj" -o mt32_sys_bench > "$out/build.log" 2>&1 || { grep -E "%Error" "$out/build.log" | head -30; tail -20 "$out/build.log"; exit 1; }
rm -f "$out/mt32_sys_bench"; cp "$out/obj/mt32_sys_bench" "$out/mt32_sys_bench"
echo "$out/mt32_sys_bench"
