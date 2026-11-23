// SPDX-License-Identifier: MIT
// uPD7810/7811 on-chip peripherals: ports, timers 0/1, timer F/F, 16-bit event counter,
// serial interface, A/D converter, interrupt request register and priority.
//
// Transcribed from MAME's upd7810.cpp (BSD-3-Clause, Juergen Buchmueller): handle_timers,
// update_sio, upd7810_sio_output/input, upd7810_take_irq, RP/WP, write_smh, write_EOM.
// MAME's quirks are kept on purpose (serial format tables, the 0xFC transmit stop bit, the
// shared timer vectors that only clear IRR when the other source is masked).
//
// adv_req advances the peripherals by adv_cycles machine cycles, as MAME's
// handle_timers(cycles) before each instruction: timers, the event counter and the
// internal-clock serial port step one cycle at a time (equivalent to MAME's while-loops);
// the A/D converter updates once for the whole batch, as MAME does.

module upd7810_periph (
    input  logic        clk,
    input  logic        reset,

    // core side
    input  logic [5:0]  sfr_id,
    input  logic        sfr_req,
    input  logic        sfr_we,
    input  logic [15:0] sfr_wdata,
    output logic [15:0] sfr_rdata,
    output logic        sfr_ack,

    input  logic [4:0]  skit_id,
    input  logic        skit_req,
    output logic        skit_flag,
    output logic        skit_ack,

    input  logic [6:0]  adv_cycles,
    input  logic        adv_req,
    output logic        adv_ack,

    output logic [15:0] irq_vec,
    output logic        irq_nmi,
    input  logic        irq_ack,

    // board side
    input  logic [7:0]  pa_in, pb_in, pc_in, pd_in, pf_in,
    output logic [7:0]  pa_out, pb_out, pc_out, pd_out, pf_out,   // pin values as MAME's callbacks see them
    output logic        pa_we, pb_we, pc_we, pd_we, pf_we,        // one pulse per MAME WP() call
    input  logic [7:0]  an [8],
    input  logic        rxd,
    output logic        txd,
    input  logic        sck_edge,     // external SCK changed state (MAME sck_w with a new level)
    input  logic        sck_level,    // the new level
    input  logic        nmi_line,     // ASSERT = 1
    input  logic        int1_line,    // ASSERT = 1 (rising edge sets INTF1)
    input  logic        int1_rise,    // a rising edge the board saw between two of its updates
    output logic        pa_rd,        // port A read through RP() with MA != 0 (MAME pa_in_cb)
    input  logic        int2_line,    // ASSERT = 1 (falling-edge logic as MAME execute_set_input)
    output logic        ram_enable,   // MM bit 3: internal RAM FF00-FFFF visible
    output logic        to, co0, co1,
    output logic [15:0] r_irr
);

    // ------------------------------------------------------------------ registers
    logic [7:0]  MA, MB, MC, MCC, MM, MF, MT, TMM, ETMM, EOM, SML, SMH, ANM, PANM, MKL, MKH, ZCM;
    logic [7:0]  TXB, RXB, TM0, TM1, CNT0, CNT1;
    logic [7:0]  CR [4];
    logic [15:0] ETM0, ETM1, ECNT, ECPT;
    logic [15:0] IRR, ITF;
    logic [7:0]  pa_o, pb_o, pc_o, pd_o, pf_o;     // m_px_out
    logic        TO, CO0, CO1, LV0, LV1, TXD, RXD, SCK, CI;
    logic        nmi_q, int1_q, int2_q;            // m_nmi, m_int1, m_int2 (physical, inverted)
    logic [15:0] txs, rxs;
    logic [7:0]  txcnt, rxcnt;
    logic        txbuf, edges;
    logic [7:0]  ovc0, ovc1, ovce;
    logic [15:0] ovcs;
    logic [15:0] adcnt;
    logic [7:0]  adtot, tmpcr;
    logic        shdone;
    logic [1:0]  adout, adin;
    logic [2:0]  adin_sel;
    logic [2:0]  adrange;

    localparam logic [15:0] INTFNMI = 16'h0001, INTFT0 = 16'h0002, INTFT1 = 16'h0004,
                            INTF1 = 16'h0008, INTF2 = 16'h0010, INTFE0 = 16'h0020,
                            INTFE1 = 16'h0040, INTFEIN = 16'h0080, INTFAD = 16'h0100,
                            INTFSR = 16'h0200, INTFST = 16'h0400, INTER = 16'h0800,
                            INTOV = 16'h1000;
    localparam logic [15:0] INTAN4 = 16'h0001, INTAN5 = 16'h0002, INTAN6 = 16'h0004,
                            INTAN7 = 16'h0008, INTSB = 16'h0010;

    // selector ids (tools/upd7810_gen.py SFR)
    localparam logic [5:0] S_PA = 0, S_PB = 1, S_PC = 2, S_PD = 3, S_PF = 4, S_MKH = 5,
        S_MKL = 6, S_ANM = 7, S_SMH = 8, S_SML = 9, S_EOM = 10, S_ETMM = 11, S_TMM = 12,
        S_MM = 13, S_MCC = 14, S_MA = 15, S_MB = 16, S_MC = 17, S_MF = 18, S_TXB = 19,
        S_RXB = 20, S_TM0 = 21, S_TM1 = 22, S_CR0 = 23, S_CR1 = 24, S_CR2 = 25, S_CR3 = 26,
        S_ZCM = 27, S_MT = 28, S_PT = 29, S_ETM0 = 30, S_ETM1 = 31, S_ECNT = 32, S_ECPT = 33;

    assign ram_enable = MM[3];
    assign r_irr = IRR;
    assign to = TO; assign co0 = CO0; assign co1 = CO1; assign txd = TXD;

    function automatic logic par7(input logic [7:0] n);
        par7 = ^n[6:0];
    endfunction
    function automatic logic par8(input logic [7:0] n);
        par8 = ^n;
    endfunction

    // ------------------------------------------------------------------ port reads (RP)
    function automatic logic [7:0] read_pc();
        logic [7:0] d;
        d = (pc_in & MC) | (pc_o & ~MC);
        if (MCC[0]) d[0] = TXD;
        if (MCC[1]) d[1] = RXD;
        if (MCC[2]) d[2] = SCK;
        if (MCC[3]) d[3] = int2_q;
        if (MCC[4]) d[4] = TO;
        if (MCC[5]) d[5] = CI;
        if (MCC[6]) d[6] = CO0;
        if (MCC[7]) d[7] = CO1;
        return d;
    endfunction

    function automatic logic [7:0] rp(input logic [2:0] port);
        case (port)
            3'd0: rp = (pa_in & MA) | (pa_o & ~MA);
            3'd1: rp = (pb_in & MB) | (pb_o & ~MB);
            3'd2: rp = read_pc();
            3'd3: case (MM[2:0])
                      3'd0: rp = pd_in;
                      3'd1: rp = pd_o;
                      default: rp = 8'hFF;
                  endcase
            default: case (MM[2:1])
                      2'd0: rp = (pf_in & MF) | (pf_o & ~MF);
                      2'd1: rp = ((pf_in & MF) | (pf_o & ~MF)) | 8'h0F;
                      2'd2: rp = ((pf_in & MF) | (pf_o & ~MF)) | 8'h3F;
                      default: rp = 8'hFF;
                  endcase
        endcase
    endfunction

    // the value MAME's write_pc passes to the port C callback
    function automatic logic [7:0] pc_pins(input logic [7:0] data);
        logic [7:0] d;
        d = (data & ~MC) | (8'hFF & MC);
        if (MCC[0]) d[0] = TXD;
        if (MCC[1]) d[1] = RXD;
        if (MCC[2]) d[2] = SCK;
        if (MCC[3]) d[3] = int2_q;
        if (MCC[4]) d[4] = TO;
        if (MCC[5]) d[5] = CI;
        if (MCC[6]) d[6] = CO0;
        if (MCC[7]) d[7] = CO1;
        return d;
    endfunction

    // ------------------------------------------------------------------ interrupt priority
    always_comb begin
        irq_vec = 16'h0;
        irq_nmi = 1'b0;
        if ((IRR & INTFNMI) != 0) begin irq_vec = 16'h0004; irq_nmi = 1'b1; end
        else if ((IRR & INTFT0) != 0 && !MKL[1]) irq_vec = 16'h0008;
        else if ((IRR & INTFT1) != 0 && !MKL[2]) irq_vec = 16'h0008;
        else if ((IRR & INTF1) != 0 && !MKL[3])  irq_vec = 16'h0010;
        else if ((IRR & INTF2) != 0 && !MKL[4])  irq_vec = 16'h0010;
        else if ((IRR & INTFE0) != 0 && !MKL[5]) irq_vec = 16'h0018;
        else if ((IRR & INTFE1) != 0 && !MKL[6]) irq_vec = 16'h0018;
        else if ((IRR & INTFEIN) != 0 && !MKL[7]) irq_vec = 16'h0020;
        else if ((IRR & INTFAD) != 0 && !MKH[0]) irq_vec = 16'h0020;
        else if ((IRR & INTFSR) != 0 && !MKH[1]) irq_vec = 16'h0028;
        else if ((IRR & INTFST) != 0 && !MKH[2]) irq_vec = 16'h0028;
    end

    // IRR bit the taken interrupt clears (MAME irr_mask)
    function automatic logic [15:0] irq_clear();
        irq_clear = 16'h0;
        if ((IRR & INTFNMI) != 0) irq_clear = INTFNMI;
        else if ((IRR & INTFT0) != 0 && !MKL[1]) irq_clear = MKL[2] ? INTFT0 : 16'h0;
        else if ((IRR & INTFT1) != 0 && !MKL[2]) irq_clear = MKL[1] ? INTFT1 : 16'h0;
        else if ((IRR & INTF1) != 0 && !MKL[3])  irq_clear = MKL[4] ? INTF1 : 16'h0;
        else if ((IRR & INTF2) != 0 && !MKL[4])  irq_clear = MKL[3] ? INTF2 : 16'h0;
        else if ((IRR & INTFE0) != 0 && !MKL[5]) irq_clear = MKL[6] ? INTFE0 : 16'h0;
        else if ((IRR & INTFE1) != 0 && !MKL[6]) irq_clear = MKL[5] ? INTFE1 : 16'h0;
        else if ((IRR & INTFEIN) != 0 && !MKL[7]) irq_clear = MKH[0] ? INTFEIN : 16'h0;
        else if ((IRR & INTFAD) != 0 && !MKH[0]) irq_clear = MKL[7] ? INTFAD : 16'h0;
        else if ((IRR & INTFSR) != 0 && !MKH[1]) irq_clear = MKH[2] ? INTFSR : 16'h0;
        else if ((IRR & INTFST) != 0 && !MKH[2]) irq_clear = MKH[1] ? INTFST : 16'h0;
    endfunction

    // ------------------------------------------------------------------ sequential
    // Side effects that MAME performs inside one call happen here in one clock; the few
    // that chain (TO change -> update_sio(1), CO change -> WP(PORTC)) are expanded inline.
    logic [6:0]  adv_left;
    logic [6:0]  adv_total;
    logic        adv_busy;

    // pending events this clock (set by tasks, applied at the end of the clock)
    logic [15:0] irr_set, irr_n, itf_n;

    // write_pc: port C callback value
    task automatic wp(input logic [2:0] port, input logic [7:0] data);
        case (port)
            3'd0: begin pa_o <= data; pa_out <= (data & ~MA) | MA; pa_we <= 1'b1; end
            3'd1: begin pb_o <= data; pb_out <= (data & ~MB) | MB; pb_we <= 1'b1; end
            3'd2: begin pc_o <= data; pc_out <= pc_pins(data); pc_we <= 1'b1; end
            3'd3: begin
                pd_o <= data;
                case (MM[2:0])
                    3'd0: begin pd_out <= 8'hFF; pd_we <= 1'b1; end
                    3'd1: begin pd_out <= data;  pd_we <= 1'b1; end
                    default: ;
                endcase
            end
            default: begin
                logic [7:0] d;
                pf_o <= data;
                d = (data & ~MF) | MF;
                case (MM[2:1])
                    2'd1: d = d | 8'h0F;
                    2'd2: d = d | 8'h3F;
                    2'd3: d = 8'hFF;
                    default: ;
                endcase
                pf_out <= d; pf_we <= 1'b1;
            end
        endcase
    endtask

    // Serial: one SIO edge (MAME update_sio loop body). Returns via nonblocking updates.
    // Called at most once per clock.
    task automatic sio_edge();
        if (edges == 1'b1) begin
            edges <= 1'b0;
            sio_input();
        end else begin
            edges <= 1'b1;
            sio_output();
        end
    endtask

    task automatic sio_output();
        if (txcnt != 0) begin
            TXD <= txs[0];
            if (MCC[0]) begin
                // WP(PORTC, m_pc_out) with the new TXD
                logic [7:0] d;
                d = pc_pins(pc_o);
                d[0] = txs[0];
                pc_out <= d; pc_we <= 1'b1;
            end
            txs <= {1'b0, txs[15:1]};
            txcnt <= txcnt - 8'd1;
            if (txcnt == 8'd1) irr_set = irr_set | INTFST;
        end else if (SMH[2] && txbuf) begin
            txbuf <= 1'b0;
            if (SML[1:0] != 2'b00) begin
                case (SML[7:2])
                    6'h12, 6'h1A: begin txs <= ({8'h0, TXB} << 1) | 16'h0100; txcnt <= 8'd9; end // 48, 68
                    6'h13, 6'h1B: begin txs <= ({8'h0, TXB} << 1) | 16'h0200; txcnt <= 8'd10; end // 4C, 6C
                    6'h16: begin txs <= ({8'h0, TXB} << 1) | ({15'h0, par7(TXB)} << 8) | 16'h0200; txcnt <= 8'd10; end // 58
                    6'h17: begin txs <= ({8'h0, TXB} << 1) | ({15'h0, par8(TXB)} << 9) | 16'h0400; txcnt <= 8'd11; end // 5C
                    6'h1E: begin txs <= ({8'h0, TXB} << 1) | ({15'h0, ~par7(TXB)} << 8) | 16'h0200; txcnt <= 8'd10; end // 78
                    6'h1F: begin txs <= ({8'h0, TXB} << 1) | ({15'h0, ~par8(TXB)} << 9) | 16'h0400; txcnt <= 8'd11; end // 7C
                    6'h32, 6'h3A: begin txs <= ({8'h0, TXB} << 1) | 16'h0300; txcnt <= 8'd10; end // C8, E8
                    6'h33, 6'h3B: begin txs <= ({8'h0, TXB} << 1) | 16'h0600; txcnt <= 8'd11; end // CC, EC
                    6'h36: begin txs <= ({8'h0, TXB} << 1) | ({15'h0, par7(TXB)} << 8) | 16'h0600; txcnt <= 8'd11; end // D8
                    6'h37: begin txs <= ({8'h0, TXB} << 1) | ({15'h0, par8(TXB)} << 9) | 16'h0C00; txcnt <= 8'd12; end // DC
                    6'h3E: begin txs <= ({8'h0, TXB} << 1) | ({15'h0, ~par7(TXB)} << 8) | 16'h0600; txcnt <= 8'd11; end // F8
                    6'h3F: begin txs <= ({8'h0, TXB} << 1) | ({15'h0, ~par8(TXB)} << 9) | 16'h0400; txcnt <= 8'd12; end // FC (MAME: one stop bit set)
                    default: ;
                endcase
            end else begin
                txs <= {8'h0, TXB};
                txcnt <= 8'd8;
            end
        end
    endtask

    task automatic sio_input();
        logic [15:0] r;
        logic [7:0]  b;
        if (rxcnt != 0) begin
            RXD <= rxd;
            r = {rxd, rxs[15:1]};
            rxs <= r;
            rxcnt <= rxcnt - 8'd1;
            if (rxcnt == 8'd1) begin
                SMH[6] <= 1'b0;
                irr_set = irr_set | INTFSR;
                if (SML[1:0] != 2'b00) begin
                    case (SML[7:2])
                        6'h12, 6'h1A: begin r = (r >> 7) | 16'hFE00; b = {1'b0, r[7:1]};
                            if ((r & 16'h0101) != 16'h0100) irr_set = irr_set | INTER; RXB <= b; rxs <= r; end
                        6'h13, 6'h1B: begin r = (r >> 6) | 16'hFC00; b = r[8:1];
                            if ((r & 16'h0201) != 16'h0200) irr_set = irr_set | INTER; RXB <= b; rxs <= r; end
                        6'h16, 6'h1E: begin r = (r >> 6) | 16'hFC00; b = {1'b0, r[7:1]};
                            if ((r & 16'h0201) != 16'h0200) irr_set = irr_set | INTER;
                            if (par7(b) != r[8]) irr_set = irr_set | INTER; RXB <= b; rxs <= r; end
                        6'h17, 6'h1F: begin r = (r >> 5) | 16'hF800; b = r[8:1];
                            if ((r & 16'h0401) != 16'h0400) irr_set = irr_set | INTER;
                            if (par8(b) != r[9]) irr_set = irr_set | INTER; RXB <= b; rxs <= r; end
                        6'h32, 6'h3A: begin r = (r >> 6) | 16'hFC00; b = {1'b0, r[7:1]};
                            if ((r & 16'h0601) != 16'h0600) irr_set = irr_set | INTER;
                            if (par7(b) != r[8]) irr_set = irr_set | INTER; RXB <= b; rxs <= r; end
                        6'h33, 6'h3B: begin r = (r >> 5) | 16'hF800; b = r[8:1];
                            if ((r & 16'h0C01) != 16'h0C00) irr_set = irr_set | INTER;
                            if (par8(b) != r[9]) irr_set = irr_set | INTER; RXB <= b; rxs <= r; end
                        6'h36, 6'h3E: begin r = (r >> 5) | 16'hF800; b = {1'b0, r[7:1]};
                            if ((r & 16'h0C01) != 16'h0C00) irr_set = irr_set | INTER;
                            if (par7(b) != r[8]) irr_set = irr_set | INTER; RXB <= b; rxs <= r; end
                        6'h37, 6'h3F: begin r = (r >> 4) | 16'hF000; b = r[8:1];
                            if ((r & 16'h1801) != 16'h1800) irr_set = irr_set | INTER;
                            if (par8(b) != r[9]) irr_set = irr_set | INTER; RXB <= b; rxs <= r; end
                        default: ;
                    endcase
                end else begin
                    r = (r >> 8) | 16'hFF00;
                    rxs <= r;
                    RXB <= r[7:0];
                end
            end
        end else if (SMH[3]) begin
            if (SML[1:0] != 2'b00) begin
                // start bit check
                RXD <= rxd;
                r = {rxd, rxs[15:1]};
                rxs <= r;
                if (r[15:14] == 2'b01) begin
                    case (SML[7:2])
                        6'h12, 6'h1A: rxcnt <= 8'd8;
                        6'h13, 6'h1B, 6'h16, 6'h1E, 6'h32, 6'h3A: rxcnt <= 8'd9;
                        6'h17, 6'h1F, 6'h33, 6'h3B, 6'h36, 6'h3E: rxcnt <= 8'd10;
                        6'h37, 6'h3F: rxcnt <= 8'd11;
                        default: rxcnt <= 8'd255;   // MAME: 0 - 1
                    endcase
                end
            end else if (SMH[6]) begin
                rxcnt <= 8'd8;
            end
        end
    endtask

    // update_sio: interval / 2 = divider[SMH & 3] * prescale[SML & 3] / 2
    function automatic logic [15:0] sio_half();
        logic [15:0] div, pre;
        case (SMH[1:0])
            2'b01: div = 16'd128;
            2'b10: div = 16'd8;
            default: div = 16'd2;
        endcase
        case (SML[1:0])
            2'd2: pre = 16'd16;
            2'd3: pre = 16'd64;
            default: pre = 16'd1;
        endcase
        sio_half = (div * pre) >> 1;
    endfunction

    logic draining;
    logic sio_pending;   // update_sio was called: drain OVCS this batch
    wire        t0_run = !TMM[4] && !TMM[3];
    wire [7:0]  div0 = TMM[2] ? 8'd128 : 8'd4;
    wire        t1_run = !TMM[7] && !TMM[6];
    wire [7:0]  div1 = TMM[5] ? 8'd128 : 8'd4;
    wire        e_run = ETMM[3:2] != 2'b00 && (ETMM[1:0] == 2'b00 || (ETMM[1:0] == 2'b01 && CI));

    // upd7810_to_output_change(state): TO-clocked serial (mode 0) takes update_sio(1)
    task automatic to_toggle(input logic v);
        TO <= v;
        if (SMH[1:0] == 2'b00) begin ovcs <= ovcs + 16'd1; sio_pending <= 1'b1; end  // update_sio(1)
        if (MCC[4]) begin
            logic [7:0] pv;
            pv = pc_pins(pc_o);
            pv[4] = v;
            pc_out <= pv; pc_we <= 1'b1;
        end
    endtask

    always_ff @(posedge clk) begin
        logic [15:0] ecnt_n;
        logic [7:0]  cnt0_n, cnt1_n;
        logic        to_n, co0_n, co1_n, lv0_n, lv1_n;
        logic        to_changed;

        sfr_ack <= 1'b0;
        pa_rd <= 1'b0;
        skit_ack <= 1'b0;
        adv_ack <= 1'b0;
        pa_we <= 1'b0; pb_we <= 1'b0; pc_we <= 1'b0; pd_we <= 1'b0; pf_we <= 1'b0;
        irr_set = 16'h0;
        irr_n = IRR;
        itf_n = ITF;

        if (reset) begin
            MA <= 8'hFF; MB <= 8'hFF; MCC <= 8'h00; MC <= 8'hFF; MM <= 8'h08; MF <= 8'hFF;
            MT <= 8'h00; TMM <= 8'hFF; ETMM <= 8'hFF; EOM <= 8'h00; SML <= 8'h00; SMH <= 8'h00;
            PANM <= 8'hFF; ANM <= 8'h00; MKL <= 8'hFF; MKH <= 8'hFF; ZCM <= 8'h00;
            TXB <= 8'h00; RXB <= 8'h00; TM0 <= 8'h00; TM1 <= 8'h00; CNT0 <= 8'h00; CNT1 <= 8'h00;
            CR[0] <= 8'h00; CR[1] <= 8'h00; CR[2] <= 8'h00; CR[3] <= 8'h00;
            ETM0 <= 16'h0; ETM1 <= 16'h0; ECNT <= 16'h0; ECPT <= 16'h0;
            IRR <= 16'h0; ITF <= 16'h0;
            pa_o <= 8'h0; pb_o <= 8'h0; pc_o <= 8'h0; pd_o <= 8'h0; pf_o <= 8'h0;
            TO <= 1'b0; CO0 <= 1'b0; CO1 <= 1'b0; LV0 <= 1'b0; LV1 <= 1'b0;
            TXD <= 1'b0; RXD <= 1'b0; SCK <= 1'b0; CI <= 1'b0;
            nmi_q <= 1'b0; int1_q <= 1'b0; int2_q <= 1'b1;
            txs <= 16'h0; rxs <= 16'hFFFF; txcnt <= 8'h0; rxcnt <= 8'h0; txbuf <= 1'b0;
            edges <= 1'b0;
            ovc0 <= 8'h0; ovc1 <= 8'h0; ovce <= 8'h0; ovcs <= 16'h0;
            adcnt <= 16'h0; adtot <= 8'h0; tmpcr <= 8'h0; shdone <= 1'b0;
            adout <= 2'd0; adin <= 2'd0; adin_sel <= 3'd0; adrange <= 3'd0;
            adv_busy <= 1'b0; draining <= 1'b0; sio_pending <= 1'b0;
            pa_out <= 8'hFF; pb_out <= 8'hFF; pc_out <= 8'hFF; pd_out <= 8'hFF; pf_out <= 8'hFF;
        end else begin
            // -------------------------------------------- external lines (execute_set_input)
            nmi_q <= nmi_line;
            if (!nmi_q && nmi_line) irr_set = irr_set | INTFNMI;
            int1_q <= int1_line;
            if ((!int1_q && int1_line) || int1_rise) irr_set = irr_set | INTF1;
            if (int2_q && int2_line) irr_set = irr_set | INTF2;   // (!m_int2) == CLEAR && ASSERT
            int2_q <= !int2_line;

            // -------------------------------------------- external SCK (sck_w)
            if (sck_edge) begin
                if (SMH[1:0] == 2'b11 && sck_level != SCK) begin
                    // update_sio(1): interval = 2 * prescale; OVCS += 1
                    if (ovcs + 16'd1 >= sio_half()) begin
                        ovcs <= ovcs + 16'd1 - sio_half();
                        sio_edge();
                    end else ovcs <= ovcs + 16'd1;
                end
                SCK <= sck_level;
            end

            // -------------------------------------------- special-register access
            if (sfr_req && !sfr_ack) begin
                sfr_ack <= 1'b1;
                if (!sfr_we) begin
                    case (sfr_id)
                        S_PA: begin sfr_rdata <= {8'h0, rp(3'd0)}; pa_rd <= MA != 8'h00; end
                        S_PB: sfr_rdata <= {8'h0, rp(3'd1)};
                        S_PC: sfr_rdata <= {8'h0, rp(3'd2)};
                        S_PD: sfr_rdata <= {8'h0, rp(3'd3)};
                        S_PF: sfr_rdata <= {8'h0, rp(3'd4)};
                        S_MKH: sfr_rdata <= {8'h0, MKH};
                        S_MKL: sfr_rdata <= {8'h0, MKL};
                        S_ANM: sfr_rdata <= {8'h0, ANM};
                        S_SMH: sfr_rdata <= {8'h0, SMH};
                        S_SML: sfr_rdata <= {8'h0, SML};
                        S_EOM: sfr_rdata <= {8'h0, EOM & 8'h22};
                        S_ETMM: sfr_rdata <= {8'h0, ETMM};
                        S_TMM: sfr_rdata <= {8'h0, TMM};
                        S_MM: sfr_rdata <= {8'h0, MM};
                        S_MCC: sfr_rdata <= {8'h0, MCC};
                        S_MA: sfr_rdata <= {8'h0, MA};
                        S_MB: sfr_rdata <= {8'h0, MB};
                        S_MC: sfr_rdata <= {8'h0, MC};
                        S_MF: sfr_rdata <= {8'h0, MF};
                        S_TXB: sfr_rdata <= {8'h0, TXB};
                        S_RXB: sfr_rdata <= {8'h0, RXB};
                        S_TM0: sfr_rdata <= {8'h0, TM0};
                        S_TM1: sfr_rdata <= {8'h0, TM1};
                        S_CR0: sfr_rdata <= {8'h0, CR[0]};
                        S_CR1: sfr_rdata <= {8'h0, CR[1]};
                        S_CR2: sfr_rdata <= {8'h0, CR[2]};
                        S_CR3: sfr_rdata <= {8'h0, CR[3]};
                        S_ZCM: sfr_rdata <= {8'h0, ZCM};
                        S_MT: sfr_rdata <= {8'h0, MT};
                        S_ECNT: sfr_rdata <= ECNT;
                        S_ECPT: sfr_rdata <= ECPT;
                        default: sfr_rdata <= 16'h0;   // PT: unconnected uPD7807 port
                    endcase
                end else begin
                    logic [7:0] d;
                    d = sfr_wdata[7:0];
                    case (sfr_id)
                        S_PA: wp(3'd0, d);
                        S_PB: wp(3'd1, d);
                        S_PC: wp(3'd2, d);
                        S_PD: wp(3'd3, d);
                        S_PF: wp(3'd4, d);
                        S_MKH: MKH <= d;
                        S_MKL: MKL <= d;
                        S_ANM: ANM <= d;
                        S_SMH: begin
                            if (!SMH[2] && d[2]) irr_set = irr_set | INTFST;
                            SMH <= d;
                        end
                        S_SML: SML <= d;
                        S_EOM: begin
                            // upd7810_write_EOM
                            logic [7:0] e;
                            logic l0, l1, c0, c1;
                            e = d; l0 = LV0; l1 = LV1; c0 = CO0; c1 = CO1;
                            case (e & 8'h0C)
                                8'h04: begin l0 = 1'b0; e = e & 8'hFB; end
                                8'h08: begin l0 = 1'b1; e = e & 8'hF7; end
                                default: ;
                            endcase
                            if (e[0]) begin
                                c0 = l0;
                                if (e[1]) l0 = ~l0;
                                e = e & 8'hFE;
                            end
                            case (e & 8'hC0)
                                8'h40: begin l1 = 1'b0; e = e & 8'hBF; end
                                8'h80: begin l1 = 1'b1; e = e & 8'h7F; end
                                default: ;
                            endcase
                            if (e[4]) begin
                                c1 = l1;
                                if (e[5]) l1 = ~l1;
                                e = e & 8'hEF;
                            end
                            EOM <= e; LV0 <= l0; LV1 <= l1; CO0 <= c0; CO1 <= c1;
                            if ((d[0] && MCC[6]) || (e[4] && MCC[7]) || (d[4] && MCC[7])) begin
                                logic [7:0] pv;
                                pv = pc_pins(pc_o);
                                if (MCC[6]) pv[6] = c0;
                                if (MCC[7]) pv[7] = c1;
                                pc_out <= pv; pc_we <= 1'b1;
                            end
                        end
                        S_ETMM: ETMM <= d;
                        S_TMM: TMM <= d;
                        S_MM: MM <= d;
                        S_MCC: if (MCC != d) begin
                                   logic [7:0] pv;
                                   MCC <= d;
                                   // WP(PORTC, m_pc_out) with the new MCC
                                   pv = (pc_o & ~MC) | MC;
                                   if (d[0]) pv[0] = TXD;
                                   if (d[1]) pv[1] = RXD;
                                   if (d[2]) pv[2] = SCK;
                                   if (d[3]) pv[3] = int2_q;
                                   if (d[4]) pv[4] = TO;
                                   if (d[5]) pv[5] = CI;
                                   if (d[6]) pv[6] = CO0;
                                   if (d[7]) pv[7] = CO1;
                                   pc_out <= pv; pc_we <= 1'b1;
                               end
                        S_MA: if (MA != d) begin MA <= d; pa_out <= (pa_o & ~d) | d; pa_we <= 1'b1; end
                        S_MB: if (MB != d) begin MB <= d; pb_out <= (pb_o & ~d) | d; pb_we <= 1'b1; end
                        S_MC: if (MC != d) begin
                                   logic [7:0] pv;
                                   MC <= d;
                                   pv = (pc_o & ~d) | d;
                                   if (MCC[0]) pv[0] = TXD;
                                   if (MCC[1]) pv[1] = RXD;
                                   if (MCC[2]) pv[2] = SCK;
                                   if (MCC[3]) pv[3] = int2_q;
                                   if (MCC[4]) pv[4] = TO;
                                   if (MCC[5]) pv[5] = CI;
                                   if (MCC[6]) pv[6] = CO0;
                                   if (MCC[7]) pv[7] = CO1;
                                   pc_out <= pv; pc_we <= 1'b1;
                               end
                        S_MF: if (MF != d) begin
                                   logic [7:0] pv;
                                   MF <= d;
                                   pv = (pf_o & ~d) | d;
                                   case (MM[2:1])
                                       2'd1: pv = pv | 8'h0F;
                                       2'd2: pv = pv | 8'h3F;
                                       2'd3: pv = 8'hFF;
                                       default: ;
                                   endcase
                                   pf_out <= pv; pf_we <= 1'b1;
                               end
                        S_TXB: begin TXB <= d; txbuf <= 1'b1; end
                        S_TM0: TM0 <= d;
                        S_TM1: TM1 <= d;
                        S_ZCM: ZCM <= d;
                        S_MT: MT <= d;
                        S_ETM0: ETM0 <= sfr_wdata;
                        S_ETM1: ETM1 <= sfr_wdata;
                        default: ;
                    endcase
                end
            end

            // -------------------------------------------- SKIT / SKNIT
            if (skit_req && !skit_ack) begin
                skit_ack <= 1'b1;
                case (skit_id)
                    5'd1:  begin skit_flag <= (IRR & INTFT0) != 0;  irr_n = irr_n & ~INTFT0; end
                    5'd2:  begin skit_flag <= (IRR & INTFT1) != 0;  irr_n = irr_n & ~INTFT1; end
                    5'd3:  begin skit_flag <= (IRR & INTF1) != 0;   irr_n = irr_n & ~INTF1; end
                    5'd4:  begin skit_flag <= (IRR & INTF2) != 0;   irr_n = irr_n & ~INTF2; end
                    5'd5:  begin skit_flag <= (IRR & INTFE0) != 0;  irr_n = irr_n & ~INTFE0; end
                    5'd6:  begin skit_flag <= (IRR & INTFE1) != 0;  irr_n = irr_n & ~INTFE1; end
                    5'd7:  begin skit_flag <= (IRR & INTFEIN) != 0; irr_n = irr_n & ~INTFEIN; end
                    5'd8:  begin skit_flag <= (IRR & INTFAD) != 0;  irr_n = irr_n & ~INTFAD; end
                    5'd9:  begin skit_flag <= (IRR & INTFSR) != 0;  irr_n = irr_n & ~INTFSR; end
                    5'd10: begin skit_flag <= (IRR & INTFST) != 0;  irr_n = irr_n & ~INTFST; end
                    5'd11: begin skit_flag <= (IRR & INTER) != 0;   irr_n = irr_n & ~INTER; end
                    5'd12: begin skit_flag <= (IRR & INTOV) != 0;   irr_n = irr_n & ~INTOV; end
                    5'd13: begin skit_flag <= (ITF & INTAN4) != 0;  itf_n = itf_n & ~INTAN4; end
                    5'd14: begin skit_flag <= (ITF & INTAN5) != 0;  itf_n = itf_n & ~INTAN5; end
                    5'd15: begin skit_flag <= (ITF & INTAN6) != 0;  itf_n = itf_n & ~INTAN6; end
                    5'd16: begin skit_flag <= (ITF & INTAN7) != 0;  itf_n = itf_n & ~INTAN7; end
                    5'd17: begin skit_flag <= (ITF & INTSB) != 0;   itf_n = itf_n & ~INTSB; end
                    5'd18: skit_flag <= !nmi_q;   // SKIT NMI: line clear; SKNIT: line asserted
                    default: skit_flag <= 1'b0;
                endcase
            end

            // -------------------------------------------- interrupt taken
            if (irq_ack) irr_n = irr_n & ~irq_clear();

            // -------------------------------------------- handle_timers, one cycle a clock
            if (adv_req && !adv_busy && !adv_ack) begin
                adv_busy <= 1'b1;
                adv_left <= adv_cycles;
                adv_total <= adv_cycles;
            end
            if (adv_busy) begin
                // MAME handle_timers(cycles): each counter adds its cycles, then a while-loop
                // drains it. Here: one cycle is added to every running counter, then the
                // pending ticks drain one per clock (timer 0, timer 1, event counter, serial,
                // MAME's order) before the next cycle. A backlog left by a divider change
                // drains exactly as MAME's while-loops do.
                if (draining) begin
                    if (t0_run && ovc0 >= div0) begin
                        ovc0 <= ovc0 - div0;
                        cnt0_n = (TMM[4] ? 8'h0 : CNT0) + 8'd1;
                        cnt1_n = CNT1; to_n = TO; to_changed = 1'b0;
                        if (cnt0_n == TM0) begin
                            cnt0_n = 8'h0;
                            irr_set = irr_set | INTFT0;
                            if (TMM[1:0] == 2'b00) begin to_n = ~to_n; to_changed = 1'b1; end
                            if (TMM[7:5] == 3'b011) begin
                                cnt1_n = cnt1_n + 8'd1;
                                if (cnt1_n == TM1) begin
                                    cnt1_n = 8'h0;
                                    irr_set = irr_set | INTFT1;
                                    if (TMM[1:0] == 2'b01) begin to_n = ~to_n; to_changed = 1'b1; end
                                end
                            end
                        end
                        CNT0 <= cnt0_n; CNT1 <= cnt1_n;
                        if (to_changed) to_toggle(to_n);
                    end else if (t1_run && ovc1 >= div1) begin
                        ovc1 <= ovc1 - div1;
                        cnt1_n = CNT1 + 8'd1;
                        if (cnt1_n == TM1) begin
                            cnt1_n = 8'h0;
                            irr_set = irr_set | INTFT1;
                            if (TMM[1:0] == 2'b01) to_toggle(~TO);
                        end
                        CNT1 <= cnt1_n;
                    end else if (e_run && ovce >= 8'd4) begin
                        ovce <= ovce - 8'd4;
                        ecnt_n = ECNT + 16'd1;
                        co0_n = CO0; co1_n = CO1; lv0_n = LV0; lv1_n = LV1;
                        if (ETM0 == ecnt_n) irr_set = irr_set | INTFE0;
                        if (ETM1 == ecnt_n) irr_set = irr_set | INTFE1;
                        if ((ETMM[5:4] == 2'b00 && ETM0 == ecnt_n) ||
                            (ETMM[5:4] == 2'b10 && ETM0 == ecnt_n) ||
                            (ETMM[5:4] == 2'b11 && (ETM0 == ecnt_n || ETM1 == ecnt_n))) begin
                            co0_n = lv0_n;
                            if (EOM[1]) lv0_n = ~lv0_n;
                        end
                        if ((ETMM[7:6] == 2'b00 && ETM1 == ecnt_n) ||
                            (ETMM[7:6] == 2'b10 && ETM1 == ecnt_n) ||
                            (ETMM[7:6] == 2'b11 && (ETM0 == ecnt_n || ETM1 == ecnt_n))) begin
                            co1_n = lv1_n;
                            if (EOM[5]) lv1_n = ~lv1_n;
                        end
                        case (ETMM[3:2])
                            2'b01: if (ecnt_n == 16'h0) itf_n = itf_n | INTOV;
                            2'b11: if (ETM1 == ecnt_n) ecnt_n = 16'h0;
                            default: ;
                        endcase
                        ECNT <= ecnt_n;
                        CO0 <= co0_n; CO1 <= co1_n; LV0 <= lv0_n; LV1 <= lv1_n;
                    end else if (sio_pending && ovcs >= sio_half()) begin
                        ovcs <= ovcs - sio_half();
                        sio_edge();
                    end else begin
                        draining <= 1'b0;
                        sio_pending <= 1'b0;
                    end
                end else if (adv_left != 0) begin
                    adv_left <= adv_left - 7'd1;
                    if (TMM[4]) CNT0 <= 8'h0;
                    if (t0_run) ovc0 <= ovc0 + 8'd1;
                    if (TMM[7]) CNT1 <= 8'h0;
                    if (t1_run) ovc1 <= ovc1 + 8'd1;
                    if (TMM[1:0] == 2'b10) to_toggle(~TO);          // timer F/F from clock / 3
                    if (ETMM[3:2] == 2'b00) ECNT <= 16'h0;
                    else if (e_run) ovce <= ovce + 8'd1;
                    if (SMH[1:0] == 2'b01 || SMH[1:0] == 2'b10) begin
                        ovcs <= ovcs + 16'd1;
                        sio_pending <= 1'b1;
                    end
                    draining <= 1'b1;
                end else begin
                    // -------- A/D converter, once per batch (MAME handle_timers tail)
                    logic [15:0] cnt;
                    logic [7:0]  tot, tcr;
                    logic [1:0]  ai, ao;
                    logic [2:0]  ar;
                    logic        sh;
                    cnt = adcnt + {9'h0, adv_total};
                    tot = adtot; tcr = tmpcr; ai = adin; ao = adout; ar = adrange; sh = shdone;
                    if (PANM != ANM) begin
                        cnt = 16'h0;
                        tot = ANM[4] ? 8'd144 : 8'd192;
                        ao = 2'd0;
                        sh = 1'b0;
                        if (ANM[0]) begin adin_sel <= ANM[3:1]; ai = 2'd0; end
                        else begin ai = 2'd0; ar = {ANM[3], 2'b00}; end
                    end
                    PANM <= ANM;
                    if (ANM[0]) begin
                        logic [2:0] sel;
                        sel = (PANM != ANM) ? ANM[3:1] : adin_sel;
                        if (!sh) begin tcr = an[sel]; sh = 1'b1; end
                        if (cnt > {8'h0, tot}) begin
                            cnt = cnt - {8'h0, tot};
                            CR[ao] <= tcr;
                            ao = ao + 2'd1;
                            if (ao == 2'd0) irr_set = irr_set | INTFAD;
                            sh = 1'b0;
                        end
                    end else begin
                        if (!sh) begin tcr = an[{ar[2], ai}]; sh = 1'b1; end
                        if (cnt > {8'h0, tot}) begin
                            cnt = cnt - {8'h0, tot};
                            CR[ao] <= tcr;
                            ai = ai + 2'd1;
                            ao = ao + 2'd1;
                            if (ao == 2'd0) irr_set = irr_set | INTFAD;
                            sh = 1'b0;
                        end
                    end
                    adcnt <= cnt; adtot <= tot; tmpcr <= tcr; adin <= ai; adout <= ao;
                    adrange <= ar; shdone <= sh;
                    adv_busy <= 1'b0;
                    adv_ack <= 1'b1;
                end
            end

            IRR <= irr_n | irr_set;
            ITF <= itf_n;
        end
    end

endmodule
