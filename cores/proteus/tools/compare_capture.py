#!/usr/bin/env python3
"""Compare the core's DDR3 audio capture with the reference model.

Usage: compare_capture.py CAPTURE.raw DEMO.bin SECONDS
CAPTURE.raw starts at sample 0 of the demo (the ring at 0x31000000 is written from its
start each time the demo starts). Reports how long a prefix of the capture equals the model output; exit 0 if at least
5 s (or all compared samples) match and contain audio.
"""
import array
import os
import sys

sys.path.insert(0, os.path.dirname(__file__))
import proteus_model  # noqa: E402

FS = 39062.5


def main():
    cap = array.array("h", open(sys.argv[1], "rb").read())
    if sys.byteorder == "big":
        cap.byteswap()
    secs = float(sys.argv[3])
    rom, events = proteus_model.load(sys.argv[2])
    n = min(len(cap) // 2, int((secs - 2) * FS))  # leave 2 s for core load / download time
    ref = proteus_model.run(rom, events, n)
    # The core keeps writing while we read, and the image download takes a few seconds, so
    # the written part of the ring is a prefix of unknown length: report the matching prefix.
    match = 0
    while match < n and cap[2 * match] == ref[2 * match] and cap[2 * match + 1] == ref[2 * match + 1]:
        match += 1
    nonzero = sum(1 for x in ref[:2 * match] if x)
    print(f"core output matches the model for the first {match} samples ({match / FS:.2f} s); "
          f"{nonzero} non-zero values in that span")
    if match < n:
        i = 2 * match
        print(f"first difference at {match / FS:.3f} s: core {cap[i:i + 8].tolist()} model {ref[i:i + 8].tolist()}")
        print("(differences past the core's current write position are expected: unwritten DDR)")
    sys.exit(0 if match >= min(n, int(5 * FS)) and nonzero else 1)


if __name__ == "__main__":
    main()
