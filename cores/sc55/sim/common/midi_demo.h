#pragma once
#include <cstdint>
#include <vector>

// GM reset, then random channel traffic (notes, programs, controllers, pitch
// bend, GS parameter SysEx with Roland checksums). Same stream for a seed.
std::vector<uint8_t> make_midi_demo(uint64_t seed);

// Loud, dense traffic at full volume and sends, sweeping all reverb x chorus macros.
std::vector<uint8_t> make_midi_loud(uint64_t seed);
