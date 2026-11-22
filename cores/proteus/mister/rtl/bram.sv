// Simple dual-port block RAM: one write port, one registered read port (infers M10K).
module bram #(parameter AW = 10, parameter DW = 8)
(
	input               clk,
	input               we,
	input      [AW-1:0] waddr,
	input      [DW-1:0] wdata,
	input      [AW-1:0] raddr,
	output reg [DW-1:0] rdata
);
reg [DW-1:0] mem [0:(1<<AW)-1];
always @(posedge clk) begin
	if (we) mem[waddr] <= wdata;
	rdata <= mem[raddr];
end
endmodule
