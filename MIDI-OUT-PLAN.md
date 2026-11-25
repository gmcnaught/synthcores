# MIDI out for the SC-55, JV-880 and MT-32

Goal: each core sends what its firmware transmits to `UART_TXD` (MidiLink), checked byte for byte
and cycle for cycle against the same reference model as the rest of the core.

## 1. Does the reference model the transmit line? (checked 2026-11-24)

Neither reference emits TX bytes. Both store the byte and model the "transmit done" timing; the
output call is commented out:

| Reference | Byte written to | TX-done timing in the model | Output |
|---|---|---|---|
| Nuked-SC55, mk1 / JV-880 / SC-155 (main MCU SCI) | `TDR` (dev 0x5b), sent when firmware clears `SSR.TDRE` | fixed 3000 MCU cycles (`mcu.cpp` `uart_tx_delay`) | `// fprintf(stderr, "tx:%x\n", ...)` in `MCU_UpdateUART_TX` ("dummy TX") |
| Nuked-SC55, mk2 / SC-155mk2 (M37450 sub-MCU) | sub-MCU dev register 0x08 (`SM_DEV_UART2_DATA`) | none: the TX interrupt requests (INT_REQUEST bits 0x4/0x2/0x1) are never set | none; the write only lands in `device_mode[]` |
| Nuked-MT32 (MCS-96) | `SBUF` (SFR 0x07) → `serial_send` | fixed 9600 CPU cycles (`serial_send_timer`) | `//m_serial_tx_cb(serial_send_buf)` |

The HDL already copies this behaviour, and the lockstep benches already compare it:
`mcu_lockstep.cpp` compares every `dev[]` register (TDR, SSR, SCR), `uart_tx_delay`, `ssr_rd` and
the sub-MCU's 32 `dm[]` registers each step; `mcs96_periph.sv` reproduces the 9600-cycle
`serial_timer`. What is missing is a recorded byte stream on both sides and a cycle-exact
comparison of it.

### Probe: what the firmware transmits in the reference

Patched copies of the references (scratchpad only, not committed) logged every write to the
transmit registers while an RQ1 data request was sent:

| Romset | Request | Observed TX bytes |
|---|---|---|
| mk1 v1.21 | GS RQ1 at 40 00 00, 40 00 04, 40 01 00 (dev 0x10 and 0x7f); model 0x45 at 10 00 00 | only `FE` (Active Sensing), every 4.8 M MCU cycles from 9.13 M; no DT1 reply |
| mk2 | GS RQ1 40 00 00 size 4 | `FE` every 2.5 M sub-MCU cycles, then the full reply `F0 41 10 42 12 40 00 00 00 04 00 00 3C F7`, one byte per 1584 sub-MCU cycles |
| JV-880 v1.0.1 | RQ1 (model 0x46) at 00 00 00 00, 00 08 20 00, 01 00 00 00; dev 0x10 and 0x11; at 4–11 s | nothing: firmware sets SCR = 0x70 (TE, RE, RIE) at cycle 80.08 M and never writes TDR |
| MT-32 (`Roland MT-32 nuked`) | RQ1 10 00 00 size 0x17; RQ1 03 00 00 size 0x10 | both DT1 replies complete and with correct checksums (`F0 41 10 16 12 10 00 00 …23 bytes… 6D F7`, `F0 41 10 16 12 03 00 00 …16 bytes… 4B F7`), one byte per ~9740 CPU cycles |

Unknown, to check in step 1:

- **mk1 and JV-880 replies.** Whether their firmware answers RQ1 at all (owner's manuals,
  MIDI implementation charts), or needs another request, device ID or panel setting. Until one
  is found, their TX test stimulus is Active Sensing (mk1) or nothing (JV-880).
- **Wire rate against reference rate.** A MIDI byte takes 320 µs on the wire. MT-32: 9600 cycles
  at 12 MHz = 800 µs, so the wire keeps up. Main-MCU SCI: 3000 cycles, about 125 µs at the 24 MHz
  the reference's own comment assumes, so firmware can write ~2.5× faster than the wire. mk2: the
  real-time length of 1584 sub-MCU cycles is not yet known.
- **mk2 connector.** Which physical port sub-MCU UART2 drives on the real mk2 (MIDI OUT or the
  serial/computer port). The reference reads received bytes from the same register.
- **SC-155 / SC-155mk2.** Not probed; expected to follow the mk1 / mk2 paths.

### Step 1 results (SC-55 / JV-880)

