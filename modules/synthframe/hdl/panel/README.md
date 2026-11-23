# synthframe panel modules

A core keeps its front panel as data: the LCD controller's memory, LED bits and key events. The
renderer that turns that data into pixels belongs to the platform (today the MiSTer's 800 x 600
or 320 x 240 output). The core's LCD controller must match the core's reference model, so it
stays with the core when the references differ. The SC-55 and JV-880 use
`modules/roland/hdl/board/lcd_ctrl.sv`, which is Nuked-SC55's `LCD_Write`. The Proteus/1 uses
`synthframe_hd44780`, which is MAME's `hd44780_base_device`.

| Module | What it does | Used by |
|---|---|---|
| `synthframe_hd44780.sv` | HD44780 controller after MAME: DDRAM 128 x 8, CGRAM 64 x 8, address counter correction, entry mode, display/cursor shift, busy timing, and a combinational render port | Proteus/1 |
| `synthframe_charlcd.sv` | Character LCD renderer, the dot layer. Draws LINES x COLS characters of 5 x 8 dots from a character LCD read port; geometry and pitch are parameters | JV-880 (the LCD and the debug line) |
| `synthframe_vtiming.sv` | Video timing counters, 800 x 600 at 60 Hz from 40 MHz by default | SC-55, JV-880 |
| `synthframe_status_strip.sv` | The status strip the MiSTer smoke tests read: state band, MIDI, underrun and lag boxes | SC-55, JV-880 |

## Character LCD read port

The renderer asks for a character, then for one dot row of that character:

| Signal | Direction (renderer's view) | Meaning |
|---|---|---|
| `cell_line`, `cell_col` | out, registered | the display position being drawn |
| `cell_code` | in, sampled one clock later | the character code at that position (combinational in the core) |
| `cell_cursor` | in, with `cell_code` | the cursor is shown at that position |
| `glyph_code`, `glyph_row` | out, registered | the character and dot row (0-7) to look up |
| `glyph_bits` | in, sampled one clock later | 5 dots, bit 4 the leftmost: CG RAM for codes below 16 (8 on the HD44780), the font otherwise |

The core supplies both lookups. The font is the core's: Nuked-SC55's `lcd_font.svh` for the
Roland cores, and the ROM pack's `hd44780_a00` for the Proteus/1. Two renderer instances that never
draw the same pixel can share one glyph lookup through their `fetch` outputs. The JV-880 does this
for its debug line.

`synthframe_hd44780`'s render port (`r_line`, `r_pos`, `r_row` → `r_code`, `r_cgbits`,
`r_is_cg`, `r_cursor`) is the same lookup with the CG RAM row resolved inside the controller.

## Tests

`modules/synthframe/ver/panel`: frame benches for the SC-55, JV-880 and Proteus/1 renderers.
They use seeded contents, 4 seeds each, and two frames hashed at every pixel clock.
`expected.txt` was recorded from the renderers before they used these modules, so
`test_panel.py` checks that the move left every pixel and sync unchanged. Optional
`PPM=prefix` writes the frames as images.
