

#include <ctype.h>

#ifdef XDOUBLE
#define ERROR_NAME "XHPR2 "
#elif defined(DOUBLE)
#define ERROR_NAME "ZHPR2 "
#else
#define ERROR_NAME "CHPR2 "
#endif

static int (*hpr2[])(BLASLONG, FLOAT, FLOAT, FLOAT *, BLASLONG, FLOAT *, BLASLONG, FLOAT *, FLOAT *) = {
#ifdef XDOUBLE
  xhpr2_U, xhpr2_L, xhpr2_V, xhpr2_M,
#elif defined(DOUBLE)
  zhpr2_U, zhpr2_L, zhpr2_V, zhpr2_M,
#else
  chpr2_U, chpr2_L, chpr2_V, chpr2_M,
#endif
};

#ifdef SMP
static int (*hpr2_thread[])(BLASLONG, FLOAT *, FLOAT *, BLASLONG, FLOAT *, BLASLONG, FLOAT *, FLOAT *, int) = {
#ifdef XDOUBLE
  xhpr2_thread_U, xhpr2_thread_L, xhpr2_thread_V, xhpr2_thread_M,
#elif defined(DOUBLE)
  zhpr2_thread_U, zhpr2_thread_L, zhpr2_thread_V, zhpr2_thread_M,
#else
  chpr2_thread_U, chpr2_thread_L, chpr2_thread_V, chpr2_thread_M,
#endif
};
#endif

#ifndef CBLAS

void NAME(char *UPLO, blasint *N, FLOAT  *ALPHA,
	 FLOAT  *x, blasint *INCX, FLOAT *y, blasint *INCY, FLOAT *a){

  char uplo_arg = *UPLO;
  blasint n		= *N;
  FLOAT alpha_r	= ALPHA[0];
  FLOAT alpha_i	= ALPHA[1];
  blasint incx	= *INCX;
  blasint incy	= *INCY;

  blasint info;
  int uplo;
  FLOAT *buffer;
#ifdef SMP
  int nthreads;
#endif

  PRINT_DEBUG_NAME;

  TOUPPER(uplo_arg);
  uplo  = -1;

  if (uplo_arg  == 'U') uplo  = 0;
  if (uplo_arg  == 'L') uplo  = 1;

  info = 0;

  if (incy == 0)          info =  7;
  if (incx == 0)          info =  5;
  if (n < 0)              info =  2;
  if (uplo  < 0)          info =  1;

  if (info != 0) {
    BLASFUNC(xerbla)(ERROR_NAME, &info, sizeof(ERROR_NAME));
    return;
  }

#else

void CNAME(enum CBLAS_ORDER order,
	   enum CBLAS_UPLO Uplo,
	   blasint n,
	   FLOAT *ALPHA,
	   FLOAT  *x, blasint incx,
	   FLOAT  *y, blasint incy,
	   FLOAT  *a) {

  FLOAT alpha_r	= ALPHA[0];
  FLOAT alpha_i	= ALPHA[1];
  FLOAT *buffer;
  int uplo;
  blasint info;
#ifdef SMP
  int nthreads;
#endif

  PRINT_DEBUG_CNAME;

  uplo = -1;
  info =  0;

  if (order == CblasColMajor) {
    if (Uplo == CblasUpper)         uplo  = 0;
    if (Uplo == CblasLower)         uplo  = 1;

    info = -1;

    if (incy == 0)          info =  7;
    if (incx == 0)          info =  5;
    if (n < 0)              info =  2;
    if (uplo  < 0)          info =  1;
  }

  if (order == CblasRowMajor) {
    if (Uplo == CblasUpper)         uplo  = 3;
    if (Uplo == CblasLower)         uplo  = 2;

    info = -1;

    if (incx == 0)          info =  7;
    if (incy == 0)          info =  5;
    if (n < 0)              info =  2;
    if (uplo  < 0)          info =  1;
  }

  if (info >= 0) {
    BLASFUNC(xerbla)(ERROR_NAME, &info, sizeof(ERROR_NAME));
    return;
  }

#endif

  if (n == 0) return;

  if ((alpha_r == ZERO) && (alpha_i == ZERO)) return;


  IDEBUG_START;

  FUNCTION_PROFILE_START();

  if (incx < 0 ) x -= (n - 1) * incx * 2;
  if (incy < 0 ) y -= (n - 1) * incy * 2;

  buffer = (FLOAT *)blas_memory_alloc(1);

#ifdef SMP
  nthreads = num_cpu_avail(2);

  if (nthreads == 1) {
#endif

    (hpr2[uplo])(n, alpha_r, alpha_i, x, incx, y, incy, a, buffer);

#ifdef SMP
  } else {

    (hpr2_thread[uplo])(n, ALPHA, x, incx, y, incy, a, buffer, nthreads);

  }
#endif

  blas_memory_free(buffer);

  FUNCTION_PROFILE_END(4, n * n / 2 + 2 * n, 2 * n * n);

  IDEBUG_END;

  return;
}
