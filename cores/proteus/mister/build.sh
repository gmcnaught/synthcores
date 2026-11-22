#!/usr/bin/env bash
# Build the Proteus MiSTer core (Proteus.qpf) into an RBF with Quartus Lite 17.0,
# locally (quartus_sh on PATH) or in CI inside raetro/quartus:17.0.
# Usage: bash mister/build.sh   -> mister/output_files/Proteus.rbf
# Exits non-zero on a failed compile or negative setup/hold slack.
set -euo pipefail
cd "$(dirname "${BASH_SOURCE[0]}")"
for q in quartus_sh /opt/intelFPGA/quartus/bin/quartus_sh /c/intelFPGA_lite/17.0/quartus/bin64/quartus_sh.exe; do
    if command -v "$q" >/dev/null 2>&1; then QSH=$(command -v "$q"); break; fi
done
[ -n "${QSH:-}" ] || { echo "ERROR: quartus_sh not found" >&2; exit 127; }
"$QSH" --version | head -2
status=0
"$QSH" --flow compile Proteus 2>&1 | tee build.log || status=1
[ -f output_files/Proteus.fit.summary ] && cat output_files/Proteus.fit.summary
if [ -f output_files/Proteus.sta.rpt ]; then
    "$(dirname "$QSH")/quartus_sta" -t sta_paths.tcl > output_files/sta_paths.log 2>&1 || tail -5 output_files/sta_paths.log
    [ -f output_files/worst_paths.txt ] && head -80 output_files/worst_paths.txt
fi
if [ -f output_files/Proteus.sta.summary ]; then
    cat output_files/Proteus.sta.summary
    if grep -E "^Slack *: *-" output_files/Proteus.sta.summary >/dev/null; then echo "ERROR: negative slack" >&2; status=1; fi
fi
[ -f output_files/Proteus.rbf ] || status=1
exit $status
