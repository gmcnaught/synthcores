# PCM chip (`hdl/pcm/sc55_pcm.sv`, M3)

Behavioural model of the SC-55's custom PCM chip, matched to the reference
`pcm.cpp` (`PCM_Write`, `PCM_Read`, `PCM_Update`, `calc_tv`,
`eram_pack/unpack`).

## Structure

One **frame** is one iteration of `PCM_Update`'s loop and lasts
`(reg_slots + 1) * 25` reference cycles. The mk1 firmware sets
`reg_slots = 24` (register 3D = B7) and 3C = F8 (oversampling, noise mask 7,
write mask 15) [OBS, model probe]: 625 cycles per frame at 20 MHz is
32 kHz, two output samples per frame, 64 kHz out.

| State | Model code |
|---|---|
| `P_LOADG` | rows 28–31 of ram1/ram2 into registers for the frame |
| `P_MIX` | final mixing: noise LFSR, accumulators, sample 1 (and 2 with oversampling); `tv_counter`; the `ram2[31][9/10]` block |
| `P_REVERB` (`sc55_pcm_reverb.svh`) | reverb/chorus steps 1–23 and 31: 37 micro-steps with at most one eram access each, read data one step later (single-port RAM pattern) |
| `P_AGEN` | the reverb address generator on row 31 |
| `P_SLOT_*` (`sc55_pcm_agen.svh`, `sc55_pcm_dpcm.svh`) | per voice: address generator and four address steps, five wave-ROM reads (nibble byte, 4 samples), DPCM, 3-tap interpolation (`sc55_pcm_interp.svh`, generated from `pcm.cpp` by `tools/gen_pcm_tables.py`), mk1 filter, IRQ, three envelopes, volume, pan, reverb/chorus sends, key/nibble update |
| `P_FRAME_END` | rows 28–31 written back, `cycles += (reg_slots + 1) * 25` |

Arithmetic follows C `int`: `addclip20` sign-extends both operands from
20 bits and adds in 32 bits (it does not clip), so ram1 keeps 32 bits.

Assumption: `reg_slots <= 28` (voices never overlap the reverb rows 28–31);
the bench stops if the model's value is larger.

## Interfaces

* CPU port: register accesses at E000–E03F. A write to 23 reads the wave ROM
  before acknowledging (`wave_byte_latch`).
* `run`/`target` → `done`: `PCM_Update(cycles)`.
* `irq0`: the IRQ0 request (`irq_assert`), cleared by reading 3E.
* Wave ROM: byte reads at a linear address (bank × 1 MB + offset; banks
  0–2 = waverom1–3 on mk1, others read 0), any latency.
* `sample_valid`, `sample_l/r`: `MCU_PostSample` values.

## Verification (`sim/pcm/pcm_lockstep.cpp`, `tests/test_pcm_lockstep.py`)

The bench runs the full model (firmware and MIDI), replays each step's PCM
register accesses into the HDL (reads must return the model's data), runs
the HDL up to the model's cycle count, and compares every output sample,
the IRQ line, all latches, accumulators, rcsum, config, every ram1/ram2
word after every step, and all of eram every 1000 steps.

| Run (mk1-v1.21) | Result |
|---|---|
| boot, 3 M steps | identical |
| GM reset + random MIDI traffic (8,333 bytes), 9 M steps, ROM latency 3 and 7 | identical: 458,522 samples (324,092 non-zero, peak −14.1 dBFS), 54,997 register accesses |

Speed (bench): up to 745 clocks per 24-voice frame with a 3-cycle ROM, 1,225
with 7 cycles; the real-time budget is set in M5 (`docs/plan.md` R3).
