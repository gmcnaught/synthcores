# E-mu Proteus → MiSTer core: research plan & blockers

Status date: 2026-11-20. Labels: **[O]** observed in a cited source, **[I]** inferred, **[U]** unknown / unverified.

**First target: Proteus/1 XR (1989).** The Proteus 2000 stays the long-term goal (Stage 4, section 7).

## 1. Progression

| Stage | Target | New unknowns added | Status |
|---|---|---|---|
| 1 | **Proteus/1 XR** | 68000 platform, 1 sound chip, 4 MB sample ROM | **Active** |
| 1b | Proteus/1, Pro/Cussion, Proteus/2/3 (+XR) | Firmware/memory-map variants, other sample ROMs | After Stage 1 |
| 2 | Vintage Keys / Planet Phatt / Orbit / Carnaval | Low-pass filter DSP chip | Later |
| 3 | UltraProteus / Morpheus | H-chip Z-plane filter (same family as P2K); CPU [U] | Later |
| 4 | Proteus 1000 → 2000 | ColdFire CPU, G-II voice chip, EMU8000 FX, 128 MB ROM | Long-term |

The MIDI, LCD/front panel, ROM loading, NVRAM save and voice-pipeline setup built in Stage 1 carry over to every later stage [I].

## 2. Proteus/1 XR target hardware

Source: MAME `src/mame/emusys/emu68k.cpp`, set `proteusxr` (skeleton, `MACHINE_NO_SOUND | MACHINE_NOT_WORKING`), plus reviews.

| Block | Detail | Evidence |
|---|---|---|
| CPU | MC68000P10, 40 MHz XTAL / 4 = 10 MHz | [O] MAME |
| Program ROM | 128 KB, 2× NMC27C512AN, `ip441c_emu__5089.ic38` (MSB) / `ip442c_emu__5089.ic37` (LSB), CRC32/SHA1 listed in MAME | [O] MAME |
| Work RAM | `0xFFC000–0xFFFFFF` (16 KB) | [O] MAME map |
| NVRAM | 2× Sony CXK5864BP-12L (8 KB each) + DS1210 + battery — marked TODO in MAME | [O] MAME comment; relation to the work-RAM range [U] |
| Timer / MIDI UART | MC68901P (MFP) at `0x900000–0x90002F`, clocked 10 MHz; IRQ level 1 unverified; interrupt vector read via FC7 space | [O] MAME |
| LCD | HD44780, 2×16 characters, at `0x700000–0x700003` | [O] MAME |
| Unidentified I/O | `0x400000` (write), `0x800000–0x8003FF` (write, 1 KB), `0xA00000` (read, "watchdog?"), `0xB00000` (write) | [O] MAME. [I] hypothesis: `0x800000` block = sound-chip registers (1 KB ÷ 32 voices = 32 B/voice); unverified |
| Sound | 32 voices, 16-bit samples, 4 MB sample ROM; no filters; no effects (chorus = doubling a voice, costs polyphony) | [O] Micro Music 1990, SOS 1992, Wikipedia |
| Sound chip | Identity, register map, sample rate, interpolation [U] | No source found |
| Presets | 384 total: 0–255 user RAM, 256–383 ROM | [O] Proteus manual, DTM Wiki |
| Expansion | Optional 4 MB sound expansion board (E-mu Proteus/2 sounds or Protologic) → 8 MB | [O] SOS 1992 |
| Audio out | 3 stereo output pairs; 2 pairs have effect-return inputs | [O] SOS 1992 |
| MIDI | In / Out / Thru | [I] standard for the line; confirm on unit |
| Front panel | Buttons + data entry control; input path empty in MAME (`INPUT_PORTS` stubs) | [O] MAME; wiring [U] |

Base Proteus/1 (set `proteus1`, firmware v2.05) uses a different map: LCD at `0x600080`, MFP at `0x600100`, clock 10 MHz unverified, MFP type flagged "not the right type at all" [O]. E-mu upgraded base units to XR [O Vintage Synth Explorer], so whether these are different boards or different firmware is [U].

### 2a. Material in hand (2026-11-20; kept outside the repo)

| File | MAME set | CRC32 / SHA1 match | Notes |
|---|---|---|---|
| `e-mu proteus 1 msb 2.05.bin`, `… lsb 2.05.bin` (2× 64 KB) | `proteus1` (base unit, **not XR**) | Yes (6a84ac60 / 3ae786b4) | Interleaved image: SSP=`0x00FFFFFE`, reset PC=`0x00005092`; version string `1992 E-mu v2.05` |
| `hd44780_a00.bin` (4 KB) | `hd44780` | Yes (e459877c) | MAME marks it `BAD_DUMP`: rebuilt from the 1985 HD44780 datasheet (A00 character set), not read from a chip |

