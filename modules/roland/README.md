# modules/roland

HDL for the Roland parts that several cores share. Each is checked against
[Nuked-SC55](https://github.com/jcmoyer/Nuked-SC55) (`modules/nuked-sc55`) by the SC-55 core's
lockstep tests.

| File | Part | Used by |
|---|---|---|
| `hdl/cpu/h8_cpu.sv` | Hitachi H8/532 CPU (H8/500 instruction set) | SC-55, JV-880 |
| `hdl/mcu/sc55_mcu.sv` | the H8/532's on-chip peripherals and the board's memory map (gate array, timers, SCI, A/D) | SC-55, JV-880 |
| `hdl/pcm/sc55_pcm.sv` and `*.svh` | the Roland PCM chip (voices, interpolation, filter, reverb and chorus) | SC-55, JV-880 |
| `hdl/submcu/sc55_submcu.sv` | the SC-55mk2's sub-MCU (M37450) | SC-55 (mk2) |
| `hdl/core/sc55_core.sv` | the MCU system and the PCM chip together (models mk1, mk2, SC-155, JV-880) | SC-55, JV-880 |
| `hdl/board/*` | board services, assembled in `roland_board_common` (pack load, DDR3, reset sequence, pacing, MIDI, status flags, audio chain) for both boards: ROM pack loader (`rom_loader`, pack format in `cores/sc55/tools/romset.py`), wave ROMs in DDR3 (`wave_ddr`, `rom2x_cache`), real-time pacing (`step_pacer`), 64/66 kHz to 48 kHz (`resample`), the LCD controller (`lcd_ctrl`) and font (`lcd_font.svh`) | SC-55, JV-880 |
| `mister/sta_paths.tcl` | the worst-path reports after a Quartus build; each core's `mister/sta_paths.tcl` sets its project and board and sources it | SC-55, JV-880 |
| `hdl/common/sdp_ram.sv` | simple dual-port block RAM | all of the above |

- **Where it came from:** moved from `cores/sc55/hdl` on 2026-11-22 (JV-880 plan, milestone J1;
  `cores/jv880/docs/plan.md`).
- **Docs:** the SC-55's `docs/cpu.md`, `docs/mcu.md` and `docs/pcm.md` describe the modules.
- **Model inputs:** models are chosen by inputs (`mk2`, `sc155`, `jv880`), not by
  separate copies.

Licence: GPL-2.0-or-later (`LICENSE`), as the SC-55 core and Nuked-SC55. These modules are not
part of synthframe, which is MIT.
