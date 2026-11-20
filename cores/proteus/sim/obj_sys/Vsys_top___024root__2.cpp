// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vsys_top.h for the primary calling header

#include "Vsys_top__pch.h"

void Vsys_top___024root___nba_sequent__TOP__6(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___nba_sequent__TOP__6\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT1) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__xToIrc = 0U;
    } else if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT3) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__xToIrc 
            = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
            .__PVT__toIrc;
    }
}

void Vsys_top___024root___nba_comb__TOP__0(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___nba_comb__TOP__0\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__ms0 
        = ((2U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                  >> 7U)) | (1U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__alue)));
}

void Vsys_top___024root___nba_sequent__TOP__7(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___nba_sequent__TOP__7\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    VlUnpacked<CData/*7:0*/, 32> sys_top__DOT__sctl__DOT__snd__DOT__v_disp;
    for (int __Vi0 = 0; __Vi0 < 32; ++__Vi0) {
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[__Vi0] = 0;
    }
    SData/*15:0*/ __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__clamp16__16__Vfuncout;
    __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__clamp16__16__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__clamp16__16__x;
    __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__clamp16__16__x = 0;
    SData/*15:0*/ __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__clamp16__17__Vfuncout;
    __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__clamp16__17__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__clamp16__17__x;
    __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__clamp16__17__x = 0;
    IData/*16:0*/ __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__level_to_gain__18__Vfuncout;
    __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__level_to_gain__18__Vfuncout = 0;
    SData/*11:0*/ __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__level_to_gain__18__level;
    __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__level_to_gain__18__level = 0;
    SData/*11:0*/ __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__env_next__19__Vfuncout;
    __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__env_next__19__Vfuncout = 0;
    SData/*11:0*/ __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__env_next__19__level;
    __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__env_next__19__level = 0;
    IData/*23:0*/ __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__env_next__19__env;
    __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__env_next__19__env = 0;
    CData/*0:0*/ __Vdly__sys_top__DOT__sys__DOT__mfp_iack;
    __Vdly__sys_top__DOT__sys__DOT__mfp_iack = 0;
    CData/*0:0*/ __Vdly__sys_top__DOT__sys__DOT__lcd_wr;
    __Vdly__sys_top__DOT__sys__DOT__lcd_wr = 0;
    CData/*0:0*/ __Vdly__sys_top__DOT__snd_rb;
    __Vdly__sys_top__DOT__snd_rb = 0;
    CData/*0:0*/ __Vdly__sys_top__DOT__sctl__DOT__snd_wr;
    __Vdly__sys_top__DOT__sctl__DOT__snd_wr = 0;
    CData/*0:0*/ __Vdly__sys_top__DOT__sctl__DOT__pan_wr;
    __Vdly__sys_top__DOT__sctl__DOT__pan_wr = 0;
    CData/*0:0*/ __Vdly__sys_top__DOT__sctl__DOT__snd_start;
    __Vdly__sys_top__DOT__sctl__DOT__snd_start = 0;
    CData/*0:0*/ __Vdly__sys_top__DOT__sctl__DOT__rb_start;
    __Vdly__sys_top__DOT__sctl__DOT__rb_start = 0;
    CData/*7:0*/ __VdlyVal__sys_top__DOT__sys__DOT__ram__v1;
    __VdlyVal__sys_top__DOT__sys__DOT__ram__v1 = 0;
    SData/*14:0*/ __VdlyDim0__sys_top__DOT__sys__DOT__ram__v1;
    __VdlyDim0__sys_top__DOT__sys__DOT__ram__v1 = 0;
    CData/*7:0*/ __VdlyVal__sys_top__DOT__sys__DOT__ram__v2;
    __VdlyVal__sys_top__DOT__sys__DOT__ram__v2 = 0;
    SData/*14:0*/ __VdlyDim0__sys_top__DOT__sys__DOT__ram__v2;
    __VdlyDim0__sys_top__DOT__sys__DOT__ram__v2 = 0;
    SData/*11:0*/ __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_level__v32;
    __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_level__v32 = 0;
    CData/*4:0*/ __VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_level__v32;
    __VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_level__v32 = 0;
    IData/*23:0*/ __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v32;
    __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v32 = 0;
    CData/*4:0*/ __VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v32;
    __VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v32 = 0;
    CData/*4:0*/ __VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_frac__v32;
    __VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_frac__v32 = 0;
    SData/*11:0*/ __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_frac__v33;
    __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_frac__v33 = 0;
    CData/*4:0*/ __VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_frac__v33;
    __VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_frac__v33 = 0;
    IData/*23:0*/ __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v33;
    __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v33 = 0;
    CData/*4:0*/ __VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v33;
    __VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v33 = 0;
    SData/*11:0*/ __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_level__v33;
    __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_level__v33 = 0;
    CData/*4:0*/ __VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_level__v33;
    __VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_level__v33 = 0;
    CData/*7:0*/ __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_disp__v32;
    __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_disp__v32 = 0;
    CData/*4:0*/ __VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_disp__v32;
    __VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_disp__v32 = 0;
    SData/*11:0*/ __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_frac__v34;
    __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_frac__v34 = 0;
    CData/*4:0*/ __VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_frac__v34;
    __VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_frac__v34 = 0;
    IData/*23:0*/ __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v34;
    __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v34 = 0;
    CData/*4:0*/ __VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v34;
    __VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v34 = 0;
    // Body
    vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__mfp_cs_wr = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__mfp_cs_rd = 0U;
    __Vdly__sys_top__DOT__sys__DOT__mfp_iack = 0U;
    __Vdly__sys_top__DOT__sys__DOT__lcd_wr = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__lcd_rd = 0U;
    vlSelfRef.sys_top__DOT__snd_wr = 0U;
    __Vdly__sys_top__DOT__snd_rb = 0U;
    vlSelfRef.bus_wr_strobe = 0U;
    if ((0U != vlSelfRef.sys_top__DOT__sys__DOT__vol_cnt)) {
        vlSelfRef.sys_top__DOT__sys__DOT__vol_cnt = 
            (0x0003ffffU & (vlSelfRef.sys_top__DOT__sys__DOT__vol_cnt 
                            - (IData)(1U)));
    }
    if (vlSelfRef.reset) {
        vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__bus_busy = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__DTACKn = 1U;
        vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__bstate = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__vol_cnt = 0U;
    } else if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rAS) {
        vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__bus_busy = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__DTACKn = 1U;
        vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__bstate = 0U;
    } else if ((1U & ((~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__bus_busy)) 
                      & ((~ ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rLDS) 
                             & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rUDS))) 
                         | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__rFC)))))) {
        vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__bus_busy = 1U;
        if ((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__rFC))) {
            __Vdly__sys_top__DOT__sys__DOT__mfp_iack = 1U;
            vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__bstate = 5U;
        } else if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rRWn) {
            if (vlSelfRef.sys_top__DOT__sys__DOT__sel_mfp) {
                vlSelfRef.sys_top__DOT__sys__DOT__mfp_cs_rd = 1U;
            }
            if ((0x001c0000U == (0x003fffffU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                                                >> 2U)))) {
                vlSelfRef.sys_top__DOT__sys__DOT__lcd_rd = 1U;
            }
            if ((0x00500000U == (0x007fffffU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                                                >> 1U)))) {
                vlSelfRef.sys_top__DOT__sys__DOT__vol_cnt 
                    = vlSelfRef.sys_top__DOT__sys__DOT__vol_load;
            }
            if ((IData)((0x00800018U == (0x00fffc1eU 
                                         & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)))) {
                vlSelfRef.__Vdly__sys_top__DOT__snd_voice 
                    = (0x0000001fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                                      >> 5U));
                vlSelfRef.sys_top__DOT__snd_word = 
                    (0x0000000fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                                    >> 1U));
                __Vdly__sys_top__DOT__snd_rb = 1U;
                vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__bstate = 4U;
            } else {
                vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__bstate = 1U;
            }
            vlSelfRef.__Vdly__sys_top__DOT__snd_voice 
                = (0x0000001fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                                  >> 5U));
            vlSelfRef.sys_top__DOT__snd_word = (0x0000000fU 
                                                & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                                                   >> 1U));
        } else {
            vlSelfRef.bus_wr_strobe = 1U;
            vlSelfRef.bus_wr_addr = (0x00fffffeU & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob);
            vlSelfRef.bus_wr_data = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__dob;
            vlSelfRef.bus_wr_ds = ((2U & ((~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rUDS)) 
                                          << 1U)) | 
                                   (1U & (~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rLDS))));
            if ((0xffU == (0x000000ffU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                                          >> 0x00000010U)))) {
                if ((1U & (~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rUDS)))) {
                    __VdlyVal__sys_top__DOT__sys__DOT__ram__v1 
                        = (0x000000ffU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__dob) 
                                          >> 8U));
                    __VdlyDim0__sys_top__DOT__sys__DOT__ram__v1 
                        = (0x00007fffU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                                          >> 1U));
                    vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__ram__v1 = 1U;
                }
                if ((1U & (~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rLDS)))) {
                    __VdlyVal__sys_top__DOT__sys__DOT__ram__v2 
                        = (0x000000ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__dob));
                    __VdlyDim0__sys_top__DOT__sys__DOT__ram__v2 
                        = (0x00007fffU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                                          >> 1U));
                    vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__ram__v2 = 1U;
                }
            }
            if (vlSelfRef.sys_top__DOT__sys__DOT__sel_mfp) {
                vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__mfp_cs_wr = 1U;
            }
            if ((0x001c0000U == (0x003fffffU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                                                >> 2U)))) {
                __Vdly__sys_top__DOT__sys__DOT__lcd_wr = 1U;
            }
            vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__bstate 
                = (((0x2000U == (0x00003fffU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                                                >> 0x0000000aU))) 
                    | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__sel_pan))
                    ? 3U : 2U);
        }
    } else if ((4U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__bstate))) {
        if ((1U & (~ ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__bstate) 
                      >> 1U)))) {
            if ((1U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__bstate))) {
                vlSelfRef.sys_top__DOT__sys__DOT__iEdb 
                    = vlSelfRef.sys_top__DOT__sys__DOT__mfp_vector;
                vlSelfRef.sys_top__DOT__sys__DOT__DTACKn = 0U;
                vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__bstate = 0U;
            } else if (vlSelfRef.sys_top__DOT__snd_rb_done) {
                vlSelfRef.sys_top__DOT__sys__DOT__iEdb 
                    = vlSelfRef.sys_top__DOT__snd_rb_data;
                vlSelfRef.sys_top__DOT__sys__DOT__DTACKn = 0U;
                vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__bstate = 0U;
            }
        }
    } else if ((2U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__bstate))) {
        if ((1U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__bstate))) {
            if ((1U & (~ (IData)(vlSelfRef.sys_top__DOT__snd_wr_full)))) {
                vlSelfRef.sys_top__DOT__snd_wr = 1U;
                if (vlSelfRef.sys_top__DOT__sys__DOT__sel_pan) {
                    vlSelfRef.sys_top__DOT__snd_pan = 1U;
                    vlSelfRef.__Vdly__sys_top__DOT__snd_voice 
                        = (0x0000001fU & ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                                           >> 1U) - (IData)(4U)));
                } else {
                    vlSelfRef.sys_top__DOT__snd_pan = 0U;
                    vlSelfRef.__Vdly__sys_top__DOT__snd_voice 
                        = (0x0000001fU & (0x0000001fU 
                                          & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                                             >> 5U)));
                }
                vlSelfRef.sys_top__DOT__snd_word = 
                    (0x0000000fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                                    >> 1U));
                vlSelfRef.sys_top__DOT__snd_data = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__dob;
                vlSelfRef.sys_top__DOT__snd_mask = 
                    ((0x0000ff00U & ((- (IData)((1U 
                                                 & (~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rUDS))))) 
                                     << 8U)) | (0x000000ffU 
                                                & (- (IData)(
                                                             (1U 
                                                              & (~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rLDS)))))));
                vlSelfRef.sys_top__DOT__sys__DOT__DTACKn = 0U;
                vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__bstate = 0U;
            }
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__DTACKn = 0U;
            vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__bstate = 0U;
        }
    } else if ((1U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__bstate))) {
        vlSelfRef.sys_top__DOT__sys__DOT__iEdb = ((0U 
                                                   == 
                                                   (0x0000007fU 
                                                    & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                                                       >> 0x00000011U)))
                                                   ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__rom_q)
                                                   : 
                                                  ((0xffU 
                                                    == 
                                                    (0x000000ffU 
                                                     & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                                                        >> 0x00000010U)))
                                                    ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__ram_q)
                                                    : 
                                                   ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__sel_mfp)
                                                     ? 
                                                    (0x0000ff00U 
                                                     | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp_dout))
                                                     : 
                                                    ((0x001c0000U 
                                                      == 
                                                      (0x003fffffU 
                                                       & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                                                          >> 2U)))
                                                      ? 
                                                     (0x0000ff00U 
                                                      | ((2U 
                                                          & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                          ? 
                                                         ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__lcd__DOT__active_cg)
                                                           ? vlSelfRef.sys_top__DOT__sys__DOT__lcd__DOT__cgram
                                                          [
                                                          (0x0000003fU 
                                                           & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__lcd__DOT__ac))]
                                                           : vlSelfRef.sys_top__DOT__sys__DOT__lcd__DOT__ddram
                                                          [
                                                          (0x0000007fU 
                                                           & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__lcd__DOT__ac))])
                                                          : 
                                                         (0x0000007fU 
                                                          & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__lcd__DOT__ac))))
                                                      : 
                                                     ((0x2000U 
                                                       == 
                                                       (0x00003fffU 
                                                        & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                                                           >> 0x0000000aU)))
                                                       ? (IData)(vlSelfRef.sys_top__DOT__snd_raw)
                                                       : 
                                                      ((0x00500000U 
                                                        == 
                                                        (0x007fffffU 
                                                         & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                                                            >> 1U)))
                                                        ? 0xffffU
                                                        : 
                                                       ((0x00600000U 
                                                         == 
                                                         (0x007fffffU 
                                                          & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                                                             >> 1U)))
                                                         ? 0U
                                                         : 
                                                        ((0x00340000U 
                                                          == 
                                                          (0x003fffffU 
                                                           & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                                                              >> 2U)))
                                                          ? 
                                                         ((2U 
                                                           & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                           ? 0U
                                                           : 2U)
                                                          : 0U))))))));
        vlSelfRef.sys_top__DOT__sys__DOT__DTACKn = 0U;
        vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__bstate = 0U;
    }
    vlSelfRef.sys_top__DOT__sys__DOT__bus_busy = vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__bus_busy;
    vlSelfRef.sys_top__DOT__sys__DOT__bstate = vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__bstate;
    vlSelfRef.sys_top__DOT__sys__DOT__lcd__DOT__ac 
        = vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__lcd__DOT__ac;
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__lcd__DOT__cgram__v0) {
        vlSelfRef.sys_top__DOT__sys__DOT__lcd__DOT__cgram[vlSelfRef.__VdlyDim0__sys_top__DOT__sys__DOT__lcd__DOT__cgram__v0] 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__lcd__DOT__cgram__v0;
    }
    vlSelfRef.__VdlyCommitQueuesys_top__DOT__sys__DOT__lcd__DOT__ddram.commit(vlSelfRef.sys_top__DOT__sys__DOT__lcd__DOT__ddram);
    vlSelfRef.lcd_code = vlSelfRef.sys_top__DOT__sys__DOT__lcd__DOT__ddram
        [(0x0000007fU & (((0x50U <= (IData)(vlSelfRef.sys_top__DOT__sys__DOT__lcd__DOT__sp))
                           ? ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__lcd__DOT__sp) 
                              - (IData)(0x50U)) : (
                                                   (0x28U 
                                                    <= (IData)(vlSelfRef.sys_top__DOT__sys__DOT__lcd__DOT__sp))
                                                    ? 
                                                   ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__lcd__DOT__sp) 
                                                    - (IData)(0x28U))
                                                    : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__lcd__DOT__sp))) 
                         + (0x40U & (- (IData)(((IData)(vlSelfRef.lcd_line) 
                                                & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__lcd__DOT__two_line)))))))];
    vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__t_active 
        = ((((0U != (7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tcdcr))) 
             << 3U) | ((0U != (7U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tcdcr) 
                                     >> 4U))) << 2U)) 
           | (((((1U <= (0x0000000fU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tbcr))) 
                 & (7U >= (0x0000000fU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tbcr)))) 
                | (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tbcr) 
                    >> 3U) & ((0U != (7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tbcr))) 
                              & ((0U != vlSelfRef.sys_top__DOT__sys__DOT__vol_cnt) 
                                 == (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__aer) 
                                           >> 3U)))))) 
               << 1U) | (1U & (((1U <= (0x0000000fU 
                                        & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tacr))) 
                                & (7U >= (0x0000000fU 
                                          & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tacr)))) 
                               | ((~ (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__aer) 
                                       >> 4U) | (0U 
                                                 == 
                                                 (7U 
                                                  & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tacr))))) 
                                  & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tacr) 
                                     >> 3U))))));
    vlSelfRef.sys_top__DOT__sys__DOT__vol_load = 0x0000e840U;
    __Vdly__sys_top__DOT__sctl__DOT__snd_wr = 0U;
    __Vdly__sys_top__DOT__sctl__DOT__pan_wr = 0U;
    __Vdly__sys_top__DOT__sctl__DOT__snd_start = 0U;
    __Vdly__sys_top__DOT__sctl__DOT__rb_start = 0U;
    vlSelfRef.sys_top__DOT__snd_rb_done = 0U;
    vlSelfRef.sample_strobe = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__rom_q = vlSelfRef.sys_top__DOT__sys__DOT__rom
        [(0x0000ffffU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                         >> 1U))];
    vlSelfRef.sys_top__DOT__sys__DOT__ram_q = vlSelfRef.sys_top__DOT__sys__DOT__ram
        [(0x00007fffU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                         >> 1U))];
    if (vlSelfRef.reset) {
        vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__q_state = 0U;
        vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__div = 0U;
        vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__tick_pending = 0U;
        vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__rb_pending = 0U;
        vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__f_rp = 0U;
        vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__cap_half = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__cap_pend = 0U;
        vlSelfRef.out_l = 0U;
        vlSelfRef.out_r = 0U;
    } else {
        if (vlSelfRef.sys_top__DOT__snd_rb) {
            vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__rb_pending = 1U;
        }
        if ((0x07ffU == (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__div))) {
            vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__div = 0U;
            vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__tick_pending = 1U;
        } else {
            vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__div 
                = (0x00000fffU & ((IData)(1U) + (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__div)));
        }
        if (((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__wr_done) 
             & (1U == (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__m_client)))) {
            vlSelfRef.sys_top__DOT__sctl__DOT__cap_pend = 0U;
        }
        if ((4U & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__q_state))) {
            if ((2U & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__q_state))) {
                vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__q_state = 0U;
            } else if ((1U & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__q_state))) {
                vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__q_state = 0U;
            } else if (vlSelfRef.sys_top__DOT__sctl__DOT__rb_done) {
                vlSelfRef.sys_top__DOT__snd_rb_done = 1U;
                vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__q_state = 0U;
            }
        } else if ((2U & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__q_state))) {
            if ((1U & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__q_state))) {
                if (vlSelfRef.sys_top__DOT__sctl__DOT__snd_done) {
                    vlSelfRef.out_l = vlSelfRef.sys_top__DOT__sctl__DOT__snd_l;
                    vlSelfRef.out_r = vlSelfRef.sys_top__DOT__sctl__DOT__snd_r;
                    vlSelfRef.sample_strobe = 1U;
                    if (vlSelfRef.sys_top__DOT__sctl__DOT__cap_half) {
                        vlSelfRef.sys_top__DOT__sctl__DOT__cap_word 
                            = (((QData)((IData)((((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd_r) 
                                                  << 0x00000010U) 
                                                 | (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd_l)))) 
                                << 0x00000020U) | (QData)((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__cap_lo)));
                        vlSelfRef.sys_top__DOT__sctl__DOT__cap_pend = 1U;
                        vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__cap_half = 0U;
                    } else {
                        vlSelfRef.sys_top__DOT__sctl__DOT__cap_lo 
                            = (((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd_r) 
                                << 0x00000010U) | (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd_l));
                        vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__cap_half = 1U;
                    }
                    vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__q_state = 0U;
                }
            } else if ((1U & (((~ (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd_busy)) 
                               & (~ (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd_wr))) 
                              & (~ (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__pan_wr))))) {
                __Vdly__sys_top__DOT__sctl__DOT__snd_start = 1U;
                vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__q_state = 3U;
            }
        } else if ((1U & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__q_state))) {
            if (vlSelfRef.sys_top__DOT__sctl__DOT__f_empty) {
                vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__q_state = 2U;
            } else {
                __Vdly__sys_top__DOT__sctl__DOT__pan_wr 
                    = (1U & (IData)((vlSelfRef.sys_top__DOT__sctl__DOT__fifo
                                     [vlSelfRef.sys_top__DOT__sctl__DOT__f_rp] 
                                     >> 0x00000029U)));
                vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd_voice 
                    = (0x0000001fU & (IData)((vlSelfRef.sys_top__DOT__sctl__DOT__fifo
                                              [vlSelfRef.sys_top__DOT__sctl__DOT__f_rp] 
                                              >> 0x00000024U)));
                vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd_word 
                    = (0x0000000fU & (IData)((vlSelfRef.sys_top__DOT__sctl__DOT__fifo
                                              [vlSelfRef.sys_top__DOT__sctl__DOT__f_rp] 
                                              >> 0x00000020U)));
                vlSelfRef.sys_top__DOT__sctl__DOT__snd_mask 
                    = (0x0000ffffU & (IData)((vlSelfRef.sys_top__DOT__sctl__DOT__fifo
                                              [vlSelfRef.sys_top__DOT__sctl__DOT__f_rp] 
                                              >> 0x00000010U)));
                vlSelfRef.sys_top__DOT__sctl__DOT__snd_data 
                    = (0x0000ffffU & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__fifo
                                             [vlSelfRef.sys_top__DOT__sctl__DOT__f_rp]));
                __Vdly__sys_top__DOT__sctl__DOT__snd_wr 
                    = (1U & (~ (IData)((vlSelfRef.sys_top__DOT__sctl__DOT__fifo
                                        [vlSelfRef.sys_top__DOT__sctl__DOT__f_rp] 
                                        >> 0x29U))));
                vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__f_rp 
                    = (0x0000003fU & ((IData)(1U) + (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__f_rp)));
            }
        } else if (vlSelfRef.sys_top__DOT__sctl__DOT__tick_pending) {
            vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__tick_pending = 0U;
            vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__q_state = 1U;
        } else if ((((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__rb_pending) 
                     & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__f_empty)) 
                    & (~ (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd_busy)))) {
            vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__rb_pending = 0U;
            __Vdly__sys_top__DOT__sctl__DOT__rb_start = 1U;
            vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__q_state = 4U;
        }
    }
    vlSelfRef.sys_top__DOT__snd_rb = __Vdly__sys_top__DOT__snd_rb;
    vlSelfRef.sys_top__DOT__sctl__DOT__q_state = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__q_state;
    vlSelfRef.sys_top__DOT__sctl__DOT__div = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__div;
    vlSelfRef.sys_top__DOT__sctl__DOT__tick_pending 
        = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__tick_pending;
    vlSelfRef.sys_top__DOT__sctl__DOT__rb_pending = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__rb_pending;
    vlSelfRef.sys_top__DOT__sctl__DOT__cap_half = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__cap_half;
    vlSelfRef.sys_top__DOT__sctl__DOT__pan_wr = __Vdly__sys_top__DOT__sctl__DOT__pan_wr;
    vlSelfRef.sys_top__DOT__sctl__DOT__m_client = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__m_client;
    if (vlSelfRef.__VdlySet__sys_top__DOT__sctl__DOT__fifo__v0) {
        vlSelfRef.sys_top__DOT__sctl__DOT__fifo[vlSelfRef.__VdlyDim0__sys_top__DOT__sctl__DOT__fifo__v0] 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sctl__DOT__fifo__v0;
    }
    vlSelfRef.sys_top__DOT__sctl__DOT__wr_done = ((1U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__m_state)) 
                                                  & ((~ (IData)(vlSelfRef.DDRAM_BUSY)) 
                                                     & (IData)(vlSelfRef.DDRAM_WE)));
    vlSelfRef.sys_top__DOT__sctl__DOT__f_rp = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__f_rp;
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__rom__v0) {
        vlSelfRef.sys_top__DOT__sys__DOT__rom[vlSelfRef.__VdlyDim0__sys_top__DOT__sys__DOT__rom__v0] 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__rom__v0;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__ram__v0) {
        vlSelfRef.sys_top__DOT__sys__DOT__ram[vlSelfRef.__VdlyDim0__sys_top__DOT__sys__DOT__ram__v0] 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__ram__v0;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__ram__v1) {
        vlSelfRef.sys_top__DOT__sys__DOT__ram[__VdlyDim0__sys_top__DOT__sys__DOT__ram__v1] 
            = ((0x00ffU & vlSelfRef.sys_top__DOT__sys__DOT__ram
                [__VdlyDim0__sys_top__DOT__sys__DOT__ram__v1]) 
               | ((SData)((IData)(__VdlyVal__sys_top__DOT__sys__DOT__ram__v1)) 
                  << 8U));
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__ram__v2) {
        vlSelfRef.sys_top__DOT__sys__DOT__ram[__VdlyDim0__sys_top__DOT__sys__DOT__ram__v2] 
            = ((0xff00U & vlSelfRef.sys_top__DOT__sys__DOT__ram
                [__VdlyDim0__sys_top__DOT__sys__DOT__ram__v2]) 
               | (SData)(__VdlyVal__sys_top__DOT__sys__DOT__ram__v2));
    }
    if (vlSelfRef.reset) {
        vlSelfRef.sys_top__DOT__sys__DOT__lcd__DOT__active_cg = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__lcd_wr = __Vdly__sys_top__DOT__sys__DOT__lcd_wr;
        vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__mfp__DOT__isr = 0U;
    } else {
        if (vlSelfRef.sys_top__DOT__sys__DOT__lcd_wr) {
            if ((1U & (~ (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                          >> 1U)))) {
                if ((0x00000080U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__dob))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__lcd__DOT__active_cg = 0U;
                } else if ((0x00000040U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__dob))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__lcd__DOT__active_cg = 1U;
                } else if ((1U & (~ ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__dob) 
                                     >> 5U)))) {
                    if ((1U & (~ ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__dob) 
                                  >> 4U)))) {
                        if ((1U & (~ ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__dob) 
                                      >> 3U)))) {
                            if ((1U & (~ ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__dob) 
                                          >> 2U)))) {
                                if ((2U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__dob))) {
                                    vlSelfRef.sys_top__DOT__sys__DOT__lcd__DOT__active_cg = 0U;
                                } else if ((1U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__dob))) {
                                    vlSelfRef.sys_top__DOT__sys__DOT__lcd__DOT__active_cg = 0U;
                                }
                            }
                        }
                    }
                }
            }
        }
        vlSelfRef.sys_top__DOT__sys__DOT__lcd_wr = __Vdly__sys_top__DOT__sys__DOT__lcd_wr;
        if (vlSelfRef.sys_top__DOT__sys__DOT__mfp_iack) {
            if (vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_any) {
                vlSelfRef.sys_top__DOT__sys__DOT__mfp_vector 
                    = ((0x000000f0U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__vr)) 
                       | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_ch));
                if ((8U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__vr))) {
                    vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__mfp__DOT__isr 
                        = ((IData)(vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__mfp__DOT__isr) 
                           | (0x0000ffffU & ((IData)(1U) 
                                             << (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_ch))));
                }
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__mfp_vector = 0x0fU;
            }
        }
        if (vlSelfRef.sys_top__DOT__sys__DOT__mfp_cs_wr) {
            if ((1U & (~ (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                          >> 5U)))) {
                if ((0x00000010U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)) {
                    if ((1U & (~ (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                                  >> 3U)))) {
                        if ((4U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)) {
                            if ((1U & (~ (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                                          >> 1U)))) {
                                vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__mfp__DOT__isr 
                                    = ((0xff00U & (IData)(vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__mfp__DOT__isr)) 
                                       | (0x000000ffU 
                                          & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__isr) 
                                             & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__dob))));
                            }
                        } else {
                            vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__mfp__DOT__isr 
                                = ((2U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                    ? ((0x00ffU & (IData)(vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__mfp__DOT__isr)) 
                                       | (0x0000ff00U 
                                          & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__isr) 
                                             & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__dob) 
                                                << 8U))))
                                    : ((0xff00U & (IData)(vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__mfp__DOT__isr)) 
                                       | (0x000000ffU 
                                          & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__isr) 
                                             & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__dob)))));
                        }
                    }
                } else if ((8U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)) {
                    if ((4U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)) {
                        if ((2U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)) {
                            vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__mfp__DOT__isr 
                                = ((0x00ffU & (IData)(vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__mfp__DOT__isr)) 
                                   | (0x0000ff00U & 
                                      ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__isr) 
                                       & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__dob) 
                                          << 8U))));
                        }
                    }
                }
            }
        }
    }
    vlSelfRef.sys_top__DOT__sys__DOT__mfp_iack = __Vdly__sys_top__DOT__sys__DOT__mfp_iack;
    vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__isr 
        = vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__mfp__DOT__isr;
    vlSelfRef.sys_top__DOT__snd_wr_full = ((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__f_rp) 
                                           == (0x0000003fU 
                                               & ((IData)(1U) 
                                                  + (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__f_wp))));
    vlSelfRef.sys_top__DOT__sctl__DOT__f_empty = ((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__f_wp) 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__f_rp));
    vlSelfRef.sys_top__DOT__sctl__DOT__snd_done = 0U;
    vlSelfRef.sys_top__DOT__sctl__DOT__rb_done = 0U;
    if (vlSelfRef.reset) {
        vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__state = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd_busy = 0U;
        vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__mem_req = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd_l = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd_r = 0U;
        vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__nx_valid = 0U;
        vlSelfRef.__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v0 = 1U;
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__vr = 0U;
    } else {
        if (vlSelfRef.sys_top__DOT__sctl__DOT__snd_wr) {
            if ((0x0cU > (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd_word))) {
                if ((1U & (~ ((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd_word) 
                              >> 3U)))) {
                    if ((4U & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd_word))) {
                        if ((1U & (~ ((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd_word) 
                                      >> 1U)))) {
                            if ((1U & ((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd_word) 
                                       & (vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__wr_val 
                                          >> 9U)))) {
                                __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_level__v32 
                                    = (0x00000fffU 
                                       & (vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__wr_val 
                                          >> 0x0cU));
                                __VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_level__v32 
                                    = vlSelfRef.sys_top__DOT__sctl__DOT__snd_voice;
                                vlSelfRef.__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_level__v32 = 1U;
                            }
                        }
                    } else if ((1U & (~ ((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd_word) 
                                         >> 1U)))) {
                        __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v32 
                            = (0x00ffffffU & vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__wr_val);
                        __VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v32 
                            = vlSelfRef.sys_top__DOT__sctl__DOT__snd_voice;
                        vlSelfRef.__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v32 = 1U;
                        __VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_frac__v32 
                            = vlSelfRef.sys_top__DOT__sctl__DOT__snd_voice;
                    }
                }
            }
        }
        if (((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__mem_req) 
             & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__mem_ack))) {
            vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__mem_req = 0U;
        }
        if ((8U & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__state))) {
            if ((4U & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__state))) {
                if ((2U & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__state))) {
                    if ((1U & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__state))) {
                        vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__state = 0U;
                    } else {
                        if ((0x00040000U & vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_pitch)) {
                            __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_frac__v33 
                                = (0x00000fffU & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__pos));
                            __VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_frac__v33 
                                = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v;
                            vlSelfRef.__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_frac__v33 = 1U;
                            __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v33 
                                = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__addr_st;
                            __VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v33 
                                = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v;
                        }
                        vlSelfRef.sys_top__DOT__sctl__DOT__snd_busy = 0U;
                        vlSelfRef.sys_top__DOT__sctl__DOT__rb_done = 1U;
                        vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__state = 0U;
                    }
                } else if ((1U & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__state))) {
                    vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__addr_st 
                        = (((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__wrap1) 
                            & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__wrap2))
                            ? vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__addr_wrap
                            : vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__addr1);
                    vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__state = 0x0eU;
                } else {
                    vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__wrap1 
                        = (vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__addr1 
                           > vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_le);
                    vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__addr_wrap 
                        = (0x00ffffffU & (vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__addr1 
                                          - vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__lspan));
                    vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__state = 0x0dU;
                }
            } else if ((2U & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__state))) {
                if ((1U & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__state))) {
                    vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__addr1 
                        = (0x00ffffffU & ((vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_addr 
                                           + (0x0000001fU 
                                              & (vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_pitch 
                                                 >> 0x0cU))) 
                                          + (1U & ((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__pos) 
                                                   >> 0x0cU))));
                    vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__state = 0x0cU;
                } else {
                    vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__pos 
                        = (0x00001fffU & ((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_frac) 
                                          + (0x00000fffU 
                                             & vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_pitch)));
                    vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__lspan 
                        = (0x00ffffffU & (vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_le 
                                          - vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_ls));
                    vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__wrap2 
                        = (vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_le 
                           >= vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_ls);
                    vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__state = 0x0bU;
                }
            } else if ((1U & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__state))) {
                vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__r_addr 
                    = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr
                    [vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v];
                vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__r_frac 
                    = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac
                    [vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v];
                vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__r_pitch 
                    = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch
                    [vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v];
                vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__r_ls 
                    = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart
                    [vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v];
                vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__r_le 
                    = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend
                    [vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v];
                if (((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__mem_req) 
                     & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__mem_ack))) {
                    vlSelfRef.sys_top__DOT__snd_rb_data 
                        = vlSelfRef.sys_top__DOT__sctl__DOT__mem_s0;
                    vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__state = 0x0aU;
                }
            } else {
                vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__acc_l 
                    = (vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__acc_l 
                       + VL_EXTENDS_II(32,22, vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__pl));
                vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__acc_r 
                    = (vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__acc_r 
                       + VL_EXTENDS_II(32,22, vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__pr));
                if ((0x1fU == (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v))) {
                    __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__clamp16__16__x 
                        = VL_SHIFTRS_III(32,32,32, 
                                         (vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__acc_l 
                                          + VL_EXTENDS_II(32,22, vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__pl)), 6U);
                    __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__clamp16__17__x 
                        = VL_SHIFTRS_III(32,32,32, 
                                         (vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__acc_r 
                                          + VL_EXTENDS_II(32,22, vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__pr)), 6U);
                    __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__clamp16__16__Vfuncout 
                        = (VL_LTS_III(32, 0x00007fffU, __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__clamp16__16__x)
                            ? 0x00007fffU : (VL_GTS_III(32, 0xffff8000U, __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__clamp16__16__x)
                                              ? 0x00008000U
                                              : (0x0000ffffU 
                                                 & __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__clamp16__16__x)));
                    __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__clamp16__17__Vfuncout 
                        = (VL_LTS_III(32, 0x00007fffU, __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__clamp16__17__x)
                            ? 0x00007fffU : (VL_GTS_III(32, 0xffff8000U, __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__clamp16__17__x)
                                              ? 0x00008000U
                                              : (0x0000ffffU 
                                                 & __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__clamp16__17__x)));
                    vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__state = 0U;
                    vlSelfRef.sys_top__DOT__sctl__DOT__snd_busy = 0U;
                    vlSelfRef.sys_top__DOT__sctl__DOT__snd_done = 1U;
                    vlSelfRef.sys_top__DOT__sctl__DOT__snd_l 
                        = __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__clamp16__16__Vfuncout;
                    vlSelfRef.sys_top__DOT__sctl__DOT__snd_r 
                        = __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__clamp16__17__Vfuncout;
                } else {
                    vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__v 
                        = (0x0000001fU & ((IData)(1U) 
                                          + (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v)));
                    vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__state = 1U;
                }
            }
        } else if ((4U & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__state))) {
            if ((2U & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__state))) {
                if ((1U & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__state))) {
                    vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__pl 
                        = (0x003fffffU & VL_MULS_III(22, 
                                                     (0x003fffffU 
                                                      & VL_EXTENDS_II(22,17, vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__vout)), 
                                                     (0x003fffffU 
                                                      & VL_EXTENDS_II(22,5, (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_pan)))));
                    vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__pr 
                        = (0x003fffffU & VL_MULS_III(22, 
                                                     (0x003fffffU 
                                                      & VL_EXTENDS_II(22,17, vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__vout)), 
                                                     (0x003fffffU 
                                                      & VL_EXTENDS_II(22,5, 
                                                                      (0x0000000fU 
                                                                       & ((IData)(0x0fU) 
                                                                          - (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_pan)))))));
                    __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_level__v33 
                        = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__lvl_n;
                    __VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_level__v33 
                        = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v;
                    vlSelfRef.__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_level__v33 = 1U;
                    vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__state = 8U;
                    __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_disp__v32 
                        = (0x000000ffU & ((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__lvl_n) 
                                          >> 4U));
                    __VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_disp__v32 
                        = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v;
                    vlSelfRef.__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_disp__v32 = 1U;
                    if ((1U & (~ (vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_pitch 
                                  >> 0x12U)))) {
                        __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_frac__v34 
                            = (0x00000fffU & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__pos));
                        __VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_frac__v34 
                            = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v;
                        vlSelfRef.__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_frac__v34 = 1U;
                        __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v34 
                            = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__addr_st;
                        __VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v34 
                            = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v;
                    }
                } else {
                    vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__vout 
                        = (0x0001ffffU & (IData)((0x00000003ffffffffULL 
                                                  & VL_SHIFTRS_QQI(34,34,32, vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__mul2, 0x00000010U))));
                    vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__state = 7U;
                }
            } else if ((1U & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__state))) {
                vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__mul2 
                    = (0x00000003ffffffffULL & VL_MULS_QQQ(34, 
                                                           (0x00000003ffffffffULL 
                                                            & VL_EXTENDS_QI(34,17, vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__smp)), 
                                                           (0x00000003ffffffffULL 
                                                            & VL_EXTENDS_QI(34,18, vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__gain))));
                vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__addr_st 
                    = (((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__wrap1) 
                        & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__wrap2))
                        ? vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__addr_wrap
                        : vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__addr1);
                vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__state = 6U;
            } else {
                vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__smp 
                    = (0x0001ffffU & ((0x1fffffffU 
                                       & VL_EXTENDS_II(29,17, 
                                                       ((0x00010000U 
                                                         & ((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__s0_r) 
                                                            << 1U)) 
                                                        | (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__s0_r)))) 
                                      + (0x1fffffffU 
                                         & VL_SHIFTRS_III(29,29,32, vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__prod, 0x0000000cU))));
                vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__wrap1 
                    = (vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__addr1 
                       > vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_le);
                vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__state = 5U;
                vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__addr_wrap 
                    = (0x00ffffffU & (vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__addr1 
                                      - vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__lspan));
            }
        } else if ((2U & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__state))) {
            if ((1U & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__state))) {
                __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__level_to_gain__18__level 
                    = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__lvl_n;
                vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__level_to_gain__Vstatic__e 
                    = (7U & ((IData)(__Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__level_to_gain__18__level) 
                             >> 9U));
                vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__prod 
                    = (0x1fffffffU & VL_MULS_III(29, 
                                                 (0x1fffffffU 
                                                  & VL_EXTENDS_II(29,17, vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__diff)), 
                                                 (0x1fffffffU 
                                                  & VL_EXTENDS_II(29,13, (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_frac)))));
                __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__level_to_gain__18__Vfuncout 
                    = (0x0001ffffU & ((0U == (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__level_to_gain__Vstatic__e))
                                       ? (0x000001ffU 
                                          & (IData)(__Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__level_to_gain__18__level))
                                       : ((0x00000200U 
                                           | (0x000001ffU 
                                              & (IData)(__Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__level_to_gain__18__level))) 
                                          << (7U & 
                                              ((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__level_to_gain__Vstatic__e) 
                                               - (IData)(1U))))));
                vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__addr1 
                    = (0x00ffffffU & ((vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_addr 
                                       + (0x0000001fU 
                                          & (vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_pitch 
                                             >> 0x0cU))) 
                                      + (1U & ((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__pos) 
                                               >> 0x0cU))));
                vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__state = 4U;
                vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__gain 
                    = __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__level_to_gain__18__Vfuncout;
            } else {
                __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__env_next__19__env 
                    = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_env;
                __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__env_next__19__level 
                    = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_level;
                vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__diff 
                    = (0x0001ffffU & (((0x00010000U 
                                        & ((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__s1_r) 
                                           << 1U)) 
                                       | (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__s1_r)) 
                                      - ((0x00010000U 
                                          & ((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__s0_r) 
                                             << 1U)) 
                                         | (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__s0_r))));
                vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__pos 
                    = (0x00001fffU & ((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_frac) 
                                      + (0x00000fffU 
                                         & vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_pitch)));
                vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__lspan 
                    = (0x00ffffffU & (vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_le 
                                      - vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_ls));
                vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__state = 3U;
                vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__target 
                    = (0x00000fffU & (__Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__env_next__19__env 
                                      >> 0x0cU));
                vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__wrap2 
                    = (vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_le 
                       >= vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_ls);
                vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__rate 
                    = (0x000001ffU & __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__env_next__19__env);
                vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__delta 
                    = ((0U == (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__rate))
                        ? 1U : ((0x00000100U & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__rate))
                                 ? (0x000001ffU & (- (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__rate)))
                                 : (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__rate)));
                vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__up 
                    = (0x00001fffU & ((IData)(__Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__env_next__19__level) 
                                      + (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__delta)));
                vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__dn 
                    = (0x00003fffU & ((IData)(__Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__env_next__19__level) 
                                      - (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__delta)));
                __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__env_next__19__Vfuncout 
                    = (0x00000fffU & (((IData)(__Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__env_next__19__level) 
                                       < (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__target))
                                       ? (((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__up) 
                                           > (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__target))
                                           ? (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__target)
                                           : (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__up))
                                       : (((IData)(__Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__env_next__19__level) 
                                           > (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__target))
                                           ? (VL_LTS_III(14, (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__dn), (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__target))
                                               ? (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__target)
                                               : (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__dn))
                                           : (IData)(__Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__env_next__19__level))));
                vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__lvl_n 
                    = __Vfunc_sys_top__DOT__sctl__DOT__snd__DOT__env_next__19__Vfuncout;
            }
        } else if ((1U & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__state))) {
            vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__r_addr 
                = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr
                [vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v];
            vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__r_frac 
                = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac
                [vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v];
            vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__r_pitch 
                = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch
                [vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v];
            vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_env 
                = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env
                [vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v];
            vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__r_ls 
                = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart
                [vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v];
            vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__r_le 
                = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend
                [vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v];
            vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_level 
                = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level
                [vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v];
            vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_pan 
                = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan
                [vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v];
            if (vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__nx_valid) {
                vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__s0_r 
                    = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__nx_s0;
                vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__s1_r 
                    = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__nx_s1;
                vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__nx_valid = 0U;
                vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__state = 2U;
                if ((0x1fU != (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v))) {
                    vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__mem_req = 1U;
                    vlSelfRef.sys_top__DOT__sctl__DOT__mem_addr 
                        = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__fetch_addr_next;
                }
            } else if (((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__mem_req) 
                        & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__mem_ack))) {
                vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__s0_r 
                    = vlSelfRef.sys_top__DOT__sctl__DOT__mem_s0;
                vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__s1_r 
                    = vlSelfRef.sys_top__DOT__sctl__DOT__mem_s1;
                vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__state = 2U;
                if ((0x1fU != (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v))) {
                    vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__mem_req = 1U;
                    vlSelfRef.sys_top__DOT__sctl__DOT__mem_addr 
                        = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__fetch_addr_next;
                }
            }
        } else if (vlSelfRef.sys_top__DOT__sctl__DOT__rb_start) {
            vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__v 
                = vlSelfRef.sys_top__DOT__snd_voice;
            vlSelfRef.sys_top__DOT__sctl__DOT__snd_busy = 1U;
            vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__mem_req = 1U;
            vlSelfRef.sys_top__DOT__sctl__DOT__mem_addr 
                = (0x003fffffU & ((IData)(4U) + vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr
                                  [vlSelfRef.sys_top__DOT__snd_voice]));
            vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__state = 9U;
        } else if (vlSelfRef.sys_top__DOT__sctl__DOT__snd_start) {
            vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__v = 0U;
            vlSelfRef.sys_top__DOT__sctl__DOT__snd_busy = 1U;
            vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__acc_l = 0U;
            vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__acc_r = 0U;
            vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__nx_valid = 0U;
            vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__mem_req = 1U;
            vlSelfRef.sys_top__DOT__sctl__DOT__mem_addr 
                = (0x003fffffU & ((IData)(4U) + vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[0U]));
            vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__state = 1U;
        }
        if ((((((1U != (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__state)) 
                & (0U != (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__state))) 
               & (9U != (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__state))) 
              & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__mem_req)) 
             & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__mem_ack))) {
            vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__nx_s0 
                = vlSelfRef.sys_top__DOT__sctl__DOT__mem_s0;
            vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__nx_s1 
                = vlSelfRef.sys_top__DOT__sctl__DOT__mem_s1;
            vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__nx_valid = 1U;
        }
        if (vlSelfRef.sys_top__DOT__sys__DOT__mfp_cs_wr) {
            if ((1U & (~ (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                          >> 5U)))) {
                if ((0x00000010U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)) {
                    if ((1U & (~ (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                                  >> 3U)))) {
                        if ((4U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)) {
                            if ((2U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)) {
                                vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__vr 
                                    = (0x000000ffU 
                                       & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__dob));
                            }
                        }
                    }
                }
            }
        }
    }
    vlSelfRef.sys_top__DOT__sctl__DOT__mem_s0 = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__mem_s0;
    vlSelfRef.sys_top__DOT__sctl__DOT__mem_s1 = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__mem_s1;
    vlSelfRef.sys_top__DOT__sctl__DOT__rb_start = __Vdly__sys_top__DOT__sctl__DOT__rb_start;
    vlSelfRef.sys_top__DOT__sctl__DOT__snd_start = __Vdly__sys_top__DOT__sctl__DOT__snd_start;
    vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__state 
        = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__state;
    vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__nx_valid 
        = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__nx_valid;
    vlSelfRef.sys_top__DOT__sctl__DOT__mem_req = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__mem_req;
    vlSelfRef.sys_top__DOT__sctl__DOT__mem_ack = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__mem_ack;
    vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__addr_st 
        = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__addr_st;
    vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__wrap1 
        = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__wrap1;
    vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__addr_wrap 
        = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__addr_wrap;
    vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__addr1 
        = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__addr1;
    vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__pos 
        = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__pos;
    vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__lspan 
        = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__lspan;
    vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__wrap2 
        = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__wrap2;
    vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_addr 
        = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__r_addr;
    vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_frac 
        = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__r_frac;
    vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_pitch 
        = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__r_pitch;
    vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_ls 
        = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__r_ls;
    vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__r_le 
        = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__r_le;
    vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__acc_l 
        = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__acc_l;
    vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__acc_r 
        = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__acc_r;
    if (vlSelfRef.__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_pitch__v0) {
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[0U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[1U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[2U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[3U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[4U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[5U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[6U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[7U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[8U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[9U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[10U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[11U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[12U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[13U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[14U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[15U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[16U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[17U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[18U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[19U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[20U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[21U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[22U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[23U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[24U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[25U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[26U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[27U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[28U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[29U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[30U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[31U] = 0U;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_pitch__v32) {
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[vlSelfRef.__VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_pitch__v32] 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_pitch__v32;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_lstart__v0) {
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[0U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[1U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[2U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[3U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[4U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[5U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[6U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[7U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[8U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[9U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[10U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[11U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[12U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[13U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[14U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[15U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[16U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[17U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[18U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[19U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[20U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[21U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[22U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[23U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[24U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[25U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[26U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[27U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[28U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[29U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[30U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[31U] = 0U;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_lstart__v32) {
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[vlSelfRef.__VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_lstart__v32] 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_lstart__v32;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_lend__v0) {
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[0U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[1U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[2U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[3U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[4U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[5U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[6U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[7U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[8U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[9U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[10U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[11U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[12U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[13U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[14U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[15U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[16U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[17U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[18U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[19U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[20U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[21U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[22U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[23U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[24U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[25U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[26U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[27U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[28U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[29U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[30U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[31U] = 0U;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_lend__v32) {
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_lend[vlSelfRef.__VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_lend__v32] 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_lend__v32;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_env__v0) {
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[0U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[1U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[2U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[3U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[4U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[5U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[6U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[7U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[8U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[9U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[10U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[11U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[12U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[13U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[14U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[15U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[16U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[17U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[18U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[19U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[20U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[21U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[22U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[23U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[24U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[25U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[26U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[27U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[28U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[29U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[30U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[31U] = 0U;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_env__v32) {
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_env[vlSelfRef.__VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_env__v32] 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_env__v32;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_pan__v0) {
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[0U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[1U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[2U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[3U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[4U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[5U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[6U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[7U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[8U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[9U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[10U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[11U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[12U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[13U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[14U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[15U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[16U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[17U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[18U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[19U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[20U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[21U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[22U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[23U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[24U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[25U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[26U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[27U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[28U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[29U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[30U] = 8U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[31U] = 8U;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_pan__v32) {
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_pan[vlSelfRef.__VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_pan__v32] 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_pan__v32;
    }
    vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v 
        = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__v;
    vlSelfRef.sys_top__DOT__sctl__DOT__snd_wr = __Vdly__sys_top__DOT__sctl__DOT__snd_wr;
    vlSelfRef.sys_top__DOT__snd_voice = vlSelfRef.__Vdly__sys_top__DOT__snd_voice;
    vlSelfRef.sys_top__DOT__sctl__DOT__snd_word = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd_word;
    vlSelfRef.sys_top__DOT__sctl__DOT__snd_voice = vlSelfRef.__Vdly__sys_top__DOT__sctl__DOT__snd_voice;
    if (vlSelfRef.__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v0) {
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[0U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[1U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[2U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[3U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[4U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[5U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[6U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[7U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[8U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[9U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[10U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[11U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[12U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[13U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[14U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[15U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[16U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[17U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[18U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[19U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[20U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[21U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[22U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[23U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[24U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[25U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[26U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[27U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[28U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[29U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[30U] = 0U;
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[31U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[0U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[1U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[2U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[3U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[4U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[5U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[6U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[7U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[8U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[9U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[10U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[11U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[12U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[13U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[14U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[15U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[16U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[17U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[18U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[19U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[20U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[21U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[22U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[23U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[24U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[25U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[26U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[27U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[28U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[29U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[30U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[31U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[0U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[1U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[2U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[3U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[4U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[5U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[6U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[7U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[8U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[9U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[10U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[11U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[12U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[13U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[14U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[15U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[16U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[17U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[18U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[19U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[20U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[21U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[22U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[23U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[24U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[25U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[26U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[27U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[28U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[29U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[30U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[31U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[0U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[1U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[2U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[3U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[4U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[5U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[6U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[7U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[8U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[9U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[10U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[11U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[12U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[13U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[14U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[15U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[16U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[17U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[18U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[19U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[20U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[21U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[22U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[23U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[24U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[25U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[26U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[27U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[28U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[29U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[30U] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[31U] = 0U;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_disp__v32) {
        sys_top__DOT__sctl__DOT__snd__DOT__v_disp[__VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_disp__v32] 
            = __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_disp__v32;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_level__v32) {
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[__VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_level__v32] 
            = __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_level__v32;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_level__v33) {
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_level[__VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_level__v33] 
            = __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_level__v33;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v32) {
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[__VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_frac__v32] = 0U;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[__VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v32] 
            = __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v32;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_frac__v33) {
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[__VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_frac__v33] 
            = __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_frac__v33;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[__VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v33] 
            = __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v33;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_frac__v34) {
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_frac[__VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_frac__v34] 
            = __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_frac__v34;
        vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr[__VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v34] 
            = __VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v34;
    }
    vlSelfRef.sys_top__DOT__snd_raw = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__raw
        [vlSelfRef.sys_top__DOT__snd_voice][vlSelfRef.sys_top__DOT__snd_word];
    vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__wr_new 
        = ((vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__raw
            [vlSelfRef.sys_top__DOT__sctl__DOT__snd_voice]
            [vlSelfRef.sys_top__DOT__sctl__DOT__snd_word] 
            & (~ (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd_mask))) 
           | ((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd_data) 
              & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd_mask)));
    vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__fetch_addr_next 
        = (0x003fffffU & ((IData)(4U) + vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr
                          [(0x0000001fU & ((IData)(1U) 
                                           + (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v)))]));
    vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__wr_val 
        = ((1U & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd_word))
            ? (((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__raw
                        [vlSelfRef.sys_top__DOT__sctl__DOT__snd_voice]
                        [(0x0000000eU & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd_word))]) 
                << 0x00000010U) | (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__wr_new))
            : (((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__wr_new) 
                << 0x00000010U) | vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__raw
               [vlSelfRef.sys_top__DOT__sctl__DOT__snd_voice]
               [(1U | (0x0000000eU & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd_word)))]));
}

void Vsys_top___024root___nba_sequent__TOP__8(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___nba_sequent__TOP__8\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ sys_top__DOT__sys__DOT__cpu__DOT__bgBlock;
    sys_top__DOT__sys__DOT__cpu__DOT__bgBlock = 0;
    // Body
    if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
        .__PVT__extReset) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rRWn = 1U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rAS = 1U;
    } else {
        if ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
             .__PVT__enPhi1 & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busEnding))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rRWn = 1U;
        } else if ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
                    .__PVT__enPhi1 & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__isWriteReg))) {
            if (((2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase) 
                 & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__isWriteReg))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rRWn = 0U;
            }
        }
        if ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
             .__PVT__enPhi1 & (2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rAS = 0U;
        } else if ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
                    .__PVT__enPhi2 & (6U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rAS = 1U;
        } else if ((0U != (0xfffffff9U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
                                          .__PVT__enPhi2 
                                          & (5U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase))))) {
            if ((1U & (~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__isRmcReg)))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rAS = 1U;
            }
        }
    }
    if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
        .__PVT__pwrUp) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__isRmcReg = 0U;
    } else if ((1U & (~ (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
                         .__PVT__enPhi2 & ((5U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase) 
                                           | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__bcReset)))))) {
        if (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT1) 
             & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
             .__PVT__permStart)) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__isRmcReg 
                = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                   .__PVT__isRmc & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                    .__PVT__isWrite));
        }
    }
    if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
        .__PVT__extReset) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rLDS = 1U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rUDS = 1U;
    } else if ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
                .__PVT__enPhi1 & (2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase))) {
        if ((1U & ((~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__isWriteReg)) 
                   & (~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busAddrErr))))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rLDS 
                = (1U & (~ ((~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__bciByte)) 
                            | vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rUDS 
                = (1U & (~ ((~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__bciByte)) 
                            | (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob))));
        }
    } else if ((((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
                  .__PVT__enPhi1 & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__isWriteReg)) 
                 & (3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase)) 
                & (~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busAddrErr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rLDS 
            = (1U & (~ ((~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__bciByte)) 
                        | vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)));
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rUDS 
            = (1U & (~ ((~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__bciByte)) 
                        | (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob))));
    } else if ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
                .__PVT__enPhi2 & (5U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rLDS = 1U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rUDS = 1U;
    }
    if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
        .__PVT__pwrUp) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__isWriteReg = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__bciByte = 0U;
    } else if ((1U & (~ (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
                         .__PVT__enPhi2 & ((5U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase) 
                                           | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__bcReset)))))) {
        if (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT1) 
             & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
             .__PVT__permStart)) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__isWriteReg 
                = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                .__PVT__isWrite;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__bciByte 
                = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__isByteT4;
        }
    }
    if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
        .__PVT__extReset) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__rFC = 0U;
    } else if (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT1) 
                & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                .__PVT__permStart)) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__rFC 
            = ((3U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__rFC)) 
               | ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__pswS) 
                  << 2U));
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__rFC 
            = ((5U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__rFC)) 
               | (2U & ((0x0001fffeU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                        >> 0x0000000fU)) 
                        | (((~ (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                >> 0x0fU)) & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod
                            .__PVT__isPcRel) << 1U))));
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__rFC 
            = ((6U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__rFC)) 
               | (1U & ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                         >> 0x0fU) | ((~ (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                          >> 0x10U)) 
                                      & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod
                                         .__PVT__isPcRel)))));
    }
    if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
        .__PVT__pwrUp) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__pswT = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__pswS = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__pswI = 0U;
    } else if ((1U & (~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT4)))) {
        if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT3) {
            if ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                 .__PVT__ftu2Sr & (~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdToCcr_t4)))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__pswT 
                    = (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__ftu) 
                             >> 0x0fU));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__pswS 
                    = (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__ftu) 
                             >> 0x0dU));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__pswI 
                    = (7U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__ftu) 
                             >> 8U));
            } else {
                if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                    .__PVT__initST) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__pswS = 1U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__pswT = 0U;
                }
                if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                    .__PVT__inl2psw) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__pswI 
                        = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__inl;
                }
            }
        }
    }
    if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
        .__PVT__extReset) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__inl = 7U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__updIll = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch = 0U;
    } else {
        if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
            .__PVT__enPhi1) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase 
                = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__next;
        }
        if ((1U & (~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT4)))) {
            if (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT1) 
                 & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__updIll))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__inl 
                    = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__iIpl;
            }
        }
        if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT4) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__updIll 
                = (1U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch);
        }
        if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__rstUrom) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                = (0x00007fffU & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch);
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                = (0x0001fffeU & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch);
        } else if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT3) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microOutput;
        }
    }
    if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
        .__PVT__pwrUp) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdToCcr_t4 = 0U;
    } else if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT4) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdToCcr_t4 
            = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod
            .__PVT__toCcr;
    }
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__ftu 
        = vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__cpu__DOT__ftu;
    if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT4) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__isByteT4 
            = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
               .__PVT__busByte & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod
                                  .__PVT__isByte | vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod
                                  .__PVT__isMovep));
    }
    sys_top__DOT__sys__DOT__cpu__DOT__bgBlock = ((6U 
                                                  == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase) 
                                                 | ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rAS) 
                                                    & (2U 
                                                       == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__wClk 
        = (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__iAddrErr) 
                 | (((5U != vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase) 
                     & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__wendReg)) 
                    | ((~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__BeI)) 
                       | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Err6591)))));
    if ((1U & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
               .__PVT__pwrUp))) {
        if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
            .__PVT__enPhi2) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__iIpl 
                = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__rIpl;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__rIpl 
                = (7U & (~ (6U | (1U & (~ (0U != ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
                                                  & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr))))))));
        }
    }
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busArbiter__DOT__next 
        = ((0U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busArbiter__DOT__dmaPhase)
            ? 1U : ((1U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busArbiter__DOT__dmaPhase)
                     ? ((IData)(sys_top__DOT__sys__DOT__cpu__DOT__bgBlock)
                         ? 1U : ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__BgackI)
                                  ? ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__BRi)
                                      ? 1U : 2U) : 4U))
                     : ((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busArbiter__DOT__dmaPhase)
                         ? (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__BRi) 
                             | (IData)(sys_top__DOT__sys__DOT__cpu__DOT__bgBlock))
                             ? (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__BgackI) 
                                 | (IData)(sys_top__DOT__sys__DOT__cpu__DOT__bgBlock))
                                 ? 1U : 4U) : 6U) : 
                        ((2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busArbiter__DOT__dmaPhase)
                          ? 3U : ((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busArbiter__DOT__dmaPhase)
                                   ? (((~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__BRi)) 
                                       & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__BgackI))
                                       ? 3U : 4U) : 
                                  ((6U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busArbiter__DOT__dmaPhase)
                                    ? 5U : ((5U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busArbiter__DOT__dmaPhase)
                                             ? ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__BgackI)
                                                 ? 
                                                ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__BRi)
                                                  ? 1U
                                                  : 3U)
                                                 : 
                                                ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__BRi)
                                                  ? 7U
                                                  : 5U))
                                             : ((7U 
                                                 == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busArbiter__DOT__dmaPhase)
                                                 ? 4U
                                                 : 1U))))))));
    if ((1U & (~ ((5U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busArbiter__DOT__next) 
                  | ((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busArbiter__DOT__next) 
                     | ((6U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busArbiter__DOT__next) 
                        | (2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busArbiter__DOT__next))))))) {
        if (((5U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busArbiter__DOT__next) 
             | ((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busArbiter__DOT__next) 
                | ((6U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busArbiter__DOT__next) 
                   | (2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busArbiter__DOT__next))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: fx68k.sv:2235: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.busArbiter", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1)
                             , '#',32,vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busArbiter__DOT__next);
                VL_STOP_MT("../third_party/fx68k/fx68k.sv", 2235, "");
            }
        }
    }
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__iplComp 
        = ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__iIpl) 
           > (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__pswI));
    if ((1U & (~ VL_ONEHOT_I(((((((((0x1eU == (0x0000001fU 
                                               & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                  >> 2U))) 
                                    | (0x0eU == (0x0000001fU 
                                                 & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                    >> 2U)))) 
                                   << 4U) | (((0x0dU 
                                               == (0x0000001fU 
                                                   & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                      >> 2U))) 
                                              << 3U) 
                                             | ((0x0bU 
                                                 == 
                                                 (0x0000001fU 
                                                  & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                     >> 2U))) 
                                                << 2U))) 
                                 | (((0x0aU == (0x0000001fU 
                                                & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                   >> 2U))) 
                                     << 1U) | (0x1cU 
                                               == (0x0000001fU 
                                                   & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                      >> 2U))))) 
                                << 0x0000000fU) | (
                                                   ((((0x0cU 
                                                       == 
                                                       (0x0000001fU 
                                                        & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                           >> 2U))) 
                                                      << 4U) 
                                                     | (((0x19U 
                                                          == 
                                                          (0x0000001fU 
                                                           & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                              >> 2U))) 
                                                         << 3U) 
                                                        | ((9U 
                                                            == 
                                                            (0x0000001fU 
                                                             & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                                >> 2U))) 
                                                           << 2U))) 
                                                    | (((8U 
                                                         == 
                                                         (0x0000001fU 
                                                          & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                             >> 2U))) 
                                                        << 1U) 
                                                       | (7U 
                                                          == 
                                                          (0x0000001fU 
                                                           & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                              >> 2U))))) 
                                                   << 0x0000000aU)) 
                              | ((((((6U == (0x0000001fU 
                                             & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                >> 2U))) 
                                     << 4U) | (((0x15U 
                                                 == 
                                                 (0x0000001fU 
                                                  & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                     >> 2U))) 
                                                << 3U) 
                                               | ((5U 
                                                   == 
                                                   (0x0000001fU 
                                                    & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                       >> 2U))) 
                                                  << 2U))) 
                                   | (((4U == (0x0000001fU 
                                               & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                  >> 2U))) 
                                       << 1U) | (3U 
                                                 == 
                                                 (0x0000001fU 
                                                  & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                     >> 2U))))) 
                                  << 5U) | ((((0x12U 
                                               == (0x0000001fU 
                                                   & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                      >> 2U))) 
                                              << 4U) 
                                             | (((2U 
                                                  == 
                                                  (0x0000001fU 
                                                   & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                      >> 2U))) 
                                                 << 3U) 
                                                | ((0x11U 
                                                    == 
                                                    (0x0000001fU 
                                                     & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                        >> 2U))) 
                                                   << 2U))) 
                                            | (((1U 
                                                 == 
                                                 (0x0000001fU 
                                                  & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                     >> 2U))) 
                                                << 1U) 
                                               | (0U 
                                                  == 
                                                  (0x0000001fU 
                                                   & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                      >> 2U))))))))))) {
        if ((0U != ((((((((0x1eU == (0x0000001fU & 
                                     (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                      >> 2U))) | (0x0eU 
                                                  == 
                                                  (0x0000001fU 
                                                   & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                      >> 2U)))) 
                         << 4U) | (((0x0dU == (0x0000001fU 
                                               & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                  >> 2U))) 
                                    << 3U) | ((0x0bU 
                                               == (0x0000001fU 
                                                   & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                      >> 2U))) 
                                              << 2U))) 
                       | (((0x0aU == (0x0000001fU & 
                                      (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                       >> 2U))) << 1U) 
                          | (0x1cU == (0x0000001fU 
                                       & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                          >> 2U))))) 
                      << 0x0000000fU) | (((((0x0cU 
                                             == (0x0000001fU 
                                                 & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                    >> 2U))) 
                                            << 4U) 
                                           | (((0x19U 
                                                == 
                                                (0x0000001fU 
                                                 & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                    >> 2U))) 
                                               << 3U) 
                                              | ((9U 
                                                  == 
                                                  (0x0000001fU 
                                                   & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                      >> 2U))) 
                                                 << 2U))) 
                                          | (((8U == 
                                               (0x0000001fU 
                                                & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                   >> 2U))) 
                                              << 1U) 
                                             | (7U 
                                                == 
                                                (0x0000001fU 
                                                 & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                    >> 2U))))) 
                                         << 0x0000000aU)) 
                    | ((((((6U == (0x0000001fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                  >> 2U))) 
                           << 4U) | (((0x15U == (0x0000001fU 
                                                 & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                    >> 2U))) 
                                      << 3U) | ((5U 
                                                 == 
                                                 (0x0000001fU 
                                                  & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                     >> 2U))) 
                                                << 2U))) 
                         | (((4U == (0x0000001fU & 
                                     (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                      >> 2U))) << 1U) 
                            | (3U == (0x0000001fU & 
                                      (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                       >> 2U))))) << 5U) 
                       | ((((0x12U == (0x0000001fU 
                                       & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                          >> 2U))) 
                            << 4U) | (((2U == (0x0000001fU 
                                               & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                  >> 2U))) 
                                       << 3U) | ((0x11U 
                                                  == 
                                                  (0x0000001fU 
                                                   & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                      >> 2U))) 
                                                 << 2U))) 
                          | (((1U == (0x0000001fU & 
                                      (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                       >> 2U))) << 1U) 
                             | (0U == (0x0000001fU 
                                       & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                          >> 2U))))))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: fx68k.sv:2007: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.sequencer", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1)
                             , '#',32,(0x0000001fU 
                                       & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                          >> 2U)));
                VL_STOP_MT("../third_party/fx68k/fx68k.sv", 2007, "");
            }
        }
    }
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__grp1LatchEn 
        = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
           & (0x00000010U != (0x00000012U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__grp0LatchEn 
        = (IData)((0x00000010U == (0x00000012U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__iplStable 
        = ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__iIpl) 
           == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__rIpl));
}

void Vsys_top___024root___nba_comb__TOP__1(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___nba_comb__TOP__1\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw 
        = (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__pswT) 
            << 0x0000000fU) | (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__pswS) 
                                << 0x0000000dU) | (
                                                   ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__pswI) 
                                                    << 8U) 
                                                   | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__pswCcr))));
    if (((((((((0U == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                      >> 8U))) | (1U 
                                                  == 
                                                  (0x0000000fU 
                                                   & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                      >> 8U)))) 
              | (2U == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                       >> 8U)))) | 
             (3U == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                    >> 8U)))) | (4U 
                                                 == 
                                                 (0x0000000fU 
                                                  & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                     >> 8U)))) 
           | (5U == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                    >> 8U)))) | (6U 
                                                 == 
                                                 (0x0000000fU 
                                                  & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                     >> 8U)))) 
         | (7U == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                  >> 8U))))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__ccTest 
            = ((0U == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                      >> 8U))) || (
                                                   (1U 
                                                    != 
                                                    (0x0000000fU 
                                                     & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                        >> 8U))) 
                                                   && (1U 
                                                       & ((2U 
                                                           == 
                                                           (0x0000000fU 
                                                            & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                               >> 8U)))
                                                           ? (IData)(
                                                                     (0U 
                                                                      == 
                                                                      (5U 
                                                                       & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw))))
                                                           : 
                                                          ((3U 
                                                            == 
                                                            (0x0000000fU 
                                                             & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                                >> 8U)))
                                                            ? 
                                                           ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw) 
                                                            | ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw) 
                                                               >> 2U))
                                                            : 
                                                           ((4U 
                                                             == 
                                                             (0x0000000fU 
                                                              & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                                 >> 8U)))
                                                             ? 
                                                            (~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw))
                                                             : 
                                                            ((5U 
                                                              == 
                                                              (0x0000000fU 
                                                               & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                                  >> 8U)))
                                                              ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw)
                                                              : 
                                                             ((6U 
                                                               == 
                                                               (0x0000000fU 
                                                                & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                                   >> 8U)))
                                                               ? 
                                                              (~ 
                                                               ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw) 
                                                                >> 2U))
                                                               : 
                                                              ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw) 
                                                               >> 2U)))))))));
    } else if (((((((((8U == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                             >> 8U))) 
                      | (9U == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                               >> 8U)))) 
                     | (0x0aU == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                 >> 8U)))) 
                    | (0x0bU == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                >> 8U)))) 
                   | (0x0cU == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                               >> 8U)))) 
                  | (0x0dU == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                              >> 8U)))) 
                 | (0x0eU == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                             >> 8U)))) 
                | (0x0fU == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                            >> 8U))))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__ccTest 
            = (1U & ((8U == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                            >> 8U)))
                      ? (~ ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw) 
                            >> 1U)) : ((9U == (0x0000000fU 
                                               & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                  >> 8U)))
                                        ? ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw) 
                                           >> 1U) : 
                                       ((0x0aU == (0x0000000fU 
                                                   & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                      >> 8U)))
                                         ? (~ ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw) 
                                               >> 3U))
                                         : ((0x0bU 
                                             == (0x0000000fU 
                                                 & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                    >> 8U)))
                                             ? ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw) 
                                                >> 3U)
                                             : ((0x0cU 
                                                 == 
                                                 (0x0000000fU 
                                                  & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                     >> 8U)))
                                                 ? 
                                                ((IData)(
                                                         (0x000aU 
                                                          == 
                                                          (0x000aU 
                                                           & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw)))) 
                                                 | (IData)(
                                                           (0U 
                                                            == 
                                                            (0x000aU 
                                                             & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw)))))
                                                 : 
                                                ((0x0dU 
                                                  == 
                                                  (0x0000000fU 
                                                   & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                      >> 8U)))
                                                  ? 
                                                 ((IData)(
                                                          (8U 
                                                           == 
                                                           (0x000aU 
                                                            & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw)))) 
                                                  | (IData)(
                                                            (2U 
                                                             == 
                                                             (0x000aU 
                                                              & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw)))))
                                                  : 
                                                 ((0x0eU 
                                                   == 
                                                   (0x0000000fU 
                                                    & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                       >> 8U)))
                                                   ? 
                                                  ((IData)(
                                                           (0x000aU 
                                                            == 
                                                            (0x000eU 
                                                             & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw)))) 
                                                   | (IData)(
                                                             (0U 
                                                              == 
                                                              (0x000eU 
                                                               & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw)))))
                                                   : 
                                                  ((((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw) 
                                                     >> 2U) 
                                                    | (IData)(
                                                              (8U 
                                                               == 
                                                               (0x000aU 
                                                                & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw))))) 
                                                   | (IData)(
                                                             (2U 
                                                              == 
                                                              (0x000aU 
                                                               & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw)))))))))))));
    }
}

void Vsys_top___024root___nba_sequent__TOP__9(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___nba_sequent__TOP__9\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microOutput 
        = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uRom__DOT__uRam
        [vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microAddr];
}

void Vsys_top___024root___nba_sequent__TOP__10(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___nba_sequent__TOP__10\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr 
        = vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__mfp__DOT__ipr;
    if (vlSelfRef.reset) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr = 0U;
    } else if (vlSelfRef.sys_top__DOT__sys__DOT__mfp_cs_wr) {
        if ((1U & (~ (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                      >> 5U)))) {
            if ((0x00000010U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)) {
                if ((1U & (~ (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                              >> 3U)))) {
                    if ((4U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)) {
                        if ((1U & (~ (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                                      >> 1U)))) {
                            vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr 
                                = ((0xff00U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)) 
                                   | (0x000000ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__dob)));
                        }
                    } else if ((2U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)) {
                        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr 
                            = ((0x00ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)) 
                               | (0x0000ff00U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__dob) 
                                                 << 8U)));
                    }
                }
            }
        }
    }
    vlSelfRef.sys_top__DOT__sys__DOT__mfp_cs_wr = vlSelfRef.__Vdly__sys_top__DOT__sys__DOT__mfp_cs_wr;
    vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_any = 0U;
    if ((1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
               & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_any = 1U;
    }
    if ((2U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
               & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_any = 1U;
    }
    if ((4U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
               & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_any = 1U;
    }
    if ((8U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
               & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_any = 1U;
    }
    if ((0x00000010U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
                        & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_any = 1U;
    }
    if ((0x00000020U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
                        & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_any = 1U;
    }
    if ((0x00000040U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
                        & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_any = 1U;
    }
    if ((0x00000080U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
                        & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_any = 1U;
    }
    if ((0x00000100U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
                        & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_any = 1U;
    }
    if ((0x00000200U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
                        & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_any = 1U;
    }
    if ((0x00000400U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
                        & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_any = 1U;
    }
    if ((0x00000800U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
                        & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_any = 1U;
    }
    if ((0x00001000U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
                        & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_any = 1U;
    }
    if ((0x00002000U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
                        & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_any = 1U;
    }
    if ((0x00004000U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
                        & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_any = 1U;
    }
    vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_ch = 0U;
    if ((1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
               & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_ch = 0U;
    }
    if ((2U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
               & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_ch = 1U;
    }
    if ((4U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
               & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_ch = 2U;
    }
    if ((8U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
               & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_ch = 3U;
    }
    if ((0x00000010U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
                        & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_ch = 4U;
    }
    if ((0x00000020U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
                        & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_ch = 5U;
    }
    if ((0x00000040U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
                        & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_ch = 6U;
    }
    if ((0x00000080U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
                        & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_ch = 7U;
    }
    if ((0x00000100U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
                        & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_ch = 8U;
    }
    if ((0x00000200U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
                        & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_ch = 9U;
    }
    if ((0x00000400U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
                        & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_ch = 0x0aU;
    }
    if ((0x00000800U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
                        & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_ch = 0x0bU;
    }
    if ((0x00001000U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
                        & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_ch = 0x0cU;
    }
    if ((0x00002000U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
                        & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_ch = 0x0dU;
    }
    if ((0x00004000U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
                        & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_ch = 0x0eU;
    }
    if ((0x00008000U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
                        & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_any = 1U;
        vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__hi_ch = 0x0fU;
    }
}

void Vsys_top___024root___nba_sequent__TOP__11(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___nba_sequent__TOP__11\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__unnamedblk1__DOT__eaIsAreg;
    sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__unnamedblk1__DOT__eaIsAreg = 0;
    // Body
    if (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT3) 
         & (0U != vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
            .__PVT__dobCtrl))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__dob 
            = ((0xff00U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__dob)) 
               | (0x000000ffU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                 .__PVT__noLowByte ? 
                                 ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dobInput) 
                                  >> 8U) : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dobInput))));
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__dob 
            = ((0x00ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__dob)) 
               | (0x0000ff00U & ((((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__byteCycle) 
                                   | vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                   .__PVT__noHighByte)
                                   ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dobInput)
                                   : ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dobInput) 
                                      >> 8U)) << 8U)));
    }
    if (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT1) 
         & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
            .__PVT__au2Aob | (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                              .__PVT__au2Db & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                              .__PVT__db2Aob)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
            = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__auReg;
    } else if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT2) {
        if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
            .__PVT__db2Aob) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                = (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__preDbh) 
                    << 0x00000010U) | ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dblIdle)
                                        ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__preDbd)
                                        : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__preDbl)));
        } else if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                   .__PVT__ab2Aob) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                = (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__preAbh) 
                    << 0x00000010U) | ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ablIdle)
                                        ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__preAbd)
                                        : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__preAbl)));
        }
    }
    if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
        .__PVT__pwrUp) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microAddr = 2U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__auReg = 0U;
    } else {
        if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT1) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microAddr 
                = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nma;
        }
        if (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT3) 
             & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
             .__PVT__auClkEn)) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__auReg 
                = ((((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Dbh) 
                     + ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__auInpMux 
                         >> 0x00000010U) + (1U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aulow 
                                                  >> 0x00000010U)))) 
                    << 0x00000010U) | (0x0000ffffU 
                                       & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aulow));
        }
    }
    if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT4) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__byteCycle 
            = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
               .__PVT__busByte & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod
               .__PVT__isByte);
    }
    vlSelfRef.sys_top__DOT__sys__DOT__sel_pan = (IData)(
                                                        ((0x00500000U 
                                                          == 
                                                          (0x00ffff00U 
                                                           & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)) 
                                                         & (0x50U 
                                                            > 
                                                            (0x000000feU 
                                                             & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob))));
    vlSelfRef.sys_top__DOT__sys__DOT__sel_mfp = (IData)(
                                                        ((0x00900000U 
                                                          == 
                                                          (0x00ffff00U 
                                                           & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)) 
                                                         & (0x30U 
                                                            > 
                                                            (0x000000feU 
                                                             & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob))));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busAddrErr 
        = (1U & ((~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__bciByte)) 
                 & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__rx 
        = (7U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                 >> 9U));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__ry 
        = (7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__isPcRel 
        = (IData)(((0x003aU == (0x003eU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird))) 
                   & (~ (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                          >> 5U) & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__isRegShift)))));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__isTas 
        = (IData)((((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                    >> 4U) & (0x0ac0U == (0x0fc0U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird)))));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__rxIsDt 
        = (1U & (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                  >> 5U) | ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                            & (~ ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                  >> 8U)))));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__rxIsUsp 
        = (IData)((((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                    >> 4U) & (0x0e60U == (0x0ff0U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird)))));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__ryIsDt 
        = (IData)(((0x0038U == (0x003aU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird))) 
                   & (~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__isRegShift))));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__isMovep 
        = ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
           & (0x0108U == (0x0138U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird))));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__implicitSp 
        = ((0x00000040U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot))
            ? (1U == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                     >> 8U))) : ((1U 
                                                  & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                                                     >> 4U)) 
                                                 && ((0x0eU 
                                                      == 
                                                      (0x0000000fU 
                                                       & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                          >> 8U))) 
                                                     | (0x21U 
                                                        == 
                                                        (0x0000003fU 
                                                         & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                            >> 6U))))));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__toCcr 
        = ((((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
             >> 4U) & ((0x0e77U == (0x00000fffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird))) 
                       | (0x13U == (0x0000003fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                   >> 6U))))) 
           | ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
              & (0U == (0x01c0U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird)))));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__ftuConst 
        = ((0U != (3U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                         >> 6U))) ? ((0x0000ff00U & 
                                      ((- (IData)((1U 
                                                   & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                      >> 7U)))) 
                                       << 8U)) | (0x000000ffU 
                                                  & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird)))
            : ((IData)((0U != (0x4020U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot))))
                ? ((0U == (7U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                 >> 9U))) ? 8U : (7U 
                                                  & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                     >> 9U)))
                : ((IData)((0U != (0x1100U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot))))
                    ? 0x000fU : (0x0080U & (- (IData)(
                                                      (1U 
                                                       & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                                                          >> 4U))))))));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__macroTvn 
        = ((0x00000010U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot))
            ? ((0x00000040U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird))
                ? ((0x00000020U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird))
                    ? 7U : (0x00000020U | (0x0000000fU 
                                           & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird))))
                : 6U) : 5U);
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__inhibitCcr 
        = (((IData)(((0x00c0U == (0x00c0U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird))) 
                     & (0U != (0x2200U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot))))) 
            | (IData)((((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                        >> 5U) & (8U == (0x0038U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird)))))) 
           | (IData)(((0x0040U == (0x01c0U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird))) 
                      & (0U != (0x000cU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot))))));
    sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__unnamedblk1__DOT__eaIsAreg 
        = ((0U != (7U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                         >> 3U))) & (7U != (7U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                  >> 3U))));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__ryIsAreg 
        = ((0x00000020U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot))
            ? ((IData)(sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__unnamedblk1__DOT__eaIsAreg) 
               & (0x19U != (0x0000001fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                           >> 3U))))
            : ((1U & (~ ((1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                                >> 6U)) || (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                                                  >> 7U))))) 
               && (1U & ((0x00004000U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot))
                          ? (~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__isRegShift))
                          : (IData)(sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__unnamedblk1__DOT__eaIsAreg)))));
    if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT2) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Dbh 
            = (0x0000ffffU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                              .__PVT__extDbh ? (- (IData)(
                                                          (1U 
                                                           & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dblIdle)
                                                               ? 
                                                              ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__preDbd) 
                                                               >> 0x0fU)
                                                               : 
                                                              ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__preDbl) 
                                                               >> 0x0fU)))))
                               : ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dbhIdle)
                                   ? ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dblIdle)
                                       ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__preDbd)
                                       : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__preDbl))
                                   : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__preDbh))));
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Dbl 
            = ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dblIdle)
                ? (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                   .__PVT__dblDbh ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__preDbh)
                    : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__preDbd))
                : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__preDbl));
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Dbd 
            = ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dbdIdle)
                ? ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dblIdle)
                    ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__preDbh)
                    : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__preDbl))
                : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__preDbd));
    }
    if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT1) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__preAbd 
            = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__abdMux;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__preAbh 
            = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__abhMux;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__preAbl 
            = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ablMux;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__preDbh 
            = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dbhMux;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__preDbd 
            = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dbdMux;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__preDbl 
            = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dblMux;
    }
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__updTpend 
        = (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__ftuCtrl));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__clrTpend 
        = (0x0eU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__ftuCtrl));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__tvn2Ftu 
        = (0x0dU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__ftuCtrl));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__const2Ftu 
        = (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__ftuCtrl));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ftu2Dbl 
        = ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__ftuCtrl)) 
           | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__ftuCtrl)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ftu2Abl 
        = (8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__ftuCtrl));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__inl2psw 
        = (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__ftuCtrl));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__pswIToFtu 
        = (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__ftuCtrl));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ftu2Sr 
        = (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__ftuCtrl));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__sr2Ftu 
        = (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__ftuCtrl));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ird2Ftu 
        = (9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__ftuCtrl));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ssw2Ftu 
        = (0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__ftuCtrl));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__initST 
        = (((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__ftuCtrl)) 
            | (0x0eU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__ftuCtrl))) 
           | (0x0fU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__ftuCtrl)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__abl2Pren 
        = (0x0cU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__ftuCtrl));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__updPren 
        = (0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__ftuCtrl));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__Ir2Ird 
        = (1U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[2U] 
                 >> 3U));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__aluDctrl 
        = (3U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[1U] 
                 >> 0x00000013U));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__aluActrl 
        = (1U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[1U] 
                 >> 0x00000012U));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__aluColumn 
        = ((4U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[1U] 
                  >> 0x0000001dU)) | ((2U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[2U] 
                                             << 1U)) 
                                      | (1U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[2U] 
                                               >> 1U))));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__aluFinish 
        = (2U == (3U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[1U] 
                        >> 0x0000001dU)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__aluInit 
        = (1U == (3U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[1U] 
                        >> 0x0000001dU)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ftu2Ccr 
        = (3U == (3U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[1U] 
                        >> 0x0000001dU)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__abdIsByte 
        = (1U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[1U] 
                 >> 6U));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__au2Db 
        = (1U == (3U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[0U] 
                        >> 0x00000014U)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__au2Ab 
        = (2U == (3U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[0U] 
                        >> 0x00000014U)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__au2Pc 
        = (3U == (3U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[0U] 
                        >> 0x00000014U)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__db2Aob 
        = (2U == (3U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[0U] 
                        >> 3U)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ab2Aob 
        = (1U == (3U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[0U] 
                        >> 3U)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__au2Aob 
        = (3U == (3U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[0U] 
                        >> 3U)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dbin2Abd 
        = (1U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[1U] 
                 >> 0x0000000eU));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dbin2Dbd 
        = (1U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[1U] 
                 >> 0x0000000fU));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__permStart 
        = (0U != (3U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[0U] 
                        >> 3U)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__isWrite 
        = (0U != (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__dobCtrl));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__busByte 
        = (1U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[1U] 
                 >> 0x0000000aU));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__noLowByte 
        = (1U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[1U] 
                 >> 0x00000017U));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__noHighByte 
        = (1U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[1U] 
                 >> 0x00000016U));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__abl2reg 
        = (1U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[1U] 
                 >> 4U));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__abh2reg 
        = (1U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[0U] 
                 >> 7U));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dbl2reg 
        = (1U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[1U] 
                 >> 1U));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dbh2reg 
        = (1U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[0U] 
                 >> 5U));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__reg2dbl 
        = (1U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[1U]);
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__reg2dbh 
        = (1U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[0U] 
                 >> 6U));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__reg2abl 
        = (1U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[1U] 
                 >> 5U));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__reg2abh 
        = (1U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[0U] 
                 >> 8U));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ssp 
        = (1U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[0U] 
                 >> 0x00000018U));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__rz 
        = (1U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[1U] 
                 >> 0x0000000bU));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__rxlDbl 
        = (1U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[1U] 
                 >> 8U));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__pcldbl 
        = (1U & ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[1U] 
                  >> 7U) | ((~ (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[1U] 
                                >> 8U)) & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__isPcRel))));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__pchdbh 
        = ((1U == (3U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[0U])) 
           | ((~ (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[0U] 
                  >> 0x00000016U)) & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__isPcRel)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__pclabl 
        = (1U & ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[1U] 
                  >> 9U) | ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__isPcRel) 
                            & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[1U] 
                               >> 8U))));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__pchabh 
        = ((2U == (3U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[0U])) 
           | ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__isPcRel) 
              & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[0U] 
                 >> 0x00000016U)));
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v0) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v0 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__auClkEn 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v0;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v1) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v1 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__auCntrl 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v1;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v2) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v2 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__noSpAlign 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v2;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v3) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v3 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__extDbh 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v3;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v4) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v4 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__extAbh 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v4;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v5) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v5 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__todbin 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v5;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v6) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v6 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__toIrc 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v6;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v7) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v7 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ablAbd 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v7;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v8) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v8 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ablAbh 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v8;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v9) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v9 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dblDbd 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v9;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v10) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v10 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dblDbh 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v10;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v11) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v11 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dbl2Atl 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v11;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v12) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v12 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__atl2Dbl 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v12;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v13) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v13 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__abl2Atl 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v13;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v14) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v14 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__atl2Abl 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v14;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v15) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v15 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__aob2Ab 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v15;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v16) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v16 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__abh2Ath 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v16;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v17) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v17 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dbh2Ath 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v17;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v18) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v18 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ath2Dbh 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v18;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v19) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v19 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ath2Abh 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v19;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v20) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v20 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__alu2Dbd 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v20;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v21) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v21 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__alu2Abd 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v21;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v22) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v22 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__abd2Dcr 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v22;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v23) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v23 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dcr2Dbd 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v23;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v24) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v24 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dbd2Alue 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v24;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v25) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v25 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__alue2Dbd 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v25;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v26) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v26 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dbd2Alub 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v26;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v27) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v27 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__abd2Alub 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v27;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v28) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v28 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dobCtrl 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v28;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v29) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v29 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__rxl2db 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v29;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v30) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v30 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__rxl2ab 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v30;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v31) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v31 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dbl2rxl 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v31;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v32) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v32 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__abl2rxl 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v32;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v33) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v33 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__rxh2dbh 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v33;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v34) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v34 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__rxh2abh 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v34;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v35) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v35 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dbh2rxh 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v35;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v36) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v36 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__abh2rxh 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v36;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v37) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v37 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dbh2ryh 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v37;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v38) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v38 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__abh2ryh 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v38;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v39) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v39 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dbl2ryl 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v39;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v40) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v40 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__abl2ryl 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v40;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v41) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v41 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ryl2db 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v41;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v42) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v42 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ryl2ab 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v42;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v43) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v43 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ryh2dbh 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v43;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v44) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v44 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ryh2abh 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v44;
    }
    if (vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v45) {
        vlSelfRef.__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v45 = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__isRmc 
            = vlSelfRef.__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v45;
    }
}

