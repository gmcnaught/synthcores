// SPDX-License-Identifier: MIT
// synthframe memory port, DDR3 backend: MiSTer's DDRAM_* (Avalon-MM, 64-bit words, burst 1).
//
// The memory-port contract (every synthframe backend: DDR3 here, PSRAM later):
//   request:  req_valid / req_ready handshake (taken when both are high) with
//             req_we, req_addr (64-bit word address), req_wdata, req_be and req_tag;
//   response: one rsp_valid pulse with rsp_data and rsp_tag per word read, in request
//             order, after any number of clocks; no backpressure (the requester must take it).
//   Writes have no response; a read after a write to the same word sees the write.
//   req_len (0 and 1 both mean one word): a read returns req_len consecutive words from
//   req_addr, one response each with the request's tag; a write is a fill, the same
//   req_wdata / req_be written to req_len consecutive words.
//   At most 2**QBITS - 1 reads are outstanding; req_ready is low while that many are (for
//   writes too, so req_ready never depends on the request).
//
// One command is held on DDRAM_RD / DDRAM_WE until the controller is not busy (Avalon; a
// read of req_len words is one burst command, a fill one burst of req_len beats, one beat per
// clock the controller is not busy);
// a new request is taken only while none is held, so at most one command every two clocks
// (as the cores' own DDR3 ports did). Read data comes back in order on DDRAM_DOUT_READY; the
// tag of each issued read waits in a FIFO.
//
// rst may be any length: a command held on DDRAM_RD / DDRAM_WE stays until the controller takes
// it (Avalon), reads already issued are counted back in with no rsp_valid, and req_ready stays
// low until the port is idle. So no response for a request taken before rst comes after it.
// The command and queue state start idle at power-on (declaration initial values).
module synthframe_mem_ddr3 #(
    parameter int TW = 4,                 // tag width
    parameter int QBITS = 4               // outstanding reads < 2**QBITS
) (
    input  logic          clk,
    input  logic          rst,

    input  logic          req_valid,
    output logic          req_ready,
    input  logic          req_we,
    input  logic [28:0]   req_addr,
    input  logic [63:0]   req_wdata,
    input  logic [7:0]    req_be,
    input  logic [7:0]    req_len,
    input  logic [TW-1:0] req_tag,

    output logic          rsp_valid,
    output logic [63:0]   rsp_data,
    output logic [TW-1:0] rsp_tag,

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
    localparam int Q = 1 << QBITS;

    logic [TW-1:0]  tq [Q];               // tags of reads taken, in order
    logic [7:0]     lq [Q];               // their lengths (words) less one
    // power-up values instead of a reset: rst must not drop a held command or forget reads in
    // flight (Quartus takes declaration values as the power-up state)
    /* verilator lint_off PROCASSINIT */
    logic [QBITS:0] tq_wp = '0, tq_rp = '0;   // tq_wp - tq_rp = reads taken and not returned
    logic [7:0]     rbeat = 8'd0;         // words of the oldest read returned so far
    logic [7:0]     beats = 8'd0;         // write beats still to be taken, after this one
    logic           drain = 1'b0;         // since rst: reads before it still coming back
    logic           rd = 1'b0, we = 1'b0; // the command held on DDRAM_RD / DDRAM_WE
    /* verilator lint_on PROCASSINIT */
    assign DDRAM_RD = rd;
    assign DDRAM_WE = we;
    wire            held = rd || we;
    wire            room = (tq_wp - tq_rp) < (QBITS+1)'(Q - 1);
    wire            idle = !held && tq_wp == tq_rp;

    assign req_ready = !rst && !drain && !held && room;

    always_ff @(posedge clk) begin
        rsp_valid <= 1'b0;
        if (rst) drain <= 1'b1;
        else if (idle) drain <= 1'b0;
        begin   // no reset: a held command completes and issued reads are counted back
            if (held) begin
                if (!DDRAM_BUSY) begin
                    if (beats != 8'd0) beats <= beats - 8'd1;
                    else begin
                        rd <= 1'b0;
                        we <= 1'b0;
                    end
                end
            end else if (req_valid && req_ready) begin
                DDRAM_ADDR <= req_addr;
                DDRAM_DIN <= req_wdata;
                DDRAM_BE <= req_be;
                DDRAM_BURSTCNT <= 8'd1;
                beats <= 8'd0;
                if (req_we) begin
                    we <= 1'b1;
                    if (req_len > 8'd1) begin
                        DDRAM_BURSTCNT <= req_len;
                        beats <= req_len - 8'd1;
                    end
                end else begin
                    rd <= 1'b1;
                    if (req_len > 8'd1) DDRAM_BURSTCNT <= req_len;
                    tq[tq_wp[QBITS-1:0]] <= req_tag;
                    lq[tq_wp[QBITS-1:0]] <= req_len > 8'd1 ? req_len - 8'd1 : 8'd0;
                    tq_wp <= tq_wp + 1'b1;
                end
            end
            if (DDRAM_DOUT_READY) begin
                rsp_valid <= !rst && !drain;
                rsp_data <= DDRAM_DOUT;
                rsp_tag <= tq[tq_rp[QBITS-1:0]];
                if (rbeat == lq[tq_rp[QBITS-1:0]]) begin
                    rbeat <= 8'd0;
                    tq_rp <= tq_rp + 1'b1;
                end else rbeat <= rbeat + 8'd1;
            end
        end
    end
endmodule
