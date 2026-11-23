# Proteus 2000: MAME prerequisites for a MiSTer core

As of 2026-11-23. The aim: everything a P2K MiSTer core is checked against comes from MAME, as
for the Proteus/1. There, the sound HDL matches a line-by-line port of the MAME model
(`tools/proteus_model.py`) sample for sample.

## Where it stands (updated 2026-11-23, steps 1-5 done)

| Part | State | Usable as a core reference? |
|---|---|---|
| CPU + MCF5206e peripherals | RTL matches MAME from reset to the OS main screen, 5,175,832 accesses (iteration 11) | Yes |
| Memory map, boot, flash, LCD, panel, knobs, MIDI In A/Out | MAME runs OS 2.26 to the main screen; findings [O] | Yes |
| SIMM format, stand-in SIMM | Builder byte-reproducible; 1,024 stand-in presets; E-mu header checksum pairs | Yes (stand-in data) |
| Register contract (G20, H, G05) | Every field [O] or a recorded [I] choice (`p2k-findings.md`, "Register contract") | Yes |
| Sound model | `p2k/model/p2k_sound.h`: integer G20 voices, H filter and interpolators, G05 effects, mixer; used by MAME (patch 0004) | Yes: the definition |
| Standalone model | `p2k/model/p2k_render`: replays `snd.log`, reproduces MAME's output bit for bit on all 22 cases | Yes |
| Reference corpus | 22 cases (filters, effects, routing, modulation, 10-minute soak), deterministic, feature-checked | Yes |
| User flash SIMMs | `p2k/model/p2k_flash.h`; the OS's rename writes a flash SIMM, it persists and plays (`tools/p2k_flash_test.py`) | Yes |
| Hardware validation | Deferred: no unit (decision 1) | No |

A sound core is checked against `p2k_render` fed the corpus logs: same `snd.raw`, bit for bit.

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

- **Done 2026-11-23.** `p2k-findings.md` "Register contract" lists every field with its firmware
  evidence and the behaviour the model implements. Settled from the listing and the corpus logs
  (no MAME experiment needed beyond the corpus): G20 +0x00 bit 12 is a live end select; +0x08
  `tone` comes from the table at 0x0112079C (linear interpolation kept); +0x04/+0x10 at voice free;
  H interpolators (+0x30-0x3E, +0x71/+0x73, +0x7C, approach time +0x44/+0x46); the +0x7B bit fields
  and output channels; the level law (H code decode, from E-mu's own level code); G05 send / dry
  bytes A, B, D; the G05 channel map. Left for hardware (step 6): the pan table's 0.156 dB reading,
  G20 +0x10's low half, the meaning of the approach-time index, the G05 microcode.

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

- **Done 2026-11-23.** `p2k/model/p2k_sound.h` (number formats in its header). Integer vs the
  floating-point reference (`p2k_sound_float.h`, same structure, double precision) over the corpus:
  SNR 65-85 dB on all 22 cases (2026-11-23 record); peak error at most 2 LSB except the filter
  cases (7-52 LSB, resonant types at Q 127) and `fxb` / `route` (14 / 4 LSB, high-feedback chorus);
  no state saturation. The float reference rounds the
  interpolated code to 1/256 and uses the integer LFO increment, as the integer model does; without
  that, the differences measured are those definitions, not arithmetic error. Choices made on the
  way, all [I]: H filter sections in direct form II (pole + zero per section, the patent datapath),
  a 16-bit clip before the gain word, level law = H code decode, G05 sends and dry levels linear.
- Listening check: not done by the agent (no ears). WAVs of every case: `p2k/model/p2k_render
  --simm 1=big.bin build/p2k/corpus/<case>.0/snd.log out.wav`.

### 3. Standalone model driven by register logs

A C++ (or Python, like `proteus_model.py`) port of the step 2 models. Its input is a
`sound.trace` register log plus the SIMM image. Its output is the audio. This is what the HDL
testbench compares against, and it removes MAME from the HDL test loop.

- **Gate:** for every corpus case, the model fed only the case's `sound.trace` and the SIMM
  reproduces MAME's `out.wav` hash exactly.
- **Prerequisite:** the log must carry every write the chips see, in order, with time. Check
  that `P2K_TRACE` misses nothing (G05 writes via the index port, H reads, the Baby latch) and
  add a sample-accurate time stamp if the integer model needs one.

- **Done 2026-11-23.** `p2k_corpus.py model`: for all 22 cases `p2k_render` fed only `snd.log` and
  the SIMM gives `snd.raw` exactly; it also checks that the headers in patch 0004 match
  `p2k/model`. The log (`P2K_SNDLOG`) records every G20, H and G05 write with its sample index;
  writes are applied after the stream is brought up to date. Reads do not change state.

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

- **Done 2026-11-23.** Cases `filt0`-`filt2`, `fxa0`, `fxa1`, `fxb`, `route`, `mod`, `soak`
  (`tests/p2k_corpus/README.md`); each passes its feature check; two renders identical.
  Bandwidth for the core's memory port (linear interpolation, 2 words per active voice per
  sample): peak 256 words a sample (11.29 M words/s) in `poly`, 240-248 in `multi`, `sweep*` and
  `soak`; 128 in the single-channel cases. Not exercised: the arpeggiator (did not start over
  SysEx, [U]); it runs on the CPU.

### 5. User flash SIMMs

- Sound-memory flash writes (G20 port B): the command sequence the OS sends, the erase and
  program timing it polls for, and the flash SIMM ID it expects.
- **Gate:** in MAME, the OS loads a user sample set into a flash SIMM image, the image survives a
  restart, and a corpus case plays from it deterministically.
- Out of scope for the first core (decision 4): MIDI In B (UART2), the Proteus 1000, a real SIMM
  image.

- **Done 2026-11-23** (gate restated: the OS has no sample import, so the test uses its own write
  path). `tools/p2k_flash_test.py`: a stand-in built with ROM ID 106 in a flash slot; the scripted
  panel (`P2K_KEYS`) runs RENAME FLASH SIMM to ID 108; the written images carry ID 108 with valid
  checksums; after a restart bank 108 plays, deterministically, and `p2k_render` reproduces it.

### 6. Hardware validation (deferred: no unit available)

The level law, the H interpolator timing, and the G05 effects' sound cannot be decided from
firmware. With a unit or recordings, these settle them:

- the diagnostics' Freq Response Test (50 Hz / 440 Hz / 19 kHz);
- level steps;
- filter sweeps;
- impulse responses of each FX algorithm.

Without them, the step 2 integer models are the definition and are labelled as such.

## Decisions (user, 2026-11-23)

1. **Hardware:** no Proteus 2000/1000 is available. Step 6 is deferred. Step 1 items that need
   a real unit (the level law, the +0x3C timing) get a recorded choice instead of an [O] finding.
2. **Reference strategy:** the step 2 integer MAME models are the definition for G20, H, G05 and
   the mixer, for now. The HDL is checked against them bit-exactly; their sound is checked by ear.
3. **Outputs on the MiSTer:** Main + Sub 1 + Sub 2 are mixed down to the one stereo pair. The
   integer mixer in step 2 defines that sum (gains, headroom, saturation). The routing cases in
   step 4 still run, to check each bus reaches the sum.
4. **Scope of step 5:** user flash SIMMs (sound-memory flash writes through G20 port B) only.
   MIDI In B, the Proteus 1000 and a real SIMM image are out of scope for the first core.
