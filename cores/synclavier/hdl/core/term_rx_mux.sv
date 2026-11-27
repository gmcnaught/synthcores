// The terminal's input bytes (dev_terminal host_rx) from three sources, one
// byte a clock: a = the board simulation's (sim_rx, first), b = the VT640
// read-back reply (tek_plane rb_*, bursts on consecutive clocks), c = the
// PS/2 keyboard (ps2_ascii, up to 3 bytes on consecutive clocks). A byte
// goes out on the clock it comes when nothing before it waits; otherwise it
// waits in its source's queue (16 bytes each) and goes out in order on
// the next free clock. Before, a keyboard byte on the clock of a read-back
// byte was lost (review 2026-11-26 sync-hdl #8). A byte that finds its
// queue full is dropped (`dropped` counts them; not reachable at the rates
// above).
module term_rx_mux (
    input  logic       clk,
    input  logic       rst,
    input  logic       a_valid,
    input  logic [7:0] a_data,
    input  logic       b_valid,
    input  logic [7:0] b_data,
    input  logic       c_valid,
    input  logic [7:0] c_data,
    output logic       out_valid,
    output logic [7:0] out_data,
    output logic [7:0] dropped
);
    logic [7:0] bq [16];
    logic [3:0] b_rp, b_wp;
    logic [4:0] b_n;
    logic [7:0] cq [16];
    logic [3:0] c_rp, c_wp;
    logic [4:0] c_n;

    // what goes out this clock
    wire b_head = b_n != 5'd0;                     // a queued b byte
    wire c_head = c_n != 5'd0;
    wire b_out  = !a_valid && (b_head || b_valid);
    wire c_out  = !a_valid && !b_head && !b_valid && (c_head || c_valid);
    // which arrivals go into their queue
    wire b_push = b_valid && (a_valid || b_head);
    wire c_push = c_valid && (a_valid || b_head || b_valid || c_head);
    wire b_pop  = b_out && b_head;
    wire c_pop  = c_out && c_head;

    assign out_valid = a_valid || b_out || c_out;
    assign out_data  = a_valid ? a_data
                     : b_out ? (b_head ? bq[b_rp] : b_data)
                     : (c_head ? cq[c_rp] : c_data);

    always_ff @(posedge clk)
        if (rst) begin
            b_rp <= '0; b_wp <= '0; b_n <= '0;
            c_rp <= '0; c_wp <= '0; c_n <= '0;
            dropped <= '0;
        end else begin
            if (b_push && (b_n != 5'd16 || b_pop)) begin bq[b_wp] <= b_data; b_wp <= b_wp + 4'd1; end
            if (c_push && (c_n != 5'd16 || c_pop)) begin cq[c_wp] <= c_data; c_wp <= c_wp + 4'd1; end
            if (b_pop) b_rp <= b_rp + 4'd1;
            if (c_pop) c_rp <= c_rp + 4'd1;
            b_n <= b_n + 5'(b_push && (b_n != 5'd16 || b_pop)) - 5'(b_pop);
            c_n <= c_n + 5'(c_push && (c_n != 5'd16 || c_pop)) - 5'(c_pop);
            if ((b_push && b_n == 5'd16 && !b_pop) || (c_push && c_n == 5'd16 && !c_pop))
                dropped <= dropped + 8'd1;
        end
endmodule
