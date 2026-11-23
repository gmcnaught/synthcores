# Proteus 2000 core: CPU plan (TG68K-based)

Decision (2026-11-21): base the P2K CPU on a 68k FPGA core, not the ColdFire V1 IP (license and
availability unknown, see p2k-sources.md). fx68k (used by the Proteus/1 core) is a 68000 and cannot
run OS 2.26; the base is **TG68K.C in 68020 mode** (as used by MiSTer's Minimig), extended to the
MCF5206e (ColdFire V2, ISA_A) behaviour the OS relies on. Tags: [O] observed in OS 2.26 / MAME,
[I] inferred, [U] unknown.

## 1. Instruction set
- OS 2.26 uses 68020-style integer instructions [O, listing counts]: `extb.l` 1220, `muls.w/mulu.w`
  635, `muls.l` 113, `divs.w/divu.w` 320, `remu.l` 1 (= 68020 `divul.l` encoding with Dr != Dq;
  ColdFire stores only the remainder [I: check the one use]), `link.w`, `movem.l`, `swap`,
  `addx/negx`, bit ops, `scc`. No bit-field, BCD, `cas`, `chk2`, `pack`, `moves` use.
- ColdFire system instructions [O]: `movec` 8 (VBR 0x801, CACR 0x002, RAMBAR 0xC04, MBAR 0xC0F),
  `cpushl` 4, `halt` 1, `pulse` 1. TG68K needs: `movec` for CACR/RAMBAR/MBAR (latch the values;
  MBAR moves the peripheral block, RAMBAR the on-chip SRAM), `cpushl`/`pulse` as NOPs, `halt` as
  stop-until-debug [I: only in a fatal path].
- `mac.l`/`msac.l` in the listing are data (filter tables at 0x010205DC, 0xAAAA fill); no
  invalid-instruction exception in MAME runs [O]. No MAC/EMAC unit needed [I].
- ColdFire forbids some 68020 forms the OS never uses; executing the 68020 superset is harmless [I].

## 2. Exceptions (must change in TG68K)
- **ColdFire exception frame** [O]: 8 bytes, first long = format (4 + SP misalignment) << 28 |
  fault status | vector << 18 | SR, second long = PC; SP is aligned to 4 before stacking. The RTOS
  trap #0 handler (0x0100BD00) reads the format nibble (`move.b (sp),d0; asr.l #4; and.l #3`) to
  recover the caller's stack, so the 68020 frame layout (SR, PC, format/offset) will not work.
- `rte` must pop the ColdFire frame and restore the SP misalignment.
- Vectors [O]: VBR = 0x01000000 (DRAM copy of the OS); traps #0 (RTOS), #1, #3, #4, #5, #7, #10,
  #11, #15 used; interrupts on vectors 25-31 (autovector levels 1-7) with handlers at 0x0100083x-
  0x01000880; illegal/address/access/privilege/trace/line-A/line-F go to fatal reporters.
- Supervisor-only system (no user mode switching observed) [I].

## 3. On-chip peripherals to implement (MBAR = 0x20000000)
Accesses counted over boot + demo playback in MAME [O]:

| Block | Offsets used | Use |
|---|---|---|
| SIM | 0x01C-0x020 (ICR), 0x036 IMR (r/w 18), 0x041 SYPCR, 0x046 PAR | interrupt levels/masking, watchdog off, pin assignment |
| DRAM controller | 0x04A-0x063 | configuration only (DRAM is an SDRAM-backed array in the core) |
| Chip selects 0-7 | 0x064-0x0B6, CSCR0 bit 7 read by the boot block | address decode is fixed glue; registers latch values; CSCR0 reset 0x3C1F |
| Timer 1 | 0x100-0x111 (TER 0x111 written ~26 000 times) | OS tick (TRR 0x80E8, TMR 0x001B) |
| Timer 2 | 0x120-0x131, TCN2 0x12C read | timing measurements |
| UART1 | 0x140-0x15C | MIDI In A / Out, 31250 baud (UBG = 33) |
| UART2 | 0x180-0x19C | MIDI In B |
| Parallel port | 0x1C5 PPDDR, 0x1C9 PPDAT (~218 000 writes) | Xilinx configuration bit-bang at boot (ignore), other bits [U] |
| 0x20C, 0x24C | r 3 / w 3 | DMA channel 0/1 byte count (BCR) only; no DMA control writes seen [I: unused] |
| SRAM | RAMBAR = 0x0115BA15 [O: initial SP 0x0115BA20 rounded up to 512, minus 512, valid bits 0x15] | on-chip SRAM at 0x0115BA00, inside the DRAM range just below the initial stack; mapping it to DRAM behaves the same [I] |

## 4. System glue (from the MAME machine, docs/p2k-findings.md)
- Memory map: flash 2 MB at 0 and 0x02000000 (28F160, user data writes), DRAM 8 MB at 0x01000000,
  LCD 0x400000/0x408000, H chips 0x500000, G05 0x600000, G20 0x77F000/0x780000, panel 0x800000,
  LEDs 0x900000/0xA00000, latch 0xB00000 (bit 6 Baby select), knob ADC 0xC00000, board ID 0xF00000,
  open bus 0x50000000.
- Clock: 33 MHz [O: UART baud computation]. TG68K at the same nominal speed is enough; the OS has
  no cycle-exact timing loops seen so far [I], audio timing comes from the sound chips.
- Boot: the 1.24 boot block plus the OS 2.26 overlay and its start-mode patch (or a flash image
  committed by the OS itself), the panel bit-7 workaround while the boot block runs.

## 5. Verification path
1. Run TG68K-with-ColdFire-changes in a Verilator testbench against the MAME machine: same flash
   image, compare bus traces (addresses, data) from reset through the boot block and OS start.
2. MAME's `P2K_TRACE` logs (G20/H/G05 writes with time and PC) are the reference for the sound-chip
   interface; `tools/p2k_diff.py` gives per-parameter expected writes.
3. Sound chips (G20 voices, H filter, G05 effects) follow the MAME models (`emup2k.cpp`,
   `emup2k_fx.h`, replaced 2026-11-23 by the integer model `p2k/model/p2k_sound.h`); their approximations are listed in docs/p2k-findings.md.

## 6. Status (2026-11-21)
- Tooling: TG68K.C vendored in `p2k/rtl/cpu/tg68k/` (LGPL-3, changes marked `-- P2K:`), converted
  to Verilog by GHDL synth (container `hdlc/ghdl:yosys`) and simulated with Verilator
  (`p2k/sim/build_cpu.sh`, `p2k/sim/p2k_cpu_tb.cpp`, ~0.8 M cycles/s). NVC runs the VHDL directly
  (`p2k/sim/tb_p2k_cpu.vhd`) but at ~1 k cycles/s.
- Reference: MAME `P2K_BUSTRACE` (binary data-access trace; `P2K_BUSTRACE_NODRAM=1` leaves out
  DRAM and flash opcode fetches), `tools/p2k_flash_image.py` (the flash image MAME boots, byte-
  identical below the user area), `tools/p2k_trace_cmp.py` (first divergence).
- CPU changes done: generic `ColdFire`; MOVEC ACR0/1, RAMBAR0/1, MBAR (MBAR/RAMBAR outputs);
  ColdFire exception frame (PC, SR, then format/vector word 0x4000 | vector << 2) and RTE (skip
  the format word, then SR, PC); no format-#2 frames. SP misalignment adjustment not done [I: the
  OS keeps SP long-aligned].
