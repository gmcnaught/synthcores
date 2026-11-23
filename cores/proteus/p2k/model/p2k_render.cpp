// license:BSD-3-Clause
// copyright-holders:emu-proteus-xr-MiSTer contributors
/*
    Standalone Proteus 2000 sound renderer: replays a sound-register log (P2K_SNDLOG from the MAME
    research machine) through p2k_sound.h and writes the audio. Fed the log and the SIMM image of
    a corpus case, it must reproduce MAME's P2K_SNDOUT output bit for bit (docs/p2k-mame-plan.md,
    step 3); the HDL testbench compares against this program, without MAME.

      p2k_render [options] SNDLOG OUT(.wav|.raw)
        --simm N=FILE     sound SIMM image of slot N (1-4), big-endian bytes (Big device)
        --frames N        output length (default: last write + 3 s)
        --nofilter --nofx bypasses (as P2K_NOFILTER / P2K_NOFX)
        --float           render the floating-point reference instead (p2k_sound_float.h)
        --snr             also run the float reference and report what the integer arithmetic changes
      p2k_render --compare A.raw B.raw     first difference, SNR and peak error of B against A
*/
#include "p2k_sound_float.h"

#include <cinttypes>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <string>

namespace {

struct rec { int64_t sample; char chip; uint32_t a, b; uint16_t data, mask; };

std::vector<rec> read_log(const char *path)
{
	std::vector<rec> v;
	FILE *f = std::fopen(path, "r");
	if (!f)
	{
		std::perror(path);
		std::exit(2);
	}
	char line[256];
	while (std::fgets(line, sizeof(line), f))
	{
		if (line[0] == '#' || line[0] == '\n')
			continue;
		rec r{};
		long long s;
		char c;
		unsigned x, y, d, m;
		if (std::sscanf(line, "%lld %c %x %x %x %x", &s, &c, &x, &y, &d, &m) == 6 && c == 'X')
			r = { s, c, x, y >> 1, uint16_t(d), uint16_t(m) };
		else if (std::sscanf(line, "%lld %c %x %x %x", &s, &c, &x, &d, &m) == 5 && (c == 'G' || c == 'H'))
			r = { s, c, x >> 1, 0, uint16_t(d), uint16_t(m) };
		else
		{
			std::fprintf(stderr, "bad log line: %s", line);
			std::exit(2);
		}
		v.push_back(r);
	}
	std::fclose(f);
	return v;
}

void apply(p2k::sound_base &s, const rec &r)
{
	if (r.chip == 'G')
		s.write_g20(r.a, r.data, r.mask);
	else if (r.chip == 'H')
		s.write_h(r.a, r.data, r.mask);
	else
		s.write_g05(r.a, r.b, r.data, r.mask);
}

void write_out(const std::string &path, const std::vector<int16_t> &pcm)
{
	std::ofstream f(path, std::ios::binary);
	if (path.size() > 4 && path.substr(path.size() - 4) == ".wav")
	{
		const uint32_t bytes = uint32_t(pcm.size() * 2);
		auto u32 = [&](uint32_t x) { f.write(reinterpret_cast<const char *>(&x), 4); };
		auto u16 = [&](uint16_t x) { f.write(reinterpret_cast<const char *>(&x), 2); };
		f.write("RIFF", 4); u32(36 + bytes); f.write("WAVEfmt ", 8); u32(16); u16(1); u16(2);
		u32(44100); u32(44100 * 4); u16(4); u16(16); f.write("data", 4); u32(bytes);
	}
	f.write(reinterpret_cast<const char *>(pcm.data()), std::streamsize(pcm.size() * 2));
}

std::vector<int16_t> read_raw(const char *path)
{
	std::ifstream f(path, std::ios::binary);
	std::vector<char> b((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
	std::vector<int16_t> v(b.size() / 2);
	std::memcpy(v.data(), b.data(), v.size() * 2);
	return v;
}

int compare(const char *pa, const char *pb)
{
	const auto a = read_raw(pa), b = read_raw(pb);
	const size_t n = std::min(a.size(), b.size());
	double sig = 0, err = 0;
	int peak = 0;
	long long first = -1;
	for (size_t i = 0; i < n; i++)
	{
		const int d = b[i] - a[i];
		if (d && first < 0)
			first = (long long)(i / 2);
		sig += double(a[i]) * a[i];
		err += double(d) * d;
		peak = std::max(peak, std::abs(d));
	}
	std::printf("frames %zu / %zu, first difference at frame %lld, peak error %d, SNR %.1f dB\n",
			a.size() / 2, b.size() / 2, first, peak, err ? 10 * std::log10(sig / err) : INFINITY);
	return (a.size() == b.size() && first < 0) ? 0 : 1;
}

} // anonymous namespace

int main(int argc, char **argv)
{
	if (argc == 4 && std::string(argv[1]) == "--compare")
		return compare(argv[2], argv[3]);
	std::vector<std::string> simm(4);
	long long frames = -1;
	bool filter = true, fx = true, use_float = false, snr = false;
	std::vector<const char *> pos;
	for (int i = 1; i < argc; i++)
	{
		const std::string a = argv[i];
		if (a == "--simm" && i + 1 < argc)
		{
			const std::string s = argv[++i];
			simm[std::clamp(s[0] - '1', 0, 3)] = s.substr(2);
		}
		else if (a == "--frames" && i + 1 < argc)
			frames = std::atoll(argv[++i]);
		else if (a == "--nofilter")
			filter = false;
		else if (a == "--nofx")
			fx = false;
		else if (a == "--float")
			use_float = true;
		else if (a == "--snr")
			snr = true;
		else
			pos.push_back(argv[i]);
	}
	if (pos.size() != 2)
	{
		std::fprintf(stderr, "usage: p2k_render [--simm N=FILE] [--frames N] [--nofilter] [--nofx] [--float] [--snr] SNDLOG OUT\n"
				"       p2k_render --compare A.raw B.raw\n");
		return 2;
	}
	// sound memory: 4 slots of 16 M words (MAME region "simm", zero-filled)
	std::vector<uint16_t> mem(size_t(4) << 24, 0);
	for (int s = 0; s < 4; s++)
	{
		if (simm[s].empty())
			continue;
		std::ifstream f(simm[s], std::ios::binary);
		std::vector<unsigned char> b((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
		const size_t words = std::min<size_t>(b.size() / 2, 1 << 24);
		for (size_t i = 0; i < words; i++)
			mem[(size_t(s) << 24) + i] = uint16_t((b[2 * i] << 8) | b[2 * i + 1]);
	}
	const auto log = read_log(pos[0]);
	if (frames < 0)
		frames = (log.empty() ? 0 : log.back().sample) + 3 * 44100;

	std::unique_ptr<p2k::sound_base> m;
	if (use_float)
		m = std::make_unique<p2k::sound_float>();
	else
		m = std::make_unique<p2k::sound>();
	std::unique_ptr<p2k::sound_float> ref = snr && !use_float ? std::make_unique<p2k::sound_float>() : nullptr;
	for (p2k::sound_base *s : { m.get(), static_cast<p2k::sound_base *>(ref.get()) })
		if (s)
		{
			s->set_memory(mem.data(), uint32_t(mem.size()));
			s->set_options(filter, fx);
		}

	std::vector<int16_t> pcm(size_t(frames) * 2);
	size_t li = 0;
	int peak_active = 0;
	double sum_active = 0;
	double sig = 0, err = 0, peak = 0;
	for (long long n = 0; n < frames; n++)
	{
		while (li < log.size() && log[li].sample <= n)
		{
			apply(*m, log[li]);
			if (ref)
				apply(*ref, log[li]);
			li++;
		}
		int active = 0;                     // voices with a non-zero increment read 2 words a sample
		for (int v = 0; v < p2k::sound_base::VOICES; v++)
			active += m->g20(v * 32 + 2) != 0;
		peak_active = std::max(peak_active, active);
		sum_active += active;
		m->step(pcm[2 * n], pcm[2 * n + 1]);
		if (ref)
		{
			int16_t l, r;
			ref->step(l, r);
			for (int c = 0; c < 2; c++)
			{
				const double fv = std::clamp(ref->m_last[c], -32768.0, 32767.0);
				const double d = pcm[2 * n + c] - fv;
				sig += fv * fv;
				err += d * d;
				peak = std::max(peak, std::fabs(d));
			}
		}
	}
	write_out(pos[1], pcm);
	if (auto *im = dynamic_cast<p2k::sound *>(m.get()))
		std::printf("frames %lld, writes %zu, filter peak state %.1f dB re 1.0 (Q8), saturations %" PRId64,
				frames, log.size(), im->m_peak_state ? 20 * std::log10(im->m_peak_state / 256.0 / 32768.0) : -999.0, im->m_sat_count);
	else
		std::printf("frames %lld, writes %zu (float reference)", frames, log.size());
	std::printf(", active voices peak %d mean %.1f, sample-memory reads peak %d per output sample (%.2f M words/s)",
			peak_active, frames ? sum_active / frames : 0.0, 2 * peak_active, 2 * peak_active * 44100 / 1e6);
	if (ref)
		std::printf(", int vs float: SNR %.1f dB, peak error %.1f, float filter resets %lld", err ? 10 * std::log10(sig / err) : INFINITY, peak, (long long)ref->m_resets);
	std::printf("\n");
	return 0;
}
