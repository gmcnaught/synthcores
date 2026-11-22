#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Build a core's MiSTer RBF with Quartus 17.0 on the Windows build host over SSH, from the
# working tree as it is (tracked files, uncommitted edits included).
#
#   scripts/build-hosts/winbuild.sh CORE [OUTDIR]       CORE: sc55 | synclavier | proteus
#
# WINBUILD (default "winbuild", an ~/.ssh/config host) must accept SSH key logins and run
# PowerShell, with Quartus Lite 17.0 at C:\intelFPGA_lite\17.0 and Git for Windows (bash, tar)
# installed; scripts/build-hosts/enable-windows-sshd.ps1 sets up the SSH side. Copies
# cores/CORE and modules/synthframe (symlinks resolved: mister/sys) to
# C:\synthbuild\<id>, runs the core's mister/build.sh under Git Bash, and copies back the
# RBF, the Quartus reports and build.log to OUTDIR (default out/winbuild/CORE-<id>). Exits with
# build.sh's status (non-zero on a failed compile or negative slack). KEEP=1 leaves the build
# folder on the host.
set -euo pipefail
ROOT=${SYNTHROOT:-$(cd "$(dirname "$0")/../.." && pwd)}   # SYNTHROOT: build another checkout
CORE=${1:?usage: winbuild.sh CORE [OUTDIR]}
[ -d "$ROOT/cores/$CORE/mister" ] || { echo "no core $CORE" >&2; exit 2; }
HOST=${WINBUILD:-winbuild}
ID=$(date +%Y%m%d-%H%M%S)-$CORE-$$
OUT=${2:-$ROOT/out/winbuild/$CORE-$ID}
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT
mkdir -p "$OUT"

# tracked files of the core and synthframe; -h stores what the symlinks point to
(cd "$ROOT" && git ls-files -z -- "cores/$CORE" modules/synthframe |
    tar --null -T - -czhf "$TMP/src.tgz" --exclude "cores/$CORE/third_party/nuked-sc55")
echo "winbuild $ID: $(du -h "$TMP/src.tgz" | cut -f1) of sources -> $HOST:C:/synthbuild/$ID"

ssh -o BatchMode=yes "$HOST" "New-Item -ItemType Directory -Force -Path C:\\synthbuild\\$ID | Out-Null"
scp -q "$TMP/src.tgz" "$HOST:C:/synthbuild/$ID/src.tgz"

case "$CORE" in
    proteus) BUILD="bash mister/build.sh" ;;
    *) BUILD="mister/build.sh _out" ;;
esac
# Git Bash runs build.sh (it finds quartus_sh on PATH or at /c/intelFPGA_lite/17.0); the
# reports go into out.tgz for the trip back.
set +e
ssh -o BatchMode=yes "$HOST" "cd C:\\synthbuild\\$ID; tar -xzf src.tgz; \$env:SC55_SEED='${SC55_SEED:-}'; \$env:SYNCLAVIER_SEED='${SYNCLAVIER_SEED:-}';
    & 'C:\\Program Files\\Git\\bin\\bash.exe' -c 'cd cores/$CORE && $BUILD > mister/build.out 2>&1; echo \$? > mister/build.status';
    tar -czf out.tgz -C cores/$CORE mister/build.out mister/build.status mister/build.log mister/_out mister/output_files 2>\$null;
    exit 0"
set -e
scp -q "$HOST:C:/synthbuild/$ID/out.tgz" "$TMP/out.tgz"
tar -xzf "$TMP/out.tgz" -C "$OUT" 2>/dev/null || true
[ "${KEEP:-0}" = 1 ] || ssh -o BatchMode=yes "$HOST" "Remove-Item -Recurse -Force C:\\synthbuild\\$ID"

status=$(tr -d '\r\n ' < "$OUT/mister/build.status" 2>/dev/null || echo 1)
grep -E "Logic utilization|Total registers|Total block memory bits|Total DSP" "$OUT"/mister/output_files/*.fit.summary 2>/dev/null || true
grep -E "^Slack|^Type" "$OUT"/mister/output_files/*.sta.summary 2>/dev/null | paste - - | sort -t: -k3 -n | head -3 || true
find "$OUT/mister" -name "*.rbf" -path "*_out*" -o -name "Proteus.rbf" 2>/dev/null | sed 's/^/rbf: /'
echo "winbuild $ID: build.sh exit $status; output in $OUT"
exit "$status"
