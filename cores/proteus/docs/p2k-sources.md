# E-mu Proteus 2000 hardware family: public sources for FPGA reimplementation

Research date: 2026-11-21. Web only. No ROM sound images downloaded. Firmware zips were inspected by reading only the zip
central directory (HTTP range request); one 1.8 KB text member was extracted the same way.

Legend: **[O]** = a source states it (URL given). **[I]** = inferred from the cited observations.

---

## 0. Top findings (highest value first)

1. **E4 Ultra full main-board schematic is public and uses the same chip set as the P2K.**
   `https://www.deepsonic.ch/deep/docs_manuals/e-mu_eos_technical_documents.pdf` (318 pp., E-MU PN FI11039 Rev A, 1999).
   - Ultra parts list (p.130): G-Chip Sound Engine 2.0 IC402 ×2, EMU8000 IC405 ×1, H-chip Digital Filter 1.5 IC413 ×4,
     "5206 3.3V Coldfire µProcessor" IM478, CY2292-375 clock generator, AV9170 PLL, TLC2932 PLL, CS4329 DAC ×4, CS5335 ADC,
     Altera 6016 FPGA "(Custom Chip)", K-Chip PLCC-68, 16 MHz crystal oscillator (only crystal listed) [O].
   - Same E-mu part numbers as snw.lonningdal.no lists for the P2K (IC402 ×2, IC413 ×4, IC405) [O] → the Ultra schematic is the
     closest available reference for the P2K voice/filter/FX datapath [I].
   - Schematic SK10485 "MAIN E4 ULTRA RACK V2", rev A.1, dated 10-5-1999, pages 263–290 of the PDF [O]. Sheets include:
     clock generation (p.265), MCF5206E CPU 160-pin PQFP with full pin list (p.266), CPU memory/FPGA/flash/EEPROM (p.268–269),
     word-clock ×512 PLL (p.270), G2.0 chips (p.276–277), H1.5 chips 0&1 / 2&3 (p.279–280), G0.5/EMU8000 effects + sampling
     FIFO + CS8411 AES receiver (p.281), DAC sheets (p.283–286) [O].
   - G2.0 net names visible in text layer: SMA0–SMA25 (sound-memory address), SMD0–SMD15 (16-bit data), SMRAS0–3N, SMOEN,
     SMWEN, G1SRCD0/1 and G2SRCD0/1 (serial sample data to H chips), G2SYNCN, GHRSTN, GHRWAITN, G20CS1N/G20CS2N, master/slave
     strapping; master G2.0 sits in a PGA121 socket (JC364) [O]. H-chip sheet nets: CSCDI/CSCDO (cascade), DACD/DACDL, STB0N–STB7N,
     HCHIPCSN, ODD/EVEN channels [O].
   - Also contains: SK535 "16MB Sound ROM SIMM for E4/e64" (p.261–262), SK417 Sound Flash SIMM (p.194), SK415 ROM SIMM (p.193),
     EIV/E4K/E4XT schematics (68EC020-based predecessors), BootProm diagnostics, EOS version history, service bulletins [O].

2. **P2K .dli OS files are raw, uncompressed ColdFire images, and their load address is known.**
   `http://web.archive.org/web/2026id_/https://dbwbp.com/synthbin/e-mu-proteus-2k-flash-analysis.zip` contains
   `proteus-2k-v124-28f160.bin` (2,097,152 B, full flash dump of a v1.24 unit) and `proteus2k-flash-analysis.txt` (toobias, Feb 2021) [O].
   The text states: big-endian; 0x000000–0x0100FF boot block (bootstrap 0–0x1FF, code 0x400–0x8880 containing DRAM/stuck-bit tests,
   "G chips (U28,U15,U24) tests", "Searching for OS", "Downloading from"); 0x010100–0x09213F main OS = byte-identical to
   `p2k124.dli` (532,544 B); v2.26 .dli is 535,080 B; 0x0A0000+ user presets/setups (512 presets) [O].
   → Any .dli can be placed at flash offset 0x10100 to reconstruct the OS region; the boot block must come from a real dump [I].

