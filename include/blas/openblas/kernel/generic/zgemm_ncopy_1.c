



int CNAME(BLASLONG m, BLASLONG n, FLOAT *a, BLASLONG lda, FLOAT *b){
  BLASLONG i, j;

  FLOAT *a_offset;
  FLOAT *b_offset;
  FLOAT  ctemp1,  ctemp2,  ctemp3,  ctemp4;
  FLOAT  ctemp5,  ctemp6,  ctemp7,  ctemp8;

  a_offset = a;
  b_offset = b;

  lda *= 2;

  i = n;

  if (i > 0){
    do {

      j = (m >> 2);
      if (j > 0){
	do{
	  ctemp1  = *(a_offset + 0);
	  ctemp2  = *(a_offset + 1);
	  ctemp3  = *(a_offset + 2);
	  ctemp4  = *(a_offset + 3);

	  ctemp5  = *(a_offset + 4);
	  ctemp6  = *(a_offset + 5);
	  ctemp7  = *(a_offset + 6);
	  ctemp8  = *(a_offset + 7);

	  *(b_offset + 0) = ctemp1;
	  *(b_offset + 1) = ctemp2;
	  *(b_offset + 2) = ctemp3;
	  *(b_offset + 3) = ctemp4;

	  *(b_offset + 4) = ctemp5;
	  *(b_offset + 5) = ctemp6;
	  *(b_offset + 6) = ctemp7;
	  *(b_offset + 7) = ctemp8;

	  a_offset += 8;
	  b_offset += 8;
	  j --;
	} while(j>0);
      }

      j = (m & 3);
      if (j > 0){
	do{
	  ctemp1  = *(a_offset + 0);
	  ctemp2  = *(a_offset + 1);
	  *(b_offset + 0) = ctemp1;
	  *(b_offset + 1) = ctemp2;
	  a_offset += 2;
	  b_offset += 2;
	  j --;
	} while(j>0);
      }
      a_offset += lda - m * 2;
      i--;
    } while (i > 0);
  }

  return 0;
}
