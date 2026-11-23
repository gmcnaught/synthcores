#!/usr/bin/env bash
# C4: the CZ-101 board HDL against MAME's cz101 with the real firmware and a MIDI script.
#   tests/c4_run.sh MIDI_SCRIPT N_INSTRUCTIONS
# Needs CZ101_ROM_DIR (MAME's cz101 set), HD44780_ROM (hd44780_a00.bin), the patched MAME
# (../mame-cz101) and out/sim/cz101_bench. MAME's midiin starts playback 10 s after power-on.
# Compares the CPU trace (registers, memory writes, interrupts) and the raw 40 kHz samples.
# Then the MiSTer core's live-MIDI path: MAME's MIDI-in decoded to bytes (tools/rxlog_bytes.py)
# and sent by the board's serializer (CZ_MIDI_BYTES) must give the same trace and samples.
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SCRIPT=$1; N=$2
NAME=$(basename "$SCRIPT" .txt)
W="${C4_WORK:-$HERE/out/c4}/$NAME"; rm -rf "$W"; mkdir -p "$W/rom/cz101" "$W/rom/hd44780"
cp "${CZ101_ROM_DIR:?}"/*.bin "$W/rom/cz101/"
cp "${HD44780_ROM:?}" "$W/rom/hd44780/"
python3 "$HERE/../proteus/tools/mkmidi.py" "$SCRIPT" "$W/in.mid"
SECS=$(( N / 300000 + 4 ))
CZ_POWER_ON=1 CZ_TRACE="$W/mame.trace" CZ_TRACE_LIMIT=$N CZ_SAMPLES="$W/mame.s32" \
    CZ_RX_LOG="$W/mame.rx" CZ_SND_LOG="$W/mame.snd" \
    "$HERE/tools/mame-cz101.sh" "$W/rom" "$W/mame" -seconds_to_run $SECS -midiin "$W/in.mid" >/dev/null 2>&1 || true
"$HERE/out/sim/cz101_bench" "$W/rom/cz101" "$N" "$W/hdl.trace" "$W/hdl.s32" "$W/mame.rx" | tee "$W/hdl.txt"
python3 "$HERE/tools/trace_cmp.py" "$W/mame.trace" "$W/hdl.trace" "$N"
python3 - "$W/mame.s32" "$W/hdl.s32" <<'PY'
import hashlib, struct, sys
a, b = (open(p, "rb").read() for p in sys.argv[1:3])
n = min(len(a), len(b)) // 4 * 4
va = struct.unpack(f"<{n // 4}i", a[:n]); vb = struct.unpack(f"<{n // 4}i", b[:n])
d = next((i for i in range(n // 4) if va[i] != vb[i]), None)
nz = sum(1 for x in va if x)
if d is not None:
    print(f"samples: FAIL at {d} of {n // 4}: MAME {va[d]} HDL {vb[d]}"); sys.exit(1)
print(f"samples: identical {n // 4} ({nz} non-zero), sha256 {hashlib.sha256(a[:n]).hexdigest()}")
PY
python3 "$HERE/tools/rxlog_bytes.py" "$W/mame.rx" > "$W/live.bytes"
CZ_MIDI_BYTES="$W/live.bytes" "$HERE/out/sim/cz101_bench" "$W/rom/cz101" "$N" "$W/live.trace" "$W/live.s32" | tee "$W/live.txt"
python3 "$HERE/tools/trace_cmp.py" "$W/mame.trace" "$W/live.trace" "$N"
python3 - "$W/mame.s32" "$W/live.s32" <<'PY'
import sys
a, b = (open(p, "rb").read() for p in sys.argv[1:3])
n = min(len(a), len(b)) // 4 * 4
if a[:n] != b[:n]:
    print("live MIDI samples: FAIL"); sys.exit(1)
print(f"live MIDI samples: identical {n // 4}")
PY
echo "C4 $NAME: PASS"
