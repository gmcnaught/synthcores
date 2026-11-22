# Proteus 2000 family hardware: patent search report (E-mu / Creative, 1993-2003)

Date of search: 2026-11-21. Method and source limits are at the end.

## Bottom line

- **Observed:** No US/EP/WO patent was found, filed 1993-2003 by E-mu Systems or Creative Technology, that describes a
  Proteus-2000-class sound/voice ASIC ("G-II"), a Z-plane filter ASIC ("H1.5"), an EMU8000-derived effects chip for
  an instrument, or ROM SIMM / sound-set handling. After 1993, E-mu Systems as assignee filed only two US patents:
  US 5,430,244 (MIDI input correction) and US 6,111,184 (guitar pickups). Creative-assigned patents by E-mu staff
  from 1995-2003 cover PC-audio silicon: the EMU10K1/FX8010 and PCI cache/DMA.
- **Inferred:** The Z-plane filter was apparently not patented as a separate invention. It rests on US 5,170,369
  (already known). The closest written description of morphing between filter frames is in two places:
  (a) the 1992 Massie/Rossum patent US 5,248,845 (and its continuation US 5,698,807), which describes a chip
  doing per-sample interpolation between two coefficient sets for 32 channels; and (b) Rossum's 2017-priority
  US 10,514,883, which is outside the date window but describes the 8-vertex "cube", 14th-order, encoded-coefficient
  morphing scheme and names E-mu's MORPHEUS and ULTRA-PROTEUS as prior art.
- **Newer continuations of patents already known:**
  - US 5,342,990 → US 5,698,803 → US 5,925,841 → US 6,137,043 → US 6,365,816 → **US 6,858,790 B2** (filed 2002-02-21,
    granted 2005-02-22). This is the newest member of the chain.
  - US 6,138,207 → **US 6,622,207 B1** (filed 2000-09-05).
  - No continuation of US 5,170,369 issued. An application cross-referenced in 1992, **Ser. No. 07/670,451, filed
    1991-03-08, "Dynamic Digital IIR Audio Filter"**, never appears as an issued patent in any E-mu/Creative listing.
    Its status is **Unknown**, probably abandoned.

## Ranked relevant patents

### 1. US 5,248,845, "Digital Sampling Instrument" (and continuation US 5,698,807)
- Inventors: Dana C. Massie, David P. Rossum. Assignee: E-mu Systems, Inc. (5,698,807: Creative Technology Ltd).
- Filed 1992-03-20 (Appl. 07/854,554). Granted 1993-09-28. Continuation US 5,698,807: filed 1996-03-05, granted 1997-12-16.
- Subject (Observed): formant/excitation separation to stop "munchkinization" during sample transposition. An analysis
  stage stores a formant filter and an excitation signal. On playback, the excitation is pitch-shifted by interpolated
  sample playback and then passed through a time-varying formant (pole-zero) filter. Figs. 4-6 show second-order
  all-pole, all-zero and pole-zero sections.
- Key text (Observed, verbatim, col. 16, near the end of the description):
  > "A preferred embodiment for a time varying formant filter is described in the above-identified cross-referenced
  > patent applications, which illustrate techniques which allow 32 channels of audio data to be filtered in a
  > time-varying manner in real time by a single silicon chip. The aforementioned patent applications teach that two
  > sets of filter coefficients can be loaded by a host microprocessor into the chip and the chip can then
  > interpolate between them. This interpolation is performed at the sample rate and eliminates any audible artifacts
  > from time-varying filters, or from interpolating between different formant shapes."
- Also (Observed, col. 16): "...interpolation is implemented using log-spaced frequency values since log-spaced frequency
  values produce the most natural transitions between formant spectra." It describes velocity morphing: "a first formant
  filter would correspond to soft sounds, a second formant filter would correspond to loud sounds, and interpolations
  between the two filters would correspond to intermediate level sounds."
