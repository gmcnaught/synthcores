#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Each core's mister/build.sh with a fake quartus_sh (no Quartus needed): the wrapper reaches
# modules/synthframe/target/mister/build.sh, SEED (and the older <PROJECT>_SEED) sets the QSF
# seed, the RBF lands in OUTDIR as <PROJECT>_20YYMMDD.rbf, and negative slack fails the build.
#
#   modules/synthframe/tests/build/run.sh
set -euo pipefail
ROOT=$(cd "$(dirname "$0")/../../../.." && pwd)
T=$(mktemp -d); trap 'rm -rf "$T"' EXIT
mkdir -p "$T/bin"
cat > "$T/bin/quartus_sh" <<'Q'
#!/usr/bin/env bash
[ "$1" = --version ] && { echo "fake quartus_sh 17.0"; exit 0; }
p=$3; mkdir -p output_files
echo "seed $(sed -n 's/^set_global_assignment -name SEED //p' "$p.qsf")" > output_files/seed
echo rbf > "output_files/$p.rbf"
printf 'Type  : Setup\nSlack : %s\n' "${FAKE_SLACK:-1.000}" > "output_files/$p.sta.summary"
touch "output_files/$p.sta.rpt"
Q
printf '#!/bin/sh\nexit 0\n' > "$T/bin/quartus_sta"
chmod +x "$T/bin/quartus_sh" "$T/bin/quartus_sta"
export PATH=$T/bin:$PATH
fails=0; n=0
check() { n=$((n + 1)); if eval "$2"; then echo "ok: $1"; else echo "FAIL: $1"; fails=$((fails + 1)); fi; }

mkdir -p "$T/repo/modules/synthframe/target/mister"
cp "$ROOT/modules/synthframe/target/mister/build.sh" "$T/repo/modules/synthframe/target/mister/"
for qsf in "$ROOT"/cores/*/mister/*.qsf; do
    c=$(basename "$(dirname "$(dirname "$qsf")")"); p=$(basename "$qsf" .qsf)
    m=$T/repo/cores/$c/mister
    mkdir -p "$m"; cp "$ROOT/cores/$c/mister/build.sh" "$qsf" "$m/"
    up=$(echo "$p" | tr '[:lower:]' '[:upper:]')
    SEED=7 "$m/build.sh" _out > "$T/$c.log" 2>&1 && st=0 || st=$?
    check "$c: SEED=7 builds" '[ $st = 0 ]'
    check "$c: QSF seed 7" 'grep -qx "seed 7" "$m/output_files/seed"'
    check "$c: _out/${p}_20*.rbf" 'ls "$m/_out/${p}"_20*.rbf >/dev/null 2>&1'
    env "${up}_SEED=9" "$m/build.sh" > "$T/$c.log" 2>&1
    check "$c: ${up}_SEED=9 still works" 'grep -qx "seed 9" "$m/output_files/seed"'
    FAKE_SLACK=-0.5 "$m/build.sh" > "$T/$c.log" 2>&1 && st=0 || st=$?
    check "$c: negative slack fails" '[ $st != 0 ]'
done
echo "$((n - fails))/$n checks passed"
[ "$fails" = 0 ]
