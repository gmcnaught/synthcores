// SC-55 core with its board services, independent of MiSTer's framework:
// ROM pack download, wave ROMs in DDR3, real-time pacing, MIDI in from a
// UART, audio to 16 bit at the output rate. mister/SC55.sv adds hps_io, the
// PLL, video; sim/board/board_lockstep.cpp drives it against the reference.
//
// The core is held in reset until a pack with a valid header has loaded,
// and again during any later download; then the PCM clears its eram, the
// MCU takes its reset vector, and steps run while the pacer allows (and
// step_gate, for the bench). The services it shares with the JV-880 are
// modules/roland/hdl/board/roland_board_common.sv.
module sc55_board #(
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
    output logic        uart_txd,         // MIDI out (synthframe_midi_tx, at the receiver's baud)
    input  logic        midi_inject,      // bench: a MIDI byte straight into the receive buffer
    input  logic [7:0]  midi_inject_data,
    input  logic [31:0] buttons,

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
    output logic        lcd_pix,          // bench: an LCD pixel at (lcd_x, lcd_y) is on vid_r/g/b
    output logic [9:0]  lcd_x,
    output logic [8:0]  lcd_y,

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
    // ---------------------------------------------------------- shared board services
    // (modules/roland/hdl/board/roland_board_common.sv): pack load, DDR3, reset sequence, pacing,
    // MIDI, status flags, audio
    logic        ld_we, ok, pack_ok, loading;
    logic [2:0]  ld_sel;
    logic [18:0] ld_addr;
    logic [7:0]  ld_data;
    logic [2:0]  model;                   // from the pack: 0 mk1, 1 SC-155, 2 mk2, 3 SC-155mk2 (4 JV-880: not this core)
    wire         mk2 = model[1];
    wire         sc155 = model[0];
    logic        core_rst, reset_start, reset_done, core_ready, run_step;
    logic        m_valid, tx_valid;
    logic [7:0]  m_data, tx_data;
    logic        midi_led, underrun_led, lag_seen;
    logic        rom_req, rom_ready, rom_rvalid;
    logic [21:0] rom_addr;
    logic [7:0]  rom_data;
    logic        ln_req, ln_valid;
    logic [14:0] ln_addr;
    logic [63:0] ln_data;

    assign ok = pack_ok && !model[2];
    assign rom_ok = ok;

    // the renderer's s16 conversion (Normalize: clamp(x >> 15)), with `gain` in 6 dB steps (like
    // the reference front ends' --gain); the pacer at 500 ns a step for the mk2
    roland_board_common #(.CLK_HZ(CLK_HZ), .WAVE_BASE(WAVE_BASE), .TO16_SHIFT(15)) common (
        .clk, .rst, .user_reset,
        .ioctl_download, .ioctl_wr, .ioctl_addr, .ioctl_dout, .ioctl_wait,
        .ld_we, .ld_sel, .ld_addr, .ld_data, .model, .pack_ok, .loading, .ok,
        .hold(1'b0), .pause(1'b0), .pace_enable, .fast(mk2), .step_gate,
        .core_rst, .reset_start, .run_step, .running, .lag_us, .core_ready, .reset_done, .step_done,
        .uart_rxd, .uart_txd, .midi_inject, .midi_inject_data, .midi_alt, .midi_seen,
        .midi_valid(m_valid), .midi_data(m_data), .tx_valid, .tx_data,
        .midi_led, .underrun_led, .lag_seen,
        .rom_req, .rom_addr, .rom_ready, .rom_rvalid, .rom_data,
        .lreq(ln_req), .laddr(ln_addr), .lvalid(ln_valid), .ldata(ln_data),
        .mk2, .gain, .sample_valid, .sample_l, .sample_r, .audio_l, .audio_r, .underruns, .overruns,
        .DDRAM_BUSY, .DDRAM_BURSTCNT, .DDRAM_ADDR, .DDRAM_DOUT, .DDRAM_DOUT_READY,
        .DDRAM_RD, .DDRAM_DIN, .DDRAM_BE, .DDRAM_WE
    );

    // ---------------------------------------------------------- core
    sc55_core core (
        .clk, .rst(core_rst),
        .run_step, .step_done, .reset_start, .reset_done, .ready(core_ready),
        .ld_we, .ld_sel, .ld_addr, .ld_data, .mk2, .sc155, .jv880(1'b0), .enc_we(1'b0), .enc_dir(1'b0),
        .nv_dump_a(15'd0), .nv_dump_d(), .nv_wrote(), .idle(),
        .rom2x_req, .rom2x_addr, .rom2x_ack, .rom2x_data,
        .midi_valid(m_valid), .midi_data(m_data), .midi_full(), .tx_valid, .tx_data, .buttons, .jv_leds(),
        .rom_req, .rom_addr, .rom_ready, .rom_data, .rom_rvalid,
        .enable_oversampling(1'b1),
        .sample_valid, .sample_l, .sample_r,
        .lcd_we(l_we), .lcd_rs(l_rs), .lcd_data(l_data), .lcd_enable(l_en),
        .cycles(), .dbg_pc, .dbg_cp, .dbg_sr()
    );

    // ---------------------------------------------------------- LCD
    logic       l_we, l_rs, l_en, lcd_on;
    logic [7:0] l_data;
    logic [7:0] dd [80];
    logic [4:0] cg [64];
    lcd_ctrl lcd (
        .clk, .rst(core_rst), .we(l_we), .rs(l_rs), .data(l_data), .enable_in(l_en),
        .dd, .cg, .enable(lcd_on), .dd_ram(), .c_flag()
    );

    // the status strip under the LCD (shown only): MIDI and underrun flashes, underrun since the
    // core started, real-time lag after the first second (roland_board_common)
    lcd_video video (
        .clk(clk_vid), .dd, .cg, .enable(lcd_on), .buttons,
        .rom_ok, .loading, .running, .midi_led, .underrun_led,
        .underrun_seen(underruns != 16'd0), .lag_seen,
        .hs(vid_hs), .vs(vid_vs), .hblank(vid_hblank), .vblank(vid_vblank), .r(vid_r), .g(vid_g), .b(vid_b),
        .lcd_pix, .lcd_x, .lcd_y
    );

    // ROM2's upper half (mk2): DDR3 (roland_board_common's line port) through a small cache
    logic        rom2x_req, rom2x_ack;
    logic [17:0] rom2x_addr;
    logic [7:0]  rom2x_data;
    rom2x_cache cache (
        .clk, .flush(rst || loading), .req(rom2x_req), .addr(rom2x_addr), .ack(rom2x_ack), .data(rom2x_data),
        .lreq(ln_req), .laddr(ln_addr), .lvalid(ln_valid), .ldata(ln_data)
    );
endmodule
