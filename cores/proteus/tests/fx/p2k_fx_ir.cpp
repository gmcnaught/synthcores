// Impulse response of the G05 reverb/chorus workalike (p2k::fx_int in p2k/model/p2k_sound.h) for a
// G05 register state.
// Build: c++ -std=c++17 -O2 -Ip2k/model tests/fx/p2k_fx_ir.cpp -o build/p2k/fx_ir
// Input: 32 lines of 64 decimal words (G05 channel x word), e.g. from tools/p2k_g05state.py --regs.
// Output: energy per 100 ms block in dB (add any second argument for per-sample L R, in samples).
#include "p2k_sound.h"
#include <cmath>
#include <cstdio>
int main(int argc, char **argv)
{
	static uint16_t regs[32][64];
	FILE *f = argc > 1 ? std::fopen(argv[1], "r") : nullptr;
	if (!f)
	{
		std::fprintf(stderr, "usage: fx_ir REGS [--samples]\n");
		return 2;
	}
	for (int c = 0; c < 32; c++)
		for (int w = 0; w < 64; w++)
		{
			unsigned v = 0;
			if (std::fscanf(f, "%u", &v) == 1)
				regs[c][w] = uint16_t(v);
		}
	p2k::fx_int fx;
	fx.configure(regs);
	const int n = 44100 * 3;
	double e[30] = {};
	for (int i = 0; i < n; i++)
	{
		int64_t l = 0, r = 0;   // Q8
		fx.process(i == 0 ? 10000 << 8 : 0, 0, l, r);
		e[i / 4410] += double(l) * l / 65536.0 + double(r) * r / 65536.0;
		if (argc > 2)
			std::printf("%d %f %f\n", i, l / 256.0, r / 256.0);
	}
	if (argc <= 2)
	{
		for (int k = 0; k < 30; k++)
			std::printf("%.1f ", 10 * std::log10(e[k] + 1e-12));
		std::printf("\n");
	}
}
