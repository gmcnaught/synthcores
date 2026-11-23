# Proteus 2000 family: firmware findings

Status date: 2026-11-21. Labels: **[O]** observed in the firmware listing, **[I]** inferred, **[U]** unknown.
Source: OS 2.26 E-Loader files from `snw.lonningdal.no/p2k/firmware226_p1kp2k_e-loader111.zip`
(kept in `roms/_src/p2k/`, not committed).

| File | SHA-1 | Image | CRC-32 |
|---|---|---|---|
| `p2k226.dli` (Proteus2000Loader, 2.26) | 932acd05bdcfdf8442f35428f6261a262c04d3ca | 0x82888 bytes | 18d5791b OK |
| `p1k226.dli` (Proteus1000Loader, 2.26) | 389f64134bda20a7564b1069e08f494bb9f05d51 | 0x7fd88 bytes | cae02a85 OK |

Listing: `tools/dli.py p2k226.dli build/p2k/p2k226.bin`, then
`m68k-neogeo-elf-objdump -D -b binary -m m68k:5206e --adjust-vma=0x01000000` (Homebrew binutils;
`m68k:5206e` decodes the ColdFire ISA).

## .dli format [O]
- 0x1A0-byte header: copyright text, then at 0x24 `{offset 0x1A0, length, CRC-32}`; name fields
  "<Model>Loader", "EXE ", "Version 2.26", "GenericOS", "dli.os.version=2.26".
- The image is not compressed or encrypted: a raw ColdFire program linked at **0x01000000**
  (DRAM). Long 0 = SSP `0x0115BB20`, long 1 = reset PC `0x01000402`. The header CRC is the
  standard CRC-32 (zlib) of the image, so a download can be checked offline.
- The OS can rewrite the boot flash ("Will destroy boot Sector!!!", "Writing boot sector"), so
  [I] the boot ROM lives in the CS0 flash and copies/loads the OS into DRAM.

## CPU and memory map [O: reset code 0x01000402–0x010005AE]
CPU is a ColdFire with MBAR at **0x20000000** and the MCF5206e SIM register layout (MAME
`mcf5206e.cpp` offsets match every write). This confirms the forum claim of an MCF5206e [I: the
5206e and 5206 share the layout; `movec rambar0` and the CACR values fit the 5206e].

| Select | Base | Size | CSCR | Port | [I] Use |
|---|---|---|---|---|---|
| DRAM bank 0 | 0x01000000 | 4 MB | — | — | OS + data (SSP 0x0115BB20) |
| DRAM bank 1 | 0x01400000 | 4 MB (size-probed at boot) | — | — | P2K 8 MB total; probe falls back if absent |
| CS0 | 0x02000000 | 2 MB | 0x0F0B / 0x0F8B / 0x1341, chosen from the boot port-size bits read back from CSCR0 (7/6) | — | boot + OS flash |
| CS1 | 0x00400000 | 4 MB | 0x009F | 16-bit, 0 wait | LCD 0x400000/0x408000, H chips 0x500000, G05 0x600000, G20 voice registers 0x77F000 |
| CS2 | 0x00800000 | 64 KB | 0x0595 | 16-bit, 1 wait | word read at 0x800000, bit 8 tested at boot [I: panel buttons] |
| CS3 | 0x00800000 | 4 MB | 0x055A | 8-bit, 1 wait | 0x900000 / 0xA00000 LED latches (RAM shadows), 0xB00000 control latch (0x8E/0x9E/0x8F at boot) [U] |
| CS4 | 0x00F00000 | 64 KB | 0x3D81 | 16-bit, 15 waits | board ID word read at 0x00F00000 ("Board ID = %x") |
| CS6 | 0x00C00000 | 64 KB | 0x215F | 8-bit, 8 waits | [U] |
| MBAR | 0x20000000 | — | — | — | on-chip SIM, timers, 2 UARTs (MIDI A/B [I]) |

CS2/CS3 overlap at 0x00800000; how the CSCR read/write enables split them is [U].

## G05 chip (U28) interface [O]
The boot block names it: the chip behind this port is **G05, U28** ("Err: G05 chip U28 failed"
when register `0x181E` does not read 0x30). It is not one of the two voice chips.
- Index/data port at **0x600000**: write the index to **0x60000E**, then access the word or long
  register at `0x600000 + offset`. Helper calls encode a register as `offset << 8 | index`
  (`0x102F60C` read word, `0x102F640` read long, `0x102F674` write word, `0x102F6A8` write long;
  offset = `(reg >> 8) & 0x7E`, index = `reg & 0x1F`). Accesses run with a nesting counter at
  `0x0112089C` raised [I: interrupt lock].
