#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Offline test of synth-mister backup / install / restore / release on a fake SD card: ssh and
# scp are replaced by shims that run the commands on this machine with /media/fat mapped to a
# temp folder. Nothing touches a real MiSTer.
#
#   modules/synthframe/tests/synth-mister/run.sh [SYNTH_MISTER]   (default: bin/synth-mister)
#
# The fake card copies the state found on the MiSTer on 2026-11-26: the oldest backup (20261122)
# predates the JV-880/CZ-101/MT-32/P2K releases, the newest has no MT-32 or P2K RBF, a backup
# folder without MANIFEST, and an SC-55 release newer than every backup.
set -euo pipefail
HERE=$(cd "$(dirname "$0")" && pwd)
SM=${1:-$HERE/../../bin/synth-mister}
SM=$(cd "$(dirname "$SM")" && pwd)/$(basename "$SM")
T=$(mktemp -d); trap 'rm -rf "$T"' EXIT
SD=$T/sd
mkdir -p "$T/bin"
cat > "$T/bin/ssh" <<SHIM
#!/usr/bin/env bash
while [ \$# -gt 0 ]; do case "\$1" in -o) shift 2 ;; -*) shift ;; *) shift; break ;; esac; done
c="\$*"; c=\${c//\\/media\\/fat/$SD}; c=\${c//\\/dev\\/MiSTer_cmd/$T/MiSTer_cmd}
exec bash -c "\$c"
SHIM
cat > "$T/bin/scp" <<SHIM
#!/usr/bin/env bash
a=()
for x in "\$@"; do case "\$x" in -q) ;; -o|ControlPath=*) ;; *) x=\${x#fake@mister:}; a+=("\${x//\\/media\\/fat/$SD}") ;; esac; done
exec cp "\${a[@]}"
SHIM
# fixed date, so that a test build installed "today" has the same name as the SC-55 release
cat > "$T/bin/date" <<'SHIM'
#!/bin/sh
case "$1" in +%Y%m%d) echo 20261126 ;; +%Y%m%d-%H%M%S) echo 20261126-120000 ;; *) exec /bin/date "$@" ;; esac
SHIM
chmod +x "$T/bin/ssh" "$T/bin/scp" "$T/bin/date"
export PATH=$T/bin:$PATH MISTER=fake@mister SYNTH_MISTER_FAT=$SD SYNTH_MISTER_CMD=$T/MiSTer_cmd

fails=0; checks=0
ok() { checks=$((checks + 1)); echo "ok: $*"; }
bad() { checks=$((checks + 1)); fails=$((fails + 1)); echo "FAIL: $*"; }
mk() { mkdir -p "$(dirname "$1")"; printf '%s\n' "$2" > "$1"; }   # PATH CONTENT
has() { [ -f "$SD/$1" ] && [ "$(cat "$SD/$1")" = "$2" ] && ok "$1 = $2" || bad "$1 should be '$2' (is '$(cat "$SD/$1" 2>/dev/null || echo missing)')"; }
gone() { [ ! -e "$SD/$1" ] && ok "$1 removed" || bad "$1 should be removed"; }
sm() { echo "\$ synth-mister $*"; "$SM" "$@"; }

# --- the card -----------------------------------------------------------------------------
B1=$SD/_synth_backup/20261122; B2=$SD/_synth_backup/20261125
for f in _Devices/SC55_20261118.rbf _Other/Proteus_20261120.rbf _Computer/Synclavier_20261121.rbf; do
    mk "$B1/$f" "old $(basename "$f")"; echo "$f" >> "$B1/MANIFEST"; done
mk "$B1/Proteus.mgl" '<rbf>_Other/Proteus</rbf>'; echo Proteus.mgl >> "$B1/MANIFEST"
for f in _Devices/SC55_20261124.rbf _Devices/Proteus_20261124.rbf _Devices/Synclavier_20261124.rbf \
         _Devices/SynclavierBN_20261124.rbf _Devices/JV880_20261125.rbf _Devices/CZ101_20261125.rbf; do
    mk "$B2/$f" "rel $(basename "$f")"; echo "$f" >> "$B2/MANIFEST"; done
mk "$SD/_synth_backup/20991231-pre/_Devices/SC55_20261101.rbf" "pre SC55"   # no MANIFEST: ignored
for f in _Devices/Proteus_20261124.rbf _Devices/Synclavier_20261124.rbf _Devices/SynclavierBN_20261124.rbf \
         _Devices/JV880_20261125.rbf _Devices/CZ101_20261125.rbf; do
    mk "$SD/$f" "rel $(basename "$f")"; done
mk "$SD/_Devices/SC55_20261126.rbf" "rel SC55_20261126.rbf"          # newer than every backup
mk "$SD/_Devices/MT32_20261125.rbf" "rel MT32_20261125.rbf"          # in no backup
mk "$SD/_Devices/P2K_20261125.rbf" "rel P2K_20261125.rbf"            # in no backup
mk "$SD/Proteus.mgl" '<rbf>_Devices/Proteus</rbf>'
mk "$SD/menu.rbf" menu
TODAY=20261126

