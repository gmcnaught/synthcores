# Board and MiSTer core (`hdl/board/`, `mister/`, M5)

`hdl/board/sc55_board.sv` wraps the core (`hdl/sc55_core.sv`) with what a
board provides; `mister/SC55.sv` (module `emu`, Template_MiSTer) adds hps_io,
the PLL (50 MHz core, 25 MHz video), DDR3 and the outputs. The parts it
shares with the JV-880's board (pack load, DDR3, reset sequence, pacing,
MIDI, status flags, audio chain) are one module,
`modules/roland/hdl/board/roland_board_common.sv`.

| Part | File | What it does |
|---|---|---|
| ROM pack | `rom_loader.sv`, `tools/romset.py pack` | hps_io download (`games/SC-55/boot.rom` or OSD "Load ROM pack"): header "SC55" + model, ROM1 and ROM2 into the MCU's block RAM, the three wave ROMs into DDR3. A v2 pack holds the wave ROMs **unscrambled**, as the reference model has them in memory (`rom_io.cpp unscramble`: address bits and data bits are permuted on the chips). A v3 pack (the layout the MRAs in `mra/` assemble from MAME's zips) holds them as dumped, and the loader unscrambles each byte on its way to DDR3 by the same bit permutation; `board_lockstep` checks all 4 MB in DDR3 against the model's wave ROMs after a v3 load. |
| Wave ROMs | `wave_ddr.sv` | The PCM's in-order byte stream on DDR3 (64-bit words from 0x30000000). A byte in the same word as the byte requested just before shares its read; reads are pipelined. |
| Pacing | `step_pacer.sv` | 600 ns of real time per step (mk1: 12 cycles at 20 MHz); up to 200 µs of credit after a stall. OSD "Real-time pacing" off runs as fast as possible. |
| MIDI | `midi_rx.sv` | MidiLink UART at 31250, or 9600 when ttyS1 fell back (from synclavierii). |
| Audio | `audio_fifo.sv` | The renderer's s16 conversion (`clamp(x >> 15)`), 64 kHz FIFO (synclavierii). Plain hold into AUDIO_L/R for now. |
| Video | `status_video.sv` | Status picture until the LCD (M6): band red / amber / green = no ROM / loading / running; MIDI and underrun boxes. |

The core is held in reset until a pack with a valid header has loaded; then
the PCM clears eram and the MCU clears SRAM and on-chip RAM (the model starts
zeroed), and the MCU takes its reset vector.

## Real-time design

The model advances everything in one `MCU_Step`; at 50 MHz the core has 30
clocks per step. The PCM runs the frames due alongside the CPU (frame ~410
clocks for 24 voices, independent of ROM latency) and stays exact:

* CPU accesses to PCM registers wait until the PCM has caught up;
* the interrupt check waits only while IRQ0 is enabled, would beat the mask,
  is not yet requested and the current frame's pre-scan (which finds the
  voice that will raise IRQ0 from the rows at frame start) is not done;
* the pre-scan also runs every voice's address generator and issues all the
  frame's wave-ROM reads, so ROM latency hides behind the reverb section.

Memory reads are two clocks (block RAM). Measured in the board bench: 23.0
clocks per step on average with loud MIDI.

## Verification (`sim/board/board_lockstep.cpp`, `tests/test_system_lockstep.py`)

Power-on with random contents in every register and memory, the pack over
the ioctl handshake (with `ioctl_wait`), DDR3 with BUSY on random clocks and
reads back after 8–24 clocks; then step by step against the reference, PC
and the two sample streams compared in order, the MCU's receive state and
device registers every step, the PCM's state whenever it has caught up.

| Run | Result |
|---|---|
| board, loud MIDI, 10 M steps | identical: 496,922 samples, peak −2.9 dBFS; 6.2 M DDR3 reads |
| system (`sc55_core`), avmidi `0A.mid` in the renderer's schedule (GM reset, 24 M steps, events at 600 ns/step) | output SHA-256 `1bc443bb…c74f` = `sc55-render` (102.9 M steps, 4,066,094 samples) |

Found by the board bench: the pack first held the raw (scrambled) wave
ROMs; the PCM state check pointed at a voice's first nibble, read from ROM.

## On the MiSTer (`scripts/mister/`)

```bash
MISTER=root@<address> scripts/mister/sc55_run.sh SC55_YYYYMMDD.rbf 0000000000000000 f07e7f0901f7 90407f
python3 scripts/mister/smf2events.py song.mid /tmp/song.txt          # timed bytes
scp /tmp/song.txt scripts/mister/play_tty.py root@<address>:/tmp/
ssh root@<address> python3 /tmp/play_tty.py /tmp/song.txt 100       # 100 s, song repeated
```

`sc55_run.sh` installs the RBF and the ROM pack, loads the core, sends MIDI
at 9600 baud and takes a screenshot. The core decodes 31250 (MidiLink) and
9600; it starts at 31250 and switches after 3 framing errors there while the
9600 receiver decodes 4 clean bytes, so start a 9600 session with a few
0x00 bytes (framing errors at 31250, data bytes the firmware ignores) and a
GM reset: bytes sent at 9600 before the switch arrive as garbage.

Status strip under the LCD: state band (green = running), MIDI (white
flash), underrun (red flash; dark red once any underrun has happened),
real-time lag (amber once the pacer lost time after the first second).

| Run (2026-11-20, the test MiSTer, build seed 3) | Result |
|---|---|
| boot, no MIDI | LCD "01 001 Piano 1", as in the bench |
| GM reset, C major chord | part 1 level bar, Piano 1 |
| avmidi `0A.mid` repeated, 100 s at 9600 baud, screenshot every ~2 s | 45 screenshots: underrun flash never lit, MIDI lit in 41, bars moving in 42 (the gap between repeats in the others) |
| same, 100 s (12,813 bytes), build with the sticky indicators | after 100 s: underrun and lag boxes both off (no underrun since start, no lag after the first second) |

The sticky indicators are checked in the board bench (`--free-run N
--stall-at CLOCK LEN` closes `step_gate`): a 2 ms stall sets the lag box,
a 20 ms stall also counts an underrun; neither is set before the stall.
