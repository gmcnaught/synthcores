// SPDX-License-Identifier: GPL-3.0-or-later
// Proteus 2000 CPU memory: the 2 MB boot/OS/user flash (28F160) and the 8 MB DRAM, both in DDR3,
// behind a direct-mapped block-RAM cache (cores/p2k/docs/plan.md).
//
// Address space P (byte addresses, 24 bits): flash at 0x000000-0x1FFFFF, DRAM at
// 0x200000-0x9FFFFF. 16-bit word w of P is lane w[1:0] (bits 16 w[1:0] +: 16) of the 64-bit DDR3
// word BASE + P[23:3]. Cache: 32 KB, 1024 lines of 32 bytes (4 DDR3 words, one burst), tag P[23:15].
//   - reads: a hit takes the lookup clock; a miss waits until the posted writes are out, reads the
//     line (burst of 4), fills it and returns the word;
//   - writes: write-through (posted, 8-deep queue), the line updated on a hit, no allocate.
// Flash: the command subset of p2k/sim/p2k_cpu_tb.cpp (MAME's intelfsh16 SHARP_LH28F160S3: maker
// 0xB0, device 0xD0), every operation complete at once (status 0x80):
//   read array (FF/F0), read ID (90), read status (70), clear status (50), word program (40/10),
//   block erase (20 D0, 64 KB), master lock (60 F1/D0), the AMD AA/55 sequences (90 ID, A0 program).
//   Program writes old & data (read through the cache, then written through). Erase writes the
//   block with all ones and invalidates the whole cache. Reads outside read-array mode return the
//   status or ID and bypass the cache.
module p2k_mem #(
    parameter logic [28:0] BASE = 29'h7200000
) (
    input  logic        clk,
    input  logic        rst,
    input  logic        flush,            // invalidate the cache (level; held through reset)
    output logic        busy_flush,
    output logic        quiet,            // idle, flushed, nothing outstanding: the CPU may leave reset
    output logic        rsp_extra,        // a response arrived with no read outstanding (sticky; diagnostics)
    output logic [63:0] dbg_rsp0,         // diagnostics: the first response after a flush
    output logic [63:0] dbg_dq,           // and the data word looked up by the third access

    // CPU access: go (level) while the CPU presents an access here; ack (combinational) completes
    // it in that clock with rdata. A hit is acknowledged in the clock after go; the access must
    // not change until ack.
    input  logic        go,
    input  logic        we,
    input  logic        flash,            // P < 0x200000
    input  logic [23:1] pa,               // word address in P
    input  logic [15:0] wdata,
    input  logic        uds,              // D15-8
    input  logic        lds,              // D7-0
    output logic        ack,
    output logic [15:0] rdata,

    input  logic        flash_lock,       // the flash is being saved: flash writes wait
    output logic        flash_changed,    // one clock: a program or erase changed the flash

    // memory port client (synthframe_mem_ddr3 contract, CTW 1)
    output logic        req_valid,
    input  logic        req_ready,
    output logic        req_we,
    output logic [28:0] req_addr,
    output logic [63:0] req_wdata,
    output logic [7:0]  req_be,
    output logic [7:0]  req_len,
    output logic [0:0]  req_tag,
    input  logic        rsp_valid,
    input  logic [63:0] rsp_data
);
    // ------------------------------------------------------------------ cache arrays
    logic [63:0] dmem [0:4095];
    logic [9:0]  tmem [0:1023];          // {valid, tag[8:0]}
    logic [11:0] d_ra, d_wa;
    logic [63:0] d_q, d_wd;
    logic [7:0]  d_be;
    logic        d_we;
    logic [9:0]  t_ra, t_wa, t_q, t_wd;
    logic        t_we;
    always_ff @(posedge clk) begin
        for (int i = 0; i < 8; i++)
            if (d_we && d_be[i]) dmem[d_wa][i*8 +: 8] <= d_wd[i*8 +: 8];
        d_q <= dmem[d_ra];
    end
    always_ff @(posedge clk) begin
        if (t_we) tmem[t_wa] <= t_wd;
        t_q <= tmem[t_ra];
    end

    // ------------------------------------------------------------------ write queue (posted)
    localparam int WQ = 8;
    // registers, not MLAB: the outputs go straight into the clock crossing (hdl/p2k_x21.sv)
    (* ramstyle = "logic" *) logic [28:0] wq_a  [0:WQ-1];
    (* ramstyle = "logic" *) logic [63:0] wq_d  [0:WQ-1];
    (* ramstyle = "logic" *) logic [7:0]  wq_be [0:WQ-1];
    logic [2:0]  wq_rd, wq_wr;
    logic [3:0]  wq_n;
    logic        wq_push;
    logic [28:0] wq_pa;
    logic [63:0] wq_pd;
    logic [7:0]  wq_pbe;
    wire         wq_full = wq_n == 4'(WQ);
    wire         wq_empty = wq_n == 0;

    // a line read waits for the queue: the backend keeps this client's requests in order
    logic        rd_req;
    logic [28:0] rd_addr;
    assign req_valid = !wq_empty || rd_req;
    assign req_we    = !wq_empty;
    assign req_addr  = !wq_empty ? wq_a[wq_rd] : rd_addr;
    assign req_wdata = wq_d[wq_rd];
    assign req_be    = !wq_empty ? wq_be[wq_rd] : 8'hff;
    assign req_len   = !wq_empty ? 8'd1 : 8'd4;
    assign req_tag   = 1'b0;
    wire wq_pop = !wq_empty && req_ready;

    always_ff @(posedge clk) begin
        if (rst) begin
            wq_rd <= '0; wq_wr <= '0; wq_n <= '0;
        end else begin
            if (wq_push) begin
                wq_a[wq_wr] <= wq_pa; wq_d[wq_wr] <= wq_pd; wq_be[wq_wr] <= wq_pbe;
                wq_wr <= wq_wr + 3'd1;
            end
            if (wq_pop) wq_rd <= wq_rd + 3'd1;
            wq_n <= wq_n + 4'(wq_push) - 4'(wq_pop);
        end
    end

    // ------------------------------------------------------------------ flash command state
    typedef enum logic [3:0] { F_NORMAL, F_READID, F_STATUS, F_WRITE1, F_CLEAR1, F_MASTER,
                               F_AMD1, F_AMD2, F_AMD3, F_BYTEPROG } fmode_t;
    fmode_t      fmode;
    logic [7:0]  fstatus;
    logic        fmaster;

    // ------------------------------------------------------------------ access state
    typedef enum logic [3:0] { M_IDLE, M_LOCK, M_LOOK, M_MISS, M_FILL, M_WLOOK, M_PROG, M_ERASE,
                               M_FLUSH } mst_t;
    mst_t        st;
    logic        l_prog, l_erase;
    logic [23:1] l_pa;
    logic [15:0] l_wd;
    logic        l_uds, l_lds;
    logic [1:0]  beat;
    logic [15:0] l_old;              // the word read (program: before the AND)
    logic [12:0] er_n;               // erase: DDR3 words written
    logic [28:0] er_a;
    logic [10:0] fl_n;               // flush: tags cleared
    logic        flush_seen;

    wire [8:0]   l_tag = l_pa[23:15];
    wire [9:0]   l_idx = l_pa[14:5];
    wire [1:0]   l_lane = l_pa[2:1];
    wire [28:0]  l_word = BASE + 29'(l_pa[23:3]);
    wire [7:0]   l_be = 8'({l_uds, l_lds}) << {l_lane, 1'b0};
    wire [63:0]  l_wd64 = {4{l_wd}};
    wire         hit = t_q == {1'b1, l_tag};
    wire [19:0]  fw = l_pa[20:1];    // flash word address (TB/MAME: w)
    wire [7:0]   fc = l_wd[7:0];
    wire [15:0]  d_lane = d_q[16*l_lane +: 16];

    assign busy_flush = st == M_FLUSH || flush_seen;
    logic [3:0] outst;                  // read words requested and not yet returned
    assign quiet = st == M_IDLE && !flush_seen && !flush && !rd_req && !done && wq_empty && outst == 0;
    always_ff @(posedge clk) begin
        if (rst) begin
            outst <= '0;
            rsp_extra <= 1'b0;
        end else begin
            outst <= outst + ((req_valid && req_ready && !req_we) ? 4'd4 : 4'd0) - (rsp_valid ? 4'd1 : 4'd0);
            if (rsp_valid && outst == 0) rsp_extra <= 1'b1;
        end
    end
    logic [1:0] dbg_n;
    logic       dbg_got;
    always_ff @(posedge clk) begin
        if (flush) begin dbg_got <= 1'b0; dbg_n <= '0; end
        if (rsp_valid && !dbg_got && !flush) begin dbg_rsp0 <= rsp_data; dbg_got <= 1'b1; end
        if (st == M_LOOK && dbg_n != 2'd3) begin
            dbg_n <= dbg_n + 2'd1;
            if (dbg_n == 2'd1) dbg_dq <= d_q;
        end
    end

    logic        done;                  // registered completion (fill, flash, erase)
    logic [15:0] rdata_r;
    wire         look_ack = 1'b0;           // hits complete through done (registered data)
    wire         wr_ack = st == M_WLOOK && !wq_full;
    assign ack = done || look_ack || wr_ack;
    assign rdata = rdata_r;

    // the read addresses: the incoming access while idle, else the latched one
    always_comb begin
        if (st == M_IDLE) begin
            d_ra = {pa[14:5], pa[4:3]};
            t_ra = pa[14:5];
        end else begin
            d_ra = {l_idx, l_pa[4:3]};
            t_ra = l_idx;
        end
    end

    function automatic logic [15:0] id_read(input fmode_t m, input logic [7:0] w, input logic master, input logic [7:0] status);
        if (m == F_STATUS) id_read = {8'h00, status};
        else if (w[7:0] == 8'd0) id_read = 16'h00b0;
        else if (w[7:0] == 8'd1) id_read = 16'h00d0;
        else if (w[7:0] == 8'd3 && m == F_READID) id_read = {15'd0, master};
        else id_read = 16'h0000;
    endfunction

    always_ff @(posedge clk) begin
        done <= 1'b0;
        d_we <= 1'b0;
        t_we <= 1'b0;
        wq_push <= 1'b0;
        flash_changed <= 1'b0;
        if (flush) flush_seen <= 1'b1;
        if (rst) begin
            st <= M_FLUSH;
            fl_n <= '0;
            flush_seen <= 1'b0;
            fmode <= F_NORMAL;
            fstatus <= 8'h80;
            fmaster <= 1'b0;
            rd_req <= 1'b0;
            l_erase <= 1'b0;
        end else begin
            case (st)
            M_IDLE:
                if (flush_seen) begin
                    flush_seen <= flush;
                    fmode <= F_NORMAL;       // the flash's RP# follows the CPU reset: read array
                    fl_n <= '0;
                    st <= M_FLUSH;
                end else if (go && !done) begin
                    l_pa <= pa; l_wd <= wdata; l_uds <= uds; l_lds <= lds;
                    l_prog <= 1'b0;
                    if (flash && we)
                        st <= M_LOCK;
                    else if (flash && (fmode == F_STATUS || fmode == F_READID || fmode == F_AMD3)) begin
                        rdata_r <= id_read(fmode, pa[8:1], fmaster, fstatus);
                        done <= 1'b1;
                    end else
                        st <= we ? M_WLOOK : M_LOOK;
                end
            M_LOCK:                                  // flash command (TB flash_write)
                if (!flash_lock) begin
                    st <= M_IDLE;
                    done <= 1'b1;
                    case (fmode)
                    F_NORMAL, F_STATUS, F_READID, F_AMD3:
                        case (fc)
                        8'hf0, 8'hff: fmode <= F_NORMAL;
                        8'h90: fmode <= F_READID;
                        8'h40, 8'h10: fmode <= F_WRITE1;
                        8'h50: begin fstatus <= 8'h80; fmode <= F_STATUS; end
                        8'h20: fmode <= F_CLEAR1;
                        8'h60: fmode <= F_MASTER;
                        8'h70: fmode <= F_STATUS;
                        8'haa: if (fw[11:0] == 12'h555 || fw[11:0] == 12'haaa) fmode <= F_AMD1;
                        default: ;
                        endcase
                    F_AMD1:
                        fmode <= (fc == 8'h55 && (fw[15:0] == 16'h2aa || fw[15:0] == 16'h2aaa || fw[11:0] == 12'h555))
                                 ? F_AMD2 : F_NORMAL;
                    F_AMD2: begin
                        automatic logic cmd = fw[15:0] == 16'h555 || fw[15:0] == 16'h5555 || fw[11:0] == 12'haaa;
                        fmode <= F_NORMAL;
                        if (cmd && fc == 8'h90) fmode <= F_AMD3;
                        else if (cmd && fc == 8'ha0) fmode <= F_BYTEPROG;
                    end
                    F_WRITE1, F_BYTEPROG: begin      // program: read the word, then write old & data
                        done <= 1'b0;
                        l_prog <= 1'b1;
                        st <= M_LOOK;
                    end
                    F_CLEAR1:
                        if (fc == 8'hd0) begin
                            done <= 1'b0;
                            er_n <= '0;
                            er_a <= BASE + 29'({fw[19:15], 15'h0} >> 2);   // 64 KB block: word (w*2 & ~0xFFFF)/8
                            st <= M_ERASE;
                        end
                    F_MASTER: begin
                        if (fc == 8'hf1) fmaster <= 1'b1;
                        else if (fc == 8'hd0) fmaster <= 1'b0;
                        fmode <= F_STATUS;
                    end
                    default: fmode <= F_NORMAL;
                    endcase
                end
            M_LOOK:                                  // tag and data read in the previous clock
                if (hit) begin
                    if (l_prog) begin
                        l_old <= d_lane;
                        st <= M_PROG;
                    end else begin
                        rdata_r <= d_lane;
                        done <= 1'b1;
                        st <= M_IDLE;
                    end
                end else
                    st <= M_MISS;
            M_MISS:
                if (wq_empty && !wq_push && !rd_req) begin
                    rd_req <= 1'b1;
                    rd_addr <= BASE + 29'({l_pa[23:5], 2'b00});
                    beat <= '0;
                    st <= M_FILL;
                end
            M_FILL: begin
                if (rd_req && wq_empty && req_ready) rd_req <= 1'b0;
                if (rsp_valid) begin
                    d_we <= 1'b1;
                    d_wa <= {l_idx, beat};
                    d_wd <= rsp_data;
                    d_be <= 8'hff;
                    if (beat == l_pa[4:3]) l_old <= rsp_data[16*l_lane +: 16];
                    beat <= beat + 2'd1;
                    if (beat == 2'd3) begin
                        t_we <= 1'b1;
                        t_wa <= l_idx;
                        t_wd <= {1'b1, l_tag};
                        if (l_prog)
                            st <= M_PROG;
                        else begin
                            rdata_r <= (beat == l_pa[4:3]) ? rsp_data[16*l_lane +: 16] : l_old;
                            done <= 1'b1;
                            st <= M_IDLE;
                        end
                    end
                end
            end
            M_WLOOK:                                 // DRAM write: through, and into the line on a hit
                if (!wq_full) begin
                    wq_push <= 1'b1;
                    wq_pa <= l_word; wq_pd <= l_wd64; wq_pbe <= l_be;
                    if (hit) begin
                        d_we <= 1'b1; d_wa <= {l_idx, l_pa[4:3]}; d_wd <= l_wd64; d_be <= l_be;
                    end
                    st <= M_IDLE;                    // wr_ack
                end
            M_PROG:                                  // the line is in the cache (hit or just filled)
                if (!wq_full && !wq_push) begin
                    automatic logic [15:0] nw = l_old & l_wd;
                    wq_push <= 1'b1;
                    wq_pa <= l_word; wq_pd <= {4{nw}}; wq_pbe <= 8'b0000_0011 << {l_lane, 1'b0};
                    d_we <= 1'b1; d_wa <= {l_idx, l_pa[4:3]}; d_wd <= {4{nw}}; d_be <= 8'b0000_0011 << {l_lane, 1'b0};
                    fstatus <= 8'h80;
                    fmode <= (fmode == F_BYTEPROG) ? F_NORMAL : F_STATUS;
                    flash_changed <= 1'b1;
                    done <= 1'b1;
                    st <= M_IDLE;
                end
            M_ERASE:
                if (!wq_full && !wq_push) begin
                    wq_push <= 1'b1;
                    wq_pa <= er_a + 29'(er_n[12:0]); wq_pd <= '1; wq_pbe <= 8'hff;
                    er_n <= er_n + 13'd1;
                    if (er_n == 13'h1fff) begin
                        fstatus <= 8'h80;
                        fmode <= F_STATUS;
                        flash_changed <= 1'b1;
                        fl_n <= '0;
                        st <= M_FLUSH;
                        l_erase <= 1'b1;     // done after the flush
                    end
                end
            M_FLUSH: begin
                t_we <= 1'b1;
                t_wa <= fl_n[9:0];
                t_wd <= '0;
                fl_n <= fl_n + 11'd1;
                if (fl_n == 11'd1023) begin
                    flush_seen <= flush;
                    st <= M_IDLE;
                    if (l_erase) begin
                        done <= 1'b1;
                        l_erase <= 1'b0;
                    end
                end
            end
            default: st <= M_IDLE;
            endcase
        end
    end
endmodule
