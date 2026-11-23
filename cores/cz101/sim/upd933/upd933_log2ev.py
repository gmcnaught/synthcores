#!/usr/bin/env pypy3
# license:BSD-3-Clause
"""Convert a patched-MAME CZ_SND_LOG into a uPD933 event list for upd933_replay / upd933_bench.

Log lines (10 MHz tick, then MAME's m_sample_count at the access):
    <tick> <count> W <reg hex> <value hex>   completed register write (3rd byte, /CS low)
    <tick> <count> R <data hex>              read
    <tick> <count> Q <irq>                   IRQ line change (update_irq)
  optional lines this converter also understands, if the MAME patch adds them:
    <tick> <count> C <state>                 cs_w(state)            (needed to check IRQ timing)
    <tick> <count> I <state>                 id_w(state)
    <tick> <count> X                         device_reset(), count before it is cleared
    <tick> <count> T                         timer_tick() fired
    <tick> <count> N <value hex>             machine().rand() value used for noise
    <tick> <count> S                         rq_r() with side effects (stream update)

Without C lines /CS is taken as low from the first access, so register writes and reads are
applied but no IRQ timer is ever armed (the IRQ comparison then fails by construction).

usage: upd933_log2ev.py LOG OUT.ev
"""
import sys


def main():
    if len(sys.argv) != 3:
        sys.stderr.write(__doc__)
        return 2
    src, dst = sys.argv[1], sys.argv[2]
    out = []
    have_cs = False
    first_tick = None
    last_tick = 0
    counts = {}
    with open(src) as f:
        for n, line in enumerate(f, 1):
            p = line.split()
            if len(p) < 3:
                continue
            try:
                tick, cnt, op = int(p[0]), int(p[1]), p[2]
            except ValueError:
                continue  # not a uPD933 log line
            counts[op] = counts.get(op, 0) + 1
            if first_tick is None:
                first_tick = tick
            last_tick = max(last_tick, tick)
            if op == 'W':
                reg, val = int(p[3], 16), int(p[4], 16)
                out.append(f"{tick} W {reg:02x}")
                out.append(f"{tick} W {val >> 8:02x}")
                out.append(f"{tick} W {val & 0xff:02x} s={cnt}")
            elif op == 'R':
                out.append(f"{tick} R {int(p[3], 16):02x} s={cnt}")
            elif op == 'Q':
                out.append(f"{tick} Q {int(p[3])}")
            elif op in ('C', 'I'):
                have_cs |= op == 'C'
                out.append(f"{tick} {op} {int(p[3])} s={cnt}")
            elif op == 'X':
                out.append(f"{tick} X s={cnt}")
            elif op == 'T':
                out.append(f"{tick} T")
            elif op == 'N':
                out.append(f"{tick} N {int(p[3], 16):x}")
            elif op == 'S':
                out.append(f"{tick} S s={cnt}")
            else:
                sys.stderr.write(f"{src}:{n}: unknown op {op!r}, skipped\n")
    if first_tick is None:
        sys.stderr.write("no uPD933 lines in the log\n")
        return 1
    if not have_cs:
        out.insert(0, f"{first_tick} C 0")
        sys.stderr.write("log has no C lines: /CS held low, IRQ timing cannot be checked\n")
    out.append(f"{last_tick} E")
    with open(dst, "w") as f:
        f.write(f"# from {src}\n")
        f.write("\n".join(out) + "\n")
    print(f"{dst}: {len(out)} events from " + ", ".join(f"{k} {v}" for k, v in sorted(counts.items())))
    return 0


if __name__ == "__main__":
    sys.exit(main())
