// HD44780 as the CZ-101 uses it (8-bit bus on port A, E/RW/RS on port C), transcribed from
// MAME's hd44780_base_device (BSD-3-Clause): control_write, data_write, control_read,
// data_read, correct_ac, update_ac, shift_display; function set allowed at any time
// (cz101 driver). The 4-bit interface is not modelled: the CZ-101 drives 8 bits.
//
// Busy flag in virtual time. MAME clears it with a timer at t_w + n / 270 kHz; the CPU sees
// the clear at the first instruction boundary s with clear_time < (s + 1) cycles of 300 ns.
// One 270 kHz cycle is 12.345679 CPU cycles, so a read in the instruction starting at s sees
// busy while s - t_w <= 122 (n = 10) or <= 5060 (n = 410).

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

    // render port: line 0/1, position 0..15
    input  logic        r_line,
    input  logic [3:0]  r_pos,
    output logic [7:0]  r_code,
    output logic        r_cursor,      // the cell's DDRAM address is the address counter
    input  logic [2:0]  r_cg_code,
    input  logic [2:0]  r_cg_row,
    output logic [4:0]  r_cg_bits,
    output logic        display_on,
    output logic        cursor_on,
    output logic        blink_on,
    output logic [6:0]  cursor_addr
);

    logic [7:0]  ddram [0:127];
    logic [7:0]  cgram [0:63];
    logic signed [7:0] ac;
    logic        cg;           // m_active_ram == CGRAM
    logic        dir_inc;      // m_direction == +1
    logic        shift_on, two_line;
    logic [6:0]  disp_shift;   // 0..0x4F
    logic        disp_on, cur_on, blk_on;
    logic [7:0]  db_latch;
    logic        e_state, rw_state, rs_state;
    logic [31:0] busy_t;       // local time of the last set_busy_flag
    logic        busy_long;    // 410 cycles (else 10)
    logic        clearing;
    logic [6:0]  clr_idx;
    logic        do_cg_clear;

    assign display_on = disp_on; assign cursor_on = cur_on; assign blink_on = blk_on;
    assign cursor_addr = ac[6:0];

    wire [31:0] since = insn_start - busy_t;
    wire        busy = busy_long ? (since <= 32'd5060) : (since <= 32'd122);

    function automatic logic signed [7:0] correct(input logic signed [7:0] a, input logic c,
                                                  input logic two);
        logic signed [7:0] mx;
        if (c) return a & 8'sh3F;
        mx = two ? 8'sh67 : 8'sh4F;
        if (a > mx) return a - mx - 8'sd1;
        if (a < 0) return mx;
        if (two && a > 8'sh27 && a < 8'sh40) return 8'sh40 + (a - 8'sh28);
        return a;
    endfunction

    function automatic logic signed [7:0] step(input logic signed [7:0] a, input logic inc,
                                               input logic c, input logic two);
        if (!c && two && !inc && a == 8'sh40) return 8'sh27;
        return correct(inc ? a + 8'sd1 : a - 8'sd1, c, two);
    endfunction

    function automatic logic [6:0] shift(input logic [6:0] sft, input logic inc);
        if (inc) return (sft == 7'h4F) ? 7'h00 : sft + 7'd1;
        return (sft == 7'h00) ? 7'h4F : sft - 7'd1;
    endfunction

    // port A read value (db_r): E high and RW = 1
    always_comb begin
        if (e_state && rw_state) begin
            if (!rs_state) db_out = {busy, ac[6:0]};
            else db_out = cg ? cgram[ac[5:0]] : ddram[ac[6:0]];
        end else db_out = 8'hFF;
    end

    task automatic set_busy(input logic long_);
        busy_t <= now;
        busy_long <= long_;
    endtask

    task automatic do_reset();
        ac <= 8'sh0; cg <= 1'b0; dir_inc <= 1'b1; shift_on <= 1'b0; two_line <= 1'b0;
        disp_shift <= 7'h0; disp_on <= 1'b0; cur_on <= 1'b0; blk_on <= 1'b0;
        clearing <= 1'b1; clr_idx <= 7'h0;
        set_busy(1'b1);
    endtask

    always_ff @(posedge clk) begin
        if (clearing) begin
            // device_reset / clear display: DDRAM to spaces (CGRAM to 0 on reset)
            ddram[clr_idx] <= 8'h20;
            if (clr_idx < 7'd64 && do_cg_clear) cgram[clr_idx[5:0]] <= 8'h00;
            clr_idx <= clr_idx + 7'd1;
            if (clr_idx == 7'd127) clearing <= 1'b0;
        end
        if (reset) begin
            do_reset();
            do_cg_clear <= 1'b1;
            busy_t <= 32'h0;
            e_state <= 1'b0; rw_state <= 1'b0; rs_state <= 1'b0; db_latch <= 8'h0;
        end else begin
            if (clearing && clr_idx == 7'd127) do_cg_clear <= 1'b0;
            if (db_we) db_latch <= db_in;
            if (lcd_reset) begin
                do_reset();
                do_cg_clear <= 1'b1;
                rw_state <= 1'b0; rs_state <= 1'b0;   // MAME device_reset: m_rs_state/m_rw_state
            end else if (db_rd && e_state && rw_state && rs_state) begin
                // data_read: busy and address counter
                set_busy(1'b0);
                ac <= step(ac, dir_inc, cg, two_line);
            end else if (pc_we) begin
                logic ne;
                ne = !pc[7];
                if (!ne && e_state && !rw_state) begin
                    if (!rs_state) control_write(db_latch);
                    else data_write(db_latch);
                end
                e_state <= ne; rw_state <= pc[6]; rs_state <= pc[5];
            end
        end
    end


    task automatic control_write(input logic [7:0] d);
        if (d[7]) begin
            cg <= 1'b0;
            ac <= correct({1'b0, d[6:0]}, 1'b0, two_line);
            set_busy(1'b0);
        end else if (d[6]) begin
            cg <= 1'b1;
            ac <= {2'b00, d[5:0]};
            set_busy(1'b0);
        end else if (d[5]) begin
            // function set (8-bit interface; 5x10 font ignored, as on the CZ-101's 5x8 LCD)
            two_line <= d[3];
            ac <= correct(ac, cg, d[3]);
            set_busy(1'b0);
        end else if (d[4]) begin
            if (d[3]) disp_shift <= shift(disp_shift, d[2]);
            else ac <= step(ac, d[2], cg, two_line);
            set_busy(1'b0);
        end else if (d[3]) begin
            disp_on <= d[2]; cur_on <= d[1]; blk_on <= d[0];
            set_busy(1'b0);
        end else if (d[2]) begin
            dir_inc <= d[1]; shift_on <= d[0];
            set_busy(1'b0);
        end else if (d[1]) begin
            ac <= 8'sh0; cg <= 1'b0; dir_inc <= 1'b1; disp_shift <= 7'h0;
            set_busy(1'b1);
        end else if (d[0]) begin
            ac <= 8'sh0; cg <= 1'b0; dir_inc <= 1'b1; disp_shift <= 7'h0;
            clearing <= 1'b1; clr_idx <= 7'h0;
            set_busy(1'b1);
        end
    endtask

    task automatic data_write(input logic [7:0] d);
        if (cg) cgram[ac[5:0]] <= d;
        else ddram[ac[6:0]] <= d;
        set_busy(1'b0);
        ac <= step(ac, dir_inc, cg, two_line);
        if (shift_on) disp_shift <= shift(disp_shift, dir_inc);
    endtask

    // ------------------------------------------------------------------ render
    // MAME screen_update: char at line l, position p reads DDRAM[(l ? 0x40 : 0) + (p + shift) % 0x28]
    // for two lines (one line: % 0x50)
    logic [6:0] sp, m;
    always_comb begin
        sp = {3'b0, r_pos} + disp_shift;
        if (two_line) begin
            m = sp;
            if (m >= 7'd80) m = m - 7'd80;
            if (m >= 7'd40) m = m - 7'd40;
            m = m + (r_line ? 7'h40 : 7'h00);
        end else begin
            m = (sp >= 7'd80) ? sp - 7'd80 : sp;
        end
    end
    assign r_code = ddram[m];
    assign r_cursor = {1'b0, m} == ac;
    assign r_cg_bits = cgram[{r_cg_code, r_cg_row}][4:0];

endmodule
