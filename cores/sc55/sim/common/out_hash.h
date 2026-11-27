// --out-hash for the board benches: a hash of every board output after every clock edge, printed
// when the bench exits. Two builds of a board that print the same hash for the same run drive the
// same values on every output pin on every clock of that run (up to a 64-bit hash collision); the
// boards' refactors are checked this way (modules/roland/hdl/board/roland_board_common.sv).
#pragma once
#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

struct OutHash
{
    bool     on = false;
    uint64_t h = 0x9e3779b97f4a7c15ull, clocks = 0;
    void add(uint64_t v)
    {
        h ^= v + 0x9e3779b97f4a7c15ull + (h << 6) + (h >> 2);
        h *= 0xff51afd7ed558ccdull;
        h ^= h >> 33;
    }
    void print() const { if (on) printf("out-hash: %016" PRIx64 " over %" PRIu64 " clocks\n", h, clocks); }
};
inline OutHash g_out_hash;
// printed on every exit from the bench, pass or fail
inline void out_hash_enable()
{
    g_out_hash.on = true;
    std::atexit([] { g_out_hash.print(); fflush(stdout); });
}
