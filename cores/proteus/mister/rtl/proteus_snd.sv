//============================================================================
//  E-mu Proteus/1 sound generator (32 voices) - behavioural HDL
//
//  Matches tools/proteus_model.py and the MAME device
//  (third_party/mame/src/mame/emusys/proteus_snd.cpp) sample for sample.
//  Register map and inferences: see the MAME source header.
//
//  One output sample per `start` pulse: voices are processed one after
//  another; the next voice's sample words are fetched while the current
//  voice is computed. `done` pulses with the new sample on out_l/out_r.
//
//  Register writes (`wr`) and pan writes (`pan_wr`) must not overlap a
//  sample computation (the demo controller only issues them between samples).
//============================================================================

module proteus_snd
(
	input             clk,
	input             reset,

	// register write port: voice, 16-bit word index 0..15, data
	input             wr,
	input       [4:0] wr_voice,
	input       [3:0] wr_word,
	input      [15:0] wr_data,
	input             pan_wr,
	input       [4:0] pan_voice,
	input       [7:0] pan_data,

	// compute one output sample
	input             start,
	output reg        busy,
	output reg        done,
	output reg signed [15:0] out_l,
	output reg signed [15:0] out_r,

	// sample memory: word address -> {word[addr+1], word[addr]} (big-endian words already swapped)
	output reg        mem_req,
	output reg [21:0] mem_addr,
	input             mem_ack,
	input      [15:0] mem_s0,
	input      [15:0] mem_s1,

	// per-voice level for display (level[11:4])
	input       [4:0] disp_voice,
	output      [7:0] disp_level
);

// ---------------------------------------------------------------- voice state
reg [15:0] raw_hi [0:31][0:5];   // high words of the 32-bit registers +0x00..+0x14
reg [15:0] raw_lo [0:31][0:5];   // low words
reg [23:0] v_addr  [0:31];
reg [11:0] v_frac  [0:31];
reg [18:0] v_pitch [0:31];       // bits 18..0 used
reg [23:0] v_env   [0:31];       // target 23..12, mode 11..9, rate 8..0
reg [23:0] v_lstart[0:31];
reg [23:0] v_lend  [0:31];
reg [11:0] v_level [0:31];
reg  [3:0] v_pan   [0:31];
reg  [7:0] v_disp  [0:31];

assign disp_level = v_disp[disp_voice];

