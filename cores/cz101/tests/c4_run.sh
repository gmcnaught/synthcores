#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# C4: the CZ-101 board HDL against MAME's cz101 with the real firmware and a MIDI script.
#   tests/c4_run.sh MIDI_SCRIPT N_INSTRUCTIONS [PANEL_SCRIPT]
# Needs CZ101_ROM_DIR (MAME's cz101 set), HD44780_ROM (hd44780_a00.bin), the patched MAME
# (../mame-cz101, or MAME_CZ101) and out/sim/cz101_bench (or CZ101_BENCH). Without the ROMs or
# MAME it prints "SKIP: ..." and exits 77. MAME's midiin starts playback 10 s after power-on.
# Compares the CPU trace (registers, memory writes, interrupts; all N instructions) and the raw
# 40 kHz samples (tools/s32_cmp.py: none missing, every one MAME made identical).
# Then the MiSTer core's live-MIDI path: MAME's MIDI-in decoded to bytes (tools/rxlog_bytes.py)
# and sent by the board's serializer (CZ_MIDI_BYTES) must give the same trace and samples.
# PANEL_SCRIPT (tools/panel_sched.py): panel buttons and the pitch wheel, applied at the same CPU
# cycles in MAME and the HDL. MIDI out: the bytes decoded from MAME's TxD line and the HDL's
# (tools/txlog_bytes.py) must be identical, each starting within 32 CPU cycles (a third of a bit)
# of MAME's: MAME moves TxD at its 2 MHz serial clock edges, the board at the next instruction
# boundary.
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
PY="$(command -v pypy3 || command -v python3)"   # comparison scripts run under pypy3 when present
SCRIPT=$1; N=$2
BENCH="${CZ101_BENCH:-$HERE/out/sim/cz101_bench}"
MAME="${MAME_CZ101:-$(cd "$HERE/../../.." && pwd)/mame-cz101/cz101}"
[ -n "${CZ101_ROM_DIR:-}" ] && [ -f "$CZ101_ROM_DIR/hn613256ps40.bin" ] || { echo "SKIP: CZ101_ROM_DIR (MAME's cz101 set) not set or empty"; exit 77; }
[ -n "${HD44780_ROM:-}" ] && [ -f "$HD44780_ROM" ] || { echo "SKIP: HD44780_ROM (hd44780_a00.bin) not set or missing"; exit 77; }
[ -x "$MAME" ] || { echo "SKIP: no patched MAME at $MAME (MAME_CZ101)"; exit 77; }
[ -x "$BENCH" ] || { echo "FAIL: no $BENCH (build cores/cz101/sim)"; exit 1; }
export MAME_CZ101="$MAME"
NAME=$(basename "$SCRIPT" .txt)
W="${C4_WORK:-$HERE/out/c4}/$NAME"; rm -rf "$W"; mkdir -p "$W/rom/cz101" "$W/rom/hd44780"
cp "${CZ101_ROM_DIR:?}"/*.bin "$W/rom/cz101/"
cp "${HD44780_ROM:?}" "$W/rom/hd44780/"
"$PY" "$HERE/../../modules/synthframe/tools/midi/mkmidi.py" "$SCRIPT" "$W/in.mid"
if [ -n "${3:-}" ]; then
    "$PY" "$HERE/tools/panel_sched.py" "$3" "$W/panel.keys" "$W/panel.wheel"
    export CZ_KEYS="$W/panel.keys" CZ_WHEEL="$W/panel.wheel"
fi
SECS=$(( N / 300000 + 4 ))
CZ_POWER_ON=1 CZ_TRACE="$W/mame.trace" CZ_TRACE_LIMIT=$N CZ_SAMPLES="$W/mame.s32" \
    CZ_RX_LOG="$W/mame.rx" CZ_SND_LOG="$W/mame.snd" CZ_TX_LOG="$W/mame.tx" \
    "$HERE/tools/mame-cz101.sh" "$W/rom" "$W/mame" -seconds_to_run $SECS -midiin "$W/in.mid" >/dev/null 2>&1 || true
CZ_TX_LOG="$W/hdl.tx" "$BENCH" "$W/rom/cz101" "$N" "$W/hdl.trace" "$W/hdl.s32" "$W/mame.rx" | tee "$W/hdl.txt"
"$PY" "$HERE/tools/trace_cmp.py" "$W/mame.trace" "$W/hdl.trace" "$N"
"$PY" "$HERE/tools/s32_cmp.py" "$W/mame.s32" "$W/hdl.s32" "$W/hdl.txt" samples
"$PY" "$HERE/tools/rxlog_bytes.py" "$W/mame.rx" > "$W/live.bytes"
CZ_MIDI_BYTES="$W/live.bytes" CZ_TX_LOG="$W/live.tx" "$BENCH" "$W/rom/cz101" "$N" "$W/live.trace" "$W/live.s32" | tee "$W/live.txt"
"$PY" "$HERE/tools/trace_cmp.py" "$W/mame.trace" "$W/live.trace" "$N"
"$PY" "$HERE/tools/s32_cmp.py" "$W/mame.s32" "$W/live.s32" "$W/live.txt" "live MIDI samples"
# MIDI out: MAME's bytes stop at its last instruction, so compare up to the shorter stream
"$PY" "$HERE/tools/txlog_bytes.py" --mame "$W/mame.tx" > "$W/mame.txbytes"
for k in hdl live; do
    "$PY" "$HERE/tools/txlog_bytes.py" "$W/$k.tx" > "$W/$k.txbytes"
    "$PY" - "$W/mame.txbytes" "$W/$k.txbytes" "$k" <<'PY'
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
