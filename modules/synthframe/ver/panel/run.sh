#!/bin/sh
# SPDX-License-Identifier: MIT
# Builds the panel frame benches with Verilator and prints one hash per bench and seed.
# Usage: run.sh [OUT_DIR]   (default: out/ next to this script)
# expected.txt holds the hashes of the renderers as they were before they moved onto
# modules/synthframe/hdl/panel (SC-55, JV-880: before iteration 8; Proteus/1 and CZ-101: main at
# 4c45522, before the CZ-101 move). The CZ-101 hashes were recorded again when it took
# synthframe_status_strip: its frames differ from 4c45522's only inside the strip
# (x 12-391, y 412-431). Front-panel art (synthframe_panel_art, cfg/panel.yaml), 2026-11-25:
# the hashes of each core that took it were recorded again; the MT-32 and P2K benches came with
# their panels (no earlier renderer hashes). test_panel.py compares against it.
set -eu
HERE=$(cd "$(dirname "$0")" && pwd)
ROOT=$(cd "$HERE/../../../.." && pwd)
OUT=${1:-$HERE/out}
mkdir -p "$OUT"
ROLAND=$ROOT/modules/roland/hdl
PANEL=$ROOT/modules/synthframe/hdl/panel
SEEDS="0x1 0x10 0x12345678 0xdeadbeef"

build() {   # name top warm clks sources...
    name=$1; top=$2; shift 2
    # warnings are waived in cfg/lint.vlt (bin/hdl-lint), not by -Wno-fatal / -Wno-lint
    # shellcheck disable=SC2046
    verilator --cc --exe --build -O2 -Wno-style --top-module "$top" \
        "$ROOT"/modules/synthframe/lint/common.vlt $(ls "$ROOT"/cores/*/cfg/lint.vlt "$ROOT"/modules/*/cfg/lint.vlt) \
        -I"$HERE" -I"$ROLAND/board" -I"$PANEL" \
        +define+LCD_BACK_HEX=\""$ROOT/cores/sc55/hdl/board/lcd_back.hex"\" \
        +define+SC55_PANEL_DIR=\""$ROOT/cores/sc55/hdl/panel/"\" -I"$ROOT/cores/sc55/hdl/panel" \
        +define+CZ101_PANEL_DIR=\""$ROOT/cores/cz101/hdl/panel/"\" -I"$ROOT/cores/cz101/hdl/panel" \
        +define+MT32_PANEL_DIR=\""$ROOT/cores/mt32/hdl/panel/"\" -I"$ROOT/cores/mt32/hdl/panel" \
        +define+MT32_FONT_HEX=\""$ROOT/cores/mt32/hdl/mt32_lcd_font.hex"\" \
        +define+P2K_PANEL_DIR=\""$ROOT/cores/p2k/hdl/panel/"\" -I"$ROOT/cores/p2k/hdl/panel" \
        +define+PROTEUS_PANEL_DIR=\""$ROOT/cores/proteus/mister/rtl/panel/"\" -I"$ROOT/cores/proteus/mister/rtl/panel" \
        +define+JV880_PANEL_DIR=\""$ROOT/cores/jv880/hdl/panel/"\" -I"$ROOT/cores/jv880/hdl/panel" \
        -CFLAGS "-O2 -DBENCH_TOP=V$top -DBENCH_H='\"V$top.h\"' -DNAME='\"$name\"'" \
        --Mdir "$OUT/$name" -o "bench_$name" "$HERE/tb_frame.cpp" "$@" >"$OUT/$name.build.log" 2>&1 \
        || { cat "$OUT/$name.build.log" >&2; exit 1; }
}

build sc55 tb_sc55 "$HERE/tb_sc55.sv" "$ROLAND/common/sdp_ram.sv" \
    $(cat "$HERE/files_sc55.txt" | sed "s|^|$ROOT/|")
build jv880 tb_jv880 "$HERE/tb_jv880.sv" \
    $(cat "$HERE/files_jv880.txt" | sed "s|^|$ROOT/|")
build proteus tb_proteus "$HERE/tb_proteus.sv" \
    $(cat "$HERE/files_proteus.txt" | sed "s|^|$ROOT/|")
build cz101 tb_cz101 "$HERE/tb_cz101.sv" \
    $(cat "$HERE/files_cz101.txt" | sed "s|^|$ROOT/|")
build mt32 tb_mt32 "$HERE/tb_mt32.sv" \
    $(cat "$HERE/files_mt32.txt" | sed "s|^|$ROOT/|")
build p2k tb_p2k "$HERE/tb_p2k.sv" \
    $(cat "$HERE/files_p2k.txt" | sed "s|^|$ROOT/|")

# one frame is 1056 x 628 clocks (SC-55, JV-880, CZ-101, MT-32, P2K) or 424 x 262 x 12 (Proteus); two frames
# each. The CZ-101 bench loads its font (4096 clocks) and runs its pin sequence (about 35,000)
# first; the P2K bench loads its font and sends its commands (about 13,000) first.
"$OUT/sc55/bench_sc55" 0 1326336 $SEEDS
"$OUT/jv880/bench_jv880" 0 1326336 $SEEDS
"$OUT/proteus/bench_proteus" 20000 2666112 $SEEDS
"$OUT/cz101/bench_cz101" 60000 1326336 $SEEDS
"$OUT/mt32/bench_mt32" 0 1326336 $SEEDS
"$OUT/p2k/bench_p2k" 20000 1326336 $SEEDS
