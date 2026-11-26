// SC-55 (mk1) MCU system: H8/532 CPU, memories, on-chip peripherals and the
// mk1 gate array, sequenced like the reference model's MCU_Step:
//
//   CPU part (h8_cpu)  ->  cycles += 12  ->  PCM update (external, pcm_tick)
//   ->  TIMER_Clock  ->  UART RX/TX (mk1)  ->  analog  ->  gate-array LCD counter
//
// Matched to mcu.cpp (MCU_Read/MCU_Write/MCU_DeviceRead/MCU_DeviceWrite,
// MCU_UpdateUART_*, MCU_UpdateAnalog, MCU_GA_SetGAInt) and mcu_timer.cpp.
//
// The PCM chip is outside: CPU accesses to e000-e03f go out on the pcm_*
// port, `pcm_tick` asks it to run up to `cycles`, and its IRQ0 request comes
// back as `pcm_irq0`.
//
// Models: mk1 (default), mk2 (`mk2`), SC-155 (`sc155`) and JV-880 (`jv880`; cores/jv880/docs/plan.md).
// The JV-880 is a non-mk1 map with the gate array at F000 (PCM F000-F3FF, gate array F400-F7FF),
// its interrupt on IRQ0 with the PCM's request on gate-array line 5 and the encoder on lines 3/4,
// the panel on P7DR, NVRAM (pages 12/13; JV880_RAMS), card RAM (pages 14/15: no card, reads 0), generic timers.

