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
//  - Status word at 0x30FFFF00, rewritten every 4096 samples (64-bit LE):
//    bits 31..0 samples produced, 47..32 missed sample deadlines (a tick arrived
//    while the previous one was still pending), 63..48 worst clocks from a tick
//    to its sample (of 2048 available).
//============================================================================

module snd_ctrl #(parameter SAMPLE_DIV = 2048)
(
	input             clk,
	input             reset,          // sound + sequencing reset
	input             loading,        // image download in progress (DDR loader active)
	input             tone_en,        // tone filter on (OSD)

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
	output      [7:0] DDRAM_BURSTCNT,
	output     [28:0] DDRAM_ADDR,
	input      [63:0] DDRAM_DOUT,
	input             DDRAM_DOUT_READY,
	output            DDRAM_RD,
	output     [63:0] DDRAM_DIN,
	output      [7:0] DDRAM_BE,
	output            DDRAM_WE
);

localparam [28:0] DDR_BASE  = 29'h6000000;          // 0x30000000 >> 3
localparam [28:0] CAPT_BASE = DDR_BASE + 29'h200000; // 0x31000000
localparam [28:0] STAT_ADDR = DDR_BASE + 29'h1FFFE0; // 0x30FFFF00

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
reg [127:0] mem_win;

proteus_snd snd
(
	.clk(clk), .reset(reset), .tone_en(tone_en),
	.wr(snd_wr), .wr_voice(snd_voice), .wr_word(snd_word), .wr_data(snd_data), .wr_mask(snd_mask),
	.pan_wr(pan_wr), .pan_voice(snd_voice), .pan_data(snd_data[7:0]),
	.rd_voice(rb_start ? cpu_voice : cpu_voice), .rd_word(cpu_word), .rd_raw(cpu_raw),
	.rd_start(rb_start), .rd_done(rb_done), .rd_data(cpu_rb_data),
	.start(snd_start), .busy(snd_busy), .done(snd_done), .out_l(snd_l), .out_r(snd_r),
	.mem_req(mem_req), .mem_addr(mem_addr), .mem_ack(mem_ack), .mem_win(mem_win),
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
reg  [31:0] st_samples;
reg  [15:0] st_missed;
reg  [15:0] st_worst;
reg  [15:0] st_busy;         // clocks since the current tick
reg         st_pend;

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

// synthframe memory port: clients 0 loader, 1 capture, 2 status (writes), 3 sample fetch
// (a 3-word read), in that priority order, onto synthframe_mem_ddr3. The port runs while
// the core is held in reset for a download (rst = reset && !loading, as before).
wire         c_req_valid [4], c_req_ready [4], c_req_we [4], c_rsp_valid [4];
wire  [28:0] c_req_addr [4];
wire  [63:0] c_req_wdata [4], c_rsp_data;
wire   [7:0] c_req_be [4], c_req_len [4];
wire   [0:0] c_req_tag [4], c_rsp_tag;
wire         m_req_valid, m_req_ready, m_req_we, m_rsp_valid;
wire  [28:0] m_req_addr;
wire  [63:0] m_req_wdata, m_rsp_data;
wire   [7:0] m_req_be, m_req_len;
wire   [2:0] m_req_tag, m_rsp_tag;

synthframe_mem_arb #(.N(4), .CTW(1)) mem_arb
(
	.c_req_valid(c_req_valid), .c_req_ready(c_req_ready), .c_req_we(c_req_we), .c_req_addr(c_req_addr),
	.c_req_wdata(c_req_wdata), .c_req_be(c_req_be), .c_req_len(c_req_len), .c_req_tag(c_req_tag),
	.c_rsp_valid(c_rsp_valid), .c_rsp_data(c_rsp_data), .c_rsp_tag(c_rsp_tag),
	.req_valid(m_req_valid), .req_ready(m_req_ready), .req_we(m_req_we), .req_addr(m_req_addr),
	.req_wdata(m_req_wdata), .req_be(m_req_be), .req_len(m_req_len), .req_tag(m_req_tag),
	.rsp_valid(m_rsp_valid), .rsp_data(m_rsp_data), .rsp_tag(m_rsp_tag)
);

synthframe_mem_ddr3 #(.TW(3), .QBITS(2)) mem
(
	.clk(clk), .rst(reset && !loading),
	.req_valid(m_req_valid), .req_ready(m_req_ready), .req_we(m_req_we), .req_addr(m_req_addr),
	.req_wdata(m_req_wdata), .req_be(m_req_be), .req_len(m_req_len), .req_tag(m_req_tag),
	.rsp_valid(m_rsp_valid), .rsp_data(m_rsp_data), .rsp_tag(m_rsp_tag),
	.DDRAM_BUSY(DDRAM_BUSY), .DDRAM_BURSTCNT(DDRAM_BURSTCNT), .DDRAM_ADDR(DDRAM_ADDR),
	.DDRAM_DOUT(DDRAM_DOUT), .DDRAM_DOUT_READY(DDRAM_DOUT_READY), .DDRAM_RD(DDRAM_RD),
	.DDRAM_DIN(DDRAM_DIN), .DDRAM_BE(DDRAM_BE), .DDRAM_WE(DDRAM_WE)
);

reg          s_wait;         // the sample fetch is issued, words coming back
reg   [1:0]  s_beat;
reg [127:0]  m_b01;          // first two words of a sample fetch
reg   [1:0]  smp_sel;

assign c_req_valid[0] = ld_pend;
assign c_req_addr[0]  = DDR_BASE + {5'd0, ld_addr[26:3]};
assign c_req_wdata[0] = ld_word;
assign c_req_valid[1] = cap_pend;
assign c_req_addr[1]  = CAPT_BASE + {8'd0, cap_ptr};
assign c_req_wdata[1] = cap_word;
assign c_req_valid[2] = st_pend;
assign c_req_addr[2]  = STAT_ADDR;
assign c_req_wdata[2] = {st_worst, st_missed, st_samples};
assign c_req_valid[3] = mem_req && !mem_ack && !s_wait;
assign c_req_addr[3]  = DDR_BASE + {9'd0, mem_addr[21:2]};
assign c_req_wdata[3] = 64'd0;
genvar gi;
generate for (gi = 0; gi < 4; gi = gi + 1) begin : g_client
	assign c_req_we[gi]  = gi != 3;
	assign c_req_be[gi]  = 8'hFF;
	assign c_req_len[gi] = gi == 3 ? 8'd3 : 8'd1;
	assign c_req_tag[gi] = 1'b0;
end endgenerate

always @(posedge clk) begin
	mem_ack <= 0;
	if (reset && !loading) begin
		s_wait <= 0;
	end
	else begin
		if (c_req_valid[3] && c_req_ready[3]) begin
			s_wait  <= 1;
			s_beat  <= 0;
			smp_sel <= mem_addr[1:0];
		end
		if (c_rsp_valid[3]) begin
			s_beat <= s_beat + 2'd1;
			if (s_beat == 2'd0) m_b01[63:0] <= c_rsp_data;
			else if (s_beat == 2'd1) m_b01[127:64] <= c_rsp_data;
			else begin
				mem_win <= win_sel({c_rsp_data, m_b01}, smp_sel);
				mem_ack <= 1;
				s_wait  <= 0;
			end
		end
	end
end

wire ld_done  = c_req_valid[0] && c_req_ready[0];
wire cap_done = c_req_valid[1] && c_req_ready[1];
wire st_done  = c_req_valid[2] && c_req_ready[2];

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
		if (ld_done) begin
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
		st_samples   <= 0;
		st_missed    <= 0;
		st_worst     <= 0;
		st_busy      <= 0;
		st_pend      <= 0;
	end
	else begin
		if (cpu_wr && !cpu_wr_full) f_wp <= f_wp + 1'd1;
		if (cpu_rb) rb_pending <= 1;

		if (div == SAMPLE_DIV - 1) begin
			div <= 0;
			tick_pending <= 1;
			if (tick_pending) st_missed <= st_missed + 1'd1;
		end
		else div <= div + 1'd1;
		if (st_busy != 16'hFFFF) st_busy <= st_busy + 1'd1;
		if (st_done) st_pend <= 0;

		if (cap_done) begin
			cap_pend <= 0;
			cap_ptr  <= cap_ptr + 1'd1;
		end

		case (q_state)
		Q_IDLE:
			if (tick_pending) begin
				tick_pending <= 0;
				st_busy <= 0;
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
				st_samples <= st_samples + 1'd1;
				if (st_busy > st_worst) st_worst <= st_busy;
				if (st_samples[11:0] == 12'hFFF) st_pend <= 1;
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
