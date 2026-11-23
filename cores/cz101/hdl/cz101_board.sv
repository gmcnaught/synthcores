// Casio CZ-101 main board, after MAME's cz101 driver (BSD-3-Clause, Dirk Best, Devin Acker):
// uPD7811, program ROM, battery RAM, RA-3 slot (empty), LED latches, key matrix, HD44780,
// uPD933 and the 2 MHz serial clock.
//
// Virtual time. The CPU core counts machine cycles (vcyc); MAME's CPU cycle is
// P = 299,999,940,000 as. An event at time T is applied before the instruction starting at
// cycle s iff T < (s + 1) P (MAME's scheduler, see docs/plan.md). At every boundary the core
// waits (ev_done) while this board applies the events due:
//   - 2 MHz SCK edges (clock_device: edge k at k x 250 ns, level k & 1): edge k is due when
//     floor(k E / P) <= s, E = 250,000,000,000 as. In units of 20,000 as: E = 12,500,000 and
//     P = 14,999,997, so the edge cycles are generated exactly by a remainder counter.
//   - uPD933 IRQ changes caused by the CPU's own accesses (MAME queues them through
//     synchronize(): applied at the next boundary).
// The uPD933 runs on 10 MHz ticks: an access at cycle a is at tick floor(a P / 1e11)
// = 3a - ceil(3a / 5,000,000).

