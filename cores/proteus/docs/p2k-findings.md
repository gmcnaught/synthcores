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
| CS1 | 0x00400000 | 4 MB | 0x009F | 16-bit, 0 wait | sound chips: H at 0x500000, G at 0x600000 |
| CS2 | 0x00800000 | 64 KB | 0x0595 | 16-bit, 1 wait | [U] |
| CS3 | 0x00800000 | 4 MB | 0x055A | 8-bit, 1 wait | byte writes to 0x900000, 0xA00000, 0xB00000 (panel/LCD/LED [U]) |
| CS4 | 0x00F00000 | 64 KB | 0x3D81 | 16-bit, 15 waits | board ID word read at 0x00F00000 ("Board ID = %x") |
| CS6 | 0x00C00000 | 64 KB | 0x215F | 8-bit, 8 waits | [U] |
| MBAR | 0x20000000 | — | — | — | on-chip SIM, timers, 2 UARTs (MIDI A/B [I]) |

CS2/CS3 overlap at 0x00800000; how the CSCR read/write enables split them is [U].

## G chip (voice engine) interface [O]
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

## Voice register block [O, base address U]
A second, memory-mapped register file at a base held in `0x01120800` (stored at run time through
a pointer; the value is [U], [I] inside CS1), **64 bytes per voice** (`voice << 6`, `0x10323AC`):

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

## Diagnostics menu (same structure as the Proteus/1)
Strings at `0x0107xxxx` [O]: 2 MFG Test, 3 Calibrate Knobs, 4 Panel Test, 5 LCD Test, 6 MIDI Test,
7 Freq Response Test (50 Hz / 440 Hz / 19 kHz; also 1/2/5/10/15/19 kHz), 8 Audio Return Test,
9 Channel Test, 10 RAM Test, 11 Program ROM Test, **12 G/H Chip Tests**, 13 FX Ram Test,
14 Sound Rom Test, 15 Potentiometer Test, 16 Auto Test, 17 Sound Data CRC Test, 19 Reboot.
Code: G/H test `0x107B364`, FX RAM test `0x107B4BC`/`0x107D94C`, Freq Response `0x107983C`
(tone index 0/4/12 → `0x102C964`, which goes through the normal voice allocator, so the
output rate must be derived from the voice pitch path, not a fixed table) [O].

Sound ROM: "ROM Slot #%d block checksums", "Sound ROM checksum: 0x%04x", "Comparing
Simm1(16Mb)<->Simm2(16Mb)", "SIMM with 2 ROM IDs", "2 SIMMs w/Same ID" [O]: SIMMs carry a ROM ID
and a 16-bit checksum, so a SIMM dump can be checked the way the Proteus/1 wave ROMs were.

## Next
1. Base address of the voice register block (trace the pointer store, or boot the OS in MAME
   with a stub machine and log it).
2. Voice start path (`0x102C8D4` → `0x10323CC` / `0x10327E0`): per-voice G registers in use.
3. Output rate: pitch computation for the 440 Hz test tone vs. a known sample root.
4. Boot flash dump: not in the .dli; needed for an emulator boot (Ray Bellis's Audity 2000
   method, or a unit).
5. P1K vs P2K diff (one G chip, 2 H chips, 2 SIMM slots).
