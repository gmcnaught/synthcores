// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef VERILATED_VSYS_TOP__SYMS_H_
#define VERILATED_VSYS_TOP__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODEL CLASS

#include "Vsys_top.h"

// INCLUDE MODULE CLASSES
#include "Vsys_top___024root.h"
#include "Vsys_top___024unit.h"

// SYMS CLASS (contains all model state)
class alignas(VL_CACHE_LINE_BYTES) Vsys_top__Syms final : public VerilatedSyms {
  public:
    // INTERNAL STATE
    Vsys_top* const __Vm_modelp;
    VlDeleter __Vm_deleter;
    bool __Vm_didInit = false;

    // MODULE INSTANCE STATE
    Vsys_top___024root             TOP;

    // CONSTRUCTORS
    Vsys_top__Syms(VerilatedContext* contextp, const char* namep, Vsys_top* modelp);
    ~Vsys_top__Syms();

    // METHODS
    const char* name() const { return TOP.vlNamep; }
};

#endif  // guard
