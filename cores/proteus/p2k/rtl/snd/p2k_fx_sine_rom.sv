// SPDX-License-Identifier: GPL-3.0-or-later
// The chorus quarter-wave sine table (p2k_fx_sine.svh) as a block-RAM ROM with a registered read.
module p2k_fx_sine_rom (
    input  logic        clk,
    input  logic [10:0] a,
    output logic [15:0] q
);
    `include "p2k_fx_sine.svh"
    logic [15:0] rom [0:2047];
    initial
        for (int i = 0; i < 2048; i++)
            rom[i] = fx_qsine(11'(i));
    always_ff @(posedge clk) q <= rom[a];
endmodule
