// SPDX-License-Identifier: GPL-3.0-or-later
// Proteus 2000 image loader: hps_io ioctl downloads into DDR3 (cores/p2k/docs/plan.md).
//   index 1, the system pack: bytes 0x000000-0x1FFFFF the flash image (big-endian words), then
//            0x200000-0x200FFF the HD44780 character ROM (hd44780_a00.bin), to font_*;
//   index 2-5, SIMM slot 1-4: bytes 0x000000-0x1FFFFF Baby, from 0x200000 Big (up to 32 MB).
// 16-bit word i of a region is lane i mod 4 of 64-bit word base + i / 4, high byte first in the
// file.
//
// Two ways in:
//   - direct (CONF_STR load address = STAGE_BASE * 8): Main_MiSTer writes the file into DDR3 at
//     STAGE_BASE itself (file byte k is byte k mod 8 of word STAGE_BASE + k / 8) and sends no
//     bytes; ioctl_addr holds the file size while ioctl_download is high. When the download
//     ends, the copier reads the staged file in bursts of 8 words and writes each word to its
//     place with the bytes of every 16-bit lane swapped, and the font bytes to font_*;
//   - bytes (no load address, e.g. an older Main_MiSTer): bytes arrive on ioctl_wr and are
//     gathered into 64-bit words; ioctl_wait holds the download while a word is written.
// pack_ok: a whole flash image and font arrived; big_words/baby_words: what each slot holds (a
// new SIMM download clears its slot first). loading and pack_loading stay high until the copy
// is done; pack_ok and the slot sizes are set as they fall. While the copier runs (hold) it
// owns the memory client and its responses; it starts only when ext_busy (another user of the
// client with reads in flight) is low.
module p2k_loader #(
    parameter logic [28:0] SMP_BASE   = 29'h6000000,
    parameter logic [28:0] BABY_BASE  = 29'h7100000,
    parameter logic [28:0] CPU_BASE   = 29'h7200000,
    parameter logic [28:0] STAGE_BASE = 29'h7800000
) (
    input  logic        clk,
    input  logic        rst,
    input  logic        ioctl_download,
    input  logic [15:0] ioctl_index,
    input  logic        ioctl_wr,
    input  logic [26:0] ioctl_addr,
    input  logic [7:0]  ioctl_dout,
    output logic        ioctl_wait,

    output logic        font_we,
    output logic [11:0] font_addr,
    output logic [7:0]  font_data,

    output logic        pack_ok,
    output logic        loading,          // a pack or SIMM download or its copy is in progress
    output logic        pack_loading,     // the same, for the system pack
    output logic        simm_loaded,      // pulse: a SIMM is in
    output logic [24:0] big_words [4],
    output logic [20:0] baby_words [4],

    input  logic        ext_busy,
    output logic        hold,

    output logic        req_valid,
    input  logic        req_ready,
    output logic        req_we,
    output logic [28:0] req_addr,
    output logic [63:0] req_wdata,
    output logic [7:0]  req_be,
    output logic [7:0]  req_len,
    input  logic        rsp_valid,
    input  logic [63:0] rsp_data
);
    wire is_pack = ioctl_index[7:0] == 8'd1;
    wire is_simm = ioctl_index[7:0] >= 8'd2 && ioctl_index[7:0] <= 8'd5;
    wire [1:0] slot = 2'(ioctl_index[7:0] - 8'd2);
    logic       dl_d, is_pack_l, is_simm_l, got_wr;
    logic [1:0] slot_l;
    logic [26:0] top_addr;           // bytes: highest byte address + 1; direct: the file size

    // slot sizes for a file of n bytes
    function automatic logic [20:0] baby_of(input logic [26:0] n);
        return n >= 27'h200000 ? 21'h100000 : 21'(n[26:1]);
    endfunction
    function automatic logic [24:0] big_of(input logic [26:0] n);
        return n >= 27'h2200000 ? 25'h1000000 : n > 27'h200000 ? 25'(27'(n - 27'h200000) >> 1) : 25'd0;
    endfunction

    // ------------------------------------------------------------------ bytes
    logic        g_valid, g_open;    // the word being gathered; it may still take bytes
    logic [28:0] g_addr;
    logic [63:0] g_data;
    logic [7:0]  g_be;

    // the byte's 64-bit word and lane: region base + byte offset / 8; byte k of 8 is
    // bits [16 (k / 2) + (k odd ? 0 : 8) +: 8]
    logic [28:0] b_word;
    logic [2:0]  b_k;
    logic        b_ok;
    always_comb begin
        b_k = ioctl_addr[2:0];
        b_ok = 1'b1;
        if (is_pack) begin
            b_word = CPU_BASE + 29'(ioctl_addr[20:3]);
            b_ok = ioctl_addr < 27'h200000;
        end else if (ioctl_addr < 27'h200000)
            b_word = BABY_BASE + 29'({slot, ioctl_addr[20:3]});
        else begin
            b_word = SMP_BASE + 29'({slot, 22'(ioctl_addr[24:3] - 22'h40000)});
            b_ok = ioctl_addr < 27'h2200000;
        end
    end
    wire [5:0] b_sh = {b_k[2:1], 4'd0} + (b_k[0] ? 6'd0 : 6'd8);
    wire g_req = g_valid && !g_open;
    assign ioctl_wait = g_req;

    // ------------------------------------------------------------------ copier
    typedef enum logic [2:0] { C_IDLE, C_WAIT, C_RD, C_GET, C_PUT, C_FONT } cst_t;
    cst_t        c_st;
    logic        c_pack;
    logic [1:0]  c_slot;
    logic [26:0] c_size;             // bytes to place
    logic [22:0] c_w;                // staged word being placed
    logic [22:0] c_n;                // staged words to place
    logic [3:0]  c_got;              // words of the burst received
    logic [2:0]  c_j;                // word of the burst being placed
    logic [2:0]  c_k;                // font byte of the word
    logic [63:0] c_buf [8];
    logic        c_req;
    logic [28:0] c_addr;
    logic [63:0] c_data;
    logic [7:0]  c_be;
    logic        c_fwe;
    logic [11:0] c_faddr;
    logic [7:0]  c_fdata;
    assign hold = c_st != C_IDLE && c_st != C_WAIT;

    // where staged word c_w goes, its data with each 16-bit lane's bytes swapped, and the byte
    // enables of a last, partial word
    logic [63:0] c_cur, c_sw;
    logic [28:0] c_dst;
    logic        c_font;
    logic [26:0] c_left;
    logic [7:0]  c_bev;
    always_comb begin
        c_cur = c_buf[c_j];
        for (int i = 0; i < 4; i++) c_sw[16 * i +: 16] = {c_cur[16 * i +: 8], c_cur[16 * i + 8 +: 8]};
        c_font = 1'b0;
        if (c_pack) begin
            c_dst = CPU_BASE + 29'(c_w[17:0]);
            c_font = c_w >= 23'h40000;
        end else if (c_w < 23'h40000)
            c_dst = BABY_BASE + 29'({c_slot, c_w[17:0]});
        else
            c_dst = SMP_BASE + 29'({c_slot, 22'(c_w - 23'h40000)});
        c_left = c_size - 27'({c_w, 3'd0});
        for (int i = 0; i < 8; i++) c_bev[i] = c_left > 27'(i ^ 1);
    end

    assign req_valid = hold ? c_req : g_req;
    assign req_we    = hold ? c_st == C_PUT : 1'b1;
    assign req_addr  = hold ? (c_st == C_RD ? STAGE_BASE + 29'(c_w) : c_addr) : g_addr;
    assign req_wdata = hold ? c_data : g_data;
    assign req_be    = hold ? c_be : g_be;
    assign req_len   = hold && c_st == C_RD ? 8'd8 : 8'd1;

    assign font_we   = c_fwe || (ioctl_wr && is_pack && ioctl_addr[26:12] == 15'h200);
    assign font_addr = c_fwe ? c_faddr : ioctl_addr[11:0];
    assign font_data = c_fwe ? c_fdata : ioctl_dout;
    // dl_d covers the clock between the download's end and the copier's start
    assign loading = (ioctl_download && (is_pack || is_simm)) || (dl_d && (is_pack_l || is_simm_l)) || c_st != C_IDLE;
    assign pack_loading = (ioctl_download && is_pack) || (dl_d && is_pack_l) || (c_st != C_IDLE && c_pack);

    always_ff @(posedge clk) begin
        simm_loaded <= 1'b0;
        dl_d <= ioctl_download;
        c_fwe <= 1'b0;
        if (rst) begin
            g_valid <= 1'b0;
            g_open <= 1'b0;
            pack_ok <= 1'b0;
            c_st <= C_IDLE;
            c_req <= 1'b0;
            for (int s = 0; s < 4; s++) begin
                big_words[s] <= '0;
                baby_words[s] <= '0;
            end
        end else begin
            // bytes
            if (g_req && req_ready && !hold) g_valid <= 1'b0;
            if (ioctl_download && !dl_d) begin                // a download starts
                is_pack_l <= is_pack;
                is_simm_l <= is_simm;
                slot_l <= slot;
                got_wr <= 1'b0;
                if (is_pack) pack_ok <= 1'b0;
                if (is_simm) begin
                    big_words[slot] <= '0;
                    baby_words[slot] <= '0;
                end
            end
            if (ioctl_wr && (is_pack || is_simm) && b_ok) begin
                if (g_valid && g_open && g_addr == b_word) begin
                    g_data[b_sh +: 8] <= ioctl_dout;
                    g_be[b_sh[5:3]] <= 1'b1;
                end else begin
                    g_addr <= b_word;
                    g_data <= 64'(ioctl_dout) << b_sh;
                    g_be <= 8'(1) << b_sh[5:3];
                    g_valid <= 1'b1;
                end
                g_open <= b_k != 3'd7;    // the 8th byte closes the word
            end
            if (ioctl_wr && (is_pack || is_simm)) begin
                got_wr <= 1'b1;
                top_addr <= ioctl_addr + 27'd1;
            end else if (ioctl_download && !got_wr)
                top_addr <= ioctl_addr;   // direct: the size, from the download's start
            if (!ioctl_download && dl_d) begin                // a download ends
                g_open <= 1'b0;           // write out a partial word
                if (got_wr) begin
                    if (is_pack_l) pack_ok <= top_addr >= 27'h201000;
                    if (is_simm_l) begin
                        baby_words[slot_l] <= baby_of(top_addr);
                        big_words[slot_l] <= big_of(top_addr);
                        simm_loaded <= 1'b1;
                    end
                end else if ((is_pack_l || is_simm_l) && top_addr != 0) begin
                    c_pack <= is_pack_l;
                    c_slot <= slot_l;
                    c_size <= is_pack_l ? (top_addr > 27'h201000 ? 27'h201000 : top_addr)
                                        : (top_addr > 27'h2200000 ? 27'h2200000 : top_addr);
                    c_st <= C_WAIT;
                end else if (is_simm_l)
                    simm_loaded <= 1'b1;
            end

            // copier
            begin
                automatic logic fin = 1'b0;   // the last word is placed
                automatic logic more = c_w + 23'd1 < c_n;
                case (c_st)
                    C_WAIT: if (!ext_busy && !g_valid) begin
                        c_w <= '0;
                        c_n <= 23'((c_size + 27'd7) >> 3);
                        c_req <= 1'b1;
                        c_st <= C_RD;
                    end
                    C_RD: if (req_ready) begin
                        c_req <= 1'b0;
                        c_got <= '0;
                        c_st <= C_GET;
                    end
                    C_GET: if (rsp_valid) begin
                        c_buf[c_got[2:0]] <= rsp_data;
                        c_got <= c_got + 4'd1;
                        if (c_got == 4'd7) begin
                            c_j <= '0;
                            c_st <= C_PUT;
                        end
                    end
                    C_PUT:
                        if (c_req) begin
                            if (req_ready) begin
                                c_req <= 1'b0;
                                c_w <= c_w + 23'd1;
                                c_j <= c_j + 3'd1;
                                if (c_j == 3'd7) begin
                                    c_req <= more;
                                    c_st <= more ? C_RD : C_IDLE;
                                    fin = !more;
                                end
                            end
                        end else if (c_w >= c_n) begin  // a partial last burst is placed
                            c_st <= C_IDLE;
                            fin = 1'b1;
                        end else if (c_font) begin
                            c_k <= '0;
                            c_st <= C_FONT;
                        end else begin
                            c_addr <= c_dst;
                            c_data <= c_sw;
                            c_be <= c_bev;
                            c_req <= 1'b1;
                        end
                    C_FONT: begin
                        c_fwe <= c_w < 23'h40200;
                        c_faddr <= {c_w[8:0], c_k};
                        c_fdata <= c_cur[8 * c_k +: 8];
                        c_k <= c_k + 3'd1;
                        if (c_k == 3'd7) begin
                            c_w <= c_w + 23'd1;
                            c_j <= c_j + 3'd1;
                            if (c_j == 3'd7) begin
                                c_req <= more;
                                c_st <= more ? C_RD : C_IDLE;
                                fin = !more;
                            end else
                                c_st <= C_PUT;
                        end
                    end
                    default: ;
                endcase
                if (fin) begin
                    if (c_pack) pack_ok <= c_size >= 27'h201000;
                    else begin
                        baby_words[c_slot] <= baby_of(c_size);
                        big_words[c_slot] <= big_of(c_size);
                        simm_loaded <= 1'b1;
                    end
                end
            end
        end
    end
endmodule
