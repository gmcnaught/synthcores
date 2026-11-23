// SPDX-License-Identifier: MIT
// synthframe: the status strip the MiSTer smoke tests read (synth-mister shot), 20 lines high
// at (X0, Y0): a state band of 200 pixels (red: no ROM pack, amber: loading, blue: ROM pack but
// not running, green: running), then 40-pixel boxes 20 pixels apart for MIDI (white while bytes
// arrive), underrun (bright red now, dark red seen) and lag (amber seen). Combinational: the
// renderer passes the coordinates of the pixel it is colouring.
module synthframe_status_strip #(
    parameter int X0 = 0,
    parameter int Y0 = 0
) (
    input  logic [10:0] x,
    input  logic [9:0]  y,
    input  logic        rom_ok,
    input  logic        loading,
    input  logic        running,
    input  logic        midi_led,
    input  logic        underrun_led,
    input  logic        underrun_seen,
    input  logic        lag_seen,
    output logic        hit,
    output logic [23:0] rgb
);
    localparam logic [23:0] DARK = 24'h303848;
    // offsets from the strip's corner; a pixel left of or above it wraps to a large offset
    logic [10:0] dx;
    logic [9:0]  dy;
    assign dx = x - 11'(X0);
    assign dy = y - 10'(Y0);
    always_comb begin
        hit = 1'b0;
        rgb = DARK;
        if (dy < 10'd20) begin
            if (dx < 11'd200) begin
                hit = 1'b1;
                rgb = running ? 24'h20c040 : loading ? 24'he0a020 : rom_ok ? 24'h2060c0 : 24'hc02020;
            end
            if (11'(dx - 11'd220) < 11'd40) begin
                hit = 1'b1;
                rgb = midi_led ? 24'hffffff : DARK;
            end
            if (11'(dx - 11'd280) < 11'd40) begin
                hit = 1'b1;
                rgb = underrun_led ? 24'hff4040 : underrun_seen ? 24'h902020 : DARK;
            end
            if (11'(dx - 11'd340) < 11'd40) begin
                hit = 1'b1;
                rgb = lag_seen ? 24'he0a020 : DARK;
            end
        end
    end
endmodule