- Per-voice registers: the G test writes offsets 0x58, 0x6A, 0x78, 0x0C, 0x1C, 0x2C, 0x3C, 0x4C,
  0x5C, 0x6C, 0x7A, 0x5A, 0x4A, 0x48, 0x68 for index 0–31 (`0x102EEC6–0x102EFFA`).
- Global registers: offset 0x18 with index 0x14–0x1F (`0x1814…0x181F`). The G test reads
  `0x181E` and requires **0x30** (`0x102EE84`) [I: chip ID/revision; the diagnostics label it
  "Testing G05 chip"]. `0x102F7A0` clears offset 0x18 for index 0–19.
- Byte write `1 → 0x60007C` before the G test [U].
- Tables of 128 words are streamed through offsets 0x28, 0x2A, 0x38, 0x3A (pointer table at
  `0x0102F894`; 4 × 32 entries, bit 15 set on odd entries by one caller) [I: routing/mix RAM].
  Code at `0x102F9C8` and `0x102FCC4` writes routing-like values there (FX sends [I]).
- **FX RAM is reached through the G chip**: the FX RAM test (`0x107D94C`) writes index 5–14 at
  offset 0x18 and polls index 4 [I: the effects processor sits behind the G chip's global
  registers, or shares its bus].
- [U] How 128 voices (2 G chips) map onto a 5-bit index; the second chip's address.

## Voice register block (G20 chips) [O]
The two voice chips are **G20, U15 and U24** (boot block). Their register files are contiguous at
**0x0077F000**, **64 bytes per voice**: U15 = voices 0–63 at 0x77F000, U24 = voices 64–127 at
0x780000 (boot test `0x020011F0`: writes 0x8211 to voice 16 +0x3E and expects 0x211, writes
0x01234567 / 0x76543210 to +0x10 and reads it back, sets voice 32 +0x3E to 0x140 / 0x1C0, clears
64 voices). The OS keeps the base in `0x01120800` and indexes it with `voice << 6` (`0x10323AC`).

| Offset | Use | Evidence |
|---|---|---|
| +0x00 | long, init 0x1000 | `0x1032118` |
| +0x04 | long, init 0x80000 | `0x103211E` |
| +0x08 | long: top 6 bits kept, low 26 bits = NOT(address) | `0x10324A2`, `0x1032846` |
| +0x0C | long: NOT(address) | `0x10324B8`, `0x103287E` |
| +0x10 | long, init 0x80000 | `0x103211A`, `0x1032852` |
| +0x14 | long: `0x04000000 | address` (bit 26 = flag) | `0x10324BE`, `0x1032872` |
| +0x18 | long: low 26 bits compared with a window [lo, hi] | `0x1032610` |
| +0x1C / +0x1E | word: sound-memory read data, ports A / B (auto-increment) | `0x102C862`, `0x103205A` |
| +0x30 / +0x34 | long: sound-memory read address (word address), ports A / B | `0x1031F20`, `0x1031F40` |
| +0x3E | word: mode (51 compare, 57 idle/read, 60) | `0x1032214–0x1032244` |

- Sample addresses are **26-bit word addresses**: 64 M words = 128 MB, the 4 × 32 MB SIMMs [I].
- Loop/end addresses are written inverted, as on the Proteus/1 (`+0x14` there = NOT(loop end)) [O],
  so the G chip keeps the Proteus/1 address scheme with wider fields [I].
- The +0x1C/+0x30 ports are how the CPU reads sound ROM (header, checksums, SIMM compare);
  the Proteus/1 had a single read-back word at +0x18.

## Sound ROM SIMMs [O]
- Sound-memory byte address of slot n = `n << 25` (32 MB per slot, `0x102CBD0`); bit 30 of an
  address selects sound memory in the copy routine `0x1032004`.
- Header: 0x6AE bytes read from the start of each slot (`0x102C3DC`).
  - +0x00 magic `0x05551212`, +0x0E "EOS Sound SIMM" (else the slot counts as empty, ID 255).
  - +0x04 size in bytes: below 8 MB → 4 checksum blocks of 2 MB, else 8 blocks of size/8.
  - +0x3E long, +0x58E long `0x00FF26F0`, +0x5AA "EROM V1.2" / "EROM 2 V1.0A" identify the
    preset/flash format; one "EROM 2 V1.0A" case expects block-0 checksum 371.
  - Block checksums: 16-bit words at +0x68E + 2·block; a block checksum is the 16-bit sum of its
    words (`0x102C600`). The diagnostics print the 16-bit sum of all blocks as "Sound ROM checksum".