module sc55_mcu #(
    parameter int RXBUF_BITS = 11,         // MIDI receive buffer: 2^RXBUF_BITS bytes
    parameter int ROM2_AW = 18,            // ROM2 block RAM: 18 = 256 KB (mk1), 19 = 512 KB (mk2)
    parameter bit JV880_RAMS = 1'b0        // the JV-880's NVRAM (32 KB block RAM and its dump copy)
) (
    input  logic        clk,
    input  logic        rst,
    input  logic        mk2,             // SC-55mk2 / SC-155mk2: memory map, timer tables, sub-MCU (0: mk1)
    input  logic        sc155,           // SC-155 / SC-155mk2: A/D sliders, P9DR model bit
    input  logic        jv880,           // JV-880 (needs JV880_RAMS)
    // JV-880 encoder: one detent (MCU_EncoderTrigger: gate-array line 3 for dir 0, 4 for dir 1)
    input  logic        enc_we,
    input  logic        enc_dir,

    // one model step per run_step pulse; step_done when the devices have advanced
    input  logic        run_step,
    output logic        step_done,
    input  logic        reset_start,     // MCU_Reset (registers, devices, reset vector)
    output logic        reset_done,
    output logic        ready,           // power-on clear of SRAM and on-chip RAM finished (the model starts zeroed)

    // memory load port (ROMs, savestates): sel 0 rom1, 1 rom2, 2 sram, 3 ram
    input  logic        ld_we,
    input  logic [2:0]  ld_sel,          // 0 ROM1, 1 ROM2, 2 SRAM, 3 RAM, 4 sub-MCU ROM, 5 NVRAM, 6 card RAM
    input  logic [18:0] ld_addr,
    input  logic [7:0]  ld_data,
    // JV-880 NVRAM dump (save file): a copy of the NVRAM with its own read port, data one clock
    // after the address; `idle` = no step in progress (with run_step low, the CPU writes nothing)
    input  logic [14:0] nv_dump_a,
    output logic [7:0]  nv_dump_d,
    output logic        nv_wrote,         // the NVRAM was written this clock
    output logic        idle,

    // MIDI bytes into the UART receive buffer (MCU_PostUART)
    input  logic        midi_valid,
    input  logic [7:0]  midi_data,
    output logic        midi_full,
    // MIDI out (the reference sends nothing; patch_ref.py's tx hook marks the same points):
    // the SCI's byte when the CPU clears TDRE (mk1, SC-155, JV-880), the sub-MCU's UART2 byte (mk2)
    output logic        tx_stb,
    output logic [7:0]  tx_byte,
    output logic        sm_tx_stb,
    output logic [7:0]  sm_tx_byte,

    // front panel
    input  logic [31:0] buttons,         // mcu.button_pressed
    // JV-880 panel LEDs (display only; the reference has none): the firmware scans them with the
    // buttons, writing the gate array's x401 (io_sd) = fe or fd, then P7DR = that row's LEDs
    // (1: lit). [4:0] row fe: UTILITY, RHYTHM, SYSTEM, EDIT, MIDI MESSAGE; [9:5] row fd:
    // TONE SWITCH 4, 3, 2, 1, PATCH (PATCH/PERFORM indicator, lit in Patch mode).
    output logic [9:0]  jv_leds,

    // PCM chip
    output logic        pcm_req,
    output logic        pcm_we,
    output logic [5:0]  pcm_addr,
    output logic [7:0]  pcm_wdata,
    input  logic [7:0]  pcm_rdata,
    input  logic        pcm_ack,
    output logic        pcm_tick,        // `cycles` advanced: the PCM runs the frames now due, alongside the CPU
    input  logic        pcm_irq0,        // INTERRUPT_SOURCE_IRQ0 request
    input  logic        pcm_irq_known,   // pcm_irq0 already has its value for the frames due by `cycles`

    // ROM2's upper 256 KB outside (mk2 with ROM2_AW = 18: DDR3 through a cache)
    output logic        rom2x_req,       // held until rom2x_ack
    output logic [17:0] rom2x_addr,
    input  logic        rom2x_ack,
    input  logic [7:0]  rom2x_data,

    // LCD
    output logic        lcd_we,          // LCD_Write(address = lcd_rs, data)
    output logic        lcd_rs,
    output logic [7:0]  lcd_data,
    output logic        lcd_enable,      // LCD_Enable

    output logic [63:0] cycles,

    // CPU bus, for the lockstep bench
    output logic        mon_req,
    output logic        mon_we,
    output logic [23:0] mon_addr,
    output logic [7:0]  mon_wdata,
    output logic [7:0]  mon_rdata,
    output logic        mon_ack,

    // CPU state (bench, status line)
    output logic [15:0] dbg_r [8],
    output logic [15:0] dbg_pc, dbg_sr,
    output logic [7:0]  dbg_cp, dbg_dp, dbg_ep, dbg_tp, dbg_br,
    output logic        dbg_sleep,
    output logic [20:0] dbg_int,
    output logic        dbg_exign,
    output logic [2:0]  dbg_exc,
    output logic [15:0] dbg_trapa,
    output logic [31:0] dbg_traps,

    // CPU state load (bench)
    input  logic        cpu_ld_en,
    input  logic [15:0] cpu_ld_r [8],
    input  logic [15:0] cpu_ld_pc, cpu_ld_sr,
    input  logic [7:0]  cpu_ld_cp, cpu_ld_dp, cpu_ld_ep, cpu_ld_tp, cpu_ld_br
);

    // ------------------------------------------------------------------
    // Memories
    // ------------------------------------------------------------------
    // Block RAMs (modules/roland/hdl/common/sdp_ram.sv): reads are registered, so a CPU read
    // takes two clocks (rd_b below); writes come through one port each.
    logic [7:0]  rom1_rd, rom2_rd, sram_rd, ram_rd, nvram_rd, card_rd;
    logic        nvram_we, card_we;
    logic [14:0] nvram_wa, card_wa;
    logic [7:0]  nvram_wd, card_wd;
    logic        sram_we, ram_we;
    logic [14:0] sram_wa;
    logic [9:0]  ram_wa;
    logic [7:0]  sram_wd, ram_wd;

    // ------------------------------------------------------------------
    // CPU
    // ------------------------------------------------------------------
    logic        c_req, c_we, c_ack, c_step, c_done, c_reset, c_more;
    wire         c_step_now;                 // the CPU starts the step in the clock run_step is seen (below)
    logic [23:0] c_addr;
    logic [7:0]  c_wdata, c_rdata;
    logic [20:0] int_pend;
    logic [7:0]  dev [128];

    h8_cpu cpu (
        .clk, .rst,
        .step(c_step_now), .step_done(c_done),
        .bus_req(c_req), .bus_we(c_we), .bus_addr(c_addr), .bus_wdata(c_wdata),
        .bus_rdata(c_rdata), .bus_ack(c_ack), .bus_more(c_more),
        .int_pending(int_pend),
        .ipra(dev[7'h70]), .iprb(dev[7'h71]), .iprc(dev[7'h72]), .iprd(dev[7'h73]), .p1cr(dev[7'h7c]),
        .int_hold(int_hold),
        .reset_start(c_reset),
        .ld_en(cpu_ld_en), .ld_r(cpu_ld_r), .ld_pc(cpu_ld_pc), .ld_sr(cpu_ld_sr),
        .ld_cp(cpu_ld_cp), .ld_dp(cpu_ld_dp), .ld_ep(cpu_ld_ep), .ld_tp(cpu_ld_tp), .ld_br(cpu_ld_br),
        .r_out(dbg_r), .pc_out(dbg_pc), .sr_out(dbg_sr), .cp_out(dbg_cp), .dp_out(dbg_dp), .ep_out(dbg_ep),
        .tp_out(dbg_tp), .br_out(dbg_br), .sleep_out(dbg_sleep), .ex_ignore_out(dbg_exign), .exc_out(dbg_exc),
        .trapa_out(dbg_trapa), .traps(dbg_traps)
    );

    // The interrupt check waits only when IRQ0 is enabled, would beat the
    // mask, is not already requested, and the PCM has not settled it yet;
    // likewise for IRQ1 while the sub-MCU (mk2) is still running the last
    // step's SM_Update (it can raise IRQ1 through GA line 5, never clear it).
    logic int_hold;
    // (JV-880: IRQ0 is the gate array's, which the PCM can raise through line 5.)
    assign int_hold = (!pcm_irq_known && !(jv880 ? jv_irq0 : pcm_irq0) && dev[7'h7c][5] && (dev[7'h70][6:4] > dbg_sr[10:8])) ||
                      (mk2 && !sm_caught_up && !int_req[2] && dev[7'h7c][6] && (dev[7'h70][2:0] > dbg_sr[10:8]));

    // ------------------------------------------------------------------
    // Sub-MCU (mk2): EC00-EFFF window, MIDI from the receive buffer, GA line 5
    // ------------------------------------------------------------------
    logic        sm_req, sm_we, sm_ack, sm_run, sm_done, sm_reset, sm_reset_done;
    logic        sm_rx_pop, sm_ga5_we, sm_ga5_val, sm_dly_we, sm_caught_up, sm_ready;
    logic [7:0]  sm_addr, sm_wdata, sm_rdata, sm_rx_byte;
    logic [63:0] sm_dly_wv;
    // The sub-MCU runs each step's SM_Update alongside the next step: the
    // device phase publishes 5 * cycles and the receive-buffer bytes it may
    // take (those that were there at the device phase).
    logic [63:0] sm_tgt5;
    logic [RXBUF_BITS-1:0] sm_wp;
    sc55_submcu sm (
        .clk, .rst,
        .ld_we(ld_we && ld_sel == 3'd4), .ld_addr(ld_addr[11:0]), .ld_data,
        .ld_en(1'b0), .ld_pc(16'h0), .ld_a(8'h0), .ld_x(8'h0), .ld_y(8'h0), .ld_s(8'h0), .ld_sr(8'h0), .ld_sleep(1'b0),
        .reset_start(sm_reset), .reset_done(sm_reset_done), .ready(sm_ready),
        .run(mk2), .target5(sm_tgt5), .done(sm_done), .caught_up(sm_caught_up),
        .sys_req(sm_req && !sm_ack), .sys_we(sm_we), .sys_addr(sm_addr), .sys_wdata(sm_wdata), .sys_rdata(sm_rdata), .sys_ack(sm_ack),
        .rx_avail(mk2 && sm_wp != rx_rp), .rx_data(rx_rd), .rx_pop(sm_rx_pop), .rx_byte(sm_rx_byte),
        .rx_delay_we(sm_dly_we), .rx_delay_wv(sm_dly_wv),
        .buttons, .ga5_we(sm_ga5_we), .ga5_val(sm_ga5_val), .tx_stb(sm_tx_stb), .tx_byte(sm_tx_byte),
        .st_pc(), .st_a(), .st_x(), .st_y(), .st_s(), .st_sr(), .st_cycles(), .st_sleep(),
        .st_p0_data(), .st_p0_dir(), .st_p1_dir(), .st_p1_data(), .st_prescaler(), .st_counter(),
        .st_timer_cycles(), .st_rx_delay(), .st_gotbyte(), .st_instructions()
    );

    assign mon_req = c_req; assign mon_we = c_we; assign mon_addr = c_addr;
    assign mon_wdata = c_wdata; assign mon_rdata = c_rdata; assign mon_ack = c_ack;

    // ------------------------------------------------------------------
    // Peripheral state
    // ------------------------------------------------------------------
    // interrupt request bits (MCU_Interrupt_Source); bit 1 (IRQ0) comes from the PCM, bit 2 (IRQ1)
    // from the gate array (int_req[2] = trigger != 0); on the JV-880 the gate array's goes to IRQ0
    logic [20:0] int_req;
    // JV-880: a rising PCM request on line 5 counts in the clock it arrives (the gate array latches
    // it at the next edge), as the PCM runs alongside the CPU
    wire jv_irq0 = int_req[2] || (pcm_irq0 && !ga_line[5] && ga_en[5]);
    always_comb begin
        int_pend = int_req;
        int_pend[1] = jv880 ? jv_irq0 : pcm_irq0;
        if (jv880) int_pend[2] = 1'b0;
    end
    assign dbg_int = int_pend;

    // FRTs (mcu_timer.cpp frt_t)
    logic [63:0] frt_deadline [3];
    logic [7:0]  frt_tcr [3], frt_tcsr [3], frt_status_rd [3];
    logic [15:0] frt_frc [3], frt_ocra [3], frt_ocrb [3], frt_icr [3];
    logic [7:0]  frt_stride [3];
    // 8-bit timer
    logic [63:0] tmr_deadline;
    logic [15:0] tmr_stride;
    logic [7:0]  tmr_tcr, tmr_tcsr, tmr_tcora, tmr_tcorb, tmr_tcnt, tmr_status_rd;
    logic [7:0]  tempreg;
    // UART
    logic [7:0]  uart_rx_byte;
    logic [63:0] uart_rx_delay, uart_tx_delay;
    logic [7:0]  ssr_rd;
    logic        tdr_written;              // MIDI out: TDR written since the last byte sent
    // analog
    logic [63:0] analog_end;
    logic        adf_rd;
    // gate array (mk1)
    logic [8:0]  ga_lcd_counter;
    logic [2:0]  ga_trigger;
    logic        ga_int1;
    logic [7:0]  io_sd;
    logic [1:0]  led_arm;                // JV-880: the next P7DR write is LED row fe ([0]) / fd ([1])
    // gate array (mk2, JV-880): interrupt enable (x402 write << 1); lines (MCU_GA_SetGAInt):
    // mk2 line 5 from the sub-MCU; JV-880 line 5 from the PCM, 3/4 from the encoder
    logic [7:0]  ga_en;
    logic [7:0]  ga_line;
    wire         nm1 = mk2 || jv880;            // not the mk1 (reference !is_mk1)
    wire  [15:0] ga_base = jv880 ? 16'hf000 : 16'he000;

    // MIDI receive buffer (MCU_PostUART ring buffer)
    logic [RXBUF_BITS-1:0] rx_wp, rx_rp, rx_wp_d1, rx_wp_d2;
    logic [7:0]  rx_rd;
    assign midi_full = (rx_wp + 1'b1) == rx_rp;
    // a byte is taken only once it is surely in the RAM (two clocks after its write)
    wire rx_ready = rx_wp_d2 != rx_rp;

    // timer step tables (mcu_timer.cpp: mk1, or generic for the mk2)
    function automatic logic [7:0] frt_stride_for(input logic [1:0] cks, input logic gen);
        case (cks) 2'd0: return 8'd4; 2'd1: return 8'd8; 2'd2: return 8'd32; default: return gen ? 8'd2 : 8'd4; endcase
    endfunction
    function automatic logic [15:0] tmr_stride_for(input logic [2:0] cks, input logic gen);
        case (cks)
            3'd1: return 16'd8; 3'd2: return 16'd64; 3'd3: return 16'd1024;
            3'd5, 3'd6, 3'd7: return gen ? 16'd2 : 16'd4;
            default: return 16'd0;
        endcase
    endfunction
    function automatic logic [63:0] align_fwd(input logic [63:0] v, input logic [15:0] iv);
        logic [63:0] m;
        m = {48'h0, iv} - 64'd1;
        return (v + m) & ~m;
    endfunction

    // A/D pin values (MCU_AnalogReadPin; the RCU path and the SC-155 sliders read 0)
    //   mk1: channel 7 = battery unless an SC-155 selects its sliders (P9DR bits 0/1)
    //   mk2: channel 7 by io_sd[3:2]: battery, NC, the switch (sw_pos 3: 0x3ff), RCU
    //   JV-880: pin 1 = battery, the others 0x3ff
    function automatic logic [9:0] analog_pin(input logic [2:0] ch, input logic m2, input logic s155,
                                              input logic [7:0] iosd, input logic [7:0] p9, input logic jv);
        if (jv) return (ch == 3'd1) ? 10'h2a0 : 10'h3ff;
        if (!m2) begin
            if (s155 && p9[0]) return 10'h000;
            if (ch != 3'd7) return 10'h000;
            return (s155 && p9[1]) ? 10'h000 : 10'h2a0;
        end
        if (s155 && iosd[4]) return 10'h000;
        if (ch != 3'd7) return 10'h000;
        case (iosd[3:2])
            2'd0: return 10'h2a0;
            2'd2: return 10'h3ff;
            default: return 10'h000;
        endcase
    endfunction
    wire [9:0] an_v [8];
    genvar gc;
    generate
        for (gc = 0; gc < 8; gc = gc + 1) begin : g_an
            assign an_v[gc] = analog_pin(3'(gc), mk2, sc155, io_sd, dev[7'h7f], jv880);
        end
    endgenerate
    function automatic logic [7:0] adc_hi(input logic [9:0] v);
        return v[9:2];
    endfunction
    function automatic logic [7:0] adc_lo(input logic [9:0] v);
        return {v[1:0], 6'h0};
    endfunction

    // ------------------------------------------------------------------
    // Sequencer
    // ------------------------------------------------------------------
    typedef enum logic [3:0] {
        Q_IDLE, Q_CPU, Q_TIMERS, Q_RESET, Q_RESET_CPU, Q_CLEAR, Q_RESET_SM, Q_SM
    } q_t;
    q_t q;
    // two-clock reads: block RAM data registers and the source chosen in the first clock
    logic [7:0] rd_dev;
    logic [2:0] rsel;
    logic       rd_b;
    logic [15:0] clr_a;
    assign ready = (q != Q_CLEAR) && sm_ready;
    assign idle = (q == Q_IDLE);
    assign c_step_now = (q == Q_IDLE) && !reset_start && run_step;

    wire [63:0] tcyc = cycles >> 1;    // timer.cycles during an instruction (= previous TIMER_Clock target)

    // bus decode
    wire [3:0]  page = c_addr[19:16];
    wire [15:0] a16  = c_addr[15:0];
    // (address & 0x3ffff) | (bit 19 ? 0x40000 : 0), then & rom2_mask (mk1 256 KB, mk2 512 KB)
    wire [18:0] rom2_a19 = {mk2 & c_addr[19], c_addr[17:0]};
    wire rom2_ext = (ROM2_AW < 19) && rom2_a19[18];

    // Sequential reads: while a block-RAM byte is delivered, the block RAMs
    // already read the next address (same page); if the CPU's next read is
    // that address and decodes to a block RAM, it is answered when taken
    // (one clock instead of two). Any write, load or clear drops it.
    logic [23:0] spec_a;
    logic        spec_v;
    logic [2:0]  bram_rs;                  // the block RAM c_addr decodes to (0: none)
    wire  [23:0] c_next = {c_addr[23:16], c_addr[15:0] + 16'd1};
    wire         accept = c_req && !c_ack && !pcm_req && !sm_req && !rom2x_req && !rd_b && !sm_wait;
    wire         hit = accept && !c_we && spec_v && c_addr == spec_a && bram_rs != 3'd0;
    // streaming: while byte A of a sequential read burst is being acknowledged and the block RAMs
    // hold A + 1 (spec), A + 1 is answered the next clock without a request
    wire  [23:0] c_next2 = {c_next[23:16], c_next[15:0] + 16'd1};
    wire  [2:0]  bram_rs_n = bram_of(c_next, mk2, jv880, dev[7'h79][7]);
    wire         stream = c_ack && c_req && !c_we && c_more && spec_v && spec_a == c_next && bram_rs_n != 3'd0
                          && !pcm_req && !sm_req && !rom2x_req && !rd_b;
    wire  [23:0] bram_a = stream ? c_next2 : (rd_b || hit) ? c_next : accept ? c_addr : spec_a;   // the block RAMs' read address
    wire  [18:0] bram_a_rom2 = {mk2 & bram_a[19], bram_a[17:0]};
    // the PCM and sub-MCU windows (page 0)
    wire in_pcm = page == 4'd0 && a16 >= ga_base && (nm1 ? a16 < (ga_base | 16'h0400) : a16 < 16'he040);
    wire in_sm  = page == 4'd0 && mk2 && a16 >= 16'hec00 && a16 < 16'hf000;
    // the block RAM an address decodes to (0: none; 1 ROM1, 2 ROM2, 3 SRAM, 4 on-chip RAM,
    // 5 NVRAM, 6 card RAM)
    function automatic logic [2:0] bram_of(input logic [23:0] ad, input logic m2, input logic jv, input logic ramcr);
        logic [15:0] a;
        logic        pcm, smw, ext;
        a = ad[15:0];
        pcm = jv ? a >= 16'hf000 && a < 16'hf400 : a >= 16'he000 && (m2 ? a < 16'he400 : a < 16'he040);
        smw = m2 && a >= 16'hec00 && a < 16'hf000;
        ext = (ROM2_AW < 19) && m2 && ad[19];
        case (ad[19:16])
            4'd0: begin
                if (!a[15]) return 3'd1;
                if (!pcm && !smw && a < 16'hff80) begin
                    if (a >= 16'hfb80 && ramcr) return 3'd4;
                    if (a < 16'he000) return 3'd3;
                end
                return 3'd0;
            end
            4'd1, 4'd2, 4'd3, 4'd4: return ext ? 3'd0 : 3'd2;
            4'd8, 4'd9: return (ext || jv) ? 3'd0 : 3'd2;
            4'd14, 4'd15: return jv ? 3'd6 : ext ? 3'd0 : 3'd2;
            4'd12, 4'd13: return jv ? 3'd5 : 3'd0;
            4'd5: return (m2 || jv) ? 3'd0 : 3'd3;
            4'd10, 4'd11: return (m2 || jv) ? 3'd3 : 3'd0;
            default: return 3'd0;
        endcase
    endfunction
    assign bram_rs = bram_of(c_addr, mk2, jv880, dev[7'h79][7]);
    // accesses that see or change what the running sub-MCU uses: wait until it has caught up
    wire sm_sync = mk2 && page == 4'd0 && (in_sm || a16 == 16'he402 || a16 == 16'hffdd || (c_we && a16 == 16'hffdc));
    wire sm_wait = sm_sync && !sm_caught_up;


    // ------------------------------------------------------------------
    // Memories and their write ports: clear (power-on), load port, CPU write
    // ------------------------------------------------------------------
    wire cpu_wr_now = !pcm_req && !sm_req && !rom2x_req && !rd_b && c_req && !c_ack && c_we && !sm_wait;
    wire dec_sram   = (c_addr[19:16] == 4'd0 && c_addr[15:0] >= 16'h8000 && c_addr[15:0] < 16'he000) ||
                      c_addr[19:16] == (nm1 ? 4'd10 : 4'd5);
    wire dec_nvram  = jv880 && c_addr[19:16] == 4'd12;      // writes: page 12 only (reads 12 and 13)
    wire dec_card   = jv880 && c_addr[19:16] == 4'd14;      // writes: page 14 only
    wire dec_ram    = c_addr[19:16] == 4'd0 && c_addr[15:0] >= 16'hfb80 && c_addr[15:0] < 16'hff80 && dev[7'h79][7];
    always_comb begin
        sram_we = 1'b0; sram_wa = c_addr[14:0]; sram_wd = c_wdata;
        ram_we = 1'b0;  ram_wa = 10'(c_addr[15:0] - 16'hfb80); ram_wd = c_wdata;
        nvram_we = 1'b0; nvram_wa = c_addr[14:0]; nvram_wd = c_wdata;
        card_we = 1'b0;  card_wa = c_addr[14:0];  card_wd = c_wdata;
        if (q == Q_CLEAR) begin
            sram_we = 1'b1; sram_wa = clr_a[14:0]; sram_wd = 8'h00;
            ram_we = (clr_a[15:10] == 6'd0); ram_wa = clr_a[9:0]; ram_wd = 8'h00;
            // NVRAM and card RAM are battery-backed: not cleared (a pack or save file loads them)
        end else if (ld_we) begin
            sram_we = (ld_sel == 3'd2); sram_wa = ld_addr[14:0]; sram_wd = ld_data;
            ram_we = (ld_sel == 3'd3); ram_wa = ld_addr[9:0]; ram_wd = ld_data;
        end else if (cpu_wr_now) begin
            sram_we = dec_sram;
            ram_we = dec_ram;
        end
        // NVRAM is battery-backed, never cleared: its load port works while the core is held in
        // reset (the board loads the pack's NVRAM then). Card RAM (no card fitted: the model's reads
        // 0) is cleared with the SRAM.
        if (ld_we) begin
            nvram_we = (ld_sel == 3'd5); nvram_wa = ld_addr[14:0]; nvram_wd = ld_data;
            card_we = (ld_sel == 3'd6);  card_wa = ld_addr[14:0];  card_wd = ld_data;
        end else if (q == Q_CLEAR) begin
            card_we = 1'b1; card_wa = clr_a[14:0]; card_wd = 8'h00;
        end else if (cpu_wr_now) begin
            nvram_we = dec_nvram;
            card_we = dec_card;
        end
    end

    sdp_ram #(.AW(15), .DW(8)) u_rom1 (.clk, .we(ld_we && ld_sel == 3'd0), .wa(ld_addr[14:0]), .wd(ld_data),
                                      .ra(bram_a[14:0]), .rd(rom1_rd));
    sdp_ram #(.AW(ROM2_AW), .DW(8)) u_rom2 (.clk, .we(ld_we && ld_sel == 3'd1), .wa(ld_addr[ROM2_AW-1:0]), .wd(ld_data),
                                      .ra(bram_a_rom2[ROM2_AW-1:0]), .rd(rom2_rd));
    sdp_ram #(.AW(15), .DW(8)) u_sram (.clk, .we(sram_we), .wa(sram_wa), .wd(sram_wd),
                                      .ra(bram_a[14:0]), .rd(sram_rd));
    sdp_ram #(.AW(10), .DW(8)) u_ram  (.clk, .we(ram_we), .wa(ram_wa), .wd(ram_wd),
                                      .ra(10'(bram_a[15:0] - 16'hfb80)), .rd(ram_rd));
    generate
        if (JV880_RAMS) begin : g_jv880_rams
            sdp_ram #(.AW(15), .DW(8)) u_nvram (.clk, .we(nvram_we), .wa(nvram_wa), .wd(nvram_wd),
                                               .ra(bram_a[14:0]), .rd(nvram_rd));
            sdp_ram #(.AW(15), .DW(8)) u_nvram_dump (.clk, .we(nvram_we), .wa(nvram_wa), .wd(nvram_wd),
                                                    .ra(nv_dump_a), .rd(nv_dump_d));
            assign nv_wrote = nvram_we;
            // Card RAM (pages 14/15): no card is fitted. The reference keeps 32 KB that reads 0
            // until written; its firmware only reads it (power-on, factory preset and jv880.mid:
            // 6,311 reads, all 0, no writes; cores/jv880/docs/plan.md), so the core reads 0 and
            // its writes (card_we) go nowhere, without the 32 KB of block RAM.
            assign card_rd = 8'h00;
        end else begin : g_no_jv880_rams
            assign nvram_rd = 8'hff;
            assign card_rd = 8'hff;
            assign nv_dump_d = 8'hff;
            assign nv_wrote = 1'b0;
        end
    endgenerate
    // JV-880: a byte that arrives while the firmware has the serial receiver off (SCR.RE, dev
    // 0x5a bit 4) is lost, as on the H8/532. The reference keeps it and delivers it later; a
    // controller's MIDI clock during boot then leaves the JV-880 deaf to notes (the SC-55 keeps the
    // reference's behaviour).
    wire rx_take = midi_valid && !midi_full && (!jv880 || dev[7'h5a][4]);
    sdp_ram #(.AW(RXBUF_BITS), .DW(8)) u_rxbuf (.clk, .we(rx_take), .wa(rx_wp), .wd(midi_data),
                                      .ra(rx_rp), .rd(rx_rd));

    always_ff @(posedge clk) begin : b_sc55_mcu_1
        logic [7:0]  d;
        logic [1:0]  t;
        logic [2:0]  rs;
        logic [3:0]  ra;
        logic        any;

        c_ack <= 1'b0;
        c_step <= 1'b0;
        c_reset <= 1'b0;
        step_done <= 1'b0;
        reset_done <= 1'b0;
        pcm_tick <= 1'b0;
        lcd_we <= 1'b0;
        tx_stb <= 1'b0;
        sm_run <= 1'b0;
        sm_reset <= 1'b0;
        sm_dly_we <= 1'b0;

        if (rx_take) rx_wp <= rx_wp + 1'b1;
        if (sm_rx_pop) rx_rp <= rx_rp + 1'b1;
        // the block RAMs read `bram_a` this clock
        if (stream) begin spec_a <= c_next2; spec_v <= 1'b1; end
        else if (rd_b || hit) begin spec_a <= c_next; spec_v <= 1'b1; end
        else if (accept) spec_v <= 1'b0;
        if (ld_we || q == Q_CLEAR || (accept && c_we)) spec_v <= 1'b0;
        // GA lines (MCU_GA_SetGAInt(line, v)): `line` triggers on a rising edge when enabled; the
        // request (IRQ1, or IRQ0 on the JV-880) = trigger != 0. Line 5: the sub-MCU (mk2) or the
        // PCM's IRQ (JV-880); lines 3/4: an encoder detent (set 0, then 1: always an edge).
        begin : b_sc55_mcu_ga
            logic [2:0] tg;
            logic [7:0] ln;
            tg = ga_trigger;
            ln = ga_line;
            if (sm_ga5_we || (jv880 && pcm_irq0 != ga_line[5])) begin
                if ((mk2 ? sm_ga5_val : pcm_irq0) && !ln[5] && ga_en[5]) tg = 3'd5;
                ln[5] = mk2 ? sm_ga5_val : pcm_irq0;
            end
            if (jv880 && enc_we) begin
                if (ga_en[enc_dir ? 4 : 3]) tg = enc_dir ? 3'd4 : 3'd3;
                ln[enc_dir ? 4 : 3] = 1'b1;
            end
            if (tg != ga_trigger || ln != ga_line) begin
                ga_trigger <= tg;
                int_req[2] <= tg != 3'd0;
                ga_line <= ln;
            end
        end
        rx_wp_d1 <= rx_wp;
        rx_wp_d2 <= rx_wp_d1;

        if (rst) begin
            rd_b <= 1'b0;
            spec_v <= 1'b0;
            q <= Q_CLEAR;
            clr_a <= 16'd0;
            cycles <= 64'd0;
            pcm_req <= 1'b0;
            sm_req <= 1'b0;
            rom2x_req <= 1'b0;
            ga_en <= 8'h00; ga_line <= 8'h00;
            sm_tgt5 <= 64'd0; sm_wp <= '0;
            rx_wp <= '0; rx_rp <= '0; rx_wp_d1 <= '0; rx_wp_d2 <= '0;
            for (int i = 0; i < 128; i++) dev[i] <= 8'h00;
            int_req <= 21'h0;
            ga_lcd_counter <= 9'd0; ga_trigger <= 3'd0; ga_int1 <= 1'b0; io_sd <= 8'h00;
            led_arm <= 2'b00; jv_leds <= 10'h000;
            uart_rx_byte <= 8'h00; uart_rx_delay <= 64'd0; uart_tx_delay <= 64'd0; ssr_rd <= 8'h00; tdr_written <= 1'b0;
            analog_end <= 64'd0; adf_rd <= 1'b0; tempreg <= 8'h00;
            lcd_enable <= 1'b0;
        end else begin
            // ------------------------------------------------ CPU bus slave
            if (pcm_req) begin
                if (pcm_ack) begin
                    pcm_req <= 1'b0;
                    c_rdata <= pcm_rdata;
                    c_ack <= 1'b1;
                end
            end else if (rom2x_req) begin
                if (rom2x_ack) begin
                    rom2x_req <= 1'b0;
                    c_rdata <= rom2x_data;
                    c_ack <= 1'b1;
                end
            end else if (sm_req) begin
                if (sm_ack) begin
                    sm_req <= 1'b0;
                    c_rdata <= sm_rdata;
                    c_ack <= 1'b1;
                end
            end else if (rd_b) begin
                // second clock of a read: the block RAMs' data is here
                case (rsel)
                    3'd1: c_rdata <= rom1_rd;
                    3'd2: c_rdata <= rom2_rd;
                    3'd3: c_rdata <= sram_rd;
                    3'd4: c_rdata <= ram_rd;
                    3'd5: c_rdata <= nvram_rd;
                    3'd6: c_rdata <= card_rd;
                    default: c_rdata <= rd_dev;
                endcase
                c_ack <= 1'b1;
                rd_b <= 1'b0;
            end else if (stream) begin
                // the next byte of a sequential read burst (its request comes with this acknowledge)
                case (bram_rs_n)
                    3'd1: c_rdata <= rom1_rd;
                    3'd2: c_rdata <= rom2_rd;
                    3'd3: c_rdata <= sram_rd;
                    3'd5: c_rdata <= nvram_rd;
                    3'd6: c_rdata <= card_rd;
                    default: c_rdata <= ram_rd;
                endcase
                c_ack <= 1'b1;
            end else if (hit) begin
                // the speculative read was this address: its data is here now
                case (bram_rs)
                    3'd1: c_rdata <= rom1_rd;
                    3'd2: c_rdata <= rom2_rd;
                    3'd3: c_rdata <= sram_rd;
                    3'd5: c_rdata <= nvram_rd;
                    3'd6: c_rdata <= card_rd;
                    default: c_rdata <= ram_rd;
                endcase
                c_ack <= 1'b1;
            end else if (c_req && !c_ack && !sm_wait) begin
                d = 8'hff;
                rs = 3'd0;
                if (!c_we) begin
                    // MCU_Read
                    case (page)
                        4'd0: begin
                            if (!a16[15]) rs = 3'd1;
                            else if (in_pcm) begin
                                pcm_req <= 1'b1; pcm_we <= 1'b0; pcm_addr <= a16[5:0];
                            end else if (in_sm) begin
                                sm_req <= 1'b1; sm_we <= 1'b0; sm_addr <= a16[7:0];
                            end else if (a16 >= 16'hff80) begin
                                // MCU_DeviceRead
                                ra = 4'd0;
                                if (a16[6:0] >= 7'h10 && a16[6:0] < 7'h40) begin
                                    t = 2'(a16[5:4] - 2'd1);
                                    if (a16[6:4] > 3'd3) d = 8'hff;
                                    else case (a16[3:0])
                                        4'h0: d = frt_tcr[t];
                                        4'h1: begin d = frt_tcsr[t]; frt_status_rd[t] <= frt_status_rd[t] | (frt_tcsr[t] & 8'hf0); end
                                        4'h2: begin tempreg <= frt_frc[t][7:0];  d = frt_frc[t][15:8]; end
                                        4'h4: begin tempreg <= frt_ocra[t][7:0]; d = frt_ocra[t][15:8]; end
                                        4'h6: begin tempreg <= frt_ocrb[t][7:0]; d = frt_ocrb[t][15:8]; end
                                        4'h8: begin tempreg <= frt_icr[t][7:0];  d = frt_icr[t][15:8]; end
                                        4'h3, 4'h5, 4'h7, 4'h9: d = tempreg;
                                        default: d = 8'hff;
                                    endcase
                                end else if (a16[6:0] >= 7'h50 && a16[6:0] < 7'h55) begin
                                    case (a16[2:0])
                                        3'd0: d = tmr_tcr;
                                        3'd1: begin d = tmr_tcsr; tmr_status_rd <= tmr_status_rd | (tmr_tcsr & 8'he0); end
                                        3'd2: d = tmr_tcora;
                                        3'd3: d = tmr_tcorb;
                                        default: d = tmr_tcnt;
                                    endcase
                                end else begin
                                    case (a16[6:0])
                                        7'h68: begin adf_rd <= dev[7'h68][7]; d = dev[7'h68]; end   // ADCSR
                                        7'h5c: begin ssr_rd <= dev[7'h5c]; d = dev[7'h5c]; end      // SSR
                                        7'h5d: d = mk2 ? sm_rx_byte : uart_rx_byte;                 // RDR
                                        7'h00: d = 8'hff;
                                        7'h0e: begin                                                // P7DR: the JV-880's panel
                                            d = 8'hff;
                                            if (jv880) begin
                                                if (io_sd == 8'b11111011) d = d & ~{3'b000, buttons[4:0]};
                                                if (io_sd == 8'b11110111) d = d & ~{3'b000, buttons[9:5]};
                                                if (io_sd == 8'b11101111) d = d & ~{4'b0000, buttons[13:10]};
                                                d = d | 8'h80;
                                            end
                                        end
                                        7'h7f: d = ((mk2 && !sc155) ? (8'h02 & ~dev[7'h7e]) : 8'h00) | (dev[7'h7f] & dev[7'h7e]);   // P9DR
                                        default: d = dev[a16[6:0]];
                                    endcase
                                end
                            end else if (a16 >= 16'hfb80 && dev[7'h79][7]) rs = 3'd4;
                            else if (a16 < 16'he000) rs = 3'd3;
                            else if (nm1) begin
                                if (a16 == (ga_base | 16'h0402)) begin
                                    d = {5'h0, ga_trigger};
                                    ga_trigger <= 3'd0;
                                    int_req[2] <= 1'b0;
                                end
                            end else if (a16 >= 16'hf000 && a16 < 16'hf100) begin
                                io_sd <= a16[7:0];
                                lcd_enable <= a16[3];
                                d = 8'hff;
                                if (!a16[0]) d = d & ~buttons[7:0];
                                if (!a16[1]) d = d & ~buttons[15:8];
                                if (!a16[2]) d = d & ~buttons[23:16];
                                if (!a16[3]) d = d & ~buttons[31:24];
                            end else if (a16 == 16'hf106) begin
                                d = {5'h0, ga_trigger};
                                ga_trigger <= 3'd0;
                                int_req[2] <= 1'b0;
                            end
                        end
                        4'd1, 4'd2, 4'd3, 4'd4: rs = 3'd2;
                        4'd8, 4'd9: if (!jv880) rs = 3'd2;
                        4'd14, 4'd15: rs = jv880 ? 3'd6 : 3'd2;
                        4'd5: if (!nm1) rs = 3'd3;
                        4'd10, 4'd11: if (nm1) rs = 3'd3;
                        4'd12, 4'd13: if (jv880) rs = 3'd5; else d = 8'hff;
                        default: d = 8'h00;
                    endcase
                    if (rs == 3'd2 && rom2_ext) begin
                        rom2x_req <= 1'b1;            // ROM2's upper half, outside
                        rom2x_addr <= rom2_a19[17:0];
                    end else if (!in_pcm && !in_sm) begin
                        rd_dev <= d;
                        rsel <= rs;
                        rd_b <= 1'b1;
                    end
                end else begin
                    // MCU_Write
                    d = c_wdata;
                    if (page == 4'd0 && a16[15]) begin
                        if (in_pcm) begin
                            pcm_req <= 1'b1; pcm_we <= 1'b1; pcm_addr <= a16[5:0]; pcm_wdata <= d;
                        end else if (in_sm) begin
                            sm_req <= 1'b1; sm_we <= 1'b1; sm_addr <= a16[7:0]; sm_wdata <= d;
                        end else if (nm1 && a16 >= (ga_base | 16'h0400) && a16 < (ga_base | 16'h0800)) begin
                            if (a16 == (ga_base | 16'h0404) || a16 == (ga_base | 16'h0405)) begin lcd_we <= 1'b1; lcd_rs <= a16[0]; lcd_data <= d; end
                            else if (a16 == (ga_base | 16'h0401)) begin
                                io_sd <= d; lcd_enable <= !d[0];
                                led_arm <= {jv880 && d == 8'hfd, jv880 && d == 8'hfe};
                            end
                            else if (a16 == (ga_base | 16'h0402)) ga_en <= {d[6:0], 1'b0};
                        end else if (a16 >= 16'hff80) begin
                            // MCU_DeviceWrite
                            if (a16[6:0] >= 7'h10 && a16[6:0] < 7'h40) begin
                                t = 2'(a16[5:4] - 2'd1);
                                case (a16[3:0])
                                    4'h0: begin
                                        frt_tcr[t] <= d;
                                        frt_stride[t] <= frt_stride_for(d[1:0], nm1);
                                        frt_deadline[t] <= align_fwd(tcyc, {8'h0, frt_stride_for(d[1:0], nm1)});
                                    end
                                    4'h1: begin : b_sc55_mcu_2
                                        logic [7:0] ts, sr;
                                        ts = {frt_tcsr[t][7:4], d[3:0]};
                                        sr = frt_status_rd[t];
                                        if (!d[4] && sr[4]) begin ts[4] = 1'b0; sr[4] = 1'b0; int_req[6 + 4 * t] <= 1'b0; end
                                        if (!d[5] && sr[5]) begin ts[5] = 1'b0; sr[5] = 1'b0; int_req[4 + 4 * t] <= 1'b0; end
                                        if (!d[6] && sr[6]) begin ts[6] = 1'b0; sr[6] = 1'b0; int_req[5 + 4 * t] <= 1'b0; end
                                        frt_tcsr[t] <= ts;
                                        frt_status_rd[t] <= sr;
                                    end
                                    4'h2, 4'h4, 4'h6, 4'h8: tempreg <= d;
                                    4'h3: frt_frc[t]  <= {tempreg, d};
                                    4'h5: frt_ocra[t] <= {tempreg, d};
                                    4'h7: frt_ocrb[t] <= {tempreg, d};
                                    4'h9: frt_icr[t]  <= {tempreg, d};
                                    default: ;
                                endcase
                            end else if (a16[6:0] >= 7'h50 && a16[6:0] < 7'h55) begin
                                case (a16[2:0])
                                    3'd0: begin
                                        tmr_tcr <= d;
                                        tmr_stride <= tmr_stride_for(d[2:0], nm1);
                                        if (tmr_stride_for(d[2:0], nm1) == 16'd0) tmr_deadline <= 64'hffffffffffffffff;
                                        else tmr_deadline <= align_fwd(tcyc, tmr_stride_for(d[2:0], nm1));
                                    end
                                    3'd1: begin : b_sc55_mcu_3
                                        logic [7:0] ts, sr;
                                        ts = {tmr_tcsr[7:4], d[3:0]};
                                        sr = tmr_status_rd;
                                        if (!d[5] && sr[5]) begin ts[5] = 1'b0; sr[5] = 1'b0; int_req[17] <= 1'b0; end
                                        if (!d[6] && sr[6]) begin ts[6] = 1'b0; sr[6] = 1'b0; int_req[15] <= 1'b0; end
                                        if (!d[7] && sr[7]) begin ts[7] = 1'b0; sr[7] = 1'b0; int_req[16] <= 1'b0; end
                                        tmr_tcsr <= ts;
                                        tmr_status_rd <= sr;
                                    end
                                    3'd2: tmr_tcora <= d;
                                    3'd3: tmr_tcorb <= d;
                                    default: tmr_tcnt <= d;
                                endcase
                            end else if (a16[6:0] == 7'h68) begin : b_sc55_mcu_4
                                // ADCSR
                                logic [7:0] v;
                                v = {dev[7'h68][7], d[6:0]};
                                if (!d[7] && adf_rd) begin v[7] = 1'b0; int_req[18] <= 1'b0; end
                                if (!d[6]) int_req[18] <= 1'b0;
                                dev[7'h68] <= v;
                            end else if (a16[6:0] == 7'h5c) begin
                                // SSR: flag clears, then the whole register is overwritten (reference behaviour)
                                // JV-880: the SCI is double-buffered, so the byte time starts when TDRE sets
                                // (below), not here (ref/patch_ref.py FIXES)
                                if (!d[7] && ssr_rd[7]) begin if (!jv880) uart_tx_delay <= cycles + 64'd3000; int_req[20] <= 1'b0; end
                                // MIDI out: TDR written, then TDRE written 0 (the SCI is double-buffered;
                                // patch_ref.py's tx hook)
                                if (!d[7] && tdr_written) begin tx_stb <= 1'b1; tx_byte <= dev[7'h5b]; tdr_written <= 1'b0; end
                                if (!d[6] && ssr_rd[6]) begin
                                    uart_rx_delay <= cycles + 64'd3000; int_req[19] <= 1'b0;
                                    sm_dly_we <= mk2; sm_dly_wv <= cycles + 64'd3000;   // mk2: the sub-MCU's (shared) delay
                                end
                                dev[7'h5c] <= d;
                            end else begin
                                dev[a16[6:0]] <= d;
                                if (a16[6:0] == 7'h5b) tdr_written <= 1'b1;
                                if (a16[6:0] == 7'h0e) begin               // P7DR: a JV-880 LED row
                                    if (led_arm[0]) jv_leds[4:0] <= d[4:0];
                                    if (led_arm[1]) jv_leds[9:5] <= d[4:0];
                                    led_arm <= 2'b00;
                                end
                                // SCR, JV-880: TXI is requested while TIE and TDRE are both 1 (ref/patch_ref.py FIXES)
                                if (a16[6:0] == 7'h5a && jv880) int_req[20] <= d[7] && dev[7'h5c][7];
                            end
                        end else if (a16 >= 16'hfb80 && dev[7'h79][7]) ;   // on-chip RAM: write port below
                        else if (a16 < 16'he000) ;                          // SRAM: write port below
                        else if (nm1) ;
                        else if (a16 >= 16'hf000 && a16 < 16'hf100) begin io_sd <= a16[7:0]; lcd_enable <= a16[3]; end
                        else if (a16 == 16'hf105) begin lcd_we <= 1'b1; lcd_rs <= 1'b0; lcd_data <= d; ga_lcd_counter <= 9'd500; end
                        else if (a16 == 16'hf104) begin lcd_we <= 1'b1; lcd_rs <= 1'b1; lcd_data <= d; ga_lcd_counter <= 9'd500; end
                        else if (a16 == 16'hf107) io_sd <= d;
                    end
                    if (!in_pcm && !in_sm)
                        c_ack <= 1'b1;
                end
            end

            // ------------------------------------------------ step sequencing
            case (q)
                Q_IDLE: begin
                    if (reset_start) begin
                        // MCU_Reset: devices (MCU_DeviceReset, TIMER_Reset), then the CPU
                        for (int i = 0; i < 3; i++) begin
                            frt_deadline[i] <= 64'd0; frt_tcr[i] <= 8'h00; frt_tcsr[i] <= 8'h00;
                            frt_frc[i] <= 16'h0; frt_ocra[i] <= 16'hffff; frt_ocrb[i] <= 16'hffff;
                            frt_icr[i] <= 16'h0; frt_status_rd[i] <= 8'h00; frt_stride[i] <= 8'd4;
                        end
                        tmr_deadline <= 64'hffffffffffffffff; tmr_stride <= 16'd0; tmr_tcr <= 8'h00;
                        tmr_tcsr <= 8'h10; tmr_tcora <= 8'hff; tmr_tcorb <= 8'hff; tmr_tcnt <= 8'h00; tmr_status_rd <= 8'h00;
                        dev[7'h00] <= 8'h03; dev[7'h02] <= 8'h00; dev[7'h7c] <= 8'h87;
                        dev[7'h01] <= 8'he0; dev[7'h03] <= 8'he0;
                        dev[7'h04] <= 8'h00; dev[7'h06] <= 8'h00; dev[7'h05] <= 8'h00; dev[7'h07] <= 8'h00;
                        dev[7'h08] <= 8'h00; dev[7'h0a] <= 8'h00; dev[7'h09] <= 8'hf0; dev[7'h0b] <= 8'hf0;
                        dev[7'h0c] <= 8'h00; dev[7'h0e] <= 8'h00; dev[7'h0f] <= 8'h00;
                        dev[7'h7e] <= 8'h00; dev[7'h7f] <= 8'h00;
                        dev[7'h5d] <= 8'h00; dev[7'h5b] <= 8'hff; dev[7'h58] <= 8'h04; dev[7'h5a] <= 8'h0c;
                        dev[7'h5c] <= 8'h87; dev[7'h59] <= 8'hff;
                        for (int i = 0; i <= 8; i++) dev[7'h60 + 7'(i)] <= 8'h00;
                        for (int i = 0; i <= 7; i++) dev[7'h70 + 7'(i)] <= 8'h00;
                        dev[7'h78] <= 8'hf3; dev[7'h79] <= 8'h80;
                        q <= Q_RESET;
                    end else if (run_step) begin
                        c_step <= 1'b1;
                        q <= Q_CPU;
                    end
                end
                Q_CLEAR: begin
                    clr_a <= clr_a + 16'd1;
                    if (clr_a == 16'h7fff) q <= Q_IDLE;
                end
                Q_RESET: begin c_reset <= 1'b1; q <= Q_RESET_CPU; end
                Q_RESET_CPU: if (c_done) begin
                    if (mk2) begin sm_reset <= 1'b1; q <= Q_RESET_SM; end
                    else begin reset_done <= 1'b1; q <= Q_IDLE; end
                end
                Q_RESET_SM: if (sm_reset_done) begin reset_done <= 1'b1; q <= Q_IDLE; end

                Q_CPU: if (c_done) begin
                    cycles <= cycles + 64'd12;
                    pcm_tick <= 1'b1;
                    q <= Q_TIMERS;
                end

                Q_TIMERS: begin : b_sc55_mcu_5
                    // TIMER_Clock: each timer clocks while its deadline is behind cycles/2, up to two
                    // clocks per timer per pass (the mk1's shortest stride is 4 timer cycles and a step is
                    // 6); when no timer is left behind, UART, A/D and gate array run in the same clock.
                    logic [15:0] frc_n [3];
                    logic [7:0]  ts_n [3], cnt_n, tts_n;
                    logic [63:0] dl_n [3], tdl_n;
                    logic        behind;
                    behind = 1'b0;
                    for (int i = 0; i < 3; i++) begin
                        frc_n[i] = frt_frc[i]; ts_n[i] = frt_tcsr[i]; dl_n[i] = frt_deadline[i];
                        for (int k = 0; k < 3; k++) begin
                            if (dl_n[i] < tcyc) begin : b_sc55_mcu_6
                                logic ma, mb;
                                ma = frc_n[i] == frt_ocra[i];
                                mb = frc_n[i] == frt_ocrb[i];
                                if (ts_n[i][0] && ma) frc_n[i] = 16'h0;
                                else begin
                                    frc_n[i] = frc_n[i] + 16'd1;
                                    if (frc_n[i] == 16'h0) ts_n[i][4] = 1'b1;
                                end
                                if (ma) ts_n[i][5] = 1'b1;
                                if (mb) ts_n[i][6] = 1'b1;
                                if (frt_tcr[i][4] && ts_n[i][4]) int_req[6 + 4 * i] <= 1'b1;
                                if (frt_tcr[i][5] && ts_n[i][5]) int_req[4 + 4 * i] <= 1'b1;
                                if (frt_tcr[i][6] && ts_n[i][6]) int_req[5 + 4 * i] <= 1'b1;
                                dl_n[i] = dl_n[i] + {56'h0, frt_stride[i]};
                            end
                        end
                        if (dl_n[i] < tcyc) behind = 1'b1;
                        frt_frc[i] <= frc_n[i]; frt_tcsr[i] <= ts_n[i]; frt_deadline[i] <= dl_n[i];
                    end
                    cnt_n = tmr_tcnt; tts_n = tmr_tcsr; tdl_n = tmr_deadline;
                    for (int k = 0; k < 3; k++) begin
                        if (tdl_n < tcyc) begin : b_sc55_mcu_7
                            logic ma, mb;
                            ma = cnt_n == tmr_tcora;
                            mb = cnt_n == tmr_tcorb;
                            if (tmr_tcr[4:3] == 2'b01 && ma) cnt_n = 8'h0;
                            else if (tmr_tcr[4:3] == 2'b10 && mb) cnt_n = 8'h0;
                            else begin
                                cnt_n = cnt_n + 8'd1;
                                if (cnt_n == 8'h0) tts_n[5] = 1'b1;
                            end
                            if (ma) tts_n[6] = 1'b1;
                            if (mb) tts_n[7] = 1'b1;
                            if (tmr_tcr[5] && tts_n[5]) int_req[17] <= 1'b1;
                            if (tmr_tcr[6] && tts_n[6]) int_req[15] <= 1'b1;
                            if (tmr_tcr[7] && tts_n[7]) int_req[16] <= 1'b1;
                            tdl_n = tdl_n + {48'h0, tmr_stride};
                        end
                    end
                    if (tdl_n < tcyc) behind = 1'b1;
                    tmr_tcnt <= cnt_n; tmr_tcsr <= tts_n; tmr_deadline <= tdl_n;

                    if (!behind) begin
                        // MCU_UpdateUART_RX / TX (mk1; on the mk2 the sub-MCU runs instead, below)
                        if (!mk2 && dev[7'h5a][4] && rx_ready && !dev[7'h5c][6] && !(cycles < uart_rx_delay)) begin
                            uart_rx_byte <= rx_rd;
                            rx_rp <= rx_rp + 1'b1;
                            d = dev[7'h5c] | 8'h40;
                            int_req[19] <= dev[7'h5a][6];
                        end else d = dev[7'h5c];
                        if (!mk2 && dev[7'h5a][5] && !d[7] && !(cycles < uart_tx_delay)) begin
                            d = d | 8'h80;
                            if (jv880) uart_tx_delay <= cycles + 64'd3000;   // shift register busy one byte time
                            int_req[20] <= dev[7'h5a][7];
                        end
                        dev[7'h5c] <= d;

                        // MCU_UpdateAnalog
                        d = dev[7'h68];
                        if (d[5]) begin
                            if (analog_end == 64'd0) analog_end <= cycles + 64'd200;
                            else if (analog_end < cycles) begin
                                if (d[4]) begin
                                    begin dev[7'h60] <= adc_hi(an_v[{d[2], 2'd0}]); dev[7'h61] <= adc_lo(an_v[{d[2], 2'd0}]); end
                                    if (2'd1 <= d[1:0]) begin dev[7'h62] <= adc_hi(an_v[{d[2], 2'd1}]); dev[7'h63] <= adc_lo(an_v[{d[2], 2'd1}]); end
                                    if (2'd2 <= d[1:0]) begin dev[7'h64] <= adc_hi(an_v[{d[2], 2'd2}]); dev[7'h65] <= adc_lo(an_v[{d[2], 2'd2}]); end
                                    if (2'd3 <= d[1:0]) begin dev[7'h66] <= adc_hi(an_v[{d[2], 2'd3}]); dev[7'h67] <= adc_lo(an_v[{d[2], 2'd3}]); end
                                    analog_end <= cycles + 64'd200;
                                    dev[7'h68] <= d | 8'h80;
                                end else begin
                                    dev[{3'h6, 1'b0, d[1:0], 1'b0}] <= adc_hi(an_v[d[2:0]]);   // ADDRxH: 0x60 + 2 * (ch & 3)
                                    dev[{3'h6, 1'b0, d[1:0], 1'b1}] <= adc_lo(an_v[d[2:0]]);
                                    dev[7'h68] <= (d & ~8'h20) | 8'h80;
                                    analog_end <= 64'd0;
                                end
                                if (d[6]) int_req[18] <= 1'b1;
                            end
                        end else analog_end <= 64'd0;

                        // mk1 gate array: LCD busy counter -> GA line 1 -> IRQ1
                        if (!nm1 && ga_lcd_counter != 9'd0) begin
                            ga_lcd_counter <= ga_lcd_counter - 9'd1;
                            if (ga_lcd_counter == 9'd1) begin
                                ga_trigger <= 3'd1;
                                ga_int1 <= 1'b1;
                                int_req[2] <= 1'b1;
                            end
                        end
                        if (mk2 && !sm_caught_up) begin
                            q <= Q_SM;                      // the last step's SM_Update is still running
                        end else begin
                            if (mk2) begin                  // SM_Update(cycles), alongside the next step
                                sm_tgt5 <= (cycles << 2) + cycles;
                                sm_wp <= rx_wp_d2;
                            end
                            step_done <= 1'b1;
                            q <= Q_IDLE;
                        end
                    end
                end
                Q_SM: if (sm_caught_up) begin
                    sm_tgt5 <= (cycles << 2) + cycles;
                    sm_wp <= rx_wp_d2;
                    step_done <= 1'b1;
                    q <= Q_IDLE;
                end
                default: q <= Q_IDLE;
            endcase
        end
    end

endmodule
