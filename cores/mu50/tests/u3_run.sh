#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# U3: one random SWP00 stimulus (tools/swp00_stimgen.py) through MAME, the C++ reference and the
# HDL. Checks: the reference's replay of MAME's logs equals MAME's samples (unless --wild), and
# the HDL equals the reference (samples, reads, and with VCHECK=1 every channel record and MEG
# register after every sample).
# Usage: tests/u3_run.sh SEED [SECONDS] [swp00_stimgen options...]
#   RECORD=1 also stores the logs and MAME's sample hash in tests/u3/ (the CI cases).
# Needs ../mame-mu50 (mame/patches/ 0001 and 0002) and the benches (cmake -S sim -B out/sim).
set -euo pipefail
HERE=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
ROOT=$(cd "$HERE/../.." && pwd)
# ../mame-mu50 beside the main checkout (also from a git worktree of it)
MAIN=$(cd "$(dirname "$(git -C "$ROOT" rev-parse --path-format=absolute --git-common-dir)")" && pwd)
export MAME_MU50=${MAME_MU50:-$(cd "$MAIN/.." && pwd)/mame-mu50/mu50}
SEED=$1; SECS=${2:-0.5}; shift; shift || true
WILD=0; for a in "$@"; do [ "$a" = --wild ] && WILD=1; done
W="${U3_WORK:-$HERE/out/u3}/$SEED"; rm -rf "$W"; mkdir -p "$W"
SIM="$HERE/out/sim"

python3 "$HERE/tools/swp00_stimgen.py" "$SEED" "$W/prog.bin" "$W/wave.bin" --seconds "$SECS" "$@"
python3 "$HERE/tools/mu50_testrom.py" "$W/prog.bin" "$W/roms" --wave "$W/wave.bin"
RUN=$(python3 -c "import math; print(math.ceil($SECS + 0.2))")    # -seconds_to_run: whole seconds
MU_SND_LOG="$W/snd" MU_SND_RLOG="$W/rd" MU_SAMPLES="$W/mame.raw" \
    "$HERE/tools/mame-mu50.sh" "$W/roms" "$W/work" -seconds_to_run "$RUN" >"$W/mame.log" 2>&1 || true
N=$(( $(wc -c <"$W/mame.raw") / 8 ))
[ "$N" -gt 0 ] || { echo "seed $SEED: FAIL (no MAME samples)"; exit 1; }

"$SIM/swp00_replay" "$W/wave.bin" "$W/snd" "$N" "$W/ref.raw" "$W/rd" 2>"$W/replay.txt"
if [ "$WILD" = 0 ] && ! cmp -s "$W/ref.raw" "$W/mame.raw"; then
    echo "seed $SEED: FAIL (reference differs from MAME; $(cat "$W/replay.txt"))"; exit 1
fi
args=(--rlog "$W/rd" --raw "$W/hdl.raw" --quiet)
[ "${VCHECK:-0}" = 1 ] && args+=(--vcheck)
if ! "$SIM/swp00_bench" "$W/wave.bin" "$W/snd" "$N" "${args[@]}" >"$W/bench.txt" 2>"$W/bench.err"; then
    echo "seed $SEED: FAIL $(cat "$W/bench.txt")"; head -5 "$W/bench.err"; exit 1
fi
cmp -s "$W/hdl.raw" "$W/ref.raw" || { echo "seed $SEED: FAIL (HDL samples differ from the reference)"; exit 1; }
sum=$(shasum -a 256 <"$W/mame.raw" | cut -c1-64)
echo "seed $SEED: PASS $N samples, $(wc -l <"$W/snd" | tr -d ' ') writes, $(wc -l <"$W/rd" | tr -d ' ') reads$([ "$WILD" = 1 ] && echo ' (wild: HDL = wrapping reference)'); $(cut -d';' -f2 "$W/bench.txt")"
if [ "${RECORD:-0}" = 1 ]; then
    mkdir -p "$HERE/tests/u3"
    gzip -9c "$W/snd" >"$HERE/tests/u3/$SEED.snd.gz"
    gzip -9c "$W/rd" >"$HERE/tests/u3/$SEED.rd.gz"
    [ "$WILD" = 1 ] && sum=$(shasum -a 256 <"$W/ref.raw" | cut -c1-64)
    echo "$SEED $N $sum $*" >>"$HERE/tests/u3/expected.txt"
fi