| `ip441c_emu__5089.ic38` (MSB), `ip442c_emu__5089.ic37` (LSB) (2× 64 KB) | `proteusxr` | Yes (01f11633 / e8154a65) | Interleaved image: SSP=`0x00FFFFFE`, reset PC=`0x0000494A`; version string `1989 E-mu v2.00`; Sound ROM test labels `ROM#1–ROM#8` |

XR firmware is v2.00 (1989), older than the base Proteus/1 v2.05 (1992) [O]. The XR Sound ROM test lists 8 ROMs vs. 4 on v2.05 [O]; [I] the 8 slots cover the base 4 MB plus the 4 MB expansion, but whether an XR ships with 8 ROM chips is [U].

Firmware strings show a built-in **diagnostics menu** [O]: `1 Auto Cycle, 2 Auto Test, 3 Panel, 4 LCD, 5 NV RAM, 6 Program ROM, 7 Sound ROM, 8 NV Glitch, 9 MIDI, 10 Freq Response, 11 Channel Test, 12 Audio Return, 13 Initialize`. Sound ROM test labels `ROM#0–ROM#3`, `Bank A`, `Bank B`; Freq Response generates 20 Hz–19 kHz test tones.
- [I] 4 sample ROM chips × 1 MB = 4 MB; Bank B = the 4 MB expansion board.
- [I] Disassembling the Sound ROM test gives the sample-ROM address layout and checksum; Channel Test and Freq Response give known-good sound-chip register sequences. These are the first disassembly targets in Phase 1.

## 3. Blockers for Stage 1 (ranked)

1. **Sound chip undocumented.** No name, part number, register map, sample rate, interpolation, envelope, or pan behavior found. Needs firmware disassembly + logic-analyzer captures + output recordings.
2. **Sample ROM not dumped.** MAME: "all of these have sample ROMs as well (none currently dumped)". Chip count, package, pinout, data width, any scrambling [U]. Requires an owned unit.
3. **No schematics found.** A Gearspace teardown thread reports none available for the 1-XR [O]. E-mu published service manuals for other models (Emulator III service manual is on synthmania.com) [O]; a Proteus/1 service manual's existence is [U].
4. **Front-panel wiring unknown.** Button/encoder scan path not in MAME.
5. **Sample-ROM licensing.** Digital Sound Factory holds the license for historical Proteus sound content [O]. The core ships no ROMs; users supply dumps.
6. **Output mismatch.** 3 stereo pairs → MiSTer stereo; mixdown needed.

No longer blockers at Stage 1 (vs. P2K): CPU (fx68k exists), firmware availability (dumped and hashed in MAME), filters and effects (none), memory size (4–8 MB fits the standard 32 MB SDRAM).

## 4. ROM / firmware sources for testing (never shipped with the core)

| Source | Use | Notes |
|---|---|---|
| Own Proteus/1 XR | Dump program EPROMs, sample ROMs, NVRAM; bus captures; reference recordings | Primary source. Verify EPROM dump against MAME CRC/SHA1. |
| MAME `proteusxr` hashes | Dump verification | MAME distributes hashes, not ROMs |
| Python-Blue (Patreon/blog) | Has extracted a Proteus/1 XR + Protologic sound ROM | Does not host standard Proteus 1/2 dumps; blog post now 404. Contact re: extraction method. |
| dbwbp.com `Proteus 1 XR w Protologic - v2.30.zip` (7.6 MB) | **ProtoLogic expansion sound ROMs only**: 8× 512 KB mask ROMs `IP505–IP508`, `IP705–IP708` (4 MB total), AKN62314BP datasheet, 4 MB 16-bit LE WAV conversion; OS EPROMs `IP485`/`IP486` v2.30 | Readme: standard Proteus 1 samples deliberately excluded. ROMs are 27C040-pinout compatible (dumped as AM27C040) [O]. Real platform-format sample data for decoding the ROM format and sound-chip bring-up. |
| dbwbp.com firmware-only sets | `E-MU Proteus 1 XR v2.03.zip`, `E-mu Proteus XR Firmware V2.03.zip`, `E-mu Proteus XR LSB-MSB (2.0).zip` (+ board photo), `emu_proteus-2xr.zip` (Proteus/2 XR `IP566A`/`IP567A` + photo of its 16× DIP32 wave ROMs), Vintage Keys (Plus), Pro/Cussion, Planet Phatt, Orbit, Carnaval | No standard Proteus/1/2/3 sample ROMs on the page (checked 2026-11-20) |
| Digital Sound Factory | License holder for Proteus sound content | Possible permission for test use [U] |
| Synthetic test ROM (generated in-repo) | CI / simulation / public test vectors | Possible once sample-ROM format is decoded |
| Factory preset SysEx / manual preset list | Preset-engine validation | Public manuals (manualslib) |

