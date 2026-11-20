# Runs locally; play_tty.py plays the output on the MiSTer.
# smf2events.py IN.mid OUT.txt: a standard MIDI file as "seconds hexbytes" lines (tempo map applied)
import sys, struct
d = open(sys.argv[1], "rb").read()
assert d[:4] == b"MThd"
fmt, ntrk, div = struct.unpack(">HHH", d[8:14])
pos = 8 + struct.unpack(">I", d[4:8])[0]
events = []  # (tick, order, bytes or ('tempo', us))
for tn in range(ntrk):
    assert d[pos:pos+4] == b"MTrk", d[pos:pos+4]
    ln = struct.unpack(">I", d[pos+4:pos+8])[0]; p = pos + 8; end = p + ln; pos = end
    tick = 0; run = 0; n = 0
    def vlq():
        global p
        v = 0
        while True:
            b = d[p]; p += 1; v = (v << 7) | (b & 0x7f)
            if not b & 0x80: return v
    while p < end:
        tick += vlq()
        st = d[p]
        if st == 0xff:
            typ = d[p+1]; p += 2; l = vlq()
            if typ == 0x51: events.append((tick, tn, n, ("tempo", int.from_bytes(d[p:p+3], "big"))))
            p += l
        elif st in (0xf0, 0xf7):
            p += 1; l = vlq(); body = d[p:p+l]; p += l
            events.append((tick, tn, n, bytes([0xf0]) + body if st == 0xf0 else body))
        else:
            if st & 0x80: run = st; p += 1
            ln2 = 1 if (run & 0xf0) in (0xc0, 0xd0) else 2
            events.append((tick, tn, n, bytes([run]) + d[p:p+ln2])); p += ln2
        n += 1
events.sort(key=lambda e: (e[0], e[1], e[2]))
tempo = 500000; last = 0; t = 0.0
with open(sys.argv[2], "w") as f:
    for tick, _, _, e in events:
        t += (tick - last) * tempo / 1e6 / div; last = tick
        if isinstance(e, tuple): tempo = e[1]
        else: f.write("%.6f %s\n" % (t, e.hex()))
print("events", len(events), "duration %.1f s" % t)
