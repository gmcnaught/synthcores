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
	input      [15:0] wr_mask,        // byte lanes to write (MAME COMBINE_DATA)
	input             pan_wr,
	input       [4:0] pan_voice,
	input       [7:0] pan_data,

	// CPU reads: raw register words (combinational), or +0x18 = sample read-back
	input       [4:0] rd_voice,
	input       [3:0] rd_word,
	output     [15:0] rd_raw,
	input             rd_start,         // start a +0x18 read-back of rd_voice (engine idle only)
	output reg        rd_done,
	output reg [15:0] rd_data,

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
reg [15:0] raw    [0:31][0:15];  // all 16 register words, as MAME m_regs
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
// Two stages (timing): stage 1 latches the write and reads the word and its partner,
// stage 2 merges and updates the voice state. Writes must be at least 2 clocks apart.
reg         wq;
reg  [4:0]  wq_voice;
reg  [3:0]  wq_word;
reg  [15:0] wq_data, wq_mask, wq_old, wq_partner;
wire [2:0]  wr_long = wq_word[3:1];          // 0:+00 1:+04 2:+08 3:+0c 4:+10 5:+14
wire [15:0] wr_new  = (wq_old & ~wq_mask) | (wq_data & wq_mask);
wire [31:0] wr_val  = wq_word[0] ? {wq_partner, wr_new} : {wr_new, wq_partner};
assign rd_raw = raw[rd_voice][rd_word];

// ---------------------------------------------------------------- engine
// Per voice: S_WAIT latches the voice state and its two sample words, then
// S_C1..S_C5 compute in registered steps (80 MHz), S_NEXT writes back.
localparam S_IDLE = 4'd0, S_WAIT = 4'd1, S_C1 = 4'd2, S_C2 = 4'd3, S_C3 = 4'd4,
           S_C4 = 4'd5, S_C5 = 4'd6, S_NEXT = 4'd7, S_OUT = 4'd8,
           R_WAIT = 4'd9, R_C1 = 4'd10, R_C2 = 4'd11, R_C3 = 4'd12, R_C4 = 4'd13, R_WB = 4'd14;
reg  [3:0]  state;
reg  [4:0]  v;                // voice being computed
reg  [15:0] s0_r, s1_r;
reg  [15:0] nx_s0, nx_s1;     // prefetched words for voice v+1
reg         nx_valid;

// latched voice state
reg  [23:0] r_addr, r_ls, r_le, r_env;
reg  [11:0] r_frac, r_level;
reg  [18:0] r_pitch;
reg  [3:0]  r_pan;

reg  [11:0] lvl_n;
reg  signed [16:0] diff;
reg  signed [28:0] prod;
reg  signed [16:0] smp;
reg  [16:0] gain;
reg  signed [33:0] mul2;
reg  signed [16:0] vout;
reg  signed [21:0] pl, pr;
reg  signed [31:0] acc_l, acc_r;

reg  [12:0] pos;
reg  [23:0] addr1, lspan, addr_wrap, addr_st;
reg         wrap1, wrap2;