- So a SIMM dump can be verified offline with no unit, as the Proteus/1 wave ROMs were.

## H chip (filters) [O, partial]
- The H test passes base **0x500000** (`0x107B402`), writes `0x98765432` patterns over 128 slots
  and reads them back (`0x10334EC`). Init (`0x10336C4`) writes offsets +68/+70 in 8 pages of
  0x1000 bytes; other code writes +0x1040, +0x3040, +0x5040 [I: register select by address bits].

## Filter types and coefficient data
Cross-checked against the Pistol Instruments Proteus 2000 Editor reference
(`proteus2000editor.pistolinstruments.com/llms-full.txt`: SysEx IDs, ranges and the filter list;
kept in `roms/_src/p2k/pistol/`, not committed).
- Descriptor table at `0x01020444` [O]: 51 entries of 8 bytes `{name pointer, code, index, 0, 0}`.
  The code is the SysEx value's low byte for the 2nd–6th-order types (Classic 0x00, Smooth 0x01,
  Shallow 0x08, Band-pass1 0x10, Swept1 0x20, PhazeShift1 0x40, FlangerLite 0x48, Aah-Ay-Eeh
  0x50, …) and `0x80 | n` for the 33 twelfth-order Z-plane types (SysEx 259–291 = 0x103–0x123,
  n = 3–0x23); off = 0x7F. Name strings at `0x01028EDC` carry order and type ("MegaSweepz   12 LPF").
- Coefficient block at **`0x010205DC`–`0x01021CDB`** (5,888 bytes) [O], read by `0x01020316`
  when the code is ≥ 0x80. Records are **52 bytes**, at `0x010205DC + 52 · (index · 16 + k)`:
  two groups of 3 × 8 bytes (two longs each), a word at +48 and a flag byte at +50 (bit 7 tested).
  Each 8-byte entry is copied as `{long, long, 0, 0}` into 16-byte slots of the filter state.
  [I] Two groups = the two H-chip channels a 12th-order filter uses; 3 entries per group = three
  biquad sections per channel, which fits "12th-order filters use 2 channels" (Audity data sheet).
  What `k` selects and the bit layout of the 8-byte entries (frame corners for the Z-plane
  morph) are [U]; the 2nd–6th-order types take another path (`0x010202F0`).

