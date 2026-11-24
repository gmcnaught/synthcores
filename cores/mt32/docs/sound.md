# MT-32 sound section: LA32, reverb chip, mix (plan milestones M3, M4)

HDL bit-exact to Nuked-MT32 (`modules/nuked-mt32`, commit `5b4e239`): `la32.cpp`, `rv2.cpp` and
the end of `la32_t::clock`. GPL-2.0-or-later, like the reference.

| File | Part |
|---|---|
| `hdl/la32/mt32_la32.sv` | LA32: 32 partial slots, envelopes (TVA/TVF counters in `ram2`), wave generator (PCM and synthesized square/saw with resonance), ring modulation, pan, accumulators, the `reg_file[5]` filter counters |
| `hdl/la32/mt32_la32_pow.hex`, `mt32_la32_logsin.hex` | the `powrom`/`powdrom` and `logsin` tables, generated from `la32.cpp` by `sim/sound/gen_la32_tables.py` |
| `hdl/rv2/mt32_rv2.sv` | reverb chip: program ROM (32 KB), delay RAM (16 K x 16), two accumulators |
| `hdl/mt32_mix.sv` | output mix: `expand(acc) + reverb * 17 / 25`, times `(255 - pwm) / 255`, clamped |
| `hdl/mt32_sound.sv` | the three together, sequenced as `la32_t::clock` |
| `sim/sound/sound_bench.cpp`, `build.sh` | Verilator replay bench |
| `tests/sound_check.sh` | the gate over the MIDI corpus |
| `sim/sound/syn/` | standalone Quartus 17 build on winbuild (`winbuild_sound.sh`) |

## Interface (`mt32_sound`)

| Port | Use |
|---|---|
| `run` | low holds the sequencer before the next slot starts |
| `cpu_turn` (pulse), `slot_idx[4:0]`, `cpu_done` (pulse in) | after each slot: the board runs the CPU to `la32_cycle * 375 / 256` CPU cycles, pushes a MIDI byte (`push_midi`), sets `la32_sh3` at slots 15 (0) and 31 (1), then pulses `cpu_done` |
| `la32_cycle[63:0]` | the reference's `la32_cycles`: 8 a slot, 256 a sample; already advanced for the slot when `cpu_turn` pulses |
| `bus_we`, `bus_addr[8:0]`, `bus_wdata`, `bus_rdata`, `extint` | the LA32 at CPU addresses 0x0C00-0x0FFF (`addr & 0x1ff`). `bus_rdata` is `la32_t::read` (int_status, or 0xFF at 0x1C0-0x1FF), combinational from `bus_addr`. `extint` is `int_state` (level): it rises only while a slot is computed (before `cpu_turn`) and falls on an odd register write |
| `rv_we`, `rv_sel[1:0]`, `rv_data` | the reverb registers with the raw CPU byte: 0 = write to 0x0800 (`saw0`), 1 = 0x0400 (`saw1`), 2 = 0x0200 (ROM bank, `(data >> 1) & 3`) |
| `pwm[7:0]` | the CPU's PWM_CONTROL (`pwm_value()`); read at the end of the sample |
| `rvrom_we`, `rvrom_addr[14:0]`, `rvrom_data` | reverb program ROM load (`mt32_reverb.bin`), before `run` |
| `pcm_req_*`, `pcm_rsp_*` | PCM ROM as a synthframe memory-port client (64-bit words, in-order responses, any latency; `req_len` is not driven: one word per request). Byte b of `mt32_pcm.bin` is bits `[8*(b&7)+7 : 8*(b&7)]` of word `PCM_BASE + b/8` |
| `sample_valid`, `out_l`, `out_r` | 32 kHz stereo, after slot 31's `cpu_done` |
| `dbg_*` | the reference's `--out-log` fields (with `sample_valid`) and `--slot-log` fields (with `dbg_slot_done`) |

Reset (`rst`) gives the state of the `la32_t` and `rv2_t` constructors: every register, counter,
accumulator and `reg_file` entry 0. The reverb delay RAM (block RAM) is not cleared by `rst`:
it is 0 after configuration (as in the reference and in Verilator), but a core reset without
reconfiguration keeps its contents. Whether that is audible after a reset is not checked.

The CPU must only write between `cpu_turn` and `cpu_done`, as the reference does.

## Schedule of one slot

`la32_t::clock`, per slot `i`, and what `mt32_sound` does for it:

