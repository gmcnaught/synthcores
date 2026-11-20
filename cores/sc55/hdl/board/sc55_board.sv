// SC-55 core with its board services, independent of MiSTer's framework:
// ROM pack download, wave ROMs in DDR3, real-time pacing, MIDI in from a
// UART, audio to 16 bit at the output rate. mister/SC55.sv adds hps_io, the
// PLL, video; sim/board/board_lockstep.cpp drives it against the reference.
//
// The core is held in reset until a pack with a valid header has loaded,
// and again during any later download; then the PCM clears its eram, the
// MCU takes its reset vector, and steps run while the pacer allows (and
// step_gate, for the bench).
module sc55_board #(
    parameter longint CLK_HZ = 50_000_000,
    parameter logic [31:0] WAVE_BASE = 32'h3000_0000
) (
    input  logic        clk,
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
    input  logic [31:0] buttons,

    input  logic        pace_enable,
    input  logic        step_gate,        // 1 on hardware; the bench opens it one step at a time

    output logic signed [15:0] audio_l,
    output logic signed [15:0] audio_r,
    output logic        sample_valid,     // the PCM's raw output (bench)
    output logic signed [31:0] sample_l,
    output logic signed [31:0] sample_r,

    output logic        lcd_we,
    output logic        lcd_rs,
    output logic [7:0]  lcd_data,
    output logic        lcd_enable,

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
    logic        ld_we, wave_wr, wave_busy, loading, ok;
    logic [1:0]  ld_sel;
    logic [17:0] ld_addr;
    logic [7:0]  ld_data, wave_data;
    logic [21:0] wave_addr;

    rom_loader loader (
        .clk, .rst,
        .ioctl_download, .ioctl_wr, .ioctl_addr, .ioctl_dout, .ioctl_wait,
        .ld_we, .ld_sel, .ld_addr, .ld_data,
        .wave_wr, .wave_addr, .wave_data, .wave_busy,
        .loading, .ok
    );
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

    // ---------------------------------------------------------- pacing
    logic allow;
    step_pacer #(.CLK_HZ(CLK_HZ), .STEP_NS(600)) pacer (
        .clk, .rst(core_rst), .enable(pace_enable), .step_done, .allow, .lag_us
    );
    assign run_step = running && allow && step_gate;

    // ---------------------------------------------------------- MIDI
    logic       rx_valid;
    logic [7:0] rx_data;
    midi_rx #(.CLK_HZ(CLK_HZ)) midi (
        .clk, .rst, .rxd(uart_rxd), .byte_valid(rx_valid), .byte_data(rx_data), .frame_err(), .alt(midi_alt)
    );
    wire       m_valid = rx_valid || midi_inject;
    wire [7:0] m_data  = midi_inject ? midi_inject_data : rx_data;
    always_ff @(posedge clk) midi_seen <= m_valid;

    // ---------------------------------------------------------- core
    logic        rom_req, rom_ready, rom_rvalid;
    logic [21:0] rom_addr;
    logic [7:0]  rom_data;

    sc55_core core (
        .clk, .rst(core_rst),
        .run_step, .step_done, .reset_start, .reset_done, .ready(core_ready),
        .ld_we, .ld_sel, .ld_addr, .ld_data,
        .midi_valid(m_valid && running), .midi_data(m_data), .midi_full(), .buttons,
        .rom_req, .rom_addr, .rom_ready, .rom_data, .rom_rvalid,
        .enable_oversampling(1'b1),
        .sample_valid, .sample_l, .sample_r,
        .lcd_we, .lcd_rs, .lcd_data, .lcd_enable,
        .cycles(), .dbg_pc, .dbg_cp, .dbg_sr()
    );

    wave_ddr #(.BASE(WAVE_BASE)) wave (
        .clk, .rst,
        .wr(wave_wr), .waddr(wave_addr), .wdata(wave_data), .wbusy(wave_busy),
        .req(rom_req), .addr(rom_addr), .ready(rom_ready), .rvalid(rom_rvalid), .rdata(rom_data),
        .DDRAM_BUSY, .DDRAM_BURSTCNT, .DDRAM_ADDR, .DDRAM_DOUT, .DDRAM_DOUT_READY,
        .DDRAM_RD, .DDRAM_DIN, .DDRAM_BE, .DDRAM_WE
    );

    // ---------------------------------------------------------- audio
    // the renderer's s16 conversion (Normalize: clamp(x >> 15)), then a FIFO
    // that plays at the output rate (mk1, oversampling on: 64 kHz)
    function automatic logic signed [15:0] to16(input logic signed [31:0] v);
        logic signed [31:0] s;
        s = v >>> 15;
        if (s > 32'sd32767) return 16'sh7fff;
        if (s < -32'sd32768) return 16'sh8000;
        return s[15:0];
    endfunction

    audio_fifo #(.CLK_HZ(CLK_HZ), .RATE(64_000), .DEPTH_BITS(8)) fifo (
        .clk, .rst(core_rst),
        .in_valid(sample_valid), .in_l(to16(sample_l)), .in_r(to16(sample_r)),
        .out_l(audio_l), .out_r(audio_r), .out_strobe(),
        .underruns, .overruns
    );
endmodule
