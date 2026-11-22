# Roland JV-880 for MiSTer: scope and plan

SESSION-PLAN iteration 12, 2026-11-22. The JV-880 (1992) is a 1U rack module: an H8/532 MCU
running the original firmware, the same PCM chip family as the SC-55, 4 MB of wave ROM and a
2×24 character LCD. [OBS] marks what was read in the reference or measured; [INF] marks
inferences.

## Reference and target

- **Reference:** Nuked-SC55 (jcmoyer's fork, `modules/nuked-sc55`) supports romsets
  `jv880-v1.0.0` and `jv880-v1.0.1` [OBS].
- **ROMs:** the user's dumps, in `nuked-sc55/firmware/Roland JV880/` (gitignored). All five
  files match the reference's SHA-256 table [OBS].
- **Factory-preset NVRAM:** `cores/sc55/ref/jv880_nvram.cpp` makes it headless. Its SHA-256 is
  d5da7845…d207, as the reference's `doc/building.rst` gives [OBS].
- **Target:** from that NVRAM, the reference renders `test/integration/jv880/jv880.mid` to
  SHA-256 73feda3c…bbbc, its integration test's hash [OBS]
  (`cores/sc55/tests/test_jv880_ref.py`).
- **Gate:** the HDL produces the same samples.

## How the reference's JV-880 differs from the SC-55 mk1

| Part | SC-55 mk1 | JV-880 | Reference |
|---|---|---|---|
| Step time (renderer) | 600 ns | 600 ns | `renderer/main.cpp` `R_NSPerStep` |
| Gate array base | E000 | F000 (PCM F000–F3FF, gate array F400–F7FF) | `mcu.cpp` `MCU_Read`/`MCU_Write` |
| LCD | custom, through the gate array | HD44780-style 2×24 at F404 (instruction) / F405 (data) | `mcu.cpp`, `lcd.cpp` |
| Gate-array interrupt | IRQ1 | IRQ0; the PCM's interrupt also goes through the gate array (line 5) | `mcu.cpp` `MCU_GA_SetGAInt`, `pcm.cpp` |
| Encoder | — | gate-array lines 3/4 (one pulse a detent) | `mcu.cpp` `MCU_EncoderTrigger` |
| Panel buttons | P1 matrix | P7DR, rows selected by `io_sd` (F401 writes): 14 buttons | `mcu.cpp` `DEV_P7DR` |
| A/D | battery, sliders | pin 1 battery, others 0x3FF | `mcu.cpp` `MCU_AnalogReadPin` |
| Memory pages | SRAM page 5 | SRAM page 10 (32 KB), NVRAM pages 12/13 (32 KB, battery-backed), card RAM pages 14/15 (32 KB); pages 8/9 read FF | `mcu.cpp` |
| ROM writes | — | writes to 6196–6199 ignored (the firmware writes into ROM at 002E77–002E7D) | `mcu.cpp` |
| Timers | mk1 step tables | generic tables | `mcu_timer.cpp` |
| Sub-MCU | none (mk1) | none | `mcu.cpp` |
| Wave ROMs | 3 × 1 MB | 2 × 2 MB (banks 0, 1); card bank 2 (2 MB); expansion banks 3–6 (8 MB) | `pcm.cpp` `PCM_ReadROM` |
| PCM filter | mk1 `multi`/`addclip20` | the non-mk1 path (plain products, as the mk2) | `pcm.cpp` |
| PCM clock | `reg_slots` cycles | × 25/29 | `pcm.cpp` `PCM_Update` |
| Output | 64 kHz | 64 kHz | `pcm.cpp` `PCM_GetOutputFrequency` |

The CPU core (`h8_cpu.sv`) already runs mk1 and mk2 firmware bit-exactly. The JV-880 changes
sit in the MCU peripherals (memory map, gate array, P7, timers), the PCM (bank mapping,
interrupt routing, clock ratio, filter path) and the board (LCD, panel, NVRAM) [INF].

## Layout

- **Its own core:** the JV-880 is a separate MiSTer core: `cores/jv880`,
  `_Devices/JV880_<date>.rbf`, with its own OSD and panel. It is not a fourth model inside the
  SC-55 core, because:
    - it needs neither the sub-MCU nor the SC-55's graphic LCD;
    - it has its own front panel;
    - one bitstream for all four models would carry logic no single model uses.
- **Shared Roland HDL:** the H8/532 CPU, the MCU peripherals, the PCM chip and `sdp_ram` move
  from `cores/sc55/hdl` to `modules/roland/hdl` (GPL-2.0-or-later, as the SC-55 core) and gain a
  `jv880` model input next to `mk2`/`sc155`. They stay out of synthframe, which is MIT.
- **Board pieces from synthframe:** the memory port, `midi_rx`, `audio_fifo`, the HD44780 model
  and renderer (iteration 8, panel as data).

## Block RAM [INF, to be checked by the first build]

| Memory | Size | M10K |
|---|---|---|
| ROM1 (internal) | 32 KB | 32 |
| ROM2 | 256 KB | 256 |
| SRAM | 32 KB | 32 |
| NVRAM | 32 KB | 32 |
| PCM work RAM, CPU RAM, FIFOs | as in the SC-55 | ~40 |
| Total | | ~390 of 553 |

The SC-55 mk1 build uses 453, including its graphic LCD background. Card RAM (32 KB) is left
out at first: no PCM card is emulated.

## Milestones

| # | Milestone | Gate |
|---|---|---|
| J1 | Shared Roland HDL in `modules/roland` | SC-55 tests identical (lockstep, board bench, system hashes); SC-55 Windows build within ±1%; MiSTer smoke unchanged |
| J2 | `jv880` model in the MCU | `mcu_lockstep --romset jv880-v1.0.1` identical to the reference through boot, "Internal RAM read error", the factory preset (button and encoder script of `jv880_nvram.cpp`) and 20 M steps of `jv880.mid` |
| J3 | `jv880` model in the PCM | `pcm_lockstep` jv880 identical; the system bench renders `jv880.mid` from the factory NVRAM to 73feda3c…bbbc |
| J4 | MiSTer board `cores/jv880` | board bench identical from power-on (ROM pack over ioctl, wave ROMs in DDR3, NVRAM from the save file); Windows build meets timing |
| J5 | On the MiSTer | boots to "Perf I-01:Syn Lead" with the factory NVRAM; plays `jv880.mid` over MidiLink with no underrun; panel keys and encoder work; NVRAM survives a reload |

## Open questions

- **Expansion boards (SR-JV80) and PCM cards:** later. The reference supports them; the
  user's dumps don't include them.
- **Real-time margin:** the SC-55 mk1 averages 19.3 clocks a step against 25 available. The
  JV-880's PCM runs 25/29 as many cycles a step, so it should fit [INF].
- **Card RAM (pages 14/15):** does the firmware need it to boot without a card? [UNK] The
  reference keeps 32 KB; check reads in the J2 trace.
