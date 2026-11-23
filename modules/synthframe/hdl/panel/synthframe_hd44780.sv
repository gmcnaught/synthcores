// SPDX-License-Identifier: MIT
// synthframe (from the Proteus/1 XR and CZ-101 cores)
//============================================================================
//  HD44780 LCD controller (8-bit interface), modelled on MAME's
//  hd44780_base_device: DDRAM 128 x 8, CGRAM 64 x 8, address counter
//  correction, entry mode, display/cursor shift, display control.
//
//  Reset (MAME device_reset) and clear display fill DDRAM with spaces, one
//  address a clock for 128 clocks; reset also zeroes CGRAM. A write during
//  those clocks lands (the busy time is far longer).
//
//  Busy flag as MAME's set_busy_flag: 10 controller cycles after an access,
//  410 after clear, home and reset. BUSY_EXT selects how time is measured:
//    0: clock cycles. Busy for BUSY_SHORT / BUSY_LONG clocks after the access
//       (Proteus/1: 270 kHz controller, 80 MHz clock: 2963 / 121481).
//    1: an external time base. The access records busy_t_set; busy reads as
//       set while busy_t_read - that time <= BUSY_SHORT / BUSY_LONG (CZ-101:
//       CPU cycles in virtual time, 122 / 5060; cores/cz101/hdl/cz101_lcd.sv).
//
//  Bus: single-cycle strobes. wr with rs = 0 is an instruction, rs = 1 data;
//  rd with rs = 1 is a data read (the address counter steps). dout is the
//  read value for rs (combinational); the caller decides when it is driven.
//
//  Render port (combinational; README.md, "Character LCD read port"): for a
//  display line/position, the character code, the CGRAM row bits for codes
//  below 0x10, and the cursor. Positions wrap as MAME's render(): 40 a line
//  with two lines, 80 with one.
//  POS_W: width of the position input (4: 16 columns, at most 5).
//============================================================================

module synthframe_hd44780 #(
	parameter int POS_W      = 4,
	parameter bit BUSY_EXT   = 0,
	parameter int BUSY_SHORT = 2963,
	parameter int BUSY_LONG  = 121481
)
(
	input            clk,
	input            reset,

	input            cs,
	input            wr,        // single-cycle strobes
	input            rd,
	input            rs,        // 0 = instruction/status, 1 = data
	input      [7:0] din,
	output reg [7:0] dout,
	input     [31:0] busy_t_set,    // BUSY_EXT: time of this clock's access (or reset)
	input     [31:0] busy_t_read,   // BUSY_EXT: time the busy flag is read at

	input            r_line,
	input [POS_W-1:0] r_pos,
	input      [2:0] r_row,
	output     [7:0] r_code,
	output     [4:0] r_cgbits,
	output           r_is_cg,
	output           r_at_ac,    // the position's DDRAM address is the address counter
	output           r_cursor,   // cursor underline at this position (row 7)
	output           display_on,
	output           cursor_on,
	output           blink_on
);

reg  [7:0] ddram [0:127];
reg  [7:0] cgram [0:63];
reg  signed [7:0] ac;
reg        active_cg;
reg        dir_inc;
reg        shift_on;
reg  [6:0] disp_shift;
reg        disp_on, cur_on, blk_on;
reg        two_line;
reg        clearing, cg_clear;
reg  [6:0] clr_idx;

assign display_on = disp_on;
assign cursor_on  = cur_on;
assign blink_on   = blk_on;

// ------------------------------------------------ busy flag
wire busy;
reg  set_busy, set_long;     // this clock's access sets the busy flag
generate if (BUSY_EXT) begin : g_ext
	reg [31:0] busy_t;
	reg        busy_long;
	wire [31:0] since = busy_t_read - busy_t;
	assign busy = busy_long ? (since <= 32'(BUSY_LONG)) : (since <= 32'(BUSY_SHORT));
	always @(posedge clk) if (reset || set_busy) begin
		busy_t <= busy_t_set;
		busy_long <= reset || set_long;
	end
end else begin : g_cnt
	reg [16:0] busy_cnt;
	assign busy = (busy_cnt != 0);
	always @(posedge clk) begin
		if (busy_cnt != 0) busy_cnt <= busy_cnt - 1'd1;
		if (reset) busy_cnt <= 17'(BUSY_LONG);
		else if (set_busy) busy_cnt <= set_long ? 17'(BUSY_LONG) : 17'(BUSY_SHORT);
	end
end endgenerate

// instructions that set the busy flag (MAME control_write: all but 0x00); clear and home are long
always @* begin
	set_busy = 1'b0; set_long = 1'b0;
	if (cs && wr) begin
		set_busy = rs || din != 8'h00;
		set_long = !rs && din[7:2] == 6'd0 && din[1:0] != 2'd0;
	end else if (cs && rd && rs) set_busy = 1'b1;
