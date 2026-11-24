#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-3.0-or-later
# Standalone Quartus 17 build of a P2K part on the Windows build host (WINBUILD, default
# "winbuild"; see scripts/build-hosts/winbuild.sh): PROJECT p2k_snd (the sound engine, p2k_snd.qsf)
# or p2k_cpu (TG68K + MCF5206e peripherals, p2k_cpu.qsf). Reports come back to OUTDIR (default
# build/p2k/syn/<id>); prints the fit summary and the worst slacks. Exits non-zero on a failed
# compile or negative setup/hold slack.
#   p2k/syn/winbuild_syn.sh PROJECT [OUTDIR]
set -euo pipefail
CORE="$(cd "$(dirname "$0")/../.." && pwd)"
ROOT="$(cd "$CORE/../.." && pwd)"
HOST=${WINBUILD:-winbuild}
P=${1:?usage: winbuild_syn.sh p2k_snd|p2k_cpu [OUTDIR]}
ID=$(date +%Y%m%d-%H%M%S)-$P-$$
OUT=${2:-$CORE/build/p2k/syn/$ID}
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT
mkdir -p "$OUT"
SSH=(ssh -o BatchMode=yes -o ControlPath=none "$HOST")
(cd "$ROOT" && COPYFILE_DISABLE=1 tar --no-xattrs -czf "$TMP/src.tgz" cores/proteus/p2k/rtl cores/proteus/p2k/syn \
    cores/proteus/p2k/sim/p2k_snd_bench.sv modules/synthframe/hdl/mem)
cat > "$TMP/remote.sh" <<REMOTE
set -u
W=/c/synthbuild/$ID
mkdir -p "\$W" && cd "\$W" && tar -xzf /c/synthbuild/$ID-src.tgz && rm -f /c/synthbuild/$ID-src.tgz
cd cores/proteus/p2k/syn
Q=/c/intelFPGA_lite/17.0/quartus/bin64
s=0
"\$Q/quartus_map" $P > map.log 2>&1 || s=1
[ \$s = 0 ] && { "\$Q/quartus_fit" $P > fit.log 2>&1 || s=2; }
[ \$s = 0 ] && { "\$Q/quartus_sta" $P > sta.log 2>&1 || s=3; }
[ \$s = 0 ] && "\$Q/quartus_sta" -t sta_paths.tcl $P > sta_paths.log 2>&1
echo \$s > status
tar -czf /c/synthbuild/$ID-out.tgz status map.log fit.log sta.log output_files/*.rpt output_files/*.summary 2>/dev/null
cd /c/synthbuild && rm -rf "\$W"
REMOTE
echo "winbuild_syn $ID -> $HOST"
"${SSH[@]}" "New-Item -ItemType Directory -Force -Path C:\\synthbuild | Out-Null"
scp -q -o ControlPath=none "$TMP/src.tgz" "$HOST:C:/synthbuild/$ID-src.tgz"
scp -q -o ControlPath=none "$TMP/remote.sh" "$HOST:C:/synthbuild/$ID-remote.sh"
start=$(date +%s)
"${SSH[@]}" "& 'C:\\Program Files\\Git\\bin\\bash.exe' /c/synthbuild/$ID-remote.sh; Remove-Item C:\\synthbuild\\$ID-remote.sh" || true
scp -q -o ControlPath=none "$HOST:C:/synthbuild/$ID-out.tgz" "$TMP/out.tgz" && "${SSH[@]}" "Remove-Item C:\\synthbuild\\$ID-out.tgz"
tar -xzf "$TMP/out.tgz" -C "$OUT"
status=$(tr -d '\r\n ' < "$OUT/status")
grep -hE "^Error" "$OUT"/map.log "$OUT"/fit.log 2>/dev/null | head -20 || true
grep -E "Logic utilization|Total registers|Total block memory bits|Total RAM Blocks|Total DSP" "$OUT"/output_files/$P.fit.summary 2>/dev/null || true
grep -A1 -E "^Type.*(Setup|Hold)" "$OUT"/output_files/$P.sta.summary 2>/dev/null | grep -v "^--" | paste - - |
    sed -E 's/Type *: //; s/[[:space:]]+Slack *: / slack /' || true
echo "winbuild_syn $ID: status $status after $(( $(date +%s) - start )) s; output in $OUT"
neg=$(grep -E "^Slack" "$OUT"/output_files/$P.sta.summary 2>/dev/null | awk '$3 < 0' | wc -l | tr -d ' ')
[ "$status" = 0 ] && [ "${neg:-1}" = 0 ]
