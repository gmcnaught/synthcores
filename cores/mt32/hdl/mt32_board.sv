// SPDX-License-Identifier: GPL-2.0-or-later
// Roland MT-32 (second generation) board, after Nuked-MT32 (nukeykt): mt32.cpp (memory map,
// bank register, buttons, MIDI pacing) and la32.cpp clock() (the CPU runs after every LA32 slot).
//
// Per LA32 slot (mt32_sound runs the slot, the reverb steps, the mix):
//   la32_cycles += 8; CPU runs to (la32_cycles * 375) >> 8 (mcs96 `run`);
//   push_midi(): if (uart_cycles == 0 || uart_cycles < total_cycles()) and a byte is waiting,
//                serial_w(byte), uart_cycles = total_cycles() + 3840;
//   after slots 15 and 31: la32_sh3 = (slot == 31).
// `hold` stops the board before the CPU turn of slot 0 (the start of a sample): the MiSTer top
// holds while its audio FIFO is full; the bench holds to queue a sample's MIDI bytes first.
//
// CPU memory map (cpu_read / cpu_write):
//   0100-017F  bank register (write: data & 1F)       0200-027F  buttons (read), reverb ROM bank (write)
//   0300-037F  LCD data queue                         0380-03FF  LCD command (write), busy (read bit 0)
//   0400-047F  reverb saw1 (write)                    0800-087F  reverb saw0 (write)
//   0C00-0FFF  LA32                                   1000-7FFF  control ROM (direct)
//   8000-BFFF  bank window: ROM bank 0-7, or RAM bank 0-1 when bit 4 is set (writable only then)
//   C000-FFFF  RAM bank 0
//   other reads FF, other writes ignored.
`ifndef MCS96_DECODE_HEX
`define MCS96_DECODE_HEX "mcs96_decode.hex"
`endif
module mt32_board #(
    parameter DECODE_HEX = `MCS96_DECODE_HEX
) (
    input  logic        clk,
    input  logic        rst,
    output logic        ready,

    // control ROM (128 KB) load, before or during reset
    input  logic        rom_we,
    input  logic [16:0] rom_addr,
    input  logic [7:0]  rom_data,

    input  logic        hold,
    output logic        at_hold,        // waiting at the start of a sample (hold is high)
    input  logic        midi_valid,     // a byte for the MIDI queue
    input  logic [7:0]  midi_byte,
    output logic        midi_full,
    input  logic [9:0]  buttons,        // MT32_BUTTTON_* order of mt32.h
    input  logic [9:0]  knob,

    // sound side (mt32_sound)
    input  logic        cpu_turn,       // pulse: slot `turn_slot` done, the CPU may run
    input  logic [4:0]  turn_slot,
    input  logic [63:0] la32_cycle,     // already advanced for the slot
    output logic        cpu_done,       // pulse: turn finished
    output logic        la_we,          // LA32 register write (0C00-0FFF)
    output logic [8:0]  la_addr,
    output logic [7:0]  la_wdata,
    input  logic [7:0]  la_rdata,
    input  logic        extint,         // LA32 interrupt line
    output logic        rv_we,          // reverb register write, raw CPU byte: 0 = 0800 saw0, 1 = 0400 saw1, 2 = 0200 ROM bank
    output logic [1:0]  rv_reg,
    output logic [7:0]  rv_value,
    output logic [7:0]  pwm,            // master volume (255 - pwm) / 255
    output logic        tx_stb,         // MIDI out: pulse, the CPU wrote tx_byte to SBUF
    output logic [7:0]  tx_byte,

    // LCD for the renderer
    input  logic [4:0]  lcd_rd_addr,
    output logic [7:0]  lcd_rd_text,
    input  logic [4:0]  lcd_cg_addr,
    output logic [4:0]  lcd_cg_row,
    output logic [4:0]  lcd_cursor,
    output logic        lcd_cursor_on,
    output logic        lcd_cursor_blink,
    output logic        lcd_cursor_font,
    output logic        lcd_lines2,
    output logic        lcd_display_on,

    // trace (bench)
    output logic [47:0] cpu_cycles,
    output logic        dbg_fetch, dbg_irq, dbg_regw,
    output logic [2:0]  dbg_level,
    output logic [15:0] dbg_pc, dbg_fpc, dbg_psw,
    output logic [7:0]  dbg_addr, dbg_data,
    output logic        dbg_busw,       // pulse: external write (dbg_waddr, dbg_wdata)
    output logic [15:0] dbg_waddr,
    output logic [7:0]  dbg_wdata,
    output logic        dbg_midi,       // pulse: byte handed to the CPU (dbg_mbyte)
    output logic [7:0]  dbg_mbyte,
    output logic        dbg_lcd,        // pulse: after an LCD command write
    output logic        bad_op,
    output logic        unsupported     // HSO command 0F ran (A/D start without MAME's nested update)
);
    // ---------------------------------------------------------------- memories
    logic [7:0] rom [131072];
    logic [7:0] ram [32768];
    logic [16:0] rom_ra;
    logic [14:0] ram_ra, ram_wa;
    logic        clearing;
    logic [15:0] clr_a;
    logic [4:0]  reg_bank;
    // read addresses straight from the CPU's bus: the data is there the clock after the request
    always_comb begin
        rom_ra = (ext_addr[15] && !reg_bank[4]) ? {reg_bank[2:0], ext_addr[13:0]} : {1'b0, ext_addr};
        ram_ra = (ext_addr[15:14] == 2'b10) ? {reg_bank[0], ext_addr[13:0]} : {1'b0, ext_addr[13:0]};
    end
    logic [7:0]  rom_q, ram_q, ram_wd;
    logic        ram_we;
    always_ff @(posedge clk) begin
        if (rom_we) rom[rom_addr] <= rom_data;
        rom_q <= rom[rom_ra];
    end
    always_ff @(posedge clk) begin
        if (ram_we) ram[ram_wa] <= ram_wd;
        ram_q <= ram[ram_ra];
    end

    // ---------------------------------------------------------------- CPU
    logic        c_run, c_busy, c_ready, ext_req, ext_we, ext_ack, serial_in, sh3;
    logic [47:0] c_target;
    logic [15:0] ext_addr;
    logic [7:0]  ext_wdata, ext_rdata, serial_byte;

    mcs96 #(.DECODE_HEX(DECODE_HEX)) cpu (
        .clk, .rst, .ready(c_ready),
        .run(c_run), .target(c_target), .busy(c_busy), .total_cycles(cpu_cycles),
        .ext_req, .ext_we, .ext_addr, .ext_wdata, .ext_rdata, .ext_ack,
        .serial_in, .serial_byte, .extint, .la32_sh3(sh3), .knob, .pwm, .tx_stb, .tx_byte,
        .dbg_fetch, .dbg_irq, .dbg_level, .dbg_pc, .dbg_fpc, .dbg_psw,
        .dbg_regw, .dbg_addr, .dbg_data, .bad_op, .unsupported
    );

    // ---------------------------------------------------------------- LCD
    logic lcd_wr, lcd_cmd, lcd_busy, lcd_done, lcd_cmd_done;
    mt32_lcd lcd (
        .clk, .rst, .wr(lcd_wr), .cmd(lcd_cmd), .data(ext_wdata), .now(cpu_cycles),
        .busy(lcd_busy), .done(lcd_done), .cmd_done(lcd_cmd_done),
        .rd_addr(lcd_rd_addr), .rd_text(lcd_rd_text), .cg_addr(lcd_cg_addr), .cg_row(lcd_cg_row),
        .cursor(lcd_cursor), .cursor_on(lcd_cursor_on), .cursor_blink(lcd_cursor_blink),
        .cursor_font(lcd_cursor_font), .lines2(lcd_lines2), .display_on(lcd_display_on)
    );
    assign dbg_lcd = lcd_cmd_done;
    assign at_hold = (ts == T_HOLD) && (slot == 5'd0) && hold;

    // ---------------------------------------------------------------- MIDI queue
    logic [7:0] mq [256];
    logic [7:0] mq_wp, mq_rp;
    logic [47:0] uart_cycles;
    assign midi_full = (mq_wp + 8'd1) == mq_rp;
    always_ff @(posedge clk) if (midi_valid && !midi_full) mq[mq_wp] <= midi_byte;

    // ---------------------------------------------------------------- external bus
    typedef enum logic [2:0] { B_IDLE, B_READ, B_LCD } b_t;
    b_t bs;
    logic [7:0] rsel;                   // read source
    wire [8:0] hi = ext_addr[15:7];


    // ---------------------------------------------------------------- turn sequencing
    typedef enum logic [2:0] { T_IDLE, T_HOLD, T_RUN, T_WAIT, T_MIDI, T_DONE } t_t;
    t_t ts;
    logic [4:0]  slot;

    always_ff @(posedge clk) begin
        c_run <= 1'b0;
        ext_ack <= 1'b0;
        ram_we <= 1'b0;
        la_we <= 1'b0;
        rv_we <= 1'b0;
        lcd_wr <= 1'b0;
        serial_in <= 1'b0;
        cpu_done <= 1'b0;
        dbg_busw <= 1'b0;
        dbg_midi <= 1'b0;
        if (rst) begin
            bs <= B_IDLE;
            ts <= T_IDLE;
            reg_bank <= 5'd0;
            mq_wp <= 8'd0; mq_rp <= 8'd0;
            uart_cycles <= 48'h0;
            sh3 <= 1'b0;
            clearing <= 1'b1; clr_a <= 16'h0;
            ready <= 1'b0;
        end else begin
            if (clearing) begin
                ram_we <= 1'b1; ram_wa <= clr_a[14:0]; ram_wd <= 8'h00;
                clr_a <= clr_a + 16'd1;
                if (clr_a == 16'h7fff) clearing <= 1'b0;
            end
            ready <= !clearing && c_ready;
            if (midi_valid && !midi_full) mq_wp <= mq_wp + 8'd1;

            // ------------------------------------------------ the CPU's turn after a slot
            case (ts)
            T_IDLE: if (cpu_turn) begin
                slot <= turn_slot;
                // 57-bit product: exact while la32_cycle < 2^48 (1.09 years). A 48-bit product
                // wrapped at la32_cycle = 2^48 / 375 (25.45 h) and stopped the CPU for good.
                c_target <= 48'(({9'h0, la32_cycle[47:0]} * 57'd375) >> 8);
                ts <= T_HOLD;
            end
            T_HOLD: if (!(hold && slot == 5'd0)) begin
                c_run <= 1'b1;
                ts <= T_RUN;
            end
            T_RUN: ts <= T_WAIT;                       // busy rises the clock after run
            T_WAIT: if (!c_busy && !c_run) ts <= T_MIDI;
            T_MIDI: begin
                if ((uart_cycles == 48'h0 || uart_cycles < cpu_cycles) && mq_rp != mq_wp) begin
                    serial_in <= 1'b1;
                    serial_byte <= mq[mq_rp];
                    dbg_midi <= 1'b1;
                    dbg_mbyte <= mq[mq_rp];
                    mq_rp <= mq_rp + 8'd1;
                    uart_cycles <= cpu_cycles + 48'd3840;
                end
                if (slot == 5'd15 || slot == 5'd31) sh3 <= (slot == 5'd31);
                cpu_done <= 1'b1;
                ts <= T_IDLE;
            end
            default: ts <= T_IDLE;
            endcase

            // ------------------------------------------------ CPU external accesses
            case (bs)
            B_IDLE: if (ext_req && !ext_ack) begin
                if (ext_we) begin
                    dbg_busw <= 1'b1; dbg_waddr <= ext_addr; dbg_wdata <= ext_wdata;
                    ext_ack <= 1'b1;                     // B_LCD below waits instead
                    if (hi >= 9'h100 && hi < 9'h180) begin
                        if (reg_bank[4]) begin ram_we <= 1'b1; ram_wa <= {reg_bank[0], ext_addr[13:0]}; ram_wd <= ext_wdata; end
                    end else if (hi >= 9'h180) begin
                        ram_we <= 1'b1; ram_wa <= {1'b0, ext_addr[13:0]}; ram_wd <= ext_wdata;
                    end else if (hi >= 9'h018 && hi < 9'h020) begin
                        la_we <= 1'b1; la_addr <= ext_addr[8:0]; la_wdata <= ext_wdata;
                    end else case (hi)
                        9'd2:  reg_bank <= ext_wdata[4:0];
                        9'd4:  begin rv_we <= 1'b1; rv_reg <= 2'd2; rv_value <= ext_wdata; end
                        9'd6:  begin lcd_wr <= 1'b1; lcd_cmd <= 1'b0; ext_ack <= 1'b0; bs <= B_LCD; end
                        9'd7:  begin lcd_wr <= 1'b1; lcd_cmd <= 1'b1; ext_ack <= 1'b0; bs <= B_LCD; end
                        9'd8:  begin rv_we <= 1'b1; rv_reg <= 2'd1; rv_value <= ext_wdata; end
                        9'd16: begin rv_we <= 1'b1; rv_reg <= 2'd0; rv_value <= ext_wdata; end
                        default: ;
                    endcase
                end else begin
                    if (hi >= 9'h020 && hi < 9'h100) rsel <= 8'd1;
                    else if (hi >= 9'h180) rsel <= 8'd2;
                    else if (hi >= 9'h100) rsel <= reg_bank[4] ? 8'd2 : 8'd1;
                    else if (hi >= 9'h018 && hi < 9'h020) begin
                        rsel <= 8'd3;
                        la_addr <= ext_addr[8:0];        // the LA32 decodes the read's own address
                    end
                    else if (hi == 9'd7) rsel <= 8'd4;
                    else if (hi == 9'd4) rsel <= 8'd5;
                    else rsel <= 8'd0;
                    bs <= B_READ;
                end
            end
            B_READ: begin
                case (rsel)
                    8'd1: ext_rdata <= rom_q;
                    8'd2: ext_rdata <= ram_q;
                    8'd3: ext_rdata <= la_rdata;
                    8'd4: ext_rdata <= {7'h0, lcd_busy};
                    8'd5: begin
                        logic [3:0] sc;
                        logic [7:0] r;
                        sc = ext_addr[4:1];
                        r = 8'hff;
                        if (!sc[1]) r = r & ~{3'b000, buttons[4:0]};
                        if (!sc[0]) r = r & ~{3'b000, buttons[9:5]};
                        ext_rdata <= r;
                    end
                    default: ext_rdata <= 8'hff;
                endcase
                ext_ack <= 1'b1;
                bs <= B_IDLE;
            end
            B_LCD: if (lcd_done) begin ext_ack <= 1'b1; bs <= B_IDLE; end
            default: bs <= B_IDLE;
            endcase
        end
    end
endmodule
