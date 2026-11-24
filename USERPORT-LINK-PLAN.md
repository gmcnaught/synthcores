# Synth MiSTer as an mt32-pi replacement (user-port link)

**Status:** parked on 2026-11-24 as a plan for later. Phase 0 needs no code. Phase 1 has not
started; the only code so far is the I2S loopback test in `modules/synthframe/tests/mt32pi_link/`.

Goal: a second MiSTer running one of these synth cores takes the place of an mt32-pi hat on a
game MiSTer. The game MiSTer runs unchanged: its core's MIDI option stays on "MT32-pi". All
changes are on the synth side.

## What the game MiSTer already does (observed in `sys/mt32pi.sv`)

The user port is not USB. It is 7 FPGA GPIOs (`USER_IO[6:0]`) on a USB 3.0 Type-A socket.
The game core instantiates `sys/mt32pi.sv`, which uses it like this:

| USER_IO | USB3-A pin | Game MiSTer | mt32-pi (and the synth MiSTer) |
|---|---|---|---|
| 0 | 3 D+ | I2C SDA, slave (open-drain) | I2C SDA, master |
| 1 | 2 D- | **MIDI out**, 31250 baud, always driven | MIDI in |
| 2 | 8 SSTX- | input | **I2S WS** |
| 3 | 7 GND_DRAIN | input | **I2C SCL** |
| 4 | 6 SSRX+ | input | **I2S BCLK** |
| 5 | 5 SSRX- | input | **I2S DATA** |
| 6 | 9 SSTX+ | input | not driven |
| — | 4 GND | GND | GND |

- **Audio comes back over the same cable.** The game side has an I2S receiver
  (`mt32_i2s_l/r`, 16-bit) and the core mixes it into its own output. One HDMI output carries
  the game and the synth, so no external mixer is needed.
- **Control is I2C, with the mt32-pi as master.** The game side is a slave at two addresses:
    - `0x45` (status and control).
        - Write 3 bytes: current mode, ROM and SoundFont. The game shows them in its OSD.
        - Read 3 bytes: the game's requests. Byte 1 is the mode: `A0` reset, `A1` MT-32,
          `A2` SoundFont. Byte 2 is the ROM set (0–3), byte 3 the SoundFont index.
    - `0x3C` (SSD1306-style display): control byte `0x40`, then 512 bytes (128×32) or
      1024 bytes (128×64). The game overlays that bitmap on its video.
    - The first valid I2C transaction sets `mt32_available`.
- **Either USB3 cable wiring works.** The game side times the clock on pins 4 and 6 and swaps
  its pin assignment to match. That covers a straight cable and a USB 3.0 A-to-A cable with
  the SuperSpeed TX/RX pairs crossed, if the synth drives 2=WS, 4=BCLK and 5=DATA and leaves
  pin 6 alone.
- **Stock cores cannot fight over the pins.** `sys_top.v` drives every `USER_IO` pin only
  low or Z, and `sys.tcl` sets a weak pull-up on each.

## Verified in simulation

`modules/synthframe/tests/mt32pi_link/run.sh` connects MiSTer's own `sys/i2s.v` (the
transmitter in `audio_out.sv`) to `sys/mt32pi.sv`. The two clocks are 200 ppm apart.
Simulated time is 40 ms.

| Sample rate | Cable | Samples correct | Bad |
|---|---|---|---|
| 48 kHz | straight | 3801 | 0 |
| 48 kHz | crossed | 3801 | 0 |
| 96 kHz | straight | 7574 | 29 |
| 96 kHz | crossed | 7574 | 29 |

So the synth side can reuse `i2s.v` unchanged, but **the link must run at a fixed 48 kHz**.
That holds even when the core's own output is set to 96 kHz (`audio_96k`).

## Design (synth side)

### Phase 0: works today, no HDL changes

