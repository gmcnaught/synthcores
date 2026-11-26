#!/usr/bin/env python3
"""tools/rebuild_proteus1_rom.py checksum compensation (review 2026-11-26 proteus1.md#13).

Builds ROM images whose per-chip byte sums (without the stored checksum and compensation
bytes) take chosen values, including 65026..65280, where one compensation byte has no solution,
and checks that set_checksums() makes every chip's 16-bit byte sum equal its stored checksum,
and that sums outside that band keep the one-byte layout (word 0x148+p unused).
Exit status: 0 passed, 1 failed.
"""
import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "tools"))
import rebuild_proteus1_rom as rb  # noqa: E402

TARGETS = [[0, 1000, 65025, 65026, 65100, 65280, 65281, 65535],
           [255, 510, 65027, 65279, 30000, 64770, 65200, 12345]]


def chip_sum(rom, p, lane):
    return sum((rom[n] >> (8 * lane)) & 0xFF for n in range(p, rb.WORDS, 4)) & 0xFFFF


def main():
    fails = 0
    for targets in TARGETS:
        rom = [0] * rb.WORDS
        for k, s0 in enumerate(targets):
            p, lane = k // 2, k % 2
            n = 0x1C0 + p
            left = s0
            while left:
                b = min(left, 255)
                rom[n] |= b << (8 * lane)
                left -= b
                n += 4
        try:
            rb.set_checksums(rom)
        except (AssertionError, StopIteration) as e:
            print(f"FAIL sums {targets}: {type(e).__name__}")
            fails += 1
            continue
        for k, s0 in enumerate(targets):
            p, lane = k // 2, k % 2
            stored = ((rom[0x24 + p] >> (8 * lane)) & 0xFF) << 8 | ((rom[0x20 + p] >> (8 * lane)) & 0xFF)
            two = (rom[0x148 + p] >> (8 * lane)) & 0xFF
            ok = chip_sum(rom, p, lane) == stored and (two != 0) == (65026 <= s0 <= 65280)
            fails += not ok
            print(f"{'ok  ' if ok else 'FAIL'} chip {p} lane {lane}: S0 {s0:5d} -> checksum {stored:04x}, "
                  f"second compensation byte {two}")
    print(f"test_rebuild_checksum: {'passed' if not fails else f'{fails} failed'}")
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
