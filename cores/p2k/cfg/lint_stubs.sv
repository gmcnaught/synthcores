// SPDX-License-Identifier: GPL-3.0-or-later
// Lint-only port list of TG68KdotC_Kernel (VHDL, cores/proteus/p2k/rtl/cpu/tg68k): Verilator
// reads no VHDL, so modules/synthframe/bin/hdl-lint lints P2K with this empty stand-in. The
// simulation benches convert the real kernel with GHDL (sim/build.sh). Never built.
/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNDRIVEN */
module TG68KdotC_Kernel (
    input  wire        clk,
    input  wire        nReset,
    input  wire        clkena_in,
    input  wire [15:0] data_in,
    input  wire [2:0]  IPL,
    input  wire        IPL_autovector,
    input  wire        berr,
    input  wire [1:0]  CPU,
    output wire [31:0] addr_out,
    output wire [15:0] data_write,
    output wire        nWr,
    output wire        nUDS,
    output wire        nLDS,
    output wire [1:0]  busstate,
    output wire        longword,
    output wire        nResetOut,
    output wire [2:0]  FC,
    output wire        clr_berr,
    output wire        skipFetch,
    output wire [31:0] regin_out,
    output wire [3:0]  CACR_out,
    output wire [31:0] VBR_out,
    output wire [31:0] MBAR_out,
    output wire [31:0] RAMBAR_out,
    output wire [31:0] PC_out,
    output wire [31:0] opcPC_out,
    output wire [7:0]  dbg_out,
    output wire        endOPC_out,
    output wire        interrupt_out
);
endmodule
/* verilator lint_on UNDRIVEN */
/* verilator lint_on UNUSEDSIGNAL */
