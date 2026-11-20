# Proteus/1 XR hardware notes (from firmware analysis)

Labels: **[O]** observed in firmware disassembly or MAME source, **[I]** inferred, **[U]** unknown.
Firmware images: `build/proteusxr_v200.bin` (XR v2.00), `build/protologic_v230.bin` (XR+ProtoLogic v2.30),
`build/proteus1_v205.bin` (Proteus/1 v2.05). Built with `tools/m68kdis.py`.

## Firmware images

| Set | Even byte (MSB) | Odd byte (LSB) | Reset SSP / PC | Version string |
|---|---|---|---|---|
| proteusxr | `ip441c_emu__5089.ic38` | `ip442c_emu__5089.ic37` | `0xFFFFFE` / `0x494A` | `1989 E-mu v2.00` |
| ProtoLogic XR | `IP486 901101.bin` | `IP485 901101.bin` | `0xFFFFFE` / `0x4B78` | `1990 E-mu v2.30` |
| proteus1 | `… msb 2.05.bin` | `… lsb 2.05.bin` | `0xFFFFFE` / `0x5092` | `1992 E-mu v2.05` |

## Memory map (XR v2.00)

| Range | Access seen | Function | Evidence |
|---|---|---|---|
| `0x000000–0x01FFFF` | code | Program ROM 128 KB | [O] MAME |
| `0x400001` | byte write `#$0D` in main loop and after sound init | [U] control latch / watchdog kick | [O] `0x4A48`, `0x4F06` |
| `0x700001` / `0x700003` | byte r/w | HD44780 LCD: instruction / data | [O] `0x4D84–0x4D9C` |
| `0x800000–0x8003FF` | word/long writes | **Sound chip: 32 voices × 32 bytes** (`addr = 0x800000 + voice<<5`) | [O] `0x4110`, `0x42B4`, `0x48B2` |
| `0x900001–0x90002F` (odd) | byte r/w | MC68901 MFP: GPDR `0x01`, ISRB `0x11`, TBDR `0x21`, RSR `0x2B`, UDR `0x2F` (MIDI) | [O] register offsets match MC68901 |
| `0xA00000` | word read (`tst.w`) | Volume-pot strobe: fires a one-shot measured by MFP timer B in pulse-width mode (`TBCR=0x0C`); count read from `TBDR`, inverted, scaled 0–127 against min `0xFFE094` / max `0xFFE095` → `0xFFF988`, attenuation `0xFFF986` | [O] `0x4E9C–0x4EBA`, `0x25DA–0x2662` (v2.30) |
| `0xB00001` | byte write, shadow at `0xFFF965` | [I] output latch (LEDs / mute / bank select); bit 2 cleared before entering diagnostics | [O] `0x4B7C–0x4B8C` |
| `0xC00001` | byte read | [I] output-jack sense: bits 0–1 / 2–3 tested when a voice is routed to sub-output 1 (`0x10`) / sub-output 2 (`0x20`); if the bits are 0 the routing bits are cleared (falls back to main). Not in MAME map. | [O] `0x3716–0x375C` |
| `0xD00001` / `0xD00003` | byte r/w | **Serial debug UART**: status bit0 = RX ready, bit1 = TX ready; data at `0xD00003`. Not in MAME map. | [O] `0x8152–0x8194`; [I] 6850 ACIA-style layout |
| `0xFFC000–0xFFFFFF` | RAM | Work RAM (stack top `0xFFFFFE`); short-absolute variables span `0xFFC000–0xFFFA8C`; [U] which part is battery-backed | [O] MAME, vectors, listing |
| `0xFF0000–…` | RAM probe | **XR RAM detection** at `0x513A`: writes/compares 128 words of patterns at `0xFF0000`; on success preset count `0xFFE2A0 = 0x180` (384), else `0xC0` (192). Upstream MAME maps no RAM here, so the XR set boots as a base Proteus/1. Upper bound of this RAM [U]. | [O] `0x5112–0x5180` |