- **JV-880 replies after the factory preset.** With an empty NVRAM the firmware ignores RQ1.
  With the factory-preset NVRAM (`jv880-nvram`), it answers the system-area RQ1 (00 00 00 00,
  16 bytes) with one 75-byte DT1 (64 data bytes, valid checksum). The temp-patch RQ1
  (00 08 20 00, in Performance mode) gets no reply.
- **"Byte sent" on the SCI.** The JV-880 firmware relies on the H8/532 SCI's double buffer:
    1. It writes byte A to TDR and clears TDRE.
    2. About 450 cycles later it writes byte B to TDR and clears TDRE again.

  On the chip, TDR moves to the shift register at once and TDRE sets again, so both bytes go
  out. The reference keeps TDRE at 0 for 3000 cycles, so B's clear lands while TDRE reads 0. Tapping
  where the model starts its transmit time (the TDRE 1→0 clear) therefore loses every second
  byte. The byte definition used on both sides is: TDR written, then SSR written with TDRE 0.
  mk1 and the MT-32 are unaffected.
- **JV-880 panel after an RQ1.** After the reply the unpatched reference leaves the LCD at "Now
  bulk se" (cut off mid-word) and stops taking panel keys (SYSTEM, PATCH/PERFORM checked for
  15 s). The CPU keeps running. The cause is the reference's SCI transmitter. It holds TDRE at 0
  for 3000 cycles after each clear, and requests TXI only at the moment TDRE sets. The H8/532's
  SCI is double-buffered, and it requests TXI while TDRE and TIE are both 1. The firmware sends
  from its TXI handler and sets TIE to start a transfer, so it never got the interrupt that ends
  the reply. Fixed for the JV-880 in `ref/patch_ref.py` FIXES and in `sc55_mcu.sv`: the byte
  time starts when TDRE sets, and an SCR write requests TXI = TIE && TDRE. Afterwards the reply
  is the same 75 bytes, and the LCD returns to "Perf … I-01:Syn Lead" about 1 s later, with
  SYSTEM working again (`tests/test_jv880_ref.py` `test_panel_after_rq1`). The SC-55 models keep
  the reference's transmitter (no symptom).
- **mk1:** no RQ1 reply in any variant; Active Sensing only.
- **Clocks:** the boards run the reference at 600 ns a step of 12 cycles (20 MHz). An SCI byte
  is 150 µs of model time against 320 µs on the wire, so the FIFO holds the difference. The
  board benches check that the 512-byte FIFO drops no byte of the 75-byte JV-880 reply.

## 2. Plan

### Step 1: reference TX log (test side only)

- `cores/sc55/ref/patch_ref.py` (traced build): a hook giving `(cycle, byte)`
    - main MCU: at an `SSR` write with TDRE 0 after a TDR write (step 1 results), byte
      = `dev_register[TDR]`;
    - sub-MCU: at the write to dev 0x08, byte = data, cycle = `sm.cycles`.
- `cores/mt32/ref/prepare_src.py`: a `mt32h_tx(byte, cycle)` hook in `i8x9x_device::sbuf_w`. The
  existing `mt32h_midi` hook is the receive side.
- **Gate:** `test_ref_patch.py` and the MT-32 `test_ref.py` still show the hooked builds render the
  same audio as the plain ones.
- Answer the step-1 unknowns (mk1/JV-880 replies, mk2 sub-MCU clock) with the hooked builds and
  record the findings in this file.

### Step 2: HDL tap (no state change)

- `modules/roland/hdl/mcu/sc55_mcu.sv`: outputs `tx_stb`, `tx_byte`
    - mk1 / JV-880: an SSR write with `!d[7]` after a TDR write (`tdr_written`), byte = `dev[7'h5b]`;
    - mk2: on the sub-MCU's write to `dm[8]`.
- `cores/mt32/hdl/mcs96/mcs96_periph.sv`: `tx_stb`, `tx_byte` on the SBUF write (`8'h07`).
- The tap only adds outputs. **Gate:** every existing lockstep and hash test is bit-identical.

### Step 3: cycle-exact comparison in the benches

- SC-55 `mcu_lockstep` / `system_lockstep`, the JV-880 bench and `mt32_sys_bench`: write the HDL
  tap as the same `(cycle, byte)` log as the reference hook, and fail on the first difference.
- New stimulus cases:
    - mk2: GS RQ1 (reply above);
    - mk1: Active Sensing over 10 s;
    - MT-32: the two RQ1 cases, added to `tests/corpus.json`;
    - JV-880: a case only if step 1 finds a request it answers.
