# P2K OS 2.26: SIMM Baby-flash "lib_" library formats (static RE)

Tags: [O] read from code, [I] inferred, [U] unknown. Addresses are OS 2.26 (linked 0x01000000).

## Library container
- Object = C++-ish lib (constructor 0x1037D20): +4 tag ("_POS" for SIMM libs), +12 base offset
  (slot base + 16), +20 size (0x1FFFF0), +24 max object ids (ROM lib 0x1388 = 5000, flash 0x2710),
  +26 extra per-object header bytes (0 for SIMM libs), +36 lib-header word +14 [O 0x1037D20, 0x100DDEA].
- Lib header, 16 bytes at base [O 0x1037DEC]: "lib_" (required, else status 107), +4 type 4cc
  (NOT checked on open; the writer 0x1038508 stores the constructor tag, so "_POS" [I]),
  +8 u32 size (stored by writer, not checked on open [O]), +12 u16 must be <= 64 [O],
  +14 u16 = extra bytes between header and first object (first object at base+16+this) [O 0x1038FA4].
- Objects follow back to back, no padding/rounding: {u32 type 4cc, u32 len incl. 12-byte header,
  u16 id, u16 flags} + data [O 0x1037FA8]. Next object = offset + len.
- Index build 0x1037FA8: table of max-id entries {type, offset, len}, indexed by object **id**
  (id >= max -> status 103). len == 0 -> status 107. len == 12 -> entry cleared (deleted object).
  Walk stops at type 0xFFFFFFFF or at the end of the region.
- Terminator [O 0x10380E0-0x1038188]: after the 0xFFFFFFFF type, the long at terminator+12 is read;
  if it is not 0xFFFFFFFF the OS scans back from the end over 0xFFFFFFFF longs and calls the
  write/erase method, which the ROM class does not support -> open fails [I]. So: fill everything
  after the last object with 0xFF (erased-flash state). No CRC/checksum/directory object [O].
- Lookups are by id (0x1038700 read data, 0x10388E0 get {type,len,offset}, 0x1037F5C next used
  id, 0x1038640 count of used ids). Data read = offset + 12 + extra(0) [O 0x1038938].
- Flags word: not referenced by the reader [I]; user flash uses 0xFFFF.

## yh_1 (library header object) [O 0x100CF02-0x100D068]
Found by scanning all ids for type "yh_1" (0x10388E0). Data:
- +0 u32 romID. Must equal the SIMM's EOS-header ID (header +0x3E, byte in table 0x114D5D8);
  if not, +44 is tried. Low byte (+3) is the registered ID (byte).
- +36 shortName: copied with strncpy(6) + NUL into the name registry 0x1123FAC (7-byte entries
  {id, name[6]}; entry 0 = id 0 "User ") -> 5 characters shown [O 0x100C84C, 0x100C7B0].
  Registration fails if the ID is already present -> "ERR: 2 SIMMs w/Same ID".
- +44 u32 alternate romID: if header ID == +44, the OS rewrites the slot ID to +0 (0x102C38C) [O].
- Debug print "lib %d: romID: %ld shortName: %s numEntries: %ld" (+0, +36, object count) [O].

## Registries (how objects become numbered records) [O 0x100D4CC, 0x100BDE8, 0x100C2D4]
Created at 0x100D4CC: {type, record size, 5 ROM slots}:
| registry | type | record size | notes |
|---|---|---|---|
| 0x1123FB8 | yp_1 | 980 (0x3D4) | presets; vtable 0x100D77C |
| 0x1123FCC | ys_1 | 52 (0x34) | sample headers; vtable 0x100D7A4 |
| 0x1123FE0 | yi_1 | variable (-1) | instruments; default vtable |
| 0x1123FF4 | yd_1 | variable | vtable 0x100F968 |
| 0x1124008 | ya_1 | 141 (0x8D) | vtable 0x100D7CC |
| 0x112401C | yr_1 | variable | default vtable |
- Loader 0x100C2D4(registry, lib, romID, slot): counts records of the registry's type (0x100C514);
  **count 0 -> returns OK** (any type may be absent) [O 0x100C310].
