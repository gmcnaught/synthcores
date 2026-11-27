#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""U2 peripheral test program: timer 1 compare-match interrupts, SLEEP, A/D scan.

Usage: u2_periph_prog.py OUT.bin

The CPU sets timer channel 1 to clear on TGRA at phi/8 with TGRA = 0100 and the IMIA interrupt
(vector 28), starts the A/D in scan mode over AN0-AN2, enables interrupts and loops on SLEEP,
reading ADCSR after each wake-up. The interrupt routine reads and clears TSR, counts in R1 and
reads ADDRA and TCNT1 as words. Run on MAME's mu50 machine (tools/mu50_testrom.py) and on
mu50sys_bench, the traces are identical (tests/ci.sh checks the hash of MAME's).
"""
import struct
import sys

img = bytearray(0x1000)
struct.pack_into(">I", img, 0, 0x400)          # reset
struct.pack_into(">I", img, 28 * 4, 0x300)     # ITU1 IMIA
main = bytes.fromhex(
    "7a070020f800"           # mov.l #20F800,er7
    "f823" "386e"            # TCR1 = 23: clear on TGRA, phi/8
    "79000100" "6b80ff74"    # TGRA1 = 0100
    "f8f9" "3870"            # TIER1 = F9: IMIA
    "f8e2" "3860"            # TSTR = E2: channel 1 runs
    "f832" "38e8"            # ADCSR = 32: scan AN0-2, start
    "063f"                   # andc #3f,ccr: interrupts on
    "0180"                   # loop: sleep
    "2ae8"                   # mov.b @FFE8,r2l
    "40fa")                  # bra loop
img[0x400:0x400 + len(main)] = main
isr = bytes.fromhex(
    "2871" "f8fe" "3871"     # TSR1 read, clear IMFA
    "0b51"                   # inc.w #1,r1
    "6b03ffe0"               # mov.w @FFE0,r3 (ADDRA)
    "6b04ff72"               # mov.w @FF72,r4 (TCNT1)
    "5670")                  # rte
img[0x300:0x300 + len(isr)] = isr
with open(sys.argv[1], "wb") as f:
    f.write(img)
