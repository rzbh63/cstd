
#ifndef _CPU_HPP_
#define _CPU_HPP_

#include "config.h"
#include "rng.hpp"
#include <ctime>
using namespace std;

#define NOT_IMPLEMENTED
enum Brew { CPU, GPU };

static void set_mode(enum Brew brew) {}

#define CPU_KERNEL_LOOP(_i, _n)  for (int _i = 0; _i < (_n); ++_i )

//Context* context[2] = {0};

typedef float(*f32_10_t)[1000];

struct Buffer {
  int size;
  Brew brew;
  union {
    void* data;
    float* fl;
    double* db;
    int* i;
    char* str;
    unsigned char* bytes;
    f32_10_t f100_;
  };
};


#define NOT_IMPL {(0);}
#ifdef CPU_ONLY
//#include "cpu_only.hpp"
//typedef Buffer Context;
//typedef GPUContext Context;
//struct GPUContext : public Buffer {};
static void gpu_Memset(Buffer* ptr, size_t nbytes) NOT_IMPL;
static void gpu_Memcpy(Buffer* dst, const Buffer* src, int nbytes) NOT_IMPL;
static void gpu_ReAlloc(Buffer* ptr, size_t nbytes) NOT_IMPL;
static void gpu_Free(Buffer* ptr) NOT_IMPL;
static void gpu_set_random_seed(const unsigned int seed) NOT_IMPL;
#else
#ifdef USE_CUDA
#include "cuda.hpp"
#define gpu_Free cuda_Free
#define gpu_Memcpy cuda_Memcpy
#define gpu_Memset cuda_Memset
#define gpu_ReAlloc cuda_ReAlloc
#define gpu_set_random_seed cuda_set_random_seed
#endif
#ifdef USE_OCL
#include "ocl.hpp"
#define gpu_Free ocl_Free
#define gpu_Memcpy ocl_Memcpy
#define gpu_Memset ocl_Memset
#define gpu_ReAlloc ocl_ReAlloc
#define gpu_set_random_seed ocl_set_random_seed
#endif
#endif

#define CPUFUN(NAME)  cpu_##NAME

static void set_random_seed(const unsigned int seed) {
  srand(seed);
  gpu_set_random_seed(seed);
}
//Buffer() { context[CPU] = this;    }
static void Memcpy(Buffer* dst, const Buffer* src, int nbytes) {
  if (GPU == dst->brew || GPU == src->brew) {
    gpu_Memcpy(dst, src, nbytes);
    return ;
  }
  CHECK_LE(nbytes, dst->size);
  CHECK_LE(nbytes, dst->size);
  memcpy(dst->data, src->data, nbytes);
}
static void Free(Buffer* ptr) {
  if (ptr->data) {
    if (GPU == ptr->brew) {
      gpu_Free(ptr);
      return;
    }
    free(ptr->data);
    ptr->data = NULL;
    ptr->size = 0;
  }
}
static void cpu_ReAlloc(Buffer* ptr, size_t nbytes) {
  ptr->brew = CPU;
  if (ptr->size<nbytes) {
    ptr->size = (int)nbytes;
    ptr->data = realloc(ptr->data, nbytes);
  }
}
static void ReAlloc(Brew brew, Buffer* ptr, size_t nbytes) {
  if (ptr->data) {
    assert(brew==ptr->brew);
  }
  if (GPU == brew) {
    gpu_ReAlloc(ptr, nbytes);
    return;
  }
  cpu_ReAlloc(ptr, nbytes);
  return ;
}
static void cpu_Memset(Buffer* ptr, size_t nbytes) {
  CHECK_LE(nbytes, ptr->size);
  memset(ptr->data, 0, nbytes);
}
static void Memset(Buffer* ptr, size_t nbytes) {
  if (GPU == ptr->brew) {
    gpu_Memset(ptr, nbytes);
    return;
  }
  cpu_Memset(ptr, nbytes);
  return ;
}
#if 0
static void MemcpyAsync(size_t nbytes, Buffer* dst, const void* src, void* stream) { memcpy(dst, src, nbytes); }
//static Buffer* BeginStream(Buffer* ptr) { return 0; }
//static void Synchronize(Buffer* ptr);
//static void EndStream(Buffer* ptr) {}
#endif


//#define mode()  globel.mode_
struct Globel
{
  //SHARED_PTR<RNG> random_generator_;
  Brew mode_;
  // Parallel training
  int solver_count_;
  int solver_rank_;
  bool multiprocess_;
  Globel() {
    Globel* s = this;
    //s->mode_ = (CPU);
    s->solver_count_ = (1);
    s->solver_rank_ = (0);
    s->multiprocess_ = (false);
  }
  ~Globel() {}
  static Globel& Get() {
    static Globel globel_;
    return globel_;
  }
};
#define globel Globel::Get()
#define mode()  globel.mode_

inline static int solver_count() { return globel.solver_count_; }
inline static void set_solver_count(int val) { globel.solver_count_ = val; }
inline static int solver_rank() { return globel.solver_rank_; }
inline static void set_solver_rank(int val) { globel.solver_rank_ = val; }
inline static bool multiprocess() { return globel.multiprocess_; }
inline static void set_multiprocess(bool val) { globel.multiprocess_ = val; }
inline static bool root_solver() { return globel.solver_rank_ == 0; }

#ifndef CPU_ONLY
#define BREW GPU
#else
#define BREW CPU
#endif


#endif // _CPU_HPP_
