#!/usr/bin/env bash
# license:BSD-3-Clause
# uPD933 gate (CZ-101 milestone C3): builds the C++ reference and the Verilator bench, then
#   1. regenerates the ROM tables with MAME's device_start code and compares them with
#      cores/cz101/hdl/upd933/*.hex;
#   2. runs the HDL against the reference on synthetic stimuli (SECONDS each, default 40):
#      samples, IRQ change ticks and read data must be identical;
#   3. runs one stimulus with a slow 10 MHz time base (--real) and early requests, and three runs
#      with ext_timer = 1 (the bench fires the IRQ timer as the board would);
#   4. self-tests the MAME-log path (log -> upd933_log2ev.py -> upd933_replay).
# Optional: UPD933_LOG=<CZ_SND_LOG file> UPD933_SAMPLES=<CZ_SAMPLES file> also replays a real
# MAME log through the reference and compares with MAME's samples.
# Prints PASS/FAIL, sample counts and a SHA-256 over the HDL's samples.
set -euo pipefail

here="$(cd "$(dirname "$0")" && pwd)"
core="$(cd "$here/.." && pwd)"
out="${UPD933_OUT:-$core/out/sim}"     # build dir of cores/cz101/sim (shared with the CPU benches)
secs="${SECONDS_PER_STIM:-40}"
py="$(command -v pypy3 || command -v python3)"
if command -v sha256sum >/dev/null; then sha() { sha256sum "$@"; }; else sha() { shasum -a 256 "$@"; }; fi
jobs="$(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 4)"

gen=()
command -v ninja >/dev/null && gen=(-G Ninja)
cmake -Wno-dev -S "$core/sim" -B "$out" "${gen[@]}" -DCMAKE_BUILD_TYPE=Release >/dev/null
cmake --build "$out" -j "$jobs" --target upd933_tables upd933_stimgen upd933_replay upd933_bench >/dev/null

fail=0
out_bin="$out"
out="$out/upd933"
mkdir -p "$out/stim" "$out/tables" "$out/s32"

# 1. tables
"$out_bin/upd933_tables" "$out/tables" >/dev/null
for t in cosine pitch pitch_fine volume; do
	if ! cmp -s "$out/tables/upd933_$t.hex" "$core/hdl/upd933/upd933_$t.hex"; then
		echo "FAIL table upd933_$t.hex differs from device_start output"; fail=1
	fi
done
[ $fail = 0 ] && echo "tables: 4 files identical to device_start output"

# 2. synthetic stimuli, fast time base
stims=()
for seed in 1 2 3 4 5 6; do
	opt=""; [ $seed -ge 5 ] && opt="--order"
	"$out_bin/upd933_stimgen" $seed "$secs" "$out/stim/s$seed.ev" $opt | head -1
	stims+=("$out/stim/s$seed.ev")
done
if ! printf '%s\n' "${stims[@]}" | xargs -P "$jobs" -I{} "$out_bin/upd933_bench" --out "$out/s32" {} >"$out/bench.txt" 2>&1; then fail=1; fi
cat "$out/bench.txt"
grep -q '^DIFF' "$out/bench.txt" && fail=1

# 3. slow 10 MHz time base (5 clk per tick), requests up to 60 ticks early
"$out_bin/upd933_stimgen" 9 5 "$out/stim/r9.ev" | head -1
"$out_bin/upd933_bench" --real 5 --lead 60 "$out/stim/r9.ev" || fail=1

# 3b. external IRQ timer (ext_timer = 1, bench plays the board)
for f in s1 s5; do "$out_bin/upd933_bench" --ext "$out/stim/$f.ev" >"$out/ext_$f.txt" 2>&1 & done
"$out_bin/upd933_bench" --ext --real 5 --lead 60 "$out/stim/r9.ev" >"$out/ext_r9.txt" 2>&1
wait
cat "$out"/ext_{s1,s5,r9}.txt >"$out/bench_ext.txt"
cat "$out/bench_ext.txt"
grep -q '^DIFF' "$out/bench_ext.txt" && fail=1
[ "$(grep -c '^MATCH' "$out/bench_ext.txt")" = 3 ] || fail=1

# 4. MAME-log path self-test
"$out_bin/upd933_stimgen" 7 5 "$out/stim/l7.ev" --order --mamelog "$out/stim/l7.log" "$out/stim/l7.s32" >/dev/null
"$py" "$core/sim/upd933/upd933_log2ev.py" "$out/stim/l7.log" "$out/stim/l7_log.ev" >/dev/null
"$out_bin/upd933_replay" "$out/stim/l7_log.ev" --samples "$out/stim/l7.s32" >"$out/replay.txt" || fail=1
echo "log self-test: $(tail -1 "$out/replay.txt")"

# optional real MAME log
if [ -n "${UPD933_LOG:-}" ]; then
	"$py" "$core/sim/upd933/upd933_log2ev.py" "$UPD933_LOG" "$out/stim/mame.ev"
	if [ -n "${UPD933_SAMPLES:-}" ]; then
		"$out_bin/upd933_replay" "$out/stim/mame.ev" --samples "$UPD933_SAMPLES" --out "$out/s32/mame_ref.s32" || fail=1
	else
		"$out_bin/upd933_replay" "$out/stim/mame.ev" --out "$out/s32/mame_ref.s32" || fail=1
	fi
fi

total=0
for s in 1 2 3 4 5 6; do total=$((total + $(wc -c <"$out/s32/s$s.s32") / 4)); done
hash="$(cat "$out"/s32/s{1,2,3,4,5,6}.s32 | sha | cut -d' ' -f1)"
echo "HDL samples: $total over 6 stimuli ($((total / 40000)) s), sha256 $hash"
if [ $fail = 0 ]; then echo "PASS"; else echo "FAIL"; exit 1; fi