void Vsys_top___024root___nba_comb__TOP__2(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___nba_comb__TOP__2\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.sys_top__DOT__sys__DOT__mfp_dout = (0x000000ffU 
                                                  & ((0x00000020U 
                                                      & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                      ? 
                                                     ((0x00000010U 
                                                       & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                       ? 0xffU
                                                       : 
                                                      ((8U 
                                                        & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                        ? 
                                                       ((4U 
                                                         & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                         ? 
                                                        ((2U 
                                                          & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                          ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__rx_data)
                                                          : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tsr))
                                                         : 
                                                        ((2U 
                                                          & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                          ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__rsr)
                                                          : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ucr)))
                                                        : 
                                                       ((4U 
                                                         & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                         ? 
                                                        ((2U 
                                                          & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                          ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__scr)
                                                          : vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tmc[3U])
                                                         : 
                                                        ((2U 
                                                          & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                          ? vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tmc[2U]
                                                          : vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tmc[1U]))))
                                                      : 
                                                     ((0x00000010U 
                                                       & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                       ? 
                                                      ((8U 
                                                        & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                        ? 
                                                       ((4U 
                                                         & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                         ? 
                                                        ((2U 
                                                          & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                          ? vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tmc[0U]
                                                          : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tcdcr))
                                                         : 
                                                        ((2U 
                                                          & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                          ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tbcr)
                                                          : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tacr)))
                                                        : 
                                                       ((4U 
                                                         & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                         ? 
                                                        ((2U 
                                                          & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                          ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__vr)
                                                          : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr))
                                                         : 
                                                        ((2U 
                                                          & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                          ? 
                                                         ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr) 
                                                          >> 8U)
                                                          : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__isr))))
                                                       : 
                                                      ((8U 
                                                        & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                        ? 
                                                       ((4U 
                                                         & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                         ? 
                                                        ((2U 
                                                          & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                          ? 
                                                         ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__isr) 
                                                          >> 8U)
                                                          : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr))
                                                         : 
                                                        ((2U 
                                                          & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                          ? 
                                                         ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
                                                          >> 8U)
                                                          : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ier)))
                                                        : 
                                                       ((4U 
                                                         & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                         ? 
                                                        ((2U 
                                                          & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                          ? 
                                                         ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ier) 
                                                          >> 8U)
                                                          : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ddr))
                                                         : 
                                                        ((2U 
                                                          & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                          ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__aer)
                                                          : 
                                                         ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__gpip) 
                                                          & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ddr))))))));
}

