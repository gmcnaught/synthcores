#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# synthframe_nvsave against an hps_io SD model (tb.cpp), at AW 13 (CZ-101, one request a pass)
# and AW 15 (JV-880, two requests of 16 KB).
#   tests/nvsave/run.sh [HDL]       default hdl/save/synthframe_nvsave.sv
set -euo pipefail
cd "$(dirname "$0")/../.."
HDL=${1:-hdl/save/synthframe_nvsave.sv}
W=$(mktemp -d); trap 'rm -rf "$W"' EXIT
fail=0
for aw in 13 15; do
    verilator --cc --exe --build -O2 -Wall -Wno-DECLFILENAME -Wno-UNUSEDSIGNAL --x-assign unique --x-initial unique \
        --Mdir "$W/aw$aw" "$HDL" tests/nvsave/tb.cpp --top-module synthframe_nvsave -GAW=$aw -CFLAGS -DAW=$aw >/dev/null
    "$W/aw$aw/Vsynthframe_nvsave" 1 +verilator+seed+1 +verilator+rand+reset+2 || fail=1
done
[ "$fail" = 0 ]