## Diagnostics menu (same structure as the Proteus/1)
Strings at `0x0107xxxx` [O]: 2 MFG Test, 3 Calibrate Knobs, 4 Panel Test, 5 LCD Test, 6 MIDI Test,
7 Freq Response Test (50 Hz / 440 Hz / 19 kHz; also 1/2/5/10/15/19 kHz), 8 Audio Return Test,
9 Channel Test, 10 RAM Test, 11 Program ROM Test, **12 G/H Chip Tests**, 13 FX Ram Test,
14 Sound Rom Test, 15 Potentiometer Test, 16 Auto Test, 17 Sound Data CRC Test, 19 Reboot.
Code: G/H test `0x107B364`, FX RAM test `0x107B4BC`/`0x107D94C`, Freq Response `0x107983C`
(tone index 0/4/12 → `0x102C964`, which goes through the normal voice allocator, so the
output rate must be derived from the voice pitch path, not a fixed table) [O].
The test-tone setup (`0x102C8D4`) takes a 12-byte entry per tone from a RAM table at `0x01120080`
{+4 offset added to the slot's sound-memory base, +8 word → voice field +8, +10 length} and a
fixed voice field +28 = 0xF85F [O]. The table is filled at run time [I: from the SIMM header,
like the Proteus/1's test sine in the wave ROM], so the test pitch needs a SIMM image or a unit.

Sound ROM: "ROM Slot #%d block checksums", "Sound ROM checksum: 0x%04x", "Comparing
Simm1(16Mb)<->Simm2(16Mb)", "SIMM with 2 ROM IDs", "2 SIMMs w/Same ID" [O]: SIMMs carry a ROM ID
and a 16-bit checksum, so a SIMM dump can be checked the way the Proteus/1 wave ROMs were.

## Next
The plan for the MAME prerequisites of a MiSTer core is `docs/p2k-mame-plan.md` (2026-11-23).
Done: MAME machine, voice start path, output rate (44.1 kHz [I]), boot block, SIMM/Baby format,
level/pan/envelope path.
1. H filter: done (E4-measured voice model). G05 effects microcode: still a workalike.
2. Sound memory flash writes (port B) for user flash SIMMs (the main user flash is done).
3. Effects (G05 / FX RAM) and output routing (6 outputs).
4. P1K vs P2K diff (one G chip, 2 H chips, 2 SIMM slots).

## Boot block (v1.24 flash dump) [O]
Source: `e-mu-proteus-2k-flash-analysis.zip` (dbwbp.com via the Wayback Machine): a 2 MB
TH28F160 dump of a v1.24 unit, `proteus-2k-v124-28f160.bin`, SHA-1
7cd9c92114f1ef583adacd7f533856f5c0031742, stored **byte-swapped** (swap each 16-bit word).
- 0x000000–0x00FFFF: boot block, linked at **0x02000000** (CS0). SSP 0x01210600, reset PC 0x02000402.
  "Proteus2000Loader", "Searching for OS...", MIDI download protocol messages: this is the
  E-Loader receiver.
- 0x010000: "lib_CODE … CPLG … 1.24 ProteusGenericOS" directory header; 0x010100: the OS image
  (identical to `p2k124.dli`'s payload per the dump's notes; the OS is copied to DRAM at 0x01000000).
- 0x0A0000 onward: user presets and setups ("lib_USER").
- Boot tests: address/data line stuck tests, "Testing DRAM SIMM", G05 U28, G20 U15, G20 U24.
- LCD: HD44780-style at **0x400000** (instruction: init 0x38, 0x0C, 0x06, 0x01) and **0x408000**
  (data), byte access (`0x02003AD4–0x02003B2C`).
- LEDs: 0x900000 and 0xA00000 byte latches with RAM shadows at 0x0120FE00/0x0120FE04
  (`0x0200151C`); boot steps light LEDs 4, 5, 6.

## MAME boot (proteus2k research machine) [O unless marked]
`mame/patches/0002-emup2k-proteus2000-research.patch`; see `mame/README.md`.
- **Dump repair**: flash 0x000DFE reads `4E38`; the boot block needs `4EB9` (`jsr 0x02003B30`)
  there, or it takes an illegal-instruction exception. [I] A two-bit read error in the dump.
- **Data-line test** (`0x02000DE4`) writes `1 << n` to 0x50000000 (no chip select) and reads it back,
  checking D23–D0: it passes only if the bus returns the last value written [I: bus hold on the
  board]. The cache test (`0x02000CB0`) writes 2 M longs over 0x50000000–0x53FFFFFF.
- **Boot wait**: `0x02000BD2` loops while 0x800000 bit 7 reads 1 (CSCR0 port-size bit 7 = 0 path).
- **Port A** is bit-banged right after reset (`0x020008B0`, ~100 k writes); the flash holds a block
  of FPGA-configuration-like data from 0x001780 [I: configures the Xilinx XCS05XL the sources
  survey found on the board].
- **CS6 0xC00000**: knob ADC (see "Level, pan and envelopes"); earlier read as a key matrix [corrected].
- **Timers**: the OS programs ColdFire timer 1 (TRR 0x80E8, TMR 0x001B) as its tick.
- **OS 1.24 from the dump** stops with "Fatal: Bus Address error", PC 0x00044EB9; its directory CRC
  (`0x02004E60`, CRC-32 table at 0x020060D0, initial value 0) does not match the stored
  0xF29AF737 in our computation [U: further read errors in the OS region, or a different range].
- **OS 2.26** (from the .dli, placed at flash 0x010100) runs to "No Sound Roms detected".

## SIMM detection and the Baby device (MAME, 2026-11-21)
- **Read port pipeline** [O]: after every address write to G20 +0x30 the OS discards one word
  (`tst.w +0x1C`, `0x01031F24`, `0x01032044`). The MAME model returns the latched word and fetches
  the next; with it the header and block checksums of a synthetic SIMM pass.
- **Start-mode mailbox** [O]: the 1.24 boot block stores 0x1E at DRAM 0x010003FC (`0x020033CA`);
  OS 2.26 reads it at `0x010009B0`, and bits 0x16 send it to "Press Enter to commit OS to flash"
  (`0x0107A414`). The 1.24 boot block sets 0x1E when its directory check (`0x020036A4`, names
  "ROMLIB"/"ROM") fails. The research driver patches 2.26 to read 0 (normal start).
- **ROM ID** (header +0x3E) [O]: 0 means a flash SIMM identified by +0x5AA ("EROM V1.2" → 1,
  "EROM 2 V1.0A" → 2); otherwise the long is the ID. The Sound Authoring guide (E-mu FI10993)
  reserves 106–119 for user flash SIMMs and says the ID is the bank-select MSB.
- **Big and Baby memory** [O]: E-mu's Sound Authoring guide says a SIMM holds "Big Flash" (sample
  data and EOS presets) and "Baby Flash" (Proteus sample headers, instruments, default presets).
  The OS selects Baby with latch 0xB00000 bit 6 (`0x01035944` sets it, `0x01035968` clears it)
  around reads in the same sound-memory window. Every slot is first probed for a JEDEC flash
  (`0x0102DAF0`, 16-bit, 0xAAAA/0x5555/0x9090; Intel 0x89 or Sharp 0xB0 accepted); otherwise
  the OS opens a read-only library at Baby offset 0x10, size 0x1FFFF0 (`0x0100DDF4`).
- **Baby library** [O]: the same `lib_` container as the flash user banks (flash 0xA0000
  `lib_USER`): 16-byte header, then objects {4cc, u32 length incl. 12-byte header, u16 id,
  u16 flags}. SIMM object types: `yh_1` (library header: ROM ID, short name), `yp_1` (presets,
  980 bytes = user `pr01` data), `ys_1` (sample headers, 52 bytes), `yi_1` (instruments),
  `yd_1`, `ya_1` (141 bytes), `yr_1`. Field layouts: docs/p2k-lib-formats.md.
- **MIDI In A** = ColdFire UART1 (UBG 33, 31250 baud from 33 MHz) [O]; MAME's MIDI file player
  starts 10 s after power-on, and the OS shows its main screen at about 20 s.

## Voice playback (G20) from a note-on [O unless marked]
MIDI note-on with a synthetic SIMM preset (bank MSB 4) programs one G20 voice per note
(`0x010327E0` and following):
- +0x18 = sample start (26-bit word address; slot base + ys_1 start), written last in the start
  sequence; +0x14 = 0x04000000 | loop start; +0x0C = NOT(loop end); +0x00 = 0x1800; +0x20–0x2E = 0.
- +0x04 bits 31-16 = phase increment, 4.12 fixed point: (pitch & 0x3FFFF) << 16 (`0x0103271C`);
  +0x10 gets the same value in its high half. +0x08 bits 31-26 = a value from the table at
  0x0112069C indexed by increment >> 10 [I: interpolation/anti-alias selector, like the
  Proteus/1 "tone"]. Notes 60 and 72 on samples at 18000 and 20000 Hz (roots 61, 73) gave
  0x629 and 0x6D9 = rate / 44100 × 2^((note − root)/12) × 4096, so the output rate is 44.1 kHz [I].
- **Voice allocation** [O, MAME with the stand-in SIMM, `tests/p2k_corpus` case `poly`]:
    - Free voices are taken in blocks of 8 in the order 0, 1, 4, 5, 2, 3, 6, 7.
    - The order across the chips is 0-55 (U15), then 64-119 (U24), then 56-63, then 120-127.
      So the last 8 voices of each chip are used only when more than 112 notes sound at once.
    - With all 128 voices busy, the 22 extra notes took voices 0-21 in order. Those held the 22
      oldest notes; this case does not tell oldest-first from lowest-index-first.
- Note-off: +0x14 = idle-loop start (0x2F, bit 26 clear), +0x08 low 26 bits = NOT(idle-loop
  end 0x43), +0x04 low 10 bits = 0x300, +0x10 = 0xA2 [O]; meaning of the last two [U].
- Correction: "no CPU-driven envelope steps" was wrong; it came from the ADC stub returning 0 (master
  volume muted, every level clamped to 0). Envelopes are computed by the CPU; see below.

## Level, pan and envelopes [O unless marked]
- **H block of voice v** (OS table 0x010331D0, read by 0x01032F14; earlier misread as a curve
  table): 0x500000 + 0x4000·(v>>6) + {0x2000, 0, 0x3000, 0x1000}[v&3] + 0x80·((((v>>2)&15)+15)&15).
  The formula matches all 128 entries. 0x01032F64 returns the previous slot (−0x80, wrapping
  0x000 → 0x780 in each 0x800 page).
- **Level bytes**: block +0x71 = right, +0x73 = left (pan −64 "64L" silences +0x71) are written by
  0x01019978 every ~70 ms while a voice sounds: level + pan table (0x0101D364, 128 words,
  20·log10(sin) × 102.4, centre −311) / 16, clamped 0..255. The level (0x0101C8F0) sums the
  envelope, layer volume, velocity, channel volume and the master volume (0x01120574).
- **Master volume** = the volume knob: event 5177 → 0x0102C1B4; below 3 the OS writes −16384
  (mute). At power-on it is 0 until the knob is read.
- **Knob ADC** at 0xC00000 (0x01035760): write 0x08 + channel to select the mux input, read the
  conversion; pot → channel map (0x0112077C) 2 1 0 4 3 6 7; pot 0 = master volume; channel 7 is
  read as the zero reference (minimum = reference + 2); value = raw − minimum, clamped to 127.
- Measured byte changes (note 60, piano, velocity 100): layer volume 0 / −20 / −40 dB → 210 / 162
  / 122; velocity 40 → 148; centre pan 210 vs hard pan 230 / 0; factory envelope 210 → 183 over 2 s;
  a time-based attack of 60 ramps from 0.
- **Level law [I]**: 0.375 dB a step (255 = 0 dB): it gives −18 dB for the −20 dB volume edit and a
  54 dB volume-knob range, and is the EMU8000 attenuation step. It conflicts with the pan table,
  which is built for 102.4 units/dB (0.156 dB a step: centre −3 dB); under 0.375 dB the centre is
  −7.5 dB. Hardware needed.
- **Preset parameters**: OS table 0x010491B4–0x01049EE0, 12-byte entries {u16 SysEx ID, u32 0,
  u16 offset in the 980-byte preset, s8 min, s8 max, u16 flags}; layer n adds n·0xCC
  (`tools/p2k_params.py` reads it from the local OS image). SysEx parameter edits sent with
  device IDs 0x7F and 0x00 had no effect in MAME [U why].
- MAME `p2k_g20_device`: address/loop/pitch with linear interpolation, per-voice L/R gain from
  the level bytes (smoothed ~5 ms). Filters (block +0x00–+0x2F, written at note-on), effects and
  output routing are not modelled.


## Front panel, flash, MIDI Out (MAME, 2026-11-21) [O unless marked]
- **Buttons**: 0x800000 bits 15–7, active low; the OS inverts and masks 0xFF80 (0x01035720) and
  maps bits to key codes (0x01034C9C): 7 → 0x130, 8 → 0x111, 9 → 0x110, 10 → 0x103, 11 → 0x104,
  12 → 0x121, 13 → 0x120, 14 → 0x101, 15 → 0x102 (14+15 → 0x140). Pressed from the main screen:
  bit 9 → "SAVE PRESET" (Save/Copy), 10 → "CH VOLS" (Multi), 11 → "Bank MSB/LSB" screen,
  14 → "PRESET NAME" (Edit), 15 → "MASTER TRANSPOSE" (Master). Bit 7 toggles an LED in the
  0xA00000 latch [I: Arp]; bits 8, 12, 13 [I: Audition, cursor right/left].
- **LEDs**: 16-bit word, low byte 0x900000, high byte 0xA00000 (0x0103570C). The latch 0xB00000
  low nibble = 8 + row (0x010359AC), bit 6 = Baby select.
- **Boot and bit 7** [U]: the 1.24 boot block (0x02000BAC) repeats an LED/cache test until bit 7
  reads 0 when CSCR0 PS1 is clear (0x02001724), but the OS reads bit 7 = 0 as a held key. With
  PS1 set the boot block takes another path ("Searching for OS..."). The driver forces bit 7 to 0
  while the CPU runs from the boot flash [I: the panel is read through the Xilinx the boot block
  configures].
- **MAME fix**: MCF5206e CSCR0–7 are 16-bit registers; MAME mapped one byte (the high half), so a
  word read lost bits 7–0 (PS1/PS0/AA...). Now mapped as 16-bit.
- **Encoder**: G05 index 0x1B register 0x18 = signed count since the last read (0x0102F80C,
  0x010359E0).
- **Flash**: 28F160 x16; the OS flash driver (0x0102DD54 → 0x0102E080) accepts maker 0x89 or
  0xB0 with device 0xD0 (also 0x14/0x16/0x17, 0x4471, 0x889D). MAME `SHARP_LH28F160S3` works: the
  user data initialises ("FlashInit error" gone) and writes persist as NVRAM. With the user area
  erased (P2K_CLEAN=1) the OS rebuilds factory defaults ("Resetting to default", "Making backup",
  "Compacting free space") in about 45 s of emulated time.
- The dump's user data has MIDI channel 1 disabled in its multi setup (notes on channel 2 play).
- **MIDI Out** = UART1 TX: device inquiry F0 7E 7F 06 01 F7 answers
  F0 7E 00 06 02 18 04 04 03 00 "2.26" F7 (device ID 0). E-mu parameter requests
  (F0 18 0F 00 55 02 …) answer with command 0x70 (error) [U: the edit-buffer protocol].

## Filters and effects (MAME, 2026-11-21)
- **Filter registers** [O]: per voice, three 16-byte sections at H block +0x00/+0x10/+0x20 (four
  coefficient words, four zero words). Classic 4-pole LP = `8408 ffcf ffcf ffcf` twice; Smooth 2-pole
  = one such section and one `ffcf ffcf ffcf ffcf` (pass); Deeper HPF puts `8004` in words 2-3; the
  third section is `ffcf ffcf 00f2 00f2` in all non-12th-order types. Q changes the low-pass words
  (word 0: 0x8408 / 0x6A07 / 0x5205 at Q 0 / 64 / 127). Frequency goes to the previous slot's block
  +0x3C/+0x3E (one byte, both halves): filter frequency 0 → 0xBD, 64 → 0xFD, ≥128 → 0xFF with the
  template's velocity cord. 12th-order types also program a second voice's block. [I] Each word is
  a pair of Z-plane frame bytes interpolated by the frequency byte; the arithmetic is unknown.
- **MAME filter model** [I]: sections classed LP/HP/pass from the patterns above; cutoff from the
  frequency byte with the OS readout curve 20 kHz × (1002/1024)^(255−F); resonance from the LP word-0
  high byte; trapezoidal 2-pole SVF per section. Not the H1.5 arithmetic.
- **G05 = EMU8000-style effects engine** [O]: the OS holds the EMU8000 init1 table (0x0102F8C4) and
  AWE32-shaped reverb parameter words (0x0103014A…); FX parameters go to G05 channels 0-31,
  registers 0x28/0x2A/0x38/0x3A (the EMU8000 INIT1-4 layout [I]), with parameter bytes OR-ed into
  table words (0x0102FA60…) and ramped over successive writes. Sends: G05 channels 0x00/0x02/0x10/
  0x12, registers 0x14/0x16, 0x64/0x66, 0x74/0x76 change with the A/B send amounts [O, encoding U].
- **Active FX state** [O]: pointer at 0x01120500 → +0x12 A algorithm, decay, HF damping, B→A,
  sends 1-4; +0x1A B algorithm, feedback, LFO rate, delay (signed), sends 1-4; a copy follows at
  +0x22 (found by DRAM dump diffs over preset FX edits).
- **Mix output** [O]: H block +0x7B = 0x72 (Main), 0x7B (Sub 1), 0x44 (Sub 2) at note-on.
- **MAME FX model** [I]: behavioural reverb (comb/all-pass), delay, chorus/flange/delay by algorithm
  family, parameters from the active FX state, every voice on bus 1. Not the G05 microcode.
- **G05 vs EMU8000 INIT slots** [O/I]: reading the G05 registers as EMU8000 INIT arrays (offset 0x28 =
  INIT1, 0x2A = INIT2, 0x38 = INIT3, 0x3A = INIT4; the EMU8000 has INIT1/2 = register 2 on data ports
  1/2 and INIT3/4 = register 3), the slots the P2K OS writes when changing FX (INIT1 0x0B, 0x11, 0x13,
  0x17, 0x19, 0x1B, 0x1F; INIT2 0x01, 0x03, 0x07, 0x09, 0x0B, 0x0F, 0x11, 0x13, 0x17, 0x1F; INIT4 0x1F)
  are almost all slots that 86Box's EMU8000 model (src/sound/snd_emu8k.c, GPL-2.0) decodes as
  reverb/chorus parameters. 86Box's decoding is itself inferred (Freeverb-style reflections, all-pass
  feedback, chorus delay/LFO), not the EMU8000 microcode. MAME has no EMU8000 device (only a TODO
  in src/mame/misc/savquest.cpp).