void Vsys_top___024root___nba_sequent__TOP__12(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___nba_sequent__TOP__12\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks.__PVT__enPhi1 
        = vlSelfRef.sys_top__DOT__sys__DOT__phi1;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks.__PVT__enPhi2 
        = vlSelfRef.sys_top__DOT__sys__DOT__phi2;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT____Vcellinp__nanoRom__clk 
        = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
        .__PVT__clk;
}

extern const VlUnpacked<CData/*6:0*/, 256> Vsys_top__ConstPool__TABLE_h1ee1637f_0;

void Vsys_top___024root___nba_comb__TOP__4(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___nba_comb__TOP__4\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*7:0*/ __Vtableidx1;
    __Vtableidx1 = 0;
    // Body
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__m01 
        = ((8U & ((~ (0U != (0x0000003fU & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__auReg))) 
                  << 3U)) | ((4U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                    >> 6U)) | (3U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__alue))));
    if (((((((((0U == (0x0000001fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                      >> 2U))) | (1U 
                                                  == 
                                                  (0x0000001fU 
                                                   & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                      >> 2U)))) 
              | (0x11U == (0x0000001fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                          >> 2U)))) 
             | (2U == (0x0000001fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                      >> 2U)))) | (0x12U 
                                                   == 
                                                   (0x0000001fU 
                                                    & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                       >> 2U)))) 
           | (3U == (0x0000001fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                    >> 2U)))) | (4U 
                                                 == 
                                                 (0x0000001fU 
                                                  & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                     >> 2U)))) 
         | (5U == (0x0000001fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                  >> 2U))))) {
        if ((0U == (0x0000001fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                   >> 2U)))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__c0c1 
                = (3U & (- (IData)((1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irc) 
                                          >> 0x0000000bU)))));
        } else if ((1U == (0x0000001fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                          >> 2U)))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__c0c1 
                = ((0U != (0x0000003fU & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__auReg))
                    ? 3U : 1U);
        } else if ((0x11U == (0x0000001fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                             >> 2U)))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__c0c1 
                = ((0U != (0x0000003fU & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__auReg))
                    ? 3U : 0U);
        } else if ((2U == (0x0000001fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                          >> 2U)))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__c0c1 
                = (1U & (~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw)));
        } else if ((0x12U == (0x0000001fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                             >> 2U)))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__c0c1 
                = (2U | (1U & (~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw))));
        } else if ((3U == (0x0000001fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                          >> 2U)))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__c0c1 
                = ((2U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw) 
                          >> 1U)) | (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw) 
                                           >> 2U)));
        } else if ((4U == (0x0000001fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                          >> 2U)))) {
            if ((0U == (3U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__pswCcr) 
                              >> 2U)))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__c0c1 = 2U;
            } else if ((2U == (3U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__pswCcr) 
                                     >> 2U)))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__c0c1 = 1U;
            } else if (((1U == (3U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__pswCcr) 
                                      >> 2U))) || (3U 
                                                   == 
                                                   (3U 
                                                    & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__pswCcr) 
                                                       >> 2U))))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__c0c1 = 3U;
            }
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__c0c1 
                = (1U | (2U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw) 
                               >> 2U)));
        }
    } else if (((((((((0x15U == (0x0000001fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                >> 2U))) 
                      | (6U == (0x0000001fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                               >> 2U)))) 
                     | (7U == (0x0000001fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                              >> 2U)))) 
                    | (8U == (0x0000001fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                             >> 2U)))) 
                   | (9U == (0x0000001fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                            >> 2U)))) 
                  | (0x19U == (0x0000001fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                              >> 2U)))) 
                 | (0x0cU == (0x0000001fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                             >> 2U)))) 
                | (0x1cU == (0x0000001fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                            >> 2U))))) {
        if ((0x15U == (0x0000001fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                      >> 2U)))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__c0c1 
                = (2U | (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw) 
                               >> 3U)));
        } else if ((6U == (0x0000001fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                          >> 2U)))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__c0c1 
                = (1U | ((IData)((0U == (0x0cU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__pswCcr)))) 
                         << 1U));
        } else if ((7U == (0x0000001fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                          >> 2U)))) {
            if (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__ms0)) 
                 || (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__ms0)))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__c0c1 = 3U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__ms0))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__c0c1 = 1U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__ms0))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__c0c1 = 2U;
            }
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__c0c1 
                = ((8U == (0x0000001fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                          >> 2U))) ? 
                   (((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__m01)) 
                       || (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__m01))) 
                      || (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__m01))) 
                     || (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__m01)))
                     ? 3U : ((((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__m01)) 
                               || (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__m01))) 
                              || (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__m01)))
                              ? 1U : ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__m01))
                                       ? 2U : 0U)))
                    : ((9U == (0x0000001fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                              >> 2U)))
                        ? ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__ccTest)
                            ? 3U : 1U) : ((0x19U == 
                                           (0x0000001fU 
                                            & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                               >> 2U)))
                                           ? ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__ccTest)
                                               ? 3U
                                               : 2U)
                                           : ((0x0cU 
                                               == (0x0000001fU 
                                                   & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                      >> 2U)))
                                               ? ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__dcr4)
                                                   ? 1U
                                                   : 3U)
                                               : ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__dcr4)
                                                   ? 2U
                                                   : 3U)))));
        }
    } else {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__c0c1 
            = ((0x0aU == (0x0000001fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                         >> 2U))) ? 
               ((4U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrCore))
                 ? 0U : 3U) : ((0x0bU == (0x0000001fU 
                                          & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                             >> 2U)))
                                ? ((0U == ((2U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__pswCcr) 
                                                  >> 2U)) 
                                           | (1U & 
                                              ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__pswCcr) 
                                               >> 1U))))
                                    ? 0U : 3U) : ((0x0dU 
                                                   == 
                                                   (0x0000001fU 
                                                    & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                       >> 2U)))
                                                   ? 
                                                  ((2U 
                                                    & ((~ 
                                                        ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw) 
                                                         >> 1U)) 
                                                       << 1U)) 
                                                   | (1U 
                                                      & (~ 
                                                         ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw) 
                                                          >> 1U))))
                                                   : 
                                                  (((0x0eU 
                                                     == 
                                                     (0x0000001fU 
                                                      & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                         >> 2U))) 
                                                    || (0x1eU 
                                                        == 
                                                        (0x0000001fU 
                                                         & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                            >> 2U))))
                                                    ? 
                                                   ((2U 
                                                     & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__enl))
                                                     ? 
                                                    ((1U 
                                                      & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__enl))
                                                      ? 
                                                     (1U 
                                                      & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                         >> 6U))
                                                      : 3U)
                                                     : 
                                                    ((1U 
                                                      & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__enl))
                                                      ? 
                                                     (1U 
                                                      & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                                         >> 6U))
                                                      : 2U))
                                                    : 0U))));
    }
    if ((2U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch)) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__uNma 
            = ((((0x0000000cU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                 >> 0x0000000bU)) | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__c0c1)) 
                << 6U) | ((0x0000003cU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                          >> 5U)) | 
                          (3U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                 >> 0x0bU))));
    } else if ((0U == (3U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                             >> 2U)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__uNma 
            = ((((0x0000000cU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                 >> 0x0000000bU)) | 
                 (3U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                        >> 5U))) << 6U) | ((0x0000003cU 
                                            & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                               >> 5U)) 
                                           | (3U & 
                                              (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                                               >> 0x0000000bU))));
    } else if ((1U == (3U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                             >> 2U)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__uNma 
            = (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rIllegal) 
                | ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rLineF) 
                   | ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rLineA) 
                      | ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rPriv) 
                         | ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rInterrupt) 
                            | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rTrace))))))
                ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__grp1Nma)
                : vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1
               [(0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                >> 0x0000000cU))]);
    } else if ((2U == (3U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                             >> 2U)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__uNma 
            = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23
            [(0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                             >> 0x0000000cU))];
    } else if ((3U == (3U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
                             >> 2U)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__uNma 
            = ((1U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__lineBmap))
                ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3)
                : vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23
               [(0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                >> 0x0000000cU))]);
    }
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nma 
        = ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__A0Err)
            ? ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__a0Rst)
                ? 2U : (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rExcRst) 
                         | ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rExcBusErr) 
                            | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rExcAdrErr)))
                         ? 1U : 3U)) : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__uNma));
    __Vtableidx1 = (0x000000ffU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nma) 
                                   >> 2U));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microToNanoAddr__DOT__orgBase 
        = Vsys_top__ConstPool__TABLE_h1ee1637f_0[__Vtableidx1];
}

