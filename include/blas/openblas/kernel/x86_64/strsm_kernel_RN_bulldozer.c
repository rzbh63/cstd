


static FLOAT dm1 = -1.;

#ifdef CONJ
#define GEMM_KERNEL   GEMM_KERNEL_R
#else
#define GEMM_KERNEL   GEMM_KERNEL_N
#endif

#if GEMM_DEFAULT_UNROLL_M == 1
#define GEMM_UNROLL_M_SHIFT 0
#endif

#if GEMM_DEFAULT_UNROLL_M == 2
#define GEMM_UNROLL_M_SHIFT 1
#endif

#if GEMM_DEFAULT_UNROLL_M == 4
#define GEMM_UNROLL_M_SHIFT 2
#endif

#if GEMM_DEFAULT_UNROLL_M == 6
#define GEMM_UNROLL_M_SHIFT 2
#endif

#if GEMM_DEFAULT_UNROLL_M == 8
#define GEMM_UNROLL_M_SHIFT 3
#endif

#if GEMM_DEFAULT_UNROLL_M == 16
#define GEMM_UNROLL_M_SHIFT 4
#endif

#if GEMM_DEFAULT_UNROLL_N == 1
#define GEMM_UNROLL_N_SHIFT 0
#endif

#if GEMM_DEFAULT_UNROLL_N == 2
#define GEMM_UNROLL_N_SHIFT 1
#endif

#if GEMM_DEFAULT_UNROLL_N == 4
#define GEMM_UNROLL_N_SHIFT 2
#endif

#if GEMM_DEFAULT_UNROLL_N == 8
#define GEMM_UNROLL_N_SHIFT 3
#endif

#if GEMM_DEFAULT_UNROLL_N == 16
#define GEMM_UNROLL_N_SHIFT 4
#endif



static void strsm_RN_solve_opt(BLASLONG n, FLOAT *a, FLOAT *b, FLOAT *c, BLASLONG ldc, FLOAT *as, FLOAT *bs)  __attribute__ ((noinline));

