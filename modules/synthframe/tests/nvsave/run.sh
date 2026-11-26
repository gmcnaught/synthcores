#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# synthframe_nvsave against an hps_io SD model (tb.cpp), at AW 13 (CZ-101, one request a pass)
# and AW 15 (JV-880, two requests of 16 KB), with SHADOW 1 and 0.
#   tests/nvsave/run.sh [HDL]       default hdl/save/synthframe_nvsave.sv
set -euo pipefail
cd "$(dirname "$0")/../.."
HDL=${1:-hdl/save/synthframe_nvsave.sv}
W=${NVSAVE_WORK:-out/tests/nvsave}   # kept: rebuilds are incremental
fail=0
# with an HDL argument (e.g. an older module), only the SHADOW checks, without -G parameters
if [ -n "${1:-}" ]; then cfgs="13:1 15:1"; else cfgs="13:1 15:1 13:0 15:0"; fi
for c in $cfgs; do
    aw=${c%:*}; sh=${c#*:}; d=$W/aw$aw-s$sh
    mkdir -p "$d"                # Verilator makes --Mdir but not its parents
    g=(); [ -z "${1:-}" ] && g=(-GAW=$aw -GSHADOW=$sh) || g=(-GAW=$aw)
    verilator --cc --exe --build --build-jobs "${VERILATOR_JOBS:-1}" -O2 -Wall -Wno-DECLFILENAME -Wno-UNUSEDSIGNAL \
        --x-assign unique --x-initial unique --Mdir "$d" "$HDL" tests/nvsave/tb.cpp --top-module synthframe_nvsave \
        "${g[@]}" -CFLAGS "-DAW=$aw -DSHADOW=$sh" >/dev/null
    echo "== AW $aw SHADOW $sh"
    "$d/Vsynthframe_nvsave" 1 +verilator+seed+1 +verilator+rand+reset+2 || fail=1
done
[ "$fail" = 0 ]