void Vsys_top___024root___nba_comb__TOP__5(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___nba_comb__TOP__5\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryl2Abl;
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryl2Abl = 0;
    CData/*0:0*/ sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryl2Dbl;
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryl2Dbl = 0;
    CData/*0:0*/ sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxl2Abl;
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxl2Abl = 0;
    CData/*0:0*/ sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxl2Dbl;
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxl2Dbl = 0;
    // Body
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__waitBusFinish 
        = (1U & (((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[2U] 
                   >> 2U) | (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[1U] 
                             >> 0x0000001cU)) | vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                 .__PVT__isWrite));
    if ((1U & (~ VL_ONEHOT_I((((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                .__PVT__dobCtrl) << 2U) 
                              | (((1U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                   .__PVT__dobCtrl) 
                                  << 1U) | (2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                            .__PVT__dobCtrl))))))) {
        if ((0U != (((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                      .__PVT__dobCtrl) << 2U) | (((1U 
                                                   == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                   .__PVT__dobCtrl) 
                                                  << 1U) 
                                                 | (2U 
                                                    == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                    .__PVT__dobCtrl))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: fx68k.sv:1671: Assertion failed in %m: unique case, but multiple matches found for '2'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1)
                             , '#',2,vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                             .__PVT__dobCtrl);
                VL_STOP_MT("../third_party/fx68k/fx68k.sv", 1671, "");
            }
        }
    }
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__dblSpecial 
        = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
        .__PVT__pcldbl;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__dbhSpecial 
        = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
        .__PVT__pchdbh;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__ablSpecial 
        = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
        .__PVT__pclabl;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__abhSpecial 
        = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
        .__PVT__pchabh;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__abhIdle = 0U;
    if ((1U & (~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Pch2Abh)))) {
        if ((1U & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                   .__PVT__rxh2abh))) {
            if ((1U & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                       .__PVT__ryh2abh))) {
                if ((1U & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                           .__PVT__au2Ab))) {
                    if ((1U & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                               .__PVT__aob2Ab))) {
                        if ((1U & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                   .__PVT__ath2Abh))) {
                            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__abhIdle = 1U;
                        }
                    }
                }
            }
        }
    }
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dbhIdle = 0U;
    if ((1U & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
               .__PVT__rxh2dbh))) {
        if ((1U & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                   .__PVT__ryh2dbh))) {
            if ((1U & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                       .__PVT__au2Db))) {
                if ((1U & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                           .__PVT__ath2Dbh))) {
                    if ((1U & (~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Pch2Dbh)))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dbhIdle = 1U;
                    }
                }
            }
        }
    }
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__auInpMux 
        = ((4U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
            .__PVT__auCntrl) ? ((2U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                 .__PVT__auCntrl) ? 
                                ((1U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                  .__PVT__auCntrl) ? 
                                 (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__byteNotSpAlign) 
                                   | vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                   .__PVT__noSpAlign)
                                   ? 0xffffffffU : 0xfffffffeU)
                                  : 0xfffffffeU) : 
                                ((1U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                  .__PVT__auCntrl) ? 4U
                                  : 2U)) : ((2U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                             .__PVT__auCntrl)
                                             ? ((1U 
                                                 & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                 .__PVT__auCntrl)
                                                 ? 
                                                (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Abh) 
                                                  << 0x00000010U) 
                                                 | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Abl))
                                                 : 0xfffffffcU)
                                             : ((1U 
                                                 & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                 .__PVT__auCntrl)
                                                 ? 
                                                (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__byteNotSpAlign) 
                                                  | vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                  .__PVT__noSpAlign)
                                                  ? 1U
                                                  : 2U)
                                                 : 0U)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__abhMux 
        = (0x0000ffffU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Pch2Abh)
                           ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__PcH)
                           : (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                              .__PVT__rxh2abh ? ((0x11U 
                                                  >= (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__actualRx))
                                                  ? vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68H
                                                 [vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__actualRx]
                                                  : 0U)
                               : (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                  .__PVT__ryh2abh ? 
                                  ((0x11U >= (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__actualRy))
                                    ? vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68H
                                   [vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__actualRy]
                                    : 0U) : (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                             .__PVT__au2Ab
                                              ? (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__auReg 
                                                 >> 0x10U)
                                              : (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                 .__PVT__aob2Ab
                                                  ? 
                                                 (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob 
                                                  >> 0x10U)
                                                  : 
                                                 (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                  .__PVT__ath2Abh
                                                   ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Ath)
                                                   : 0U)))))));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryl2Abd 
        = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
           .__PVT__ryl2ab & ((~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryIsAreg)) 
                             | vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                             .__PVT__ablAbd));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxl2Abd 
        = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
           .__PVT__rxl2ab & ((~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxIsAreg)) 
                             | vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                             .__PVT__ablAbd));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryl2Dbd 
        = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
           .__PVT__ryl2db & ((~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryIsAreg)) 
                             | vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                             .__PVT__dblDbd));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxl2Dbd 
        = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
           .__PVT__rxl2db & ((~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxIsAreg)) 
                             | vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                             .__PVT__dblDbd));
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryl2Dbl 
        = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
           .__PVT__ryl2db & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryIsAreg) 
                             | vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                             .__PVT__dblDbd));
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxl2Dbl 
        = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
           .__PVT__rxl2db & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxIsAreg) 
                             | vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                             .__PVT__dblDbd));
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryl2Abl 
        = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
           .__PVT__ryl2ab & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryIsAreg) 
                             | vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                             .__PVT__ablAbd));
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxl2Abl 
        = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
           .__PVT__rxl2ab & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxIsAreg) 
                             | vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                             .__PVT__ablAbd));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__dOperand 
        = (0x0000ffffU & ((2U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                           .__PVT__aluDctrl) ? (- (IData)(
                                                          (1U 
                                                           & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                           .__PVT__aluDctrl)))
                           : ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Dbd) 
                              & (- (IData)((1U & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                  .__PVT__aluDctrl)))))));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aOperand 
        = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
           .__PVT__aluActrl ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alub)
            : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Abd));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dbhMux 
        = (0x0000ffffU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                          .__PVT__rxh2dbh ? ((0x11U 
                                              >= (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__actualRx))
                                              ? vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68H
                                             [vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__actualRx]
                                              : 0U)
                           : (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                              .__PVT__ryh2dbh ? ((0x11U 
                                                  >= (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__actualRy))
                                                  ? vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68H
                                                 [vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__actualRy]
                                                  : 0U)
                               : (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                  .__PVT__au2Db ? (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__auReg 
                                                   >> 0x10U)
                                   : (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                      .__PVT__ath2Dbh
                                       ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Ath)
                                       : ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Pch2Dbh)
                                           ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__PcH)
                                           : 0U))))));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aulow 
        = (0x0001ffffU & ((0x0000ffffU & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__auInpMux) 
                          + (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Dbl)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__abdIdle = 0U;
    if ((1U & (~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryl2Abd)))) {
        if ((1U & (~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxl2Abd)))) {
            if ((1U & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                       .__PVT__dbin2Abd))) {
                if ((1U & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                           .__PVT__alu2Abd))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__abdIdle = 1U;
                }
            }
        }
    }
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dbdIdle = 0U;
    if ((1U & (~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryl2Dbd)))) {
        if ((1U & (~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxl2Dbd)))) {
            if ((1U & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                       .__PVT__alue2Dbd))) {
                if ((1U & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                           .__PVT__dbin2Dbd))) {
                    if ((1U & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                               .__PVT__alu2Dbd))) {
                        if ((1U & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                   .__PVT__dcr2Dbd))) {
                            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dbdIdle = 1U;
                        }
                    }
                }
            }
        }
    }
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dblIdle = 0U;
    if ((1U & (~ (IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxl2Dbl)))) {
        if ((1U & (~ (IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryl2Dbl)))) {
            if ((1U & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                       .__PVT__ftu2Dbl))) {
                if ((1U & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                           .__PVT__au2Db))) {
                    if ((1U & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                               .__PVT__atl2Dbl))) {
                        if ((1U & (~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Pcl2Dbl)))) {
                            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dblIdle = 1U;
                        }
                    }
                }
            }
        }
    }
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dblMux 
        = (0x0000ffffU & ((IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxl2Dbl)
                           ? ((0x11U >= (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__actualRx))
                               ? vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68L
                              [vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__actualRx]
                               : 0U) : ((IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryl2Dbl)
                                         ? ((0x11U 
                                             >= (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__actualRy))
                                             ? vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68L
                                            [vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__actualRy]
                                             : 0U) : 
                                        (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                         .__PVT__ftu2Dbl
                                          ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__ftu)
                                          : (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                             .__PVT__au2Db
                                              ? vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__auReg
                                              : (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                 .__PVT__atl2Dbl
                                                  ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Atl)
                                                  : 
                                                 ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Pcl2Dbl)
                                                   ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__PcL)
                                                   : 0U)))))));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ablIdle = 0U;
    if ((1U & (~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Pcl2Abl)))) {
        if ((1U & (~ (IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxl2Abl)))) {
            if ((1U & (~ (IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryl2Abl)))) {
                if ((1U & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                           .__PVT__ftu2Abl))) {
                    if ((1U & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                               .__PVT__au2Ab))) {
                        if ((1U & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                   .__PVT__aob2Ab))) {
                            if ((1U & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                       .__PVT__atl2Abl))) {
                                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ablIdle = 1U;
                            }
                        }
                    }
                }
            }
        }
    }
    if ((1U & (~ VL_ONEHOT_I((((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                .__PVT__dcr2Dbd << 5U) 
                               | ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                   .__PVT__alu2Dbd 
                                   << 4U) | (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                             .__PVT__dbin2Dbd 
                                             << 3U))) 
                              | ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                  .__PVT__alue2Dbd 
                                  << 2U) | (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxl2Dbd) 
                                             << 1U) 
                                            | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryl2Dbd)))))))) {
        if ((0U != (((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                      .__PVT__dcr2Dbd << 5U) | ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                 .__PVT__alu2Dbd 
                                                 << 4U) 
                                                | (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                   .__PVT__dbin2Dbd 
                                                   << 3U))) 
                    | ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                        .__PVT__alue2Dbd << 2U) | (
                                                   ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxl2Dbd) 
                                                    << 1U) 
                                                   | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryl2Dbd)))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: fx68k.sv:1333: Assertion failed in %m: unique case, but multiple matches found for '1'h1'\n",3, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1));
                VL_STOP_MT("../third_party/fx68k/fx68k.sv", 1333, "");
            }
        }
    }
    if ((1U & (~ VL_ONEHOT_I(((((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Pcl2Dbl) 
                                << 5U) | ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                           .__PVT__atl2Dbl 
                                           << 4U) | 
                                          (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                           .__PVT__au2Db 
                                           << 3U))) 
                              | ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                  .__PVT__ftu2Dbl << 2U) 
                                 | (((IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryl2Dbl) 
                                     << 1U) | (IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxl2Dbl)))))))) {
        if ((0U != ((((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Pcl2Dbl) 
                      << 5U) | ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                 .__PVT__atl2Dbl << 4U) 
                                | (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                   .__PVT__au2Db << 3U))) 
                    | ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                        .__PVT__ftu2Dbl << 2U) | (((IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryl2Dbl) 
                                                   << 1U) 
                                                  | (IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxl2Dbl)))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: fx68k.sv:1343: Assertion failed in %m: unique case, but multiple matches found for '1'h1'\n",3, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1));
                VL_STOP_MT("../third_party/fx68k/fx68k.sv", 1343, "");
            }
        }
    }
    if ((1U & (~ VL_ONEHOT_I(((((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Pch2Dbh) 
                                << 4U) | ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                           .__PVT__ath2Dbh 
                                           << 3U) | 
                                          (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                           .__PVT__au2Db 
                                           << 2U))) 
                              | ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                  .__PVT__ryh2dbh << 1U) 
                                 | vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                 .__PVT__rxh2dbh)))))) {
        if ((0U != ((((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Pch2Dbh) 
                      << 4U) | ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                 .__PVT__ath2Dbh << 3U) 
                                | (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                   .__PVT__au2Db << 2U))) 
                    | ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                        .__PVT__ryh2dbh << 1U) | vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                       .__PVT__rxh2dbh)))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: fx68k.sv:1353: Assertion failed in %m: unique case, but multiple matches found for '1'h1'\n",3, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1));
                VL_STOP_MT("../third_party/fx68k/fx68k.sv", 1353, "");
            }
        }
    }
    if ((1U & (~ VL_ONEHOT_I((((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                .__PVT__alu2Abd << 3U) 
                               | (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                  .__PVT__dbin2Abd 
                                  << 2U)) | (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxl2Abd) 
                                              << 1U) 
                                             | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryl2Abd))))))) {
        if ((0U != (((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                      .__PVT__alu2Abd << 3U) | (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                .__PVT__dbin2Abd 
                                                << 2U)) 
                    | (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxl2Abd) 
                        << 1U) | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryl2Abd))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: fx68k.sv:1362: Assertion failed in %m: unique case, but multiple matches found for '1'h1'\n",3, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1));
                VL_STOP_MT("../third_party/fx68k/fx68k.sv", 1362, "");
            }
        }
    }
    if ((1U & (~ VL_ONEHOT_I((((((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                  .__PVT__atl2Abl << 3U) 
                                 | (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                    .__PVT__aob2Ab 
                                    << 2U)) | ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                .__PVT__au2Ab 
                                                << 1U) 
                                               | vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                               .__PVT__ftu2Abl)) 
                               << 3U) | (((IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryl2Abl) 
                                          << 2U) | 
                                         (((IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxl2Abl) 
                                           << 1U) | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Pcl2Abl)))))))) {
        if ((0U != (((((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                        .__PVT__atl2Abl << 3U) | (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                  .__PVT__aob2Ab 
                                                  << 2U)) 
                      | ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                          .__PVT__au2Ab << 1U) | vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                         .__PVT__ftu2Abl)) << 3U) | 
                    (((IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryl2Abl) 
                      << 2U) | (((IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxl2Abl) 
                                 << 1U) | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Pcl2Abl)))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: fx68k.sv:1370: Assertion failed in %m: unique case, but multiple matches found for '1'h1'\n",3, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1));
                VL_STOP_MT("../third_party/fx68k/fx68k.sv", 1370, "");
            }
        }
    }
    if ((1U & (~ VL_ONEHOT_I((((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                .__PVT__ath2Abh << 5U) 
                               | ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                   .__PVT__aob2Ab << 4U) 
                                  | (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                     .__PVT__au2Ab 
                                     << 3U))) | ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                  .__PVT__ryh2abh 
                                                  << 2U) 
                                                 | ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                     .__PVT__rxh2abh 
                                                     << 1U) 
                                                    | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Pch2Abh)))))))) {
        if ((0U != (((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                      .__PVT__ath2Abh << 5U) | ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                 .__PVT__aob2Ab 
                                                 << 4U) 
                                                | (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                   .__PVT__au2Ab 
                                                   << 3U))) 
                    | ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                        .__PVT__ryh2abh << 2U) | ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                   .__PVT__rxh2abh 
                                                   << 1U) 
                                                  | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Pch2Abh)))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: fx68k.sv:1381: Assertion failed in %m: unique case, but multiple matches found for '1'h1'\n",3, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1));
                VL_STOP_MT("../third_party/fx68k/fx68k.sv", 1381, "");
            }
        }
    }
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ablMux 
        = (0x0000ffffU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Pcl2Abl)
                           ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__PcL)
                           : ((IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxl2Abl)
                               ? ((0x11U >= (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__actualRx))
                                   ? vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68L
                                  [vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__actualRx]
                                   : 0U) : ((IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryl2Abl)
                                             ? ((0x11U 
                                                 >= (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__actualRy))
                                                 ? vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68L
                                                [vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__actualRy]
                                                 : 0U)
                                             : (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                .__PVT__ftu2Abl
                                                 ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__ftu)
                                                 : 
                                                (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                 .__PVT__au2Ab
                                                  ? vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__auReg
                                                  : 
                                                 (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                  .__PVT__aob2Ab
                                                   ? vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob
                                                   : 
                                                  (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                   .__PVT__atl2Abl
                                                    ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Atl)
                                                    : 0U))))))));
}