3. **CPU confirmed as ColdFire MCF5206E.**
   - E4 Ultra schematic: MCF5206E, 3.3 V, 160-pin PQFP, clocked from CY2292 output `C33M` via `UPCLK_A..D` [O] (33 MHz [I]).
   - Yahoo xl7 group (search snippet, 2013-07-11, Jack Pratt): "the processor is a freescale (ex Motorola) coldfire on the
     ultra/p2k/command…"; 2014-07-16: "That slot is a DRAM slot for the program memory of the coldfire microprocessor. The command
     station has 8MB of SDRAM on the motherboard" [O] — `https://synth-diy.org/yg-archives2/messages?q=coldfire&scope=xl7`.
   - Gearspace (via search-result summary only; page 403 to fetch): Audity 2000 uses XCF5206E at 33 MHz vs P2K at 40 MHz [O, unverified]
     `https://gearspace.com/board/electronic-music-instruments-and-electronic-music-production/1216512-e-mu-audity-2000-p2k-series-service-manual.html`.
   - Syntaur P2K main board photo (AP10182, PCB "PC10182 REV 2", "E-MU SYSTEMS INC ©1998"):
     `https://cdn.syntaur.com/media/items/11261-Lg.jpg` — CPU marking reads approximately "COLDFIRE XCF5206EFT40" [I: low-res read];
     EMU8000 marking legible; Xilinx Spartan XCS05XL PC84 visible; six AMI-fabbed custom QFPs (2 smaller + 4 larger) [O photo / I identification].

4. **Official audio specs (P2K Operation Manual Rev E, p.192):** "Sample Playback Rate: 44.1 kHz", "16-bit linear data, 20-bit ∆∑
   main outputs, 18-bit submix outs", "Filters: 6th Order (17 different types)", "Audio Channels: 128", "Effects Engine: 24-bit
   internal processing" [O] — `https://www.emumania.net/wp-content/uploads/emu/manuals/E-MUProteus2000OperationManualRev.E.zip`.

5. **H-chip channel = 6th order; 12th-order filters consume two channels.** Audity 2000 V2 data sheet: "Audio Channels: 64 (2 per 12th
   order filter, 1 per 6th order filter)"; "Filters: 12th order (33 different types), 6th order (17 different types)" [O] —
   `https://github.com/findsp/emu-audity-2000-upgrade/tree/main/audity-2000-resources`. Matches ds10fan model "three sections in
   series" per voice [O] → one H-chip voice = 3 biquads; 12th order = 2 voices cascaded (CSCDI/CSCDO nets) [I].

6. **Signal path / chip-count statement from an E-mu employee:** Aaron Eppolito, xl7 group, 2010-03-19 (search snippet): "the E-Mu signal
   path is as follows: G chip -- H chip -- G05 chip -- outputs The G chip, of which there is 1 per 64 channels, is the sound
   generation…" [O] — `https://synth-diy.org/yg-archives2/messages?q=coldfire&scope=xl7` (message 20433; full message blocked by Anubis).

---

## 1. Products on the platform

### 1a. Device-family member codes (authoritative product list)
Proteus Family SysEx spec 2.2 Device Inquiry reply: manufacturer 18h, family 04h 04h, member "Audity 02h 00h", "Proteus 03h 00h" [O]
(`http://snw.lonningdal.no/p2k/OpMan/E-MU_ProteusFamilySysEx2.2.pdf`, live, 1,021,847 B). Full member-code table from the
open-source editor prodatum (`https://github.com/haxorhax/prodatum/blob/master/src/pxk.cpp`, `PXK::get_name`) [O]:

| Code | Model | Code | Model |
|---|---|---|---|
| 0x02 | Audity 2000 | 0x0E | MP-7 |
| 0x03 | Proteus 2000 | 0x0F | Proteus 2500 |
| 0x04 | B-3 | 0x10 | Orbit 3 |
| 0x05 | XL-1 | 0x11 | PK-6 |
| 0x06 | Virtuoso 2000 | 0x12 | XK-6 |
| 0x07 | Mo'Phatt | 0x13 | MK-6 |
| 0x08 | B-3 Turbo | 0x14 | Halo (Ensoniq brand) |
| 0x09 | XL-1 Turbo | 0x15 | Proteus 1000 |
| 0x0A | Mo'Phatt Turbo (Turbo Phatt) | 0x16 | Vintage Pro |
| 0x0B | Planet Earth | 0x17 | Vintage Keys (P2K-era keyboard) |
| 0x0C | Planet Earth Turbo | 0x18 | PX-7 |
| 0x0D | XL-7 | | |

Wikipedia also lists Proteus Orchestra (2000) and Proteus Custom (2002) [O] (`https://en.wikipedia.org/wiki/E-mu_Proteus`); no member
code seen for them → likely report as Proteus 2000 [I]. "Command Station" = XL-7 / MP-7 / PX-7 / Proteus 2500 (shared Command
Station OS 2.00) [O] (emumania pages below).

