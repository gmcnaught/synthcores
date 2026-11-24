# Proteus 2000 sound HDL (iteration 14)

`p2k/rtl/snd/p2k_snd.sv` (G20 voices, H interpolators and filter, mixer) and `p2k/rtl/snd/p2k_fx.sv`
(G05 effects) compute the Proteus 2000's audio from its sound-chip registers. The definition they
match bit for bit is the integer model `p2k/model/p2k_sound.h` (`docs/p2k-mame-plan.md`, decision 1).
Each block's comments name the model function it follows.

This is the reference datapath, checked in simulation only. It is not yet the MiSTer design: no CPU
connection, no DDR3 port and no Quartus build (see "Towards the MiSTer core").

## Interface

| Port | Use |
|---|---|
| `wr_valid/wr_ready`, `wr_chip`, `wr_addr`, `wr_data`, `wr_mask` | One register write while idle. `wr_chip` 0 = G20 (word offset 0-0xFFF), 1 = H (word offset 0-0x7FFFF; only the voice blocks below byte 0x8000 are kept), 2 = G05 (`{channel, word}`). `COMBINE_DATA` mask, as in MAME |
| `start` / `done`, `out_l`, `out_r` | One 44.1 kHz output sample. Writes made before `start` take effect in that sample, as in `sound::step` |
| `mem_req`, `mem_addr[26:0]`, `mem_ack`, `mem_data` | Sample memory, one 16-bit word per request; addresses of 2^26 and above read 0 |
| `filter_on`, `fx_on` | The model's `--nofilter` / `--nofx` bypasses |

## Datapath

- **Voices (`sound::step`, `sound_base::advance`):** 128 voices in turn. A voice with a zero increment
  is skipped. Two words are read (`s0`, `s1`), interpolated in Q8, filtered, and the address advances
  in 4.12. An end crossing jumps to `+0x14`; a loop keeps the overshoot modulo the loop length. When
  the overshoot is at least the loop length, a shift-and-subtract divider takes 27 clocks.
- **Register side effects (`write_g20`, `write_h`):** `+0x02` sets the end select, `+0x1A` restarts the
  voice and clears its filter state, H writes below `+0x28` time-stamp the block for 12th-order
  chaining, and the interpolator offsets set the current value or a destination with its increment
  code (Table I encode).
- **H filter (`sound::filter`, `sound::stage`):** three DF-II sections per stage, one section a clock,
  128-bit intermediate products as in the model (`__int128`), states saturated to ±(2^47 − 1). A
  chained voice runs a second stage through the next slot's block. The gain word follows the 16-bit
  latch.
- **Interpolators (`tick_interps`):** all 256 blocks × 3 after the voice loop, one block a clock.
- **Effects (`fx_int`):** configured from the G05 registers every 256 samples, then five clocks a
  sample: damping and the six reflection combs, the tails, the two diffusers, then the tail return,
  output mix and chorus. Delay lines whose length changes are cleared, as the model's `resize`.

## Checks

- `tests/p2k_snd/test_p2k_snd.py` (CI, no ROMs): random register logs (`tools/p2k_snd_randlog.py`)
  over a random-data SIMM, bench against `p2k_render` in the three modes.
- `tools/p2k_snd_check.py` (local, needs the corpus): every corpus case in the three modes, bench
  against `p2k_render`; in full mode also against MAME's `snd.raw` (the manifest hash).

Results: see "Corpus gate" below.

## Corpus gate

Run on 2026-11-24 with the Composer stand-in SIMM (`P2K_SIMMDIR=build/p2k/simm/composer
tools/p2k_snd_check.py --jobs 6`): **66/66 identical** (22 cases × 3 modes), 87,245,389 frames a
mode. In full mode every hash equals MAME's `snd.raw` and the manifest's `sndout_sha256`.

| Mode | Runs | Clocks per sample, worst mean / max | Chained voice-samples | Bench time |
|---|---|---|---|---|
| voices (`--nofilter --nofx`) | 22/22 | 1549.9 / 1796 | 0 | 4.7 h CPU |
| filter (`--nofx`) | 22/22 | 2138.9 / 2561 | 1,850,724,286 | 7.7 h CPU |
| full | 22/22 | 2146.9 / 2569 | 1,850,724,286 | 7.8 h CPU |

The worst case in every mode is `soak` (27,122,206 frames, 10 minutes). Memory reads: 206.8 per
sample on average in `soak`. The loop divider (overshoot of at least one loop length) never ran on
the corpus; the random tests in CI cover it.

Random stimuli before the corpus run: voices identical on 19 seeds, filter on 15 (saturation and
chaining, 100k-213k chained voice-samples a seed), full on 6 seeds with G05 registers.

## Towards the MiSTer core

Observed (bench, sample memory latency 3 clocks):
- **Clock budget:** the worst sample took 2569 clocks, so this sequential datapath needs at least
  113.3 MHz for 44.1 kHz (2569 × 44,100). The filter adds about 40% to the voice loop.
- **Sample memory:** at most 256 word reads a sample (2 per voice), 11.29 M words/s; 206.8 a
  sample on average in `soak`. Each read waits for its data, so the clock count grows by about
  256 clocks for each extra clock of latency.
- **Effects delay lines:** 137,872 words of 32 bits (reflections 6 × 7,986, tails 2 × 8,228,
  diffusers 4 × 1,991, chorus 2 × 32,768), about 4.4 Mbit. The DE10-Nano has 5.6 Mbit of M10K,
  so these belong in SDRAM or DDR3 rather than block RAM.

Inferred: a MiSTer top needs the sample reads pipelined (several voices' requests in flight) to
tolerate DDR3 latency, and the 128-bit filter products split over more clocks or DSP blocks to
meet timing on the Cyclone V.

Unknown: multiplier and M10K counts and Fmax. They depend on that restructuring and need a
Quartus build; none was run in this iteration.
