#include "midi_demo.h"

#include <random>

std::vector<uint8_t> make_midi_demo(uint64_t seed)
{
    std::vector<uint8_t> out;
    const uint64_t midi_seed = seed;
        std::mt19937_64 mr(midi_seed);
        const uint8_t gm_reset[] = {0xf0, 0x7e, 0x7f, 0x09, 0x01, 0xf7};
        out.insert(out.end(), gm_reset, gm_reset + sizeof gm_reset);
        for (int i = 0; i < 4000; ++i)
        {
            uint8_t ch = (uint8_t)(mr() % 16);
            switch (mr() % 10)
            {
            case 0: case 1: case 2: case 3:
                out.insert(out.end(), {(uint8_t)(0x90 | ch), (uint8_t)(36 + mr() % 60), (uint8_t)(1 + mr() % 127)}); break;
            case 4: case 5:
                out.insert(out.end(), {(uint8_t)(0x80 | ch), (uint8_t)(36 + mr() % 60), 0x40}); break;
            case 6:
                out.insert(out.end(), {(uint8_t)(0xc0 | ch), (uint8_t)(mr() % 128)}); break;
            case 7: {
                static const uint8_t ccs[] = {1, 7, 10, 11, 64, 91, 93, 121, 0, 32, 100, 101, 6};
                out.insert(out.end(), {(uint8_t)(0xb0 | ch), ccs[mr() % sizeof ccs], (uint8_t)(mr() % 128)}); break;
            }
            case 8:
                out.insert(out.end(), {(uint8_t)(0xe0 | ch), (uint8_t)(mr() % 128), (uint8_t)(mr() % 128)}); break;
            default: {
                // GS: reverb/chorus macro, master volume, part parameters (Roland checksum)
                uint8_t addr[3] = {0x40, 0x01, (uint8_t)(0x30 + mr() % 0x10)};
                if (mr() % 2) { addr[1] = (uint8_t)(0x10 + mr() % 16); addr[2] = (uint8_t)(0x15 + mr() % 0x20); }
                uint8_t data = (uint8_t)(mr() % 128);
                uint8_t sum = (uint8_t)((128 - ((addr[0] + addr[1] + addr[2] + data) & 0x7f)) & 0x7f);
                out.insert(out.end(), {0xf0, 0x41, 0x10, 0x42, 0x12, addr[0], addr[1], addr[2], data, sum, 0xf7});
                break;
            }
            }
        }
    return out;
}

static void gs_param(std::vector<uint8_t>& out, uint8_t a0, uint8_t a1, uint8_t a2, uint8_t data)
{
    uint8_t sum = (uint8_t)((128 - ((a0 + a1 + a2 + data) & 0x7f)) & 0x7f);
    out.insert(out.end(), {0xf0, 0x41, 0x10, 0x42, 0x12, a0, a1, a2, data, sum, 0xf7});
}

std::vector<uint8_t> make_midi_loud(uint64_t seed)
{
    // Loud and dense: GM reset, master volume, every channel at full volume,
    // expression and sends; then for each reverb macro x chorus macro pair a
    // burst of velocity-127 chords on several programs plus drums.
    std::mt19937_64 mr(seed);
    std::vector<uint8_t> out = {0xf0, 0x7e, 0x7f, 0x09, 0x01, 0xf7};
    out.insert(out.end(), {0xf0, 0x7f, 0x7f, 0x04, 0x01, 0x00, 0x7f, 0xf7});   // master volume max
    static const uint8_t programs[] = {0, 16, 48, 61, 80, 29, 52, 73, 19, 38};
    for (uint8_t ch = 0; ch < 16; ++ch)
    {
        out.insert(out.end(), {(uint8_t)(0xb0 | ch), 7, 127, (uint8_t)(0xb0 | ch), 11, 127,
                               (uint8_t)(0xb0 | ch), 91, 127, (uint8_t)(0xb0 | ch), 93, 127});
        if (ch != 9) out.insert(out.end(), {(uint8_t)(0xc0 | ch), programs[ch % 10]});
    }
    for (int round = 0; round < 64; ++round)
    {
        gs_param(out, 0x40, 0x01, 0x30, (uint8_t)(round % 8));          // reverb macro
        gs_param(out, 0x40, 0x01, 0x38, (uint8_t)((round / 8) % 8));    // chorus macro
        gs_param(out, 0x40, 0x01, 0x33, 127);                           // reverb level
        gs_param(out, 0x40, 0x01, 0x3a, 127);                           // chorus level
        for (int chord = 0; chord < 6; ++chord)
        {
            for (int v = 0; v < 6; ++v)
            {
                uint8_t ch = (uint8_t)(mr() % 16);
                out.insert(out.end(), {(uint8_t)(0x90 | ch), (uint8_t)(30 + mr() % 70), 127});
            }
            out.insert(out.end(), {(uint8_t)(0xb0 | (mr() % 16)), 1, (uint8_t)(mr() % 128)});
            out.insert(out.end(), {(uint8_t)(0xe0 | (mr() % 16)), (uint8_t)(mr() % 128), (uint8_t)(mr() % 128)});
        }
        for (uint8_t ch = 0; ch < 16; ++ch)
            out.insert(out.end(), {(uint8_t)(0xb0 | ch), 123, 0});      // all notes off
    }
    return out;
}