**Not on this platform:** Planet Phatt (1997), Orbit 9090/V2, Carnaval — 68000-based, covered by MAME `emu68k.cpp` [O]
(`https://github.com/mamedev/mame/blob/master/src/mame/emusys/emu68k.cpp`). "Xtreme Lead-1" = XL-1 above.

### 1b. Polyphony / voice and filter chip counts

| Model(s) | Voices | G-II (IC402) | H1.5 (IC413) | FX | CPU | Source |
|---|---|---|---|---|---|---|
| Proteus 2000, Virtuoso 2000, Orbit 3, Vintage Pro, Turbo models | 128 | 2 [O P2K] | 4 [O P2K] | EMU8000 [O P2K] | MCF5206E (40 MHz forum) | snw p2k.php; emumania |
| Proteus 1000 | 64 | 1 [O] | 2 [O] | EMU8000 [I] | ColdFire [I] | snw p2k.php: "P1K has only two filter chips and one sound engine chip" |
| XL-1, Mo'Phatt, B-3, Planet Earth (non-Turbo) | 64 [O] | 1 [I] | 2 [I] | EMU8000 [I] | ColdFire [I] | emumania (use "ProteusV2.26OS64Voice.zip") |
| PK-6, XK-6, MK-6, Halo, Vintage Keys (keyboards) | 64 [O] | 1 [I] | 2 [I] | [I] | [I] | emumania keyboard pages; vintagesynth |
| XL-7, MP-7, PX-7, Proteus 2500 | 128 [O] | 2 [I] | 4 [I] | [I] | ColdFire [O xl7 snippet]; + Xilinx XCS05XL [O photo] + 8 MB SDRAM on board [O snippet] | polynominal.com; emumania |
| Audity 2000 | 32 (v1) / 64 (v2 OS) [O] | 1 [I from "1 G chip per 64 channels"] | [Unknown, ≥2] | EMU8000 [I] | XCF5206E 33 MHz [O forum] | SOS Aug 1998; A2K v2 data sheet |
| E4 Ultra / E4XT Ultra / E6400 Ultra / E-Synth Ultra / E5000 Ultra | 128 | 2 [O] | 4 [O] | EMU8000 [O] | MCF5206E 3.3V [O] | deepsonic tech doc |
| EIV, e64, E4K, E4X, e6400, E-Synth (pre-Ultra, 1994–98) | 64–128 | 1–2 G2.0 [O] | 2–4 H1.5 [O] | EMU8000 "G0.5 effects board" [O] | 68C020/68EC020 25 MHz [O] | deepsonic tech doc |

Notes:
- G-chip naming: EIIIXP used "original 32-voice G-chip"; EIV used "a 128-voice configuration of two G-chip IIs" (Rossum) [O]
  `https://www.amazona.de/interview-dave-rossum-e-mu-english-version-part-four/`. → 64 channels per G-II [O, consistent with Eppolito].
- Older E-mu effects used an ADSP-2105 (16-bit); EMU8000 ("G0.5") was designed so "E-mu could bypass the sampler and use just the
  effects engine with our G and H chips" [O] (same interview).
- Turbo upgrade: 64→128 voices, 2→6 outs + S/PDIF, more ROM slots [O, retailer listings] (`https://zzounds.com/item--EMU9114`) → Turbo
  adds the second G-II + 2 H1.5 [I]; physical form (daughterboard vs. new mainboard) [Unknown].

---

## 2. Service manuals / schematics