void Vsys_top___024root___nba_comb__TOP__6(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___nba_comb__TOP__6\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT3 
        = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
           .__PVT__enPhi1 & (2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__tState));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT4 
        = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
           .__PVT__enPhi2 & ((0U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__tState) 
                             | (3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__tState)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__rstUrom 
        = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
           .__PVT__enPhi1 & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enErrClk));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__bcReset 
        = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
           .__PVT__extReset | (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__addrOeDelay) 
                                & (~ ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__BeI) 
                                      | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__BeiDelay)))) 
                               & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Vpai)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT2 
        = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
           .__PVT__enPhi2 & (1U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__tState));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT1 
        = ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
            .__PVT__enPhi1 & (4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__tState)) 
           & (~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__wClk)));
}

void Vsys_top___024root___nba_comb__TOP__7(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___nba_comb__TOP__7\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    SData/*15:0*/ sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__subResult;
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__subResult = 0;
    CData/*0:0*/ sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__subCout;
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__subCout = 0;
    CData/*0:0*/ sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__subOv;
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__subOv = 0;
    CData/*0:0*/ sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shftCin;
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shftCin = 0;
    CData/*0:0*/ sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shftMsb;
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shftMsb = 0;
    CData/*0:0*/ sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__rm;
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__rm = 0;
    CData/*0:0*/ sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__sm;
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__sm = 0;
    CData/*0:0*/ sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__dm;
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__dm = 0;
    CData/*0:0*/ sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__tsm;
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__tsm = 0;
    IData/*31:0*/ sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shifter__DOT__tdata;
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shifter__DOT__tdata = 0;
    SData/*15:0*/ __Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__inpa;
    __Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__inpa = 0;
    SData/*15:0*/ __Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__inpb;
    __Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__inpb = 0;
    CData/*0:0*/ __Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__cin;
    __Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__cin = 0;
    CData/*0:0*/ __Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__bAdd;
    __Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__bAdd = 0;
    CData/*0:0*/ __Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__isByte;
    __Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__isByte = 0;
    CData/*0:0*/ __Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__ov;
    __Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__ov = 0;
    // Body
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 0U;
    if ((1U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
         .__PVT__aluColumn)) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 1U;
    } else if ((5U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                .__PVT__aluColumn)) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 5U;
    } else {
        if ((1U & ((0U != (0x039eU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row))) 
                   | ((1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row) 
                             >> 5U)) || (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row) 
                                               >> 6U)))))) {
            if ((2U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row))) {
                if ((2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                     .__PVT__aluColumn)) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 2U;
                } else if ((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                            .__PVT__aluColumn)) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 0x0aU;
                } else if (((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                             .__PVT__aluColumn) || 
                            (6U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                             .__PVT__aluColumn))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 0x15U;
                }
                if ((1U & (~ VL_ONEHOT_I(((((6U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                             .__PVT__aluColumn) 
                                            | (4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                               .__PVT__aluColumn)) 
                                           << 2U) | 
                                          (((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                             .__PVT__aluColumn) 
                                            << 1U) 
                                           | (2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                              .__PVT__aluColumn))))))) {
                    if ((0U == ((((6U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                   .__PVT__aluColumn) 
                                  | (4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                     .__PVT__aluColumn)) 
                                 << 2U) | (((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                             .__PVT__aluColumn) 
                                            << 1U) 
                                           | (2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                              .__PVT__aluColumn))))) {
                        if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                            VL_WRITEF_NX("[%0t] %%Error: fx68kAlu.sv:539: Assertion failed in %m: unique case, but none matched for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit.alu.aluGetOp", 'T',-9
                                         , '#',64,VL_TIME_UNITED_Q(1)
                                         , '#',32,vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                         .__PVT__aluColumn);
                            VL_STOP_MT("../third_party/fx68k/fx68kAlu.sv", 539, "");
                        }
                    } else if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: fx68kAlu.sv:539: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit.alu.aluGetOp", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                     .__PVT__aluColumn);
                        VL_STOP_MT("../third_party/fx68k/fx68kAlu.sv", 539, "");
                    }
                }
            } else if ((4U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row))) {
                if ((2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                     .__PVT__aluColumn)) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 4U;
                } else if ((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                            .__PVT__aluColumn)) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 0x0bU;
                } else if ((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                            .__PVT__aluColumn)) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 0x0eU;
                }
                if ((1U & (~ VL_ONEHOT_I((((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                            .__PVT__aluColumn) 
                                           << 2U) | 
                                          (((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                             .__PVT__aluColumn) 
                                            << 1U) 
                                           | (2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                              .__PVT__aluColumn))))))) {
                    if ((0U == (((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                  .__PVT__aluColumn) 
                                 << 2U) | (((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                             .__PVT__aluColumn) 
                                            << 1U) 
                                           | (2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                              .__PVT__aluColumn))))) {
                        if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                            VL_WRITEF_NX("[%0t] %%Error: fx68kAlu.sv:546: Assertion failed in %m: unique case, but none matched for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit.alu.aluGetOp", 'T',-9
                                         , '#',64,VL_TIME_UNITED_Q(1)
                                         , '#',32,vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                         .__PVT__aluColumn);
                            VL_STOP_MT("../third_party/fx68k/fx68kAlu.sv", 546, "");
                        }
                    } else if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: fx68kAlu.sv:546: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit.alu.aluGetOp", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                     .__PVT__aluColumn);
                        VL_STOP_MT("../third_party/fx68k/fx68kAlu.sv", 546, "");
                    }
                }
            } else if ((8U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row))) {
                if ((2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                     .__PVT__aluColumn)) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 0x0cU;
                } else if ((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                            .__PVT__aluColumn)) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp 
                        = ((2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                            .__PVT__aluDctrl) ? 0x00000016U
                            : 4U);
                } else if ((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                            .__PVT__aluColumn)) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 0x0dU;
                }
                if ((1U & (~ VL_ONEHOT_I((((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                            .__PVT__aluColumn) 
                                           << 2U) | 
                                          (((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                             .__PVT__aluColumn) 
                                            << 1U) 
                                           | (2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                              .__PVT__aluColumn))))))) {
                    if ((0U == (((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                  .__PVT__aluColumn) 
                                 << 2U) | (((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                             .__PVT__aluColumn) 
                                            << 1U) 
                                           | (2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                              .__PVT__aluColumn))))) {
                        if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                            VL_WRITEF_NX("[%0t] %%Error: fx68kAlu.sv:553: Assertion failed in %m: unique case, but none matched for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit.alu.aluGetOp", 'T',-9
                                         , '#',64,VL_TIME_UNITED_Q(1)
                                         , '#',32,vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                         .__PVT__aluColumn);
                            VL_STOP_MT("../third_party/fx68k/fx68kAlu.sv", 553, "");
                        }
                    } else if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: fx68kAlu.sv:553: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit.alu.aluGetOp", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                     .__PVT__aluColumn);
                        VL_STOP_MT("../third_party/fx68k/fx68kAlu.sv", 553, "");
                    }
                }
            } else if ((0x00000010U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp 
                    = ((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                        .__PVT__aluColumn) ? 0x0000000fU
                        : 1U);
            } else if (((1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row) 
                               >> 5U)) || (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row) 
                                                 >> 6U)))) {
                if ((2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                     .__PVT__aluColumn)) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 2U;
                } else if ((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                            .__PVT__aluColumn)) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 0x0aU;
                } else if ((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                            .__PVT__aluColumn)) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 0x10U;
                }
                if ((1U & (~ VL_ONEHOT_I((((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                            .__PVT__aluColumn) 
                                           << 2U) | 
                                          (((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                             .__PVT__aluColumn) 
                                            << 1U) 
                                           | (2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                              .__PVT__aluColumn))))))) {
                    if ((0U == (((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                  .__PVT__aluColumn) 
                                 << 2U) | (((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                             .__PVT__aluColumn) 
                                            << 1U) 
                                           | (2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                              .__PVT__aluColumn))))) {
                        if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                            VL_WRITEF_NX("[%0t] %%Error: fx68kAlu.sv:564: Assertion failed in %m: unique case, but none matched for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit.alu.aluGetOp", 'T',-9
                                         , '#',64,VL_TIME_UNITED_Q(1)
                                         , '#',32,vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                         .__PVT__aluColumn);
                            VL_STOP_MT("../third_party/fx68k/fx68kAlu.sv", 564, "");
                        }
                    } else if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: fx68kAlu.sv:564: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit.alu.aluGetOp", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                     .__PVT__aluColumn);
                        VL_STOP_MT("../third_party/fx68k/fx68kAlu.sv", 564, "");
                    }
                }
            } else if ((0x00000080U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row))) {
                if ((2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                     .__PVT__aluColumn)) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 2U;
                } else if ((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                            .__PVT__aluColumn)) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 4U;
                } else if ((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                            .__PVT__aluColumn)) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 0x14U;
                }
                if ((1U & (~ VL_ONEHOT_I((((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                            .__PVT__aluColumn) 
                                           << 2U) | 
                                          (((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                             .__PVT__aluColumn) 
                                            << 1U) 
                                           | (2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                              .__PVT__aluColumn))))))) {
                    if ((0U == (((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                  .__PVT__aluColumn) 
                                 << 2U) | (((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                             .__PVT__aluColumn) 
                                            << 1U) 
                                           | (2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                              .__PVT__aluColumn))))) {
                        if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                            VL_WRITEF_NX("[%0t] %%Error: fx68kAlu.sv:571: Assertion failed in %m: unique case, but none matched for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit.alu.aluGetOp", 'T',-9
                                         , '#',64,VL_TIME_UNITED_Q(1)
                                         , '#',32,vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                         .__PVT__aluColumn);
                            VL_STOP_MT("../third_party/fx68k/fx68kAlu.sv", 571, "");
                        }
                    } else if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: fx68kAlu.sv:571: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit.alu.aluGetOp", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                     .__PVT__aluColumn);
                        VL_STOP_MT("../third_party/fx68k/fx68kAlu.sv", 571, "");
                    }
                }
            } else if ((0x00000100U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row))) {
                if ((2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                     .__PVT__aluColumn)) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 5U;
                } else if ((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                            .__PVT__aluColumn)) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 1U;
                } else if ((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                            .__PVT__aluColumn)) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 0x14U;
                }
                if ((1U & (~ VL_ONEHOT_I((((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                            .__PVT__aluColumn) 
                                           << 2U) | 
                                          (((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                             .__PVT__aluColumn) 
                                            << 1U) 
                                           | (2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                              .__PVT__aluColumn))))))) {
                    if ((0U == (((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                  .__PVT__aluColumn) 
                                 << 2U) | (((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                             .__PVT__aluColumn) 
                                            << 1U) 
                                           | (2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                              .__PVT__aluColumn))))) {
                        if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                            VL_WRITEF_NX("[%0t] %%Error: fx68kAlu.sv:580: Assertion failed in %m: unique case, but none matched for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit.alu.aluGetOp", 'T',-9
                                         , '#',64,VL_TIME_UNITED_Q(1)
                                         , '#',32,vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                         .__PVT__aluColumn);
                            VL_STOP_MT("../third_party/fx68k/fx68kAlu.sv", 580, "");
                        }
                    } else if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: fx68kAlu.sv:580: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit.alu.aluGetOp", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                     .__PVT__aluColumn);
                        VL_STOP_MT("../third_party/fx68k/fx68kAlu.sv", 580, "");
                    }
                }
            } else {
                if ((2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                     .__PVT__aluColumn)) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 3U;
                } else if ((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                            .__PVT__aluColumn)) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 6U;
                } else if ((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                            .__PVT__aluColumn)) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 0x11U;
                }
                if ((1U & (~ VL_ONEHOT_I((((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                            .__PVT__aluColumn) 
                                           << 2U) | 
                                          (((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                             .__PVT__aluColumn) 
                                            << 1U) 
                                           | (2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                              .__PVT__aluColumn))))))) {
                    if ((0U == (((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                  .__PVT__aluColumn) 
                                 << 2U) | (((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                             .__PVT__aluColumn) 
                                            << 1U) 
                                           | (2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                              .__PVT__aluColumn))))) {
                        if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                            VL_WRITEF_NX("[%0t] %%Error: fx68kAlu.sv:587: Assertion failed in %m: unique case, but none matched for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit.alu.aluGetOp", 'T',-9
                                         , '#',64,VL_TIME_UNITED_Q(1)
                                         , '#',32,vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                         .__PVT__aluColumn);
                            VL_STOP_MT("../third_party/fx68k/fx68kAlu.sv", 587, "");
                        }
                    } else if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: fx68kAlu.sv:587: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit.alu.aluGetOp", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                     .__PVT__aluColumn);
                        VL_STOP_MT("../third_party/fx68k/fx68kAlu.sv", 587, "");
                    }
                }
            }
        } else if ((0x00000400U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row))) {
            if ((2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                 .__PVT__aluColumn)) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 3U;
            } else if ((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                        .__PVT__aluColumn)) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 0x0aU;
            } else if ((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                        .__PVT__aluColumn)) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 0x12U;
            }
            if ((1U & (~ VL_ONEHOT_I((((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                        .__PVT__aluColumn) 
                                       << 2U) | (((3U 
                                                   == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                   .__PVT__aluColumn) 
                                                  << 1U) 
                                                 | (2U 
                                                    == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                    .__PVT__aluColumn))))))) {
                if ((0U == (((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                              .__PVT__aluColumn) << 2U) 
                            | (((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                 .__PVT__aluColumn) 
                                << 1U) | (2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                          .__PVT__aluColumn))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: fx68kAlu.sv:594: Assertion failed in %m: unique case, but none matched for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit.alu.aluGetOp", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                     .__PVT__aluColumn);
                        VL_STOP_MT("../third_party/fx68k/fx68kAlu.sv", 594, "");
                    }
                } else if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                    VL_WRITEF_NX("[%0t] %%Error: fx68kAlu.sv:594: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit.alu.aluGetOp", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                 .__PVT__aluColumn);
                    VL_STOP_MT("../third_party/fx68k/fx68kAlu.sv", 594, "");
                }
            }
        } else if ((0x00000800U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row))) {
            if ((2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                 .__PVT__aluColumn)) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 7U;
            } else if ((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                        .__PVT__aluColumn)) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 7U;
            } else if ((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                        .__PVT__aluColumn)) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 0x13U;
            }
            if ((1U & (~ VL_ONEHOT_I((((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                        .__PVT__aluColumn) 
                                       << 2U) | (((3U 
                                                   == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                   .__PVT__aluColumn) 
                                                  << 1U) 
                                                 | (2U 
                                                    == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                    .__PVT__aluColumn))))))) {
                if ((0U == (((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                              .__PVT__aluColumn) << 2U) 
                            | (((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                 .__PVT__aluColumn) 
                                << 1U) | (2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                          .__PVT__aluColumn))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: fx68kAlu.sv:601: Assertion failed in %m: unique case, but none matched for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit.alu.aluGetOp", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                     .__PVT__aluColumn);
                        VL_STOP_MT("../third_party/fx68k/fx68kAlu.sv", 601, "");
                    }
                } else if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                    VL_WRITEF_NX("[%0t] %%Error: fx68kAlu.sv:601: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit.alu.aluGetOp", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                 .__PVT__aluColumn);
                    VL_STOP_MT("../third_party/fx68k/fx68kAlu.sv", 601, "");
                }
            }
        } else if ((0x00001000U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 0x0cU;
        } else if ((0x00002000U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = 9U;
        } else if ((0x00004000U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp 
                = ((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                    .__PVT__aluColumn) ? 9U : 8U);
        } else if ((0x00008000U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp 
                = ((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                    .__PVT__aluColumn) ? 4U : 8U);
        }
        if ((1U & (~ VL_ONEHOT_I(((0x00003f80U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row) 
                                                  >> 2U)) 
                                  | ((0x00000060U & 
                                      ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row) 
                                       >> 2U)) | (((IData)(
                                                           (0U 
                                                            != 
                                                            (0x0060U 
                                                             & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row)))) 
                                                   << 4U) 
                                                  | (0x0000000fU 
                                                     & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row) 
                                                        >> 1U))))))))) {
            if ((0U == ((0x00003f80U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row) 
                                        >> 2U)) | (
                                                   (0x00000060U 
                                                    & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row) 
                                                       >> 2U)) 
                                                   | (((IData)(
                                                               (0U 
                                                                != 
                                                                (0x0060U 
                                                                 & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row)))) 
                                                       << 4U) 
                                                      | (0x0000000fU 
                                                         & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row) 
                                                            >> 1U))))))) {
                if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                    VL_WRITEF_NX("[%0t] %%Error: fx68kAlu.sv:537: Assertion failed in %m: unique case, but none matched for '1'h1'\n",3, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit.alu.aluGetOp", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1));
                    VL_STOP_MT("../third_party/fx68k/fx68kAlu.sv", 537, "");
                }
            } else if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: fx68kAlu.sv:537: Assertion failed in %m: unique case, but multiple matches found for '1'h1'\n",3, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit.alu.aluGetOp", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1));
                VL_STOP_MT("../third_party/fx68k/fx68kAlu.sv", 537, "");
            }
        }
    }
    if ((1U & (~ VL_ONEHOT_I((((5U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                .__PVT__aluColumn) 
                               << 1U) | (1U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                         .__PVT__aluColumn)))))) {
        if ((0U != (((5U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                      .__PVT__aluColumn) << 1U) | (1U 
                                                   == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                   .__PVT__aluColumn)))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: fx68kAlu.sv:532: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit.alu.aluGetOp", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1)
                             , '#',32,vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                             .__PVT__aluColumn);
                VL_STOP_MT("../third_party/fx68k/fx68kAlu.sv", 532, "");
            }
        }
    }
    if ((1U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
         .__PVT__aluColumn)) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__cMask 
            = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
               .__PVT__aluFinish ? ((0x00000080U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row))
                                     ? 0x0fU : 0x0cU)
                : ((0U != (3U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row) 
                                 >> 0x0000000dU))) ? 4U
                    : 0x0fU));
    } else if (((2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                 .__PVT__aluColumn) || (3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                        .__PVT__aluColumn))) {
        if ((2U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__cMask = 0x0fU;
        } else if (((1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row) 
                           >> 3U)) || (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row) 
                                             >> 9U)))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__cMask 
                = ((2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                    .__PVT__aluColumn) ? 0x1bU : 0x1fU);
        } else if (((((1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row) 
                             >> 2U)) || (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row) 
                                               >> 5U))) 
                     || (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row) 
                               >> 0x0aU))) || (1U & 
                                               ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row) 
                                                >> 0x0cU)))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__cMask = 0x1fU;
        } else if ((((1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row) 
                            >> 6U)) || (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row) 
                                              >> 7U))) 
                    || (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row) 
                              >> 0x0bU)))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__cMask = 0x0fU;
        } else if (((((1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row) 
                             >> 4U)) || (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row) 
                                               >> 8U))) 
                     || (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row) 
                               >> 0x0dU))) || (1U & 
                                               ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row) 
                                                >> 0x0eU)))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__cMask = 0x0fU;
        } else if ((0x00008000U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__cMask = 0U;
        }
        if ((1U & (~ VL_ONEHOT_I(((((4U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row) 
                                           >> 0x0000000dU)) 
                                    | (((IData)((0U 
                                                 != 
                                                 (0x6110U 
                                                  & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row)))) 
                                        << 1U) | (IData)(
                                                         (0U 
                                                          != 
                                                          (0x08c0U 
                                                           & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row)))))) 
                                   << 3U) | (((IData)(
                                                      (0U 
                                                       != 
                                                       (0x1424U 
                                                        & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row)))) 
                                              << 2U) 
                                             | (((IData)(
                                                         (0U 
                                                          != 
                                                          (0x0208U 
                                                           & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row)))) 
                                                 << 1U) 
                                                | (1U 
                                                   & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row) 
                                                      >> 1U))))))))) {
            if ((0U == ((((4U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row) 
                                 >> 0x0000000dU)) | 
                          (((IData)((0U != (0x6110U 
                                            & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row)))) 
                            << 1U) | (IData)((0U != 
                                              (0x08c0U 
                                               & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row)))))) 
                         << 3U) | (((IData)((0U != 
                                             (0x1424U 
                                              & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row)))) 
                                    << 2U) | (((IData)(
                                                       (0U 
                                                        != 
                                                        (0x0208U 
                                                         & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row)))) 
                                               << 1U) 
                                              | (1U 
                                                 & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row) 
                                                    >> 1U))))))) {
                if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                    VL_WRITEF_NX("[%0t] %%Error: fx68kAlu.sv:785: Assertion failed in %m: unique case, but none matched for '1'h1'\n",3, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit.alu.ccrTable", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1));
                    VL_STOP_MT("../third_party/fx68k/fx68kAlu.sv", 785, "");
                }
            } else if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: fx68kAlu.sv:785: Assertion failed in %m: unique case, but multiple matches found for '1'h1'\n",3, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit.alu.ccrTable", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1));
                VL_STOP_MT("../third_party/fx68k/fx68kAlu.sv", 785, "");
            }
        }
    } else if ((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                .__PVT__aluColumn)) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__cMask 
            = (((((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row)) 
                  || (8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row))) 
                 || (0x0010U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row))) 
                || (0x0020U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row)))
                ? 0x1fU : ((0x0080U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row))
                            ? 0x0fU : (((0x0200U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row)) 
                                        || (0x0400U 
                                            == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row)))
                                        ? 0x0fU : (
                                                   ((0x0100U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row)) 
                                                    || (0x0800U 
                                                        == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row)))
                                                    ? 0x1fU
                                                    : 0U))));
        if ((1U & (~ VL_ONEHOT_I((((((0x0800U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row)) 
                                     | (0x0100U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row))) 
                                    << 3U) | (((0x0400U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row)) 
                                               | (0x0200U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row))) 
                                              << 2U)) 
                                  | (((0x0080U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row)) 
                                      << 1U) | ((0x0020U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row)) 
                                                | ((0x0010U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row)) 
                                                   | ((8U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row)) 
                                                      | (4U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row))))))))))) {
            if ((0U != (((((0x0800U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row)) 
                           | (0x0100U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row))) 
                          << 3U) | (((0x0400U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row)) 
                                     | (0x0200U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row))) 
                                    << 2U)) | (((0x0080U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row)) 
                                                << 1U) 
                                               | ((0x0020U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row)) 
                                                  | ((0x0010U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row)) 
                                                     | ((8U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row)) 
                                                        | (4U 
                                                           == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row))))))))) {
                if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                    VL_WRITEF_NX("[%0t] %%Error: fx68kAlu.sv:809: Assertion failed in %m: unique case, but multiple matches found for '16'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit.alu.ccrTable", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',16,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row));
                    VL_STOP_MT("../third_party/fx68k/fx68kAlu.sv", 809, "");
                }
            }
        }
    } else {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__cMask 
            = ((5U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                .__PVT__aluColumn) ? ((2U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row))
                                       ? 0x0fU : 0U)
                : 0U);
    }
    if ((1U & (~ VL_ONEHOT_I(((((5U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                 .__PVT__aluColumn) 
                                << 3U) | ((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                           .__PVT__aluColumn) 
                                          << 2U)) | 
                              ((((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                  .__PVT__aluColumn) 
                                 | (2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                    .__PVT__aluColumn)) 
                                << 1U) | (1U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                          .__PVT__aluColumn))))))) {
        if ((0U != ((((5U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                       .__PVT__aluColumn) << 3U) | 
                     ((4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                       .__PVT__aluColumn) << 2U)) | 
                    ((((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                        .__PVT__aluColumn) | (2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                              .__PVT__aluColumn)) 
                      << 1U) | (1U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                .__PVT__aluColumn))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: fx68kAlu.sv:781: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit.alu.ccrTable", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1)
                             , '#',32,vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                             .__PVT__aluColumn);
                VL_STOP_MT("../third_party/fx68k/fx68kAlu.sv", 781, "");
            }
        }
    }
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dobInput 
        = ((2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
            .__PVT__dobCtrl) ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Abd)
            : ((1U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                .__PVT__dobCtrl) ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Dbd)
                : ((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                    .__PVT__dobCtrl) ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluLatch)
                    : 0U)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__abdMux 
        = ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryl2Abd)
            ? ((0x11U >= (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__actualRy))
                ? vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68L
               [vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__actualRy]
                : 0U) : ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxl2Abd)
                          ? ((0x11U >= (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__actualRx))
                              ? vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68L
                             [vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__actualRx]
                              : 0U) : (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                       .__PVT__dbin2Abd
                                        ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dbin)
                                        : (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                           .__PVT__alu2Abd
                                            ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluLatch)
                                            : 0U))));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dbdMux 
        = ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryl2Dbd)
            ? ((0x11U >= (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__actualRy))
                ? vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68L
               [vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__actualRy]
                : 0U) : ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxl2Dbd)
                          ? ((0x11U >= (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__actualRx))
                              ? vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68L
                             [vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__actualRx]
                              : 0U) : (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                       .__PVT__alue2Dbd
                                        ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__alue)
                                        : (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                           .__PVT__dbin2Dbd
                                            ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dbin)
                                            : (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                               .__PVT__alu2Dbd
                                                ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluLatch)
                                                : (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                   .__PVT__dcr2Dbd
                                                    ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dcrOutput)
                                                    : 0U))))));
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shftMsb 
        = (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__isLong)
                  ? ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__alue) 
                     >> 0x0000000fU) : ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__isByte)
                                         ? ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aOperand) 
                                            >> 7U) : 
                                        ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aOperand) 
                                         >> 0x0000000fU))));
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shftCin 
        = (1U & ((~ ((0x0fU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                     | ((0x0dU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                        | (0x10U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))))) 
                 & (((0x11U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                     | (0x0eU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)))
                     ? (IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shftMsb)
                     : ((0x12U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))
                         ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aOperand)
                         : (((0x13U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                             | (0x14U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)))
                             ? ((0x00000080U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row))
                                 ? ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__rIrd8)
                                     ? (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__pswCcr) 
                                         >> 3U) ^ ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__pswCcr) 
                                                   >> 1U))
                                     : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__pswCcr))
                                 : ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__pswCcr) 
                                    >> 4U)) : ((0x15U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                               & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                                  .__PVT__aluColumn 
                                                  >> 1U)))))));
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shifter__DOT__tdata 
        = (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__alue) 
            << 0x00000010U) | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aOperand));
    if (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__isByte) 
         & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shftRight))) {
        sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shifter__DOT__tdata 
            = ((0xfffffeffU & sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shifter__DOT__tdata) 
               | ((IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shftCin) 
                  << 8U));
    } else if (((~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__isLong)) 
                & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shftRight))) {
        sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shifter__DOT__tdata 
            = ((0xfffeffffU & sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shifter__DOT__tdata) 
               | ((IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shftCin) 
                  << 0x00000010U));
    }
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shftResult 
        = (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT____Vcellinp__shifter__swapWords) 
            & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shftRight))
            ? ((((0x00008000U & (sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shifter__DOT__tdata 
                                 << 0x0000000fU)) | 
                 (sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shifter__DOT__tdata 
                  >> 0x00000011U)) << 0x00000010U) 
               | (((IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shftCin) 
                   << 0x0000000fU) | (0x00007fffU & 
                                      (sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shifter__DOT__tdata 
                                       >> 1U)))) : 
           ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT____Vcellinp__shifter__swapWords)
             ? ((((0x0000fffeU & (sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shifter__DOT__tdata 
                                  >> 0x0000000fU)) 
                  | (IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shftCin)) 
                 << 0x00000010U) | ((0x0000fffeU & 
                                     (sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shifter__DOT__tdata 
                                      << 1U)) | (sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shifter__DOT__tdata 
                                                 >> 0x0000001fU)))
             : ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shftRight)
                 ? (((IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shftCin) 
                     << 0x0000001fU) | (sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shifter__DOT__tdata 
                                        >> 1U)) : (
                                                   (sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shifter__DOT__tdata 
                                                    << 1U) 
                                                   | (IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shftCin)))));
    __Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__isByte 
        = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__isByte;
    __Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__bAdd 
        = (((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
            | (0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
           | (0x0cU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)));
    __Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__cin 
        = (1U & ((~ ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                     | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)))) 
                 & ((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                    | (((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                        | (0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)))
                        ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrCore)
                        : (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__pswCcr) 
                            >> 4U) & ((0x0cU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                      | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))))))));
    __Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__inpb 
        = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__dOperand;
    __Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__inpa 
        = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aOperand;
    if (__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__isByte) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__rtemp 
            = (0x0001ffffU & ((IData)(__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__bAdd)
                               ? (((0x000000ffU & (IData)(__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__inpb)) 
                                   + (0x000000ffU & (IData)(__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__inpa))) 
                                  + (IData)(__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__cin))
                               : (((0x000000ffU & (IData)(__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__inpb)) 
                                   - (0x000000ffU & (IData)(__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__inpa))) 
                                  - (IData)(__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__cin))));
        vlSelfRef.__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__result 
            = ((0x0000ff00U & ((- (IData)((1U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__rtemp 
                                                 >> 7U)))) 
                               << 8U)) | (0x000000ffU 
                                          & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__rtemp));
        vlSelfRef.__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__cout 
            = (1U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__rtemp 
                     >> 8U));
        sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__rm 
            = (1U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__rtemp 
                     >> 7U));
        sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__dm 
            = (1U & ((IData)(__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__inpb) 
                     >> 7U));
        sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__tsm 
            = (1U & ((IData)(__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__inpa) 
                     >> 7U));
    } else {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__rtemp 
            = (0x0001ffffU & ((IData)(__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__bAdd)
                               ? (((IData)(__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__inpb) 
                                   + (IData)(__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__inpa)) 
                                  + (IData)(__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__cin))
                               : (((IData)(__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__inpb) 
                                   - (IData)(__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__inpa)) 
                                  - (IData)(__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__cin))));
        vlSelfRef.__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__result 
            = (0x0000ffffU & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__rtemp);
        vlSelfRef.__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__cout 
            = (1U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__rtemp 
                     >> 0x10U));
        sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__rm 
            = (1U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__rtemp 
                     >> 0x0fU));
        sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__dm 
            = (1U & ((IData)(__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__inpb) 
                     >> 0x0fU));
        sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__tsm 
            = (1U & ((IData)(__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__inpa) 
                     >> 0x0fU));
    }
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__sm 
        = (1U & ((IData)(__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__bAdd)
                  ? (IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__tsm)
                  : (~ (IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__tsm))));
    __Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__ov 
        = ((((IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__sm) 
             & (IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__dm)) 
            & (~ (IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__rm))) 
           | (((~ (IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__sm)) 
               & (~ (IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__dm))) 
              & (IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__rm)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__subHcarry 
        = (1U & ((((IData)(__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__inpa) 
                   ^ (IData)(__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__inpb)) 
                  ^ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__rtemp) 
                 >> 4U));
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__subResult 
        = vlSelfRef.__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__result;
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__subCout 
        = vlSelfRef.__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__cout;
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__subOv 
        = __Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__ov;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__result 
        = (0x0000ffffU & ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))
                           ? ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aOperand) 
                              & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__dOperand))
                           : ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))
                               ? ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aOperand) 
                                  | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__dOperand))
                               : ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))
                                   ? ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aOperand) 
                                      ^ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__dOperand))
                                   : ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))
                                       ? ((0x0000ff00U 
                                           & ((- (IData)(
                                                         (1U 
                                                          & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aOperand) 
                                                             >> 7U)))) 
                                              << 8U)) 
                                          | (0x000000ffU 
                                             & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aOperand)))
                                       : ((((((((((0x15U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                                  || (0x0dU 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                                                 || (0x0eU 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                                                || (0x0fU 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                                               || (0x10U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                                              || (0x11U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                                             || (0x12U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                                            || (0x13U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                                           || (0x14U 
                                               == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)))
                                           ? vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shftResult
                                           : ((((((
                                                   ((4U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                                    || (0x0bU 
                                                        == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                                                   || (0x0cU 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                                                  || (2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                                                 || (0x0aU 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                                                || (7U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                                               || (3U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)))
                                               ? (IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__subResult)
                                               : ((
                                                   (0x16U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                                   || (6U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)))
                                                   ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__bcdLatch)
                                                   : 0U))))))));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp 
        = ((0x0fU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp)) 
           | (0x00000010U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__pswCcr)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp 
        = (0x1cU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp 
        = ((0x1bU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp)) 
           | (4U & (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__isByte)
                      ? (~ (0U != (0x000000ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__result))))
                      : (~ (0U != (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__result)))) 
                    << 2U)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp 
        = ((0x17U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp)) 
           | (8U & (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__isByte)
                      ? ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__result) 
                         >> 7U) : ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__result) 
                                   >> 0x0fU)) << 3U)));
    if (((((((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
               | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                   || (8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                  || (9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)))) 
              | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
             | (0x15U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
            | ((0x0fU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
               || (0x13U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)))) 
           | ((0x10U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
              || (0x14U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)))) 
          | (0x0dU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
         | (0x0eU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)))) {
        if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) {
            if ((5U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                 .__PVT__aluColumn)) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp 
                    = (0x0000000aU | (0x11U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp)));
            }
        } else if ((((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                     || (8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                    || (9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp 
                = (0x1cU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp));
        } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp 
                = ((0x1eU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp)) 
                   | (((1U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                        .__PVT__aluColumn) & (IData)(
                                                     (0U 
                                                      != 
                                                      (0x0900U 
                                                       & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row))))) 
                      & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__pswCcr) 
                         >> 4U)));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp 
                = (0x1dU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp));
        } else if ((0x15U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp 
                = ((0x1eU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp)) 
                   | (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aOperand) 
                            >> 0x0fU)));
        } else if (((0x0fU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                    || (0x13U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp 
                = ((0x1eU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp)) 
                   | (IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shftMsb));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp 
                = ((0x0fU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp)) 
                   | ((IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shftMsb) 
                      << 4U));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp 
                = (0x1dU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp));
        } else if (((0x10U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                    || (0x14U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp 
                = ((0x1eU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp)) 
                   | (1U & ((~ ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row) 
                                >> 7U)) & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aOperand))));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp 
                = ((0x0fU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp)) 
                   | (0x00000010U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aOperand) 
                                     << 4U)));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp 
                = (0x1dU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp));
        } else if ((0x0dU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp 
                = ((0x0fU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp)) 
                   | ((IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shftMsb) 
                      << 4U));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp 
                = ((0x1eU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp)) 
                   | (IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shftMsb));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp 
                = ((0x1dU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp)) 
                   | (2U & ((0xfffffffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__pswCcr)) 
                            | (((IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shftMsb) 
                                ^ ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__isLong)
                                    ? ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__alue) 
                                       >> 0x0eU) : 
                                   ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__isByte)
                                     ? ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aOperand) 
                                        >> 6U) : ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aOperand) 
                                                  >> 0x0eU)))) 
                               << 1U))));
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp 
                = ((0x0fU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp)) 
                   | (0x00000010U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aOperand) 
                                     << 4U)));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp 
                = ((0x1cU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp)) 
                   | (1U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aOperand)));
        }
    } else if ((0x11U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp 
            = ((0x1eU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp)) 
               | (IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shftMsb));
    } else if ((0x12U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp 
            = ((0x1eU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp)) 
               | (1U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aOperand)));
    } else if (((((((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                    || (0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                   || (0x0cU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                  || (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                 || (0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                || (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp 
            = ((0x1eU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp)) 
               | (IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__subCout));
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp 
            = ((0x0fU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp)) 
               | ((IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__subCout) 
                  << 4U));
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp 
            = ((0x1dU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp)) 
               | ((IData)(sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__subOv) 
                  << 1U));
    } else if (((0x16U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                || (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp 
            = ((0x0fU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp)) 
               | ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__bcdCarry) 
                  << 4U));
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp 
            = ((0x1cU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp)) 
               | (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__bcdOverf) 
                   << 1U) | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__bcdCarry)));
    }
    vlSelfRef.__VdfgRegularize_hebeb780c_0_0 = (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp) 
                                                 & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrMask)) 
                                                | ((~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrMask)) 
                                                   & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__pswCcr)));
}

