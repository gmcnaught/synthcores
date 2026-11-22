//============================================================================
//  Proteus/1 front panel from MiSTer keyboard / joystick -> MFP GPIP inputs
//
//  Firmware (v2.30) usage, see docs/hardware-notes.md:
//    GPIP0..3  four buttons, active low, debounced by a 3-sample scan (0x24A0)
//    GPIP4     data-entry encoder clock: falling edge interrupt (0x23F2)
//    GPIP5     encoder direction, read in that interrupt: 1 = -1, 0 = +1
//    GPIP4..6  must all read 1 for several scans before the next step is accepted
//    GPIP7     unused here (its interrupt vector points into the debug monitor); held 1
//
//  Keyboard: M = Master, E = Edit, C or Tab = Cursor, Enter = Enter,
//            Left/Down/'-' = knob -1, Right/Up/'=' = knob +1 (auto-repeat while held)
//  Joystick: right/up = +1, left/down = -1, buttons 1..4 = Master, Edit, Cursor, Enter
//============================================================================

module panel #(parameter CLK_HZ = 80_000_000)
(
	input             clk,
	input             reset,
	input      [10:0] ps2_key,
	input      [31:0] joy,
	output      [7:0] gpio,
	output      [3:0] buttons_dbg,
	output            step_dbg
);

localparam PULSE  = CLK_HZ / 500;        // 2 ms encoder pulse
localparam GAP    = CLK_HZ / 66;         // 15 ms before the next step
localparam DELAY  = CLK_HZ / 3;          // auto-repeat delay 333 ms
localparam REPEAT = CLK_HZ / 20;         // then a step every 50 ms

// ------------------------------------------------ keyboard state
reg  k_master, k_edit, k_cursor, k_enter, k_up, k_down;
reg  old_toggle;
wire pressed = ps2_key[9];
wire ext     = ps2_key[8];
wire [7:0] code = ps2_key[7:0];

always @(posedge clk) begin
	old_toggle <= ps2_key[10];
	if (reset) begin
		{k_master, k_edit, k_cursor, k_enter, k_up, k_down} <= 0;
	end
	else if (old_toggle != ps2_key[10]) begin
		casez ({ext, code})
			9'h03A: k_master <= pressed;               // M
			9'h024: k_edit   <= pressed;               // E
			9'h021, 9'h00D: k_cursor <= pressed;       // C, Tab
			9'h05A, 9'h15A: k_enter  <= pressed;       // Enter, keypad Enter
			9'h174, 9'h175, 9'h055, 9'h079: k_up   <= pressed;   // Right, Up, '=', keypad +
			9'h16B, 9'h172, 9'h04E, 9'h07B: k_down <= pressed;   // Left, Down, '-', keypad -
			default: ;
		endcase
	end
end

wire [3:0] btn = {k_enter | joy[7], k_cursor | joy[6], k_edit | joy[5], k_master | joy[4]};
wire up   = k_up   | joy[0] | joy[3];
wire down = k_down | joy[1] | joy[2];
assign buttons_dbg = btn;

// ------------------------------------------------ knob: step requests with auto-repeat
reg  [24:0] rep_cnt;
reg         held_dir;          // 1 = down
reg         held;
reg         step_req, step_dir;

always @(posedge clk) begin
	step_req <= 0;
	if (reset || (!up && !down)) begin
		held <= 0;
	end
	else if (!held || held_dir != down) begin
		held     <= 1;
		held_dir <= down;
		rep_cnt  <= DELAY;
		step_req <= 1;
		step_dir <= down;
	end
	else if (rep_cnt == 0) begin
		rep_cnt  <= REPEAT;
		step_req <= 1;
		step_dir <= down;
	end
	else rep_cnt <= rep_cnt - 1'd1;
end

// ------------------------------------------------ encoder pulse generator (one step pending at most)
localparam E_IDLE = 2'd0, E_PULSE = 2'd1, E_GAP = 2'd2;
reg  [1:0]  e_state;
reg  [24:0] e_cnt;
reg         pend, pend_dir;
reg         enc_clk, enc_dir;

always @(posedge clk) begin
	if (step_req) begin pend <= 1; pend_dir <= step_dir; end
	if (reset) begin
		e_state <= E_IDLE; enc_clk <= 1; enc_dir <= 1; pend <= 0;
	end
	else case (e_state)
		E_IDLE:
			if (pend) begin
				pend    <= step_req;          // a request in this same clock stays pending
				enc_dir <= pend_dir;          // 1 = -1
				enc_clk <= 0;                 // falling edge -> GPIP4 interrupt
				e_cnt   <= PULSE;
				e_state <= E_PULSE;
			end
		E_PULSE:
			if (e_cnt == 0) begin
				enc_clk <= 1; enc_dir <= 1;   // back to idle (all high)
				e_cnt   <= GAP;
				e_state <= E_GAP;
			end
			else e_cnt <= e_cnt - 1'd1;
		E_GAP:
			if (e_cnt == 0) e_state <= E_IDLE;
			else e_cnt <= e_cnt - 1'd1;
		default: e_state <= E_IDLE;
	endcase
end

assign step_dbg = !enc_clk;
assign gpio = {1'b1, 1'b1, enc_dir, enc_clk, ~btn};

endmodule
