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
//
// NVRAM save file (synthframe_nvsave, SD slot 0): after a download the core stays in reset
// until the mounted save file, if it holds 32 KB, has replaced the pack's NVRAM; a save writes
// the NVRAM (from the MCU's copy for the dump) to the file with the steps running, and again with
// them paused only if the firmware wrote the NVRAM meanwhile (it writes it on edits, not while
// playing: none in 10 s of notes and program changes on the reference).
module jv880_board #(
    parameter longint CLK_HZ = 50_000_000,
    parameter logic [31:0] WAVE_BASE = 32'h3000_0000,
    parameter PANEL_DIR = "../hdl/panel/"   // front-panel art ROMs; Quartus: relative to mister/
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

    // save file (hps_io SD slot 0)
    input  logic        img_mounted,
    input  logic        img_readonly,
    input  logic [63:0] img_size,
    output logic [31:0] sd_lba,
    output logic [5:0]  sd_blk_cnt,
    output logic        sd_rd,
    output logic        sd_wr,
    input  logic        sd_ack,
    input  logic [13:0] sd_buff_addr,
    input  logic [7:0]  sd_buff_dout,
    input  logic        sd_buff_wr,
    output logic [7:0]  sd_buff_din,
    input  logic        save_req,         // OSD "Save NVRAM" (level)
    input  logic        osd_open,
    input  logic        autosave,         // save when the OSD opens
    output logic        saving,
    output logic        nv_restored,      // the NVRAM came from the save file

    input  logic        uart_rxd,
    output logic        uart_txd,         // MIDI out (synthframe_midi_tx, at the receiver's baud)
    input  logic        midi_inject,      // bench: a MIDI byte straight into the receive buffer
    input  logic [7:0]  midi_inject_data,
    input  logic [13:0] buttons,          // MCU_BUTTON_CURSOR_L (bit 0) .. MCU_BUTTON_RHYTHM (13)
    input  logic        enc_we,           // encoder detent (one clock)
    input  logic        enc_dir,          // 1: up (MCU_EncoderTrigger dir 1)

    input  logic        pace_enable,
    input  logic [1:0]  gain,             // output gain: 0, +6, +12, +18 dB (0 dB: 20-bit PCM full scale = s16 full scale)
    input  logic        step_gate,        // 1 on hardware; the bench opens it one step at a time
    input  logic        diag,             // the diagnostics line under the status strip

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
    // ---------------------------------------------------------- shared board services
    // (modules/roland/hdl/board/roland_board_common.sv): pack load, DDR3, reset sequence, pacing,
    // MIDI, status flags, audio
    logic        ld_we, ok, pack_ok, loading;
    logic [2:0]  ld_sel, model;
    logic [18:0] ld_addr;
    logic [7:0]  ld_data;
    logic        core_rst, reset_start, reset_done, core_ready, run_step;
    logic        m_valid, tx_valid;
    logic [7:0]  m_data, tx_data;
    logic        midi_led, underrun_led, lag_seen;
    logic        rom_req, rom_ready, rom_rvalid;
    logic [21:0] rom_addr;
    logic [7:0]  rom_data;
    logic        nv_we, nv_hold, nv_pause, core_idle, nv_wrote;
    logic [14:0] nv_addr;
    logic [7:0]  nv_wdata, nv_rdata;

    assign ok = pack_ok && model == 3'd4;      // a JV-880 pack
    assign rom_ok = ok;

    // 600 ns a step (R_NSPerStep). Audio: s16 conversion clamp(x >> (16 - gain)), resampled at the
    // mk1 rate. The PCM's samples are its 20-bit mix << 12; at 0 dB the 20-bit full scale is s16
    // full scale, so nothing clamps. The renderer's conversion (x >> 15, +6 dB here) clamps the top
    // 6 dB: 48 of the 64 factory patches reach it with one note or a 4-note chord at velocity 127.
    // The core stays in reset while the save file is restored (nv_hold) and pauses for a save's
    // second pass (nv_pause).
    roland_board_common #(.CLK_HZ(CLK_HZ), .WAVE_BASE(WAVE_BASE), .TO16_SHIFT(16)) common (
        .clk, .rst, .user_reset,
        .ioctl_download, .ioctl_wr, .ioctl_addr, .ioctl_dout, .ioctl_wait,
        .ld_we, .ld_sel, .ld_addr, .ld_data, .model, .pack_ok, .loading, .ok,
        .hold(nv_hold), .pause(nv_pause), .pace_enable, .fast(1'b0), .step_gate,
        .core_rst, .reset_start, .run_step, .running, .lag_us, .core_ready, .reset_done, .step_done,
        .uart_rxd, .uart_txd, .midi_inject, .midi_inject_data, .midi_alt, .midi_seen,
        .midi_valid(m_valid), .midi_data(m_data), .tx_valid, .tx_data,
        .midi_led, .underrun_led, .lag_seen,
        .rom_req, .rom_addr, .rom_ready, .rom_rvalid, .rom_data,
        .lreq(1'b0), .laddr(15'd0), .lvalid(), .ldata(),
        .mk2(1'b0), .gain, .sample_valid, .sample_l, .sample_r, .audio_l, .audio_r, .underruns, .overruns,
        .DDRAM_BUSY, .DDRAM_BURSTCNT, .DDRAM_ADDR, .DDRAM_DOUT, .DDRAM_DOUT_READY,
        .DDRAM_RD, .DDRAM_DIN, .DDRAM_BE, .DDRAM_WE
    );

    // ---------------------------------------------------------- NVRAM save file
    // SHADOW 0: a 32 KB shadow needs 32 M10K and the JV-880 has 27 free (526 / 553 at
    // build-rbf 45859974183), so saves keep the paused second pass when the MCU writes the NVRAM
    synthframe_nvsave #(.AW(15), .SHADOW(1'b0)) nvsave (
        .clk, .rst, .loading(ioctl_download || loading), .image_ok(ok),
        .img_mounted, .img_readonly, .img_size,
        .sd_lba, .sd_blk_cnt, .sd_rd, .sd_wr, .sd_ack, .sd_buff_addr, .sd_buff_dout, .sd_buff_wr, .sd_buff_din,
        .save_req, .osd_open, .autosave, .mem_written(nv_wrote),
        .mem_we(nv_we), .mem_addr(nv_addr), .mem_wdata(nv_wdata), .mem_rdata(nv_rdata),
        .core_idle, .boot_hold(nv_hold), .pause(nv_pause), .saving, .restored(nv_restored)
    );
    // the MCU's load port: the pack loader, or the save file into the NVRAM (ld_sel 5)
    wire        c_ld_we   = ld_we || nv_we;
    wire [2:0]  c_ld_sel  = nv_we ? 3'd5 : ld_sel;
    wire [18:0] c_ld_addr = nv_we ? {4'd0, nv_addr} : ld_addr;
    wire [7:0]  c_ld_data = nv_we ? nv_wdata : ld_data;

    // ---------------------------------------------------------- panel: encoder detents (registered)
    logic enc_go, enc_pdir;
    always_ff @(posedge clk) begin
        enc_go <= enc_we && running;
        enc_pdir <= enc_dir;
    end

    // ---------------------------------------------------------- core
    logic [9:0] jv_leds;                  // panel LEDs (sc55_mcu), for the video
    sc55_core #(.ROM2_AW(18), .JV880_RAMS(1'b1)) core (
        .clk, .rst(core_rst),
        .run_step, .step_done, .reset_start, .reset_done, .ready(core_ready),
        .ld_we(c_ld_we), .ld_sel(c_ld_sel), .ld_addr(c_ld_addr), .ld_data(c_ld_data),
        .nv_dump_a(nv_addr), .nv_dump_d(nv_rdata), .nv_wrote, .idle(core_idle),
        .mk2(1'b0), .sc155(1'b0), .jv880(1'b1),
        .enc_we(enc_go), .enc_dir(enc_pdir),
        .rom2x_req(), .rom2x_addr(), .rom2x_ack(1'b0), .rom2x_data(8'h00),
        .midi_valid(m_valid), .midi_data(m_data), .midi_full(), .tx_valid, .tx_data, .buttons({18'h0, buttons}), .jv_leds,
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


    // diagnostics line (OSD; hex): MIDI bytes received, the last 4 bytes, the PCM's peak |left|
    // (bits 31:16) and the output's peak |left| over the last 2**24 clocks (0.34 s), underruns
    logic [15:0] dbg_n, dbg_pcm_pk, dbg_out_pk, pcm_pk_run, out_pk_run;
    logic [31:0] dbg_last;
    logic [23:0] dbg_win;
    always_ff @(posedge clk) begin : dbg
        logic [15:0] a, o;
        a = sample_l[31] ? 16'(~sample_l[31:16]) : sample_l[31:16];
        o = audio_l[15] ? ~audio_l : audio_l;
        if (m_valid) begin dbg_n <= dbg_n + 16'd1; dbg_last <= {dbg_last[23:0], m_data}; end
        dbg_win <= dbg_win + 24'd1;
        if (dbg_win == '1) begin
            dbg_pcm_pk <= pcm_pk_run; dbg_out_pk <= out_pk_run; pcm_pk_run <= '0; out_pk_run <= '0;
        end else begin
            if (sample_valid && a > pcm_pk_run) pcm_pk_run <= a;
            if (o > out_pk_run) out_pk_run <= o;
        end
    end
    function automatic logic [7:0] hexc(input logic [3:0] n);
        return n < 4'd10 ? 8'h30 + 8'(n) : 8'h37 + 8'(n);
    endfunction
    logic [95:0] dbg_bits;
    logic [7:0]  dbg_ch [24];
    assign dbg_bits = {dbg_n, dbg_last, dbg_pcm_pk, dbg_out_pk, underruns};
    always_comb for (int k = 0; k < 24; k++) dbg_ch[k] = hexc(dbg_bits[95 - 4 * k -: 4]);

    jv880_lcd_video #(.PANEL_DIR(PANEL_DIR)) video (
        .clk(clk_vid), .dd(lcd_dd), .cg, .dd_ram, .cursor_on(c_flag), .buttons, .leds(jv_leds),
        .rom_ok, .loading, .running, .midi_led, .underrun_led,
        .underrun_seen(underruns != 16'd0), .lag_seen, .dbg_ch, .dbg_on(diag),
        .hs(vid_hs), .vs(vid_vs), .hblank(vid_hblank), .vblank(vid_vblank), .r(vid_r), .g(vid_g), .b(vid_b)
    );

endmodule
