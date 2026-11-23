// SPDX-License-Identifier: MIT
// license:BSD-3-Clause
// uPD933 memories: inferred ROM (from a $readmemh file) and simple dual-port RAM.
// Both have a registered read: the address is sampled at a clock edge and the data is valid
// after it. Vendor-neutral; Quartus infers M10K (or MLAB for the small RAMs).

module upd933_rom #(
	parameter int AW = 11,
	parameter int DW = 12,
	parameter INIT = "upd933_cosine.hex"
) (
	input  logic          clk,
	input  logic [AW-1:0] addr,
	output logic [DW-1:0] q
);
	logic [DW-1:0] mem [0:(1<<AW)-1];
	initial $readmemh(INIT, mem);
	always_ff @(posedge clk) q <= mem[addr];
endmodule

module upd933_ram #(
	parameter int AW = 3,
	parameter int DW = 72
) (
	input  logic          clk,
	input  logic          we,
	input  logic [AW-1:0] wa,
	input  logic [DW-1:0] wd,
	input  logic [AW-1:0] ra,
	output logic [DW-1:0] q
);
	logic [DW-1:0] mem [0:(1<<AW)-1];
	initial for (int i = 0; i < (1<<AW); i++) mem[i] = '0;
	always_ff @(posedge clk) begin
		if (we) mem[wa] <= wd;
		q <= mem[ra];
	end
endmodule
