



#ifndef USE_ALPHA
#define REAL_PART(a, b)  (a)
#define IMAGE_PART(a, b) (b)
#else
#define REAL_PART(a, b)  (alpha_r * (a) - alpha_i * (b))
#define IMAGE_PART(a, b) (alpha_i * (a) + alpha_r * (b))
#endif

#if defined(REAL_ONLY)
#define CMULT(a, b) (REAL_PART(a, b))
#elif defined(IMAGE_ONLY)
#define CMULT(a, b) (IMAGE_PART(a, b))
#else
#define CMULT(a, b) (REAL_PART(a, b) + IMAGE_PART(a, b))
#endif

int CNAME(BLASLONG m, BLASLONG n, FLOAT *a, BLASLONG lda, BLASLONG posX, BLASLONG posY,
#ifdef USE_ALPHA
	   FLOAT alpha_r, FLOAT alpha_i,
#endif
	   FLOAT *b){

  lda *= 2;

  BLASLONG i, js, offset;

  FLOAT data01, data02;
  FLOAT *ao1, *ao2;

  js = (n >> 1);
  while (js > 0){

    offset = posX - posY;

    if (offset >  0) ao1 = a + (posX + 0) * 2 + posY * lda; else ao1 = a + posY * 2 + (posX + 0) * lda;
    if (offset > -1) ao2 = a + (posX + 1) * 2 + posY * lda; else ao2 = a + posY * 2 + (posX + 1) * lda;

    i     = m;

    while (i > 0) {
      data01 = CMULT(*(ao1 + 0), *(ao1 + 1));
      data02 = CMULT(*(ao2 + 0), *(ao2 + 1));

      if (offset >   0) ao1 += lda; else ao1 += 2;
      if (offset >  -1) ao2 += lda; else ao2 += 2;

      b[ 0] = data01;
      b[ 1] = data02;

      b += 2;

      offset --;
      i --;
    }

    posX += 2;
    js --;
  }

  if (n & 1) {

    offset = posX - posY;

    if (offset >  0) ao1 = a + (posX + 0) * 2 + posY * lda; else ao1 = a + posY * 2 + (posX + 0) * lda;

    i     = m;

    while (i > 0) {
      data01 = CMULT(*(ao1 + 0), *(ao1 + 1));

      if (offset >   0) ao1 += lda; else ao1 += 2;

      b[ 0] = data01;

      b ++;

      offset --;
      i --;
    }
  }

  return 0;
}
