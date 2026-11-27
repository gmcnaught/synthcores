#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# U0 corpus: render each tests/midi/*.txt with the patched MAME and the user's ROMs, twice, and
# print the sample and SWP00-write hashes. Fails if the two runs differ, or if a hash differs from
# tests/midi/expected.txt. Without ROMs or MAME it prints SKIP and exits 77.
# Usage: tests/u0_corpus.sh [WORKDIR]       (MU50_ROM_DIR: a rompath holding mu50/ and mulcd/)
set -euo pipefail
HERE=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
ROOT=$(cd "$HERE/../.." && pwd)
ROMS=${MU50_ROM_DIR:-$ROOT/nuked-sc55/firmware/mu50-mame}
MAME=${MAME_MU50:-$(cd "$ROOT/.." && pwd)/mame-mu50/mu50}
[ -f "$ROMS/mu50/xr174c0.ic7" ] || { echo "SKIP: no MU50 ROMs in $ROMS"; exit 77; }
[ -x "$MAME" ] || { echo "SKIP: no patched MAME at $MAME"; exit 77; }
W=${1:-$(mktemp -d)}
mkdir -p "$W"
SECS=18
out=""
for s in "$HERE"/tests/midi/*.txt; do
    n=$(basename "$s" .txt)
    python3 "$ROOT/modules/synthframe/tools/midi/mkmidi.py" "$s" "$W/$n.mid"
    for r in 1 2; do
        rm -rf "$W/$n-w$r"
        MU_SND_LOG="$W/$n.$r.snd" MU_SAMPLES="$W/$n.$r.raw" \
            "$HERE/tools/mame-mu50.sh" "$ROMS" "$W/$n-w$r" -seconds_to_run $SECS -midiin "$W/$n.mid" \
            >"$W/$n.$r.log" 2>&1
    done
    for f in raw snd; do
        cmp -s "$W/$n.1.$f" "$W/$n.2.$f" || { echo "u0_corpus: FAIL: $n: two runs differ in .$f"; exit 1; }
    done
    line="$n $SECS $(shasum -a 256 <"$W/$n.1.raw" | cut -c1-16) $(shasum -a 256 <"$W/$n.1.snd" | cut -c1-16) $(wc -l <"$W/$n.1.snd" | tr -d ' ')"
    echo "$line"
    out+="$line"$'\n'
done
if [ -f "$HERE/tests/midi/expected.txt" ]; then
    diff <(grep -v '^#' "$HERE/tests/midi/expected.txt") <(printf %s "$out") \
        || { echo "u0_corpus: FAIL: hashes differ from tests/midi/expected.txt"; exit 1; }
fi
echo "u0_corpus: ok ($W)"
