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

`mt32_la32` stages (one clock each; the pow and logsin tables are dual-port ROMs with
registered reads):

| Stage | Work |
|---|---|
| IDLE | `slot_go`: the `w46` envelope-run shift, register file and `ram2` reads, pow lookups for both envelopes |
| TV | envelope sums, `ram2[0..1]` written, interrupt (envelope done); pow lookup for the pitch |
| WG | phase, `ram2[2]`; PCM: wave addresses, end interrupts; synthesis: the filter cutoff and resonance chain, pow and logsin lookups |
| PREQ1, PREQ2, PWAIT | PCM: up to two 64-bit reads (none for an address at or above 0x80000) |
| W3, W4 | PCM: sample decode, pow lookups, interpolation or the square/saw combine, ring modulation |
| N3-N6 | synthesis: products `m2`-`m5`, logsin lookups, the resonance and wave-shape sums, pow lookups, combine, ring modulation |
| ACC | pan, accumulators, `reg_file[5]` counter update, `w186`-`w188` latch; `slot_done` |

## Results

See the end of this file (filled in from the gate run).
