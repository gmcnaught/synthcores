// SPDX-License-Identifier: MIT
// synthframe: video timing counters. Defaults: 800 x 600 at 60 Hz from a 40 MHz pixel clock
// (VESA, 1056 x 628 total), as the SC-55 and JV-880 renderers had their own copies. x and y are
// registers; the blanking and sync flags are combinational from them (active high).
module synthframe_vtiming #(
    parameter int H_TOTAL = 1056, H_ACT = 800, HS_START = 840, HS_END = 968,
    parameter int V_TOTAL = 628,  V_ACT = 600, VS_START = 601, VS_END = 605
) (
    input  logic        clk,
    output logic [10:0] x,
    output logic [9:0]  y,
    output logic        hblank,
    output logic        vblank,
    output logic        hsync,
    output logic        vsync
);
    always_ff @(posedge clk) begin
        if (x == 11'(H_TOTAL - 1)) begin
            x <= 11'd0;
            y <= (y == 10'(V_TOTAL - 1)) ? 10'd0 : y + 10'd1;
        end else x <= x + 11'd1;
    end
    assign hblank = x >= 11'(H_ACT);
    assign vblank = y >= 10'(V_ACT);
    assign hsync  = x >= 11'(HS_START) && x < 11'(HS_END);
    assign vsync  = y >= 10'(VS_START) && y < 10'(VS_END);
endmodule
