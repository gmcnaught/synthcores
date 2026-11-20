# Proteus sound demo core (MiSTer)

Plays register writes captured from MAME (Proteus/1 XR firmware v2.30 playing presets 0–23)
through `rtl/proteus_snd.sv`, an HDL model of the Proteus/1 sound chip. There is no 68000 in
this core yet: the event stream stands in for the firmware.

## Use
0. Capture the register trace from MAME (see `mame/README.md` for building it):
   ```sh
   cp build/run_xr/nvram_proteusxrpl_init.bin build/run_xr/nvram/proteusxrpl/nvram   # presets initialised once with mame/scripts/call_init.lua
   PROTEUS_TRACE=build/demo_trace.txt PROTEUS_SCRIPTS=load_waves.lua:trace_io.lua \
     tools/mame-headless.sh proteusxrpl -seconds_to_run 55 -nvram_directory build/run_xr/nvram \
     -midiin build/demo_presets.mid -autoboot_script mame/scripts/run_test.lua
   ```
   (`build/demo_presets.mid` = `tools/mkmidi.py tests/midi/demo_presets.txt`.)
1. Build the demo image (needs your own ROM data, never shipped):
   `python3 tools/make_demo.py build/demo_trace.txt roms/_rebuilt/proteus1_waves.bin roms/proteusxrpl build/proteus_demo.bin --from 10.3`
2. Copy `Proteus_<date>.rbf` to `_Other/` and the image to `games/Proteus/proteus_demo.bin`.
3. Load the core, OSD → **Load Demo**. Playback starts when the download ends and loops.
   **Restart Demo** starts it again from the first event.

`tools/mister_demo.sh RBF build/proteus_demo.bin` does all of this over SSH (`MISTER=root@<address>`)
and checks the output against the reference model.

The screen shows 32 bars (one per voice, height = envelope level) over a green strip (demo running)
or a red strip (no demo loaded).

## Design
| Block | File | Notes |
|---|---|---|
| Top | `Proteus.sv` | 80 MHz `clk_sys` (PLL), 2048 clocks per 39062.5 Hz output sample |
| Sound chip | `rtl/proteus_snd.sv` | 32 voices processed in turn; next voice's samples prefetched; matches `tools/proteus_model.py` sample for sample |
| Demo controller | `rtl/demo_ctrl.sv` | ioctl → DDR3 loader, event player (WAIT / write / pan / END), DDR3 arbiter, capture writer |
| Display | `rtl/level_display.sv` | 320×240 @ 60 Hz, 6.667 MHz pixel clock |

DDR3 map (physical): `0x30000000` sample words (8 MB, big-endian), `0x30800000` events,
`0x31000000` output capture ring (16 MB, left/right s16 LE per sample).

## Verification
- `sim/`: Verilator harness with a DDR3 model (random BUSY, 12–39 cycle read latency); loads the image
  through ioctl, compares outputs and the capture ring with `tools/proteus_model.py`.
- On hardware: `tools/mister_demo.sh` reads the capture ring back over SSH (`/dev/mem`) and compares it.

## Status (2026-11-20)
- Build 2 (`Build RBF` run 44805460547): 32 % ALMs, 37 DSP blocks, all clocks meet timing (80 MHz setup slack +0.82 ns).
- On a MiSTer (DE10-Nano, Linux 6.18.38-MiSTer): the DDR3 capture equals `tools/proteus_model.py` for the
  first 2,578,125 samples (66.0 s, including the demo's restart at 46.7 s). Listening test: pending.

## Build
`bash mister/build.sh` with Quartus Lite 17.0, or the `Build RBF` GitHub workflow (raetro/quartus:17.0).