echo "== 1. restore with no test build installed deletes nothing it cannot replace"
sm restore && bad "restore should report the unbacked SC-55 release" || ok "restore exit non-zero (SC-55 release not backed up)"
has _Devices/SC55_20261126.rbf "rel SC55_20261126.rbf"
has _Devices/JV880_20261125.rbf "rel JV880_20261125.rbf"
has _Devices/CZ101_20261125.rbf "rel CZ101_20261125.rbf"
has _Devices/MT32_20261125.rbf "rel MT32_20261125.rbf"
has _Devices/P2K_20261125.rbf "rel P2K_20261125.rbf"
has _Devices/Proteus_20261124.rbf "rel Proteus_20261124.rbf"
gone _Devices/SC55_20261118.rbf
gone _Devices/SC55_20261101.rbf

echo "== 2. install backs up first (P2K, MT-32 included), then replaces the core's RBF"
sm install p2k <(echo "test p2k") || bad "install failed"
sm install sc55 <(echo "test sc55") || bad "install failed"
sm install jv880 <(echo "test jv880") || bad "install failed"
NEW=$(for d in "$SD"/_synth_backup/2*; do if [ -f "$d/MANIFEST" ]; then echo "$d"; fi; done | sort | tail -1)
[ "$NEW" != "$B2" ] && ok "new backup $(basename "$NEW")" || bad "install made no backup"
for f in _Devices/P2K_20261125.rbf _Devices/MT32_20261125.rbf _Devices/SC55_20261126.rbf \
         _Devices/JV880_20261125.rbf _Devices/CZ101_20261125.rbf Proteus.mgl; do
    grep -qxF "$f" "$NEW/MANIFEST" && ok "MANIFEST lists $f" || bad "MANIFEST misses $f"; done
grep -q "test" "$NEW/MANIFEST" && bad "MANIFEST lists a test build" || ok "no test build in MANIFEST"
grep -qx "_Devices/P2K_$TODAY.rbf" "$NEW/MANIFEST" && bad "test P2K backed up" || ok "test P2K not backed up"
has "_synth_backup/$(basename "$NEW")/_Devices/SC55_20261126.rbf" "rel SC55_20261126.rbf"
has "_Devices/P2K_$TODAY.rbf" "test p2k"
gone _Devices/P2K_20261125.rbf
has _Devices/SC55_20261126.rbf "test sc55"
out=$(sm backup); echo "$out"
case "$out" in *"up to date"*) ok "backup with test builds installed is up to date" ;; *) bad "backup made a copy" ;; esac

echo "== 3. restore one core"
sm restore jv880
has _Devices/JV880_20261125.rbf "rel JV880_20261125.rbf"
gone "_Devices/JV880_$TODAY.rbf"
has "_Devices/SC55_$TODAY.rbf" "test sc55"
has "_Devices/P2K_$TODAY.rbf" "test p2k"

echo "== 4. release returns the newest backed-up release"
sm release p2k "$T/rel.rbf" && [ "$(cat "$T/rel.rbf")" = "rel P2K_20261125.rbf" ] && ok "release p2k" || bad "release p2k"
sm release sc55 "$T/rel.rbf" && [ "$(cat "$T/rel.rbf")" = "rel SC55_20261126.rbf" ] && ok "release sc55" || bad "release sc55"

echo "== 5. restore everything"
sm restore
has _Devices/P2K_20261125.rbf "rel P2K_20261125.rbf"
has _Devices/SC55_20261126.rbf "rel SC55_20261126.rbf"
has _Devices/MT32_20261125.rbf "rel MT32_20261125.rbf"
has _Devices/CZ101_20261125.rbf "rel CZ101_20261125.rbf"
has _Devices/SynclavierBN_20261124.rbf "rel SynclavierBN_20261124.rbf"
n=$(ls "$SD"/_Devices/*_"$TODAY".rbf 2>/dev/null | wc -l | tr -d ' ')
[ "$n" = 1 ] && ok "no test build left" || bad "$((n - 1)) test builds left"   # SC55_20261126 is the release
grep -q 'load_core .*/menu.rbf' "$T/MiSTer_cmd" && ok "menu loaded" || bad "menu not loaded"

echo "== 6. restore --from an older backup, then back"
sm restore --from 20261122 sc55
has _Devices/SC55_20261118.rbf "old SC55_20261118.rbf"
gone _Devices/SC55_20261126.rbf
sm restore sc55
has _Devices/SC55_20261126.rbf "rel SC55_20261126.rbf"
gone _Devices/SC55_20261118.rbf

echo "== 7. a test build with no backed-up release is kept"
rm "$SD/_Devices/MT32_20261125.rbf"
for d in "$SD"/_synth_backup/2*; do
    [ -f "$d/MANIFEST" ] && grep -v MT32 "$d/MANIFEST" > "$d/M" && mv "$d/M" "$d/MANIFEST"; done
sm install mt32 <(echo "test mt32") || bad "install failed"
sm restore mt32
has "_Devices/MT32_$TODAY.rbf" "test mt32"

echo "$((checks - fails))/$checks checks passed"
[ "$fails" = 0 ]