| Document | URL | Contents |
|---|---|---|
| EOS Technical Documents FI11039 Rev A (1999), 318 pp. | `https://www.deepsonic.ch/deep/docs_manuals/e-mu_eos_technical_documents.pdf` | Parts lists (all IC part numbers & locations) for EIV, E4K, E4XT/e6400/E-Synth Rack, **Ultra**; schematics SK414/500/411/415/417/518 (EIV), SK502–505 (E4K), SK524/507/535 (E4XT), **SK10485 (E4 Ultra main)**, SK510/511/639/10255 (option cards); diagnostics; service bulletins (incl. "48kHz Digital Rework Mod"). Text layer is extractable with `pdftotext` (net names, pin numbers). [O] |
| EOS Sound Author manual FI10993 Rev B | `https://www.deepsonic.ch/deep/docs_manuals/e-mu_eos_sound_author.pdf` | How Ultra writes Proteus flash SIMMs; "Big Flash" (EOS samples + headers + EOS presets) vs "Baby Flash" (Proteus presets/instruments); flash SIMM models 6876 (16 MB) / 6877 (32 MB); flash ROM IDs 106–119. [O] |
| Proteus 2000 Operation Manual Rev E | emumania zip (above) | Tech specs p.192; 17 filter types list p.108. [O] |
| Audity 2000 Op Manual Rev C, V2 data sheet, 2.0 addendum | `https://github.com/findsp/emu-audity-2000-upgrade/tree/main/audity-2000-resources` | Specs incl. channel/filter-order accounting. [O] |
| Proteus Family SysEx 2.2 | snw (above); also emumania zip | Parameter IDs, dump formats, hardware-config reply (SIMM ID, #presets, #instruments per SIMM), LCD screen dump (P2K/A2K only). [O] |
| Audity 2000 service manual | — | Ray Bellis has a paper copy (estate sale); not published [O] `https://zine.r-massive.com/e-mu-audity-2000-os-upgrade-for-v1-00-units/`. Gearspace thread says it was never released [O, search summary]. |
| P2K / XL-7 / PK-6 service manual or schematic | — | **Not found publicly.** syntaur forum: only the Ultra schematic is "on the WWW" [O] `https://forums.syntaur.com/t/e-mu-schematics-or-service-manuals/748`. polynominal.com "SERVICE MANUAL / SCHEMATICS" link just points to a guestbook [O]. A Yahoo xl7 post offered a hard copy of the EOS tech docs (private) [O]. |
| E4 Ultra service manual (full) | Facebook group "EMU Samplers & Software" | Mentioned by a modwiggler/DIY write-up; not fetched [O via search summary] `https://www.modwiggler.com/forum/viewtopic.php?t=156617`. |

### Clock / memory facts extracted from SK10485 (E4 Ultra) [O unless marked]
- Y1 = 16 MHz crystal oscillator (ZX318) → CY2292-375 (U32) outputs `C33M` (CPU → UPCLK_A–D), `C16M`, `C24M`, `C4M`, `C22_25M`, `SECLK`.
- AV9170-1 PLL (U35) generates `FS512` (fanned out as FS512_A–F to G2.0, H1.5, DACs) and `FS1024` (to EMU8000/G0.5); `SEL48K` select net.
  → FS512 = 512 × fs = 22.5792 MHz at 44.1 kHz [I]; Ultra supports 44.1/48 kHz [I from SEL48K + word-clock PLL].
- TLC2932 (U37) = "WORD CLOCK X 512 PLL" for external sync.
- Pre-Ultra boards (EIV/E4X) used discrete oscillators 16 MHz, 24 MHz, 45.1584 MHz (=1024×44.1k), 49.152 MHz (=1024×48k).
- CPU chip selects UPCS0N–UPCS6N, RAS0N/RAS1N + CAS0N–3N DRAM, 93C46 EEPROM, flash with FLASHPGM, Altera FPGA config (CFGN/CFGDONE/INITDONE).
  No memory-map table found in the text layer [Unknown; may be readable from the PAL equations, which are not included].
- SK535 16MB Sound ROM SIMM: 4× MX23C3210 (2M×16 mask ROM) or MX23C1610 (8 MB build) or MX29F1610 flash; 74ACT138 decodes SMA[22:20]
  (approximately) into SMCS0N–3N; 16-bit data SMD[15:0]; 72-pin SIMM connector pin numbers are in the text layer of p.262 but need
  visual reading of the drawing to map cleanly [O parts / I decode].
- SK417 Sound Flash SIMM: 4× DD28F016SA/28F032SA; table "G2.0 CHIP ADDRESS RANGE (WORDS)" 000000–7FFFFF [O].

### P2K-specific clocks / DACs
- Output rate 44.1 kHz [O manual]. P2K board crystal values: [Unknown] (not in any fetched source; Syntaur photo too low-res).
- DACs per board revision [O snw p2k.php]: early (E-MU logo, 1998 silkscreen): CS4329 main + AD1861 ×2 subs, NE5532 op-amps;
  later (Pactron, 2000): AK4324 ×3, JRC4560; variant with TSOP56 flash + JRC4562. P2500 = AK4324/JRC4560.
- Manual: 20-bit ∆∑ mains, 18-bit subs [O] → matches CS4329 + AD1861 (early board) [I].
- Audity 2000: "44.1kHz sample playback rate, with 18-bit outputs" [O SOS Aug 1998, `https://www.soundonsound.com/reviews/emu-audity-2000`];
  emumania says "39kHz 16-bit" [O, conflicts; likely refers to ROM sample data rate, unverified].

---

## 3. Firmware / OS files

| Model | Version | File | URL | Notes |
|---|---|---|---|---|
| P2K-family 128-voice | 2.26 (2002-11-01) | ProteusV2.26OS128Voice.zip | `https://www.emumania.net/wp-content/uploads/emu/os/ProteusV2.26OS128Voice.zip` | [O] |
| P2K-family 64-voice (P1K, XL-1, Mo'Phatt, B-3, Planet Earth) | 2.26 | ProteusV2.26OS64Voice.zip | `https://www.emumania.net/wp-content/uploads/emu/os/ProteusV2.26OS64Voice.zip` | [O] |
| P1K + P2K | 2.26 | p1k226.dli (524,072 B), p2k226.dli (535,080 B) + E-Loader 1.1 | `http://snw.lonningdal.no/p2k/firmware226_p1kp2k_e-loader111.zip` (9.5 MB, live) | [O, zip directory read] |
| P2K | 1.24 | full 2 MB flash dump (boot block + OS + presets) + analysis | dbwbp zip via Wayback (above) | dbwbp.com itself timed out 2026-11-21; Wayback copy works [O] |
| Command Station (XL-7/MP-7/PX-7/P2500) | 2.00 + Command Prep 10 | CommandStationV2.00OS.zip, CommandPrep10.zip | `https://www.emumania.net/wp-content/uploads/emu/os/` + filenames | [O] |
| Audity 2000 | 2.01 | Audity2000v2.01OSUpgrade.zip | `https://www.emumania.net/wp-content/uploads/emu/os/Audity2000v2.01OSUpgrade.zip` | [O] |
| Audity 2000 | 1.00 / 2.00 / 2.01 | audity-update-0.3-*.bin (AM29F040B boot ROM image containing xz-compressed 2 MB OS + updater; menu has "Dump OS to MIDI") | `https://github.com/findsp/emu-audity-2000-upgrade/tree/main/bin` ; `https://github.com/FIN-DSP/emu-audity-2000-upgrade` | Ray Bellis, CC BY-NC-ND. Boots from U13 PLCC32 with W1 jumper; supports Intel/Micron 28F800 flash. Mirror on dbwbp: `E-mu_audity_2000-v1.0_update_ROM-0.3.zip` [O] |
| PK-6/XK-6/MK-6/Halo/Vintage Keys | — | — | not found | [Unknown] |
| EOS Ultra | 4.7 | — | not located in this pass (modwiggler mentions a Dropbox archive) | [Unknown] |

Also on dbwbp (Wayback snapshot 20261025194015 of `https://dbwbp.com/index.php/9-misc/37-synth-eprom-dumps`): "Emu Emulator Boot ROMs
(various, includes rare WILDCARD).zip" (may include EIV/Ultra boot ROMs — contents not inspected), Planet Phatt 1.01, Orbit 9090,
Carnaval, ESI-32/4000, Proteus 1/XR [O].

emu.tools (Ray Bellis): SPA with tools e-loader (MIDI firmware update), audity (Audity 2000 update firmware), e-xplorer (E-mu disk
image explorer), e-dumper (P2K/CS preset/pattern/song loader), e-sysex, e-sds, e-remote (E4/EOS remote) [O, from JS bundle
`https://www.emu.tools/main-ZSSRHRQR.js`]. Direct `/e-loader` returns HTTP 403 to non-browser clients. Issue tracker only:
`https://github.com/raybellis/emu.tools-public`. Related: `raybellis/emu3fs` (Linux FS driver for EIII/EIV disks), `raybellis/emu3bm` [O].

---

## 4. Sound-ROM SIMMs

- Format: 72-pin SIMM; 16-bit wide; up to 32 MB per SIMM; four wave-ROM footprints + preset ("baby") flash/ROM [O snw ID doc; haxorhax;
  SK535]. Gearspace: "basically some old Intel Flash ICs on a 72-pin stick" [O search summary] `https://gearspace.com/threads/emu-flash-rom.1221995/`.
- PCB variants PC530 (A2K, 4× 44-pin ROMs, presets in ROM), PC638, PC10172, PC10498 (Sharp 28F160 TSOP56 preset flash), PC12068 (3.3 V,
  74LVT245/16245 buffers), flash SIMMs PC642 (DA28F320J5 / DA28F640J5) and PC10903 (E28F320J3A / E28F640J3A) [O]
  `http://snw.lonningdal.no/p2k/Identifying_EMU_ROM_SIMMs_for_Proteus_2K_Family.txt` (live; .doc original at
  `https://synth-diy.org/yg-archives2/raw/xl7/attachments/210788658/96520353-Identifying-EMU-ROM-SIMMs-for-Proteus-2K-Family.doc`).
  That doc also lists per-SIMM wave-ROM IP numbers (e.g., Composer IP1035–1038, preset IP10193) and retail part numbers [O].
- PC10498: U5 = LH28F160S5T (preset flash), U6 = 74ACT138 [O] `http://learnsax.co.uk/docs/make-simms.pdf`.
- Data layout: EOS sample data + EOS presets ("Big Flash") readable by Ultra; Proteus presets/instruments in "Baby Flash" [O Sound Author].
  → sample headers follow the EOS bank format; Ray Bellis's emu3fs / e-xplorer parse EIII/EIV formats [I].
- **ROM IDs** (prodatum `src/data.cpp` `ROM::name`) [O]: 0 USER, 2 XTREM, 3 AUDTY, 4 CMPSR, 5 P-123 (Protozoa), 6 B-3, 7 XL-1, 8 ZR/QROM,
  9 WORLD, 10 ORCH1, 11 ORCH2, 13 PHATT, 14 XL-7/XROM, 15 MP-7/MROM, 16 SONIQ, 17 P2500/PROM1, 18 VROM, 19 DRUM, 64 GRAIL, 65 TECNO,
  66 AORCH, 67 BEAT. SysEx spec: "xlead = 7" [O]. User flash SIMMs: 106–119 [O Sound Author].
- Clone maker: haxorhax `https://haxorhax.com/emu-rom-clones` (reprogrammable clones, split-ROM variants, ESYNTH/AUDTY/XTREM;
  no images distributed) [O]. SysEx preset archives: `https://haxorhax.com/emu-sysex-archive` (ALL_SYSEX.zip ~10 MB) [O].
- Public sound-ROM images (archive.org etc.): **none found** [O negative search]. Arduino-based ROM→flash SIMM copying reported on Gearspace,
  no code linked [O search summary].

---

## 5. Emulators, prior RE, photos, forums

### MAME
- No driver for any ColdFire-era E-mu product. `src/mame/emusys/` = emax.cpp, emu2.cpp, emu3.cpp, emu68k.cpp [O, GitHub API].
- `emu68k.cpp` (AJR): skeleton for Proteus/1, Proteus/1 XR, Pro/Cussion, Vintage Keys (1993), Orbit 9090, Planet Phatt, Carnaval —
  MC68000 @ 40 MHz/4, MC68901 MFP, HD44780; MACHINE_NO_SOUND | NOT_WORKING [O].
- MCF5206E CPU core exists: `src/devices/cpu/m68000/mcf5206e.cpp` (used by BFM SC5/AD5, JPM Pluto6/Sys7, Elektron Monomachine,
  funcube, gamtor) [O] — usable as a CPU+SIM peripheral reference model [I].

### Chip-level RE
- **H-chip**: `https://github.com/ds10fan/emu-hchip-patent` — Rust cycle-level model of US 5,170,369 (Rossum, dynamic digital IIR filter):
  FIG 6 datapath, FIG 7 block-floating-point multiplier, FIG 8 coefficient decoder, reservation-table microcode, Table I/II interpolator;
  `VOICE_INTERFACE.md` gives per-voice 10×32-bit control words + cutoff + flags, coefficient code decode formula, measured on an E4 to ~0.02 dB;
  widths (24-bit acc, 16-bit delay/mult operands) are guesses [O]. Includes Rossum's ARMAdillo and filter-morphing papers.
- **Z-plane (software)**: `https://github.com/noneofyourbisness/EMU-Emulator-X3-ZPlaneFilter-Research` — bit-exact reimplementation of
  Emulator X3's 55 filters (17 standard + 33 Z-plane + 5 programmable), float, 44.1–192 kHz tables. AI-generated code per README [O].
  Relevance to H1.5 coefficient tables: [Unknown].
- **G-chip interpolation**: US 5,111,727 (Rossum, 1992) multi-channel interpolative sample playback, 7th/8th-point interpolation
  `https://patents.google.com/patent/US5111727`; related US 5,144,676, US 5,698,803 (cache memory for interpolator channels) [O].
  Rossum: G-chip = "order seven" interpolator design goal; G-II "very similar but not identical" maths, "slightly brighter" [O amazona parts 3/4].
  Dattorro (`https://ccrma.stanford.edu/~dattorro/EffectDesignPart2.pdf`, p.769 note) — Proteus uses 7th-order Remez-designed polynomials [O via KVR].
  KVR measurement on e6400 Ultra suggested something close to linear at -2 oct (contested, informal) [O] `https://www.kvraudio.com/forum/viewtopic.php?t=594228`.
- **EMU8000 (G0.5)**: no P2K-specific RE found. General emulation: 86Box `src/sound/snd_emu8k.c` (behavioural, cubic/linear resampler, Moog-style filter) [O];
  Linux `sound/isa/sb/emu8000*.c` (register map, effects init) [O]. EMU8000 used 4-point interpolation [O, Rossum article cited in search].
- No decap / die photos of G-II or H1.5 found [O negative].

### Photos with chip markings
- Syntaur P2K main board AP10182 / PC10182 rev 2 (1000×569): `https://cdn.syntaur.com/media/items/11261-Lg.jpg` [O].
- polynominal XL-7 page: Xilinx Spartan XCS05XL PC84 close-up `https://www.polynominal.com/Emu-XL1-Xtreme-Lead/xilinx%20spartan.jpg`, Intel flash,
  Mo'Phatt internals `https://www.polynominal.com/emu-mo-phatt/internal.jpg`; page states "Coldfire-based processor" [O].
- Audity 2000 board (U13 PLCC32 socket, W1/W2 jumpers, Micron flash): findsp repo `images/` [O].
- Ray Bellis article images (Audity jumpers/socket): zine.r-massive.com link above [O].

### Forums / reviews
- Yahoo xl7 group archive (search works; message pages are behind Anubis JS proof-of-work and were not fetched):
  `https://synth-diy.org/yg-archives2/group/xl7/`. Relevant threads by subject: "[p2k] Any interest remaining in a self-programming FLASH SIMM?"
  (2013, 82 msgs; ".. 249918 lines of assembly code in the disassembled 2.0 OS image"), "FPGA & ASIC [Re: Open source the OS]" (2003),
  "re: P1000 / P2000 / P2500 / Command Station - Hardware Differences?" (2014), "[WTB] Schematics of E4 Ultra Sampler or Proteus 2000 series
  module" (msgs 23521–23522), "HELP: FX RAM failed error" (2010, Eppolito signal-path post), "Service Manuals" (24230), "XX-7 SCHEMATICS" (17174) [O titles/snippets].
- Gearspace (HTTP 403 to automated fetch): "What's this Proteus 2000 chip?" (1041927), "E-mu Audity 2000 (p2k series) Service Manual???" (1216512),
  "emu flash rom" (1221995), "HELP! Looking for an ex EMU technician for a FLASH ROM project for Command stations" (1055342) [O titles].
- Sound On Sound: Proteus 2000 (Mar 1999: 128 voices, 17 Z-plane types, two 24-bit FX, six 18-bit outs) `https://www.soundonsound.com/reviews/emu-proteus-2000`;
  Audity 2000 (Aug 1998: 44.1 kHz, 18-bit outs, 50 Z-plane types, 16 MB ROM); also Mo'Phatt, Proteus 2500, Vintage Pro, XL-1, E4XT Ultra reviews exist [O].
- Rossum interviews (amazona.de, English, parts 1–4): chip naming (E/F/G/H/K/R chips), G-II/H-chip/EMU8000 history [O].

---

## 6. Clock / sample-rate summary

| Item | Value | Status |
|---|---|---|
| P2K output / playback rate | 44.1 kHz | [O] manual p.192 |
| Audity 2000 playback rate | 44.1 kHz (SOS, manual) vs "39kHz" (emumania) | [O, conflicting] |
| P2K CPU clock | 40 MHz (XCF5206E…FT40) | [O forum] + [I photo] |
| Audity 2000 CPU clock | 33 MHz | [O forum, unverified] |
| E4 Ultra CPU clock | C33M from CY2292-375 off 16 MHz crystal | [O net names] / 33 MHz [I] |
| G2/H1.5 master clock | FS512 (512×fs) from AV9170 PLL; EMU8000 gets FS1024 | [O net names, E4 Ultra] |
| P2K crystal frequencies | — | [Unknown] |
| P2K DACs | CS4329 + 2× AD1861 (1998 boards); 3× AK4324 (2000+ Pactron boards) | [O snw] |
| Ultra DACs | 4× CS4329, ADC CS5335, AES rx CS8411 | [O] |

---

## Unknown / not found
- P2K/Command Station/keyboard schematics or service manual (none public).
- P2K crystal values, FPGA (XCS05XL) function and bitstream, CPU memory map (PAL/FPGA equations absent).
- G-II and H1.5 register maps / internal word widths (only patent-based models exist); G-II exact interpolation kernel.
- EMU8000 effects microcode as used in P2K.
- Turbo-upgrade hardware form; Audity 2000 H-chip count; per-model chip population for 64-voice units (inferred only).
- Public PK-6/XK-6/MK-6/Halo/Vintage Keys OS files; EOS 4.7 download location.

## Access issues during this pass
- dbwbp.com: TCP connect timeout (used Wayback snapshot 2026-10-25).
- gearspace.com: HTTP 403 (Cloudflare) to WebFetch and curl.
- synth-diy.org yg-archives2 message pages: Anubis JS challenge; only search-result snippets used.
- web.archive.org CDX API: "Temporarily Offline"; individual snapshots worked.
- emu.tools subpages: HTTP 403 to non-browser clients.

## Added 2026-11-21
- Pistol Instruments Proteus 2000 Editor (https://www.pistolinstruments.com/e-mu-proteus-2000-editor/,
  app https://proteus2000editor.pistolinstruments.com/): free Web MIDI editor/librarian for the
  whole P2K family. `llms-full.txt` gives the SysEx frame (`F0 18 0F dev 55 cmd … F7`), every
  parameter ID/range/default, the 50 filter types with SysEx values, 73 cord sources / 66
  destinations, effect A (44) / B (32) algorithm counts, preset dump layout [O]. Code is
  "© 2026 Patrik Pistol … do not republish"; contact for hardware findings on the About page.
- MediaFire `Proteus_2000.zip` (268 MB, user-supplied 2026-11-21; local only in `roms/_src/p2k/mf/`):
  CDXtract conversion (SF2 INFO: `ISFT CDXtract`, `ICOP Copyright Supersound Production`, 2008) of
  five P2K-family sound sets as 7052 mono 16-bit WAVs with `smpl` chunks (root key, 3837 loops)
  plus 9 SF2 banks for the Composer set [O].

  | Set | WAVs | Samples (16-bit words) | vs 32 MB SIMM |
  |---|---|---|---|
  | Proteus 2000 (Composer) | 1845 | 16,331,042 | 97.3 % |
  | Mo' Phatt | 1286 | 16,288,984 | 97.1 % |
  | PX7 Protean Drums | 702 | 16,698,921 | 99.5 % |
  | Xtreme Lead-1 | 1332 | 13,725,267 | 81.8 % |
  | Orchestral Kits (Planet Earth + Virtuoso 2000) | 1887 | 50,702,114 | 3 SIMMs [I] |

  The rates are per sample and non-standard (449 distinct values in the Composer set, e.g. 22325,
  42193, 36001 Hz) [O]. That means native rates, not resampled to 44.1 kHz [I]. Most peaks are
  between 16k and 32700, so the sets are not normalized to full scale [O]. They are not a SIMM dump:
  there is no SIMM header, sample directory, instrument or preset data, and no ROM order [O].
  Bit-exactness is unknown. The Proteus/1 `.ebl` conversions rescaled some samples and redid some
  loops (see sample-sources.md). On the P2K the sample table and presets live in the SIMM, not the
  OS, so a rebuild needs the OS's SIMM-directory parser reversed first.
- EMU8000 Programmer's Guide rev 1.00 (Rossum, E-mu/Creative 1994-96) and AWE32 Developer
  Information Pack 3.01, from github.com/qb40/sound-blaster-awe32 `DOC/EMU8KPGM.PDF`,
  `DOC/ADIP301.PDF`, `DOS/EMUINC/EMU8000.H` (local only in `roms/_src/p2k/docs/`). The guide
  documents the voice registers, including the per-voice sends (PTRX bits 15-8 reverb send, CSL
  bits 31-24 chorus send, PSST pan send), and gives the INIT1-4 arrays for the default "Hall 2" /
  "Chorus 3" programs. It treats the arrays as opaque ("can be ignored" after initialisation): the
  effects program they configure is not documented [O]. The init1 table it prints is the one in
  P2K OS 2.26 at 0x0102F8C4.
