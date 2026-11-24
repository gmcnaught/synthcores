# Proteus 2000 MiSTer core: plan (iteration 18)

As of 2026-11-24. The parts exist and have been checked separately:

- **CPU:** TG68K with the ColdFire changes and the MCF5206e peripherals (`cores/proteus/p2k/rtl/cpu`).
  It matches MAME from reset to the OS 2.26 main screen (iteration 11).
- **Sound engine:** `cores/proteus/p2k/rtl/snd`. It is bit-exact against `p2k_render` and meets
  100 MHz on its own (iteration 16).

This iteration adds the board around them and the MiSTer top. The board's definition is MAME's
`proteus2k` machine (`emup2k.cpp`, patches 0002-0004): the memory map, the chip-select glue and
the sound-chip read paths. Tags: [O] from MAME or the firmware, [I] a recorded choice.

## Decisions

- **Core folder `cores/p2k`** (LAYOUT-PLAN: "split out of proteus/p2k once the CPU boots"). The
  CPU and sound RTL stay where they are for now; `cfg/files.yaml` points at them.
- **All memory is in DDR3** (from 0x30000000, free for cores), so no SDRAM board is needed. Layout,
  in 64-bit words of `DDRAM_ADDR`:

  | Region | Base | Size |
  |---|---|---|
  | Sample memory (4 SIMM slots, `SMP_BASE`) | 0x6000000 | 128 MB |
  | Effect delay lines (`FX_BASE`) | 0x7000000 | 1.8 MB |
  | Baby flash/ROM, 4 slots of 2 MB | 0x7100000 | 8 MB |
  | Boot/OS/user flash (28F160) | 0x7200000 | 2 MB |
  | DRAM | 0x7240000 | 8 MB |

- **CPU memory** goes through a direct-mapped block-RAM cache (32 KB, 32-byte lines, write-through)
  over flash and DRAM. The real MCF5206e has only a 4 KB I-cache and DRAM with wait states, so this
  is not slower than the original.
    - Flash in command mode (status, IDs) bypasses the cache.
    - A flash program invalidates its line; an erase flushes the cache.