Connect the game MiSTer's MidiLink USB MIDI out, through a USB-MIDI interface, a DIN cable and
another USB-MIDI interface, to the synth MiSTer's MidiLink. The synth cores already take
MidiLink input. Mix the audio externally. Use this as a baseline for latency and correctness.

### Phase 1: MIDI in and audio return over the user port

Add a new MIT module, `modules/synthframe/hdl/midi/synthframe_userport_link.sv`. Write it from
the protocol above; don't copy `mt32pi.sv`, which is GPL. It has three parts:

1. **MIDI in.** Read `USER_IN[1]` into the existing `synthframe_midi_rx`. An OSD option picks
   the input: "MIDI in: MidiLink / User port". Merging both streams would need a
   message-level merger, so it is deferred.
2. **I2S out.** A second `i2s` instance with its own 48 kHz clock enable, fed from the core's
   final `AUDIO_L/R`, drives `USER_OUT[2]`=WS, `[4]`=BCLK and `[5]`=DATA. It does not depend
   on `audio_96k`.
3. **Pin drive: only if the hardware test needs it.** Build phase 1 without touching
   `sys_top.v` first, then scope BCLK.
    - Observed: `sys_top.v:1670-1676` drives each pin open-drain. A 0 goes to ground through
      a strong transistor. A 1 releases the pin to a ~25 kΩ weak pull-up (`sys.tcl:39`).
    - Inferred:
        - With an estimated ~50 pF of cable and pins, a rising edge takes about 1.25 µs.
        - BCLK at 1.536 MHz is high for only ~325 ns, so it may never reach a clean high.
        - MIDI (32 µs per bit) and I2C (100–400 kHz) are slow enough for open-drain.
    - The mt32-pi avoids the problem because the Pi's GPIO outputs are push-pull.
    - A core can't get push-pull itself: its `USER_OUT` passes through the open-drain logic in
      `sys_top.v`.
    - The fix, if needed:
        - Change `sys_top.v` (shared through the `mister/sys` symlink), behind a macro in
          `cfg/macros.def`, to drive pins 2, 3, 4 and 5 push-pull while the link is enabled.
          That is one patch to reapply on every upstream `sys/` sync.
        - Alternative with no code change: stronger external pull-ups in the cable.
    - Observed: `mt32pi.sv` sets `USER_OUT[6:2]='1` (Z), so the game side never drives these
      pins. Push-pull is safe against an mt32pi game core.
    - Pin 0 (SDA) stays open-drain, because both sides drive it. Pin 6 stays Z.

Phase 1 needs no Main_MiSTer, MidiLink or game-core changes. The work is the synthframe
module, plus a `USER_OUT`/`USER_IN` hookup and an OSD option in each core's MiSTer top
(`SC55.sv` and others, which set `USER_OUT = '1` today). The possible exception is the
`sys_top.v` change above.

**MIDI-only variant:** read `USER_IN[1]` and add the OSD option, with no I2S output. Synth
audio then goes from the synth MiSTer's own output to an external mixer. Pins 1–4 are enough
for this.

Phase 1 alone gives MIDI in plus mixed audio on the game MiSTer's HDMI. Observed:
`mt32pi.sv` drives MIDI on pin 1 whether or not I2C is present. Unknown: whether a given game
core also gates MIDI on `mt32_available`. ao486's `emu.sv` needs checking.

### Phase 2: I2C control and LCD overlay

Add an I2C master to the same module. The game samples SDA/SCL at CLK_AUDIO/8 with a
2-sample filter, so 400 kHz is likely fine; check it in simulation against `mt32pi.sv` before
relying on it.

- **Status and requests.** About every 100 ms, write mode, ROM and SF to `0x45`, then read the
  request bytes back. Map the requests to what the loaded core can do:
    - `A0`: reset the synth, or send a GM/GS reset.
    - ROM and SF indexes: variants inside one core, such as MT-32 / CM-32L on the MT-32 core.
