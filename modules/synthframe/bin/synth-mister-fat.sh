#!/bin/sh
# SPDX-License-Identifier: MIT
# The SD-card side of synth-mister: backup, restore, install and release bookkeeping under
# FAT/_synth_backup. synth-mister sends this file over ssh and runs it with `sh -s` on the MiSTer
# (busybox/POSIX sh: no bash features, no GNU-only options); the offline test runs it on a fake
# tree. File names must not contain spaces (none of the cores' files do).
#
#   sh synth-mister-fat.sh FAT TABLE COMMAND [ARGS]
#
#   TABLE   "core=dir/Prefix ..." from synth-mister, e.g. "sc55=_Devices/SC55 p2k=_Devices/P2K"
#
#   backup                  copy every core's installed RBFs (<Prefix>_*.rbf and variants such as
#                           SynclavierBN_*.rbf) and root MGLs (<Prefix>*.mgl) that are not test
#                           builds to FAT/_synth_backup/<YYYYMMDD-HHMMSS>/, with a MANIFEST (one
#                           path per line, written last). Skipped when the newest backup already
#                           holds the same files. A core whose only installed RBF is a test build
#                           keeps its RBF from the previous backup.
#   restore [--from NAME] [CORE...]
#                           for each core (default: all), from the newest backup that has a
#                           MANIFEST (or backup NAME): if the backup has the core's <Prefix>_*.rbf,
#                           replace the installed <Prefix>_*.rbf with it; otherwise leave the core
#                           alone. An installed RBF that is neither a test build nor in any backup
#                           is never removed (that core is skipped, exit 1). Variant RBFs and MGLs
#                           are copied back only where missing.
#   install CORE INCOMING   back up (as above), then replace the core's <Prefix>_2*.rbf with
#                           INCOMING as <Prefix>_<YYYYMMDD>.rbf and record it in
#                           _synth_backup/INSTALLED (path, cksum, size) as a test build
#   release CORE            print the path of the newest backed-up <Prefix>_2*.rbf of CORE
#   list                    backups (with MANIFEST) and recorded test builds
# `local` is not POSIX but busybox ash, dash and bash all have it
# shellcheck disable=SC3043
set -eu
FAT=$1; TABLE=$2; CMD=$3; shift 3
BK=$FAT/_synth_backup
LEDGER=$BK/INSTALLED
cd "$FAT"

die() { echo "$*" >&2; exit 1; }
prefix_of() {   # CORE -> dir/Prefix
    local e
    for e in $TABLE; do [ "${e%%=*}" = "$1" ] && { echo "${e#*=}"; return 0; }; done
    echo "unknown core: $1" >&2; exit 2
}
cores() { for e in $TABLE; do echo "${e%%=*}"; done; }
# backups made when Proteus lived in _Other and Synclavier in _Computer restore to _Devices
target_of() { echo "$1" | sed 's#^_Other/#_Devices/#; s#^_Computer/#_Devices/#'; }
sum_of() { cksum < "$1" | awk '{print $1 " " $2}'; }
backups() {     # backup folders with a MANIFEST, oldest first
    local d
    for d in "$BK"/2*; do if [ -f "$d/MANIFEST" ]; then echo "$d"; fi; done | sort
}
newest() { backups | tail -n 1; }
core_files() {  # CORE -> installed RBFs (main and variants) and root MGLs
    local p b f
    p=$(prefix_of "$1"); b=${p##*/}
    for f in "$p"*_*.rbf "$b"*.mgl; do if [ -f "$f" ]; then echo "$f"; fi; done | sort -u
}
is_test() {     # FILE: installed by `install` and unchanged since
    [ -f "$LEDGER" ] && grep -qxF "$1 $(sum_of "$1")" "$LEDGER"
}
in_backup() {   # FILE: an identical copy is in some backup
    local t d m
    t=$(target_of "$1")
    for d in $(backups); do
        while read -r m; do
            [ "$(target_of "$m")" = "$t" ] && cmp -s "$d/$m" "$1" && return 0
        done < "$d/MANIFEST"
    done
    return 1
}
drop_ledger() { # PREFIX: forget the test builds of this core
    [ -f "$LEDGER" ] || return 0
    grep -v "^$1_" "$LEDGER" > "$LEDGER.tmp" || true
    mv "$LEDGER.tmp" "$LEDGER"
}

do_backup() {
    local prev list c p main tested f m t s a b d n
    mkdir -p "$BK"
    prev=$(newest)
    list=$BK/.list.$$
    : > "$list"
    for c in $(cores); do
        p=$(prefix_of "$c"); main=0; tested=0
        for f in $(core_files "$c"); do
            if is_test "$f"; then tested=1; continue; fi
            printf '%s %s\n' "$f" "$FAT/$f" >> "$list"
            case "$f" in "$p"_*.rbf) main=1 ;; esac
        done
        # the core's release was replaced by a test build: keep the release from the last backup
        if [ $tested = 1 ] && [ $main = 0 ] && [ -n "$prev" ]; then
            while read -r m; do
                t=$(target_of "$m")
                case "$t" in "$p"_*.rbf) printf '%s %s\n' "$t" "$prev/$m" >> "$list" ;; esac
            done < "$prev/MANIFEST"
        fi
    done
    if [ ! -s "$list" ]; then rm -f "$list"; echo "backup: nothing installed"; return 0; fi
    if [ -n "$prev" ]; then
        a=$(while read -r t s; do echo "$t $(sum_of "$s")"; done < "$list" | sort)
        b=$(while read -r m; do echo "$(target_of "$m") $(sum_of "$prev/$m")"; done < "$prev/MANIFEST" | sort)
        if [ "$a" = "$b" ]; then rm -f "$list"; echo "backup up to date: $prev"; return 0; fi
    fi
    d=$BK/$(date +%Y%m%d-%H%M%S); n=1
    while [ -e "$d" ]; do n=$((n + 1)); d=$BK/$(date +%Y%m%d-%H%M%S)-$n; done
    mkdir -p "$d"
    while read -r t s; do
        mkdir -p "$d/$(dirname "$t")"; cp -p "$s" "$d/$t"; echo "$t" >> "$d/MANIFEST.tmp"
    done < "$list"
    rm -f "$list"
    mv "$d/MANIFEST.tmp" "$d/MANIFEST"
    echo "backup: $d ($(wc -l < "$d/MANIFEST" | tr -d ' ') files)"
}

