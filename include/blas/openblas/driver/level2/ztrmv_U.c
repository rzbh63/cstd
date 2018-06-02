

#include <ctype.h>


static FLOAT dp1 = 1.;

int CNAME(BLASLONG m, FLOAT *a, BLASLONG lda, FLOAT *b, BLASLONG incb, FLOAT *buffer){

  BLASLONG i, is, min_i;
#if (TRANSA == 2) || (TRANSA == 4)
  OPENBLAS_COMPLEX_FLOAT temp;
#endif
#ifndef UNIT
  FLOAT atemp1, atemp2, btemp1, btemp2;
#endif
  FLOAT *gemvbuffer = (FLOAT *)buffer;
  FLOAT *B = b;

  if (incb != 1) {
    B = buffer;
    gemvbuffer = (FLOAT *)(((BLASLONG)buffer + m * sizeof(FLOAT) * 2 + 15) & ~15);
    COPY_K(m, b, incb, buffer, 1);
  }

  for (is =0; is < m; is += DTB_ENTRIES){

    min_i = MIN(m - is, DTB_ENTRIES);

#if (TRANSA) == 1 || (TRANSA == 3)
    if (is > 0){
#if   TRANSA == 1
      GEMV_N(is, min_i, 0, dp1, ZERO,
	     a + is * lda * 2, lda,
	     B + is       * 2, 1,
	     B,                1, gemvbuffer);
#else
      GEMV_R(is, min_i, 0, dp1, ZERO,
	     a + is * lda * 2, lda,
	     B + is       * 2, 1,
	     B,                1, gemvbuffer);
#endif
    }
#endif

    for (i = 0; i < min_i; i++) {
      FLOAT *AA = a + (is + (i + is) * lda) * 2;
      FLOAT *BB = B + is * 2;

#if (TRANSA == 1) || (TRANSA == 3)
#if   TRANSA == 1
      if (i > 0) AXPYU_K (i, 0, 0, BB[i * 2 + 0],  BB[i * 2 + 1],
			  AA, 1, BB, 1, NULL, 0);
#else
      if (i > 0) AXPYC_K(i, 0, 0, BB[i * 2 + 0],  BB[i * 2 + 1],
			  AA, 1, BB, 1, NULL, 0);
#endif
#endif

#ifndef UNIT
      atemp1 = AA[i * 2 + 0];
      atemp2 = AA[i * 2 + 1];

      btemp1 = BB[i * 2 + 0];
      btemp2 = BB[i * 2 + 1];

#if (TRANSA == 1) || (TRANSA == 2)
      BB[i * 2 + 0] = atemp1 * btemp1 - atemp2 * btemp2;
      BB[i * 2 + 1] = atemp1 * btemp2 + atemp2 * btemp1;
#else
      BB[i * 2 + 0] = atemp1 * btemp1 + atemp2 * btemp2;
      BB[i * 2 + 1] = atemp1 * btemp2 - atemp2 * btemp1;
#endif
#endif

#if (TRANSA == 2) || (TRANSA == 4)
      if (i < min_i - 1) {
#if TRANSA == 2
	temp = DOTU_K(min_i - i - 1,
			  AA + (i + 1) * 2, 1,
			  BB + (i + 1) * 2, 1);
#else
	temp = DOTC_K(min_i - i - 1,
			  AA + (i + 1) * 2, 1,
			  BB + (i + 1) * 2, 1);
#endif

      BB[i * 2 + 0] += CREAL(temp);
      BB[i * 2 + 1] += CIMAG(temp);
      }
#endif

    }

#if (TRANSA) == 2 || (TRANSA == 4)
    if (m - is > min_i){
#if TRANSA == 2
      GEMV_T(m - is - min_i, min_i, 0, dp1, ZERO,
	      a + (is + min_i + is * lda) * 2,  lda,
	      B + (is + min_i) * 2, 1,
	      B +  is            * 2, 1, gemvbuffer);
#else
      GEMV_C(m - is - min_i, min_i, 0, dp1, ZERO,
	      a + (is + min_i + is * lda) * 2,  lda,
	      B + (is + min_i) * 2, 1,
	      B +  is            * 2, 1, gemvbuffer);
#endif
    }
#endif
  }

  if (incb != 1) {
    COPY_K(m, buffer, 1, b, incb);
  }

  return 0;
}

