# Sample and firmware sources in hand (local only, never committed)

Status date: 2026-11-20. All files live under `roms/` (gitignored). Inspect `.ebl` packs with `tools/ebl.py`.

## Firmware

| Set / version | Files | CRC32 | In MAME | Notes |
|---|---|---|---|---|
| Proteus/1 XR v2.00 (1989) | `ip441c_emu__5089.ic38` (MSB), `ip442c_emu__5089.ic37` (LSB) | 01f11633 / e8154a65 | yes (`proteusxr`) | Also in `E-mu Proteus XR LSB-MSB (2.0).zip` as `Proteus-1_NMC27C512AN_IC38/IC37.BIN`, same hashes, plus a front-panel photo |
| Proteus/1 XR v2.03 (1989) | IC38 (MSB) / IC37 (LSB) | accc7f42 / 0fb3f23e | **no** | Two independent zips have identical bytes. Reset PC `0x4932`. Candidate second BIOS for `proteusxr`. |
| Proteus/1 XR + ProtoLogic v2.30 (1990) | `IP486` (MSB), `IP485` (LSB) | 036bf047 / 3c1d85c6 | added (`proteusxrpl`) | |
| Proteus/1 v2.05 (1992) | msb/lsb 2.05 | 6a84ac60 / 3ae786b4 | yes (`proteus1`) | |
| HD44780 A00 font | `hd44780_a00.bin` | e459877c | yes | `BAD_DUMP` (from datasheet) |

## Sound ROMs (hardware dumps)

| Set | Chips | Layout | Status |
|---|---|---|---|
| ProtoLogic expansion (4 MB) | 8× 512 KB, `IP505–508` (low bytes), `IP705–708` (high bytes) | word n = `{IP70x[n>>2], IP50x[n>>2]}`, x = 5 + (n & 3); 16-bit signed | Verified byte-exact against the dump's WAV conversion |

## Emulator X / Proteus X sample packs (E-mu conversions, not ROM dumps)

`.exb` = bank (presets), `SamplePool/*.ebl` = one sample each: 16-bit PCM plus start/end/loop and a per-sample rate field (values 7000–39000; [I] sample rate in Hz).

| Pack | Samples | PCM bytes | Hardware ROM size | PCM / ROM | Use |
|---|---|---|---|---|---|
| Proteus 1 | 217 | 4,189,000 | 4 MB (Proteus/1) | 99.9 % | Rebuild the Proteus/1 sound ROM (Stage 1) |
| Proteus 2 | 298 | 8,113,754 | 8 MB (Proteus/2: 16× DIP32 per board photo) | 96.7 % | Stage 1b |
| Proteus 3 | 171 | 4,007,766 | 4 MB [U] | 95.6 % | Stage 1b |
| Vintage Keys | 727 | 13,411,702 | 8 MB (VK) / [U] VK Plus | > 8 MB: [I] includes Vintage Keys Plus content | Stage 2 |
| Production Set | 384 | 9,132,756 | none | — | Emulator X library, not a Proteus module ROM; no hardware target |

The ROM rebuild needs the firmware's sample table (ROM word address of each sample, loop points). Check: the firmware's Sound ROM test (diagnostics item 7) on the rebuilt image.

## Rebuilt Proteus/1 sound ROM (functional reconstruction)

Built by `tools/rebuild_proteus1_rom.py build/proteusxr_v200.bin "roms/_src/proteus1_exb/Proteus 1/SamplePool" build/proteus1_rebuild --sine roms/proteusxrpl`.
Output staged at `roms/_rebuilt/proteus1_waves.bin` (big-endian 16-bit, 4 MB) with `rebuild_report.txt`; per-chip images in `build/proteus1_rebuild/`.
MAME loads it at start-up with `-autoboot_script mame/scripts/load_waves.lua` (the driver keeps the base set `NO_DUMP`).

**Not a dump.** Sources of each part:

| Part | Source | Confidence |
|---|---|---|
| Sample placement (194 records: start/end/loop, ROM word addresses) | Firmware XR v2.00 table at `0x112EE` (v2.30: `0x18A12`) | [O] read by the voice-start code at v2.30 `0x3912–0x3A40` |
| Sample PCM | Emulator X `.ebl` files, matched per record (tiers A–H in the script) | A/B (79 records): loop points identical or uniformly shifted → [I] same data. C–G (59): loops re-done by the conversion; aligned on start or loop end. H (26): 133-word single-cycle waves assigned by order only. |
| Silent regions | 29 records with no plausible `.ebl` (mostly drum one-shots, some orchestral and short waves); ≈ 15 % of sample words | Gaps stay zero |
| Header words `0x000–0x003` | Byte-lane test pattern `0100 0302 0504 0706` | [O] same on ProtoLogic; the firmware's Sound ROM test checks it |
| Header ID text `0x004–0x01F` | `REBUILT` (ours) | Original text unknown |
| Test sine `0x150–0x1BF` | Copied from the ProtoLogic ROM | [I] E-mu standard test waveform; used by Channel Test / Freq Response |
| Checksums `0x020–0x027` | Computed; compensation byte in `0x14C–0x14F` | Scheme [O] verified on all 8 ProtoLogic chips: stored 16-bit value = byte sum of the whole chip |

Firmware-side check (the diagnostics Sound ROM test) needs the sound-chip read-back register (`+0x18`) in the MAME device; pending.
