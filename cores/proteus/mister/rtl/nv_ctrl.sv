//============================================================================
//  Battery RAM persistence through the MiSTer save file (hps_io SD slot 0)
//
//  The system image is loaded with "FS1" in CONF_STR, so Main_MiSTer mounts
//  saves/Proteus/<image>.sav on slot 0 right after the download (the file is
//  created on the first write). This module:
//   - at boot: if the mounted save is at least 64 KB, reads it into the RAM
//     (4 x 16 KB requests) before the CPU leaves reset; otherwise the RAM keeps
//     the image's contents
//   - on save (OSD "Save Presets", or autosave when the OSD opens): pauses the
//     CPU and writes the 64 KB RAM to the file (4 x 16 KB requests). No save starts
//     while `inhibit` is set (the preset-initialisation macro drives the panel open
//     loop and must not lose CPU time); a trigger in that time is dropped.
//  File layout = RAM bytes in 68000 order (byte 2n = high byte of word n),
//  the same as the image's RAM section.
//============================================================================

module nv_ctrl
(
	input             clk,
	input             reset,          // core reset (not the download)
	input             loading,        // system image download in progress
	input             loaded,         // image present

	// hps_io SD slot 0
	input             img_mounted,
	input             img_readonly,
	input      [63:0] img_size,
	output reg [31:0] sd_lba,
	output      [5:0] sd_blk_cnt,
	output reg        sd_rd,
	output reg        sd_wr,
	input             sd_ack,
	input      [13:0] sd_buff_addr,
	input       [7:0] sd_buff_dout,
	input             sd_buff_wr,
	output      [7:0] sd_buff_din,

	// triggers
	input             save_req,       // OSD menu item (level, rising edge used)
	input             osd_open,       // OSD_STATUS
	input             autosave,       // save when the OSD opens
	input             inhibit,        // do not start a save

	// RAM access (proteus_sys)
	output reg        nv_active,      // RAM port belongs to this module
	output            nv_we,
	output     [15:0] nv_addr,        // byte address in the 64 KB RAM
	output      [7:0] nv_wdata,
	input       [7:0] nv_rdata,       // byte at nv_addr, one clock later

	output reg        boot_hold = 1,  // keep the CPU in reset (restoring the save)
	output reg        cpu_pause,      // freeze the CPU (saving)
	output reg        saving,
	output reg        restored        // the RAM came from the save file at the last boot
);

assign sd_blk_cnt = 6'd31;            // 32 x 512 = 16 KB per request
reg  [1:0] chunk;
assign nv_addr     = {chunk, sd_buff_addr};
assign nv_we       = nv_active && boot_hold && sd_ack && sd_buff_wr;
assign nv_wdata    = sd_buff_dout;
assign sd_buff_din = nv_rdata;

reg mounted, have_save, ro;
reg save_d, osd_d, loading_d;
reg [4:0] settle;

localparam N_IDLE = 3'd0, N_RD_REQ = 3'd1, N_RD_ACK = 3'd2, N_PAUSE = 3'd3, N_WR_REQ = 3'd4, N_WR_ACK = 3'd5;
reg [2:0] state;

always @(posedge clk) begin
	save_d    <= save_req;
	osd_d     <= osd_open;
	loading_d <= loading;

	// the save file is mounted while the image download is still running
	if (img_mounted) begin
		mounted   <= 1;
		ro        <= img_readonly;
		have_save <= (img_size >= 64'd65536);
	end

	if (loading && !loading_d) begin
		mounted   <= 0;
		have_save <= 0;
		boot_hold <= 1;
		restored  <= 0;
		state     <= N_IDLE;
		nv_active <= 0;
		sd_rd     <= 0;
		sd_wr     <= 0;
		cpu_pause <= 0;
		saving    <= 0;
	end
	else case (state)
		N_IDLE: begin
			if (loaded && boot_hold) begin
				if (mounted && have_save) begin
					nv_active <= 1;
					chunk     <= 0;
					sd_lba    <= 0;
					sd_rd     <= 1;
					state     <= N_RD_REQ;
				end
				else boot_hold <= 0;
			end
			else if (loaded && !reset && !inhibit && mounted && !ro &&
			         ((save_req && !save_d) || (autosave && osd_open && !osd_d))) begin
				cpu_pause <= 1;
				saving    <= 1;
				settle    <= 5'd31;
				state     <= N_PAUSE;
			end
		end

		// ---------------- restore
		N_RD_REQ: if (sd_ack) begin sd_rd <= 0; state <= N_RD_ACK; end
		N_RD_ACK:
			if (!sd_ack) begin
				if (chunk == 2'd3) begin
					nv_active <= 0;
					boot_hold <= 0;
					restored  <= 1;
					state     <= N_IDLE;
				end
				else begin
					chunk  <= chunk + 1'd1;
					sd_lba <= {25'd0, chunk + 2'd1, 5'd0};
					sd_rd  <= 1;
					state  <= N_RD_REQ;
				end
			end

		// ---------------- save
		N_PAUSE:
			// let any bus access that already latched its data finish before taking the RAM port
			if (settle != 0) settle <= settle - 1'd1;
			else begin
				nv_active <= 1;
				chunk     <= 0;
				sd_lba    <= 0;
				sd_wr     <= 1;
				state     <= N_WR_REQ;
			end
		N_WR_REQ: if (sd_ack) begin sd_wr <= 0; state <= N_WR_ACK; end
		N_WR_ACK:
			if (!sd_ack) begin
				if (chunk == 2'd3) begin
					nv_active <= 0;
					cpu_pause <= 0;
					saving    <= 0;
					have_save <= 1;
					state     <= N_IDLE;
				end
				else begin
					chunk  <= chunk + 1'd1;
					sd_lba <= {25'd0, chunk + 2'd1, 5'd0};
					sd_wr  <= 1;
					state  <= N_WR_REQ;
				end
			end
		default: state <= N_IDLE;
	endcase
end

endmodule
