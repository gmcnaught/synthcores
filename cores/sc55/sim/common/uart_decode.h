#pragma once
// Software UART for the board benches: decodes the board's 8N1 uart_txd (MIDI out), sampled once
// a clock, at the bit centres.
#include <cstdint>
#include <vector>

struct UartDecode
{
    explicit UartDecode(int64_t clocks_per_bit) : div(clocks_per_bit) {}

    void clock(int line)
    {
        ++now;
        if (at < 0)
        {
            if (prev && !line) { at = now + div / 2; bit = 0; sh = 0; }
        }
        else if (now == at)
        {
            if (bit == 0 && line) { ++errors; at = -1; }                       // not a start bit
            else if (bit >= 1 && bit <= 8) sh |= (unsigned)line << (bit - 1);
            else if (bit == 9)
            {
                if (line) bytes.push_back((uint8_t)sh);
                else ++errors;                                                    // no stop bit
                at = -1;
            }
            if (at >= 0) { ++bit; at += div; }
        }
        prev = line;
    }
    bool idle() const { return at < 0; }

    std::vector<uint8_t> bytes;
    int                  errors = 0;

private:
    int64_t  div, now = 0, at = -1;
    int      bit = 0, prev = 1;
    unsigned sh = 0;
};
