// Impulse response of emup2k_fx.h (the MAME G05 reverb/chorus workalike) for a G05 register state.
// Build: clang++ -std=c++17 -O2 -Ithird_party/mame/src/mame/emusys tests/fx/p2k_fx_ir.cpp -o build/p2k/fx_ir
// Input: 32 lines of 64 decimal words (G05 channel x word), e.g. from tools/p2k_g05state.py --regs.
// Output: energy per 100 ms block in dB (add any second argument for per-sample L R).
#include "emup2k_fx.h"
#include <cstdio>
#include <cmath>
int main(int argc, char **argv)
{
	static uint16_t regs[32][64];
	FILE *f = fopen(argv[1], "r");
	for (int c = 0; c < 32; c++) for (int w = 0; w < 64; w++) { unsigned v; fscanf(f, "%u", &v); regs[c][w] = v; }
	emup2k_fx fx; fx.configure(regs);
	const int n = 44100 * 3; double e[30] = {0};
	for (int i = 0; i < n; i++) {
		float l = 0, r = 0; fx.process(i == 0 ? 10000.0f : 0.0f, 0.0f, l, r);
		e[i / 4410] += double(l) * l + double(r) * r;
		if (argc > 2) printf("%d %f %f\n", i, l, r);
	}
	if (argc <= 2) { for (int k = 0; k < 30; k++) printf("%.1f ", 10 * log10(e[k] + 1e-12)); printf("\n"); }
}