void Vsys_top___024root___nba_comb__TOP__8(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___nba_comb__TOP__8\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
        .__PVT__ssp) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxMux = 0x10U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxIsSp = 1U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxReg = 0U;
    } else if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod
               .__PVT__rxIsUsp) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxMux = 0x0fU;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxIsSp = 1U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxReg = 0U;
    } else if ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod
                .__PVT__rxIsDt & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod
                                  .__PVT__implicitSp))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxMux = 0x11U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxIsSp = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxReg = 0U;
    } else {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxReg 
            = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod
               .__PVT__implicitSp ? 0x0fU : (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod
                                             .__PVT__rxIsMovem
                                              ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__movemRx)
                                              : ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod
                                                  .__PVT__rxIsAreg 
                                                  << 3U) 
                                                 | vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod
                                                 .__PVT__rx)));
        if ((0x0000000fU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxReg))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxMux 
                = ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__pswS)
                    ? 0x00000010U : 0x0000000fU);
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxIsSp = 1U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxMux 
                = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxReg;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxIsSp = 0U;
        }
    }
    if ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod
         .__PVT__ryIsDt & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                           .__PVT__rz))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryMux = 0x11U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryIsSp = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryReg = 0U;
    } else {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryReg 
            = (0x0000000fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                              .__PVT__rz ? ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irc) 
                                            >> 0x0cU)
                               : ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod
                                   .__PVT__ryIsAreg 
                                   << 3U) | vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod
                                  .__PVT__ry)));
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryIsSp 
            = (0x0000000fU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryReg));
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryMux 
            = (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryIsSp) 
                & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__pswS))
                ? 0x10U : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryReg));
    }
}

