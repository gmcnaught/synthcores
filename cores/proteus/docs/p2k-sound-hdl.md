# Proteus 2000 sound HDL

`p2k/rtl/snd/` computes the Proteus 2000's audio from its sound-chip registers: `p2k_snd.sv` (G20
voices, H interpolators, mixer, write path), `p2k_snd_filt.sv` (H filter), `p2k_fx.sv` (G05 effects),
with `p2k_ram.sv`, `p2k_mul.sv`, `p2k_fx_sine_rom.sv` and the shared functions in `p2k_snd_fn.svh`.
The definition they match bit for bit is the integer model `p2k/model/p2k_sound.h`
(`docs/p2k-mame-plan.md`, decision 1). Comments name the model function each part follows.

Iteration 14 wrote the datapath for simulation. Iteration 16 restructured it for the DE10-Nano's
Cyclone V without changing an output sample: it now builds with Quartus 17 and meets timing at
100 MHz on its own. It is not yet a MiSTer core: no CPU connection, no sample loading, no
hardware test (see "Towards the MiSTer core").

## Interface

| Port | Use |
|---|---|
| `wr_valid/wr_ready`, `wr_chip`, `wr_addr`, `wr_data`, `wr_mask` | One register write while idle. `wr_chip` 0 = G20 (word offset 0-0xFFF), 1 = H (word offset 0-0x7FFFF; only the voice blocks below byte 0x8000 are kept), 2 = G05 (`{channel, word}`). `COMBINE_DATA` mask, as in MAME |
| `start` / `done`, `out_l`, `out_r` | One 44.1 kHz output sample. Writes made before `start` take effect in that sample, as in `sound::step` |
| `mem_req_*`, `mem_rsp_*` | synthframe memory port (`synthframe_mem_ddr3` contract), tag bit 1 = client (0 voices, 1 effects) |
| `filter_on`, `fx_on` | The model's `--nofilter` / `--nofx` bypasses |

After `rst` the engine clears its register files (2048 clocks) before it takes a write or a start.

Memory (64-bit words): sample word `a` (`a < 2^26`) is bits `[16 (a mod 4) +: 16]` of word
`SMP_BASE + a / 4`; words from 2^26 up read as 0. The effects delay lines are 14 × 2^14 words at
`FX_BASE` (lines 0-5 reflections, 6-7 tails, 8-11 diffusers, 12-13 chorus).

## Structure

- **Register files in block RAM** (`p2k_ram`, registered read). The G20 words the voice loop reads
  (2, 4-7, 10-12) and the two a write needs (1, 13) are one RAM each, indexed by voice. The H RAM
  is not kept whole: only the words the engine reads have copies, the filter coefficient words
  (0-3, 8-11, 16-19 of each block, four RAMs indexed `{block, section}`) and the flag byte 0x7A
  (three copies, for the front end and the filter's two reads). Writes read-modify-write the copy.
- **Write path:** read the old value (1 clock), combine and set the side effects (1), write (1).
  An interpolator target takes two more clocks for the increment code; a voice restart clears its
  six filter sections (6 clocks).
- **Front end** (`sound::step` voice loop, `sound_base::advance`): 3 clocks for a voice with a zero
  increment less than that: 1. It decides the 12th-order chain, reads the block's route and level
  interpolators, advances the address (the loop-modulo divider stalls it for 27 clocks; the corpus
  never needs it), and asks for the sample pair: one 64-bit read, two when the pair straddles a
  word. The pairs come back in order into a 16-entry ring.
- **Interpolation** `y = (s0 << 8) + ((s1 - s0) frac >> 4)` in three clocks, then to the filter
  (`filter_on`) or straight to the mixer. A voice leaves the ring only when a filter context is
  free.
- **Filter** (`p2k_snd_filt`, `sound::filter`, `sound::stage`): up to 8 voices in flight, each
  issuing one operation every 8 clocks (sections 0-2 and the gain; again through the next slot's
  block when chained). An operation goes down a 14-clock pipeline: coefficient words and state,
  `hcode` products, `hdec`, the Q28 coefficients, five 32 × 48 products (`p2k_mul`, four 27 × 27
  partial products each), sums, rounding and saturation. Only `g x` depends on the voice's running
  value, read 6 clocks after the issue and written back after 13:
  `x' = sat47(w + rsh(n2 s1 - n1 s0, 28))` with `w = sat47(rsh(g x + a1 s0 - a2 s1, 28))`.
  The model's 128-bit intermediates are at most 82 bits wide here (bounds in the comments).
- **Mixer:** per voice, two 32 × 24 products and the four bus accumulators (48 bits; the model's
  sums stay below 2^39).
- **After the last voice:** the effects sends (16 products through one 48 × 17 multiplier),
  `fx_int::process`, and in parallel the interpolator tick (256 blocks, one a clock).
- **Effects** (`p2k_fx`): the G05 registers in block RAM; `fx_int::configure` reads its 41
  registers one a clock at the start of every 256th sample. At each sample start it computes the
  read positions and the chorus LFO and asks for the 16 delay-line words the sample will read, so
  they are in by the time the voice mix is done. `process` then runs on one shared
  multiply-add (two 32 × 18 products, 36 operations) and writes 13 words.
- **Delay lines in DDR3, no clearing:** the model clears a line at reset and when configure changes
  its length. A comb, tail or diffuser line is read and written at one position a sample, moving
  by one, so its first `len` reads after a clear are of positions not yet written: they read zero
  (a per-line count). The chorus is written at `cpos` from 0 at reset; a tap reads zero while
  `cpos` has not yet passed it. The bench fills the effects region with random data to check this.

## Checks

- `tests/p2k_snd/test_p2k_snd.py` (CI, no ROMs): random register logs (`tools/p2k_snd_randlog.py`)
  over a random-data SIMM, bench against `p2k_render` in the three modes, also with all 128 voices
  at DDR3 latency 1 and at 30-60 clocks with BUSY every other clock.
- `tools/p2k_snd_check.py` (local, needs the corpus): every corpus case in the three modes, bench
  against `p2k_render`; in full mode also against MAME's `snd.raw` (the manifest hash).
- The bench (`p2k/sim/p2k_snd_tb.cpp`, top `p2k_snd_bench.sv`) runs `p2k_snd` on
  `synthframe_mem_ddr3` and a DDR3 model: BUSY on a random 1 clock in 4, reads back in order after
  8-24 clocks (`--ddr-busy`, `--ddr-lat`). The Verilator build stops on a block-RAM read and write
  of one word in the same clock and on a bus sum beyond 2^39.
- `p2k/syn/winbuild_snd.sh`: Quartus 17 build of the bench top (all pins virtual) on the Windows
  build host; reports the fit, the timing per corner and the worst paths.

## Results (iteration 16)

RESULTS_PLACEHOLDER

## Towards the MiSTer core

- **Clock:** the engine meets 100 MHz on its own with little margin (0.05 ns at the slow -40 °C
  corner). In a full core a lower clock, down to the worst sample's clock count × 44,100, is
  available as slack.
- **CPU writes:** the write path takes 4-6 clocks a write and the sample loop does not run while
  writes are applied. The firmware's write bursts at note-on are listed in the results; the top
  will need a write FIFO from the CPU and to schedule samples against it.
- **Sample memory:** the corpus reads up to about 130 words a sample; a P2K SIMM image (32 MB per
  slot) goes to DDR3 at `SMP_BASE`.
- **Not done:** the MiSTer top (TG68K/ColdFire CPU from iteration 11, MIDI, SIMM and flash
  loading, audio FIFO), a full-core Quartus build and hardware tests.