- Records are numbered 0..n-1 in object-id order (ids walked with 0x1037EF4/0x1037F5C) [O].
- Fixed-size types: count = data length / record size, but the loader (vtable+32 = 0x100C758) reads
  only `record size` bytes per object (0x1038700 with len = recsize), so use exactly **one record
  per object** (object len = 12 + recsize) [O/I].
- Variable types: one record per object, any length [O].
- Lookup: 0x100C24C(registry, romID byte, index) -> record pointer (romID must be registered,
  index < count) [O]; 0x100C200 takes a flat index across slots [O].

## ys_1 sample header, 52 bytes [O 0x102C21C, 0x101AAB8, 0x1018E2E, 0x101904A]
Fields per channel ch (0 = mono/left, 1 = right):
| off | size | use |
|---|---|---|
| +0..+15 | 16 | not read by the voice path [U: name/EOS sample number?] |
| +16 + 4ch | u32 | sample start |
| +24 + 4ch | u32 | sample end |
| +32 + 4ch | u32 | loop start |
| +40 + 4ch | u32 | loop end |
| +48 | s16 | pitch offset added to the zone/preset pitch (units of 1/64 semitone [I]) 0x1018E2E |
| +50 | u8 | bits 0-1: channel layout (3 = stereo pair, 2 voices) 0x101AB28; bit 3: no release jump 0x102C350; bits 4-5: loop/play mode (below) |
| +51 | u8 | romID; sound-memory base = slot byte base (0x102C5D8: slot << 25) >> 1 |
- Addresses are **word offsets relative to the SIMM slot** (added to base = (slot<<25)>>1) [O 0x102C246].
- Mode (+50 bits 4-5, or a caller override) -> voice params vp (0x102C21C), `off` = start offset:
  - 0 or 3 (one-shot): vp.cur = base+off+start; vp.loopStart = 8 (idle silence loop);
    vp.loopEnd = base+end.
  - 1, 2 (forward loop): vp.cur = base+off+start; vp.loopStart = base+loopStart;
    vp.loopEnd = base+loopEnd-1.
  - 4 (reverse one-shot) / 5,6 (reverse loop) use end/start mirrored (mode field is 2 bits, so
    4-6 only come from callers) [O].
- Voice start 0x10327E0 writes G20 regs: +0x00 = 0x1800, +0x04 = 0, +0x18 = vp.cur,
  +0x14 = 0x04000000 | vp.loopStart, +0x0C = NOT(vp.loopEnd), +0x10 = vp+28;
  if vp.loopStart == 8: +0x14 = idle start, +0x08 low 26 = NOT(idle end) (idle loop at
  SIMM header +0x4E words) [O].
- Release (0x102C33C), when bit 3 clear: loopStart = 8 (idle), loopEnd = base + long at
  +18 + 4k [U: k from the voice, offset looks odd].

