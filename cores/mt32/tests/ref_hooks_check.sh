#!/bin/bash
# SPDX-License-Identifier: GPL-2.0-or-later
# The log hooks leave the reference unchanged: build ref/mt32_ref from the upstream sources
# without the hooks (HOOKS=0 ref/build.sh) and check the corpus hashes in tests/corpus.json.
# Needs MT32_ROM_DIR (exit 77, skipped, without it). Default cases: boot programs panel.
set -euo pipefail
core=$(cd "$(dirname "$0")/.." && pwd)
if [ -z "${MT32_ROM_DIR:-}" ]; then echo "SKIP: ref_hooks_check: MT32_ROM_DIR not set"; exit 77; fi
HOOKS=0 "$core/ref/build.sh" "$core/build/ref_nohooks" >/dev/null
if grep -q mt32h_ "$core/build/ref_nohooks/src/mt32.cpp"; then
    echo "ref_hooks_check: FAIL: the HOOKS=0 sources contain hooks"; exit 1
fi
# shellcheck disable=SC2086
python3 "$core/tools/corpus.py" check --renderer "$core/build/ref_nohooks/mt32_ref" --case ${*:-boot programs panel}
