#!/bin/sh
# SPDX-License-Identifier: GPL-2.0-or-later
# Build the co-simulation: Nuked-MT32's board, LA32, reverb and LCD code (with the log hooks)
# driving the Verilated 8098 (hdl/mcs96) instead of MAME's C++ core. Same options and log
# formats as ref/mt32_ref.
#   sim/cpu/build.sh [OUTDIR]      (default build/cosim) -> OUTDIR/mt32_cosim
set -eu
here=$(cd "$(dirname "$0")" && pwd)
core=$(cd "$here/../.." && pwd)
root=$(cd "$core/../.." && pwd)
out=${1:-$core/build/cosim}
mkdir -p "$out"
out=$(cd "$out" && pwd)
src=$out/src
python3 "$core/ref/prepare_src.py" "$root/modules/nuked-mt32" "$src"
cp "$here/i8x9x.h" "$src/mame/i8x9x.h"
verilator --cc --exe --build -O3 -j "${JOBS:-4}" --top-module mcs96 \
    -Wno-fatal -Wno-WIDTH -Wno-UNUSED \
    "-DMCS96_DECODE_HEX=\"$core/hdl/mcs96/mcs96_decode.hex\"" \
    -F "$core/sim/files.f" \
    "$src/mt32.cpp" "$src/la32.cpp" "$src/rv2.cpp" "$src/lcd.cpp" \
    "$core/ref/mt32_ref.cpp" "$here/mcs96_shim.cpp" \
    -CFLAGS "-std=c++17 -O2 -w -I$src -I$src/mame" \
    --Mdir "$out/obj" -o mt32_cosim >"$out.log" 2>&1 || { tail -30 "$out.log"; exit 1; }
rm -f "$out/mt32_cosim"; cp "$out/obj/mt32_cosim" "$out/mt32_cosim"
echo "$out/mt32_cosim"
