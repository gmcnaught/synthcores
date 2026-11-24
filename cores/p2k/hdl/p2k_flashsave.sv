// SPDX-License-Identifier: GPL-3.0-or-later
// Proteus 2000 flash <-> the MiSTer save file (hps_io SD slot 0), as synthframe_nvsave but for the
// 2 MB flash in DDR3 (cores/p2k/docs/plan.md). The file holds the flash's bytes in order (the
// same layout as the system pack's flash image).
//   - restore: after the system pack's download, when the pack is good and the mounted file holds
//     2 MB, the file replaces the flash image; boot_hold keeps the CPU in reset until then;
//   - save: on a rising save_req, or when the OSD opens with autosave set and the OS has changed
//     the flash since the last save or restore (dirty). flash_lock holds the CPU's flash writes
//     for the whole save, so the file is consistent;
//   - nothing is written to a read-only file.
// 16 KB at a time (hps_io's limit) through a block-RAM staging buffer: DDR3 -> buffer -> SD, or
// SD -> buffer -> DDR3. Byte k (0-7) of each 8 is bits [16 (k / 2) + (k odd ? 0 : 8) +: 8] of the
// DDR3 word (big-endian 16-bit words, word i in lane i mod 4).
module p2k_flashsave #(
    parameter logic [28:0] BASE = 29'h7200000
) (
    input  logic          clk,
    input  logic          rst,
    input  logic          loading,        // system pack download in progress
    input  logic          image_ok,       // a good system pack is loaded

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

    input  logic          save_req,
    input  logic          osd_open,
    input  logic          autosave,
    input  logic          flash_changed,

    output logic          boot_hold,
    output logic          flash_lock,
    output logic          saving,
    output logic          restored,
    output logic          dirty,

    output logic          req_valid,
    input  logic          req_ready,
    output logic          req_we,
    output logic [28:0]   req_addr,
    output logic [63:0]   req_wdata,
    output logic [7:0]    req_len,
    input  logic          rsp_valid,
    input  logic [63:0]   rsp_data
);
    localparam int NCH = 128;                         // 16 KB requests in 2 MB
    assign sd_blk_cnt = 6'd31;                        // 32 blocks of 512 bytes

    // ------------------------------------------------------------------ staging buffer
    logic [10:0] s_ra, s_wa;
    logic [7:0]  s_we;
    logic [63:0] s_wd;                                // byte k in s_wd[8k +: 8]
    logic [7:0]  s_q [8];
    genvar gk;
    generate
        for (gk = 0; gk < 8; gk++) begin : g_stg
            logic [7:0] stg [0:2047];
            always_ff @(posedge clk) begin
                if (s_we[gk]) stg[s_wa] <= s_wd[8*gk +: 8];
                s_q[gk] <= stg[s_ra];
            end
        end
    endgenerate
    function automatic logic [5:0] lane_sh(input int k);
        lane_sh = 6'(16 * (k / 2) + ((k % 2 != 0) ? 0 : 8));
    endfunction

    typedef enum logic [3:0] { S_IDLE, S_RD, S_RD_ACK, S_PUT_A, S_PUT_D, S_GET, S_WR, S_WR_ACK } st_t;
    st_t st;
    logic        mounted, ro, have, save_d, osd_d, loading_d;
    logic [11:0] gcnt;               // save: words requested (2048 = all)
    logic [6:0]  chunk;
    logic [10:0] cnt, rcnt;
    logic [2:0]  lane_d;
    logic        get_wait;

    // SD side of the buffer
    always_comb begin
        s_ra = (st == S_WR || st == S_WR_ACK) ? sd_buff_addr[13:3] : cnt;
        s_we = '0;
        s_wa = sd_buff_addr[13:3];
        s_wd = {8{sd_buff_dout}};
        if ((st == S_RD || st == S_RD_ACK) && sd_ack && sd_buff_wr)
            s_we[sd_buff_addr[2:0]] = 1'b1;
        else if (st == S_GET && rsp_valid) begin
            s_we = 8'hff;
            s_wa = rcnt;
            for (int k = 0; k < 8; k++) s_wd[8*k +: 8] = rsp_data[lane_sh(k) +: 8];
        end
    end
    always_ff @(posedge clk) lane_d <= sd_buff_addr[2:0];
    assign sd_buff_din = s_q[lane_d];

    // DDR3 side
    logic [63:0] put_word;
    always_comb
        for (int k = 0; k < 8; k++) put_word[lane_sh(k) +: 8] = s_q[k];
    assign req_valid = st == S_PUT_D || (st == S_GET && !get_wait && !gcnt[11]);
    assign req_we = st == S_PUT_D;
    assign req_addr = BASE + 29'({chunk, st == S_GET ? gcnt[10:0] : cnt});
    assign req_wdata = put_word;
    assign req_len = st == S_PUT_D ? 8'd1 : 8'd8;

    assign flash_lock = saving;

    always_ff @(posedge clk) begin
        save_d <= save_req;
        osd_d <= osd_open;
        loading_d <= loading;
        if (flash_changed) dirty <= 1'b1;
        if (img_mounted) begin
            mounted <= 1'b1;
            ro <= img_readonly;
            have <= img_size >= 64'h200000;
        end
        if (rst || (loading && !loading_d)) begin
            mounted <= 1'b0;
            have <= 1'b0;
            boot_hold <= 1'b1;
            restored <= 1'b0;
            st <= S_IDLE;
            sd_rd <= 1'b0;
            sd_wr <= 1'b0;
            saving <= 1'b0;
            dirty <= 1'b0;
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
                             ((save_req && !save_d) || (autosave && dirty && osd_open && !osd_d))) begin
                    saving <= 1'b1;
                    dirty <= 1'b0;
                    chunk <= '0;
                    sd_lba <= '0;
                    gcnt <= '0;
                    rcnt <= '0;
                    get_wait <= 1'b0;
                    st <= S_GET;
                end
            // restore: SD -> buffer -> DDR3
            S_RD: if (sd_ack) begin sd_rd <= 1'b0; st <= S_RD_ACK; end
            S_RD_ACK: if (!sd_ack) begin cnt <= '0; st <= S_PUT_A; end
            S_PUT_A: st <= S_PUT_D;                   // buffer word cnt read
            S_PUT_D:
                if (req_ready) begin
                    cnt <= cnt + 11'd1;
                    st <= S_PUT_A;
                    if (cnt == 11'h7ff) begin
                        if (chunk == 7'(NCH - 1)) begin
                            boot_hold <= 1'b0;
                            restored <= 1'b1;
                            dirty <= 1'b0;
                            st <= S_IDLE;
                        end else begin
                            chunk <= chunk + 7'd1;
                            sd_lba <= sd_lba + 32'd32;
                            sd_rd <= 1'b1;
                            st <= S_RD;
                        end
                    end
                end
            // save: DDR3 -> buffer -> SD
            S_GET: begin
                if (!get_wait && !gcnt[11] && req_ready) begin
                    get_wait <= 1'b1;
                    gcnt <= gcnt + 12'd8;
                end
                if (rsp_valid) begin
                    rcnt <= rcnt + 11'd1;
                    if (rcnt[2:0] == 3'd7) get_wait <= 1'b0;
                    if (rcnt == 11'h7ff) begin
                        sd_wr <= 1'b1;
                        st <= S_WR;
                    end
                end
            end
            S_WR: if (sd_ack) begin sd_wr <= 1'b0; st <= S_WR_ACK; end
            S_WR_ACK:
                if (!sd_ack) begin
                    if (chunk == 7'(NCH - 1)) begin
                        saving <= 1'b0;
                        st <= S_IDLE;
                    end else begin
                        chunk <= chunk + 7'd1;
                        sd_lba <= sd_lba + 32'd32;
                        gcnt <= '0;
                        rcnt <= '0;
                        get_wait <= 1'b0;
                        st <= S_GET;
                    end
                end
            default: st <= S_IDLE;
            endcase
        end
    end
endmodule
