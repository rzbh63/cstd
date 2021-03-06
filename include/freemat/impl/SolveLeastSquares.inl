{
  T *a = (T*)A->data;
  T *b = (T*)B->data;
  T *x = (T*)X->data;

  //	SUBROUTINE ZGELSY( M, N, NRHS, A, LDA, B, LDB, JPVT, RCOND, RANK,
  //     $                   WORK, LWORK, RWORK, INFO )
  //*
  //*  -- LAPACK driver routine (version 3.0) --
  //*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
  //*     Courant Institute, Argonne National Lab, and Rice University
  //*     June 30, 1999
  //*
  //*     .. Scalar Arguments ..
  //	INTEGER            INFO, LDA, LDB, LWORK, M, N, NRHS, RANK
  //	DOUBLE PRECISION   RCOND
  //*     ..
  //*     .. Array Arguments ..
  //	INTEGER            JPVT( * )
  //	DOUBLE PRECISION   RWORK( * )
  //	COMPLEX*16         A( LDA, * ), B( LDB, * ), WORK( * )
  //*     ..
  //*
  //*  Purpose
  //*  =======
  //*
  //*  ZGELSY computes the minimum-norm solution to a real linear least
  //*  squares problem:
  //*      minimize || A * X - B ||
  //*  using a complete orthogonal factorization of A.  A is an M-by-N
  //*  matrix which may be rank-deficient.
  //*
  //*  Several right hand side vectors b and solution vectors x can be
  //*  handled in a single call; they are stored as the columns of the
  //*  M-by-NRHS right hand side matrix B and the N-by-NRHS solution
  //*  matrix X.
  //*
  //*  The routine first computes a QR factorization with column pivoting:
  //*      A * P = Q * [ R11 R12 ]
  //*                  [  0  R22 ]
  //*  with R11 defined as the largest leading submatrix whose estimated
  //*  condition number is less than 1/RCOND.  The order of R11, RANK,
  //*  is the effective rank of A.
  //*
  //*  Then, R22 is considered to be negligible, and R12 is annihilated
  //*  by orthogonal transformations from the right, arriving at the
  //*  complete orthogonal factorization:
  //*     A * P = Q * [ T11 0 ] * Z
  //*                 [  0  0 ]
  //*  The minimum-norm solution is then
  //*     X = P * Z' [ inv(T11)*Q1'*B ]
  //*                [        0       ]
  //*  where Q1 consists of the first RANK columns of Q.
  //*
  //*  This routine is basically identical to the original xGELSX except
  //*  three differences:
  //*    o The call to the subroutine xGEQPF has been substituted by the
  //*      the call to the subroutine xGEQP3. This subroutine is a Blas-3
  //*      version of the QR factorization with column pivoting.
  //*    o Matrix B (the right hand side) is updated with Blas-3.
  //*    o The permutation of matrix B (the right hand side) is faster and
  //*      more simple.
  //*
  //*  Arguments
  //*  =========
  //*
  //*  M       (input) INTEGER
  //*          The number of rows of the matrix A.  M >= 0.

  int M = m;

  //*  N       (input) INTEGER
  //*          The number of columns of the matrix A.  N >= 0.

  int N = n;

  //*  NRHS    (input) INTEGER
  //*          The number of right hand sides, i.e., the number of
  //*          columns of matrices B and X. NRHS >= 0.

  int NRHS = k;

  //*  A       (input/output) COMPLEX*16 array, dimension (LDA,N)
  //*          On entry, the M-by-N matrix A.
  //*          On exit, A has been overwritten by details of its
  //*          complete orthogonal factorization.

  //*  LDA     (input) INTEGER
  //*          The leading dimension of the array A.  LDA >= max(1,M).

  int LDA = m;

  //*  B       (input/output) DOUBLE PRECISION array, dimension (LDB,NRHS)
  //*          On entry, the M-by-NRHS right hand side matrix B.
  //*          On exit, the N-by-NRHS solution matrix X.

  int Bsize = (M > N) ? M : N;
  // This passing convention requires that we copy our source matrix
  // into the destination array with the appropriate padding.
  T* B_ = (T*)malloc(sizeof(T)*Bsize*NRHS*2);

  //*  LDB     (input) INTEGER
  //*          The leading dimension of the array B. LDB >= max(1,M,N).

  int LDB = Bsize;

  //*  JPVT    (input/output) INTEGER array, dimension (N)
  //*          On entry, if JPVT(i) .ne. 0, the i-th column of A is permuted
  //*          to the front of AP, otherwise column i is a Free column.
  //*          On exit, if JPVT(i) = k, then the i-th column of AP
  //*          was the k-th column of A.

  int* JPVT = (int*)malloc(sizeof(int)*N);

  //*  RCOND   (input) DOUBLE PRECISION
  //*          RCOND is used to determine the effective rank of A, which
  //*          is defined as the order of the largest leading triangular
  //*          submatrix R11 in the QR factorization with pivoting of A,
  //*          whose estimated condition number < 1/RCOND.

  T1 RCOND = (T1)Tlamch();
  
  //*  RANK    (output) INTEGER
  //*          The effective rank of A, i.e., the order of the submatrix
  //*          R11.  This is the same as the order of the submatrix T11
  //*          in the complete orthogonal factorization of A.

  int RANK;

  //*  WORK    (workspace/output) COMPLEX*16 array, dimension (LWORK)
  //*          On exit, if INFO = 0, WORK(1) returns the optimal LWORK.

  T WORKSIZE[2];

  //*  LWORK   (input) INTEGER
  //*          The dimension of the array WORK.
  //*          The unblocked strategy requires that:
  //*            LWORK >= MN + MAX( 2*MN, N+1, MN+NRHS )
  //*          where MN = min(M,N).
  //*          The block algorithm requires that:
  //*            LWORK >= MN + MAX( 2*MN, NB*(N+1), MN+MN*NB, MN+NB*NRHS )
  //*          where NB is an upper bound on the blocksize returned
  //*          by ILAENV for the routines ZGEQP3, ZTZRZF, CTZRQF, ZUNMQR,
  //*          and ZUNMRZ.
  //*
  //*          If LWORK = -1, then a workspace query is assumed; the routine
  //*          only calculates the optimal size of the WORK array, returns
  //*          this value as the first entry of the WORK array, and no error
  //*          message related to LWORK is issued by XERBLA.

  int LWORK;

  //*  RWORK   (workspace) DOUBLE PRECISION array, dimension (2*N)

  T1* RWORK = iscomplex ? (T1*)malloc(sizeof(T1)*2*N) : NULL;

  //*  INFO    (output) INTEGER
  //*          = 0: successful exit
  //*          < 0: If INFO = -i, the i-th argument had an illegal value.

  int INFO;
  T* WORK;

  //*  Further Details
  //*  ===============
  //*
  //*  Based on contributions by
  //*    A. Petitet, Computer Science Dept., Univ. of Tenn., Knoxville, USA
  //*    E. Quintana-Orti, Depto. de Informatica, Universidad Jaime I, Spain
  //*    G. Quintana-Orti, Depto. de Informatica, Universidad Jaime I, Spain
  //*
  //*  =====================================================================
  //
  changeStride(B_,2*Bsize,b,2*m,2*m,NRHS, sizeof(T));
  LWORK = -1;
  Tgelsy(&M, &N, &NRHS, a, &LDA, B_, &LDB, JPVT, &RCOND, &RANK, WORKSIZE, &LWORK, RWORK, &INFO);
  LWORK = (int) ((T1*)WORKSIZE)[0];
  WORK = (T*)malloc(sizeof(T)*LWORK);
  Tgelsy(&M, &N, &NRHS, a, &LDA, B_, &LDB, JPVT, &RCOND, &RANK, WORK, &LWORK, RWORK, &INFO);
  // Check the rank...
  if (M > N) {
    // Problem should be overdetermined, rank should be N
    if (RANK < N) {
      //WarningMessage(QString("Matrix is rank deficient to machine precision.  RANK = %1\n").arg(RANK));
    }
  } else if (RANK < M) {
    // Problem should be underderemined, rank should be M
    //WarningMessage(QString("Matrix is rank deficient to machine precision.  RANK = %1\n").arg(RANK));
  }
  changeStride(x,2*n,B_,2*Bsize,2*n,k, sizeof(T));
  free(JPVT);
  if (RWORK) {
    free(RWORK);
  }
  free(WORK);
}
#undef Tgelsy
#undef Tlamch