- **Display.** Push the synth's panel to `0x3C` as a 128×32 1bpp page-ordered bitmap:
    - the SC-55 LCD bitmap;
    - HD44780 text from JV-880 and CZ-101, rendered with the shared charlcd font.
  At 400 kHz a 514-byte frame takes about 12 ms.

### Phase 3 (open question): switching cores from the game OSD

"MT-32" vs "SoundFont" from the game OSD could map to loading the MT-32 core vs the SC-55
core. Unknown: Main_MiSTer has no known path for a core to ask the HPS to load another RBF.
That needs a check before any design.

## Cable

Both user ports are sockets, so the cable needs a male Type-A plug at each end. Pin 1 (VBUS)
must not connect:
- The mt32-pi hat takes 5 V from this pin, often not enough
  ([forum](https://www.misterfpga.org/viewtopic.php?p=45658)). That works because the Pi is
  only a load.
- Two MiSTers would both supply 5 V into the same line.

**Build:**
1. Buy a USB 3.0 A-to-A cable. Before cutting, check with a meter that all 9 pins connect
   end to end. Spec-compliant A-to-A cables may leave out D+/D−, which carry MIDI and SDA.
2. Cut it. Keep one plug end, which goes into the game MiSTer.
3. Screw the loose wires into a male USB 3.0 Type-A breakout board, which goes into the synth
   MiSTer. Wire pin N to pin N for pins 2–9. Identify wires by meter; the usual colours are in
   the table.
4. Leave pin 1 unconnected and insulate it. Insulate the drain wire (pin 7, SCL) from the
   braid and from the shell. Pin 7 to the shell must read open, or I2C fails.
5. Keep the cable short, 30 cm or less, because of the 1.5 MHz BCLK.
6. Check at the breakout:
    - pin 1 to every other pin: open;
    - pins 2–8 to the same pin on the plug: closed;
    - adjacent pins: open.

| Pin | Signal | Usual colour | Phase 1 | Phase 2 |
|---|---|---|---|---|
| 1 | VBUS | red | **not connected** | **not connected** |
| 2 | D− / MIDI | white | yes | yes |
| 3 | D+ / SDA | green | — | yes |
| 4 | GND | black | yes | yes |
| 5 | SSRX− / I2S DATA | blue | yes | yes |
| 6 | SSRX+ / I2S BCLK | yellow | yes | yes |
| 7 | GND_DRAIN / SCL | bare drain | — | yes, isolated from the shield |
| 8 | SSTX− / I2S WS | purple | yes | yes |
| 9 | SSTX+ | orange | optional | optional |

**Series resistors:** fit 220–330 Ω inline on pins 5–8 only if the push-pull `sys_top.v`
change goes in. They limit current if the game core is left in another user-port mode (SNAC,
serial) and drives those pins low while the synth drives them high.

**USB 2.0 power blockers** (such as the PortaPow, sold as a USB 2.0 part) pass only pins 2–4.
That allows the MIDI-only variant, but no I2S and no SCL.

## Test plan

1. **Sim (done):** I2S loopback in `tests/mt32pi_link/run.sh`. Add it to `tests.yml`.
2. **Sim:** the new module's I2C master against `mt32pi.sv`. Check `mt32_available`,
   `mt32_mode/rom/sf`, a 512-byte `lcd_data` frame, and that request bytes round-trip.
3. **Sim:** game `midi_tx` → pin 1 → `synthframe_midi_rx`, comparing the byte stream.
4. **Hardware:**
    - Game MiSTer: ao486 running a General MIDI DOS game, MIDI set to MT32-pi.
    - Synth MiSTer: SC-55 first, since it matches mt32-pi's SoundFont use.
    - Check MIDI, audio on the game HDMI (the two crystals drift, so the zero-order hold will
      sometimes repeat or drop a sample, as with a real mt32-pi), the OSD status, and the
      LCD overlay.
    - Scope BCLK on pin 4 with open-drain vs push-pull drive to confirm the phase 1 pin-drive
      decision.
