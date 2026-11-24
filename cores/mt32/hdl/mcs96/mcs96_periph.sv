// SPDX-License-Identifier: MIT
// 8098 register file and peripherals, written from MAME's i8x9x device as Nuked-MT32 uses it
// (modules/nuked-mt32/mame/i8x9x.cpp; Olivier Galibert, BSD-3-Clause; see NOTICE):
// internal RAM 18-FF, SFRs 00-17 (08/09 are the core's), timers 1 and 2, the HSO CAM, A/D,
// serial port, ports, PWM.
//
// Time is MAME's total_cycles() (cycles of the 12 MHz clock, cycles_scaling 3 per CPU cycle).
// Timer values are time / 24. `internal_update(T)` runs as in MAME: from the scheduler (`upd_*`)
// and from SFR writes that call it (HSO time high byte, A/D start). Each run ends with the next
// event time on `ev_time` (`ev_valid` pulse); the scheduler turns it into bcount.
//
// Not modelled, as in the reference board: HSI inputs (never driven), watchdog, port 1. HSO
// command 0F (start A/D from the CAM) would make MAME call internal_update recursively; the
// MT-32 firmware uses only commands 18/19 (software timers), so 0F starts the A/D without the
// nested update and raises `unsupported`.
module mcs96_periph (
    input  logic        clk,
    input  logic        rst,
    output logic        ready,         // internal RAM cleared to FF after reset

    // register-space byte accesses from the core
    input  logic        r_req,
    input  logic        r_we,
    input  logic [7:0]  r_addr,
    input  logic [7:0]  r_wdata,
    output logic [7:0]  r_rdata,
    output logic        r_ack,

    input  logic [47:0] now,           // total_cycles() while the access or update runs

    // internal_update from the scheduler
    input  logic        upd_req,
    input  logic [47:0] upd_time,
    output logic        upd_ack,
    output logic        ev_valid,      // pulse after every internal_update
    output logic [47:0] ev_time,

    // board
    input  logic        serial_in,     // serial_w(): a byte into SBUF
    input  logic [7:0]  serial_byte,
    input  logic        extint,        // EXTINT line level
    input  logic        la32_sh3,
    input  logic [9:0]  knob,
    output logic [7:0]  pwm,
    output logic [7:0]  irq_set,
    output logic        unsupported
);
    localparam logic [7:0] IRQ_TIMER = 8'h01, IRQ_AD = 8'h02, IRQ_HSO = 8'h08, IRQ_SOFT = 8'h20,
                           IRQ_SERIAL = 8'h40, IRQ_EXTINT = 8'h80;

    // ---------------------------------------------------------------- state
    logic [7:0]  ram [256];
    logic [7:0]  ram_q;
    logic [7:0]  clr_a;
    logic        clearing;

    logic [3:0]  ad_command;
    logic [15:0] ad_result;
    logic [47:0] ad_done;
    logic [15:0] hso_time;
    logic [7:0]  hso_command, hso_active, ios0, ios1, ioc0, ioc1, port2, sbuf, sp_con, sp_stat;
    logic [7:0]  cam_cmd [8];
    logic [15:0] cam_time [8];
    logic [7:0]  hold_cmd;
    logic [15:0] hold_time;
    logic [47:0] base2, serial_timer;
    logic [15:0] baud_reg;
    logic        brh, extint_q;

    // ---------------------------------------------------------------- divide by 24
    // q = floor(x / 24) = floor((x >> 3) / 3) = ((x >> 3) * M) >> 47, M = (2**47 + 1) / 3,
    // exact for x < 2**48. Two clocks.
    localparam logic [45:0] M3 = 46'd46912496118443;
    logic [47:0] dv_x;
    logic [91:0] dv_p;
    logic [44:0] dv_q;
    always_ff @(posedge clk) begin
        dv_p <= {47'h0, dv_x[47:3]} * {46'h0, M3};
    end
    assign dv_q = dv_p[91:47];

    // ---------------------------------------------------------------- update FSM
    typedef enum logic [4:0] {
        U_IDLE, U_D1, U_D1W, U_D2, U_D2W, U_TIMERS, U_CAM, U_CAMNEXT, U_AD, U_SER,
        U_D3, U_D3W, U_EV, U_EVTU, U_EVNEXT, U_T1, U_T2, U_DONE
    } u_t;
    u_t us;
    logic [47:0] ut;               // current_time
    logic [44:0] q1, q2;           // full quotients for timer 1 and timer 2
    logic [15:0] ct1, ct2;         // current_timer1 / 2
    logic [2:0]  ui;
    logic [47:0] uev;
    logic        uev_set;
    logic        u_from_reg;       // the update was called from an SFR write

    // register-access FSM
    typedef enum logic [2:0] { A_IDLE, A_RAM, A_DIV, A_DIVW, A_UPD, A_ACK } a_t;
    a_t as;
    logic [7:0] a_q;

    function automatic logic [15:0] u16(input logic [47:0] x); return x[15:0]; endfunction

    // timer_time_until(): base + ((delta + tdelta) * 24)
    function automatic logic [47:0] time_until(input logic [47:0] base, input logic [44:0] delta,
                                               input logic [15:0] tv);
        logic [16:0] td;
        logic [47:0] k;
        td = {1'b0, tv - delta[15:0]};
        if (td == 17'h0) td = 17'h10000;
        k = {3'h0, delta} + {31'h0, td};
        return base + (k << 4) + (k << 3);
    endfunction

    logic       wr_en;
    logic [7:0] wr_a, wr_d;

    always_ff @(posedge clk) begin
        logic [7:0] act;
        logic [7:0] irq;
        irq = 8'h0;
        wr_en <= 1'b0;
        ev_valid <= 1'b0;
        upd_ack <= 1'b0;
        r_ack <= 1'b0;
        if (rst) begin
            clearing <= 1'b1; clr_a <= 8'h0; ready <= 1'b0;
            ad_command <= 4'h0; ad_result <= 16'h0; ad_done <= 48'h0;
            hso_time <= 16'h0; hso_command <= 8'h0; hso_active <= 8'h0;
            ios0 <= 8'h0; ios1 <= 8'h0; ioc0 <= 8'h0; ioc1 <= 8'h01; port2 <= 8'h01;
            sbuf <= 8'h0; sp_con <= 8'h0; sp_stat <= 8'h0; pwm <= 8'h0;
            hold_cmd <= 8'h0; hold_time <= 16'h0;
            for (int i = 0; i < 8; i++) begin cam_cmd[i] <= 8'h0; cam_time[i] <= 16'h0; end
            base2 <= 48'h0; serial_timer <= 48'h0; baud_reg <= 16'h0; brh <= 1'b0;
            extint_q <= 1'b0;
            us <= U_IDLE; as <= A_IDLE;
            unsupported <= 1'b0;
        end else begin
            if (clearing) begin
                wr_en <= 1'b1; wr_a <= clr_a; wr_d <= 8'hff;
                clr_a <= clr_a + 8'd1;
                if (clr_a == 8'hff) begin clearing <= 1'b0; ready <= 1'b1; end
            end

            // board inputs (only while the CPU waits between slices)
            if (serial_in) begin
                sbuf <= serial_byte;
                sp_stat <= sp_stat | 8'h40;
                irq = irq | IRQ_SERIAL;
            end
            extint_q <= extint;
            if (extint && !extint_q && !ioc1[1]) irq = irq | IRQ_EXTINT;

            // ---------------------------------------------------- internal_update(ut)
            case (us)
            U_IDLE: if (upd_req && as == A_IDLE) begin
                ut <= upd_time; u_from_reg <= 1'b0; us <= U_D1;
            end
            U_D1:  begin dv_x <= ut; us <= U_D1W; end
            U_D1W: us <= U_D2;
            U_D2:  begin q1 <= dv_q; dv_x <= ut - base2; us <= U_D2W; end
            U_D2W: us <= U_TIMERS;
            U_TIMERS: begin
                logic [44:0] qq;
                qq = dv_q;
                q2 <= qq;
                ct1 <= q1[15:0];
                ct2 <= qq[15:0];
                if (q1[15:0] == 16'h0 && !ios1[5]) begin
                    ios1[5] <= 1'b1;
                    if (ioc1[2]) irq = irq | IRQ_TIMER;
                end
                if (qq[15:0] == 16'h0 && !ios1[4]) begin
                    ios1[4] <= 1'b1;
                    if (ioc1[3]) irq = irq | IRQ_TIMER;
                end
                ui <= 3'd0;
                us <= U_CAM;
            end
            // for each active CAM entry whose time matches: trigger_cam()
            U_CAM: begin
                logic [7:0] c;
                c = cam_cmd[ui];
                if (hso_active[ui] && (c[6] ? (cam_time[ui] == ct2) : (cam_time[ui] == ct1))) begin
                    logic [7:0] io0;
                    io0 = ios0;
                    if (hso_active == 8'hff && !io0[7]) io0 = io0 & 8'hbf;
                    case (c[3:0])
                        4'h0, 4'h1, 4'h2, 4'h3, 4'h4, 4'h5:
                            io0 = c[5] ? (io0 | (8'd1 << c[2:0])) : (io0 & ~(8'd1 << c[2:0]));
                        4'h6: io0 = c[5] ? (io0 | 8'h03) : (io0 & 8'hfc);
                        4'h7: io0 = c[5] ? (io0 | 8'h0c) : (io0 & 8'hf3);
                        4'h8, 4'h9, 4'ha, 4'hb: ios1 <= ios1 | (8'd1 << c[1:0]);
                        4'he: base2 <= ut;
                        4'hf: begin
                            ad_result <= {6'h0, (ad_command[2:0] == 3'd7) ? knob : 10'h0} << 6 | {12'h0, 1'b1, ad_command[2:0]};
                            ad_done <= ut + 48'd504;
                            unsupported <= 1'b1;
                        end
                        default: ;
                    endcase
                    ios0 <= io0;
                    hso_active[ui] <= 1'b0;
                    if (c[4]) irq = irq | (c[3] ? IRQ_SOFT : IRQ_HSO);
                end
                us <= U_CAMNEXT;
            end
            U_CAMNEXT: if (ui == 3'd7) us <= U_AD; else begin ui <= ui + 3'd1; us <= U_CAM; end
            U_AD: begin
                if (ad_done != 48'h0 && ut >= ad_done) begin
                    ad_done <= 48'h0;
                    ad_result[3] <= 1'b0;
                    irq = irq | IRQ_AD;
                end
                us <= U_SER;
            end
            U_SER: begin
                if (ut == serial_timer) begin
                    serial_timer <= 48'h0;
                    irq = irq | IRQ_SERIAL;
                    sp_stat <= sp_stat | 8'h20;
                end
                // current_timer2 is read again below (line 603), after the CAM may have reset timer 2
                dv_x <= ut - base2;
                us <= U_D3;
            end
            U_D3:  us <= U_D3W;
            U_D3W: begin q2 <= dv_q; ui <= 3'd0; uev <= 48'h0; uev_set <= 1'b0; us <= U_EV; end
            // event_time: refill free CAM slots from the hold register, earliest CAM time
            U_EV: begin
                act = hso_active;
                if (!act[ui] && ios0[7]) begin
                    cam_cmd[ui] <= hold_cmd;
                    cam_time[ui] <= hold_time;
                    act[ui] = 1'b1;
                    hso_active <= act;
                    ios0 <= (act == 8'hff) ? ((ios0 & 8'h7f) | 8'h40) : (ios0 & 8'h7f);
                    us <= U_EVTU;               // the refilled entry is read next clock
                end else begin
                    us <= act[ui] ? U_EVTU : U_EVNEXT;
                end
            end
            U_EVTU: begin
                logic [47:0] nt;
                if (cam_cmd[ui][6]) nt = time_until(base2, q2, cam_time[ui]);
                else                nt = time_until(48'h0, q1, cam_time[ui]);
                if (!uev_set || nt < uev) begin uev <= nt; uev_set <= 1'b1; end
                us <= U_EVNEXT;
            end
            U_EVNEXT: if (ui == 3'd7) us <= U_T1; else begin ui <= ui + 3'd1; us <= U_EV; end
            U_T1: begin
                logic [47:0] e;
                logic        es;
                e = uev;
                es = uev_set;
                if (ad_done != 48'h0 && (!es || ad_done < e)) begin e = ad_done; es = 1'b1; end
                if (serial_timer != 48'h0 && (!es || serial_timer < e)) begin e = serial_timer; es = 1'b1; end
                uev <= e;
                uev_set <= es;
                us <= U_T2;
            end
            U_T2: begin
                logic [47:0] t1, t2, e;
                t1 = now + ({32'h0, 17'h10000 - {1'b0, ct1}} * 48'd24);
                t2 = now + ({32'h0, 17'h10000 - {1'b0, q2[15:0]}} * 48'd24);
                e = uev;
                if (!uev_set || e > t1) e = t1;
                if (e > t2) e = t2;
                ev_time <= e;
                ev_valid <= 1'b1;
                us <= U_DONE;
            end
            U_DONE: begin
                if (u_from_reg) begin end else upd_ack <= 1'b1;
                us <= U_IDLE;
            end
            default: us <= U_IDLE;
            endcase

            // ---------------------------------------------------- register accesses
            case (as)
            A_IDLE: if (r_req && !r_ack && ready && us == U_IDLE && !upd_req) begin
                if (r_addr >= 8'h18) begin
                    if (r_we) begin wr_en <= 1'b1; wr_a <= r_addr; wr_d <= r_wdata; as <= A_ACK; end
                    else as <= A_RAM;
                end else if (!r_we) begin
                    case (r_addr)
                        8'h02: begin a_q <= ad_result[7:0]; as <= A_ACK; end
                        8'h03: begin a_q <= ad_result[15:8]; as <= A_ACK; end
                        8'h06: begin a_q <= 8'h00; as <= A_ACK; end        // hsi_status (no HSI)
                        8'h07: begin a_q <= sbuf; as <= A_ACK; end
                        8'h0a, 8'h0b: begin dv_x <= now; as <= A_DIV; end
                        8'h0c, 8'h0d: begin dv_x <= now - base2; as <= A_DIV; end
                        8'h0e: begin a_q <= {3'b000, la32_sh3, 4'h0}; as <= A_ACK; end
                        8'h0f: begin a_q <= 8'hff; as <= A_ACK; end
                        8'h10: begin a_q <= 8'hfd & (port2 | (extint_q ? 8'h1e : 8'h1a)); as <= A_ACK; end
                        8'h11: begin a_q <= sp_stat; sp_stat <= sp_stat & 8'h80; as <= A_ACK; end
                        8'h15: begin a_q <= ios0; as <= A_ACK; end
                        8'h16: begin a_q <= ios1; ios1 <= ios1 & 8'hc0; as <= A_ACK; end
                        default: begin a_q <= 8'h00; as <= A_ACK; end    // 00, 01, 04, 05, 08, 09, ...
                    endcase
                end else begin
                    as <= A_ACK;
                    case (r_addr)
                        8'h02: begin
                            ad_command <= r_wdata[3:0];
                            if (r_wdata[3]) begin
                                // ad_start(total_cycles()): only ACH7 (the knob) is connected
                                ad_result <= (r_wdata[2:0] == 3'd7) ? ({6'h0, knob} << 6) | 16'h000f
                                                                    : {12'h0, 1'b1, r_wdata[2:0]};
                                ad_done <= now + 48'd504;
                                ut <= now; u_from_reg <= 1'b1; us <= U_D1; as <= A_UPD;
                            end
                        end
                        8'h03: ;                                           // hsi_mode
                        8'h04: hso_time[7:0] <= r_wdata;
                        8'h05: begin
                            // hso_time_w() -> commit_hso_cam()
                            logic [15:0] t;
                            t = {r_wdata, hso_time[7:0]};
                            hso_time <= t;
                            if (hso_active != 8'hff) begin
                                logic [2:0] f;
                                f = 3'd0;
                                for (int i = 7; i >= 0; i--) if (!hso_active[i]) f = 3'(i);
                                hso_active[f] <= 1'b1;
                                if ((hso_active | (8'd1 << f)) == 8'hff) ios0 <= ios0 | 8'h40;
                                cam_cmd[f] <= hso_command;
                                cam_time[f] <= t;
                                ut <= now; u_from_reg <= 1'b1; us <= U_D1; as <= A_UPD;
                            end else begin
                                ios0 <= ios0 | 8'hc0;
                                hold_cmd <= hso_command;
                                hold_time <= t;
                            end
                        end
                        8'h06: hso_command <= r_wdata;
                        8'h07: serial_timer <= now + 48'd9600;            // sbuf_w -> serial_send
                        8'h0e: begin
                            if (brh) baud_reg[15:8] <= r_wdata; else baud_reg[7:0] <= r_wdata;
                            brh <= !brh;
                        end
                        8'h10: port2 <= r_wdata & 8'h21;
                        8'h11: sp_con <= r_wdata & 8'h1f;
                        8'h15: begin ioc0 <= r_wdata & 8'hfd; if (r_wdata[1]) base2 <= now; end
                        8'h16: ioc1 <= r_wdata;
                        8'h17: pwm <= r_wdata;
                        default: ;                                         // 00, 01, 08, 09 (core), 0A watchdog...
                    endcase
                end
            end
            A_RAM:  begin a_q <= ram_q; as <= A_ACK; end
            A_DIV:  as <= A_DIVW;
            A_DIVW: begin
                // timer_value(): reading the low byte or the high byte of the quotient
                a_q <= r_addr[0] ? dv_q[15:8] : dv_q[7:0];
                as <= A_ACK;
            end
            A_UPD: if (us == U_DONE) as <= A_ACK;
            A_ACK: begin r_ack <= 1'b1; as <= A_IDLE; end
            default: as <= A_IDLE;
            endcase
        end
        irq_set <= rst ? 8'h0 : irq;
    end

    always_ff @(posedge clk) begin
        if (wr_en) ram[wr_a] <= wr_d;
        ram_q <= ram[r_addr];
    end
    assign r_rdata = a_q;
endmodule
