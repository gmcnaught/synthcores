#!/usr/bin/env bash
# Build the standalone P2K sound renderer: build/p2k/model/p2k_render
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OUT="${1:-$HERE/../../build/p2k/model}"
mkdir -p "$OUT"
"${CXX:-c++}" -std=c++17 -O2 -Wall -Wextra -o "$OUT/p2k_render" "$HERE/p2k_render.cpp"
"${CXX:-c++}" -std=c++17 -O1 -Wall -Wextra -o "$OUT/test_model" "$HERE/test_model.cpp"
"$OUT/test_model" >&2
echo "$OUT/p2k_render"