- Cross-referenced applications (Observed, col. 1): Ser. 07/462,392 (= US 5,111,727), Ser. 07/576,203 (= US 5,170,369)
  and **Ser. 07/670,451, filed Mar. 8, 1991, "Dynamic Digital IIR Audio Filter"**.
- Pitch-shift interpolator (Observed): the convolution form with coefficients C_i(f) that are "approximately a windowed"
  sinc, as in 5,111,727. It refers to the 5,111,727 applications for details and gives no new point count.
- Relevance (Inferred): this is the only E-mu-era patent text found that states a **single-chip, 32-channel, per-sample
  interpolation between two host-loaded coefficient sets**. That is the 1991-92 filter-chip capability that preceded
  Morpheus Z-plane (1993). It does not mention 64 voices, ROM SIMMs, or more than two frames.

### 2. US 10,514,883 B2, "Multi-channel morphing digital audio filter" (outside the date window; context only)
- Inventor: Dave Rossum. Assignee: Rossum Electro-Music, LLC. Provisional 62/462,703 filed 2017-02-23. Granted 2019-12-24.
  It cites US 5,170,369 and Rossum's "The 'ARMAdillo' Coefficient Encoding Scheme for Digital Audio Filters" (IEEE ASSP
  Workshop, Oct. 1991).
- E-mu statement (Observed, verbatim, col. 1, Background; OCR):
  > "music synthesizers released by E-mu Systems, Inc., including MORPHEUS and ULTRA-PROTEUS, provided the ability to
  > smoothly change a filter function over time. However, conventional systems were unable to provide an unconditionally
  > stable filter structure or morph in multiple axes in real-time. Furthermore, traditionally the pole and zero frequency
  > and amplitude have not been completely independent, which may cause undesirable interaction."
- What the new filter does (Observed, col. 3-4):
  - "a complex, 14th order, filter having a frequency response that varies in real-time ... determined by interpolating
    among as many as eight different fixed frequency responses ... represented as the eight vertices of a cube."
  - The three axes are frequency, morph and transform.
  - One example configuration is "six independent bands of parametric equalizer sections ... and a low-pass section".
  - Coefficients are "encoded into a perceptual space so that interpolation within the encoded space results in
    perceptually even changes when the linearly interpolated encoded coefficients are decoded". Decoding uses
    "exponentiation from the logarithmic space".
- Relevance (Inferred): this documents the Z-plane cube concept: 8 frames, 3 axes, 7 second-order sections, linear
  interpolation of log-encoded pole/zero parameters. **It describes Rossum's 2017 design, not the E-mu H-chip.** By its
  own statement, the E-mu implementation differed: it was not unconditionally stable, and pole/zero frequency and
  amplitude were not fully independent. The Morpheus restriction that only one axis is real-time and the others are set
  at note-on comes from Rossum's product page, not from the patent.

### 3. US 5,928,342, "Audio effects processor integrated on a single chip with a multiport memory..."
- Inventors: David P. Rossum, Jay Scott Fuller. Assignee: Creative Technology Ltd. Filed 1997-07-02. Granted 1999-07-27.
- Subject (Observed): the EMU8010 (EMU10K1) PC audio chip: FX8010 effects DSP, TRAM delay memory, sample-rate converters,
  AC97/S/PDIF I/O.
- Key text (Observed, verbatim, col. 3-4):
  > "Sound Engine. The Sound engine 32 is a 64-channel version of the prior EMU8000 wavetable synthesis engine. One
  > difference is that any four of 16 output channels can be selected as the destinations for each Synthesizer Voice,
  > with (smoothed) variable scaling on each of the four sends. The 16 Synthesizer output channels become 16 input
  > channels to the sound effects engine."
  (OCR reads "ends"; "sends" is the evident word.)
