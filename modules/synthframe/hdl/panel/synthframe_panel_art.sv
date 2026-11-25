// SPDX-License-Identifier: MIT
// synthframe: front-panel art from block RAM ROMs that modules/synthframe/tools/panel_art.py
// makes from a core's cfg/panel.yaml. The screen is cut into 8 x 8 cells; the map gives each
// cell a tile and an element (0: none), the tile gives each pixel a palette index of BPP bits.
// An element is an LED or a button: while state[element] is 1, its cell's pixels take their
// colours from LIT instead of PAL (they differ only in the entries kept for unlit LED and button
// faces), so the LED lights or the button shows pressed.
//
// Latency: rgb is the colour of the pixel at (x, y) three clocks earlier. Outside W x H (the
// blanking) rgb is undefined.
module synthframe_panel_art #(
    parameter int W = 800,
    parameter int H = 600,
    parameter int BPP = 4,
    parameter int TB = 10,          // tile number bits
    parameter int EB = 6,           // element number bits
    parameter int NT = 1024,        // tiles
    parameter logic [24*(2**BPP)-1:0] PAL = '0,
    parameter logic [24*(2**BPP)-1:0] LIT = '0,
    parameter MAP_HEX = "",
    parameter TILE_HEX = ""
) (
    input  logic              clk,
    input  logic [10:0]       x,
    input  logic [9:0]        y,
    input  logic [2**EB-1:0]  state,
    output logic [23:0]       rgb
);
    localparam int COLS = W / 8, ROWS = H / 8, CELLS = COLS * ROWS;
    localparam int MAW = $clog2(CELLS);

    logic [TB+EB-1:0] map_rom [CELLS];
    logic [8*BPP-1:0] tile_rom [NT*8];
    initial begin
        $readmemh(MAP_HEX, map_rom);
        $readmemh(TILE_HEX, tile_rom);
    end

    // clock 1: the cell's map entry
    logic [MAW-1:0]   maddr;
    logic [TB+EB-1:0] map_q;
    logic [2:0]       xl1, yl1, xl2;
    always_comb begin
        if (x < 11'(W) && y < 10'(H)) maddr = MAW'(32'(y[9:3]) * COLS + 32'(x[10:3]));
        else maddr = '0;
    end
    always_ff @(posedge clk) begin
        map_q <= map_rom[maddr];
        xl1 <= x[2:0];
        yl1 <= y[2:0];
    end

    // clock 2: the tile row, and the cell's element state
    logic [8*BPP-1:0] tile_q;
    logic             on2;
    always_ff @(posedge clk) begin
        tile_q <= tile_rom[{map_q[TB-1:0], yl1}];
        on2 <= state[map_q[TB+EB-1:TB]];
        xl2 <= xl1;
    end

    // clock 3: the colour
    logic [BPP-1:0] idx;
    assign idx = tile_q[xl2*BPP +: BPP];
    always_ff @(posedge clk) rgb <= on2 ? LIT[idx*24 +: 24] : PAL[idx*24 +: 24];
endmodule
