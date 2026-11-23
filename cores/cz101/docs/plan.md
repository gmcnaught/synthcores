# Casio CZ-101 for MiSTer: scope and plan

SESSION-PLAN iteration 13, 2026-11-23. The CZ-101 (1984) is an 8-voice phase-distortion
synthesizer: a NEC uPD7811 running a 32 KB program ROM, one NEC/Casio uPD933 sound chip, 4 KB of
battery-backed RAM, an optional RA-3 RAM cartridge and a 2×16 HD44780 LCD. It is brief §8's second
Phase 1 core and the first core in this repo with a CPU that has no existing HDL. [OBS] marks what
was read in the reference or measured; [INF] marks inferences; [UNK] what is still open.

## Reference

- **MAME** `src/mame/casio/cz101.cpp` (Dirk Best, Devin Acker), `cpu/upd7810` (Juergen
  Buchmueller), `sound/upd933.cpp` (Devin Acker). All three are BSD-3-Clause [OBS]. MAME marks
  the driver working and save-state safe [OBS].
- **MAME is the definition**, as for the P2K (`p2k-core-decisions`): the uPD933 model is
  behavioural, not checked against a chip at bit level [OBS: no die or datasheet reference in the
  source]. The HDL matches MAME's integer behaviour; MAME's error against a real CZ-101 is out of
  scope.
- **MAME builds:**
    - stock MAME 0.289 (Homebrew) has the `cz101` set [OBS];
    - `../mame-cz101`, a clone at the P2K checkout's commit 4a92d534, builds the `cz101` subtarget
      with this core's logging patches (`cores/cz101/mame/patches/`). The P2K checkout is not
      touched.
- **No existing HDL** for the uPD7810 family or the uPD933 (searched 2026-11-23) [OBS].

## ROMs

MAME's set `cz101` [OBS, `ROM_START( cz101 )`]:

| File | Size | Use |
|---|---|---|
| `hn613256ps40.bin` (Version II) or `hn613256pn26.bin` (Version I) | 32 KB | program ROM, 0000–7FFF |
| `upd7811g-120.bin` | 4 KB | uPD7811 internal ROM; disabled by the mode pins on the CZ-101 [OBS, comment] |
| `init_ram.bin` | 4 KB | initial contents of the battery-backed RAM (8000–8FFF) |

- **The user's set** (2026-11-23): `synth-projects/nuked-sc55/firmware/cz101-mame/` (gitignored),
  all four files matching MAME's hashes; dbwbp.com's `Casio CZ-101 EPROM Firmware.zip` is the same
  Version II program ROM (SHA-1 2aa5bfb7…). Tests find the set through `CZ101_ROM_DIR`.
- **Internal ROM** [OBS]: `upd7811g-120.bin` is byte-identical to the first 4 KB of
  `hn613256ps40.bin` (reset: JMP 00C0; MM = 0F; JMP 4000; vectors and the CALT table point into
  4000+). MAME's mapping of the internal ROM over 0000-0FFF therefore changes nothing; the core reads
  0000-0FFF from the program ROM and needs no internal-ROM file.
- **Boot** [OBS]: MAME with `init_ram.bin` and Power switched on shows "*PRESET* NO.01 / BRASS ENS.1".
  MAME draws the LCD only after the Power input changes (`m_power`); the firmware runs either way.

## Hardware, from the MAME driver [OBS]

| Part | Detail |
|---|---|
| CPU | uPD7811 at 10 MHz; one machine cycle = 3 clocks (`execute_clocks_to_cycles`) |
| Memory map | 0000–7FFF program ROM; 8000–8FFF RAM (NVRAM); 9000–97FF RA-3 cartridge; 9800–B7FF four LED latches (write); B800–BFFF key matrix (read, row = PB0–3); C000–FEFF uPD933; FF00–FFFF internal RAM |
| Port A | HD44780 data bus |
| Port B | 0–3 key row; 4 uPD933 IRQ (in); 5 uPD933 /CS; 6 write enable; 7 power switch (in, also /NMI) |
| Port C | 0 MIDI TX; 1 MIDI RX; 2 MIDI clock; 3 power-down (low = reset everything); 5 LCD RS; 6 LCD RW; 7 LCD E (inverted) |
| Serial | MIDI on the uPD7811 SIO, SCK from a 2 MHz clock |
| A/D | AN1 pitch wheel (centre 7F), AN2 battery level (FF = normal) |
| Interrupts | uPD933 IRQ → INTF1 |
| Sound | uPD933 at 4.48 MHz, 112 clocks a sample = 40 kHz, 8 voices, one output |
| LCD | HD44780, 2×16, 5×8 font |

