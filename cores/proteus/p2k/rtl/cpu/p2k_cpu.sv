// Proteus 2000 CPU subsystem: TG68K kernel (68020 mode, ColdFire changes, see docs/p2k-core-cpu.md)
// + MCF5206e peripherals at MBAR. External bus: one 16-bit access per cycle when ext_req is high
// (busstate 00 fetch, 10 read, 11 write; the board must return ext_din combinationally).
module p2k_cpu (
	input  logic        clk,
	input  logic        ce_sys,       // MCF5206e system-clock enable for the peripherals
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
	output logic [31:0] mbar_out,
	// Testbench controls (tie cpu_ena = 1, ipl_mask = 0, warp_level = 0 in a build): cpu_ena = 0 holds the kernel
	// while the peripherals keep running; ipl_mask = 1 hides pending interrupts from the kernel.
	input  logic        cpu_ena,
	input  logic        ipl_mask,
	input  logic [2:0]  warp_level,   // see mcf5206e_periph (tie to 0)
	output logic [2:0]  ipl_raw,      // interrupt level from the SIM before ipl_mask
	output logic [31:0] vbr_out,
	output logic [31:0] pc_out,       // kernel TG68_PC
	output logic [31:0] opc_pc,       // kernel last_opc_pc (address of the prefetched opcode)
	output logic [7:0]  kdbg,         // kernel state bits (see TG68KdotC_Kernel dbg_out)
	output logic        endopc,       // instruction boundary this clock
	output logic        irq_take      // interrupt accepted at this boundary
);
	logic [15:0] din, per_dout;
	logic nwr, nuds, nlds, skip;
	logic [2:0] ipl, ipl_k;
	logic [31:0] rambar;
	logic [2:0]  fc;

	TG68KdotC_Kernel cpu (
		.clk(clk), .nReset(reset_n), .clkena_in(cpu_ena), .data_in(din), .IPL(~ipl_k), .IPL_autovector(1'b1),
		.berr(1'b0), .CPU(2'b11), .addr_out(addr), .data_write(dout), .nWr(nwr), .nUDS(nuds), .nLDS(nlds),
		.busstate(busstate), .longword(), .nResetOut(), .FC(fc), .clr_berr(), .skipFetch(skip), .regin_out(),
		.CACR_out(), .VBR_out(vbr_out), .MBAR_out(mbar_out), .RAMBAR_out(rambar),
		.PC_out(pc_out), .opcPC_out(opc_pc), .dbg_out(kdbg), .endOPC_out(endopc), .interrupt_out(irq_take)
	);
	assign ipl_k = ipl_mask ? 3'd0 : ipl;
	assign ipl_raw = ipl;
	assign uds = ~nuds;
	assign lds = ~nlds;

	// MBAR window (valid bit 0, base 31:10)
	logic per_sel, iack;
	assign per_sel = busstate != 2'b01 && !skip && mbar_out[0] && addr[31:10] == mbar_out[31:10];
	// Interrupt acknowledge (CPU space, FC 7, address 0xFFFFFFF0 | level << 1): the MCF5206e SIM
	// answers it internally (autovector), so it never reaches the board bus.
	assign iack = fc == 3'b111 && busstate == 2'b10;
	assign ext_req = cpu_ena && busstate != 2'b01 && !skip && !per_sel && !iack;
	assign din = per_sel ? per_dout : ext_din;

	mcf5206e_periph per (
		.clk(clk), .ce_sys(ce_sys), .reset(!reset_n), .sel(per_sel && cpu_ena), .wr(busstate == 2'b11), .offs({addr[9:1], 1'b0}),
		.uds(uds), .lds(lds), .din(dout), .dout(per_dout), .ipl(ipl), .warp_level(warp_level),
		.u1_rxd(u1_rxd), .u2_rxd(u2_rxd), .u1_txd(u1_txd), .u2_txd(u2_txd),
		.pp_in(pp_in), .pp_out(pp_out), .pp_ddr(pp_ddr)
	);
endmodule
