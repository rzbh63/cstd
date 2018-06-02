


int CNAME(BLASLONG m, FLOAT alpha, FLOAT *x,
		      BLASLONG incx, FLOAT *a, BLASLONG lda, FLOAT *buffer){

  BLASLONG i;
  FLOAT *X;

  X = x;
  lda *= 2;

  if (incx != 1) {
    COPY_K(m, x, incx, buffer, 1);
    X = buffer;
  }

  for (i = 0; i < m; i++){
#ifndef HEMVREV
#ifndef LOWER
    AXPYU_K(i + 1, 0, 0, alpha * X[i * 2 + 0], -alpha * X[i * 2 + 1], X,         1, a, 1, NULL, 0);
    a[i * 2 + 1] = ZERO;
    a += lda;
#else
    AXPYU_K(m - i, 0, 0, alpha * X[i * 2 + 0], -alpha * X[i * 2 + 1], X + i * 2, 1, a, 1, NULL, 0);
    a[1] = ZERO;
    a += 2 + lda;
#endif
#else
#ifndef LOWER
    AXPYC_K(i + 1, 0, 0, alpha * X[i * 2 + 0], alpha * X[i * 2 + 1], X,         1, a, 1, NULL, 0);
    a[i * 2 + 1] = ZERO;
    a += lda;
#else
    AXPYC_K(m - i, 0, 0, alpha * X[i * 2 + 0], alpha * X[i * 2 + 1], X + i * 2, 1, a, 1, NULL, 0);
    a[1] = ZERO;
    a += 2 + lda;
#endif
#endif
    }

  return 0;
}
