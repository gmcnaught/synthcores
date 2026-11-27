#!/usr/bin/env python3
"""Proteus 2000 user flash SIMM test (docs/p2k-mame-plan.md step 5) on the MAME research machine.

OS 2.26 has no path that imports samples; it writes a user flash SIMM (ROM ID 106-119) through its
Save/Copy menu: rename / change ID, copy the user bank, duplicate slot 0 to slot 1
(docs/p2k-findings.md "User flash SIMMs"). This test drives the rename page with the scripted front
panel (P2K_KEYS) and checks that the change persists and the SIMM still plays:

  1. slot 1 = a stand-in SIMM built with ROM ID 106 (P2K_FLASH1=1: writable, written back at exit);
     boot, Save/Copy -> RENAME FLASH SIMM, change the ID to 108, Enter;
  2. the images on disk now carry ID 108 (Big header +0x3E, Baby yh_1) with valid block checksums;
  3. restart from the modified images, select bank MSB 108, play a note: the voice plays from the
     SIMM, the LCD shows the bank, and two runs are identical.

  p2k_flash_test.py --simm DIR [--out DIR] [--keys FILE]
DIR holds big.bin, baby.bin (ROM ID 106, tools/p2k_build_simm.py) and flash_clean.nv. Needs
P2K_MAME and P2K_ROMPATH as tools/p2k_corpus.py.
"""
import pathlib
import argparse
import hashlib
import os
import shutil
import struct
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import p2k_lib  # noqa: E402
import p2k_simm  # noqa: E402


