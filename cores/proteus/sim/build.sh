#!/usr/bin/env bash
# Build a Proteus/1 Verilator bench and print the path of the executable.
#
#   sim/build.sh BENCH [OUTDIR]        (OUTDIR default: build/sim/BENCH under the core)
#
# BENCH:
#   snd    snd_tb.cpp: snd_ctrl + proteus_snd + synthframe_mem_* (no CPU), driven
#          by a demo image; output compared with tools/proteus_model.py (tests/test_snd_model.py)
#   mfp    mfp_tb.cpp: mfp68901 alone (tests/test_mfp.py)
#   midi   midi_bench.sv + midi_tb.cpp: midi_in in front of mfp68901 (tests/test_midi_in.py)
#   unit   unit_bench.sv + unit_tb.cpp: proteus_core with a small hand-assembled 68000 program
#          (no ROMs; tests/test_core_unit.py)
#   core   proteus_core + core_main.cpp: the FPGA's wiring, real system image (ROMs)
#   sys    sys_top.sv + sys_main.cpp: proteus_sys + snd_ctrl, real system image (ROMs)
#
# fx68k reads microrom.mem / nanorom.mem and display.sv rtl/panel/*.hex from the working
# directory; they are copied next to the executable, so run the CPU benches from OUTDIR. Warnings
# are waived per file in cfg/lint.vlt (modules/synthframe/bin/hdl-lint), not by -Wno-* options;
# fx68k's microcode latch, which mixes blocking and non-blocking writes (BLKANDNBLK), is one.
# Builds are incremental in OUTDIR (keep it between runs) and use ccache when it is installed
# (OBJCACHE); BUILD_JOBS sets the parallel jobs (default 1).
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CORE="$HERE/.."
RTL="$CORE/mister/rtl"
MEM="$CORE/../../modules/synthframe/hdl/mem"
BENCH=${1:?usage: sim/build.sh snd|mfp|midi|unit|core|sys [OUTDIR]}
OUT="${2:-$CORE/build/sim/$BENCH}"
mkdir -p "$OUT"
# ccache through bin/objcache: plain ccache would share a precompiled header between checkouts
if [ -z "${OBJCACHE+x}" ] && command -v ccache >/dev/null; then export OBJCACHE="$CORE/../../modules/synthframe/bin/objcache"; fi
ROOT="$CORE/../.."
FLAGS=(-O3 -Wno-style "$ROOT"/modules/synthframe/lint/common.vlt "$ROOT"/cores/*/cfg/lint.vlt "$ROOT"/modules/*/cfg/lint.vlt
       --x-assign fast --x-initial fast -CFLAGS "-O2 -std=c++17" -j "${BUILD_JOBS:-1}")

case "$BENCH" in
  snd)  SRC=("$RTL/interp_rom.sv" "$RTL/proteus_snd.sv" "$MEM/synthframe_mem_ddr3.sv" "$MEM/synthframe_mem_arb.sv"
             "$RTL/snd_ctrl.sv"); TOP=snd_ctrl; MAIN=snd_tb.cpp ;;
  mfp)  SRC=("$RTL/mfp68901.sv"); TOP=mfp68901; MAIN=mfp_tb.cpp ;;
  midi) SRC=("$ROOT/modules/synthframe/hdl/midi/synthframe_midi_rx.sv" "$RTL/midi_in.sv" "$RTL/mfp68901.sv"
             "$HERE/midi_bench.sv"); TOP=midi_bench; MAIN=midi_tb.cpp ;;
  unit) SRC=(-F "$HERE/files.f" "$HERE/unit_bench.sv"); TOP=unit_bench; MAIN=unit_tb.cpp ;;
  core) SRC=(-F "$HERE/files.f"); TOP=proteus_core; MAIN=core_main.cpp ;;
  sys)  SRC=(-F "$HERE/files.f" "$HERE/sys_top.sv"); TOP=sys_top; MAIN=sys_main.cpp ;;
  *) echo "unknown bench $BENCH" >&2; exit 2 ;;
esac

verilator --cc "${SRC[@]}" --top-module "$TOP" --exe "$HERE/$MAIN" -Mdir "$OUT/obj" "${FLAGS[@]}" ${VFLAGS:-} \
  --build -o "$OUT/V$TOP" > "$OUT/verilator.log" 2>&1 \
  || { grep -E '%Error' "$OUT/verilator.log" | head -40 >&2; tail -20 "$OUT/verilator.log" >&2; exit 1; }
case "$BENCH" in unit|core|sys)
  cp "$RTL/fx68k/microrom.mem" "$RTL/fx68k/nanorom.mem" "$OUT/"
  mkdir -p "$OUT/rtl/panel" && cp "$RTL"/panel/*.hex "$OUT/rtl/panel/" ;;   # display.sv's panel art, by Quartus path
esac
echo "$OUT/V$TOP"
