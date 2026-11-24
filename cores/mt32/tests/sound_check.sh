#!/bin/sh
# SPDX-License-Identifier: GPL-2.0-or-later
# M3/M4 gate: the sound section (LA32, reverb chip, mix) against Nuked-MT32 on the MIDI corpus.
#
#   tests/sound_check.sh [CASE ...]      (default: every tests/midi/*.txt)
#
# For each case the reference writes its logs, then the bench replays them with random PCM
# memory latency (1-40 clocks) for two seeds; every sample (accumulators, reverb, volume,
# output) and every LA32 interrupt must be identical. SLOT=1 also compares every slot's state.
# Needs MT32_ROM_DIR; skips without it. JOBS (default 4) cases run at once.
set -eu
here=$(cd "$(dirname "$0")" && pwd)
core=$(dirname "$here")
if [ -z "${MT32_ROM_DIR:-}" ]; then echo "sound_check: MT32_ROM_DIR not set, skipped"; exit 0; fi
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
    "$core/build/ref/mt32_ref" --roms "$MT32_ROM_DIR" --midi "$core/tests/midi/$c.txt" --samples "$n" \
        --la32-log "$d/l.txt" --rv-log "$d/r.txt" --out-log "$d/o.txt" --irq-log "$d/i.txt" $slot
    for seed in 1 2; do
        slotb=""; [ "${SLOT:-0}" = 1 ] && slotb="--slot-log $d/s.txt"
        r=$("$core/build/sound/sound_bench" --roms "$MT32_ROM_DIR" --la32-log "$d/l.txt" --rv-log "$d/r.txt" \
            --out-log "$d/o.txt" --irq-log "$d/i.txt" --seed "$seed" $slotb 2>&1 | tail -3) || true
        echo "$c seed $seed: $r"
    done
    rm -f "$d"/*.txt
}
for c in $cases; do
    while [ "$(jobs -p | wc -l)" -ge "${JOBS:-4}" ]; do sleep 1; done
    run_case "$c" > "$tmp/$c.result" 2>&1 &
done
wait
cat "$tmp"/*.result
! grep -q -v ": identical" "$tmp"/*.result
