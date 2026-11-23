// license:BSD-3-Clause
// Writes the uPD933 tables (computed by MAME's device_start code, upd933_ref.cpp) as $readmemh
// files for the HDL:
//   upd933_tables OUTDIR   -> OUTDIR/upd933_{cosine,pitch,pitch_fine,volume}.hex
// The HDL ROM widths are fixed here; the program fails if a value does not fit.
#include "upd933_ref.h"

#include <cstdio>
#include <string>

using namespace upd933ref;

template <typename T>
static bool emit(const std::string &dir, const char *name, const T *v, unsigned n, unsigned bits)
{
	const std::string path = dir + "/upd933_" + name + ".hex";
	FILE *f = fopen(path.c_str(), "w");
	if (!f) { fprintf(stderr, "cannot write %s\n", path.c_str()); return false; }
	const unsigned digits = (bits + 3) / 4;
	unsigned long long maxv = 0;
	for (unsigned i = 0; i < n; i++)
	{
		const unsigned long long x = v[i];
		if (x >> bits) { fprintf(stderr, "%s[%u] = %llx does not fit in %u bits\n", name, i, x, bits); fclose(f); return false; }
		if (x > maxv) maxv = x;
		fprintf(f, "%0*llx\n", digits, x);
	}
	fclose(f);
	printf("%-10s %4u x %2u bits, max %llu\n", name, n, bits, maxv);
	return true;
}

int main(int argc, char **argv)
{
	const std::string dir = argc > 1 ? argv[1] : ".";
	const Tables &t = tables();
	bool ok = emit(dir, "cosine", t.cosine, 0x800, 12)
		&& emit(dir, "pitch", t.pitch, 0x80, 30)
		&& emit(dir, "pitch_fine", t.pitch_fine, 0x200, 8)
		&& emit(dir, "volume", t.volume, 0x200, 14);
	return ok ? 0 : 1;
}
