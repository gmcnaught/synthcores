// SPDX-License-Identifier: GPL-2.0-or-later
// Log hooks added to Nuked-MT32 by prepare_src.py. All are null unless the harness sets them.
#pragma once
#include <stdint.h>

// CPU (cycle = the CPU's total_cycles() at the call).
extern void (*mt32h_fetch)(uint16_t pc, uint16_t psw, uint64_t cycle);         // before each instruction
extern void (*mt32h_irq)(int level, uint16_t pc, uint64_t cycle);              // interrupt taken (pc = return address)
extern void (*mt32h_regw)(uint8_t addr, uint8_t data, uint64_t cycle);         // register file / SFR write
extern void (*mt32h_input)(int line, int state, uint64_t cycle);               // external input line change
// Board.
extern void (*mt32h_busw)(uint16_t addr, uint8_t data, uint64_t cycle);        // external bus write
extern void (*mt32h_midi)(uint8_t byte, uint64_t cycle);                       // byte written to SBUF
extern void (*mt32h_tx)(uint8_t byte, uint64_t cycle);                         // byte the CPU sends (sbuf_w)
extern void (*mt32h_lcd)(const uint8_t* text20, uint64_t cycle);               // after an LCD command
// LA32 and reverb (cycle = LA32 cycle, 256 a sample).
extern void (*mt32h_la32w)(uint16_t addr, uint8_t data, uint64_t la32_cycle);
extern void (*mt32h_rvw)(int reg, uint32_t value, uint64_t la32_cycle);        // 0 saw0, 1 saw1, 2 ROM bank
extern void (*mt32h_rvin)(int which, int16_t value, uint64_t la32_cycle);
extern void (*mt32h_out)(const int16_t* acc4, const int16_t* reverb2, int32_t vol,
                         int16_t l, int16_t r, uint64_t la32_cycle);
extern void (*mt32h_la32irq)(int set, uint8_t status, uint64_t la32_cycle);     // set at slot start, clear at write
// Debug, per slot: kind 0 after the wave generator (i, oo, prev, w46, ram2[0..2][i]),
// kind 1 at the slot end (i, accum[cur][2], [3], [6], [7], w186<<16|w187, w188).
extern void (*mt32h_slot)(int kind, uint32_t i, int64_t a, int64_t b, int64_t c, int64_t d, int64_t e,
                          int64_t f, uint64_t la32_cycle);
