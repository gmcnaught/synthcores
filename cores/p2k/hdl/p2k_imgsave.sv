// SPDX-License-Identifier: GPL-3.0-or-later
// Proteus 2000 images on the SD card (hps_io virtual disks 0-2) <-> DDR3 (cores/p2k/docs/plan.md).
//
// Image 0, the flash save (saves/P2K/<pack>.sav; Main_MiSTer mounts it with the system pack): the
// 2 MB board flash, the file in the flash image's byte order.
//   - restore: after the system pack's download, when the pack is good and the mounted file holds
//     2 MB, the file replaces the flash image; boot_hold keeps the CPU in reset until then;
//   - save: on a rising save_req, or when the OSD opens with autosave set and the OS has changed the
//     flash since the last save or restore (flash_changed). flash_lock holds the CPU's flash writes
//     for the whole save, so the file is consistent. A pack download starting during a save stops it
//     after the chunk in flight (that chunk is not written).
// Images 1-2, user flash SIMMs in slots 1-2 (OSD "Flash SIMM slot N"): a .sim file (Baby 2 MB, then
// Big up to 32 MB, as a SIMM download).
//   - mount: the slot becomes a flash slot (slot_flash) and the file is loaded into it; the part of
//     the slot beyond the file is erased (0xFFFF). simm_busy holds the CPU until simm_loaded. An
//     empty mount (size 0) leaves the slot empty (slot_empty). A ROM SIMM download into the slot
//     (rom_load) makes it a ROM slot again; a load in flight stops at the next chunk.
//   - save: with the image 0 save's triggers (save_req, or autosave on OSD open): the 256 KB regions
//     the OS changed (fl_dirty from p2k_sndbus; Baby regions 0-7, Big 8-135) are written back, the
//     part within the file only (Main_MiSTer does not grow a mounted image). A region's mark is
//     cleared before it is read, so a program during the save marks it again for the next one.
// Nothing is written to a read-only file. One image is moved at a time, 16 KB at a time (hps_io's
// limit) through a block-RAM staging buffer: SD -> buffer -> DDR3, or DDR3 -> buffer -> SD. Byte k
// (0-7) of each 8 is bits [16 (k / 2) + (k odd ? 0 : 8) +: 8] of the DDR3 word (big-endian 16-bit
// words, word i in lane i mod 4). The memory client is used only while grant is high; want asks for
// it (p2k MiSTer top: shared with the loader's copier).
module p2k_imgsave #(
    parameter logic [28:0] CPU_BASE  = 29'h7200000,
    parameter logic [28:0] SMP_BASE  = 29'h6000000,
    parameter logic [28:0] BABY_BASE = 29'h7100000
) (
    input  logic          clk,
    input  logic          rst,
    input  logic          loading,        // system pack download in progress
    input  logic          image_ok,       // a good system pack is loaded
    input  logic [1:0]    rom_load,       // a ROM SIMM download into slot 1 / 2 starts (pulse)

    input  logic [2:0]    img_mounted,
    input  logic          img_readonly,
    input  logic [63:0]   img_size,
    output logic [31:0]   sd_lba,
    output logic [5:0]    sd_blk_cnt,
    output logic [2:0]    sd_rd,
    output logic [2:0]    sd_wr,
    input  logic [2:0]    sd_ack,
    input  logic [13:0]   sd_buff_addr,
    input  logic [7:0]    sd_buff_dout,
    input  logic          sd_buff_wr,
    output logic [7:0]    sd_buff_din,

    input  logic          save_req,
    input  logic          osd_open,
    input  logic          autosave,
    input  logic          flash_changed,
    input  logic          fl_dirty,
    input  logic [1:0]    fl_dirty_slot,
    input  logic [7:0]    fl_dirty_region,

    output logic          boot_hold,
    output logic          flash_lock,
    output logic          saving,
    output logic          restored,
    output logic          dirty,          // image 0
    output logic [1:0]    slot_flash,
    output logic [1:0]    slot_empty,
    output logic          simm_busy,
    output logic          simm_loaded,    // pulse

    output logic          want,
    input  logic          grant,
    output logic          req_valid,
    input  logic          req_ready,
    output logic          req_we,
    output logic [28:0]   req_addr,
    output logic [63:0]   req_wdata,
    output logic [7:0]    req_len,
    input  logic          rsp_valid,
    input  logic [63:0]   rsp_data
);
    localparam logic [11:0] SIMM_CHUNKS = 12'd2176;   // 34 MB in 16 KB
    localparam logic [25:0] SIMM_MAX = 26'h2200000;
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

    typedef enum logic [3:0] { S_IDLE, S_SCAN, S_CHUNK, S_RD, S_RD_ACK, S_PUT_A, S_PUT_D, S_FILL, S_GET,
                               S_WR, S_WR_ACK } st_t;
    typedef enum logic [1:0] { O_RESTORE0, O_LOAD, O_SAVE0, O_SAVES } op_t;
    st_t         st;
    op_t         op;
    logic [1:0]  img;                // image of the op: 0, or slot + 1
    wire         sl = img[1];        // the slot (images 1, 2 -> slots 0, 1)
    logic [11:0] c, c_end;           // chunk; end of the region (save of a SIMM)
    logic [7:0]  r;                  // region (save of a SIMM)
    logic        ab;                 // stop the op at the next chunk
    logic [11:0] gcnt;               // save: words requested (2048 = all)
    logic [10:0] cnt, rcnt;
    logic [4:0]  fk;                 // erase: fills issued
    logic [2:0]  lane_d;
    logic        get_wait;

    // image 0
    logic        mounted0, ro0, have0, save_d, osd_d, loading_d, sv0;
    // SIMM images
    logic [1:0]  mnt, ro, pend, sv;
    logic [25:0] size [2];
    logic [135:0] dreg [2];

    // the size of the image of the op, for the part of a SIMM file beyond its end
    wire [25:0] cur_size = size[sl];
    wire        past_end = img != 2'd0 && {c, 14'd0} >= cur_size;
    wire [2:0]  cur_bit = 3'(1) << img;

    // the DDR3 word of chunk c, word n
    function automatic logic [28:0] dest(input logic [1:0] im, input logic [11:0] ch, input logic [10:0] n);
        if (im == 2'd0) return CPU_BASE + 29'({ch[6:0], n});
        else if (ch < 12'd128) return BABY_BASE + 29'({im[1], 18'd0}) + 29'({ch[6:0], n});
        else return SMP_BASE + 29'({im[1], 22'd0}) + 29'({11'(ch - 12'd128), n});
    endfunction

    // SD side of the buffer; bytes of a SIMM file past its end read as erased flash
    always_comb begin
        s_ra = (st == S_WR || st == S_WR_ACK) ? sd_buff_addr[13:3] : cnt;
        s_we = '0;
        s_wa = sd_buff_addr[13:3];
        s_wd = {8{sd_buff_dout}};
        if ((st == S_RD || st == S_RD_ACK) && (sd_ack & cur_bit) != 0 && sd_buff_wr) begin
            s_we[sd_buff_addr[2:0]] = 1'b1;
            if (img != 2'd0 && {c, sd_buff_addr} >= cur_size) s_wd = '1;
        end else if (st == S_GET && rsp_valid) begin
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
    assign req_valid = st == S_PUT_D || st == S_FILL || (st == S_GET && !get_wait && !gcnt[11]);
    assign req_we    = st == S_PUT_D || st == S_FILL;
    assign req_addr  = dest(img, c, st == S_GET ? gcnt[10:0] : st == S_FILL ? {fk[3:0], 7'd0} : cnt);
    assign req_wdata = st == S_FILL ? '1 : put_word;
    assign req_len   = st == S_GET ? 8'd8 : st == S_FILL ? 8'd128 : 8'd1;

    assign flash_lock = st != S_IDLE && op == O_SAVE0;
    assign saving = st != S_IDLE && (op == O_SAVE0 || op == O_SAVES);
    assign simm_busy = pend != 2'd0 || (st != S_IDLE && op == O_LOAD);

    // the next op, in S_IDLE
    logic       n_any;
    op_t        n_op;
    logic [1:0] n_img;
    always_comb begin
        n_any = 1'b1;
        n_op = O_RESTORE0;
        n_img = 2'd0;
        if (boot_hold && !loading && image_ok && mounted0 && have0) begin
            n_op = O_RESTORE0;
        end else if (pend[0] || pend[1]) begin
            n_op = O_LOAD;
            n_img = pend[0] ? 2'd1 : 2'd2;
        end else if (sv0) begin
            n_op = O_SAVE0;
        end else if (sv[0] || sv[1]) begin
            n_op = O_SAVES;
            n_img = sv[0] ? 2'd1 : 2'd2;
        end else
            n_any = 1'b0;
    end
    assign want = st != S_IDLE || n_any;

    wire save_edge = save_req && !save_d;
    wire osd_edge  = osd_open && !osd_d;

    always_ff @(posedge clk) begin
        save_d <= save_req;
        osd_d <= osd_open;
        loading_d <= loading;
        simm_loaded <= 1'b0;
        if (flash_changed) dirty <= 1'b1;
        // mounts
        if (img_mounted[0]) begin
            mounted0 <= 1'b1;
            ro0 <= img_readonly;
            have0 <= img_size >= 64'h200000;
        end
        for (int s = 0; s < 2; s++) begin
            if (img_mounted[s + 1]) begin
                ro[s] <= img_readonly;
                size[s] <= img_size > 64'(SIMM_MAX) ? SIMM_MAX : 26'(img_size);
                mnt[s] <= img_size != 0;
                pend[s] <= img_size != 0;
                slot_flash[s] <= img_size != 0;
                slot_empty[s] <= img_size == 0;
                sv[s] <= 1'b0;
                dreg[s] <= '0;
                if (st != S_IDLE && img == 2'(s + 1)) ab <= 1'b1;
            end
            if (rom_load[s]) begin
                pend[s] <= 1'b0;
                slot_flash[s] <= 1'b0;
                slot_empty[s] <= 1'b0;
                sv[s] <= 1'b0;
                if (st != S_IDLE && img == 2'(s + 1)) ab <= 1'b1;
            end
        end
        // save triggers
        if (image_ok && !loading && mounted0 && !ro0 && (save_edge || (autosave && dirty && osd_edge)))
            sv0 <= 1'b1;
        for (int s = 0; s < 2; s++)
            if (slot_flash[s] && !pend[s] && mnt[s] && !ro[s] && (save_edge || (autosave && osd_edge)) &&
                !(st != S_IDLE && op == O_LOAD && img == 2'(s + 1)))
                sv[s] <= 1'b1;

        if (rst) begin
            mounted0 <= 1'b0;
            have0 <= 1'b0;
            boot_hold <= 1'b1;
            restored <= 1'b0;
            dirty <= 1'b0;
            sv0 <= 1'b0;
            mnt <= '0;
            pend <= '0;
            sv <= '0;
            slot_flash <= '0;
            slot_empty <= '0;
            st <= S_IDLE;
            sd_rd <= '0;
            sd_wr <= '0;
            ab <= 1'b0;
        end else begin
            if (loading && !loading_d) begin                  // a system pack download starts
                mounted0 <= 1'b0;
                have0 <= 1'b0;
                boot_hold <= 1'b1;
                restored <= 1'b0;
                dirty <= 1'b0;
                sv0 <= 1'b0;
                if (st != S_IDLE && (op == O_RESTORE0 || op == O_SAVE0)) ab <= 1'b1;
            end
            case (st)
            S_IDLE: begin
                ab <= 1'b0;
                if (boot_hold && !loading && image_ok && !(mounted0 && have0))
                    boot_hold <= 1'b0;                        // nothing to restore
                else if (n_any && grant) begin
                    op <= n_op;
                    img <= n_img;
                    c <= '0;
                    r <= '0;
                    case (n_op)
                    O_LOAD: pend[n_img[1]] <= 1'b0;
                    O_SAVE0: sv0 <= 1'b0;
                    O_SAVES: sv[n_img[1]] <= 1'b0;
                    default: ;
                    endcase
                    if (n_op == O_SAVE0) dirty <= 1'b0;
                    st <= n_op == O_SAVES ? S_SCAN : S_CHUNK;
                end
            end
            S_SCAN:                                          // the next changed region
                if (r == 8'd136) st <= S_IDLE;
                else if (dreg[sl][r]) begin
                    dreg[sl][r] <= 1'b0;
                    c <= {r, 4'd0};
                    c_end <= {r, 4'd0} + 12'd16;
                    st <= S_CHUNK;
                end else
                    r <= r + 8'd1;
            S_CHUNK: begin
                cnt <= '0;
                gcnt <= '0;
                rcnt <= '0;
                get_wait <= 1'b0;
                fk <= '0;
                case (op)
                O_RESTORE0, O_LOAD:
                    if (ab) st <= S_IDLE;
                    else if (c == (op == O_RESTORE0 ? 12'd128 : SIMM_CHUNKS)) begin
                        if (op == O_RESTORE0) begin
                            boot_hold <= 1'b0;
                            restored <= 1'b1;
                            dirty <= 1'b0;
                        end else
                            simm_loaded <= 1'b1;
                        st <= S_IDLE;
                    end else if (past_end)
                        st <= S_FILL;
                    else begin
                        sd_lba <= {15'd0, c, 5'd0};
                        sd_rd <= cur_bit;
                        st <= S_RD;
                    end
                O_SAVE0:
                    if (ab || c == 12'd128) st <= S_IDLE;
                    else st <= S_GET;
                default:                                     // O_SAVES: chunks of region r
                    if (ab) st <= S_IDLE;
                    else if (c == c_end) begin
                        r <= r + 8'd1;
                        st <= S_SCAN;
                    end else if (past_end)
                        c <= c + 12'd1;
                    else
                        st <= S_GET;
                endcase
            end
            // load: SD -> buffer -> DDR3, or an erased chunk
            S_RD: if ((sd_ack & cur_bit) != 0) begin sd_rd <= '0; st <= S_RD_ACK; end
            S_RD_ACK: if ((sd_ack & cur_bit) == 0) st <= S_PUT_A;
            S_PUT_A: st <= S_PUT_D;                          // buffer word cnt read
            S_PUT_D:
                if (req_ready) begin
                    cnt <= cnt + 11'd1;
                    st <= S_PUT_A;
                    if (cnt == 11'h7ff) begin
                        c <= c + 12'd1;
                        st <= S_CHUNK;
                    end
                end
            S_FILL:
                if (req_ready) begin
                    fk <= fk + 5'd1;
                    if (fk == 5'd15) begin
                        c <= c + 12'd1;
                        st <= S_CHUNK;
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
                        if (ab) st <= S_IDLE;
                        else begin
                            sd_lba <= {15'd0, c, 5'd0};
                            sd_wr <= cur_bit;
                            st <= S_WR;
                        end
                    end
                end
            end
            S_WR: if ((sd_ack & cur_bit) != 0) begin sd_wr <= '0; st <= S_WR_ACK; end
            S_WR_ACK:
                if ((sd_ack & cur_bit) == 0) begin
                    c <= c + 12'd1;
                    st <= S_CHUNK;
                end
            default: st <= S_IDLE;
            endcase
        end
        // regions the OS changed (after the scan's clear: a mark in the same clock stays)
        if (fl_dirty && !fl_dirty_slot[1]) dreg[fl_dirty_slot[0]][fl_dirty_region] <= 1'b1;
    end
endmodule