def mame(args, d, env_extra, seconds):
    env = dict(os.environ, SDL_VIDEODRIVER="dummy", SDL_AUDIODRIVER="dummy", P2K_TRACE="1", **env_extra)
    for k in ("P2K_NOFX", "P2K_NOFILTER", "P2K_CLEAN", "P2K_BUSTRACE"):
        env.pop(k, None)
    cmd = [os.environ["P2K_MAME"], "proteus2k", "-rompath", os.environ["P2K_ROMPATH"], "-video", "none",
           "-sound", "none", "-nothrottle", "-skip_gameinfo", "-keyboardprovider", "none", "-mouseprovider", "none",
           "-joystickprovider", "none", "-log", "-seconds_to_run", f"{seconds:.3f}"] + args
    r = subprocess.run(cmd, cwd=d, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if r.returncode:
        raise SystemExit(f"MAME exit {r.returncode}: {r.stdout.decode(errors='ignore')[-400:]}")
    lcd, flash = [], 0
    with open(os.path.join(d, "error.log"), errors="ignore") as f:
        for ln in f:
            if ln.startswith("[:] LCD "):
                lcd.append(ln[4:].rstrip())
            elif " F 1 " in ln:
                flash += 1
    return lcd, flash


def rom_id(big):
    with open(big, "rb") as f:
        h = f.read(0x6AE)
    return struct.unpack_from(">I", h, 0x3E)[0]


def baby_ids(baby):
    """ROM IDs of the yh_1 (library header) objects of the Baby library at offset 0x10, with flags."""
    d = pathlib.Path(baby).read_bytes()
    _, _, _, _, objs, _ = p2k_lib.walk(d, 0x10)
    return [(struct.unpack_from(">I", d, o[0] + 12)[0], o[4]) for o in objs if o[1] == "yh_1"]


def checksums_ok(big):
    img = pathlib.Path(big).read_bytes()
    size = struct.unpack_from(">I", img, 4)[0]
    n, blk = p2k_simm.blocks(size)
    return all(p2k_simm.word_sum(img[i * blk:(i + 1) * blk]) == struct.unpack_from(">H", img, 0x68E + 2 * i)[0]
               for i in range(n))


def sha(p):
    return hashlib.sha256(pathlib.Path(p).read_bytes()).hexdigest()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--simm", required=True)
    ap.add_argument("--out", default=os.path.join(os.path.dirname(HERE), "build", "p2k", "flashtest"))
    ap.add_argument("--keys", default=os.path.join(os.path.dirname(HERE), "tests", "p2k_flash", "rename_106_108.keys"))
    ap.add_argument("--seconds", type=float, default=40.0)
    a = ap.parse_args()
    out = os.path.abspath(a.out)
    if os.path.isdir(out):
        shutil.rmtree(out)
    d1 = os.path.join(out, "edit")
    os.makedirs(os.path.join(d1, "nvram", "proteus2k"))
    for f in ("big.bin", "baby.bin"):
        shutil.copy(os.path.join(a.simm, f), os.path.join(out, f))
    shutil.copy(os.path.join(a.simm, "flash_clean.nv"), os.path.join(d1, "nvram", "proteus2k", "flash"))
    big, baby = os.path.join(out, "big.bin"), os.path.join(out, "baby.bin")
    env = {"P2K_SIMM1": big, "P2K_BABY1": baby, "P2K_FLASH1": "1", "P2K_KEYS": os.path.abspath(a.keys)}
    before = rom_id(big)
    lcd, flash = mame([], d1, env, a.seconds)
    print(f"edit run: {flash} flash writes; last LCD lines:")
    for ln in lcd[-6:]:
        print("   ", ln)
    after = rom_id(big)
    ok = True
    print(f"ROM ID on disk: {before} -> {after}; block checksums {'ok' if checksums_ok(big) else 'BAD'}; "
          f"Baby yh_1 (id, flags): {baby_ids(baby)}")
    ok &= after == 108 and checksums_ok(big) and any(i == 108 for i, _ in baby_ids(baby))

    # restart from the modified images, select bank 108 and play
    script = os.path.join(out, "play.txt")
    with open(script, "w") as f:
        f.write("2.0 cc 1 0 108\n2.0 cc 1 32 0\n2.05 pc 1 0\n2.5 on 1 60 100\n3.5 off 1 60\n")
    hashes = []
    for rep in range(2):
        d = os.path.join(out, f"play.{rep}")
        os.makedirs(os.path.join(d, "nvram", "proteus2k"))
        shutil.copy(os.path.join(d1, "nvram", "proteus2k", "flash"), os.path.join(d, "nvram", "proteus2k", "flash"))
        subprocess.run([sys.executable, os.path.join(HERE, "..", "..", "..", "modules", "synthframe", "tools", "midi", "mkmidi.py"), script, os.path.join(d, "in.mid")], check=True)
        envp = {"P2K_SIMM1": big, "P2K_BABY1": baby, "P2K_FLASH1": "1",
                "P2K_SNDLOG": os.path.join(d, "snd.log"), "P2K_SNDOUT": os.path.join(d, "snd.raw")}
        lcd, _ = mame(["-midiin", os.path.join(d, "in.mid")], d, envp, 16.0)
        starts = sum(1 for ln in pathlib.Path(os.path.join(d, "snd.log")).read_text().splitlines() if ln.split()[1:2] == ["G"] and
                     int(ln.split()[0]) > 12 * 44100 and int(ln.split()[2], 16) & 0x3f == 0x1a)
        raw = pathlib.Path(os.path.join(d, "snd.raw")).read_bytes()
        peak = max(abs(v) for v in struct.unpack(f"<{len(raw) // 2}h", raw)) if raw else 0
        hashes.append(sha(os.path.join(d, "snd.raw")))
        print(f"play run {rep}: voice starts {starts}, peak {peak}, LCD {lcd[-1] if lcd else None}")
        ok &= starts >= 1 and peak > 100
    ok &= hashes[0] == hashes[1]
    print(f"play runs identical: {hashes[0] == hashes[1]} ({hashes[0][:12]})")
    # the standalone renderer, fed the play run's register log and the modified image, reproduces it
    exe = subprocess.run([os.path.join(os.path.dirname(HERE), "p2k", "model", "build.sh")], check=True,
                         stdout=subprocess.PIPE, stderr=subprocess.DEVNULL, text=True).stdout.strip()
    d = os.path.join(out, "play.0")
    frames = os.path.getsize(os.path.join(d, "snd.raw")) // 4
    subprocess.run([exe, "--simm", "1=" + big, "--frames", str(frames), os.path.join(d, "snd.log"),
                    os.path.join(d, "render.raw")], check=True, stdout=subprocess.DEVNULL)
    same = sha(os.path.join(d, "render.raw")) == hashes[0]
    print(f"standalone renderer reproduces the play run: {same}")
    ok &= same
    print("PASS" if ok else "FAIL")
    return 0 if ok else 1


if __name__ == "__main__":
    raise SystemExit(main())
