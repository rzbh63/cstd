#ifndef _RNG_CPP_HPP_
#define _RNG_CPP_HPP_

#ifdef _WIN32
#include <process.h> // _getpid
#else
#include <unistd.h>
#endif // _WIN32
//#include <direct.h>

#include "std/rand_c.h"

//typedef mt19937ar_t rng_t;

inline rng_t* caffe_rng()
{
  return mt_static();
}
static unsigned int caffe_rng_rand()
{
  return rng_int32(caffe_rng());
}


#if 0
// FisherĘCYates algorithm
template <class RandomAccessIterator, class RandomGenerator>
inline void shuffle(RandomAccessIterator begin, RandomAccessIterator end,
  RandomGenerator* gen)
{
  typedef typename std::iterator_traits<RandomAccessIterator>::difference_type
    difference_type;
  typedef typename std::uniform_int_distribution<difference_type> dist_type;
  difference_type length = std::distance(begin, end);
  if (length <= 0) { return; }
  for (difference_type i = length - 1; i > 0; --i) {
    dist_type dist(0, i);
    std::iter_swap(begin + i, begin + dist(*gen));
  }
}

template <class RandomAccessIterator>
inline void shuffle(RandomAccessIterator begin, RandomAccessIterator end)
{
  shuffle(begin, end, caffe_rng());
}
#endif

// random seeding
static int64 cluster_seedgen(void)
{
  int64 s, seed, pid;
#ifndef _MSC_VER
  FILE* f = fopen("/dev/urandom", "rb");
  if (f && fread(&seed, 1, sizeof(seed), f) == sizeof(seed)) {
    fclose(f);
    return seed;
  }
  LOG(INFO) << "System entropy source not available, "
    "using fallback algorithm to generate seed instead.";
  if (f) {
    fclose(f);
  }
  pid = getpid();
#else
  pid = _getpid();
#endif
  s = time(NULL);
  seed = (((s * 181) * ((pid - 83) * 359)) % 104729);
  seed = ABS(seed);
  return seed;
}


#endif  // CAFFE_RNG_HPP_