## Boot (XR v2.00, `0x494A`)
- Reads MFP GPDR (`0x900001`) & `0x0F`; `== 0x0C` jumps to diagnostics at `0x25DE` [O]. [I] Front-panel buttons held at power-on are wired to MFP GPIO.
- Init sequence: `0x4E32`, LED/latch shadow `= 0x0F`, `0x4C84`, `0x4D26` (LCD), `0x5004` (preset checksum over `0xFFE1A0`, 128 words), `0x50F6`, `0x4EC2` (sound chip init), `0x4E40` (MFP init), … [O]
- Main loop at `0x49FE` writes `#$0D` to `0x400001` each pass [O].

## Sound chip init (`0x4EC2`)
```
a0 = 0x800000
word  [a0+0x5C] = 0x0027          ; voice 2 slot +0x1C — [I] global control register
for v in 0..31:                   ; a0 += 0x20
  long [+0x08] = 0x00000FFF
  long [+0x04] = 0x00000000
  long [+0x10] = 0x000001C0
  long [+0x14] = 0x00FFFE2F
  long [+0x00] = 0x000001C0
  long [+0x04] = 0x00001000
byte [0x400001] = 0x0D
```
Register meanings: see the table below and the header of `proteus_snd.cpp`.

## Additional I/O (from v2.30 analysis)
| Address | Use | Evidence |
|---|---|---|
| `0x500009 + 2·voice` | byte per voice; channel test writes `0x0F/0x00/0x1F/0x10/0x2F/0x20` → [I] output pair + left/right/pan select | [O] v2.30 `0x2AF8`, `0x490C` |

## Sound chip registers (per voice, `0x800000 + v·0x20`) — partial
| Offset | Written as | Meaning | Evidence |
|---|---|---|---|
| `+0x00` | long | current/start address (ROM word address) = record.start − 4 (+0x200000 for expansion bank B) | [O] `0x3964–0x396A`, `0x4902` |
| `+0x04` | long | pitch / phase increment (`0x41000` and `0xC4000` used by ROM tests; test reads step 1 or 4 words) [I] | [O] `0x2BE0`, `0x2D86` |
| `+0x08` | long | envelope: `(level << 12) | 0xC00 | rate(9 bits)` [I] | [O] `0x4882–0x4896` |
| `+0x0C` | long | `0xFFF000` in ROM tests [U] | [O] |
| `+0x10` | long | loop start = record.loop_start − 4 | [O] `0x48F2` |
| `+0x14` | long | NOT(loop end), loop end = record.loop_end − 3 | [O] `0x48F8–0x48FE` |
| `+0x18` | word read | current sample read-back (used by the Sound ROM test) | [O] `0x2C3A`, `0x2D36` |

## Sound ROM format (verified on the ProtoLogic ROM)
- 16-bit signed samples; 4-way word interleave across chip pairs (word n → pair n&3, chip address n>>2).
- Header: `0x000–0x003` = `0100 0302 0504 0706`; `0x004–0x01F` ID text (each char ×4); `0x020–0x027` per-chip checksums (lo bytes `0x20+p`, hi bytes `0x24+p`; value = 16-bit byte sum of the whole chip, stored bytes included); text/name area to `0x14F`; test sine `0x150–0x1BF`; samples from `0x1C0`.
- Samples: zero padding before start, data continues past the table's end (guard words), gaps of 40 or 49 words.
- Sample table in firmware: 16-byte records `{start, end, loop_start, loop_end}`; XR v2.00 at `0x112EE` (194 records), v2.30 at `0x18A12` (283 records incl. 88 ProtoLogic, addresses relative to bank B).

## Output and level (implemented in the MAME model)
- Output rate 39062.5 Hz = 10 MHz / 256 [O]: the frequency-response test pitches (`0x10625`, `0x1D7DC`, `0x1F213` for 10/18/19 kHz on the 64-sample sine) all give 39062 Hz with 12 fraction bits.
- Envelope runs in firmware (timer C ISR); the chip slews a 12-bit float level (3-bit exponent, 9-bit mantissa, 0x200 per 6 dB) toward the target [I from the firmware level tables at `0x176D0`, `0x177D0`, `0x187D0`].
- Pitch bit 18 is set only by the ROM tests; modelled as "step on CPU read of +0x18" [I]; with it the firmware's Sound ROM test passes on the real ProtoLogic dump.