- **Two clocks from one VCO (882 MHz), rising edges aligned:**
    - `clk_sys` = 88.2 MHz for the sound engine, the sound bus and DDR3: 2000 clocks per
      44.1 kHz sample. The corpus's worst sample needs 1547 clocks with writes.
    - `clk_cpu` = 44.1 MHz for the CPU, its bus, the cache and the panel I/O. A standalone build of
      TG68K with the MCF5206e peripherals (`cores/proteus/p2k/syn/p2k_cpu.qsf`) needs 21.4 ns
      (−10.0 ns slack at 88.2 MHz; the paths are TG68K's own: exec → ALU → register file).
    - `hdl/p2k_x21.sv` carries the cache's memory requests and the sound-chip accesses between
      the two clocks as ordinary synchronous paths.
    - The MCF5206e peripherals run on a 33 MHz clock enable (fractional, from `clk_cpu`), so the
      UART gives 31250 baud exactly and timer 1 the OS tick.
    - The CPU runs as fast as its memory allows: the OS has no cycle-counted loops [I,
      iteration 11], and audio timing comes from the sample tick. A cache hit takes 2 CPU clocks.
- **Sound-chip writes** (G20, H, G05) go into a FIFO. The engine takes them between samples: a due
  sample starts first, then writes until the next one is due. The CPU waits only when the FIFO is
  full. A write therefore takes effect at a sample boundary, as in MAME's model, where writes are
  applied after the stream is brought up to date.
- **Sound-chip reads** (MAME `g20_r`, `h_r`, `g05_r`) come from CPU-side shadow copies of every
  register written:
    - G20: 4 K words.
    - H: the 16 K words below byte 0x8000; every read in the corpus traces is below it.
    - G05: 32 × 64 words.
  The live values come from the engine, read while it is idle with the write FIFO empty:
    - G20 +0x02 bit 12 (end select) and +0x18/+0x1A (voice address);
    - H +0x34/+0x36/+0x3E (interpolator values).
  The rest is glue:
    - H +0x3A returns the block's +0x7A word.
    - G05 has the index register (+0x0E), chip ID 0x30 (index 0x1E reg 0x18) and the encoder
      delta (index 0x1B reg 0x18).
    - G20 ports A/B (+0x1C/+0x1E) are pipelined sample-memory reads: they return the latched word
      and fetch the next. Baby is selected by latch 0xB00000 bit 6. An unloaded slot reads 0
      (Big) or 0xFFFF (Baby).
- **Flash:** a 28F160 command model (the subset in `p2k/sim/p2k_cpu_tb.cpp` and MAME's
  LH28F160S3: read array, status, ID 0xB0/0xD0, word program, block erase, clear status). Data is
  in DDR3. The whole 2 MB is the save file (`saves/P2K/<pack>.sav`), written when the OS has
  changed the flash.
- **Files the user makes from their own dumps** (no ROMs in the repo):
    - System pack: the flash image (`tools/p2k_flash_image.py`: the v1.24 dump + OS 2.26 .dli,
      user area erased) followed by the HD44780 A00 font.
    - One file per SIMM slot: Baby (2 MB) followed by Big (up to 32 MB), from
      `tools/p2k_build_simm.py`.
  An MGL loads them. The CPU stays in reset while anything loads and restarts after each load.
- **Panel:** keyboard keys for the 9 buttons and the data-entry encoder (MAME's `proteus2k` input
  names); OSD master volume (ADC pot 0); LCD 2 × 24 through `synthframe_hd44780` and
  `synthframe_charlcd`; the 16 LEDs drawn below it.
- **MIDI:** MiSTer UART (MidiLink) to UART1 RX, UART1 TX to the UART out. Main + Sub 1 + Sub 2
  are mixed down, as the engine already does (decision 3 of `p2k-mame-plan.md`).

## Steps and gates

| Step | Work | Gate |
|---|---|---|
| P0 | `cores/p2k` layout (`cfg/`, generated Quartus files), plan, CI entry | `synthcore check` passes; CI green |
| P1 | `hdl/p2k_board.sv`: CPU bus, cache, flash, DRAM, I/O glue, LCD, sound bridge, engine, sample tick | Verilator lint clean; unit benches for the cache (random accesses vs a flat model) and the sound bridge (shadow reads vs `p2k_sound.h`) |
| P2 | System bench `sim/` (board + DDR3 model, packs preloaded, MIDI from a file, LCD/audio/sound-log dumps) | OS 2.26 boots to the main screen with the Composer stand-in, LCD text as MAME's; a corpus MIDI file plays and `p2k_render` on the bench's applied-write log equals the bench's audio bit for bit; note-on start addresses in the same order as MAME's log |
| P3 | `mister/P2K.sv`: hps_io, packs, flash save, keyboard, OSD, video, audio, MIDI, PLL | Quartus 17 build on winbuild meets timing; ALMs/M10K/DSP recorded |
| P4 | Pack tools and MGL (`tools/p2k_pack.py`), smoke script `scripts/mister/smoke.sh` | packs built from the user's dumps; hashes recorded |
| P5 | MiSTer hardware | boots to the main screen (screenshot); MIDI over MidiLink plays (audio peak, no stall); a panel key changes the screen; an edited preset survives a core reload |
| P6 | User flash SIMMs: G20 port B writes, Intel command model for Big/Baby, saved per slot | RENAME FLASH SIMM 106 → 108 as `tools/p2k_flash_test.py`, in the bench (may move to a later iteration) |

## Risks

- **TG68K at 44.1 MHz:** 1.3 ns of margin in the standalone build. The bench reaches the main
  screen at 11.25 s, against MAME's 11.5 s.
- **The sound engine's 100 MHz margin was 0.05 ns standalone.** At 88.2 MHz there are 1.3 ns
  more, but the full design shares routing.
- **Boot time in simulation.** Iteration 11 took about 35 minutes for 12 s of system time with
  the CPU alone. The system bench reports clocks/s; the boot gate runs in the background.
- **First boot with the user area erased:** the OS rebuilds the factory defaults (about 45 s in
  MAME) and writes them to flash, which then goes to the save file.
