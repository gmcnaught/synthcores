//============================================================================
//  Proteus demo controller
//
//  - Loads the demo image (tools/make_demo.py) from the HPS into DDR3 at
//    0x30000000: 8 MB of big-endian sample words, then 32-bit events.
//  - Every SAMPLE_DIV clocks: applies the events due this sample (register
//    and pan writes, WAIT n, END = restart), then runs one proteus_snd sample.
//  - Writes every output sample to a capture ring in DDR3 at 0x31000000
//    (16 MB, 32 bits per sample: left s16 LE, right s16 LE) so the output
//    can be read back from Linux and compared with tools/proteus_model.py.
//
//  DDR3 accesses (one at a time): loader writes, capture writes, event
//  reads (burst 1), sample reads (burst 2).
//============================================================================

module demo_ctrl #(parameter SAMPLE_DIV = 2048)
(
	input             clk,
	input             reset,          // stops the demo; restart from event 0 after release
	input             run,            // image loaded

	// HPS download (8-bit ioctl)
	input             ioctl_download,
	input             ioctl_wr,
	input      [26:0] ioctl_addr,
	input       [7:0] ioctl_dout,
	output reg        ioctl_wait,

	// sound chip
	output reg        snd_wr,
	output reg  [4:0] snd_voice,
	output reg  [3:0] snd_word,
	output reg [15:0] snd_data,
	output reg        pan_wr,
	output reg  [7:0] pan_data,
	output reg        snd_start,
	input             snd_busy,
	input             snd_done,
	input signed [15:0] snd_l,
	input signed [15:0] snd_r,
	input             mem_req,
	input      [21:0] mem_addr,
	output reg        mem_ack,
	output reg [127:0] mem_win,      // 8 words from mem_addr (big-endian words swapped)

	// status
	output reg [15:0] overruns,
	output reg [31:0] samples,

	// DDR3
	input             DDRAM_BUSY,
	output reg  [7:0] DDRAM_BURSTCNT,
	output reg [28:0] DDRAM_ADDR,
	input      [63:0] DDRAM_DOUT,
	input             DDRAM_DOUT_READY,
	output reg        DDRAM_RD,
	output reg [63:0] DDRAM_DIN,
	output reg  [7:0] DDRAM_BE,
	output reg        DDRAM_WE
);

localparam [28:0] DDR_BASE    = 29'h6000000;          // 0x30000000 >> 3
localparam [28:0] EVENT_BASE  = DDR_BASE + 29'h100000; // +0x800000 bytes
localparam [28:0] CAPT_BASE   = DDR_BASE + 29'h200000; // +0x1000000 bytes

// ---------------------------------------------------------------- loader byte assembly
reg  [63:0] ld_word;
reg  [26:0] ld_addr;
reg         ld_pend;

// ---------------------------------------------------------------- capture buffer
reg  [31:0] cap_lo;
reg         cap_half;
reg  [63:0] cap_word;
reg         cap_pend;
reg  [20:0] cap_ptr;         // 2M 64-bit words = 16 MB, wraps

// ---------------------------------------------------------------- event fetch
reg         ev_req;          // need the 64-bit word holding event ev_idx
reg  [22:0] ev_idx;
reg  [21:0] ev_word_idx;     // cached word index
reg  [63:0] ev_word;
reg         ev_word_ok;
wire [31:0] ev = ev_idx[0] ? ev_word[63:32] : ev_word[31:0];

