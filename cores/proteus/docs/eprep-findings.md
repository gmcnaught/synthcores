# Findings from proteus.nekoweb.org (EPREP), checked 2026-11-20

Source: https://proteus.nekoweb.org (E-mu Proteus Reverse Engineering Project). Downloads were
examined locally; nothing from them is committed.

## 1. Real Proteus/1 wave ROM dump (`poprockroms.zip`, posted 2026-11-14)
- 8 mask ROMs: `ip382_b0lsb`…`ip385_b3lsb` (low bytes), `ip402_b0msb`…`ip405_b3msb` (high bytes);
  also `full_waverom.bin` (= the interleave, little-endian words) and per-bank files.
- [O] Interleave word n = {msb[n&3][n>>2], lsb[n&3][n>>2]} gives header `0100 0302 0504 0706`,
  ID text, test sine at 0x150. All 8 per-chip byte-sum checksums (chip bytes 8/9) match.

| chip | SHA-1 |
|---|---|
| ip382_b0lsb.bin | 5eacb2208e254043676de685b545d8f17f284f5c |
| ip383_b1lsb.bin | c3abe07987703fe4a19f27de40ca10ae3cf6b706 |
| ip384_b2lsb.bin | c246851b97e13fdd057fcfe0fbe6af7016a22e07 |
| ip385_b3lsb.bin | 4d662ac18bc2bd8f5fdeb45124b48a62e0ac5489 |
| ip402_b0msb.bin | edb714212bce3a84d5b76abbff1ac4af2961a42a |
| ip403_b1msb.bin | 7ff5eb2c83ea45f33b9fc8983f94b8cf7cca4d4c |
| ip404_b2msb.bin | a1c02c379d97faac2680507244b863fc3730d069 |
| ip405_b3msb.bin | 66361832d6f9a160f49068e0730a80ae65b85fe3 |

Rebuilt ROM (`tools/rebuild_proteus1_rom.py`) vs dump, per firmware v2.30 sample record:
- [O] Only 1.6 % of words equal at the same address. With a 1-word shift, tier A and E records are
  bit-identical (residual > 150 dB down): the rebuild places every sample one word late
  (loop-end alignment convention off by one).
- [O] Some records differ in gain: the Emulator X conversion rescaled them (e.g. rec 4 "Strings D2":
  dump = 0.79 × rebuild, otherwise identical).
- [O] Tiers F/G/H (44 records: neighbour / nearest-length / single-cycle guesses) are the wrong
  data (median correlation 0.06–0.26). 29 records are silent in the rebuild.

## 2. Reverse sound (firmware v2.30)
- [O] `0x49E8–0x49F4` and `0x4A16–0x4A22`: when the voice's reverse flag (`$8(a6)` bit 0) is set,
  `neg.l d1; andi.l #$3FFFF, d1` → the +0x04 increment is an **18-bit two's-complement** value.
- [O] `0x39B0–0x39DA`: reverse start = record.end − 0x64 − sound-start offset; register +0x14 source
  (`$84`) = previous record's end + 9, +0x10 source (`$88`) = that + 0x18: a 24-word loop inside the
  zero gap after the previous sample (manual: reversed sounds ignore loops and do not sustain).
- [I] The forward rule `addr -= (end − start)` also covers reverse if "passed end" is tested in the
  direction of travel.
- Current model (`proteus_snd.cpp` `step()`, `proteus_snd.sv`, `tools/proteus_model.py`) takes bits
  16..0 unsigned and loops only when end ≥ start, so a reversed voice runs forward at ~31 words per
  sample.

## 3. "Tone" field in +0x04 bits 23..21
- [O] `0x3B8A–0x3BC0`: value = instrument byte bits 4..2, plus (when transposed up) 1 per
  0x140 pitch units (5 semitones) + 1, clamped to 5; stored `<< 4`, then `0x33E8–0x33F2` keeps
  bits 6..4 and shifts left 1 → bits 7..5 of byte +0x05 = long bits 23..21. ORed into every pitch
  write (`0x49FA–0x4A00`).
- [O] MAME demo trace (`build/demo_trace.txt`): high word of +0x04 is 0x0000 / 0x0020 / 0x0040 /
  0x0060 / 0x0080 (values 0–4); nonzero on 31 % of pitch writes.
- [O] Manuals: "Dark Strings / Dark Choir / Dark Sax / Dark … Trumpet / Dark Synth … with a darker
  tone"; Tone is a note-on modulation destination (not realtime).
- [I] A per-voice low-pass / interpolation-filter setting. [U] Its response: no source on the site;
  needs recordings of a real unit. The current model ignores it (Dark Strings = Strings).

## 4. No change
- Schematics/tech manual: 40 MHz crystal, IC367 (LSI #2) makes 20/10/2.5 MHz and 500 kHz; six
  Burr-Brown PCM56P 16-bit DACs (one per output, serial data from IC367), 3-pole 0.5 dB Chebyshev
  reconstruction filter. Consistent with 10 MHz CPU, 39062.5 Hz, 16-bit output, stereo sum.
- `prom_srom_proteus_1_xr_230.zip` (labelled v2.35 on the page) is byte-identical to the v2.30 +
  ProtoLogic set already used. `proteus_mame.zip` holds the same v2.05 firmware as `roms/proteus1`.
- `lsi_registers.txt` agrees with docs/hardware-notes.md apart from naming byte +0x05
  ("tone nibble and reverse nibble, reverse is 3"), explained by §2 and §3.
- Upstream MAME `emu68k.cpp`: no change since 2026-07-24 (no wave ROM definition yet).

## Status (2026-11-20)
- §1: `tools/build_image.py --waverom poprockroms.zip` uses the dump (SHA-1 per chip); MAME
  `proteusxr`/`proteusxrpl` load the 8 chips; the firmware's Sound ROM test passes on the dump.
  The rebuild fallback's one-word offset is fixed (53 % of words now equal the dump).
- §2: signed 18-bit increment and direction-aware loop in MAME, `tools/proteus_model.py` and
  `proteus_snd.sv` (bit-exact). MAME run with SysEx "pri reverse sound" = On confirms the
  firmware's register values (`tests/midi/reverse.txt`).
- §3: inferred one-pole tone filter (`tools/tone_lut.py`), OSD "Tone Filter" On/Off.
