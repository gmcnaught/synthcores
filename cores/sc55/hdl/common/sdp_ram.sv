// Simple dual-port RAM (one write port, one registered read port), written
// so that Quartus maps it to block RAM. rd = mem[ra] of the previous clock;
// a read of the address written in the same clock returns the old data.
// INIT: optional $readmemh file.
module sdp_ram #(
    parameter int AW = 8,
    parameter int DW = 8,
    parameter INIT = ""
) (
    input  logic          clk,
    input  logic          we,
    input  logic [AW-1:0] wa,
    input  logic [DW-1:0] wd,
    input  logic [AW-1:0] ra,
    output logic [DW-1:0] rd
);
    logic [DW-1:0] mem [2**AW];
    initial if (INIT != "") $readmemh(INIT, mem);
    always_ff @(posedge clk) begin
        if (we) mem[wa] <= wd;
        rd <= mem[ra];
    end
endmodule
