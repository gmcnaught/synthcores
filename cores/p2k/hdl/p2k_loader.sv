// SPDX-License-Identifier: GPL-3.0-or-later
// Proteus 2000 image loader: hps_io ioctl downloads into DDR3 (cores/p2k/docs/plan.md).
//   index 1, the system pack: bytes 0x000000-0x1FFFFF the flash image (big-endian words), then
//            0x200000-0x200FFF the HD44780 character ROM (hd44780_a00.bin), to font_*;
//   index 2-5, SIMM slot 1-4: bytes 0x000000-0x1FFFFF Baby, from 0x200000 Big (up to 32 MB).
// 16-bit word i of a region is lane i mod 4 of 64-bit word base + i / 4, high byte first in the
// file. Bytes are gathered into 64-bit words; ioctl_wait holds the download while a word is
// written. pack_ok: a whole flash image and font arrived; big_words/baby_words: what each slot
// holds (a new SIMM download clears its slot first).
module p2k_loader #(
    parameter logic [28:0] SMP_BASE  = 29'h6000000,
    parameter logic [28:0] BABY_BASE = 29'h7100000,
    parameter logic [28:0] CPU_BASE  = 29'h7200000
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
    output logic        loading,          // a pack or SIMM download is in progress
    output logic        simm_loaded,      // pulse: a SIMM download ended
    output logic [24:0] big_words [4],
    output logic [20:0] baby_words [4],

    output logic        req_valid,
    input  logic        req_ready,
    output logic [28:0] req_addr,
    output logic [63:0] req_wdata,
    output logic [7:0]  req_be
);
    wire is_pack = ioctl_index[7:0] == 8'd1;
    wire is_simm = ioctl_index[7:0] >= 8'd2 && ioctl_index[7:0] <= 8'd5;
    wire [1:0] slot = 2'(ioctl_index[7:0] - 8'd2);
    logic       dl_d, is_pack_l, is_simm_l;
    logic [1:0] slot_l;

    // the word being gathered
    logic        g_valid;
    logic [28:0] g_addr;
    logic [63:0] g_data;
    logic [7:0]  g_be;
    logic [26:0] top_addr;           // highest byte address + 1 of the current download

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

    logic g_open;                         // the gathered word may still take bytes
    assign req_valid = g_valid && !g_open;
    assign req_addr = g_addr;
    assign req_wdata = g_data;
    assign req_be = g_be;
    assign ioctl_wait = g_valid && !g_open;

    assign font_we = ioctl_wr && is_pack && ioctl_addr[26:12] == 15'h200;
    assign font_addr = ioctl_addr[11:0];
    assign font_data = ioctl_dout;
    assign loading = ioctl_download && (is_pack || is_simm);

    always_ff @(posedge clk) begin
        simm_loaded <= 1'b0;
        dl_d <= ioctl_download;
        if (rst) begin
            g_valid <= 1'b0;
            g_open <= 1'b0;
            pack_ok <= 1'b0;
            for (int s = 0; s < 4; s++) begin
                big_words[s] <= '0;
                baby_words[s] <= '0;
            end
        end else begin
            if (req_valid && req_ready) g_valid <= 1'b0;
            if (ioctl_download && !dl_d) begin                // a download starts
                is_pack_l <= is_pack;
                is_simm_l <= is_simm;
                slot_l <= slot;
                top_addr <= '0;
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
            if (ioctl_wr && (is_pack || is_simm)) top_addr <= ioctl_addr + 27'd1;
            if (!ioctl_download && dl_d) begin                // a download ends
                g_open <= 1'b0;           // write out a partial word
                if (is_pack_l) pack_ok <= top_addr >= 27'h201000;
                if (is_simm_l) begin
                    baby_words[slot_l] <= top_addr >= 27'h200000 ? 21'h100000 : 21'(top_addr[26:1]);
                    big_words[slot_l] <= top_addr > 27'h200000 ? 25'(27'(top_addr - 27'h200000) >> 1) : 25'd0;
                    simm_loaded <= 1'b1;
                end
            end
        end
    end
endmodule
