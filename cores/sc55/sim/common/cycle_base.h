#pragma once
// --cycle-base B (sim only): start the time counters of the HDL and of the model at B instead of 0,
// so a short run crosses a counter wrap that real time reaches only after days or months
// (review 2026-11-26 roland.md #9, #10). Apply right after both have reset, before the first step:
// then every counter is still at its reset value and moving the origin by B changes nothing else.
//
// What moves (model -> HDL):
//   mcu.cycles = B                 -> mcu.cycles
//   timer.cycles, frt[i].deadline  -> mcu.frt_deadline[i] = B / 2   (tmr.deadline stays "never")
//   pcm.cycles = B                 -> pcm.pcm_cycles               (system bench only)
//   sm.cycles, sm.timer_cycles     -> sm.cycles, sm.timer_cycles = 5 B, sm.cyc48 = 5 B + 48,
//                                     mcu.sm_tgt5 = 5 B             (mk2 only)
// The UART and A/D delays are 0 ("expired" / idle) and stay 0, which B does not change.
// B must be a multiple of 3 * 2^20: a multiple of 12 (one step) whose half is a multiple of every
// timer stride and whose fivefold is a multiple of the sub-MCU timer's 16.
#include <cstdint>
#include <cstdio>

#include "mcu.h"
#include "mcu_timer.h"
#include "pcm.h"
#include "submcu.h"

inline bool cycle_base_ok(uint64_t b)
{
    if (b % (3ull << 20) != 0)
    {
        fprintf(stderr, "--cycle-base must be a multiple of 3 * 2^20 (3145728)\n");
        return false;
    }
    return true;
}

inline void model_cycle_base(mcu_t& m, pcm_t& p, bool mk2, uint64_t b)
{
    m.cycles = b;
    m.timer->cycles = b / 2;
    for (auto& f : m.timer->frt) f.deadline = b / 2;
    p.cycles = b;
    if (mk2)
    {
        m.sm->cycles = 5 * b;
        m.sm->timer_cycles = 5 * b;
    }
}

// Write a register and Verilator's delayed-assignment shadow, if it has one (the shadow is copied
// back into the register on every clock edge whether or not the register is assigned).
#define CB_POKE(R, sig, v)                                                     \
    [&](auto* r_) {                                                            \
        r_->sig = (v);                                                         \
        if constexpr (requires { r_->__Vdly__##sig; }) r_->__Vdly__##sig = (v); \
    }(R)

// P: the sc55_mcu instance's prefix, e.g. sc55_mcu__DOT__ or sc55_core__DOT__mcu__DOT__.
#define HDL_MCU_CYCLE_BASE(R, P, mk2, b)                                   \
    do {                                                                   \
        CB_POKE(R, P##cycles, (b));                                        \
        for (int i_ = 0; i_ < 3; ++i_) (R)->P##frt_deadline[i_] = (b) / 2; \
        if (mk2)                                                           \
        {                                                                  \
            CB_POKE(R, P##sm_tgt5, 5 * (b));                               \
            CB_POKE(R, P##sm__DOT__cycles, 5 * (b));                       \
            CB_POKE(R, P##sm__DOT__timer_cycles, 5 * (b));                 \
            CB_POKE(R, P##sm__DOT__cyc48, 5 * (b) + 48);                   \
        }                                                                  \
    } while (0)
