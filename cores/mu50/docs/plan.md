# Yamaha MU50 for MiSTer: scope and plan

SESSION-PLAN iteration 22, 2026-11-27. The MU50 (1995) is a half-rack General MIDI / GS / XG tone
module with 16 parts and 32-note polyphony. Its parts are a Hitachi H8/3003 running a 512 KB program
ROM, a Yamaha SWP00 sound chip (AWM2 sample player and MEG effects DSP) reading a 4 MB wave ROM,
64 KB of battery-backed RAM, and a 4×20 character LCD with LEDs. It is brief §8's Phase 1 core after
the MT-32. [OBS] marks what was read in the reference or measured; [INF] marks inferences; [UNK]
what is still open.

## Reference

- **MAME** `src/mame/yamaha/ymmu50.cpp` (R. Belmont, Olivier Galibert),
  `src/devices/cpu/h8/` (Olivier Galibert), `src/devices/sound/swp00.cpp` (Olivier Galibert) and
  `src/mame/yamaha/mulcd.cpp`. All are BSD-3-Clause [OBS]. MAME marks `mu50` working (flags 0)
  [OBS, `CONS( 1995, mu50, ...)`].
- **MAME is the definition**, as for the CZ-101 and the P2K: the SWP00 is a reimplementation.
  MAME's comments say the MEG programs are internal, not dumpable, and were reimplemented
  [OBS, swp00.cpp header]. The HDL matches MAME's integer behaviour; MAME's error against a real
  MU50 is out of scope.
- **Integer model** [OBS]: `swp00.cpp` (2,398 lines) uses `double` only in two debug helpers
  (`sx7`, `sx9`); the sound path is integer. A bit-exact output gate is possible.
- **MAME build:** a `../mame-mu50` checkout of upstream master (2026-11-27 or later) with this
  core's logging patches (`cores/mu50/mame/patches/`), building the `mu50` subtarget. The other
  MAME checkouts are not touched.
- **No usable H8/300H HDL** (checked 2026-11-27) [OBS]: the only open core found
  (sugawara-systems.com, 2004) has no licence, implements an instruction subset, is deliberately
  not cycle-compatible and has no on-chip peripherals. The H8/532 in `modules/roland` is the
  H8/500 family, a different instruction set.

## ROMs

MAME's set `mu50` [OBS, `ROM_START( mu50 )`]:

