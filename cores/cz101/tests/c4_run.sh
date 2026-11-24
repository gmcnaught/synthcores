#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# C4: the CZ-101 board HDL against MAME's cz101 with the real firmware and a MIDI script.
#   tests/c4_run.sh MIDI_SCRIPT N_INSTRUCTIONS [PANEL_SCRIPT]
# Needs CZ101_ROM_DIR (MAME's cz101 set), HD44780_ROM (hd44780_a00.bin), the patched MAME
# (../mame-cz101) and out/sim/cz101_bench. MAME's midiin starts playback 10 s after power-on.
# Compares the CPU trace (registers, memory writes, interrupts) and the raw 40 kHz samples.
# Then the MiSTer core's live-MIDI path: MAME's MIDI-in decoded to bytes (tools/rxlog_bytes.py)
# and sent by the board's serializer (CZ_MIDI_BYTES) must give the same trace and samples.
# PANEL_SCRIPT (tools/panel_sched.py): panel buttons and the pitch wheel, applied at the same CPU
# cycles in MAME and the HDL. MIDI out: the bytes decoded from MAME's TxD line and the HDL's
# (tools/txlog_bytes.py) must be identical, each starting within 32 CPU cycles (a third of a bit)
# of MAME's: MAME moves TxD at its 2 MHz serial clock edges, the board at the next instruction
# boundary.
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SCRIPT=$1; N=$2
NAME=$(basename "$SCRIPT" .txt)
W="${C4_WORK:-$HERE/out/c4}/$NAME"; rm -rf "$W"; mkdir -p "$W/rom/cz101" "$W/rom/hd44780"
cp "${CZ101_ROM_DIR:?}"/*.bin "$W/rom/cz101/"
cp "${HD44780_ROM:?}" "$W/rom/hd44780/"
python3 "$HERE/../proteus/tools/mkmidi.py" "$SCRIPT" "$W/in.mid"
if [ -n "${3:-}" ]; then
    python3 "$HERE/tools/panel_sched.py" "$3" "$W/panel.keys" "$W/panel.wheel"
    export CZ_KEYS="$W/panel.keys" CZ_WHEEL="$W/panel.wheel"
fi
SECS=$(( N / 300000 + 4 ))
CZ_POWER_ON=1 CZ_TRACE="$W/mame.trace" CZ_TRACE_LIMIT=$N CZ_SAMPLES="$W/mame.s32" \
    CZ_RX_LOG="$W/mame.rx" CZ_SND_LOG="$W/mame.snd" CZ_TX_LOG="$W/mame.tx" \
    "$HERE/tools/mame-cz101.sh" "$W/rom" "$W/mame" -seconds_to_run $SECS -midiin "$W/in.mid" >/dev/null 2>&1 || true
CZ_TX_LOG="$W/hdl.tx" "$HERE/out/sim/cz101_bench" "$W/rom/cz101" "$N" "$W/hdl.trace" "$W/hdl.s32" "$W/mame.rx" | tee "$W/hdl.txt"
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
CZ_MIDI_BYTES="$W/live.bytes" CZ_TX_LOG="$W/live.tx" "$HERE/out/sim/cz101_bench" "$W/rom/cz101" "$N" "$W/live.trace" "$W/live.s32" | tee "$W/live.txt"
python3 "$HERE/tools/trace_cmp.py" "$W/mame.trace" "$W/live.trace" "$N"
python3 - "$W/mame.s32" "$W/live.s32" <<'PY'
import sys
a, b = (open(p, "rb").read() for p in sys.argv[1:3])
n = min(len(a), len(b)) // 4 * 4
if a[:n] != b[:n]:
    print("live MIDI samples: FAIL"); sys.exit(1)
print(f"live MIDI samples: identical {n // 4}")
PY
# MIDI out: MAME's bytes stop at its last instruction, so compare up to the shorter stream
python3 "$HERE/tools/txlog_bytes.py" --mame "$W/mame.tx" > "$W/mame.txbytes"
for k in hdl live; do
    python3 "$HERE/tools/txlog_bytes.py" "$W/$k.tx" > "$W/$k.txbytes"
    python3 - "$W/mame.txbytes" "$W/$k.txbytes" "$k" <<'PY'
import sys
a, b = ([l.split() for l in open(p)] for p in sys.argv[1:3])
n = min(len(a), len(b))
d = next((i for i in range(n) if a[i][1] != b[i][1] or abs(int(a[i][0]) - int(b[i][0])) > 32), None)
if d is not None or abs(len(a) - len(b)) > 1:
    d = n if d is None else d
    print(f"{sys.argv[3]} MIDI out: FAIL at byte {d}: MAME {a[d:d+3]} HDL {b[d:d+3]} ({len(a)} / {len(b)} bytes)"); sys.exit(1)
dt = max((abs(int(a[i][0]) - int(b[i][0])) for i in range(n)), default=0)
print(f"{sys.argv[3]} MIDI out: identical {n} bytes, start times within {dt} cycles "
      f"({' '.join(x[1] for x in a[:n])[:90]})")
PY
done
echo "C4 $NAME: PASS"
