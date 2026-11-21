First release of an E-mu **Proteus/1 XR** core for MiSTer: the original firmware (v2.30 with the
ProtoLogic expansion) runs on an fx68k 68000, with HDL models of the MC68901 MFP, the HD44780 LCD
and the Proteus/1 sound chip (32 voices, 39.0625 kHz).

**No ROM, firmware or sample data is included.** Build the system image from your own files with
`tools/build_image.py` (see the README).

## Install
1. Build `proteus_system.bin`:
   ```sh
   python3 tools/build_image.py --protologic "Proteus 1 XR w Protologic - v2.30.zip" \
       --samples "Proteus 1.zip" --font hd44780.zip -o proteus_system.bin
   ```
2. Copy `Proteus_20261120.rbf` to `/media/fat/_Other/`, `proteus_system.bin` to
   `/media/fat/games/Proteus/`, and optionally `Proteus.mgl` to `/media/fat/`.
3. Load the core, OSD → **Load System** (or run `Proteus.mgl`). The first boot loads the factory
   presets through the firmware's diagnostics *Initialize* (about 9 s) and creates
   `saves/Proteus/proteus_system.sav`.
4. Set the UART mode to **MIDI** and plug a USB MIDI keyboard into the MiSTer (MidiLink).

## Controls
MASTER = M, EDIT = E, CURSOR = C/Tab, ENTER = Enter, data-entry knob = arrows or `-`/`=`;
joystick buttons 1–4 and the d-pad do the same. OSD: Volume, Output Level, Save Presets, Autosave on
OSD open, Factory Reset Presets.

## Verified
- Sound chip HDL bit-exact against the reference model (Verilator); full system's CPU writes match
  MAME from power-on; MIDI note-on programs MAME's exact voice parameters.
- On a MiSTer: boots to `000 Stereo Piano`; MIDI plays at correct pitch (C4 = 262.2 Hz); front panel
  and LEDs; first-boot preset initialisation, save, and restore across reloads.
- Soak test: SOAK_RESULT

## Known limits
- The main Proteus/1 sound ROM is rebuilt from E-mu's Emulator X conversion, not a dump: 164 of 193
  samples placed (85 % of sample data); the rest are silent, mostly drum one-shots.
- Sound-chip interpolation, envelope slew and pan law are inferred, not compared with a real unit.
- Untested so far: assignable MIDI controllers, SysEx, multitimbral setups.
- Only XR v2.30 + ProtoLogic is supported.
