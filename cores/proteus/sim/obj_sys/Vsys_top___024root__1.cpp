// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vsys_top.h for the primary calling header

#include "Vsys_top__pch.h"

void Vsys_top___024root___nba_sequent__TOP__5(Vsys_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsys_top___024root___nba_sequent__TOP__5\n"); );
    Vsys_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
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
                            = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01d5U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x015bU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01c5U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
                    } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01d5U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0x00001000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01c5U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
                    } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x000bU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d7U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 6U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x015aU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x000bU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01cbU;
                    } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x000fU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d7U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x021cU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x015aU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x000fU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01cbU;
                    } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x0179U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d7U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x0103U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x015aU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x0179U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01cbU;
                    } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01c6U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d7U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01c2U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x015aU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01c6U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01cbU;
                    } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01e7U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d7U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01e3U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x015aU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01e7U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01cbU;
                    } else {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x000eU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d7U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x000aU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x015aU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x000eU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01cbU;
                    }
                } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01e6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01e2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x015aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01e6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01cbU;
                } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01c6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x015aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01c6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01cbU;
                } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01e7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x015aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01e7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01cbU;
                } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x00a7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d5U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x00eaU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x015bU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x00a7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c5U;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000800U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00001000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00002000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
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
                            VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1889: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                         , '#',64,VL_TIME_UNITED_Q(1)
                                         , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                            VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1889, "");
                            VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:2031: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                         , '#',64,VL_TIME_UNITED_Q(1)
                                         , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                            VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 2031, "");
                            VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:2173: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                         , '#',64,VL_TIME_UNITED_Q(1)
                                         , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                            VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 2173, "");
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
                            = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x010cU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0x00001000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01c5U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
                    } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x006fU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x03e3U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x010bU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
                    } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x000bU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x029dU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x000bU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x029dU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x000bU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x029dU;
                    } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x000fU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x029dU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x000fU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x029dU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x000fU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x029dU;
                    } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x0179U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x029dU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x0179U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x029dU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x0179U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x029dU;
                    } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01c6U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x029dU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01c6U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x029dU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01c6U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x029dU;
                    } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01e7U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x029dU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01e7U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x029dU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01e7U;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x029dU;
                    } else {
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x000eU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x029dU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x000eU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x029dU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                            = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x000eU;
                        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x029dU;
                    }
                } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01e6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x029dU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01e6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x029dU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01e6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x029dU;
                } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000800U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00001000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00002000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
                } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000800U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00001000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00002000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
                } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000800U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00001000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00002000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00000800U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00001000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00002000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
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
                            VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1872: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                         , '#',64,VL_TIME_UNITED_Q(1)
                                         , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                            VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1872, "");
                            VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:2014: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                         , '#',64,VL_TIME_UNITED_Q(1)
                                         , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                            VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 2014, "");
                            VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:2156: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                         , '#',64,VL_TIME_UNITED_Q(1)
                                         , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                            VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 2156, "");
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
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x0100U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x03e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01c1U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x006bU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x03e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x010fU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x0299U;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x0299U;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x0299U;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x0299U;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x0299U;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x0299U;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x0299U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x0299U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x0299U;
            } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000800U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00001000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00002000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000800U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00001000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00002000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000800U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00001000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00002000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000800U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00001000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00002000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
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
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1855: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1855, "");
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1997: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1997, "");
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:2139: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 2139, "");
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
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x0100U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01cdU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01c1U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x006bU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x0107U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x010fU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x0299U;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x0299U;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x0299U;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x0299U;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x0299U;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x0299U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x0299U;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x0299U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x0299U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x0299U;
            } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000800U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00001000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00002000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000800U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00001000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00002000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000800U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00001000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00002000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000800U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00001000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00002000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
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
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1838: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1838, "");
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1980: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1980, "");
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:2122: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 2122, "");
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
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01d9U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x015bU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01c9U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01d9U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00001000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01c9U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01cfU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x015aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c7U;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01cfU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x015aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x021cU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c7U;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01cfU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x015aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x0103U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c7U;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01cfU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x015aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01c2U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c7U;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01cfU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x015aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01e3U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c7U;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01cfU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x015aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x000aU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c7U;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01cfU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x015aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01e2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c7U;
            } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01cfU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x015aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c7U;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01cfU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x015aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c7U;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x00eaU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x00eaU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x015bU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x00eaU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c9U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000800U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00001000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00002000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
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
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1821: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1821, "");
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1963: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1963, "");
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:2105: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 2105, "");
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
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01d5U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01c5U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01c5U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
                } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01d5U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0x00001000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01c5U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
                } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x000bU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x000bU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01cbU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x000bU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01cbU;
                } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x000fU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x000fU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01cbU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x000fU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01cbU;
                } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x0179U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x0179U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01cbU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x0179U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01cbU;
                } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01c6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01c6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01cbU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01c6U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01cbU;
                } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01e7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01e7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01cbU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01e7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01cbU;
                } else {
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x000eU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d7U;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x000eU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01cbU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                        = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x000eU;
                    vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01cbU;
                }
            } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01e6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01e6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01cbU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01e6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01cbU;
            } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01c6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01c6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01cbU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01c6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01cbU;
            } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01e7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01e7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01cbU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01e7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01cbU;
            } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x00a7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d5U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x00a7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01c5U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x00a7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c5U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000800U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00001000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00002000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
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
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1804: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1804, "");
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1946: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1946, "");
                        VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:2088: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1)
                                     , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                        VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 2088, "");
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
                    = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01d1U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01c1U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01c1U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01d1U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00001000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01c1U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01c3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c3U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01c3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c3U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01c3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c3U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01c3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c3U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01c3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c3U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01c3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c3U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d3U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01c3U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c3U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01c2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d3U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01c2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01c3U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01c2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c3U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01e3U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d3U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01e3U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01c3U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01e3U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c3U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x00eaU;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d1U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x00eaU;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01c1U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x00eaU;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c1U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000800U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00001000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00002000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1787: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1787, "");
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1929: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1929, "");
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:2071: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 2071, "");
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
                    = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01d1U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01c1U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01c1U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000800U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00001000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00002000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01c3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c3U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01c3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c3U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01c3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c3U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01c3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c3U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01c3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c3U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01c3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c3U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d3U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01c3U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c3U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01c2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d3U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01c2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01c3U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01c2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c3U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x01e3U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d3U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x01e3U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01c3U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x01e3U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c3U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xf7ffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0x00eaU;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0x01d1U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xefffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0x00eaU;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0x01c1U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xdfffU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0x00eaU;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0x01c1U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000800U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[11U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[11U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00001000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[12U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[12U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00002000U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[13U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[13U] = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1770: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1770, "");
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:1912: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 1912, "");
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:2054: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 2054, "");
                }
            }
        }
    }
    if ((0U == (0x00000027U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                               >> 6U)))) {
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
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0133U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x02b8U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x02b8U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x02b8U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x02b8U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x02b8U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x02b8U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x02b8U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:548: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 548, "");
                }
            }
        }
    } else if ((1U == (0x00000027U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                      >> 6U)))) {
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
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0133U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x02b8U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x02b8U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x02b8U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x02b8U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x02b8U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x02b8U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x02b8U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:566: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 566, "");
                }
            }
        }
    } else if ((2U == (0x00000027U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                      >> 6U)))) {
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
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0137U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x000bU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x02bcU;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x000fU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x02bcU;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0179U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x02bcU;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01c6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x02bcU;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x02bcU;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x000eU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x02bcU;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e6U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x02bcU;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:584: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 584, "");
                }
            }
        }
    } else if ((3U == (0x0000003fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                      >> 6U)))) {
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
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x03a5U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x03a1U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x03a1U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x03a1U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x03a1U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x03a1U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x03a1U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x03a1U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:602: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 602, "");
                }
            }
        }
    } else if ((0x13U == (0x0000003fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                         >> 6U)))) {
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
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0301U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0159U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0159U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0159U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0159U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0159U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0159U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0159U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01c2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0159U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e3U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0159U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x00eaU;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0301U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:620: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 620, "");
                }
            }
        }
    } else if ((0x1bU == (0x0000003fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                         >> 6U)))) {
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
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0301U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0159U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0159U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0159U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0159U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0159U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0159U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0159U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01c2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0159U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e3U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0159U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x00eaU;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0301U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:638: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 638, "");
                }
            }
        }
    } else if ((0x20U == (0x0000003fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                         >> 6U)))) {
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
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x013bU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x015cU;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x015cU;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x015cU;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x015cU;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x015cU;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x015cU;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x015cU;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:656: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 656, "");
                }
            }
        }
    } else if ((0x21U == (0x0000003fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                         >> 6U)))) {
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
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0341U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x017cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x017dU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01ffU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0178U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01faU;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x017dU;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01ffU;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:674: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 674, "");
                }
            }
        }
    } else if ((0x22U == (0x0000003fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                         >> 6U)))) {
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
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0133U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x03a0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x03a4U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01f1U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0325U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01edU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e5U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:692: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 692, "");
                }
            }
        }
    } else if ((0x23U == (0x0000003fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                         >> 6U)))) {
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
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0232U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x03a0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x03a4U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01f1U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0325U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01edU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e5U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:710: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 710, "");
                }
            }
        }
    } else if ((0x28U == (0x0000003fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                         >> 6U)))) {
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
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x012dU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x03c3U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x03c3U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x03c3U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x03c3U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x03c3U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x03c3U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x03c3U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:728: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 728, "");
                }
            }
        }
    } else if ((0x29U == (0x0000003fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                         >> 6U)))) {
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
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x012dU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x03c3U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x03c3U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x03c3U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x03c3U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x03c3U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x03c3U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x03c3U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:746: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 746, "");
                }
            }
        }
    } else if ((0x2aU == (0x0000003fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                         >> 6U)))) {
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
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0125U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x000bU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x03cbU;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x000fU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x03cbU;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0179U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x03cbU;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01c6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x03cbU;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x03cbU;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x000eU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x03cbU;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e6U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x03cbU;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:764: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 764, "");
                }
            }
        }
    } else if ((0x2bU == (0x0000003fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                         >> 6U)))) {
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
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0345U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0343U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0343U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0343U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0343U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0343U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0343U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0343U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:782: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 782, "");
                }
            }
        }
    } else if ((0x00000032U == (0x0000003eU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                               >> 6U)))) {
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
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0127U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0123U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01fdU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01f5U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01f9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e9U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01fdU;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01f5U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:800: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 800, "");
                }
            }
        }
    } else if ((6U == (7U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                             >> 6U)))) {
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
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0152U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0151U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0151U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0151U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0151U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0151U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0151U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0151U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01c2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0151U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01e3U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0151U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x00eaU;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0x0152U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:818: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 818, "");
                }
            }
        }
    } else if ((7U == (7U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                             >> 6U)))) {
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
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x02f1U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x02f2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01fbU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0275U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x03e4U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x02f2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01fbU;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:836: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 836, "");
                }
            }
        }
    } else if ((0x3aU == (0x0000003fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                         >> 6U)))) {
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
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0273U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x02b0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01f3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0293U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01f2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x02b0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01f3U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:854: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 854, "");
                }
            }
        }
    } else if ((0x3bU == (0x0000003fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                         >> 6U)))) {
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
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0255U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x02b4U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01f7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x0297U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01f6U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x02b4U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0x01f7U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:872: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 872, "");
                }
            }
        }
    } else if ((0x39U == (0x0000003fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                         >> 6U)))) {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
            = ((0xffefU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll)) 
               | ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__illMisc) 
                  << 4U));
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] 
            = vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__a1Misc;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
    } else {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
            = (0x00000010U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[4U] = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[4U] = 0U;
    }
    if ((8U == (0x0000001fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                               >> 6U)))) {
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
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x0100U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01ccU;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:178: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 178, "");
                }
            }
        }
    } else if ((0U == (0x00000037U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                      >> 6U)))) {
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
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x0100U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01ccU;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:196: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 196, "");
                }
            }
        }
    } else if ((9U == (0x0000001fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                      >> 6U)))) {
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
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x0100U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01ccU;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:214: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 214, "");
                }
            }
        }
    } else if ((1U == (0x00000037U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                      >> 6U)))) {
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
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x0100U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01ccU;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:232: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 232, "");
                }
            }
        }
    } else if ((0x0000000aU == (0x0000001fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                               >> 6U)))) {
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
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x010cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x000bU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x029dU;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x000fU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x029dU;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x0179U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x029dU;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01c6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x029dU;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x029dU;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x000eU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x029dU;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e6U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x029dU;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:250: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 250, "");
                }
            }
        }
    } else if ((2U == (0x00000037U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                      >> 6U)))) {
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
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x010cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x000bU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x029dU;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x000fU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x029dU;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x0179U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x029dU;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01c6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x029dU;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x029dU;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x000eU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x029dU;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e6U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x029dU;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:268: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 268, "");
                }
            }
        }
    } else if ((0x00000010U == (0x00000037U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                               >> 6U)))) {
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
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x0100U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:286: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 286, "");
                }
            }
        }
    } else if ((0x00000011U == (0x00000037U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                               >> 6U)))) {
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
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x0100U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0299U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:304: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 304, "");
                }
            }
        }
    } else if ((0x00000012U == (0x00000037U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                               >> 6U)))) {
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
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x010cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x000bU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x029dU;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x000fU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x029dU;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x0179U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x029dU;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01c6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x029dU;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x029dU;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x000eU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x029dU;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e6U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x029dU;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:322: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 322, "");
                }
            }
        }
    } else if ((4U == (7U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                             >> 6U)))) {
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
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x01d2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0215U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0215U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0215U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0215U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0215U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0215U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0215U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x01c2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0215U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x01e3U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0215U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x00eaU;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x00abU;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:340: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 340, "");
                }
            }
        }
    } else if ((5U == (7U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                             >> 6U)))) {
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
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03efU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x01d6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0081U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0081U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0081U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0081U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0081U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0081U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0081U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:358: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 358, "");
                }
            }
        }
    } else if ((7U == (7U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                             >> 6U)))) {
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
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03efU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x01ceU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0081U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0081U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0081U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0081U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0081U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0081U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0081U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:376: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 376, "");
                }
            }
        }
    } else if ((6U == (7U & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                             >> 6U)))) {
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
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03ebU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x01caU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0069U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0069U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0069U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0069U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0069U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0069U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0069U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:394: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 394, "");
                }
            }
        }
    } else if ((0x20U == (0x0000003fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                         >> 6U)))) {
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
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x03e7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0215U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0215U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0215U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0215U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0215U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0215U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0215U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01c2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0215U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e3U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0215U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:412: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 412, "");
                }
            }
        }
    } else if ((0x21U == (0x0000003fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                         >> 6U)))) {
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
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x03efU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0081U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0081U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0081U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0081U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0081U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0081U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0081U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:430: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 430, "");
                }
            }
        }
    } else if ((0x23U == (0x0000003fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                         >> 6U)))) {
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
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x03efU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0081U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0081U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0081U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0081U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0081U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0081U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0081U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:448: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 448, "");
                }
            }
        }
    } else if ((0x22U == (0x0000003fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                         >> 6U)))) {
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
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x03ebU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0069U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0069U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0069U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0069U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0069U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0069U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0069U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:466: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 466, "");
                }
            }
        }
    } else if ((0x30U == (0x0000003fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                         >> 6U)))) {
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
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x0108U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0087U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0087U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0087U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0087U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0087U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0087U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0087U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:484: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 484, "");
                }
            }
        }
    } else if ((0x31U == (0x0000003fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                         >> 6U)))) {
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
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x0108U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0087U;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x021cU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0087U;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x0103U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0087U;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01c2U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0087U;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e3U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0087U;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x000aU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0087U;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x02b9U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e2U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x0087U;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:502: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 502, "");
                }
            }
        }
    } else if ((0x32U == (0x0000003fU & ((IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__Ir) 
                                         >> 6U)))) {
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
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x0104U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((1U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
            } else if ((2U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x000bU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x008fU;
            } else if ((3U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x000fU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x008fU;
            } else if ((4U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x0179U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x008fU;
            } else if ((5U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01c6U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x008fU;
            } else if ((6U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e7U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x008fU;
            } else {
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                    = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x000eU;
                vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x008fU;
            }
        } else if ((8U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (0xfffeU & (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0x03e0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0x01e6U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0x008fU;
        } else if ((9U == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0aU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else if ((0x0bU == (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol))) {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
        } else {
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
                = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
            vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
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
                    VL_WRITEF_NX("[%0t] %%Error: uaddrPla.sv:520: Assertion failed in %m: unique case, but multiple matches found for '32'h%X'\n",4, 'M',vlSymsp->name(),"sys_top.sys.cpu.uaddrDecode.pla_lined", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1)
                                 , '#',32,(IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__eaCol));
                    VL_STOP_MT("../third_party/fx68k/uaddrPla.sv", 520, "");
                }
            }
        }
    } else {
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll 
            = (1U | (IData)(vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arIll));
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA1[0U] = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__arA23[0U] = 0U;
        vlSelfRef.sys_top__DOT__sys__DOT__cpu__DOT__uaddrDecode__DOT__pla_lined__DOT__scA3 = 0U;
    }
}
