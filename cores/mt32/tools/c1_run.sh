#!/bin/sh
# SPDX-License-Identifier: GPL-2.0-or-later
# M1/M2 CPU check on one random program: tools/c1_run.sh SEED [SAMPLES] [--sfr]
# Runs the program (tools/mcs96_randprog.py) on the reference (build/ref/mt32_ref) and on the
# co-simulation (build/cosim/mt32_cosim) and compares the CPU traces. Needs MT32_ROM_DIR
# (for the PCM and reverb ROMs; the control ROM is the generated program). INJECT_ERROR=1 flips
# one bit of the HDL trace before the comparison (trace_cmp.py --inject-error), which must fail.
set -eu
core=$(cd "$(dirname "$0")/.." && pwd)
seed=$1; samples=${2:-3200}; shift; [ $# -gt 0 ] && shift || true
out=$core/out/c1; mkdir -p "$out"
python3 "$core/tools/mcs96_randprog.py" "$seed" "$out/p$seed.bin" "$@"
timeout 300 "$core/build/ref/mt32_ref" --roms "$MT32_ROM_DIR" --cpu-rom "$out/p$seed.bin" --samples "$samples" --trace "$out/r$seed.tr" >/dev/null
timeout 1200 "$core/build/cosim/mt32_cosim" --roms "$MT32_ROM_DIR" --cpu-rom "$out/p$seed.bin" --samples "$samples" --trace "$out/h$seed.tr" >/dev/null
if python3 "$core/tools/trace_cmp.py" "$out/r$seed.tr" "$out/h$seed.tr" ${INJECT_ERROR:+--inject-error} >"$out/cmp$seed.txt"; then
    echo "seed $seed: $(cat "$out/cmp$seed.txt")"; rm -f "$out/r$seed.tr" "$out/h$seed.tr"
else
    echo "seed $seed: DIFFERENT"; cat "$out/cmp$seed.txt"; exit 1
fi
