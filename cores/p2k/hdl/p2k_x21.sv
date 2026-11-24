// SPDX-License-Identifier: GPL-3.0-or-later
// Request/response crossing between two related clocks from one PLL: clk_a (the CPU, 44.1 MHz)
// and clk_b at exactly twice its rate (88.2 MHz), rising edges aligned. Every path between them
// is a normal synchronous path the timing analyser checks; no synchronisers are needed.
//
// Requests A -> B: a_valid/a_ready (A may issue when the previous request has been taken by B).
// B holds one request at a time on b_valid until b_ready.
// Responses B -> A: b_rsp_valid pulses (one per B clock, back to back allowed) fill a FIFO that A
// reads with a_rsp_pop. DEPTH must exceed the responses that can be outstanding (no full check).
module p2k_x21 #(
    parameter int RW = 8,
    parameter int SW = 8,
    parameter int DEPTH = 8
) (
    input  logic          clk_a,
    input  logic          rst_a,
    input  logic          clk_b,
    input  logic          rst_b,

    input  logic          a_valid,
    output logic          a_ready,
    input  logic [RW-1:0] a_data,

    output logic          b_valid,
    input  logic          b_ready,
    output logic [RW-1:0] b_data,

    input  logic          b_rsp_valid,
    input  logic [SW-1:0] b_rsp_data,

    output logic          a_rsp_valid,
    output logic [SW-1:0] a_rsp_data,
    input  logic          a_rsp_pop
);
    localparam int PW = $clog2(DEPTH);

    // ------------------------------------------------------------------ requests
    logic          req_tog, got_tog;
    logic [RW-1:0] a_hold;
    assign a_ready = req_tog == got_tog;
    always_ff @(posedge clk_a) begin
        if (rst_a) req_tog <= 1'b0;
        else if (a_valid && a_ready) begin
            a_hold <= a_data;
            req_tog <= !req_tog;
        end
    end
    always_ff @(posedge clk_b) begin
        if (rst_b) begin
            got_tog <= 1'b0;
            b_valid <= 1'b0;
        end else begin
            if (b_valid && b_ready) b_valid <= 1'b0;
            if (req_tog != got_tog && (!b_valid || b_ready)) begin
                b_data <= a_hold;
                b_valid <= 1'b1;
                got_tog <= req_tog;
            end
        end
    end

    // ------------------------------------------------------------------ responses
    // registers, not an inferred RAM: it is written in clk_b and read combinationally in clk_a,
    // which an MLAB's write timing does not support (a hardware run read a corrupted bit)
    (* ramstyle = "logic" *) logic [SW-1:0] fifo [0:DEPTH-1];
    logic [PW:0]   wp, rp;
    always_ff @(posedge clk_b) begin
        if (rst_b) wp <= '0;
        else if (b_rsp_valid) begin
            fifo[wp[PW-1:0]] <= b_rsp_data;
            wp <= wp + 1'b1;
        end
    end
    always_ff @(posedge clk_a) begin
        if (rst_a) rp <= '0;
        else if (a_rsp_pop && a_rsp_valid) rp <= rp + 1'b1;
    end
    assign a_rsp_valid = wp != rp;
    assign a_rsp_data = fifo[rp[PW-1:0]];
endmodule
