// SPDX-License-Identifier: MIT
// H8/3003 on-chip peripherals as MAME runs them (src/devices/cpu/h8: h83003.cpp, h8_intc.cpp,
// h8_timer16.cpp, h8_sci.cpp, h8_adc.cpp (H8/3337 variant), h8_port.cpp, h8_watchdog.cpp,
// h8_refresh.cpp; BSD-3-Clause, Olivier Galibert). Registers FFFF20-FFFFFF.
//
// MAME's peripherals are lazy: each keeps the time of its next event, catches its counters up
// when a register is accessed, and is serviced between CPU bus accesses. This module is the same
// model in virtual time (CPU states):
//   - acc: one CPU access to a register, at acc_time = the time at the end of the access (MAME's
//     total_cycles() inside the handler). A word access to two byte registers is two handler
//     calls, even address first.
//   - proc: service every event at or before proc_time, earliest first (ties in MAME's device
//     order: A/D, SCI0, SCI1, timer 0-4, watchdog), each at its own event time. The bus side
//     calls it after every CPU access, as MAME's execute_run does after the access that crosses
//     an event.
//   - rx: an SCI receive pin change (h8_sci_device::do_rx_w) with MAME's now_as_cycles().
// The interrupt request to the CPU (vector, level) changes only where MAME calls
// update_irq_state(), so a stale request (after an ICR write, for instance) stays stale as in
// MAME.
//
// Implemented as MAME does it, including its quirks: ICR writes keep only the written byte
// (h8h_intc_device::icr_w), the H8H timer phase is always 0, TSR flag clears do not clear the
// pending interrupt. Not implemented (the MU50 firmware does not use them): DMA registers (read
// 0), SCI clocked-synchronous and external-clock modes, the refresh timer's counting (its
// registers read and write), watchdog reset.
//
// Divisions (SCI clock start, timer catch-up modulo) run on a shared one-bit-per-clock divider;
// everything else takes one clock per handler. Requests (acc_req, proc_req,
// rx_req) are levels held until the matching done pulse.

