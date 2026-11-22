// Wave ROM in MiSTer's DDR3, through the synthframe memory port (synthframe_mem_arb onto
// synthframe_mem_ddr3: 64-bit words, reads returned in order after any delay).
//
// Read stream for the PCM chip (sc55_pcm rom_*): byte requests taken with
// req && ready, bytes returned in request order on rvalid. A request in the
// same 8-byte word as the request just before it shares that word's read
// (the four sample bytes of a voice are usually consecutive), so a voice
// costs about two reads; reads are pipelined (requests do not wait for data).
// Write port for loading: one byte per write, held until the memory port takes it
// (wbusy). Line port: one 64-bit word at BASE + 0x400000 + 8 * laddr (ROM2's upper
// 256 KB on the mk2, for rom2x_cache). Memory-port clients, in priority order: write,
// line, stream.
//
// The wave ROMs live at BASE (bytes; DDR3 from 0x30000000 is free for cores).
module wave_ddr #(
    parameter logic [31:0] BASE = 32'h3000_0000,
    parameter int QBITS = 4
) (
    input  logic        clk,
    input  logic        rst,

    input  logic        wr,
    input  logic [22:0] waddr,
    input  logic [7:0]  wdata,
    output logic        wbusy,

    input  logic        req,
    input  logic [21:0] addr,
    output logic        ready,
    output logic        rvalid,
    output logic [7:0]  rdata,

    input  logic        lreq,             // held until lvalid
    input  logic [14:0] laddr,
    output logic        lvalid,
    output logic [63:0] ldata,

    input  logic        DDRAM_BUSY,
    output logic [7:0]  DDRAM_BURSTCNT,
    output logic [28:0] DDRAM_ADDR,
    input  logic [63:0] DDRAM_DOUT,
    input  logic        DDRAM_DOUT_READY,
    output logic        DDRAM_RD,
    output logic [63:0] DDRAM_DIN,
    output logic [7:0]  DDRAM_BE,
    output logic        DDRAM_WE
);
    localparam int Q = 1 << QBITS;
    localparam logic [28:0] BASE_W = BASE[31:3];
    localparam int CW = 0, CL = 1, CS = 2;          // clients: write, line, stream

    // ------------------------------------------------------------ memory port
    logic        c_req_valid [3], c_req_ready [3], c_req_we [3], c_rsp_valid [3];
    logic [28:0] c_req_addr [3];
    logic [63:0] c_req_wdata [3], c_rsp_data;
    logic [7:0]  c_req_be [3], c_req_len [3];
    logic [0:0]  c_req_tag [3], c_rsp_tag;
    logic        m_req_valid, m_req_ready, m_req_we, m_rsp_valid;
    logic [28:0] m_req_addr;
    logic [63:0] m_req_wdata, m_rsp_data;
    logic [7:0]  m_req_be, m_req_len;
    logic [2:0]  m_req_tag, m_rsp_tag;

    synthframe_mem_arb #(.N(3), .CTW(1)) arb (
        .c_req_valid, .c_req_ready, .c_req_we, .c_req_addr, .c_req_wdata, .c_req_be, .c_req_len, .c_req_tag,
        .c_rsp_valid, .c_rsp_data, .c_rsp_tag,
        .req_valid(m_req_valid), .req_ready(m_req_ready), .req_we(m_req_we), .req_addr(m_req_addr),
        .req_wdata(m_req_wdata), .req_be(m_req_be), .req_len(m_req_len), .req_tag(m_req_tag),
        .rsp_valid(m_rsp_valid), .rsp_data(m_rsp_data), .rsp_tag(m_rsp_tag));

    synthframe_mem_ddr3 #(.TW(3), .QBITS(QBITS)) mem (
        .clk, .rst,
        .req_valid(m_req_valid), .req_ready(m_req_ready), .req_we(m_req_we), .req_addr(m_req_addr),
        .req_wdata(m_req_wdata), .req_be(m_req_be), .req_len(m_req_len), .req_tag(m_req_tag),
        .rsp_valid(m_rsp_valid), .rsp_data(m_rsp_data), .rsp_tag(m_rsp_tag),
        .DDRAM_BUSY, .DDRAM_BURSTCNT, .DDRAM_ADDR, .DDRAM_DOUT, .DDRAM_DOUT_READY,
        .DDRAM_RD, .DDRAM_DIN, .DDRAM_BE, .DDRAM_WE);

    // ------------------------------------------------------------ write client
    logic [28:0] w_addr;
    logic [7:0]  w_be;
    logic [63:0] w_din;
    assign c_req_valid[CW] = wbusy;
    assign c_req_we[CW] = 1'b1;
    assign c_req_addr[CW] = w_addr;
    assign c_req_wdata[CW] = w_din;
    assign c_req_be[CW] = w_be;
    assign c_req_len[CW] = 8'd1;
    assign c_req_tag[CW] = 1'b0;

    // ------------------------------------------------------------- line client
    logic        l_busy;                // the line read is issued or waiting
    assign c_req_valid[CL] = lreq && !l_busy && !lvalid;
    assign c_req_we[CL] = 1'b0;
    assign c_req_addr[CL] = BASE_W + 29'h80000 + {14'h0, laddr};
    assign c_req_wdata[CL] = '0;
    assign c_req_be[CL] = '0;
    assign c_req_len[CL] = 8'd1;
    assign c_req_tag[CL] = 1'b0;

    // ----------------------------------------------------------- stream client
    // request entries {new word read, byte in word}, in order
    logic [3:0]        rq [Q];
    logic [QBITS:0]    rq_wp, rq_rp;
    // word reads (word offsets) still to issue
    logic [18:0]       cq [Q];
    logic [QBITS:0]    cq_wp, cq_rp;
    // returned words
    logic [63:0]       dq [Q];
    logic [QBITS:0]    dq_wp, dq_rp;
    logic [QBITS:0]    inflight;        // stream reads issued, data not yet back
    logic [18:0]       last_word;
    logic              have_last;
    logic [63:0]       cur;

    wire [18:0] word = addr[21:3];
    wire        same = have_last && word == last_word;
    wire        rq_full = (rq_wp - rq_rp) == (QBITS+1)'(Q);
    wire        cq_full = (cq_wp - cq_rp) == (QBITS+1)'(Q);
    // a new word needs room for its data too (reads issued + queued + words held < Q)
    wire        d_room = ((cq_wp - cq_rp) + inflight + (dq_wp - dq_rp)) < (QBITS+1)'(Q - 1);
    assign ready = !rq_full && !cq_full && d_room && !wbusy;

    assign c_req_valid[CS] = cq_wp != cq_rp;
    assign c_req_we[CS] = 1'b0;
    assign c_req_addr[CS] = BASE_W + {10'h0, cq[cq_rp[QBITS-1:0]]};
    assign c_req_wdata[CS] = '0;
    assign c_req_be[CS] = '0;
    assign c_req_len[CS] = 8'd1;
    assign c_req_tag[CS] = 1'b0;

    always_ff @(posedge clk) begin
        rvalid <= 1'b0;
        lvalid <= 1'b0;
        if (rst) begin
            wbusy <= 1'b0;
            l_busy <= 1'b0;
            rq_wp <= '0; rq_rp <= '0; cq_wp <= '0; cq_rp <= '0; dq_wp <= '0; dq_rp <= '0;
            inflight <= '0;
            have_last <= 1'b0;
        end else begin : b_wave_ddr_1
            logic issued, returned;
            issued = c_req_valid[CS] && c_req_ready[CS];
            returned = c_rsp_valid[CS];

            // write: held until the memory port takes it
            if (wbusy) begin
                if (c_req_ready[CW]) wbusy <= 1'b0;
            end else if (wr) begin
                w_addr <= BASE_W + {9'h0, waddr[22:3]};
                w_din <= {8{wdata}};
                w_be <= 8'd1 << waddr[2:0];
                wbusy <= 1'b1;
                have_last <= 1'b0;        // contents change: no merging across a load
            end

            // line read
            if (c_req_valid[CL] && c_req_ready[CL]) l_busy <= 1'b1;
            if (c_rsp_valid[CL]) begin
                ldata <= c_rsp_data;
                lvalid <= 1'b1;
                l_busy <= 1'b0;
            end

            // accept a stream request
            if (req && ready) begin
                rq[rq_wp[QBITS-1:0]] <= {!same, addr[2:0]};
                rq_wp <= rq_wp + 1'b1;
                if (!same) begin
                    cq[cq_wp[QBITS-1:0]] <= word;
                    cq_wp <= cq_wp + 1'b1;
                end
                last_word <= word;
                have_last <= 1'b1;
            end
            if (issued) cq_rp <= cq_rp + 1'b1;
            if (returned) begin
                dq[dq_wp[QBITS-1:0]] <= c_rsp_data;
                dq_wp <= dq_wp + 1'b1;
            end
            inflight <= inflight + (QBITS+1)'(issued) - (QBITS+1)'(returned);

            // bytes out, in request order
            if (rq_wp != rq_rp) begin : b_wave_ddr_2
                logic [3:0] e;
                logic [63:0] w;
                e = rq[rq_rp[QBITS-1:0]];
                if (!e[3] || dq_wp != dq_rp) begin
                    w = cur;
                    if (e[3]) begin
                        w = dq[dq_rp[QBITS-1:0]];
                        dq_rp <= dq_rp + 1'b1;
                        cur <= w;
                    end
                    rdata <= w[8 * e[2:0] +: 8];   // little-endian: byte 0 in bits 7:0
                    rvalid <= 1'b1;
                    rq_rp <= rq_rp + 1'b1;
                end
            end
        end
    end
endmodule
