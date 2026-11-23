// SPDX-License-Identifier: MIT
// synthframe: character LCD renderer (the dot layer). Draws LINES x COLS characters of 5 x 8
// dots at (X0, Y0) from a character LCD read port (README.md): for every pixel it asks for the
// character at (line, col) and then for that character's dot row.
//
// Geometry: characters on a CHAR_P-pixel pitch, lines on LINE_P; dots DOT_W x DOT_H pixels on
// a DOT_PX x DOT_PY pitch; the first character LEFT pixels right of X0 and its first dot row TOP
// pixels below Y0. The panel rectangle is PW x PH at (X0, Y0). Row 7 (the cursor row) shows the glyph's row 7 when ROW7_GLYPH = 1,
// otherwise only the cursor.
//
// Timing: x and y in clock 0; cell_line/cell_col are registers, and cell_code/cell_cursor are
// sampled one clock later (the port is combinational); glyph_code/glyph_row are registers and
// glyph_bits is sampled one clock after that. A core whose font is a synchronous RAM looks up
// (cell_code, cell_row) instead and registers glyph_bits on the clock glyph_code is registered.
// panel/dot/lit belong to the pixel at (x, y) two clocks earlier; the caller registers its colour from them (three clocks in total). Two
// instances that never draw the same pixel can share one glyph lookup: use the code and row of
// the one whose fetch is high.
module synthframe_charlcd #(
    parameter int X0 = 0,
    parameter int Y0 = 0,
    parameter int COLS = 24,
    parameter int LINES = 2,
    parameter int PW = 768,
    parameter int PH = 104,
    parameter int LEFT = 0,
    parameter int TOP = 4,
    parameter int CHAR_P = 32,
    parameter int LINE_P = 50,
    parameter int DOT_W = 4,
    parameter int DOT_H = 4,
    parameter int DOT_PX = 5,
    parameter int DOT_PY = 5,
    parameter bit ROW7_GLYPH = 0
) (
    input  logic        clk,
    input  logic [10:0] x,
    input  logic [9:0]  y,
    // character LCD read port
    output logic [1:0]  cell_line,
    output logic [5:0]  cell_col,
    output logic [2:0]  cell_row,        // the dot row, with cell_line/cell_col
    input  logic [7:0]  cell_code,
    input  logic        cell_cursor,     // the cursor is at (cell_line, cell_col) and shown
    output logic [7:0]  glyph_code,
    output logic [2:0]  glyph_row,
    input  logic [4:0]  glyph_bits,      // bit 4 = leftmost dot
    output logic        fetch,           // glyph_code/glyph_row are for a pixel in the panel
    // the pixel, two clocks late
    output logic        panel,           // inside the panel rectangle
    output logic        dot,             // inside a dot cell
    output logic        lit              // the dot is on (glyph or cursor)
);
    // ------------------------------------------------------------ stage 0: cell and dot
    logic        in_panel0, dot_x0, dot_y0;
    logic [2:0]  dc0, dr0;
    always_ff @(posedge clk) begin : stage0
        logic [10:0] lx;
        logic [9:0]  ly, oy;
        logic [10:0] ox;
        lx = x - 11'(X0 + LEFT);
        ly = y - 10'(Y0 + TOP);
        ox = lx % 11'(CHAR_P);
        oy = ly % 10'(LINE_P);
        in_panel0 <= x >= 11'(X0) && x < 11'(X0 + PW) && y >= 10'(Y0) && y < 10'(Y0 + PH);
        cell_col  <= 6'(lx[9:0] / 10'(CHAR_P));
        cell_line <= 2'(ly / 10'(LINE_P));
        dc0 <= 3'(ox / 11'(DOT_PX));
        dot_x0 <= x >= 11'(X0 + LEFT) && ox < 11'(5 * DOT_PX) && (ox % 11'(DOT_PX)) < 11'(DOT_W);
        dr0 <= 3'(oy / 10'(DOT_PY));
        dot_y0 <= y >= 10'(Y0 + TOP) && ly < 10'(LINES * LINE_P) && oy < 10'(8 * DOT_PY)
                  && (oy % 10'(DOT_PY)) < 10'(DOT_H);
    end

    // ------------------------------------------------------------ stage 1: character code
    logic        cur1, in_panel1, dot1;
    logic [2:0]  dc1;
    always_ff @(posedge clk) begin
        glyph_code <= cell_code;
        glyph_row  <= dr0;
        cur1 <= cell_cursor && dr0 == 3'd7;
        in_panel1 <= in_panel0;
        dot1 <= in_panel0 && dot_x0 && dot_y0 && cell_col < 6'(COLS);
        dc1 <= dc0;
    end

    // ------------------------------------------------------------ stage 2: dot row
    logic [4:0]  row2;
    logic        cur2, in_panel2, dot2;
    logic [2:0]  dc2;
    always_ff @(posedge clk) begin
        row2 <= (!ROW7_GLYPH && glyph_row == 3'd7) ? 5'h00 : glyph_bits;
        cur2 <= cur1; in_panel2 <= in_panel1; dot2 <= dot1; dc2 <= dc1;
    end
    assign cell_row = dr0;
    assign fetch = in_panel1;
    assign panel = in_panel2;
    assign dot   = dot2;
    assign lit   = dot2 && (row2[3'd4 - dc2] || cur2);
endmodule
