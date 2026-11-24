#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Build a core's MiSTer RBF with Quartus 17.0 on the Windows build host over SSH, from the
# working tree as it is (tracked files, uncommitted edits included).
#
#   [SEED=N] scripts/build-hosts/winbuild.sh CORE [OUTDIR]    CORE: sc55 | synclavier | proteus | jv880 | cz101 | p2k
#
# WINBUILD (default "winbuild", an ~/.ssh/config host) must accept SSH key logins and run
# PowerShell, with Quartus Lite 17.0 at C:\intelFPGA_lite\17.0 and Git for Windows (bash, tar)
# installed; scripts/build-hosts/enable-windows-sshd.ps1 sets up the SSH side. Sends cores/CORE
# and modules/ (without the reference-model submodules; symlinks resolved, so mister/sys comes
# along).
#
# Every build is a full compile in its own folder, C:\synthbuild\<id>, removed afterwards
# (KEEP=1 keeps it). Quartus Lite has no Rapid Recompile or partial recompilation, so builds of
# different cores, or of one core with different seeds, run side by side. SEED=N overrides the
# fitter seed (the core's build.sh reads it as <CORE>_SEED). The core's mister/build.sh runs
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

# tracked files of the core and of modules/ (shared HDL: synthframe, roland, ...), without the
# reference-model submodules; -h stores what the symlinks point to
# (the P2K core also takes the CPU and sound RTL from cores/proteus/p2k/rtl)
EXTRA=()
[ "$CORE" = p2k ] && EXTRA=(cores/proteus/p2k/rtl)
(cd "$ROOT" && git ls-files -z -- "cores/$CORE" "${EXTRA[@]}" modules ':!modules/nuked-sc55' ':!modules/nuked-mt32' |
    COPYFILE_DISABLE=1 tar --null -T - -czhf "$TMP/src.tgz" --exclude "cores/$CORE/third_party/nuked-sc55")

case "$CORE" in
    proteus) BUILD="bash mister/build.sh" ;;
    *) BUILD="mister/build.sh _out" ;;
esac

# the script that runs on the host, under Git Bash
cat > "$TMP/remote.sh" <<REMOTE
set -u
W=/c/synthbuild/$ID
mkdir -p "\$W" && cd "\$W" || exit 1
tar -xzf /c/synthbuild/$ID-src.tgz && rm -f /c/synthbuild/$ID-src.tgz
cd cores/$CORE || exit 1
SC55_SEED='$SEED' SYNCLAVIER_SEED='$SEED' JV880_SEED='$SEED' $BUILD > mister/build.out 2>&1
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
find "$OUT/mister" \( -path "*_out*" -name "*.rbf" \) -o -name "Proteus.rbf" 2>/dev/null | sed 's/^/rbf: /'
echo "winbuild $ID: build.sh exit $status after $(( $(date +%s) - start )) s; output in $OUT"
exit "$status"
