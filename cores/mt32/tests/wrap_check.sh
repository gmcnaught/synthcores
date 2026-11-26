#!/bin/bash
# SPDX-License-Identifier: GPL-2.0-or-later
# Uptime gate: the CPU keeps running when the LA32 cycle count passes 2^48 / 375 (25.45 h), and
# the HDL stays bit-exact with Nuked-MT32 across that point.
#
#   tests/wrap_check.sh [CASE]      (default: programs)
#
# Both models start their counters there instead of 0: the LA32 cycle count at 5592 * 2^27 and
# the CPU's at 5592 * 2^27 * 375 / 256 (the system bench built with the sim-only defines
# MT32_SIM_LA32_START / MCS96_SIM_TC_START; ref/mt32_ref --start). The count passes 2^48 / 375 at
# sample 212,512; `programs` sends MIDI until sample 261,440 and runs to 313,600. The output and
# the LCD text must be identical. Before the fix (CPU target truncated to 48 bits) the HDL CPU
# stopped there. The output differs from the run from 0 in tests/corpus.json in both models.
# Needs MT32_ROM_DIR (exit 77, skipped, without it). BENCH=path and REF=path use binaries already
# built (the bench with the defines above).
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
core=$(dirname "$here")
case=${1:-programs}
if [ -z "${MT32_ROM_DIR:-}" ]; then echo "SKIP: wrap_check: MT32_ROM_DIR not set"; exit 77; fi
la32_start=$((5592 * (1 << 27)))
cpu_start=$((la32_start * 375 / 256))
wrap=750599937896                       # first LA32 cycle with la32 * 375 >= 2^48
bench=${BENCH:-}
ref=${REF:-}
if [ -z "$ref" ]; then "$core/ref/build.sh" >/dev/null; ref=$core/build/ref/mt32_ref; fi
if [ -z "$bench" ]; then
    VDEFS="-DMT32_SIM_LA32_START=$la32_start -DMCS96_SIM_TC_START=$cpu_start" \
        "$core/sim/system/build.sh" "$core/build/system_wrap" >/dev/null
    bench=$core/build/system_wrap/mt32_sys_bench
fi
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT
n=$(head -1 "$core/tests/midi/$case.txt" | cut -d' ' -f3)
args=(--roms "$MT32_ROM_DIR" --midi "$core/tests/midi/$case.txt" --samples "$n")
"$ref" "${args[@]}" --start "$la32_start:$cpu_start" \
    --out "$tmp/ref.raw" --lcd-log "$tmp/ref.lcd" &
"$bench" "${args[@]}" --out "$tmp/hdl.raw" --lcd-log "$tmp/hdl.lcd" --stats 2> "$tmp/stats.txt"
wait
cat "$tmp/stats.txt"
end=$(sed -n 's/^end: la32_cycle \([0-9]*\),.*/\1/p' "$tmp/stats.txt")
if ! python3 -c "import sys; sys.exit(not ($wrap < int(sys.argv[1] or 0) < $wrap + 10**9))" "$end"; then
    echo "wrap_check: FAIL: the run ended at LA32 cycle ${end:-?}, not just past $wrap (preset not built in?)"
    exit 1
fi
got=$(shasum -a 256 "$tmp/hdl.raw" | cut -d' ' -f1)
want=$(shasum -a 256 "$tmp/ref.raw" | cut -d' ' -f1)
size=$(wc -c < "$tmp/ref.raw" | tr -d ' ')
if [ "$size" != $((n * 4)) ] || [ "$got" != "$want" ]; then
    echo "wrap_check: FAIL: $case across the 2^48/375 point: HDL ${got:0:16}, reference ${want:0:16} ($size bytes)"
    exit 1
fi
python3 "$core/tools/trace_cmp.py" --min 10 "$tmp/ref.lcd" "$tmp/hdl.lcd" | sed 's/^/LCD text: /'
echo "wrap_check: $case identical across the 2^48/375 point ($n samples, end LA32 cycle $end, ${got:0:16})"
