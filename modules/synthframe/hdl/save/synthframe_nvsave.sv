// SPDX-License-Identifier: MIT
// synthframe: battery-backed RAM <-> the MiSTer save file (hps_io SD slot 0)
//
// A core whose image is loaded through an "FS" entry in CONF_STR ("FSC" also reloads the last
// image when the core starts) gets saves/<core>/<image>.sav mounted on slot 0 by Main_MiSTer
// at the end of the download, while ioctl_download is still high. This module:
//   - restore: after the download, when the image is good and the mounted file holds at least
//     2**AW bytes, reads it into the RAM through mem_we/mem_addr/mem_wdata; boot_hold keeps
//     the core in reset until then (a missing or short file leaves the RAM as it is);
//   - save: on a rising save_req, or when the OSD opens with autosave set, raises pause, waits
//     until the core reports idle (no step in progress), and writes the RAM to the file,
//     reading mem_rdata at mem_addr (the byte one clock after the address);
//   - writes nothing to a read-only file.
// The file is the RAM's bytes in address order, 2**AW bytes, moved in requests of up to 16 KB
// (hps_io's limit; sd_blk_cnt + 1 blocks of 512 bytes).
module synthframe_nvsave #(
    parameter int AW = 15                 // RAM size 2**AW bytes, 9 .. 20
) (
    input  logic          clk,
    input  logic          rst,
    input  logic          loading,        // image download in progress (ioctl_download)
    input  logic          image_ok,       // a usable image is loaded

    // hps_io SD slot 0
    input  logic          img_mounted,
    input  logic          img_readonly,
    input  logic [63:0]   img_size,
    output logic [31:0]   sd_lba,
    output logic [5:0]    sd_blk_cnt,
    output logic          sd_rd,
    output logic          sd_wr,
    input  logic          sd_ack,
    input  logic [13:0]   sd_buff_addr,
    input  logic [7:0]    sd_buff_dout,
    input  logic          sd_buff_wr,
    output logic [7:0]    sd_buff_din,

    // triggers
    input  logic          save_req,       // level; the rising edge saves
    input  logic          osd_open,       // OSD_STATUS
    input  logic          autosave,       // save when the OSD opens

    // the RAM
    output logic          mem_we,
    output logic [AW-1:0] mem_addr,
    output logic [7:0]    mem_wdata,
    input  logic [7:0]    mem_rdata,      // byte at mem_addr, one clock later

    // the core
    input  logic          core_idle,      // no step in progress
    output logic          boot_hold,      // keep the core in reset (restoring)
    output logic          pause,          // start no steps (saving)
    output logic          saving,
    output logic          restored        // the RAM came from the file at the last load
);
    localparam int CAW = (AW < 14) ? AW : 14;             // bytes per request: 2**CAW
    localparam int NCH = 1 << (AW - CAW);                  // requests per file
    localparam logic [31:0] LBA_STEP = 32'(1 << (CAW - 9));

    assign sd_blk_cnt = 6'((1 << (CAW - 9)) - 1);
    logic [19:0] chunk;                                    // request number
    assign mem_addr  = AW'({chunk, sd_buff_addr[CAW-1:0]} );
    assign mem_wdata = sd_buff_dout;
    assign sd_buff_din = mem_rdata;

    typedef enum logic [2:0] { S_IDLE, S_RD, S_RD_ACK, S_WAIT, S_WR, S_WR_ACK } st_t;
    st_t st;
    assign mem_we = (st == S_RD || st == S_RD_ACK) && sd_ack && sd_buff_wr;

    logic mounted, ro, have, save_d, osd_d, loading_d;
    logic [3:0] quiet;

    always_ff @(posedge clk) begin
        save_d <= save_req;
        osd_d <= osd_open;
        loading_d <= loading;
        if (img_mounted) begin
            mounted <= 1'b1;
            ro <= img_readonly;
            have <= img_size >= 64'(1 << AW);
        end
        if (rst || (loading && !loading_d)) begin
            mounted <= 1'b0;
            have <= 1'b0;
            boot_hold <= 1'b1;
            restored <= 1'b0;
            st <= S_IDLE;
            sd_rd <= 1'b0;
            sd_wr <= 1'b0;
            pause <= 1'b0;
            saving <= 1'b0;
        end else begin
            case (st)
                S_IDLE:
                    if (boot_hold) begin
                        if (!loading && image_ok) begin
                            if (mounted && have) begin
                                chunk <= '0;
                                sd_lba <= '0;
                                sd_rd <= 1'b1;
                                st <= S_RD;
                            end else boot_hold <= 1'b0;
                        end
                    end else if (image_ok && !loading && mounted && !ro &&
                                 ((save_req && !save_d) || (autosave && osd_open && !osd_d))) begin
                        pause <= 1'b1;
                        saving <= 1'b1;
                        quiet <= '0;
                        st <= S_WAIT;
                    end
                // restore
                S_RD: if (sd_ack) begin sd_rd <= 1'b0; st <= S_RD_ACK; end
                S_RD_ACK:
                    if (!sd_ack) begin
                        if (chunk == 20'(NCH - 1)) begin
                            boot_hold <= 1'b0;
                            restored <= 1'b1;
                            st <= S_IDLE;
                        end else begin
                            chunk <= chunk + 20'd1;
                            sd_lba <= sd_lba + LBA_STEP;
                            sd_rd <= 1'b1;
                            st <= S_RD;
                        end
                    end
                // save: the core idle for 8 clocks with pause up (no step can have started)
                S_WAIT:
                    if (!core_idle) quiet <= '0;
                    else if (quiet != 4'd8) quiet <= quiet + 4'd1;
                    else begin
                        chunk <= '0;
                        sd_lba <= '0;
                        sd_wr <= 1'b1;
                        st <= S_WR;
                    end
                S_WR: if (sd_ack) begin sd_wr <= 1'b0; st <= S_WR_ACK; end
                S_WR_ACK:
                    if (!sd_ack) begin
                        if (chunk == 20'(NCH - 1)) begin
                            pause <= 1'b0;
                            saving <= 1'b0;
                            have <= 1'b1;
                            st <= S_IDLE;
                        end else begin
                            chunk <= chunk + 20'd1;
                            sd_lba <= sd_lba + LBA_STEP;
                            sd_wr <= 1'b1;
                            st <= S_WR;
                        end
                    end
                default: st <= S_IDLE;
            endcase
        end
    end
endmodule