## 5. Stage 1 plan

**Phase 0a – Start without hardware (inputs in hand: XR v2.00, Proteus/1 v2.05, HD44780 font; available: ProtoLogic XR v2.30 OS + its 4 MB sound ROMs)**
- MAME: boot `proteusxr`; add a ProtoLogic variant pairing OS v2.30 (`IP485`/`IP486`) with `IP505–508`/`IP705–708` — firmware and sound ROMs from the same unit.
- Disassemble the Sound ROM test, Channel Test, Freq Response and note-on paths; log sound-chip register writes.
- Decode sample-ROM layout (chip interleave, sample headers/tables) and cross-check against the included WAV conversion.
- First audible output: a provisional MAME sound device driven by logged writes + ProtoLogic samples.
- Not resolvable without a unit: bit-exact sound-chip behavior (interpolation, envelope timing, mixing, output rate), front-panel wiring, standard Proteus/1 sample ROMs, I/O ranges the firmware never exercises.

**Phase 0 – Acquire & document**
- Buy a Proteus/1 XR (optionally a base Proteus/1 to resolve the memory-map question).
- Photograph both board sides; record every IC marking, crystal, socketed vs. soldered parts, jumper and connector.
- Dump program EPROMs and compare with MAME hashes. Dump the sample ROMs (EPROM programmer + adapter, or in-circuit reader if soldered). Read NVRAM contents via SysEx preset dump and/or directly.
- Record reference audio: one-shot notes per sample, pitch sweeps, velocity/envelope tests, captured digitally from the outputs.

**Phase 1 – MAME reference model**
- Extend `proteusxr` in `emu68k.cpp`: front-panel inputs, NVRAM (CXK5864/DS1210), confirm MFP IRQ level, identify `0x400000/0x800000/0xA00000/0xB00000`.
- Disassemble firmware; locate the sound-chip driver; log all writes to the sound-chip range during note-on/off.
- Target: boots to the LCD UI, accepts MIDI via MFP USART. Upstream to MAME.

**Phase 2 – Sound chip reverse engineering**
- Logic-analyzer captures of the CPU↔sound chip and sound chip↔sample ROM buses while playing single-voice test notes.
- Derive: register map, pitch/phase accumulator, interpolation, loop handling, envelope generator, volume/pan, output sample rate, mixing to 3 stereo pairs.
- Implement as a MAME sound device; compare against Phase 0 recordings (target: bit-exact where digital capture allows).

**Phase 3 – MiSTer HDL**
- Template_MiSTer core. CPU: fx68k (≈5,171 ALMs per MiSTer reference). MFP: reuse an existing MC68901 implementation (e.g. from the MiSTer Atari ST core — verify licensing/compatibility).
- HD44780 2×16 LCD + front panel rendered to video; OSD/controller mapping for buttons and data entry.
- Sound chip HDL (32 voices, time-multiplexed); sample ROM in SDRAM loaded via MRA/ioctl; NVRAM as save file.
- MIDI in via `UART_*` ports / MidiLink; MIDI out optional.
- Verilator testbench comparing against the MAME model.

**Phase 4 – Stage 1b variants**: base Proteus/1, Pro/Cussion (firmware v1.01 in MAME), Proteus/2 and /3 sample sets, 4 MB expansion board.

## 6. Open questions for Stage 1
1. Sound chip identity and register map.
2. Sample-ROM chip count, package, width, pinout, and any address/data scrambling.
3. Output sample rate.
4. What the `0x400000`, `0x800000`, `0xA00000`, `0xB00000` I/O ranges are.
5. Whether base Proteus/1 and XR are different boards or different firmware.
6. Front-panel scan wiring.
7. Whether a Proteus/1 service manual exists.

## 7. Stage 4 reference: Proteus 2000 (retained findings)

