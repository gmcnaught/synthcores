#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# U4 check (local, needs the MU50 ROMs and ../mame-mu50 with mame/patches/0001-0003): the whole
# board (hdl/mu50_board.sv: H8/3003, peripherals, RAM, SWP00, LCD) runs the v1.05 firmware on
# sim/mu50_bench.cpp from power-on and its samples are identical to MAME's for every corpus case
# (tests/midi/*.txt, 18 s each): the bench's sample file must hash to the rawhash column of
# tests/midi/expected.txt. MIDI in is replayed at the CPU positions MAME logs (MU_RX_LOG).
# Usage: tests/u4_check.sh [WORKDIR]       Exits 77 (skip) without ROMs or MAME.
set -euo pipefail
HERE=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
ROOT=$(cd "$HERE/../.." && pwd)
ROMS=${MU50_ROM_DIR:-$ROOT/nuked-sc55/firmware/mu50-mame}
MAME=${MAME_MU50:-$(cd "$ROOT/.." && pwd)/mame-mu50/mu50}
W=${1:-$(mktemp -d)}
[ -f "$ROMS/mu50/xr174c0.ic7" ] || { echo "u4_check: SKIP (no ROMs in $ROMS)"; exit 77; }
[ -x "$MAME" ] || { echo "u4_check: SKIP (no $MAME)"; exit 77; }
mkdir -p "$W"
cmake -Wno-dev -S "$HERE/sim" -B "$HERE/out/sim" -G Ninja -DCMAKE_BUILD_TYPE=Release >/dev/null
ninja -C "$HERE/out/sim" mu50_bench >/dev/null

fail=0
while read -r name secs rawhash _; do
    [ -z "$name" ] || [ "${name:0:1}" = "#" ] && continue
    python3 "$ROOT/modules/synthframe/tools/midi/mkmidi.py" "$HERE/tests/midi/$name.txt" "$W/$name.mid"
    rm -rf "$W/$name-mame"
    MAME_MU50="$MAME" MU_RX_LOG="$W/$name.rx" MU_SAMPLES="$W/$name.mame.raw" \
        "$HERE/tools/mame-mu50.sh" "$ROMS" "$W/$name-mame" -seconds_to_run "$secs" -midiin "$W/$name.mid" \
        >/dev/null 2>&1
    n=$(($(wc -c < "$W/$name.mame.raw") / 8))
    "$HERE/out/sim/mu50_bench" "$ROMS" "$n" "$W/$name.hdl.raw" --rx "$W/$name.rx" --lcd "$W/$name.lcd" \
        2> "$W/$name.log"
    got=$(shasum -a 256 < "$W/$name.hdl.raw" | cut -c1-16)
    if [ "$got" = "$rawhash" ] && cmp -s "$W/$name.mame.raw" "$W/$name.hdl.raw"; then
        echo "u4 $name: identical to MAME ($n sample pairs, $rawhash); $(tail -1 "$W/$name.log")"
    else
        echo "u4 $name: DIFFERENT (bench $got, expected $rawhash)"; cmp "$W/$name.mame.raw" "$W/$name.hdl.raw" | head -1
        fail=1
    fi
done < "$HERE/tests/midi/expected.txt"
[ "$fail" = 0 ] && echo "u4_check: ok ($W)"
exit "$fail"
