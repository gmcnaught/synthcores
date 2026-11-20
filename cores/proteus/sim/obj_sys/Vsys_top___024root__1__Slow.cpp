// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vsys_top.h for the primary calling header

#include "Vsys_top__pch.h"

extern const VlUnpacked<CData/*6:0*/, 256> Vsys_top__ConstPool__TABLE_h1ee1637f_0;

VL_ATTR_COLD void Vsys_top___024root___stl_sequent__TOP__2(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___stl_sequent__TOP__2\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__unnamedblk1__DOT__eaIsAreg;
    sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__unnamedblk1__DOT__eaIsAreg = 0;
    CData/*7:0*/ __Vtableidx1;
    __Vtableidx1 = 0;
    CData/*4:0*/ __VdfgRegularize_hebeb780c_0_5;
    __VdfgRegularize_hebeb780c_0_5 = 0;
    // Body
    if ((1U & (~ VL_ONEHOT_I(((((IData)((0U != (0x2a00U 
                                                & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot)))) 
                                << 4U) | (((2U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                                                  >> 0x0000000bU)) 
                                           | (1U & 
                                              ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                                               >> 8U))) 
                                          << 2U)) | 
                              ((2U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                                      >> 3U)) | (IData)(
                                                        (0U 
                                                         != 
                                                         (0x000eU 
                                                          & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot)))))))))) {
        if ((0U != ((((IData)((0U != (0x2a00U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot)))) 
                      << 4U) | (((2U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                                        >> 0x0000000bU)) 
                                 | (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                                          >> 8U))) 
                                << 2U)) | ((2U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                                                  >> 3U)) 
                                           | (IData)(
                                                     (0U 
                                                      != 
                                                      (0x000eU 
                                                       & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot)))))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: fx68k.sv:962: Assertion failed in %m: unique case, but multiple matches found for '1'h1'\n",3, 'M',vlSymsp->name(),"sys_top.sys.cpu.irdDecode", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1));
                VL_STOP_MT("../third_party/fx68k/fx68k.sv", 962, "");
            }
        }
    }
    if ((1U & (~ VL_ONEHOT_I(((4U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                                     >> 0x0000000cU)) 
                              | (((IData)((0U != (0x00c0U 
                                                  & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot)))) 
                                  << 1U) | (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                                                  >> 5U)))))))) {
        if ((0U != ((4U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                           >> 0x0000000cU)) | (((IData)(
                                                        (0U 
                                                         != 
                                                         (0x00c0U 
                                                          & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot)))) 
                                                << 1U) 
                                               | (1U 
                                                  & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                                                     >> 5U)))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: fx68k.sv:1012: Assertion failed in %m: unique case, but multiple matches found for '1'h1'\n",3, 'M',vlSymsp->name(),"sys_top.sys.cpu.irdDecode.unnamedblk1", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1));
                VL_STOP_MT("../third_party/fx68k/fx68k.sv", 1012, "");
            }
        }
    }
    if ((1U & (~ VL_ONEHOT_I((((IData)((0U != (0x7b00U 
                                               & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot)))) 
                               << 4U) | ((8U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                                                >> 2U)) 
                                         | ((4U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                                                   >> 2U)) 
                                            | (3U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot))))))))) {
        if ((0U != (((IData)((0U != (0x7b00U & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot)))) 
                     << 4U) | ((8U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                                      >> 2U)) | ((4U 
                                                  & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                                                     >> 2U)) 
                                                 | (3U 
                                                    & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot))))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: fx68k.sv:1036: Assertion failed in %m: unique case, but multiple matches found for '1'h1'\n",3, 'M',vlSymsp->name(),"sys_top.sys.cpu.irdDecode", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1));
                VL_STOP_MT("../third_party/fx68k/fx68k.sv", 1036, "");
            }
        }
    }
    if ((1U & (~ VL_ONEHOT_I(((2U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                                     >> 3U)) | (1U 
                                                & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                                                   >> 6U))))))) {
        if ((0U != ((2U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                           >> 3U)) | (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                                            >> 6U))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: fx68k.sv:1071: Assertion failed in %m: unique case, but multiple matches found for '1'h1'\n",3, 'M',vlSymsp->name(),"sys_top.sys.cpu.irdDecode", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1));
                VL_STOP_MT("../third_party/fx68k/fx68k.sv", 1071, "");
            }
        }
    }
    if ((1U & (~ VL_ONEHOT_I(((((2U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                                       >> 3U)) | (IData)(
                                                         (0U 
                                                          != 
                                                          (0x1100U 
                                                           & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot))))) 
                               << 2U) | (((IData)((0U 
                                                   != 
                                                   (0x4020U 
                                                    & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot)))) 
                                          << 1U) | (IData)(
                                                           (0U 
                                                            != 
                                                            (0x00c0U 
                                                             & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot)))))))))) {
        if ((0U != ((((2U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
                             >> 3U)) | (IData)((0U 
                                                != 
                                                (0x1100U 
                                                 & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot))))) 
                     << 2U) | (((IData)((0U != (0x4020U 
                                                & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot)))) 
                                << 1U) | (IData)((0U 
                                                  != 
                                                  (0x00c0U 
                                                   & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot)))))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: fx68k.sv:1093: Assertion failed in %m: unique case, but multiple matches found for '1'h1'\n",3, 'M',vlSymsp->name(),"sys_top.sys.cpu.irdDecode", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1));
                VL_STOP_MT("../third_party/fx68k/fx68k.sv", 1093, "");
            }
        }
    }
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
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__isRegShift 
        = (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot) 
            >> 0x0000000eU) & (3U != (3U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                                            >> 6U))));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__isWrite 
        = (0U != (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__dobCtrl));
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
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__rstUrom 
        = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
           .__PVT__enPhi1 & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enErrClk));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__enT1 
        = ((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Clks
            .__PVT__enPhi1 & (4U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__tState)) 
           & (~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__wClk)));
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
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__isPcRel 
        = (IData)(((0x003aU == (0x003eU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird))) 
                   & (~ (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird) 
                          >> 5U) & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__isRegShift)))));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__ryIsDt 
        = (IData)(((0x0038U == (0x003aU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ird))) 
                   & (~ (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__isRegShift))));
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
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluCorf__DOT__htemp 
        = ((6U != (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))
            ? (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_7)
            : (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_2));
    __VdfgRegularize_hebeb780c_0_5 = (6U & (- (IData)(
                                                      (1U 
                                                       & (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__pswCcr) 
                                                           >> 4U) 
                                                          | ((6U 
                                                              != (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper)) 
                                                             & (((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluCorf__DOT__htemp) 
                                                                 >> 8U) 
                                                                | ((0U 
                                                                    != 
                                                                    (3U 
                                                                     & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluCorf__DOT__htemp) 
                                                                        >> 5U))) 
                                                                   & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluCorf__DOT__htemp) 
                                                                      >> 7U)))))))));
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
    vlSelfRef.__VdfgRegularize_hebeb780c_0_3 = (0x0000001fU 
                                                & (((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_2) 
                                                    >> 4U) 
                                                   - (IData)(__VdfgRegularize_hebeb780c_0_5)));
    vlSelfRef.__VdfgRegularize_hebeb780c_0_4 = (0x0000001fU 
                                                & (((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_7) 
                                                    >> 4U) 
                                                   + (IData)(__VdfgRegularize_hebeb780c_0_5)));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluCorf__DOT__hNib 
        = ((6U != (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper))
            ? (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_4)
            : (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_3));
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

