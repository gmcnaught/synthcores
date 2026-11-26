#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# synthframe_mem_ddr3 against a DDR3 model, with short resets while reads are outstanding
# (tb.cpp), for QBITS 2 (Proteus/1) and 4 (MT-32, P2K).
#   tests/mem_ddr3/run.sh [HDL]      default hdl/mem/synthframe_mem_ddr3.sv
set -euo pipefail
cd "$(dirname "$0")/../.."
HDL=${1:-hdl/mem/synthframe_mem_ddr3.sv}
W=$(mktemp -d); trap 'rm -rf "$W"' EXIT
fail=0
for q in 2 4; do
    verilator --cc --exe --build -O2 -Wall -Wno-DECLFILENAME -Wno-UNUSEDSIGNAL --x-assign unique --x-initial unique \
        --Mdir "$W/q$q" "$HDL" tests/mem_ddr3/tb.cpp --top-module synthframe_mem_ddr3 -GQBITS=$q -GTW=4 >/dev/null
    for seed in 1 2 3; do
        echo -n "QBITS $q "
        "$W/q$q/Vsynthframe_mem_ddr3" "$seed" 200000 +verilator+seed+$seed +verilator+rand+reset+2 | tail -1 || fail=1
    done
done
[ "$fail" = 0 ]
