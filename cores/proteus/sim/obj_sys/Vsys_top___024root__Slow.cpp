// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vsys_top.h for the primary calling header

#include "Vsys_top__pch.h"

void Vsys_top___024root___ctor_var_reset(Vsys_top___024root* vlSelf);

Vsys_top___024root::Vsys_top___024root(Vsys_top__Syms* symsp, const char* namep)
 {
    vlSymsp = symsp;
    vlNamep = strdup(namep);
    // Reset structure values
    Vsys_top___024root___ctor_var_reset(this);
}

void Vsys_top___024root::__Vconfigure(bool first) {
    (void)first;  // Prevent unused variable warning
}

Vsys_top___024root::~Vsys_top___024root() {
    VL_DO_DANGLING(std::free(const_cast<char*>(vlNamep)), vlNamep);
}
