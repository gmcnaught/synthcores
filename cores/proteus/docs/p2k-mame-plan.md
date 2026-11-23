# Proteus 2000: MAME prerequisites for a MiSTer core

As of 2026-11-23. The aim: everything a P2K MiSTer core is checked against comes from MAME, as
for the Proteus/1. There, the sound HDL matches a line-by-line port of the MAME model
(`tools/proteus_model.py`) sample for sample.

## Where it stands

| Part | State | Usable as a core reference? |
|---|---|---|
| CPU + MCF5206e peripherals | RTL matches MAME from reset to the OS main screen, 5,175,832 accesses (iteration 11) | Yes |
| Memory map, boot, flash, LCD, panel, knobs, MIDI In A/Out | MAME runs OS 2.26 to the main screen; findings [O] | Yes |
| SIMM format, stand-in SIMM | Builder byte-reproducible; 1,024 stand-in presets | Yes (stand-in data) |
| G20 voices | Address/loop/pitch, linear interpolation, level bytes; some register fields [U] | Not yet: float gains, unknown fields |
| H filters | E4-measured voice model in double precision | Not yet: floating point |
| G05 effects | Reverb/chorus workalike (86Box-style), float | Not yet: floating point, slots [U] |
| Output routing | Main/Sub 1/Sub 2 bytes observed; MAME mixes everything to one stereo pair | Not yet |
| Reference corpus | 13 cases, register logs + audio hashes, deterministic (PR #12) | Register logs yes; audio only once the models are integer |

A sound core can only be bit-exact with an integer reference. The MAME filter and effects models
are floating point, and some G20/H/G05 register fields are still unknown. These two things are
what remains before the HDL starts.

## Order of work

Each step has a gate decided by evidence: a hash, a test result or a documented finding. Steps 1
and 2 come first; 3 and 4 can run alongside them.

### 1. Close the register-contract unknowns (firmware + MAME experiments)

The core implements whatever the OS writes. These fields decide the HDL's interface, so each
needs an [O] finding or a recorded decision:

- **G20:**
    - note-off writes: +0x04 low 10 bits = 0x300, +0x10 = 0xA2;
    - +0x00 = 0x1800;
    - +0x3E modes 51/57/60;
    - +0x08 bits 31-26 (the per-pitch table at 0x0112069C, [I] an interpolation selector).
- **H:**
    - the level law: 0.375 dB a step against the pan table built for 0.156 dB (the findings
      say "hardware needed");
    - the interpolator approach-time byte +0x3C;
    - +0x7B routing values other than 0x72/0x7B/0x44.
- **G05:**
    - send encoding (offsets 0x14/0x16, 0x64/0x66, 0x74/0x76);
    - INIT1[0x0D], INIT3[0x13]/[0x15], INIT4[5]/[7];
    - how the 128 voices map onto the 5-bit channel index;
    - the byte write 1 → 0x60007C.
- **Method:** the listing first, then `tools/p2k_diff.py` runs. A SysEx parameter edit plus a
  diff of the register logs isolates one field at a time.
- **Gate:** every item above is [O], or [I] with the behaviour chosen and written into
  `p2k-findings.md`. Anything that can only be settled with a real unit goes to step 6.

### 2. Integer sound models in MAME (the bit-exact definition)

Replace the floating-point parts of `emup2k.cpp`/`emup2k_fx.h` with fixed-point arithmetic that
HDL can reproduce exactly:

- **G20:**
    - phase accumulator width;
    - interpolation (linear, or the Proteus/1 kernel approach if the +0x08 selector says so);
    - the sample word format;
    - output width.
- **Level:** an integer gain table for the chosen level law, and the ~5 ms smoothing as an
  integer ramp.
- **H filter:**
    - the E4 voice structure (f0²/D0 · N1 · N5 · P4 · P8 · g9) in fixed point;
    - frame interpolation by the cutoff byte;
    - per-section word widths and rounding;
    - 12th-order chaining.
- **G05:**
    - reverb and chorus in integer arithmetic;
    - delay-line lengths in samples at 44.1 kHz;
    - the 86Box-derived structure kept or replaced (licence: 86Box is GPL-2.0; the Proteus core
      is GPL-3.0, so check "or later" before porting any code, as opposed to the structure).
- **Mixer:** the buses, effect returns, the −6 dB headroom, and saturation.
- **Gate:**
    - the corpus is re-recorded with the integer models;
    - for each case the float-vs-integer difference is reported (SNR, peak error);
    - listening checks on the sweep cases show no audible regression.

### 3. Standalone model driven by register logs

A C++ (or Python, like `proteus_model.py`) port of the step 2 models. Its input is a
`sound.trace` register log plus the SIMM image. Its output is the audio. This is what the HDL
testbench compares against, and it removes MAME from the HDL test loop.

- **Gate:** for every corpus case, the model fed only the case's `sound.trace` and the SIMM
  reproduces MAME's `out.wav` hash exactly.
- **Prerequisite:** the log must carry every write the chips see, in order, with time. Check
  that `P2K_TRACE` misses nothing (G05 writes via the index port, H reads, the Baby latch) and
  add a sample-accurate time stamp if the integer model needs one.

### 4. Corpus extension

Add cases for what the current 13 do not cover:

- **Filters:** each of the 51 filter types, with cutoff and Q sweeps through SysEx edits,
  including the 12th-order Z-plane types.
- **Effects:** FX A algorithms (rooms, halls, delays), FX B algorithms (chorus, flange, delay),
  send amounts, the B→A route.
- **Routing:** layer Mix Output Main/Sub 1/Sub 2 and the six outputs.
- **CPU-side modulation:** LFOs, envelopes, cords, the arpeggiator, multi mode with 16 channels
  and SysEx edits, a 10-minute soak.
- **Memory bandwidth figures for the core's memory port:** sample-memory reads per output
  sample (voices × interpolation taps), and the peak over each case.
- **Gate:**
    - new cases deterministic (`--repeat 2`);
    - `--update` recorded;
    - every case's register log exercises the feature it names (checked by its stats).

### 5. Variant and storage features (only if the core needs them)

- Sound-memory flash writes (G20 port B) for user flash SIMMs.
- MIDI In B (UART2): 32 channels.
- Proteus 1000 (one G chip, two H chips, two SIMM slots).
- A real SIMM image instead of the stand-in, if a dump becomes available. The format checks
  (header, block checksums, ROM ID) are already in the findings.

### 6. Hardware validation (needs a real unit or recordings)

The level law, the H interpolator timing, and the G05 effects' sound cannot be decided from
firmware. With a unit or recordings, these settle them:

- the diagnostics' Freq Response Test (50 Hz / 440 Hz / 19 kHz);
- level steps;
- filter sweeps;
- impulse responses of each FX algorithm.

Without them, the step 2 integer models are the definition and are labelled as such.

## Decisions needed from the user

1. **Hardware:** is a Proteus 2000/1000 unit, or recordings from one, available? This decides
   whether step 6 runs or the MAME models stay the definition.
2. **Reference strategy for H and G05:** accept "the integer MAME model is the definition" for
   the filter and effects (exact HDL lockstep, sound verified only by ear)?
3. **Outputs on the MiSTer:** mix Main + Sub 1 + Sub 2 down to the one stereo pair, or select
   one from the OSD?
4. **Scope of step 5:** which of user flash SIMMs, MIDI In B and the Proteus 1000 the first core
   needs.