1. the LA32 computes slot `i` (`mt32_la32`, `slot_go` .. `slot_done`); in parallel the reverb
   chip runs its two program steps (`rv2->clock(8)`). At `i == 4` the reverb input is
   `expand(accum[prev][6])`, at `i == 20` `expand(accum[prev][2])` (the sample before), set
   before the steps. Running the two in parallel is exact: the reverb steps depend on the
   LA32 only through those inputs, which slot `i` does not change.
2. `la32_cycle += 8`, then `cpu_turn`. CPU writes in the turn reach the LA32 registers before
   slot `i + 1`, and the reverb registers before its next steps.
3. after slot 31's turn: the mix of the sample before. The LA32 clamps this sample's
   accumulators after slot 31; the next sample's slot 0 clears the other bank.

`mt32_la32` stages (one clock each). The pow and logsin tables are dual-port ROMs; a lookup whose
address is presented in stage X is used in stage X + 3 (ROM output register, then the pow add,
then the pow shift), hence the wait stages W:

| Stage | Work |
|---|---|
| IDLE | `slot_go`: the `w46` envelope-run shift, register file and `ram2` reads; pow lookups for both envelopes |
| TVW, TVW2, TV | envelope sums, `ram2[0..1]`, envelope interrupt; pow lookup for the pitch |
| WGW, WGW2, WG1 | phase, `ram2[2]` |
| WG2 | PCM: wave addresses, end interrupts. Synthesis: cutoff/resonance chain |
| PREQ1, PREQ2, PWAIT | PCM: up to two 64-bit reads (none for an address at or above 0x80000) |
| W3, W3W, W3W2 | PCM: sample decode, pow lookups |
| WG3, N2W, N2W2 | synthesis: pow and logsin lookups |
| N3A, N3A2, N3B, N3C, N3D | synthesis: products `m2`-`m5`, the `w4xx` flags, logsin lookups, `t45 * t44` |
| N4, N5, N5W, N5W2 | synthesis: resonance and wave-shape sums, pow lookups |
| G0, G1, G2 | signs of w1/w2; interpolation or square/saw combine; ring modulation |
| ACC1, ACC1B, ACC2 | pan product, accumulators, `reg_file[5]` counters, `w186`-`w188` latch; `slot_done` |

From `slot_go` to `slot_done` a synthesis slot takes 26 clocks and a PCM slot 20 plus the
memory wait beyond one clock. The reverb chip's 7 clocks for a slot run alongside.

## Results (2026-11-24)

**Gate (`SLOT=1 tests/sound_check.sh`):** every case identical for both seeds (random PCM latency
1-40 clocks, random `req_ready`). That covers every sample's nine `--out-log` fields, every
LA32 interrupt set and clear, and every slot's state (`--slot-log`: oo, prev, w46, ram2,
accumulators, w186-w188).

| Case | Samples | IRQ events | PCM reads/sample | Clocks/slot mean (max) |
|---|---|---|---|---|
| boot | 96,000 | 0 | 0.00 | 26.5 (41) |
| controllers | 224,000 | 86 | 4.20 | 27.9 (83) |
| dense | 771,519 | 146,466 | 16.30 | 32.0 (82) |
| parts | 294,400 | 7,728 | 6.31 | 28.6 (85) |
| programs | 313,600 | 14,844 | 8.01 | 29.2 (82) |
| sysex | 374,400 | 4,164 | 4.38 | 28.0 (83) |

- **Clocks/slot:** measured in the bench from `cpu_done` to `slot_done`, with the bench's
  memory latency of 1-40 clocks (mean about 20) per read. Real time allows
  `f_clk / 1,024,000` clocks a slot for the LA32 and the CPU turn together: 97.7 at 100 MHz,
  48.8 at 50 MHz.
- **Bench check:** changing one logsin table entry makes the bench stop at the first slot that
  uses it.

**Standalone Quartus 17 build** (`sim/sound/syn/winbuild_sound.sh`, 5CSEBA6U23I7, all pins
virtual):
- Resources: 5,202 ALMs, 7,134 registers, 64 M10K (522,752 bits: reverb ROM and RAM, LA32
  tables) and 20 DSP blocks.
- Timing at 100 MHz: setup slack +0.111 ns (slow 100C) and -0.554 ns (slow -40C), so about
  95 MHz at the worst corner; hold met.
- The remaining paths are spread out: the mix's 64-bit reciprocal products, the interpolation
  product, and the envelope sums.
