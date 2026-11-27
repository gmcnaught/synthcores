#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# U2 check (local, needs the MU50 ROMs and ../mame-mu50 with mame/patches/0001-0003): the
# H8/3003 core with its peripherals runs the v1.05 firmware on mu50sys_bench (software board:
# LCD, keys, A/D, IRQ0) and its trace is identical to MAME's, through boot and the MIDI of
# tests/midi/basic.txt. MIDI pin changes are replayed at the CPU positions MAME logs
# (MU_RX_LOG); SWP00 reads take the values MAME returned (MU_SND_RLOG), as the SWP00 is not on
# this bench. The LCD busy flag follows the board rule in sim/mu50sys_bench.cpp.
# Usage: tests/u2_check.sh [SECONDS [WORKDIR]]   (default 12 s; the MIDI starts at 10 s)
# Exits 77 (skip) without ROMs or MAME.
set -euo pipefail
HERE=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
ROOT=$(cd "$HERE/../.." && pwd)
ROMS=${MU50_ROM_DIR:-$ROOT/nuked-sc55/firmware/mu50-mame}
MAME=${MAME_MU50:-$(cd "$ROOT/.." && pwd)/mame-mu50/mu50}
SECS=${1:-12}
W=${2:-$(mktemp -d)}
[ -f "$ROMS/mu50/xr174c0.ic7" ] || { echo "u2_check: SKIP (no ROMs in $ROMS)"; exit 77; }
[ -x "$MAME" ] || { echo "u2_check: SKIP (no $MAME)"; exit 77; }
mkdir -p "$W"

cmake -Wno-dev -S "$HERE/sim" -B "$HERE/out/sim" -G Ninja -DCMAKE_BUILD_TYPE=Release >/dev/null
ninja -C "$HERE/out/sim" mu50sys_bench >/dev/null
python3 "$ROOT/modules/synthframe/tools/midi/mkmidi.py" "$HERE/tests/midi/basic.txt" "$W/basic.mid"
python3 - "$ROMS/mu50/xr174c0.ic7" "$W/fw.bin" <<'PY'
import sys
b = bytearray(open(sys.argv[1], "rb").read())
b[0::2], b[1::2] = b[1::2], b[0::2]     # ROM_LOAD16_WORD_SWAP: back to CPU byte order
open(sys.argv[2], "wb").write(b)
PY
rm -rf "$W/mame"
MAME_MU50="$MAME" MU_TRACE="$W/mame.trace" MU_RX_LOG="$W/rx.log" MU_SND_RLOG="$W/swp.rlog" \
    "$HERE/tools/mame-mu50.sh" "$ROMS" "$W/mame" -seconds_to_run "$SECS" -midiin "$W/basic.mid" >/dev/null 2>&1
# SWP00 reads as bytes, in order (a word read is two bytes, even address first)
python3 - "$W/swp.rlog" "$W/swp.txt" <<'PY'
import sys
with open(sys.argv[1]) as f, open(sys.argv[2], "w") as o:
    for line in f:
        d, m = int(line.split()[3], 16), line.split()[4]
        if m == "FFFF":
            o.write("%02X\n%02X\n" % (d >> 8, d & 0xff))
        else:
            o.write("%02X\n" % ((d >> 8) if m == "FF00" else (d & 0xff)))
PY
# compare every complete instruction line (MAME may stop in the middle of its last one)
N=$(($(grep -vc '^W' "$W/mame.trace") - 1))
"$HERE/out/sim/mu50sys_bench" "$W/fw.bin" "$N" "$W/hdl.trace" --rx "$W/rx.log" --swp "$W/swp.txt"
awk -v n="$N" '/^W/{b = b $0 "\n"; next} {printf "%s", b; b = ""; print; if (++k == n) exit}' \
    "$W/mame.trace" > "$W/mame.cmp"
if cmp -s "$W/mame.cmp" "$W/hdl.trace"; then
    echo "u2_check: ok ($N instruction lines identical to MAME, $(grep -c ' I ' "$W/hdl.trace") interrupts, $(wc -l < "$W/rx.log") MIDI pin changes; $W)"
else
    echo "u2_check: FAIL"; cmp "$W/mame.cmp" "$W/hdl.trace" | head -2
    diff "$W/mame.cmp" "$W/hdl.trace" | head -6; exit 1
fi
rm -f "$W/mame.trace" "$W/mame.cmp" "$W/hdl.trace"
