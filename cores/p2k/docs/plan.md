# Proteus 2000 MiSTer core: plan (iteration 18)

**Status (2026-11-25):** P0-P6 done. Results are in "Results" at the end.

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
  | Debug trace (`TRACE_BASE`) | 0x7400000 | 16 KB |
  | Download staging (`STAGE_BASE`, CONF_STR address 0x3C000000) | 0x7800000 | 34 MB |

- **Downloads go straight to DDR3.** The pack and SIMM entries in CONF_STR carry the load address
  0x3C000000, so Main_MiSTer copies the file into the staging region itself instead of sending it
  a byte at a time through hps_io (about 2.7 µs a byte, 100 s for a 34 MB SIMM). When the
  download ends, `p2k_loader` copies the staged words to their places in bursts of 8, swapping
  the bytes of each 16-bit word, and the font bytes to their block RAM (34 MB in about 0.3 s).
  The file formats are unchanged, and a byte download (no load address) still works.

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
- **User flash SIMMs (P6)** in slots 1 and 2: a `.sim` file mounted as "Flash SIMM slot N"
  (CONF_STR `S1`/`S2`, hps_io virtual disks 1 and 2; disk 0 stays the flash save file).
    - `hdl/p2k_sndbus.sv`: port B data writes (+0x1E) to a flash slot go to MAME's
      `p2k::intel_flash` (`cores/proteus/p2k/model/p2k_flash.h`): Big is four x16 chips (word
      address bits 23-22), Baby one; program ANDs the word into DDR3 (read, then a lane write),
      erase fills the block with 0xFFFF in fills of 128 words, buffered program has one 32-word
      buffer per Big chip. In any mode but read array and ID a port read returns 0x0080 (ready).
      The CPU waits for each operation. The chips return to read array while the CPU is held.
    - `hdl/p2k_imgsave.sv` (replaces `p2k_flashsave.sv`, same behaviour for the flash save file):
      a mount loads the file through the SD interface (16 KB requests) into the slot's Baby and
      Big words, erases the rest of the slot, and restarts the CPU. The sound bus marks the
      256 KB region of the file each program or erase lands in; a save (the flash save's
      triggers: "Save flash", or autosave when the OSD opens) writes those regions back,
      within the file (Main_MiSTer does not grow a mounted image, so `tools/p2k_pack.py simm
      --flash` makes 34 MB files). A ROM SIMM download into the slot makes it a ROM slot again.
    - The loader's copier and the image transfers share the memory client; whichever asks first
      owns it until it is done (`x_owner` in `mister/P2K.sv`).
    - Why mounts and not the download path: a download has no way back to the file, and
      Main_MiSTer mounts the save file of an `FS` entry on disk 0 only.
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
| P6 | User flash SIMMs: G20 port B writes, Intel command model for Big/Baby, saved per slot | RENAME FLASH SIMM 106 → 108 as `tools/p2k_flash_test.py`, in the bench: the written images equal MAME's; on the MiSTer the mounted file is written back with ID 108 |

## Risks

- **TG68K at 44.1 MHz:** 1.3 ns of margin in the standalone build. The bench reaches the main
  screen at 11.25 s, against MAME's 11.5 s.
- **The sound engine's 100 MHz margin was 0.05 ns standalone.** At 88.2 MHz there are 1.3 ns
  more, but the full design shares routing.
- **Boot time in simulation.** Iteration 11 took about 35 minutes for 12 s of system time with
  the CPU alone. The system bench reports clocks/s; the boot gate runs in the background.
- **First boot with the user area erased:** the OS rebuilds the factory defaults (about 45 s in
  MAME) and writes them to flash, which then goes to the save file.

## Results (2026-11-24)

**P1, units (CI, no ROMs):**
- `tests/test_p2k_mem.py`: the cache and flash model against a flat model, 3 seeds of
  150,000 random accesses with random memory-port timing.
- `tests/test_p2k_sndbus.py`: the sound bus against MAME's `emup2k.cpp` handlers, 3 seeds of
  100,000 accesses.
- Both identical. A mutation of the cache's byte-lane update is caught.
- The sound-bus bench found two bugs: the port address was read from the wrong shadow words, and
  an unloaded slot's constant read raised a stray memory request.

