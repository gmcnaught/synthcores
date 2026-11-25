// SPDX-License-Identifier: MIT
// synthframe: MIDI bytes out to MiSTer's UART (MidiLink), 8N1.
//
// The cores' CPUs keep their reference models' transmit timing, which can be faster than the wire
// (the SC-55's SCI finishes a byte in 3000 CPU cycles, 125 us; a MIDI byte takes 320 us), so the
// bytes wait in a FIFO. A byte that arrives with the FIFO full is dropped and counted in `dropped`.
//
// `alt` sends at ALT_BAUD: synthframe_midi_rx's `alt` (the HPS UART fell back to 9600, so it
// expects 9600 both ways). It is sampled at the start of each byte.
module synthframe_midi_tx #(
    parameter longint CLK_HZ = 64'd50_000_000,
    parameter longint BAUD = 64'd31_250,
    parameter longint ALT_BAUD = 64'd9_600,
    parameter int DEPTH_BITS = 9                  // FIFO: 2^DEPTH_BITS bytes (512: one M10K)
) (
    input  logic        clk,
    input  logic        rst,
    input  logic        alt,
    input  logic        valid,
    input  logic [7:0]  data,
    output logic        txd,
    output logic        busy,                     // bytes queued or on the wire
    output logic [15:0] dropped                   // bytes lost to a full FIFO (saturates)
);
    localparam int DIV = int'(CLK_HZ / BAUD);
    localparam int ALT_DIV = int'(CLK_HZ / ALT_BAUD);
    localparam int CW = $clog2(ALT_DIV > DIV ? ALT_DIV + 1 : DIV + 1);

    logic [7:0]          mem [2**DEPTH_BITS];
    logic [DEPTH_BITS:0] wp, rp;
    logic [7:0]          q;
    wire                 empty = wp == rp;
    wire                 full = (wp[DEPTH_BITS] != rp[DEPTH_BITS]) && (wp[DEPTH_BITS-1:0] == rp[DEPTH_BITS-1:0]);

    always_ff @(posedge clk) begin
        if (valid && !full) mem[wp[DEPTH_BITS-1:0]] <= data;
        q <= mem[rp[DEPTH_BITS-1:0]];             // registered read (block RAM)
    end

    // 0 idle, 1 wait for q, 2 shifting
    logic [1:0]    st;
    logic [9:0]    sh;                            // stop, data[7:0], start; sent LSB first
    logic [3:0]    bitn;
    logic [CW-1:0] cnt;
    logic          slow;                          // this byte at ALT_BAUD
    always_ff @(posedge clk) begin
        if (rst) begin
            wp <= '0;
            rp <= '0;
            st <= 2'd0;
            txd <= 1'b1;
            dropped <= '0;
        end else begin
            if (valid) begin
                if (!full) wp <= wp + 1'b1;
                else if (dropped != 16'hffff) dropped <= dropped + 16'd1;
            end
            case (st)
                2'd0: if (!empty) st <= 2'd1;
                2'd1: begin
                    sh <= {1'b1, q, 1'b0};
                    rp <= rp + 1'b1;
                    bitn <= 4'd0;
                    slow <= alt;
                    cnt <= CW'(alt ? ALT_DIV - 1 : DIV - 1);
                    txd <= 1'b0;
                    st <= 2'd2;
                end
                default: begin
                    if (cnt != '0) cnt <= cnt - 1'b1;
                    else if (bitn == 4'd9) begin
                        st <= 2'd0;                // stop bit done
                    end else begin
                        cnt <= CW'(slow ? ALT_DIV - 1 : DIV - 1);
                        bitn <= bitn + 4'd1;
                        txd <= sh[bitn + 4'd1];
                    end
                end
            endcase
        end
    end
    assign busy = !empty || st != 2'd0;
endmodule
