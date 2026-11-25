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
//     0xFFFF (Baby). Port B data writes (+0x1E) step the address; on a ROM SIMM they are lost.
//   - User flash SIMMs (flash_slot): port B data writes go to the slot's flash chips, MAME's
//     p2k::intel_flash (cores/proteus/p2k/model/p2k_flash.h): Big is four x16 chips of 4 M words
//     (word address bits 23-22; maker 0x0089, device 0x0015, 64 K-word blocks, buffered program
//     0xE8), Baby one x16 chip of 1 M words (maker 0x00B0, device 0x00D0, 32 K-word blocks).
//     Commands 0x40/0x10 program, 0x20 + 0xD0 erase, 0xE8 + count + data + 0xD0 buffered program,
//     0x70 status, 0x90 ID, 0x50 clear status, anything else read array. Program ANDs the word
//     into DDR3 (read, then write the lane); erase fills the block with 0xFFFF in fills of 32 words.
//     Each memory request starts only while the engine is idle with no sample due, so at most one
//     short request delays a sample's start (the erase of a 64 K-word block would otherwise hold
//     the memory for a whole sample). Both complete at
//     once: in any mode but read array and ID a port read returns 0x0080 (ready). The CPU waits
//     for the operation. Each Big chip has its own write buffer (32 words), as MAME's.
//     The modes return to read array while flash_reset (the CPU is held). fl_dirty marks the
//     256 KB region of the slot's file (Baby bytes 0-2 MB: 0-7, Big from 2 MB: 8-135) that a
//     program or erase changed.
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
    input  logic [3:0]  flash_slot,       // the slot is a user flash SIMM
    input  logic        flash_reset,      // flash chips to read array
    output logic        fl_dirty,         // a program or erase changed region fl_dirty_region of slot fl_dirty_slot
    output logic [1:0]  fl_dirty_slot,
    output logic [7:0]  fl_dirty_region,

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

    // memory port client (port reads, flash programs and erases; CTW 1)
    output logic        req_valid,
    input  logic        req_ready,
    output logic        req_we,
    output logic [28:0] req_addr,
    output logic [63:0] req_wdata,
    output logic [7:0]  req_be,
    output logic [7:0]  req_len,
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
    logic fl_ok;                        // a flash memory request may start
    logic        rd_chip_l;
    logic [7:0]  rd_addr_l;
    assign snd_start    = pend && snd_idle;
    assign snd_wr_valid = head_v && !pend;
    assign {snd_wr_chip, snd_wr_addr, snd_wr_data, snd_wr_mask} = head;
    wire   quiet = !head_v && f_mem_n == 0 && !f_push && !f_fetched;
    assign fl_ok = snd_idle && !pend && !tick;
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

    // ------------------------------------------------------------------ user flash SIMMs
    localparam logic [2:0] M_ARRAY = 3'd0, M_ID = 3'd1, M_STATUS = 3'd2, M_PROG = 3'd3, M_ERASE = 3'd4,
                           M_BCOUNT = 3'd5, M_BDATA = 3'd6, M_BCONF = 3'd7;
    logic [2:0]  fm [4][5];             // chip modes: Big chips 0-3, Baby 4
    logic [25:0] fl_a;                  // the port B write's word address
    logic        fl_bsel;               // ... to Baby
    logic [8:0]  b_count [4][4], b_got [4][4];   // buffered program per Big chip: words announced, received
    logic [8:0]  b_i, b_n;              // programming a buffer: word, words
    logic [1:0]  b_c;                   // ... of chip b_c of fl_slot
    logic [39:0] bbuf [0:511];          // {slot, chip, entry}: {word offset in the slot, data}
    logic [39:0] bb_q;
    logic [8:0]  bb_ra;
    logic        bb_we;
    logic [8:0]  bb_wa;
    logic [39:0] bb_wd;
    always_ff @(posedge clk) begin
        if (bb_we) bbuf[bb_wa] <= bb_wd;
        bb_q <= bbuf[bb_ra];
    end
    logic        fl_req, fl_we, fl_buf;  // a request; a write; programming the buffer
    logic [28:0] fl_addr;
    logic [63:0] fl_wdata;
    logic [7:0]  fl_be, fl_len;
    logic [23:0] fp_off;                // the word being programmed
    logic [15:0] fp_d, fp_old;
    logic [9:0]  fe_n;                  // erase: fills issued
    logic [1:0]  fl_ph;                 // program: 0 read, 1 read taken, 2 word back, 3 write taken
    wire  [1:0]  fl_slot = fl_a[25:24];
    // a slot word's DDR3 word
    function automatic logic [28:0] fl_word(input logic [1:0] sl, input logic bs, input logic [23:0] o);
        return bs ? BABY_BASE + 29'({sl, o[19:2]}) : SMP_BASE + 29'({sl, o[23:2]});
    endfunction
    function automatic logic [7:0] fl_region(input logic bs, input logic [23:0] o);
        return bs ? {5'd0, o[19:17]} : 8'(o[23:17]) + 8'd8;
    endfunction

    assign req_valid = (pf_busy && !pf_wait && !pf_zero) || fl_req;
    assign req_addr  = fl_req ? fl_addr : pf_addr;
    assign req_we    = fl_req && fl_we;
    assign req_wdata = fl_wdata;
    assign req_be    = fl_req ? fl_be : 8'hff;
    assign req_len   = fl_req ? fl_len : 8'd1;

    // ------------------------------------------------------------------ CPU access state
    typedef enum logic [3:0] { B_IDLE, B_WFULL, B_SHRD, B_SHW, B_SHQ, B_LIVE, B_LIVEQ, B_PORT, B_PADDR1,
                               B_PADDR2, B_PADDR3, B_FL, B_FLP, B_FLB, B_FLB1, B_FLE } bst_t;
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
        if (flash_slot[slot]) begin                             // p2k::intel_flash::read
            automatic logic [2:0] m = baby_sel ? fm[slot][4] : fm[slot][{1'b0, off[23:22]}];
            pf_addr <= fl_word(slot, baby_sel, off);
            pf_lane <= off[1:0];
            if (baby_sel && off >= 24'h100000) begin
                pf_zero <= 1'b1;
                pf_fill <= 16'hffff;
            end else if (m == M_ID) begin
                pf_zero <= 1'b1;
                pf_fill <= baby_sel ? (off[0] ? 16'h00d0 : 16'h00b0) : (off[0] ? 16'h0015 : 16'h0089);
            end else if (m != M_ARRAY) begin
                pf_zero <= 1'b1;
                pf_fill <= 16'h0080;
            end
        end else if (baby_sel) begin
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
        bb_we <= 1'b0;
        fl_dirty <= 1'b0;
        if (rst || flash_reset)
            for (int sl = 0; sl < 4; sl++)
                for (int c = 0; c < 5; c++) fm[sl][c] <= M_ARRAY;
        if (rst) begin
            fl_req <= 1'b0;
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
                        fl_a <= p_addr[1];
                        fl_bsel <= baby_sel;
                        if (flash_slot[p_addr[1][25:24]])
                            st <= B_FL;
                        else begin
                            done <= 1'b1;
                            st <= B_IDLE;
                        end
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
            // ---------------------------------------------- flash: p2k::intel_flash::write
            B_FL: begin
                automatic logic [23:0] off = fl_a[23:0];
                automatic logic [2:0]  c = fl_bsel ? 3'd4 : {1'b0, off[23:22]};
                automatic logic [2:0]  m = fm[fl_slot][c];
                automatic logic [7:0]  cmd = l_wd[7:0];
                automatic logic        fin = 1'b1;
                if (!(fl_bsel && off >= 24'h100000)) begin       // beyond Baby: ignored
                    case (m)
                    M_PROG: begin
                        fm[fl_slot][c] <= M_STATUS;
                        fp_off <= off;
                        fp_d <= l_wd;
                        fl_buf <= 1'b0;
                        fl_ph <= 2'd0;
                        st <= B_FLP;
                        fin = 1'b0;
                    end
                    M_ERASE: begin
                        fm[fl_slot][c] <= M_STATUS;
                        if (cmd == 8'hd0) begin
                            fl_addr <= fl_bsel ? BABY_BASE + 29'({fl_slot, off[19:15], 13'd0})
                                               : SMP_BASE + 29'({fl_slot, off[23:16], 14'd0});
                            fe_n <= '0;
                            st <= B_FLE;
                            fin = 1'b0;
                        end
                    end
                    M_BCOUNT: begin
                        b_count[fl_slot][c[1:0]] <= 9'(cmd) + 9'd1;
                        b_got[fl_slot][c[1:0]] <= '0;
                        fm[fl_slot][c] <= M_BDATA;
                    end
                    M_BDATA: begin
                        automatic logic [8:0] g = b_got[fl_slot][c[1:0]];
                        if (g < 9'd32) begin
                            bb_we <= 1'b1;
                            bb_wa <= {fl_slot, c[1:0], g[4:0]};
                            bb_wd <= {off, l_wd};
                        end
                        b_got[fl_slot][c[1:0]] <= g + 9'd1;
                        if (g + 9'd1 >= b_count[fl_slot][c[1:0]]) fm[fl_slot][c] <= M_BCONF;
                    end
                    M_BCONF: begin
                        automatic logic [8:0] n = b_count[fl_slot][c[1:0]];
                        fm[fl_slot][c] <= M_STATUS;
                        if (cmd == 8'hd0) begin
                            b_i <= '0;
                            b_n <= n < 9'd32 ? n : 9'd32;
                            b_c <= c[1:0];
                            st <= B_FLB;
                            fin = 1'b0;
                        end
                    end
                    default:
                        case (cmd)
                        8'h40, 8'h10: fm[fl_slot][c] <= M_PROG;
                        8'h20: fm[fl_slot][c] <= M_ERASE;
                        8'he8: fm[fl_slot][c] <= c != 3'd4 ? M_BCOUNT : M_ARRAY;
                        8'h70: fm[fl_slot][c] <= M_STATUS;
                        8'h50: ;
                        8'h90: fm[fl_slot][c] <= M_ID;
                        default: fm[fl_slot][c] <= M_ARRAY;
                        endcase
                    endcase
                end
                if (fin) begin
                    done <= 1'b1;
                    st <= B_IDLE;
                end
            end
            B_FLB:                                           // buffered: the next word
                if (b_i < b_n) begin
                    bb_ra <= {fl_slot, b_c, b_i[4:0]};
                    st <= B_FLB1;
                end else begin
                    done <= 1'b1;
                    st <= B_IDLE;
                end
            B_FLB1: begin                                    // bb_q is valid in B_FLP
                fl_buf <= 1'b1;
                fl_ph <= 2'd0;
                st <= B_FLP;
            end
            B_FLP:                                           // program one word: read, AND, write
                case (fl_ph)
                2'd0: if (!pf_busy && fl_ok) begin
                    automatic logic [23:0] o = fl_buf ? bb_q[39:16] : fp_off;
                    if (fl_buf) begin
                        fp_off <= bb_q[39:16];
                        fp_d <= bb_q[15:0];
                    end
                    fl_req <= 1'b1;
                    fl_we <= 1'b0;
                    fl_addr <= fl_word(fl_slot, fl_bsel, o);
                    fl_len <= 8'd1;
                    fl_ph <= 2'd1;
                end
                2'd1: if (req_ready) begin
                    fl_req <= 1'b0;
                    fl_ph <= 2'd2;
                end
                2'd2: if (rsp_valid) begin
                    fl_req <= 1'b1;
                    fl_we <= 1'b1;
                    fl_wdata <= {4{rsp_data[16 * fp_off[1:0] +: 16] & fp_d}};
                    fl_be <= 8'b11 << {fp_off[1:0], 1'b0};
                    fl_ph <= 2'd3;
                end
                default: if (req_ready) begin
                    fl_req <= 1'b0;
                    fl_we <= 1'b0;
                    fl_dirty <= 1'b1;
                    fl_dirty_slot <= fl_slot;
                    fl_dirty_region <= fl_region(fl_bsel, fp_off);
                    if (fl_buf) begin
                        b_i <= b_i + 9'd1;
                        st <= B_FLB;
                    end else begin
                        done <= 1'b1;
                        st <= B_IDLE;
                    end
                end
                endcase
            B_FLE:                                           // erase: fills of 32 words
                if (!fl_req && !pf_busy && fl_ok) begin
                    fl_req <= 1'b1;
                    fl_we <= 1'b1;
                    fl_wdata <= '1;
                    fl_be <= 8'hff;
                    fl_len <= 8'd32;
                end else if (fl_req && req_ready) begin
                    fl_req <= 1'b0;
                    fl_we <= 1'b0;
                    fl_addr <= fl_addr + 29'd32;
                    fe_n <= fe_n + 10'd1;
                    if (fe_n == (fl_bsel ? 10'd255 : 10'd511)) begin
                        fl_dirty <= 1'b1;
                        fl_dirty_slot <= fl_slot;
                        fl_dirty_region <= fl_region(fl_bsel, fl_a[23:0]);
                        done <= 1'b1;
                        st <= B_IDLE;
                    end
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
