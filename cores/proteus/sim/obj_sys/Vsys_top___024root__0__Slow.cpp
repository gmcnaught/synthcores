// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vsys_top.h for the primary calling header

#include "Vsys_top__pch.h"

VL_ATTR_COLD void Vsys_top___024root___eval_static(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___eval_static\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __Vinline__eval_static__TOP_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__rm;
    __Vinline__eval_static__TOP_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__rm = 0;
    CData/*0:0*/ __Vinline__eval_static__TOP_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__sm;
    __Vinline__eval_static__TOP_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__sm = 0;
    CData/*0:0*/ __Vinline__eval_static__TOP_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__dm;
    __Vinline__eval_static__TOP_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__dm = 0;
    CData/*0:0*/ __Vinline__eval_static__TOP_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__tsm;
    __Vinline__eval_static__TOP_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__tsm = 0;
    CData/*0:0*/ __Vinline__eval_static__TOP_sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__unnamedblk1__DOT__eaIsAreg;
    __Vinline__eval_static__TOP_sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__unnamedblk1__DOT__eaIsAreg = 0;
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->vlNamep);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__rtemp = VL_SCOPED_RAND_RESET_I(17, __VscopeHash, 8012263869315928002ull);
    __Vinline__eval_static__TOP_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__rm = 0;
    __Vinline__eval_static__TOP_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__sm = 0;
    __Vinline__eval_static__TOP_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__dm = 0;
    __Vinline__eval_static__TOP_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__tsm = 0;
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__rowDecoder__DOT__unnamedblk1__DOT__stype = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 5497625345376916888ull);
    __Vinline__eval_static__TOP_sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__unnamedblk1__DOT__eaIsAreg = 0;
    vlSelf->sys_top__DOT__sys__DOT__lcd__DOT__correct__Vstatic__maxa = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 8679475921414794208ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__level_to_gain__Vstatic__e = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 11461992853841695544ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__target = VL_SCOPED_RAND_RESET_I(12, __VscopeHash, 16290112868822641421ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__rate = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 14161047287444608567ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__delta = VL_SCOPED_RAND_RESET_I(13, __VscopeHash, 6268842369917061398ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__up = VL_SCOPED_RAND_RESET_I(13, __VscopeHash, 16066281652313774502ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__dn = VL_SCOPED_RAND_RESET_I(14, __VscopeHash, 4885537314200562256ull);
    vlSelfRef.__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0 
        = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod;
    vlSelfRef.__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__0 
        = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod;
    vlSelfRef.__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1 
        = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod;
    vlSelfRef.__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__1 
        = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod;
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
    vlSelfRef.__Vtrigprevexpr_h4ed98bcf__1 = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
        .__PVT__clk;
    vlSelfRef.__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT____Vcellinp__nanoRom__clk__0 
        = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT____Vcellinp__nanoRom__clk;
}

VL_ATTR_COLD void Vsys_top___024root___eval_static__TOP(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___eval_static__TOP\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__rm;
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__rm = 0;
    CData/*0:0*/ sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__sm;
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__sm = 0;
    CData/*0:0*/ sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__dm;
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__dm = 0;
    CData/*0:0*/ sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__tsm;
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__tsm = 0;
    CData/*0:0*/ sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__unnamedblk1__DOT__eaIsAreg;
    sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__unnamedblk1__DOT__eaIsAreg = 0;
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->vlNamep);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__rtemp = VL_SCOPED_RAND_RESET_I(17, __VscopeHash, 8012263869315928002ull);
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__rm = 0;
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__sm = 0;
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__dm = 0;
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__Vstatic__tsm = 0;
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__rowDecoder__DOT__unnamedblk1__DOT__stype = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 5497625345376916888ull);
    sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__unnamedblk1__DOT__eaIsAreg = 0;
    vlSelf->sys_top__DOT__sys__DOT__lcd__DOT__correct__Vstatic__maxa = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 8679475921414794208ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__level_to_gain__Vstatic__e = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 11461992853841695544ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__target = VL_SCOPED_RAND_RESET_I(12, __VscopeHash, 16290112868822641421ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__rate = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 14161047287444608567ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__delta = VL_SCOPED_RAND_RESET_I(13, __VscopeHash, 6268842369917061398ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__up = VL_SCOPED_RAND_RESET_I(13, __VscopeHash, 16066281652313774502ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__env_next__Vstatic__dn = VL_SCOPED_RAND_RESET_I(14, __VscopeHash, 4885537314200562256ull);
}

VL_ATTR_COLD void Vsys_top___024root___eval_initial__TOP(Vsys_top___024root* vlSelf);

VL_ATTR_COLD void Vsys_top___024root___eval_initial(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___eval_initial\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vsys_top___024root___eval_initial__TOP(vlSelf);
}

VL_ATTR_COLD void Vsys_top___024root___eval_initial__TOP(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___eval_initial__TOP\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[6U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[7U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[10U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[15U] = 0U;
    VL_READMEM_N(false, 68, 336, 0, "nanorom.mem"s,  &(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoRom__DOT__nRam)
                 , 0, ~0ULL);
    VL_READMEM_N(false, 17, 1024, 0, "microrom.mem"s
                 ,  &(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uRom__DOT__uRam)
                 , 0, ~0ULL);
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68L[0U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68H[0U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68L[1U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68H[1U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68L[2U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68H[2U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68L[3U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68H[3U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68L[4U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68H[4U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68L[5U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68H[5U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68L[6U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68H[6U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68L[7U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68H[7U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68L[8U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68H[8U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68L[9U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68H[9U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68L[10U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68H[10U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68L[11U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68H[11U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68L[12U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68H[12U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68L[13U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68H[13U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68L[14U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68H[14U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68L[15U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68H[15U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68L[16U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68H[16U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68L[17U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68H[17U] = 0U;
}

VL_ATTR_COLD void Vsys_top___024root___eval_final(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___eval_final\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vsys_top___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 2> &triggers, const std::string &tag);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vsys_top___024root___eval_phase__stl(Vsys_top___024root* vlSelf);

VL_ATTR_COLD void Vsys_top___024root___eval_settle(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___eval_settle\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VstlIterCount;
    // Body
    __VstlIterCount = 0U;
    vlSelfRef.__VstlFirstIteration = 1U;
    do {
        if (VL_UNLIKELY(((0x00002710U < __VstlIterCount)))) {
#ifdef VL_DEBUG
            Vsys_top___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
#endif
            VL_FATAL_MT("sys_top.sv", 2, "", "DIDNOTCONVERGE: Settle region did not converge after '--converge-limit' of 10000 tries");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        vlSelfRef.__VstlPhaseResult = Vsys_top___024root___eval_phase__stl(vlSelf);
        vlSelfRef.__VstlFirstIteration = 0U;
    } while (vlSelfRef.__VstlPhaseResult);
}

VL_ATTR_COLD void Vsys_top___024root___eval_triggers_vec__stl(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___eval_triggers_vec__stl\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VstlTriggered[1U] = ((0xfffffffffffffffeULL 
                                      & vlSelfRef.__VstlTriggered[1U]) 
                                     | (IData)((IData)(vlSelfRef.__VstlFirstIteration)));
    vlSelfRef.__VstlTriggered[0U] = (QData)((IData)(
                                                    (((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod 
                                                       != vlSelfRef.__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__0) 
                                                      << 1U) 
                                                     | (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod 
                                                        != vlSelfRef.__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0))));
    vlSelfRef.__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0 
        = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod;
    vlSelfRef.__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__0 
        = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod;
    if (VL_UNLIKELY(((1U & (~ (IData)(vlSelfRef.__VstlDidInit)))))) {
        vlSelfRef.__VstlDidInit = 1U;
        vlSelfRef.__VstlTriggered[0U] = (1ULL | vlSelfRef.__VstlTriggered[0U]);
        vlSelfRef.__VstlTriggered[0U] = (2ULL | vlSelfRef.__VstlTriggered[0U]);
    }
}

VL_ATTR_COLD bool Vsys_top___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 2> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vsys_top___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 2> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(Vsys_top___024root___trigger_anySet__stl(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: @([hybrid] sys_top.sys.cpu.Nanod)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 1U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 1 is active: @([hybrid] sys_top.sys.cpu.Irdecod)\n");
    }
    if ((1U & (IData)(triggers[1U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 64 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD bool Vsys_top___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 2> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___trigger_anySet__stl\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        if (in[n]) {
            return (1U);
        }
        n = ((IData)(1U) + n);
    } while ((2U > n));
    return (0U);
}

extern const VlUnpacked<SData/*15:0*/, 16> Vsys_top__ConstPool__TABLE_h3c753ce8_0;

VL_ATTR_COLD void Vsys_top___024root___stl_sequent__TOP__0(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___stl_sequent__TOP__0\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ sys_top__DOT__sys__DOT__cpu__DOT__bgBlock;
    sys_top__DOT__sys__DOT__cpu__DOT__bgBlock = 0;
    CData/*3:0*/ sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa;
    sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa = 0;
    CData/*3:0*/ __Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__0__Vfuncout;
    __Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__0__Vfuncout = 0;
    CData/*5:0*/ __Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__0__eaBits;
    __Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__0__eaBits = 0;
    CData/*3:0*/ __Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__1__Vfuncout;
    __Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__1__Vfuncout = 0;
    CData/*5:0*/ __Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__1__eaBits;
    __Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__1__eaBits = 0;
    CData/*3:0*/ __Vtableidx2;
    __Vtableidx2 = 0;
    CData/*3:0*/ __Vtableidx3;
    __Vtableidx3 = 0;
    CData/*3:0*/ __Vtableidx4;
    __Vtableidx4 = 0;
    SData/*8:0*/ __VdfgRegularize_hebeb780c_0_6;
    __VdfgRegularize_hebeb780c_0_6 = 0;
    // Body
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
    if ((1U & (~ VL_ONEHOT_I((((((((0x0fU == (0x0000000fU 
                                              & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                 >> 8U))) 
                                   << 3U) | ((0x0eU 
                                              == (0x0000000fU 
                                                  & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                     >> 8U))) 
                                             << 2U)) 
                                 | (((0x0dU == (0x0000000fU 
                                                & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                   >> 8U))) 
                                     << 1U) | (0x0cU 
                                               == (0x0000000fU 
                                                   & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                      >> 8U))))) 
                                << 0x0000000cU) | (
                                                   ((((0x0bU 
                                                       == 
                                                       (0x0000000fU 
                                                        & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                           >> 8U))) 
                                                      << 3U) 
                                                     | ((0x0aU 
                                                         == 
                                                         (0x0000000fU 
                                                          & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                             >> 8U))) 
                                                        << 2U)) 
                                                    | (((9U 
                                                         == 
                                                         (0x0000000fU 
                                                          & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                             >> 8U))) 
                                                        << 1U) 
                                                       | (8U 
                                                          == 
                                                          (0x0000000fU 
                                                           & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                              >> 8U))))) 
                                                   << 8U)) 
                              | ((((((7U == (0x0000000fU 
                                             & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                >> 8U))) 
                                     << 3U) | ((6U 
                                                == 
                                                (0x0000000fU 
                                                 & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                    >> 8U))) 
                                               << 2U)) 
                                   | (((5U == (0x0000000fU 
                                               & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                  >> 8U))) 
                                       << 1U) | (4U 
                                                 == 
                                                 (0x0000000fU 
                                                  & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                     >> 8U))))) 
                                  << 4U) | ((((3U == 
                                               (0x0000000fU 
                                                & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                   >> 8U))) 
                                              << 3U) 
                                             | ((2U 
                                                 == 
                                                 (0x0000000fU 
                                                  & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                     >> 8U))) 
                                                << 2U)) 
                                            | (((1U 
                                                 == 
                                                 (0x0000000fU 
                                                  & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                     >> 8U))) 
                                                << 1U) 
                                               | (0U 
                                                  == 
                                                  (0x0000000fU 
                                                   & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                      >> 8U))))))))))) {
        if ((0U == (((((((0x0fU == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                   >> 8U))) 
                         << 3U) | ((0x0eU == (0x0000000fU 
                                              & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                 >> 8U))) 
                                   << 2U)) | (((0x0dU 
                                                == 
                                                (0x0000000fU 
                                                 & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                    >> 8U))) 
                                               << 1U) 
                                              | (0x0cU 
                                                 == 
                                                 (0x0000000fU 
                                                  & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                     >> 8U))))) 
                      << 0x0000000cU) | (((((0x0bU 
                                             == (0x0000000fU 
                                                 & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                    >> 8U))) 
                                            << 3U) 
                                           | ((0x0aU 
                                               == (0x0000000fU 
                                                   & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                      >> 8U))) 
                                              << 2U)) 
                                          | (((9U == 
                                               (0x0000000fU 
                                                & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                   >> 8U))) 
                                              << 1U) 
                                             | (8U 
                                                == 
                                                (0x0000000fU 
                                                 & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                    >> 8U))))) 
                                         << 8U)) | 
                    ((((((7U == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                >> 8U))) 
                         << 3U) | ((6U == (0x0000000fU 
                                           & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                              >> 8U))) 
                                   << 2U)) | (((5U 
                                                == 
                                                (0x0000000fU 
                                                 & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                    >> 8U))) 
                                               << 1U) 
                                              | (4U 
                                                 == 
                                                 (0x0000000fU 
                                                  & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                     >> 8U))))) 
                      << 4U) | ((((3U == (0x0000000fU 
                                          & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                             >> 8U))) 
                                  << 3U) | ((2U == 
                                             (0x0000000fU 
                                              & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                 >> 8U))) 
                                            << 2U)) 
                                | (((1U == (0x0000000fU 
                                            & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                               >> 8U))) 
                                    << 1U) | (0U == 
                                              (0x0000000fU 
                                               & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                  >> 8U))))))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: fx68k.sv:2079: Assertion failed in %m: unique case, but none matched for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.sequencer", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1)
                             , '#',32,(0x0000000fU 
                                       & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                          >> 8U)));
                VL_STOP_MT("../third_party/fx68k/fx68k.sv", 2079, "");
            }
        } else if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
            VL_WRITEF_NX("[%0t] %%Error: fx68k.sv:2079: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.sequencer", 'T',-9
                         , '#',64,VL_TIME_UNITED_Q(1)
                         , '#',32,(0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                  >> 8U)));
            VL_STOP_MT("../third_party/fx68k/fx68k.sv", 2079, "");
        }
    }
    if ((1U & (~ VL_ONEHOT_I((((((((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                   | (0x16U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                                  << 5U) | ((((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                              | ((0x0aU 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                                 | ((2U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                                    | ((0x0cU 
                                                        == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                                       | ((0x0bU 
                                                           == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                                          | (4U 
                                                             == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))))))) 
                                             << 4U) 
                                            | ((0x12U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                               << 3U))) 
                                | (((0x11U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                    << 2U) | (((0x0eU 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                               << 1U) 
                                              | (0x0dU 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))))) 
                               << 6U) | (((((0x14U 
                                             == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                            | (0x10U 
                                               == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                                           << 5U) | 
                                          ((((0x13U 
                                              == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                             | (0x0fU 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                                            << 4U) 
                                           | ((0x15U 
                                               == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                              << 3U))) 
                                         | (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                             << 2U) 
                                            | ((((9U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                                 | ((8U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                                    | (7U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)))) 
                                                << 1U) 
                                               | (5U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)))))))))) {
        if ((0U == (((((((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                         | (0x16U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                        << 5U) | ((((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                    | ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                       | ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                          | ((0x0cU 
                                              == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                             | ((0x0bU 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                                | (4U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))))))) 
                                   << 4U) | ((0x12U 
                                              == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                             << 3U))) 
                      | (((0x11U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                          << 2U) | (((0x0eU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                     << 1U) | (0x0dU 
                                               == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))))) 
                     << 6U) | (((((0x14U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                  | (0x10U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                                 << 5U) | ((((0x13U 
                                              == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                             | (0x0fU 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                                            << 4U) 
                                           | ((0x15U 
                                               == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                              << 3U))) 
                               | (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                   << 2U) | ((((9U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                               | ((8U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                                  | (7U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)))) 
                                              << 1U) 
                                             | (5U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)))))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: fx68kAlu.sv:313: Assertion failed in %m: unique case, but none matched for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit.alu", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1)
                             , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper));
                VL_STOP_MT("../third_party/fx68k/fx68kAlu.sv", 313, "");
            }
        } else if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
            VL_WRITEF_NX("[%0t] %%Error: fx68kAlu.sv:313: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit.alu", 'T',-9
                         , '#',64,VL_TIME_UNITED_Q(1)
                         , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper));
            VL_STOP_MT("../third_party/fx68k/fx68kAlu.sv", 313, "");
        }
    }
    if ((1U & (~ VL_ONEHOT_I(((((((0x0eU == (0x0000000fU 
                                             & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                >> 0x0cU))) 
                                  << 5U) | (((0x0dU 
                                              == (0x0000000fU 
                                                  & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                     >> 0x0cU))) 
                                             << 4U) 
                                            | ((0x0cU 
                                                == 
                                                (0x0000000fU 
                                                 & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                    >> 0x0cU))) 
                                               << 3U))) 
                                | (((0x0bU == (0x0000000fU 
                                               & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                  >> 0x0cU))) 
                                    << 2U) | (((9U 
                                                == 
                                                (0x0000000fU 
                                                 & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                    >> 0x0cU))) 
                                               << 1U) 
                                              | (8U 
                                                 == 
                                                 (0x0000000fU 
                                                  & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                     >> 0x0cU)))))) 
                               << 6U) | ((((7U == (0x0000000fU 
                                                   & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                      >> 0x0cU))) 
                                           << 5U) | 
                                          (((6U == 
                                             (0x0000000fU 
                                              & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                 >> 0x0cU))) 
                                            << 4U) 
                                           | ((5U == 
                                               (0x0000000fU 
                                                & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                   >> 0x0cU))) 
                                              << 3U))) 
                                         | ((((3U == 
                                               (0x0000000fU 
                                                & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                   >> 0x0cU))) 
                                              | ((2U 
                                                  == 
                                                  (0x0000000fU 
                                                   & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                      >> 0x0cU))) 
                                                 | (1U 
                                                    == 
                                                    (0x0000000fU 
                                                     & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                        >> 0x0cU))))) 
                                             << 2U) 
                                            | (((0U 
                                                 == 
                                                 (0x0000000fU 
                                                  & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                     >> 0x0cU))) 
                                                << 1U) 
                                               | (4U 
                                                  == 
                                                  (0x0000000fU 
                                                   & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                      >> 0x0cU))))))))))) {
        if ((0U != ((((((0x0eU == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                  >> 0x0cU))) 
                        << 5U) | (((0x0dU == (0x0000000fU 
                                              & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                 >> 0x0cU))) 
                                   << 4U) | ((0x0cU 
                                              == (0x0000000fU 
                                                  & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                     >> 0x0cU))) 
                                             << 3U))) 
                      | (((0x0bU == (0x0000000fU & 
                                     ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                      >> 0x0cU))) << 2U) 
                         | (((9U == (0x0000000fU & 
                                     ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                      >> 0x0cU))) << 1U) 
                            | (8U == (0x0000000fU & 
                                      ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                       >> 0x0cU)))))) 
                     << 6U) | ((((7U == (0x0000000fU 
                                         & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                            >> 0x0cU))) 
                                 << 5U) | (((6U == 
                                             (0x0000000fU 
                                              & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                 >> 0x0cU))) 
                                            << 4U) 
                                           | ((5U == 
                                               (0x0000000fU 
                                                & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                   >> 0x0cU))) 
                                              << 3U))) 
                               | ((((3U == (0x0000000fU 
                                            & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                               >> 0x0cU))) 
                                    | ((2U == (0x0000000fU 
                                               & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                  >> 0x0cU))) 
                                       | (1U == (0x0000000fU 
                                                 & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                    >> 0x0cU))))) 
                                   << 2U) | (((0U == 
                                               (0x0000000fU 
                                                & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                   >> 0x0cU))) 
                                              << 1U) 
                                             | (4U 
                                                == 
                                                (0x0000000fU 
                                                 & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                    >> 0x0cU))))))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: fx68kAlu.sv:646: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.excUnit.alu.rowDecoder", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1)
                             , '#',32,(0x0000000fU 
                                       & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                          >> 0x0cU)));
                VL_STOP_MT("../third_party/fx68k/fx68kAlu.sv", 646, "");
            }
        }
    }
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prHbit = 0U;
    if ((0x00008000U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prenLatch))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prHbit = 0x0fU;
    }
    if ((0x00004000U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prenLatch))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prHbit = 0x0eU;
    }
    if ((0x00002000U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prenLatch))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prHbit = 0x0dU;
    }
    if ((0x00001000U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prenLatch))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prHbit = 0x0cU;
    }
    if ((0x00000800U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prenLatch))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prHbit = 0x0bU;
    }
    if ((0x00000400U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prenLatch))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prHbit = 0x0aU;
    }
    if ((0x00000200U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prenLatch))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prHbit = 9U;
    }
    if ((0x00000100U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prenLatch))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prHbit = 8U;
    }
    if ((0x00000080U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prenLatch))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prHbit = 7U;
    }
    if ((0x00000040U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prenLatch))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prHbit = 6U;
    }
    if ((0x00000020U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prenLatch))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prHbit = 5U;
    }
    if ((0x00000010U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prenLatch))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prHbit = 4U;
    }
    if ((8U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prenLatch))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prHbit = 3U;
    }
    if ((4U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prenLatch))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prHbit = 2U;
    }
    if ((2U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prenLatch))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prHbit = 1U;
    }
    if ((1U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prenLatch))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prHbit = 0U;
    }
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__isShift = 0U;
    if ((1U != (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) {
        if ((8U != (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) {
            if ((9U != (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) {
                if ((5U != (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) {
                    if ((((((((((0x15U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                || (0x0dU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                               || (0x0eU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                              || (0x0fU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                             || (0x10U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                            || (0x11U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                           || (0x12U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                          || (0x13U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))) 
                         || (0x14U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__isShift = 1U;
                    }
                }
            }
        }
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
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__grp1LatchEn 
        = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch 
           & (0x00000010U != (0x00000012U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__grp0LatchEn 
        = (IData)((0x00000010U == (0x00000012U & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__microLatch)));
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
    vlSelfRef.sys_top__DOT__snd_wr_full = ((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__f_rp) 
                                           == (0x0000003fU 
                                               & ((IData)(1U) 
                                                  + (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__f_wp))));
    vlSelfRef.sys_top__DOT__sctl__DOT__f_empty = ((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__f_wp) 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__f_rp));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busAddrErr 
        = (1U & ((~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__bciByte)) 
                 & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__iplStable 
        = ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__iIpl) 
           == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__rIpl));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__iplComp 
        = ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__iIpl) 
           > (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__pswI));
    vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__fetch_addr_next 
        = (0x003fffffU & ((IData)(4U) + vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v_addr
                          [(0x0000001fU & ((IData)(1U) 
                                           + (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__v)))]));
    if (((((((((4U == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                      >> 0x0cU))) | 
               (0U == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                      >> 0x0cU)))) 
              | (((1U == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                         >> 0x0cU))) 
                  || (2U == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                            >> 0x0cU)))) 
                 || (3U == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                           >> 0x0cU))))) 
             | (5U == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                      >> 0x0cU)))) 
            | (6U == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                     >> 0x0cU)))) | 
           (7U == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                  >> 0x0cU)))) | (8U 
                                                  == 
                                                  (0x0000000fU 
                                                   & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                      >> 0x0cU)))) 
         | (9U == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                  >> 0x0cU))))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__cRow 
            = ((4U == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                      >> 0x0cU))) ? 
               ((0x00000100U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird))
                 ? 0x0040U : ((0U == (7U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                            >> 9U)))
                               ? 0x0400U : ((1U == 
                                             (7U & 
                                              ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                               >> 9U)))
                                             ? 0x0010U
                                             : ((2U 
                                                 == 
                                                 (7U 
                                                  & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                     >> 9U)))
                                                 ? 0x0020U
                                                 : 
                                                ((3U 
                                                  == 
                                                  (7U 
                                                   & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                      >> 9U)))
                                                  ? 0x0800U
                                                  : 
                                                 ((4U 
                                                   == 
                                                   (7U 
                                                    & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                       >> 9U)))
                                                   ? 
                                                  ((0x00000080U 
                                                    & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird))
                                                    ? 0x0100U
                                                    : 0x0200U)
                                                   : 
                                                  ((5U 
                                                    == 
                                                    (7U 
                                                     & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                        >> 9U)))
                                                    ? 0x8000U
                                                    : 0U)))))))
                : ((0U == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                          >> 0x0cU)))
                    ? ((0x00000100U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird))
                        ? ((0x00000080U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird))
                            ? 0x4000U : 0x2000U) : 
                       ((0U == (7U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                      >> 9U))) ? 0x4000U
                         : ((1U == (7U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                          >> 9U))) ? 0x0010U
                             : ((2U == (7U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                              >> 9U)))
                                 ? 0x0020U : ((3U == 
                                               (7U 
                                                & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                   >> 9U)))
                                               ? 4U
                                               : ((4U 
                                                   == 
                                                   (7U 
                                                    & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                       >> 9U)))
                                                   ? 
                                                  ((0x00000080U 
                                                    & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird))
                                                    ? 0x4000U
                                                    : 0x2000U)
                                                   : 
                                                  ((5U 
                                                    == 
                                                    (7U 
                                                     & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                        >> 9U)))
                                                    ? 0x2000U
                                                    : 
                                                   ((6U 
                                                     == 
                                                     (7U 
                                                      & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                         >> 9U)))
                                                     ? 0x0040U
                                                     : 0U))))))))
                    : ((((1U == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                >> 0x0cU))) 
                         || (2U == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                   >> 0x0cU)))) 
                        || (3U == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                  >> 0x0cU))))
                        ? 4U : ((5U == (0x0000000fU 
                                        & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                           >> 0x0cU)))
                                 ? ((3U == (3U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                  >> 6U)))
                                     ? 0x8000U : ((0x00000100U 
                                                   & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird))
                                                   ? 0x0020U
                                                   : 4U))
                                 : ((6U == (0x0000000fU 
                                            & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                               >> 0x0cU)))
                                     ? 0U : ((7U == 
                                              (0x0000000fU 
                                               & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                  >> 0x0cU)))
                                              ? 4U : 
                                             ((8U == 
                                               (0x0000000fU 
                                                & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                   >> 0x0cU)))
                                               ? ((3U 
                                                   == 
                                                   (3U 
                                                    & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                       >> 6U)))
                                                   ? 2U
                                                   : 
                                                  ((IData)(
                                                           (0x0100U 
                                                            == 
                                                            (0x0130U 
                                                             & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird))))
                                                    ? 0x0200U
                                                    : 0x4000U))
                                               : ((IData)(
                                                          ((0x0100U 
                                                            == 
                                                            (0x0130U 
                                                             & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird))) 
                                                           & (~ 
                                                              (3U 
                                                               == 
                                                               (3U 
                                                                & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                                   >> 6U))))))
                                                   ? 0x0400U
                                                   : 0x0020U))))))));
    } else if ((0x0bU == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                         >> 0x0cU)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__cRow 
            = (((((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                  >> 8U) & (~ (3U == (3U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                            >> 6U))))) 
                & (1U != (7U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                >> 3U)))) ? 0x2000U
                : 0x0040U);
    } else if ((0x0cU == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                         >> 0x0cU)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__cRow 
            = ((3U == (3U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                             >> 6U))) ? 0x0080U : ((IData)(
                                                           (0x0100U 
                                                            == 
                                                            (0x0130U 
                                                             & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird))))
                                                    ? 8U
                                                    : 0x0010U));
    } else if ((0x0dU == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                         >> 0x0cU)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__cRow 
            = ((IData)(((0x0100U == (0x0130U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird))) 
                        & (~ (3U == (3U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                           >> 6U))))))
                ? 0x1000U : 4U);
    } else if ((0x0eU == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                         >> 0x0cU)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__rowDecoder__DOT__unnamedblk1__DOT__stype 
            = (3U & ((3U == (3U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                   >> 6U))) ? ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                               >> 9U)
                      : ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                         >> 3U)));
        if (((((((((0U == (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__rowDecoder__DOT__unnamedblk1__DOT__stype) 
                            << 1U) | (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                            >> 8U)))) 
                   | (1U == (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__rowDecoder__DOT__unnamedblk1__DOT__stype) 
                              << 1U) | (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                              >> 8U))))) 
                  | (2U == (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__rowDecoder__DOT__unnamedblk1__DOT__stype) 
                             << 1U) | (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                             >> 8U))))) 
                 | (3U == (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__rowDecoder__DOT__unnamedblk1__DOT__stype) 
                            << 1U) | (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                            >> 8U))))) 
                | (4U == (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__rowDecoder__DOT__unnamedblk1__DOT__stype) 
                           << 1U) | (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                           >> 8U))))) 
               | (5U == (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__rowDecoder__DOT__unnamedblk1__DOT__stype) 
                          << 1U) | (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                          >> 8U))))) 
              | (6U == (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__rowDecoder__DOT__unnamedblk1__DOT__stype) 
                         << 1U) | (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                         >> 8U))))) 
             | (7U == (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__rowDecoder__DOT__unnamedblk1__DOT__stype) 
                        << 1U) | (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                        >> 8U)))))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__cRow 
                = ((0U == (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__rowDecoder__DOT__unnamedblk1__DOT__stype) 
                            << 1U) | (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                            >> 8U))))
                    ? 4U : ((1U == (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__rowDecoder__DOT__unnamedblk1__DOT__stype) 
                                     << 1U) | (1U & 
                                               ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                >> 8U))))
                             ? 8U : ((2U == (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__rowDecoder__DOT__unnamedblk1__DOT__stype) 
                                              << 1U) 
                                             | (1U 
                                                & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                   >> 8U))))
                                      ? 0x0020U : (
                                                   (3U 
                                                    == 
                                                    (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__rowDecoder__DOT__unnamedblk1__DOT__stype) 
                                                      << 1U) 
                                                     | (1U 
                                                        & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                           >> 8U))))
                                                    ? 0x0010U
                                                    : 
                                                   ((4U 
                                                     == 
                                                     (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__rowDecoder__DOT__unnamedblk1__DOT__stype) 
                                                       << 1U) 
                                                      | (1U 
                                                         & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                            >> 8U))))
                                                     ? 0x0100U
                                                     : 
                                                    ((5U 
                                                      == 
                                                      (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__rowDecoder__DOT__unnamedblk1__DOT__stype) 
                                                        << 1U) 
                                                       | (1U 
                                                          & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                             >> 8U))))
                                                      ? 0x0800U
                                                      : 
                                                     ((6U 
                                                       == 
                                                       (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__rowDecoder__DOT__unnamedblk1__DOT__stype) 
                                                         << 1U) 
                                                        | (1U 
                                                           & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                                              >> 8U))))
                                                       ? 0x0400U
                                                       : 0x0200U)))))));
        }
    } else {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__cRow = 0U;
    }
    vlSelfRef.sys_top__DOT__snd_raw = vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__raw
        [vlSelfRef.sys_top__DOT__snd_voice][vlSelfRef.sys_top__DOT__snd_word];
    vlSelfRef.sys_top__DOT__sctl__DOT__wr_done = ((1U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__m_state)) 
                                                  & ((~ (IData)(vlSelfRef.DDRAM_BUSY)) 
                                                     & (IData)(vlSelfRef.DDRAM_WE)));
    vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__t_enabled 
        = ((((0U != (7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tcdcr))) 
             << 3U) | ((0U != (7U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tcdcr) 
                                     >> 4U))) << 2U)) 
           | ((((0U != (0x0000000fU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tbcr))) 
                & (8U != (0x0000000fU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tbcr)))) 
               << 1U) | ((0U != (0x0000000fU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tacr))) 
                         & (8U != (0x0000000fU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tacr))))));
    vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tmode[0U] 
        = (0x0000000fU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tacr));
    vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tmode[1U] 
        = (0x0000000fU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tbcr));
    vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tmode[2U] 
        = (7U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tcdcr) 
                 >> 4U));
    vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tmode[3U] 
        = (7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__mfp__DOT__tcdcr));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
        = ((0xff3fU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll)) 
           | (0x00000080U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                             >> 1U)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
        = (0x00000400U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
        = (0x00008000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
    __Vtableidx3 = (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__abdIsByte)
                                    ? (7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Abd))
                                    : (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Abd)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dcrCode 
        = Vsys_top__ConstPool__TABLE_h3c753ce8_0[__Vtableidx3];
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
    vlSelfRef.sys_top__DOT__sys__DOT__lcd__DOT__sp 
        = (0x0000007fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__lcd__DOT__disp_shift) 
                          + (IData)(vlSelfRef.lcd_pos)));
    sys_top__DOT__sys__DOT__cpu__DOT__bgBlock = ((6U 
                                                  == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase) 
                                                 | ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rAS) 
                                                    & (2U 
                                                       == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase)));
    vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__wr_new 
        = ((vlSelfRef.sys_top__DOT__sctl__DOT__snd__DOT__raw
            [vlSelfRef.sys_top__DOT__sctl__DOT__snd_voice]
            [vlSelfRef.sys_top__DOT__sctl__DOT__snd_word] 
            & (~ (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd_mask))) 
           | ((IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd_data) 
              & (IData)(vlSelfRef.sys_top__DOT__sctl__DOT__snd_mask)));
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
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__iAddrErr 
        = ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__addrOe) 
           & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__rAddrErr));
    __Vtableidx2 = (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                   >> 0x0000000cU));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__lineBmap 
        = Vsys_top__ConstPool__TABLE_h3c753ce8_0[__Vtableidx2];
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[6U] 
        = (0x000003ffU & ((1U == (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                                 >> 8U)))
                           ? ((0U != (0x000000ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir)))
                               ? 0x00000089U : 0x000000a9U)
                           : ((0U != (0x000000ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir)))
                               ? 0x00000308U : 0x00000068U)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[7U] = 0x023bU;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[10U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[15U] = 0U;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__rx 
        = (7U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                 >> 9U));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__ry 
        = (7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__grp1Nma = 0x01c0U;
    if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rExcRst) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__tvn = 0U;
    } else if (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rExcBusErr) 
                | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rExcAdrErr))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__tvn 
            = (2U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rExcAdrErr));
    } else if (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rSpurious) 
                | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rAutovec))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__tvn 
            = ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rSpurious)
                ? 0x0000000cU : 0x0000000dU);
    } else if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rTrace) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__tvn = 9U;
    } else if (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rInterrupt) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__tvn = 0x0fU;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__grp1Nma = 0x01c4U;
    } else {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__tvn 
            = ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rIllegal)
                ? 4U : ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rPriv)
                         ? 8U : ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rLineA)
                                  ? 0x0aU : ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rLineF)
                                              ? 0x0bU
                                              : 1U))));
        if (VL_UNLIKELY(((1U & (~ VL_ONEHOT_I(((((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rLineF) 
                                                 << 3U) 
                                                | ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rLineA) 
                                                   << 2U)) 
                                               | (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rPriv) 
                                                   << 1U) 
                                                  | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rIllegal))))))))) {
            if ((0U != ((((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rLineF) 
                          << 3U) | ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rLineA) 
                                    << 2U)) | (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rPriv) 
                                                << 1U) 
                                               | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rIllegal))))) {
                if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                    VL_WRITEF_NX("[%0t] %%Error: fx68k.sv:2155: Assertion failed in %m: unique case, but multiple matches found for '1'h1'\n",3, 'M',vlSymsp->name(),"sys_top.sys.cpu.sequencer", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1));
                    VL_STOP_MT("../third_party/fx68k/fx68k.sv", 2155, "");
                }
            }
        }
    }
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__enl 
        = ((2U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                  >> 5U)) | (1U & (~ (0U != (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prenLatch)))));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__ms0 
        = ((2U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                  >> 7U)) | (1U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__alue)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__m01 
        = ((8U & ((~ (0U != (0x0000003fU & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__auReg))) 
                  << 3U)) | ((4U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                    >> 6U)) | (3U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__alue))));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks.__PVT__clk 
        = vlSelfRef.clk;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks.__PVT__extReset 
        = vlSelfRef.reset;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks.__PVT__pwrUp 
        = vlSelfRef.reset;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks.__PVT__enPhi1 
        = vlSelfRef.sys_top__DOT__sys__DOT__phi1;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks.__PVT__enPhi2 
        = vlSelfRef.sys_top__DOT__sys__DOT__phi2;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__illMisc = 0U;
    if ((0x00000020U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir))) {
        if ((0x00000010U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir))) {
            if ((8U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__illMisc = 1U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__a1Misc = 0U;
            } else if ((4U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir))) {
                if ((1U & (~ ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                              >> 1U)))) {
                    if ((1U & (~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir)))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__illMisc = 1U;
                    }
                }
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__a1Misc 
                    = ((2U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir))
                        ? ((1U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir))
                            ? 0x012aU : 0x006dU) : 
                       ((1U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir))
                         ? 0x0126U : 0U));
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__a1Misc 
                    = ((2U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir))
                        ? ((1U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir))
                            ? 0x012aU : 0x03a2U) : 
                       ((1U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir))
                         ? 0x0363U : 0x03a6U));
            }
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__a1Misc 
                = ((8U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir))
                    ? 0x0230U : 0x02f5U);
        }
    } else {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__a1Misc 
            = ((0x00000010U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir))
                ? ((8U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir))
                    ? 0x0119U : 0x030bU) : 0x01d0U);
    }
    __Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__1__eaBits 
        = ((0x00000038U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                           >> 3U)) | (7U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                            >> 9U)));
    __Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__1__Vfuncout 
        = ((7U == (7U & ((IData)(__Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__1__eaBits) 
                         >> 3U))) ? ((4U & (IData)(__Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__1__eaBits))
                                      ? ((2U & (IData)(__Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__1__eaBits))
                                          ? 0x0cU : 
                                         ((1U & (IData)(__Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__1__eaBits))
                                           ? 0x0cU : 0x0bU))
                                      : ((2U & (IData)(__Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__1__eaBits))
                                          ? ((1U & (IData)(__Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__1__eaBits))
                                              ? 0x0aU
                                              : 9U)
                                          : ((1U & (IData)(__Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__1__eaBits))
                                              ? 8U : 7U)))
            : (7U & ((IData)(__Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__1__eaBits) 
                     >> 3U)));
    if ((7U != (7U & ((IData)(__Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__1__eaBits) 
                      >> 3U)))) {
        if ((7U == (7U & ((IData)(__Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__1__eaBits) 
                          >> 3U)))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: fx68k.sv:1830: Assertion failed in %m: unique case, but multiple matches found for '3'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.eaDecode", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1)
                             , '#',3,(7U & ((IData)(__Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__1__eaBits) 
                                            >> 3U)));
                VL_STOP_MT("../third_party/fx68k/fx68k.sv", 1830, "");
            }
        }
    }
    sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa 
        = __Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__1__Vfuncout;
    __VdfgRegularize_hebeb780c_0_6 = (6U & (- (IData)(
                                                      ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__coreH) 
                                                       | ((6U 
                                                           != (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                                          & ((0U 
                                                              != 
                                                              (3U 
                                                               & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluLatch) 
                                                                  >> 1U))) 
                                                             & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluLatch) 
                                                                >> 3U)))))));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__psw 
        = (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__pswT) 
            << 0x0000000fU) | (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__pswS) 
                                << 0x0000000dU) | (
                                                   ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__pswI) 
                                                    << 8U) 
                                                   | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__pswCcr))));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT____Vcellinp__shifter__swapWords 
        = (IData)((0U != (0x0082U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row))));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shftRight 
        = ((~ ((0x13U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
               | ((0x11U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                  | ((0x0fU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                     | ((0x0dU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                        | (0x15U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))))))) 
           & ((0x12U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
              | ((0x10U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                 | ((0x0eU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                    | (0x14U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))))));
    __Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__0__eaBits 
        = (0x0000003fU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir));
    __Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__0__Vfuncout 
        = ((7U == (7U & ((IData)(__Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__0__eaBits) 
                         >> 3U))) ? ((4U & (IData)(__Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__0__eaBits))
                                      ? ((2U & (IData)(__Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__0__eaBits))
                                          ? 0x0cU : 
                                         ((1U & (IData)(__Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__0__eaBits))
                                           ? 0x0cU : 0x0bU))
                                      : ((2U & (IData)(__Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__0__eaBits))
                                          ? ((1U & (IData)(__Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__0__eaBits))
                                              ? 0x0aU
                                              : 9U)
                                          : ((1U & (IData)(__Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__0__eaBits))
                                              ? 8U : 7U)))
            : (7U & ((IData)(__Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__0__eaBits) 
                     >> 3U)));
    if ((7U != (7U & ((IData)(__Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__0__eaBits) 
                      >> 3U)))) {
        if ((7U == (7U & ((IData)(__Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__0__eaBits) 
                          >> 3U)))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: fx68k.sv:1830: Assertion failed in %m: unique case, but multiple matches found for '3'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.eaDecode", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1)
                             , '#',3,(7U & ((IData)(__Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__0__eaBits) 
                                            >> 3U)));
                VL_STOP_MT("../third_party/fx68k/fx68k.sv", 1830, "");
            }
        }
    }
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol 
        = __Vfunc_sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaDecode__0__Vfuncout;
    __Vtableidx4 = (0x0000000fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                   >> 0x0000000cU));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot 
        = Vsys_top__ConstPool__TABLE_h3c753ce8_0[__Vtableidx4];
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
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__dobCtrl 
        = ((2U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[1U] 
                  >> 0x00000017U)) | (1U & (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[1U] 
                                            >> 0x00000015U)));
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
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enErrClk 
        = ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__iBusErr) 
           | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__iAddrErr));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__wClk 
        = (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__iAddrErr) 
                 | (((5U != vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase) 
                     & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__wendReg)) 
                    | ((~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__BeI)) 
                       | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Err6591)))));
    if ((1U & (~ VL_ONEHOT_I((((0x0010U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__lineBmap)) 
                               << 1U) | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__lineBmap))))))) {
        if ((0U != (((0x0010U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__lineBmap)) 
                     << 1U) | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__lineBmap))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: fx68k.sv:1859: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1)
                             , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__lineBmap));
                VL_STOP_MT("../third_party/fx68k/fx68k.sv", 1859, "");
            }
        }
    }
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT____Vcellinp__nanoRom__clk 
        = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
        .__PVT__clk;
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT2 
        = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
           .__PVT__enPhi2 & (1U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__tState));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT3 
        = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
           .__PVT__enPhi1 & (2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__tState));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT4 
        = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
           .__PVT__enPhi2 & ((0U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__tState) 
                             | (3U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__tState)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__bcReset 
        = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
           .__PVT__extReset | (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__addrOeDelay) 
                                & (~ ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__BeI) 
                                      | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__BeiDelay)))) 
                               & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Vpai)));
    vlSelfRef.__VdfgRegularize_hebeb780c_0_2 = (0x000001ffU 
                                                & ((0x000000ffU 
                                                    & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluLatch)) 
                                                   - (IData)(__VdfgRegularize_hebeb780c_0_6)));
    vlSelfRef.__VdfgRegularize_hebeb780c_0_7 = (0x000001ffU 
                                                & ((IData)(__VdfgRegularize_hebeb780c_0_6) 
                                                   + 
                                                   (0x000000ffU 
                                                    & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluLatch))));
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
    if ((IData)((0x00c0U == (0x08c0U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir))))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[14U] = 0x03c7U;
        if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xbfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[14U] = 6U;
        } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xbfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[14U] = 0x021cU;
        } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xbfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[14U] = 0x0103U;
        } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xbfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[14U] = 0x01c2U;
        } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xbfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[14U] = 0x01e3U;
        } else if ((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xbfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[14U] = 0x000aU;
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xbfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[14U] = 0x01e2U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00004000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[14U] = 0U;
        }
        if ((1U & (~ VL_ONEHOT_I(((((((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                      << 3U) | ((7U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                << 2U)) 
                                    | (((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                        << 1U) | (5U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) 
                                   << 3U) | (((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                              << 2U) 
                                             | (((3U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 1U) 
                                                | (2U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))))))) {
            if ((0U != ((((((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                            << 3U) | ((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                      << 2U)) | (((6U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                  << 1U) 
                                                 | (5U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) 
                         << 3U) | (((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                    << 2U) | (((3U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (2U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))))) {
                if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:111: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 111, "");
                }
            }
        }
    } else {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[14U] = 0U;
        if ((0x00000080U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir))) {
            if ((0x00000040U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00004000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[14U] = 0U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xbfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[14U] 
                    = ((0x00000020U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir))
                        ? 0x00000386U : 0x00000385U);
            }
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xbfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[14U] 
                = ((0x00000020U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir))
                    ? 0x00000382U : 0x00000381U);
        }
    }
    if (((((((((0U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
               | (2U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) 
              | (3U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) 
             | (4U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) 
            | (5U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) 
           | (6U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) 
          | (7U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) 
         | (8U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)))) {
        if ((0U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) {
            if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                       | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                    | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                   | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x0121U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (2U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x029bU;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x029bU;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x029bU;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x029bU;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x029bU;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x029bU;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x029bU;
            } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x029bU;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x029bU;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x00eaU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x0121U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (2U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0U;
            }
            if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 5U) | 
                                         (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                                        | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 2U) 
                                           | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (6U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                       << 6U) | (((
                                                   (5U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 5U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 4U) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 3U))) 
                                                 | (((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U) 
                                                    | (((1U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 1U) 
                                                       | (0U 
                                                          == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                              | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 1U) 
                                            | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                             << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 5U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 4U) 
                                                   | ((3U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 3U))) 
                                       | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U) | 
                                          (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:905: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 905, "");
                    }
                }
            }
        } else if ((2U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) {
            if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                       | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                    | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                   | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x02faU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (2U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x03abU;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x03abU;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x03abU;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x03abU;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x03abU;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x03abU;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x03abU;
            } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x03abU;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x03abU;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x00eaU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x02faU;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (2U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0U;
            }
            if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 5U) | 
                                         (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                                        | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 2U) 
                                           | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (6U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                       << 6U) | (((
                                                   (5U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 5U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 4U) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 3U))) 
                                                 | (((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U) 
                                                    | (((1U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 1U) 
                                                       | (0U 
                                                          == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                              | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 1U) 
                                            | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                             << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 5U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 4U) 
                                                   | ((3U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 3U))) 
                                       | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U) | 
                                          (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:922: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 922, "");
                    }
                }
            }
        } else if ((3U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) {
            if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                       | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                    | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                   | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x02feU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (2U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x03afU;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x03afU;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x03afU;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x03afU;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x03afU;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x03afU;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x03afU;
            } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x03afU;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x03afU;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x00eaU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x02feU;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (2U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0U;
            }
            if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 5U) | 
                                         (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                                        | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 2U) 
                                           | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (6U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                       << 6U) | (((
                                                   (5U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 5U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 4U) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 3U))) 
                                                 | (((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U) 
                                                    | (((1U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 1U) 
                                                       | (0U 
                                                          == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                              | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 1U) 
                                            | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                             << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 5U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 4U) 
                                                   | ((3U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 3U))) 
                                       | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U) | 
                                          (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:939: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 939, "");
                    }
                }
            }
        } else if ((4U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) {
            if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                       | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                    | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                   | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x02f8U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (2U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x038bU;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x038bU;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x038bU;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x038bU;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x038bU;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x038bU;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x038bU;
            } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x038bU;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x038bU;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x00eaU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x02f8U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (2U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0U;
            }
            if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 5U) | 
                                         (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                                        | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 2U) 
                                           | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (6U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                       << 6U) | (((
                                                   (5U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 5U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 4U) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 3U))) 
                                                 | (((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U) 
                                                    | (((1U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 1U) 
                                                       | (0U 
                                                          == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                              | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 1U) 
                                            | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                             << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 5U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 4U) 
                                                   | ((3U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 3U))) 
                                       | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U) | 
                                          (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:956: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 956, "");
                    }
                }
            }
        } else if ((5U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) {
            if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                       | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                    | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                   | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x02daU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (2U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x038aU;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x038aU;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x038aU;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x038aU;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x038aU;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x038aU;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x038aU;
            } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x038aU;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x038aU;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x00eaU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x02daU;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (2U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0U;
            }
            if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 5U) | 
                                         (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                                        | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 2U) 
                                           | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (6U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                       << 6U) | (((
                                                   (5U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 5U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 4U) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 3U))) 
                                                 | (((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U) 
                                                    | (((1U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 1U) 
                                                       | (0U 
                                                          == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                              | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 1U) 
                                            | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                             << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 5U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 4U) 
                                                   | ((3U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 3U))) 
                                       | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U) | 
                                          (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:973: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 973, "");
                    }
                }
            }
        } else if ((6U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) {
            if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                       | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                    | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                   | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01ebU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (2U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x0298U;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x0298U;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x0298U;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x0298U;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x0298U;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x0298U;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x0298U;
            } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x0298U;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x0298U;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x00eaU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x01ebU;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (2U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0U;
            }
            if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 5U) | 
                                         (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                                        | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 2U) 
                                           | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (6U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                       << 6U) | (((
                                                   (5U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 5U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 4U) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 3U))) 
                                                 | (((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U) 
                                                    | (((1U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 1U) 
                                                       | (0U 
                                                          == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                              | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 1U) 
                                            | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                             << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 5U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 4U) 
                                                   | ((3U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 3U))) 
                                       | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U) | 
                                          (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:990: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 990, "");
                    }
                }
            }
        } else if ((7U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) {
            if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                       | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                    | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                   | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x02d9U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (2U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x0388U;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x0388U;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x0388U;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x0388U;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x0388U;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x0388U;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x0388U;
            } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x0388U;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x0388U;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x00eaU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x02d9U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (2U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0U;
            }
            if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 5U) | 
                                         (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                                        | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 2U) 
                                           | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (6U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                       << 6U) | (((
                                                   (5U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 5U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 4U) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 3U))) 
                                                 | (((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U) 
                                                    | (((1U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 1U) 
                                                       | (0U 
                                                          == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                              | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 1U) 
                                            | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                             << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 5U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 4U) 
                                                   | ((3U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 3U))) 
                                       | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U) | 
                                          (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1007: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1007, "");
                    }
                }
            }
        } else {
            if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                       | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                    | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                   | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01eaU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (2U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x032bU;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x032bU;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x032bU;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x032bU;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x032bU;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x032bU;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x032bU;
            } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x032bU;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x032bU;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffdU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0x00eaU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0x01eaU;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (2U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0U;
            }
            if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 5U) | 
                                         (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                                        | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 2U) 
                                           | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (6U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                       << 6U) | (((
                                                   (5U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 5U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 4U) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 3U))) 
                                                 | (((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U) 
                                                    | (((1U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 1U) 
                                                       | (0U 
                                                          == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                              | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 1U) 
                                            | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                             << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 5U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 4U) 
                                                   | ((3U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 3U))) 
                                       | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U) | 
                                          (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1024: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1024, "");
                    }
                }
            }
        }
    } else {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
            = (2U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[1U] = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[1U] = 0U;
    }
    if ((1U & (~ VL_ONEHOT_I(((((((8U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                  << 3U) | ((7U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                            << 2U)) 
                                | (((6U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                    << 1U) | (5U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)))) 
                               << 4U) | ((((4U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                           << 3U) | 
                                          ((3U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                           << 2U)) 
                                         | (((2U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                             << 1U) 
                                            | (0U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))))))))) {
        if ((0U != ((((((8U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                        << 3U) | ((7U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                  << 2U)) | (((6U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                              << 1U) 
                                             | (5U 
                                                == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)))) 
                     << 4U) | ((((4U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                 << 3U) | ((3U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                           << 2U)) 
                               | (((2U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                   << 1U) | (0U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:902: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1)
                             , '#',32,(IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa));
                VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 902, "");
            }
        }
    }
    if (((((((((0U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
               | (1U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) 
              | (2U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) 
             | (3U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) 
            | (4U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) 
           | (5U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) 
          | (6U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) 
         | (7U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)))) {
        if ((0U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) {
            if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                       | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                    | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                   | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x0129U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x0129U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x000bU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x029fU;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x000fU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x029fU;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x0179U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x029fU;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01c6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x029fU;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01e7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x029fU;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x000eU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x029fU;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01e6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x029fU;
            } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01c6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x029fU;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01e7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x029fU;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x00a7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x0129U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (4U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0U;
            }
            if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 5U) | 
                                         (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                                        | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 2U) 
                                           | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (6U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                       << 6U) | (((
                                                   (5U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 5U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 4U) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 3U))) 
                                                 | (((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U) 
                                                    | (((1U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 1U) 
                                                       | (0U 
                                                          == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                              | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 1U) 
                                            | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                             << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 5U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 4U) 
                                                   | ((3U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 3U))) 
                                       | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U) | 
                                          (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1048: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1048, "");
                    }
                }
            }
        } else if ((1U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) {
            if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                       | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                    | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                   | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x0129U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x0129U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x000bU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x029fU;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x000fU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x029fU;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x0179U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x029fU;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01c6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x029fU;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01e7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x029fU;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x000eU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x029fU;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01e6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x029fU;
            } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01c6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x029fU;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01e7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x029fU;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x00a7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x0129U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (4U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0U;
            }
            if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 5U) | 
                                         (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                                        | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 2U) 
                                           | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (6U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                       << 6U) | (((
                                                   (5U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 5U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 4U) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 3U))) 
                                                 | (((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U) 
                                                    | (((1U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 1U) 
                                                       | (0U 
                                                          == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                              | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 1U) 
                                            | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                             << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 5U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 4U) 
                                                   | ((3U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 3U))) 
                                       | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U) | 
                                          (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1065: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1065, "");
                    }
                }
            }
        } else if ((2U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) {
            if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                       | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                    | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                   | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x02f9U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x02f9U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x000bU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x03a9U;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x000fU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x03a9U;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x0179U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x03a9U;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01c6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x03a9U;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01e7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x03a9U;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x000eU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x03a9U;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01e6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x03a9U;
            } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01c6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x03a9U;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01e7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x03a9U;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x00a7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x02f9U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (4U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0U;
            }
            if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 5U) | 
                                         (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                                        | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 2U) 
                                           | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (6U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                       << 6U) | (((
                                                   (5U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 5U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 4U) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 3U))) 
                                                 | (((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U) 
                                                    | (((1U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 1U) 
                                                       | (0U 
                                                          == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                              | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 1U) 
                                            | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                             << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 5U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 4U) 
                                                   | ((3U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 3U))) 
                                       | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U) | 
                                          (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1082: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1082, "");
                    }
                }
            }
        } else if ((3U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) {
            if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                       | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                    | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                   | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x02fdU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x02fdU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x000bU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x03adU;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x000fU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x03adU;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x0179U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x03adU;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01c6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x03adU;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01e7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x03adU;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x000eU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x03adU;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01e6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x03adU;
            } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01c6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x03adU;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01e7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x03adU;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x00a7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x02fdU;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (4U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0U;
            }
            if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 5U) | 
                                         (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                                        | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 2U) 
                                           | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (6U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                       << 6U) | (((
                                                   (5U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 5U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 4U) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 3U))) 
                                                 | (((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U) 
                                                    | (((1U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 1U) 
                                                       | (0U 
                                                          == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                              | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 1U) 
                                            | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                             << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 5U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 4U) 
                                                   | ((3U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 3U))) 
                                       | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U) | 
                                          (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1099: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1099, "");
                    }
                }
            }
        } else if ((4U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) {
            if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                       | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                    | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                   | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x02fcU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x02fcU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x000bU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x038fU;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x000fU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x038fU;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x0179U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x038fU;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01c6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x038fU;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01e7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x038fU;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x000eU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x038fU;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01e6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x038fU;
            } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01c6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x038fU;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01e7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x038fU;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x00a7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x02fcU;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (4U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0U;
            }
            if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 5U) | 
                                         (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                                        | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 2U) 
                                           | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (6U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                       << 6U) | (((
                                                   (5U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 5U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 4U) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 3U))) 
                                                 | (((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U) 
                                                    | (((1U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 1U) 
                                                       | (0U 
                                                          == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                              | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 1U) 
                                            | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                             << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 5U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 4U) 
                                                   | ((3U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 3U))) 
                                       | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U) | 
                                          (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1116: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1116, "");
                    }
                }
            }
        } else if ((5U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) {
            if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                       | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                    | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                   | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x02deU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x02deU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x000bU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x038eU;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x000fU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x038eU;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x0179U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x038eU;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01c6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x038eU;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01e7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x038eU;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x000eU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x038eU;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01e6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x038eU;
            } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01c6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x038eU;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01e7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x038eU;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x00a7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x02deU;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (4U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0U;
            }
            if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 5U) | 
                                         (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                                        | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 2U) 
                                           | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (6U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                       << 6U) | (((
                                                   (5U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 5U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 4U) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 3U))) 
                                                 | (((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U) 
                                                    | (((1U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 1U) 
                                                       | (0U 
                                                          == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                              | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 1U) 
                                            | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                             << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 5U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 4U) 
                                                   | ((3U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 3U))) 
                                       | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U) | 
                                          (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1133: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1133, "");
                    }
                }
            }
        } else if ((6U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) {
            if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                       | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                    | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                   | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01efU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01efU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x000bU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x029cU;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x000fU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x029cU;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x0179U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x029cU;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01c6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x029cU;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01e7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x029cU;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x000eU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x029cU;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01e6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x029cU;
            } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01c6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x029cU;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01e7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x029cU;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x00a7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x01efU;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (4U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0U;
            }
            if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 5U) | 
                                         (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                                        | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 2U) 
                                           | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (6U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                       << 6U) | (((
                                                   (5U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 5U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 4U) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 3U))) 
                                                 | (((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U) 
                                                    | (((1U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 1U) 
                                                       | (0U 
                                                          == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                              | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 1U) 
                                            | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                             << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 5U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 4U) 
                                                   | ((3U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 3U))) 
                                       | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U) | 
                                          (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1150: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1150, "");
                    }
                }
            }
        } else {
            if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                       | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                    | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                   | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x02ddU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x02ddU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x000bU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x038cU;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x000fU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x038cU;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x0179U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x038cU;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01c6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x038cU;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01e7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x038cU;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x000eU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x038cU;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01e6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x038cU;
            } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01c6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x038cU;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01e7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x038cU;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x00a7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x02ddU;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (4U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0U;
            }
            if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 5U) | 
                                         (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                                        | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 2U) 
                                           | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (6U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                       << 6U) | (((
                                                   (5U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 5U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 4U) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 3U))) 
                                                 | (((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U) 
                                                    | (((1U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 1U) 
                                                       | (0U 
                                                          == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                              | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 1U) 
                                            | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                             << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 5U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 4U) 
                                                   | ((3U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 3U))) 
                                       | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U) | 
                                          (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1167: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1167, "");
                    }
                }
            }
        }
    } else if ((8U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) {
        if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                   | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
               | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
              | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
             | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
            if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01eeU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01eeU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x000bU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x030fU;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x000fU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x030fU;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x0179U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x030fU;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01c6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x030fU;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01e7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x030fU;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x000eU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x030fU;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01e6U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x030fU;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01c6U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x030fU;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x01e7U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x030fU;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffbU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0x00a7U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0x01eeU;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (4U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0U;
        }
        if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                      << 5U) | (((0x0aU 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 4U) 
                                                | ((9U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 3U))) 
                                    | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                        << 2U) | ((
                                                   (7U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 1U) 
                                                  | (6U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                   << 6U) | ((((5U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 5U) 
                                              | (((4U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                  << 4U) 
                                                 | ((3U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 3U))) 
                                             | (((2U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 2U) 
                                                | (((1U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 1U) 
                                                   | (0U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
            if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                            << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                       << 4U) | ((9U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 3U))) 
                          | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                              << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 1U) | (6U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                         << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                     << 5U) | (((4U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                << 4U) 
                                               | ((3U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                  << 3U))) 
                                   | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                       << 2U) | (((1U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                  << 1U) 
                                                 | (0U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1184: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1184, "");
                }
            }
        }
    } else {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
            = (4U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[2U] = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[2U] = 0U;
    }
    if ((1U & (~ VL_ONEHOT_I(((((((8U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                  << 4U) | (((7U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                             << 3U) 
                                            | ((6U 
                                                == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                               << 2U))) 
                                | (((5U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                    << 1U) | (4U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)))) 
                               << 4U) | ((((3U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                           << 3U) | 
                                          ((2U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                           << 2U)) 
                                         | (((1U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                             << 1U) 
                                            | (0U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))))))))) {
        if ((0U != ((((((8U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                        << 4U) | (((7U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                   << 3U) | ((6U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                             << 2U))) 
                      | (((5U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                          << 1U) | (4U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)))) 
                     << 4U) | ((((3U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                 << 3U) | ((2U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                           << 2U)) 
                               | (((1U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                   << 1U) | (0U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1045: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1)
                             , '#',32,(IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa));
                VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1045, "");
            }
        }
    }
    if (((((((((0U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
               | (1U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) 
              | (2U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) 
             | (3U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) 
            | (4U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) 
           | (5U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) 
          | (6U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) 
         | (7U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)))) {
        if ((0U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) {
            if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                       | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                    | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                   | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x0121U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x0121U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x029bU;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x029bU;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x029bU;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x029bU;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x029bU;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x029bU;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x029bU;
            } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x029bU;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x029bU;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x00eaU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x0121U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (8U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0U;
            }
            if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 5U) | 
                                         (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                                        | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 2U) 
                                           | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (6U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                       << 6U) | (((
                                                   (5U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 5U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 4U) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 3U))) 
                                                 | (((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U) 
                                                    | (((1U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 1U) 
                                                       | (0U 
                                                          == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                              | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 1U) 
                                            | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                             << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 5U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 4U) 
                                                   | ((3U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 3U))) 
                                       | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U) | 
                                          (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1208: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1208, "");
                    }
                }
            }
        } else if ((1U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) {
            if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                       | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                    | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                   | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x0279U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x0279U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x0158U;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x0158U;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x0158U;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x0158U;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x0158U;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x0158U;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x0158U;
            } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x0158U;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x0158U;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x00eaU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x0279U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (8U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0U;
            }
            if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 5U) | 
                                         (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                                        | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 2U) 
                                           | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (6U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                       << 6U) | (((
                                                   (5U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 5U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 4U) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 3U))) 
                                                 | (((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U) 
                                                    | (((1U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 1U) 
                                                       | (0U 
                                                          == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                              | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 1U) 
                                            | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                             << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 5U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 4U) 
                                                   | ((3U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 3U))) 
                                       | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U) | 
                                          (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1225: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1225, "");
                    }
                }
            }
        } else if ((2U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) {
            if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                       | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                    | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                   | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x02faU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x02faU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x03abU;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x03abU;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x03abU;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x03abU;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x03abU;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x03abU;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x03abU;
            } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x03abU;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x03abU;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x00eaU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x02faU;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (8U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0U;
            }
            if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 5U) | 
                                         (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                                        | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 2U) 
                                           | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (6U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                       << 6U) | (((
                                                   (5U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 5U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 4U) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 3U))) 
                                                 | (((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U) 
                                                    | (((1U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 1U) 
                                                       | (0U 
                                                          == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                              | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 1U) 
                                            | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                             << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 5U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 4U) 
                                                   | ((3U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 3U))) 
                                       | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U) | 
                                          (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1242: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1242, "");
                    }
                }
            }
        } else if ((3U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) {
            if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                       | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                    | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                   | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x02feU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x02feU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x03afU;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x03afU;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x03afU;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x03afU;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x03afU;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x03afU;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x03afU;
            } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x03afU;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x03afU;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x00eaU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x02feU;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (8U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0U;
            }
            if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 5U) | 
                                         (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                                        | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 2U) 
                                           | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (6U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                       << 6U) | (((
                                                   (5U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 5U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 4U) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 3U))) 
                                                 | (((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U) 
                                                    | (((1U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 1U) 
                                                       | (0U 
                                                          == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                              | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 1U) 
                                            | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                             << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 5U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 4U) 
                                                   | ((3U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 3U))) 
                                       | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U) | 
                                          (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1259: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1259, "");
                    }
                }
            }
        } else if ((4U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) {
            if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                       | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                    | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                   | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x02f8U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x02f8U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x038bU;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x038bU;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x038bU;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x038bU;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x038bU;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x038bU;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x038bU;
            } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x038bU;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x038bU;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x00eaU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x02f8U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (8U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0U;
            }
            if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 5U) | 
                                         (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                                        | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 2U) 
                                           | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (6U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                       << 6U) | (((
                                                   (5U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 5U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 4U) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 3U))) 
                                                 | (((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U) 
                                                    | (((1U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 1U) 
                                                       | (0U 
                                                          == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                              | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 1U) 
                                            | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                             << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 5U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 4U) 
                                                   | ((3U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 3U))) 
                                       | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U) | 
                                          (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1276: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1276, "");
                    }
                }
            }
        } else if ((5U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) {
            if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                       | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                    | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                   | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x02daU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x02daU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x038aU;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x038aU;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x038aU;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x038aU;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x038aU;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x038aU;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x038aU;
            } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x038aU;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x038aU;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x00eaU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x02daU;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (8U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0U;
            }
            if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 5U) | 
                                         (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                                        | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 2U) 
                                           | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (6U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                       << 6U) | (((
                                                   (5U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 5U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 4U) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 3U))) 
                                                 | (((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U) 
                                                    | (((1U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 1U) 
                                                       | (0U 
                                                          == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                              | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 1U) 
                                            | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                             << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 5U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 4U) 
                                                   | ((3U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 3U))) 
                                       | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U) | 
                                          (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1293: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1293, "");
                    }
                }
            }
        } else if ((6U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) {
            if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                       | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                    | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                   | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01ebU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01ebU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x0298U;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x0298U;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x0298U;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x0298U;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x0298U;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x0298U;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x0298U;
            } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x0298U;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x0298U;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x00eaU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x01ebU;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (8U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0U;
            }
            if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 5U) | 
                                         (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                                        | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 2U) 
                                           | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (6U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                       << 6U) | (((
                                                   (5U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 5U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 4U) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 3U))) 
                                                 | (((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U) 
                                                    | (((1U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 1U) 
                                                       | (0U 
                                                          == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                              | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 1U) 
                                            | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                             << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 5U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 4U) 
                                                   | ((3U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 3U))) 
                                       | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U) | 
                                          (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1310: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1310, "");
                    }
                }
            }
        } else {
            if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                       | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                    | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                   | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x02d9U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x02d9U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x0388U;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x0388U;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x0388U;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x0388U;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x0388U;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x0388U;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x0388U;
            } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x0388U;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x0388U;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x00eaU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x02d9U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (8U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0U;
            }
            if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 5U) | 
                                         (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                                        | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 2U) 
                                           | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (6U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                       << 6U) | (((
                                                   (5U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 5U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 4U) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 3U))) 
                                                 | (((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U) 
                                                    | (((1U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 1U) 
                                                       | (0U 
                                                          == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                              | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 1U) 
                                            | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                             << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 5U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 4U) 
                                                   | ((3U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 3U))) 
                                       | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U) | 
                                          (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1327: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1327, "");
                    }
                }
            }
        }
    } else if ((8U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))) {
        if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                   | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
               | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
              | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
             | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
            if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01eaU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01eaU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x032bU;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x032bU;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x032bU;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x032bU;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x032bU;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x032bU;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x032bU;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01c2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x032bU;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x01e3U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x032bU;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfff7U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0x00eaU;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0x01eaU;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (8U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0U;
        }
        if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                      << 5U) | (((0x0aU 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 4U) 
                                                | ((9U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 3U))) 
                                    | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                        << 2U) | ((
                                                   (7U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 1U) 
                                                  | (6U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                   << 6U) | ((((5U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 5U) 
                                              | (((4U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                  << 4U) 
                                                 | ((3U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 3U))) 
                                             | (((2U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 2U) 
                                                | (((1U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 1U) 
                                                   | (0U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
            if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                            << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                       << 4U) | ((9U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 3U))) 
                          | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                              << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 1U) | (6U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                         << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                     << 5U) | (((4U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                << 4U) 
                                               | ((3U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                  << 3U))) 
                                   | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                       << 2U) | (((1U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                  << 1U) 
                                                 | (0U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1344: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1344, "");
                }
            }
        }
    } else {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
            = (8U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[3U] = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[3U] = 0U;
    }
    if ((1U & (~ VL_ONEHOT_I(((((((8U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                  << 4U) | (((7U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                             << 3U) 
                                            | ((6U 
                                                == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                               << 2U))) 
                                | (((5U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                    << 1U) | (4U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)))) 
                               << 4U) | ((((3U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                           << 3U) | 
                                          ((2U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                           << 2U)) 
                                         | (((1U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                             << 1U) 
                                            | (0U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))))))))) {
        if ((0U != ((((((8U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                        << 4U) | (((7U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                   << 3U) | ((6U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                             << 2U))) 
                      | (((5U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                          << 1U) | (4U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)))) 
                     << 4U) | ((((3U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                 << 3U) | ((2U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                           << 2U)) 
                               | (((1U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa)) 
                                   << 1U) | (0U == (IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa))))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1205: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1)
                             , '#',32,(IData)(sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__movEa));
                VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1205, "");
            }
        }
    }
    if ((0x00000100U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir))) {
        if ((0x00000080U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir))) {
            if ((0x00000040U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir))) {
                if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                           | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                          | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                         | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                        | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                       | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                    if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x0384U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x00aeU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01c5U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
                    } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x006cU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0x00000100U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01c5U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
                    } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 6U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x0380U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 6U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x00acU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x000bU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01cbU;
                    } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x021cU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x0380U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x021cU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x00acU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x000fU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01cbU;
                    } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x0103U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x0380U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x0103U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x00acU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x0179U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01cbU;
                    } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x01c2U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x0380U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01c2U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x00acU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01c6U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01cbU;
                    } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x01e3U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x0380U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01e3U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x00acU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01e7U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01cbU;
                    } else {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x000aU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x0380U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x000aU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x00acU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x000eU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01cbU;
                    }
                } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x01e2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x0380U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01e2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x00acU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01e6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01cbU;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000020U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0U;
                    if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01c2U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x00acU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01c6U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01cbU;
                    } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01e3U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x00acU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01e7U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01cbU;
                    } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x00eaU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x00aeU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x00a7U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c5U;
                    } else {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0x00000100U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0x00000200U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
                    }
                }
                if ((1U & (~ VL_ONEHOT_I(((((((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                              << 4U) 
                                             | (((7U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 3U) 
                                                | ((6U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 2U))) 
                                            | (((5U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                << 1U) 
                                               | (4U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) 
                                           << 4U) | 
                                          ((((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 3U) 
                                            | ((2U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 2U)) 
                                           | (((1U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (0U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))))))) {
                    if ((0U != ((((((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                    << 4U) | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 3U) 
                                              | ((6U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 2U))) 
                                  | (((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                      << 1U) | (4U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) 
                                 << 4U) | ((((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 3U) 
                                            | ((2U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 2U)) 
                                           | (((1U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (0U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))))) {
                        if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                            VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1466: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                         , '#',64,VL_TIME_UNITED_Q(1)
                                         , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                            VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1466, "");
                        }
                    }
                }
                if ((1U & (~ VL_ONEHOT_I(((((((0x0bU 
                                               == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                              << 5U) 
                                             | (((0x0aU 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 4U) 
                                                | ((9U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 3U))) 
                                            | (((8U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                << 2U) 
                                               | (((7U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 1U) 
                                                  | (6U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                           << 6U) | 
                                          ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 5U) 
                                            | (((4U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                << 4U) 
                                               | ((3U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                  << 3U))) 
                                           | (((2U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 2U) 
                                              | (((1U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                  << 1U) 
                                                 | (0U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                    if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                    << 5U) | (((0x0aU 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 4U) 
                                              | ((9U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 3U))) 
                                  | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                      << 2U) | (((7U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 1U) 
                                                | (6U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                 << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 5U) 
                                            | (((4U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                << 4U) 
                                               | ((3U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                  << 3U))) 
                                           | (((2U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 2U) 
                                              | (((1U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                  << 1U) 
                                                 | (0U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                        if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                            VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1605: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                         , '#',64,VL_TIME_UNITED_Q(1)
                                         , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                            VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1605, "");
                            VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1747: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                         , '#',64,VL_TIME_UNITED_Q(1)
                                         , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                            VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1747, "");
                        }
                    }
                }
            } else {
                if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                           | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                          | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                         | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                        | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                       | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                    if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x02dcU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0x00000100U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01c5U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
                    } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x02dcU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0x00000100U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x010bU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
                    } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x000bU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f7U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x000bU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x029dU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x000bU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x029dU;
                    } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x000fU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f7U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x000fU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x029dU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x000fU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x029dU;
                    } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x0179U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f7U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x0179U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x029dU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x0179U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x029dU;
                    } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x01c6U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f7U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01c6U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x029dU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01c6U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x029dU;
                    } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x01e7U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f7U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01e7U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x029dU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01e7U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x029dU;
                    } else {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x000eU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f7U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x000eU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x029dU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x000eU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x029dU;
                    }
                } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x01e6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01e6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x029dU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01e6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x029dU;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000020U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0U;
                    if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0x00000100U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0x00000200U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
                    } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0x00000100U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0x00000200U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
                    } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0x00000100U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0x00000200U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
                    } else {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0x00000100U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0x00000200U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
                    }
                }
                if ((1U & (~ VL_ONEHOT_I(((((((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                              << 4U) 
                                             | (((7U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 3U) 
                                                | ((6U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 2U))) 
                                            | (((5U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                << 1U) 
                                               | (4U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) 
                                           << 4U) | 
                                          ((((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 3U) 
                                            | ((2U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 2U)) 
                                           | (((1U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (0U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))))))) {
                    if ((0U != ((((((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                    << 4U) | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 3U) 
                                              | ((6U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 2U))) 
                                  | (((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                      << 1U) | (4U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) 
                                 << 4U) | ((((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 3U) 
                                            | ((2U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 2U)) 
                                           | (((1U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (0U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))))) {
                        if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                            VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1452: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                         , '#',64,VL_TIME_UNITED_Q(1)
                                         , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                            VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1452, "");
                        }
                    }
                }
                if ((1U & (~ VL_ONEHOT_I(((((((0x0bU 
                                               == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                              << 5U) 
                                             | (((0x0aU 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 4U) 
                                                | ((9U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 3U))) 
                                            | (((8U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                << 2U) 
                                               | (((7U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 1U) 
                                                  | (6U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                           << 6U) | 
                                          ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 5U) 
                                            | (((4U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                << 4U) 
                                               | ((3U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                  << 3U))) 
                                           | (((2U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 2U) 
                                              | (((1U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                  << 1U) 
                                                 | (0U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                    if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                    << 5U) | (((0x0aU 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 4U) 
                                              | ((9U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 3U))) 
                                  | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                      << 2U) | (((7U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 1U) 
                                                | (6U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                 << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 5U) 
                                            | (((4U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                << 4U) 
                                               | ((3U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                  << 3U))) 
                                           | (((2U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 2U) 
                                              | (((1U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                  << 1U) 
                                                 | (0U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                        if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                            VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1588: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                         , '#',64,VL_TIME_UNITED_Q(1)
                                         , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                            VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1588, "");
                            VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1730: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                         , '#',64,VL_TIME_UNITED_Q(1)
                                         , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                            VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1730, "");
                        }
                    }
                }
            }
        } else if ((0x00000040U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir))) {
            if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                       | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                    | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                   | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x02d8U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000100U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01c1U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x02dcU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000100U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x010fU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x0299U;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x0299U;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x0299U;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x0299U;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x0299U;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x0299U;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x0299U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x0299U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000020U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0U;
                if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000100U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000200U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
                } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000100U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000200U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
                } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000100U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000200U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000100U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000200U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
                }
            }
            if ((1U & (~ VL_ONEHOT_I(((((((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 4U) | 
                                         (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U) | 
                                          ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U))) 
                                        | (((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) 
                                       << 4U) | (((
                                                   (3U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 3U) 
                                                  | ((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U)) 
                                                 | (((1U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 1U) 
                                                    | (0U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))))))) {
                if ((0U != ((((((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 4U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U) | 
                                          ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U))) 
                              | (((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 1U) | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) 
                             << 4U) | ((((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 3U) | (
                                                   (2U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 2U)) 
                                       | (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 1U) | 
                                          (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1438: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1438, "");
                    }
                }
            }
            if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 5U) | 
                                         (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                                        | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 2U) 
                                           | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (6U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                       << 6U) | (((
                                                   (5U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 5U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 4U) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 3U))) 
                                                 | (((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U) 
                                                    | (((1U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 1U) 
                                                       | (0U 
                                                          == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                              | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 1U) 
                                            | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                             << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 5U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 4U) 
                                                   | ((3U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 3U))) 
                                       | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U) | 
                                          (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1571: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1571, "");
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1713: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1713, "");
                    }
                }
            }
        } else {
            if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                       | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                    | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                   | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x02d8U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01cdU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01c1U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000020U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x0107U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x010fU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x0299U;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x0299U;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x0299U;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x0299U;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x0299U;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x0299U;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x0299U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x0299U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000020U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0U;
                if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000100U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000200U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
                } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000100U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000200U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
                } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000100U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000200U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000100U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000200U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
                }
            }
            if ((1U & (~ VL_ONEHOT_I(((((((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 4U) | 
                                         (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U) | 
                                          ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U))) 
                                        | (((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) 
                                       << 4U) | (((
                                                   (3U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 3U) 
                                                  | ((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U)) 
                                                 | (((1U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 1U) 
                                                    | (0U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))))))) {
                if ((0U != ((((((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 4U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U) | 
                                          ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U))) 
                              | (((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 1U) | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) 
                             << 4U) | ((((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 3U) | (
                                                   (2U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 2U)) 
                                       | (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 1U) | 
                                          (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1424: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1424, "");
                    }
                }
            }
            if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 5U) | 
                                         (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                                        | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 2U) 
                                           | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (6U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                       << 6U) | (((
                                                   (5U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 5U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 4U) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 3U))) 
                                                 | (((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U) 
                                                    | (((1U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 1U) 
                                                       | (0U 
                                                          == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                              | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 1U) 
                                            | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                             << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 5U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 4U) 
                                                   | ((3U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 3U))) 
                                       | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U) | 
                                          (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1554: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1554, "");
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1696: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1696, "");
                    }
                }
            }
        }
    } else if ((0x00000080U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir))) {
        if ((0x00000040U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir))) {
            if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                       | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                    | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                   | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x0384U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x00a6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01c9U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x006cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000100U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01c9U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x0380U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x00a4U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c7U;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x0380U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x00a4U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c7U;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x0380U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x00a4U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c7U;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x0380U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x00a4U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c7U;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x0380U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x00a4U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c7U;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x0380U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x00a4U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c7U;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x0380U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x00a4U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c7U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000020U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0U;
                if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x00a4U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c7U;
                } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x00a4U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c7U;
                } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x00eaU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x00a6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x00eaU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c9U;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000100U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000200U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
                }
            }
            if ((1U & (~ VL_ONEHOT_I(((((((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 4U) | 
                                         (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U) | 
                                          ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U))) 
                                        | (((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) 
                                       << 4U) | (((
                                                   (3U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 3U) 
                                                  | ((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U)) 
                                                 | (((1U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 1U) 
                                                    | (0U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))))))) {
                if ((0U != ((((((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 4U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U) | 
                                          ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U))) 
                              | (((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 1U) | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) 
                             << 4U) | ((((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 3U) | (
                                                   (2U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 2U)) 
                                       | (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 1U) | 
                                          (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1410: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1410, "");
                    }
                }
            }
            if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 5U) | 
                                         (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                                        | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 2U) 
                                           | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (6U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                       << 6U) | (((
                                                   (5U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 5U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 4U) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 3U))) 
                                                 | (((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U) 
                                                    | (((1U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 1U) 
                                                       | (0U 
                                                          == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                              | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 1U) 
                                            | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                             << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 5U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 4U) 
                                                   | ((3U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 3U))) 
                                       | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U) | 
                                          (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1537: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1537, "");
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1679: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1679, "");
                    }
                }
            }
        } else {
            if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                       | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                      | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                     | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                    | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                   | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
                if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x02dcU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01c5U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01c5U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x02dcU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000100U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01c5U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x000bU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x000bU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01cbU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x000bU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01cbU;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x000fU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x000fU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01cbU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x000fU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01cbU;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x0179U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x0179U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01cbU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x0179U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01cbU;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x01c6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01c6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01cbU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01c6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01cbU;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x01e7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01e7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01cbU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01e7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01cbU;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x000eU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x000eU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01cbU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x000eU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01cbU;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x01e6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01e6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01cbU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01e6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01cbU;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000020U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0U;
                if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01c6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01cbU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01c6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01cbU;
                } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01e7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01cbU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01e7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01cbU;
                } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x00a7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01c5U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x00a7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c5U;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000100U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000200U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
                }
            }
            if ((1U & (~ VL_ONEHOT_I(((((((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 4U) | 
                                         (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U) | 
                                          ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U))) 
                                        | (((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) 
                                       << 4U) | (((
                                                   (3U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 3U) 
                                                  | ((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U)) 
                                                 | (((1U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 1U) 
                                                    | (0U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))))))) {
                if ((0U != ((((((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 4U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U) | 
                                          ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U))) 
                              | (((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 1U) | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) 
                             << 4U) | ((((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 3U) | (
                                                   (2U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 2U)) 
                                       | (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 1U) | 
                                          (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1396: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1396, "");
                    }
                }
            }
            if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                          << 5U) | 
                                         (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                                        | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 2U) 
                                           | (((7U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 1U) 
                                              | (6U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                       << 6U) | (((
                                                   (5U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 5U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 4U) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 3U))) 
                                                 | (((2U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                     << 2U) 
                                                    | (((1U 
                                                         == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                        << 1U) 
                                                       | (0U 
                                                          == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
                if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 4U) | 
                                          ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 3U))) 
                              | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                  << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                             << 1U) 
                                            | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                             << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 5U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 4U) 
                                                   | ((3U 
                                                       == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                      << 3U))) 
                                       | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                           << 2U) | 
                                          (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                            << 1U) 
                                           | (0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1520: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1520, "");
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1662: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1662, "");
                    }
                }
            }
        }
    } else if ((0x00000040U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir))) {
        if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                   | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
               | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
              | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
             | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
            if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x02d8U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01c1U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01c1U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x02dcU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000100U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01c1U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01c3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c3U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01c3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c3U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01c3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c3U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01c3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c3U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01c3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c3U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01c3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c3U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f3U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01c3U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c3U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000020U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0U;
            if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01c3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c3U;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01c3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c3U;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x00eaU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01c1U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x00eaU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c1U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000100U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000200U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
            }
        }
        if ((1U & (~ VL_ONEHOT_I(((((((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                      << 4U) | (((7U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 3U) 
                                                | ((6U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 2U))) 
                                    | (((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                        << 1U) | (4U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) 
                                   << 4U) | ((((3U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 3U) 
                                              | ((2U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 2U)) 
                                             | (((1U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 1U) 
                                                | (0U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))))))) {
            if ((0U != ((((((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                            << 4U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                       << 3U) | ((6U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 2U))) 
                          | (((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                              << 1U) | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) 
                         << 4U) | ((((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                     << 3U) | ((2U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 2U)) 
                                   | (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                       << 1U) | (0U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))))) {
                if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1382: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1382, "");
                }
            }
        }
        if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                      << 5U) | (((0x0aU 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 4U) 
                                                | ((9U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 3U))) 
                                    | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                        << 2U) | ((
                                                   (7U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 1U) 
                                                  | (6U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                   << 6U) | ((((5U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 5U) 
                                              | (((4U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                  << 4U) 
                                                 | ((3U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 3U))) 
                                             | (((2U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 2U) 
                                                | (((1U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 1U) 
                                                   | (0U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
            if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                            << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                       << 4U) | ((9U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 3U))) 
                          | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                              << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 1U) | (6U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                         << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                     << 5U) | (((4U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                << 4U) 
                                               | ((3U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                  << 3U))) 
                                   | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                       << 2U) | (((1U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                  << 1U) 
                                                 | (0U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1503: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1503, "");
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1645: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1645, "");
                }
            }
        }
    } else {
        if (((((((((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                   | (1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                  | (2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                 | (3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
                | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
               | (5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
              | (6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) 
             | (7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) {
            if ((0U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x02d8U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01c1U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01c1U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000020U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000100U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000200U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01c3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c3U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01c3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c3U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01c3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c3U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01c3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c3U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01c3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c3U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01c3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c3U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffdfU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0x02f3U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01c3U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c3U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000020U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[5U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[5U] = 0U;
            if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01c3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c3U;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01c3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c3U;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfeffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0x00eaU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0x01c1U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfdffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0x00eaU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0x01c1U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000100U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[8U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[8U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000200U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[9U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[9U] = 0U;
            }
        }
        if ((1U & (~ VL_ONEHOT_I(((((((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                      << 4U) | (((7U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 3U) 
                                                | ((6U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 2U))) 
                                    | (((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                        << 1U) | (4U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) 
                                   << 4U) | ((((3U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 3U) 
                                              | ((2U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 2U)) 
                                             | (((1U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 1U) 
                                                | (0U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))))))) {
            if ((0U != ((((((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                            << 4U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                       << 3U) | ((6U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 2U))) 
                          | (((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                              << 1U) | (4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))) 
                         << 4U) | ((((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                     << 3U) | ((2U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 2U)) 
                                   | (((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                       << 1U) | (0U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))))) {
                if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1368: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1368, "");
                }
            }
        }
        if ((1U & (~ VL_ONEHOT_I(((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                      << 5U) | (((0x0aU 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 4U) 
                                                | ((9U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 3U))) 
                                    | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                        << 2U) | ((
                                                   (7U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                   << 1U) 
                                                  | (6U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                                   << 6U) | ((((5U 
                                                == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                               << 5U) 
                                              | (((4U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                  << 4U) 
                                                 | ((3U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 3U))) 
                                             | (((2U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 2U) 
                                                | (((1U 
                                                     == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                    << 1U) 
                                                   | (0U 
                                                      == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))))) {
            if ((0U != ((((((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                            << 5U) | (((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                       << 4U) | ((9U 
                                                  == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                 << 3U))) 
                          | (((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                              << 2U) | (((7U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                         << 1U) | (6U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))))) 
                         << 6U) | ((((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                     << 5U) | (((4U 
                                                 == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                << 4U) 
                                               | ((3U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                  << 3U))) 
                                   | (((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                       << 2U) | (((1U 
                                                   == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)) 
                                                  << 1U) 
                                                 | (0U 
                                                    == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol)))))))) {
                if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1486: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1486, "");
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1628: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1628, "");
                }
            }
        }
    }
}