**P2, system bench** (`sim/p2k_board_tb`, the user's flash and the Composer stand-in):
- OS 2.26 reaches the main screen at 12.25 s; MAME at 11.5 s. The screen equals MAME's rendered
  screenshot: `C01A Vol127 Pan00  User` / `000` + CGRAM glyph + ` drm:Piatti`. MAME's text log
  differs in one character because it shadows data writes only.
- Corpus case `poly`, with its MIDI offset to 11.5 s so it arrives after the main screen as in
  MAME: all 214 note starts (voice and sample address, in order) equal MAME's `snd.log`.
- The bench's audio equals `p2k_render` replaying the bench's own write log, bit for bit
  (1,058,399 frames, SHA-256 `8bc01aa03e551a59…`). No sample started late.

**P3, Quartus 17.0** (winbuild, seed 1):

| | |
|---|---|
| ALMs | 26,088 (62 %) |
| Registers | 36,968 |
| Block memory | 1,623,687 bits, 243 M10K |
| DSP blocks | 92 (82 %) |
| Setup slack | clk_sys +1.625 ns, clk_cpu +1.386 ns, video +5.915 ns, HDMI +0.381 ns |
| Hold slack | all positive |

**P5, MiSTer** (`scripts/mister/smoke.sh`, the build above, 2026-11-24 19:00; a build with
3-clock cache hits passed it too):
- Main screen as above, about 110 s after the MGL load. Most of that is the 36 MB SIMM download;
  the CPU restarts after it.
- MIDI out (2026-11-25, `scripts/mister/midi_probe.py`): the universal identity request
  `F0 7E 7F 06 01 F7` sent to the core gets `F0 7E 00 06 02 18 04 04 03 00 32 2E 32 36 F7` back on
  the UART (E-mu, OS "2.26").
- MIDI over the UART: the poly case's program changes show (`P2Ksy 070 rom:Click 909`) and the
  level meter moves.
- The Master key opens MASTER TRANSPOSE. No late sample.
- A pack with an erased user area: the OS writes its factory defaults, the save file is 2 MB
  with them, and the reload restores them.

**Hardware bugs found on the way:**
- Quartus built the response FIFO of the CPU's clock crossing (`p2k_x21`) as an MLAB read
  combinationally from the other clock. On hardware the CPU read the reset vector with one bit
  wrong and ran away. The FIFO (and the cache's write queue) are registers now
  (`ramstyle "logic"`).
- The CPU could leave reset while an access from before the reset was still in flight. It now
  waits until the cache and the sound bus are quiet.
- The flash returns to read-array mode on a CPU reset.

**Debug aids kept in the core:**
- The OSD "Diagnostics" line: PC, last bus address, load and hold flags.
- A trace (`TRACE`): the last 1,024 CPU bus accesses, written to DDR3 at byte 0x3A000000 when
  the PC leaves flash and DRAM. Read it with `synth-mister peek`.

**Iteration 19, direct downloads** (see "Downloads go straight to DDR3" above):
- `sim/loader_tb.cpp` (CI: `tests/test_p2k_loader.py`): packs and SIMMs, odd sizes and partial
  last words, downloaded as bytes and direct with random memory timing and a save in flight:
  identical DDR3 words, font, slot sizes and pack_ok against the file layout, `loading` and
  `pack_loading` high once per download. 3 seeds and a 34 MB SIMM (27M clocks, 0.31 s at
  88.2 MHz with the port ready 1 clock in 2).
- Quartus 17.0 (winbuild, seed 1): 26,747 ALMs (64 %), 37,610 registers, 245 M10K, 92 DSP;
  setup slack clk_sys +1.103 ns, clk_cpu +1.861 ns, video +5.835 ns, HDMI +0.651 ns; hold all
  positive.
- MiSTer smoke test: main screen 19 s after the MGL load (iteration 18: about 110 s); MIDI, level
  meter, Master key, no late sample; the save file is 2 MB with the factory defaults and the
  reload restores it.
- Found on the MiSTer: `pack_loading` fell for the one clock between the download's end and the
  copier's start; the flash save took the rise after it for a new pack download and forgot the
  mounted save file (no restore, no save). Fixed; the loader bench now checks it.

**P6, user flash SIMMs** (2026-11-25, iteration 21):
- `tests/test_p2k_sndbus.py`: a flash slot added to the sound-bus bench, driven by the OS's command
  sequences (program, erase, buffered program with up to 40 words, status, ID, read array, other
  commands, beyond Baby) and compared with MAME's `p2k::intel_flash` on its own copy of the slot:
  port reads identical, the slot's DDR3 equal to MAME's copy at the end, every changed region
  marked. 3 seeds; a mutation without the AND of a program is caught. The bench found that one
  write buffer shared by all chips differs from MAME's per-chip buffers; each Big chip has its own.
- `tests/test_p2k_imgsave.py` (`sim/imgsave_tb.cpp`): the image transfers against an hps_io SD
  model and a DDR3 model: restore and autosave of the save file, loads of an odd-sized and a
  34 MB SIMM file (the rest erased), write-back of exactly the marked regions within each file, a
  ROM download and a pack download stopping a transfer, read-only and empty mounts. 3 seeds and
  the 34 MB case; a mutation that drops the marks is caught.
- Reference: MAME (`cores/proteus/tools/p2k_flash_test.py --seconds 150`) with the Composer
  stand-in built as ROM ID 106 ("UsrFl") and the user's flash: RENAME FLASH SIMM to 108 shows
  "Done. Please reboot now!" at 47.25 s after 3,812,040 port-B writes; ID 108 in the Big header,
  block checksums valid, the Baby library rewritten. Written SIMM file (Baby + Big, 34 MB)
  SHA-256 `ee79865ee082efa5…b0d7`. The 40 s run of the older test stops in the middle of the
  rewrite with this larger library.
- System bench (`sim/p2k_board_tb --flash-slot 1 --keys .../rename_106_108.keys --until "Done. Please"`,
  the same flash and SIMM): "Done. Please reboot now!" at 73.75 s (the core's sound-chip accesses
  wait for the clock crossing, MAME's do not); the written Baby + Big equal MAME's file byte for
  byte (`ee79865ee082efa5…b0d7`); 8 regions changed, all marked dirty; no late sample.
- Quartus 17.0 (winbuild, seed 1): 27,645 ALMs (66 %), 38,742 registers, 247 M10K, 92 DSP;
  setup slack clk_sys +0.979 ns, clk_cpu +1.162 ns, video +5.689 ns, HDMI +0.527 ns; hold all
  positive.
- MiSTer, `scripts/mister/flash_smoke.sh` (pack from the same flash, the ID 106 file mounted as
  Flash SIMM slot 1): main screen 24 s after the MGL load (the 34 MB file read through the SD
  interface included); the rename on the panel keys; after the OSD opened, the file on the SD card
  has ID 108 and equals MAME's written file byte for byte; no late sample; the reload reaches the
  main screen. `scripts/mister/smoke.sh` on the same build: main screen at 20 s, MIDI and level
  meter, Master key, no late sample, the 2 MB save file written and restored.
- Found on the MiSTer: the erase held the sample memory for whole fills of 128 words while the
  engine waited, and a sample started late (the status strip's underrun box). Flash memory
  requests now start only while the engine is idle with no sample due, and erase fills are 32
  words. `flash_smoke.sh` checks the underrun box.

- Found on the MiSTer with the release build: loading the core over a running P2K (the smoke
  test's last MGL, then the flash test's) left the system pack's first 8 flash words as 8 words of
  sample data now and then (3 of 4 tries): the CPU read its reset vector from them and ran away
  (blank LCD; the bus trace in DDR3 showed it). The rest of the 2 MB was right, and the data was
  from the part of slot 1 the previous core's engine had been reading: the copier's first read
  burst came back with other words. [I] Read data from the previous core still pending when the
  FPGA was loaded; not settled. The copier now reads every burst twice and writes it only when
  both agree (`copy_retries` on the diagnostics line; 0.3 s more for a 34 MB SIMM); the loader
  bench injects misread bursts. The same sequence then booted 6 of 6 times.

**Open:** none of the planned steps.