static void strsm_RN_solve_opt(BLASLONG n, FLOAT *a, FLOAT *b, FLOAT *c, BLASLONG ldc, FLOAT *as, FLOAT *bs)
{

	FLOAT *c1 = c + ldc ;
	BLASLONG n1 = n * 8;
	BLASLONG i=0;

        __asm__  __volatile__
        (
	"	vzeroupper							\n\t"
	"	prefetcht0	(%4)						\n\t"
	"	prefetcht0	(%5)						\n\t"
	"	vxorps	%%xmm8 , %%xmm8 , %%xmm8				\n\t"
	"	vxorps	%%xmm9 , %%xmm9 , %%xmm9				\n\t"
	"	vxorps	%%xmm10, %%xmm10, %%xmm10				\n\t"
	"	vxorps	%%xmm11, %%xmm11, %%xmm11				\n\t"
	"	vxorps	%%xmm12, %%xmm12, %%xmm12				\n\t"
	"	vxorps	%%xmm13, %%xmm13, %%xmm13				\n\t"
	"	vxorps	%%xmm14, %%xmm14, %%xmm14				\n\t"
	"	vxorps	%%xmm15, %%xmm15, %%xmm15				\n\t"

	"	cmpq	       $0, %0						\n\t"
	"	je	       2f						\n\t"

	"	.align 16							\n\t"
	"1:									\n\t"

	"	vbroadcastss	(%3,%1,1), %%xmm0				\n\t"	// read b
	"	vmovups         (%2,%1,8), %%xmm4				\n\t"
	"	vbroadcastss   4(%3,%1,1), %%xmm1				\n\t"	
	"	vmovups       16(%2,%1,8), %%xmm5				\n\t"
	"	vmovups       32(%2,%1,8), %%xmm6				\n\t"
	"	vmovups       48(%2,%1,8), %%xmm7				\n\t"

	"	vfmaddps	%%xmm8 , %%xmm0 , %%xmm4 , %%xmm8		\n\t"
	"	vfmaddps	%%xmm12, %%xmm1 , %%xmm4 , %%xmm12		\n\t"
	"	vfmaddps	%%xmm9 , %%xmm0 , %%xmm5 , %%xmm9		\n\t"
	"	vfmaddps	%%xmm13, %%xmm1 , %%xmm5 , %%xmm13		\n\t"
	"	vfmaddps	%%xmm10, %%xmm0 , %%xmm6 , %%xmm10		\n\t"
	"	vfmaddps	%%xmm14, %%xmm1 , %%xmm6 , %%xmm14		\n\t"
	"	addq		$8, %1						\n\t"
	"	vfmaddps	%%xmm11, %%xmm0 , %%xmm7 , %%xmm11		\n\t"
	"	vfmaddps	%%xmm15, %%xmm1 , %%xmm7 , %%xmm15		\n\t"
	"	cmpq		%1, %0						\n\t"

	"	jnz		1b						\n\t"

	"2:									\n\t"


	"	vmovups		  (%4) , %%xmm0					\n\t"
	"	vmovups		16(%4) , %%xmm1					\n\t"
	"	vmovups		32(%4) , %%xmm2					\n\t"
	"	vmovups		48(%4) , %%xmm3					\n\t"

	"	vmovups		  (%5) , %%xmm4					\n\t"
	"	vmovups		16(%5) , %%xmm5					\n\t"
	"	vmovups		32(%5) , %%xmm6					\n\t"
	"	vmovups		48(%5) , %%xmm7					\n\t"

	"	vsubps		%%xmm8 , %%xmm0 , %%xmm8			\n\t"
	"	vsubps		%%xmm9 , %%xmm1 , %%xmm9			\n\t"
	"	vsubps		%%xmm10, %%xmm2 , %%xmm10			\n\t"
	"	vsubps		%%xmm11, %%xmm3 , %%xmm11			\n\t"

	"	vsubps		%%xmm12, %%xmm4 , %%xmm12			\n\t"
	"	vsubps		%%xmm13, %%xmm5 , %%xmm13			\n\t"
	"	vsubps		%%xmm14, %%xmm6 , %%xmm14			\n\t"
	"	vsubps		%%xmm15, %%xmm7 , %%xmm15			\n\t"

	"3:									\n\t"	// i = 0

	"	vbroadcastss	(%7), %%xmm0					\n\t"	// read bb
	"	vbroadcastss   4(%7), %%xmm1					\n\t"	// read b

	"	vmulps		%%xmm8  ,  %%xmm0 ,  %%xmm8 			\n\t"	// aa * bb 
	"	vmulps		%%xmm9  ,  %%xmm0 ,  %%xmm9 			\n\t"
	"	vmulps		%%xmm10 ,  %%xmm0 ,  %%xmm10			\n\t"
	"	vmulps		%%xmm11 ,  %%xmm0 ,  %%xmm11			\n\t"

	"	vmovups		%%xmm8  ,    (%6)				\n\t"	// write a
	"	vmovups		%%xmm9  ,  16(%6)				\n\t"
	"	vmovups		%%xmm10 ,  32(%6)				\n\t"
	"	vmovups		%%xmm11 ,  48(%6)				\n\t"

	"	vmovups		%%xmm8  ,    (%4)				\n\t"	// write c0
	"	vmovups		%%xmm9  ,  16(%4)				\n\t"
	"	vmovups		%%xmm10 ,  32(%4)				\n\t"
	"	vmovups		%%xmm11 ,  48(%4)				\n\t"

	"	vfnmaddps	%%xmm12 ,  %%xmm8  , %%xmm1 , %%xmm12		\n\t"  // c = c - aa * b 
	"	vfnmaddps	%%xmm13 ,  %%xmm9  , %%xmm1 , %%xmm13		\n\t"   
	"	vfnmaddps	%%xmm14 ,  %%xmm10 , %%xmm1 , %%xmm14		\n\t"   
	"	vfnmaddps	%%xmm15 ,  %%xmm11 , %%xmm1 , %%xmm15		\n\t"   

	"									\n\t" // i = 1
	"	addq		$8  , %7					\n\t" // b = b + 2
	"	addq	       $64  , %6					\n\t" // a = a + 16

	"	vbroadcastss      4(%7), %%xmm0					\n\t"	// read bb

	"	vmulps		%%xmm12 ,  %%xmm0 ,  %%xmm12			\n\t"	// aa * bb 
	"	vmulps		%%xmm13 ,  %%xmm0 ,  %%xmm13			\n\t"	// aa * bb 
	"	vmulps		%%xmm14 ,  %%xmm0 ,  %%xmm14			\n\t"	// aa * bb 
	"	vmulps		%%xmm15 ,  %%xmm0 ,  %%xmm15			\n\t"	// aa * bb 

	"	vmovups		%%xmm12 ,    (%6)				\n\t"	// write a
	"	vmovups		%%xmm13 ,  16(%6)				\n\t"	// write a
	"	vmovups		%%xmm14 ,  32(%6)				\n\t"	// write a
	"	vmovups		%%xmm15 ,  48(%6)				\n\t"	// write a

	"	vmovups		%%xmm12 ,    (%5)				\n\t"	// write c1
	"	vmovups		%%xmm13 ,  16(%5)				\n\t"	
	"	vmovups		%%xmm14 ,  32(%5)				\n\t"	
	"	vmovups		%%xmm15 ,  48(%5)				\n\t"	

	"	vzeroupper							\n\t"

        :
        :
          "r" (n1),     // 0    
          "a" (i),      // 1    
          "r" (a),      // 2
          "r" (b),      // 3
          "r" (c),      // 4
          "r" (c1),     // 5
          "r" (as),     // 6
          "r" (bs)      // 7
        : "cc",
          "%xmm0", "%xmm1", "%xmm2", "%xmm3",
          "%xmm4", "%xmm5", "%xmm6", "%xmm7",
          "%xmm8", "%xmm9", "%xmm10", "%xmm11",
          "%xmm12", "%xmm13", "%xmm14", "%xmm15",
          "memory"
        );

}



