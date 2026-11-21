# MAME reference model

Our changes to upstream MAME live in `patches/0001-emu68k-proteusxr-sound.patch` and `patches/0002-emup2k-proteus2000-research.patch` (apply in order; base commit in `patches/BASE`).

```sh
git clone https://github.com/mamedev/mame.git third_party/mame
git -C third_party/mame checkout $(cut -d' ' -f3 mame/patches/BASE)
git -C third_party/mame apply ../../mame/patches/0001-emu68k-proteusxr-sound.patch
git -C third_party/mame apply ../../mame/patches/0002-emup2k-proteus2000-research.patch
make -C third_party/mame SUBTARGET=proteus SOURCES=src/mame/emusys/emu68k.cpp,src/mame/emusys/emup2k.cpp NOWERROR=1 REGENIE=1 -j8
```

What the patch adds:
- `src/mame/emusys/proteus_snd.*`: behavioural model of the Proteus/1 sound chip (32 voices, 39062.5 Hz, register map and inferences documented in the source header).
- `emu68k.cpp` (`proteusxr`): RAM at 0xff0000 (XR detection), MIDI in/out on the MFP, 6850 debug serial at 0xd00000, output-jack sense at 0xc00001, volume pot via MFP timer B pulse width (0xa00000 strobe), per-voice pan at 0x500009, sound ROM region with the dumped Proteus/1 sound ROMs (IP382–IP385, IP402–IP405; from proteus.nekoweb.org `poprockroms.zip`, put them in `roms/proteusxr/`); new set `proteusxrpl` (ProtoLogic expansion).
- `mc68901.cpp`: timer A/B pulse-width mode now counts at the prescaled rate while the input is active (it never counted before).

Run headless (never takes macOS focus): `tools/mame-headless.sh <set> [args]`.

Scripts in `scripts/` (`-autoboot_script mame/scripts/run_test.lua`, choose with `PROTEUS_SCRIPTS=a.lua:b.lua`):
| Script | Purpose |
|---|---|
| `load_waves.lua` | Replace the sound region with an image file (`PROTEUS_WAVES`, e.g. the rebuilt ROM, for comparison) |
| `trace_io.lua` | Log sound-chip and I/O accesses to `PROTEUS_TRACE` |
| `call_init.lua` | Call the firmware's diagnostics "Initialize" (factory presets into NVRAM) |
| `sound_rom_test.lua` | Call the firmware's Sound ROM test on bank `PROTEUS_ROMTEST_BANK`; result in `error.log` as `ROMTEST d0=0` (pass). Needs `-debug -debugger none -log`. |

Verified 2026-11-20 (`proteusxrpl`, firmware v2.30):
- Sound ROM test passes on bank 1 (ProtoLogic dump) and bank 0 (Proteus/1 dump; earlier also on the rebuilt ROM).
- Reverse Sound (SysEx parameter 40, `tests/midi/reverse.txt`): the firmware programs a negated increment (`0x000574` → `0x03FA8C`), start = sample end − 0x64 and a 24-word loop below the sample start; the model plays it backwards.
- MIDI files given with `-midiin` start playing about 10 s after power-on.
- MIDI notes 48/60/72 play at 131.1/262.3/521.7 Hz (expected 130.8/261.6/523.3).
- Presets 0–23 all audible, no clipping (`build/demo_presets.wav`).

## Proteus 2000 research machine (`proteus2k`, patch 0002)
`src/mame/emusys/emup2k.cpp`: MCF5206e, 8 MB DRAM, the 2 MB flash, every device stubbed and logged
(memory map in `docs/p2k-findings.md`). Not a working emulation; it exists to run the real boot
block and OS and log what they do. `mcf5206e.cpp`: core logging reduced to invalid/timer/SIM.

