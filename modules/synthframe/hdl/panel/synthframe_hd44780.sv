// SPDX-License-Identifier: MIT
// synthframe (from the Proteus/1 XR core, cores/proteus/mister/rtl/hd44780.sv)
//============================================================================
//  HD44780 LCD controller (8-bit interface), modelled on MAME's
//  hd44780_base_device: DDRAM 128 x 8, CGRAM 64 x 8, address counter
//  correction, entry mode, display/cursor shift, display control.
//  Busy flag timing as MAME: 10 controller cycles per access, 410 for clear,
//  home and reset, at the 270 kHz clock the proteusxr driver uses
//  (37 us / 1.52 ms = 2963 / 121481 cycles of the 80 MHz clock).
//
//  Render port: for a display line/position returns the character code and,
//  for codes < 0x10, the CGRAM row bits; the font ROM lives in the display.
//  The render port is combinational (README.md, "Character LCD read port").
//  POS_W: width of the position input (4: 16 columns).
//============================================================================

module synthframe_hd44780 #(parameter int POS_W = 4)
(
	input            clk,
	input            reset,

	input            cs,
	input            wr,        // single-cycle strobes
	input            rd,
	input            rs,        // 0 = instruction/status, 1 = data
	input      [7:0] din,
	output reg [7:0] dout,

	input            r_line,
	input [POS_W-1:0] r_pos,
	input      [2:0] r_row,
	output     [7:0] r_code,
	output     [4:0] r_cgbits,
	output           r_is_cg,
	output           r_cursor,   // cursor underline at this position (row 7)
	output           display_on
);

reg  [7:0] ddram [0:127];
reg  [7:0] cgram [0:63];
reg  signed [7:0] ac;
reg        active_cg;
reg        dir_inc;
reg        shift_on;
reg  [6:0] disp_shift;
reg        disp_on, cursor_on, blink_on;
reg        two_line;
reg [16:0] busy_cnt;
localparam [16:0] BUSY_SHORT = 17'd2963, BUSY_LONG = 17'd121481;
wire       busy = (busy_cnt != 0);

assign display_on = disp_on;

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

integer i;
always @(posedge clk) begin
	if (busy_cnt != 0) busy_cnt <= busy_cnt - 1'd1;
	if (reset) begin
		busy_cnt <= BUSY_LONG;
		ac <= 0; active_cg <= 0; dir_inc <= 1; shift_on <= 0; disp_shift <= 0;
		disp_on <= 0; cursor_on <= 0; blink_on <= 0; two_line <= 0;
		for (i = 0; i < 128; i = i + 1) ddram[i] <= 8'h20;
	end
	else if (cs && wr) begin
		busy_cnt <= (!rs && din[7:2] == 6'd0 && din[1:0] != 2'd0) ? BUSY_LONG : BUSY_SHORT;
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
				two_line <= din[3];
				ac <= correct(ac, active_cg, din[3]);
			end
			else if (din[4]) begin
				if (din[3]) disp_shift <= shift_disp(disp_shift, din[2]);
				else ac <= step_ac(ac, din[2], active_cg, two_line);
			end
			else if (din[3]) begin
				disp_on <= din[2]; cursor_on <= din[1]; blink_on <= din[0];
			end
			else if (din[2]) begin
				dir_inc <= din[1]; shift_on <= din[0];
			end
			else if (din[1]) begin
				ac <= 0; active_cg <= 0; dir_inc <= 1; disp_shift <= 0;
			end
			else if (din[0]) begin
				ac <= 0; active_cg <= 0; dir_inc <= 1; disp_shift <= 0;
				for (i = 0; i < 128; i = i + 1) ddram[i] <= 8'h20;
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
		busy_cnt <= BUSY_SHORT;
	end
end

always @* begin
	if (!rs) dout = {busy, ac[6:0]};
	else     dout = active_cg ? cgram[ac[5:0]] : ddram[ac[6:0]];
end

// ------------------------------------------------ render
wire [6:0] sp   = 7'(r_pos) + disp_shift;                  // 0..118
wire [6:0] m40  = (sp >= 7'd80) ? sp - 7'd80 : (sp >= 7'd40) ? sp - 7'd40 : sp;
wire [6:0] cpos = (two_line ? (r_line ? 7'h40 : 7'h00) : 7'h00) + m40;
assign r_code   = ddram[cpos];
assign r_is_cg  = (r_code < 8'h10);
assign r_cgbits = cgram[{r_code[2:0], r_row}][4:0];
assign r_cursor = cursor_on && (cpos == ac[6:0]) && (r_row == 3'd7);

endmodule