void Vsys_top___024root___nba_comb__TOP__9(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___nba_comb__TOP__9\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VdfgRegularize_hebeb780c_0_1;
    __VdfgRegularize_hebeb780c_0_1 = 0;
    // Body
    __VdfgRegularize_hebeb780c_0_1 = (((((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__BRi) 
                                         & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__BgackI) 
                                            & ((~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busArbiter__DOT__rGranted)) 
                                               & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Halti)))) 
                                        | ((1U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase) 
                                           & ((~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rAS)) 
                                              & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__isRmcReg)))) 
                                       & ((~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__bcReset)) 
                                          & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                             .__PVT__permStart 
                                             | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__bcPend))))
                                       ? 2U : 1U);
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__next 
        = ((0U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase)
            ? 1U : ((6U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase)
                     ? 1U : ((2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase)
                              ? 3U : ((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase)
                                       ? 4U : ((4U 
                                                == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase)
                                                ? (
                                                   (1U 
                                                    & ((~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__rDtack)) 
                                                       | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__iStop)))
                                                    ? 5U
                                                    : 4U)
                                                : (
                                                   (5U 
                                                    == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase)
                                                    ? 
                                                   (((5U 
                                                      == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase) 
                                                     & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__bcReset) 
                                                        & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__isRmcReg)))
                                                     ? 6U
                                                     : __VdfgRegularize_hebeb780c_0_1)
                                                    : 
                                                   ((1U 
                                                     == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase)
                                                     ? __VdfgRegularize_hebeb780c_0_1
                                                     : 1U)))))));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busEnding 
        = ((1U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__next) 
           | (2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__next));
}

