// SPDX-License-Identifier: GPL-2.0-or-later
// LCD renderer bench: for every line of an LCD state log (ref/mt32_ref --lcd-state-log or
// sim/system --lcd-state-log) load the state into hdl/mt32_lcd_video.sv, render a frame and
// print the dots in ref/mt32_ref --lcd-dots format, so tests/test_lcd.py can compare them with
// Nuked-MT32's lcd_render().
//
//   lcd_bench STATE_LOG > DOTS
//
// Dot (cell i, row r, column j) is sampled at the centre of its 6 x 6 pixel square:
// x = 40 + 36i + 6j + 3, y = 70 + 6r + 3 (mt32_lcd_video X0, Y0, CW). Lit 10300c, unlit 6cb030,
// display off 102008; anything else is an error.
#include "Vtb_lcd.h"
#include "verilated.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

int main(int argc, char** argv)
{
    if (argc != 2) { fprintf(stderr, "usage: lcd_bench STATE_LOG\n"); return 2; }
    FILE* f = fopen(argv[1], "r");
    if (!f) { perror(argv[1]); return 2; }
    auto* t = new Vtb_lcd;
    auto tick = [&] { t->clk = 0; t->eval(); t->clk = 1; t->eval(); };
    const int W = 800, H = 600;
    std::vector<uint32_t> fb(W * H);
    char line[512];
    int n = 0;
    while (fgets(line, sizeof line, f)) {
        char cyc[32], txt[64], cg[96];
        unsigned st;
        if (sscanf(line, "S %31s %63s %95s %x", cyc, txt, cg, &st) != 4 || strlen(txt) != 40 || strlen(cg) != 64) {
            fprintf(stderr, "lcd_bench: bad line: %s", line);
            return 1;
        }
        auto hex2 = [](const char* p) { char b[3] = { p[0], p[1], 0 }; return (uint8_t)strtoul(b, nullptr, 16); };
        t->wr = 1;
        for (int i = 0; i < 20; i++) { t->wa = i; t->wd = hex2(txt + 2 * i); tick(); }
        for (int i = 0; i < 32; i++) { t->wa = 32 + i; t->wd = hex2(cg + 2 * i); tick(); }
        t->wr = 0;
        t->cursor = st & 31; t->cursor_on = st >> 5 & 1; t->cursor_blink = st >> 6 & 1;
        t->cursor_font = st >> 7 & 1; t->lines2 = st >> 8 & 1; t->display_on = st >> 9 & 1;
        // wait for the start of a frame (vblank falls), then take one frame of visible pixels
        while (!t->vblank) tick();
        while (t->vblank) tick();
        int x = 0, y = 0;
        bool pv = false;
        while (y < H) {
            bool vis = !t->hblank && !t->vblank;
            if (vis) { if (x < W) fb[y * W + x] = (uint32_t)t->r << 16 | t->g << 8 | t->b; x++; }
            if (!vis && pv) { y++; x = 0; }
            pv = vis;
            tick();
        }
        int lit = 0, unlit = 0, off = 0;
        std::vector<uint8_t> rows(160);
        for (int i = 0; i < 20; i++)
            for (int r = 0; r < 8; r++)
                for (int j = 0; j < 5; j++) {
                    uint32_t p = fb[(70 + 6 * r + 3) * W + 40 + 36 * i + 6 * j + 3];
                    if (p == 0x10300c) { lit++; rows[i * 8 + r] |= 1 << (4 - j); }
                    else if (p == 0x6cb030) unlit++;
                    else if (p == 0x102008) off++;
                    else { fprintf(stderr, "lcd_bench: unexpected colour %06x\n", p); return 1; }
                }
        printf("%s", cyc);
        if (off == 800) printf(" off\n");
        else if (off) { fprintf(stderr, "lcd_bench: %d of 800 dots in the display-off colour\n", off); return 1; }
        else {
            for (int i = 0; i < 20; i++) {
                putchar(' ');
                for (int r = 0; r < 8; r++) printf("%02x", rows[i * 8 + r]);
            }
            putchar('\n');
        }
        n++;
    }
    fclose(f);
    fprintf(stderr, "lcd_bench: %d states\n", n);
    delete t;
    return n ? 0 : 1;
}
