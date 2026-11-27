#!/bin/bash
# SPDX-License-Identifier: GPL-2.0-or-later
# M3/M4 gate: the sound section (LA32, reverb chip, mix) against Nuked-MT32 on the MIDI corpus.
#
#   tests/sound_check.sh [CASE ...]      (default: every tests/midi/*.txt)
#
# For each case the reference writes its logs, then the bench replays them with random PCM
# memory latency (1-40 clocks) for two seeds; every sample (accumulators, reverb, volume,
# output) and every LA32 interrupt must be identical. SLOT=1 also compares every slot's state.
# Needs MT32_ROM_DIR: without it prints SKIP and exits 77. JOBS (default 4) cases run at once.
# Every case must report "identical" for both seeds; an empty or missing result is a failure.
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
core=$(dirname "$here")
if [ -z "${MT32_ROM_DIR:-}" ]; then echo "SKIP: sound_check: MT32_ROM_DIR not set"; exit 77; fi
"$core/ref/build.sh" >/dev/null
"$core/sim/sound/build.sh" >/dev/null
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT
cases=${*:-$(cd "$core/tests/midi" && ls *.txt | sed 's/\.txt$//')}
run_case() {
    c=$1
    d="$tmp/$c"; mkdir -p "$d"
    n=$(head -1 "$core/tests/midi/$c.txt" | cut -d' ' -f3)
    slot=""; [ "${SLOT:-0}" = 1 ] && slot="--slot-log $d/s.txt"
    # "# args" lines: panel buttons and knob for the reference (the bench replays its logs)
    args=$(sed -n 's/^# args //p' "$core/tests/midi/$c.txt" | tr '\n' ' ')
    # shellcheck disable=SC2086
    "$core/build/ref/mt32_ref" --roms "$MT32_ROM_DIR" --midi "$core/tests/midi/$c.txt" --samples "$n" \
        --la32-log "$d/l.txt" --rv-log "$d/r.txt" --out-log "$d/o.txt" --irq-log "$d/i.txt" $slot $args
    for seed in 1 2; do
        slotb=""; [ "${SLOT:-0}" = 1 ] && slotb="--slot-log $d/s.txt"
        r=$("$core/build/sound/sound_bench" --roms "$MT32_ROM_DIR" --la32-log "$d/l.txt" --rv-log "$d/r.txt" \
            --out-log "$d/o.txt" --irq-log "$d/i.txt" --samples "$n" --seed "$seed" $slotb 2>&1 | tail -3) || true
        echo "$c seed $seed: $r"
    done
    rm -f "$d"/*.txt
}
# at most JOBS cases at once: wait for the oldest before starting another (a count of
# `jobs -p` in a command substitution kept listing finished jobs and never dropped)
pids=()
for c in $cases; do
    if [ "${#pids[@]}" -ge "${JOBS:-4}" ]; then wait "${pids[0]}" || true; pids=("${pids[@]:1}"); fi
    run_case "$c" > "$tmp/$c.result" 2>&1 &
    pids+=($!)
done
wait
pass=0; fail=0
for c in $cases; do
    cat "$tmp/$c.result" 2>/dev/null || true
    for seed in 1 2; do
        if grep -q "^$c seed $seed: identical" "$tmp/$c.result" 2>/dev/null; then pass=$((pass + 1)); else fail=$((fail + 1)); fi
    done
done
echo "sound_check: $pass identical, $fail failed (cases x 2 seeds)"
[ "$fail" = 0 ] && [ "$pass" -gt 0 ]