module cz101_board #(
    parameter string DECODE_HEX = "upd7810_decode.hex",
    parameter string TABLE_DIR = "./"
) (
    input  logic        clk,
    input  logic        reset,          // power-on reset of the whole board
    input  logic        cyc_ce,         // one CPU machine cycle of real time (or 1: run free)

    // ROM pack load (before reset is released): 0000-7FFF program ROM, 8000-8FFF RAM image
    input  logic        load_wr,
    input  logic [15:0] load_addr,
    input  logic [7:0]  load_data,

    // inputs
    input  logic        power_on,       // Power switch (port B bit 7)
    input  logic [7:0]  keys [16],      // key matrix rows, active low (MAME kc0-kc15)
    input  logic [7:0]  pitch_wheel,    // AN1, 7F = centre
    // MIDI in line changes as events: applied before the instruction starting at cycle s when
    // rx_cycle <= s, and before SCK edge k when rx_edge < k (rx_edge = SCK edges strictly
    // before the change). Live input: rx_cycle = rx_edge = 0 (applied at the next boundary).
    input  logic        rx_valid,
    input  logic [31:0] rx_cycle,
    input  logic [31:0] rx_edge,
    input  logic        rx_level,
    output logic        rx_pop,
    output logic        midi_tx,

    // outputs
    output logic [15:0] leds,           // MAME m_leds[0..15]
    output logic [7:0]  led_latch [4],  // raw LED latches (envelope / tone matrices)
    output logic        lcd_power,      // MAME m_power: the LCD and LEDs are lit
    output logic        sample_valid,
    output logic signed [15:0] sample,
    output logic signed [18:0] sample_sum,   // before clamping (MAME's CZ_SAMPLES)

    // LCD render port
    input  logic        r_line,
    input  logic [3:0]  r_pos,
    output logic [7:0]  r_code,
    input  logic [2:0]  r_cg_code,
    input  logic [2:0]  r_cg_row,
    output logic [4:0]  r_cg_bits,
    output logic        lcd_display_on,

    // trace
    output logic        insn_done,
    output logic [6:0]  insn_cc,
    output logic [15:0] insn_pc,
    output logic [7:0]  insn_op,
    output logic [7:0]  insn_op2,
    output logic        halt_cycle,
    output logic        irq_taken,
    output logic [15:0] irq_taken_vec,
    output logic [7:0]  r_v, r_a, r_b, r_c, r_d, r_e, r_h, r_l,
    output logic [15:0] r_ea, r_sp, r_pc,
    output logic [7:0]  r_psw,
    output logic [15:0] r_irr,
    output logic        wr_done,
    output logic [15:0] wr_addr,
    output logic [7:0]  wr_data,
    output logic        upd933_late
);

    // ------------------------------------------------------------------ CPU
    logic [15:0] ext_addr;
    logic        ext_req, ext_we, ext_ack;
    logic [7:0]  ext_wdata, ext_rdata;
    logic [7:0]  pa_out, pb_out, pc_out, pd_out, pf_out;
    logic        pa_we, pb_we, pc_we, pd_we, pf_we, pa_rd;
    logic [7:0]  pa_in, pb_in;
    logic [7:0]  an [8];
    logic        sck_edge, sck_level, int1_rise, int1_line;
    logic [31:0] vcyc, acc_time;
    logic        at_boundary, ev_done;
    logic        cpu_reset;          // power-down holds the CPU in reset (MAME INPUT_LINE_RESET)
    logic        rx_line;            // m_midi_rx

    upd7811 #(.DECODE_HEX(DECODE_HEX)) cpu (
        .clk, .reset(reset || cpu_reset), .cyc_ce,
        .rom_wr(load_wr && load_addr[15:12] == 4'h0), .rom_addr(load_addr[11:0]),
        .rom_data(load_data),
        .ext_addr, .ext_req, .ext_we, .ext_wdata, .ext_rdata, .ext_ack,
        .pa_in, .pb_in, .pc_in(8'h00), .pd_in(8'h00), .pf_in(8'h00),
        .pa_out, .pb_out, .pc_out, .pd_out, .pf_out,
        .pa_we, .pb_we, .pc_we, .pd_we, .pf_we,
        .an, .rxd(rx_line), .txd(midi_tx), .sck_edge, .sck_level,
        .nmi_line(1'b0), .int1_line, .int1_rise, .pa_rd, .int2_line(1'b0),
        .vcyc, .acc_time, .at_boundary, .ev_done,
        .insn_done, .insn_cc, .insn_pc, .insn_op, .insn_op2, .halt_cycle, .irq_taken,
        .irq_taken_vec, .r_v, .r_a, .r_b, .r_c, .r_d, .r_e, .r_h, .r_l, .r_ea, .r_sp, .r_pc,
        .r_psw, .r_irr, .wr_done, .wr_addr, .wr_data
    );

    always_comb begin
        for (int i = 0; i < 8; i++) an[i] = 8'h00;
        an[1] = pitch_wheel;
        an[2] = 8'hFF;              // battery normal
    end

    // ------------------------------------------------------------------ memory
    logic [7:0] prog [0:32767];
    logic [7:0] ram  [0:4095];
    logic [7:0] prog_q, ram_q;
    always_ff @(posedge clk) begin
        if (load_wr && !load_addr[15]) prog[load_addr[14:0]] <= load_data;
        prog_q <= prog[ext_addr[14:0]];
    end
    always_ff @(posedge clk) begin
        if (load_wr && load_addr[15:12] == 4'h8) ram[load_addr[11:0]] <= load_data;
        else if (bus_st == B_IDLE && ext_req && ext_we && ext_addr[15:12] == 4'h8)
            ram[ext_addr[11:0]] <= ext_wdata;
        ram_q <= ram[ext_addr[11:0]];
    end

    // port latches as MAME's driver sees them
    logic [7:0] port_b;             // m_port_b (key row select, uPD933 /CS)
    logic       power;              // m_power

    // ------------------------------------------------------------------ uPD933
    logic        s_req, s_ack, s_settled, s_irq, s_irq_evt, s_late, s_busy;
    logic [2:0]  s_op;
    logic [7:0]  s_data, s_rd;
    logic [39:0] s_tick, now_tick, s_irq_tick;
    logic        s_valid;
    logic signed [18:0] s_sum;
    logic signed [15:0] s_out;

    upd933 #(.TABLE_DIR(TABLE_DIR)) snd (
        .clk, .rst(reset), .now_tick, .settled(s_settled),
        .req_valid(s_req), .req_op(s_op), .req_data(s_data), .req_tick(s_tick),
        .req_order(s_tick), .req_ack(s_ack), .rd_data(s_rd),
        .irq(s_irq), .irq_evt(s_irq_evt), .irq_evt_tick(s_irq_tick),
        .sample_valid(s_valid), .sample_sum(s_sum), .sample_out(s_out),
        .late(s_late), .busy(s_busy),
        .ext_timer(1'b1), .timer_arm(t_arm), .timer_n(t_n), .timer_cancel(t_cancel),
        .timer_fire(t_fire), .timer_fire_tick(now_tick)
    );

    // ------------------------------------------------------------------ uPD933 IRQ timer
    // MAME: expiry D = a P + n S, S = 112 clocks of the truncated 4.48 MHz period
    // (24,999,999,999,968 as); applied before the instruction starting at s = a + floor(n S / P).
    localparam logic [76:0] S_AS = 77'd24999999999968;
    localparam logic [38:0] P_AS = 39'd299999940000;
    logic        t_arm, t_cancel, t_fire;
    logic [31:0] t_n;
    logic [31:0] arm_time;          // access cycle of the /CS request in flight
    logic [31:0] arm_base;          // access cycle of the /CS request that armed the timer
    logic        tmr_valid;         // armed, deadline known
    logic        div_busy;
    logic [6:0]  div_i;
    logic [76:0] div_num;           // n S, shifted out MSB first
    logic [39:0] div_rem;
    logic [37:0] div_q;
    logic [31:0] tmr_cycle;         // boundary cycle the timer fires at

    always_ff @(posedge clk) begin
        if (reset) begin
            tmr_valid <= 1'b0; div_busy <= 1'b0;
        end else begin
            if (t_cancel || t_fire) tmr_valid <= 1'b0;
            if (t_arm) begin
                arm_base <= arm_time;
                div_num <= {45'h0, t_n} * S_AS;
                div_rem <= 40'h0; div_q <= 38'h0; div_i <= 7'd77;
                div_busy <= 1'b1; tmr_valid <= 1'b0;
            end else if (div_busy) begin
                logic [39:0] r;
                r = {div_rem[38:0], div_num[76]};
                div_num <= {div_num[75:0], 1'b0};
                if (r >= {1'b0, P_AS}) begin
                    div_rem <= r - {1'b0, P_AS};
                    div_q <= {div_q[36:0], 1'b1};
                end else begin
                    div_rem <= r;
                    div_q <= {div_q[36:0], 1'b0};
                end
                div_i <= div_i - 7'd1;
                if (div_i == 7'd1) begin
                    div_busy <= 1'b0;
                    tmr_valid <= 1'b1;
                end
            end
            if (div_busy && div_i == 7'd0) ;
        end
    end
    // quotient settles the clock div_busy falls
    always_comb tmr_cycle = arm_base + div_q[31:0];
    assign sample_valid = s_valid;
    assign sample = s_out;
    assign sample_sum = s_sum;
    assign upd933_late = s_late;

    // 10 MHz tick of a CPU cycle count: 3a - ceil(3a / 5,000,000)
    function automatic logic [39:0] ticks(input logic [31:0] a);
        logic [39:0] t;
        t = 40'd3 * {8'h0, a};
        return t - (t + 40'd4999999) / 40'd5000000;
    endfunction

    // (a constant division: fine in simulation; synthesis gets an incremental counter)
    always_comb now_tick = ticks(vcyc);

    // port B input: bit 4 = uPD933 /RQ (active low), bit 7 = Power
    assign pb_in = {power_on, 2'b00, !s_irq, 4'h0};

    // ------------------------------------------------------------------ LCD
    logic [7:0] lcd_db;
    logic       lcd_pc_we, lcd_reset;
    cz101_lcd lcd (
        .clk, .reset, .now(acc_time), .insn_start(vcyc),
        .pc_we(lcd_pc_we), .pc(pc_out), .lcd_reset,
        .db_in(pa_out), .db_we(pa_we), .db_rd(pa_rd), .db_out(lcd_db),
        .r_line, .r_pos, .r_code, .r_cg_code, .r_cg_row, .r_cg_bits,
        .display_on(lcd_display_on), .cursor_on(), .blink_on(), .cursor_addr()
    );
    assign pa_in = lcd_db;
    assign lcd_power = power;

    // ------------------------------------------------------------------ external bus
    typedef enum logic [2:0] { B_IDLE, B_MEM, B_SND, B_SND_WAIT, B_DONE } bus_t;
    bus_t bus_st;
    logic [7:0] rdata_q;

    // board-side uPD933 requests come from the bus and from port B / power-down
    logic        pb_pending, rst_pending;
    logic        pb_cs;
    logic [31:0] pb_time;          // acc_time of the port write

    always_ff @(posedge clk) begin
        ext_ack <= 1'b0;
        lcd_pc_we <= 1'b0;
        lcd_reset <= 1'b0;
        if (reset) begin
            bus_st <= B_IDLE;
            s_req <= 1'b0;
            port_b <= 8'h00;
            power <= 1'b0;
            cpu_reset <= 1'b0;
            led_latch[0] <= 8'hFF; led_latch[1] <= 8'hFF; led_latch[2] <= 8'hFF; led_latch[3] <= 8'hFF;
            pb_pending <= 1'b0; rst_pending <= 1'b0;
        end else begin
            if (power_on) power <= 1'b1;

            // port writes (MAME port_b_w / port_c_w), in the clock the CPU performs them
            if (pb_we) begin
                port_b <= pb_out;
                pb_pending <= 1'b1;
                pb_time <= acc_time;
                pb_cs <= pb_out[5];
            end
            if (pc_we) begin
                if (pc_out[3]) lcd_pc_we <= 1'b1;
                else begin
                    // power down: CPU held in reset, LCD and uPD933 reset, LEDs off
                    power <= 1'b0;
                    cpu_reset <= 1'b1;
                    lcd_reset <= 1'b1;
                    rst_pending <= 1'b1;
                    pb_time <= acc_time;
                    led_latch[0] <= 8'hFF; led_latch[1] <= 8'hFF; led_latch[2] <= 8'hFF; led_latch[3] <= 8'hFF;
                end
            end

            case (bus_st)
                B_IDLE: begin
                    if (pb_pending || rst_pending) begin
                        // the CPU waits for its next bus access anyway; send /CS or reset first
                        s_req <= 1'b1;
                        s_op <= rst_pending ? 3'd4 : 3'd2;
                        s_data <= {7'h0, pb_cs};
                        s_tick <= ticks(pb_time);
                        arm_time <= pb_time;
                        bus_st <= B_SND_WAIT;
                        if (rst_pending) rst_pending <= 1'b0; else pb_pending <= 1'b0;
                    end else if (ext_req && !ext_ack) begin
                        if (ext_addr >= 16'hC000 && ext_addr <= 16'hFEFF) begin
                            s_req <= 1'b1;
                            s_op <= ext_we ? 3'd0 : 3'd1;
                            s_data <= ext_wdata;
                            s_tick <= ticks(acc_time);
                            bus_st <= B_SND;
                        end else begin
                            if (ext_we) begin
                                case (ext_addr[15:11])
                                    5'b10011: led_latch[3] <= power ? ext_wdata : 8'hFF;   // 9800-9FFF
                                    5'b10100: led_latch[2] <= power ? ext_wdata : 8'hFF;   // A000-A7FF
                                    5'b10101: led_latch[1] <= power ? ext_wdata : 8'hFF;   // A800-AFFF
                                    5'b10110: led_latch[0] <= power ? ext_wdata : 8'hFF;   // B000-B7FF
                                    default: ;
                                endcase
                            end
                            bus_st <= B_MEM;
                        end
                    end
                end
                B_MEM: begin
                    // ROM / RAM data valid one clock after the address
                    if (!ext_addr[15]) rdata_q <= prog_q;
                    else if (ext_addr[15:12] == 4'h8) rdata_q <= ram_q;
                    else if (ext_addr[15:11] == 5'b10111) rdata_q <= keys[port_b[3:0]];   // B800-BFFF
                    else rdata_q <= 8'hFF;   // RA-3 slot empty, LED latches write-only
                    bus_st <= B_DONE;
                end
                B_SND: if (s_ack) begin
                    s_req <= 1'b0;
                    rdata_q <= s_rd;
                    bus_st <= B_DONE;
                end
                B_SND_WAIT: if (s_ack) begin
                    s_req <= 1'b0;
                    bus_st <= B_IDLE;
                end
                default: begin
                    ext_rdata <= rdata_q;
                    ext_ack <= 1'b1;
                    bus_st <= B_IDLE;
                end
            endcase
        end
    end

    // LEDs as MAME's led_1_w .. led_4_w (m_leds only; the matrices come from led_latch)
    always_comb begin
        for (int i = 0; i < 7; i++) leds[i] = !led_latch[3][i];
        leds[7] = !led_latch[1][7];
        for (int i = 0; i < 8; i++) leds[8 + i] = !led_latch[0][i];
    end

    // ------------------------------------------------------------------ events at boundaries
    // uPD933 IRQ to INTF1: changes are applied at the next boundary (MAME synchronize()).
    logic irq_seen_rise;   // a rising edge of the uPD933 IRQ since the last boundary
    logic irq_prev;
    logic [1:0] fire_wait;

    // SCK edges: edge k due when its cycle c_k = floor(k E / P) <= vcyc
    localparam logic [31:0] E_U = 32'd12500000, P_U = 32'd14999997;
    logic [31:0] sck_cyc;          // c_k of the next edge
    logic [31:0] sck_k;            // index of the next edge (1-based)
    logic [31:0] sck_rem;          // k E mod P (units of 20,000 as)
    logic        sck_lvl_next;     // level after the next edge

    wire sck_due = sck_cyc <= vcyc;

    always_ff @(posedge clk) begin
        sck_edge <= 1'b0;
        int1_rise <= 1'b0;
        rx_pop <= 1'b0;
        if (reset) begin
            // first edge at 250 ns: c_1 = floor(E / P) = 0, remainder E
            sck_cyc <= 32'd0; sck_rem <= E_U; sck_lvl_next <= 1'b1; sck_k <= 32'd1;
            rx_line <= 1'b1;
            irq_prev <= 1'b0; irq_seen_rise <= 1'b0; int1_line <= 1'b0;
            ev_done <= 1'b0; t_fire <= 1'b0; fire_wait <= 2'd0;
        end else begin
            if (s_irq && !irq_prev) irq_seen_rise <= 1'b1;
            irq_prev <= s_irq;
            ev_done <= 1'b0;
            t_fire <= 1'b0;
            if (t_fire) fire_wait <= 2'd3;                 // let the IRQ line settle
            else if (fire_wait != 2'd0) fire_wait <= fire_wait - 2'd1;
            if (at_boundary && !ev_done && s_settled && bus_st == B_IDLE && !pb_pending && !rst_pending
                && !div_busy && !t_arm && !t_fire && fire_wait == 2'd0) begin
                if (tmr_valid && tmr_cycle <= vcyc) begin
                    t_fire <= 1'b1;           // MAME timer_tick at D (before this instruction)
                end else if (irq_seen_rise || int1_line != s_irq) begin
                    int1_rise <= irq_seen_rise;
                    int1_line <= s_irq;
                    irq_seen_rise <= 1'b0;
                end else if (rx_valid && !rx_pop && rx_cycle <= vcyc && (!sck_due || rx_edge < sck_k)) begin
                    rx_line <= rx_level;
                    rx_pop <= 1'b1;
                end else if (sck_due) begin
                    sck_k <= sck_k + 32'd1;
                    sck_edge <= 1'b1;
                    sck_level <= sck_lvl_next;
                    sck_lvl_next <= !sck_lvl_next;
                    if (sck_rem + E_U >= P_U) begin
                        sck_rem <= sck_rem + E_U - P_U;
                        sck_cyc <= sck_cyc + 32'd1;
                    end else sck_rem <= sck_rem + E_U;
                end else ev_done <= 1'b1;
            end
        end
    end

endmodule
