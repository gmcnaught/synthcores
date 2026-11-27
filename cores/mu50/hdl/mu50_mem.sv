// SPDX-License-Identifier: MIT
// MU50 memories in DDR3 (MiSTer DDRAM through synthframe_mem_arb and synthframe_mem_ddr3).
//
// Layout, bytes from BASE (DDR3 from 0x30000000 is free for cores):
//   000000-07FFFF  program ROM (CPU byte order)   \ the ROM pack (tools/mkpack.py), written by
//   080000-47FFFF  wave ROM (4 MB)                 | the loader from the OSD download
//   480000-480FFF  LCD character ROM               /
//   500000-61FFFF  SWP00 delay lines, 0x48000 32-bit words (zeroed after each download)
//
// Clients, in priority order: 0 loader (and the delay-line clear), 1 SWP00 delay lines,
// 2 SWP00 wave ROM, 3 the program ROM cache.
//   - Program ROM: 32 KB direct-mapped cache, 32-byte lines (4 DDR3 words, one burst), for the
//     CPU's 16-bit reads (rom_req held until rom_ack).
//   - Wave ROM: 4 bytes from byte address wv_addr (one or two DDR3 words), little endian. A read
//     that runs past the end of the 4 MB returns the character ROM's bytes for the part past the
//     end where the SWP00 would wrap to the start (it never reads there: sample data ends well
//     before).
//   - Delay lines: 32-bit words, half a DDR3 word each; writes are acknowledged when taken (a
//     later read sees them: synthframe_mem_ddr3).
module mu50_mem #(
    parameter logic [31:0] BASE = 32'h3000_0000
) (
    input  logic          clk,
    input  logic          rst,

    // loader: the ROM pack's bytes in order (ioctl), and the clear after it
    input  logic          ld_wr,
    input  logic [22:0]   ld_addr,
    input  logic [7:0]    ld_data,
    output logic          ld_wait,         // ioctl_wait
    input  logic          ld_done,         // the download ended: clear the delay lines
    output logic          ready,           // pack loaded, delay lines clear
    output logic          cg_we,           // the character ROM's bytes, for the LCD video
    output logic [11:0]   cg_addr,
    output logic [7:0]    cg_data,

    input  logic          rom_req,
    input  logic [18:1]   rom_addr,
    output logic          rom_ack,
    output logic [15:0]   rom_data,

    input  logic          wv_req,
    input  logic [21:0]   wv_addr,
    output logic          wv_ack,
    output logic [31:0]   wv_data,

    input  logic          dm_req,
    input  logic          dm_we,
    input  logic [18:0]   dm_addr,
    input  logic [31:0]   dm_wdata,
    output logic          dm_ack,
    output logic [31:0]   dm_rdata,

    input  logic          DDRAM_BUSY,
    output logic [7:0]    DDRAM_BURSTCNT,
    output logic [28:0]   DDRAM_ADDR,
    input  logic [63:0]   DDRAM_DOUT,
    input  logic          DDRAM_DOUT_READY,
    output logic          DDRAM_RD,
    output logic [63:0]   DDRAM_DIN,
    output logic [7:0]    DDRAM_BE,
    output logic          DDRAM_WE
);
    localparam logic [28:0] W0 = 29'(BASE >> 3);          // word address of BASE
    localparam logic [28:0] W_WAVE = W0 + 29'h10000;      // 080000 / 8
    localparam logic [28:0] W_DLY = W0 + 29'hA0000;       // 500000 / 8
    localparam int DLY_WORDS = 32'h24000;                 // 0x48000 32-bit words

    // ---------------------------------------------------------------- arbiter and DDR3
    localparam int N = 4, CTW = 2;
    logic            c_req_valid [N];
    logic            c_req_ready [N];
    logic            c_req_we    [N];
    logic [28:0]     c_req_addr  [N];
    logic [63:0]     c_req_wdata [N];
    logic [7:0]      c_req_be    [N];
    logic [7:0]      c_req_len   [N];
    logic [CTW-1:0]  c_req_tag   [N];
    logic            c_rsp_valid [N];
    logic [63:0]     c_rsp_data;
    logic [CTW-1:0]  c_rsp_tag;
    logic            req_valid, req_ready, req_we, rsp_valid;
    logic [28:0]     req_addr;
    logic [63:0]     req_wdata, rsp_data;
    logic [7:0]      req_be, req_len;
    logic [CTW+1:0]  req_tag, rsp_tag;

    synthframe_mem_arb #(.N(N), .CTW(CTW)) arb (
        .c_req_valid, .c_req_ready, .c_req_we, .c_req_addr, .c_req_wdata, .c_req_be, .c_req_len,
        .c_req_tag, .c_rsp_valid, .c_rsp_data, .c_rsp_tag,
        .req_valid, .req_ready, .req_we, .req_addr, .req_wdata, .req_be, .req_len, .req_tag,
        .rsp_valid, .rsp_data, .rsp_tag
    );
    synthframe_mem_ddr3 #(.TW(CTW + 2), .QBITS(4)) ddr (
        .clk, .rst,
        .req_valid, .req_ready, .req_we, .req_addr, .req_wdata, .req_be, .req_len, .req_tag,
        .rsp_valid, .rsp_data, .rsp_tag,
        .DDRAM_BUSY, .DDRAM_BURSTCNT, .DDRAM_ADDR, .DDRAM_DOUT, .DDRAM_DOUT_READY, .DDRAM_RD,
        .DDRAM_DIN, .DDRAM_BE, .DDRAM_WE
    );

    // ---------------------------------------------------------------- 0: loader and clear
    logic [63:0] ld_word;
    logic        ld_full, clearing;
    logic [17:0] clr_n;            // words of the delay lines still to clear
    assign ld_wait = ld_full;
    assign c_req_valid[0] = ld_full || (clearing && clr_n != 18'd0);
    assign c_req_we[0] = 1'b1;
    assign c_req_be[0] = 8'hff;
    assign c_req_tag[0] = '0;
    always_comb begin
        if (ld_full) begin
            c_req_addr[0] = W0 + 29'(ld_addr[22:3]);
            c_req_wdata[0] = ld_word;
            c_req_len[0] = 8'd1;
        end else begin
            c_req_addr[0] = W_DLY + 29'(DLY_WORDS) - 29'(clr_n);
            c_req_wdata[0] = 64'h0;
            c_req_len[0] = clr_n > 18'd255 ? 8'd255 : 8'(clr_n);
        end
    end
    assign cg_we = ld_wr && ld_addr >= 23'h480000 && ld_addr < 23'h481000;
    assign cg_addr = ld_addr[11:0];
    assign cg_data = ld_data;
    always_ff @(posedge clk) begin
        if (rst) begin
            ld_full <= 1'b0;
            clearing <= 1'b0;
            clr_n <= 18'd0;
            ready <= 1'b0;
        end else begin
            if (ld_wr) begin
                ready <= 1'b0;
                ld_word[8 * ld_addr[2:0] +: 8] <= ld_data;
                if (ld_addr[2:0] == 3'd7) ld_full <= 1'b1;
            end
            if (ld_full && c_req_ready[0]) ld_full <= 1'b0;
            if (ld_done) begin
                clearing <= 1'b1;
                clr_n <= 18'(DLY_WORDS);
            end else if (clearing && !ld_full && c_req_ready[0]) begin
                if (clr_n <= 18'd255) begin
                    clr_n <= 18'd0;
                    clearing <= 1'b0;
                    ready <= 1'b1;
                end else
                    clr_n <= clr_n - 18'd255;
            end
        end
    end

    // ---------------------------------------------------------------- 1: delay lines
    logic dm_busy;
    assign c_req_valid[1] = dm_req && !dm_busy && !dm_ack;
    assign c_req_we[1] = dm_we;
    assign c_req_addr[1] = W_DLY + 29'(dm_addr[18:1]);
    assign c_req_wdata[1] = {dm_wdata, dm_wdata};
    assign c_req_be[1] = dm_addr[0] ? 8'hf0 : 8'h0f;
    assign c_req_len[1] = 8'd1;
    assign c_req_tag[1] = '0;
    logic dm_half;
    always_ff @(posedge clk) begin
        dm_ack <= 1'b0;
        if (rst) dm_busy <= 1'b0;
        else if (c_req_valid[1] && c_req_ready[1]) begin
            if (dm_we) dm_ack <= 1'b1;
            else begin
                dm_busy <= 1'b1;
                dm_half <= dm_addr[0];
            end
        end else if (dm_busy && c_rsp_valid[1]) begin
            dm_rdata <= dm_half ? c_rsp_data[63:32] : c_rsp_data[31:0];
            dm_ack <= 1'b1;
            dm_busy <= 1'b0;
        end
    end

    // ---------------------------------------------------------------- 2: wave ROM
    logic       wv_busy, wv_two, wv_first;
    logic [2:0] wv_off;
    logic [63:0] wv_w0;
    wire  [25:0] wv_byte = 26'h80000 + 26'(wv_addr);
    assign c_req_valid[2] = wv_req && !wv_busy && !wv_ack;
    assign c_req_we[2] = 1'b0;
    assign c_req_addr[2] = W0 + 29'(wv_byte[25:3]);
    assign c_req_wdata[2] = '0;
    assign c_req_be[2] = 8'hff;
    assign c_req_len[2] = wv_byte[2:0] > 3'd4 ? 8'd2 : 8'd1;
    assign c_req_tag[2] = '0;
    always_ff @(posedge clk) begin
        wv_ack <= 1'b0;
        if (rst) wv_busy <= 1'b0;
        else if (c_req_valid[2] && c_req_ready[2]) begin
            wv_busy <= 1'b1;
            wv_two <= wv_byte[2:0] > 3'd4;
            wv_first <= 1'b1;
            wv_off <= wv_byte[2:0];
        end else if (wv_busy && c_rsp_valid[2]) begin
            if (wv_two && wv_first) begin
                wv_w0 <= c_rsp_data;
                wv_first <= 1'b0;
            end else begin
                logic [127:0] both;
                both = wv_two ? {c_rsp_data, wv_w0} : {64'h0, c_rsp_data};
                wv_data <= 32'(both >> (8 * wv_off));
                wv_ack <= 1'b1;
                wv_busy <= 1'b0;
            end
        end
    end

    // ---------------------------------------------------------------- 3: program ROM cache
    // 1024 lines of 4 words; tag = rom address bits 18:15
    logic [63:0] cdata [4096];
    logic [4:0]  ctag [1024];         // {valid, tag}
    logic [11:0] cd_ra, cd_wa;
    logic [63:0] cd_q, cd_wd;
    logic        cd_we;
    logic [4:0]  ct_q, ct_wd;
    logic [9:0]  ct_wa, sweep;
    logic        ct_we, sweeping;
    always_ff @(posedge clk) begin
        if (cd_we) cdata[cd_wa] <= cd_wd;
        cd_q <= cdata[cd_ra];
        if (ct_we) ctag[ct_wa] <= ct_wd;
        ct_q <= ctag[rom_addr[14:5]];
    end
    initial for (int i = 0; i < 1024; i++) ctag[i] = 5'h0;
    typedef enum logic [2:0] { R_IDLE, R_LOOK, R_HIT, R_MISS, R_FILL, R_ACK } r_t;
    r_t rq;
    logic [1:0] fill_n;
    assign cd_ra = {rom_addr[14:5], rom_addr[4:3]};
    assign c_req_valid[3] = rq == R_MISS;
    assign c_req_we[3] = 1'b0;
    assign c_req_addr[3] = W0 + 29'({rom_addr[18:5], 2'b00});
    assign c_req_wdata[3] = '0;
    assign c_req_be[3] = 8'hff;
    assign c_req_len[3] = 8'd4;
    assign c_req_tag[3] = '0;
    assign rom_ack = rq == R_ACK;
    // a new pack: every line invalid (one a clock, during the download)
    always_comb begin
        ct_we = sweeping || (rq == R_FILL && c_rsp_valid[3] && fill_n == 2'd3);
        ct_wa = sweeping ? sweep : rom_addr[14:5];
        ct_wd = sweeping ? 5'h0 : {1'b1, rom_addr[18:15]};
    end
    always_ff @(posedge clk) begin
        cd_we <= 1'b0;
        if (ld_wr && ld_addr == 23'd0) begin
            sweeping <= 1'b1;
            sweep <= 10'd0;
        end else if (sweeping) begin
            sweep <= sweep + 10'd1;
            if (sweep == 10'd1023) sweeping <= 1'b0;
        end
        if (rst) begin
            rq <= R_IDLE;
        end else begin
            case (rq)
                R_IDLE: if (rom_req) rq <= R_LOOK;        // tag and data addresses go in
                R_LOOK: rq <= R_HIT;                      // read
                R_HIT: begin
                    if (ct_q == {1'b1, rom_addr[18:15]}) begin
                        logic [5:0] o;
                        o = {rom_addr[2:1], 4'b0};        // bit offset of the even byte
                        rom_data <= {cd_q[o +: 8], cd_q[o + 8 +: 8]};
                        rq <= R_ACK;
                    end else
                        rq <= R_MISS;
                end
                R_MISS: if (c_req_ready[3]) begin
                    fill_n <= 2'd0;
                    rq <= R_FILL;
                end
                R_FILL: if (c_rsp_valid[3]) begin
                    cd_we <= 1'b1;
                    cd_wa <= {rom_addr[14:5], fill_n};
                    cd_wd <= c_rsp_data;
                    fill_n <= fill_n + 2'd1;
                    if (fill_n == 2'd3) rq <= R_IDLE;   // look again (the CPU still requests)
                end
                R_ACK: rq <= R_IDLE;
                default: rq <= R_IDLE;
            endcase
        end
    end
endmodule
