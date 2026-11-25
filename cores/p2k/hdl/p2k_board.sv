// SPDX-License-Identifier: GPL-3.0-or-later
// Proteus 2000 board (cores/p2k/docs/plan.md): the CPU (TG68K + MCF5206e peripherals,
// cores/proteus/p2k/rtl/cpu), its memory (p2k_mem: flash and DRAM in DDR3 behind a cache), the
// board glue of MAME's emup2k.cpp, the sound-chip bus (p2k_sndbus) and the sound engine
// (cores/proteus/p2k/rtl/snd), on one DDR3 port. No MiSTer framework here: the loader, the save
// file, the keyboard, MIDI, video and audio output are in the MiSTer top.
//
// Clocks: clk_sys (88.2 MHz: 2000 clocks a 44.1 kHz sample) for the engine, the sound bus and DDR3;
// clk_cpu = clk_sys / 2 from the same PLL, rising edges aligned (TG68K does not reach 88 MHz) for
// the CPU, its bus, the cache and the panel I/O. p2k_x21 carries requests between them.
//
// CPU address map (MAME emup2k_state::mem_map, [O] unless marked):
//   0x000000-0x1FFFFF, 0x02000000-0x021FFFFF  flash (28F160, p2k_mem)
//   0x00400000 / 0x00408000                    HD44780 instruction / data (byte, D15-8)
//   0x00500000-0x005FFFFF H, 0x00600000-0x0060007F G05, 0x0077F000-0x00780FFF G20 (p2k_sndbus)
//   0x00800000 panel buttons (bits 15-7, active low; bit 7 reads 0 while the CPU runs from the
//              boot flash, MAME's workaround)
//   0x00900000 / 0x00A00000 LED latches (low / high byte), 0x00B00000 control latch (bit 6 Baby)
//   0x00C00000 knob ADC (write: select, read: conversion of the selected input; pot 0, the master
//              volume, is channel 2)
//   0x00F00000 board ID 0x0000
//   0x01000000-0x017FFFFF DRAM 8 MB (p2k_mem)
//   0x50000000-0x53FFFFFF one 32-bit register (the boot data-line test reads back what it wrote)
//   MBAR (0x20000000) on-chip peripherals: in p2k_cpu. Everything else reads 0xFFFF.
module p2k_board #(
    parameter longint SYS_HZ = 88_200_000,
    parameter logic [28:0] SMP_BASE  = 29'h6000000,   // DDR3 64-bit words (from 0x30000000)
    parameter logic [28:0] FX_BASE   = 29'h7000000,
    parameter logic [28:0] BABY_BASE = 29'h7100000,
    parameter logic [28:0] CPU_BASE  = 29'h7200000,   // flash, then DRAM at + 0x40000
    parameter logic [28:0] TRACE_BASE = 29'h7400000,  // debug trace (see below)
    parameter bit          TRACE = 1'b1
) (
    input  logic        clk_sys,
    input  logic        clk_cpu,
    input  logic        rst,              // clk_sys domain
    input  logic        cpu_hold,         // clk_sys: keep the CPU in reset (loading, restoring)

    // panel (clk_sys)
    input  logic [15:7] buttons,          // pressed
    input  logic        enc_step,         // one data-entry encoder step
    input  logic        enc_dir,          // 1 = up
    input  logic [7:0]  master_adc,       // ADC conversion of pot 0, the volume (MAME: adjuster 0-100 x 255 / 100)
    input  logic        filter_on,
    input  logic        fx_on,

    // MIDI (clk_cpu): UART1
    input  logic        midi_rxd,
    output logic        midi_txd,

    // SIMM images loaded (clk_sys), in 16-bit words
    input  logic [24:0] big_words [4],
    input  logic [20:0] baby_words [4],

    // audio (clk_sys): a new sample on audio_strobe
    output logic signed [15:0] audio_l,
    output logic signed [15:0] audio_r,
    output logic        audio_strobe,

    // LCD render port (any clock; combinational, display only)
    input  logic        r_line,
    input  logic [4:0]  r_pos,
    input  logic [2:0]  r_row,
    output logic [7:0]  r_code,
    output logic [4:0]  r_cgbits,
    output logic        r_at_ac,
    output logic        lcd_display_on,
    output logic        lcd_cursor_on,
    output logic        lcd_blink_on,
    output logic [15:0] leds,

    // flash save (clk_sys)
    input  logic        flash_lock,       // the save is reading the flash: flash writes wait
    output logic        flash_changed,    // a program or erase (pulse)

    // a further memory client (clk_sys; loader, save), lowest priority
    input  logic        x_req_valid,
    output logic        x_req_ready,
    input  logic        x_req_we,
    input  logic [28:0] x_req_addr,
    input  logic [63:0] x_req_wdata,
    input  logic [7:0]  x_req_be,
    input  logic [7:0]  x_req_len,
    output logic        x_rsp_valid,
    output logic [63:0] x_rsp_data,

    // status
    output logic        late,             // a sample started late (sticky)
    output logic [31:0] cpu_pc,
    output logic [31:0] cpu_addr,         // address of the last board access (diagnostics)
    output logic        cpu_running,
    output logic        mem_rsp_extra,

    // bench: sound writes as the engine applies them, sample starts
    output logic        dbg_snd_wr,
    output logic [1:0]  dbg_snd_chip,
    output logic [18:0] dbg_snd_addr,
    output logic [15:0] dbg_snd_data,
    output logic [15:0] dbg_snd_mask,
    output logic        dbg_snd_start,

    // DDR3 (clk_sys)
    input  logic        DDRAM_BUSY,
    output logic [7:0]  DDRAM_BURSTCNT,
    output logic [28:0] DDRAM_ADDR,
    input  logic [63:0] DDRAM_DOUT,
    input  logic        DDRAM_DOUT_READY,
    output logic        DDRAM_RD,
    output logic [63:0] DDRAM_DIN,
    output logic [7:0]  DDRAM_BE,
    output logic        DDRAM_WE
);
    // ================================================================== resets
    logic rst_s;                          // sys: rst
    logic hold_s;                         // sys: rst or cpu_hold
    logic rst_c, hold_c;                  // cpu domain
    always_ff @(posedge clk_sys) begin
        rst_s <= rst;
        hold_s <= rst || cpu_hold;
    end
    always_ff @(posedge clk_cpu) begin
        rst_c <= rst_s;
        hold_c <= hold_s;
    end

    // ================================================================== clock enables
    // MCF5206e system clock 33 MHz (peripherals; UART 31250 baud, timer 1 the OS tick) on clk_cpu
    localparam longint CPU_HZ = SYS_HZ / 2;
    logic [31:0] ce_acc;
    logic        ce_sys;
    always_ff @(posedge clk_cpu) begin
        if (ce_acc + 32'd33_000_000 >= 32'(CPU_HZ)) begin
            ce_acc <= ce_acc + 32'd33_000_000 - 32'(CPU_HZ);
            ce_sys <= 1'b1;
        end else begin
            ce_acc <= ce_acc + 32'd33_000_000;
            ce_sys <= 1'b0;
        end
    end
    // 44.1 kHz sample tick on clk_sys
    logic [31:0] tk_acc;
    logic        tick;
    always_ff @(posedge clk_sys) begin
        if (tk_acc + 32'd44_100 >= 32'(SYS_HZ)) begin
            tk_acc <= tk_acc + 32'd44_100 - 32'(SYS_HZ);
            tick <= !rst_s;
        end else begin
            tk_acc <= tk_acc + 32'd44_100;
            tick <= 1'b0;
        end
    end

    // ================================================================== CPU (clk_cpu)
    logic [31:0] a;
    logic [1:0]  bs;
    logic        ext_sel, ext_ack, uds, lds;
    logic [15:0] cpu_dout, ext_din;
    logic [31:0] pc;
    // the CPU leaves reset only when the memory and the sound bus have finished whatever was in
    // flight at the reset (a late completion would answer the new CPU's first access)
    logic        s_wait, s_rsp_valid;
    logic cpu_rst_n, mem_quiet;
    always_ff @(posedge clk_cpu)
        if (hold_c) cpu_rst_n <= 1'b0;
        else if (mem_quiet && !s_wait && !s_rsp_valid) cpu_rst_n <= 1'b1;
    p2k_cpu cpu (
        .clk(clk_cpu), .ce_sys, .reset_n(cpu_rst_n),
        .addr(a), .busstate(bs), .ext_req(), .ext_sel, .ext_ack, .uds, .lds, .dout(cpu_dout), .ext_din,
        .u1_rxd(midi_rxd), .u2_rxd(1'b1), .u1_txd(midi_txd), .u2_txd(),
        .pp_in(8'hff), .pp_out(), .pp_ddr(), .mbar_out(),
        .cpu_ena(1'b1), .ipl_mask(1'b0), .warp_level(3'd0), .ipl_raw(), .vbr_out(),
        .pc_out(pc), .opc_pc(), .kdbg(), .endopc(), .irq_take()
    );
    assign cpu_pc = pc;
    always_ff @(posedge clk_cpu) if (ext_sel) cpu_addr <= a;
    assign cpu_running = !hold_c;
    wire wr = bs == 2'b11;

    // ------------------------------------------------------------------ decode
    wire        d_flash = a[31:21] == 11'h000 || a[31:21] == 11'h010;
    wire        d_dram  = a[31:23] == 9'h002;
    wire        d_io    = a[31:24] == 8'h00 && a[23:22] != 2'b00;
    wire        d_h     = d_io && a[23:20] == 4'h5;
    wire        d_g05   = d_io && a[23:7] == 17'h0C000;
    wire        d_g20   = d_io && a[23:12] >= 12'h77F && a[23:12] <= 12'h780;
    wire        d_snd   = d_h || d_g05 || d_g20;
    wire        d_open  = a[31:26] == 6'b010100;
    wire [23:0] p_byte  = d_flash ? {3'b000, a[20:0]} : 24'h200000 + {1'b0, a[22:0]};
    wire [11:0] g20_off = 12'(a[23:1] - 23'h3BF800);           // (a - 0x77F000) / 2

    // ------------------------------------------------------------------ memory (cache)
    logic        mem_ack;
    logic [63:0] dbg_rsp0, dbg_dq;
    logic [15:0] mem_rdata;
    logic        flash_lock_c, mem_flash_changed;
    logic        m_req_valid, m_req_ready, m_req_we, m_rsp_valid;
    logic [28:0] m_req_addr;
    logic [63:0] m_req_wdata, m_rsp_data;
    logic [7:0]  m_req_be, m_req_len;
    always_ff @(posedge clk_cpu) flash_lock_c <= flash_lock;
    p2k_mem #(.BASE(CPU_BASE)) mem (
        .clk(clk_cpu), .rst(rst_c), .flush(hold_c), .busy_flush(), .quiet(mem_quiet), .rsp_extra(mem_rsp_extra), .dbg_rsp0(dbg_rsp0), .dbg_dq(dbg_dq),
        .go(ext_sel && (d_flash || d_dram)), .we(wr), .flash(d_flash), .pa(p_byte[23:1]),
        .wdata(cpu_dout), .uds, .lds, .ack(mem_ack), .rdata(mem_rdata),
        .flash_lock(flash_lock_c), .flash_changed(mem_flash_changed),
        .req_valid(m_req_valid), .req_ready(m_req_ready), .req_we(m_req_we), .req_addr(m_req_addr),
        .req_wdata(m_req_wdata), .req_be(m_req_be), .req_len(m_req_len), .req_tag(),
        .rsp_valid(m_rsp_valid), .rsp_data(m_rsp_data)
    );
    // flash_changed: held for a clk_sys clock pair, enough for a level in clk_sys
    assign flash_changed = mem_flash_changed;

    // cache line requests to clk_sys
    logic        mb_valid, mb_ready, mb_we;
    logic [28:0] mb_addr;
    logic [63:0] mb_wdata;
    logic [7:0]  mb_be, mb_len;
    logic        mb_rsp_valid;
    logic [63:0] mb_rsp_data;
    p2k_x21 #(.RW(1 + 29 + 64 + 8 + 8), .SW(64), .DEPTH(8)) x_mem (
        .clk_a(clk_cpu), .rst_a(rst_c), .clk_b(clk_sys), .rst_b(rst_s),
        .a_valid(m_req_valid), .a_ready(m_req_ready), .a_data({m_req_we, m_req_addr, m_req_wdata, m_req_be, m_req_len}),
        .b_valid(mb_valid), .b_ready(mb_ready), .b_data({mb_we, mb_addr, mb_wdata, mb_be, mb_len}),
        .b_rsp_valid(mb_rsp_valid), .b_rsp_data(mb_rsp_data),
        .a_rsp_valid(m_rsp_valid), .a_rsp_data(m_rsp_data), .a_rsp_pop(m_rsp_valid)
    );

    // ------------------------------------------------------------------ sound bus requests
    logic [15:0] s_rsp_data;
    wire         s_issue = ext_sel && d_snd && !s_wait;
    logic        s_a_ready;
    wire  [1:0]  s_chip = d_g20 ? 2'd0 : d_h ? 2'd1 : 2'd2;
    wire  [18:0] s_woff = d_g20 ? {7'd0, g20_off} : d_h ? a[19:1] : {13'd0, a[6:1]};
    logic        sb_valid, sb_we, sb_uds, sb_lds;
    logic [1:0]  sb_chip;
    logic [18:0] sb_woff;
    logic [15:0] sb_wdata;
    logic        sb_done;
    logic [15:0] sb_rdata;
    p2k_x21 #(.RW(1 + 2 + 19 + 16 + 2), .SW(16), .DEPTH(2)) x_snd (
        .clk_a(clk_cpu), .rst_a(rst_c), .clk_b(clk_sys), .rst_b(rst_s),
        .a_valid(s_issue), .a_ready(s_a_ready), .a_data({wr, s_chip, s_woff, cpu_dout, uds, lds}),
        .b_valid(sb_valid), .b_ready(1'b1), .b_data({sb_we, sb_chip, sb_woff, sb_wdata, sb_uds, sb_lds}),
        .b_rsp_valid(sb_done), .b_rsp_data(sb_rdata),
        .a_rsp_valid(s_rsp_valid), .a_rsp_data(s_rsp_data), .a_rsp_pop(s_rsp_valid)
    );
    always_ff @(posedge clk_cpu) begin
        if (rst_c) s_wait <= 1'b0;
        else if (s_issue && s_a_ready) s_wait <= 1'b1;
        else if (s_rsp_valid) s_wait <= 1'b0;
    end

    // ------------------------------------------------------------------ panel I/O (zero wait)
    logic [7:0]  led_lo, led_hi, latch, adc_row;
    logic [31:0] open_bus;
    logic [15:7] btn_c;
    logic [7:0]  vol_c;
    always_ff @(posedge clk_cpu) begin
        btn_c <= buttons;
        vol_c <= master_adc;
    end
    wire [7:0]  adc_val = (adc_row[2:0] == 3'd2) ? vol_c : 8'd0;
    wire        a_lcd_i = a[31:1] == 31'h00200000;              // 0x400000
    wire        a_lcd_d = a[31:1] == 31'h00204000;              // 0x408000
    wire        io_strobe = ext_sel && d_io && !d_snd;
    logic [7:0] lcd_dout;
    logic [15:0] io_rdata;
    always_comb begin
        io_rdata = 16'hffff;
        if (d_open) io_rdata = a[1] ? open_bus[15:0] : open_bus[31:16];
        else if (a_lcd_i || a_lcd_d) io_rdata = {lcd_dout, 8'hff};
        else if (a[31:1] == 31'h00400000) begin                  // 0x800000 panel
            io_rdata = {~btn_c, 7'h7f};
            if (pc >= 32'h02000000) io_rdata[7] = 1'b0;
        end else if (a[31:1] == 31'h00600000) io_rdata = {adc_val, 8'hff};
        else if (a[31:1] == 31'h00780000) io_rdata = 16'h0000;   // 0xF00000 board ID
    end
    always_ff @(posedge clk_cpu) begin
        if (rst_c) begin
            led_lo <= '0; led_hi <= '0; latch <= '0; adc_row <= '0;
        end else if (io_strobe && wr) begin
            if (a[31:1] == 31'h00480000 && uds) led_lo <= cpu_dout[15:8];
            if (a[31:1] == 31'h00500000 && uds) led_hi <= cpu_dout[15:8];
            if (a[31:1] == 31'h00580000 && uds) latch <= cpu_dout[15:8];
            if (a[31:1] == 31'h00600000 && uds) adc_row <= cpu_dout[15:8];
        end
        if (ext_sel && d_open && wr) begin
            if (a[1]) open_bus[15:0] <= cpu_dout;
            else open_bus[31:16] <= cpu_dout;
        end
    end
    assign leds = {led_hi, led_lo};

    // HD44780 (MAME: 270 kHz; busy 37 us / 1.52 ms)
    synthframe_hd44780 #(.POS_W(5), .BUSY_EXT(0),
                         .BUSY_SHORT(int'(CPU_HZ * 37 / 1_000_000)),
                         .BUSY_LONG(int'(CPU_HZ * 1518 / 1_000_000))) lcd (
        .clk(clk_cpu), .reset(hold_c),
        .cs(1'b1), .wr(ext_sel && (a_lcd_i || a_lcd_d) && wr && uds), .rd(ext_sel && a_lcd_d && !wr),
        .rs(a_lcd_d), .din(cpu_dout[15:8]), .dout(lcd_dout),
        .busy_t_set(32'd0), .busy_t_read(32'd0),
        .r_line, .r_pos, .r_row, .r_code, .r_cgbits, .r_is_cg(), .r_at_ac, .r_cursor(),
        .display_on(lcd_display_on), .cursor_on(lcd_cursor_on), .blink_on(lcd_blink_on));

    // ------------------------------------------------------------------ CPU data in, wait states
    always_comb begin
        if (d_flash || d_dram) begin
            ext_ack = mem_ack;
            ext_din = mem_rdata;
        end else if (d_snd) begin
            ext_ack = s_rsp_valid;
            ext_din = s_rsp_data;
        end else begin
            ext_ack = 1'b1;
            ext_din = io_rdata;
        end
    end

    // ================================================================== clk_sys side
    // control latch bit 6 and the knob/encoder state the sound bus needs
    logic baby_sel_s;
    always_ff @(posedge clk_sys) baby_sel_s <= latch[6];
    logic [7:0] enc_count;
    logic       enc_take;
    always_ff @(posedge clk_sys) begin
        if (rst_s) enc_count <= '0;
        else if (enc_take) enc_count <= enc_step ? (enc_dir ? 8'd1 : 8'hff) : 8'd0;
        else if (enc_step) enc_count <= enc_dir ? enc_count + 8'd1 : enc_count - 8'd1;
    end

    // ------------------------------------------------------------------ sound bus and engine
    logic        snd_idle, snd_start, snd_wr_valid, snd_rd_valid, snd_rd_chip, snd_rd_done, snd_rd_vsel;
    logic [1:0]  snd_wr_chip;
    logic [18:0] snd_wr_addr;
    logic [15:0] snd_wr_data, snd_wr_mask;
    logic [7:0]  snd_rd_addr;
    logic [25:0] snd_rd_vaddr;
    logic [15:0] snd_rd_ipx [0:2];
    logic        p_req_valid, p_req_ready, p_rsp_valid;
    logic [28:0] p_req_addr;
    logic [63:0] p_rsp_data;
    p2k_sndbus #(.SMP_BASE(SMP_BASE), .BABY_BASE(BABY_BASE)) sbus (
        .clk(clk_sys), .rst(rst_s),
        .go(sb_valid), .we(sb_we), .chip(sb_chip), .woff(sb_woff), .wdata(sb_wdata), .uds(sb_uds), .lds(sb_lds),
        .done(sb_done), .rdata(sb_rdata),
        .baby_sel(baby_sel_s), .enc_count, .enc_take, .big_words, .baby_words,
        .tick, .late,
        .snd_idle, .snd_start, .snd_wr_valid, .snd_wr_chip, .snd_wr_addr, .snd_wr_data, .snd_wr_mask,
        .snd_rd_valid, .snd_rd_chip, .snd_rd_addr, .snd_rd_done, .snd_rd_vaddr, .snd_rd_vsel, .snd_rd_ipx,
        .req_valid(p_req_valid), .req_ready(p_req_ready), .req_addr(p_req_addr),
        .rsp_valid(p_rsp_valid), .rsp_data(p_rsp_data),
        .fifo_level()
    );

    logic        e_req_valid, e_req_ready, e_req_we, e_rsp_valid;
    logic [28:0] e_req_addr;
    logic [63:0] e_req_wdata;
    logic [7:0]  e_req_be, e_req_len;
    logic [1:0]  e_req_tag;
    logic        snd_done, snd_wr_ready;
    logic signed [15:0] out_l, out_r;
    logic [1:0]  rsp_ctag;
    logic [63:0] rsp_cdata;
    p2k_snd #(.SMP_BASE(SMP_BASE), .FX_BASE(FX_BASE)) snd (
        .clk(clk_sys), .rst(rst_s), .filter_on, .fx_on,
        .wr_valid(snd_wr_valid), .wr_ready(snd_wr_ready), .wr_chip(snd_wr_chip), .wr_addr(snd_wr_addr),
        .wr_data(snd_wr_data), .wr_mask(snd_wr_mask),
        .idle(snd_idle), .rd_valid(snd_rd_valid), .rd_ready(), .rd_chip(snd_rd_chip), .rd_addr(snd_rd_addr),
        .rd_done(snd_rd_done), .rd_vaddr(snd_rd_vaddr), .rd_vsel(snd_rd_vsel), .rd_ipx(snd_rd_ipx),
        .start(snd_start), .done(snd_done), .out_l, .out_r,
        .mem_req_valid(e_req_valid), .mem_req_ready(e_req_ready), .mem_req_we(e_req_we), .mem_req_addr(e_req_addr),
        .mem_req_wdata(e_req_wdata), .mem_req_be(e_req_be), .mem_req_len(e_req_len), .mem_req_tag(e_req_tag),
        .mem_rsp_valid(e_rsp_valid), .mem_rsp_data(rsp_cdata), .mem_rsp_tag(rsp_ctag),
        .stat_div_cycles(), .stat_chained()
    );
    always_ff @(posedge clk_sys) begin
        audio_strobe <= snd_done;
        if (snd_done) begin
            audio_l <= out_l;
            audio_r <= out_r;
        end
    end
    assign dbg_snd_wr = snd_wr_valid && snd_idle && !snd_start;
    assign dbg_snd_chip = snd_wr_chip;
    assign dbg_snd_addr = snd_wr_addr;
    assign dbg_snd_data = snd_wr_data;
    assign dbg_snd_mask = snd_wr_mask;
    assign dbg_snd_start = snd_start;

    // ------------------------------------------------------------------ debug trace (TRACE)
    // The last 1024 CPU board accesses {PC, address, data, busstate, uds, lds} in a ring; it stops
    // 16 accesses after the PC leaves flash and DRAM (a runaway) and is then written to DDR3 at
    // TRACE_BASE: word 2i = {PC, address}, word 2i + 1 = {count, data, 12'd0, bs, uds, lds}, oldest
    // first, then a marker word 0x5452414345000000 | entries written (2048 words in).
    logic        tr_valid, tr_ready;
    logic [28:0] tr_addr;
    logic [63:0] tr_wdata;
    generate if (TRACE) begin : g_trace
        logic [127:0] ring [0:1023];
        logic [9:0]   tw;
        logic [4:0]   after;
        logic         frozen, stopping;
        logic [31:0]  cnt;
        wire          pc_ok = pc[31:21] == 11'h000 || pc[31:21] == 11'h010 || pc[31:23] == 9'h002;
        always_ff @(posedge clk_cpu) begin
            if (hold_c) begin
                tw <= '0; after <= '0; frozen <= 1'b0; stopping <= 1'b0; cnt <= '0;
            end else if (!frozen && ext_sel && ext_ack) begin
                ring[tw] <= {pc, a, cnt[15:0], wr ? cpu_dout : ext_din, 12'd0, bs, uds, lds, 16'd0};
                tw <= tw + 10'd1;
                cnt <= cnt + 32'd1;
                if (stopping) begin
                    after <= after + 5'd1;
                    if (after == 5'd15) frozen <= 1'b1;
                end
            end
            if (!pc_ok && !hold_c) stopping <= 1'b1;
        end
        // clk_sys: write the frozen ring out (the ring no longer changes)
        logic        fz_s, done;
        logic [1:0]  settle;
        logic [11:0] n;
        logic [127:0] e;
        always_ff @(posedge clk_sys) begin
            fz_s <= frozen;
            e <= ring[10'(tw + n[11:1])];
            if (rst_s || !fz_s) begin
                n <= '0; done <= 1'b0; tr_valid <= 1'b0; settle <= 2'd3;
            end else if (!done) begin
                if (settle != 0) settle <= settle - 2'd1;     // e follows n
                else if (!tr_valid) begin
                    tr_valid <= 1'b1;
                    tr_addr <= TRACE_BASE + 29'(n);
                    tr_wdata <= n[11] ? (n[1:0] == 2'd0 ? 64'h5452414345000000 | 64'(cnt) : n[1:0] == 2'd1 ? dbg_rsp0 : dbg_dq)
                                      : (n[0] ? e[63:0] : e[127:64]);
                end else if (tr_ready) begin
                    tr_valid <= 1'b0;
                    n <= n + 12'd1;
                    settle <= 2'd2;
                    if (n[11] && n[1:0] == 2'd2) done <= 1'b1;
                end
            end
        end
    end else begin : g_notrace
        assign tr_valid = 1'b0;
        assign tr_addr = '0;
        assign tr_wdata = '0;
    end endgenerate
    assign tr_ready = c_req_ready[4];

    // ------------------------------------------------------------------ DDR3: engine, ports, CPU, loader
    logic        c_req_valid [5], c_req_ready [5], c_req_we [5], c_rsp_valid [5];
    logic [28:0] c_req_addr [5];
    logic [63:0] c_req_wdata [5];
    logic [7:0]  c_req_be [5], c_req_len [5];
    logic [1:0]  c_req_tag [5];
    always_comb begin
        c_req_valid[0] = e_req_valid; c_req_we[0] = e_req_we; c_req_addr[0] = e_req_addr;
        c_req_wdata[0] = e_req_wdata; c_req_be[0] = e_req_be; c_req_len[0] = e_req_len; c_req_tag[0] = e_req_tag;
        c_req_valid[1] = p_req_valid; c_req_we[1] = 1'b0; c_req_addr[1] = p_req_addr;
        c_req_wdata[1] = '0; c_req_be[1] = 8'hff; c_req_len[1] = 8'd1; c_req_tag[1] = 2'd0;
        c_req_valid[2] = mb_valid; c_req_we[2] = mb_we; c_req_addr[2] = mb_addr;
        c_req_wdata[2] = mb_wdata; c_req_be[2] = mb_be; c_req_len[2] = mb_len; c_req_tag[2] = 2'd0;
        c_req_valid[3] = x_req_valid; c_req_we[3] = x_req_we; c_req_addr[3] = x_req_addr;
        c_req_wdata[3] = x_req_wdata; c_req_be[3] = x_req_be; c_req_len[3] = x_req_len; c_req_tag[3] = 2'd0;
        c_req_valid[4] = tr_valid; c_req_we[4] = 1'b1; c_req_addr[4] = tr_addr;
        c_req_wdata[4] = tr_wdata; c_req_be[4] = 8'hff; c_req_len[4] = 8'd1; c_req_tag[4] = 2'd0;
    end
    assign e_req_ready = c_req_ready[0];
    assign p_req_ready = c_req_ready[1];
    assign mb_ready = c_req_ready[2];
    assign x_req_ready = c_req_ready[3];
    assign e_rsp_valid = c_rsp_valid[0];
    assign p_rsp_valid = c_rsp_valid[1];
    assign p_rsp_data = rsp_cdata;
    assign mb_rsp_valid = c_rsp_valid[2];
    assign mb_rsp_data = rsp_cdata;
    assign x_rsp_valid = c_rsp_valid[3];
    assign x_rsp_data = rsp_cdata;

    logic        req_valid, req_ready, req_we, rsp_valid;
    logic [28:0] req_addr;
    logic [63:0] req_wdata, rsp_data;
    logic [7:0]  req_be, req_len;
    logic [4:0]  req_tag, rsp_tag;
    synthframe_mem_arb #(.N(5), .CTW(2)) arb (
        .c_req_valid, .c_req_ready, .c_req_we, .c_req_addr, .c_req_wdata, .c_req_be, .c_req_len,
        .c_req_tag, .c_rsp_valid, .c_rsp_data(rsp_cdata), .c_rsp_tag(rsp_ctag),
        .req_valid, .req_ready, .req_we, .req_addr, .req_wdata, .req_be, .req_len, .req_tag,
        .rsp_valid, .rsp_data, .rsp_tag
    );
    synthframe_mem_ddr3 #(.TW(5), .QBITS(4)) ddr (
        .clk(clk_sys), .rst(rst_s),
        .req_valid, .req_ready, .req_we, .req_addr, .req_wdata, .req_be, .req_len, .req_tag,
        .rsp_valid, .rsp_data, .rsp_tag,
        .DDRAM_BUSY, .DDRAM_BURSTCNT, .DDRAM_ADDR, .DDRAM_DOUT, .DDRAM_DOUT_READY, .DDRAM_RD,
        .DDRAM_DIN, .DDRAM_BE, .DDRAM_WE
    );
endmodule
