#define NN
#define ASMNAME _dgemm_nn
#define ASMFNAME _dgemm_nn_
#define NAME dgemm_nn_
#define CNAME dgemm_nn
#define CHAR_NAME "dgemm_nn_"
#define CHAR_CNAME "dgemm_nn"
#define DOUBLE
#include "blas/OpenBLAS/driver/level3/gemm.c"
#undef DOUBLE
