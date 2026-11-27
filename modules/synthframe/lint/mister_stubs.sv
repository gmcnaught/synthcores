// SPDX-License-Identifier: MIT
// Lint-only stand-ins for the MiSTer modules that have no Verilog source: the Quartus PLL
// megafunction (each core's mister/rtl/pll). Used by bin/hdl-lint, never built.
/* verilator lint_off UNUSEDSIGNAL */
module pll (
    input  wire refclk,
    input  wire rst,
    output wire outclk_0,
    output wire outclk_1,
    output wire outclk_2,
    output wire locked
);
    assign outclk_0 = refclk;
    assign outclk_1 = refclk;
    assign outclk_2 = refclk;
    assign locked = 1'b1;
endmodule
/* verilator lint_on UNUSEDSIGNAL */
