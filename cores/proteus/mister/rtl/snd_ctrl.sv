//============================================================================
//  Sound subsystem controller for the full Proteus system
//
//  - CPU register writes (0x800000) and pan writes (0x500000) go into a FIFO
//    that is drained between output samples, so a write affects the next
//    sample, as in MAME (stream update before each write).
//  - CPU reads: raw register words directly; +0x18 read-back waits for the
//    FIFO to be empty and the engine to be idle, then runs a read-back.
//  - DDR3: image loader (byte offsets < 8 MB), sample reads, output capture
//    ring at 0x31000000 (left/right s16 LE per sample).
//============================================================================

module snd_ctrl #(parameter SAMPLE_DIV = 2048)
(
	input             clk,
	input             reset,          // sound + sequencing reset
	input             loading,        // image download in progress (DDR loader active)

	// HPS download
	input             ioctl_wr,
	input      [26:0] ioctl_addr,
	input       [7:0] ioctl_dout,
	output reg        ioctl_wait,

	// CPU side
	input             cpu_wr,          // single-cycle strobe
	input             cpu_pan,         // 1: pan write (voice, byte), 0: register write
	input       [4:0] cpu_voice,
	input       [3:0] cpu_word,
	input      [15:0] cpu_data,
	input      [15:0] cpu_mask,
	output            cpu_wr_full,     // CPU must wait
	input             cpu_rb,          // single-cycle strobe: +0x18 read-back of cpu_voice
	output reg        cpu_rb_done,
	output     [15:0] cpu_rb_data,
	output     [15:0] cpu_raw,         // raw register word cpu_voice/cpu_word

	// audio
	output reg signed [15:0] out_l,
	output reg signed [15:0] out_r,
	output reg        sample_strobe,

	// display
	input       [4:0] disp_voice,
	output      [7:0] disp_level,

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

localparam [28:0] DDR_BASE  = 29'h6000000;          // 0x30000000 >> 3
localparam [28:0] CAPT_BASE = DDR_BASE + 29'h200000; // 0x31000000

// ---------------------------------------------------------------- sound chip
reg         snd_wr, pan_wr, snd_start, rb_start;
reg  [4:0]  snd_voice;
reg  [3:0]  snd_word;
reg  [15:0] snd_data, snd_mask;
wire        snd_busy, snd_done, rb_done;
wire signed [15:0] snd_l, snd_r;
wire        mem_req;
wire [21:0] mem_addr;
reg         mem_ack;
reg  [15:0] mem_s0, mem_s1;

proteus_snd snd
(
	.clk(clk), .reset(reset),
	.wr(snd_wr), .wr_voice(snd_voice), .wr_word(snd_word), .wr_data(snd_data), .wr_mask(snd_mask),
	.pan_wr(pan_wr), .pan_voice(snd_voice), .pan_data(snd_data[7:0]),
	.rd_voice(rb_start ? cpu_voice : cpu_voice), .rd_word(cpu_word), .rd_raw(cpu_raw),
	.rd_start(rb_start), .rd_done(rb_done), .rd_data(cpu_rb_data),
	.start(snd_start), .busy(snd_busy), .done(snd_done), .out_l(snd_l), .out_r(snd_r),
	.mem_req(mem_req), .mem_addr(mem_addr), .mem_ack(mem_ack), .mem_s0(mem_s0), .mem_s1(mem_s1),
	.disp_voice(disp_voice), .disp_level(disp_level)
);

// ---------------------------------------------------------------- write FIFO
reg  [41:0] fifo [0:63];
reg  [5:0]  f_wp, f_rp;
wire [6:0]  f_count = {1'b0, f_wp} - {1'b0, f_rp};
wire        f_empty = (f_wp == f_rp);
assign cpu_wr_full = ((f_wp + 6'd1) == f_rp);

always @(posedge clk) begin
	if (cpu_wr && !cpu_wr_full) fifo[f_wp] <= {cpu_pan, cpu_voice, cpu_word, cpu_mask, cpu_data};
end

// ---------------------------------------------------------------- loader / capture buffers
reg  [63:0] ld_word;
reg  [26:0] ld_addr;
reg         ld_pend;
reg  [31:0] cap_lo;
reg         cap_half;
reg  [63:0] cap_word;
reg         cap_pend;
reg  [20:0] cap_ptr;

// ---------------------------------------------------------------- DDR arbiter
localparam M_IDLE = 2'd0, M_ISSUE = 2'd1, M_READ = 2'd2;
reg  [1:0] m_state;
reg  [1:0] m_client;         // 0 loader, 1 capture, 3 sample
reg        m_beat;
reg  [1:0] smp_sel;

always @(posedge clk) begin
	mem_ack <= 0;
	if (reset && !loading) begin
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
		else if (mem_req && !mem_ack) begin
			m_client <= 2'd3; DDRAM_ADDR <= DDR_BASE + {9'd0, mem_addr[21:2]}; smp_sel <= mem_addr[1:0];
			DDRAM_BURSTCNT <= 8'd2; DDRAM_RD <= 1; m_state <= M_ISSUE;
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
			m_beat <= 1;
			if (!m_beat) begin
				// words are big-endian in the image: word i = {byte 2i, byte 2i+1}
				case (smp_sel)
					2'd0: begin mem_s0 <= {DDRAM_DOUT[ 7: 0], DDRAM_DOUT[15: 8]}; mem_s1 <= {DDRAM_DOUT[23:16], DDRAM_DOUT[31:24]}; end
					2'd1: begin mem_s0 <= {DDRAM_DOUT[23:16], DDRAM_DOUT[31:24]}; mem_s1 <= {DDRAM_DOUT[39:32], DDRAM_DOUT[47:40]}; end
					2'd2: begin mem_s0 <= {DDRAM_DOUT[39:32], DDRAM_DOUT[47:40]}; mem_s1 <= {DDRAM_DOUT[55:48], DDRAM_DOUT[63:56]}; end
					2'd3: mem_s0 <= {DDRAM_DOUT[55:48], DDRAM_DOUT[63:56]};
				endcase
			end
			else begin
				if (smp_sel == 2'd3) mem_s1 <= {DDRAM_DOUT[7:0], DDRAM_DOUT[15:8]};
				mem_ack <= 1;
				m_state <= M_IDLE;
			end
		end

	default: m_state <= M_IDLE;
	endcase
end

wire wr_done = (m_state == M_ISSUE) && !DDRAM_BUSY && DDRAM_WE;

// ---------------------------------------------------------------- loader (DDR part: offsets < 8 MB)
always @(posedge clk) begin
	if (!loading) begin
		ld_pend    <= 0;
		ioctl_wait <= 0;
	end
	else begin
		if (ioctl_wr && ioctl_addr < 27'h800000) begin
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
localparam Q_IDLE = 3'd0, Q_DRAIN = 3'd1, Q_RUN = 3'd2, Q_WAIT = 3'd3, Q_RB = 3'd4;
reg  [2:0]  q_state;
reg  [11:0] div;
reg         tick_pending;
reg         rb_pending;
reg  [1:0]  wgap;            // proteus_snd needs 2 clocks between writes and before start

always @(posedge clk) begin
	snd_wr        <= 0;
	pan_wr        <= 0;
	snd_start     <= 0;
	rb_start      <= 0;
	cpu_rb_done   <= 0;
	sample_strobe <= 0;
	wgap <= (snd_wr || pan_wr) ? 2'd2 : (wgap != 0 ? wgap - 1'd1 : 2'd0);

	if (reset) begin
		q_state      <= Q_IDLE;
		div          <= 0;
		tick_pending <= 0;
		rb_pending   <= 0;
		f_wp         <= 0;
		f_rp         <= 0;
		cap_half     <= 0;
		cap_pend     <= 0;
		cap_ptr      <= 0;
		out_l        <= 0;
		out_r        <= 0;
	end
	else begin
		if (cpu_wr && !cpu_wr_full) f_wp <= f_wp + 1'd1;
		if (cpu_rb) rb_pending <= 1;

		if (div == SAMPLE_DIV - 1) begin div <= 0; tick_pending <= 1; end
		else div <= div + 1'd1;

		if (wr_done && m_client == 2'd1) begin
			cap_pend <= 0;
			cap_ptr  <= cap_ptr + 1'd1;
		end

		case (q_state)
		Q_IDLE:
			if (tick_pending) begin
				tick_pending <= 0;
				q_state <= Q_DRAIN;
			end
			else if (rb_pending && f_empty && !snd_busy && wgap == 0) begin
				rb_pending <= 0;
				rb_start   <= 1;
				q_state    <= Q_RB;
			end

		Q_DRAIN:
			if (snd_wr || pan_wr) ;   // one write every other clock
			else if (!f_empty) begin
				{pan_wr, snd_voice, snd_word, snd_mask, snd_data} <= {fifo[f_rp][41], fifo[f_rp][40:0]};
				snd_wr <= !fifo[f_rp][41];
				f_rp   <= f_rp + 1'd1;
			end
			else q_state <= Q_RUN;

		Q_RUN:
			if (!snd_busy && !snd_wr && !pan_wr && wgap == 0) begin snd_start <= 1; q_state <= Q_WAIT; end

		Q_WAIT:
			if (snd_done) begin
				out_l <= snd_l;
				out_r <= snd_r;
				sample_strobe <= 1;
				if (!cap_half) begin
					cap_lo   <= {snd_r, snd_l};
					cap_half <= 1;
				end
				else begin
					cap_word <= {snd_r, snd_l, cap_lo};
					cap_pend <= 1;
					cap_half <= 0;
				end
				q_state <= Q_IDLE;
			end

		Q_RB:
			if (rb_done) begin
				cpu_rb_done <= 1;
				q_state     <= Q_IDLE;
			end

		default: q_state <= Q_IDLE;
		endcase
	end
end

endmodule
