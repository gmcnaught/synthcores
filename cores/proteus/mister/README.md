# Proteus/1 XR core (MiSTer)

Runs the original Proteus/1 XR firmware (v2.30, ProtoLogic expansion) on an fx68k 68000 with
HDL models of the MC68901 MFP, the HD44780 LCD and the Proteus/1 sound chip. MIDI comes in on the
MiSTer UART (MidiLink). No ROM, firmware or sample data is part of the core.

## Use
1. Build the system image from your own data:
   ```sh
   python3 tools/make_system.py roms/_rebuilt/proteus1_waves.bin roms/proteusxrpl \
       roms/proteusxrpl/ip486_901101.bin roms/proteusxrpl/ip485_901101.bin \
       build/run_xr/nvram_proteusxrpl_init.bin roms/hd44780/hd44780_a00.bin build/proteus_system.bin
   ```
   Contents: 8 MB sample ROM (rebuilt Proteus/1 + ProtoLogic), 128 KB firmware, 64 KB preset RAM
   (a MAME NVRAM file after the firmware's Initialize; see `mame/README.md`), 4 KB LCD font.
2. Copy `Proteus_<date>.rbf` to `_Devices/` and the image to `games/Proteus/proteus_system.bin`.
3. Load the core, OSD → **Load System**. The firmware boots (about 2.3 s) to `C01 Vol127 Pan=P` /
   `000 Stereo Piano`.
4. MIDI: set the MiSTer UART mode to MIDI (MidiLink) and play a USB MIDI keyboard, or send bytes to
   `/dev/ttyS1` at 31250 baud (`tools/mister_midi.py`).

`tools/mister_system.sh RBF build/proteus_system.bin tests/midi/hw_test.txt` installs, boots, plays
MIDI, screenshots and reads back the audio capture over SSH (`MISTER=root@<address>`).

## Controls
| Front panel | Keyboard | Joystick |
|---|---|---|
| MASTER | M | button 1 (A) |
| EDIT | E | button 2 (B) |
| CURSOR | C or Tab | button 3 (X) |
| ENTER | Enter | button 4 (Y) |
| Data-entry knob −1 / +1 | Left, Down, `-` / Right, Up, `=` (repeats while held) | left/down / right/up |
| Volume knob | OSD **Volume** (Max/75/50/25 %) | |
| (core) output level | MiSTer core volume: above 0 dB gives the framework boost (+12 / +24 dB, soft knee). OSD **Output Level** adds 0 (default) / +6 / +12 / +18 dB with hard saturation | |

On screen: the front panel (`cfg/panel.yaml`, drawn by `synthframe_panel_art`) with the LCD, the
LEDs (MIDI, MASTER, EDIT, ENTER) lit and the buttons shown pressed, and one level bar per voice.
Wiring (from the firmware, verified in simulation): buttons on MFP GPIP0–3 (active low:
0 MASTER → `MASTER TUNE`, 1 EDIT → `PRESET NAME`, 2 CURSOR, 3 ENTER); knob = GPIP4 falling edge
with GPIP5 as direction (1 = −1); LEDs = latch 0xB00001 bits 0 (MASTER), 1 (EDIT), 3 (MIDI), active
low. Holding MASTER + EDIT at power-on enters the diagnostics menu (GPIP0–3 = 1100).

Verified on a MiSTer (build 6, 2026-11-20) with keys injected through uinput (`tools/mister_keys.py`):
`M` → `MASTER TUNE +00` with the MASTER LED on, `E` → `PRESET NAME` with the EDIT LED on,
Right ×2 → `002 InChoirIrie`. A MIDI controller sending MIDI clock (e.g. a Launchkey at 120 BPM,
48 bytes/s) keeps the MIDI LED lit, since the LED is stretched to 60 ms per received byte.

## Saving presets
The image is loaded with `FS1` in CONF_STR, so MiSTer mounts `saves/Proteus/<image>.sav` on SD
slot 0. `rtl/nv_ctrl.sv` restores the 64 KB battery RAM from it before the CPU starts (if the file
holds 64 KB) and writes it back on OSD **Save Presets** or when the OSD opens (**Autosave**).
Images from `tools/build_image.py` have blank RAM and flag byte 0x831000 = 1: with no save file,
`rtl/init_macro.sv` drives the panel lines through the firmware's diagnostics *Initialize*
(MASTER+EDIT at power-on, knob to item 13, ENTER, knob, ENTER), restarts the CPU and saves 4 s later.

Verified on a MiSTer (build 9, 2026-11-20): with no save file, first boot initialised the presets
and created `saves/Proteus/proteus_system.sav` (65,536 bytes); stepping to preset 003 and opening
the OSD rewrote the save; reloading the core came back on `003 BigCityBrass`.

## Design
| Block | File | Notes |
|---|---|---|
| Top | `Proteus.sv` | 80 MHz `clk_sys`; `UART31250,MIDI` in CONF_STR enables MidiLink |
| System | `rtl/proteus_sys.sv` | fx68k at 10 MHz (phi enables every 4 clocks), MAME `proteusxr` memory map, ROM/RAM/font block RAM (`rtl/bram.sv`), volume one-shot, IACK |
| MFP | `rtl/mfp68901.sv` | MC68901 subset modelled on MAME (interrupts, timers incl. pulse-width, 8N1 USART) |
| LCD | `rtl/hd44780.sv`, `rtl/display.sv` | HD44780 modelled on MAME incl. busy timing; 2×16 LCD, front panel art (`rtl/panel/`) + 32 voice bars on 320×240 |
| Sound | `rtl/snd_ctrl.sv`, `rtl/proteus_snd.sv` | CPU write FIFO drained between samples, +0x18 read-back, DDR3 samples, capture ring |
| CPU | `rtl/fx68k/` | fx68k by Jorge Cwik (GPL-3), unmodified |

DDR3 (physical): `0x30000000` samples (8 MB), `0x31000000` output capture ring (16 MB).

## Verification
- `sim/sys_main.cpp` (Verilator, full system): from power-on, the CPU's LCD (4720), sound-chip (770)
  and other I/O writes in the first 2.5 s equal MAME's (`mame/scripts/trace_writes.lua`,
  `tools/compare_writes.py`); RAM/MFP differ only by sub-millisecond interrupt jitter. A MIDI
  note-on programs the same voice registers as MAME (start, loop, pitch) and plays audio.
- With the MAME fixes in patch 0001 (2026-11-23), 6 s from power-on: sound-chip (770) and other
  I/O writes still equal; the firmware's stored volume (0xFFF98E) is 0xCA on all 188 measurements
  in both (the previous MAME stored one 0xCF, from a timer B catch-up burst).
- `sim/sim_main.cpp` (sound chip only): bit-exact with `tools/proteus_model.py`.
- Earlier event-replay demo build: hardware capture bit-exact with the model for 66 s.

## Build
`bash mister/build.sh` with Quartus Lite 17.0, or the `Build RBF` GitHub workflow.
The event-replay demo (`rtl/demo_ctrl.sv`, `rtl/level_display.sv`) is kept for the sound-chip
testbench but is not in the core.
