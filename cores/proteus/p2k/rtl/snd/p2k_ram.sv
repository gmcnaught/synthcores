// SPDX-License-Identifier: GPL-3.0-or-later
// Simple dual-port RAM with a registered read (M10K or MLAB). The users never read and write one
// word in the same clock; the Verilator build checks that.
module p2k_ram #(
    parameter int W = 16,
    parameter int AW = 8
) (
    input  logic          clk,
    input  logic          re,
    input  logic [AW-1:0] raddr,
    output logic [W-1:0]  rdata,
    input  logic          we,
    input  logic [AW-1:0] waddr,
    input  logic [W-1:0]  wdata
);
    (* ramstyle = "no_rw_check" *) logic [W-1:0] mem [0:(1<<AW)-1];

    always_ff @(posedge clk) begin
        if (we) mem[waddr] <= wdata;
        if (re) rdata <= mem[raddr];
    end

`ifdef VERILATOR
    always_ff @(posedge clk)
        if (re && we && raddr == waddr)
            $fatal(1, "p2k_ram %m: read and write of word %0d in one clock", raddr);
`endif
endmodule
