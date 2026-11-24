#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-3.0-or-later
# Standalone Quartus 17 build of the P2K sound engine (p2k/syn/p2k_snd.qsf) on the Windows build
# host (WINBUILD, default "winbuild"; see scripts/build-hosts/winbuild.sh). Reports come back to
# OUTDIR (default build/p2k/syn/<id>); prints the fit summary and the worst slacks. Exits non-zero
# on a failed compile or negative setup/hold slack.
#   p2k/syn/winbuild_snd.sh [OUTDIR]
set -euo pipefail
CORE="$(cd "$(dirname "$0")/../.." && pwd)"
ROOT="$(cd "$CORE/../.." && pwd)"
HOST=${WINBUILD:-winbuild}
ID=$(date +%Y%m%d-%H%M%S)-p2ksnd-$$
OUT=${1:-$CORE/build/p2k/syn/$ID}
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT
mkdir -p "$OUT"
SSH=(ssh -o BatchMode=yes -o ControlPath=none "$HOST")
(cd "$ROOT" && COPYFILE_DISABLE=1 tar --no-xattrs -czf "$TMP/src.tgz" cores/proteus/p2k/rtl/snd cores/proteus/p2k/syn \
    cores/proteus/p2k/sim/p2k_snd_bench.sv modules/synthframe/hdl/mem)
cat > "$TMP/remote.sh" <<REMOTE
set -u
W=/c/synthbuild/$ID
mkdir -p "\$W" && cd "\$W" && tar -xzf /c/synthbuild/$ID-src.tgz && rm -f /c/synthbuild/$ID-src.tgz
cd cores/proteus/p2k/syn
Q=/c/intelFPGA_lite/17.0/quartus/bin64
s=0
"\$Q/quartus_map" p2k_snd > map.log 2>&1 || s=1
[ \$s = 0 ] && { "\$Q/quartus_fit" p2k_snd > fit.log 2>&1 || s=2; }
[ \$s = 0 ] && { "\$Q/quartus_sta" p2k_snd > sta.log 2>&1 || s=3; }
[ \$s = 0 ] && "\$Q/quartus_sta" -t sta_paths.tcl > sta_paths.log 2>&1
echo \$s > status
tar -czf /c/synthbuild/$ID-out.tgz status map.log fit.log sta.log output_files/*.rpt output_files/*.summary 2>/dev/null
cd /c/synthbuild && rm -rf "\$W"
REMOTE
echo "winbuild_snd $ID -> $HOST"
"${SSH[@]}" "New-Item -ItemType Directory -Force -Path C:\\synthbuild | Out-Null"
scp -q -o ControlPath=none "$TMP/src.tgz" "$HOST:C:/synthbuild/$ID-src.tgz"
scp -q -o ControlPath=none "$TMP/remote.sh" "$HOST:C:/synthbuild/$ID-remote.sh"
start=$(date +%s)
"${SSH[@]}" "& 'C:\\Program Files\\Git\\bin\\bash.exe' /c/synthbuild/$ID-remote.sh; Remove-Item C:\\synthbuild\\$ID-remote.sh" || true
scp -q -o ControlPath=none "$HOST:C:/synthbuild/$ID-out.tgz" "$TMP/out.tgz" && "${SSH[@]}" "Remove-Item C:\\synthbuild\\$ID-out.tgz"
tar -xzf "$TMP/out.tgz" -C "$OUT"
status=$(tr -d '\r\n ' < "$OUT/status")
grep -hE "^Error" "$OUT"/map.log "$OUT"/fit.log 2>/dev/null | head -20 || true
grep -E "Logic utilization|Total registers|Total block memory bits|Total RAM Blocks|Total DSP" "$OUT"/output_files/p2k_snd.fit.summary 2>/dev/null || true
grep -A1 -E "^Type.*(Setup|Hold)" "$OUT"/output_files/p2k_snd.sta.summary 2>/dev/null | grep -v "^--" | paste - - |
    sed -E 's/Type *: //; s/[[:space:]]+Slack *: / slack /' || true
echo "winbuild_snd $ID: status $status after $(( $(date +%s) - start )) s; output in $OUT"
neg=$(grep -E "^Slack" "$OUT"/output_files/p2k_snd.sta.summary 2>/dev/null | awk '$3 < 0' | wc -l | tr -d ' ')
[ "$status" = 0 ] && [ "${neg:-1}" = 0 ]
