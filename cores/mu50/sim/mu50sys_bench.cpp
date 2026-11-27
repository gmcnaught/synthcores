// SPDX-License-Identifier: MIT
// U2 bench: h8h_sys (H8/3003 core, peripherals, on-chip RAM) on a software MU50 board, writing a
// trace in the patched MAME's format (MU_TRACE without MU_BUS: instruction lines and writes).
//
//   mu50sys_bench FIRMWARE.bin N OUT.trace [--rx RXLOG] [--lcd LCDLOG] [--swp SWPREADS]
//
// FIRMWARE.bin is the program ROM in CPU byte order. The board is MAME's ymmu50.cpp:
//   000000-07FFFF ROM, 200000-20FFFF RAM (zeroed), 400000-4007FF SWP00 (writes ignored, reads
//   from --swp: one hex byte a line, in order, as MAME's bus log gives them);
//   port 6 / port A: the LCD (MAME's hd44780 in 8-bit mode), port B LEDs, port C keys (none
//   pressed), ports 7 and 9 read 0; A/D AN0 3FF, AN2 3FF, AN6 200, others 0; IRQ0 asserted.
// Pin changes MAME makes between timeslices are replayed at the CPU position MAME logged:
//   --rx  MU_RX_LOG lines "<sci> <CPU states> <now cycles> <state>"
//   --lcd MU_LCD_LOG lines "<CPU states> <time>": the LCD busy flag clears there (default: the
//         rule below).
// LCD busy flag without --lcd: MAME's timer clears it at t_w + n / 270 kHz (t_w: the end of the
// port 6 write that drops E; n = 10, or 410 for clear and home and at reset). MAME's H8 ends its
// timeslice after the first bus access ending at or after floor(clear time) (icount <= 0, even
// mid-instruction), or at the end of an instruction whose internal operations crossed it, and
// the timer runs there; so a read sees busy while both the access before it and the start of
// the current step (instruction, dispatch step, interrupt entry) are before t_w + 370 (n = 10)
// or t_w + 15185 (n = 410).
#include "Vh8h_sys.h"
#include "verilated.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <string>
#include <vector>

enum { OP_F16 = 1, OP_R8 = 2, OP_R16 = 3, OP_W8 = 4, OP_W16 = 5 };

static std::vector<uint8_t> rom(0x80000), ram(0x10000);
static std::deque<uint8_t> swp_reads;

// ---- LCD (hd44780_base_device, 8-bit interface)
struct Lcd {
    uint8_t ddram[0x80], cgram[0x40];
    int ac = 0, active_ddram = 1, direction = 1, num_line = 1, data_len = 8, disp_shift = 0;
    bool busy = true, shift_on = false, first_cmd = true;
    bool use_rule = true;
    uint64_t t_w = 0, busy_until = 15185, prev_end = 0, step_start = 0;   // reset: n = 410 at 0
    void set_busy(int n) {
        busy = true; busy_until = t_w + (n == 10 ? 370 : 15185);
        if (getenv("LCD_DEBUG")) fprintf(stderr, "lcd set_busy %d at %llu -> %llu\n", n, (unsigned long long)t_w, (unsigned long long)busy_until);
    }
    bool busy_now() const { return use_rule ? (prev_end < busy_until && step_start < busy_until) : busy; }
    uint8_t ir = 0, dr = 0;
    Lcd() { memset(ddram, 0x20, sizeof ddram); memset(cgram, 0, sizeof cgram); }
    void correct_ac() {
        if (active_ddram) {
            int max_ac = num_line == 1 ? 0x4f : 0x67;
            if (ac > max_ac) ac -= max_ac + 1;
            else if (ac < 0) ac = max_ac;
            else if (num_line == 2 && ac > 0x27 && ac < 0x40) ac = 0x40 + (ac - 0x28);
        } else
            ac &= 0x3f;
    }
    void update_ac(int d) {
        if (active_ddram && num_line == 2 && d == -1 && ac == 0x40) ac = 0x27;
        else ac += d;
        correct_ac();
    }
    void shift_display(int d) {
        disp_shift += d;
        if (disp_shift == 0x50) disp_shift = 0;
        else if (disp_shift == -1) disp_shift = 0x4f;
    }
    void control_write(uint8_t data) {
        ir = data;
        if (ir & 0x80) { active_ddram = 1; ac = ir & 0x7f; correct_ac(); set_busy(10); return; }
        if (ir & 0x40) { active_ddram = 0; ac = ir & 0x3f; set_busy(10); return; }
        if (ir & 0x20) {
            if (!first_cmd && data_len == ((ir & 0x10) ? 8 : 4) &&
                (((ir & 0x04) ? 10 : 8) != 8 || num_line != ((ir >> 3 & 1) + 1)))
                return;
            first_cmd = true;
            data_len = (ir & 0x10) ? 8 : 4;
            num_line = (ir >> 3 & 1) + 1;
            correct_ac();
            set_busy(10);
            return;
        }
        if (ir & 0x10) {
            int d = (ir & 0x04) ? 1 : -1;
            if (ir & 0x08) shift_display(d); else update_ac(d);
            set_busy(10);
        } else if (ir & 0x08) set_busy(10);
        else if (ir & 0x04) { direction = (ir & 0x02) ? 1 : -1; shift_on = ir & 1; set_busy(10); }
        else if (ir & 0x02) { ac = 0; active_ddram = 1; direction = 1; disp_shift = 0; set_busy(410); }
        else if (ir & 0x01) {
            ac = 0; active_ddram = 1; direction = 1; disp_shift = 0;
            memset(ddram, 0x20, sizeof ddram);
            set_busy(410);
            return;
        }
        first_cmd = false;
    }
    uint8_t control_read() const { return (busy_now() ? 0x80 : 0) | (ac & 0x7f); }
    void data_write(uint8_t data) {
        dr = data;
        if (active_ddram) ddram[ac & 0x7f] = dr; else cgram[ac & 0x3f] = dr;
        set_busy(10);
        update_ac(direction);
        if (shift_on) shift_display(direction);
    }
    uint8_t data_peek() const { return active_ddram ? ddram[ac & 0x7f] : cgram[ac & 0x3f]; }
    void data_read_effects() { set_busy(10); update_ac(direction); }
} lcd;

