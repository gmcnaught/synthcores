// proteus_core with a few internal signals brought out for sim/unit_tb.cpp.
module unit_bench
(
	input             clk,
	input             reset_in,
	input             ioctl_download,
	input      [15:0] ioctl_index,
	input             ioctl_wr,
	input      [26:0] ioctl_addr,
	input       [7:0] ioctl_dout,
	output            ioctl_wait,
	input             img_mounted,
	input             img_readonly,
	input      [63:0] img_size,
	output     [31:0] sd_lba,
	output      [5:0] sd_blk_cnt,
	output            sd_rd,
	output            sd_wr,
	input             sd_ack,
	input      [13:0] sd_buff_addr,
	input       [7:0] sd_buff_dout,
	input             sd_buff_wr,
	output      [7:0] sd_buff_din,
	input             save_req,
	input             autosave,
	input             osd_open,
	input             factory_reset,
	input             DDRAM_BUSY,
	output      [7:0] DDRAM_BURSTCNT,
	output     [28:0] DDRAM_ADDR,
	input      [63:0] DDRAM_DOUT,
	input             DDRAM_DOUT_READY,
	output            DDRAM_RD,
	output     [63:0] DDRAM_DIN,
	output      [7:0] DDRAM_BE,
	output            DDRAM_WE,
	output            dbg_wr_strobe,
	output     [23:0] dbg_wr_addr,
	output     [15:0] dbg_wr_data,
	output      [1:0] dbg_wr_ds,
	// internals
	output            macro_active,
	output            cpu_pause,
	output            fifo_wr,         // proteus_sys -> snd_ctrl write FIFO
	output            fifo_pan,
	output      [4:0] fifo_voice,
	output     [15:0] fifo_data
);
proteus_core core
(
	.clk(clk), .reset_in(reset_in),
	.ioctl_download(ioctl_download), .ioctl_index(ioctl_index), .ioctl_wr(ioctl_wr), .ioctl_addr(ioctl_addr),
	.ioctl_dout(ioctl_dout), .ioctl_wait(ioctl_wait),
	.img_mounted(img_mounted), .img_readonly(img_readonly), .img_size(img_size),
	.sd_lba(sd_lba), .sd_blk_cnt(sd_blk_cnt), .sd_rd(sd_rd), .sd_wr(sd_wr), .sd_ack(sd_ack),
	.sd_buff_addr(sd_buff_addr), .sd_buff_dout(sd_buff_dout), .sd_buff_wr(sd_buff_wr), .sd_buff_din(sd_buff_din),
	.volume_sel(2'd0), .gain_sel(2'd0), .save_req(save_req), .autosave(autosave), .osd_open(osd_open),
	.factory_reset(factory_reset), .tone_en(1'b1),
	.ps2_key(11'd0), .joy(32'd0), .midi_in(1'b1), .midi_out(),
	.audio_l(), .audio_r(), .ce_pix(), .hsync(), .vsync(), .hblank(), .vblank(), .r(), .g(), .b(),
	.loaded_out(), .saving(),
	.DDRAM_BUSY(DDRAM_BUSY), .DDRAM_BURSTCNT(DDRAM_BURSTCNT), .DDRAM_ADDR(DDRAM_ADDR), .DDRAM_DOUT(DDRAM_DOUT),
	.DDRAM_DOUT_READY(DDRAM_DOUT_READY), .DDRAM_RD(DDRAM_RD), .DDRAM_DIN(DDRAM_DIN), .DDRAM_BE(DDRAM_BE), .DDRAM_WE(DDRAM_WE),
	.dbg_wr_strobe(dbg_wr_strobe), .dbg_wr_addr(dbg_wr_addr), .dbg_wr_data(dbg_wr_data), .dbg_wr_ds(dbg_wr_ds),
	.dbg_sample(), .dbg_out_l(), .dbg_out_r()
);
assign macro_active = core.macro_active;
assign cpu_pause    = core.cpu_pause;
assign fifo_wr      = core.snd_wr;
assign fifo_pan     = core.snd_pan;
assign fifo_voice   = core.snd_voice;
assign fifo_data    = core.snd_data;
endmodule
