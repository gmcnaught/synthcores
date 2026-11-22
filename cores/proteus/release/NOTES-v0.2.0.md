E-mu **Proteus/1 XR** core for MiSTer, second release: the real Proteus/1 sound ROM, Reverse Sound,
E-mu's 7-point interpolation and the per-voice tone filters.

**No ROM, firmware or sample data is included.** Build the system image from your own files with
`tools/build_image.py` (see the README). Rebuild your `proteus_system.bin`: the v0.1.0 image holds
the old reconstructed sound ROM. Your saved presets (`saves/Proteus/proteus_system.sav`) keep working.

## Changes
- **Sound ROM dump.** `build_image.py --waverom poprockroms.zip` uses the dump of the Proteus/1
  sound ROMs (proteus.nekoweb.org). Compared with the v0.1.0 reconstruction: no silent samples
  (29 before), 44 samples that had the wrong data, samples one word out of place, and samples at
  the level E-mu's Emulator X conversion had changed. The reconstruction (`--samples`) stays as a
  fallback and its one-word offset is fixed.
- **Reverse Sound** now plays backwards. The firmware programs a negative pitch increment; the
  previous chip model read it as a very fast forward one, which produced noise.
- **Interpolation and tone.** The sound chip now interpolates with 7 points, using the filter
  kernel printed in E-mu's patent US 5,111,727 (previously linear interpolation). The firmware's
  per-voice tone value ("Dark" instruments use it; notes above a sample's root get more of it)
  selects one of a family of lower-cutoff kernels, as that patent describes. Their exact cutoffs
  are not published; tones 1–5 are designed to match the published kernel. OSD **Tone Filter**
  (On by default) set to Off uses the published kernel for every voice. See `docs/patents.md`.

## Install
1. Build `proteus_system.bin`:
   ```sh
   python3 tools/build_image.py --protologic "Proteus 1 XR w Protologic - v2.30.zip" \
       --waverom poprockroms.zip --font hd44780.zip -o proteus_system.bin
   ```
2. Copy the `.rbf` to `/media/fat/_Other/`, `proteus_system.bin` to `/media/fat/games/Proteus/`, and
   optionally `Proteus.mgl` to `/media/fat/`.
3. Load the core, OSD → **Load System** (or run `Proteus.mgl`).

## Verified
- The firmware's own Sound ROM test passes on the dump (MAME).
- Sound chip HDL bit-exact against the reference model on a 57 s preset demo and on a reversed
  note recorded from the firmware in MAME.
- On a MiSTer: a sample the v0.1.0 image left silent (program 46, C4) plays; Reverse Sound
  (SysEx parameter 40) plays the note backwards; 5-minute dense-MIDI soak with 0 missed sample
  deadlines, worst case 1,376 of 2,048 clocks per sample. Quartus timing met (setup slack 0.505 ns).

## Known limits
- Tone 1–5 cutoffs, envelope slew and pan law are inferred; nothing has been compared with a real
  unit.
- Not yet verified: assignable controllers A–D, SysEx dumps, multitimbral setups.
- Only XR v2.30 + ProtoLogic is supported.
