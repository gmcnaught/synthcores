#!/bin/sh
# SPDX-License-Identifier: GPL-2.0-or-later
# Build the LCD renderer bench (hdl/mt32_lcd_video.sv): sim/lcd/build.sh [OUTDIR] (default build/lcd)
# JOBS sets the build jobs (default 4)
set -eu
here=$(cd "$(dirname "$0")" && pwd)
core=$(cd "$here/../.." && pwd)
out=${1:-$core/build/lcd}
mkdir -p "$out"
out=$(cd "$out" && pwd)
h=$core/hdl
verilator --cc --exe --build -O2 -j "${JOBS:-4}" --top-module tb_lcd \
    -Wno-fatal -Wno-lint -Wno-style \
    "+define+MT32_FONT_HEX=\"$h/mt32_lcd_font.hex\"" "+define+MT32_PANEL_DIR=\"$h/panel/\"" \
    -F "$core/sim/files.f" "$here/tb_lcd.sv" "$here/lcd_bench.cpp" \
    -CFLAGS "-O2 -std=c++17" \
    --Mdir "$out/obj" -o lcd_bench > "$out/build.log" 2>&1 || { grep -E "%Error" "$out/build.log" | head -30; tail -20 "$out/build.log"; exit 1; }
rm -f "$out/lcd_bench"; cp "$out/obj/lcd_bench" "$out/lcd_bench"
echo "$out/lcd_bench"
