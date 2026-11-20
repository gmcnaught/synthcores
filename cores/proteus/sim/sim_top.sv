// Simulation harness: demo controller + sound chip, DDR3 port exposed to C++.
module sim_top
(
	input             clk,
	input             reset,
	input             run,
	input             ioctl_download,
	input             ioctl_wr,
	input      [26:0] ioctl_addr,
	input       [7:0] ioctl_dout,
	output            ioctl_wait,
	input             DDRAM_BUSY,
	output      [7:0] DDRAM_BURSTCNT,
	output     [28:0] DDRAM_ADDR,
	input      [63:0] DDRAM_DOUT,
	input             DDRAM_DOUT_READY,
	output            DDRAM_RD,
	output     [63:0] DDRAM_DIN,
	output      [7:0] DDRAM_BE,
	output            DDRAM_WE,
	output            snd_done,
	output signed [15:0] snd_l,
	output signed [15:0] snd_r,
	output     [15:0] overruns
);
wire snd_wr, pan_wr, snd_start, snd_busy, mem_req, mem_ack;
wire [4:0] snd_voice; wire [3:0] snd_word; wire [15:0] snd_data; wire [7:0] pan_data;
wire [21:0] mem_addr; wire [15:0] mem_s0, mem_s1; wire [31:0] samples;

demo_ctrl ctrl
(
	.clk(clk), .reset(reset), .run(run),
	.ioctl_download(ioctl_download), .ioctl_wr(ioctl_wr), .ioctl_addr(ioctl_addr), .ioctl_dout(ioctl_dout), .ioctl_wait(ioctl_wait),
	.snd_wr(snd_wr), .snd_voice(snd_voice), .snd_word(snd_word), .snd_data(snd_data),
	.pan_wr(pan_wr), .pan_data(pan_data), .snd_start(snd_start), .snd_busy(snd_busy), .snd_done(snd_done),
	.snd_l(snd_l), .snd_r(snd_r), .mem_req(mem_req), .mem_addr(mem_addr), .mem_ack(mem_ack), .mem_s0(mem_s0), .mem_s1(mem_s1),
	.overruns(overruns), .samples(samples),
	.DDRAM_BUSY(DDRAM_BUSY), .DDRAM_BURSTCNT(DDRAM_BURSTCNT), .DDRAM_ADDR(DDRAM_ADDR), .DDRAM_DOUT(DDRAM_DOUT),
	.DDRAM_DOUT_READY(DDRAM_DOUT_READY), .DDRAM_RD(DDRAM_RD), .DDRAM_DIN(DDRAM_DIN), .DDRAM_BE(DDRAM_BE), .DDRAM_WE(DDRAM_WE)
);

proteus_snd snd
(
	.clk(clk), .reset(reset),
	.wr(snd_wr), .wr_voice(snd_voice), .wr_word(snd_word), .wr_data(snd_data), .wr_mask(16'hFFFF),
	.rd_voice(5'd0), .rd_word(4'd0), .rd_raw(), .rd_start(1'b0), .rd_done(), .rd_data(),
	.pan_wr(pan_wr), .pan_voice(snd_voice), .pan_data(pan_data),
	.start(snd_start), .busy(snd_busy), .done(snd_done), .out_l(snd_l), .out_r(snd_r),
	.mem_req(mem_req), .mem_addr(mem_addr), .mem_ack(mem_ack), .mem_s0(mem_s0), .mem_s1(mem_s1),
	.disp_voice(5'd0), .disp_level()
);
endmodule
