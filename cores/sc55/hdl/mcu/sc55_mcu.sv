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

module sc55_mcu #(
    parameter int RXBUF_BITS = 11          // MIDI receive buffer: 2^RXBUF_BITS bytes
) (
    input  logic        clk,
    input  logic        rst,

    // one model step per run_step pulse; step_done when the devices have advanced
    input  logic        run_step,
    output logic        step_done,
    input  logic        reset_start,     // MCU_Reset (registers, devices, reset vector)
    output logic        reset_done,
    output logic        ready,           // power-on clear of SRAM and on-chip RAM finished (the model starts zeroed)

    // memory load port (ROMs, savestates): sel 0 rom1, 1 rom2, 2 sram, 3 ram
    input  logic        ld_we,
    input  logic [1:0]  ld_sel,
    input  logic [17:0] ld_addr,
    input  logic [7:0]  ld_data,

    // MIDI bytes into the UART receive buffer (MCU_PostUART)
    input  logic        midi_valid,
    input  logic [7:0]  midi_data,
    output logic        midi_full,

    // front panel
    input  logic [31:0] buttons,         // mcu.button_pressed

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

    // LCD
    output logic        lcd_we,          // LCD_Write(address = lcd_rs, data)
    output logic        lcd_rs,
    output logic [7:0]  lcd_data,
    output logic        lcd_enable,      // LCD_Enable

    output logic [47:0] cycles,

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
    logic [7:0] rom1 [32768];
    logic [7:0] rom2 [262144];
    logic [7:0] sram [32768];
    logic [7:0] ram  [1024];

    // ------------------------------------------------------------------
    // CPU
    // ------------------------------------------------------------------
    logic        c_req, c_we, c_ack, c_step, c_done, c_reset;
    logic [23:0] c_addr;
    logic [7:0]  c_wdata, c_rdata;
    logic [20:0] int_pend;
    logic [7:0]  dev [128];

    h8_cpu cpu (
        .clk, .rst,
        .step(c_step), .step_done(c_done),
        .bus_req(c_req), .bus_we(c_we), .bus_addr(c_addr), .bus_wdata(c_wdata),
        .bus_rdata(c_rdata), .bus_ack(c_ack),
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
    // mask, is not already requested, and the PCM has not settled it yet.
    logic int_hold;
    assign int_hold = !pcm_irq_known && !pcm_irq0 && dev[7'h7c][5] && (dev[7'h70][6:4] > dbg_sr[10:8]);

    assign mon_req = c_req; assign mon_we = c_we; assign mon_addr = c_addr;
    assign mon_wdata = c_wdata; assign mon_rdata = c_rdata; assign mon_ack = c_ack;

    // ------------------------------------------------------------------
    // Peripheral state
    // ------------------------------------------------------------------
    // interrupt request bits (MCU_Interrupt_Source); bit 1 (IRQ0) comes from the PCM
    logic [20:0] int_req;
    always_comb begin
        int_pend = int_req;
        int_pend[1] = pcm_irq0;
    end
    assign dbg_int = int_pend;

    // FRTs (mcu_timer.cpp frt_t)
    logic [47:0] frt_deadline [3];
    logic [7:0]  frt_tcr [3], frt_tcsr [3], frt_status_rd [3];
    logic [15:0] frt_frc [3], frt_ocra [3], frt_ocrb [3], frt_icr [3];
    logic [7:0]  frt_stride [3];
    // 8-bit timer
    logic [47:0] tmr_deadline;
    logic [15:0] tmr_stride;
    logic [7:0]  tmr_tcr, tmr_tcsr, tmr_tcora, tmr_tcorb, tmr_tcnt, tmr_status_rd;
    logic [7:0]  tempreg;
    // UART
    logic [7:0]  uart_rx_byte;
    logic [47:0] uart_rx_delay, uart_tx_delay;
    logic [7:0]  ssr_rd;
    // analog
    logic [47:0] analog_end;
    logic        adf_rd;
    // gate array (mk1)
    logic [8:0]  ga_lcd_counter;
    logic [2:0]  ga_trigger;
    logic        ga_int1;
    logic [7:0]  io_sd;

    // MIDI receive buffer (MCU_PostUART ring buffer)
    logic [7:0]  rxbuf [2**RXBUF_BITS];
    logic [RXBUF_BITS-1:0] rx_wp, rx_rp;
    assign midi_full = (rx_wp + 1'b1) == rx_rp;

    // timer step tables (mk1)
    function automatic logic [7:0] frt_stride_for(input logic [1:0] cks);
        case (cks) 2'd0: return 8'd4; 2'd1: return 8'd8; 2'd2: return 8'd32; default: return 8'd4; endcase
    endfunction
    function automatic logic [15:0] tmr_stride_for(input logic [2:0] cks);
        case (cks)
            3'd1: return 16'd8; 3'd2: return 16'd64; 3'd3: return 16'd1024;
            3'd5, 3'd6, 3'd7: return 16'd4;
            default: return 16'd0;
        endcase
    endfunction
    function automatic logic [47:0] align_fwd(input logic [47:0] v, input logic [15:0] iv);
        logic [47:0] m;
        m = {32'h0, iv} - 48'd1;
        return (v + m) & ~m;
    endfunction

    // A/D pin values (mk1 SC-55: channel 7 = battery, others read 0 through the RCU path)
    function automatic logic [9:0] analog_pin(input logic [2:0] ch);
        return (ch == 3'd7) ? 10'h2a0 : 10'h000;
    endfunction
    function automatic logic [7:0] adc_hi(input logic [2:0] ch);
        logic [9:0] v;
        v = analog_pin(ch);
        return v[9:2];
    endfunction
    function automatic logic [7:0] adc_lo(input logic [2:0] ch);
        logic [9:0] v;
        v = analog_pin(ch);
        return {v[1:0], 6'h0};
    endfunction

    // ------------------------------------------------------------------
    // Sequencer
    // ------------------------------------------------------------------
    typedef enum logic [3:0] {
        Q_IDLE, Q_CPU, Q_TIMERS, Q_RESET, Q_RESET_CPU, Q_CLEAR
    } q_t;
    q_t q;
    // two-clock reads: block RAM data registers and the source chosen in the first clock
    logic [7:0] q_rom1, q_rom2, q_sram_ram, rd_dev;
    logic [1:0] rsel;
    logic       rd_b;
    logic [15:0] clr_a;
    assign ready = (q != Q_CLEAR);

    wire [47:0] tcyc = cycles >> 1;    // timer.cycles during an instruction (= previous TIMER_Clock target)

    // bus decode
    wire [3:0]  page = c_addr[19:16];
    wire [15:0] a16  = c_addr[15:0];
    // (address & 0x3ffff) | (bit 19 ? 0x40000 : 0), then & rom2_mask (256 KB: 0x3ffff) -> address & 0x3ffff
    wire [17:0] rom2_a = c_addr[17:0];


    always_ff @(posedge clk) begin : b_sc55_mcu_1
        logic [7:0]  d;
        logic [1:0]  t;
        logic [1:0]  rs;
        logic [3:0]  ra;
        logic        any;

        c_ack <= 1'b0;
        c_step <= 1'b0;
        c_reset <= 1'b0;
        step_done <= 1'b0;
        reset_done <= 1'b0;
        pcm_tick <= 1'b0;
        lcd_we <= 1'b0;

        if (ld_we) begin
            case (ld_sel)
                2'd0: rom1[ld_addr[14:0]] <= ld_data;
                2'd1: rom2[ld_addr] <= ld_data;
                2'd2: sram[ld_addr[14:0]] <= ld_data;
                default: ram[ld_addr[9:0]] <= ld_data;
            endcase
        end

        if (midi_valid && !midi_full) begin
            rxbuf[rx_wp] <= midi_data;
            rx_wp <= rx_wp + 1'b1;
        end

        if (rst) begin
            rd_b <= 1'b0;
            q <= Q_CLEAR;
            clr_a <= 16'd0;
            cycles <= 48'd0;
            pcm_req <= 1'b0;
            rx_wp <= '0; rx_rp <= '0;
            for (int i = 0; i < 128; i++) dev[i] <= 8'h00;
            int_req <= 21'h0;
            ga_lcd_counter <= 9'd0; ga_trigger <= 3'd0; ga_int1 <= 1'b0; io_sd <= 8'h00;
            uart_rx_byte <= 8'h00; uart_rx_delay <= 48'd0; uart_tx_delay <= 48'd0; ssr_rd <= 8'h00;
            analog_end <= 48'd0; adf_rd <= 1'b0; tempreg <= 8'h00;
            lcd_enable <= 1'b0;
        end else begin
            // ------------------------------------------------ CPU bus slave
            if (pcm_req) begin
                if (pcm_ack) begin
                    pcm_req <= 1'b0;
                    c_rdata <= pcm_rdata;
                    c_ack <= 1'b1;
                end
            end else if (rd_b) begin
                // second clock of a read: the block RAMs' data is here
                case (rsel)
                    2'd0: c_rdata <= rd_dev;
                    2'd1: c_rdata <= q_rom1;
                    2'd2: c_rdata <= q_rom2;
                    default: c_rdata <= q_sram_ram;
                endcase
                c_ack <= 1'b1;
                rd_b <= 1'b0;
            end else if (c_req && !c_ack) begin
                d = 8'hff;
                rs = 2'd0;
                // block RAM reads (synchronous: the data is used next clock)
                q_rom1 <= rom1[a16[14:0]];
                q_rom2 <= rom2[rom2_a];
                if (page == 4'd0 && a16 >= 16'hfb80) q_sram_ram <= ram[10'(a16 - 16'hfb80)];
                else q_sram_ram <= sram[a16[14:0]];
                if (!c_we) begin
                    // MCU_Read (mk1)
                    case (page)
                        4'd0: begin
                            if (!a16[15]) rs = 2'd1;
                            else if (a16 >= 16'he000 && a16 < 16'he040) begin
                                pcm_req <= 1'b1; pcm_we <= 1'b0; pcm_addr <= a16[5:0];
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
                                        7'h5d: d = uart_rx_byte;                                    // RDR
                                        7'h00: d = 8'hff;
                                        7'h0e: d = 8'hff;                                           // P7DR (not JV-880)
                                        7'h7f: d = dev[7'h7f] & dev[7'h7e];                         // P9DR (mk1 cfg 0)
                                        default: d = dev[a16[6:0]];
                                    endcase
                                end
                            end else if (a16 >= 16'hfb80 && dev[7'h79][7]) rs = 2'd3;
                            else if (a16 < 16'he000) rs = 2'd3;
                            else if (a16 >= 16'hf000 && a16 < 16'hf100) begin
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
                        4'd1, 4'd2, 4'd3, 4'd4, 4'd8, 4'd9, 4'd14, 4'd15: rs = 2'd2;
                        4'd5: rs = 2'd3;
                        4'd10, 4'd11, 4'd12, 4'd13: d = 8'hff;
                        default: d = 8'h00;
                    endcase
                    if (!(page == 4'd0 && a16 >= 16'he000 && a16 < 16'he040)) begin
                        rd_dev <= d;
                        rsel <= rs;
                        rd_b <= 1'b1;
                    end
                end else begin
                    // MCU_Write (mk1)
                    d = c_wdata;
                    if (page == 4'd0 && a16[15]) begin
                        if (a16 >= 16'he000 && a16 < 16'he040) begin
                            pcm_req <= 1'b1; pcm_we <= 1'b1; pcm_addr <= a16[5:0]; pcm_wdata <= d;
                        end else if (a16 >= 16'hff80) begin
                            // MCU_DeviceWrite
                            if (a16[6:0] >= 7'h10 && a16[6:0] < 7'h40) begin
                                t = 2'(a16[5:4] - 2'd1);
                                case (a16[3:0])
                                    4'h0: begin
                                        frt_tcr[t] <= d;
                                        frt_stride[t] <= frt_stride_for(d[1:0]);
                                        frt_deadline[t] <= align_fwd(tcyc, {8'h0, frt_stride_for(d[1:0])});
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
                                        tmr_stride <= tmr_stride_for(d[2:0]);
                                        if (tmr_stride_for(d[2:0]) == 16'd0) tmr_deadline <= 48'hffffffffffff;
                                        else tmr_deadline <= align_fwd(tcyc, tmr_stride_for(d[2:0]));
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
                                if (!d[7] && ssr_rd[7]) begin uart_tx_delay <= cycles + 48'd3000; int_req[20] <= 1'b0; end
                                if (!d[6] && ssr_rd[6]) begin uart_rx_delay <= cycles + 48'd3000; int_req[19] <= 1'b0; end
                                dev[7'h5c] <= d;
                            end else begin
                                dev[a16[6:0]] <= d;
                            end
                        end else if (a16 >= 16'hfb80 && dev[7'h79][7]) ram[10'(a16 - 16'hfb80)] <= d;
                        else if (a16 < 16'he000) sram[a16[14:0]] <= d;
                        else if (a16 >= 16'hf000 && a16 < 16'hf100) begin io_sd <= a16[7:0]; lcd_enable <= a16[3]; end
                        else if (a16 == 16'hf105) begin lcd_we <= 1'b1; lcd_rs <= 1'b0; lcd_data <= d; ga_lcd_counter <= 9'd500; end
                        else if (a16 == 16'hf104) begin lcd_we <= 1'b1; lcd_rs <= 1'b1; lcd_data <= d; ga_lcd_counter <= 9'd500; end
                        else if (a16 == 16'hf107) io_sd <= d;
                    end else if (page == 4'd5) sram[a16[14:0]] <= d;
                    if (!(page == 4'd0 && a16 >= 16'he000 && a16 < 16'he040))
                        c_ack <= 1'b1;
                end
            end

            // ------------------------------------------------ step sequencing
            case (q)
                Q_IDLE: begin
                    if (reset_start) begin
                        // MCU_Reset: devices (MCU_DeviceReset, TIMER_Reset), then the CPU
                        for (int i = 0; i < 3; i++) begin
                            frt_deadline[i] <= 48'd0; frt_tcr[i] <= 8'h00; frt_tcsr[i] <= 8'h00;
                            frt_frc[i] <= 16'h0; frt_ocra[i] <= 16'hffff; frt_ocrb[i] <= 16'hffff;
                            frt_icr[i] <= 16'h0; frt_status_rd[i] <= 8'h00; frt_stride[i] <= 8'd4;
                        end
                        tmr_deadline <= 48'hffffffffffff; tmr_stride <= 16'd0; tmr_tcr <= 8'h00;
                        tmr_tcsr <= 8'h10; tmr_tcora <= 8'hff; tmr_tcorb <= 8'hff; tmr_tcnt <= 8'h00; tmr_status_rd <= 8'h00;
                        dev[7'h00] <= 8'h03; dev[7'h02] <= 8'h00; dev[7'h7c] <= 8'h87;
                        dev[7'h01] <= 8'he0; dev[7'h03] <= 8'he0;
                        dev[7'h04] <= 8'h00; dev[7'h06] <= 8'h00; dev[7'h05] <= 8'h00; dev[7'h07] <= 8'h00;
                        dev[7'h08] <= 8'h00; dev[7'h0a] <= 8'h00; dev[7'h09] <= 8'hf0; dev[7'h0b] <= 8'hf0;
                        dev[7'h0c] <= 8'h00; dev[7'h0e] <= 8'h00; dev[7'h0f] <= 8'h00;
                        dev[7'h7e] <= 8'h00; dev[7'h7f] <= 8'h00;
                        dev[7'h5d] <= 8'h00; dev[7'h5b] <= 8'hff; dev[7'h58] <= 8'h04; dev[7'h5a] <= 8'h0c;
                        dev[7'h5c] <= 8'h87; dev[7'h59] <= 8'hff;
                        for (int i = 'h60; i <= 'h68; i++) dev[7'(i)] <= 8'h00;
                        for (int i = 'h70; i <= 'h77; i++) dev[7'(i)] <= 8'h00;
                        dev[7'h78] <= 8'hf3; dev[7'h79] <= 8'h80;
                        q <= Q_RESET;
                    end else if (run_step) begin
                        c_step <= 1'b1;
                        q <= Q_CPU;
                    end
                end
                Q_CLEAR: begin
                    sram[clr_a[14:0]] <= 8'h00;
                    if (clr_a[15:10] == 6'd0) ram[clr_a[9:0]] <= 8'h00;
                    clr_a <= clr_a + 16'd1;
                    if (clr_a == 16'h7fff) q <= Q_IDLE;
                end
                Q_RESET: begin c_reset <= 1'b1; q <= Q_RESET_CPU; end
                Q_RESET_CPU: if (c_done) begin reset_done <= 1'b1; q <= Q_IDLE; end

                Q_CPU: if (c_done) begin
                    cycles <= cycles + 48'd12;
                    pcm_tick <= 1'b1;
                    q <= Q_TIMERS;
                end

                Q_TIMERS: begin : b_sc55_mcu_5
                    // TIMER_Clock: each timer clocks while its deadline is behind cycles/2, up to two
                    // clocks per timer per pass (the mk1's shortest stride is 4 timer cycles and a step is
                    // 6); when no timer is left behind, UART, A/D and gate array run in the same clock.
                    logic [15:0] frc_n [3];
                    logic [7:0]  ts_n [3], cnt_n, tts_n;
                    logic [47:0] dl_n [3], tdl_n;
                    logic        behind;
                    behind = 1'b0;
                    for (int i = 0; i < 3; i++) begin
                        frc_n[i] = frt_frc[i]; ts_n[i] = frt_tcsr[i]; dl_n[i] = frt_deadline[i];
                        for (int k = 0; k < 2; k++) begin
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
                                dl_n[i] = dl_n[i] + {40'h0, frt_stride[i]};
                            end
                        end
                        if (dl_n[i] < tcyc) behind = 1'b1;
                        frt_frc[i] <= frc_n[i]; frt_tcsr[i] <= ts_n[i]; frt_deadline[i] <= dl_n[i];
                    end
                    cnt_n = tmr_tcnt; tts_n = tmr_tcsr; tdl_n = tmr_deadline;
                    for (int k = 0; k < 2; k++) begin
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
                            tdl_n = tdl_n + {32'h0, tmr_stride};
                        end
                    end
                    if (tdl_n < tcyc) behind = 1'b1;
                    tmr_tcnt <= cnt_n; tmr_tcsr <= tts_n; tmr_deadline <= tdl_n;

                    if (!behind) begin
                        // MCU_UpdateUART_RX / TX
                        if (dev[7'h5a][4] && rx_wp != rx_rp && !dev[7'h5c][6] && !(cycles < uart_rx_delay)) begin
                            uart_rx_byte <= rxbuf[rx_rp];
                            rx_rp <= rx_rp + 1'b1;
                            d = dev[7'h5c] | 8'h40;
                            int_req[19] <= dev[7'h5a][6];
                        end else d = dev[7'h5c];
                        if (dev[7'h5a][5] && !d[7] && !(cycles < uart_tx_delay)) begin
                            d = d | 8'h80;
                            int_req[20] <= dev[7'h5a][7];
                        end
                        dev[7'h5c] <= d;

                        // MCU_UpdateAnalog
                        d = dev[7'h68];
                        if (d[5]) begin
                            if (analog_end == 48'd0) analog_end <= cycles + 48'd200;
                            else if (analog_end < cycles) begin
                                if (d[4]) begin
                                    begin dev[7'h60] <= adc_hi({d[2], 2'd0}); dev[7'h61] <= adc_lo({d[2], 2'd0}); end
                                    if (2'd1 <= d[1:0]) begin dev[7'h62] <= adc_hi({d[2], 2'd1}); dev[7'h63] <= adc_lo({d[2], 2'd1}); end
                                    if (2'd2 <= d[1:0]) begin dev[7'h64] <= adc_hi({d[2], 2'd2}); dev[7'h65] <= adc_lo({d[2], 2'd2}); end
                                    if (2'd3 <= d[1:0]) begin dev[7'h66] <= adc_hi({d[2], 2'd3}); dev[7'h67] <= adc_lo({d[2], 2'd3}); end
                                    analog_end <= cycles + 48'd200;
                                    dev[7'h68] <= d | 8'h80;
                                end else begin
                                    dev[{4'h6, d[1:0], 1'b0}] <= adc_hi(d[2:0]);
                                    dev[{4'h6, d[1:0], 1'b1}] <= adc_lo(d[2:0]);
                                    dev[7'h68] <= (d & ~8'h20) | 8'h80;
                                    analog_end <= 48'd0;
                                end
                                if (d[6]) int_req[18] <= 1'b1;
                            end
                        end else analog_end <= 48'd0;

                        // mk1 gate array: LCD busy counter -> GA line 1 -> IRQ1
                        if (ga_lcd_counter != 9'd0) begin
                            ga_lcd_counter <= ga_lcd_counter - 9'd1;
                            if (ga_lcd_counter == 9'd1) begin
                                ga_trigger <= 3'd1;
                                ga_int1 <= 1'b1;
                                int_req[2] <= 1'b1;
                            end
                        end
                        step_done <= 1'b1;
                        q <= Q_IDLE;
                    end
                end
                default: q <= Q_IDLE;
            endcase
        end
    end

endmodule