## yi_1 instrument (variable length) [O 0x101AE7C, 0x103F820, 0x1018D0C, 0x1018E2E, 0x10190B0]
Lookup 0x103F820(romID, instrument number) -> 0x100C24C(yi_1 registry); missing -> OS default
instrument at 0x0104A2D6 = "(not installed) " with 0 zones.
| off | size | use |
|---|---|---|
| +0 | 16 | name ("(not installed) " in the default) [O data, I use] |
| +16 | u8 | zone count [O 0x101AF7A] |
| +17..19 | 3 | pad [I] |
| +20 + 36z | 36 | zone z |
Zone (36 bytes):
| off | size | use |
|---|---|---|
| +0 | 4 | key range {lo, lo-fade width, hi-fade width, hi}; full level inside [lo+loFade, hi-hiFade], -96 outside [O 0x1018D0C] |
| +4 | 4 | velocity range, same format [O 0x101AF9C] |
| +8 | u16 | sample number = index into the ys_1 records of the ROM in +34 [O 0x101AAC6] |
| +10 | s16 | fine tune, added to pitch (1/64 semitone [I]) [O 0x1018E36] |
| +12 | u8 | root (original) key: pitch -= root*64 [O 0x1018EAE] |
| +13 | s8 | volume offset [O 0x10190B6] |
| +14 | s8 | pan offset [O 0x1019108] |
| +15 | u8 | bits 0-1 channel select for stereo samples (0 = both) [O 0x101AB3A] |
| +16, +18 | u16 | copied to voice +8 (with layer +12 *32) and +10 [U] |
| +20..31 | 12 | factory volume envelope, used when the layer's volume envelope mode (SysEx 1793, layer +0x38) is 0 "Factory" [O 0x0101A83E: zone +0x14 instead of layer +0x2C, same loader 0x0101A33C]. Same layout as the layer's envelope: {rate, level} for Attack 1, Attack 2, Decay 1, Decay 2, Release 1, Release 2; rate 0..127 (0 fastest), level 0..100 |
| +32 | u8 | envelope mode: 2 = tempo-based, else time-based [O 0x0101A3D0] |
| +33 | u8 | low nibble copied to envelope +0x30 (repeat flag: non-zero keeps cycling at the stage mask 0x28) [O 0x0101A438, 0x0101A27A] |
| +34 | u8 | sample romID [O 0x101AAE2] |
| +35 | | [U] |
Envelope rates [O 0x0101B684]: each tick a stage moves by table[rate] × elapsed ticks
(u16 × 128 at 0x0101CBFC, 65535 at rate 0 down to 4 at rate 127) towards level × 512. Full scale
at rate 127 is 12800 ticks; with the manual's "about 1 ms to 160 seconds" a tick is ~12.5 ms [I].
Seen in MAME (2026-11-26): a zone with all-zero envelope bytes in Factory mode never sounds (the
level bytes stay 0), and an attack that completes on the first tick also leaves the level
unwritten until the release [U why: another zone field may matter]; the Composer stand-in uses
an attack rate of at least 2.
Pitch word (0x1018E2E): ys+48 + zone+10 + layer coarse*64 + layer fine + keytable[key] - root*64
(layer +19 bit 6 = no key tracking) [O].

## yp_1 preset, 980 bytes (= user-bank pr01 data, 0x3E0 - 12) [O flash dump 0xA0310, 0x101AE7C]
- +8 name[16]. Layers: 4 x 0xCC bytes at +0xA4, +0x170, +0x23C, +0x308 (first bytes "00 CC") [O dump].
- Layer +5 u8 = instrument romID, +6 u16 = instrument number (lookup 0x103F820) [O 0x101AF50].
  Other layer fields used at note-on: +10/+11 coarse/fine, +12..+19, +36 bit 7 (12th-order filter,
  2 voices per channel), +98..+102 (volume/pan/envelope-ish) [O, meaning U].
- User preset "kb1:DynamicGrand" (flash 0xA0310) layer 0 = romID 4, instrument 1; layer 1 =
  romID 4, instrument 505; layer 3 instrument 0 [O]. romID 4 = the Composer ROM = MIDI bank MSB 4
  [I: E-mu Sound Authoring guide "ROM ID is the MSB of the MIDI Bank Select"; flash IDs 106-119].
- Minimal preset: copy a user pr01 record (or the OS default preset at 0x0104A2EA,
  "(not installed)") and set layer 0 +5/+6; set unused layers' instrument to a 0-zone
  instrument [I].

## yd_1 / ya_1 / yr_1 (optional)
- yd_1: demo sequences ("DemoProxy", 16-byte name then data, loaded lazily 0x100F59C) [O/I].
- ya_1: arpeggiator patterns, 141 bytes [I: "arps" registry, ar01 user objects].
- yr_1: riffs (16-byte name then a Standard MIDI File; OS default 0x0104A6C0 "(not installed)" + MThd) [O data].
- All three can be absent: the loader returns OK for count 0 and lookups fall back to OS
  defaults [O 0x100C310, 0x103F88C] [I that every UI path tolerates it].

## Bank name / ID
- Name shown on the main screen comes from the registry 0x1123FAC filled from yh_1 +36 (5 chars)
  [O 0x100C84C]; id 0 = "User " [O 0x100CD50].
- romID is a byte; it must be unique across slots and match the SIMM EOS header +0x3E [O].
