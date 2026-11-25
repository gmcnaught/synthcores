// SPDX-License-Identifier: GPL-2.0-or-later
// MT-32 (second generation): board (8098, memories, LCD, MIDI) and sound (LA32, reverb, mix),
// after Nuked-MT32. The PCM ROM is a synthframe memory-port client; ROMs load through the
// two load ports before `rst` is released (the control ROM may also load during reset).
`ifndef MCS96_DECODE_HEX
`define MCS96_DECODE_HEX "mcs96_decode.hex"
`endif
module mt32_core #(
    parameter DECODE_HEX = `MCS96_DECODE_HEX,
    parameter logic [28:0] PCM_BASE = 29'd0,
    parameter POW_HEX = "mt32_la32_pow.hex",
    parameter LOGSIN_HEX = "mt32_la32_logsin.hex"
) (
    input  logic        clk,
    input  logic        rst,
    output logic        ready,

    input  logic        rom_we,         // control ROM, 128 KB
    input  logic [16:0] rom_addr,
    input  logic [7:0]  rom_data,
    input  logic        rvrom_we,       // reverb ROM, 32 KB
    input  logic [14:0] rvrom_addr,
    input  logic [7:0]  rvrom_data,

    output logic        pcm_req_valid,
    input  logic        pcm_req_ready,
    output logic [28:0] pcm_req_addr,
    output logic [1:0]  pcm_req_tag,
    input  logic        pcm_rsp_valid,
    input  logic [63:0] pcm_rsp_data,
    input  logic [1:0]  pcm_rsp_tag,

    input  logic        hold,
    output logic        at_hold,
    input  logic        midi_valid,
    input  logic [7:0]  midi_byte,
    output logic        midi_full,
    input  logic [9:0]  buttons,
    input  logic [9:0]  knob,
    output logic        tx_stb,         // MIDI out: pulse, a byte the CPU sends (tx_byte)
    output logic [7:0]  tx_byte,
    output logic        msg_led,        // MIDI MESSAGE LED: bit 0 of the last write to 0200 (MAME so_w)

    output logic        sample_valid,
    output logic signed [15:0] out_l,
    output logic signed [15:0] out_r,

    input  logic [4:0]  lcd_rd_addr,
    output logic [7:0]  lcd_rd_text,
    input  logic [4:0]  lcd_cg_addr,
    output logic [4:0]  lcd_cg_row,
    output logic [4:0]  lcd_cursor,
    output logic        lcd_cursor_on,
    output logic        lcd_display_on,

    // trace
    output logic [47:0] cpu_cycles,
    output logic [63:0] la32_cycle,
    output logic        dbg_fetch, dbg_irq, dbg_regw, dbg_busw, dbg_midi, dbg_lcd, bad_op,
    output logic [2:0]  dbg_level,
    output logic [15:0] dbg_pc, dbg_fpc, dbg_psw, dbg_waddr,
    output logic [7:0]  dbg_addr, dbg_data, dbg_wdata, dbg_mbyte,
    output logic        extint,
    output logic signed [15:0] dbg_acc2, dbg_acc3, dbg_acc6, dbg_acc7, dbg_rv0, dbg_rv1,
    output logic [8:0]  dbg_vol
);
    logic        cpu_turn, cpu_done, la_we, rv_we, b_ready;
    logic [4:0]  slot_idx;
    logic [8:0]  la_addr;
    logic [7:0]  la_wdata, la_rdata, rv_value, pwm;
    logic [1:0]  rv_reg;

    assign ready = b_ready;

    mt32_board #(.DECODE_HEX(DECODE_HEX)) board (
        .clk, .rst, .ready(b_ready),
        .rom_we, .rom_addr, .rom_data,
        .hold, .at_hold, .midi_valid, .midi_byte, .midi_full, .buttons, .knob,
        .cpu_turn, .turn_slot(slot_idx), .la32_cycle, .cpu_done,
        .la_we, .la_addr, .la_wdata, .la_rdata, .extint,
        .rv_we, .rv_reg, .rv_value, .pwm, .tx_stb, .tx_byte,
        .lcd_rd_addr, .lcd_rd_text, .lcd_cg_addr, .lcd_cg_row, .lcd_cursor, .lcd_cursor_on,
        .lcd_display_on,
        .cpu_cycles, .dbg_fetch, .dbg_irq, .dbg_regw, .dbg_level, .dbg_pc, .dbg_fpc, .dbg_psw,
        .dbg_addr, .dbg_data, .dbg_busw, .dbg_waddr, .dbg_wdata, .dbg_midi, .dbg_mbyte, .dbg_lcd,
        .bad_op
    );

    mt32_sound #(.PCM_BASE(PCM_BASE), .POW_HEX(POW_HEX), .LOGSIN_HEX(LOGSIN_HEX)) sound (
        .clk, .rst, .run(b_ready),
        .cpu_turn, .slot_idx, .cpu_done, .la32_cycle,
        .bus_we(la_we), .bus_addr(la_addr), .bus_wdata(la_wdata), .bus_rdata(la_rdata), .extint,
        .rv_we, .rv_sel(rv_reg), .rv_data(rv_value), .pwm,
        .rvrom_we, .rvrom_addr, .rvrom_data,
        .pcm_req_valid, .pcm_req_ready, .pcm_req_addr, .pcm_req_tag,
        .pcm_rsp_valid, .pcm_rsp_data, .pcm_rsp_tag,
        .sample_valid, .out_l, .out_r,
        .dbg_acc2, .dbg_acc3, .dbg_acc6, .dbg_acc7, .dbg_rv0, .dbg_rv1, .dbg_vol,
        .dbg_slot_done(), .dbg_oo(), .dbg_prev(), .dbg_w46(), .dbg_ram2_0(), .dbg_ram2_1(),
        .dbg_ram2_2(), .dbg_sacc2(), .dbg_sacc3(), .dbg_sacc6(), .dbg_sacc7(), .dbg_w186(),
        .dbg_w187(), .dbg_w188()
    );
    // the 0200 write (rv_reg 2) also carries the MIDI MESSAGE LED in bit 0
    always_ff @(posedge clk)
        if (rst) msg_led <= 1'b0;
        else if (rv_we && rv_reg == 2'd2) msg_led <= rv_value[0];
endmodule
