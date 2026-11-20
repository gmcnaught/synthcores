// DPCM reference and interpolation (pcm.cpp voice loop "dpcm", "interpolation").
// Included in sc55_pcm.sv state P_SLOT_DPCM.
begin
    int reference_, test_, preshift, shift_, shifted, step_;
    logic [3:0] nn, sel;
    int samp_ [4];
    nn = v_newsel ? v_rom[0][7:4] : v_rom[0][3:0];
    for (int k = 0; k < 4; k++) samp_[k] = int'(s8(v_rom[k + 1]));
    reference_ = int'(r1[5]);
    for (int k = 0; k < 4; k++) begin
        preshift = samp_[k] << 10;
        sel = v_ncmp[k + 2] ? v_old_nibble : nn;
        shift_ = (10 - int'(sel)) & 15;
        shifted = (preshift << 1) >>> shift_;
        if (int'(v_spo) >= k + 1) reference_ = addclip20(reference_, shifted >>> 1, shifted & 1);
    end
    test_ = int'(r1[5]);
    for (int k = 0; k < 3; k++) begin
        step_ = multi({20'h0, interp_lut(2'(k), v_ratio)} << 6, v_rom[k + 1]) >>> 8;
        sel = v_ncmp[k + 2] ? v_old_nibble : nn;
        shift_ = (10 - int'(sel)) & 15;
        step_ = (step_ << 1) >>> shift_;
        test_ = addclip20(test_, step_ >>> 1, step_ & 1);
    end
    v_reference <= reference_;
    v_test <= test_;
end
