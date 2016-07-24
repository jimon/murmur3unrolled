//#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stddef.h>
#include <time.h>

#define MURMUR_IMPLEMENTATION
#include "murmur3hash.h"

#define _ROTL32(x,r) ((x << r) | (x >> (32 - r)))

uint32_t murmur16_loop(const uint32_t * data)
{
	uint32_t h1 = _MURMUR_SEED;
	uint32_t k1;

	for (size_t i = 0; i < 4; ++i)
	{
		k1 = data[i];
		k1 *= 0xcc9e2d51; // round
		k1  = _ROTL32(k1, 15);
		k1 *= 0x1b873593;
		h1 ^= k1;
		h1  = _ROTL32(h1, 13);
		h1  = h1 * 5 + 0xe6546b64;
	}

	h1 ^= 16; // len
	h1 ^= h1 >> 16; // fmix32
	h1 *= 0x85ebca6b;
	h1 ^= h1 >> 13;
	h1 *= 0xc2b2ae35;
	h1 ^= h1 >> 16;

	return h1;
}

// values in nanoseconds
uint64_t timer()
{
	struct timespec ts;
	if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
	{
		return 0;
	}

	return (uint64_t)ts.tv_sec * 1000000000 + (uint64_t)ts.tv_nsec;
}

static double duration_in_s(uint64_t start, uint64_t end)
{
	return (double)(end - start) / 1.0e9;
}

typedef uint32_t testfunc_t(uint32_t niter);

const uint32_t data[] = { 0xDEADCAFE, 0xBABEBABE, 0xDEADC0DE, 0x0BADC0DE };

uint32_t unrolled(uint32_t niter)
{
	uint32_t h = _MURMUR_SEED;
	for (size_t i = 0; i < niter; ++i)
	{
		h ^= murmur16(data);
	}
	return h;
}

uint32_t loop(uint32_t niter)
{
	uint32_t h = _MURMUR_SEED;
	for (size_t i = 0; i < niter; ++i)
	{
		h ^= murmur16_loop(data);
	}
	return h;
}

void microbench(const char *name, testfunc_t *fn)
{
	int niter = 1000000000; // 1 billion

	uint32_t h = 0;

	// warm up
	h = fn(niter / 4 + 1);
	printf("%08x\n", h);

	uint64_t startt = timer();
	h = fn(niter + 1);
	uint64_t endt = timer();
	printf("%08x\n", h);
	double secs = duration_in_s(startt, endt);
	double clock_rate_ghz = 3.8;
	double ns_per_iter = (secs * 1e9) / (double)niter;
	double clocks_per_iter = ns_per_iter * clock_rate_ghz;

	printf("%s: %.2f ns/iter (=%.2f cycles @ %.2fGHz)\n", name, ns_per_iter, clocks_per_iter, clock_rate_ghz);
}

int main()
{
	microbench("unrolled", unrolled);
	microbench("loop", loop);
	return 0;
}