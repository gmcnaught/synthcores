#!/bin/sh
# SPDX-License-Identifier: GPL-2.0-or-later
# Build the reference renderer: ref/build.sh [OUTDIR]  (default build/ref)
# Copies modules/nuked-mt32 with the log hooks (prepare_src.py) and compiles mt32_ref.
# HOOKS=0 builds from the upstream sources without the hooks (the log options then do nothing).
set -eu
here=$(cd "$(dirname "$0")" && pwd)
core=$(dirname "$here")
root=$(cd "$core/../.." && pwd)
out=${1:-$core/build/ref}
src=$out/src
nohooks=""; [ "${HOOKS:-1}" = 0 ] && nohooks=--no-hooks
python3 "$here/prepare_src.py" "$root/modules/nuked-mt32" "$src" $nohooks
CXX=${CXX:-c++}
$CXX -std=c++17 -O2 -w -I"$src" -I"$src/mame" \
    "$src/mt32.cpp" "$src/la32.cpp" "$src/rv2.cpp" "$src/lcd.cpp" \
    "$src/mame/mcs96.cpp" "$src/mame/i8x9x.cpp" "$here/mt32_ref.cpp" -o "$out/mt32_ref"
echo "$out/mt32_ref"