- **Gate:** identical byte streams, identical cycles.

### Step 4: `synthframe_midi_tx` (shared, MIT)

- New module `modules/synthframe/hdl/midi/synthframe_midi_tx.sv`, next to
  `synthframe_midi_rx.sv`: byte FIFO, then an 8N1 31,250-baud shifter, with an overflow counter.
- The CPU keeps the reference's TX-done timing. Holding TDRE until the wire is free would change
  CPU timing and break lockstep. The FIFO absorbs the difference.
- FIFO depth: the longest burst seen in step 3 at the main-MCU rate (expected one M10K, 256 or
  512 bytes). A sustained bulk dump longer than that drops bytes; they are counted, not hidden.
- **Gate:** a synthframe unit test decodes the serial line back to the pushed bytes, including a
  burst written faster than the wire, and the overflow count.

### Step 5: MiSTer tops

- Each board exports `midi_tx`. `SC55.sv`, `JV880.sv` and `MT32.sv` replace
  `assign UART_TXD = 1'b1;` with `assign UART_TXD = midi_tx;`, as `CZ101.sv` does.
- File lists through `cfg/files.yaml` and `synthcore`.
- **Gate:** `scripts/build-hosts/winbuild.sh` for all three cores. ALMs and slack are recorded
  against main, and timing is met.

### Step 6: hardware smoke

- Reuse `cores/cz101/scripts/mister/tty_midi_read.py` (reads `/dev/ttyS1` at 31,250 baud).
- Each core's smoke script sends the step-3 request and expects the exact reference reply:
    - SC-55 mk2: the DT1 above;
    - MT-32: both DT1 replies;
    - SC-55 mk1: `FE` bytes.
- **Gate:** smoke passes on the MiSTer (`root@192.168.20.62`), and MIDI in still plays
  (no regression in the existing smoke).

### Order and PRs

| PR | Steps | Contents | Needs the MiSTer |
|---|---|---|---|
| 1 | 1, 2, 3 | Reference hooks, HDL taps, bench comparison | no |
| 2 | 4 | `synthframe_midi_tx` and its test | no |
| 3 | 5, 6 | MT-32 MiSTer top and smoke | yes |
| 4 | 5, 6 | SC-55 MiSTer top and smoke | yes |
| 5 | 5, 6 | JV-880 MiSTer top and smoke | yes |

MT-32 goes first: its reference replies cleanly and the wire is faster than the firmware.

### Not in scope

- MIDI THRU / soft-thru (the reference has none).
- Wire-paced TDRE as an OSD option.
- SC-55 mk2 serial/computer port selection.

## 3. Results (2026-11-24)

| Core | Simulation | Quartus (winbuild, default seed) | MiSTer smoke |
|---|---|---|---|
| SC-55 | `mcu_lockstep`: mk1 Active Sensing, mk2 GS RQ1 → DT1, byte and cycle identical; `board_lockstep`: 4 (mk1) and 202 (mk2) bytes on `uart_txd` = the model's, none dropped; whole suite passes | 33,168 ALMs (main: 32,973); core setup slack +0.868 ns | mk2 pack: `F0 41 10 42 12 40 00 00 00 04 00 00 3C F7` on `/dev/ttyS1` = the model's; Active Sensing every ~21 ms, the model's sub-MCU rate |
| JV-880 | `mcu_lockstep` (factory NVRAM, system RQ1): 75-byte DT1, byte and cycle identical; `jv880_board_bench`: the same 75 bytes on `uart_txd` | 28,703 ALMs; worst setup slack +0.265 ns (HDMI PLL), core +0.267 ns | the 75-byte DT1 = the model's; panel then at "Now bulk s", as the unpatched model (fixed since: see the JV-880 panel finding above) |
| MT-32 | PR #39: `T` lines in the trace, 59 bytes (two DT1s) identical over 11,254,172 trace lines; corpus 7/7 | 18,666 ALMs (main 18,647, same run); core slack +0.466 ns | both DT1s = the model's at 31,250; at 9600 the same, master volume 60 after the smoke's MIDI |

Findings from the hardware runs:

- **MidiLink restarts after each core load.** A core load starts `/sbin/uartmode` loops that
  restart MidiLink every second, and MidiLink then reads the core's output off `/dev/ttyS1`.
  `synth-mister midi-off` stops both.
- **First bytes lost after a speed change.** When `synthframe_midi_rx` switches between 31,250
  and 9600 it drops the first bytes, so the smokes send zero bytes before the first request.
