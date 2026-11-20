// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vsys_top.h for the primary calling header

#include "Vsys_top__pch.h"


Vsys_top___024unit::Vsys_top___024unit() = default;
Vsys_top___024unit::~Vsys_top___024unit() = default;

void Vsys_top___024unit::ctor(Vsys_top__Syms* symsp, const char* namep) {
    vlSymsp = symsp;
    vlNamep = strdup(Verilated::catName(vlSymsp->name(), namep));
    // Reset structure values
}

void Vsys_top___024unit::__Vconfigure(bool first) {
    (void)first;  // Prevent unused variable warning
}

void Vsys_top___024unit::dtor() {
    VL_DO_DANGLING(std::free(const_cast<char*>(vlNamep)), vlNamep);
}