VL_ATTR_COLD void Vsys_top___024root___stl_sequent__TOP__3(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___stl_sequent__TOP__3\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__updSsw 
        = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
        .__PVT__aob2Ab;
}

VL_ATTR_COLD void Vsys_top___024root___stl_comb__TOP__1(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___stl_comb__TOP__1\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__waitBusFinish 
        = (1U & (((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[2U] 
                   >> 2U) | (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[1U] 
                             >> 0x0000001cU)) | vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                 .__PVT__isWrite));
}

VL_ATTR_COLD void Vsys_top___024root___stl_sequent__TOP__4(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___stl_sequent__TOP__4\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__isPcRel 
        = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod
           .__PVT__isPcRel & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                              .__PVT__rz));
}

VL_ATTR_COLD void Vsys_top___024root___stl_comb__TOP__2(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___stl_comb__TOP__2\n"); );
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
    IData/*31:0*/ __VdfgRegularize_hebeb780c_0_1;
    __VdfgRegularize_hebeb780c_0_1 = 0;
    // Body
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
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aulow 
        = (0x0001ffffU & ((0x0000ffffU & vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__auInpMux) 
                          + (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Dbl)));
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
    sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shftMsb 
        = (1U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__isLong)
                  ? ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__alue) 
                     >> 0x0000000fU) : ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__isByte)
                                         ? ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aOperand) 
                                            >> 7U) : 
                                        ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aOperand) 
                                         >> 0x0000000fU))));
    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busEnding 
        = ((1U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__next) 
           | (2U == vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__next));
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

VL_ATTR_COLD void Vsys_top___024root___stl_sequent__TOP__0(Vsys_top___024root* vlSelf);
void Vsys_top___024root___nba_sequent__TOP__5(Vsys_top___024root* vlSelf);
void Vsys_top___024root___act_sequent__TOP__0(Vsys_top___024root* vlSelf);

VL_ATTR_COLD void Vsys_top___024root___eval_stl(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___eval_stl\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VstlTriggered[1U])) {
        Vsys_top___024root___stl_sequent__TOP__0(vlSelf);
        Vsys_top___024root___nba_sequent__TOP__5(vlSelf);
        Vsys_top___024root___stl_sequent__TOP__2(vlSelf);
    }
    if (((1ULL & vlSelfRef.__VstlTriggered[1U]) | (2ULL 
                                                   & vlSelfRef.__VstlTriggered[0U]))) {
        Vsys_top___024root___act_sequent__TOP__0(vlSelf);
    }
    if ((1ULL & vlSelfRef.__VstlTriggered[0U])) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__updSsw 
            = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
            .__PVT__aob2Ab;
    }
    if ((1ULL & (vlSelfRef.__VstlTriggered[1U] | vlSelfRef.__VstlTriggered[0U]))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__waitBusFinish 
            = (1U & (((vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[2U] 
                       >> 2U) | (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch[1U] 
                                 >> 0x0000001cU)) | vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                     .__PVT__isWrite));
    }
    if ((3ULL & vlSelfRef.__VstlTriggered[0U])) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__isPcRel 
            = (vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Irdecod
               .__PVT__isPcRel & (~ vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Nanod
                                  .__PVT__rz));
    }
    if (((1ULL & vlSelfRef.__VstlTriggered[1U]) | (3ULL 
                                                   & vlSelfRef.__VstlTriggered[0U]))) {
        Vsys_top___024root___stl_comb__TOP__2(vlSelf);
    }
}

VL_ATTR_COLD void Vsys_top___024root___eval_triggers_vec__stl(Vsys_top___024root* vlSelf);
#ifdef VL_DEBUG
VL_ATTR_COLD void Vsys_top___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 2> &triggers, const std::string &tag);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vsys_top___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 2> &in);

VL_ATTR_COLD bool Vsys_top___024root___eval_phase__stl(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___eval_phase__stl\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VstlExecute;
    // Body
    Vsys_top___024root___eval_triggers_vec__stl(vlSelf);
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vsys_top___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
    }
