// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary model header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef VERILATED_VSYS_TOP_H_
#define VERILATED_VSYS_TOP_H_  // guard

#include "verilated.h"

class Vsys_top__Syms;
class Vsys_top___024root;
#include "Vsys_top___024unit.h"


// This class is the main interface to the Verilated model
class alignas(VL_CACHE_LINE_BYTES) Vsys_top VL_NOT_FINAL : public VerilatedModel {
  private:
    // Symbol table holding complete model state (owned by this class)
    Vsys_top__Syms* const vlSymsp;

  public:

    // CONSTEXPR CAPABILITIES
    // Verilated with --trace?
    static constexpr bool traceCapable = false;

    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(&clk,0,0);
    VL_IN8(&reset,0,0);
    VL_IN8(&loading,0,0);
    VL_IN8(&ioctl_wr,0,0);
    VL_IN8(&ioctl_dout,7,0);
    VL_IN8(&midi_in,0,0);
    VL_OUT8(&midi_out,0,0);
    VL_IN8(&DDRAM_BUSY,0,0);
    VL_OUT8(&DDRAM_BURSTCNT,7,0);
    VL_IN8(&DDRAM_DOUT_READY,0,0);
    VL_OUT8(&DDRAM_RD,0,0);
    VL_OUT8(&DDRAM_BE,7,0);
    VL_OUT8(&DDRAM_WE,0,0);
    VL_OUT8(&bus_wr_strobe,0,0);
    VL_OUT8(&bus_wr_ds,1,0);
    VL_OUT8(&sample_strobe,0,0);
    VL_IN8(&lcd_line,0,0);
    VL_IN8(&lcd_pos,3,0);
    VL_OUT8(&lcd_code,7,0);
    VL_OUT16(&bus_wr_data,15,0);
    VL_OUT16(&out_l,15,0);
    VL_OUT16(&out_r,15,0);
    VL_IN(&ioctl_addr,26,0);
    VL_OUT(&DDRAM_ADDR,28,0);
    VL_OUT(&bus_wr_addr,23,0);
    VL_IN64(&DDRAM_DOUT,63,0);
    VL_OUT64(&DDRAM_DIN,63,0);

    // CELLS
    // Public to allow access to /* verilator public */ items.
    // Otherwise the application code can consider these internals.
    Vsys_top___024unit* const __PVT____024unit;

    // Root instance pointer to allow access to model internals,
    // including inlined /* verilator public_flat_* */ items.
    Vsys_top___024root* const rootp;

    // CONSTRUCTORS
    /// Construct the model; called by application code
    /// If contextp is null, then the model will use the default global context
    /// If name is "", then makes a wrapper with a
    /// single model invisible with respect to DPI scope names.
    explicit Vsys_top(VerilatedContext* contextp, const char* name = "TOP");
    explicit Vsys_top(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    virtual ~Vsys_top();
  private:
    VL_UNCOPYABLE(Vsys_top);  ///< Copying not allowed

  public:
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval() { eval_step(); }
    /// Evaluate when calling multiple units/models per time step.
    void eval_step();
    /// Evaluate at end of a timestep for tracing, when using eval_step().
    /// Application must call after all eval() and before time changes.
    void eval_end_step() {}
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    /// Are there scheduled events to handle?
    bool eventsPending();
    /// Returns time at next time slot. Aborts if !eventsPending()
    uint64_t nextTimeSlot();
    /// Trace signals in the model; called by application code
    void trace(VerilatedTraceBaseC* tfp, int levels, int options = 0) { contextp()->trace(tfp, levels, options); }
    /// Retrieve name of this model instance (as passed to constructor).
    const char* name() const;

    // Abstract methods from VerilatedModel
    const char* hierName() const override final;
    const char* modelName() const override final;
    unsigned threads() const override final;
    /// Prepare for cloning the model at the process level (e.g. fork in Linux)
    /// Release necessary resources. Called before cloning.
    void prepareClone() const;
    /// Re-init after cloning the model at the process level (e.g. fork in Linux)
    /// Re-allocate necessary resources. Called after cloning.
    void atClone() const;
  private:
    // Internal functions - trace registration
    void traceBaseModel(VerilatedTraceBaseC* tfp, int levels, int options);
};

#endif  // guard
