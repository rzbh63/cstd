



int CNAME(BLASLONG m, BLASLONG n, FLOAT *a, BLASLONG lda, BLASLONG posX, BLASLONG posY, FLOAT *b){

  BLASLONG i, js, offset;

  FLOAT data01, data02, data03, data04, data05, data06, data07, data08;
  FLOAT *ao1, *ao2, *ao3, *ao4;

  lda *= 2;

  js = (n >> 2);
  while (js > 0){

    offset = posX - posY;

    if (offset >  0) ao1 = a + posY * 2 + (posX + 0) * lda; else ao1 = a + (posX + 0) * 2 + posY * lda;
    if (offset > -1) ao2 = a + posY * 2 + (posX + 1) * lda; else ao2 = a + (posX + 1) * 2 + posY * lda;
    if (offset > -2) ao3 = a + posY * 2 + (posX + 2) * lda; else ao3 = a + (posX + 2) * 2 + posY * lda;
    if (offset > -3) ao4 = a + posY * 2 + (posX + 3) * lda; else ao4 = a + (posX + 3) * 2 + posY * lda;

    i     = m;

    while (i > 0) {
      data01 = *(ao1 + 0);
      data02 = *(ao1 + 1);
      data03 = *(ao2 + 0);
      data04 = *(ao2 + 1);
      data05 = *(ao3 + 0);
      data06 = *(ao3 + 1);
      data07 = *(ao4 + 0);
      data08 = *(ao4 + 1);

      if (offset >   0) ao1 += 2; else ao1 += lda;
      if (offset >  -1) ao2 += 2; else ao2 += lda;
      if (offset >  -2) ao3 += 2; else ao3 += lda;
      if (offset >  -3) ao4 += 2; else ao4 += lda;

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

    posX += 4;
    js --;
  }

  if (n & 2) {

    offset = posX - posY;

    if (offset >  0) ao1 = a + posY * 2 + (posX + 0) * lda; else ao1 = a + (posX + 0) * 2 + posY * lda;
    if (offset > -1) ao2 = a + posY * 2 + (posX + 1) * lda; else ao2 = a + (posX + 1) * 2 + posY * lda;

    i     = m;

    while (i > 0) {
      data01 = *(ao1 + 0);
      data02 = *(ao1 + 1);
      data03 = *(ao2 + 0);
      data04 = *(ao2 + 1);

      if (offset >   0) ao1 += 2; else ao1 += lda;
      if (offset >  -1) ao2 += 2; else ao2 += lda;

      b[ 0] = data01;
      b[ 1] = data02;
      b[ 2] = data03;
      b[ 3] = data04;

      b += 4;

      offset --;
      i --;
    }

    posX += 2;
  }

  if (n & 1) {

    offset = posX - posY;

    if (offset >  0) ao1 = a + posY * 2 + (posX + 0) * lda; else ao1 = a + (posX + 0) * 2 + posY * lda;

    i     = m;

    while (i > 0) {
      data01 = *(ao1 + 0);
      data02 = *(ao1 + 1);

      if (offset >   0) ao1 += 2; else ao1 += lda;

      b[ 0] = data01;
      b[ 1] = data02;

      b += 2;

      offset --;
      i --;
    }

  }

  return 0;
}
