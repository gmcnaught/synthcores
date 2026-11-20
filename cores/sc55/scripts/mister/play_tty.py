# Runs on the MiSTer (python3 there). ttyS1 must already be at 9600 (sc55_run.sh sets it).
# play_tty.py EVENTS SECONDS: write timed MIDI bytes to /dev/ttyS1, repeating the song for SECONDS
import sys, time, os
ev = [(float(a), bytes.fromhex(b)) for a, b in (l.split() for l in open(sys.argv[1]))]
dur = float(sys.argv[2]); song = ev[-1][0] + 1.0
fd = os.open("/dev/ttyS1", os.O_WRONLY)
t0 = time.monotonic(); off = 0.0; n = 0
while True:
    for t, b in ev:
        if off + t > dur: break
        w = t0 + off + t - time.monotonic()
        if w > 0: time.sleep(w)
        os.write(fd, b); n += len(b)
    else:
        off += song; continue
    break
os.write(fd, b"".join(bytes([0xb0 + c, 123, 0]) for c in range(16)))   # all notes off
print("sent", n, "bytes in %.1f s" % (time.monotonic() - t0))