// ---- board (mu50_state)
static uint8_t cur_p6 = 0xff, cur_p9 = 0xff, cur_pa = 0xff, cur_pb = 0xff, cur_pc = 0xff;
enum { P6_LCD_ENABLE = 1, P6_LCD_RW = 2, P6_LCD_RS = 4 };

static void port_write(int port, uint8_t data) {
    switch (port) {
    case 5:
        data ^= P6_LCD_ENABLE;
        if ((cur_p6 & P6_LCD_ENABLE) && !(data & P6_LCD_ENABLE) && !(cur_p6 & P6_LCD_RW)) {
            if (cur_p6 & P6_LCD_RS) lcd.data_write(cur_pa); else lcd.control_write(cur_pa);
        }
        cur_p6 = data;
        break;
    case 8: cur_p9 = data; break;
    case 9: cur_pa = data; break;
    case 10: cur_pb = data; break;
    case 11: cur_pc = data; break;
    default: break;
    }
}
static uint8_t port_value(int port) {
    switch (port) {
    case 5: return cur_p6;
    case 9:
        if (cur_p6 & P6_LCD_ENABLE) {
            if (cur_p6 & P6_LCD_RW) return (cur_p6 & P6_LCD_RS) ? lcd.data_peek() : lcd.control_read();
            return 0x00;
        }
        return cur_pa;
    case 10: return cur_pb;
    case 11: return cur_pc | 0x7c;     // no key pressed
    default: return 0x00;              // ports 7 and 9 read 0
    }
}
static void port_read_effects(int port) {
    if (port == 9 && (cur_p6 & P6_LCD_ENABLE) && (cur_p6 & P6_LCD_RW) && (cur_p6 & P6_LCD_RS))
        lcd.data_read_effects();
}

static uint8_t rd8(uint32_t a) {
    a &= 0xffffff;
    if (a < 0x80000) return rom[a];
    if (a >= 0x200000 && a < 0x210000) return ram[a - 0x200000];
    if (a >= 0x400000 && a < 0x400800) {
        if (swp_reads.empty()) { fprintf(stderr, "SWP00 read %06X past the --swp list\n", a); return 0; }
        uint8_t v = swp_reads.front();
        swp_reads.pop_front();
        return v;
    }
    return 0;
}
static void wr8(uint32_t a, uint8_t v) {
    a &= 0xffffff;
    if (a >= 0x200000 && a < 0x210000) ram[a - 0x200000] = v;
}

struct RxEv { uint64_t pos, now; int sci, val; };

