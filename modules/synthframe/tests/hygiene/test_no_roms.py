#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""No ROMs, firmware, NVRAM/save images, disk images or built cores anywhere in the repository
(CLAUDE.md "ROMs"). Checks every file git tracks (the index, so a `git add`ed file counts), from
the repo root; submodules (the reference models) are not ours and are skipped.

  - no file with a ROM/image/core extension;
  - no binary file (a NUL byte in its first 8 KB) except the listed kinds (images, fonts);
  - no file over 256 KB except the listed fonts;
  - no file whose SHA-1 or SHA-256 is a known ROM's: the hashes the cores' pack tools check
    (cores/*/tools/*pack*.py) and the Nuked-SC55 romset table (when the submodule is checked out).

    python3 modules/synthframe/tests/hygiene/test_no_roms.py
"""
import hashlib
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]

FORBIDDEN = (".bin", ".rom", ".rbf", ".sof", ".jic", ".zip", ".7z", ".wav", ".raw", ".sst",
             ".nvram", ".nvr", ".sav", ".img", ".ima", ".hfe", ".dsk", ".hdf", ".iso", ".chd",
             ".simg", ".stmb", ".ssnd", ".sprg", ".sseq", ".sim", ".p2k", ".syx")
BINARY_OK = (".png", ".ttf")           # panel art and the panel fonts
MAX_BYTES = 256 * 1024
BIG_OK = ("modules/synthframe/tools/fonts/",)
HASH_TABLES = ["modules/nuked-sc55/doc/supported_romsets.rst"]
HASH_GLOBS = ["cores/*/tools/*pack*.py"]
MIN_ROM = 2048                          # smaller files are not ROM dumps


def tracked():
    out = subprocess.run(["git", "ls-files", "-s", "-z"], cwd=ROOT, capture_output=True, check=True).stdout
    files = []
    for rec in out.decode().split("\0"):
        if not rec:
            continue
        meta, path = rec.split("\t", 1)
        mode = meta.split()[0]
        if mode == "160000":           # submodule
            continue
        f = ROOT / path
        if f.is_symlink() or not f.is_file():
            continue
        files.append(path)
    return files


def known_hashes():
    srcs = [ROOT / p for p in HASH_TABLES if (ROOT / p).is_file()]
    for g in HASH_GLOBS:
        srcs += sorted(ROOT.glob(g))
    found = set()
    for s in srcs:
        found |= set(re.findall(r"\b(?:[0-9a-f]{64}|[0-9a-f]{40})\b", s.read_text(errors="replace")))
    return found, srcs


def check(files):
    errors = []
    for p in files:
        if p.lower().endswith(FORBIDDEN):
            errors.append(f"{p}: ROM/image/core file type")
    for p in files:
        f = ROOT / p
        if f.stat().st_size > MAX_BYTES and not p.startswith(BIG_OK):
            errors.append(f"{p}: {f.stat().st_size} bytes (over {MAX_BYTES})")
        with f.open("rb") as fh:
            if b"\0" in fh.read(8192) and not p.lower().endswith(BINARY_OK):
                errors.append(f"{p}: binary file")
    known, _ = known_hashes()
    for p in files:
        f = ROOT / p
        if f.stat().st_size >= MIN_ROM:
            data = f.read_bytes()
            if hashlib.sha1(data).hexdigest() in known or hashlib.sha256(data).hexdigest() in known:
                errors.append(f"{p}: content is a known ROM")
    return errors


def main():
    files = tracked()
    known, srcs = known_hashes()
    errors = check(files)
    for e in errors:
        print("FAIL:", e)
    print(f"no-ROM hygiene: {len(files)} tracked files, {len(known)} known ROM hashes from {len(srcs)} tables: "
          f"{'FAIL' if errors else 'ok'}")
    return 1 if errors else 0


if __name__ == "__main__":
    sys.exit(main())
