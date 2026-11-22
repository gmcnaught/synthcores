// SPDX-License-Identifier: MIT
// synthframe memory port, DDR3 backend: MiSTer's DDRAM_* (Avalon-MM, 64-bit words, burst 1).
//
// The memory-port contract (every synthframe backend: DDR3 here, PSRAM later):
//   request:  req_valid / req_ready handshake (taken when both are high) with
//             req_we, req_addr (64-bit word address), req_wdata, req_be and req_tag;
//   response: one rsp_valid pulse with rsp_data and rsp_tag per read, in request order,
//             after any number of clocks; no backpressure (the requester must take it).
//   Writes have no response; a read after a write to the same word sees the write.
//   At most 2**QBITS - 1 reads are outstanding; req_ready is low while that many are (for
//   writes too, so req_ready never depends on the request).
//
// One command is held on DDRAM_RD / DDRAM_WE until the controller is not busy (Avalon);
// a new request is taken only while none is held, so at most one command every two clocks
// (as the cores' own DDR3 ports did). Read data comes back in order on DDRAM_DOUT_READY; the
// tag of each issued read waits in a FIFO.
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
    logic [QBITS:0] tq_wp, tq_rp;         // tq_wp - tq_rp = reads taken and not returned
    wire            held = DDRAM_RD || DDRAM_WE;
    wire            room = (tq_wp - tq_rp) < (QBITS+1)'(Q - 1);

    assign DDRAM_BURSTCNT = 8'd1;
    assign req_ready = !rst && !held && room;

    always_ff @(posedge clk) begin
        rsp_valid <= 1'b0;
        if (rst) begin
            DDRAM_RD <= 1'b0;
            DDRAM_WE <= 1'b0;
            tq_wp <= '0;
            tq_rp <= '0;
        end else begin
            if (held) begin
                if (!DDRAM_BUSY) begin
                    DDRAM_RD <= 1'b0;
                    DDRAM_WE <= 1'b0;
                end
            end else if (req_valid && req_ready) begin
                DDRAM_ADDR <= req_addr;
                DDRAM_DIN <= req_wdata;
                DDRAM_BE <= req_be;
                if (req_we) begin
                    DDRAM_WE <= 1'b1;
                end else begin
                    DDRAM_RD <= 1'b1;
                    tq[tq_wp[QBITS-1:0]] <= req_tag;
                    tq_wp <= tq_wp + 1'b1;
                end
            end
            if (DDRAM_DOUT_READY) begin
                rsp_valid <= 1'b1;
                rsp_data <= DDRAM_DOUT;
                rsp_tag <= tq[tq_rp[QBITS-1:0]];
                tq_rp <= tq_rp + 1'b1;
            end
        end
    end
endmodule
