#!/bin/sh
# SPDX-License-Identifier: GPL-2.0-or-later
# Build the MT-32 sound bench: sim/sound/build.sh [OUTDIR]  (default build/sound)
set -eu
here=$(cd "$(dirname "$0")" && pwd)
core=$(cd "$here/../.." && pwd)
out=${1:-$core/build/sound}
mkdir -p "$out"
out=$(cd "$out" && pwd)
verilator --cc --exe --build -O3 -j "${JOBS:-4}" --top-module mt32_sound \
    -Wall -Wno-fatal -Wno-UNUSEDSIGNAL -Wno-UNUSEDPARAM -Wno-DECLFILENAME -Wno-BLKSEQ \
    -GPOW_HEX="\"$core/hdl/la32/mt32_la32_pow.hex\"" \
    -GLOGSIN_HEX="\"$core/hdl/la32/mt32_la32_logsin.hex\"" \
    -CFLAGS "-O2 -std=c++17" -MAKEFLAGS "OPT_FAST=-O2 OPT_SLOW=-O1" --Mdir "$out/obj" -o "$out/sound_bench" \
    -F "$core/sim/files.f" "$here/sound_bench.cpp" > "$out/build.log" 2>&1 || { tail -40 "$out/build.log"; exit 1; }
grep -E "%Warning|%Error" "$out/build.log" | head -40 || true
echo "$out/sound_bench"
