// SPDX-License-Identifier: GPL-2.0-or-later
// The Verilated 8098 (hdl/mcs96/mcs96.sv) behind Nuked-MT32's CPU interface, so the reference's
// board, LA32, reverb and LCD code drive it unchanged (plan milestones M1 and M2). The trace hooks
// fire from the HDL's debug outputs with the same text as the reference's.
#include "mt32.h"
#include "mame/i8x9x.h"
#include "Vmcs96.h"
#include "verilated.h"
#include <cstdio>
#include <cstdlib>

static uint64_t g_ticks;

mcs96_device::~mcs96_device() { delete top; }

void mcs96_device::device_start()
{
    top = new Vmcs96;
    top->clk = 0;
    top->rst = 1;
    top->run = 0;
    top->ext_ack = 0;
    top->serial_in = 0;
    top->extint = 0;
    top->la32_sh3 = 0;
    top->knob = 1020;
    for (int i = 0; i < 4; i++) tick();
    top->rst = 0;
    while (!top->ready) tick();
}

void mcs96_device::device_reset() {}

void mcs96_device::tick()
{
    top->clk = 1;
    top->eval();
    g_ticks++;
    // pulses of the edge just taken
    if (top->dbg_irq && mt32h_irq) mt32h_irq(top->dbg_level, top->dbg_pc, top->total_cycles);
    if (top->dbg_fetch && mt32h_fetch) mt32h_fetch(top->dbg_fpc, top->dbg_psw, top->total_cycles);
    if (top->dbg_regw && mt32h_regw) mt32h_regw(top->dbg_addr, top->dbg_data, top->total_cycles);
    if (top->tx_stb && mt32h_tx) mt32h_tx(top->tx_byte, top->total_cycles);
    if (top->bad_op) { fprintf(stderr, "mcs96: opcode without a handler at %04x\n", top->dbg_pc); exit(3); }
    // one external access per request (taken only while ack is low), acknowledged for one clock
    if (top->ext_req && !top->ext_ack) {
        if (top->ext_we) {
            mt32->cpu_write(top->ext_addr, top->ext_wdata);
            top->ext_rdata = 0;
        } else {
            top->ext_rdata = mt32->cpu_read(top->ext_addr);
        }
        top->ext_ack = 1;
    } else {
        top->ext_ack = 0;
    }
    top->clk = 0;
    top->eval();
}

void mcs96_device::clock(uint64_t target)
{
    auto* d = static_cast<i8x9x_device*>(this);
    top->la32_sh3 = d->la32_sh3;
    top->knob = mt32->get_knob();
    top->target = target;
    top->run = 1;
    tick();
    top->run = 0;
    uint64_t n = 0;
    while (top->busy) {
        tick();
        if (++n == 2000000) {
            fprintf(stderr, "mcs96: no progress in clock(%llu): pc %04x total_cycles %llu\n",
                    (unsigned long long)target, top->dbg_pc, (unsigned long long)top->total_cycles);
            exit(4);
        }
    }
}

uint64_t mcs96_device::total_cycles() { return top->total_cycles; }

void i8x9x_device::serial_w(uint8_t val)
{
    top->serial_in = 1;
    top->serial_byte = val;
    tick();
    top->serial_in = 0;
}

uint8_t i8x9x_device::pwm_value() { return top->pwm; }

void i8x9x_device::execute_set_input(int linenum, int state)
{
    if (mt32h_input) mt32h_input(linenum, state, total_cycles());
    if (linenum == EXTINT_LINE) top->extint = state ? 1 : 0;
}