// ---------------------------------------------------------------- DDR arbiter
// Sample fetch: 3 beats (12 words) from the aligned address; the engine gets the 8 words
// starting at word sel. Words are big-endian in the image: word i = {byte 2i, byte 2i+1}.
function [127:0] win_sel(input [191:0] b, input [1:0] sel);
	integer k;
	reg [3:0] w;
	begin
		for (k = 0; k < 8; k = k + 1) begin
			w = {2'd0, sel} + k[3:0];
			win_sel[16 * k +: 16] = {b[16 * w +: 8], b[16 * w + 8 +: 8]};
		end
	end
endfunction

localparam M_IDLE = 2'd0, M_ISSUE = 2'd1, M_READ = 2'd2;
reg  [1:0] m_state;
reg  [1:0] m_client;         // 0 loader, 1 capture, 2 event, 3 sample
reg  [1:0] m_beat;
reg [127:0] m_b01;          // first two beats of a sample fetch
reg  [1:0] smp_sel;

always @(posedge clk) begin
	mem_ack <= 0;
	if (reset && !ioctl_download) begin
		m_state  <= M_IDLE;
		DDRAM_RD <= 0;
		DDRAM_WE <= 0;
	end
	else case (m_state)
	M_IDLE: begin
		DDRAM_BE <= 8'hFF;
		if (ld_pend) begin
			m_client <= 2'd0; DDRAM_ADDR <= DDR_BASE + {5'd0, ld_addr[26:3]}; DDRAM_DIN <= ld_word;
			DDRAM_BURSTCNT <= 8'd1; DDRAM_WE <= 1; m_state <= M_ISSUE;
		end
		else if (cap_pend) begin
			m_client <= 2'd1; DDRAM_ADDR <= CAPT_BASE + {8'd0, cap_ptr}; DDRAM_DIN <= cap_word;
			DDRAM_BURSTCNT <= 8'd1; DDRAM_WE <= 1; m_state <= M_ISSUE;
		end
		else if (ev_req) begin
			m_client <= 2'd2; DDRAM_ADDR <= EVENT_BASE + {7'd0, ev_idx[22:1]};
			DDRAM_BURSTCNT <= 8'd1; DDRAM_RD <= 1; m_state <= M_ISSUE;
		end
		else if (mem_req && !mem_ack) begin
			m_client <= 2'd3; DDRAM_ADDR <= DDR_BASE + {9'd0, mem_addr[21:2]}; smp_sel <= mem_addr[1:0];
			DDRAM_BURSTCNT <= 8'd3; DDRAM_RD <= 1; m_state <= M_ISSUE;
		end
	end

	M_ISSUE:
		if (!DDRAM_BUSY) begin
			DDRAM_RD <= 0;
			DDRAM_WE <= 0;
			m_beat   <= 0;
			m_state  <= DDRAM_RD ? M_READ : M_IDLE;
		end

	M_READ:
		if (DDRAM_DOUT_READY) begin
			m_beat <= m_beat + 2'd1;
			if (m_client == 2'd2) begin
				m_state <= M_IDLE;
			end
			else if (m_beat == 2'd0) begin
				m_b01[63:0] <= DDRAM_DOUT;
			end
			else if (m_beat == 2'd1) m_b01[127:64] <= DDRAM_DOUT;
			else begin
				mem_win <= win_sel({DDRAM_DOUT, m_b01}, smp_sel);
				mem_ack <= 1;
				m_state <= M_IDLE;
			end
		end

	default: m_state <= M_IDLE;
	endcase
end

// completion strobes for the other clients
wire wr_done = (m_state == M_ISSUE) && !DDRAM_BUSY && DDRAM_WE;
wire ev_done = (m_state == M_READ) && DDRAM_DOUT_READY && (m_client == 2'd2);

// ---------------------------------------------------------------- loader
always @(posedge clk) begin
	if (reset && !ioctl_download) begin
		ld_pend    <= 0;
		ioctl_wait <= 0;
	end
	else begin
		if (ioctl_download && ioctl_wr) begin
			ld_word[{ioctl_addr[2:0], 3'b000} +: 8] <= ioctl_dout;
			if (ioctl_addr[2:0] == 3'd7) begin
				ld_addr    <= ioctl_addr;
				ld_pend    <= 1;
				ioctl_wait <= 1;
			end
		end
		if (wr_done && m_client == 2'd0) begin
			ld_pend    <= 0;
			ioctl_wait <= 0;
		end
	end
end

// ---------------------------------------------------------------- sequencer
localparam Q_STOP = 3'd0, Q_TICK = 3'd1, Q_EVENT = 3'd2, Q_APPLY = 3'd3, Q_RUN = 3'd4, Q_WAITDONE = 3'd5;
reg  [2:0]  q_state;
reg  [11:0] div;
reg         tick;
reg  [30:0] wait_cnt;
reg         tick_pending;

always @(posedge clk) begin
	snd_wr    <= 0;
	pan_wr    <= 0;
	snd_start <= 0;

	if (reset || !run) begin
		q_state      <= Q_STOP;
		div          <= 0;
		tick         <= 0;
		tick_pending <= 0;
		wait_cnt     <= 0;
		ev_idx       <= 0;
		ev_req       <= 0;
		ev_word_ok   <= 0;
		cap_half     <= 0;
		cap_pend     <= 0;
		cap_ptr      <= 0;
		overruns     <= 0;
		samples      <= 0;
	end
	else begin
		// sample clock
		tick <= 0;
		if (div == SAMPLE_DIV - 1) begin div <= 0; tick <= 1; end
		else div <= div + 1'd1;
		if (tick) begin
			if (q_state != Q_STOP && q_state != Q_TICK) begin
				tick_pending <= 1;
				if (tick_pending) overruns <= overruns + 1'd1;
			end
			else tick_pending <= 1;
		end

		// event word fetch completion
		if (ev_done) begin
			ev_word     <= DDRAM_DOUT;
			ev_word_idx <= ev_idx[22:1];
			ev_word_ok  <= 1;
			ev_req      <= 0;
		end

		// capture write completion
		if (wr_done && m_client == 2'd1) begin
			cap_pend <= 0;
			cap_ptr  <= cap_ptr + 1'd1;
		end

		case (q_state)
		Q_STOP: q_state <= Q_TICK;

		Q_TICK:
			if (tick_pending) begin
				tick_pending <= 0;
				if (wait_cnt != 0) begin
					wait_cnt <= wait_cnt - 1'd1;
					q_state  <= Q_RUN;
				end
				else q_state <= Q_EVENT;
			end

		Q_EVENT:
			if (ev_word_ok && ev_word_idx == ev_idx[22:1]) q_state <= Q_APPLY;
			else if (!ev_req && !ev_done) begin ev_word_ok <= 0; ev_req <= 1; end

		Q_APPLY: begin
			if (ev == 32'd0) begin
				ev_idx  <= 0;                 // END: restart
				q_state <= Q_EVENT;
			end
			else if (!ev[31]) begin
				wait_cnt <= ev[30:0] - 1'd1;  // this sample is the first of the wait
				ev_idx   <= ev_idx + 1'd1;
				q_state  <= Q_RUN;
			end
			else begin
				if (ev[30]) begin
					pan_wr <= 1; snd_voice <= ev[29:25]; pan_data <= ev[7:0];
				end
				else begin
					snd_wr <= 1; snd_voice <= ev[29:25]; snd_word <= ev[24:21]; snd_data <= ev[15:0];
				end
				ev_idx  <= ev_idx + 1'd1;
				q_state <= Q_EVENT;
			end
		end

		Q_RUN: if (!snd_busy && !snd_wr && !pan_wr) begin snd_start <= 1; q_state <= Q_WAITDONE; end

		Q_WAITDONE:
			if (snd_done) begin
				samples <= samples + 1'd1;
				if (!cap_half) begin
					cap_lo   <= {snd_r, snd_l};
					cap_half <= 1;
				end
				else begin
					cap_word <= {snd_r, snd_l, cap_lo};
					cap_pend <= 1;
					cap_half <= 0;
				end
				q_state <= Q_TICK;
			end

		default: q_state <= Q_TICK;
		endcase
	end
end

endmodule
