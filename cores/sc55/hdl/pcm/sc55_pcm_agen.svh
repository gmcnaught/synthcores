// Per-slot address generator (pcm.cpp voice loop: "address generator" and
// the four address steps), as blocking code. Inputs: AGR1(j), AGR2(j) (the
// slot's ram1/ram2 words as they are at the start of the frame), AG_KEY (its
// voice_active bit), pitch[], nfs. Outputs: the ag_* locals. A slot's row is
// changed only by that slot, so this can run before the voice loop reaches it.
begin : b_sc55_pcm_agen_1
    logic b15_, b6_, b7_, ncmp1, b4, xor2, check1, xor1, n_add, n_sub, acmp, a_add, a_sub, nb15, use_;
    int address_, aend, aloop, cmp1_, nib_addr, wave_, sp, spo_, acnt, acnt2, nxt;
    logic [15:0] r8n, r7_;

    r7_ = `AGR2(7);
    ag_okey = r7_[5];
    ag_key = `AG_KEY;
    ag_active = ag_okey && ag_key;
    ag_kon = ag_key && !ag_okey;
    r8n = `AGR2(8);
    b15_ = r8n[15];
    b6_ = r7_[6];
    b7_ = r7_[7];
    ag_hi = r7_[11:8];
    ag_oldn = r7_[15:12];
    address_ = int'(`AGR1(4));
    aend = int'(`AGR1(0));
    aloop = int'(`AGR1(2));

    cmp1_ = b15_ ? aloop : aend;
    ncmp1 = (cmp1_ & 'hffff0) == (address_ & 'hffff0);
    if (ag_kon) ag_irq = ((cmp1_ + aloop) & 'h100000) != 0;
    else ag_irq = ((address_ + ((-aloop) & 'hfffff)) & 'h100000) != 0;
    ag_irq = ag_irq ^ b7_;

    nib_addr = (!b6_ && ncmp1) ? aloop : address_;
    b4 = (nib_addr & 'h10) != 0;
    wave_ = nib_addr >>> 5;
    xor2 = b4 ^ b7_;
    check1 = xor2 && ag_active;
    xor1 = b15_ ^ !ncmp1;
    n_add = b6_ ? (check1 && xor1) : (!ncmp1 && check1);
    n_sub = b6_ && !xor1 && ag_active && !xor2;
    if (b7_) wave_ = wave_ - (int'(n_add) - int'(n_sub));
    else wave_ = wave_ + (int'(n_add) - int'(n_sub));
    ag_wave = 20'(wave_ & 'hfffff);
    ag_newsel = b4 ^ ((b6_ || !ncmp1) && ag_okey);

    sp = int'({16'h0, `AGR2(8) & 16'h3fff});
    ag_ratio = 7'((sp >>> 7) & 127);
    sp = sp + int'(pitch[r7_[4:0]]);
    spo_ = (sp >>> 14) & 7;
    r8n = `AGR2(8);
    if (nfs) r8n = (r8n & ~16'h3fff) | 16'(sp & 'h3fff);

    // address 0 and the four steps
    acnt = address_;
    ag_addr[0] = 32'(acnt);
    ag_ncmp[2] = (address_ & 'hffff0) == (acnt & 'hffff0);
    cmp1_ = b15_ ? aloop : aend;
    acmp = (cmp1_ & 'hfffff) == (acnt & 'hfffff);
    nxt = acnt;
    nb15 = b15_;
    use_ = !ag_ncmp[2];
    for (int k = 1; k <= 4; k++) begin
        cmp1_ = (!b6_ && acmp) ? aloop : acnt;
        acnt2 = (ag_kon || (!b6_ && acmp)) ? cmp1_ : acnt;
        a_add = (!acmp && b6_ && !b15_) || (!acmp && !b6_);
        a_sub = !acmp && b6_ && b15_;
        if (b7_) acnt2 = acnt2 - (int'(a_add) - int'(a_sub));
        else acnt2 = acnt2 + (int'(a_add) - int'(a_sub));
        acnt = acnt2 & 'hfffff;
        if (k < 4) b15_ = b6_ && (b15_ ^ acmp);
        if (k < 4) ag_addr[k] = 32'(acnt);
        ag_ncmp[k + 2] = (address_ & 'hffff0) == (acnt & 'hffff0);
        if (k < 4) begin
            cmp1_ = b15_ ? aloop : aend;
            acmp = (cmp1_ & 'hfffff) == (acnt & 'hfffff);
        end
        if (spo_ >= k) begin
            nxt = acnt;
            use_ = !ag_ncmp[k + 2];
            if (k < 4) nb15 = b15_;
        end
    end
    ag_usenew = use_;
    ag_r14_we = ag_active && nfs;
    ag_r14 = 32'(nxt);
    if (nfs) r8n = (r8n & ~16'h8000) | {nb15, 15'h0};
    ag_r28 = r8n;
    ag_b7 = b7_;
    ag_spo = 3'(spo_);
end
