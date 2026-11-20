// Reverb/chorus section of a frame (pcm.cpp "chorus/reverb", steps 1..23, 31),
// as a sequence of micro-steps with at most one eram access each (a read
// issued in step n is in er_q in step n+1). Included in sc55_pcm.sv state
// P_REVERB. G1(r,i) = g1[r-28][i], G2(r,i) = g2[r-28][i]; tv = tv_counter.
`define G1(r, i) g1[(r) - 28][i]
`define G2(r, i) g2[(r) - 28][i]
`define EA(r, i) 14'(`G2(r, i) + tv_counter)
`define EA1(r, i) 14'(`G2(r, i) + tv_counter + 16'd1)
`define ER(addr) begin er_ra <= (addr); er_wait <= 2'd2; end
`define EW(addr, val) begin er_we <= 1'b1; er_wa <= (addr); er_wd <= eram_pack_v(val); end
`define UNP(t) eram_unpack_v(er_q, t)
begin : b_sc55_pcm_reverb_1
    logic signed [31:0] m1_, m2_, s1_, s2_, v2_, v3_, t1_, t2_;
    logic [15:0] w1;
    logic [31:0] ct_;
    // Steps with multiply -> add -> multiply -> add run in two halves
    // (rv_half): the first registers the middle value, the second finishes
    // and does the step's eram access.
    rv_pc <= rv_pc + 7'd1;
    case (rv_pc)
        7'd0: begin
            // ram1[29][1] from ram2[31][1] and rcsum[1]
            w1 = `G2(31, 1);
            m1_ = multi(`G1(29, 1), w1[15:8]) >>> 5;
            m2_ = multi(rcsum[1], w1[7:0]) >>> 5;
            `G1(29, 1) <= addclip20(m1_ >>> 1, m2_ >>> 1, (m1_ | m2_) & 1);
            // calc_tv(1, ram2[30][0], &ram2[30][9], okey of slot 31)
            ct_ = calc_tv(1, `G2(30, 0), `G2(30, 9), `G2(31, 7)[5], tv_counter, nfs);
            `G2(30, 9) <= ct_[31:16];
            // ram1[29][0] from ram2[30][1] and rcsum[0]
            w1 = `G2(30, 1);
            m1_ = multi(`G1(29, 0), w1[15:8]) >>> 5;
            m2_ = multi(rcsum[0], w1[7:0]) >>> 5;
            `G1(29, 0) <= addclip20(m1_ >>> 1, m2_ >>> 1, (m1_ | m2_) & 1);
            `ER(`EA(28, 1))
        end
        7'd1: begin  // step 1 (uses the new ram1[29][0])
            w1 = `G2(30, 4);
            s1_ = `UNP(1); s2_ = `UNP(0);
            if (!rv_half) begin
                m1_ = multi(`G1(29, 0), w1[15:8]) >>> 6;
                v2_ = (w1 & 16'h30) != 0 ? s1_ : 32'sd0;
                `G1(29, 4) <= addclip20(m1_, v2_ ^ 32'hfffff, 1);
                rv_half <= 1'b1; rv_pc <= rv_pc;
            end else begin
                m2_ = multi(`G1(29, 4), w1[7:0]) >>> 5;
                `G1(29, 5) <= addclip20(m2_ >>> 1, s2_, m2_ & 1);
                rv_half <= 1'b0;
                `ER(`EA(28, 2))
            end
        end
        7'd2: begin
            w1 = `G2(30, 4);
            s1_ = `UNP(1); s2_ = `UNP(0);
            if (!rv_half) begin
                v2_ = (w1 & 16'h30) != 0 ? s1_ : 32'sd0;
                `G1(29, 5) <= addclip20(`G1(29, 5), v2_ ^ 32'hfffff, 1);
                rv_half <= 1'b1; rv_pc <= rv_pc;
            end else begin
                m2_ = multi(`G1(29, 5), w1[7:0]) >>> 5;
                `G1(28, 0) <= addclip20(m2_ >>> 1, s2_, m2_ & 1);
                rv_half <= 1'b0;
                `ER(`EA(28, 3))
            end
        end
        7'd3: begin
            w1 = `G2(30, 4);
            s1_ = `UNP(1); s2_ = `UNP(0);
            if (!rv_half) begin
                v2_ = (w1 & 16'h30) != 0 ? s1_ : 32'sd0;
                `G1(28, 0) <= addclip20(`G1(28, 0), v2_ ^ 32'hfffff, 1);
                rv_half <= 1'b1; rv_pc <= rv_pc;
            end else begin
                m2_ = multi(`G1(28, 0), w1[7:0]) >>> 5;
                `G1(28, 1) <= addclip20(m2_ >>> 1, s2_, m2_ & 1);
                rv_half <= 1'b0;
                `ER(`EA(28, 5))
            end
        end
        7'd4: begin
            `G1(28, 2) <= `UNP(0);
            `ER(`EA(28, 4))
        end
        7'd5: begin
            w1 = `G2(30, 5);
            s1_ = `UNP(1); s2_ = `UNP(0);
            if (!rv_half) begin
                v2_ = (w1 & 16'h30) != 0 ? s1_ : 32'sd0;
                `G1(28, 1) <= addclip20(`G1(28, 1), v2_ ^ 32'hfffff, 1);
                rv_half <= 1'b1; rv_pc <= rv_pc;
            end else begin
                m2_ = multi(`G1(28, 1), w1[7:0]) >>> 5;
                `G1(28, 3) <= addclip20(m2_ >>> 1, s2_, m2_ & 1);
                rv_half <= 1'b0;
                `ER(`EA(29, 1))
            end
        end
        7'd6: begin
            `G1(28, 4) <= `UNP(0);
            `ER(`EA(29, 0))
        end
        7'd7: begin  // step 5
            w1 = `G2(30, 7);
            m1_ = multi(`G1(29, 2), w1[15:8]) >>> 5;
            s1_ = `UNP(0);
            m2_ = multi(s1_, w1[7:0]) >>> 5;
            `G1(29, 2) <= addclip20(m1_ >>> 1, m2_ >>> 1, (m1_ | m2_) & 1);
            `EW(`EA(28, 0), `G1(29, 4))
        end
        7'd8: `ER(`EA(29, 8))
        7'd9: begin  // step 6
            w1 = `G2(30, 8);
            m1_ = multi(`G1(29, 3), w1[15:8]) >>> 5;
            s1_ = `UNP(0);
            m2_ = multi(s1_, w1[7:0]) >>> 5;
            `G1(29, 3) <= addclip20(m1_ >>> 1, m2_ >>> 1, (m1_ | m2_) & 1);
            `EW(`EA(28, 1), `G1(29, 5))
        end
        7'd10: `EW(`EA(28, 2), `G1(28, 0))
        7'd11: begin  // step 7
            w1 = `G2(30, 9);
            v2_ = `G1(28, 3);
            m1_ = multi(`G1(29, 2), w1[15:8]) >>> 5;
            m2_ = multi(`G1(29, 3), w1[15:8]) >>> 5;
            `G1(28, 3) <= addclip20(v2_, m1_ >>> 1, m1_ & 1);
            `G1(28, 5) <= addclip20(v2_, m2_ >>> 1, m2_ & 1);
            `EW(`EA(28, 3), `G1(28, 1))
        end
        7'd12: begin  // step 8
            w1 = `G2(30, 6);
            if (!rv_half) begin
                m1_ = multi(`G1(28, 2), w1[15:8]) >>> 5;
                `G1(28, 3) <= addclip20(`G1(28, 3), m1_ >>> 1, m1_ & 1);
                rv_half <= 1'b1; rv_pc <= rv_pc;
            end else begin
                m2_ = multi(`G1(28, 3), w1[7:0]) >>> 5;
                `G1(28, 2) <= addclip20(`G1(28, 2), m2_ >>> 1, m2_ & 1);
                rv_half <= 1'b0;
                `ER(`EA(28, 9))
            end
        end
        7'd13: begin  // step 9
            w1 = `G2(30, 6);
            if (!rv_half) begin
                `G1(28, 1) <= `UNP(0);
                m1_ = multi(`G1(28, 4), w1[15:8]) >>> 5;
                `G1(28, 5) <= addclip20(`G1(28, 5), m1_ >>> 1, m1_ & 1);
                rv_half <= 1'b1; rv_pc <= rv_pc;
            end else begin
                m2_ = multi(`G1(28, 5), w1[7:0]) >>> 5;
                `G1(28, 4) <= addclip20(`G1(28, 4), m2_ >>> 1, m2_ & 1);
                rv_half <= 1'b0;
                `ER(`EA(29, 5))
            end
        end
        7'd14: begin
            `G1(29, 4) <= `UNP(0);
            `ER(`EA(28, 8))
        end
        7'd15: begin  // step 10
            w1 = `G2(30, 6);
            if (!rv_half) begin
                rv_t <= `G1(28, 1);
                m1_ = multi(`G1(28, 1), w1[15:8]) >>> 5;
                s1_ = `UNP(0);
                `G1(28, 1) <= addclip20(m1_ >>> 1, s1_, m1_ & 1);
                rv_half <= 1'b1; rv_pc <= rv_pc;
            end else begin
                m2_ = multi(`G1(28, 1), w1[7:0]) >>> 5;
                `G1(29, 5) <= addclip20(m2_ >>> 1, rv_t, m2_ & 1);
                rv_half <= 1'b0;
                `EW(`EA(28, 4), `G1(28, 3))
            end
        end
        7'd16: `ER(`EA(29, 4))
        7'd17: begin  // step 11
            w1 = `G2(30, 6);
            if (!rv_half) begin
                rv_t <= `G1(29, 4);
                m1_ = multi(`G1(29, 4), w1[15:8]) >>> 5;
                s1_ = `UNP(0);
                `G1(29, 4) <= addclip20(m1_ >>> 1, s1_, m1_ & 1);
                rv_half <= 1'b1; rv_pc <= rv_pc;
            end else begin
                m2_ = multi(`G1(29, 4), w1[7:0]) >>> 5;
                `G1(28, 0) <= addclip20(m2_ >>> 1, rv_t, m2_ & 1);
                rv_half <= 1'b0;
                `EW(`EA(28, 5), `G1(28, 2))
            end
        end
        7'd18: `EW(`EA(29, 0), `G1(28, 5))
        7'd19: `ER(`EA(28, 6))                                    // step 12
        7'd20: begin `G1(28, 5) <= `UNP(0); `ER(`EA(28, 10)) end
        7'd21: begin                                                // step 13
            `G1(28, 5) <= addclip20(`G1(28, 5), `UNP(0), 0);
            `ER(`EA(29, 2))
        end
        7'd22: begin `G1(28, 2) <= `UNP(0); `ER(`EA(29, 6)) end
        7'd23: begin                                                // step 14
            t1_ = addclip20(`UNP(0), `G1(28, 2), 0);
            `G1(28, 5) <= addclip20(t1_, `G1(28, 5), 0);
            `ER(`EA(28, 7))
        end
        7'd24: begin `G1(28, 2) <= `UNP(0); `ER(`EA(28, 11)) end
        7'd25: begin                                                // step 15
            `G1(28, 2) <= addclip20(`G1(28, 2), `UNP(0), 0);
            `ER(`EA(29, 3))
        end
        7'd26: begin `G1(28, 3) <= `UNP(0); `ER(`EA(29, 7)) end
        7'd27: begin                                                // step 16
            t1_ = addclip20(`UNP(0), `G1(28, 2), 0);
            `G1(28, 2) <= addclip20(t1_, `G1(28, 3), 0);
            `EW(`EA(29, 1), `G1(28, 4))
        end
        7'd28: `EW(`EA(28, 8), `G1(28, 1))
        7'd29: begin                                                // step 17
            w1 = `G2(30, 2);
            v2_ = `G1(28, 5);
            rcadd[0] <= multi(v2_, w1[15:8]) >>> 5;
            rcadd2[0] <= multi(v2_, w1[7:0]) >>> 5;
            `ER(`EA1(29, 10))
        end
        7'd30: begin
            `EW(`EA(28, 9), `G1(29, 5))
            `G1(29, 5) <= `UNP(0);
        end
        7'd31: begin                                                // step 18
            w1 = `G2(30, 3);
            v2_ = `G1(28, 2);
            rcadd[1] <= multi(v2_, w1[15:8]) >>> 5;
            rcadd2[1] <= multi(v2_, w1[7:0]) >>> 5;
            `ER(`EA1(29, 11))
        end
        7'd32: begin `G1(28, 1) <= `UNP(0); `ER(`EA(29, 10)) end
        7'd33: begin                                                // step 19
            w1 = `G2(31, 9);
            s1_ = `UNP(0);
            `EW(`EA(29, 4), `G1(29, 4))
            m1_ = multi(s1_, w1[15:8]) >>> 5;
            m2_ = multi(`G1(29, 5), w1[15:8]) >>> 5;
            t2_ = addclip20(s1_, (m1_ >>> 1) ^ 32'hfffff, 1);
            `G1(29, 5) <= addclip20(t2_, m2_ >>> 1, m2_ & 1);
        end
        7'd34: `ER(`EA(29, 11))
        7'd35: begin                                                // step 20
            w1 = `G2(31, 10);
            s1_ = `UNP(0);
            `EW(`EA(29, 5), `G1(28, 0))
            m1_ = multi(s1_, w1[15:8]) >>> 5;
            m2_ = multi(`G1(28, 1), w1[15:8]) >>> 5;
            t2_ = addclip20(s1_, (m1_ >>> 1) ^ 32'hfffff, 1);
            `G1(28, 1) <= addclip20(t2_, m2_ >>> 1, m2_ & 1);
        end
        7'd36: `EW(`EA(29, 9), `G1(29, 1))
        default: begin                                              // steps 21, 22, 23, 31
            w1 = `G2(31, 2);
            rcadd[2] <= multi(`G1(29, 5), w1[15:8]) >>> 5;
            rcadd2[2] <= multi(`G1(29, 5), w1[7:0]) >>> 5;
            w1 = `G2(31, 3);
            rcadd[3] <= multi(`G1(29, 5), w1[15:8]) >>> 5;
            rcadd2[3] <= multi(`G1(29, 5), w1[7:0]) >>> 5;
            w1 = `G2(31, 4);
            rcadd[4] <= multi(`G1(28, 1), w1[15:8]) >>> 5;
            rcadd2[4] <= multi(`G1(28, 1), w1[7:0]) >>> 5;
            w1 = `G2(31, 5);
            rcadd[5] <= multi(`G1(28, 1), w1[15:8]) >>> 5;
            rcadd2[5] <= multi(`G1(28, 1), w1[7:0]) >>> 5;
            ps <= P_AGEN;
        end
    endcase
end
`undef G1
`undef G2
`undef EA
`undef EA1
`undef ER
`undef EW
`undef UNP
