// MCF5206e on-chip peripherals used by the Proteus 2000 OS (docs/p2k-core-cpu.md), at MBAR:
// SIM interrupt controller (ICR1-15, IMR, IPR), timers 1/2, UART1/2 (68681-style subset),
// parallel port, and latch-only registers for the rest (chip selects, DRAM controller, ...).
// Bus: one 16-bit access per cycle (TG68K kernel), byte lanes as on the 68000 bus (upper = even
// address). Offsets are bytes from MBAR; byte registers at odd offsets use the lower lane.
module mcf5206e_periph #(
	parameter int SYSCLK = 33_000_000
) (
	input  logic        clk,
	input  logic        ce_sys,       // 1 pulse per MCF5206e system clock (timers, UART baud); the
	                                  // CPU (TG68K) runs at a multiple of it
	input  logic        reset,
	input  logic        sel,          // access to MBAR + 0x000..0x3FF this cycle
	input  logic        wr,
	input  logic [9:0]  offs,         // byte offset, even
	input  logic        uds, lds,     // byte lanes (upper = offs, lower = offs + 1)
	input  logic [15:0] din,
	output logic [15:0] dout,
	output logic [2:0]  ipl,          // interrupt level to the CPU (0 = none)
	// Testbench only (tie to 0): a running timer with ORI set whose ICR level equals warp_level
	// reaches its reference now (TER.REF set, TCN restarts), as if its period had just ended.
	input  logic [2:0]  warp_level,
	// UARTs (idle high)
	input  logic        u1_rxd, u2_rxd,
	output logic        u1_txd, u2_txd,
	// parallel port
	input  logic [7:0]  pp_in,
	output logic [7:0]  pp_out, pp_ddr
);
	// ---------------------------------------------------------------- register file (latch-only)
	logic [15:0] regs [0:511];
	initial begin
		for (int i = 0; i < 512; i++) regs[i] = 16'h0000;
		regs[9'h037] = 16'h3c1f;   // CSCR0 (0x06E) reset: AA/PS1/PS0 pins clear
	end

	// ---------------------------------------------------------------- interrupt controller
	logic [7:0]  icr [1:15];          // ICR1..ICR15 at 0x014..0x022
	logic [15:0] imr;                 // 0x036
	logic [15:0] ipr;
	logic t1_irq, t2_irq, u1_irq, u2_irq;
	always_comb begin
		ipr = 16'h0;
		ipr[9]  = t1_irq;
		ipr[10] = t2_irq;
		ipr[12] = u1_irq;
		ipr[13] = u2_irq;
	end
	always_comb begin
		ipl = 3'd0;
		for (int s = 1; s <= 15; s++)
			if (ipr[s] && !imr[s] && icr[s][4:2] > ipl)
				ipl = icr[s][4:2];
	end

	// ---------------------------------------------------------------- timers
	typedef struct packed {
		logic [15:0] tmr, trr, tcr, tcn;
		logic [1:0]  ter;
		logic [7:0]  pre;
	} timer_t;
	timer_t t1, t2;
	// warp (testbench): a running timer with ORI at warp_level counts as at its reference now
	logic t1_warp, t2_warp;
	assign t1_warp = warp_level != 3'd0 && icr[9][4:2] == warp_level && t1.tmr[0] && t1.tmr[4] && ^t1.tmr[2:1];
	assign t2_warp = warp_level != 3'd0 && icr[10][4:2] == warp_level && t2.tmr[0] && t2.tmr[4] && ^t2.tmr[2:1];
	assign t1_irq = (t1.ter[1] || t1_warp) && t1.tmr[4];
	assign t2_irq = (t2.ter[1] || t2_warp) && t2.tmr[4];

	function automatic timer_t tick(timer_t t);
		timer_t r = t;
		logic step;
		if (!t.tmr[0] || t.tmr[2:1] == 2'b00 || t.tmr[2:1] == 2'b11)
			return r;
		step = 1'b0;
		// prescaler: TMR[15:8] + 1 input clocks per count; CLK=10 divides sysclk by 16 first
		if (r.pre == 0) begin
			r.pre = t.tmr[15:8];
			step = 1'b1;
		end else
			r.pre = r.pre - 8'd1;
		if (step) begin
			if (t.tcn == t.trr) begin
				r.ter[1] = 1'b1;
				r.tcn = t.tmr[3] ? 16'd0 : t.tcn + 16'd1;
			end else
				r.tcn = t.tcn + 16'd1;
		end
		return r;
	endfunction

	logic [3:0] div16;

	// ---------------------------------------------------------------- UARTs
	typedef struct packed {
		logic [7:0]  mr1, mr2, csr, cr, imr, ivr, ubg1, ubg2;
		logic        mrptr, rx_en, tx_en;
		logic [7:0]  rxf0, rxf1, rxf2;     // FIFO, rxf0 = oldest
		logic [1:0]  rxn;
		logic        overrun;
		logic [7:0]  thr;
		logic        thr_full, txsr_busy, txemp;
		logic [3:0]  txbit;
		logic [9:0]  txsr;
		logic [15:0] txcnt;
		logic [3:0]  rxbit;
		logic [7:0]  rxsr;
		logic [15:0] rxcnt;
		logic        rxbusy;
		logic        rxd_q;
	} uart_t;
	uart_t ua, ub;

	function automatic logic [7:0] uart_usr(uart_t u);
		// bit 0 RxRDY, 1 FFULL, 2 TxRDY, 3 TxEMP, 4 OE
		return {3'b000, u.overrun, u.txemp & u.tx_en, ~u.thr_full & u.tx_en, u.rxn == 2'd3, u.rxn != 2'd0};
	endfunction
	function automatic logic [7:0] uart_isr(uart_t u);
		logic [7:0] usr = uart_usr(u);
		return {6'b0, u.mr1[6] ? usr[1] : usr[0], usr[2]};
	endfunction
	assign u1_irq = |(uart_isr(ua) & ua.imr);
	assign u2_irq = |(uart_isr(ub) & ub.imr);

	function automatic logic [15:0] uart_period(uart_t u);
		// CSR clock select 0xD = timer (UBG): baud = SYSCLK / (32 * UBG); one bit = 32 * UBG clocks
		logic [20:0] p = {u.ubg1, u.ubg2, 5'd0};
		return p == 0 ? 16'd32 : p > 21'hffff ? 16'hffff : p[15:0];
	endfunction

	function automatic uart_t uart_step(uart_t u, logic rxd, output logic txd);
		uart_t r = u;
		logic [15:0] per = uart_period(u);
		// transmitter
		if (!r.txsr_busy && r.thr_full && r.tx_en) begin
			r.txsr = {1'b1, r.thr, 1'b0};
			r.txbit = 4'd10;
			r.txcnt = per;
			r.txsr_busy = 1'b1;
			r.thr_full = 1'b0;
			r.txemp = 1'b0;
		end else if (r.txsr_busy) begin
			if (r.txcnt <= 16'd1) begin
				r.txsr = {1'b1, r.txsr[9:1]};
				r.txcnt = per;
				r.txbit = r.txbit - 4'd1;
				if (r.txbit == 4'd0) begin
					r.txsr_busy = 1'b0;
					// TxEMP only while enabled (a disabled transmitter does not set it again)
					if (!r.thr_full && r.tx_en)
						r.txemp = 1'b1;
				end
			end else
				r.txcnt = r.txcnt - 16'd1;
		end
		txd = r.txsr_busy ? r.txsr[0] : 1'b1;
		// receiver (8N1, sample mid-bit)
		r.rxd_q = rxd;
		if (!r.rxbusy) begin
			if (u.rxd_q && !rxd) begin
				r.rxbusy = 1'b1;
				r.rxcnt = per + per / 2;
				r.rxbit = 4'd0;
			end
		end else if (r.rxcnt <= 16'd1) begin
			if (r.rxbit < 4'd8) begin
				r.rxsr = {rxd, r.rxsr[7:1]};
				r.rxbit = r.rxbit + 4'd1;
				r.rxcnt = per;
			end else begin
				r.rxbusy = 1'b0;           // stop bit
				if (r.rx_en) begin
					case (r.rxn)
						2'd0: begin r.rxf0 = r.rxsr; r.rxn = 2'd1; end
						2'd1: begin r.rxf1 = r.rxsr; r.rxn = 2'd2; end
						2'd2: begin r.rxf2 = r.rxsr; r.rxn = 2'd3; end
						default: begin r.rxf2 = r.rxsr; r.overrun = 1'b1; end
					endcase
				end
			end
		end else
			r.rxcnt = r.rxcnt - 16'd1;
		return r;
	endfunction

	function automatic uart_t uart_cmd(uart_t u, logic [7:0] d);
		uart_t r = u;
		case (d[6:4])
			3'd1: r.mrptr = 1'b0;
			3'd2: begin r.rx_en = 1'b0; r.rxn = 2'd0; r.overrun = 1'b0; end
			3'd3: begin r.tx_en = 1'b0; r.thr_full = 1'b0; r.txsr_busy = 1'b0; r.txemp = 1'b0; end
			3'd4: r.overrun = 1'b0;
			default: ;
		endcase
		if (d[0]) r.rx_en = 1'b1;
		if (d[1]) r.rx_en = 1'b0;
		if (d[2] && !r.tx_en) begin r.tx_en = 1'b1; r.txemp = !r.txsr_busy && !r.thr_full; end
		if (d[3]) begin r.tx_en = 1'b0; r.txemp = 1'b0; end
		return r;
	endfunction

	// ---------------------------------------------------------------- parallel port
	logic [7:0] ppdat;
	assign pp_out = ppdat;

	// ---------------------------------------------------------------- read mux
	function automatic logic [7:0] rd8(logic [9:0] a);
		logic [15:0] w = regs[a[9:1]];
		logic [7:0] r = a[0] ? w[7:0] : w[15:8];
		if (a >= 10'h014 && a <= 10'h022) r = icr[a - 10'h013];
		case (a)
			10'h036: r = imr[15:8];
			10'h037: r = imr[7:0];
			10'h03a: r = ipr[15:8];
			10'h03b: r = ipr[7:0];
			10'h100: r = t1.tmr[15:8]; 10'h101: r = t1.tmr[7:0];
			10'h104: r = t1.trr[15:8]; 10'h105: r = t1.trr[7:0];
			10'h10c: r = t1.tcn[15:8]; 10'h10d: r = t1.tcn[7:0];
			10'h111: r = {6'b0, t1.ter};
			10'h120: r = t2.tmr[15:8]; 10'h121: r = t2.tmr[7:0];
			10'h124: r = t2.trr[15:8]; 10'h125: r = t2.trr[7:0];
			10'h12c: r = t2.tcn[15:8]; 10'h12d: r = t2.tcn[7:0];
			10'h131: r = {6'b0, t2.ter};
			10'h140: r = ua.mrptr ? ua.mr2 : ua.mr1;   10'h180: r = ub.mrptr ? ub.mr2 : ub.mr1;
			10'h144: r = uart_usr(ua);                 10'h184: r = uart_usr(ub);
			10'h14c: r = ua.rxf0;                      10'h18c: r = ub.rxf0;
			10'h154: r = uart_isr(ua);                 10'h194: r = uart_isr(ub);
			10'h158: r = ua.ubg1; 10'h15c: r = ua.ubg2; 10'h198: r = ub.ubg1; 10'h19c: r = ub.ubg2;
			10'h170: r = ua.ivr;                       10'h1b0: r = ub.ivr;
			10'h1c5: r = pp_ddr;
			10'h1c9: r = (pp_in & ~pp_ddr) | (ppdat & pp_ddr);
			default: ;
		endcase
		return r;
	endfunction
	always_comb dout = {rd8({offs[9:1], 1'b0}), rd8({offs[9:1], 1'b1})};

	// ---------------------------------------------------------------- writes and state
	always_ff @(posedge clk) begin
		logic t1d, t2d;
		if (reset) begin
			for (int s = 1; s <= 15; s++) icr[s] <= 8'h00;
			imr <= 16'h3ffe;
			t1 <= '0; t2 <= '0;
			ua <= '0; ub <= '0;
			ua.rxd_q <= 1'b1; ub.rxd_q <= 1'b1;
			ppdat <= 8'h00; pp_ddr <= 8'h00;
			div16 <= 4'd0;
		end else begin
			// timers (CLK = 01 sysclk; CLK = 10 sysclk/16) and UART baud, on the system clock enable
			if (ce_sys) begin
				div16 <= div16 + 4'd1;
				if (t1.tmr[2:1] == 2'b01 || (t1.tmr[2:1] == 2'b10 && div16 == 4'd0)) t1 <= tick(t1);
				if (t2.tmr[2:1] == 2'b01 || (t2.tmr[2:1] == 2'b10 && div16 == 4'd0)) t2 <= tick(t2);
				ua <= uart_step(ua, u1_rxd, u1_txd);
				ub <= uart_step(ub, u2_rxd, u2_txd);
			end
			if (t1_warp && !t1.ter[1]) begin
				t1.ter[1] <= 1'b1; t1.tcn <= 16'd0; t1.pre <= t1.tmr[15:8];
			end
			if (t2_warp && !t2.ter[1]) begin
				t2.ter[1] <= 1'b1; t2.tcn <= 16'd0; t2.pre <= t2.tmr[15:8];
			end

			if (sel && !wr) begin
				// reading the receive buffer pops the FIFO
				if (offs == 10'h14c && uds) begin
					ua.rxf0 <= ua.rxf1; ua.rxf1 <= ua.rxf2;
					if (ua.rxn != 0) ua.rxn <= ua.rxn - 2'd1;
				end
				if (offs == 10'h18c && uds) begin
					ub.rxf0 <= ub.rxf1; ub.rxf1 <= ub.rxf2;
					if (ub.rxn != 0) ub.rxn <= ub.rxn - 2'd1;
				end
			end

			if (sel && wr) begin
				for (int lane = 0; lane < 2; lane++) begin
					logic [9:0] a;
					logic [7:0] d;
					logic en;
					a = {offs[9:1], lane[0]};
					d = lane == 0 ? din[15:8] : din[7:0];
					en = lane == 0 ? uds : lds;
					if (en) begin
						if (lane == 0) regs[offs[9:1]][15:8] <= d; else regs[offs[9:1]][7:0] <= d;
						if (a >= 10'h014 && a <= 10'h022) icr[a - 10'h013] <= d;
						case (a)
							10'h036: imr[15:8] <= d;
							10'h037: imr[7:0] <= d;
							10'h100: t1.tmr[15:8] <= d;
							10'h101: begin t1.tmr[7:0] <= d; if (!d[0]) begin t1.tcn <= 0; t1.pre <= 0; end end
							10'h104: t1.trr[15:8] <= d; 10'h105: t1.trr[7:0] <= d;
							10'h10c, 10'h10d: t1.tcn <= 16'd0;
							10'h111: t1.ter <= t1.ter & ~d[1:0];
							10'h120: t2.tmr[15:8] <= d;
							10'h121: begin t2.tmr[7:0] <= d; if (!d[0]) begin t2.tcn <= 0; t2.pre <= 0; end end
							10'h124: t2.trr[15:8] <= d; 10'h125: t2.trr[7:0] <= d;
							10'h12c, 10'h12d: t2.tcn <= 16'd0;
							10'h131: t2.ter <= t2.ter & ~d[1:0];
							10'h140: begin if (ua.mrptr) ua.mr2 <= d; else begin ua.mr1 <= d; ua.mrptr <= 1'b1; end end
							10'h180: begin if (ub.mrptr) ub.mr2 <= d; else begin ub.mr1 <= d; ub.mrptr <= 1'b1; end end
							10'h144: ua.csr <= d;  10'h184: ub.csr <= d;
							10'h148: ua <= uart_cmd(ua, d);  10'h188: ub <= uart_cmd(ub, d);
							10'h14c: begin ua.thr <= d; ua.thr_full <= 1'b1; ua.txemp <= 1'b0; end
							10'h18c: begin ub.thr <= d; ub.thr_full <= 1'b1; ub.txemp <= 1'b0; end
							10'h154: ua.imr <= d;  10'h194: ub.imr <= d;
							10'h158: ua.ubg1 <= d; 10'h15c: ua.ubg2 <= d;
							10'h198: ub.ubg1 <= d; 10'h19c: ub.ubg2 <= d;
							10'h170: ua.ivr <= d;  10'h1b0: ub.ivr <= d;
							10'h1c5: pp_ddr <= d;
							10'h1c9: ppdat <= d;
							default: ;
						endcase
					end
				end
			end
		end
	end
endmodule
