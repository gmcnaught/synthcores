// DPCM and interpolation, 1/2 (pcm.cpp voice loop "dpcm", "interpolation"):
// the shifted sample deltas and interpolation steps; P_SLOT_DPCM2 sums them.
// Included in sc55_pcm.sv state P_SLOT_DPCM.
begin : b_sc55_pcm_dpcm_1
    int preshift, shift_, shifted, step_;
    logic [3:0] nn, sel;
    int samp_ [4];
    nn = v_newsel ? v_rom[0][7:4] : v_rom[0][3:0];
    for (int k = 0; k < 4; k++) samp_[k] = int'(s8(v_rom[k + 1]));
    for (int k = 0; k < 4; k++) begin
        preshift = samp_[k] << 10;
        sel = v_ncmp[k + 2] ? v_old_nibble : nn;
        shift_ = (10 - int'(sel)) & 15;
        shifted = (preshift << 1) >>> shift_;
        dp_sh[k] <= shifted;
    end
    for (int k = 0; k < 3; k++) begin
        step_ = multi({20'h0, interp_lut(2'(k), v_ratio)} << 6, v_rom[k + 1]) >>> 8;
        sel = v_ncmp[k + 2] ? v_old_nibble : nn;
        shift_ = (10 - int'(sel)) & 15;
        dp_st[k] <= (step_ << 1) >>> shift_;
    end
end
