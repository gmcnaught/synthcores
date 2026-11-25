# E-mu Proteus/1 XR for MiSTer

An FPGA core of the E-mu Proteus/1 XR sound module (1989–90) for the MiSTer (DE10-Nano). It runs the
original firmware on a 68000 (fx68k) with HDL models of the MC68901 MFP, the HD44780 LCD and the
Proteus/1 sound chip (32 voices, 39.0625 kHz). Play it from a MIDI keyboard through MidiLink; edit
from the MiSTer keyboard or a controller.

No ROM, firmware or sample data is included. You build the core's system image from files you
download yourself (below).

## What you need
| File | Where | Used for |
|---|---|---|
| `Proteus 1 XR w Protologic - v2.30.zip` | dbwbp.com synth EPROM archive (also on proteus.nekoweb.org) | firmware v2.30 and the ProtoLogic expansion sound ROMs |
| `poprockroms.zip` | proteus.nekoweb.org ("Proteus/1 Waverom Dump") | the Proteus/1 sound ROMs (IP382–IP385, IP402–IP405) |
| `hd44780.zip` | a MAME ROM set | LCD character generator |

Without the sound ROM dump, `--samples "Proteus 1.zip"` (archive.org `e-mu-sample-sets`, E-mu's
Emulator X conversion) rebuilds an approximation instead: 29 samples silent and others wrong or
at a different level.

## Build the system image
```sh
python3 tools/build_image.py \
    --protologic "Proteus 1 XR w Protologic - v2.30.zip" \
    --waverom poprockroms.zip --font hd44780.zip -o proteus_system.bin
```
The script checks every input by SHA-1 and needs only Python 3.

## Install
- `Proteus_<date>.rbf` → `/media/fat/_Devices/`
- `proteus_system.bin` → `/media/fat/games/Proteus/`
- optional `Proteus.mgl` (`release/Proteus.mgl`; loads the core and the image in one step):
  ```xml
  <mistergamedescription>
      <rbf>_Devices/Proteus</rbf>
      <file delay="1" type="f" index="1" path="proteus_system.bin"/>
  </mistergamedescription>
  ```

Load the core, then OSD → **Load System**. On the first boot the core loads the factory presets by
running the firmware's own diagnostics *Initialize* (about 9 s; the LCD shows the steps), restarts,
and saves the presets to `saves/Proteus/proteus_system.sav`. Later boots restore that file.

## MIDI
Set the core's UART mode to **MIDI** in the OSD and connect a USB MIDI keyboard to the MiSTer
(MidiLink). Notes, program changes, CC7 (volume) and CC10 (pan) are tested; the Proteus/1's other
MIDI features (multitimbral setups, assignable controllers A–D, SysEx) come from the original
firmware and have not been verified on the core yet.

## Controls
| Front panel | Keyboard | Joystick |
|---|---|---|
| MASTER | M | button 1 |
| EDIT | E | button 2 |
| CURSOR | C or Tab | button 3 |
| ENTER | Enter | button 4 |
| Data-entry knob −1 / +1 | Left, Down, `-` / Right, Up, `=` | left, down / right, up |

OSD: **Volume** (front-panel volume knob), **Output Level** (extra gain, 0 dB default; the MiSTer core
volume boost also works), **Tone Filter** (on by default, see Known limits), **Save Presets**,
**Autosave on OSD open** (on by default), **Factory Reset Presets** (re-runs the initialisation;
overwrites your presets on the next save).

The screen shows the front panel (`cfg/panel.yaml`) with the LCD, the MIDI / MASTER / EDIT / ENTER LEDs
lit and the buttons shown pressed, and a level bar per voice.

## Known limits
- The sound chip is modelled from the firmware's use of it and from E-mu's patents of the period
  (`docs/patents.md`): 7-point interpolation with the patent's printed kernel. The envelope slew
  rate and the pan law are inferred; nothing has been compared with recordings of a real unit.
- Tone: the firmware gives each voice a tone value (the "Dark" instruments use it, and notes
  played above a sample's root get more of it). Per the patent it selects one of a family of
  interpolation filters with lower cutoffs; their exact cutoffs are not published, so tones 1–5
  are designed to match the published one (`tools/interp_kernels.py`). The OSD **Tone Filter**
  option (Off) uses the published filter for every voice.
- Only the configuration above (XR v2.30 + ProtoLogic) is supported.

## How it was made / verification
- `docs/`: research plan, hardware notes (memory map, sound-chip registers, front panel), sources,
  E-mu patents that describe the sound chip (`patents.md`).
- `mame/`: MAME driver and sound-chip model used as the reference (patch against upstream MAME).
- `sim/`: Verilator testbenches. The sound chip is bit-exact with the reference model; the full
  system's CPU writes match MAME's from power-on; the core testbench covers first boot, preset
  initialisation and the save file.
- `mister/`: the core (`Proteus.qpf`, Quartus Lite 17.0; `Build RBF` GitHub workflow).

## License
GPL-3.0-or-later; see `LICENSE` and `NOTICE.md` (fx68k by Jorge Cwik, MiSTer framework, MAME
patch under BSD-3-Clause). E-mu, Proteus and ProtoLogic are trademarks of their owners; this project
is not affiliated with them.
