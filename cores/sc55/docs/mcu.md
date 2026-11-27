# MCU system (`modules/roland/hdl/mcu/sc55_mcu.sv`, M2)

The H8/532 CPU (`docs/cpu.md`) with the mk1's memories, on-chip peripherals
and gate array, sequenced like the reference model's `MCU_Step`:

```
run_step -> CPU part (h8_cpu) -> cycles += 12 -> pcm_tick (PCM_Update, external)
         -> TIMER_Clock -> UART RX/TX -> MCU_UpdateAnalog -> gate-array LCD counter -> step_done
```

## Memory map (mk1, `MCU_Read`/`MCU_Write`) [OBS mcu.cpp]

| Page | Address | Read | Write |
|---|---|---|---|
| 0 | 0000–7FFF | ROM1 (32 KB) | ignored |
| 0 | 8000–DFFF | SRAM (32 KB) | SRAM |
| 0 | E000–E03F | PCM chip | PCM chip |
| 0 | F000–F0FF | buttons, active low by `io_sd` = address low byte; sets `io_sd`, LCD enable = bit 3 | sets `io_sd` |
| 0 | F104 / F105 | — | LCD data / command; gate-array LCD counter = 500 |
| 0 | F106 | gate-array interrupt source; clears it and IRQ1 | — |
| 0 | F107 | — | `io_sd` |
| 0 | FB80–FF7F | on-chip RAM (1 KB) when RAMCR bit 7 | same |
| 0 | FF80–FFFF | device registers | device registers |
| 1–4, 8, 9, 14, 15 | | ROM2 at `address & 0x3ffff` (256 KB) | ignored |
| 5 | | SRAM | SRAM |
| 6, 7 | | 0x00 | ignored |
| 10–13 | | 0xFF | ignored |

Other page-0 reads return 0xFF.

## Peripherals [OBS mcu.cpp, mcu_timer.cpp]

* **FRT 1–3** and the **8-bit timer** run on `cycles / 2`; each has a
  deadline and a stride (mk1 tables FRT {4, 8, 32, 4}, TMR {0, 8, 64, 1024,
  0, 4, 4, 4}). A TCR write sets the deadline to the next multiple of the
  stride. Flags clear only when the CPU read them set first (`status_rd`).
  The high-byte temporary register is shared by all FRTs.
* **SCI**: receive takes a byte from the MIDI buffer when RX is enabled,
  RDRF is clear and `cycles >= uart_rx_delay`; clearing RDRF sets the
  delay to `cycles + 3000`. An SSR write overwrites the whole register
  after the flag logic (reference behaviour). Transmit (mk1, SC-155,
  JV-880) is double-buffered as on the H8/532 (`ref/patch_ref.py` FIXES):
  TDRE sets again as soon as the shift register is free, the 3000-cycle
  byte time starts then, and TXI is requested while TIE and TDRE are both
  1, including when an SCR write sets TIE (synthcores #52).
* **A/D**: a conversion ends 200 cycles after it starts; scan mode repeats.
  mk1 pin values: channel 7 = battery 0x2A0, all others 0 (RCU path).
* **Gate array**: 500 steps after an LCD write it raises source 1 on IRQ1;
  reading F106 returns and clears the source.
* **IRQ0** comes from the PCM chip (`pcm_irq0`).

## Verification (`sim/mcu/mcu_lockstep.cpp`, `tests/test_mcu_lockstep.py`)

The HDL serves its own memory and devices; the bench supplies only the PCM
chip's register-read data and IRQ0 level (from the model) and feeds the
same MIDI bytes and button presses to both. Every CPU bus access is compared
(direction, address, read and write data), and after every step the CPU
state, the interrupt requests, device registers 00–7F (outside the timer
ranges), FRT/TMR state, the SCI, A/D and gate-array state and `cycles`.
SRAM and on-chip RAM are compared at the end.

| Run (mk1-v1.21) | Result |
|---|---|
| boot, 20 M steps | identical |
| GM reset + random MIDI (14,700 bytes) + random button presses (not POWER), 20 M steps | identical; 94,717 PCM register accesses |
| 14 seeded faults (`sim/mutate.py ROMDIR mcu`) | 12 caught; the 2 missed are configurations the mk1 firmware never uses (FRT clock select 1; RAMCR bit 7 clear) |

Found by the bench: an A/D scan written as a loop with static locals feeding
non-blocking array writes left the channel registers unwritten in Verilator;
the scan is now written out per channel.