| File | Size | Use |
|---|---|---|
| `xr174c0.ic7` (v1.05, 1995-08-21), `yamaha_mu50.bin` (v1.04) or `xq332e0.bin` (v1.02) | 512 KB | program ROM, 000000–07FFFF, 16-bit, word-swapped |
| `xq057c0.ic18`, `xq058c0.ic19` | 2 MB each | SWP00 wave ROM, 4 MB |
| MULCD `hd44780u_b04.bin` | 4 KB | LCD character ROM (part of MAME's `mulcd` device) |

- **The user's set** (2026-11-27): `synth-projects/nuked-sc55/firmware/mu50-mame/` (gitignored),
  a MAME rompath with `mu50/` and `mulcd/`. All seven files match MAME's SHA-1s: v1.05, v1.04
  (from the separate `YAMAHA MU-50 (ver.1.04).BIN`; the copy in the uploaded `mu50.zip` was
  truncated), v1.02, both wave ROMs, and the mulcd character ROM and artwork. Tests find the set
  through `MU50_ROM_DIR`.
- The wave ROM is identical to the DB50XG's and the MU10's [OBS, ROM hashes], so the same core
  can later run their program ROMs (see "Variants").

## Hardware, from the MAME driver [OBS]

| Part | Detail |
|---|---|
| CPU | H8/3003 at 10 MHz, one state per clock; mode pins give a 24-bit address space |
| Memory map | 000000–07FFFF program ROM; 200000–20FFFF 64 KB work RAM (the NVRAM); 400000–4007FF SWP00 |
| Port 6 | bit 0 LCD E (inverted), bit 1 LCD R/W, bit 2 LCD RS |
| Port 9 | bits 4–5 LCD contrast |
| Port A | LCD data bus |
| Port B | bit 1 contrast, bits 2–6 the five LEDs (active low) |
| Port C | bits 0, 1, 7 select the three key rows (active low); the rows read back on bits 2–6 |
| A/D | AN0 / AN2 analog input level, AN4 host switch (000 = MIDI), AN6 battery (200) |
| Serial | SCI1 is MIDI in and MIDI out; the host-computer port (TO HOST) is not emulated |
| Interrupts | none from the SWP00; the firmware's timing comes from the H8's own timers [INF] |
| Sound | SWP00 at 33.8688 MHz, 768 clocks a sample pair = 44,100 Hz stereo, 32 voices |
| Effects | MEG: 192-step fixed program, delay lines of 128 K (reverb), 32 K (chorus) and 128 K (variation) words |
| LCD | HD44780 with ROM B04, 4×20 characters, plus the five LEDs and the contrast |

## How MAME runs the CPU [OBS, `h8.cpp`, `h8.lst`]

- The instruction handlers are generated from `h8.lst` by `h8make.py` (2,932 lines of
  description). Each handler is a sequence of bus accesses and internal steps.
- Every bus access costs 2 states on the H8/300H, with no wait states and no 8-bit/16-bit area
  difference (`read16i`, `read8`, `write16` …: `m_icount -= 2`). Internal operations cost
  `cycles + 1` (`internal()`).
- An instruction can stop between accesses (`m_inst_substate`, `do_exec_partial`), and the
  peripherals are brought up to date at the access where their next event falls (`m_bcount`,
  `internal_update`). So peripheral events land at bus-access granularity, not at instruction
  boundaries.
- Interrupts, DMA and DTC requests are taken at `prefetch_done`.

The HDL follows the same model: it performs the instruction's bus accesses in MAME's order, each
taking 2 states of a 10 MHz clock enable, and the internal steps in MAME's state counts. The
access sequences and counts are generated from `h8.lst`, so timing matches by construction, as the
CZ-101's tables were generated from MAME's.

## Sound timing [INF, to be confirmed by U0]

- The SWP00 stream runs at 44,100 Hz. One sample is 226.757… CPU states, so CPU time and sample
  time are compared in attoseconds, as in the CZ-101 plan.
- Each register write calls `m_stream->update()` first, so a write at time T is applied before
  sample n if T falls before that sample's time. U0 logs every write with its state count and
  measures the exact rule.

## Layout

- `cores/mu50/`: `hdl/` (board), `mister/`, `sim/`, `tests/`, `tools/`, `mame/patches/`, `ref/`.
- New CPU and sound HDL:
    - `cores/mu50/hdl/h8h/` (H8/300H CPU and the H8/3003 peripherals: interrupt controller,
      16-bit timers, SCI, A/D, ports, DMA, watchdog, refresh controller);
    - `cores/mu50/hdl/swp00/` (AWM2 and MEG).
  The DB50XG, DB60XG, MU10 and QY70 also use an H8/3002 or H8/3003, so `h8h/` moves to
  `modules/` once a second core needs it.
- From synthframe: `midi_rx`, `midi_tx`, `audio_fifo`, `nvsave` (the 64 KB RAM to the SD card),
  `mem_ddr3` / `mem_arb` (wave ROM, program ROM and the effects delay lines),
  `synthframe_hd44780` with the B04 character ROM, `synthframe_charlcd` and `synthframe_vtiming`
  for the screen.

## Size [INF, to be checked by the first build]

| Part | Estimate |
|---|---|
| External memory | 4 MB wave ROM, 512 KB program ROM, 288 K words of effects delay lines. The delay lines (about 1.1 MB at 32 bits a word in MAME) do not fit in block RAM |
| Block RAM | 64 KB work RAM (about 52 M10K), on-chip RAM 512 B, SWP00 voice state (32 voices × 5 blocks), MEG constants and offsets, a program-ROM cache, the LCD |
| Memory bandwidth | per sample: about 64 MEG delay-line accesses and 2 wave-ROM bytes per voice for 32 voices, at 44.1 kHz; about 5.6 M accesses a second, plus CPU instruction fetches that miss the cache |
| Logic | H8/300H core plus peripherals and the SWP00; to be measured. The CZ-101's uPD7811 + uPD933 core fitted in 14,577 ALMs |

## Milestones

| # | Milestone | Gate |
|---|---|---|
| U0 | Reference | Patched MAME logs, per instruction, PC, opcode, registers, CCR and state count; every bus access with its state; every SWP00 register write with its state count; the raw 44.1 kHz stereo samples. A test-ROM mode runs a generated program on the same machine. With the ROMs: a MIDI corpus rendered to hashes, the same on two runs |
| U1 | H8/300H instruction set | Generated test ROMs covering every opcode in `h8.lst` for the H8/300H (all addressing modes, byte/word/long, MULXS/DIVXS, EEPMOV, TRAPA, SLEEP) give a trace identical to MAME's: registers, CCR, memory writes, state counts |
| U2 | H8/3003 peripherals | Interrupt controller, 16-bit timers, SCI (MIDI in and out), A/D, ports, DMA, watchdog, refresh. Random programs exercising them identical to MAME; then the firmware from power-on identical through boot and the first screen |
| U3 | SWP00 | Fed MAME's register-write log (with times) and the wave ROM, the HDL gives the same samples: AWM2 (streaming, envelopes, filter, LFO, mixer) first, then the MEG. Synthetic write streams first, so U3 does not wait for the ROMs |
| U4 | System bench | CPU + SWP00 + LCD + keys + MIDI from power-on: every corpus hash identical to MAME's; LCD text and LEDs identical |
| U5 | MiSTer | Windows build meets timing; on the MiSTer: boots to the play screen, plays MIDI from MidiLink with no underrun, panel keys work, the RAM survives a reload, MIDI out matches MAME's |

U1 and U3 run in parallel: they share nothing but the bench.

## Status (2026-11-27)

| # | Result |
|---|---|
| U0 | Done: `../mame-mu50` (upstream `db140b2d`) with `mame/patches/0001-mu50-logging.patch`; `tools/mame-mu50.sh`; test-ROM mode (`tools/mu50_testrom.py`); `tests/u0_check.sh` passes. Corpus: `tests/u0_corpus.sh` renders `tests/midi/basic.txt` (18 s) identically twice and matches `tests/midi/expected.txt`. More corpus cases come with U4 |

U0 findings [OBS]:
- **Program ROM file order:** MAME's `ROM_LOAD16_WORD_SWAP` wants each byte pair of the CPU
  image swapped in the file. A reset vector of 00000100 written unswapped was read as 00000001.
- **Reset:** the CPU reads the vector as two words (000000, 000002) and runs from its low 24
  bits; the first instruction starts at state 6.
- **State counts** (t0): MOV.W #imm,Rd 4; MOV.L #imm,ERd 6; MOV.W Rs,@ERd 4 (the write lands at
  the end, state 20 for an instruction starting at 16); BRA 4. Each is 2 states per bus access.