#endif
    __VstlExecute = Vsys_top___024root___trigger_anySet__stl(vlSelfRef.__VstlTriggered);
    if (__VstlExecute) {
        Vsys_top___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

bool Vsys_top___024root___trigger_anySet__ico(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vsys_top___024root___dump_triggers__ico(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___dump_triggers__ico\n"); );
    // Body
    if ((1U & (~ (IData)(Vsys_top___024root___trigger_anySet__ico(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: Internal 'ico' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

bool Vsys_top___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vsys_top___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(Vsys_top___024root___trigger_anySet__act(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: @([hybrid] sys_top.sys.cpu.Nanod)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 1U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 1 is active: @([hybrid] sys_top.sys.cpu.Irdecod)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 2U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 2 is active: @(posedge clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 3U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 3 is active: @(posedge sys_top.sys.cpu.Clks.clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 4U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 4 is active: @(posedge sys_top.sys.cpu.__Vcellinp__nanoRom__clk)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vsys_top___024root___ctor_var_reset(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___ctor_var_reset\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->vlNamep);
    vlSelf->clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16707436170211756652ull);
    vlSelf->reset = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9928399931838511862ull);
    vlSelf->loading = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3420914228660564915ull);
    vlSelf->ioctl_wr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1473125302338472463ull);
    vlSelf->ioctl_addr = VL_SCOPED_RAND_RESET_I(27, __VscopeHash, 10355640217577368817ull);
    vlSelf->ioctl_dout = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 11403743308754436898ull);
    vlSelf->midi_in = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4692646756488609118ull);
    vlSelf->midi_out = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9376089101035429850ull);
    vlSelf->DDRAM_BUSY = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13336884550884172711ull);
    vlSelf->DDRAM_BURSTCNT = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 357439523108104526ull);
    vlSelf->DDRAM_ADDR = VL_SCOPED_RAND_RESET_I(29, __VscopeHash, 11876977206841699062ull);
    vlSelf->DDRAM_DOUT = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 10246674735760176919ull);
    vlSelf->DDRAM_DOUT_READY = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1112804330567764335ull);
    vlSelf->DDRAM_RD = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 223676783988881609ull);
    vlSelf->DDRAM_DIN = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 16889201761959101765ull);
    vlSelf->DDRAM_BE = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 12636279550318602775ull);
    vlSelf->DDRAM_WE = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2010546461232683727ull);
    vlSelf->bus_wr_strobe = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16377625391494764772ull);
    vlSelf->bus_wr_addr = VL_SCOPED_RAND_RESET_I(24, __VscopeHash, 11838225851175715063ull);
    vlSelf->bus_wr_data = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 15418629847356052503ull);
    vlSelf->bus_wr_ds = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 15311270934591744657ull);
    vlSelf->sample_strobe = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17133636610032163868ull);
    vlSelf->out_l = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 10168477844359313032ull);
    vlSelf->out_r = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 5057021945385051194ull);
    vlSelf->lcd_line = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12812373767388305433ull);
    vlSelf->lcd_pos = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 5833927090391683047ull);
    vlSelf->lcd_code = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 17862301876434899326ull);
    vlSelf->sys_top__DOT__snd_wr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1293647269388892629ull);
    vlSelf->sys_top__DOT__snd_pan = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15290464024951222443ull);
    vlSelf->sys_top__DOT__snd_wr_full = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3279021784607796501ull);
    vlSelf->sys_top__DOT__snd_rb = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3315455954493021495ull);
    vlSelf->sys_top__DOT__snd_rb_done = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16628552621393791830ull);
    vlSelf->sys_top__DOT__snd_voice = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 15635694087134121760ull);
    vlSelf->sys_top__DOT__snd_word = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 4968282561838062183ull);
    vlSelf->sys_top__DOT__snd_data = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 8857766584857908123ull);
    vlSelf->sys_top__DOT__snd_mask = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 18061758209776659929ull);
    vlSelf->sys_top__DOT__snd_rb_data = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 16382401524302522543ull);
    vlSelf->sys_top__DOT__snd_raw = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 3048334046879280655ull);
    vlSelf->sys_top__DOT__sys__DOT__ph = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 14077779956335058443ull);
    vlSelf->sys_top__DOT__sys__DOT__tdiv = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 10032169816003108093ull);
    vlSelf->sys_top__DOT__sys__DOT__sdiv = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 18193637814504712990ull);
    vlSelf->sys_top__DOT__sys__DOT__phi1 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17724896667031979716ull);
    vlSelf->sys_top__DOT__sys__DOT__phi2 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14519028725235058580ull);
    vlSelf->sys_top__DOT__sys__DOT__ce_timer = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18076563930553355111ull);
    vlSelf->sys_top__DOT__sys__DOT__ce_ser = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9383389278802470195ull);
    vlSelf->sys_top__DOT__sys__DOT__iEdb = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 15745885992936220400ull);
    vlSelf->sys_top__DOT__sys__DOT__DTACKn = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10562728748384728075ull);
    for (int __Vi0 = 0; __Vi0 < 65536; ++__Vi0) {
        vlSelf->sys_top__DOT__sys__DOT__rom[__Vi0] = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 18336980397867484211ull);
    }
    for (int __Vi0 = 0; __Vi0 < 32768; ++__Vi0) {
        vlSelf->sys_top__DOT__sys__DOT__ram[__Vi0] = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 14234832008226760116ull);
    }
    vlSelf->sys_top__DOT__sys__DOT__rom_q = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 18376119713624109193ull);
    vlSelf->sys_top__DOT__sys__DOT__ram_q = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 6942501470292443301ull);
    vlSelf->sys_top__DOT__sys__DOT__ld_hi = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 17033279958629177002ull);
    vlSelf->sys_top__DOT__sys__DOT__sel_pan = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14165459641106030552ull);
    vlSelf->sys_top__DOT__sys__DOT__sel_mfp = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13940436039292039046ull);
    vlSelf->sys_top__DOT__sys__DOT__bus_busy = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4118310697734023573ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp_cs_wr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2146334796337305427ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp_cs_rd = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14034639558917406603ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp_iack = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8045632819145872113ull);
    vlSelf->sys_top__DOT__sys__DOT__lcd_wr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7305856347683959898ull);
    vlSelf->sys_top__DOT__sys__DOT__lcd_rd = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8216183375619440565ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp_dout = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 2152274986257738220ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp_vector = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 15690294095264438266ull);
    vlSelf->sys_top__DOT__sys__DOT__vol_cnt = VL_SCOPED_RAND_RESET_I(18, __VscopeHash, 7781294279236510647ull);
    vlSelf->sys_top__DOT__sys__DOT__vol_load = VL_SCOPED_RAND_RESET_I(18, __VscopeHash, 11779772007346803460ull);
    vlSelf->sys_top__DOT__sys__DOT__bstate = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 18397710928667705579ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Clks.__PVT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13531320881705707000ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Clks.__PVT__extReset = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13531320881705707000ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Clks.__PVT__pwrUp = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13531320881705707000ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Clks.__PVT__enPhi1 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13531320881705707000ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Clks.__PVT__enPhi2 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13531320881705707000ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__wClk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16890234047489638934ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__tState = 0;
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__enT1 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7112909402876566007ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__enT2 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14065274032053046336ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__enT3 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2955247254473329805ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__enT4 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12547408032447274063ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__rDtack = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18352654055117051646ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__rBerr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 172389553664225718ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__rIpl = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 6026879785063312946ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__iIpl = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 11671113663026115465ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Vpai = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4369088432009140207ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__BeI = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16416365548306992538ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Halti = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6598499158480107462ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__BRi = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3982147333860745323ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__BgackI = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2045633209947347233ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__BeiDelay = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8658358219936983865ull);
    VL_SCOPED_RAND_RESET_W(68, vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__nanoLatch, __VscopeHash, 13096771802096095820ull);
    VL_SCOPED_RAND_RESET_W(68, vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__nanoOutput, __VscopeHash, 16990964875103620236ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__microLatch = VL_SCOPED_RAND_RESET_I(17, __VscopeHash, 16255842911393515261ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__microOutput = VL_SCOPED_RAND_RESET_I(17, __VscopeHash, 8195272722676754917ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__microAddr = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 15185237607517838232ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__nma = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 16358690798345813270ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__nanoAddr = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 6418481978328178667ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__rstUrom = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16479862636206066732ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT____Vcellinp__nanoRom__clk = 0;
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__permStart = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__waitBusFinish = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__isWrite = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__busByte = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__isRmc = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__noLowByte = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__noHighByte = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__updTpend = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__clrTpend = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__tvn2Ftu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__const2Ftu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ftu2Dbl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ftu2Abl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__abl2Pren = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__updPren = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__inl2psw = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ftu2Sr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__sr2Ftu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ftu2Ccr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__pswIToFtu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ird2Ftu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ssw2Ftu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__initST = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__Ir2Ird = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__auClkEn = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__noSpAlign = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__auCntrl = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__todbin = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__toIrc = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dbl2Atl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__abl2Atl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__atl2Abl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__atl2Dbl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__abh2Ath = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dbh2Ath = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ath2Dbh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ath2Abh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__db2Aob = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ab2Aob = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__au2Aob = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__aob2Ab = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__updSsw = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dobCtrl = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__abh2reg = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__abl2reg = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__reg2abl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__reg2abh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dbh2reg = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dbl2reg = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__reg2dbl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__reg2dbh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ssp = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__pchdbh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__pcldbl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__pclabl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__pchabh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__rxh2dbh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__rxh2abh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dbl2rxl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dbh2rxh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__rxl2db = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__rxl2ab = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__abl2rxl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__abh2rxh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dbh2ryh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__abh2ryh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ryl2db = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ryl2ab = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ryh2dbh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ryh2abh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dbl2ryl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__abl2ryl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__rz = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__rxlDbl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__aluColumn = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__aluDctrl = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__aluActrl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__aluInit = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__aluFinish = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__abd2Dcr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dcr2Dbd = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dbd2Alue = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__alue2Dbd = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dbd2Alub = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__abd2Alub = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__alu2Dbd = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__alu2Abd = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__au2Db = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__au2Ab = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__au2Pc = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dbin2Abd = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dbin2Dbd = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__extDbh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__extAbh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ablAbd = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__ablAbh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dblDbd = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__dblDbh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Nanod.__PVT__abdIsByte = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13638534350988460425ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__isPcRel = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17734758101805082811ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__isTas = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17734758101805082811ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__implicitSp = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17734758101805082811ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__toCcr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17734758101805082811ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__rxIsDt = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17734758101805082811ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__ryIsDt = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17734758101805082811ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__rxIsUsp = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17734758101805082811ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__rxIsMovem = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17734758101805082811ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__movemPreDecr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17734758101805082811ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__isByte = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17734758101805082811ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__isMovep = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17734758101805082811ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__rx = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 17734758101805082811ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__ry = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 17734758101805082811ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__rxIsAreg = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17734758101805082811ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__ryIsAreg = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17734758101805082811ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__ftuConst = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 17734758101805082811ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__macroTvn = VL_SCOPED_RAND_RESET_I(6, __VscopeHash, 17734758101805082811ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Irdecod.__PVT__inhibitCcr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17734758101805082811ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Tpend = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9219999632344965940ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__intPend = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1621732424543669131ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__pswT = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1414396966114656587ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__pswS = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9119491110428741102ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__pswI = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 9568413494891497972ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__psw = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 16017590215336420617ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__ftu = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 11754033592537640208ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Irc = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 17300966147543167511ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Ir = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 12035174859759298223ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Ird = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 7892732623316932792ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__alue = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 2678401654032488372ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__dcr4 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6299356405778194495ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__tvn = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 12561526940206235095ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__busAddrErr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18200519760543870574ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__addrOe = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1349797678426758258ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__iStop = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11723859509750477469ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__A0Err = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1584199753572654326ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excRst = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 891885214982200116ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__BerrA = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14909146191864694285ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Spuria = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16436726845664150644ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Avia = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17907895467499076305ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__rAddrErr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1709310456841190896ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__iBusErr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8974167611982136076ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__Err6591 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13157376936028043133ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__iAddrErr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16353231857734560157ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__enErrClk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4105337830224063963ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__rFC = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 2096561222350857601ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__inl = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 17011497801437864898ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__updIll = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14141211658991883330ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__prevNmi = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9466178821819318194ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__iplStable = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5622385770441816971ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__iplComp = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5773226449955196832ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__eCntr = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 13182572195030985639ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__rVma = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7692225735052628821ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__irdToCcr_t4 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8959756839985566459ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__ssw = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 4170922247244374209ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__tvnLatch = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 976312342360348478ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__inExcept01 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17078617231669460726ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__microToNanoAddr__DOT__orgBase = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 2578613279777777449ull);
    for (int __Vi0 = 0; __Vi0 < 336; ++__Vi0) {
        VL_SCOPED_RAND_RESET_W(68, vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__nanoRom__DOT__nRam[__Vi0], __VscopeHash, 373445491878293327ull);
    }
    for (int __Vi0 = 0; __Vi0 < 1024; ++__Vi0) {
        vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__uRom__DOT__uRam[__Vi0] = VL_SCOPED_RAND_RESET_I(17, __VscopeHash, 95007017406776883ull);
    }
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__lineBmap = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 5665471856635059282ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 6318349732854393843ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 15993760113715789548ull);
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[__Vi0] = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 10573635871304611797ull);
    }
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[__Vi0] = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 403340031597714169ull);
    }
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 8770985131018526573ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__illMisc = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12506103145807736850ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__a1Misc = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 6110493371553823266ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__uNma = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 6555134762434813280ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__grp1Nma = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 4601257760769326132ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__c0c1 = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 14362120425631797170ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__a0Rst = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 852189018440958732ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__enl = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 11511193883428651434ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__ms0 = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 3087165180191668582ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__m01 = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 14004459577718644758ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__ccTest = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15859161923369047144ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rTrace = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7307438920005251525ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rInterrupt = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6877767118334229722ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rIllegal = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13705682647186861348ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rPriv = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15715309433318310587ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rLineA = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9301457879558793589ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rLineF = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13328792927170336074ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rExcRst = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18392608350599181929ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rExcAdrErr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15729928426726663046ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rExcBusErr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10868898766542474457ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rSpurious = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8698031570923107151ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__rAutovec = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14438022688269054117ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__grp1LatchEn = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6911357674008462161ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__sequencer__DOT__grp0LatchEn = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6351360573687007249ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT____Vlvbound_hf1d1f6c7__0 = 0;
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT____Vlvbound_hbc3e4165__0 = 0;
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT____Vlvbound_hb21dc05c__2 = 0;
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT____Vlvbound_h0c93c5bf__0 = 0;
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT____Vlvbound_hb21dc05c__1 = 0;
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT____Vlvbound_hb21dc05c__0 = 0;
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT____Vlvbound_h77a255be__2 = 0;
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT____Vlvbound_h93f55855__0 = 0;
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT____Vlvbound_h77a255be__1 = 0;
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT____Vlvbound_h77a255be__0 = 0;
    for (int __Vi0 = 0; __Vi0 < 18; ++__Vi0) {
        vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68L[__Vi0] = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 5223829756744739067ull);
    }
    for (int __Vi0 = 0; __Vi0 < 18; ++__Vi0) {
        vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__regs68H[__Vi0] = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 9436567541750654308ull);
    }
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dbin = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 3489343833731507792ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dcrOutput = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 16219960440304496684ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__PcL = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 14110787683824277712ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__PcH = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 12422818659369475363ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__auReg = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2755930339800370089ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aob = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 682854551212058222ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Ath = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 5808447143784755180ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Atl = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 8473271746187989052ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Dbl = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 11936717156362077772ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Dbh = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 9433394240296323878ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Abh = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 3629235316744485432ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Abl = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 3276770061307918408ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Abd = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 11776835098078283340ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Dbd = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 16301208083300721649ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dblMux = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 12372492379198558494ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dbhMux = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 7899931832859709364ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__abhMux = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 5944517664313086087ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ablMux = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 7689502114538867813ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__abdMux = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 15992478473093602683ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dbdMux = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 3842610701529331500ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__abdIsByte = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13870458829150656518ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Pcl2Dbl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15688401904865810077ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Pch2Dbh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15149374029479813758ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Pcl2Abl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17123389429991568650ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__Pch2Abh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17986210347236982722ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__actualRx = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 5202440130627656283ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__actualRy = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 2156455984867922878ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__movemRx = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 753831047744349365ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__byteNotSpAlign = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3637647714963542196ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxMux = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 306976178896965472ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryMux = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 14664324234757169636ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxReg = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 8535912461992484782ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryReg = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 17041139065124443177ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxIsSp = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1536006169216604938ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryIsSp = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5228659118653199383ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxIsAreg = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1565263264434748234ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryIsAreg = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6434417380246581161ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryl2Abd = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4963117939192475385ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ryl2Dbd = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9962682826741407198ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxl2Abd = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9230250338194447063ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__rxl2Dbd = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 330810350577605285ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__abhIdle = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6415502129131845837ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__ablIdle = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3656697021618566402ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__abdIdle = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8652140076810650056ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dbhIdle = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16465724641201670494ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dblIdle = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4760574059454823746ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dbdIdle = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3256758090678031025ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__preAbh = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 13428291556720034636ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__preAbl = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 8264368644551598111ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__preAbd = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 6715903402904074962ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__preDbh = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 8048039114711481692ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__preDbl = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 16597023075516102936ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__preDbd = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 7383412114574682969ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__auInpMux = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6392683417416780464ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__aulow = VL_SCOPED_RAND_RESET_I(17, __VscopeHash, 4976307348925002760ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dbl2Pcl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14713895368416393841ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dbh2Pch = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4197950505577061536ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__abh2Pch = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13358724987365739168ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__abl2Pcl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10620222743124704955ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prHbit = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 5885293029816597406ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__prenLatch = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 2457013184056510038ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dcrCode = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 13716265145902020436ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alub = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 15873731070279710496ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dobInput = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 12467303165238360042ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__dob = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 13229625594138789455ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__xToDbin = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12693569516326510754ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__xToIrc = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3578124947999447558ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__dbinNoLow = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8256644296603387052ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__dbinNoHigh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7833169475438399161ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__byteMux = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5890578927033976050ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__isByte_T4 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7561298831379254674ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__dataIo__DOT__byteCycle = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13303984631590315410ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluLatch = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 13055683647202141314ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__pswCcr = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 8405582747547680220ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrCore = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 3582912716867793773ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__result = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 3793936295247230904ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrTemp = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 11772376928116305092ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__coreH = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4891003019061651105ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__subHcarry = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9724361184683296888ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__row = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 7060309925545372318ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__isArX = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11083672251364235292ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__noCcrEn = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9150518860586822334ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__isByte = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5086078847134601448ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__ccrMask = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 13559865329375719825ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__oper = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 1765313639480445558ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aOperand = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 4707918970838571496ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__dOperand = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 14648732584439101241ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__cRow = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 17929032150764974011ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__cMask = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 4878079892138488249ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluOp = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 8443033267319975988ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shftResult = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5906156710535009140ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__bcdLatch = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 1891633092369925603ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__bcdCarry = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9656190339163097964ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__bcdOverf = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13262515970497543181ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__isLong = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7377370269498397204ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__rIrd8 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9777844152353905618ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__isShift = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7380547017994693087ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__shftRight = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15687382522254291751ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT____Vcellinp__shifter__swapWords = 0;
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluCorf__DOT__htemp = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 7551821600646304635ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__aluCorf__DOT__hNib = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 11298306086867737353ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__ftuCtrl = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 9254793856470773491ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__dobCtrl = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 9175501681927180806ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__dblSpecial = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15938942787576291608ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__dbhSpecial = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5728683406474402564ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__ablSpecial = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8356073122868503806ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__abhSpecial = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15468795376291973707ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__nDecoder__DOT__isPcRel = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3358146344459651377ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__lineOnehot = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 1204887557616431231ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__irdDecode__DOT__isRegShift = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12839911069080126639ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rAS = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17207682218054811813ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rLDS = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7136444244673670164ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rUDS = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14729890091390245449ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__rRWn = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13678146036680400010ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__bcPend = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13245309062639897815ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__isWriteReg = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14496655328535565210ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__bciByte = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5189797863491888653ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__isRmcReg = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1818285698861731924ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__wendReg = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16553541415800950876ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__addrOeDelay = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15454573943903071719ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__isByteT4 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5191392928444852303ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__bcReset = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14098286954337757243ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busPhase = 0;
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__next = 0;
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__busControl__DOT__busEnding = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1227907661303629808ull);
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__busArbiter__DOT__dmaPhase = 0;
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__busArbiter__DOT__next = 0;
    vlSelf->sys_top__DOT__sys__DOT__cpu__DOT__busArbiter__DOT__rGranted = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2349189918819159306ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__tout = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 8890460704737095320ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__gpip = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 9171936725481695642ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__aer = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 4635356226339795632ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__ddr = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 6181655006556851559ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__vr = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 6892831121876048622ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__scr = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 14778759196812713921ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__ucr = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 7067402669275637766ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__rsr = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 3448117554479945466ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__tsr = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 2420810855447187256ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__ier = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 10663698544480737867ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__ipr = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 12003665236540992523ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__isr = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 6448893892484505104ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__imr = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 17427736227401878099ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__tacr = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 17333805669442151664ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__tbcr = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 5486328244215557500ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__tcdcr = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 2132846874381844429ull);
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__tdr[__Vi0] = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 3785421678824055118ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__tmc[__Vi0] = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 5008528919801365418ull);
    }
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__gpio_last = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 17024563502200773964ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__ti_last = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 16463070704722026641ull);
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__tmode[__Vi0] = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 5881338885274800035ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__pcount[__Vi0] = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 10169801855361148158ull);
    }
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__t_active = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 15881004742686131514ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__t_enabled = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 10792334322321474677ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__rx_ph = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 5933338777598471444ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__rx_bit = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 16149500232994107307ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__rx_shift = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 8165220780250294374ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__rx_busy = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2719572800233561541ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__rx_data = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 7143858213816852359ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__si_s = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1904313728215491624ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__si_ss = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 308996637273784431ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__tx_ph = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 6926719562934025101ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__tx_bit = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 5729438905944309102ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__tx_shift = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 15587279893984157854ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__tx_busy = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1970052019429141287ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__tx_buf = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 16096723815179697717ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__hi_ch = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 17972777041682921546ull);
    vlSelf->sys_top__DOT__sys__DOT__mfp__DOT__hi_any = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 328326700260735204ull);
    for (int __Vi0 = 0; __Vi0 < 128; ++__Vi0) {
        vlSelf->sys_top__DOT__sys__DOT__lcd__DOT__ddram[__Vi0] = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 8337838744584729118ull);
    }
    for (int __Vi0 = 0; __Vi0 < 64; ++__Vi0) {
        vlSelf->sys_top__DOT__sys__DOT__lcd__DOT__cgram[__Vi0] = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 12140934886910710027ull);
    }
    vlSelf->sys_top__DOT__sys__DOT__lcd__DOT__ac = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 120352634051232618ull);
    vlSelf->sys_top__DOT__sys__DOT__lcd__DOT__active_cg = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10567620058333454641ull);
    vlSelf->sys_top__DOT__sys__DOT__lcd__DOT__dir_inc = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17030249391524092211ull);
    vlSelf->sys_top__DOT__sys__DOT__lcd__DOT__shift_on = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4773489906756247782ull);
    vlSelf->sys_top__DOT__sys__DOT__lcd__DOT__disp_shift = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 13324701557006312484ull);
    vlSelf->sys_top__DOT__sys__DOT__lcd__DOT__two_line = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11699244143456008601ull);
    vlSelf->sys_top__DOT__sys__DOT__lcd__DOT__i = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10249148502335370384ull);
    vlSelf->sys_top__DOT__sys__DOT__lcd__DOT__sp = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 15928879847994691130ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd_wr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9834694992540757777ull);
    vlSelf->sys_top__DOT__sctl__DOT__pan_wr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16489685897285115153ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd_start = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10485527651196687744ull);
    vlSelf->sys_top__DOT__sctl__DOT__rb_start = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15826689357944770889ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd_voice = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 10058317100101693182ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd_word = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 13153250403244148046ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd_data = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 10801600536202448714ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd_mask = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 12093327731192771170ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd_busy = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14726832796204405134ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd_done = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3298349711621041881ull);
    vlSelf->sys_top__DOT__sctl__DOT__rb_done = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12493998516912215348ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd_l = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 10539541624097601529ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd_r = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 5562335159138048623ull);
    vlSelf->sys_top__DOT__sctl__DOT__mem_req = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13522850230846554087ull);
    vlSelf->sys_top__DOT__sctl__DOT__mem_addr = VL_SCOPED_RAND_RESET_I(22, __VscopeHash, 3983590162265948727ull);
    vlSelf->sys_top__DOT__sctl__DOT__mem_ack = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10318341268484354955ull);
    vlSelf->sys_top__DOT__sctl__DOT__mem_s0 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 12625529919215315180ull);
    vlSelf->sys_top__DOT__sctl__DOT__mem_s1 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 16853563866754697625ull);
    for (int __Vi0 = 0; __Vi0 < 64; ++__Vi0) {
        vlSelf->sys_top__DOT__sctl__DOT__fifo[__Vi0] = VL_SCOPED_RAND_RESET_Q(42, __VscopeHash, 13054442818998003779ull);
    }
    vlSelf->sys_top__DOT__sctl__DOT__f_wp = VL_SCOPED_RAND_RESET_I(6, __VscopeHash, 11478241565282228477ull);
    vlSelf->sys_top__DOT__sctl__DOT__f_rp = VL_SCOPED_RAND_RESET_I(6, __VscopeHash, 14224674052738844331ull);
    vlSelf->sys_top__DOT__sctl__DOT__f_empty = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10967547069442168072ull);
    vlSelf->sys_top__DOT__sctl__DOT__ld_word = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 1070850653045173638ull);
    vlSelf->sys_top__DOT__sctl__DOT__ld_addr = VL_SCOPED_RAND_RESET_I(27, __VscopeHash, 8864781769303235646ull);
    vlSelf->sys_top__DOT__sctl__DOT__ld_pend = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3385934855212720740ull);
    vlSelf->sys_top__DOT__sctl__DOT__cap_lo = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2809241989027961004ull);
    vlSelf->sys_top__DOT__sctl__DOT__cap_half = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4872608413836268771ull);
    vlSelf->sys_top__DOT__sctl__DOT__cap_word = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 7138672147044573502ull);
    vlSelf->sys_top__DOT__sctl__DOT__cap_pend = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5472050442176190270ull);
    vlSelf->sys_top__DOT__sctl__DOT__cap_ptr = VL_SCOPED_RAND_RESET_I(21, __VscopeHash, 16270723964448754700ull);
    vlSelf->sys_top__DOT__sctl__DOT__m_state = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 17375002289707373570ull);
    vlSelf->sys_top__DOT__sctl__DOT__m_client = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 8279558773122674315ull);
    vlSelf->sys_top__DOT__sctl__DOT__m_beat = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8876638902785213002ull);
    vlSelf->sys_top__DOT__sctl__DOT__smp_sel = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 2863663069230607996ull);
    vlSelf->sys_top__DOT__sctl__DOT__wr_done = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8885401114971790895ull);
    vlSelf->sys_top__DOT__sctl__DOT__q_state = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 15926158139866304856ull);
    vlSelf->sys_top__DOT__sctl__DOT__div = VL_SCOPED_RAND_RESET_I(12, __VscopeHash, 11820591610516050638ull);
    vlSelf->sys_top__DOT__sctl__DOT__tick_pending = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8623426712354813776ull);
    vlSelf->sys_top__DOT__sctl__DOT__rb_pending = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1149759135850942871ull);
    for (int __Vi0 = 0; __Vi0 < 32; ++__Vi0) {
        for (int __Vi1 = 0; __Vi1 < 16; ++__Vi1) {
            vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__raw[__Vi0][__Vi1] = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 858821310364058609ull);
        }
    }
    for (int __Vi0 = 0; __Vi0 < 32; ++__Vi0) {
        vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__v_addr[__Vi0] = VL_SCOPED_RAND_RESET_I(24, __VscopeHash, 18366289344040343958ull);
    }
    for (int __Vi0 = 0; __Vi0 < 32; ++__Vi0) {
        vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__v_frac[__Vi0] = VL_SCOPED_RAND_RESET_I(12, __VscopeHash, 8951994994436823256ull);
    }
    for (int __Vi0 = 0; __Vi0 < 32; ++__Vi0) {
        vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__v_pitch[__Vi0] = VL_SCOPED_RAND_RESET_I(19, __VscopeHash, 6487582561450141897ull);
    }
    for (int __Vi0 = 0; __Vi0 < 32; ++__Vi0) {
        vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__v_env[__Vi0] = VL_SCOPED_RAND_RESET_I(24, __VscopeHash, 15849453466152051660ull);
    }
    for (int __Vi0 = 0; __Vi0 < 32; ++__Vi0) {
        vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__v_lstart[__Vi0] = VL_SCOPED_RAND_RESET_I(24, __VscopeHash, 671073211862756594ull);
    }
    for (int __Vi0 = 0; __Vi0 < 32; ++__Vi0) {
        vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__v_lend[__Vi0] = VL_SCOPED_RAND_RESET_I(24, __VscopeHash, 10207399154617544170ull);
    }
    for (int __Vi0 = 0; __Vi0 < 32; ++__Vi0) {
        vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__v_level[__Vi0] = VL_SCOPED_RAND_RESET_I(12, __VscopeHash, 17781299378775940507ull);
    }
    for (int __Vi0 = 0; __Vi0 < 32; ++__Vi0) {
        vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__v_pan[__Vi0] = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 11738098607427583654ull);
    }
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__wr_new = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 6826423813471403590ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__wr_val = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15743358406768788922ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__state = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 18417108706961204372ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__v = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 16231650781196484979ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__s0_r = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 3084457200782032171ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__s1_r = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 4162407361960443090ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__nx_s0 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 510578557870951681ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__nx_s1 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 4599770465451500397ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__nx_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15361799106451337485ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__r_addr = VL_SCOPED_RAND_RESET_I(24, __VscopeHash, 7154036098688479979ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__r_ls = VL_SCOPED_RAND_RESET_I(24, __VscopeHash, 14319079143119947549ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__r_le = VL_SCOPED_RAND_RESET_I(24, __VscopeHash, 8086431656547012049ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__r_env = VL_SCOPED_RAND_RESET_I(24, __VscopeHash, 4042026549795862326ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__r_frac = VL_SCOPED_RAND_RESET_I(12, __VscopeHash, 17634806232868498997ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__r_level = VL_SCOPED_RAND_RESET_I(12, __VscopeHash, 7974612379395875595ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__r_pitch = VL_SCOPED_RAND_RESET_I(19, __VscopeHash, 10993080932212554782ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__r_pan = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 18157745160919983217ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__lvl_n = VL_SCOPED_RAND_RESET_I(12, __VscopeHash, 14629105709195039146ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__diff = VL_SCOPED_RAND_RESET_I(17, __VscopeHash, 9089180251191223243ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__prod = VL_SCOPED_RAND_RESET_I(29, __VscopeHash, 5737445465460386386ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__smp = VL_SCOPED_RAND_RESET_I(17, __VscopeHash, 12624611623609910134ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__gain = VL_SCOPED_RAND_RESET_I(17, __VscopeHash, 14226220298546674926ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__mul2 = VL_SCOPED_RAND_RESET_Q(34, __VscopeHash, 14731555119984553083ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__vout = VL_SCOPED_RAND_RESET_I(17, __VscopeHash, 10192480184392049852ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__pl = VL_SCOPED_RAND_RESET_I(22, __VscopeHash, 1041023297395181921ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__pr = VL_SCOPED_RAND_RESET_I(22, __VscopeHash, 14015770044226574235ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__acc_l = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7443437251835029281ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__acc_r = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3466831911904001098ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__pos = VL_SCOPED_RAND_RESET_I(13, __VscopeHash, 10360507155592178561ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__addr1 = VL_SCOPED_RAND_RESET_I(24, __VscopeHash, 13501353621058728631ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__lspan = VL_SCOPED_RAND_RESET_I(24, __VscopeHash, 5963675325261156710ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__addr_wrap = VL_SCOPED_RAND_RESET_I(24, __VscopeHash, 1446946111168864207ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__addr_st = VL_SCOPED_RAND_RESET_I(24, __VscopeHash, 7801714641331232138ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__wrap1 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16314579399763264393ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__wrap2 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11237614018136597631ull);
    vlSelf->sys_top__DOT__sctl__DOT__snd__DOT__fetch_addr_next = VL_SCOPED_RAND_RESET_I(22, __VscopeHash, 8756421437424651201ull);
    vlSelf->__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__result = 0;
    vlSelf->__Vtask_sys_top__DOT__sys__DOT__cpu__DOT__excUnit__DOT__alu__DOT__mySubber__2__cout = 0;
    vlSelf->__VdfgRegularize_hebeb780c_0_0 = 0;
    vlSelf->__VdfgRegularize_hebeb780c_0_2 = 0;
    vlSelf->__VdfgRegularize_hebeb780c_0_3 = 0;
    vlSelf->__VdfgRegularize_hebeb780c_0_4 = 0;
    vlSelf->__VdfgRegularize_hebeb780c_0_7 = 0;
    vlSelf->__Vdly__sys_top__DOT__sys__DOT__mfp_cs_wr = 0;
    vlSelf->__Vdly__sys_top__DOT__sys__DOT__bus_busy = 0;
    vlSelf->__Vdly__sys_top__DOT__sys__DOT__bstate = 0;
    vlSelf->__Vdly__sys_top__DOT__snd_voice = 0;
    vlSelf->__Vdly__sys_top__DOT__sys__DOT__mfp__DOT__ipr = 0;
    vlSelf->__Vdly__sys_top__DOT__sys__DOT__mfp__DOT__isr = 0;
    vlSelf->__Vdly__sys_top__DOT__sys__DOT__lcd__DOT__ac = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__mem_ack = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__m_client = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__mem_s1 = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__mem_s0 = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__q_state = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__div = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__tick_pending = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__rb_pending = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__f_rp = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__cap_half = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__snd_voice = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__snd_word = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__state = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__mem_req = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__nx_valid = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__addr_st = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__wrap1 = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__addr_wrap = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__addr1 = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__pos = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__lspan = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__wrap2 = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__r_addr = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__r_frac = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__r_pitch = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__r_ls = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__r_le = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__acc_l = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__acc_r = 0;
    vlSelf->__Vdly__sys_top__DOT__sctl__DOT__snd__DOT__v = 0;
    vlSelf->__Vdly__sys_top__DOT__sys__DOT__cpu__DOT__ftu = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__rom__v0 = 0;
    vlSelf->__VdlyDim0__sys_top__DOT__sys__DOT__rom__v0 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__rom__v0 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__ram__v0 = 0;
    vlSelf->__VdlyDim0__sys_top__DOT__sys__DOT__ram__v0 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__ram__v0 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__ram__v1 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__ram__v2 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__lcd__DOT__cgram__v0 = 0;
    vlSelf->__VdlyDim0__sys_top__DOT__sys__DOT__lcd__DOT__cgram__v0 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__lcd__DOT__cgram__v0 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sctl__DOT__fifo__v0 = 0;
    vlSelf->__VdlyDim0__sys_top__DOT__sctl__DOT__fifo__v0 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sctl__DOT__fifo__v0 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v0 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_level__v32 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_addr__v32 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_frac__v33 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_level__v33 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_disp__v32 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_frac__v34 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_pitch__v0 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_pitch__v32 = 0;
    vlSelf->__VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_pitch__v32 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_pitch__v32 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_lend__v0 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_lend__v32 = 0;
    vlSelf->__VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_lend__v32 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_lend__v32 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_env__v0 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_env__v32 = 0;
    vlSelf->__VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_env__v32 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_env__v32 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_lstart__v0 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_lstart__v32 = 0;
    vlSelf->__VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_lstart__v32 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_lstart__v32 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_pan__v0 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sctl__DOT__snd__DOT__v_pan__v32 = 0;
    vlSelf->__VdlyDim0__sys_top__DOT__sctl__DOT__snd__DOT__v_pan__v32 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sctl__DOT__snd__DOT__v_pan__v32 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v0 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v0 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v1 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v1 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v2 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v2 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v3 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v3 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v4 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v4 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v5 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v5 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v6 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v6 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v7 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v7 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v8 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v8 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v9 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v9 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v10 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v10 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v11 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v11 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v12 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v12 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v13 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v13 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v14 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v14 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v15 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v15 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v16 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v16 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v17 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v17 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v18 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v18 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v19 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v19 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v20 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v20 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v21 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v21 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v22 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v22 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v23 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v23 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v24 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v24 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v25 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v25 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v26 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v26 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v27 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v27 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v28 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v28 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v29 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v29 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v30 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v30 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v31 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v31 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v32 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v32 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v33 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v33 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v34 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v34 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v35 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v35 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v36 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v36 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v37 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v37 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v38 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v38 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v39 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v39 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v40 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v40 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v41 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v41 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v42 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v42 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v43 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v43 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v44 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v44 = 0;
    vlSelf->__VdlyVal__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v45 = 0;
    vlSelf->__VdlySet__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__v45 = 0;
    for (int __Vi0 = 0; __Vi0 < 2; ++__Vi0) {
        vlSelf->__VstlTriggered[__Vi0] = 0;
    }
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__permStart = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__waitBusFinish = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__isWrite = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__busByte = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__isRmc = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__noLowByte = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__noHighByte = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__updTpend = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__clrTpend = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__tvn2Ftu = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__const2Ftu = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__ftu2Dbl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__ftu2Abl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__abl2Pren = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__updPren = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__inl2psw = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__ftu2Sr = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__sr2Ftu = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__ftu2Ccr = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__pswIToFtu = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__ird2Ftu = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__ssw2Ftu = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__initST = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__Ir2Ird = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__auClkEn = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__noSpAlign = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__auCntrl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__todbin = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__toIrc = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__dbl2Atl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__abl2Atl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__atl2Abl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__atl2Dbl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__abh2Ath = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__dbh2Ath = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__ath2Dbh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__ath2Abh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__db2Aob = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__ab2Aob = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__au2Aob = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__aob2Ab = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__updSsw = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__dobCtrl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__abh2reg = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__abl2reg = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__reg2abl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__reg2abh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__dbh2reg = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__dbl2reg = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__reg2dbl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__reg2dbh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__ssp = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__pchdbh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__pcldbl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__pclabl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__pchabh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__rxh2dbh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__rxh2abh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__dbl2rxl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__dbh2rxh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__rxl2db = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__rxl2ab = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__abl2rxl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__abh2rxh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__dbh2ryh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__abh2ryh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__ryl2db = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__ryl2ab = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__ryh2dbh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__ryh2abh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__dbl2ryl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__abl2ryl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__rz = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__rxlDbl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__aluColumn = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__aluDctrl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__aluActrl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__aluInit = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__aluFinish = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__abd2Dcr = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__dcr2Dbd = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__dbd2Alue = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__alue2Dbd = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__dbd2Alub = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__abd2Alub = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__alu2Dbd = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__alu2Abd = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__au2Db = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__au2Ab = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__au2Pc = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__dbin2Abd = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__dbin2Dbd = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__extDbh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__extAbh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__ablAbd = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__ablAbh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__dblDbd = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__dblDbh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__0.__PVT__abdIsByte = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__0.__PVT__isPcRel = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__0.__PVT__isTas = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__0.__PVT__implicitSp = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__0.__PVT__toCcr = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__0.__PVT__rxIsDt = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__0.__PVT__ryIsDt = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__0.__PVT__rxIsUsp = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__0.__PVT__rxIsMovem = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__0.__PVT__movemPreDecr = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__0.__PVT__isByte = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__0.__PVT__isMovep = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__0.__PVT__rx = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__0.__PVT__ry = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__0.__PVT__rxIsAreg = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__0.__PVT__ryIsAreg = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__0.__PVT__ftuConst = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__0.__PVT__macroTvn = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__0.__PVT__inhibitCcr = 0;
    vlSelf->__VstlDidInit = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VicoTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VactTriggered[__Vi0] = 0;
    }
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__permStart = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__waitBusFinish = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__isWrite = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__busByte = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__isRmc = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__noLowByte = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__noHighByte = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__updTpend = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__clrTpend = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__tvn2Ftu = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__const2Ftu = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__ftu2Dbl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__ftu2Abl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__abl2Pren = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__updPren = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__inl2psw = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__ftu2Sr = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__sr2Ftu = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__ftu2Ccr = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__pswIToFtu = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__ird2Ftu = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__ssw2Ftu = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__initST = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__Ir2Ird = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__auClkEn = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__noSpAlign = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__auCntrl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__todbin = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__toIrc = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__dbl2Atl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__abl2Atl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__atl2Abl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__atl2Dbl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__abh2Ath = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__dbh2Ath = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__ath2Dbh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__ath2Abh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__db2Aob = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__ab2Aob = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__au2Aob = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__aob2Ab = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__updSsw = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__dobCtrl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__abh2reg = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__abl2reg = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__reg2abl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__reg2abh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__dbh2reg = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__dbl2reg = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__reg2dbl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__reg2dbh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__ssp = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__pchdbh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__pcldbl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__pclabl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__pchabh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__rxh2dbh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__rxh2abh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__dbl2rxl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__dbh2rxh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__rxl2db = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__rxl2ab = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__abl2rxl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__abh2rxh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__dbh2ryh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__abh2ryh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__ryl2db = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__ryl2ab = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__ryh2dbh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__ryh2abh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__dbl2ryl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__abl2ryl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__rz = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__rxlDbl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__aluColumn = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__aluDctrl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__aluActrl = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__aluInit = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__aluFinish = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__abd2Dcr = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__dcr2Dbd = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__dbd2Alue = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__alue2Dbd = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__dbd2Alub = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__abd2Alub = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__alu2Dbd = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__alu2Abd = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__au2Db = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__au2Ab = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__au2Pc = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__dbin2Abd = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__dbin2Dbd = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__extDbh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__extAbh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__ablAbd = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__ablAbh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__dblDbd = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__dblDbh = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Nanod__1.__PVT__abdIsByte = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__1.__PVT__isPcRel = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__1.__PVT__isTas = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__1.__PVT__implicitSp = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__1.__PVT__toCcr = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__1.__PVT__rxIsDt = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__1.__PVT__ryIsDt = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__1.__PVT__rxIsUsp = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__1.__PVT__rxIsMovem = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__1.__PVT__movemPreDecr = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__1.__PVT__isByte = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__1.__PVT__isMovep = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__1.__PVT__rx = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__1.__PVT__ry = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__1.__PVT__rxIsAreg = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__1.__PVT__ryIsAreg = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__1.__PVT__ftuConst = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__1.__PVT__macroTvn = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT__Irdecod__1.__PVT__inhibitCcr = 0;
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = 0;
    vlSelf->__Vtrigprevexpr_h4ed98bcf__1 = 0;
    vlSelf->__Vtrigprevexpr___TOP__sys_top__DOT__sys__DOT__cpu__DOT____Vcellinp__nanoRom__clk__0 = 0;
    vlSelf->__VactDidInit = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VnbaTriggered[__Vi0] = 0;
    }
}
