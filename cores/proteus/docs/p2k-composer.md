# Composer (ROM ID 4) stand-in SIMM

`tools/p2k_build_composer.py` builds a Proteus 2000 sound SIMM that carries the Composer ROM's
1024 factory presets and its 1173 instruments in E-mu's numbering, from public material. It is
not a dump of the ROM: the instruments' keymaps, envelopes and samples are reconstructed, and
nothing here has been compared with a real unit (none is available, `p2k-mame-plan.md`).

## Sources (none shipped)

| Input | Where from | What it gives |
|---|---|---|
| `CMPSR.zip` | [haxorhax.com E-MU Sysex Archival](https://haxorhax.com/emu-sysex-archive/) (Command Station dumps) | the 1024 presets as SysEx preset dumps, `04-NNNN-name_PRES.syx`, exact |
| `composer_instruments.tsv` | Proteus 2000 Operations Manual Rev E, Instrument Listing ([archive.org](https://archive.org/details/synthmanual-emu-proteus-2000-owners-manual)), parsed to "number, name" | the 1173 instrument numbers and names, exact |
| SF2 banks | the CDXtract conversion of the Composer set (`p2k-sources.md`, MediaFire `Proteus_2000.zip`) | samples with loops and root keys; each preset flattened to one SF2 instrument |
| two `.sav` flash files | the P2K core after OS 2.26 received CMPSR 0-511 and 512-1023 as user presets | the OS's own 980-byte record of every preset |
| `p2k226.bin` | OS 2.26 | the envelope rate table |

The manual's list skips "Cymbal 18" (1025 is Cymbal 17, 1026 Cymbal 19); two entries (928, 929,
Hat 15 and 16) are split across lines by its layout. The CMPSR dumps have 56 common parameters
(OS 2.26 has 52); the four extra are 0 in all 1024 presets.

## Steps

1. **Preset records.** OS 2.26 converts a SysEx preset into its record itself; 86 parameters are
   not stored as plain bytes, so the conversion is left to the OS: on the MiSTer, each half of
   CMPSR goes to user presets 0-511 (header preset number set, ROM ID 0; `syx.py send`, closed
   loop), and the flash save file is copied. The records keep layer instrument ROM ID 4.
   Check: the factory user bank in `flash124.bin` is CMPSR 512-1023 (user 0 = `kb1:DynamicGrand`);
   the second half's records are compared with it.
2. **SF2 presets to CMPSR presets** by name (the SF2 bank files follow the user-bank numbering:
   SF2 "Bank 0" = CMPSR bank 4). 1020 of 1023 match.
3. **Instrument keymaps.** CDXtract wrote each layer as the instrument's zones with the layer's
   coarse (SysEx 1425) and fine tune (1426) added; layer transpose, volume and pan are not in the
   zones, and the layer's key range clips them. Per instrument the zones of the best layer that
   uses it are kept (whole key range first): from presets with one instrument, then by the sample
   names of instruments already found, then by layer key ranges. Instruments still without zones
   get a full-keyboard zone on the SF2 sample of the same name, or none.
4. **Factory envelopes.** 882 of the bank's 1939 layers use the instrument's factory volume
   envelope (SysEx 1793 = 0), stored in the zone (`p2k-lib-formats.md`, zone +20..33). It is
   taken from the SF2 zones of layers in Factory mode and converted to the six-stage form with
   the OS rate table: A1 to 100 at the attack time (at least rate 2), D1 to the sustain level at
   the decay time, R1 to 0 at the release time; the SF2 hold time is dropped.
5. **Samples**: every SF2 sample once (30.5 MB); `-L`/`-R` pairs used together become one stereo
   sample header. Zone volume is the SF2 attenuation relative to the instrument's quietest zone,
   at 0.04 dB per cB (CDXtract adds a constant per instrument, 45 dB on the piano) [I].

## Use

```sh
tools/p2k_build_composer.py --cmpsr CMPSR.zip --sf2 "…/Proteus 2000 Samples/Soundfont" \
    --names composer_instruments.tsv --records half0.sav half1.sav --os p2k226.bin build/p2k/composer4
../p2k/tools/p2k_pack.py simm composer.sim --baby build/p2k/composer4/baby.bin --big build/p2k/composer4/big.bin
P2K_MAME=… P2K_ROMPATH=… tools/p2k_composer_check.py build/p2k/composer4 /tmp/check --every 16
```

`p2k_composer_check.py` needs a `flash_clean.nv` in the SIMM directory (any SIMM's clean boot).

## Results (2026-11-26)

- **Preset records:** OS 2.26 accepted all 1024 (8192 closed-loop packets ACKed). Records
  512-1023 are byte-identical to the factory user bank of `flash124.bin` for 497 of 512 presets;
  the other 15 differ in 1-5 bytes (the CMPSR dumps come from a later OS [I]). Every used layer
  carries ROM ID 4 and the SysEx instrument number.
- **Instruments:** 1173 in the manual's numbering. 615 have zones (424 from presets with one
  instrument, 77 by sample names, 10 by key ranges, 104 from a sample of the same name), 558 none.
  Of the 533 instruments the presets use, 521 have zones; the 12 without are hits, riffs and FX
  (`report.json`), leaving 11 presets with a silent layer. 388 instruments carry a factory
  envelope from the SF2; the rest get instant attack, full sustain, ~0.3 s release.
- **MAME** (`p2k_composer_check.py --every 1`, note 60, velocity 110, 0.8 s each): all 1024
  presets selected by bank select 4/LSB and program change; 1022 show their name on the LCD and
  1013 sound. The 11 silent ones: layers on instruments without zones (`hit:Orch Fatlow`,
  `hit:Brass Hits 3`, `hit:Deep Brass`, ...), kits with nothing on key 60 (`kit:CookUsAHit`,
  `kit:SounDfacTory`, `kit:This & That`), `bs2:Pop Slide 4`, `bs5:Smoothy`, `str:Tron`,
  `brs:Ensemble` and the empty `:default`. Median peak by volume envelope mode: Factory −23.5 dB
  (544 presets), time-based −10.6 dB (433), mixed −13.0 dB (47).
- **MiSTer** (P2K core 2026-11-25, `composer.sim` from `cores/p2k/tools/p2k_pack.py`): the bank
  shows as `CMPSR`; bank select 4/0, 4/5, 4/7 with program changes 0, 0, 48 show
  `brs:SforzadoSect`, `kb4:Dance`, `gtr:Steel`, and the output meter moves for each.

## Known differences from a real Composer ROM

- Keymaps, sample choice and tuning are CDXtract's view of the instruments, which came from the
  E4/EOS library, not the ROM; the zone ends are stretched to keys 0 and 127.
- Zone volumes, factory envelopes (6 stages rebuilt from ADSR, hold dropped, attack ≥ rate 2 and
  ≤ 1 s) and the 0.04 dB/cB scaling are inferences.
- Presets whose layers use Factory envelopes play quieter than time-based ones (median peak
  13 dB lower in MAME). The same envelope in the layer instead of the zone gives the same level
  (A/B on `kb4:Dance`), so the difference is in the reconstructed instruments or the presets
  themselves; there is no reference to say which.
- No arpeggiator patterns (`ya_1`) or riffs (`yr_1`) are in the SIMM; presets that use the ROM's
  arps or riffs get the OS defaults.
- 558 instruments that no factory preset uses have no zones (most are the single-sample
  percussion instruments `prc:*`, whose generic names do not match the sample names).
