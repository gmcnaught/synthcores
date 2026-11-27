// SPDX-License-Identifier: MIT
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
//   - the Power switch's /NMI (an input change between MAME's timeslices).
// The uPD933 runs on 10 MHz ticks: an access at cycle a is at tick floor(a P / 1e11)
// = 3a - ceil(3a / 5,000,000).

module cz101_board #(
    parameter DECODE_HEX = "upd7810_decode.hex",
    parameter TABLE_DIR = "./"
) (
    input  logic        clk,
    input  logic        reset,          // power-on reset of the whole board
    input  logic        cyc_ce,         // one CPU machine cycle of real time (or 1: run free)

    // ROM pack load (before reset is released): 0000-7FFF program ROM, 8000-8FFF RAM image
    input  logic        load_wr,
    input  logic [15:0] load_addr,
    input  logic [7:0]  load_data,

    // inputs
    input  logic        power_on,       // Power switch (port B bit 7; off asserts /NMI)
    input  logic        cart_present,   // RA-3 RAM cartridge at 9000-97FF (sense: keys[13] bit 0)
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
    // live MIDI bytes (MiSTer): each is sent to the serial input as 10 bits (start, 8 data,
    // stop) of 128 SCK edges each (31,250 baud in virtual time), back to back
    input  logic        midi_valid,
    input  logic [7:0]  midi_byte,
    output logic        midi_pop,
    output logic        midi_tx,        // the uPD7811's TxD as MAME's, moved at instruction boundaries:
                                        // an edge due inside a long instruction (DIV: 59 cycles) is
                                        // late by up to half a bit, so the MiSTer sends midi_out_byte
    output logic        midi_out_valid, // one pulse: a byte entered the TxD shift register with TxD high
                                        // (MIDI out; the firmware's boot byte, loaded while TxD is
                                        // still at its reset level 0, never shows on MAME's line)
    output logic [7:0]  midi_out_byte,

    // second port on the battery RAM (save file): 0000-0FFF RAM, 1000-17FF cartridge
    input  logic [12:0] nv_addr,
    input  logic        nv_we,
    input  logic [7:0]  nv_wdata,
    output logic [7:0]  nv_rdata,
    output logic        nv_written,     // the CPU wrote the battery RAM or cartridge this clock
    output logic        powered_down,   // the firmware switched the power off (CPU in reset)
    output logic [31:0] cycles,         // vt: machine cycles of virtual time (vcyc while the CPU runs)

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
    input  logic [2:0]  r_row,
    output logic        r_at_ac,
    output logic [4:0]  r_cg_bits,
    output logic        lcd_display_on,
    output logic        lcd_cursor_on,
    output logic        lcd_blink_on,

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
    logic        tx_load;
    assign midi_out_valid = tx_load && midi_tx;   // a load does not move TxD: the level before it
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
    logic        nmi_ev;             // /NMI (Power switch off), changed at instruction boundaries

    upd7811 #(.DECODE_HEX(DECODE_HEX)) cpu (
        .clk, .reset(reset || cpu_reset), .cyc_ce,
        .rom_wr(load_wr && load_addr[15:12] == 4'h0), .rom_addr(load_addr[11:0]),
        .rom_data(load_data),
        .ext_addr, .ext_req, .ext_we, .ext_wdata, .ext_rdata, .ext_ack,
        .pa_in, .pb_in, .pc_in(8'h00), .pd_in(8'h00), .pf_in(8'h00),
        .pa_out, .pb_out, .pc_out, .pd_out, .pf_out,
        .pa_we, .pb_we, .pc_we, .pd_we, .pf_we,
        .an, .rxd(rx_line), .txd(midi_tx), .tx_load(tx_load), .tx_byte(midi_out_byte), .sck_edge, .sck_level,
        .nmi_line(nmi_ev), .int1_line, .int1_rise, .pa_rd, .int2_line(1'b0),
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
    logic [7:0] prog_q;
    always_ff @(posedge clk) begin
        if (load_wr && !load_addr[15]) prog[load_addr[14:0]] <= load_data;
        prog_q <= prog[ext_addr[14:0]];
    end
    // true dual port: A = ROM pack load / CPU, B = save file (no read-during-write is relied on)
    (* ramstyle = "no_rw_check" *) logic [7:0] ram [0:8191];   // 0000-0FFF battery RAM, 1000-17FF RA-3
    logic [7:0] ram_q;
    // synthesis translate_off
    initial for (int i = 0; i < 8192; i++) ram[i] = 8'hFF;   // (FPGA: M10K powers up as 00)
    // synthesis translate_on
    wire        in_cart = ext_addr[15:11] == 5'b10010 && cart_present;   // 9000-97FF
    wire        ld_ram  = load_wr && load_addr[15:12] == 4'h8;
    wire [12:0] ram_a   = ld_ram ? {1'b0, load_addr[11:0]}
                        : in_cart ? {2'b10, ext_addr[10:0]} : {1'b0, ext_addr[11:0]};
    wire        ram_we  = ld_ram || (bus_st == B_IDLE && ext_req && ext_we
                                     && (ext_addr[15:12] == 4'h8 || in_cart));
    wire [7:0]  ram_d   = ld_ram ? load_data : ext_wdata;
    assign nv_written = ram_we && !ld_ram;
    always_ff @(posedge clk) begin
        if (ram_we) ram[ram_a] <= ram_d;
        ram_q <= ram[ram_a];
    end
    always_ff @(posedge clk) begin
        if (nv_we) ram[nv_addr] <= nv_wdata;
        nv_rdata <= ram[nv_addr];
    end

    // port latches as MAME's driver sees them
    logic [7:0] port_b;             // m_port_b (key row select, uPD933 /CS)
    logic       power;              // m_power
    logic       power_on_q;

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
    logic        div_busy, mul_busy;
    logic [6:0]  div_i;
    logic [31:0] mul_n;             // n, shifted out MSB first (n S = sum of S << i)
    logic [76:0] div_num;           // n S, shifted out MSB first
    logic [39:0] div_rem;
    logic [37:0] div_q;
    logic [31:0] tmr_cycle;         // boundary cycle the timer fires at, once tmr_far = 0
    // The deadline can be up to 2^37 cycles ahead (an envelope moving away from its target makes
    // MAME's remaining time wrap: up to 2^29 samples). The 32-bit compare below is valid for less
    // than 2^31 cycles, so the deadline is kept as tmr_far periods of 2^31 cycles plus
    // div_q[30:0]: each time vcyc reaches tmr_cycle while tmr_far != 0, one period is used up.
    logic [6:0]  tmr_far;
    logic        tmr_hi;            // bit 31 added to tmr_cycle by the periods used up
    wire  [31:0] tmr_ahead = vcyc - tmr_cycle;   // wrap-safe tmr_cycle <= vcyc: bit 31 clear

    always_ff @(posedge clk) begin
        if (reset) begin
            tmr_valid <= 1'b0; div_busy <= 1'b0; mul_busy <= 1'b0; tmr_far <= 7'd0; tmr_hi <= 1'b0;
        end else begin
            if (t_cancel || t_fire) tmr_valid <= 1'b0;
            if (tmr_valid && tmr_far != 7'd0 && !tmr_ahead[31]) begin
                tmr_far <= tmr_far - 7'd1;
                tmr_hi <= !tmr_hi;
            end
            if (t_arm) begin
                arm_base <= arm_time;
                tmr_hi <= 1'b0;
                // n S by shift and add (32 clocks), then n S / P by restoring division (77)
                mul_n <= t_n; div_num <= 77'h0; div_i <= 7'd32;
                mul_busy <= 1'b1; div_busy <= 1'b0; tmr_valid <= 1'b0;
            end else if (mul_busy) begin
                div_num <= {div_num[75:0], 1'b0} + (mul_n[31] ? S_AS : 77'h0);
                mul_n <= {mul_n[30:0], 1'b0};
                div_i <= div_i - 7'd1;
                if (div_i == 7'd1) begin
                    mul_busy <= 1'b0;
                    div_rem <= 40'h0; div_q <= 38'h0; div_i <= 7'd77;
                    div_busy <= 1'b1;
                end
            end else if (div_busy) begin
                logic [39:0] r;
                logic [37:0] q;
                r = {div_rem[38:0], div_num[76]};
                div_num <= {div_num[75:0], 1'b0};
                q = {div_q[36:0], r >= {1'b0, P_AS}};
                div_q <= q;
                div_rem <= (r >= {1'b0, P_AS}) ? r - {1'b0, P_AS} : r;
                div_i <= div_i - 7'd1;
                if (div_i == 7'd1) begin
                    div_busy <= 1'b0;
                    tmr_valid <= 1'b1;
                    tmr_far <= q[37:31];
                end
            end
        end
    end
    // quotient settles the clock div_busy falls
    always_comb tmr_cycle = arm_base + {tmr_hi, div_q[30:0]};
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

    // Board time vt: vcyc while the CPU runs. A firmware power-down (PC3 = 0) holds the CPU in
    // reset, which returns vcyc to 0; MAME's machine time goes on and its uPD933 stream keeps
    // running (silence). vt then continues from the last vcyc, one cycle per cyc_ce while the
    // uPD933 is settled (as the CPU would wait for it at a boundary). Only a board reset (the
    // MiSTer's Power on) ends a power-down, so vt = vcyc whenever the CPU runs.
    logic        down_q;           // cpu_reset one clock late: vcyc is still valid in the first clock
    logic [31:0] vdown;
    wire  [31:0] vt = down_q ? vdown : vcyc;
    always_ff @(posedge clk) begin
        if (reset) begin
            down_q <= 1'b0; vdown <= 32'h0;
        end else begin
            down_q <= cpu_reset;
            if (!down_q) vdown <= vcyc;
            else if (cyc_ce && s_settled) vdown <= vdown + 32'd1;
        end
    end

    // now_tick = ticks(vt), kept incrementally: 3 tv = 5,000,000 tq + tr, where tv follows
    // vt one clock late (vt moves by at most an instruction's cycle count a clock); the
    // tick of any time a = tv + e (0 <= e < 512: vt, an access time) comes from that state.
    // Unlike ticks(), it keeps counting when the 32-bit vcyc wraps (after 21 minutes). The 40-bit
    // tick time wraps after 30.5 h; the uPD933 compares ticks modulo 2^40.
    localparam logic [22:0] T_DIV = 23'd5000000;
    logic [31:0] tv;               // vcyc the state belongs to
    logic [39:0] t3;               // 3 vcyc (not wrapped)
    logic [39:0] tq;
    logic [22:0] tr;
    wire  [31:0] tdv = vt - tv;
    always_ff @(posedge clk) begin
        if (reset) begin
            tv <= 32'h0; t3 <= 40'h0; tq <= 40'h0; tr <= 23'h0;
        end else if (tdv != 32'h0) begin
            logic [23:0] r;
            r = {1'b0, tr} + 24'(3 * tdv[8:0]);
            tv <= vt;
            t3 <= t3 + 40'(3 * tdv[8:0]);
            if (r >= {1'b0, T_DIV}) begin tr <= 23'(r - {1'b0, T_DIV}); tq <= tq + 40'd1; end
            else tr <= r[22:0];
        end
    end
    function automatic logic [39:0] tick_at(input logic [31:0] a);   // a = tv + e
        logic [8:0]  e;
        logic [23:0] r;
        logic [39:0] q;
        e = 9'(a - tv);
        r = {1'b0, tr} + 24'(3 * e);
        q = tq;
        if (r >= {1'b0, T_DIV}) begin r = r - {1'b0, T_DIV}; q = q + 40'd1; end
        return t3 + 40'(3 * e) - q - ((r != 24'h0) ? 40'd1 : 40'd0);
    endfunction
    always_comb now_tick = tick_at(vt);
    // synthesis translate_off
    logic [39:0] sim_tick_base = 40'h0;   // cz101_bench CZ_TICK_BASE: tick time at reset (t3 and the uPD933 start there)
    always_ff @(posedge clk) if (!reset) begin
        if (now_tick - sim_tick_base != ticks(vt)) $fatal(1, "now_tick %0d != base + ticks(%0d)", now_tick, vt);
        if (tdv >= 32'd512) $fatal(1, "vt %0d tv %0d", vt, tv);
        if (!cpu_reset && !down_q && 32'(acc_time - tv) >= 32'd512) $fatal(1, "acc_time %0d vcyc %0d tv %0d", acc_time, vcyc, tv);
    end
    // synthesis translate_on

    // port B input: bit 4 = uPD933 /RQ (active low), bit 7 = Power
    assign pb_in = {power_on, 2'b00, !s_irq, 4'h0};

    // ------------------------------------------------------------------ LCD
    logic [7:0] lcd_db;
    logic       lcd_pc_we, lcd_reset;
    cz101_lcd lcd (
        .clk, .reset, .now(acc_time), .insn_start(vcyc),
        .pc_we(lcd_pc_we), .pc(pc_out), .lcd_reset,
        .db_in(pa_out), .db_we(pa_we), .db_rd(pa_rd), .db_out(lcd_db),
        .r_line, .r_pos, .r_row, .r_code, .r_at_ac, .r_cg_bits,
        .display_on(lcd_display_on), .cursor_on(lcd_cursor_on), .blink_on(lcd_blink_on)
    );
    assign pa_in = lcd_db;
    assign lcd_power = power;
    assign powered_down = cpu_reset;
    assign cycles = vt;

    // ------------------------------------------------------------------ external bus
    typedef enum logic [2:0] { B_IDLE, B_MEM, B_SND, B_SND_WAIT, B_DONE } bus_t;
    bus_t bus_st;
    logic [7:0] rdata_q;

    // board-side uPD933 requests come from the bus and from port B / power-down
    logic        pb_pending, rst_pending;
    logic        pb_cs;
    logic [31:0] pb_time;          // acc_time of the port write
    logic [39:0] pb_tick;          // its 10 MHz tick

    always_ff @(posedge clk) begin
        ext_ack <= 1'b0;
        lcd_pc_we <= 1'b0;
        lcd_reset <= 1'b0;
        if (reset) begin
            bus_st <= B_IDLE;
            s_req <= 1'b0;
            port_b <= 8'h00;
            power <= 1'b0;
            power_on_q <= 1'b0;
            cpu_reset <= 1'b0;
            led_latch[0] <= 8'hFF; led_latch[1] <= 8'hFF; led_latch[2] <= 8'hFF; led_latch[3] <= 8'hFF;
            pb_pending <= 1'b0; rst_pending <= 1'b0;
        end else begin
            // MAME power_w: m_power = 1 when the switch turns on (the first clock after reset
            // counts as that edge: CZ_POWER_ON). A power-down clears it until the next edge.
            power_on_q <= power_on;
            if (power_on && !power_on_q) power <= 1'b1;

            // port writes (MAME port_b_w / port_c_w), in the clock the CPU performs them
            if (pb_we) begin
                port_b <= pb_out;
                pb_pending <= 1'b1;
                pb_time <= acc_time;
                pb_tick <= tick_at(acc_time);
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
                    pb_tick <= tick_at(acc_time);
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
                        s_tick <= pb_tick;
                        arm_time <= pb_time;
                        bus_st <= B_SND_WAIT;
                        if (rst_pending) rst_pending <= 1'b0; else pb_pending <= 1'b0;
                    end else if (ext_req && !ext_ack) begin
                        if (ext_addr >= 16'hC000 && ext_addr <= 16'hFEFF) begin
                            s_req <= 1'b1;
                            s_op <= ext_we ? 3'd0 : 3'd1;
                            s_data <= ext_wdata;
                            s_tick <= tick_at(acc_time);
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
                    else if (ext_addr[15:12] == 4'h8 || in_cart) rdata_q <= ram_q;
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
    // live MIDI serializer, stepped by the SCK edges it applies
    logic        ser_active;
    logic [8:0]  ser_sh;           // bits still to send, LSB first (data, then stop)
    logic [3:0]  ser_left;
    logic [6:0]  ser_cnt;          // edges into the current bit

    wire sck_due = !sck_ahead[31];
    wire [31:0] sck_ahead = vcyc - sck_cyc;      // wrap-safe sck_cyc <= vcyc
    wire [31:0] rx_ahead  = vcyc - rx_cycle;
    wire [31:0] rx_before = rx_edge - sck_k;

    always_ff @(posedge clk) begin
        sck_edge <= 1'b0;
        int1_rise <= 1'b0;
        rx_pop <= 1'b0;
        midi_pop <= 1'b0;
        if (reset) begin
            ser_active <= 1'b0;
            // first edge at 250 ns: c_1 = floor(E / P) = 0, remainder E
            sck_cyc <= 32'd0; sck_rem <= E_U; sck_lvl_next <= 1'b1; sck_k <= 32'd1;
            rx_line <= 1'b1;
            irq_prev <= 1'b0; irq_seen_rise <= 1'b0; int1_line <= 1'b0; nmi_ev <= 1'b0;
            ev_done <= 1'b0; t_fire <= 1'b0; fire_wait <= 2'd0;
        end else begin
            if (s_irq && !irq_prev) irq_seen_rise <= 1'b1;
            irq_prev <= s_irq;
            ev_done <= 1'b0;
            t_fire <= 1'b0;
            if (t_fire) fire_wait <= 2'd3;                 // let the IRQ line settle
            else if (fire_wait != 2'd0) fire_wait <= fire_wait - 2'd1;
            if (at_boundary && !ev_done && s_settled && bus_st == B_IDLE && !pb_pending && !rst_pending
                && !div_busy && !mul_busy && !t_arm && !t_fire && fire_wait == 2'd0) begin
                if (tmr_valid && tmr_far == 7'd0 && !tmr_ahead[31]) begin
                    t_fire <= 1'b1;           // MAME timer_tick at D (before this instruction)
                end else if (irq_seen_rise || int1_line != s_irq) begin
                    int1_rise <= irq_seen_rise;
                    int1_line <= s_irq;
                    irq_seen_rise <= 1'b0;
                end else if (nmi_ev != !power_on) begin
                    // MAME power_w changes INPUT_LINE_NMI between timeslices, so the CPU sees it
                    // before an instruction, never while it pushes for an interrupt it took
                    nmi_ev <= !power_on;
                end else if (rx_valid && !rx_pop && !rx_ahead[31] && (!sck_due || rx_before[31])) begin
                    rx_line <= rx_level;
                    rx_pop <= 1'b1;
                end else if (sck_due) begin
                    sck_k <= sck_k + 32'd1;
                    sck_edge <= 1'b1;
                    sck_level <= sck_lvl_next;
                    sck_lvl_next <= !sck_lvl_next;
                    // a byte's 10 bits take 1,280 edges; the edge that ends a stop bit starts
                    // the next queued byte
                    if (ser_active && !(ser_cnt == 7'd127 && ser_left == 4'd0)) begin
                        if (ser_cnt == 7'd127) begin
                            rx_line <= ser_sh[0];
                            ser_sh <= {1'b1, ser_sh[8:1]};
                            ser_left <= ser_left - 4'd1;
                        end
                        ser_cnt <= ser_cnt + 7'd1;
                    end else if (midi_valid && !midi_pop) begin
                        rx_line <= 1'b0;                              // start bit
                        ser_sh <= {1'b1, midi_byte};
                        ser_left <= 4'd9;
                        ser_cnt <= 7'd0;
                        ser_active <= 1'b1;
                        midi_pop <= 1'b1;
                    end else ser_active <= 1'b0;                      // stop bit sent, idle
                    if (sck_rem + E_U >= P_U) begin
                        sck_rem <= sck_rem + E_U - P_U;
                        sck_cyc <= sck_cyc + 32'd1;
                    end else sck_rem <= sck_rem + E_U;
                end else ev_done <= 1'b1;
            end
        end
    end

endmodule
