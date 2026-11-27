#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Build a core's MiSTer RBF with Quartus 17.0 on the Windows build host over SSH, from the
# working tree as it is (tracked files, uncommitted edits included).
#
#   [SEED=N] scripts/build-hosts/winbuild.sh CORE [OUTDIR]    CORE: sc55 | synclavier | proteus | jv880 | mt32 | cz101 | p2k
#
# WINBUILD (default "winbuild", an ~/.ssh/config host) must accept SSH key logins and run
# PowerShell, with Quartus Lite 17.0 at C:\intelFPGA_lite\17.0 and Git for Windows (bash, tar)
# installed; scripts/build-hosts/enable-windows-sshd.ps1 sets up the SSH side. Sends cores/CORE,
# modules/ (without the reference-model submodules; symlinks resolved, so mister/sys comes
# along) and any other directory CORE's cfg/files.yaml uses (`synthcore deps CORE`, e.g. the
# P2K's CPU and sound RTL in cores/proteus/p2k/rtl). Needs PyYAML for synthcore.
#
# Every build is a full compile in its own folder, C:\synthbuild\<id>, removed afterwards
# (KEEP=1 keeps it). Quartus Lite has no Rapid Recompile or partial recompilation, so builds of
# different cores, or of one core with different seeds, run side by side. SEED=N overrides the
# fitter seed (every core's build.sh reads SEED). The core's mister/build.sh runs
# under Git Bash; the RBF, the Quartus reports and build.log come back to OUTDIR (default
# out/winbuild/CORE-<id>). Exits with build.sh's status (non-zero on a failed compile or
# negative slack).
set -euo pipefail
ROOT=${SYNTHROOT:-$(cd "$(dirname "$0")/../.." && pwd)}   # SYNTHROOT: build another checkout
CORE=${1:?usage: winbuild.sh CORE [OUTDIR]}
[ -d "$ROOT/cores/$CORE/mister" ] || { echo "no core $CORE" >&2; exit 2; }
HOST=${WINBUILD:-winbuild}
SEED=${SEED:-}
ID=$(date +%Y%m%d-%H%M%S)-$CORE${SEED:+-s$SEED}-$$
OUT=${2:-$ROOT/out/winbuild/$CORE-$ID}
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT
mkdir -p "$OUT"
SSH=(ssh -o BatchMode=yes -o ControlPath=none "$HOST")

# tracked files of the core, of the directories its files.yaml takes from other cores, and of
# modules/ (shared HDL: synthframe, roland, ...), without the reference-model submodules; -h
# stores what the symlinks point to
DEPS=$(python3 "$ROOT/modules/synthframe/bin/synthcore" deps "$CORE") || { echo "synthcore deps $CORE failed" >&2; exit 2; }
# shellcheck disable=SC2086  # one directory a line, no spaces
(cd "$ROOT" && git ls-files -z -- "cores/$CORE" $DEPS modules ':!modules/nuked-sc55' ':!modules/nuked-mt32' |
    COPYFILE_DISABLE=1 tar --null -T - -czhf "$TMP/src.tgz" --exclude "cores/$CORE/third_party/nuked-sc55")

# the script that runs on the host, under Git Bash
cat > "$TMP/remote.sh" <<REMOTE
set -u
W=/c/synthbuild/$ID
mkdir -p "\$W" && cd "\$W" || exit 1
tar -xzf /c/synthbuild/$ID-src.tgz && rm -f /c/synthbuild/$ID-src.tgz
cd cores/$CORE || exit 1
SEED='$SEED' bash mister/build.sh _out > mister/build.out 2>&1
echo \$? > mister/build.status
tar -czf /c/synthbuild/$ID-out.tgz mister/build.out mister/build.status mister/build.log \
    mister/_out mister/output_files 2>/dev/null
cd /c/synthbuild
if [ "${KEEP:-0}" != 1 ]; then rm -rf "\$W"; fi
exit 0
REMOTE

echo "winbuild $ID: $(du -h "$TMP/src.tgz" | cut -f1) of sources${SEED:+, seed $SEED} -> $HOST:C:/synthbuild/$ID"
"${SSH[@]}" "New-Item -ItemType Directory -Force -Path C:\\synthbuild | Out-Null"
scp -q -o ControlPath=none "$TMP/src.tgz" "$HOST:C:/synthbuild/$ID-src.tgz"
scp -q -o ControlPath=none "$TMP/remote.sh" "$HOST:C:/synthbuild/$ID-remote.sh"
start=$(date +%s)
"${SSH[@]}" "& 'C:\\Program Files\\Git\\bin\\bash.exe' /c/synthbuild/$ID-remote.sh; Remove-Item C:\\synthbuild\\$ID-remote.sh" || true
if scp -q -o ControlPath=none "$HOST:C:/synthbuild/$ID-out.tgz" "$TMP/out.tgz"; then
    "${SSH[@]}" "Remove-Item C:\\synthbuild\\$ID-out.tgz"
    tar -xzf "$TMP/out.tgz" -C "$OUT" 2>/dev/null || true
fi

status=$(tr -d '\r\n ' < "$OUT/mister/build.status" 2>/dev/null || echo 1)
grep -E "Logic utilization|Total registers|Total block memory bits|Total DSP" "$OUT"/mister/output_files/*.fit.summary 2>/dev/null || true
grep -E "^Slack|^Type" "$OUT"/mister/output_files/*.sta.summary 2>/dev/null | paste - - | sort -t: -k3 -n | head -3 || true
find "$OUT/mister" -path "*_out*" -name "*.rbf" 2>/dev/null | sed 's/^/rbf: /'
echo "winbuild $ID: build.sh exit $status after $(( $(date +%s) - start )) s; output in $OUT"
exit "$status"
