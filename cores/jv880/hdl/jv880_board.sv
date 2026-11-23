// SPDX-License-Identifier: GPL-2.0-or-later
// JV-880 core with its board services, independent of MiSTer's framework: ROM pack download
// (with the NVRAM image), wave ROMs in DDR3, real-time pacing, MIDI in from a UART, the panel
// (14 buttons, the encoder), audio to 16 bit at 48 kHz, the LCD on video. mister/JV880.sv adds
// hps_io, the PLL, video output; sim/jv880_board_bench.cpp drives it against the reference.
//
// The Roland parts are modules/roland (the SC-55's H8/532, MCU and PCM with the jv880 model).
// The core is held in reset until a JV-880 pack (rom_loader model 4) has loaded, and again
// during any later download; then the PCM clears its eram, the MCU takes its reset vector, and
// steps run while the pacer allows (and step_gate, for the bench).
module jv880_board #(
    parameter longint CLK_HZ = 50_000_000,
    parameter logic [31:0] WAVE_BASE = 32'h3000_0000
) (
    input  logic        clk,
    input  logic        clk_vid,          // 40 MHz: 800 x 600 video
    input  logic        rst,              // power-on / hard reset
    input  logic        user_reset,       // OSD reset: re-run the MCU reset

    input  logic        ioctl_download,
    input  logic        ioctl_wr,
    input  logic [26:0] ioctl_addr,
    input  logic [7:0]  ioctl_dout,
    output logic        ioctl_wait,

    input  logic        uart_rxd,
    input  logic        midi_inject,      // bench: a MIDI byte straight into the receive buffer
    input  logic [7:0]  midi_inject_data,
    input  logic [13:0] buttons,          // MCU_BUTTON_CURSOR_L (bit 0) .. MCU_BUTTON_RHYTHM (13)
    input  logic        enc_we,           // encoder detent (one clock)
    input  logic        enc_dir,          // 1: up (MCU_EncoderTrigger dir 1)

    input  logic        pace_enable,
    input  logic [1:0]  gain,             // output gain: 0, +6, +12, +18 dB (after the PCM, saturating)
    input  logic        step_gate,        // 1 on hardware; the bench opens it one step at a time

    output logic signed [15:0] audio_l,
    output logic signed [15:0] audio_r,
    output logic        sample_valid,     // the PCM's raw output (bench)
    output logic signed [31:0] sample_l,
    output logic signed [31:0] sample_r,

    // video (clk_vid)
    output logic        vid_hs,
    output logic        vid_vs,
    output logic        vid_hblank,
    output logic        vid_vblank,
    output logic [7:0]  vid_r,
    output logic [7:0]  vid_g,
    output logic [7:0]  vid_b,
    // LCD state (bench)
    output logic [7:0]  lcd_dd [80],

    // status
    output logic        rom_ok,
    output logic        running,
    output logic        step_done,
    output logic        midi_seen,        // a MIDI byte arrived (LED)
    output logic        midi_alt,         // receiving at 9600 baud
    output logic [31:0] lag_us,
    output logic [15:0] underruns,
    output logic [15:0] overruns,
    output logic [15:0] dbg_pc,
    output logic [7:0]  dbg_cp,

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
    // ---------------------------------------------------------- ROM load
    logic        ld_we, wave_wr, wave_busy, loading, pack_ok, ok;
    logic [2:0]  ld_sel, model;
    logic [18:0] ld_addr;
    logic [7:0]  ld_data, wave_data;
    logic [22:0] wave_addr;

    rom_loader loader (
        .clk, .rst,
        .ioctl_download, .ioctl_wr, .ioctl_addr, .ioctl_dout, .ioctl_wait,
        .ld_we, .ld_sel, .ld_addr, .ld_data,
        .wave_wr, .wave_addr, .wave_data, .wave_busy, .model,
        .loading, .ok(pack_ok)
    );
    assign ok = pack_ok && model == 3'd4;      // a JV-880 pack
    assign rom_ok = ok;

    // ---------------------------------------------------------- sequencing
    typedef enum logic [2:0] { B_HOLD, B_CLEAR, B_RESET, B_RUN } bst_t;
    bst_t bst;
    logic core_rst, reset_start, reset_done, core_ready, run_step;

    always_ff @(posedge clk) begin
        reset_start <= 1'b0;
        if (rst || loading || !ok || user_reset) begin
            bst <= B_HOLD;
            core_rst <= 1'b1;
        end else begin
            case (bst)
                B_HOLD:  begin core_rst <= 1'b0; bst <= B_CLEAR; end
                B_CLEAR: if (core_ready) begin reset_start <= 1'b1; bst <= B_RESET; end
                B_RESET: if (reset_done) bst <= B_RUN;
                default: ;
            endcase
        end
    end
    assign running = (bst == B_RUN);

    // ---------------------------------------------------------- pacing (600 ns a step, R_NSPerStep)
    logic allow;
    step_pacer #(.CLK_HZ(CLK_HZ), .STEP_NS(600), .STEP_NS_FAST(600)) pacer (
        .clk, .rst(core_rst), .enable(pace_enable), .fast(1'b0), .step_done, .allow, .lag_us
    );
    assign run_step = running && allow && step_gate;

    // ---------------------------------------------------------- MIDI
    logic       rx_valid;
    logic [7:0] rx_data;
    synthframe_midi_rx #(.CLK_HZ(CLK_HZ)) midi (
        .clk, .rst, .rxd(uart_rxd), .byte_valid(rx_valid), .byte_data(rx_data), .frame_err(), .alt(midi_alt)
    );
    wire       m_valid = rx_valid || midi_inject;
    wire [7:0] m_data  = midi_inject ? midi_inject_data : rx_data;
    always_ff @(posedge clk) midi_seen <= m_valid;

    // ---------------------------------------------------------- panel: encoder detents (registered)
    logic enc_go, enc_pdir;
    always_ff @(posedge clk) begin
        enc_go <= enc_we && running;
        enc_pdir <= enc_dir;
    end

    // ---------------------------------------------------------- core
    logic        rom_req, rom_ready, rom_rvalid;
    logic [21:0] rom_addr;
    logic [7:0]  rom_data;

    sc55_core #(.ROM2_AW(18), .JV880_RAMS(1'b1)) core (
        .clk, .rst(core_rst),
        .run_step, .step_done, .reset_start, .reset_done, .ready(core_ready),
        .ld_we, .ld_sel, .ld_addr, .ld_data, .mk2(1'b0), .sc155(1'b0), .jv880(1'b1),
        .enc_we(enc_go), .enc_dir(enc_pdir),
        .rom2x_req(), .rom2x_addr(), .rom2x_ack(1'b0), .rom2x_data(8'h00),
        .midi_valid(m_valid && running), .midi_data(m_data), .midi_full(), .buttons({18'h0, buttons}),
        .rom_req, .rom_addr, .rom_ready, .rom_data, .rom_rvalid,
        .enable_oversampling(1'b1),
        .sample_valid, .sample_l, .sample_r,
        .lcd_we(l_we), .lcd_rs(l_rs), .lcd_data(l_data), .lcd_enable(l_en),
        .cycles(), .dbg_pc, .dbg_cp, .dbg_sr()
    );

    // ---------------------------------------------------------- LCD
    logic       l_we, l_rs, l_en, lcd_on, c_flag;
    logic [7:0] l_data;
    logic [4:0] cg [64];
    logic [6:0] dd_ram;
    lcd_ctrl lcd (
        .clk, .rst(core_rst), .we(l_we), .rs(l_rs), .data(l_data), .enable_in(l_en),
        .dd(lcd_dd), .cg, .enable(lcd_on), .dd_ram, .c_flag
    );

    // status strip: MIDI and underrun flashes; underrun since the core started (sticky); real-time
    // lag that grew after the first second of running (sticky)
    logic [23:0] midi_stretch, ur_stretch;
    logic [15:0] last_ur;
    logic [25:0] run_time;
    logic [31:0] lag_base;
    logic        lag_seen;
    always_ff @(posedge clk) begin
        midi_stretch <= midi_seen ? 24'hffffff : (midi_stretch != 0 ? midi_stretch - 24'd1 : 24'd0);
        last_ur <= underruns;
        ur_stretch <= (underruns != last_ur) ? 24'hffffff : (ur_stretch != 0 ? ur_stretch - 24'd1 : 24'd0);
        if (!running) begin
            run_time <= '0;
            lag_seen <= 1'b0;
        end else if (run_time != 26'(CLK_HZ)) begin
            run_time <= run_time + 26'd1;
            lag_base <= lag_us;
        end else if (lag_us != lag_base) lag_seen <= 1'b1;
    end
    jv880_lcd_video video (
        .clk(clk_vid), .dd(lcd_dd), .cg, .dd_ram, .cursor_on(c_flag),
        .rom_ok, .loading, .running, .midi_led(midi_stretch != 0), .underrun_led(ur_stretch != 0),
        .underrun_seen(underruns != 16'd0), .lag_seen,
        .hs(vid_hs), .vs(vid_vs), .hblank(vid_hblank), .vblank(vid_vblank), .r(vid_r), .g(vid_g), .b(vid_b)
    );

    // ---------------------------------------------------------- wave ROMs (DDR3)
    wave_ddr #(.BASE(WAVE_BASE)) wave (
        .clk, .rst,
        .wr(wave_wr), .waddr(wave_addr), .wdata(wave_data), .wbusy(wave_busy),
        .req(rom_req), .addr(rom_addr), .ready(rom_ready), .rvalid(rom_rvalid), .rdata(rom_data),
        .lreq(1'b0), .laddr(15'd0), .lvalid(), .ldata(),
        .DDRAM_BUSY, .DDRAM_BURSTCNT, .DDRAM_ADDR, .DDRAM_DOUT, .DDRAM_DOUT_READY,
        .DDRAM_RD, .DDRAM_DIN, .DDRAM_BE, .DDRAM_WE
    );

    // ---------------------------------------------------------- audio
    // the renderer's s16 conversion (clamp(x >> 15)) with `gain` in 6 dB steps, 64 -> 48 kHz
    // (resample.sv, the mk1 rate), then a FIFO that plays at 48 kHz in real time
    function automatic logic signed [15:0] to16(input logic signed [31:0] v, input logic [1:0] g);
        logic signed [31:0] s;
        s = v >>> (5'd15 - {3'd0, g});
        if (s > 32'sd32767) return 16'sh7fff;
        if (s < -32'sd32768) return 16'sh8000;
        return s[15:0];
    endfunction

    logic        rs_valid;
    logic signed [15:0] rs_l, rs_r;
    resample rs (
        .clk, .rst(core_rst), .mk2(1'b0),
        .in_valid(sample_valid), .in_l(to16(sample_l, gain)), .in_r(to16(sample_r, gain)),
        .out_valid(rs_valid), .out_l(rs_l), .out_r(rs_r)
    );

    synthframe_audio_fifo #(.CLK_HZ(CLK_HZ), .RATE(48_000), .DEPTH_BITS(8)) fifo (
        .clk, .rst(core_rst),
        .in_valid(rs_valid), .in_l(rs_l), .in_r(rs_r),
        .out_l(audio_l), .out_r(audio_r), .out_strobe(),
        .underruns, .overruns
    );
endmodule
