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
| `0xB00001` | byte write, shadow at `0xFFF965` (v2.30: `0xFFF995`) | LED latch, active low: bit 0 MASTER, bit 1 EDIT, bit 2 ENTER, bit 3 MIDI (see *Front-panel LEDs* below); bit 2 is lit on the "* SYSTEM ERROR *" screen while it waits for ENTER | [O] `0x4B7C–0x4B8C`; v2.30 `0x4E88`, `0x4DBA` |
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

## Front-panel LEDs (v2.30)
Latch `0xB00001`, active low: bit 0 MASTER, bit 1 EDIT, bit 2 ENTER, bit 3 MIDI [O].
- `0x4E88` refreshes the latch from its shadow `0xFFF995`. The ENTER LED follows its mode byte
  `0xFFF996`: 0 off, 1 on, 2 blinking with bit 8 of the tick counter `0xFFF998` (~256 ms per
  phase). Table byte `0x9D` at `0x4EF0` [O].
- A page sets `0xFFF996` from bits 3–2 of byte 3 of its descriptor in the page table at `0xDE2`
  (`0x5ECE`); `0x7D10–0x7D52` set it to 1 or 2 elsewhere [O].
- ENTER (GPIP3) is entry `0x250A` of the button jump table at `0x24E6`: it clears `0xFFF996`,
  then jumps to the ENTER handler held at `0xFFF9A8` [O].
- `0x4DBA` lights bit 2 on the "* SYSTEM ERROR *" screen and waits for GPIP3 [O].
- Full-system simulation (`sim/sys_main.cpp`, v2.30 ip485/ip486, GPIO script: MASTER, the
  encoder to "SEND MIDI DATA / User Presets", CURSOR, ENTER) [O]:
  ```
  3.195974 b00000 3e3e   MASTER pressed (bit 0 on)
  5.995905 fff996 0202   CURSOR on "SEND MIDI DATA": ENTER LED mode 2
  6.095113 b00000 3a3a   bit 2 on
  6.350885 b00000 3e3e   bit 2 off; toggles every ~256 ms until
  8.495731 fff996 0000   ENTER pressed: "SENDING DATA... / Done.", SysEx out; bit 2 stays off
  ```
  Bit 2 stayed off when idle, on every Master page and on every Edit page (including
  "SAVE PRESET to" with the cursor on the title).

## Sound chip registers (per voice, `0x800000 + v·0x20`) — partial
| Offset | Written as | Meaning | Evidence |
|---|---|---|---|
| `+0x00` | long | current/start address (ROM word address) = record.start − 4 (+0x200000 for expansion bank B) | [O] `0x3964–0x396A`, `0x4902` |
| `+0x04` | long | bits 17..0: signed increment, 12 fraction bits (Reverse Sound negates it: `neg.l; andi.l #$3FFFF`); bit 18: ROM-test step mode [I] (`0x41000`, `0xC4000`); bits 23..21: tone 0–5 (instrument base 4 for "Dark" instruments, else 0; at or above the sample root 1 + 1 per 5 semitones up, max 5) | [O] `0x2BE0`, `0x2D86`, `0x49E8–0x4A2E`, `0x3B8A–0x3BC0`, `0x33E8` |
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
- Pitch bit 18 is set only by the ROM tests; modelled as "step on CPU read of +0x18" [I]; with it the firmware's Sound ROM test passes on both the ProtoLogic and Proteus/1 dumps.
- Reverse Sound [O `0x39B0–0x39DA`]: start = sample end − 0x64 − sound start; loop end = previous sample's end + 9, loop start = loop end + 0x18 (a backward loop in the zero gap). Modelled: the loop wraps when the address passes loop end in the direction of travel.
- Interpolation and tone: 7-point interpolation with a kernel per tone value, as described in E-mu's patent US 5,111,727 (`docs/patents.md`). Tone 0 = the patent's Table 1 kernel; tones 1–5 [I] designed to match it with the cutoff scaled by 5 semitones per step (`tools/interp_kernels.py`); OSD "Tone Filter" Off uses tone 0 for every voice. Instrument table flag byte: `0x80` normal, `0x90` "Dark" (v2.30 `0x19ECE…`).
- Looping as in E-mu's patent US 5,342,990: when the address exceeds loop end, the loop size is subtracted; the interpolator reads straight across the seam.