## G05 effects as EMU8000 INIT arrays (2026-11-21) [O unless marked]
- The OS writes the EMU8000 init tables to G05 offsets 0x28 (INIT1 0x03FF…), 0x2A (INIT2 0x43FF…),
  0x38 (INIT3 0x83FF…), 0x3A (INIT4 0xC3FF…) of channels 0-31: the G05 register file has the
  EMU8000 layout (offset = 0x10·register + 8/0xA for data ports 1/2; 0x18/0x1A = register 1).
- Room 1's values have the shape of the AWE32 reverb tables (0xB4xx, 0xA4xx, 0x44xx/0x45xx,
  0x842A/0x852A, 0x72F4/0x72A4/0x7254…). Delay (algorithm 7) zeroes all six reflection gains and
  feedbacks. Decay changes INIT1[0x0D] and INIT4[0x1F] (tail return).
- FX B slots (differ from 86Box's AWE32 chorus interpretation): INIT3[0x1A] = delay in samples at
  44.1 kHz (50/200/500 ms → 2205/8820/22050, which also confirms the 44.1 kHz rate), INIT3[0x17]
  bits 7-0 = 2 × feedback, INIT4[0x0B] bits 7-0 = LFO depth (0 for delays), channel 0x0A offset
  0x1A = LFO rate (0 at rate 0, 0x0C80 at 100). INIT3[0x13]/[0x15], INIT4[5]/[7] constant [U].
- Sends: channel 0 offsets 0x14/0x16 (EMU8000 PTRX) bits 15-8 = reverb send (0xE0 at 100 %, 0xC8 at
  50 %); 0x74/0x76 (CSL) bits 31-24 = chorus send (0xA0 at B send 100) [I: EMU8000 meaning].
- **MAME `emup2k_fx.h`**: reverb = an independent implementation of 86Box's interpretation of the
  INIT slots (six damped comb "reflections", tail delay + two diffusers); chorus = the P2K slots
  above. Checked against 86Box's reverb code fed the same P2K register state (local harness, not
  distributed): correlation 0.9994-0.9998 over 0.5 s for Room 1, Room 1 decay 90 and Delay; the
  residual 2-3.5 % is 86Box's integer rounding. Both are workalikes: the EMU8000 microcode is
  undocumented.

