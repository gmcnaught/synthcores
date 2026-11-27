#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# U0 check: the patched MAME (tools/mame-mu50.sh) runs generated programs and its logs are as
# expected and deterministic. Needs the ../mame-mu50 build (mame/patches/), not the MU50 ROMs.
# Usage: tests/u0_check.sh [WORKDIR]
set -euo pipefail
HERE=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
W=${1:-$(mktemp -d)}
mkdir -p "$W"

python3 - "$W" <<'PY'
import struct, sys
w = sys.argv[1]
def prog(name, code):
    img = bytearray(0x200)
    struct.pack_into(">I", img, 0, 0x100)          # reset vector
    img[0x100:0x100 + len(code)] = code
    open(f"{w}/{name}.bin", "wb").write(img)
# t0: mov.w #0x1234,r0; mov.l #0x200000,er1; mov.w r0,@er1; bra $
prog("t0", bytes.fromhex("79001234" "7a0100200000" "6990" "40fe"))
# t1: mov.l #0x400200,er1; mov.b #0x55,r0l; mov.b r0l,@er1; mov.w #0xffff,r2;
#     1: dec.w #1,r2; bne 1b; mov.b r0l,@er1; bra $
prog("t1", bytes.fromhex("7a0100400200" "f855" "6898" "7902ffff" "1b52" "46fc" "6898" "40fe"))
PY

run() { # name, extra MAME args...
    local n=$1; shift
    rm -rf "$W/roms-$n" "$W/work-$n"
    python3 "$HERE/tools/mu50_testrom.py" "$W/$n.bin" "$W/roms-$n"
    MU_TRACE="$W/$n.trace" MU_SND_LOG="$W/$n.snd" MU_SAMPLES="$W/$n.raw" \
        "$HERE/tools/mame-mu50.sh" "$W/roms-$n" "$W/work-$n" "$@" >"$W/$n.log" 2>&1
}
fail() { echo "u0_check: FAIL: $*" >&2; exit 1; }

# t0: reset vector, state counts, one write
MU_TRACE_LIMIT=5 MU_BUS=1 run t0
grep -qx "6 000100 7900 .* 80" "$W/t0.trace" || fail "t0: first instruction not at 000100, state 6"
grep -q "^10 000104 7A01 00001234 " "$W/t0.trace" || fail "t0: mov.w #imm not 4 states"
grep -qx "W2 200000 1234 20" "$W/t0.trace" || fail "t0: write 1234 to 200000 at state 20 missing"

# t1: SWP00 writes with sample counts; one second of samples; deterministic
cp "$W/t1.bin" "$W/t1b.bin"
for n in t1 t1b; do run "$n" -seconds_to_run 1; done
[ "$(cat "$W/t1.snd")" = $'18 1 200 5500 FF00\n393236 1735 200 5500 FF00' ] || fail "t1: SWP00 log $(tr '\n' '|' <"$W/t1.snd")"
[ "$(wc -c <"$W/t1.raw")" -eq $((44101 * 8)) ] || fail "t1: expected 44101 sample pairs"
for f in trace snd raw; do cmp -s "$W/t1.$f" "$W/t1b.$f" || fail "t1: two runs differ in .$f"; done
echo "u0_check: ok ($W)"
