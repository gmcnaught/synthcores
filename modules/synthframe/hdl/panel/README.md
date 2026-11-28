# synthframe panel modules

A core keeps its front panel as data: the LCD controller's memory, LED bits and key events. The
renderer that turns that data into pixels belongs to the platform (today the MiSTer's 800 x 600
or 320 x 240 output). The core's LCD controller must match the core's reference model, so it
stays with the core when the references differ. The SC-55 and JV-880 use
`modules/roland/hdl/board/lcd_ctrl.sv`, which is Nuked-SC55's `LCD_Write`. The Proteus/1 and
CZ-101 use `synthframe_hd44780`, which is MAME's `hd44780_base_device`.

| Module | What it does |
|---|---|
| `synthframe_hd44780.sv` | HD44780 controller after MAME: DDRAM 128 x 8, CGRAM 64 x 8, address counter correction, entry mode, display/cursor shift, busy timing, and a combinational render port |
| `synthframe_charlcd.sv` | Character LCD renderer, the dot layer. Draws LINES x COLS characters of 5 x 8 dots from a character LCD read port; geometry and pitch are parameters |
| `synthframe_vtiming.sv` | Video timing counters, 800 x 600 at 60 Hz from 40 MHz by default |
| `synthframe_panel_art.sv` | Front-panel art from block RAM: an 8 x 8 cell map (tile, element) and a tile ROM of 4 or 5 bits a pixel, made by `modules/synthframe/tools/panel_art.py` from `cores/<core>/cfg/panel.yaml`. LEDs light and buttons show pressed by swapping the palette of their cells (`state[element]`). Three clocks (enabled clocks) of latency, as the renderers' |
| `synthframe_status_strip.sv` | The status strip the MiSTer smoke tests read: state band, MIDI, underrun and lag boxes |

Which cores use a module: `git grep -l <module> -- 'cores/*/cfg/files.yaml'`.

`synthframe_hd44780` measures the busy flag in clock cycles (`BUSY_EXT = 0`; the Proteus/1 at
80 MHz) or against an external time base (`BUSY_EXT = 1`; the CZ-101's virtual CPU cycles, so
the firmware's busy polls match MAME's instruction by instruction). Reset and clear display
fill DDRAM one address a clock (128 clocks); reset also zeroes CGRAM, as MAME's device_reset.
In one-line mode MAME draws only line 0; the render port returns line 0's characters for line
1 too.

## Front panel art

`cores/<core>/cfg/panel.yaml` describes the panel at the output resolution: the case, labels,
buttons (with the MiSTer key that presses each one), LEDs, knobs, the LCD rectangle and the status
strip. A `type: lay` entry pulls in a MAME layout view (the CZ-101 uses MAME's CC0 `cz101.lay`,
the MU50 `mu50.lay`, whose SVG case art panel_art.py rasterises with `rsvg-convert`). Buttons
get their expression from `key_expr` (`kc<n>` tags) or `input_map` (regex on the tag; `{bit}` is
the mask's bit), LEDs from `led_map`.
Buttons with `input:` and LEDs with `bit:` hold an HDL expression in the renderer's scope; they
are the dynamic elements. `panel_art.py PANEL.yaml --hdl DIR --name NAME` writes
`NAME_panel_map.hex`, `NAME_panel_tiles.hex`, `NAME_panel.svh` (geometry and palettes) and
`NAME_panel_state.svh` (`assign panel_state[i] = <expression>;`); `--png` draws a mock-up.
The renderer includes both `.svh` files, instantiates `synthframe_panel_art` with the pixel
counters, and uses its colour wherever the LCD and the status strip do not draw. Fonts:
`tools/fonts/` (Inter, SIL OFL 1.1). The palette keeps off the status strip's band colours,
which the smoke tests search for.

## Character LCD read port

The renderer asks for a character, then for one dot row of that character:

| Signal | Direction (renderer's view) | Meaning |
|---|---|
| `cell_line`, `cell_col` | out, registered | the display position being drawn |
| `cell_row` | out, with `cell_line` | the dot row being drawn |
| `cell_code` | in, sampled one clock later | the character code at that position (combinational in the core) |
| `cell_cursor` | in, with `cell_code` | the cursor is shown at that position |
| `glyph_code`, `glyph_row` | out, registered | the character and dot row (0-7) to look up |
| `glyph_bits` | in, sampled one clock later | 5 dots, bit 4 the leftmost: CG RAM for codes below 16 (8 on the HD44780), the font otherwise |

The core supplies both lookups. The font is the core's: Nuked-SC55's `lcd_font.svh` for the
Roland cores, and the ROM pack's `hd44780_a00` for the Proteus/1 and CZ-101. A font in a
synchronous RAM (the CZ-101's) is read with (`cell_code`, `cell_row`) and registered on the
clock that registers `glyph_code`, so `glyph_bits` is ready one clock later as for the others. Two renderer instances that never
draw the same pixel can share one glyph lookup through their `fetch` outputs. The JV-880 does this
for its debug line.

`synthframe_hd44780`'s render port (`r_line`, `r_pos`, `r_row` → `r_code`, `r_cgbits`,
`r_is_cg`, `r_at_ac`, `r_cursor`) is the same lookup with the CG RAM row resolved inside the
controller. `r_at_ac` marks the address counter's cell, for the cursor underline
(`cursor_on`) and blink (`blink_on`; the blink clock is the renderer's).

## Tests

`modules/synthframe/ver/panel`: frame benches for the SC-55, JV-880, Proteus/1 and CZ-101
renderers. They use seeded contents, 4 seeds each, and two frames hashed at every pixel clock.
The Proteus/1 and CZ-101 benches include the controller (the CZ-101's through its pins).
`expected.txt` was recorded from the renderers before they used these modules (`run.sh` says
when), so `test_panel.py` checks that the move left every pixel and sync unchanged. Optional
`PPM=prefix` writes the frames as images.
