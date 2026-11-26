#!/bin/bash
# SPDX-License-Identifier: GPL-2.0-or-later
# Uptime gate: the HDL stays bit-exact with Nuked-MT32 across the two points where 48-bit time
# used to wrap:
#   la32  the LA32 cycle count passes 2^48 / 375 (25.45 h): the CPU target la32 * 375 >> 8 was
#         truncated to 48 bits and the CPU stopped for good (review 2026-11-26 mt32.md#1);
#   cpu   the CPU cycle count passes 2^48 (271 days): the CPU time base, timers and board/LCD
#         time stamps were 48 bits; MAME's timer1 = t / 24 mod 2^16 would jump there.
#
#   tests/wrap_check.sh [POINT ...]      (default: la32 cpu; case CASE=programs)
#
# Both models start their counters near the point instead of 0: the system bench built with the
# sim-only defines MT32_SIM_LA32_START / MCS96_SIM_TC_START, the reference with
# ref/mt32_ref --start LA32:CPU (CPU = LA32 * 375 / 256, the CPU's first target). `programs`
# sends MIDI from sample 64,000 to 261,440 and runs to 313,600; la32 is crossed at sample
# 212,512 and cpu at sample 150,000. Output and LCD text must be identical, and the run must end
# just past the point. The offset itself changes the output in both models, so the comparison
# is with the reference started at the same point, not with tests/corpus.json.
# Needs MT32_ROM_DIR (exit 77, skipped, without it). REF=path uses a reference already built;
# BENCH_LA32= / BENCH_CPU= use benches already built with the matching defines.
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
core=$(dirname "$here")
case=${CASE:-programs}
points=${*:-la32 cpu}
if [ -z "${MT32_ROM_DIR:-}" ]; then echo "SKIP: wrap_check: MT32_ROM_DIR not set"; exit 77; fi
ref=${REF:-}
if [ -z "$ref" ]; then "$core/ref/build.sh" >/dev/null; ref=$core/build/ref/mt32_ref; fi
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT
n=$(head -1 "$core/tests/midi/$case.txt" | cut -d' ' -f3)
args=(--roms "$MT32_ROM_DIR" --midi "$core/tests/midi/$case.txt" --samples "$n")
pass=0
for p in $points; do
    case $p in
    la32) la32_start=$((5592 * (1 << 27))); field=la32_cycle; wrap=750599937896; bench=${BENCH_LA32:-} ;;
    cpu)  la32_start=$((256 * ((1 << 48) / 375 - 150000))); field=cpu_cycles; wrap=$((1 << 48)); bench=${BENCH_CPU:-} ;;
    *) echo "wrap_check: unknown point $p"; exit 2 ;;
    esac
    cpu_start=$((la32_start * 375 / 256))
    if [ -z "$bench" ]; then
        VDEFS="-DMT32_SIM_LA32_START=$la32_start -DMCS96_SIM_TC_START=$cpu_start" \
            "$core/sim/system/build.sh" "$core/build/system_wrap_$p" >/dev/null
        bench=$core/build/system_wrap_$p/mt32_sys_bench
    fi
    "$ref" "${args[@]}" --start "$la32_start:$cpu_start" --out "$tmp/ref.raw" --lcd-log "$tmp/ref.lcd" &
    "$bench" "${args[@]}" --out "$tmp/hdl.raw" --lcd-log "$tmp/hdl.lcd" --stats 2> "$tmp/stats.txt"
    wait
    cat "$tmp/stats.txt"
    end=$(sed -n "s/^end: .*$field \([0-9]*\).*/\1/p" "$tmp/stats.txt")
    if ! python3 -c "import sys; sys.exit(not ($wrap < int(sys.argv[1] or 0) < $wrap + 10**9))" "$end"; then
        echo "wrap_check: FAIL ($p): the run ended at $field ${end:-?}, not just past $wrap (preset not built in?)"
        exit 1
    fi
    got=$(shasum -a 256 "$tmp/hdl.raw" | cut -d' ' -f1)
    want=$(shasum -a 256 "$tmp/ref.raw" | cut -d' ' -f1)
    size=$(wc -c < "$tmp/ref.raw" | tr -d ' ')
    if [ "$size" != $((n * 4)) ] || [ "$got" != "$want" ]; then
        echo "wrap_check: FAIL ($p): $case across $field $wrap: HDL ${got:0:16}, reference ${want:0:16} ($size bytes)"
        exit 1
    fi
    "$(command -v pypy3 || command -v python3)" "$core/tools/trace_cmp.py" --min 10 "$tmp/ref.lcd" "$tmp/hdl.lcd" | sed 's/^/LCD text: /'
    echo "wrap_check: $p: $case identical across $field $wrap ($n samples, end $end, ${got:0:16})"
    pass=$((pass + 1))
done
echo "wrap_check: $pass points identical"