module h83003_periph #(
    parameter int TW = 48
) (
    input  logic          clk,
    input  logic          rst,

    input  logic          acc_req,
    input  logic          acc_we,
    input  logic          acc_word,
    input  logic [7:0]    acc_off,        // address bits 7:0 (FFFFxx)
    input  logic [15:0]   acc_wdata,      // byte accesses: data in [7:0]
    input  logic [TW-1:0] acc_time,
    output logic          acc_done,
    output logic [15:0]   acc_rdata,      // byte accesses: data in [7:0]

    input  logic          proc_req,
    input  logic [TW-1:0] proc_time,
    output logic          proc_done,
    output logic [TW-1:0] next_event,     // earliest pending event, 0 = none
    output logic          idle,           // nothing in progress or pending (bus fast path)

    input  logic          rx_req,
    input  logic          rx_sci,
    input  logic          rx_val,
    input  logic [TW-1:0] rx_now,
    output logic          rx_done,

    // CPU side
    output logic [7:0]    irq_vector,
    output logic [7:0]    irq_level,      // -1 (FF) when no request
    input  logic          filter_we,      // the CPU called update_irq_filter()
    input  logic [1:0]    filter,
    input  logic          irq_taken,
    input  logic [7:0]    irq_taken_vector,
    input  logic [7:0]    cpu_ccr,
    output logic          syscr_ue,

    // IRQ0-5 pins, asserted (active low on the chip: 1 = ASSERT_LINE here)
    input  logic [5:0]    irq_pin,

    // ports, MAME numbering (PORT_4 = 3 ... PORT_C = 11)
    output logic          pr_stb,         // do_read_port(pr_port) is called now
    output logic [3:0]    pr_port,
    input  logic [7:0]    pr_data,        // its value, combinational
    output logic          pw_stb,         // do_write_port(pw_port, pw_data, pw_ddr)
    output logic [3:0]    pw_port,
    output logic [7:0]    pw_data,
    output logic [7:0]    pw_ddr,

    input  logic [7:0][9:0] adc_in,       // do_read_adc(0..7)

    output logic [1:0]    sci_tx          // do_sci_tx
);
    localparam logic [TW-1:0] T0 = '0;

    // ================================================================ shared divider
    logic          dv_go, dv_busy;
    logic [TW-1:0] dv_n, dv_q, dv_r;
    logic [16:0]   dv_d;
    logic [6:0]    dv_i;
    wire  [TW:0]   dv_try = {dv_r, dv_n[TW-1]} - {{(TW-16){1'b0}}, dv_d};

    // ================================================================ state
    // intc
    logic [63:0] pend;
    logic [7:0]  ier, isr, iscr, irq_input, irq_edge;    // irq_edge: 1 = EDGE_FALL
    logic [23:0] icr;
    logic [1:0]  icr_filter;
    logic        upd_req;
    logic [7:0]  syscr;

    // ports 4..C (index = MAME port number)
    logic [11:0][7:0] p_dr, p_ddr, p_pcr;
    logic [11:0][16:0] p_last;

    // timers
    logic [7:0]  tstr;
    logic [4:0][7:0]  t_tcr, t_tier, t_ier, t_isr;
    logic [4:0][15:0] t_tcnt;
    logic [4:0][3:0][15:0] t_tgr;
    logic [4:0][1:0]  t_div;
    logic [4:0]       t_div1;          // clock_type == DIV_1
    logic [4:0][1:0]  t_clr;           // 0: TGRA, 1: TGRB, 2: none, 3: external
    logic [4:0][16:0] t_cycle;
    logic [4:0][TW-1:0] t_last, t_event;
    logic [4:0]       t_active;

    // SCI
    localparam logic [2:0] ST_IDLE = 3'd0, ST_START = 3'd1, ST_BIT = 3'd2, ST_PARITY = 3'd3,
                           ST_STOP = 3'd4, ST_LAST = 3'd5;
    logic [1:0][7:0]  s_rdr, s_tdr, s_smr, s_scr, s_ssr, s_ssr_read, s_brr, s_rsr, s_tsr;
    logic [1:0][2:0]  s_txst, s_rxst;
    logic [1:0][3:0]  s_txbit, s_rxbit, s_txcc, s_rxcc;
    logic [1:0][1:0]  s_clk;           // bit 0 TX, bit 1 RX running
    logic [1:0]       s_txpar, s_rxpar, s_rxval;
    logic [1:0][TW-1:0] s_event;
    logic [1:0][16:0] s_div, s_step;

    // A/D
    logic [3:0][9:0]  a_addr;
    logic [9:0]       a_buf;
    logic [7:0]       a_adcsr, a_adcr;
    logic [3:0]       a_mode, a_start_mode;  // bit 0 ACTIVE, 1 HALTED, 2 REPEAT, 3 ROTATE
    logic [2:0]       a_start_ch, a_end_ch, a_ch;
    logic             a_powered;
    logic [TW-1:0]    a_event;

    // watchdog (type H), refresh registers
    logic [7:0]       w_tcsr, w_tcnt, w_rst;
    logic [TW-1:0]    w_base;
    logic [7:0]       r_rtmcsr, r_rtcnt, r_rtcor;

    int pass;              // word access to two byte registers: 0 even byte, 1 odd byte
    logic [4:0] rc_mask;   // timer channels whose recalc_event() is owed (run one a clock)

    // ================================================================ helpers
    localparam int V_TIMER0 = 24, V_SCI0 = 52, V_ADC = 60;

    function automatic int vslot(input int v);
        if (v >= 12 && v <= 13) return v - 12;
        if (v >= 14 && v <= 15) return 2;
        if (v >= 16 && v <= 19) return 3;
        if (v >= 20 && v <= 23) return 4;
        if (v >= 24 && v <= 55) return 5 + (v - 24) / 4;
        if (v >= 56 && v <= 63) return 13 + (v - 56) / 4;
        return -1;
    endfunction

    // timer event time (recalc_event, phase always 0 on the H8H)
    function automatic logic [TW-1:0] t_when(input logic [TW-1:0] cur, input logic [1:0] dv,
                                             input logic [31:0] delay);
        logic [TW-1:0] s;
        s = (cur + (T0 + (1 << dv))) >> dv;
        return (s + {{(TW-32){1'b0}}, delay} - T0 - 1) << dv;
    endfunction

    // ================================================================ intc
    task automatic internal_interrupt(input int v);
`ifdef H8H_DEBUG
        $display("periph: internal_interrupt %0d now %0d", v, now);
`endif
        pend[v] = 1'b1;
        upd_req = 1'b1;
    endtask
    task automatic clear_interrupt(input int v);
        pend[v] = 1'b0;
        upd_req = 1'b1;
    endtask
    task automatic check_level_irqs(input bit upd);
        bit set;
        set = 0;
        for (int i = 0; i < 8; i++)
            if (!irq_edge[i] && irq_input[i] && !isr[i]) begin
                isr[i] = 1'b1;
                set = 1;
            end
        if (set && upd) upd_req = 1'b1;
    endtask
    task automatic set_filter_from(input logic [7:0] ccr, input logic ue);
        if (ue) icr_filter = ccr[7] ? 2'd2 : 2'd0;
        else if (ccr[7] && ccr[6]) icr_filter = 2'd2;
        else icr_filter = ccr[7] ? 2'd1 : 2'd0;
        upd_req = 1'b1;
    endtask

    // update_irq_state(), in two stages: the best request in each group of 8 vectors, then over
    // the groups (vectors in order; a higher level wins, the lower vector on a tie)
    function automatic int vpri(input int v);
        if (v == 7) return 2;
        if (vslot(v) < 0) return 0;
        return int'(icr[vslot(v) ^ 7]);
    endfunction
    logic [7:0][5:0] g_vec;          // per group: vector, valid, level
    logic [7:0]      g_ok;
    logic [7:0][1:0] g_lvl;
    logic            upd_s1;
    always_ff @(posedge clk) begin
        upd_s1 <= 1'b0;
        if (rst) begin
            irq_vector <= 8'd0;
            irq_level <= 8'hff;
        end else begin
            if (do_update) begin
                logic [63:0] p;
                p = pend;
                p[19:12] = isr & ier;
                for (int gi = 0; gi < 8; gi++) begin
                    g_ok[gi] <= 1'b0;
                    g_vec[gi] <= 6'd0;
                    g_lvl[gi] <= 2'd0;
                    begin : grp
                        bit ok;
                        int lv;
                        ok = 0;
                        lv = 0;
                        for (int j = 0; j < 8; j++)
                            if (p[gi * 8 + j] && vpri(gi * 8 + j) >= int'(icr_filter) &&
                                (!ok || vpri(gi * 8 + j) > lv)) begin
                                ok = 1;
                                lv = vpri(gi * 8 + j);
                                g_vec[gi] <= 6'(gi * 8 + j);
                                g_lvl[gi] <= 2'(lv);
                            end
                        g_ok[gi] <= ok;
                    end
                end
                upd_s1 <= 1'b1;
            end
            if (upd_s1) begin
                logic [7:0] vec, lvl;
                vec = 8'd0;
                lvl = 8'hff;
                for (int gi = 0; gi < 8; gi++)
                    if (g_ok[gi] && (lvl == 8'hff || {6'h0, g_lvl[gi]} > lvl)) begin
                        vec = {2'b0, g_vec[gi]};
                        lvl = {6'h0, g_lvl[gi]};
                    end
                irq_vector <= vec;
                irq_level <= lvl;
`ifdef H8H_DEBUG
                $display("periph: update_irq_state -> %0d level %0d (filter %0d, icr %h)", vec, $signed(lvl), icr_filter, icr);
`endif
            end
        end
    end

    // ================================================================ ports
    task automatic port_update_output(input int n, input logic [7:0] mask);
        logic [7:0] data, ddr;
        data = (p_dr[n] | ~p_ddr[n]) & ~mask;
        ddr = p_ddr[n] & ~mask;
        if ({1'b0, ddr, data} != p_last[n]) begin
            p_last[n] = {1'b0, ddr, data};
            pw_stb = 1'b1;
            pw_port = 4'(n);
            pw_data = data;
            pw_ddr = ddr;
        end
    endtask
    function automatic logic [7:0] port_mask(input int n);
        case (n)
            5: return 8'h80;
            7: return 8'he0;
            8: return 8'hc0;
            default: return 8'h00;
        endcase
    endfunction
    // pr_port is decoded from the access offset (combinational), so pr_data is valid in the
    // clock the handler runs
    task automatic port_read(input int n, output logic [7:0] res);
        logic [7:0] m;
        m = port_mask(n);
        res = m | (p_dr[n] & p_ddr[n]);
        if ((p_ddr[n] & ~m) != ~m) begin
            pr_stb = 1'b1;
            res = res | (pr_data & ~p_ddr[n]);
        end
    endtask
    always_comb begin
        logic [7:0] o;
        o = acc_word ? {acc_off[7:1], pass[0]} : acc_off;
        case (o)
            8'hc7: pr_port = 4'd3; 8'hca: pr_port = 4'd4; 8'hcb: pr_port = 4'd5;
            8'hce: pr_port = 4'd6; 8'hcf: pr_port = 4'd7; 8'hd2: pr_port = 4'd8;
            8'hd3: pr_port = 4'd9; 8'hd6: pr_port = 4'd10; default: pr_port = 4'd11;
        endcase
    end

    // ================================================================ timers
    // update_counter(cur): stage 1. Returns 1 when a modulo by t_cycle is needed (dv_* set up);
    // stage 2 (t_update_fin) completes with the result.
    logic [TW-1:0] tu_tt, tu_delta, tu_val;   // tu_val: the new count before "% cycle"
    logic [15:0]   tu_prev;

    task automatic t_update_start(input int c, input logic [TW-1:0] cur, output bit need_mod,
                                  output bit act);
        logic [TW-1:0] base, nw;
        need_mod = 0;
        act = 0;
        if (!t_div1[c]) return;
        if (!t_active[c]) begin
            t_last[c] = cur;
            return;
        end
        base = t_last[c] >> t_div[c];
        nw = cur >> t_div[c];
        t_last[c] = cur;
        if (nw == base) return;
        act = 1;
        tu_prev = t_tcnt[c];
        tu_delta = nw - base;
        tu_tt = {{(TW-16){1'b0}}, t_tcnt[c]} + tu_delta;
        if ({1'b0, t_tcnt[c]} >= t_cycle[c]) begin
            if (tu_tt >= TW'(32'h10000)) tu_val = tu_tt - TW'(32'h10000);
            else begin
                tu_val = tu_tt;    // no modulo: tcnt = tt
                return;
            end
        end else
            tu_val = tu_tt;
        // tu_val % cycle: without the divider when tu_val < 2 * cycle
        if (tu_val < {{(TW-17){1'b0}}, t_cycle[c]}) return;
        if (tu_val < {{(TW-18){1'b0}}, t_cycle[c], 1'b0}) begin
            tu_val = tu_val - {{(TW-17){1'b0}}, t_cycle[c]};
            return;
        end
        need_mod = 1;
    endtask

    task automatic t_update_fin(input int c, input logic [15:0] newcnt);
        logic [15:0] cmp;
        logic [16:0] cyc;
        bit match;
        cyc = t_cycle[c];
        t_tcnt[c] = newcnt;
        for (int i = 0; i < 2; i++) begin
            cmp = t_tgr[c][i] + 16'd1;
            match = (t_tcnt[c] == cmp) || (tu_tt == {{(TW-16){1'b0}}, cmp} && tu_tt == {{(TW-17){1'b0}}, cyc});
            if (!match) begin
                if ({1'b0, tu_prev} >= cyc)
                    match = (cmp > tu_prev && tu_tt >= {{(TW-16){1'b0}}, cmp}) ||
                            ({1'b0, cmp} <= cyc && {1'b0, t_tcnt[c]} < cyc &&
                             (tu_delta - (TW'(32'h10000) - {{(TW-16){1'b0}}, tu_prev})) >= {{(TW-16){1'b0}}, cmp});
                else if ({1'b0, cmp} <= cyc)
                    match = tu_delta >= {{(TW-17){1'b0}}, cyc} ||
                            (tu_prev < cmp && tu_tt >= {{(TW-16){1'b0}}, cmp}) ||
                            (t_tcnt[c] <= tu_prev && t_tcnt[c] >= cmp);
            end
            if (match) begin
                t_isr[c][i] = 1'b1;
                if (t_ier[c][i]) internal_interrupt(V_TIMER0 + 4 * c + i);
            end
        end
        if (tu_tt >= TW'(32'h10000) && (cyc == 17'h10000 || {1'b0, tu_prev} >= cyc)) begin
            t_isr[c][4] = 1'b1;
            if (t_ier[c][4]) internal_interrupt(V_TIMER0 + 4 * c + 2);
        end
    endtask

    // recalc_event() in two steps (one clock each): the delay, then the event time
    logic [31:0] rc_dly;
    logic        rc_off;
    task automatic t_recalc_a(input int c);
        logic [31:0] dly, nd;
        logic [15:0] cmp;
        logic [16:0] cyc;
        rc_off = !t_active[c] || !t_div1[c];
        if (rc_off) return;
        if (t_clr[c] <= 2'd1) t_cycle[c] = {1'b0, t_tgr[c][t_clr[c]]} + 17'd1;
        else t_cycle[c] = 17'h10000;
        cyc = t_cycle[c];
        dly = 32'hffffffff;
        if (t_ier[c][4] && (cyc == 17'h10000 || {1'b0, t_tcnt[c]} >= cyc))
            dly = 32'h10000 - {16'h0, t_tcnt[c]};
        for (int i = 0; i < 2; i++)
            if (t_ier[c][i]) begin
                nd = 32'hffffffff;
                cmp = t_tgr[c][i] + 16'd1;
                if (cmp > t_tcnt[c]) begin
                    if ({1'b0, t_tcnt[c]} >= cyc || {1'b0, cmp} <= cyc)
                        nd = {16'h0, cmp} - {16'h0, t_tcnt[c]};
                end else if ({1'b0, cmp} <= cyc) begin
                    if ({1'b0, t_tcnt[c]} < cyc) nd = {15'h0, cyc} - {16'h0, t_tcnt[c]} + {16'h0, cmp};
                    else nd = 32'h10000 - {16'h0, t_tcnt[c]} + {16'h0, cmp};
                end
                if (dly > nd) dly = nd;
            end
        rc_dly = dly;
    endtask
    task automatic t_recalc_b(input int c, input logic [TW-1:0] cur);
        if (rc_off) t_event[c] = T0;
        else t_event[c] = (rc_dly != 32'hffffffff) ? t_when(cur, t_div[c], rc_dly) : T0;
    endtask


    // ================================================================ SCI (asynchronous)
    task automatic s_clock_update(input int s);
        s_div[s] = 17'((32'd2 << (2 * s_smr[s][1:0])) * ({24'h0, s_brr[s]} + 32'd1));
    endtask
    // clock_start(mode, now): stage 1 returns 1 when (now / step + 1) * step must be computed
    task automatic s_clock_start(input int s, input int mode, output bit need_div);
        need_div = 0;
        if (s_clk[s][mode]) return;
        if (mode == 0) s_txcc[s] = 4'd15; else s_rxcc[s] = 4'd15;
        s_clk[s][mode] = 1'b1;
        if (s_clk[s] != (2'b01 << mode)) return;
        s_step[s] = 17'd0;
        // internal clock (SCR.CKE1 = 0); external clocks are not implemented
        if (!s_scr[s][1]) begin
            s_step[s] = s_div[s];
            need_div = 1;
        end
    endtask
    task automatic s_clock_stop(input int s, input int mode);
        s_clk[s][mode] = 1'b0;
        if (s_clk[s] == 2'b00) begin
            s_event[s] = T0;
            s_step[s] = 17'd0;
        end
    endtask
    task automatic s_tx_start(input int s, output bit need_div);
        s_ssr[s][7] = 1'b1;          // TDRE
        s_tsr[s] = s_tdr[s];
        s_txpar[s] = !s_smr[s][4];
        if (s_scr[s][7]) internal_interrupt(V_SCI0 + 4 * s + 2);
        s_txst[s] = ST_START;
        s_txbit[s] = 4'd1;
        s_clock_start(s, 0, need_div);
    endtask
    task automatic s_rx_start(input int s, output bit need_div);
        s_rxpar[s] = !s_smr[s][4];
        s_rsr[s] = 8'h00;
        s_rxst[s] = ST_START;
        s_rxbit[s] = 4'd1;
        s_clock_start(s, 1, need_div);
    endtask
    function automatic bit s_rx_err(input int s);
        return (s_ssr[s] & 8'h38) != 8'h00;
    endfunction
    task automatic s_rx_done(input int s);
        if (!s_ssr[s][4]) begin
            if (s_smr[s][5] && s_rxpar[s]) s_ssr[s][3] = 1'b1;
            else if (s_ssr[s][6]) s_ssr[s][5] = 1'b1;
            else begin
                s_ssr[s][6] = 1'b1;
                s_rdr[s] = s_rsr[s];
            end
        end
        if (s_scr[s][6]) begin
            if (s_rx_err(s)) internal_interrupt(V_SCI0 + 4 * s);
            else internal_interrupt(V_SCI0 + 4 * s + 1);
        end
        s_rxst[s] = ST_IDLE;
        s_clock_stop(s, 1);
    endtask
    // one transmit tick; returns 1 when tx_start needs a clock start (cannot happen while the
    // TX clock runs, so it is never set here)
    bit sci_txdiv;     // a tick restarted the transmitter's clock: (now / step + 1) * step
    task automatic s_tx_tick(input int s);
        bit nd;
        s_txcc[s] = s_txcc[s] + 4'd1;
        if (s_txcc[s] != 4'd0) return;
        case (s_txst[s])
            ST_START: begin
                sci_tx[s] = 1'b0;
                s_txst[s] = ST_BIT;
                s_txbit[s] = s_smr[s][6] ? 4'd7 : 4'd8;
            end
            ST_BIT: begin
                s_txpar[s] = s_txpar[s] ^ s_tsr[s][0];
                sci_tx[s] = s_tsr[s][0];
                s_tsr[s] = s_tsr[s] >> 1;
                s_txbit[s] = s_txbit[s] - 4'd1;
                if (s_txbit[s] == 4'd0) begin
                    if (s_smr[s][5]) begin
                        s_txst[s] = ST_PARITY;
                        s_txbit[s] = 4'd1;
                    end else begin
                        s_txst[s] = ST_STOP;
                        s_txbit[s] = s_smr[s][3] ? 4'd2 : 4'd1;
                    end
                end
            end
            ST_PARITY: begin
                sci_tx[s] = s_txpar[s];
                s_txst[s] = ST_STOP;
                s_txbit[s] = s_smr[s][3] ? 4'd2 : 4'd1;
            end
            ST_STOP: begin
                sci_tx[s] = 1'b1;
                s_txbit[s] = s_txbit[s] - 4'd1;
                if (s_txbit[s] == 4'd0) begin
                    if (!s_ssr[s][7]) s_tx_start(s, nd);
                    else begin
                        s_txst[s] = ST_LAST;
                        s_txbit[s] = 4'd0;
                    end
                end
            end
            ST_LAST: begin
                s_txst[s] = ST_IDLE;
                s_txbit[s] = 4'd0;
                s_clock_stop(s, 0);
                sci_tx[s] = 1'b1;
                if (s_scr[s][2]) internal_interrupt(V_SCI0 + 4 * s + 3);
                if (s_scr[s][5] && !s_ssr[s][7]) begin
                    s_tx_start(s, nd);
                    if (nd) sci_txdiv = 1;
                end else s_ssr[s][2] = 1'b1;     // TEND
            end
            default: ;
        endcase
    endtask
    task automatic s_rx_tick(input int s);
        s_rxcc[s] = s_rxcc[s] + 4'd1;
`ifdef H8H_DEBUG
        if (s_rxcc[s] == 4'd8 || s_rxcc[s] == 4'd0) $display("periph: sci%0d rx tick cc %0d at %0d val %0d st %0d", s, s_rxcc[s], s_event[s], s_rxval[s], s_rxst[s]);
`endif
        if (s_rxcc[s] != 4'd8) return;
        case (s_rxst[s])
            ST_START: begin
                if (s_rxval[s]) begin
                    s_rxst[s] = ST_IDLE;
                    s_clock_stop(s, 1);
                end else begin
                    s_rxst[s] = ST_BIT;
                    s_rxbit[s] = s_smr[s][6] ? 4'd7 : 4'd8;
                end
            end
            ST_BIT: begin
                s_rsr[s] = s_rsr[s] >> 1;
                if (s_rxval[s]) begin
                    s_rxpar[s] = !s_rxpar[s];
                    s_rsr[s] = s_rsr[s] | ((s_smr[s][7:6] == 2'b01) ? 8'h40 : 8'h80);
                end
                s_rxbit[s] = s_rxbit[s] - 4'd1;
                if (s_rxbit[s] == 4'd0) begin
                    if (s_smr[s][5]) s_rxst[s] = ST_PARITY;
                    else s_rxst[s] = ST_STOP;
                    s_rxbit[s] = 4'd1;
                end
            end
            ST_PARITY: begin
                s_rxpar[s] = s_rxpar[s] ^ s_rxval[s];
                s_rxst[s] = ST_STOP;
                s_rxbit[s] = 4'd1;
            end
            ST_STOP: begin
                if (!s_rxval[s]) s_ssr[s][4] = 1'b1;
                else if (s_smr[s][5] && s_rxpar[s]) s_ssr[s][3] = 1'b1;
                s_rx_done(s);
            end
            default: ;
        endcase
    endtask

    // ================================================================ A/D (H8/3337)
    task automatic a_mode_update();
        if (a_adcsr[4]) begin
            a_start_mode = 4'b1101;     // ACTIVE | REPEAT | ROTATE
            a_start_ch = {a_adcsr[2], 2'b00};
            a_end_ch = a_adcsr[2:0];
        end else begin
            a_start_mode = 4'b0001;
            a_start_ch = a_adcsr[2:0];
            a_end_ch = a_adcsr[2:0];
        end
    endtask
    function automatic logic [TW-1:0] a_conv(input bit first);
        if (first) return a_adcsr[3] ? TW'(134) : TW'(266);
        return a_adcsr[3] ? TW'(128) : TW'(256);
    endfunction
    task automatic a_timeout(input logic [TW-1:0] cur);
        a_addr[a_ch[1:0]] = a_buf;
        if (a_mode[3]) begin
            if (a_ch != a_end_ch) begin
                a_ch = a_ch + 3'd1;
                a_buf = adc_in[a_ch];
                a_event = cur + a_conv(0);
                return;
            end
            a_ch = a_start_ch;
        end
        a_adcsr[7] = 1'b1;
        if (a_adcsr[6]) internal_interrupt(V_ADC);
        if (a_mode[2]) begin
            a_ch = a_start_ch;
            a_buf = adc_in[a_ch];
            a_event = cur + a_conv(0);
            return;
        end
        a_mode = 4'b0000;
        a_adcsr[5] = 1'b0;
    endtask

    // ================================================================ watchdog (type H)
    function automatic int w_shift(input logic [2:0] cks);
        case (cks)
            3'd0: return 1; 3'd1: return 5; 3'd2: return 6; 3'd3: return 7;
            3'd4: return 8; 3'd5: return 9; 3'd6: return 11; default: return 12;
        endcase
    endfunction
    task automatic w_update(input logic [TW-1:0] cur);
        logic [TW-1:0] nx;
        int sh;
        if (w_tcsr[5]) begin
            sh = w_shift(w_tcsr[2:0]);
            nx = {{(TW-8){1'b0}}, w_tcnt} + ((cur >> sh) - (w_base >> sh));
            w_tcnt = nx[7:0];
            w_base = cur;
            if (nx >= TW'(256)) begin
                if (!w_tcsr[6] && !w_tcsr[7]) begin   // interval mode: OVF + WOVI
                    w_tcsr[7] = 1'b1;
                    internal_interrupt(20);
                end
            end
        end else
            w_tcnt = 8'h00;
    endtask
    function automatic logic [TW-1:0] w_next();
        int sh;
        if (!w_tcsr[5]) return T0;
        sh = w_shift(w_tcsr[2:0]);
        return ((w_base >> sh) + TW'(256) - {{(TW-8){1'b0}}, w_tcnt}) << sh;
    endfunction

    // ================================================================ register access
    // one byte handler call; word registers (TCNT, TGR, TBR, watchdog) are handled as words
    logic [15:0] h_rdata;

    task automatic reg_write8(input logic [7:0] off, input logic [7:0] d, input logic [TW-1:0] now,
                              output bit need_div, output int div_sci);
        need_div = 0;
        div_sci = 0;
        casez (off)
            8'h60: begin
                tstr = d;
                for (int c = 0; c < 5; c++) t_active[c] = d[c];   // set_enable (counters current)
                rc_mask = 5'b11111;
            end
            8'h64, 8'h6e, 8'h78, 8'h82, 8'h92: begin
                int c;
                c = (off == 8'h64) ? 0 : (off == 8'h6e) ? 1 : (off == 8'h78) ? 2 : (off == 8'h82) ? 3 : 4;
                t_tcr[c] = d;
                case (d[6:5])
                    2'b00: t_clr[c] = 2'd2;
                    2'b01: t_clr[c] = 2'd0;
                    2'b10: t_clr[c] = 2'd1;
                    default: t_clr[c] = 2'd3;
                endcase
                t_div1[c] = !d[2];
                t_div[c] = d[2] ? 2'd0 : d[1:0];
                rc_mask[c] = 1'b1;
            end
            8'h66, 8'h70, 8'h7a, 8'h84, 8'h94: begin
                int c;
                c = (off == 8'h66) ? 0 : (off == 8'h70) ? 1 : (off == 8'h7a) ? 2 : (off == 8'h84) ? 3 : 4;
                t_tier[c] = d | 8'hf8;
                t_ier[c] = {3'b0, d[2], 2'b0, d[1], d[0]};
                rc_mask[c] = 1'b1;
            end
            8'h67, 8'h71, 8'h7b, 8'h85, 8'h95: begin
                int c;
                c = (off == 8'h67) ? 0 : (off == 8'h71) ? 1 : (off == 8'h7b) ? 2 : (off == 8'h85) ? 3 : 4;
                if (!d[0]) t_isr[c][0] = 1'b0;
                if (!d[1]) t_isr[c][1] = 1'b0;
                if (!d[2]) t_isr[c][4] = 1'b0;
                rc_mask[c] = 1'b1;
            end
            8'had: begin
                r_rtmcsr = (r_rtmcsr & 8'h80) | (d & 8'h78);
                if (!d[7]) r_rtmcsr[7] = 1'b0;
                if (r_rtmcsr[7] && r_rtmcsr[6]) internal_interrupt(21);
            end
            8'hae: r_rtcnt = d;
            8'haf: r_rtcor = d;
            8'hb0, 8'hb8: begin
                int s;
                s = int'(off[3]);
                s_smr[s] = d;
                s_clock_update(s);
            end
            8'hb1, 8'hb9: begin
                int s;
                s = int'(off[3]);
                s_brr[s] = d;
                s_clock_update(s);
            end
            8'hb2, 8'hba: begin
                int s;
                logic [7:0] delta;
                s = int'(off[3]);
                delta = s_scr[s] ^ d;
                s_scr[s] = d;
                s_clock_update(s);
                if (delta[4] && !d[4]) begin
                    s_rxst[s] = ST_IDLE;
                    s_clock_stop(s, 1);
                end
                if (delta[7] && d[7] && s_ssr[s][7]) internal_interrupt(V_SCI0 + 4 * s + 2);
                if (delta[2] && d[2] && s_ssr[s][2]) internal_interrupt(V_SCI0 + 4 * s + 3);
                if (delta[6] && d[6] && s_ssr[s][6]) internal_interrupt(V_SCI0 + 4 * s + 1);
                if (delta[6] && d[6] && s_rx_err(s)) internal_interrupt(V_SCI0 + 4 * s);
                if (delta[7] && !d[7]) clear_interrupt(V_SCI0 + 4 * s + 2);
                if (delta[2] && !d[2]) clear_interrupt(V_SCI0 + 4 * s + 3);
                if (delta[6] && !d[6]) begin
                    clear_interrupt(V_SCI0 + 4 * s + 1);
                    clear_interrupt(V_SCI0 + 4 * s);
                end
            end
            8'hb3, 8'hbb: s_tdr[off[3]] = d;
            8'hb4, 8'hbc: begin
                int s;
                s = int'(off[3]);
                if (s_scr[s][5] && s_ssr[s][7] && s_ssr_read[s][7] && !d[7])
                    s_ssr[s] = s_ssr[s] & 8'h7b;
                s_ssr[s] = (s_ssr[s] & (~s_ssr_read[s] | d | 8'h86) & 8'hfe) | (d & 8'h01);
                s_ssr_read[s] = s_ssr_read[s] & s_ssr[s];
                if (s_txst[s] == ST_IDLE && !s_ssr[s][7]) begin
                    s_tx_start(s, need_div);
                    div_sci = s;
                end
            end
            8'hc5, 8'hc8, 8'hc9, 8'hcd, 8'hd0, 8'hd1, 8'hd4, 8'hd5: begin
                int n;
                case (off)
                    8'hc5: n = 3; 8'hc8: n = 4; 8'hc9: n = 5; 8'hcd: n = 7;
                    8'hd0: n = 8; 8'hd1: n = 9; 8'hd4: n = 10; default: n = 11;
                endcase
                p_ddr[n] = d;
                port_update_output(n, port_mask(n));
            end
            8'hc7, 8'hca, 8'hcb, 8'hcf, 8'hd2, 8'hd3, 8'hd6, 8'hd7: begin
                int n;
                case (off)
                    8'hc7: n = 3; 8'hca: n = 4; 8'hcb: n = 5; 8'hcf: n = 7;
                    8'hd2: n = 8; 8'hd3: n = 9; 8'hd6: n = 10; default: n = 11;
                endcase
                p_dr[n] = d;
                port_update_output(n, port_mask(n));
            end
            8'hda: p_pcr[3] = d;
            8'he8: begin
                logic [7:0] prev;
                prev = a_adcsr;
                a_adcsr = (d & 8'h7f) | (a_adcsr & d & 8'h80);
                a_mode_update();
                if (prev[7] && !a_adcsr[7] && a_mode[1]) begin
                    a_mode[1] = 1'b0;     // HALTED (not reached: no suspend on the 3337)
                end
                if (!prev[5] && a_adcsr[5]) begin
                    a_mode = a_start_mode;
                    a_ch = a_start_ch;
                    a_buf = adc_in[a_ch];
                    a_event = now + a_conv(1);
                    a_powered = 1'b1;
                end
            end
            8'he9: begin
                a_adcr = d;
                a_mode_update();
            end
            8'hf2: begin
                syscr = d;
                set_filter_from(cpu_ccr, d[3]);
            end
            8'hf4: begin
                iscr = d;
                irq_edge = d;
                check_level_irqs(1);
            end
            8'hf5: begin
                ier = d;
                upd_req = 1'b1;
            end
            8'hf6: begin
                isr = isr & d;
                check_level_irqs(0);
                upd_req = 1'b1;
            end
            8'hf8: icr = (icr & 24'h0000ff) | {16'h0, d};          // MAME icr_w keeps only
            8'hf9: icr = (icr & 24'h00ff00) | {8'h0, d, 8'h0};     // the written byte
            8'hfa: icr = (icr & 24'hff0000) | {d, 16'h0};
            default: ;
        endcase
    endtask

    task automatic reg_read8(input logic [7:0] off, output logic [7:0] r);
        r = 8'h00;
        casez (off)
            8'h60: r = tstr;
            8'h64: r = t_tcr[0]; 8'h6e: r = t_tcr[1]; 8'h78: r = t_tcr[2]; 8'h82: r = t_tcr[3]; 8'h92: r = t_tcr[4];
            8'h66: r = t_tier[0]; 8'h70: r = t_tier[1]; 8'h7a: r = t_tier[2]; 8'h84: r = t_tier[3]; 8'h94: r = t_tier[4];
            8'h67, 8'h71, 8'h7b, 8'h85, 8'h95: begin
                int c;
                c = (off == 8'h67) ? 0 : (off == 8'h71) ? 1 : (off == 8'h7b) ? 2 : (off == 8'h85) ? 3 : 4;
                r = {5'b11111, t_isr[c][4], t_isr[c][1], t_isr[c][0]};
            end
            8'had: r = r_rtmcsr | 8'h07;
            8'hae: r = r_rtcnt;
            8'haf: r = r_rtcor;
            8'hb0, 8'hb8: r = s_smr[off[3]];
            8'hb1, 8'hb9: r = s_brr[off[3]];
            8'hb2, 8'hba: r = s_scr[off[3]];
            8'hb3, 8'hbb: r = s_tdr[off[3]];
            8'hb4, 8'hbc: begin
                s_ssr_read[off[3]] = s_ssr[off[3]];
                r = s_ssr[off[3]];
            end
            8'hb5, 8'hbd: r = s_rdr[off[3]];
            8'hc5, 8'hc8, 8'hc9, 8'hcd, 8'hd0, 8'hd1, 8'hd4, 8'hd5: r = 8'hff;
            8'hc7: port_read(3, r);
            8'hca: port_read(4, r);
            8'hcb: port_read(5, r);
            8'hce: port_read(6, r);
            8'hcf: port_read(7, r);
            8'hd2: port_read(8, r);
            8'hd3: port_read(9, r);
            8'hd6: port_read(10, r);
            8'hd7: port_read(11, r);
            8'hda: r = p_pcr[3];
            8'b1110_0???: r = off[0] ? {a_addr[off[2:1]][1:0], 6'h00} : a_addr[off[2:1]][9:2];
            8'he8: r = a_adcsr;
            8'he9: r = a_adcr;
            8'hf2: r = syscr;
            8'hf4: r = iscr;
            8'hf5: r = ier;
            8'hf6: r = isr;
            8'hf8: r = icr[7:0];
            8'hf9: r = icr[15:8];
            8'hfa: r = icr[23:16];
            default: r = 8'h00;
        endcase
    endtask

    // 16-bit registers: TCNT, TGR, TBR, watchdog
    function automatic bit is_word_reg(input logic [7:0] off);
        logic [7:0] e;
        e = {off[7:1], 1'b0};
        return e == 8'h68 || e == 8'h72 || e == 8'h7c || e == 8'h86 || e == 8'h96 ||
               (e >= 8'h6a && e <= 8'h6c) || (e >= 8'h74 && e <= 8'h76) || (e >= 8'h7e && e <= 8'h80) ||
               (e >= 8'h88 && e <= 8'h8e) || (e >= 8'h98 && e <= 8'h9e) || e == 8'ha8 || e == 8'haa;
    endfunction
    // which timer channel a register belongs to (5 = none)
    function automatic int t_chan(input logic [7:0] off);
        if (off >= 8'h64 && off <= 8'h6d) return 0;
        if (off >= 8'h6e && off <= 8'h77) return 1;
        if (off >= 8'h78 && off <= 8'h81) return 2;
        if (off >= 8'h82 && off <= 8'h8f) return 3;
        if (off >= 8'h92 && off <= 8'h9f) return 4;
        return 5;
    endfunction
    // does the handler call update_counter() first?
    function automatic bit t_needs_update(input logic [7:0] off, input bit we);
        logic [7:0] r;
        int c;
        c = t_chan(off);
        if (c == 5) return off == 8'h60 && we;
        r = off - ((c == 0) ? 8'h64 : (c == 1) ? 8'h6e : (c == 2) ? 8'h78 : (c == 3) ? 8'h82 : 8'h92);
        if (r == 8'd1) return 0;                    // TIOR
        if (r >= 8'd8) return we && r < 8'd12;     // TBR: no; TGR write: yes
        if (r == 8'd0 || r == 8'd2) return we;     // TCR, TIER write
        return 1;                                   // TSR, TCNT read and write
    endfunction

    task automatic word_access(input logic [7:0] off, input bit we, input logic [15:0] d,
                               input logic [15:0] mmask, input logic [TW-1:0] now,
                               output logic [15:0] r);
        int c;
        logic [7:0] e;
        e = {off[7:1], 1'b0};
        r = 16'h0;
        c = t_chan(off);
        if (e == 8'ha8) begin
            if (!we) begin
                w_update(now);
                r = {(w_tcsr | 8'h18), w_tcnt};
            end else if (mmask == 16'hffff) begin
                if (d[15:8] == 8'ha5) begin
                    w_update(now);
                    if (!w_tcsr[5] && d[5]) w_base = now;
                    w_tcsr = (w_tcsr & d[7:0] & 8'h80) | (d[7:0] & 8'h7f);
                end
                if (d[15:8] == 8'h5a && w_tcsr[5]) begin
                    w_tcnt = d[7:0];
                    w_base = now;
                end
            end
        end else if (e == 8'haa) begin
            r = {w_rst | 8'h3f, 8'h00};
        end else if (c < 5) begin
            logic [7:0] base;
            int k;
            base = (c == 0) ? 8'h64 : (c == 1) ? 8'h6e : (c == 2) ? 8'h78 : (c == 3) ? 8'h82 : 8'h92;
            k = int'(e) - int'(base);
            if (k == 4) begin
                r = t_tcnt[c];
                if (we) begin
                    t_tcnt[c] = (t_tcnt[c] & ~mmask) | (d & mmask);
                    rc_mask[c] = 1'b1;
                end
            end else begin
                int g;
                g = (k - 6) / 2;     // TGRA 0, TGRB 1, TBRA 2, TBRB 3
                r = t_tgr[c][g];
                if (we) begin
                    t_tgr[c][g] = (t_tgr[c][g] & ~mmask) | (d & mmask);
                    if (g < 2) rc_mask[c] = 1'b1;
                end
            end
        end
    endtask

    // ================================================================ sequencer
    // The earliest event, registered in two stages (MAME's device order on ties: A/D, SCI0,
    // SCI1, timers 0-4, watchdog). The sequencer waits EV_LAT clocks after changing event times.
    logic [TW-1:0] w_next_r;
    always_ff @(posedge clk) w_next_r <= w_next();
    logic [TW-1:0] ev_t [9];
    always_comb begin
        ev_t[0] = a_event;
        ev_t[1] = s_event[0]; ev_t[2] = s_event[1];
        for (int c = 0; c < 5; c++) ev_t[3 + c] = t_event[c];
        ev_t[8] = w_next_r;
    end
    function automatic logic later(input logic [TW-1:0] a, input logic [TW-1:0] b);
        // a is not earlier than b (0 = no event)
        return a == T0 || (b != T0 && a >= b);
    endfunction
    logic [2:0][TW-1:0] ev1_t;
    logic [2:0][3:0]    ev1_d;
    logic [TW-1:0]      ev_min;
    logic [3:0]         ev_dev;
    always_ff @(posedge clk) begin
        for (int k = 0; k < 3; k++) begin
            logic [TW-1:0] m;
            logic [3:0] d;
            m = ev_t[3 * k];
            d = 4'(3 * k);
            for (int j = 1; j < 3; j++)
                if (!later(ev_t[3 * k + j], m)) begin
                    m = ev_t[3 * k + j];
                    d = 4'(3 * k + j);
                end
            ev1_t[k] <= m;
            ev1_d[k] <= d;
        end
        begin : st2
            logic [TW-1:0] m;
            logic [3:0] d;
            m = ev1_t[0];
            d = ev1_d[0];
            for (int k = 1; k < 3; k++)
                if (!later(ev1_t[k], m)) begin
                    m = ev1_t[k];
                    d = ev1_d[k];
                end
            ev_min <= m;
            ev_dev <= d;
        end
    end
    localparam int EV_LAT = 3;
    logic [1:0] settle;
    assign next_event = ev_min;

    typedef enum logic [4:0] {
        Q_IDLE, Q_RESET, Q_WAIT, Q_UPD, Q_DONE,
        Q_A_PRE, Q_A_NEXT, Q_A_DO, Q_A_DIV, Q_A_RC, Q_A_END,
        Q_T_U1, Q_T_MOD, Q_T_FIN, Q_RC_A, Q_RC_B,
        Q_P_FIND, Q_P_RC, Q_P_SET, Q_P_SDIV, Q_RX_DIV, Q_SETTLE
    } q_t;
    q_t q, t_ret, s_ret, rc_ret;
    logic [TW-1:0] rc_now;
    logic [1:0] job;              // 0 acc, 1 proc, 2 rx
    logic       do_update;
    logic [TW-1:0] now, t_now;    // the operation's time; update_counter's time
    logic [2:0] tc;               // the timer channel the datapath works on
    logic       tstr_all;         // TSTR write: update every channel
    int         cur_sci;
    logic [15:0] rd;
    logic [7:0]  rst_port;

    assign syscr_ue = syscr[3];
    assign idle = q == Q_IDLE && !upd_req && !do_update && !upd_s1 && !irq_taken && !filter_we;

    always_ff @(posedge clk) begin
        acc_done <= 1'b0;
        proc_done <= 1'b0;
        rx_done <= 1'b0;
        do_update <= 1'b0;
        pw_stb = 1'b0;
        pr_stb = 1'b0;
        sci_txdiv = 0;

        // divider
        if (dv_go) begin
            dv_go <= 1'b0;
            dv_busy <= 1'b1;
            dv_i <= 7'(TW);
            dv_r <= T0;
        end else if (dv_busy) begin
            if (dv_i != 7'd0) begin
                if (!dv_try[TW]) begin
                    dv_r <= dv_try[TW-1:0];
                    dv_n <= {dv_n[TW-2:0], 1'b1};
                end else begin
                    dv_r <= {dv_r[TW-2:0], dv_n[TW-1]};
                    dv_n <= {dv_n[TW-2:0], 1'b0};
                end
                dv_i <= dv_i - 7'd1;
            end else
                dv_busy <= 1'b0;
        end

        if (rst) begin
            q <= Q_RESET;
            rst_port <= 8'd3;
            pend = '0; ier = 0; isr = 0; iscr = 0; irq_input = 0; irq_edge = 0; icr = 0;
            icr_filter = 0; upd_req = 0; syscr = 8'h09; rc_mask = 0;
            p_dr = '0; p_pcr = '0; p_last = '1;
            p_ddr = '0;
            p_ddr[4] = 8'h0f; p_ddr[5] = 8'h80; p_ddr[7] = 8'hf0;
            tstr = 8'he0;
            for (int c = 0; c < 5; c++) begin
                t_tcr[c] = 0; t_tier[c] = 0; t_ier[c] = 0; t_isr[c] = 0; t_tcnt[c] = 0;
                t_tgr[c] = '1; t_div[c] = 0; t_div1[c] = 1; t_clr[c] = 2'd2; t_cycle[c] = 17'h10000;
                t_last[c] = T0; t_event[c] = T0; t_active[c] = 1'b0;
            end
            for (int i = 0; i < 2; i++) begin
                s_rdr[i] = 0; s_tdr[i] = 8'hff; s_smr[i] = 0; s_scr[i] = 0; s_ssr[i] = 8'h84;
                s_ssr_read[i] = 0; s_brr[i] = 8'hff; s_rsr[i] = 0; s_tsr[i] = 8'hff;
                s_txst[i] = ST_IDLE; s_rxst[i] = ST_IDLE; s_txbit[i] = 0; s_rxbit[i] = 0;
                s_txcc[i] = 0; s_rxcc[i] = 0; s_clk[i] = 0; s_txpar[i] = 0; s_rxpar[i] = 0;
                s_rxval[i] = 1'b1; s_event[i] = T0; s_step[i] = 0;
                s_div[i] = 17'd512;     // clock_update() with SMR 0, BRR FF
            end
            sci_tx = 2'b11;
            a_addr = '0; a_buf = 0; a_adcsr = 0; a_adcr = 0; a_mode = 0; a_ch = 0; a_event = T0;
            a_powered = 1'b1;
            a_start_mode = 4'b0001; a_start_ch = 3'd0; a_end_ch = 3'd0;
            w_tcsr = 0; w_tcnt = 0; w_rst = 0; w_base = T0;
            r_rtmcsr = 0; r_rtcnt = 0; r_rtcor = 8'hff;
            dv_go <= 1'b0; dv_busy <= 1'b0;
        end else begin
            case (q)
                Q_RESET: begin
                    // h8_port device_reset: update_output() for each port; IRQ0, asserted by the
                    // board (machine_reset), arrives through irq_pin
                    if (rst_port <= 8'd11) begin
                        port_update_output(int'(rst_port), port_mask(int'(rst_port)));
                        rst_port <= rst_port + 8'd1;
                    end else
                        q <= Q_IDLE;
                end

                Q_IDLE: begin
                    // pin changes (h8_intc set_input)
                    for (int i = 0; i < 6; i++)
                        if (irq_pin[i] != irq_input[i]) begin
                            bit set;
                            set = irq_edge[i] ? (irq_pin[i] && !irq_input[i]) : irq_pin[i];
                            irq_input[i] = irq_pin[i];
                            if (set) begin
                                isr[i] = 1'b1;
                                upd_req = 1'b1;
                            end
                        end
                    if (irq_taken) begin
                        int v;
                        v = int'(irq_taken_vector);
`ifdef H8H_DEBUG
                        $display("periph: interrupt_taken %0d", v);
`endif
                        pend[v] = 1'b0;
                        if (v >= 12 && v < 20)
                            if (irq_edge[v - 12] || !irq_input[v - 12]) isr[v - 12] = 1'b0;
                        upd_req = 1'b1;
                    end
                    if (filter_we) begin
                        icr_filter = filter;
                        upd_req = 1'b1;
                    end
                    if (acc_req) begin
                        job <= 2'd0;
                        now = acc_time;
                        pass = 0;
                        q <= Q_A_PRE;
                    end else if (proc_req) begin
                        job <= 2'd1;
                        now = proc_time;
                        q <= Q_P_FIND;
                    end else if (rx_req) begin
                        int i;
                        bit nd;
                        i = int'(rx_sci);
                        job <= 2'd2;
                        nd = 0;
                        if (rx_val != s_rxval[i] && s_clk[i][1])
                            if (s_rxcc[i] == 4'd1 || s_rxcc[i] == 4'd15) s_rxcc[i] = 4'd0;
                        if (s_rxst[i] == ST_IDLE && !s_smr[i][7] && s_scr[i][4] && s_rxval[i] && !rx_val)
                            s_rx_start(i, nd);
                        s_rxval[i] = rx_val;
`ifdef H8H_DEBUG
                        $display("periph: sci%0d rx_w %0d now %0d cc %0d clk %b nd %0d", i, rx_val, rx_now, s_rxcc[i], s_clk[i], nd);
`endif
                        if (nd) begin
                            cur_sci = i;
                            dv_n <= rx_now;
                            dv_d <= s_step[i];
                            dv_go <= 1'b1;
                            q <= Q_RX_DIV;
                        end else begin
                            settle <= 2'(EV_LAT);
                            s_ret <= Q_UPD;
                            q <= Q_SETTLE;
                        end
                    end else if (upd_req) begin
                        upd_req = 1'b0;
                        do_update <= 1'b1;
                    end
                end

                // ---------------------------------------------------- timer datapath
                // update_counter(t_now) for channel tc, then back to t_ret
                Q_T_U1: begin
                    bit nm, act;
                    t_update_start(int'(tc), t_now, nm, act);
                    if (nm) begin
                        dv_n <= tu_val;
                        dv_d <= t_cycle[tc];
                        dv_go <= 1'b1;
                        q <= Q_T_MOD;
                    end else if (act)
                        q <= Q_T_FIN;
                    else
                        q <= t_ret;
                end
                Q_T_MOD: if (!dv_go && !dv_busy) begin
                    tu_val = dv_r;
                    q <= Q_T_FIN;
                end
                Q_T_FIN: begin
                    t_update_fin(int'(tc), tu_val[15:0]);
                    q <= t_ret;
                end

                // ---------------------------------------------------- register access
                Q_A_PRE: begin
                    // the timer handlers' update_counter() first (TSTR: every channel)
                    logic [7:0] off;
                    int c;
                    off = acc_word ? {acc_off[7:1], pass[0]} : acc_off;
                    c = t_chan(off);
                    t_now = now;
                    if (t_needs_update(off, acc_we)) begin
                        tstr_all <= c == 5;
                        tc <= c == 5 ? 3'd0 : 3'(c);
                        t_ret <= Q_A_NEXT;
                        q <= Q_T_U1;
                    end else
                        q <= Q_A_DO;
                end
                Q_A_NEXT: begin
                    if (tstr_all && tc != 3'd4) begin
                        tc <= tc + 3'd1;
                        q <= Q_T_U1;
                    end else
                        q <= Q_A_DO;
                end
                Q_A_DO: begin
                    logic [7:0] off, b;
                    bit nd;
                    int ds;
                    off = acc_word ? {acc_off[7:1], pass[0]} : acc_off;
                    nd = 0;
                    ds = 0;
                    if (is_word_reg(off)) begin
                        logic [15:0] mm, w;
                        mm = acc_word ? 16'hffff : (off[0] ? 16'h00ff : 16'hff00);
                        word_access(off, acc_we, acc_word ? acc_wdata : {acc_wdata[7:0], acc_wdata[7:0]}, mm, now, w);
                        rd = acc_word ? w : {8'h0, off[0] ? w[7:0] : w[15:8]};
                        pass = 1;            // one handler call
                    end else if (acc_we)
                        reg_write8(off, acc_word ? (pass == 0 ? acc_wdata[15:8] : acc_wdata[7:0]) : acc_wdata[7:0],
                                   now, nd, ds);
                    else begin
                        reg_read8(off, b);
                        if (acc_word) begin
                            if (pass == 0) rd[15:8] = b; else rd[7:0] = b;
                        end else
                            rd = {8'h0, b};
                    end
                    if (nd) begin
                        cur_sci = ds;
                        dv_n <= now;
                        dv_d <= s_step[ds];
                        dv_go <= 1'b1;
                        q <= Q_A_DIV;
                    end else
                        q <= Q_A_RC;
                end
                Q_A_DIV: if (!dv_go && !dv_busy) begin
                    // clock_event = (now / step + 1) * step
                    s_event[cur_sci] = (dv_n + TW'(1)) * {{(TW-17){1'b0}}, s_step[cur_sci]};
                    q <= Q_A_RC;
                end
                Q_A_RC: begin
                    // recalc_event() owed by the handler, one channel at a time
                    if (rc_mask != 5'd0) begin
                        int c;
                        c = 0;
                        for (int k = 4; k >= 0; k--) if (rc_mask[k]) c = k;
                        tc <= 3'(c);
                        rc_mask[c] = 1'b0;
                        rc_now <= now;
                        rc_ret <= Q_A_RC;
                        q <= Q_RC_A;
                    end else
                        q <= Q_A_END;
                end
                Q_RC_A: begin
                    t_recalc_a(int'(tc));
                    q <= Q_RC_B;
                end
                Q_RC_B: begin
                    t_recalc_b(int'(tc), rc_now);
                    q <= rc_ret;
                end
                Q_A_END: begin
                    if (acc_word && pass == 0) begin
                        pass = 1;
                        q <= Q_A_PRE;
                    end else begin
                        settle <= 2'(EV_LAT);
                        s_ret <= Q_UPD;
                        q <= Q_SETTLE;
                    end
                end

                // ---------------------------------------------------- events
                Q_SETTLE: begin
                    // wait for the event-time pipeline
                    if (settle == 2'd0) q <= s_ret;
                    else settle <= settle - 2'd1;
                end
                Q_P_FIND: begin
                    if (ev_min == T0 || ev_min > now)
                        q <= Q_UPD;
                    else begin
                        logic [TW-1:0] et;
                        et = ev_min;
                        if (ev_dev == 4'd0) begin
                            a_event = T0;
                            a_timeout(et);
                            settle <= 2'(EV_LAT);
                            s_ret <= Q_P_FIND;
                            q <= Q_SETTLE;
                        end else if (ev_dev <= 4'd2) begin
                            int i;
                            i = int'(ev_dev) - 1;
                            if (s_clk[i][0]) s_tx_tick(i);
                            if (s_clk[i][1]) s_rx_tick(i);
                            if (sci_txdiv) begin
                                // clock_start(CLK_TX) with now = total_cycles(); MAME's
                                // internal_update then still adds a step
                                cur_sci = i;
                                dv_n <= now;
                                dv_d <= s_step[i];
                                dv_go <= 1'b1;
                                q <= Q_P_SDIV;
                            end else begin
                                if (s_clk[i] != 2'b00 && s_step[i] != 17'd0)
                                    s_event[i] = s_event[i] + {{(TW-17){1'b0}}, s_step[i]};
                                else
                                    s_event[i] = T0;
                                settle <= 2'(EV_LAT);
                                s_ret <= Q_P_FIND;
                                q <= Q_SETTLE;
                            end
                        end else if (ev_dev <= 4'd7) begin
                            // update_counter(event_time); recalc_event(event_time)
                            tc <= 3'(ev_dev - 4'd3);
                            t_now = et;
                            t_ret <= Q_P_RC;
                            q <= Q_T_U1;
                        end else begin
                            w_update(et);
                            settle <= 2'(EV_LAT);
                            s_ret <= Q_P_FIND;
                            q <= Q_SETTLE;
                        end
                    end
                end
                Q_P_RC: begin
                    // recalc_event(event_time), then wait for the event pipeline
                    rc_now <= t_now;
                    rc_ret <= Q_P_SET;
                    q <= Q_RC_A;
                end
                Q_P_SET: begin
                    settle <= 2'(EV_LAT);
                    s_ret <= Q_P_FIND;
                    q <= Q_SETTLE;
                end
                Q_P_SDIV: if (!dv_go && !dv_busy) begin
                    s_event[cur_sci] = (dv_n + TW'(1)) * {{(TW-17){1'b0}}, s_step[cur_sci]} +
                                       {{(TW-17){1'b0}}, s_step[cur_sci]};
                    settle <= 2'(EV_LAT);
                    s_ret <= Q_P_FIND;
                    q <= Q_SETTLE;
                end
                Q_RX_DIV: if (!dv_go && !dv_busy) begin
                    s_event[cur_sci] = (dv_n + TW'(1)) * {{(TW-17){1'b0}}, s_step[cur_sci]};
                    settle <= 2'(EV_LAT);
                    s_ret <= Q_UPD;
                    q <= Q_SETTLE;
                end

                Q_UPD: begin
                    if (upd_req) begin
                        upd_req = 1'b0;
                        do_update <= 1'b1;
                        q <= Q_DONE;
                    end else
                        q <= Q_DONE;
                end
                Q_DONE: begin
                    // wait for the interrupt request pipeline (two stages)
                    if (!do_update && !upd_s1) begin
                        acc_rdata <= rd;
                        if (job == 2'd0) acc_done <= 1'b1;
                        else if (job == 2'd1) proc_done <= 1'b1;
                        else rx_done <= 1'b1;
                        q <= Q_WAIT;
                    end
                end
                // requests are levels, held until done: wait for this one to drop
                Q_WAIT: if (job == 2'd0 ? !acc_req : job == 2'd1 ? !proc_req : !rx_req) q <= Q_IDLE;
                default: q <= Q_IDLE;
            endcase
        end
    end
endmodule
