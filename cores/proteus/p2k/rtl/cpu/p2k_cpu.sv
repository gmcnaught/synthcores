// Proteus 2000 CPU subsystem: TG68K kernel (68020 mode, ColdFire changes, see docs/p2k-core-cpu.md)
// + MCF5206e peripherals at MBAR. External bus: one 16-bit access per cycle when ext_req is high
// (busstate 00 fetch, 10 read, 11 write; the board must return ext_din combinationally).
module p2k_cpu (
	input  logic        clk,
	input  logic        reset_n,
	output logic [31:0] addr,
	output logic [1:0]  busstate,     // 00 fetch, 10 read, 11 write, 01 idle
	output logic        ext_req,      // access goes to the board (not MBAR, not skipped)
	output logic        uds, lds,
	output logic [15:0] dout,
	input  logic [15:0] ext_din,
	input  logic        u1_rxd, u2_rxd,
	output logic        u1_txd, u2_txd,
	input  logic [7:0]  pp_in,
	output logic [7:0]  pp_out, pp_ddr,
	output logic [31:0] mbar_out
);
	logic [15:0] din, per_dout;
	logic nwr, nuds, nlds, skip;
	logic [2:0] ipl;
	logic [31:0] rambar;

	TG68KdotC_Kernel cpu (
		.clk(clk), .nReset(reset_n), .clkena_in(1'b1), .data_in(din), .IPL(~ipl), .IPL_autovector(1'b1),
		.berr(1'b0), .CPU(2'b11), .addr_out(addr), .data_write(dout), .nWr(nwr), .nUDS(nuds), .nLDS(nlds),
		.busstate(busstate), .longword(), .nResetOut(), .FC(), .clr_berr(), .skipFetch(skip), .regin_out(),
		.CACR_out(), .VBR_out(), .MBAR_out(mbar_out), .RAMBAR_out(rambar)
	);
	assign uds = ~nuds;
	assign lds = ~nlds;

	// MBAR window (valid bit 0, base 31:10)
	logic per_sel;
	assign per_sel = busstate != 2'b01 && !skip && mbar_out[0] && addr[31:10] == mbar_out[31:10];
	assign ext_req = busstate != 2'b01 && !skip && !per_sel;
	assign din = per_sel ? per_dout : ext_din;

	mcf5206e_periph per (
		.clk(clk), .reset(!reset_n), .sel(per_sel), .wr(busstate == 2'b11), .offs({addr[9:1], 1'b0}),
		.uds(uds), .lds(lds), .din(dout), .dout(per_dout), .ipl(ipl),
		.u1_rxd(u1_rxd), .u2_rxd(u2_rxd), .u1_txd(u1_txd), .u2_txd(u2_txd),
		.pp_in(pp_in), .pp_out(pp_out), .pp_ddr(pp_ddr)
	);
endmodule
