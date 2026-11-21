# MAME reference model

Our changes to upstream MAME live in `patches/0001-emu68k-proteusxr-sound.patch` (base commit in `patches/BASE`).

```sh
git clone https://github.com/mamedev/mame.git third_party/mame
git -C third_party/mame checkout $(cut -d' ' -f3 mame/patches/BASE)
git -C third_party/mame apply ../../mame/patches/0001-emu68k-proteusxr-sound.patch
make -C third_party/mame SUBTARGET=proteus SOURCES=src/mame/emusys/emu68k.cpp NOWERROR=1 REGENIE=1 -j8
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
