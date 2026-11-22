# E-mu patents and the Proteus/1 sound chip

None of these patents names the Proteus. They are E-mu filings from the same period (1989–1992)
whose described hardware matches what the firmware programs. Checked 2026-11-20; full texts were
read from Google Patents.

## Used in the core

**US 5,111,727** — Rossum, "Digital sampling instrument for digital audio data", filed 1990-01-05.
- 7- or 8-point interpolation: the output is a sum of surrounding sample words times coefficients
  that depend on the fractional address. With 7 points the window is centred on the nearest word
  ("conceptually adding 1/2 to the effective waveform memory address").
- Coefficients come from one impulse response stored single-sided (112 points for 7-point, 32 per
  sample period), mirrored, and linearly interpolated by the low fraction bits.
- 16 cycles per output point; with 7 points, 2 of them are left for volume scaling and mixing.
  32 voices × 16 cycles × 39,062.5 Hz = 20 MHz, the clock the Proteus's LSI #2 provides. That
  arithmetic, and the chip doing volume and mixing itself, is why the core uses 7 points [I].
- Design: weighted Remez, with weight 2000 at notches at multiples of the sample rate and 66 at
  the peaks between them; transition band don't-care.
- Table 1 prints a typical 7-point kernel (223 coefficients). Two values are misprinted and
  corrected in `tools/interp_kernels.py`. Its response: −3 dB at 0.96 of the source Nyquist,
  notches −87 to −124 dB, peaks −63 to −76 dB.
- "Typically a family of six filters": lower cutoffs for upward pitch shifts and to darken the
  timbre, selected per voice. FIG. 6A–F plots them without an axis scale.

How the core uses it:
- Tone 0 is the Table 1 kernel.
- Tones 1–5 (the firmware's range; 6–7 are defined but unused) come from the same design method
  fitted to Table 1. The −6 dB point is scaled by 5 semitones per tone, because the firmware raises
  the tone value by one per 5 semitones of upward transposition [I].
- The exact cutoffs of tones 1–5 are unknown.

**US 5,342,990** — Rossum, "Digital sampling instrument employing cache-memory", filed 1992-05-11
(continuation-in-part of 5,111,727).
- Looping: "the new current address is compared against a loop end address. If the end address
  has been exceeded, the size of the loop is subtracted."
- The interpolator reads straight across the loop seam; the sample data must repeat around it.

This matches the core's existing loop rule, and the guard words after each sample in the ROM.

## Consistent with the core; nothing to change

**US 5,072,645** — Rossum, output stage with automatic detection of submix outputs, filed
1990-01-11.
- Describes the Proteus jack layout: a main stereo pair plus submix pairs 2 and 3.
- Each channel has a per-channel output-select number.
- Notes go to the main pair when no plug is in a submix jack.
- The ring of each submix jack is summed into the main output.

The core reports no plugs, so the firmware routes everything to the main pair, which matches
summing to stereo.

**US 5,170,369** — Rossum, dynamic digital IIR filter, filed 1990-08-29.
- A per-sample increment toward a target, clamped at the target, with an exponent/mantissa
  increment format.
- It describes a filter chip, not this one.

The Proteus firmware computes its envelope rate as a linear level difference shifted by a power of
two (`0x4866–0x4882`). That fits the core's linear ramp; nothing in the patent says this chip uses
its increment encoding.

**US 5,317,104** — Frost, Procussion, filed 1992.
- "32 audio channels… channels 1 and 2 are phase locked."
- "Each individual channel has its own independent control of pitch shifting value, its own
  envelope shape, and its own volume and pan control."

The Proteus firmware writes paired ("stacked") voices together with interrupts off (`0x4A42`).
How the chip phase-locks a pair, if it does, is unknown.

## Checked, not about this chip
- **Lineage only:**
  - US 4,987,600, 5,144,676, 5,303,309: Emulator II-era, 8 channels.
  - US 6,138,207, 5,763,800: EMU10K1 and SoundFont era.
- **Sample preparation** (crossfade loops, resampling, done offline):
  - US 5,298,671.
  - WO 1991/010987.
- **Unrelated:** US 5,248,845, 5,524,074, 5,430,244, 4,506,579, 4,699,038, 5,013,105, 6,111,184.
