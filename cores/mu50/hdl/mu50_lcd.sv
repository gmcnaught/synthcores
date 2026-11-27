// SPDX-License-Identifier: MIT
// The MU50's LCD as MAME's ymmu50.cpp drives it: an HD44780 (mulcd, ROM B04) on port A (data)
// and port 6 (bit 0 E, inverted; bit 1 RW; bit 2 RS). The controller is
// modules/synthframe/hdl/panel/synthframe_hd44780.sv (MAME's hd44780_base_device).
//
//   p6_w: E (after the inversion) falling with RW = 0 writes port A's latch: RS ? data : control.
//   pa_r: E high: RW ? (RS ? data_r : control_r) : 00; E low: the port A latch.
//
// Busy flag in virtual time. MAME clears it with a timer at t_w + n / 270 kHz (t_w: the end of
// the port 6 write; n = 10, or 410 after clear, home and reset). MAME's H8 ends its timeslice
// after the first bus access ending at or after floor(clear time), or at the end of an
// instruction whose internal operations crossed it, and the timer runs there. So a read sees busy
// while max(end of the access before it, start of the current execution step) - t_w <= 369
// (n = 10) or 15184 (n = 410). Checked against MAME's log of every clear in 12 s of the v1.05
// firmware (3,049 clears).

module mu50_lcd (
    input  logic        clk,
    input  logic        reset,
    input  logic [31:0] now,           // end time of the current access
    input  logic [31:0] t_read,        // max(previous access end, current step start)

    input  logic        p6_we,         // do_write_port(PORT_6, data)
    input  logic        pa_we,         // do_write_port(PORT_A, data)
    input  logic [7:0]  pw_data,
    input  logic        pa_rd,         // port A read with side effects (pr_stb)
    output logic [7:0]  p6_val,        // p6_r
    output logic [7:0]  pa_val,        // pa_r (combinational)

    input  logic        r_line,
    input  logic [4:0]  r_pos,
    input  logic [2:0]  r_row,
    output logic [7:0]  r_code,
    output logic [4:0]  r_cg_bits,
    output logic        r_at_ac,
    output logic        display_on,
    output logic        cursor_on,
    output logic        blink_on
);
    localparam logic [7:0] E = 8'h01, RW = 8'h02, RS = 8'h04;
    logic [7:0] cur_p6, cur_pa, dout;
    wire  [7:0] p6_new = pw_data ^ E;
    wire        wr = p6_we && (cur_p6 & E) != 0 && (p6_new & E) == 0 && (cur_p6 & RW) == 0;
    wire        rd = pa_rd && (cur_p6 & E) != 0 && (cur_p6 & RW) != 0 && (cur_p6 & RS) != 0;

    always_ff @(posedge clk) begin
        if (reset) begin
            cur_p6 <= 8'hff;
            cur_pa <= 8'hff;
        end else begin
            if (p6_we) cur_p6 <= p6_new;
            if (pa_we) cur_pa <= pw_data;
        end
    end

    synthframe_hd44780 #(.POS_W(5), .BUSY_EXT(1), .BUSY_SHORT(369), .BUSY_LONG(15184)) hd (
        .clk, .reset,
        .cs(1'b1), .wr, .rd, .rs((cur_p6 & RS) != 0), .din(cur_pa), .dout,
        .busy_t_set(reset ? 32'd0 : now), .busy_t_read(t_read),
        .r_line, .r_pos, .r_row, .r_code, .r_cgbits(r_cg_bits), .r_is_cg(), .r_at_ac,
        .r_cursor(), .display_on, .cursor_on, .blink_on);

    assign p6_val = cur_p6;
    assign pa_val = (cur_p6 & E) == 0 ? cur_pa : (cur_p6 & RW) != 0 ? dout : 8'h00;
endmodule