// ---------------------------------------------------------------- arithmetic helpers
function [16:0] level_to_gain(input [11:0] level);
	reg [2:0] e;
	begin
		e = level[11:9];
		if (e == 3'd0) level_to_gain = {8'd0, level[8:0]};
		else           level_to_gain = {7'd0, 1'b1, level[8:0]} << (e - 3'd1);
	end
endfunction

function [11:0] env_next(input [11:0] level, input [23:0] env);
	reg [11:0] target;
	reg [8:0]  rate;
	reg [12:0] delta;
	reg [12:0] up;
	reg signed [13:0] dn;
	begin
		target = env[23:12];
		rate   = env[8:0];
		if (rate == 9'd0)       delta = 13'd1;
		else if (rate[8])       delta = {4'd0, 9'd0 - rate};   // |negative rate|
		else                    delta = {4'd0, rate};
		up = {1'b0, level} + delta;
		dn = $signed({2'b00, level}) - $signed({1'b0, delta});
		if (level < target)      env_next = (up > {1'b0, target}) ? target : up[11:0];
		else if (level > target) env_next = (dn < $signed({2'b00, target})) ? target : dn[11:0];
		else                     env_next = level;
	end
endfunction

// ---------------------------------------------------------------- register writes
// reg_long(w) as the MAME model: write the word, then rebuild the 32-bit value
wire [2:0]  wr_long = wr_word[3:1];          // 0:+00 1:+04 2:+08 3:+0c 4:+10 5:+14
wire [31:0] wr_val  = wr_word[0] ? {raw_hi[wr_voice][wr_long], wr_data}
                                 : {wr_data, raw_lo[wr_voice][wr_long]};

// ---------------------------------------------------------------- engine
localparam S_IDLE = 3'd0, S_WAIT = 3'd1, S_C1 = 3'd2, S_C2 = 3'd3, S_C3 = 3'd4, S_C4 = 3'd5, S_NEXT = 3'd6;
reg  [2:0]  state;
reg  [4:0]  v;                // voice being computed
reg         fetch_pending;    // a request for voice v is outstanding or its data is latched
reg         have_data;
reg  [15:0] s0_r, s1_r;
reg  [15:0] nx_s0, nx_s1;     // prefetched words for voice v+1
reg         nx_valid;

reg  [11:0] lvl_n;
reg  signed [16:0] diff;
reg  signed [28:0] prod;
reg  signed [16:0] smp;
reg  [16:0] gain;
reg  signed [33:0] mul2;
reg  signed [16:0] vout;
reg  signed [31:0] acc_l, acc_r;

wire [23:0] cur_addr = v_addr[v];
wire [21:0] fetch_addr_next = v_addr[v + 5'd1][21:0] + 22'd4;

// address step (uses the current voice's state)
wire [16:0] inc     = v_pitch[v][16:0];
wire [12:0] pos     = {1'b0, v_frac[v]} + {1'b0, inc[11:0]};
wire [23:0] addr1   = cur_addr + {19'd0, inc[16:12]} + {23'd0, pos[12]};
wire [23:0] ls      = v_lstart[v];
wire [23:0] le      = v_lend[v];
wire [23:0] addr_st = ((addr1 > le) && (le >= ls)) ? (addr1 - (le - ls)) : addr1;

function signed [15:0] clamp16(input signed [31:0] x);
	begin
		if (x > 32'sd32767)       clamp16 = 16'sd32767;
		else if (x < -32'sd32768) clamp16 = -16'sd32768;
		else                      clamp16 = x[15:0];
	end
endfunction

integer i, j;
always @(posedge clk) begin
	done <= 0;

	if (reset) begin
		state    <= S_IDLE;
		busy     <= 0;
		mem_req  <= 0;
		out_l    <= 0;
		out_r    <= 0;
		for (i = 0; i < 32; i = i + 1) begin
			for (j = 0; j < 6; j = j + 1) begin
				raw_hi[i][j] <= 0;
				raw_lo[i][j] <= 0;
			end
			v_addr[i] <= 0; v_frac[i] <= 0; v_pitch[i] <= 0; v_env[i] <= 0;
			v_lstart[i] <= 0; v_lend[i] <= 0; v_level[i] <= 0; v_pan[i] <= 4'h8; v_disp[i] <= 0;
		end
	end
	else begin
		// register writes (only between samples)
		if (wr) begin
			if (wr_word[0]) raw_lo[wr_voice][wr_long] <= wr_data;
			else            raw_hi[wr_voice][wr_long] <= wr_data;
			case (wr_long)
				3'd0: begin v_addr[wr_voice] <= wr_val[23:0]; v_frac[wr_voice] <= 0; end
				3'd1: v_pitch[wr_voice] <= wr_val[18:0];
				3'd2: begin
					v_env[wr_voice] <= wr_val[23:0];
					if (wr_word[0] && wr_val[9]) v_level[wr_voice] <= wr_val[23:12];
				end
				3'd4: v_lstart[wr_voice] <= wr_val[23:0];
				3'd5: v_lend[wr_voice] <= ~wr_val[23:0];
				default: ;
			endcase
		end
		if (pan_wr) v_pan[pan_voice] <= pan_data[3:0];

		// memory handshake: one outstanding request
		if (mem_req && mem_ack) mem_req <= 0;

		case (state)
		S_IDLE:
			if (start) begin
				busy     <= 1;
				v        <= 0;
				acc_l    <= 0;
				acc_r    <= 0;
				nx_valid <= 0;
				mem_req  <= 1;
				mem_addr <= v_addr[0][21:0] + 22'd4;
				state    <= S_WAIT;
			end

		S_WAIT: begin
			// data for voice v: either prefetched or arriving now
			if (nx_valid) begin
				s0_r <= nx_s0; s1_r <= nx_s1; nx_valid <= 0;
				state <= S_C1;
				if (v != 5'd31) begin mem_req <= 1; mem_addr <= fetch_addr_next; end
			end
			else if (mem_req && mem_ack) begin
				s0_r <= mem_s0; s1_r <= mem_s1;
				state <= S_C1;
				if (v != 5'd31) begin mem_req <= 1; mem_addr <= fetch_addr_next; end
			end
		end

		S_C1: begin
			lvl_n <= env_next(v_level[v], v_env[v]);
			diff  <= $signed({s1_r[15], s1_r}) - $signed({s0_r[15], s0_r});
			state <= S_C2;
		end

		S_C2: begin
			prod  <= diff * $signed({1'b0, v_frac[v]});
			gain  <= level_to_gain(lvl_n);
			state <= S_C3;
		end

		S_C3: begin
			smp   <= $signed({s0_r[15], s0_r}) + (prod >>> 12);
			state <= S_C4;
		end

		S_C4: begin
			mul2  <= smp * $signed({1'b0, gain});
			state <= S_NEXT;
		end

		S_NEXT: begin
			vout = mul2 >>> 16;
			acc_l <= acc_l + vout * $signed({1'b0, v_pan[v]});
			acc_r <= acc_r + vout * $signed({1'b0, 4'd15 - v_pan[v]});
			v_level[v] <= lvl_n;
			v_disp[v]  <= lvl_n[11:4];
			if (!v_pitch[v][18]) begin
				v_frac[v] <= pos[11:0];
				v_addr[v] <= addr_st;
			end
			if (v == 5'd31) begin
				state <= S_IDLE;
				busy  <= 0;
				done  <= 1;
				out_l <= clamp16((acc_l + vout * $signed({1'b0, v_pan[v]})) >>> 6);
				out_r <= clamp16((acc_r + vout * $signed({1'b0, 4'd15 - v_pan[v]})) >>> 6);
			end
			else begin
				v     <= v + 5'd1;
				state <= S_WAIT;
			end
		end

		default: state <= S_IDLE;
		endcase

		// capture a prefetch that completes while the current voice is computed
		if (state != S_WAIT && state != S_IDLE && mem_req && mem_ack) begin
			nx_s0 <= mem_s0; nx_s1 <= mem_s1; nx_valid <= 1;
		end
	end
end

endmodule
