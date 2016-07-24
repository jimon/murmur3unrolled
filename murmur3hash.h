
#pragma once

#include <stdint.h>

// -----------------------------------------------------------------------------

// murmur 32 bit hashes for small inputs

uint32_t murmur4 (uint32_t data); // 4 bytes
uint32_t murmur8 (uint64_t data); // 8 bytes
uint32_t murmur16(uint64_t data1, uint64_t data2); // 16 bytes

// -----------------------------------------------------------------------------

#ifdef MURMUR_IMPLEMENTATION

// based on https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.

#if defined(_MSC_VER)
#include <stdlib.h>
#define _ROTL32(x,y) _rotl(x, y)
#else
inline uint32_t _rotl32(uint32_t x, int8_t r) { return (x << r) | (x >> (32 - r)); }
#define _ROTL32(x,y) _rotl32(x, y)
#endif

#ifndef _MURMUR_SEED
#define _MURMUR_SEED 0x5f3759df
#endif

uint32_t murmur4(uint32_t data)
{
	uint32_t h1 = _MURMUR_SEED;
	uint32_t k1 = data;
	k1 *= 0xcc9e2d51; // round
	k1  = _ROTL32(k1, 15);
	k1 *= 0x1b873593;
	h1 ^= k1;
	h1  = _ROTL32(h1, 13);
	h1  = h1 * 5 + 0xe6546b64;
	h1 ^= 4;        // len
	h1 ^= h1 >> 16; // fmix32
	h1 *= 0x85ebca6b;
	h1 ^= h1 >> 13;
	h1 *= 0xc2b2ae35;
	h1 ^= h1 >> 16;
	return h1;
}

uint32_t murmur8(uint64_t data)
{
	uint32_t h1 = _MURMUR_SEED;
	uint32_t k1 = ((uint32_t*)&data)[0];
	k1 *= 0xcc9e2d51; // round
	k1  = _ROTL32(k1, 15);
	k1 *= 0x1b873593;
	h1 ^= k1;
	h1  = _ROTL32(h1, 13);
	h1  = h1 * 5 + 0xe6546b64;
	k1 = ((uint32_t*)&data)[1];
	k1 *= 0xcc9e2d51; // round
	k1  = _ROTL32(k1, 15);
	k1 *= 0x1b873593;
	h1 ^= k1;
	h1  = _ROTL32(h1, 13);
	h1  = h1 * 5 + 0xe6546b64;
	h1 ^= 8; // len
	h1 ^= h1 >> 16; // fmix32
	h1 *= 0x85ebca6b;
	h1 ^= h1 >> 13;
	h1 *= 0xc2b2ae35;
	h1 ^= h1 >> 16;
	return h1;
}

uint32_t murmur16(uint64_t data1, uint64_t data2)
{
	uint32_t h1 = _MURMUR_SEED;
	uint32_t k1 = ((uint32_t*)&data1)[0];
	k1 *= 0xcc9e2d51; // round
	k1  = _ROTL32(k1, 15);
	k1 *= 0x1b873593;
	h1 ^= k1;
	h1  = _ROTL32(h1, 13);
	h1  = h1 * 5 + 0xe6546b64;
	k1 = ((uint32_t*)&data1)[1];
	k1 *= 0xcc9e2d51; // round
	k1  = _ROTL32(k1, 15);
	k1 *= 0x1b873593;
	h1 ^= k1;
	h1  = _ROTL32(h1, 13);
	h1  = h1 * 5 + 0xe6546b64;
	k1 = ((uint32_t*)&data2)[0];
	k1 *= 0xcc9e2d51; // round
	k1  = _ROTL32(k1, 15);
	k1 *= 0x1b873593;
	h1 ^= k1;
	h1  = _ROTL32(h1, 13);
	h1  = h1 * 5 + 0xe6546b64;
	k1 = ((uint32_t*)&data2)[1];
	k1 *= 0xcc9e2d51; // round
	k1  = _ROTL32(k1, 15);
	k1 *= 0x1b873593;
	h1 ^= k1;
	h1  = _ROTL32(h1, 13);
	h1  = h1 * 5 + 0xe6546b64;
	h1 ^= 16; // len
	h1 ^= h1 >> 16; // fmix32
	h1 *= 0x85ebca6b;
	h1 ^= h1 >> 13;
	h1 *= 0xc2b2ae35;
	h1 ^= h1 >> 16;
	return h1;
}

#endif
