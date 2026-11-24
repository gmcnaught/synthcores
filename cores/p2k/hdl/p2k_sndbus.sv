// SPDX-License-Identifier: GPL-3.0-or-later
// Proteus 2000 sound-chip bus: the CPU side of the G20 voices (0x77F000), the H chips (0x500000) and
// the G05 (0x600000), as MAME's emup2k.cpp g20_r/g20_w, h_r/h_w, g05_r/g05_w (cores/p2k/docs/plan.md).
//
// Writes go to shadow copies of the registers (what CPU reads return) and into a FIFO; the engine
// (cores/proteus/p2k/rtl/snd/p2k_snd.sv) takes them while idle between samples, so a write takes
// effect at a sample boundary, as in MAME's model. A sample due (tick) starts before any write.
// The CPU waits only when the FIFO is full.
//
// Reads:
//   - G20 +0x1C/+0x1E: sound-memory ports A/B, pipelined as MAME: the latched word is returned and
//     the word at the port address fetched into the latch, the address incremented. The address
//     is set by writes to +0x30/+0x32 (A) and +0x34/+0x36 (B) of any voice, from that voice's
//     registers. Big/Baby by latch 0xB00000 bit 6; a slot beyond its loaded image reads 0 (Big) or
//     0xFFFF (Baby). Port B data writes (+0x1E) only step the address (ROM SIMMs).
//   - G20 +0x18/+0x1A: the voice's live address; +0x02: bit 12 the live end select. H +0x34/+0x36/
//     +0x3E: the block's live interpolator values; +0x3A: the block's +0x7A word. These wait until
//     the FIFO is empty and the engine idle, then read the engine.
//   - G05: +0x0E the index register (not passed to the engine); index 0x1E +0x18 the chip ID 0x30;
//     index 0x1B +0x18 the data-entry encoder's signed count since the last read.
//   - everything else: the shadow. H is shadowed below byte 0x8000 only (the voice blocks; every H
//     read in the corpus traces is there); above it reads return 0.
module p2k_sndbus #(
    parameter logic [28:0] SMP_BASE  = 29'h6000000,
    parameter logic [28:0] BABY_BASE = 29'h7100000
) (
    input  logic        clk,
    input  logic        rst,

    // CPU access
    input  logic        go,
    input  logic        we,
    input  logic [1:0]  chip,             // 0 G20, 1 H, 2 G05
    input  logic [18:0] woff,             // word offset in the chip's window
    input  logic [15:0] wdata,
    input  logic        uds,
    input  logic        lds,
    output logic        done,
    output logic [15:0] rdata,

    // board state
    input  logic        baby_sel,
    input  logic [7:0]  enc_count,        // encoder steps since the last take (signed)
    output logic        enc_take,
    input  logic [24:0] big_words [4],    // Big image words loaded per slot
    input  logic [20:0] baby_words [4],   // Baby image words loaded per slot

    // sample clock
    input  logic        tick,             // a 44.1 kHz sample is due
    output logic        late,             // a tick came while the previous was still pending (sticky)

    // engine
    input  logic        snd_idle,
    output logic        snd_start,
    output logic        snd_wr_valid,
    output logic [1:0]  snd_wr_chip,
    output logic [18:0] snd_wr_addr,
    output logic [15:0] snd_wr_data,
    output logic [15:0] snd_wr_mask,
    output logic        snd_rd_valid,
    output logic        snd_rd_chip,
    output logic [7:0]  snd_rd_addr,
    input  logic        snd_rd_done,
    input  logic [25:0] snd_rd_vaddr,
    input  logic        snd_rd_vsel,
    input  logic [15:0] snd_rd_ipx [0:2],

    // memory port client (port reads; CTW 1)
    output logic        req_valid,
    input  logic        req_ready,
    output logic [28:0] req_addr,
    input  logic        rsp_valid,
    input  logic [63:0] rsp_data,

    output logic [9:0]  fifo_level
);
    // ------------------------------------------------------------------ shadows
    logic [13:0] sh_ra, sh_wa;
    logic [1:0]  sh_rchip, sh_wchip, sh_be;
    logic [15:0] sh_wd, q_g20, q_h, q_g05;
    logic        sh_we;
    // one simple dual-port RAM per chip and byte lane (block RAM)
    p2k_sb_ram #(.AW(12)) u_g20h (.clk, .we(sh_we && sh_wchip == 2'd0 && sh_be[1]), .wa(sh_wa[11:0]), .wd(sh_wd[15:8]), .ra(sh_ra[11:0]), .q(q_g20[15:8]));
    p2k_sb_ram #(.AW(12)) u_g20l (.clk, .we(sh_we && sh_wchip == 2'd0 && sh_be[0]), .wa(sh_wa[11:0]), .wd(sh_wd[7:0]),  .ra(sh_ra[11:0]), .q(q_g20[7:0]));
    p2k_sb_ram #(.AW(14)) u_hh   (.clk, .we(sh_we && sh_wchip == 2'd1 && sh_be[1]), .wa(sh_wa),       .wd(sh_wd[15:8]), .ra(sh_ra),       .q(q_h[15:8]));
    p2k_sb_ram #(.AW(14)) u_hl   (.clk, .we(sh_we && sh_wchip == 2'd1 && sh_be[0]), .wa(sh_wa),       .wd(sh_wd[7:0]),  .ra(sh_ra),       .q(q_h[7:0]));
    p2k_sb_ram #(.AW(11)) u_g05h (.clk, .we(sh_we && sh_wchip == 2'd2 && sh_be[1]), .wa(sh_wa[10:0]), .wd(sh_wd[15:8]), .ra(sh_ra[10:0]), .q(q_g05[15:8]));
    p2k_sb_ram #(.AW(11)) u_g05l (.clk, .we(sh_we && sh_wchip == 2'd2 && sh_be[0]), .wa(sh_wa[10:0]), .wd(sh_wd[7:0]),  .ra(sh_ra[10:0]), .q(q_g05[7:0]));
    wire [15:0] sh_q = sh_rchip == 2'd0 ? q_g20 : sh_rchip == 2'd1 ? q_h : q_g05;

    // ------------------------------------------------------------------ write FIFO (show-ahead)
    localparam int FW = 2 + 19 + 16 + 16;
    logic [FW-1:0] fmem [0:511];
    logic [8:0]    f_wp, f_rp;
    logic [9:0]    f_mem_n;             // entries in fmem
    logic          f_push, f_fetch, f_fetched, head_v;
    logic [FW-1:0] f_din, f_q, head;
    wire           f_pop = snd_wr_valid && snd_idle && !snd_start;
    assign fifo_level = f_mem_n + 10'(head_v);
    wire           f_full = fifo_level >= 10'd510;
    always_ff @(posedge clk) begin
        if (f_push) fmem[f_wp] <= f_din;
        f_q <= fmem[f_rp];
    end
    always_ff @(posedge clk) begin
        if (rst) begin
            f_wp <= '0; f_rp <= '0; f_mem_n <= '0; head_v <= 1'b0; f_fetched <= 1'b0;
        end else begin
            f_fetched <= f_fetch;
            if (f_push) f_wp <= f_wp + 9'd1;
            if (f_fetch) f_rp <= f_rp + 9'd1;
            f_mem_n <= f_mem_n + 10'(f_push) - 10'(f_fetch);
            if (f_fetched) begin head <= f_q; head_v <= 1'b1; end
            else if (f_pop) head_v <= 1'b0;
        end
    end
    assign f_fetch = !rst && f_mem_n != 0 && !f_fetched && (!head_v || f_pop) && !(head_v && !f_pop);

    // ------------------------------------------------------------------ engine schedule
    logic pend, rd_req, rd_busy;
    logic        rd_chip_l;
    logic [7:0]  rd_addr_l;
    assign snd_start    = pend && snd_idle;
    assign snd_wr_valid = head_v && !pend;
    assign {snd_wr_chip, snd_wr_addr, snd_wr_data, snd_wr_mask} = head;
    wire   quiet = !head_v && f_mem_n == 0 && !f_push && !f_fetched;
    assign snd_rd_valid = rd_req && !rd_busy && !pend && quiet;
    assign snd_rd_chip = rd_chip_l;
    assign snd_rd_addr = rd_addr_l;
    always_ff @(posedge clk) begin
        if (rst) begin
            pend <= 1'b0; late <= 1'b0; rd_busy <= 1'b0;
        end else begin
            if (tick) begin
                pend <= 1'b1;
                if (pend && !snd_start) late <= 1'b1;
            end else if (snd_start)
                pend <= 1'b0;
            if (snd_rd_valid && snd_idle && !snd_start && !snd_wr_valid) rd_busy <= 1'b1;
            if (snd_rd_done) rd_busy <= 1'b0;
        end
    end

    // ------------------------------------------------------------------ sound-memory ports
    logic [25:0] p_addr [2];
    logic [15:0] p_latch [2];
    logic        pf_busy, pf_wait, pf_port, pf_zero;
    logic [15:0] pf_fill;
    logic [1:0]  pf_lane;
    logic [28:0] pf_addr;
    assign req_valid = pf_busy && !pf_wait && !pf_zero;
    assign req_addr = pf_addr;

    // ------------------------------------------------------------------ CPU access state
    typedef enum logic [3:0] { B_IDLE, B_WFULL, B_SHRD, B_SHW, B_SHQ, B_LIVE, B_LIVEQ, B_PORT, B_PADDR1,
                               B_PADDR2, B_PADDR3 } bst_t;
    bst_t        st;
    logic        l_uds, l_lds;
    logic [1:0]  l_chip;
    logic [18:0] l_off;
    logic [15:0] l_wd;
    logic [4:0]  g05_idx;
    logic [15:0] g05_index;
    logic [1:0]  l_kind;             // live: 0 voice address hi, 1 lo, 2 end select, 3 interpolator
    logic [1:0]  l_ipn;
    logic [15:0] l_hi;
    logic        l_port;

    wire [6:0]  l_voice = l_off[11:5];
    wire [4:0]  l_word = l_off[4:0];
    wire [15:0] l_mask = {{8{l_uds}}, {8{l_lds}}};

    // a fetch for port p at address a (MAME snd_read)
    task automatic port_fetch(input logic p, input logic [25:0] a);
        automatic logic [1:0]  slot = a[25:24];
        automatic logic [23:0] off = a[23:0];
        pf_port <= p;
        pf_busy <= 1'b1;
        pf_wait <= 1'b0;
        pf_zero <= 1'b0;
        if (baby_sel) begin
            pf_addr <= BABY_BASE + 29'({slot, off[19:2]});
            pf_lane <= off[1:0];
            if (off >= 24'h100000 || {3'd0, off[20:0]} >= {3'd0, baby_words[slot]}) begin
                pf_zero <= 1'b1;
                pf_fill <= 16'hffff;
            end
        end else begin
            pf_addr <= SMP_BASE + 29'(a[25:2]);
            pf_lane <= a[1:0];
            if ({1'b0, off} >= big_words[slot]) begin
                pf_zero <= 1'b1;
                pf_fill <= 16'h0000;
            end
        end
    endtask

    always_ff @(posedge clk) begin
        done <= 1'b0;
        sh_we <= 1'b0;
        f_push <= 1'b0;
        enc_take <= 1'b0;
        if (rst) begin
            st <= B_IDLE;
            rd_req <= 1'b0;
            pf_busy <= 1'b0;
            pf_wait <= 1'b0;
            g05_index <= '0;
            p_addr[0] <= '0; p_addr[1] <= '0;
            p_latch[0] <= '0; p_latch[1] <= '0;
        end else begin
            // port fetch: request, then the response or a constant
            if (pf_busy) begin
                if (pf_zero) begin
                    p_latch[pf_port] <= pf_fill;
                    pf_busy <= 1'b0;
                end else if (!pf_wait && req_ready)
                    pf_wait <= 1'b1;
                else if (pf_wait && rsp_valid) begin
                    p_latch[pf_port] <= rsp_data[16*pf_lane +: 16];
                    pf_busy <= 1'b0;
                    pf_wait <= 1'b0;
                end
            end
            if (snd_rd_valid && snd_idle && !snd_start && !snd_wr_valid) rd_req <= 1'b0;

            case (st)
            B_IDLE:
                if (go) begin
                    l_chip <= chip; l_off <= woff; l_wd <= wdata; l_uds <= uds; l_lds <= lds;
                    g05_idx <= g05_index[4:0];
                    if (chip == 2'd2 && woff[5:0] == 6'h07) begin        // G05 index register
                        if (we) begin
                            if (uds) g05_index[15:8] <= wdata[15:8];
                            if (lds) g05_index[7:0] <= wdata[7:0];
                        end
                        rdata <= g05_index;
                        done <= 1'b1;
                    end else if (we)
                        st <= B_WFULL;
                    else
                        st <= B_SHRD;
                end
            B_WFULL:                                                 // a write: shadow and FIFO
                if (!f_full) begin
                    automatic logic [15:0] d = (l_chip == 2'd0 && l_word == 5'd31) ? (l_wd & 16'h03ff) : l_wd;
                    automatic logic [18:0] fa = l_chip == 2'd2 ? {8'd0, g05_idx, l_off[5:0]} : l_off;
                    f_push <= 1'b1;
                    f_din <= {l_chip, fa, l_wd, l_mask};
                    sh_we <= l_chip != 2'd1 || l_off < 19'h4000;
                    sh_wchip <= l_chip;
                    sh_wa <= l_chip == 2'd2 ? {3'd0, g05_idx, l_off[5:0]} : l_off[13:0];
                    sh_wd <= d;
                    sh_be <= {l_uds, l_lds};
                    if (l_chip == 2'd0 && l_word == 5'd15) begin       // port B data: step the address
                        p_addr[1] <= p_addr[1] + 26'd1;
                        done <= 1'b1;
                        st <= B_IDLE;
                    end else if (l_chip == 2'd0 && l_word[4:2] == 3'b110) begin   // +0x30-0x36: port address
                        l_port <= l_word[1];
                        st <= B_PADDR1;
                    end else begin
                        done <= 1'b1;
                        st <= B_IDLE;
                    end
                end
            B_PADDR1: begin                                          // the shadow write lands
                sh_rchip <= 2'd0;
                sh_ra <= {2'd0, l_voice, 3'b110, l_port, 1'b0};       // +0x30 / +0x34
                st <= B_PADDR2;
            end
            B_PADDR2: begin
                sh_ra <= {2'd0, l_voice, 3'b110, l_port, 1'b1};       // +0x32 / +0x36
                st <= B_PADDR3;
            end
            B_PADDR3: begin
                l_hi <= sh_q;                                        // +0x30 / +0x34 read
                st <= B_LIVEQ;                                       // the low half next clock
                l_kind <= 2'd0;
            end
            B_SHRD: begin                                            // a read: decide the source
                st <= B_SHW;
                sh_rchip <= l_chip;
                sh_ra <= l_chip == 2'd2 ? {3'd0, g05_idx, l_off[5:0]} : l_off[13:0];
                case (l_chip)
                2'd0:
                    if (l_word == 5'd14 || l_word == 5'd15) begin    // ports A/B
                        l_port <= l_word[0];
                        st <= B_PORT;
                    end else if (l_word == 5'd12 || l_word == 5'd13 || l_word == 5'd1) begin
                        l_kind <= l_word == 5'd12 ? 2'd0 : l_word == 5'd13 ? 2'd1 : 2'd2;
                        rd_req <= 1'b1;
                        rd_chip_l <= 1'b0;
                        rd_addr_l <= {1'b0, l_voice};
                        st <= B_LIVE;
                    end
                2'd1:
                    if (l_off >= 19'h4000) begin
                        rdata <= '0;
                        done <= 1'b1;
                        st <= B_IDLE;
                    end else if (l_off[5:0] == 6'h1a || l_off[5:0] == 6'h1b || l_off[5:0] == 6'h1f) begin
                        l_kind <= 2'd3;
                        l_ipn <= l_off[5:0] == 6'h1a ? 2'd0 : l_off[5:0] == 6'h1b ? 2'd1 : 2'd2;
                        rd_req <= 1'b1;
                        rd_chip_l <= 1'b1;
                        rd_addr_l <= l_off[13:6];
                        st <= B_LIVE;
                    end else if (l_off[5:0] == 6'h1d)
                        sh_ra <= {l_off[13:6], 6'h3d};                // +0x3A: the +0x7A word
                default:
                    if (l_off[5:0] == 6'h0c && g05_idx == 5'h1e) begin
                        rdata <= 16'h0030;
                        done <= 1'b1;
                        st <= B_IDLE;
                    end else if (l_off[5:0] == 6'h0c && g05_idx == 5'h1b) begin
                        rdata <= {8'h00, enc_count};
                        enc_take <= 1'b1;
                        done <= 1'b1;
                        st <= B_IDLE;
                    end
                endcase
            end
            B_SHW: st <= B_SHQ;                                      // the shadow read address lands
            B_SHQ: begin                                             // shadow word read
                rdata <= sh_q;
                done <= 1'b1;
                st <= B_IDLE;
            end
            B_LIVE:
                if (snd_rd_done) begin
                    case (l_kind)
                    2'd0: begin rdata <= {6'd0, snd_rd_vaddr[25:16]}; done <= 1'b1; st <= B_IDLE; end
                    2'd1: begin rdata <= snd_rd_vaddr[15:0]; done <= 1'b1; st <= B_IDLE; end
                    2'd2: st <= B_LIVEQ;                             // shadow word with the live bit 12
                    default: begin rdata <= snd_rd_ipx[l_ipn]; done <= 1'b1; st <= B_IDLE; end
                    endcase
                end
            B_LIVEQ:
                if (l_kind == 2'd2) begin                            // +0x02: shadow (read in B_SHRD)
                    rdata <= {q_g20[15:13], snd_rd_vsel, q_g20[11:0]};
                    done <= 1'b1;
                    st <= B_IDLE;
                end else begin                                       // port address: low half
                    p_addr[l_port] <= {l_hi[9:0], sh_q};
                    done <= 1'b1;
                    st <= B_IDLE;
                end
            B_PORT:
                if (!pf_busy) begin
                    rdata <= p_latch[l_port];
                    p_addr[l_port] <= p_addr[l_port] + 26'd1;
                    port_fetch(l_port, p_addr[l_port]);
                    done <= 1'b1;
                    st <= B_IDLE;
                end
            default: st <= B_IDLE;
            endcase
        end
    end
endmodule

// a shadow byte lane: simple dual-port block RAM, registered read, initially 0 (MAME's registers)
module p2k_sb_ram #(
    parameter int AW = 12
) (
    input  logic          clk,
    input  logic          we,
    input  logic [AW-1:0] wa,
    input  logic [7:0]    wd,
    input  logic [AW-1:0] ra,
    output logic [7:0]    q
);
    logic [7:0] mem [0:(1 << AW) - 1];
    // block RAM powers up zero; the loop is for simulation (Quartus stops at 5000 iterations)
    // synthesis translate_off
    initial for (int i = 0; i < (1 << AW); i++) mem[i] = 8'h00;
    // synthesis translate_on
    always_ff @(posedge clk) begin
        if (we) mem[wa] <= wd;
        q <= mem[ra];
    end
endmodule
