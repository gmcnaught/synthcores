// SPDX-License-Identifier: MIT
// MU50 on MiSTer: mu50_board (the instrument) with its memories in DDR3 (mu50_mem), the ROM pack
// download, real-time pacing, MIDI in from MiSTer's UART (MidiLink), audio, the NVRAM save file,
// the panel keys from a PS/2 keyboard and the LCD video.
//
// Pacing. The board's CPU keeps virtual time (CPU states of 10 MHz). It may start an operation
// while it is at most AHEAD states ahead of real time (pre_ack); real time is a counter of
// states at CLK_HZ / 10 MHz. When the CPU falls more than LAG_CAP states behind (a long stall),
// real time is pulled back and the loss counted (lag_seen). The audio FIFO plays the SWP00's
// 44.1 kHz samples at a steady rate in real time.
//
// MIDI in. Bytes from the UART (synthframe_midi_rx) are sent to the CPU's SCI1 receive pin as
// bits in virtual time: start bit, 8 data bits LSB first, stop bit, 320 states (31,250 baud)
// each, a byte starting no earlier than the end of the previous one. Each edge goes in through
// the board's pre stage as MAME's midi port does (h8h_sys: inj_*, rx_*).
module mu50_top #(
    parameter longint CLK_HZ = 50_000_000
) (
    input  logic        clk,
    input  logic        clk_vid,
    input  logic        rst,              // power-on, RESET
    input  logic        user_reset,       // OSD reset: the CPU restarts, memories kept

    // ROM pack download (hps_io ioctl, index 1)
    input  logic        ioctl_download,
    input  logic        ioctl_wr,
    input  logic [26:0] ioctl_addr,
    input  logic [7:0]  ioctl_dout,
    output logic        ioctl_wait,

    // NVRAM save file (hps_io SD slot 0)
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
    input  logic        save_req,
    input  logic        osd_open,
    input  logic        autosave,
    output logic        saving,

    input  logic        uart_rxd,
    output logic        uart_txd,
    input  logic [10:0] ps2_key,

    input  logic        pace_enable,
    input  logic [1:0]  gain,             // 0, +6, +12, +18 dB

    output logic signed [15:0] audio_l,
    output logic signed [15:0] audio_r,
    output logic        vid_hs,
    output logic        vid_vs,
    output logic        vid_hblank,
    output logic        vid_vblank,
    output logic [7:0]  vid_r,
    output logic [7:0]  vid_g,
    output logic [7:0]  vid_b,

    output logic        rom_ok,
    output logic        running,
    output logic        midi_seen,
    output logic [15:0] underruns,

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
    localparam int TW = 48;
    localparam int AHEAD = 2000;          // states: 200 us
    localparam int LAG_CAP = 200000;      // states: 20 ms
    localparam int PACK_SIZE = 32'h481000;   // tools/mkpack.py

    // ---------------------------------------------------------------- ROM pack and memories
    logic        ld_done, mem_ready, cg_we;
    logic [11:0] cg_addr;
    logic [7:0]  cg_data;
    logic        dl_q;
    logic [26:0] dl_bytes;
    always_ff @(posedge clk) begin
        dl_q <= ioctl_download;
        ld_done <= dl_q && !ioctl_download;
        if (ioctl_download && !dl_q) dl_bytes <= '0;
        else if (ioctl_download && ioctl_wr) dl_bytes <= ioctl_addr + 27'd1;
        if (rst) rom_ok <= 1'b0;
        else if (ld_done) rom_ok <= dl_bytes == 27'(PACK_SIZE);
    end

    logic          rom_req, rom_ack, wv_req, wv_ack, dm_req, dm_we, dm_ack;
    logic [18:1]   rom_addr;
    logic [15:0]   rom_data;
    logic [21:0]   wv_addr;
    logic [31:0]   wv_data, dm_wdata, dm_rdata;
    logic [18:0]   dm_addr;

    mu50_mem mem (
        .clk, .rst,
        .ld_wr(ioctl_download && ioctl_wr && ioctl_addr < 27'(PACK_SIZE)),
        .ld_addr(ioctl_addr[22:0]), .ld_data(ioctl_dout), .ld_wait(ioctl_wait),
        .ld_done, .ready(mem_ready), .cg_we, .cg_addr, .cg_data,
        .rom_req, .rom_addr, .rom_ack, .rom_data,
        .wv_req, .wv_addr, .wv_ack, .wv_data,
        .dm_req, .dm_we, .dm_addr, .dm_wdata, .dm_ack, .dm_rdata,
        .DDRAM_BUSY, .DDRAM_BURSTCNT, .DDRAM_ADDR, .DDRAM_DOUT, .DDRAM_DOUT_READY, .DDRAM_RD,
        .DDRAM_DIN, .DDRAM_BE, .DDRAM_WE
    );

    // ---------------------------------------------------------------- NVRAM save
    logic        nv_hold, nv_pause, nv_we, ram_written;
    logic [15:0] nv_addr;
    logic [7:0]  nv_wdata, nv_rdata;
    logic        core_idle;
    synthframe_nvsave #(.AW(16), .SHADOW(1'b0)) nvsave (
        .clk, .rst, .loading(ioctl_download), .image_ok(rom_ok),
        .img_mounted, .img_readonly, .img_size, .sd_lba, .sd_blk_cnt, .sd_rd, .sd_wr, .sd_ack,
        .sd_buff_addr, .sd_buff_dout, .sd_buff_wr, .sd_buff_din,
        .save_req, .osd_open, .autosave, .mem_written(ram_written),
        .mem_we(nv_we), .mem_addr(nv_addr), .mem_wdata(nv_wdata), .mem_rdata(nv_rdata),
        .core_idle, .boot_hold(nv_hold), .pause(nv_pause), .saving, .restored()
    );

    // ---------------------------------------------------------------- reset
    logic core_rst;
    always_ff @(posedge clk) core_rst <= rst || user_reset || !rom_ok || !mem_ready || ioctl_download || nv_hold;
    assign running = !core_rst;

    // ---------------------------------------------------------------- pacing
    localparam int CPS = int'(CLK_HZ / 10_000_000);     // clocks a state (integer clock ratio)
    logic [TW-1:0] rt;                    // real time in states
    logic [7:0]    rt_div;
    logic [63:0]   vtime;
    always_ff @(posedge clk) begin
        if (core_rst) begin
            rt <= '0;
            rt_div <= 8'd0;
        end else begin
            if (rt_div == 8'(CPS - 1)) begin
                rt_div <= 8'd0;
                rt <= rt + TW'(1);
            end else
                rt_div <= rt_div + 8'd1;
            if (rt > vtime[TW-1:0] + TW'(LAG_CAP)) rt <= vtime[TW-1:0] + TW'(LAG_CAP);
        end
    end
    wire pace_ok = !pace_enable || vtime[TW-1:0] <= rt + TW'(AHEAD);

    // ---------------------------------------------------------------- MIDI in
    logic       mb_valid, frame_err, alt;
    logic [7:0] mb_data;
    synthframe_midi_rx #(.CLK_HZ(CLK_HZ)) midi (
        .clk, .rst(core_rst), .rxd(uart_rxd), .byte_valid(mb_valid), .byte_data(mb_data),
        .frame_err, .alt);
    logic [7:0] mq [16];
    logic [3:0] mq_wp, mq_rp;
    wire        mq_empty = mq_wp == mq_rp;
    always_ff @(posedge clk) begin
        if (core_rst) mq_wp <= 4'd0;
        else if (mb_valid && mq_wp + 4'd1 != mq_rp) begin
            mq[mq_wp] <= mb_data;
            mq_wp <= mq_wp + 4'd1;
        end
    end
    // serializer in virtual time: edge k of the current byte at t0 + 320 k
    logic          ser_busy, rx_busy, rx_gap;
    logic [3:0]    ser_k;
    logic [9:0]    ser_bits;          // stop, data[7:0], start (sent from bit 0)
    logic [TW-1:0] ser_t, ser_free;   // the next edge's time; when the line is free again
    logic          pre_req, pre_ack, rx_req, rx_done, inj_pending;
    logic [TW-1:0] pre_time;
    always_ff @(posedge clk) begin
        if (core_rst) begin
            ser_busy <= 1'b0;
            mq_rp <= 4'd0;
            ser_free <= '0;
            rx_busy <= 1'b0;
            rx_gap <= 1'b0;
            midi_seen <= 1'b0;
        end else begin
            midi_seen <= 1'b0;
            rx_gap <= rx_done;      // rx_req drops for a clock after each edge (periph Q_WAIT)
            if (!ser_busy && !mq_empty) begin
                ser_bits <= {1'b1, mq[mq_rp], 1'b0};
                mq_rp <= mq_rp + 4'd1;
                ser_k <= 4'd0;
                ser_t <= (vtime[TW-1:0] > ser_free) ? vtime[TW-1:0] : ser_free;
                ser_busy <= 1'b1;
                midi_seen <= 1'b1;
            end
            if (rx_req && !rx_busy) rx_busy <= 1'b1;
            if (rx_done) begin
                rx_busy <= 1'b0;
                ser_k <= ser_k + 4'd1;
                ser_t <= ser_t + TW'(320);
                if (ser_k == 4'd9) begin
                    ser_busy <= 1'b0;
                    ser_free <= ser_t + TW'(320);
                end
            end
        end
    end
    assign inj_pending = ser_busy;
    wire edge_due = ser_busy && ser_t <= pre_time;
    assign rx_req = (pre_req && edge_due && !rx_done && !rx_gap) || rx_busy;
    assign pre_ack = pre_req && !edge_due && !rx_busy && pace_ok && !nv_pause;
    assign core_idle = pre_req;     // between CPU operations

    // ---------------------------------------------------------------- the board
    logic        smp_valid, lcd_on, r_line, r_at_ac;
    logic [17:0] smp_l, smp_r;
    logic [14:0] keys;
    logic [4:0]  leds, r_pos, r_cg_bits;
    logic [2:0]  contrast, r_row;
    logic [7:0]  r_code;

    mu50_board #(.TW(TW)) board (
        .clk, .rst(core_rst),
        .rom_req, .rom_addr, .rom_ack, .rom_data,
        .wv_req, .wv_addr, .wv_ack, .wv_data,
        .dm_req, .dm_we, .dm_addr, .dm_wdata, .dm_ack, .dm_rdata,
        .smp_valid, .smp_l, .smp_r,
        .pre_req, .pre_time, .pre_ack,
        .inj_pending, .inj_pos(ser_t), .inj_time(ser_t),
        .rx_req, .rx_val(ser_bits[ser_k]), .rx_now(ser_t), .rx_done,
        .midi_tx(uart_txd),
        .keys, .leds, .contrast,
        .r_line, .r_pos, .r_row, .r_code, .r_cg_bits, .r_at_ac, .lcd_on,
        .nv_we, .nv_addr, .nv_wdata, .nv_rdata, .ram_written,
        .illegal(), .vtime,
        .tr_stb(), .tr_state(), .tr_npc(), .tr_r(), .tr_ccr(), .tr_tvec(), .tr_tlvl(),
        .bl_stb(), .bl_kind(), .bl_addr(), .bl_data(), .bl_time()
    );

    // ---------------------------------------------------------------- panel keys (PS/2)
    // MAME's key map: A Play, E Edit, S Mute/Solo, [ ] Part -/+, U Util, F Effect, Enter,
    // , . Select </>, M Mode, Backspace Exit, - = Value -/+
    logic ps2_t;
    always_ff @(posedge clk) begin
        if (rst) begin
            keys <= '0;
            ps2_t <= 1'b0;
        end else begin
            ps2_t <= ps2_key[10];
            if (ps2_key[10] != ps2_t && !ps2_key[8])
                case (ps2_key[7:0])
                    8'h1c: keys[0] <= ps2_key[9];
                    8'h24: keys[1] <= ps2_key[9];
                    8'h1b: keys[2] <= ps2_key[9];
                    8'h54: keys[3] <= ps2_key[9];
                    8'h5b: keys[4] <= ps2_key[9];
                    8'h3c: keys[5] <= ps2_key[9];
                    8'h2b: keys[6] <= ps2_key[9];
                    8'h5a: keys[7] <= ps2_key[9];
                    8'h41: keys[8] <= ps2_key[9];
                    8'h49: keys[9] <= ps2_key[9];
                    8'h3a: keys[10] <= ps2_key[9];
                    8'h66: keys[12] <= ps2_key[9];
                    8'h4e: keys[13] <= ps2_key[9];
                    8'h55: keys[14] <= ps2_key[9];
                    default: ;
                endcase
        end
    end

    // ---------------------------------------------------------------- audio
    function automatic logic signed [15:0] to16(input logic [17:0] s, input logic [1:0] g);
        logic signed [20:0] v;
        v = 21'($signed(s)) <<< g;
        v = v >>> 2;
        if (v > 21'sd32767) return 16'sd32767;
        if (v < -21'sd32768) return -16'sd32768;
        return 16'(v);
    endfunction
    logic lag_seen_unused;
    synthframe_audio_fifo #(.CLK_HZ(CLK_HZ), .RATE(44_100), .DEPTH_BITS(10)) fifo (
        .clk, .rst(core_rst), .in_valid(smp_valid), .in_l(to16(smp_l, gain)), .in_r(to16(smp_r, gain)),
        .out_l(audio_l), .out_r(audio_r), .out_strobe(), .underruns, .overruns()
    );

    // ---------------------------------------------------------------- video
    mu50_lcd_video video (
        .clk(clk_vid),
        .cg_clk(clk), .cg_we, .cg_addr, .cg_data,
        .r_line, .r_pos, .r_row, .r_code, .r_cg_bits, .lcd_on, .leds,
        .rom_ok, .loading(ioctl_download), .running, .midi_led(midi_seen),
        .underrun_led(1'b0), .underrun_seen(underruns != 16'd0), .lag_seen(1'b0),
        .hs(vid_hs), .vs(vid_vs), .hblank(vid_hblank), .vblank(vid_vblank),
        .r(vid_r), .g(vid_g), .b(vid_b)
    );
endmodule
