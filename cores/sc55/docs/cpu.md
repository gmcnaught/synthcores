# H8/532 CPU (`hdl/cpu/h8_cpu.sv`, M1)

A behavioural H8/500-family CPU that does what the reference model's MCU
does, bus access for bus access: `mcu_opcodes.cpp` (instructions),
`mcu_interrupt.cpp` (exceptions, TRAPA, interrupts) and the CPU part of
`MCU_Step` in `mcu.cpp`.

## Interface

| Port | Use |
|---|---|
| `step` → `step_done` | one `MCU_Step` CPU part: interrupt check (skipped once after `ex_ignore`), one instruction unless asleep, trace check. The system advances the devices by 12 reference cycles between steps. |
| `bus_req/we/addr[23:0]/wdata` ↔ `bus_rdata/bus_ack` | byte accesses in the model's order. `addr` = page << 16 \| offset, as `MCU_GetAddress`. 16-bit accesses are two bytes at `addr & ~1` (high first), vectors four bytes. `bus_req` is held until a one-cycle `bus_ack`. |
| `int_pending[20:0]`, `ipra..iprd`, `p1cr` | `MCU_Interrupt_Source` request bits and the registers `MCU_Interrupt_GetVL` reads |
| `reset_start` | `MCU_Reset`: clear registers, load cp/pc from the vector at 0 |
| `ld_en`, `ld_*` | load all registers (lockstep reseeding; later savestates) |
| `traps` | count of `MCU_ErrorTrap` calls |

## Model behaviour kept on purpose [OBS, reference source]

* Stack accesses (`MCU_PushStack/PopStack`) use page 0, not TP; an odd R7
  sets a pending address error and the access still happens at R7 & ~1.
* `MOV:F` (0x80–0x9F): with bit 3 set the model reads a word and ORs it into
  the register with the high byte kept (`r = (r & 0xff00) | data16`), flags
  from the byte; with bit 3 clear it reads a byte, zero-extends it, flags as
  a word. Stores mirror this. No odd-address check.
* `EXTS` sets N and Z from the register before extension.
* `RTE` restores SR without `sr_mask`.
* `SUBX` sets Z from the result alone; `ADDX` keeps Z clear once clear.
* Error-trap paths continue: `ADDQ` with an invalid count adds 0 and writes
  back; `RTD #16` pops PC then traps; invalid control registers read 0;
  `DIVXU` by zero sets N=0 Z=1 V=0 C=0 and leaves the registers.
* Interrupt choice: TRAPA (lowest number) first, then a pending exception,
  then NMI, then the **first pending source in enum order** whose level
  beats the mask (not the highest level). FRT input-capture sources have no
  vector.
* An interrupt entry and the handler's first instruction happen in the
  same step.

## Verification (`sim/cpu/cpu_lockstep.cpp`, `tests/test_cpu_lockstep.py`)

The bench links the hooked reference backend (`docs/reference.md`). Per
step it records the model's bus accesses, then runs the HDL step: every
HDL access must equal the next recorded one (direction, address, write
data) and reads return the recorded data. After the step R0–R7, PC, SR,
CP/DP/EP/TP/BR, sleep, ex_ignore, the pending exception, the TRAPA requests
and the error-trap count must match.

| Run | Result (2026-11-19) |
|---|---|
| mk1-v1.21 firmware from reset, 30 M steps (18 s of SC-55 time) | identical; 2,683 distinct PCs, 71,295 interrupt entries |
| firmware + GM reset + random MIDI traffic (notes, programs, CCs, bend, GS SysEx), 15 M steps | identical; 9,459 distinct PCs |
| random code (instruction-shaped bytes, state reseeded every 50 steps), 4 seeds × 500 k steps | identical; 3,842/4,608 general-format mode × size × opcode-byte combinations run, every opcode group × mode × size at least once; 70–90 k error traps per run |
| 18 seeded faults (`sim/cpu/mutate.py`): carry, overflow, flag rules, addressing, page registers, interrupt priority, trace, model quirks, stack page, divide limit, odd-address exception | all 18 caught |

Speed in the bench (one-cycle memory): 8–15 clocks per step on firmware,
about 23 on random code. The real-time budget at 20 MHz reference cycles is
600 ns per step (for example 28.8 clocks at 48 MHz), shared with the memory
latency of the board; see `docs/plan.md` R2.