end

// ------------------------------------------------ address counter helpers (MAME correct_ac)
function signed [7:0] correct(input signed [7:0] a, input cg, input two);
	reg signed [7:0] maxa;
	begin
		if (cg) correct = a & 8'sh3f;
		else begin
			maxa = two ? 8'sh67 : 8'sh4f;
			if (a > maxa)                          correct = a - maxa - 8'sd1;
			else if (a < 0)                        correct = maxa;
			else if (two && a > 8'sh27 && a < 8'sh40) correct = 8'sh40 + (a - 8'sh28);
			else                                   correct = a;
		end
	end
endfunction

function signed [7:0] step_ac(input signed [7:0] a, input inc, input cg, input two);
	begin
		if (!cg && two && !inc && a == 8'sh40) step_ac = 8'sh27;
		else step_ac = correct(inc ? a + 8'sd1 : a - 8'sd1, cg, two);
	end
endfunction

function [6:0] shift_disp(input [6:0] s, input inc);
	begin
		if (inc) shift_disp = (s == 7'd79) ? 7'd0 : s + 7'd1;
		else     shift_disp = (s == 7'd0) ? 7'd79 : s - 7'd1;
	end
endfunction

always @(posedge clk) begin
	if (clearing) begin
		ddram[clr_idx] <= 8'h20;
		if (cg_clear && !clr_idx[6]) cgram[clr_idx[5:0]] <= 8'h00;
		clr_idx <= clr_idx + 7'd1;
		if (clr_idx == 7'd127) clearing <= 1'b0;
	end
	if (reset) begin
		ac <= 0; active_cg <= 0; dir_inc <= 1; shift_on <= 0; disp_shift <= 0;
		disp_on <= 0; cur_on <= 0; blk_on <= 0; two_line <= 0;
		clearing <= 1; clr_idx <= 0; cg_clear <= 1;
	end
	else begin
		if (clearing && clr_idx == 7'd127) cg_clear <= 0;
		if (cs && wr) begin
			if (!rs) begin
				// instruction
				if (din[7]) begin
					active_cg <= 0;
					ac <= correct({1'b0, din[6:0]}, 1'b0, two_line);
				end
				else if (din[6]) begin
					active_cg <= 1;
					ac <= {2'b00, din[5:0]};
				end
				else if (din[5]) begin
					// function set (8-bit interface; the 5 x 10 font is not modelled)
					two_line <= din[3];
					ac <= correct(ac, active_cg, din[3]);
				end
				else if (din[4]) begin
					if (din[3]) disp_shift <= shift_disp(disp_shift, din[2]);
					else ac <= step_ac(ac, din[2], active_cg, two_line);
				end
				else if (din[3]) begin
					disp_on <= din[2]; cur_on <= din[1]; blk_on <= din[0];
				end
				else if (din[2]) begin
					dir_inc <= din[1]; shift_on <= din[0];
				end
				else if (din[1]) begin
					ac <= 0; active_cg <= 0; dir_inc <= 1; disp_shift <= 0;
				end
				else if (din[0]) begin
					ac <= 0; active_cg <= 0; dir_inc <= 1; disp_shift <= 0;
					clearing <= 1; clr_idx <= 0;
				end
			end
			else begin
				// data write
				if (active_cg) cgram[ac[5:0]] <= din;
				else           ddram[ac[6:0]] <= din;
				ac <= step_ac(ac, dir_inc, active_cg, two_line);
				if (shift_on) disp_shift <= shift_disp(disp_shift, dir_inc);
			end
		end
		else if (cs && rd && rs) begin
			ac <= step_ac(ac, dir_inc, active_cg, two_line);
		end
	end
end

always @* begin
	if (!rs) dout = {busy, ac[6:0]};
	else     dout = active_cg ? cgram[ac[5:0]] : ddram[ac[6:0]];
end

// ------------------------------------------------ render (MAME render(): line_size = 80 / lines)
wire [7:0] sp   = 8'(r_pos) + 8'(disp_shift);                      // 0..110
wire [7:0] m80  = (sp >= 8'd80) ? sp - 8'd80 : sp;
wire [6:0] m40  = 7'((m80 >= 8'd40) ? m80 - 8'd40 : m80);
wire [6:0] cpos = two_line ? ((r_line ? 7'h40 : 7'h00) + m40) : 7'(m80);
assign r_code   = ddram[cpos];
assign r_is_cg  = (r_code < 8'h10);
assign r_cgbits = cgram[{r_code[2:0], r_row}][4:0];
assign r_at_ac  = (cpos == ac[6:0]);
assign r_cursor = cur_on && r_at_ac && (r_row == 3'd7);

endmodule