- **SWP00 write timing:** a CPU write at state s (counted after the access) is applied after
  floor(s × 44,100 / 10⁷) + 1 samples: state 18 after 1 sample, state 393,236 (39.32 ms) after
  1,735. One second makes 44,101 samples. The exact rule at sample boundaries (MAME's
  attosecond periods) is pinned in U3.
- **Determinism:** two runs give identical traces, write logs and samples.
- **What the firmware uses** (v1.05, 18 s: boot plus `basic.txt`): interrupts IRQ0 (vector 12,
  33 times; MAME holds the pin low from reset), ITU1 IMIA (28, 15,691: the main tick), ITU4
  IMIA/IMIB/OVI (40-42: 420, 696, 201), SCI1 RXI (57, 92 at level 1: MIDI in). No DMA and no
  A/D interrupt; the A/D is started by the CPU and polled. SCI0 and SCI1 are set up; the
  watchdog and refresh registers are written only at init. Every init register is written twice
  [UNK why]. So U2 needs the interrupt controller, the five 16-bit timer channels, both SCIs,
  the A/D and the ports; DMA can wait.
- **Sound:** silent until MIDI arrives; the basic case makes 39,507 SWP00 writes in 18 s.

## Variants (after U5)

| Variant | Difference from the MU50 [OBS, MAME] | Core change |
|---|---|---|
| DB50XG (`xr253a0`) | H8/3002 at 12 MHz, no panel, MIDI only | an H8/3002 mode and a firmware option |
| DB60XG / NEC XR385 (`xr560b0`) | as the DB50XG, plus the analog input | the same |
| MU10 | H8/3002 at 12 MHz, no screen, a gate array that sends the analog input to the effects | the gate array, if wanted |

The SWP00 and the wave ROM are the same in all four.

## Open questions

- **Licence (default, following the CZ-101):** MIT, as the rest of the repository. The core is
  written from MAME's BSD-3-Clause sources, so `NOTICE` keeps MAME's notice and names the
  authors; `ref/` and `mame/patches/` stay BSD-3-Clause.
- **Effects delay-line width:** MAME stores them as `s32`. The real width decides the external
  memory layout; U3 reads the MEG code for the bits it keeps [UNK].
- **DMA:** not used by v1.05 in boot or MIDI playback (U0 trace) [OBS]. U2 leaves it for
  later; the bench reports any DMA register write.
- **Program ROM speed:** MAME fetches every instruction in 2 states with no wait. The core needs
  the program ROM in block RAM or behind a cache that never stalls the CPU's virtual time. The CPU
  runs ahead of real time and waits for it, as the CZ-101 does, so a cache miss costs FPGA clocks
  only, not virtual time [INF].