int main(int argc, char **argv) {
    if (argc < 4) { fprintf(stderr, "usage: %s FIRMWARE N OUT [--rx F] [--lcd F] [--swp F]\n", argv[0]); return 2; }
    FILE *f = fopen(argv[1], "rb");
    if (!f) { perror(argv[1]); return 1; }
    size_t got = fread(rom.data(), 1, rom.size(), f);
    fclose(f);
    (void)got;
    const uint64_t n = strtoull(argv[2], nullptr, 0);
    FILE *out = fopen(argv[3], "w");
    std::deque<RxEv> rx;
    std::deque<uint64_t> lcd_clear;
    for (int i = 4; i + 1 < argc; i += 2) {
        FILE *g = fopen(argv[i + 1], "r");
        if (!g) { perror(argv[i + 1]); return 1; }
        char line[256];
        while (fgets(line, sizeof line, g)) {
            if (!strcmp(argv[i], "--rx")) {
                RxEv e; unsigned long long p, nw;
                if (sscanf(line, "%d %llu %llu %d", &e.sci, &p, &nw, &e.val) == 4) { e.pos = p; e.now = nw; rx.push_back(e); }
            } else if (!strcmp(argv[i], "--lcd")) {
                lcd.use_rule = false;
                unsigned long long p;
                if (sscanf(line, "%llu", &p) == 1) lcd_clear.push_back(p);
            } else if (!strcmp(argv[i], "--swp")) {
                unsigned v;
                if (sscanf(line, "%x", &v) == 1) swp_reads.push_back(v);
            }
        }
        fclose(g);
    }

    Verilated::commandArgs(argc, argv);
    auto *c = new Vh8h_sys;
    c->rst = 1; c->ext_ack = 0; c->pre_ack = 0; c->rx_req = 0; c->irq_pin = 0;
    const uint16_t adc[8] = {0x3ff, 0, 0x3ff, 0, 0, 0, 0x200, 0};
    for (int i = 0; i < 8; i++) {
        // adc_in is [7:0][9:0] packed: 80 bits
        for (int b = 0; b < 10; b++) {
            int bit = i * 10 + b;
            if (adc[i] >> b & 1) c->adc_in[bit / 32] |= 1u << (bit % 32);
        }
    }
    for (int i = 0; i < 4; i++) { c->clk = 0; c->eval(); c->clk = 1; c->eval(); }
    c->rst = 0;

    uint64_t count = 0;
    int rx_busy = 0;
    uint64_t last_progress = 0;
    for (uint64_t clk = 0; count < n; clk++) {
        if (clk - last_progress > 100000) {
            fprintf(stderr, "stalled at clock %llu, vtime %llu, %llu lines, pre_req %d ext_req %d\n",
                    (unsigned long long)clk, (unsigned long long)c->vtime, (unsigned long long)count,
                    c->pre_req, c->ext_req);
            break;
        }
        c->clk = 0;
        c->eval();
        c->irq_pin = clk > 0 ? 1 : 0;          // machine_reset: IRQ0 asserted (wired to ground)
        c->ext_ack = 0;
        c->pre_ack = 0;
        if (!rx_busy) c->rx_req = 0;
        if (c->ext_req) {
            uint32_t a = c->ext_addr;
            if (c->ext_we) {
                if (c->ext_word) { wr8(a, c->ext_wdata >> 8); wr8(a + 1, c->ext_wdata & 0xff); }
                else wr8(a, c->ext_wdata & 0xff);
            } else if (c->ext_word)
                c->ext_rdata = (rd8(a) << 8) | rd8(a + 1);
            else
                c->ext_rdata = rd8(a);
            c->ext_ack = 1;
        }
        if (c->pre_req && !rx_busy) {
            const uint64_t pos = c->pre_time;
            while (!lcd_clear.empty() && lcd_clear.front() <= pos) { lcd.busy = false; lcd_clear.pop_front(); }
            if (!rx.empty() && rx.front().pos <= pos) {
                c->rx_req = 1; c->rx_sci = rx.front().sci; c->rx_val = rx.front().val; c->rx_now = rx.front().now;
                rx.pop_front();
                rx_busy = 1;
            } else
                c->pre_ack = 1;
        }
        c->inj_pending = !rx.empty();
        if (!rx.empty()) { c->inj_pos = rx.front().pos; c->inj_time = rx.front().now; }
        c->pr_data = port_value(c->pr_port);
        c->eval();
        const bool tr = c->tr_stb;
        if (tr) {
            if (c->tr_state < 0x10000)
                fprintf(out, "%llu %06X %04X", (unsigned long long)c->vtime, c->tr_npc, c->tr_state);
            else
                fprintf(out, "%llu %06X %05X", (unsigned long long)c->vtime, c->tr_npc, c->tr_state);
            for (int r = 0; r < 8; r++) {
                uint32_t hi = (c->tr_r[(r + 8) / 2] >> (((r + 8) & 1) * 16)) & 0xffff;
                uint32_t lo = (c->tr_r[r / 2] >> ((r & 1) * 16)) & 0xffff;
                fprintf(out, " %04X%04X", hi, lo);
            }
            fprintf(out, " %02X", c->tr_ccr);
            if (c->tr_state == 0x10001) fprintf(out, " I %d %d", c->tr_tvec, (int8_t)c->tr_tlvl);
            fprintf(out, "\n");
            count++;
            last_progress = clk;
            lcd.step_start = c->vtime;

        }
        c->clk = 1;
        c->eval();
        if (c->rx_done) rx_busy = 0;
        if (c->pw_stb) { lcd.t_w = c->ext_time; port_write(c->pw_port, c->pw_data); }
        if (c->pr_stb) port_read_effects(c->pr_port);
        if (c->bl_stb) lcd.prev_end = c->bl_time;
        if (c->bl_stb && (c->bl_kind == OP_W8 || c->bl_kind == OP_W16))
            fprintf(out, "W%d %06X %0*X %llu\n", c->bl_kind == OP_W8 ? 1 : 2, c->bl_addr,
                    c->bl_kind == OP_W8 ? 2 : 4, c->bl_kind == OP_W8 ? c->bl_data & 0xff : c->bl_data,
                    (unsigned long long)c->bl_time);
        if (c->illegal) { fprintf(stderr, "illegal instruction\n"); break; }
    }
    fclose(out);
    delete c;
    return 0;
}