- Peripherals: `p2k/rtl/cpu/mcf5206e_periph.sv` (interrupt controller, timers 1/2, UART1/2 with
  the TxEMT-on-disable behaviour, parallel port with input pins, latch-only registers) and
  `p2k/rtl/cpu/p2k_cpu.sv` (kernel + MBAR decode, autovectored IPL). The testbench honours the
  kernel's `skipFetch` (no bus cycle for the read half of CLR on 68010+, as the TG68K wrapper).
- Verified: the first 5.2 M data word accesses of the boot (25.6 M MAME records, boot block and
  DRAM tests) match MAME exactly.
- Verified (2026-11-22, iteration 11): reset to the OS 2.26 main screen. Replay mode
  (`p2k/sim/replay.sh`, `P2K_TB_REPLAY`) matches all 5,175,832 non-DRAM word accesses of a
  12 s MAME `P2K_BUSTRACE_NODRAM` trace (demo SIMM; MAME main screen at 10.5 s), including
  6,504 timer interrupts and 35,006 TRAP #0 entries; `tools/p2k_trace_cmp.py` agrees on the
  saved RTL trace. The RTL LCD then shows `C02A Vol127 Pan00  P2Ksy`. Replay mechanics:
    - MAME patch 0003 keeps vector-table reads in the NODRAM trace (exception entries).
    - Interrupts: the SIM's level reaches the kernel only when MAME's next access is an interrupt
      vector and the kernel would stack MAME's interrupted PC (TG68_PC in a fetch state, else
      last_opc_pc; P2K debug outputs `PC_out`, `opcPC_out`, `dbg_out`); a timer at that level
      then counts as at its reference (`warp_level`). MAME's interrupt points depend on its
      cycle counts, which do not model the MCF5206e, so the RTL takes them where MAME did.
    - LCD status and flash status/ID reads return MAME's values (its HD44780 and intelfsh
      models are busy for emulated time; the poll counts decide where ticks land).
    - Testbench flash: the LH28F160S3 command subset of MAME's intelfsh16 (IDs 0xB0/0xD0).
- RTL change found by the replay: the interrupt-acknowledge cycle (FC 7, 0xFFFFFFF0 | level << 1)
  no longer reaches the board bus; the SIM answers it (autovector).
- ColdFire-only opcodes: every `cpushl` (F4xx) and `pulse` (4ACC) in the OS 2.26 and boot-block
  disassembly sits inside data tables; `halt` (4AC8) occurs once, at boot 0x020005E4 after a
  fatal-error call that does not return. [I] None is executed on a normal boot, so TG68K keeps
  its 68020 decode (TAS An → illegal-instruction trap) for these.
- Peripheral timing: `ce_sys` (one pulse per 33 MHz system clock) drives the timers and UART baud
  generators; the testbench runs the CPU at `P2K_TB_RATIO` (default 4) clocks per system clock.
