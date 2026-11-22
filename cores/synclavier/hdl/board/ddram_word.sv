// 16-bit word port (synclavier_core xr_*) on MiSTer's DDR3, through the synthframe memory
// port (synthframe_mem_arb onto synthframe_mem_ddr3: 64-bit words, reads returned in order
// after any delay). Word w lives at byte BASE + 2w; DDR3 from 0x30000000 is free for cores.
// `wide` with wr writes wdata to the 512 words from w (a multiple of 512): one fill of 128
// 64-bit words (poly memory clear).
//
// Handshake as the simulation RAM models: hold rd/wr until ack, a one-clock pulse with rdata
// valid; the clock after an ack is ignored. A write is acknowledged when the memory port
// takes it (the port keeps the order, so a later read sees it).
//
// Read stream (s_*, the poly voices' fetches, M15/M26): s_rd held with s_addr until s_ack
// (the memory port took the read; the clock after s_ack is ignored); each read is the 64-bit
// word holding word s_addr (words s_addr & ~3 .. | 3, word i in bits 16i+15..16i); reads do
// not wait for each other's data, which comes back in order as s_rvalid pulses. Memory-port
// clients, in priority order: the word port, the stream.
module ddram_word #(
    parameter logic [31:0] BASE = 32'h3000_0000
) (
    input  logic        clk,
    input  logic        rst,

    input  logic [26:0] addr,          // words: 256 MB from BASE
    input  logic        rd,
    input  logic        wr,
    input  logic        wide,
    input  logic [15:0] wdata,
    output logic [15:0] rdata,
    output logic        ack,

    input  logic [26:0] s_addr,
    input  logic        s_rd,
    output logic        s_ack,
    output logic        s_rvalid,
    output logic [63:0] s_rdata,

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
    localparam logic [28:0] BASE_Q = 29'(BASE >> 3);
    localparam int CP = 0, CS = 1;                  // clients: word port, stream

    // ------------------------------------------------------------ memory port
    logic        c_req_valid [2], c_req_ready [2], c_req_we [2], c_rsp_valid [2];
    logic [28:0] c_req_addr [2];
    logic [63:0] c_req_wdata [2], c_rsp_data;
    logic [7:0]  c_req_be [2], c_req_len [2];
    logic [0:0]  c_req_tag [2], c_rsp_tag;
    logic        m_req_valid, m_req_ready, m_req_we, m_rsp_valid;
    logic [28:0] m_req_addr;
    logic [63:0] m_req_wdata, m_rsp_data;
    logic [7:0]  m_req_be, m_req_len;
    logic [1:0]  m_req_tag, m_rsp_tag;

    synthframe_mem_arb #(.N(2), .CTW(1)) arb (
        .c_req_valid, .c_req_ready, .c_req_we, .c_req_addr, .c_req_wdata, .c_req_be, .c_req_len, .c_req_tag,
        .c_rsp_valid, .c_rsp_data, .c_rsp_tag,
        .req_valid(m_req_valid), .req_ready(m_req_ready), .req_we(m_req_we), .req_addr(m_req_addr),
        .req_wdata(m_req_wdata), .req_be(m_req_be), .req_len(m_req_len), .req_tag(m_req_tag),
        .rsp_valid(m_rsp_valid), .rsp_data(m_rsp_data), .rsp_tag(m_rsp_tag));

    // up to 31 reads in flight (the stream kept 30 before)
    synthframe_mem_ddr3 #(.TW(2), .QBITS(5)) mem (
        .clk, .rst,
        .req_valid(m_req_valid), .req_ready(m_req_ready), .req_we(m_req_we), .req_addr(m_req_addr),
        .req_wdata(m_req_wdata), .req_be(m_req_be), .req_len(m_req_len), .req_tag(m_req_tag),
        .rsp_valid(m_rsp_valid), .rsp_data(m_rsp_data), .rsp_tag(m_rsp_tag),
        .DDRAM_BUSY, .DDRAM_BURSTCNT, .DDRAM_ADDR, .DDRAM_DOUT, .DDRAM_DOUT_READY,
        .DDRAM_RD, .DDRAM_DIN, .DDRAM_BE, .DDRAM_WE);

    // --------------------------------------------------------- word port client
    logic       p_wait;                 // a read is issued, waiting for its word
    logic [1:0] lane;
    assign c_req_valid[CP] = (rd || wr) && !ack && !p_wait;
    assign c_req_we[CP] = wr;
    assign c_req_addr[CP] = BASE_Q + 29'(addr[26:2]);
    assign c_req_wdata[CP] = {4{wdata}};
    assign c_req_be[CP] = wr && wide ? 8'hFF : 8'b0000_0011 << {addr[1:0], 1'b0};
    assign c_req_len[CP] = wr && wide ? 8'd128 : 8'd1;
    assign c_req_tag[CP] = 1'b0;

    // ------------------------------------------------------------ stream client
    assign c_req_valid[CS] = s_rd && !s_ack;
    assign c_req_we[CS] = 1'b0;
    assign c_req_addr[CS] = BASE_Q + 29'(s_addr[26:2]);
    assign c_req_wdata[CS] = '0;
    assign c_req_be[CS] = '0;
    assign c_req_len[CS] = 8'd1;
    assign c_req_tag[CS] = 1'b0;

    always_ff @(posedge clk) begin
        ack <= 1'b0;
        s_ack <= 1'b0;
        s_rvalid <= 1'b0;
        if (rst) begin
            p_wait <= 1'b0;
        end else begin
            if (c_req_valid[CP] && c_req_ready[CP]) begin
                if (wr) ack <= 1'b1;
                else begin
                    p_wait <= 1'b1;
                    lane <= addr[1:0];
                end
            end
            if (c_rsp_valid[CP]) begin
                rdata <= c_rsp_data[{lane, 4'd0} +: 16];
                ack <= 1'b1;
                p_wait <= 1'b0;
            end
            if (c_req_valid[CS] && c_req_ready[CS]) s_ack <= 1'b1;
            if (c_rsp_valid[CS]) begin
                s_rdata <= c_rsp_data;
                s_rvalid <= 1'b1;
            end
        end
    end
endmodule
