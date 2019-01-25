
#ifndef _MT19937AR_INL_
#define _MT19937AR_INL_
#if 0
// Mersenne Twister random number generator.
// Inspired by http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/MT2002/CODES/mt19937ar.c
//  A C-program for MT19937, with initialization improved 2002/1/26.
//  Coded by Takuji Nishimura and Makoto Matsumoto.
//  Before using, initialize the state by using init_genrand(seed)
//  or init_by_array(init_key, key_length).
//  Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
//  All rights reserved.
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//  1. Redistributions of source code must retain the above copyright
//  notice, this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright
//  notice, this list of conditions and the following disclaimer in the
//  documentation and/or other materials provided with the distribution.
//  3. The names of its contributors may not be used to endorse or promote
//  products derived from this software without specific prior written
//  permission.
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;LOSS OF USE, DATA, OR
//  PROFITS;OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//  Any feedback is very welcome.
//  http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
//  email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)

#define RNG_MT19937_N  624
#define RNG_MT19937_M  397
typedef struct RNG_MT19937 {
  unsigned state[RNG_MT19937_N];
  int mti;
} RNG_MT19937;

static void RNG_MT19937_seed(RNG_MT19937* g, unsigned s)
{
  unsigned* state = g->state;
  state[0] = s;
  for (g->mti = 1;g->mti < RNG_MT19937_N;g->mti++) {
    /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
    state[g->mti] = (1812433253U * (state[g->mti - 1] ^ (state[g->mti - 1] >> 30)) + g->mti);
  }
}
static unsigned RNG_MT19937_next(RNG_MT19937* g)
{
  /* mag01[x] = x * MATRIX_A for x=0,1 */
  unsigned* state = g->state;
  static unsigned mag01[2] = { 0x0U, /*MATRIX_A*/ 0x9908b0dfU};
  const unsigned UPPER_MASK = 0x80000000U;
  const unsigned LOWER_MASK = 0x7fffffffU;
  enum {N=RNG_MT19937_N, M=RNG_MT19937_M};
  unsigned y;
  /* generate N words at one time */
  if (g->mti >= N) {
    int kk = 0;
    for (;kk < N - M;++kk) {
      y = (state[kk] & UPPER_MASK) | (state[kk + 1] & LOWER_MASK);
      state[kk] = state[kk + M] ^ (y >> 1) ^ mag01[y & 0x1U];
    }
    for (;kk < N - 1;++kk) {
      y = (state[kk] & UPPER_MASK) | (state[kk + 1] & LOWER_MASK);
      state[kk] = state[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1U];
    }
    y = (state[N - 1] & UPPER_MASK) | (state[0] & LOWER_MASK);
    state[N - 1] = state[M - 1] ^ (y >> 1) ^ mag01[y & 0x1U];
    g->mti = 0;
  }
  y = state[g->mti++];
  /* Tempering */
  y ^= (y >> 11);
  y ^= (y << 7) & 0x9d2c5680U;
  y ^= (y << 15) & 0xefc60000U;
  y ^= (y >> 18);
  return y;
}
static float RNG_MT19937_float(RNG_MT19937* g)
{
  return RNG_MT19937_next(g) * (1.f / 4294967296.f);
}
static double RNG_MT19937_double(RNG_MT19937* g)
{
  unsigned a = RNG_MT19937_next(g) >> 5;
  unsigned b = RNG_MT19937_next(g) >> 6;
  return (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
}
// returns uniformly distributed integer random number from [a,b) range
static int RNG_MT19937_uniform_int(RNG_MT19937* g, int a, int b)
{
  return (int)(RNG_MT19937_next(g) % (b - a) + a);
}
// returns uniformly distributed floating-point random number from [a,b) range
static float RNG_MT19937_uniform_float(RNG_MT19937* g, float a, float b)
{
  return RNG_MT19937_float(g) * (b - a) + a;
}
// returns uniformly distributed double-precision floating-point random number from [a,b) range
static double RNG_MT19937_uniform_double(RNG_MT19937* g, double a, double b)
{
  return RNG_MT19937_double(g) * (b - a) + a;
}
#endif


// A C-program for MT19937, with initialization improved 2002/1/26.
// Coded by Takuji Nishimura and Makoto Matsumoto.
//
// Before using, initialize the state by using mt19937ar_init(seed)
// or mt19937ar_init_by_array(init_key, key_length).
//
// Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//   1. Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//
//   2. Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//   3. The names of its contributors may not be used to endorse or promote
//      products derived from this software without specific prior written
//      permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//
// Any feedback is very welcome.
// http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
// email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)


/* Period parameters */
#define MT19937AR_N 624
#define MT19937AR_M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

typedef struct {
  rng_t rng[1];
  int mti;//=MT19937AR_N+1; /* r->mti==N+1 means mt[N] is not initialized */
  unsigned long mt[MT19937AR_N]; /* the array for the state vector  */
} mt19937ar_t;
/* initializes mt[N] with a seed */
static void mt19937ar_init(mt19937ar_t* r, unsigned long s)
{
  unsigned long* mt = r->mt;
  mt[0] = s & 0xffffffffUL;

  for (r->mti = 1; r->mti < MT19937AR_N; r->mti++) {
    mt[r->mti] =
        (1812433253UL * (mt[r->mti - 1] ^ (mt[r->mti - 1] >> 30)) + r->mti);
    /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
    /* In the previous versions, MSBs of the seed affect   */
    /* only MSBs of the array mt[].                        */
    /* 2002/01/09 modified by Makoto Matsumoto             */
    mt[r->mti] &= 0xffffffffUL;
    /* for >32 bit machines */
  }
}

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
/* slight change for C++, 2004/2/26 */
static void mt19937ar_init_by_array(mt19937ar_t* r, unsigned long init_key[], int key_length)
{
  int i, j, k;
  unsigned long* mt = r->mt;
  mt19937ar_init(r, 19650218UL);
  i = 1;
  j = 0;
  k = (MT19937AR_N > key_length ? MT19937AR_N : key_length);

  for (; k; k--) {
    mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 30)) * 1664525UL))
        + init_key[j] + j; /* non linear */
    mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
    i++;
    j++;

    if (i >= MT19937AR_N) {
      mt[0] = mt[MT19937AR_N - 1];
      i = 1;
    }

    if (j >= key_length) {
      j = 0;
    }
  }

  for (k = MT19937AR_N - 1; k; k--) {
    mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 30)) * 1566083941UL))
        - i; /* non linear */
    mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
    i++;

    if (i >= MT19937AR_N) {
      mt[0] = mt[MT19937AR_N - 1];
      i = 1;
    }
  }

  mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */
}