#ifndef COMPLEX

static inline void solve(BLASLONG m, BLASLONG n, FLOAT *a, FLOAT *b, FLOAT *c, BLASLONG ldc) {

  FLOAT aa, bb;

  int i, j, k;

  for (i = 0; i < n; i++) {

    bb = *(b + i);

    for (j = 0; j < m; j ++) {
      aa = *(c + j + i * ldc);
      aa *= bb;
      *a  = aa;
      *(c + j + i * ldc) = aa;
      a ++;

      for (k = i + 1; k < n; k ++){
	*(c + j + k * ldc) -= aa * *(b + k);
      }

    }
    b += n;
  }
}

#else

static inline void solve(BLASLONG m, BLASLONG n, FLOAT *a, FLOAT *b, FLOAT *c, BLASLONG ldc) {

  FLOAT aa1, aa2;
  FLOAT bb1, bb2;
  FLOAT cc1, cc2;

  int i, j, k;

  ldc *= 2;

  for (i = 0; i < n; i++) {

    bb1 = *(b + i * 2 + 0);
    bb2 = *(b + i * 2 + 1);

    for (j = 0; j < m; j ++) {
      aa1 = *(c + j * 2 + 0 + i * ldc);
      aa2 = *(c + j * 2 + 1 + i * ldc);

#ifndef CONJ
      cc1 = aa1 * bb1 - aa2 * bb2;
      cc2 = aa1 * bb2 + aa2 * bb1;
#else
      cc1 =  aa1 * bb1 + aa2 * bb2;
      cc2 = -aa1 * bb2 + aa2 * bb1;
#endif

      *(a + 0) = cc1;
      *(a + 1) = cc2;
      *(c + j * 2 + 0 + i * ldc) = cc1;
      *(c + j * 2 + 1 + i * ldc) = cc2;
      a += 2;

      for (k = i + 1; k < n; k ++){
#ifndef CONJ
	*(c + j * 2 + 0 + k * ldc) -= cc1 * *(b + k * 2 + 0) - cc2 * *(b + k * 2 + 1);
	*(c + j * 2 + 1 + k * ldc) -= cc1 * *(b + k * 2 + 1) + cc2 * *(b + k * 2 + 0);
#else
	*(c + j * 2 + 0 + k * ldc) -=   cc1 * *(b + k * 2 + 0) + cc2 * *(b + k * 2 + 1);
	*(c + j * 2 + 1 + k * ldc) -= - cc1 * *(b + k * 2 + 1) + cc2 * *(b + k * 2 + 0);
#endif
      }

    }
    b += n * 2;
  }
}

