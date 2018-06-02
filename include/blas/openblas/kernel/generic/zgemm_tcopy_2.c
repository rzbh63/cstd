



int CNAME(BLASLONG m, BLASLONG n, FLOAT *a, BLASLONG lda, FLOAT *b){

  BLASLONG i, j;
  FLOAT *a_offset, *a_offset1, *a_offset2;
  FLOAT *b_offset, *b_offset1, *b_offset2;
  FLOAT  ctemp1,  ctemp2,  ctemp3,  ctemp4;
  FLOAT  ctemp5,  ctemp6,  ctemp7,  ctemp8;
  FLOAT  ctemp9, ctemp10, ctemp11, ctemp12;
  FLOAT ctemp13, ctemp14, ctemp15, ctemp16;

  a_offset = a;
  b_offset = b;

  b_offset2 = b + m * (n & ~1) * 2;

  lda *= 2;

  j = (m >> 1);
  if (j > 0){
    do{
      a_offset1 = a_offset;
      a_offset2 = a_offset1 + lda;
      a_offset += 2 * lda;

      b_offset1 = b_offset;
      b_offset += 8;

      i = (n >> 2);
      if (i > 0){
	do{
	  ctemp1  = *(a_offset1 +  0);
	  ctemp2  = *(a_offset1 +  1);
	  ctemp3  = *(a_offset1 +  2);
	  ctemp4  = *(a_offset1 +  3);

	  ctemp5  = *(a_offset1 +  4);
	  ctemp6  = *(a_offset1 +  5);
	  ctemp7  = *(a_offset1 +  6);
	  ctemp8  = *(a_offset1 +  7);

	  ctemp9  = *(a_offset2 +  0);
	  ctemp10 = *(a_offset2 +  1);
	  ctemp11 = *(a_offset2 +  2);
	  ctemp12 = *(a_offset2 +  3);

	  ctemp13 = *(a_offset2 +  4);
	  ctemp14 = *(a_offset2 +  5);
	  ctemp15 = *(a_offset2 +  6);
	  ctemp16 = *(a_offset2 +  7);

	  *(b_offset1 + 0) = ctemp1;
	  *(b_offset1 + 1) = ctemp2;
	  *(b_offset1 + 2) = ctemp3;
	  *(b_offset1 + 3) = ctemp4;

	  *(b_offset1 + 4) = ctemp9;
	  *(b_offset1 + 5) = ctemp10;
	  *(b_offset1 + 6) = ctemp11;
	  *(b_offset1 + 7) = ctemp12;

	  b_offset1 += m * 4;

	  *(b_offset1 + 0) = ctemp5;
	  *(b_offset1 + 1) = ctemp6;
	  *(b_offset1 + 2) = ctemp7;
	  *(b_offset1 + 3) = ctemp8;

	  *(b_offset1 + 4) = ctemp13;
	  *(b_offset1 + 5) = ctemp14;
	  *(b_offset1 + 6) = ctemp15;
	  *(b_offset1 + 7) = ctemp16;

	  b_offset1 += m * 4;

	  a_offset1 += 8;
	  a_offset2 += 8;
	  i --;
	} while(i>0);
      }

      if (n & 2){
	ctemp1  = *(a_offset1 +  0);
	ctemp2  = *(a_offset1 +  1);
	ctemp3  = *(a_offset1 +  2);
	ctemp4  = *(a_offset1 +  3);

	ctemp9  = *(a_offset2 +  0);
	ctemp10 = *(a_offset2 +  1);
	ctemp11 = *(a_offset2 +  2);
	ctemp12 = *(a_offset2 +  3);

	*(b_offset1 + 0) = ctemp1;
	*(b_offset1 + 1) = ctemp2;
	*(b_offset1 + 2) = ctemp3;
	*(b_offset1 + 3) = ctemp4;

	*(b_offset1 + 4) = ctemp9;
	*(b_offset1 + 5) = ctemp10;
	*(b_offset1 + 6) = ctemp11;
	*(b_offset1 + 7) = ctemp12;

	b_offset1 += m * 4;
	a_offset1 += 4;
	a_offset2 += 4;
      }

      if (n & 1){
	ctemp1  = *(a_offset1 +  0);
	ctemp2  = *(a_offset1 +  1);
	ctemp9  = *(a_offset2 +  0);
	ctemp10 = *(a_offset2 +  1);

	*(b_offset2 + 0) = ctemp1;
	*(b_offset2 + 1) = ctemp2;
	*(b_offset2 + 2) = ctemp9;
	*(b_offset2 + 3) = ctemp10;
	b_offset2 += 4;
      }
      j--;
    } while(j > 0);
  }

  if (m & 1){
    i = (n >> 2);
    if (i > 0){
      do{
	ctemp1  = *(a_offset +  0);
	ctemp2  = *(a_offset +  1);
	ctemp3  = *(a_offset +  2);
	ctemp4  = *(a_offset +  3);

	ctemp5  = *(a_offset +  4);
	ctemp6  = *(a_offset +  5);
	ctemp7  = *(a_offset +  6);
	ctemp8  = *(a_offset +  7);

	*(b_offset + 0) = ctemp1;
	*(b_offset + 1) = ctemp2;
	*(b_offset + 2) = ctemp3;
	*(b_offset + 3) = ctemp4;

	b_offset += m * 4;

	*(b_offset + 0) = ctemp5;
	*(b_offset + 1) = ctemp6;
	*(b_offset + 2) = ctemp7;
	*(b_offset + 3) = ctemp8;

	b_offset += m * 4;
	a_offset += 8;
	i --;
      } while(i > 0);
    }

    if (n & 2){
      ctemp1  = *(a_offset +  0);
      ctemp2  = *(a_offset +  1);
      ctemp3  = *(a_offset +  2);
      ctemp4  = *(a_offset +  3);

      *(b_offset + 0) = ctemp1;
      *(b_offset + 1) = ctemp2;
      *(b_offset + 2) = ctemp3;
      *(b_offset + 3) = ctemp4;

      b_offset += m * 4;
      a_offset += 4;
    }

    if (n & 1){
      ctemp1  = *(a_offset +  0);
      ctemp2  = *(a_offset +  1);
      *(b_offset2 + 0) = ctemp1;
      *(b_offset2 + 1) = ctemp2;
    }
  }

  return 0;
}
