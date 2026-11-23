#!/usr/bin/env bash
# Regenerate mame/patches/0004-p2k-integer-sound-model.patch from a MAME tree that has 0001-0004
# applied and the current p2k/model/p2k_sound.h and p2k_flash.h copied in. The base (0002 + 0003) versions of the
# driver files are rebuilt from the patches themselves.
# Usage: mame/scripts/mkpatch0004.sh MAME_TREE
set -euo pipefail
CORE="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
MAME="$(cd "$1" && pwd)"
P="$CORE/mame/patches"
T="$(mktemp -d)"
trap 'rm -rf "$T"' EXIT
mkdir -p "$T/a" "$T/b/src/mame/emusys"
( cd "$T/a" && git init -q && git apply --include='src/mame/emusys/emup2k*' "$P/0002-emup2k-proteus2000-research.patch" \
    && git apply --include='src/mame/emusys/emup2k*' "$P/0003-p2k-bustrace-vector-reads.patch" )
for h in p2k_sound.h p2k_flash.h; do
    cmp -s "$CORE/p2k/model/$h" "$MAME/src/mame/emusys/$h" || { echo "$h in $MAME differs from p2k/model" >&2; exit 1; }
done
cp "$MAME/src/mame/emusys/emup2k.cpp" "$MAME/src/mame/emusys/p2k_sound.h" "$MAME/src/mame/emusys/p2k_flash.h" "$T/b/src/mame/emusys/"
rm -rf "$T/a/.git"
( cd "$T" && git diff --no-index --no-prefix a b > out.patch || true )
# --no-prefix keeps the a/ b/ directory names as the prefixes; make the headers name both sides
sed -e 's#^diff --git a/\(src/[^ ]*\) a/#diff --git a/\1 b/#' -e 's#^diff --git b/\(src/[^ ]*\) b/#diff --git a/\1 b/#' "$T/out.patch" > "$P/0004-p2k-integer-sound-model.patch"
grep '^diff' "$P/0004-p2k-integer-sound-model.patch"