void Vsys_top___024root___nba_sequent__TOP__0(Vsys_top___024root* vlSelf);
void Vsys_top___024root___nba_sequent__TOP__1(Vsys_top___024root* vlSelf);
void Vsys_top___024root___nba_sequent__TOP__2(Vsys_top___024root* vlSelf);
void Vsys_top___024root___nba_sequent__TOP__4(Vsys_top___024root* vlSelf);
void Vsys_top___024root___nba_sequent__TOP__5(Vsys_top___024root* vlSelf);
void Vsys_top___024root___act_sequent__TOP__0(Vsys_top___024root* vlSelf);

void Vsys_top___024root___eval_nba(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___eval_nba\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __Vinline__nba_comb__TOP__9___VdfgRegularize_hebeb780c_0_1;
    __Vinline__nba_comb__TOP__9___VdfgRegularize_hebeb780c_0_1 = 0;
    // Body
    if ((4ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vsys_top___024root___nba_sequent__TOP__0(vlSelf);
    }
    if ((8ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vsys_top___024root___nba_sequent__TOP__1(vlSelf);
    }
    if ((0x0000000000000010ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vsys_top___024root___nba_sequent__TOP__2(vlSelf);
    }
    if ((8ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vsys_top___024root___nba_sequent__TOP__4(vlSelf);
        Vsys_top___024root___nba_sequent__TOP__5(vlSelf);
        if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT1) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__xToIrc = 0U;
        } else if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT3) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__xToIrc 
                = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                .__PVT__toIrc;
        }
    }
    if ((0x0000000000000018ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__ms0 
            = ((2U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                      >> 7U)) | (1U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__alue)));
    }
    if ((4ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vsys_top___024root___nba_sequent__TOP__7(vlSelf);
    }
    if ((8ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vsys_top___024root___nba_sequent__TOP__8(vlSelf);
    }
    if ((0x0000000000000018ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vsys_top___024root___nba_comb__TOP__1(vlSelf);
    }
    if ((0x0000000000000010ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microOutput 
            = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uRom__DOT__uRam
            [vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microAddr];
    }
    if ((4ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vsys_top___024root___nba_sequent__TOP__10(vlSelf);
    }
    if ((8ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vsys_top___024root___nba_sequent__TOP__11(vlSelf);
    }
    if ((0x000000000000000cULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.sys_top__DOT__sys__DOT__mfp_dout 
            = (0x000000ffU & ((0x00000020U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                               ? ((0x00000010U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                   ? 0xffU : ((8U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                               ? ((4U 
                                                   & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                   ? 
                                                  ((2U 
                                                    & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                    ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__rx_data)
                                                    : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tsr))
                                                   : 
                                                  ((2U 
                                                    & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                    ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__rsr)
                                                    : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ucr)))
                                               : ((4U 
                                                   & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                   ? 
                                                  ((2U 
                                                    & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                    ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__scr)
                                                    : vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tmc[3U])
                                                   : 
                                                  ((2U 
                                                    & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                                    ? vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tmc[2U]
                                                    : vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tmc[1U]))))
                               : ((0x00000010U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                   ? ((8U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                       ? ((4U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                           ? ((2U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                               ? vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tmc[0U]
                                               : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tcdcr))
                                           : ((2U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                               ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tbcr)
                                               : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tacr)))
                                       : ((4U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                           ? ((2U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                               ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__vr)
                                               : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr))
                                           : ((2U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                               ? ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__imr) 
                                                  >> 8U)
                                               : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__isr))))
                                   : ((8U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                       ? ((4U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                           ? ((2U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                               ? ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__isr) 
                                                  >> 8U)
                                               : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr))
                                           : ((2U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                               ? ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ipr) 
                                                  >> 8U)
                                               : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ier)))
                                       : ((4U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                           ? ((2U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                               ? ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ier) 
                                                  >> 8U)
                                               : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ddr))
                                           : ((2U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob)
                                               ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__aer)
                                               : ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__gpip) 
                                                  & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__ddr))))))));
    }
    if ((0x000000000000000aULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vsys_top___024root___act_sequent__TOP__0(vlSelf);
    }
    if ((4ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks.__PVT__enPhi1 
            = vlSelfRef.sys_top__DOT__sys__DOT__phi1;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks.__PVT__enPhi2 
            = vlSelfRef.sys_top__DOT__sys__DOT__phi2;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT____Vcellinp__nanoRom__clk 
            = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
            .__PVT__clk;
    }
    if ((0x0000000000000018ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vsys_top___024root___nba_comb__TOP__4(vlSelf);
    }
    if ((9ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vsys_top___024root___nba_comb__TOP__5(vlSelf);
    }
    if ((0x000000000000000cULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT3 
            = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
               .__PVT__enPhi1 & (2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__tState));
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT4 
            = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
               .__PVT__enPhi2 & ((0U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__tState) 
                                 | (3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__tState)));
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__rstUrom 
            = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
               .__PVT__enPhi1 & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enErrClk));
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__bcReset 
            = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
               .__PVT__extReset | (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__addrOeDelay) 
                                    & (~ ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__BeI) 
                                          | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__BeiDelay)))) 
                                   & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Vpai)));
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT2 
            = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
               .__PVT__enPhi2 & (1U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__tState));
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT1 
            = ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
                .__PVT__enPhi1 & (4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__tState)) 
               & (~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__wClk)));
    }
    if ((0x0000000000000019ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vsys_top___024root___nba_comb__TOP__7(vlSelf);
    }
    if ((0x000000000000000bULL & vlSelfRef.__VnbaTriggered[0U])) {
        if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
            .__PVT__ssp) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxMux = 0x10U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxIsSp = 1U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxReg = 0U;
        } else if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod
                   .__PVT__rxIsUsp) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxMux = 0x0fU;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxIsSp = 1U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxReg = 0U;
        } else if ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod
                    .__PVT__rxIsDt & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod
                                      .__PVT__implicitSp))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxMux = 0x11U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxIsSp = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxReg = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxReg 
                = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod
                   .__PVT__implicitSp ? 0x0fU : (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod
                                                 .__PVT__rxIsMovem
                                                  ? (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__movemRx)
                                                  : 
                                                 ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod
                                                   .__PVT__rxIsAreg 
                                                   << 3U) 
                                                  | vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod
                                                  .__PVT__rx)));
            if ((0x0000000fU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxReg))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxMux 
                    = ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__pswS)
                        ? 0x00000010U : 0x0000000fU);
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxIsSp = 1U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxMux 
                    = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxReg;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxIsSp = 0U;
            }
        }
        if ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod
             .__PVT__ryIsDt & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                               .__PVT__rz))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryMux = 0x11U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryIsSp = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryReg = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryReg 
                = (0x0000000fU & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                  .__PVT__rz ? ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irc) 
                                                >> 0x0cU)
                                   : ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod
                                       .__PVT__ryIsAreg 
                                       << 3U) | vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod
                                      .__PVT__ry)));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryIsSp 
                = (0x0000000fU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryReg));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryMux 
                = (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryIsSp) 
                    & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__pswS))
                    ? 0x10U : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryReg));
        }
    }
    if ((0x000000000000000dULL & vlSelfRef.__VnbaTriggered[0U])) {
        __Vinline__nba_comb__TOP__9___VdfgRegularize_hebeb780c_0_1 
            = (((((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__BRi) 
                  & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__BgackI) 
                     & ((~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busArbiter__DOT__rGranted)) 
                        & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Halti)))) 
                 | ((1U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase) 
                    & ((~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rAS)) 
                       & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__isRmcReg)))) 
                & ((~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__bcReset)) 
                   & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                      .__PVT__permStart | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__bcPend))))
                ? 2U : 1U);
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__next 
            = ((0U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase)
                ? 1U : ((6U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase)
                         ? 1U : ((2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase)
                                  ? 3U : ((3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase)
                                           ? 4U : (
                                                   (4U 
                                                    == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase)
                                                    ? 
                                                   ((1U 
                                                     & ((~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__rDtack)) 
                                                        | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__iStop)))
                                                     ? 5U
                                                     : 4U)
                                                    : 
                                                   ((5U 
                                                     == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase)
                                                     ? 
                                                    (((5U 
                                                       == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase) 
                                                      & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__bcReset) 
                                                         & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__isRmcReg)))
                                                      ? 6U
                                                      : __Vinline__nba_comb__TOP__9___VdfgRegularize_hebeb780c_0_1)
                                                     : 
                                                    ((1U 
                                                      == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase)
                                                      ? __Vinline__nba_comb__TOP__9___VdfgRegularize_hebeb780c_0_1
                                                      : 1U)))))));
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busEnding 
            = ((1U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__next) 
               | (2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__next));
    }
}

void Vsys_top___024root___trigger_orInto__act_vec_vec(VlUnpacked<QData/*63:0*/, 1> &out, const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___trigger_orInto__act_vec_vec\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = (out[n] | in[n]);
        n = ((IData)(1U) + n);
    } while ((0U >= n));
}

void Vsys_top___024root___eval_triggers_vec__act(Vsys_top___024root* vlSelf);
#ifdef VL_DEBUG
VL_ATTR_COLD void Vsys_top___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG
bool Vsys_top___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in);
void Vsys_top___024root___eval_act(Vsys_top___024root* vlSelf);

bool Vsys_top___024root___eval_phase__act(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___eval_phase__act\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VactExecute;
    // Body
    Vsys_top___024root___eval_triggers_vec__act(vlSelf);
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vsys_top___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
    }
#endif
    Vsys_top___024root___trigger_orInto__act_vec_vec(vlSelfRef.__VnbaTriggered, vlSelfRef.__VactTriggered);
    __VactExecute = Vsys_top___024root___trigger_anySet__act(vlSelfRef.__VactTriggered);
    if (__VactExecute) {
        Vsys_top___024root___eval_act(vlSelf);
    }
    return (__VactExecute);
}

void Vsys_top___024root___trigger_clear__act(VlUnpacked<QData/*63:0*/, 1> &out) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___trigger_clear__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = 0ULL;
        n = ((IData)(1U) + n);
    } while ((1U > n));
}

bool Vsys_top___024root___eval_phase__nba(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___eval_phase__nba\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = Vsys_top___024root___trigger_anySet__act(vlSelfRef.__VnbaTriggered);
    if (__VnbaExecute) {
        Vsys_top___024root___eval_nba(vlSelf);
        Vsys_top___024root___trigger_clear__act(vlSelfRef.__VnbaTriggered);
    }
    return (__VnbaExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vsys_top___024root___dump_triggers__ico(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG
bool Vsys_top___024root___eval_phase__ico(Vsys_top___024root* vlSelf);

void Vsys_top___024root___eval(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___eval\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VicoIterCount;
    IData/*31:0*/ __VnbaIterCount;
    // Body
    __VicoIterCount = 0U;
    vlSelfRef.__VicoFirstIteration = 1U;
    do {
        if (VL_UNLIKELY(((0x00002710U < __VicoIterCount)))) {
#ifdef VL_DEBUG
            Vsys_top___024root___dump_triggers__ico(vlSelfRef.__VicoTriggered, "ico"s);
#endif
            VL_FATAL_MT("sys_top.sv", 2, "", "DIDNOTCONVERGE: Input combinational region did not converge after '--converge-limit' of 10000 tries");
        }
        __VicoIterCount = ((IData)(1U) + __VicoIterCount);
        vlSelfRef.__VicoPhaseResult = Vsys_top___024root___eval_phase__ico(vlSelf);
        vlSelfRef.__VicoFirstIteration = 0U;
    } while (vlSelfRef.__VicoPhaseResult);
    __VnbaIterCount = 0U;
    do {
        if (VL_UNLIKELY(((0x00002710U < __VnbaIterCount)))) {
#ifdef VL_DEBUG
            Vsys_top___024root___dump_triggers__act(vlSelfRef.__VnbaTriggered, "nba"s);
#endif
            VL_FATAL_MT("sys_top.sv", 2, "", "DIDNOTCONVERGE: NBA region did not converge after '--converge-limit' of 10000 tries");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        vlSelfRef.__VactIterCount = 0U;
        do {
            if (VL_UNLIKELY(((0x00002710U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                Vsys_top___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
#endif
                VL_FATAL_MT("sys_top.sv", 2, "", "DIDNOTCONVERGE: Active region did not converge after '--converge-limit' of 10000 tries");
            }
            vlSelfRef.__VactIterCount = ((IData)(1U) 
                                         + vlSelfRef.__VactIterCount);
            vlSelfRef.__VactPhaseResult = Vsys_top___024root___eval_phase__act(vlSelf);
        } while (vlSelfRef.__VactPhaseResult);
        vlSelfRef.__VnbaPhaseResult = Vsys_top___024root___eval_phase__nba(vlSelf);
    } while (vlSelfRef.__VnbaPhaseResult);
}

#ifdef VL_DEBUG
void Vsys_top___024root___eval_debug_assertions(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___eval_debug_assertions\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (VL_UNLIKELY(((vlSelfRef.clk & 0xfeU)))) {
        Verilated::overWidthError("clk");
    }
    if (VL_UNLIKELY(((vlSelfRef.reset & 0xfeU)))) {
        Verilated::overWidthError("reset");
    }
    if (VL_UNLIKELY(((vlSelfRef.loading & 0xfeU)))) {
        Verilated::overWidthError("loading");
    }
    if (VL_UNLIKELY(((vlSelfRef.ioctl_wr & 0xfeU)))) {
        Verilated::overWidthError("ioctl_wr");
    }
    if (VL_UNLIKELY(((vlSelfRef.ioctl_addr & 0xf8000000U)))) {
        Verilated::overWidthError("ioctl_addr");
    }
    if (VL_UNLIKELY(((vlSelfRef.midi_in & 0xfeU)))) {
        Verilated::overWidthError("midi_in");
    }
    if (VL_UNLIKELY(((vlSelfRef.DDRAM_BUSY & 0xfeU)))) {
        Verilated::overWidthError("DDRAM_BUSY");
    }
    if (VL_UNLIKELY(((vlSelfRef.DDRAM_DOUT_READY & 0xfeU)))) {
        Verilated::overWidthError("DDRAM_DOUT_READY");
    }
    if (VL_UNLIKELY(((vlSelfRef.lcd_line & 0xfeU)))) {
        Verilated::overWidthError("lcd_line");
    }
    if (VL_UNLIKELY(((vlSelfRef.lcd_pos & 0xf0U)))) {
        Verilated::overWidthError("lcd_pos");
    }
}
#endif  // VL_DEBUG