One sample is exactly 250 periods of the 10 MHz crystal, and one CPU cycle is 3. A shared 10 MHz
tick count therefore places every CPU access in its exact sample, as MAME's scheduler does
[INF].

## How MAME runs the CPU [OBS, `upd7810.cpp` `execute_run`]

- One instruction at a time. The opcode's cycle count (or its skip count, when the SK flag is set)
  comes from the tables in `upd7810_table.cpp`.
- The timers and serial port advance by that count **before** the instruction executes.
- Interrupts are taken between instructions (`upd7810_take_irq`).
- 1,003 opcode handlers, over the main table and the prefixes 48, 4C, 4D, 60, 64, 70, 74.

The HDL follows the same model: it executes an instruction in as many FPGA clocks as it needs,
then waits out the instruction's cycle count on a 10 MHz/3 clock enable. The decoder's cycle,
skip-cycle and length columns are generated from MAME's tables, so timing matches by
construction.

## Layout

- `cores/cz101/`: `hdl/` (board), `mister/`, `sim/`, `tests/`, `tools/`, `mame/patches/`.
- New CPU and sound HDL:
    - `cores/cz101/hdl/upd7810/` (CPU and peripherals);
    - `cores/cz101/hdl/upd933/`.
  If a later core needs the uPD7810 (Juno-106, TR-909 per brief §2), it moves to `modules/`.
- From synthframe: `midi_rx` is not needed (MIDI enters the CPU's own serial port as bits);
  `audio_fifo`, `nvsave` (RAM and cartridge to the SD card).
- HD44780: `hdl/cz101_lcd.sv`, transcribed from MAME's hd44780 with the busy flag in CPU
  cycles.

## Size [INF, to be checked by the first build]

| Part | Estimate |
|---|---|
| Block RAM | program ROM 32 KB, RAM 4 KB, cartridge 2 KB, internal RAM 256 B, uPD933 tables (cosine 2048×12, pitch 128×32, fine pitch 512×16, volume 512×16) and voice state: under 60 M10K |
| Logic | brief §3: 9–11k ALMs for the whole core, plus the MiSTer framework |
| DSP | a few: the uPD933 volume and ring-modulation products, time-multiplexed over 8 voices |

## Milestones

| # | Milestone | Gate |
|---|---|---|
| C0 | Reference | Patched MAME logs, per instruction, PC, opcode, registers, PSW and cycle count; every uPD933 write with its 10 MHz tick; the raw 40 kHz samples. A test-ROM mode runs a generated 32 KB program on the same machine. With the ROMs: a MIDI corpus rendered to hashes, the same on two runs. |
| C1 | uPD7811 instruction set | Generated test ROMs covering every opcode in the uPD7810 tables (including skips, SK, interrupts disabled) give a trace identical to MAME's: registers, flags, memory writes, cycle counts |
| C2 | uPD7811 peripherals | Timers, event counter, serial port (MIDI), A/D, ports, interrupt controller, internal RAM. Firmware from power-on: trace identical to MAME's through boot, the first patch display and every corpus case |
| C3 | uPD933 | Fed MAME's write log, the HDL gives the same samples and the same IRQ times; tables generated with MAME's own `device_start` code and compared |
| C4 | System bench | CPU + uPD933 + LCD + keys + MIDI from power-on: every corpus hash identical; LCD text identical to MAME's |
| C5 | MiSTer | Windows build meets timing; on the MiSTer: boots to the first patch, plays MIDI from MidiLink with no underrun, panel keys work, RAM and cartridge survive a reload |

## Status (2026-11-23)

| # | Result |
|---|---|
| C0 | Patched MAME builds (`../mame-cz101`); test-ROM harness works. Waiting for the ROMs for the corpus. |
| C1 | Done: 120 random programs, 3 M instructions identical to MAME (`docs/upd7810.md`) |
| C2 | Peripherals done for the random tests (160 programs, 310 k interrupts identical). Firmware boot lockstep waits for the ROMs. |
| C3 | In progress (uPD933) |

## Open questions

- **Licence.** Every file so far is written from MAME's BSD-3-Clause sources; the LCD is
  `hdl/cz101_lcd.sv`, transcribed from MAME's hd44780, not the Proteus core's GPL-3.0
  `hd44780.sv`. Default: BSD-3-Clause, as the reference. The author decides before release.
- **ROM version** in the dbwbp archive [UNK].
- **SIO timing:** MAME clocks the serial port from a 2 MHz clock device (4 M callbacks/s). The
  HDL needs the same bit timing to keep the MIDI corpus in lockstep [UNK how exact this must be;
  C2 decides].
- **Keyboard:** the CZ-101's 49 mini keys are in the key matrix. The MiSTer core plays from MIDI;
  mapping a PC keyboard to them is optional.
