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
2. Copy `Proteus_<date>.rbf` to `_Other/` and the image to `games/Proteus/proteus_system.bin`.
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

On screen: the LCD, LEDs (MIDI, MASTER, EDIT) and one level bar per voice.
Wiring (from the firmware, verified in simulation): buttons on MFP GPIP0–3 (active low:
0 MASTER → `MASTER TUNE`, 1 EDIT → `PRESET NAME`, 2 CURSOR, 3 ENTER); knob = GPIP4 falling edge
with GPIP5 as direction (1 = −1); LEDs = latch 0xB00001 bits 0 (MASTER), 1 (EDIT), 3 (MIDI), active
low. Holding MASTER + EDIT at power-on enters the diagnostics menu (GPIP0–3 = 1100).

## Design
| Block | File | Notes |
|---|---|---|
| Top | `Proteus.sv` | 80 MHz `clk_sys`; `UART31250,MIDI` in CONF_STR enables MidiLink |
| System | `rtl/proteus_sys.sv` | fx68k at 10 MHz (phi enables every 4 clocks), MAME `proteusxr` memory map, ROM/RAM/font block RAM (`rtl/bram.sv`), volume one-shot, IACK |
| MFP | `rtl/mfp68901.sv` | MC68901 subset modelled on MAME (interrupts, timers incl. pulse-width, 8N1 USART) |
| LCD | `rtl/hd44780.sv`, `rtl/display.sv` | HD44780 modelled on MAME incl. busy timing; 2×16 panel + 32 voice bars on 320×240 |
| Sound | `rtl/snd_ctrl.sv`, `rtl/proteus_snd.sv` | CPU write FIFO drained between samples, +0x18 read-back, DDR3 samples, capture ring |
| CPU | `rtl/fx68k/` | fx68k by Jorge Cwik (GPL-3), unmodified |

DDR3 (physical): `0x30000000` samples (8 MB), `0x31000000` output capture ring (16 MB).

## Verification
- `sim/sys_main.cpp` (Verilator, full system): from power-on, the CPU's LCD (4720), sound-chip (770)
  and other I/O writes in the first 2.5 s equal MAME's (`mame/scripts/trace_writes.lua`,
  `tools/compare_writes.py`); RAM/MFP differ only by sub-millisecond interrupt jitter. A MIDI
  note-on programs the same voice registers as MAME (start, loop, pitch) and plays audio.
- `sim/sim_main.cpp` (sound chip only): bit-exact with `tools/proteus_model.py`.
- Earlier event-replay demo build: hardware capture bit-exact with the model for 66 s.

## Build
`bash mister/build.sh` with Quartus Lite 17.0, or the `Build RBF` GitHub workflow.
The event-replay demo (`rtl/demo_ctrl.sv`, `rtl/level_display.sv`) is kept for the sound-chip
testbench but is not in the core.