#endif


int CNAME(BLASLONG m, BLASLONG n, BLASLONG k, FLOAT dummy1,
#ifdef COMPLEX
	   FLOAT dummy2,
#endif
	   FLOAT *a, FLOAT *b, FLOAT *c, BLASLONG ldc, BLASLONG offset){

  FLOAT *aa, *cc;
  BLASLONG  kk;
  BLASLONG i, j, jj;

#if 0
  fprintf(stderr, "TRSM RN KERNEL m = %3ld  n = %3ld  k = %3ld offset = %3ld\n",
	  m, n, k, offset);
#endif

  jj = 0;
  j = (n >> GEMM_UNROLL_N_SHIFT);
  kk = -offset;

  while (j > 0) {

    aa = a;
    cc = c;

    i = (m >> GEMM_UNROLL_M_SHIFT);

    if (i > 0) {
      do {

	strsm_RN_solve_opt(kk, aa, b, cc, ldc, aa + kk * GEMM_UNROLL_M * COMPSIZE, b + kk * GEMM_UNROLL_N * COMPSIZE);


	aa += GEMM_UNROLL_M * k * COMPSIZE;
	cc += GEMM_UNROLL_M     * COMPSIZE;
	i --;
      } while (i > 0);
    }


    if (m & (GEMM_UNROLL_M - 1)) {
      i = (GEMM_UNROLL_M >> 1);
      while (i > 0) {
	if (m & i) {
	    if (kk > 0) {
	      GEMM_KERNEL(i, GEMM_UNROLL_N, kk, dm1,
#ifdef COMPLEX
			  ZERO,
#endif
			  aa, b, cc, ldc);
	    }
	  solve(i, GEMM_UNROLL_N,
		aa + kk * i             * COMPSIZE,
		b  + kk * GEMM_UNROLL_N * COMPSIZE,
		cc, ldc);

	  aa += i * k * COMPSIZE;
	  cc += i     * COMPSIZE;
	}
	i >>= 1;
      }
    }

    kk += GEMM_UNROLL_N;
    b += GEMM_UNROLL_N * k   * COMPSIZE;
    c += GEMM_UNROLL_N * ldc * COMPSIZE;
    j --;
    jj += GEMM_UNROLL_M;
  }

  if (n & (GEMM_UNROLL_N - 1)) {

    j = (GEMM_UNROLL_N >> 1);
    while (j > 0) {
      if (n & j) {

	aa = a;
	cc = c;

	i = (m >> GEMM_UNROLL_M_SHIFT);

	while (i > 0) {
	  if (kk > 0) {
	    GEMM_KERNEL(GEMM_UNROLL_M, j, kk, dm1,
#ifdef COMPLEX
			ZERO,
#endif
			aa,
			b,
			cc,
			ldc);
	  }

	  solve(GEMM_UNROLL_M, j,
		aa + kk * GEMM_UNROLL_M * COMPSIZE,
		b  + kk * j             * COMPSIZE, cc, ldc);

	  aa += GEMM_UNROLL_M * k * COMPSIZE;
	  cc += GEMM_UNROLL_M     * COMPSIZE;
	  i --;
	}

	if (m & (GEMM_UNROLL_M - 1)) {
	  i = (GEMM_UNROLL_M >> 1);
	  while (i > 0) {
	    if (m & i) {
	      if (kk > 0) {
		GEMM_KERNEL(i, j, kk, dm1,
#ifdef COMPLEX
			    ZERO,
#endif
			    aa,
			    b,
			    cc,
			    ldc);
	      }

	      solve(i, j,
		    aa + kk * i * COMPSIZE,
		    b  + kk * j * COMPSIZE, cc, ldc);

	      aa += i * k * COMPSIZE;
	      cc += i     * COMPSIZE;
	      }
	    i >>= 1;
	  }
	}

	b += j * k   * COMPSIZE;
	c += j * ldc * COMPSIZE;
	kk += j;
      }
      j >>= 1;
    }
  }

  return 0;
}
