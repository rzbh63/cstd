


int CNAME(BLASLONG m, FLOAT alpha_r, FLOAT alpha_i,
	  FLOAT *x, BLASLONG incx, FLOAT *a, FLOAT *buffer){

  BLASLONG i;
  FLOAT *X;

  X = x;

  if (incx != 1) {
    COPY_K(m, x, incx, buffer, 1);
    X = buffer;
  }

  for (i = 0; i < m; i++){
#ifndef LOWER
    if ((X[i * 2 + 0] != ZERO) && (X[i * 2 + 1] != ZERO)) {
      AXPYU_K(i + 1, 0, 0,
	      alpha_r * X[i * 2 + 0] - alpha_i * X[i * 2 + 1],
	      alpha_i * X[i * 2 + 0] + alpha_r * X[i * 2 + 1],
	      X,         1, a, 1, NULL, 0);
    }
    a += (i + 1) * 2;
#else
    if ((X[i * 2 + 0] != ZERO) && (X[i * 2 + 1] != ZERO)) {
      AXPYU_K(m - i, 0, 0,
	      alpha_r * X[i * 2 + 0] - alpha_i * X[i * 2 + 1],
	      alpha_i * X[i * 2 + 0] + alpha_r * X[i * 2 + 1],
	      X + i * 2, 1, a, 1, NULL, 0);
    }
    a += (m - i) * 2;
#endif
    }

  return 0;
}