## H filter = the E4 H-chip voice; routing; SysEx; CPU (2026-11-21)
- **Filter words** [O]: the P2K writes the same H-chip words as the E-mu E4 (measured by
  github.com/ds10fan/emu-hchip-patent, VOICE_INTERFACE.md; patent US 5,170,369): word k at H block
  + 4k (k = 0..9), high half = frame for cutoff 0, low = frame for cutoff 255, each half = X/F codes
  (B2', f^2). Classic 2-pole Q 0 = 8408FFCF (E4 "2 Pole Lowpass" Q 0 identical), Q 64 = 6A07E8E4,
  Q 127 = 5205DCED; neutral FFCFFFCF; Deeper HPF zeros 80048004; gain word 9 = 00F200F2 (Classic).
  Classic 4-pole = words 0 and 4; Q also programs zero word 1 (FFCFE1E8 at Q 127), whose DC gain
  f^2 makes resonant settings louder at high cutoff (+6.3 dB predicted, +6.4 dB in MAME).
- **Per-slot pipeline** [O/I]: the cutoff (+0x3E) and flags (+0x7B bit 7: unnormalised poles,
  set for high-pass and 12th-order) of the voice in H block X are in the previous slot's block. A
  12th-order type also programs the next slot's block as a second stage, and the OS then writes
  the level bytes into that next block (end of chain). The +0x7B byte's low bits route the voice:
  0x72 Main, 0x7B Sub 1, 0x44 Sub 2 (layer Mix Output 0/1/2) [O].
- **Effect send buses** [O]: send bus n = G05 channels 4(n−1), +2, +0x10, +0x12; Main uses bus 1,
  Sub 1 bus 2, Sub 2 bus 3 (MAME: reverb tail only when the voice's bus has a send).
- **MAME model**: E4 voice structure in double precision (f0²/D0 · N1 · N5 · P4 · P8 · g9), cutoff
  smoothed ~2 ms (the patent's interpolator not modelled), 12th-order chaining by coefficient-write
  timing [I].
- **SysEx** [O]: parameter request `F0 18 0F 00 55 02 <n ids> <id lsb msb>… F7` → reply
  `… 01 <2n words> <id> <value>… F7`. Parameter edit `F0 18 0F 00 55 01 <count of 14-bit words>
  <id value>… F7`: the count is words (2 per parameter), checked against the F7 position (0x0104DC1C).
  Edit 897 = −1 (edit buffer), 898 = layer, 1410 = −20 → read back −20 and the note-on level byte
  drops by 48 units, as with the preset edited in the SIMM.
- **MAME UART fix**: the 68681-family transmitter set TxEMT again after being disabled; OS 2.26
  disables the transmitter after its last byte and loops in its UART interrupt until the status is 0
  (0x0100B526), so any MIDI Out reply hung the OS. TxEMT is now only set while the transmitter is
  enabled.
- **CPU** [O/I]: MAME runs the MCF5206e on its Musashi 68000-family core in a ColdFire mode
  (`m68000_musashi_device`, `init_cpu_coldfire`; 56 ColdFire-enabled handler names, no MAC unit).
  OS 2.26 uses 68020-style integer instructions (mulsl, divsw/divuw, extbl, linkw) plus movec,
  cpushl, halt, pulse; the macl/msacl "instructions" in the listing are data (filter tables,
  0xAAAA fill) and no invalid-instruction exception occurs. A MiSTer core could start from a
  68020-capable soft core (e.g. TG68K) with the ColdFire system instructions, exception frame and
  the SIM/timer/UART peripherals added [I].
- Open [U]: FX slots INIT1[0x0D], INIT3[0x13]/[0x15], INIT4[5]/[7]; routing byte values other than
  the three observed; the H interpolator's approach-time byte (+0x3C).
