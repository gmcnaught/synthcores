// SPDX-License-Identifier: MIT
// HD44780 as the CZ-101 drives it: 8-bit bus on port A, E/RW/RS on port C. The controller is
// modules/synthframe/hdl/panel/synthframe_hd44780.sv (MAME's hd44780_base_device); this file
// is the pin side of MAME's device (db_w, db_r, e_w, rw_w, rs_w): the data latch, the write on
// the falling edge of E, the read side effects, and 0xFF on port A while E is low or RW is 0.
//
// Busy flag in virtual time. MAME clears it with a timer at t_w + n / 270 kHz; the CPU sees
// the clear at the first instruction boundary s with clear_time < (s + 1) cycles of 300 ns.
// One 270 kHz cycle is 12.345679 CPU cycles, so a read in the instruction starting at s sees
// busy while s - t_w <= 122 (n = 10) or <= 5060 (n = 410). Power-on reset starts the 410 at
// time 0, the power-down reset (m_hd44780->reset()) at the current access.

module cz101_lcd (
    input  logic        clk,
    input  logic        reset,         // power-on / MAME device_reset
    input  logic [31:0] now,           // CPU local time of the current access (acc_time)
    input  logic [31:0] insn_start,    // start of the current instruction (vcyc)

    // port C callback (MAME port_c_w with bit 3 set): E = !pc[7], RW = pc[6], RS = pc[5]
    input  logic        pc_we,
    input  logic [7:0]  pc,
    input  logic        lcd_reset,     // power-down: m_hd44780->reset()
    input  logic [7:0]  db_in,         // port A output (MAME db_w on every port A write)
    input  logic        db_we,
    input  logic        db_rd,         // port A is read (MAME db_r): side effects
    output logic [7:0]  db_out,        // value a port A read returns

    // render port: line 0/1, position 0..15, dot row 0..7
    input  logic        r_line,
    input  logic [3:0]  r_pos,
    input  logic [2:0]  r_row,
    output logic [7:0]  r_code,
    output logic        r_at_ac,       // the cell's DDRAM address is the address counter
    output logic [4:0]  r_cg_bits,     // CG RAM row r_row of r_code (codes below 0x10)
    output logic        display_on,
    output logic        cursor_on,
    output logic        blink_on
);
    logic       e_state, rw_state, rs_state;
    logic [7:0] db_latch, dout;
    wire        rd = db_rd && e_state && rw_state && rs_state;      // data_read
    wire        wr = pc_we && !rd && pc[7] && e_state && !rw_state;       // E falls, RW = 0

    always_ff @(posedge clk) begin
        if (reset) begin
            e_state <= 1'b0; rw_state <= 1'b0; rs_state <= 1'b0; db_latch <= 8'h0;
        end else begin
            if (db_we) db_latch <= db_in;
            if (lcd_reset) begin
                rw_state <= 1'b0; rs_state <= 1'b0;   // MAME device_reset: m_rs_state/m_rw_state
            end else if (pc_we && !rd) begin
                e_state <= !pc[7]; rw_state <= pc[6]; rs_state <= pc[5];
            end
        end
    end

    synthframe_hd44780 #(.POS_W(4), .BUSY_EXT(1), .BUSY_SHORT(122), .BUSY_LONG(5060)) hd (
        .clk, .reset(reset || lcd_reset),
        .cs(1'b1), .wr, .rd, .rs(rs_state), .din(db_latch), .dout,
        .busy_t_set(reset ? 32'd0 : now), .busy_t_read(insn_start),
        .r_line, .r_pos, .r_row, .r_code, .r_cgbits(r_cg_bits), .r_is_cg(), .r_at_ac,
        .r_cursor(), .display_on, .cursor_on, .blink_on);

    assign db_out = (e_state && rw_state) ? dout : 8'hFF;
endmodule