wire [21:0] fetch_addr_next = v_addr[v + 5'd1][21:0] + 22'd4;

function signed [15:0] clamp16(input signed [31:0] x);
	begin
		if (x > 32'sd32767)       clamp16 = 16'sd32767;
		else if (x < -32'sd32768) clamp16 = -16'sd32768;
		else                      clamp16 = x[15:0];
	end
endfunction

integer i, j;
always @(posedge clk) begin
	done    <= 0;
	rd_done <= 0;

	if (reset) begin
		state    <= S_IDLE;
		busy     <= 0;
		mem_req  <= 0;
		wq       <= 0;
		out_l    <= 0;
		out_r    <= 0;
		nx_valid <= 0;
		for (i = 0; i < 32; i = i + 1) begin
			for (j = 0; j < 16; j = j + 1) raw[i][j] <= 0;
			v_addr[i] <= 0; v_frac[i] <= 0; v_pitch[i] <= 0; v_env[i] <= 0;
			v_lstart[i] <= 0; v_lend[i] <= 0; v_level[i] <= 0; v_pan[i] <= 4'h8; v_disp[i] <= 0;
		end
	end
	else begin
		// register writes (only between samples)
		wq <= wr;
		if (wr) begin
			wq_voice   <= wr_voice;
			wq_word    <= wr_word;
			wq_data    <= wr_data;
			wq_mask    <= wr_mask;
			wq_old     <= raw[wr_voice][wr_word];
			wq_partner <= raw[wr_voice][wr_word ^ 4'd1];
		end
		if (wq) begin
			raw[wq_voice][wq_word] <= wr_new;
			if (wq_word < 4'd12) case (wr_long)
				3'd0: begin v_addr[wq_voice] <= wr_val[23:0]; v_frac[wq_voice] <= 0; end
				3'd1: v_pitch[wq_voice] <= wr_val[18:0];
				3'd2: begin
					v_env[wq_voice] <= wr_val[23:0];
					if (wq_word[0] && wr_val[9]) v_level[wq_voice] <= wr_val[23:12];
				end
				3'd4: v_lstart[wq_voice] <= wr_val[23:0];
				3'd5: v_lend[wq_voice] <= ~wr_val[23:0];
				default: ;
			endcase
		end
		if (pan_wr) v_pan[pan_voice] <= pan_data[3:0];

		// memory handshake: one outstanding request
		if (mem_req && mem_ack) mem_req <= 0;

		case (state)
		S_IDLE:
			if (rd_start) begin
				// +0x18 read-back: ROM word at address + 4, then step if pitch bit 18 is set
				busy     <= 1;
				v        <= rd_voice;
				mem_req  <= 1;
				mem_addr <= v_addr[rd_voice][21:0] + 22'd4;
				state    <= R_WAIT;
			end
			else if (start) begin
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
			r_addr  <= v_addr[v];
			r_frac  <= v_frac[v];
			r_pitch <= v_pitch[v];
			r_env   <= v_env[v];
			r_ls    <= v_lstart[v];
			r_le    <= v_lend[v];
			r_level <= v_level[v];
			r_pan   <= v_pan[v];
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
			lvl_n <= env_next(r_level, r_env);
			diff  <= $signed({s1_r[15], s1_r}) - $signed({s0_r[15], s0_r});
			pos   <= {1'b0, r_frac} + {1'b0, r_pitch[11:0]};
			lspan <= r_le - r_ls;
			wrap2 <= (r_le >= r_ls);
			state <= S_C2;
		end

		S_C2: begin
			prod  <= diff * $signed({1'b0, r_frac});
			gain  <= level_to_gain(lvl_n);
			addr1 <= r_addr + {19'd0, r_pitch[16:12]} + {23'd0, pos[12]};
			state <= S_C3;
		end

		S_C3: begin
			smp       <= $signed({s0_r[15], s0_r}) + (prod >>> 12);
			wrap1     <= (addr1 > r_le);
			addr_wrap <= addr1 - lspan;
			state     <= S_C4;
		end

		S_C4: begin
			mul2    <= smp * $signed({1'b0, gain});
			addr_st <= (wrap1 && wrap2) ? addr_wrap : addr1;
			state   <= S_C5;
		end

		S_C5: begin
			vout  <= mul2 >>> 16;
			state <= S_NEXT;
		end

		S_NEXT: begin
			pl <= vout * $signed({1'b0, r_pan});
			pr <= vout * $signed({1'b0, 4'd15 - r_pan});
			v_level[v] <= lvl_n;
			v_disp[v]  <= lvl_n[11:4];
			if (!r_pitch[18]) begin
				v_frac[v] <= pos[11:0];
				v_addr[v] <= addr_st;
			end
			state <= S_OUT;
		end

		S_OUT: begin
			acc_l <= acc_l + pl;
			acc_r <= acc_r + pr;
			if (v == 5'd31) begin
				state <= S_IDLE;
				busy  <= 0;
				done  <= 1;
				out_l <= clamp16((acc_l + pl) >>> 6);
				out_r <= clamp16((acc_r + pr) >>> 6);
			end
			else begin
				v     <= v + 5'd1;
				state <= S_WAIT;
			end
		end

		R_WAIT: begin
			r_addr  <= v_addr[v];
			r_frac  <= v_frac[v];
			r_pitch <= v_pitch[v];
			r_ls    <= v_lstart[v];
			r_le    <= v_lend[v];
			if (mem_req && mem_ack) begin
				rd_data <= mem_s0;
				state   <= R_C1;
			end
		end

		R_C1: begin
			pos   <= {1'b0, r_frac} + {1'b0, r_pitch[11:0]};
			lspan <= r_le - r_ls;
			wrap2 <= (r_le >= r_ls);
			state <= R_C2;
		end

		R_C2: begin
			addr1 <= r_addr + {19'd0, r_pitch[16:12]} + {23'd0, pos[12]};
			state <= R_C3;
		end

		R_C3: begin
			wrap1     <= (addr1 > r_le);
			addr_wrap <= addr1 - lspan;
			state     <= R_C4;
		end

		R_C4: begin
			addr_st <= (wrap1 && wrap2) ? addr_wrap : addr1;
			state   <= R_WB;
		end

		R_WB: begin
			if (r_pitch[18]) begin
				v_frac[v] <= pos[11:0];
				v_addr[v] <= addr_st;
			end
			busy    <= 0;
			rd_done <= 1;
			state   <= S_IDLE;
		end

		default: state <= S_IDLE;
		endcase

		// capture a prefetch that completes while the current voice is computed
		if (state != S_WAIT && state != S_IDLE && state != R_WAIT && mem_req && mem_ack) begin
			nx_s0 <= mem_s0; nx_s1 <= mem_s1; nx_valid <= 1;
		end
	end
end

endmodule
