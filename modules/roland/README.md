# modules/roland

HDL for the Roland parts that several cores share. Each is checked against
[Nuked-SC55](https://github.com/jcmoyer/Nuked-SC55) (`modules/nuked-sc55`) by the SC-55 core's
lockstep tests.

| File | Part | Used by |
|---|---|---|
| `hdl/cpu/h8_cpu.sv` | Hitachi H8/532 CPU (H8/500 instruction set) | SC-55, JV-880 (planned) |
| `hdl/mcu/sc55_mcu.sv` | the H8/532's on-chip peripherals and the board's memory map (gate array, timers, SCI, A/D) | SC-55, JV-880 (planned) |
| `hdl/pcm/sc55_pcm.sv` and `*.svh` | the Roland PCM chip (voices, interpolation, filter, reverb and chorus) | SC-55, JV-880 (planned) |
| `hdl/common/sdp_ram.sv` | simple dual-port block RAM | all of the above |

- **Where it came from:** moved from `cores/sc55/hdl` on 2026-11-22 (JV-880 plan, milestone J1;
  `cores/jv880/docs/plan.md`).
- **Docs:** the SC-55's `docs/cpu.md`, `docs/mcu.md` and `docs/pcm.md` describe the modules.
- **Model inputs:** models are chosen by inputs (`mk2`, `sc155`; `jv880` to come), not by
  separate copies.

Licence: GPL-2.0-or-later (`LICENSE`), as the SC-55 core and Nuked-SC55. These modules are not
part of synthframe, which is MIT.
