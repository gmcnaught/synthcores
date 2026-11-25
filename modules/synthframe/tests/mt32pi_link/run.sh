#!/bin/sh
# Loopback: MiSTer sys i2s.v (synth MiSTer) -> sys mt32pi.sv receiver (game MiSTer).
# Expect bad=0 at 48 kHz for both cable wirings. 96 kHz shows bit errors; the link runs at 48 kHz.
set -e
cd "$(dirname "$0")"
S=../../target/mister/sys
O=$(mktemp -d)
for c in 0 1; do for r in 0 1; do
  verilator --binary --timing -Wno-fatal -Wno-lint -Wno-style -GRATE96=$r -GCROSSED=$c \
    --top-module tb -Mdir $O/o$c$r tb_i2s.sv $S/i2s.v $S/mt32pi.sv >$O/build.log 2>&1
  $O/o$c$r/Vtb | grep RATE96
done; done
rm -rf $O