- Relevance (Inferred): this confirms a 1997 Creative/E-mu 64-voice EMU8000-lineage engine. Any link to the
  G-II (Proteus 2000's 64 voices per chip) is **unverified**. The patent gives no interpolation, filter or envelope
  details for the voice engine.
- Related FX8010-family patents (EMU10K1 DSP; same relevance caveat): US 5,930,158 (Hoge, instruction set; filed
  1997-07-02), US 6,529,922 (linear-to-log instruction), US 6,032,235 (Hoge, TRAM memory init), US 6,275,899 (Savell/Hoge,
  delay caches; filed 1998-11-13), US 6,351,757 (Lange/Hoge, pooled INTERP instructions for parameter ramps; filed
  1998-11-13), US 6,289,435, US 6,195,715, AU 8181198 A.

### 4. US 6,858,790 B2 / US 6,365,816 B1, "Digital sampling instrument employing cache memory" (newest continuations of 5,342,990)
- Inventor: David P. Rossum. Assignee: Creative Technology Ltd.
- Chain (Observed, Related U.S. Application Data):
  - 5,342,990 (Ser. 07/882,178, filed 1992-05-11) → Ser. 08/202,922 (filed 1994-02-28, abandoned)
  - → US 5,698,803 (Ser. 08/636,827, filed 1996-04-23; assignee E-mu Systems; granted 1997-12-16)
  - → US 5,925,841 (division, Ser. 08/903,329, filed 1997-07-29)
  - → US 6,137,043 (Ser. 09/187,139, filed 1998-11-06)
  - → US 6,365,816 (Ser. 09/618,963, filed 2000-07-19)
  - → US 6,858,790 (Ser. 10/080,527, filed 2002-02-21, granted 2005-02-22).
- US 6,858,790 claim 1 (Observed, verbatim):
  > "A method for the multichannel interpolative playback of digital waveform data samples stored in a waveform memory,
  > comprising: accessing said waveform memory samples from said waveform memory using an address update unit and a
  > memory access unit; storing two or more waveform memory samples for each channel in a cache memory; linearly
  > interpolating between two adjacent waveform memory samples stored in said cache memory to form a linear interpolation
  > result, and operating said memory access unit asynchronously from said address update unit and said linearly
  > interpolating."
- Description is the 5,342,990 text: L channels, N-point interpolator, M memory accesses per sample period, priority unit,
  cache size encoded in log2(M) bits. Nothing new for P2K beyond the already-known patent.

### 5. US 6,622,207 B1, "Interpolation looping of prioritized audio samples in cache connected to system bus"
- Inventor: Rossum. Assignee: Creative. Filed 2000-09-05 (continuation of 6,138,207). Same EMU10K1 PCI cache/loop content
  as the known 6,138,207. It claims loopEnd-truncated cache requests, two-stage priority, and doubleword trimming for PCI.

### 6. Lower relevance, E-mu-originated
- **US 5,877,446**, "Data compression of sound data" (Monahan et al.; Creative; filed 1997-09-16; continuation chain from
  Ser. 465,732, 1990-01-18; foreign counterparts GB 2,270,448 and DE 4190102). Observed: sample-data reduction by deleting
  data between attack and loop start and splicing; single-cycle loop duplication; FIR lowpass/bandpass/highpass;
  crossfade loop. Inferred: possibly relevant to how E-mu ROM sound sets were authored. No hardware or ROM format given.
- **US 5,524,074 / US 5,748,747**, "Digital signal processor for adding harmonic content..." (Massie; E-mu, filed
  1992-06-29; continuation Creative, filed 1997-04-09). Observed: priority-encoder plus barrel-shift nonlinear
  "fractal" transfer function, with scale/offset table lookup and optional input/output filters. No link to any
  product is stated.

## Checked and rejected (one line each)
- US 6,111,184 (E-mu, Cloud, 1998): interchangeable guitar pickups. Not synth silicon.
- US 5,430,244 (E-mu, Ruffcorn / "Robert S." [surname OCR-garbled] / Scott Wedge, 1993): MIDI input-stream scale/harmony correction. Software, not the voice engine.
- US 5,317,104 (E-mu, Frost): known (Procussion).
- EP 0 819 300 B1 / WO 96/31867 (Rossum, Creative, priority 1995-04-07): FM synthesis with a first-order FIR highpass
  between modulator and carrier. OPL3-compatible PC FM, not sample playback.
- US 6,091,269 / EP 0 819 301 B1 / WO 96/31868 (Rossum, Creative, 1995): OPL3 waveform generation by mux/shift plus Boolean
  gates. PC FM.
- US 5,864,876 (Rossum/Tan, Creative, 1997): DMA with local page table. PC bus.
- US 6,016,522, US 6,092,126, US 6,324,235, US 6,819,732, US 6,226,661 (Rossum/Savell, Creative, 1997-2000): sample-rate
  tracking/conversion for PC digital inputs.
- US 7,280,878 (Rossum, Creative, filed 1999-10-27): SRC with distributed filtering. Its FIR coefficients are linearly
  interpolated from a table. It is a computer-program SRC with no instrument voice engine (could be revisited if the
  G-II interpolator kernel is of interest).
- US 5,943,427 (Massie/Rossum, Creative, 1995): 3D audio spatialization.
- US 6,084,170 (Laroche, Creative, 1999): offline loop-point harmonic adjustment. Authoring, not hardware.
- US 5,763,800 / EP 0 845 138: known (SoundFont formatting).
- Laroche, Jot, Avendano, Sibbald, Goodwin, Dicker (Creative ATC, 1996-2003): reverb, 3D, phase-vocoder and upmix work.
  None concern instrument voice silicon.
- US 5,170,369 counterparts DE 4030338 A1/C2: same disclosure as the known US patent.
- "Byron Sheppard": the only audio match is EP 0 778 536 B1 (Nintendo, 1995, N64 coprocessor). Not E-mu.
- "Michael Hendricks": no E-mu or Creative patents found.
- "Scott Wedge": only US 4,699,038 (1985) and US 5,430,244 (1993).
- "Dave Hoge": not found. Stephen Hoge (Creative) patents are all FX8010/EMU10K1 (listed above).
- "Robert Crawford": Robert S. Crawford appears on US 5,763,800 (known). A "Robert S." co-inventor on US 5,430,244 may be Crawford (OCR-garbled, not verified).
- Google keyword searches "morphing filter" and "z-plane interpolation", 1992-2003: no audio-filter hits.

## Unknown / not checked
- Ser. No. 07/670,451 (1991, "Dynamic Digital IIR Audio Filter"): no issued patent found. This is the most likely
  written description of the 1991-92 two-frame interpolating filter chip, if it was ever published. Applications from
  that era were not published unless granted, so the file wrapper would be needed.
- A full CPC G10H sweep of Creative patents for priority 1997-2003 was not completed because Google Patents rate-limited
  this session (HTTP 503). Coverage came from:
  - assignee "E-mu Systems" (all 21 results);
  - inventor searches restricted to Creative (Rossum, Massie, Hoge, Savell, Wedge, Ruffcorn, Monahan, Crawford, Laroche,
    Jot, Dicker, Frost);
  - "Creative" + filter (110 results; ~100 reviewed), + synthesizer (79), + envelope generator (16);
  - forward citations of US 5,111,727 and US 5,170,369.
- Ensoniq-origin patents assigned to Creative after the 1998 merger into E-mu/Ensoniq were not swept.
- No patent text found mentions SIMM, sound-set ROM identification, or 64-voice instrument chips.

## Sources
- Google Patents result pages and xhr search API (before rate limiting); full texts from patentimages.storage.googleapis.com
  PDFs (pdftotext). US 5,698,803 and US 10,514,883 come from USPTO image PDFs (image-ppubs.uspto.gov), OCR'd with
  tesseract, so their quotes may contain OCR errors.
- Rossum Electro-Music Morpheus product page (rossum-electro.com) for the note about the original Morpheus's real-time axis.
