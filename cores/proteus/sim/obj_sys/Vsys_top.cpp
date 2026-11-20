// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vsys_top__pch.h"

//============================================================
// Constructors

Vsys_top::Vsys_top(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vsys_top__Syms(contextp(), _vcname__, this)}
    , clk{vlSymsp->TOP.clk}
    , reset{vlSymsp->TOP.reset}
    , loading{vlSymsp->TOP.loading}
    , ioctl_wr{vlSymsp->TOP.ioctl_wr}
    , ioctl_dout{vlSymsp->TOP.ioctl_dout}
    , midi_in{vlSymsp->TOP.midi_in}
    , midi_out{vlSymsp->TOP.midi_out}
    , DDRAM_BUSY{vlSymsp->TOP.DDRAM_BUSY}
    , DDRAM_BURSTCNT{vlSymsp->TOP.DDRAM_BURSTCNT}
    , DDRAM_DOUT_READY{vlSymsp->TOP.DDRAM_DOUT_READY}
    , DDRAM_RD{vlSymsp->TOP.DDRAM_RD}
    , DDRAM_BE{vlSymsp->TOP.DDRAM_BE}
    , DDRAM_WE{vlSymsp->TOP.DDRAM_WE}
    , bus_wr_strobe{vlSymsp->TOP.bus_wr_strobe}
    , bus_wr_ds{vlSymsp->TOP.bus_wr_ds}
    , sample_strobe{vlSymsp->TOP.sample_strobe}
    , lcd_line{vlSymsp->TOP.lcd_line}
    , lcd_pos{vlSymsp->TOP.lcd_pos}
    , lcd_code{vlSymsp->TOP.lcd_code}
    , bus_wr_data{vlSymsp->TOP.bus_wr_data}
    , out_l{vlSymsp->TOP.out_l}
    , out_r{vlSymsp->TOP.out_r}
    , ioctl_addr{vlSymsp->TOP.ioctl_addr}
    , DDRAM_ADDR{vlSymsp->TOP.DDRAM_ADDR}
    , bus_wr_addr{vlSymsp->TOP.bus_wr_addr}
    , DDRAM_DOUT{vlSymsp->TOP.DDRAM_DOUT}
    , DDRAM_DIN{vlSymsp->TOP.DDRAM_DIN}
    , __PVT____024unit{vlSymsp->TOP.__PVT____024unit}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vsys_top::Vsys_top(const char* _vcname__)
    : Vsys_top(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vsys_top::~Vsys_top() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vsys_top___024root___eval_debug_assertions(Vsys_top___024root* vlSelf);
#endif  // VL_DEBUG
void Vsys_top___024root___eval_static(Vsys_top___024root* vlSelf);
void Vsys_top___024root___eval_initial(Vsys_top___024root* vlSelf);
void Vsys_top___024root___eval_settle(Vsys_top___024root* vlSelf);
void Vsys_top___024root___eval(Vsys_top___024root* vlSelf);

void Vsys_top::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vsys_top::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vsys_top___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vsys_top___024root___eval_static(&(vlSymsp->TOP));
        Vsys_top___024root___eval_initial(&(vlSymsp->TOP));
        Vsys_top___024root___eval_settle(&(vlSymsp->TOP));
        vlSymsp->__Vm_didInit = true;
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vsys_top___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vsys_top::eventsPending() { return false; }

uint64_t Vsys_top::nextTimeSlot() {
    VL_FATAL_MT(__FILE__, __LINE__, "", "No delays in the design");
    return 0;
}

//============================================================
// Utilities

const char* Vsys_top::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vsys_top___024root___eval_final(Vsys_top___024root* vlSelf);

VL_ATTR_COLD void Vsys_top::final() {
    contextp()->executingFinal(true);
    Vsys_top___024root___eval_final(&(vlSymsp->TOP));
    contextp()->executingFinal(false);
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vsys_top::hierName() const { return vlSymsp->name(); }
const char* Vsys_top::modelName() const { return "Vsys_top"; }
unsigned Vsys_top::threads() const { return 1; }
void Vsys_top::prepareClone() const { contextp()->prepareClone(); }
void Vsys_top::atClone() const {
    contextp()->threadPoolpOnClone();
}