| Block | Part | Evidence |
|---|---|---|
| Host CPU | ColdFire MCF5206e, OS in on-board flash | [U] single forum claim |
| Voice engine | 2× G-II "Sound Engine" (IC402) → 128 voices | [O] snw.lonningdal.no |
| Filters | 4× H1.5 Z-plane (IC413) | [O] snw; H1.5 also in E4 Ultra service manual |
| Effects | 1× EMU8000 (IC405), dual 24-bit FX, pitch-shift path bypassed | [O] snw |
| RAM | 8 MB DRAM (P2K), 4 MB (P1K) | [O] snw |
| Sound ROM | 4 SIMM sockets; ≤32 MB wave ROM (4 ICs) + 2–4 MB preset flash per SIMM | [O] "Identifying EMU ROM SIMMs" doc |
| DACs | CS4329 + AD1861 (1998 board) or AK4324 (2000 board); S/PDIF | [O] snw |
| Firmware | OS 2.26 (2002-11-01), `.dli` via E-Loader over MIDI | [O] snw, E-Loader manual |

P1K = 1 G-II + 2 H1.5, 2 SIMM slots, 64 voices [O].

**P2K blockers:** G-II undocumented (nearest relative EMU8000: AWE32 docs, 86Box `snd_emu8k.c`); H1.5 widths guessed (US 5,170,369; `ds10fan/emu-hchip-patent`); EMU8000 FX programs undocumented; no schematics; no ColdFire soft core on MiSTer (MAME has `mcf5206e`); SIMM pinout/scrambling unknown; 128 MB ROM needs 128 MB SDRAM or DDR3; 6 outs and 2 MIDI ins vs. MiSTer stereo/1 UART.

**P2K test sources:** own unit + Composer SIMM (01, IP1035–1038, presets IP10193); OS 2.26 (`snw.lonningdal.no/p2k/firmware226_p1kp2k_e-loader111.zip`, emu.tools/e-loader); haxorhax.com ROM clones and SysEx archive; dbwbp.com `e-mu-proteus-2k-flash-analysis.zip` (contents [U]); Proteus VX (Composer samples in `.exb`); Proteus Family SysEx 2.2 spec. Ray Bellis dumped Audity 2000 flash over MIDI with a custom boot ROM, so the family is dumpable in principle.

## Sources
- MAME: https://github.com/mamedev/mame/blob/master/src/mame/emusys/emu68k.cpp ; https://github.com/mamedev/mame/blob/master/src/devices/cpu/m68000/mcf5206e.cpp
- Wikipedia: https://en.wikipedia.org/wiki/E-mu_Proteus
- SOS 1992 Hands On: https://www.muzines.co.uk/articles/hands-on-emu-systems-proteus/9558
- Micro Music 1990: https://www.muzines.co.uk/articles/proteus-one/5367
- Proteus manual preset map: https://www.manualslib.com/manual/895817/E-Mu-Proteus.html?page=76
- DTM Wiki: https://dtm.noyu.me/wiki/Proteus/1
- Vintage Synth Explorer: https://vintagesynth.com/e-mu/proteus
- Gearspace 1-XR teardown: https://gearspace.com/board/electronic-music-instruments-and-electronic-music-production/897430-taking-apart-e-mu-proteus-1-xr.html
- Python-Blue: https://www.python-blue.com/blog/tech-talk-sound-rom-extraction (404 as of 2026-11-20)
- Emulator III service manual: https://synthmania.com/wp-content/uploads/2020/06/EmulatorIII_Service_Manual.pdf
- P2K: https://snw.lonningdal.no/p2k.php ; https://snw.lonningdal.no/p2k/Identifying_EMU_ROM_SIMMs_for_Proteus_2K_Family.txt ; https://haxorhax.com/emu-rom-clones/ ; https://haxorhax.com/emu-sysex-archive/ ; https://dbwbp.com/index.php/9-misc/37-synth-eprom-dumps ; https://archive.org/details/proteus-vx_202103 ; http://zine.r-massive.com/e-mu-audity-2000-os-upgrade-for-v1-00-units/ ; https://github.com/ds10fan/emu-hchip-patent ; https://github.com/noneofyourbisness/EMU-Emulator-X3-ZPlaneFilter-Research ; https://github.com/86Box/86Box/blob/master/src/sound/snd_emu8k.c ; https://www.emu.tools/e-loader/ ; https://c3.zzounds.com/media/ProteusFamilySysex2.2-7da5faed3832df43c6123b96293b3cd1.pdf