do_restore() {
    local from d sel rc c p b have cur unsafe f got m t v
    from=
    if [ "${1:-}" = --from ]; then from=${2:?restore --from NAME}; shift 2; fi
    if [ -n "$from" ]; then
        d=$BK/${from##*/}
        [ -f "$d/MANIFEST" ] || die "no backup $d (or it has no MANIFEST)"
    else
        d=$(newest); [ -n "$d" ] || die "no backup in $BK"
    fi
    sel=$*; [ -n "$sel" ] || sel=$(cores)
    rc=0
    for c in $sel; do
        p=$(prefix_of "$c"); b=${p##*/}
        have=0
        while read -r m; do case "$(target_of "$m")" in "$p"_*.rbf) have=1 ;; esac; done < "$d/MANIFEST"
        cur=$(for f in "$p"_*.rbf "_Other/${b}"_*.rbf "_Computer/${b}"_*.rbf; do if [ -f "$f" ]; then echo "$f"; fi; done)
        if [ $have = 0 ]; then
            [ -n "$cur" ] && echo "$c: kept" $cur "(no $b RBF in $d)"
            continue
        fi
        unsafe=
        for f in $cur; do is_test "$f" || in_backup "$f" || unsafe="$unsafe $f"; done
        if [ -n "$unsafe" ]; then
            echo "$c: SKIPPED, not a test build and not in any backup:$unsafe (run backup first)" >&2
            rc=1; continue
        fi
        for f in $cur; do rm -f "$f"; done
        got=
        while read -r m; do
            t=$(target_of "$m")
            case "$t" in
                "$p"_*.rbf) cp -p "$d/$m" "$t"; got="$got $t" ;;
                "$p"*_*.rbf)        # variant (e.g. SynclavierBN_*): only if none installed
                    v=$(for f in "${t%_*}"_*.rbf; do if [ -f "$f" ]; then echo "$f"; fi; done)
                    [ -n "$v" ] || { cp -p "$d/$m" "$t"; got="$got $t"; } ;;
                "$b"*.mgl)
                    [ -e "$t" ] || {
                        sed 's#<rbf>_Other/#<rbf>_Devices/#; s#<rbf>_Computer/#<rbf>_Devices/#' "$d/$m" > "$t"
                        got="$got $t"; } ;;
            esac
        done < "$d/MANIFEST"
        drop_ledger "$p"
        echo "$c: restored$got from $d"
    done
    return $rc
}

do_install() {
    local c in p f t
    c=${1:?install CORE INCOMING}; in=${2:?install CORE INCOMING}
    p=$(prefix_of "$c")
    [ -f "$in" ] || die "no file $in"
    do_backup
    for f in "$p"_2*.rbf; do
        [ -f "$f" ] || continue
        is_test "$f" || in_backup "$f" || die "$f is not in a backup; not replacing it"
    done
    rm -f "$p"_2*.rbf
    drop_ledger "$p"
    t=${p}_$(date +%Y%m%d).rbf
    mv "$in" "$t"
    echo "$t $(sum_of "$t")" >> "$LEDGER"
    echo "installed $FAT/$t"
}

do_release() {
    local p d f
    p=$(prefix_of "${1:?release CORE}")
    for d in $(backups | sort -r); do
        f=$(while read -r m; do case "$(target_of "$m")" in ("$p"_2*.rbf) echo "$m" ;; esac; done < "$d/MANIFEST" | sort | tail -n 1)
        [ -n "$f" ] && { echo "$d/$f"; return 0; }
    done
    die "no backed-up release of $1"
}

case "$CMD" in
    backup) do_backup ;;
    restore) do_restore "$@" ;;
    install) do_install "$@" ;;
    release) do_release "$@" ;;
    list)
        for d in $(backups); do echo "$d $(wc -l < "$d/MANIFEST" | tr -d ' ') files"; done
        [ -s "$LEDGER" ] && sed 's/^/test build: /' "$LEDGER"; true ;;
    *) die "unknown command: $CMD" ;;
esac
