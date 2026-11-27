#!/usr/bin/env python3
"""ColdFire exception-frame regression for the TG68K changes (runs the NVC testbench).

Program: VBR = 0, trap #0 at 0x106; the handler stores the first two stacked longs at 0x01000200
and returns with RTE; the program then writes 0x12345678 to 0x01000100.
Expected frame: 0x4080 (format 4, vector 32), SR, PC 0x00000108 (next instruction).

  p2k/sim/frame_test.py      (needs nvc; work library in build/p2k/rtl)
"""
import pathlib
import os
import struct
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
W = os.path.join(ROOT, "build", "p2k", "rtl")


def image():
    img = bytearray(b"\xff" * 0x200000)
    put = lambda a, ws: [struct.pack_into(">H", img, a + 2 * i, w) for i, w in enumerate(ws)]
    put(0, [0x0100, 0x1000, 0x0000, 0x0100])
    struct.pack_into(">I", img, 0x80, 0x200)
    put(0x100, [0x7000, 0x4E7B, 0x0801, 0x4E40, 0x23FC, 0x1234, 0x5678, 0x0100, 0x0100, 0x60FE])
    put(0x200, [0x2217, 0x23C1, 0x0100, 0x0200, 0x242F, 0x0004, 0x23C2, 0x0100, 0x0204, 0x4E73])
    return img


def main():
    os.makedirs(W, exist_ok=True)
    img = image()
    with open(os.path.join(W, "frame_test.hex"), "w") as f:
        for i in range(0, len(img), 2):
            f.write("%02x%02x\n" % (img[i], img[i + 1]))
    nvc = ["nvc", "--std=2008", "-H", "1g", "-L", ".", "--work=work"]
    src = [os.path.join(ROOT, p) for p in ("p2k/rtl/cpu/tg68k/TG68K_Pack.vhd", "p2k/rtl/cpu/tg68k/TG68K_ALU.vhd",
                                           "p2k/rtl/cpu/tg68k/TG68KdotC_Kernel.vhd", "p2k/sim/tb_p2k_cpu.vhd")]
    subprocess.run(nvc + ["-a", "--relaxed"] + src, cwd=W, check=True, capture_output=True)
    subprocess.run(nvc + ["-e", "tb_p2k_cpu", "-gCYCLES=400", "-gFLASH_HEX=frame_test.hex", "-gTRACE=frame.trace"],
                   cwd=W, check=True, capture_output=True)
    subprocess.run(nvc + ["-r", "tb_p2k_cpu"], cwd=W, capture_output=True)
    t = {}
    for line in pathlib.Path(os.path.join(W, "frame.trace")).read_text().splitlines(keepends=True):
        k, a, d = line.split()[:3]
        if k == "W":
            t[int(a, 16)] = int(d, 16)
    frame = (t.get(0x01000200), t.get(0x01000202), t.get(0x01000204), t.get(0x01000206))
    marker = (t.get(0x01000100), t.get(0x01000102))
    ok = frame[0] == 0x4080 and frame[2:] == (0x0000, 0x0108) and marker == (0x1234, 0x5678)
    print("frame", ["%04x" % v if v is not None else None for v in frame], "marker", marker, "PASS" if ok else "FAIL")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
