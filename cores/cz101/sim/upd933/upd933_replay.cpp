// license:BSD-3-Clause
// Runs the C++ reference on an event list converted from a real MAME log
// (cores/cz101/sim/upd933/upd933_log2ev.py) and checks it against what MAME logged:
//   upd933_replay FILE.ev [--samples CZ_SAMPLES.bin] [--seed 0x9d14abd7] [--out REF.s32]
// Checks: the stream-update rule (logged sample count at each access), read data, timer expiry
// ticks (T lines), IRQ changes (Q lines), and the samples against MAME's int32 dump.
#include "upd933_ref.h"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

using namespace upd933ref;

int main(int argc, char **argv)
{
	std::string evf, samplesf, outf;
	RunOptions opt;
	for (int i = 1; i < argc; i++)
	{
		const std::string a = argv[i];
		if (a == "--samples" && i + 1 < argc) samplesf = argv[++i];
		else if (a == "--seed" && i + 1 < argc) opt.rand_seed = u32(std::strtoul(argv[++i], nullptr, 0));
		else if (a == "--out" && i + 1 < argc) outf = argv[++i];
		else evf = a;
	}
	if (evf.empty()) { fprintf(stderr, "usage: %s FILE.ev [--samples MAME.s32] [--seed N] [--out REF.s32]\n", argv[0]); return 2; }

	std::vector<Event> ev;
	std::string err;
	if (!load_events(evf, ev, err)) { fprintf(stderr, "%s\n", err.c_str()); return 2; }
	const RunResult r = run(ev, opt);

	int bad = 0;
	auto line = [&](const char *what, u64 checked, u64 mism, const std::string &first) {
		printf("%-8s %10llu checked, %llu mismatched%s%s\n", what, (unsigned long long)checked, (unsigned long long)mism,
			mism ? "; first: " : "", mism ? first.c_str() : "");
		bad += mism != 0;
	};
	line("rule", r.rule_checked, r.rule_mismatch, r.rule_first);
	line("reads", r.read_checked, r.read_mismatch, r.read_first);
	line("timer", r.timer_checked, r.timer_mismatch, r.timer_first);
	line("irq", r.q_checked, r.q_mismatch, r.q_first);

	if (!outf.empty())
		if (FILE *o = fopen(outf.c_str(), "wb")) { fwrite(r.samples.data(), 4, r.samples.size(), o); fclose(o); }

	if (!samplesf.empty())
	{
		FILE *f = fopen(samplesf.c_str(), "rb");
		if (!f) { fprintf(stderr, "cannot open %s\n", samplesf.c_str()); return 2; }
		std::vector<s32> m;
		s32 v;
		while (fread(&v, 4, 1, f) == 1) m.push_back(v);
		fclose(f);
		const size_t n = std::min(m.size(), r.samples.size());
		size_t first = n, diffs = 0;
		for (size_t i = 0; i < n; i++)
			if (m[i] != r.samples[i]) { diffs++; if (first == n) first = i; }
		printf("samples  %10zu compared (MAME %zu, model %zu), %zu mismatched", n, m.size(), r.samples.size(), diffs);
		if (diffs) printf("; first at %zu: model %d, MAME %d", first, r.samples[first], m[first]);
		printf("\n");
		bad += diffs != 0;
	}
	printf("%s\n", bad ? "FAIL" : "PASS");
	return bad ? 1 : 0;
}
