//============================================================================
//  Preset initialisation macro: drives the front-panel lines through the
//  firmware's own diagnostics "Initialize" (factory presets into RAM), then
//  restarts the CPU normally. Sequence verified in the Verilator system sim
//  (v2.30 firmware):
//    CPU reset, MASTER+EDIT held at start      -> DIAGNOSTICS / 1 Auto Cycle
//    knob +1 x 12                              -> 13 Initialize
//    ENTER                                     -> DESTROYS PRESETS  Yes / No
//    knob +1, ENTER                            -> INITIALIZE RAM  Done.
//    CPU reset (buttons released)              -> normal boot
//  GPIP: bit0 MASTER, bit1 EDIT, bit3 ENTER (active low), bit4 knob clock
//  (falling edge), bit5 knob direction (0 = +1).
//============================================================================

module init_macro #(parameter CLK_HZ = 80_000_000)
(
	input            clk,
	input            start,         // single-cycle strobe
	output reg       active,        // gpio and CPU reset are overridden
	output reg [7:0] gpio,
	output reg       cpu_reset,
	output reg       done           // single-cycle strobe when the CPU has been restarted
);

localparam MS = CLK_HZ / 1000;

// step list: {gpio value, CPU reset, duration in ms}
localparam N = 36;
function [20:0] stepdef(input [5:0] i);
	begin
		if (i == 0)                 stepdef = {8'hFC, 1'b1, 12'd10};    // reset, MASTER+EDIT held
		else if (i == 1)            stepdef = {8'hFC, 1'b0, 12'd600};   // boot into diagnostics
		else if (i == 2)            stepdef = {8'hFF, 1'b0, 12'd400};
		else if (i <= 26)           stepdef = i[0] ? {8'hCF, 1'b0, 12'd2}     // 3,5,..25: knob +1 pulse
		                                           : {8'hFF, 1'b0, 12'd398};  // 4,6,..26: idle
		else if (i == 27)           stepdef = {8'hF7, 1'b0, 12'd100};   // ENTER -> DESTROYS PRESETS
		else if (i == 28)           stepdef = {8'hFF, 1'b0, 12'd500};
		else if (i == 29)           stepdef = {8'hCF, 1'b0, 12'd2};     // knob +1
		else if (i == 30)           stepdef = {8'hFF, 1'b0, 12'd498};
		else if (i == 31)           stepdef = {8'hF7, 1'b0, 12'd100};   // ENTER -> INITIALIZE RAM Done.
		else if (i == 32)           stepdef = {8'hFF, 1'b0, 12'd1500};
		else if (i == 33)           stepdef = {8'hFF, 1'b1, 12'd10};    // restart normally
		else                        stepdef = {8'hFF, 1'b0, 12'd1};
	end
endfunction

reg [5:0]  step;
reg [11:0] ms_left;
reg [16:0] tick;

always @(posedge clk) begin
	done <= 0;
	if (start) begin
		active    <= 1;
		step      <= 0;
		{gpio, cpu_reset, ms_left} <= stepdef(6'd0);
		tick      <= 0;
	end
	else if (active) begin
		if (tick == MS - 1) begin
			tick <= 0;
			if (ms_left == 1) begin
				if (step == N - 1) begin
					active    <= 0;
					cpu_reset <= 0;
					done      <= 1;
				end
				else begin
					step <= step + 1'd1;
					{gpio, cpu_reset, ms_left} <= stepdef(step + 1'd1);
				end
			end
			else ms_left <= ms_left - 1'd1;
		end
		else tick <= tick + 1'd1;
	end
	else begin
		gpio      <= 8'hFF;
		cpu_reset <= 0;
	end
end

endmodule
