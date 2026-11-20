// 640 x 480 at 60 Hz (25 MHz pixel clock, one pixel per clock) with a plain
// status picture until the LCD is drawn (M6): a band at the top shows the
// state (red: no ROM pack, amber: loading, green: running), a box lights on
// MIDI input, another on audio underruns. Inputs come from the core clock and
// are only shown.
module status_video (
    input  logic       clk,          // pixel clock
    input  logic       rom_ok,
    input  logic       loading,
    input  logic       running,
    input  logic       midi_led,
    input  logic       underrun_led,
    output logic       hs,
    output logic       vs,
    output logic       hblank,
    output logic       vblank,
    output logic [7:0] r,
    output logic [7:0] g,
    output logic [7:0] b
);
    logic [9:0] x, y;
    logic [4:0] s1, s2;
    always_ff @(posedge clk) begin
        s1 <= {underrun_led, midi_led, running, loading, rom_ok};
        s2 <= s1;
        if (x == 10'd799) begin
            x <= 10'd0;
            y <= (y == 10'd524) ? 10'd0 : y + 10'd1;
        end else x <= x + 10'd1;
        hblank <= (x >= 10'd640);
        vblank <= (y >= 10'd480);
        hs <= !(x >= 10'd656 && x < 10'd752);
        vs <= !(y >= 10'd490 && y < 10'd492);
        {r, g, b} <= 24'h101828;
        if (y >= 10'd40 && y < 10'd80 && x >= 10'd40 && x < 10'd600)
            {r, g, b} <= s2[2] ? 24'h20c040 : s2[1] ? 24'he0a020 : s2[0] ? 24'h2060c0 : 24'hc02020;
        if (y >= 10'd120 && y < 10'd180 && x >= 10'd40 && x < 10'd120)
            {r, g, b} <= s2[3] ? 24'hffffff : 24'h303848;
        if (y >= 10'd120 && y < 10'd180 && x >= 10'd160 && x < 10'd240)
            {r, g, b} <= s2[4] ? 24'hff4040 : 24'h303848;
    end
endmodule