ROMs in `roms/proteus2k/`: `proteus-2k-v124-28f160.bin` (required; dbwbp.com
`e-mu-proteus-2k-flash-analysis.zip`) and optionally `p2k226.dli` (OS 2.26 E-Loader file), which
replaces the flash OS at 0x010100. Run: `tools/p2k-run.sh WALL OUTDIR -seconds_to_run N
[-autoboot_script ../../../mame/scripts/p2k_pc.lua]` (host-time limit and a ~200 MB log cap).

Verified 2026-11-21:
- Boot block: chip selects, DRAM SIMM test, G05 ID (0x30), G20 U15/U24 tests, OS copy and jump.
- With `p2k226.dli`: OS 2.26 shows "E-mu Systems, Inc. presents", passes its G chip, G05 and FX RAM
  tests, reports "No Sound Roms detected" and idles (0x010372DC, RTOS idle task).
- With the dump's own OS 1.24: "Fatal: Bus Address error" (jump to 0x00044EB9) after start-up.

Verified 2026-11-21 (synthetic SIMM, OS 2.26):
- Build a SIMM from WAVs: `tools/p2k_build_simm.py tests/p2k_simm_demo.json build/p2k/simm/demo`
  (spec paths point at the local, git-ignored sample set in `roms/_src/p2k/mf/`). Run with
  `P2K_SIMM1=$PWD/build/p2k/simm/demo/big.bin P2K_BABY1=$PWD/build/p2k/simm/demo/baby.bin
  tools/p2k-run.sh 300 build/p2k/run -seconds_to_run 32 -midiin $PWD/build/p2k/p2k_demo.mid
  -wavwrite $PWD/build/p2k/run/demo.wav` (MIDI from `tools/mkmidi.py tests/midi/p2k_demo.txt`).
- The OS accepts the SIMM (header, 8 block checksums, Baby library), shows bank "P2Ksy" on the
  main screen, takes bank select MSB 4 + program change from MIDI In A, and plays: note 60 on
  the piano instrument is periodic at 261.6 Hz, note 72 at 523.3 Hz, A4 on preset 1 "wav:Sine"
  at 440 Hz.
- Levels: the knob ADC at 0xC00000 is modelled (MAME adjusters; master volume defaults to full).
  The OS then writes per-voice level bytes and the model applies them: layer volume −20 dB →
  −18 dB, pan, velocity, the factory and time-based envelopes all change the output.
- `tools/p2k_diff.py run NAME ID=VALUE…` builds a SIMM variant with layer-0 parameters set and
  logs the note-on/note-off register writes (`P2K_TRACE=1`); `p2k_diff.py diff BASE NAME`.
- Not modelled: filters, effects (H chip, G05). The user flash is read-only, so the OS reports
  "FlashInit error" at start-up and continues.


Front panel and storage (2026-11-21):
- Keys: Q Master, E Edit, M Multi, S Save/Copy, Enter Home/Enter, Left/Right cursor, A Arp,
  Space Audition, -/= data entry (also a MAME dial); knobs are MAME adjusters (master volume full).
- The flash is a writable 28F160; user presets and settings persist in `nvram/proteus2k/flash`.
  `P2K_CLEAN=1` (first run, no NVRAM) erases the dump owner's user data so the OS builds defaults;
  let it run ~60 s emulated once.
- MIDI Out: `-midiout` (UART1 TX).

Filters, effects, LEDs (2026-11-21), all approximations marked [I] in the driver:
- Filter: per-voice SVF classed from the OS's section patterns, cutoff from the frequency byte
  (`P2K_NOFILTER=1` bypasses). Effects: behavioural reverb/delay/chorus/flange from the OS's
  active FX settings (`P2K_NOFX=1` disables). Output −6 dB headroom.
- LEDs: MAME outputs `led0`–`led15`.
- Interactive use (window, sound): `P2K_SIMM1=… P2K_BABY1=… third_party/mame/proteus proteus2k
  -rompath roms` (keys listed above; master volume and knobs under Slider Controls).
- Stress test: 400 notes on two channels with sustain and mod wheel, no clipping, ~8× real time
  headless (`build/p2k/stress.txt` pattern).
