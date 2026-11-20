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
- `emu68k.cpp` (`proteusxr`): RAM at 0xff0000 (XR detection), MIDI in/out on the MFP, 6850 debug serial at 0xd00000, output-jack sense at 0xc00001, volume pot via MFP timer B pulse width (0xa00000 strobe), per-voice pan at 0x500009, sound ROM region; new set `proteusxrpl` (ProtoLogic expansion).
- `mc68901.cpp`: timer A/B pulse-width mode now counts at the prescaled rate while the input is active (it never counted before).

Run headless (never takes macOS focus): `tools/mame-headless.sh <set> [args]`.

Scripts in `scripts/` (`-autoboot_script mame/scripts/run_test.lua`, choose with `PROTEUS_SCRIPTS=a.lua:b.lua`):
| Script | Purpose |
|---|---|
| `load_waves.lua` | Load `roms/_rebuilt/proteus1_waves.bin` into the sound region (base set is NO_DUMP) |
| `trace_io.lua` | Log sound-chip and I/O accesses to `PROTEUS_TRACE` |
| `call_init.lua` | Call the firmware's diagnostics "Initialize" (factory presets into NVRAM) |
| `sound_rom_test.lua` | Call the firmware's Sound ROM test on bank `PROTEUS_ROMTEST_BANK`; result in `error.log` as `ROMTEST d0=0` (pass). Needs `-debug -debugger none -log`. |

Verified 2026-11-20 (`proteusxrpl`, firmware v2.30):
- Sound ROM test passes on bank 1 (real ProtoLogic dump) and bank 0 (rebuilt Proteus/1 ROM).
- MIDI notes 48/60/72 play at 131.1/262.3/521.7 Hz (expected 130.8/261.6/523.3).
- Presets 0–23 all audible, no clipping (`build/demo_presets.wav`).
