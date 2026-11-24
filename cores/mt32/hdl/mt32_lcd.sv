// SPDX-License-Identifier: GPL-2.0-or-later
// MT-32 LCD controller and gate-array queue, after Nuked-MT32 (nukeykt): mt32.cpp cpu_write()
// cases 6 and 7, lcd.cpp lcd_write() / lcd_reset(), and the busy flag the CPU reads at 0380.
//
// Writes to 0300 queue up to 20 data bytes. A write to 0380 with bit 7 set and a position in
// 0-9 or 40h-49h replays the queue as data after the command; any other 0380 write is a
// command on its own (bit 7 set with another position does nothing); the queue is emptied.
// Every lcd_write moves the busy end: lcd_cycles = max(lcd_cycles + 100, now + 100).
module mt32_lcd (
    input  logic        clk,
    input  logic        rst,

    input  logic        wr,            // CPU write to 0300-037F (cmd = 0) or 0380-03FF (cmd = 1)
    input  logic        cmd,
    input  logic [7:0]  data,
    input  logic [47:0] now,           // CPU total_cycles() at the write / read
    output logic        busy,          // read value bit 0 at 0380: now < lcd_cycles
    output logic        done,          // pulse: the write (and any replay) is finished
    output logic        cmd_done,      // pulse with done after a 0380 write (trace: LCD text)

    // display state for the renderer and the trace
    input  logic [4:0]  rd_addr,       // 0-19: text;
    output logic [7:0]  rd_text,
    input  logic [4:0]  cg_addr,       // {char[1:0], row[2:0]}
    output logic [4:0]  cg_row,
    output logic [4:0]  cursor,
    output logic        cursor_on,
    output logic        cursor_blink,
    output logic        display_on
);
    logic [7:0]  text [20];
    logic [4:0]  cg [32];
    logic [7:0]  q [20];
    logic [4:0]  qn, qi;
    logic [47:0] lcd_cycles;
    logic [4:0]  cgram_addr_r;          // 2 bits used
    logic [2:0]  cgram_row;
    logic        dir, lines2, font, replay;
    logic [7:0]  cmdv;

    assign busy = now < lcd_cycles;
    assign rd_text = text[rd_addr < 5'd20 ? rd_addr : 5'd0];
    assign cg_row = cg[cg_addr];

    // one lcd_write(a, d)
    task automatic lcd_write(input logic a, input logic [7:0] d);
        logic [47:0] c;
        c = lcd_cycles + 48'd100;
        if (c < now + 48'd100) c = now + 48'd100;
        lcd_cycles <= c;
        if (!a) begin
            if (d[7]) begin
                logic [5:0] p;
                p = {1'b0, d[4:0]} + (d[6] ? 6'd10 : 6'd0);
                cursor <= (p > 6'd19) ? 5'd19 : p[4:0];
            end else if (d[6]) begin
                cg[{cgram_addr_r[1:0], cgram_row}] <= d[4:0];
                cgram_row <= cgram_row + 3'd1;
            end else if (d[5]) begin
                cgram_addr_r <= {3'b000, d[1:0]};
                cgram_row <= 3'd0;
            end else if (d == 8'h10) begin
                // lcd_reset(soft = true): CG RAM kept
                cursor <= 5'd0; cgram_addr_r <= 5'd0; cgram_row <= 3'd0; dir <= 1'b0;
                lines2 <= 1'b0; font <= 1'b0; cursor_blink <= 1'b0; display_on <= 1'b0;
            end else case (d[4:1])
                4'd2: dir <= d[0];
                4'd3: begin
                    if (d[0]) begin
                        if (!lines2) begin if (cursor != 5'd0) cursor <= cursor - 5'd1; end
                        else if (cursor != 5'd0 && cursor != 5'd10) cursor <= cursor - 5'd1;
                    end else begin
                        if (!lines2) begin if (cursor != 5'd19) cursor <= cursor + 5'd1; end
                        else if (cursor != 5'd9 && cursor != 5'd19) cursor <= cursor + 5'd1;
                    end
                end
                4'd4: font <= d[0];
                4'd5: cursor_blink <= d[0];
                4'd6: display_on <= d[0];
                4'd7: cursor_on <= d[0];
                4'd9: lines2 <= d[0];
                default: ;
            endcase
        end else begin
            text[cursor] <= d;
            if (dir) begin
                if (!lines2) begin if (cursor != 5'd0) cursor <= cursor - 5'd1; end
                else if (cursor != 5'd0 && cursor != 5'd10) cursor <= cursor - 5'd1;
            end else begin
                if (!lines2) begin if (cursor != 5'd19) cursor <= cursor + 5'd1; end
                else if (cursor != 5'd9 && cursor != 5'd19) cursor <= cursor + 5'd1;
            end
        end
    endtask

    always_ff @(posedge clk) begin
        done <= 1'b0;
        cmd_done <= 1'b0;
        if (rst) begin
            // constructor: lcd_reset() (hard: CG RAM cleared), lcd_cycles 0, queue empty
            cursor <= 5'd0; cgram_addr_r <= 5'd0; cgram_row <= 3'd0; dir <= 1'b0;
            lines2 <= 1'b0; font <= 1'b0; cursor_blink <= 1'b0; display_on <= 1'b0;
            cursor_on <= 1'b0;
            for (int i = 0; i < 32; i++) cg[i] <= 5'h0;
            for (int i = 0; i < 20; i++) text[i] <= 8'h0;
            lcd_cycles <= 48'h0;
            qn <= 5'd0;
            replay <= 1'b0;
        end else if (replay) begin
            // the queued data bytes after the command, one a clock
            if (qi == qn) begin
                replay <= 1'b0;
                qn <= 5'd0;
                done <= 1'b1;
                cmd_done <= 1'b1;
            end else begin
                lcd_write(1'b1, q[qi]);
                qi <= qi + 5'd1;
            end
        end else if (wr) begin
            if (!cmd) begin
                if (qn < 5'd20) begin q[qn] <= data; qn <= qn + 5'd1; end
                done <= 1'b1;
            end else begin
                if (data[7]) begin
                    if (data[6:0] < 7'd10 || (data[6:0] >= 7'd64 && data[6:0] < 7'd74)) begin
                        lcd_write(1'b0, data);
                        qi <= 5'd0;
                        replay <= 1'b1;
                    end else begin
                        qn <= 5'd0;
                        done <= 1'b1;
                        cmd_done <= 1'b1;
                    end
                end else begin
                    lcd_write(1'b0, data);
                    qn <= 5'd0;
                    done <= 1'b1;
                    cmd_done <= 1'b1;
                end
            end
        end
    end
endmodule
