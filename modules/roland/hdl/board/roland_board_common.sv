// SPDX-License-Identifier: GPL-2.0-or-later
// The board services the SC-55 and JV-880 boards share (cores/sc55/hdl/board/sc55_board.sv,
// cores/jv880/hdl/jv880_board.sv): the ROM pack download, the wave ROMs in DDR3, the core's reset
// sequence, real-time pacing, MIDI in and out, the status strip's flags, and the audio chain to
// 16 bit at 48 kHz. Each board adds its core, LCD, video and own parts (the SC-55's ROM2 cache,
// the JV-880's NVRAM save file and panel).
//
// Sequence: the core is held in reset (core_rst) after rst or user_reset, during a download and
// until the board accepts the pack (ok), and while `hold`; then the PCM clears its eram
// (core_ready), the MCU takes its reset vector (reset_start .. reset_done), and steps run
// (run_step) while the pacer allows, step_gate is open and `pause` is low.
module roland_board_common #(
    parameter longint CLK_HZ = 50_000_000,
    parameter logic [31:0] WAVE_BASE = 32'h3000_0000,
    parameter int TO16_SHIFT = 15         // s16 = clamp(sample >>> (TO16_SHIFT - gain))
) (
    input  logic        clk,
    input  logic        rst,              // power-on / hard reset
    input  logic        user_reset,       // OSD reset: re-run the MCU reset

    // ROM pack download (rom_loader)
    input  logic        ioctl_download,
    input  logic        ioctl_wr,
    input  logic [26:0] ioctl_addr,
    input  logic [7:0]  ioctl_dout,
    output logic        ioctl_wait,
    output logic        ld_we,
    output logic [2:0]  ld_sel,
    output logic [18:0] ld_addr,
    output logic [7:0]  ld_data,
    output logic [2:0]  model,            // from the pack header
    output logic        pack_ok,          // a pack with a known header loaded completely
    output logic        loading,
    input  logic        ok,               // the board accepts the pack (its model)

    // sequencing and pacing
    input  logic        hold,             // keep the core in reset after the load
    input  logic        pause,            // no steps
    input  logic        pace_enable,
    input  logic        fast,             // the pacer's STEP_NS_FAST (500 ns) a step, not 600
    input  logic        step_gate,
    output logic        core_rst,
    output logic        reset_start,
    output logic        run_step,
    output logic        running,
    output logic [31:0] lag_us,
    input  logic        core_ready,
    input  logic        reset_done,
    input  logic        step_done,

    // MIDI
    input  logic        uart_rxd,
    output logic        uart_txd,
    input  logic        midi_inject,
    input  logic [7:0]  midi_inject_data,
    output logic        midi_alt,
    output logic        midi_seen,
    output logic        midi_valid,       // to the core: a byte, while running
    output logic [7:0]  midi_data,
    input  logic        tx_valid,
    input  logic [7:0]  tx_data,

    // status strip flags
    output logic        midi_led,
    output logic        underrun_led,
    output logic        lag_seen,

    // wave ROMs (wave_ddr): the PCM's byte port and the 64-bit line port
    input  logic        rom_req,
    input  logic [21:0] rom_addr,
    output logic        rom_ready,
    output logic        rom_rvalid,
    output logic [7:0]  rom_data,
    input  logic        lreq,
    input  logic [14:0] laddr,
    output logic        lvalid,
    output logic [63:0] ldata,

    // audio
    input  logic        mk2,              // resample from the mk2's rate
    input  logic [1:0]  gain,             // 0, +6, +12, +18 dB
    input  logic        sample_valid,
    input  logic signed [31:0] sample_l,
    input  logic signed [31:0] sample_r,
    output logic signed [15:0] audio_l,
    output logic signed [15:0] audio_r,
    output logic [15:0] underruns,
    output logic [15:0] overruns,

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
    logic        wave_wr, wave_busy;
    logic [7:0]  wave_data;
    logic [22:0] wave_addr;

    rom_loader loader (
        .clk, .rst,
        .ioctl_download, .ioctl_wr, .ioctl_addr, .ioctl_dout, .ioctl_wait,
        .ld_we, .ld_sel, .ld_addr, .ld_data,
        .wave_wr, .wave_addr, .wave_data, .wave_busy, .model,
        .loading, .ok(pack_ok)
    );

    // ---------------------------------------------------------- sequencing
    typedef enum logic [2:0] { B_HOLD, B_CLEAR, B_RESET, B_RUN } bst_t;
    bst_t bst;

    always_ff @(posedge clk) begin
        reset_start <= 1'b0;
        if (rst || loading || !ok || user_reset) begin
            bst <= B_HOLD;
            core_rst <= 1'b1;
        end else begin
            case (bst)
                B_HOLD:  if (!hold) begin core_rst <= 1'b0; bst <= B_CLEAR; end
                B_CLEAR: if (core_ready) begin reset_start <= 1'b1; bst <= B_RESET; end
                B_RESET: if (reset_done) bst <= B_RUN;
                default: ;
            endcase
        end
    end
    assign running = (bst == B_RUN);

    // ---------------------------------------------------------- pacing
    logic allow;
    step_pacer #(.CLK_HZ(CLK_HZ), .STEP_NS(600), .STEP_NS_FAST(500)) pacer (
        .clk, .rst(core_rst), .enable(pace_enable), .fast, .step_done, .allow, .lag_us
    );
    assign run_step = running && allow && step_gate && !pause;

    // ---------------------------------------------------------- MIDI
    logic       rx_valid;
    logic [7:0] rx_data;
    synthframe_midi_rx #(.CLK_HZ(CLK_HZ)) midi (
        .clk, .rst, .rxd(uart_rxd), .byte_valid(rx_valid), .byte_data(rx_data), .frame_err(), .alt(midi_alt)
    );
    synthframe_midi_tx #(.CLK_HZ(CLK_HZ)) midi_out (
        .clk, .rst, .alt(midi_alt), .valid(tx_valid), .data(tx_data), .txd(uart_txd), .busy(), .dropped()
    );
    wire m_valid = rx_valid || midi_inject;
    assign midi_data = midi_inject ? midi_inject_data : rx_data;
    assign midi_valid = m_valid && running;
    always_ff @(posedge clk) midi_seen <= m_valid;

    // ---------------------------------------------------------- status strip
    // MIDI and underrun flashes; underrun since the core started (sticky, from `underruns`); real-time
    // lag that grew after the first second of running (sticky; the reset and memory clear before
    // running always count some)
    logic [23:0] midi_stretch, ur_stretch;
    logic [15:0] last_ur;
    logic [25:0] run_time;
    logic [31:0] lag_base;
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
    assign midi_led = midi_stretch != 0;
    assign underrun_led = ur_stretch != 0;

    // ---------------------------------------------------------- wave ROMs (DDR3)
    wave_ddr #(.BASE(WAVE_BASE)) wave (
        .clk, .rst,
        .wr(wave_wr), .waddr(wave_addr), .wdata(wave_data), .wbusy(wave_busy),
        .req(rom_req), .addr(rom_addr), .ready(rom_ready), .rvalid(rom_rvalid), .rdata(rom_data),
        .lreq, .laddr, .lvalid, .ldata,
        .DDRAM_BUSY, .DDRAM_BURSTCNT, .DDRAM_ADDR, .DDRAM_DOUT, .DDRAM_DOUT_READY,
        .DDRAM_RD, .DDRAM_DIN, .DDRAM_BE, .DDRAM_WE
    );

    // ---------------------------------------------------------- audio
    // s16 conversion clamp(x >>> (TO16_SHIFT - gain)), 64 (or the mk2's 66.2) -> 48 kHz
    // (resample.sv), then a FIFO that plays at 48 kHz in real time
    function automatic logic signed [15:0] to16(input logic signed [31:0] v, input logic [1:0] g);
        logic signed [31:0] s;
        s = v >>> (5'(TO16_SHIFT) - {3'd0, g});
        if (s > 32'sd32767) return 16'sh7fff;
        if (s < -32'sd32768) return 16'sh8000;
        return s[15:0];
    endfunction

    logic        rs_valid;
    logic signed [15:0] rs_l, rs_r;
    resample rs (
        .clk, .rst(core_rst), .mk2,
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
