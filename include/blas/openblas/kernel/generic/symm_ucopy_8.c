



int CNAME(BLASLONG m, BLASLONG n, FLOAT *a, BLASLONG lda, BLASLONG posX, BLASLONG posY, FLOAT *b){

  BLASLONG i, js, offset;

  FLOAT data01, data02, data03, data04, data05, data06, data07, data08;
  FLOAT *ao1, *ao2, *ao3, *ao4, *ao5, *ao6, *ao7, *ao8;

  js = (n >> 3);
  while (js > 0){

    offset = posX - posY;

    if (offset >  0) ao1 = a + posY + (posX + 0) * lda; else ao1 = a + posX + 0 + posY * lda;
    if (offset > -1) ao2 = a + posY + (posX + 1) * lda; else ao2 = a + posX + 1 + posY * lda;
    if (offset > -2) ao3 = a + posY + (posX + 2) * lda; else ao3 = a + posX + 2 + posY * lda;
    if (offset > -3) ao4 = a + posY + (posX + 3) * lda; else ao4 = a + posX + 3 + posY * lda;
    if (offset > -4) ao5 = a + posY + (posX + 4) * lda; else ao5 = a + posX + 4 + posY * lda;
    if (offset > -5) ao6 = a + posY + (posX + 5) * lda; else ao6 = a + posX + 5 + posY * lda;
    if (offset > -6) ao7 = a + posY + (posX + 6) * lda; else ao7 = a + posX + 6 + posY * lda;
    if (offset > -7) ao8 = a + posY + (posX + 7) * lda; else ao8 = a + posX + 7 + posY * lda;

    i     = m;

    while (i > 0) {
      data01 = *(ao1 + 0);
      data02 = *(ao2 + 0);
      data03 = *(ao3 + 0);
      data04 = *(ao4 + 0);
      data05 = *(ao5 + 0);
      data06 = *(ao6 + 0);
      data07 = *(ao7 + 0);
      data08 = *(ao8 + 0);

      if (offset >   0) ao1 ++; else ao1 += lda;
      if (offset >  -1) ao2 ++; else ao2 += lda;
      if (offset >  -2) ao3 ++; else ao3 += lda;
      if (offset >  -3) ao4 ++; else ao4 += lda;
      if (offset >  -4) ao5 ++; else ao5 += lda;
      if (offset >  -5) ao6 ++; else ao6 += lda;
      if (offset >  -6) ao7 ++; else ao7 += lda;
      if (offset >  -7) ao8 ++; else ao8 += lda;

      b[ 0] = data01;
      b[ 1] = data02;
      b[ 2] = data03;
      b[ 3] = data04;
      b[ 4] = data05;
      b[ 5] = data06;
      b[ 6] = data07;
      b[ 7] = data08;

      b += 8;

      offset --;
      i --;
    }

    posX += 8;
    js --;
  }

  if (n & 4) {

    offset = posX - posY;

    if (offset >  0) ao1 = a + posY + (posX + 0) * lda; else ao1 = a + posX + 0 + posY * lda;
    if (offset > -1) ao2 = a + posY + (posX + 1) * lda; else ao2 = a + posX + 1 + posY * lda;
    if (offset > -2) ao3 = a + posY + (posX + 2) * lda; else ao3 = a + posX + 2 + posY * lda;
    if (offset > -3) ao4 = a + posY + (posX + 3) * lda; else ao4 = a + posX + 3 + posY * lda;

    i     = m;

    while (i > 0) {
      data01 = *(ao1 + 0);
      data02 = *(ao2 + 0);
      data03 = *(ao3 + 0);
      data04 = *(ao4 + 0);

      if (offset >   0) ao1 ++; else ao1 += lda;
      if (offset >  -1) ao2 ++; else ao2 += lda;
      if (offset >  -2) ao3 ++; else ao3 += lda;
      if (offset >  -3) ao4 ++; else ao4 += lda;

      b[ 0] = data01;
      b[ 1] = data02;
      b[ 2] = data03;
      b[ 3] = data04;

      b += 4;

      offset --;
      i --;
    }

    posX += 4;
  }

  if (n & 2) {
    offset = posX - posY;

    if (offset >  0) ao1 = a + posY + (posX + 0) * lda; else ao1 = a + posX + 0 + posY * lda;
    if (offset > -1) ao2 = a + posY + (posX + 1) * lda; else ao2 = a + posX + 1 + posY * lda;

    i     = m;

    while (i > 0) {
      data01 = *(ao1 + 0);
      data02 = *(ao2 + 0);

      if (offset >   0) ao1 ++; else ao1 += lda;
      if (offset >  -1) ao2 ++; else ao2 += lda;

      b[ 0] = data01;
      b[ 1] = data02;

      b += 2;

      offset --;
      i --;
    }

    posX += 2;
  }

  if (n & 1) {
    offset = posX - posY;

    if (offset >  0) ao1 = a + posY + (posX + 0) * lda; else ao1 = a + posX + 0 + posY * lda;

    i     = m;

    while (i > 0) {
      data01 = *(ao1 + 0);

      if (offset >   0) ao1 ++; else ao1 += lda;

      b[ 0] = data01;

      b ++;

      offset --;
      i --;
    }
  }

  return 0;
}
