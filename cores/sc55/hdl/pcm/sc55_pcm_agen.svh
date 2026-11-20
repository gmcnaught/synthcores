// Per-slot address generator (pcm.cpp voice loop, "address generator" and the
// four address steps). Included in sc55_pcm.sv state P_SLOT_AG; uses r1/r2 of
// the slot and writes the v_* registers, r1[4] and r2[8].
begin
    logic okey_, key_, active_, kon_, b15_, b6_, b7_, ncmp1, irq_f, b4, xor2, check1, xor1, n_add, n_sub;
    logic acmp, a_add, a_sub, nb15;
    logic [3:0] hi_, oldn;
    int address_, aend, aloop, cmp1_, nib_addr, wave_, sp, spo_, acnt, acnt2, nxt;
    logic ncmp_ [2:6];
    int addr_ [4];
    logic [15:0] r8n;

    okey_ = r2[7][5];
    key_ = voice_active[slot[4:0]];
    active_ = okey_ && key_;
    kon_ = key_ && !okey_;
    b15_ = r2[8][15];
    b6_ = r2[7][6];
    b7_ = r2[7][7];
    hi_ = r2[7][11:8];
    oldn = r2[7][15:12];
    address_ = int'(r1[4]);
    aend = int'(r1[0]);
    aloop = int'(r1[2]);

    cmp1_ = b15_ ? aloop : aend;
    ncmp1 = (cmp1_ & 'hffff0) == (address_ & 'hffff0);
    if (kon_) irq_f = ((cmp1_ + aloop) & 'h100000) != 0;
    else irq_f = ((address_ + ((-aloop) & 'hfffff)) & 'h100000) != 0;
    irq_f = irq_f ^ b7_;

    nib_addr = (!b6_ && ncmp1) ? aloop : address_;
    b4 = (nib_addr & 'h10) != 0;
    wave_ = nib_addr >>> 5;
    xor2 = b4 ^ b7_;
    check1 = xor2 && active_;
    xor1 = b15_ ^ !ncmp1;
    n_add = b6_ ? (check1 && xor1) : (!ncmp1 && check1);
    n_sub = b6_ && !xor1 && active_ && !xor2;
    if (b7_) wave_ = wave_ - (int'(n_add) - int'(n_sub));
    else wave_ = wave_ + (int'(n_add) - int'(n_sub));
    wave_ = wave_ & 'hfffff;
    v_newsel <= b4 ^ ((b6_ || !ncmp1) && okey_);

    sp = int'({16'h0, r2[8] & 16'h3fff});
    v_ratio <= 7'((sp >>> 7) & 127);
    sp = sp + int'(pitch[r2[7][4:0]]);
    spo_ = (sp >>> 14) & 7;
    r8n = r2[8];
    if (nfs) r8n = (r8n & ~16'h3fff) | 16'(sp & 'h3fff);

    // address 0
    acnt = address_;
    addr_[0] = acnt;
    ncmp_[2] = (address_ & 'hffff0) == (acnt & 'hffff0);
    cmp1_ = b15_ ? aloop : aend;
    acmp = (cmp1_ & 'hfffff) == (acnt & 'hfffff);
    nxt = acnt;
    v_usenew <= 1'b0;
    nb15 = b15_;
    begin : steps
        logic use_;
        use_ = !ncmp_[2];
        for (int k = 1; k <= 4; k++) begin
            cmp1_ = (!b6_ && acmp) ? aloop : acnt;
            acnt2 = (kon_ || (!b6_ && acmp)) ? cmp1_ : acnt;
            a_add = (!acmp && b6_ && !b15_) || (!acmp && !b6_);
            a_sub = !acmp && b6_ && b15_;
            if (b7_) acnt2 = acnt2 - (int'(a_add) - int'(a_sub));
            else acnt2 = acnt2 + (int'(a_add) - int'(a_sub));
            acnt = acnt2 & 'hfffff;
            if (k < 4) b15_ = b6_ && (b15_ ^ acmp);
            if (k < 4) addr_[k] = acnt;
            ncmp_[k + 2] = (address_ & 'hffff0) == (acnt & 'hffff0);
            if (k < 4) begin
                cmp1_ = b15_ ? aloop : aend;
                acmp = (cmp1_ & 'hfffff) == (acnt & 'hfffff);
            end
            if (spo_ >= k) begin
                nxt = acnt;
                use_ = !ncmp_[k + 2];
                if (k < 4) nb15 = b15_;
            end
        end
        v_usenew <= use_;
    end

    if (active_ && nfs) r1[4] <= 32'(nxt);
    if (nfs) r8n = (r8n & ~16'h8000) | {nb15, 15'h0};
    r2[8] <= r8n;

    v_active <= active_;
    v_kon <= kon_;
    v_key <= key_;
    v_okey <= okey_;
    v_b7 <= b7_;
    v_irq_flag <= irq_f;
    v_old_nibble <= oldn;
    v_hiaddr <= hi_;
    v_wave_address <= 20'(wave_);
    for (int k = 0; k < 4; k++) v_addr[k] <= 32'(addr_[k]);
    for (int k = 2; k <= 6; k++) v_ncmp[k] <= ncmp_[k];
    v_spo <= 3'(spo_);
end
