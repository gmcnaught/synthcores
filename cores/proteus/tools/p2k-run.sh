#!/usr/bin/env bash
# Run the proteus2k research machine headless for WALL seconds of host time, log capped at ~200 MB.
# Usage: tools/p2k-run.sh WALL OUTDIR [extra MAME args]
set -uo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
wall=$1; out=$2; shift 2
mkdir -p "$out"; cd "$out"; rm -f error.log
( ulimit -f 400000; exec "$ROOT/tools/mame-headless.sh" proteus2k -log "$@" ) > mame.out 2>&1 &
pid=$!
for ((i = 0; i < wall; i++)); do kill -0 $pid 2>/dev/null || break; sleep 1; done
kill -9 $pid 2>/dev/null; wait $pid 2>/dev/null
ls -la error.log 2>/dev/null