/* generates a random number on [0,0xffffffff]-interval */
static unsigned long mt19937ar_int32(mt19937ar_t* r)
{
  unsigned long y;
  static unsigned long mag01[2] = {0x0UL, MATRIX_A};
  unsigned long* mt = r->mt;
  /* mag01[x] = x * MATRIX_A  for x=0,1 */

  if (r->mti >= MT19937AR_N) { /* generate N words at one time */
    int kk;

    if (r->mti == MT19937AR_N + 1) { /* if mt19937ar_init() has not been called, */
      mt19937ar_init(r, 5489UL);  /* a default initial seed is used */
    }

    for (kk = 0; kk < MT19937AR_N - MT19937AR_M; kk++) {
      y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
      mt[kk] = mt[kk + MT19937AR_M] ^ (y >> 1) ^ mag01[y & 0x1UL];
    }

    for (; kk < MT19937AR_N - 1; kk++) {
      y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
      mt[kk] = mt[kk + (MT19937AR_M - MT19937AR_N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
    }

    y = (mt[MT19937AR_N - 1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
    mt[MT19937AR_N - 1] = mt[MT19937AR_M - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];

    r->mti = 0;
  }

  y = mt[r->mti++];

  /* Tempering */
  y ^= (y >> 11);
  y ^= (y << 7) & 0x9d2c5680UL;
  y ^= (y << 15) & 0xefc60000UL;
  y ^= (y >> 18);

  return y;
}

#endif // _MT19937AR_INL_
