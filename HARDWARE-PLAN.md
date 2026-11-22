# Using the MiSTer and the M64 for the Vintage Synth FPGA Module

As of 2026-11-22. Design brief: https://claude.ai/artifact/C3PuPP2mcDQTQASFHiE1Um

## Summary

- **MiSTer (DE10-Nano):** the main development platform and the reference platform. It runs all of
  Phase 1 and keeps the computer-class cores (Synclavier) permanently.
- **ModRetro M64 (XCAU15P + 4 PSRAM):** use it as a test board loaded over JTAG, not as a synth.
  The M64 has no MIDI port and no analog audio. Its only audio path is HDMI
  ([CNX Software](https://www.cnx-software.com/2026/06/03/modretro-m64-an-amd-artix-ultrascale-fpga-based-open-source-nintendo-64-console-with-original-cartridge-support/)).
  It answers three questions before the own board is designed:
    - How fast the PSRAM really is.
    - How many LUTs, BRAMs and DSPs the cores really use on the AMD part.
    - Whether a core can run from PSRAM and still match its reference bit for bit.
- **Three steps need no M64 at all:** the memory-port refactor, the PSRAM controller with its chip
  timing model in simulation, and a Vivado synthesis-only build of the SC-55 for the XCAU15P.
  These come first.

## What each device is used for

| Device | Role | Runs | Needed before use |
|---|---|---|---|
| MiSTer | Development and reference | SC-55, Proteus/1 XR, Synclavier, then every Phase 1 core (JV-880, CZ-101, MT-32, MU50, DMX, SD-1, S1000) | Already working: Quartus 17.0 "Build RBF" workflows and MidiLink. Optional: a DIN MIDI adapter on the user port for timing tests (brief §8) |
| M64 | Test board loaded over JTAG | PSRAM tests, SC-55 port, SC-88Pro (the first core too large for the DE10-Nano) | An x86-64 Linux machine with Vivado; a Xilinx-compatible JTAG cable plus a TC2050 cable; a UART adapter; the M64 schematic stored locally |
| Linux x86-64 machine (VM or cloud) | Vivado builds | Synthesis and implementation for the XCAU15P | Vivado ML Standard. The free tier lists the XCAU15P ([UG973](https://docs.amd.com/r/en-US/ug973-vivado-release-notes-install-license/Supported-Devices)). Licensing tiers changed in 2026.1, so check the terms ([AMD licensing](https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vivado/vivado-licensing-options.html)) |

This Mac is arm64. Vivado has no macOS version, and running Vivado under x86 emulation in Docker
is not a reliable build host. Quartus builds keep running in the existing GitHub Actions workflows.

## Current state of the three cores

| Repo | Remote | State | How it reads DDR3 |
|---|---|---|---|
| `cores/sc55` (was `nuked-sc55`) | gmcnaught/sc55-MiSTer | clean, release `SC55_20261120.rbf` | `hdl/board/wave_ddr.sv` and `rom2x_cache.sv`: already a separate module |
| `cores/synclavier` (was `synclavier-ii-MiSTer`) | gmcnaught/synclavierii | clean | `hdl/board/ddram_word.sv`: separate module |
| `cores/proteus` (was `emu-proteus-2k-MiSTer`) | gmcnaught/emu-proteus-xr-MiSTer | P2K CPU work committed as `1f2cfdb` on `p2k-research` and imported | `mister/rtl/snd_ctrl.sv` drives the Avalon `DDRAM_*` signals directly and arbitrates 4 clients itself. This is the least portable of the three. |

- **Vendor-specific code outside `mister/sys`:** only `pll.v` in each repo, plus the RAM hint in
  `cores/synclavier/hdl/dev/pcm_reverb.sv`.
- **Duplicated code:** `audio_fifo.sv` exists in two repos. Some SC-55 board modules were copied
  from the Synclavier.

## Order of work

### Phase 1: MiSTer only, can start now

1. **Shared library.** Create `modules/vsframe` in the monorepo described in `LAYOUT-PLAN.md`
   (the jtcores and JTFRAME layout). It holds four contracts:
    - the memory port (tagged requests, in-order data, any delay);
    - the panel as data (HD44780 character RAM or a framebuffer of at most 256×64, plus LED and
      key events);
    - the block loader;
    - shared modules: `midi_rx`, `audio_fifo`.
2. **Memory port with a DDR3 backend.** Port the SC-55 first. It is the smallest change, and its
   lockstep tests already run at ROM latencies of 3 and 7. Then the Synclavier (`ddram_word`).
   Then the Proteus/1 (`snd_ctrl` splits into 4 clients and an arbiter).
   - Gate: every existing test stays bit-identical, with random latency in the benches.
3. **PSRAM backend in simulation.** Write a Verilator timing model of the APS256XXN and
   APS512XXN: chip-select time limit, page-boundary penalty, fixed latency. Then the controller,
   then the SC-55 and Synclavier benches through it.
   - Gate: the SC-55 `0A.mid` hash is unchanged. The model reports the read rate it sustains.
4. **Vendor-neutral build.** Add a PLL wrapper per platform and an inference-only path for the
   `pcm_reverb` RAM.
5. **Vivado synthesis-only build of the SC-55 for `xcau15p-ffvb676-2`.** This needs the Linux
   machine, not the M64.
   - Gate: measured LUT, BRAM and DSP counts replace the ±30% conversions in brief §3.
6. **More cores**, following brief §8: JV-880 first, which reuses the H8/532. The P2K CPU work
   (TG68K with ColdFire changes) continues in `emu-proteus-2k-MiSTer`.

### Phase 2: M64 over JTAG

1. **Before the first load**, use the schematic (brief: page 20 has `JTAG_UART_HEADER`, a TC2050):
    - Find what the STM32 does with PROGRAM_B, INIT_B and DONE.
    - Confirm a JTAG load does not touch the flash, and that a power cycle restores the N64 image.
2. **JTAG chain detect.** The IDCODE confirms the exact part. Then a bitstream with a UART "hello"
   and a blinking LED.
3. **PSRAM test bitstream.** Measure random 16-byte reads per second and latency on each of the
   4 buses.
   - Gate: a measured number replaces the brief's estimate of 8–12 M reads/s per chip.
4. **SC-55 from PSRAM.** Load the ROM pack and MIDI over UART; the output hash goes back over UART.
   - Gate: the hash equals the simulator's.
5. **Audio decision.** Listening on the M64 needs either HDMI with audio or I2S on spare pins.
   Which one depends on how the board drives HDMI, which is not checked yet. If neither is
   practical, check sound by hash on the M64 and by ear only on the own board.
6. **SC-88Pro:** the first core developed for the AMD part only.

### Phase 3: own board

Brief §5 is unchanged, but it starts only after the Phase 2 PSRAM number and the SC-55 port are
proven.

## Corrections to the brief

- **§2 and §9 say the Proteus 2000 CPU is "not identified".** The repo has found it:
    - It is a ColdFire **MCF5206e** at 33 MHz (`cores/proteus/docs/p2k-findings.md`,
      `docs/p2k-core-cpu.md`).
    - The CPU is being built on TG68K.C in 68020 mode with ColdFire changes. TG68K is LGPL-3,
      which adds a licence to the core.
    - The P2K testbench includes an HD44780 model (`.remember/now.md`, 2026-11-21). This suggests
      a character LCD, which is not confirmed.

- **DX7 (target #5) is dropped.** Another author is already building the DX7 as a device
  (user, 2026-11-22). Its slot in the target set was there to cover 6-operator FM and the HD6303
  CPU. Once that core exists, use it as the FM reference instead of building one here.

## Unknown

- **MiSTer add-ons.** Is there a user-port MIDI adapter, an I/O board or an SDRAM board? Is the
  MiSTer reachable over SSH? No SSH host is configured in `~/.ssh/config`.
- **M64 schematic.** The ModRetro support page does not render for automated fetches, so the
  schematic facts in the brief (PSRAM parts, header pinout) could not be checked again. Save the
  PDF to `hw/m64/`.
- **M64 HDMI path.** Is it driven straight from the FPGA or through a transmitter chip? This
  decides step 5 of Phase 2.
- **openFPGALoader.** Support for the XCAU15P is not confirmed. The Vivado Hardware Manager is the
  known-good loader.
- **JTAG cable.** Which cable you have, if any.
