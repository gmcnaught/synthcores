// SPDX-License-Identifier: GPL-2.0-or-later
// Co-simulation stand-in for Nuked-MT32's mame/i8x9x.h: the same classes and the members the
// board, LA32 and LCD code use, implemented on the Verilated HDL (hdl/mcs96/mcs96.sv).
// build.sh copies this file over the reference's mame/i8x9x.h in the co-simulation source tree.
#pragma once
#include <stdint.h>

class mt32_t;
class Vmcs96;

class mcs96_device {
public:
    void clock(uint64_t target);
    uint64_t total_cycles();
    virtual void device_start();
    virtual void device_reset();
    virtual ~mcs96_device();

protected:
    explicit mcs96_device(mt32_t* m) : mt32(m) {}
    mt32_t* mt32;
    Vmcs96* top = nullptr;
    bool ack_given = false;
    void tick();
};

class i8x9x_device : public mcs96_device {
public:
    enum { EXTINT_LINE = 0, HSI0_LINE, HSI1_LINE, HSI2_LINE, HSI3_LINE };
    void serial_w(uint8_t val);
    uint8_t pwm_value();
    void execute_set_input(int linenum, int state);
    uint8_t la32_sh3 = 0;

protected:
    explicit i8x9x_device(mt32_t* m) : mcs96_device(m) {}
};

class p8098_device : public i8x9x_device {
public:
    explicit p8098_device(mt32_t* m) : i8x9x_device(m) {}
};
