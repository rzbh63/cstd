

/***************************************************************************
 * Linear equation solver for real matrices
 ***************************************************************************/

template <typename T>
static void Tgesvx(char* FACT, char* TRANS, int * N, int * NRHS, 
		   T *A, int * LDA, T * AF, int * LDAF, 
		   int * IPIV, char * EQUED, T * R, T * C, 
		   T * B, int * LDB, T * X, int * LDX, 
		   T * RCOND, T * FERR, T * BERR,
		   T * WORK, int * IWORK, int * INFO, 
		   ftnlen l1, ftnlen l2, ftnlen l3);

template <>
void Tgesvx(char* FACT, char* TRANS, int * N, int * NRHS, 
	    float *A, int * LDA, float * AF, int * LDAF, 
	    int * IPIV, char * EQUED, float * R, float * C, 
	    float * B, int * LDB, float * X, int * LDX, 
	    float * RCOND, float * FERR, float * BERR,
	    float * WORK, int * IWORK, int * INFO,
	    ftnlen l1, ftnlen l2, ftnlen l3) {
  return sgesvx_(FACT,TRANS,N,NRHS,A,LDA,AF,LDAF,IPIV,EQUED,R,C,B,
		 LDB,X,LDX,RCOND,FERR,BERR,WORK,IWORK,INFO,l1,l2,l3);
}

template <>
void Tgesvx(char* FACT, char* TRANS, int * N, int * NRHS, 
		   double *A, int * LDA, double * AF, int * LDAF, 
		   int * IPIV, char * EQUED, double * R, double * C, 
		   double * B, int * LDB, double * X, int * LDX, 
		   double * RCOND, double * FERR, double * BERR,
	    double * WORK, int * IWORK, int * INFO, ftnlen l1, ftnlen l2, ftnlen l3) {
  return dgesvx_(FACT,TRANS,N,NRHS,A,LDA,AF,LDAF,IPIV,EQUED,R,C,B,
		 LDB,X,LDX,RCOND,FERR,BERR,WORK,IWORK,INFO,l1,l2,3);
}

template <typename T>
static void Tgesvx(char* FACT, char* TRANS, int * N, int * NRHS, 
		   T *A, int * LDA, T * AF, int * LDAF, 
		   int * IPIV, char * EQUED, T * R, T * C, 
		   T * B, int * LDB, T * X, int * LDX, 
		   T * RCOND, T * FERR, T * BERR,
		   T * WORK, T * RWORK, int * INFO, ftnlen l1, ftnlen l2, ftnlen l3);

template <>
void Tgesvx(char* FACT, char* TRANS, int * N, int * NRHS, 
		   float *A, int * LDA, float * AF, int * LDAF, 
		   int * IPIV, char * EQUED, float * R, float * C, 
		   float * B, int * LDB, float * X, int * LDX, 
		   float * RCOND, float * FERR, float * BERR,
		   float * WORK, float * RWORK, int * INFO, ftnlen l1, ftnlen l2, ftnlen l3) {
  return cgesvx_(FACT, TRANS, N, NRHS, A, LDA, AF, LDAF, IPIV, EQUED, R, C, B,
		 LDB, X, LDX, RCOND, FERR, BERR, WORK, RWORK, INFO, l1, l2, l3);
}

template <>
void Tgesvx(char* FACT, char* TRANS, int * N, int * NRHS, 
		   double *A, int * LDA, double * AF, int * LDAF, 
		   int * IPIV, char * EQUED, double * R, double * C, 
		   double * B, int * LDB, double * X, int * LDX, 
		   double * RCOND, double * FERR, double * BERR,
		   double * WORK, double * RWORK, int * INFO, ftnlen l1, ftnlen l2, ftnlen l3) {
  return zgesvx_(FACT, TRANS, N, NRHS, A, LDA, AF, LDAF, IPIV, EQUED, R, C, B,
		 LDB, X, LDX, RCOND, FERR, BERR, WORK, RWORK, INFO, l1, l2, l3);
}

// Solve A*C = B, where A is m x m, and B is m x n, all quantities are real.
template <typename T>
static void realSolveLinEq(int m, int n, T *c, T* a, T *b) {
  if ((m == 0) || (n == 0)) return;
}

// Solve A*C = B, where A is m x m, and B is m x n, all quantities are real.

template <typename T>
static void complexSolveLinEq(int m, int n, T *c, T* a, T* b) {
  if ((m == 0) || (n == 0)) return;
  // Here are the comments from the LAPACK routine used:
  //SUBROUTINE ZGESVX( FACT, TRANS, N, NRHS, A, LDA, AF, LDAF, IPIV,
  //$                   EQUED, R, C, B, LDB, X, LDX, RCOND, FERR, BERR,
  //$                   WORK, IWORK, INFO )
  //*
  //*  -- LAPACK driver routine (version 3.0) --
  //*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
  //*     Courant Institute, Argonne National Lab, and Rice University
  //*     June 30, 1999
  //*
  //*     .. Scalar Arguments ..
  //	CHARACTER          EQUED, FACT, TRANS
  //	INTEGER            INFO, LDA, LDAF, LDB, LDX, N, NRHS
  //	DOUBLE PRECISION   RCOND
  //*     ..
  //*     .. mxArray Arguments ..
  //      INTEGER            IPIV( * )
  //      DOUBLE PRECISION   BERR( * ), C( * ), FERR( * ), R( * ),
  //     $                   RWORK( * )
  //      COMPLEX*16         A( LDA, * ), AF( LDAF, * ), B( LDB, * ),
  //     $                   WORK( * ), X( LDX, * )
  //*     ..
  //*
  //*  Purpose
  //*  =======
  //*
  //*  ZGESVX uses the LU factorization to compute the solution to a real
  //*  system of linear equations
  //*     A * X = B,
  //*  where A is an N-by-N matrix and X and B are N-by-NRHS matrices.
  //*
  //*  Error bounds on the solution and a condition estimate are also
  //*  provided.
  //*
  //*  Description
  //*  ===========
  //*
  //*  The following steps are performed:
  //*
  //*  1. If FACT = 'E', real scaling factors are computed to equilibrate
  //*     the system:
  //*        TRANS = 'N':  diag(R)*A*diag(C)     *inv(diag(C))*X = diag(R)*B
  //*        TRANS = 'T': (diag(R)*A*diag(C))**T *inv(diag(R))*X = diag(C)*B
  //*        TRANS = 'C': (diag(R)*A*diag(C))**H *inv(diag(R))*X = diag(C)*B
  //*     Whether or not the system will be equilibrated depends on the
  //*     scaling of the matrix A, but if equilibration is used, A is
  //*     overwritten by diag(R)*A*diag(C) and B by diag(R)*B (if TRANS='N')
  //*     or diag(C)*B (if TRANS = 'T' or 'C').
  //*
  //*  2. If FACT = 'N' or 'E', the LU decomposition is used to factor the
  //*     matrix A (after equilibration if FACT = 'E') as
  //*        A = P * L * U,
  //*     where P is a permutation matrix, L is a unit lower triangular
  //*     matrix, and U is upper triangular.
  //*
  //*  3. If some U(i,i)=0, so that U is exactly singular, then the routine
  //*     returns with INFO = i. Otherwise, the factored form of A is used
  //*     to estimate the condition number of the matrix A.  If the
  //*     reciprocal of the condition number is less than machine precision,
  //*     INFO = N+1 is returned as a warning, but the routine still goes on
  //*     to solve for X and compute error bounds as described below.
  //*
  //*  4. The system of equations is solved for X using the factored form
  //*     of A.
  //*
  //*  5. Iterative refinement is applied to improve the computed solution
  //*     matrix and calculate error bounds and backward error estimates
  //*     for it.
  //*
  //*  6. If equilibration was used, the matrix X is premultiplied by
  //*     diag(C) (if TRANS = 'N') or diag(R) (if TRANS = 'T' or 'C') so
  //*     that it solves the original system before equilibration.
  //*
  //*  Arguments
  //*  =========
  //*
  //*  FACT    (input) CHARACTER*1
  //*          Specifies whether or not the factored form of the matrix A is
  //*          supplied on entry, and if not, whether the matrix A should be
  //*          equilibrated before it is factored.
  //*          = 'F':  On entry, AF and IPIV contain the factored form of A.
  //*                  If EQUED is not 'N', the matrix A has been
  //*                  equilibrated with scaling factors given by R and C.
  //*                  A, AF, and IPIV are not modified.
  //*          = 'N':  The matrix A will be copied to AF and factored.
  //*          = 'E':  The matrix A will be equilibrated if necessary, then
  //*                  copied to AF and factored.

  char FACT = 'E';

  //*
  //*  TRANS   (input) CHARACTER*1
  //*          Specifies the form of the system of equations:
  //*          = 'N':  A * X = B     (No transpose)
  //*          = 'T':  A**T * X = B  (Transpose)
  //*          = 'C':  A**H * X = B  (Transpose)

  char TRANS = 'N';

  //*
  //*  N       (input) INTEGER
  //*          The number of linear equations, i.e., the order of the
  //*          matrix A.  N >= 0.
  //*

  int N = m;

  //*  NRHS    (input) INTEGER
  //*          The number of right hand sides, i.e., the number of columns
  //*          of the matrices B and X.  NRHS >= 0.

  int NRHS = n;
  
  //*  A       (input/output) COMPLEX*16 array, dimension (LDA,N)
  //*          On entry, the N-by-N matrix A.  If FACT = 'F' and EQUED is
  //*          not 'N', then A must have been equilibrated by the scaling
  //*          factors in R and/or C.  A is not modified if FACT = 'F' or
  //*          'N', or if FACT = 'E' and EQUED = 'N' on exit.
  //*
  //*          On exit, if EQUED .ne. 'N', A is scaled as follows:
  //*          EQUED = 'R':  A := diag(R) * A
  //*          EQUED = 'C':  A := A * diag(C)
  //*          EQUED = 'B':  A := diag(R) * A * diag(C).
  
  T* A = a;

  //*  LDA     (input) INTEGER
  //*          The leading dimension of the array A.  LDA >= max(1,N).
  //*

  int LDA = m;

  //*  AF      (input or output) COMPLEX*16 array, dimension (LDAF,N)
  //*          If FACT = 'F', then AF is an input argument and on entry
  //*          contains the factors L and U from the factorization
  //*          A = P*L*U as computed by DGETRF.  If EQUED .ne. 'N', then
  //*          AF is the factored form of the equilibrated matrix A.
  //*
  //*          If FACT = 'N', then AF is an output argument and on exit
  //*          returns the factors L and U from the factorization A = P*L*U
  //*          of the original matrix A.
  //*
  //*          If FACT = 'E', then AF is an output argument and on exit
  //*          returns the factors L and U from the factorization A = P*L*U
  //*          of the equilibrated matrix A (see the description of A for
  //*          the form of the equilibrated matrix).

  MemBlock<T> AF(2*LDA*N);

  //*  LDAF    (input) INTEGER
  //*          The leading dimension of the array AF.  LDAF >= max(1,N).
  //*

  int LDAF = m;

  //*  IPIV    (input or output) INTEGER array, dimension (N)
  //*          If FACT = 'F', then IPIV is an input argument and on entry
  //*          contains the pivot indices from the factorization A = P*L*U
  //*          as computed by DGETRF; row i of the matrix was interchanged
  //*          with row IPIV(i).
  //*
  //*          If FACT = 'N', then IPIV is an output argument and on exit
  //*          contains the pivot indices from the factorization A = P*L*U
  //*          of the original matrix A.
  //*
  //*          If FACT = 'E', then IPIV is an output argument and on exit
  //*          contains the pivot indices from the factorization A = P*L*U
  //*          of the equilibrated matrix A.

  MemBlock<int> IPIV(N);

  //*  EQUED   (input or output) CHARACTER*1
  //*          Specifies the form of equilibration that was done.
  //*          = 'N':  No equilibration (always true if FACT = 'N').
  //*          = 'R':  Row equilibration, i.e., A has been premultiplied by
  //*                  diag(R).
  //*          = 'C':  Column equilibration, i.e., A has been postmultiplied
  //*                  by diag(C).
  //*          = 'B':  Both row and column equilibration, i.e., A has been
  //*                  replaced by diag(R) * A * diag(C).
  //*          EQUED is an input argument if FACT = 'F'; otherwise, it is an
  //*          output argument.
  
  char EQUED;

  //*  R       (input or output) DOUBLE PRECISION array, dimension (N)
  //*          The row scale factors for A.  If EQUED = 'R' or 'B', A is
  //*          multiplied on the left by diag(R); if EQUED = 'N' or 'C', R
  //*          is not accessed.  R is an input argument if FACT = 'F';
  //*          otherwise, R is an output argument.  If FACT = 'F' and
  //*          EQUED = 'R' or 'B', each element of R must be positive.

  MemBlock<T> R(N);

  //*  C       (input or output) DOUBLE PRECISION array, dimension (N)
  //*          The column scale factors for A.  If EQUED = 'C' or 'B', A is
  //*          multiplied on the right by diag(C); if EQUED = 'N' or 'R', C
  //*          is not accessed.  C is an input argument if FACT = 'F';
  //*          otherwise, C is an output argument.  If FACT = 'F' and
  //*          EQUED = 'C' or 'B', each element of C must be positive.
  
  MemBlock<T> C(N);

  //*  B       (input/output) COMPLEX*16 array, dimension (LDB,NRHS)
  //*          On entry, the N-by-NRHS right hand side matrix B.
  //*          On exit,
  //*          if EQUED = 'N', B is not modified;
  //*          if TRANS = 'N' and EQUED = 'R' or 'B', B is overwritten by
  //*          diag(R)*B;
  //*          if TRANS = 'T' or 'C' and EQUED = 'C' or 'B', B is
  //*          overwritten by diag(C)*B.

  T *B = b;

  //*  LDB     (input) INTEGER
  //*          The leading dimension of the array B.  LDB >= max(1,N).

  int LDB = m;

  //*  X       (output) COMPLEX*16 array, dimension (LDX,NRHS)
  //*          If INFO = 0 or INFO = N+1, the N-by-NRHS solution matrix X
  //*          to the original system of equations.  Note that A and B are
  //*          modified on exit if EQUED .ne. 'N', and the solution to the
  //*          equilibrated system is inv(diag(C))*X if TRANS = 'N' and
  //*          EQUED = 'C' or 'B', or inv(diag(R))*X if TRANS = 'T' or 'C'
  //*          and EQUED = 'R' or 'B'.
  
  T *X = c;

  //*  LDX     (input) INTEGER
  //*          The leading dimension of the array X.  LDX >= max(1,N).
  
  int LDX = m;

  //*  RCOND   (output) DOUBLE PRECISION
  //*          The estimate of the reciprocal condition number of the matrix
  //*          A after equilibration (if done).  If RCOND is less than the
  //*          machine precision (in particular, if RCOND = 0), the matrix
  //*          is singular to working precision.  This condition is
  //*          indicated by a return code of INFO > 0.

  T RCOND;
  
  //*  FERR    (output) DOUBLE PRECISION array, dimension (NRHS)
  //*          The estimated forward error bound for each solution vector
  //*          X(j) (the j-th column of the solution matrix X).
  //*          If XTRUE is the true solution corresponding to X(j), FERR(j)
  //*          is an estimated upper bound for the magnitude of the largest
  //*          element in (X(j) - XTRUE) divided by the magnitude of the
  //*          largest element in X(j).  The estimate is as reliable as
  //*          the estimate for RCOND, and is almost always a slight
  //*          overestimate of the true error.

  MemBlock<T> FERR(n);

  //*  BERR    (output) DOUBLE PRECISION array, dimension (NRHS)
  //*          The componentwise relative backward error of each solution
  //*          vector X(j) (i.e., the smallest relative change in
  //*          any element of A or B that makes X(j) an exact solution).

  MemBlock<T> BERR(n);

  //*  WORK    (workspace) COMPLEX*16 array, dimension (2*N)

  MemBlock<T> WORK(2*(2*N));
  
  //*  RWORK   (workspace/output) DOUBLE PRECISION array, dimension (2*N)
  //*          On exit, RWORK(1) contains the reciprocal pivot growth
  //*          factor norm(A)/norm(U). The "max absolute element" norm is
  //*          used. If RWORK(1) is much less than 1, then the stability
  //*          of the LU factorization of the (equilibrated) matrix A
  //*          could be poor. This also means that the solution X, condition
  //*          estimator RCOND, and forward error bound FERR could be
  //*          unreliable. If factorization fails with 0<INFO<=N, then
  //*          RWORK(1) contains the reciprocal pivot growth factor for the
  //*          leading INFO columns of A.

  MemBlock<T> RWORK(2*N);

  //*  INFO    (output) INTEGER
  //*          = 0:  successful exit
  //*          < 0:  if INFO = -i, the i-th argument had an illegal value
  //*          > 0:  if INFO = i, and i is
  //*                <= N:  U(i,i) is exactly zero.  The factorization has
  //*                       been completed, but the factor U is exactly
  //*                       singular, so the solution and error bounds
  //*                       could not be computed. RCOND = 0 is returned.
  //*                = N+1: U is nonsingular, but RCOND is less than machine
  //*                       precision, meaning that the matrix is singular
  //*                       to working precision.  Nevertheless, the
  //*                       solution and error bounds are computed because
  //*                       there are a number of situations where the
  //*                       computed solution can be more accurate than the
  //*                       value of RCOND would suggest.
  //*
  //*  =====================================================================

  int INFO;

  Tgesvx(&FACT, &TRANS, &N, &NRHS, A, &LDA, &AF, &LDAF, &IPIV, &EQUED, &R, &C, B,
	 &LDB, X, &LDX, &RCOND, &FERR, &BERR, &WORK, &RWORK, &INFO, 1, 1, 1);
  if ((INFO == N) || (INFO == N+1) || (RCOND < lamch<T>())) {
    WarningMessage(QString("Matrix is singular to working precision.  RCOND = %1\n").arg(RCOND));
  }
}

template <typename T>
static mxArray SolveLinearEqReal(BasicArray<T> A, BasicArray<T> B) {
  BasicArray<T> C(NTuple(A.rows(),B.cols()));
  realSolveLinEq(int(A.rows()),int(B.cols()),C.data(),A.data(),B.data());
  return mxArray(C);
}

template <typename T>
static mxArray SolveLinearEqComplex(BasicArray<T> A, BasicArray<T> B) {
  BasicArray<T> C(NTuple(A.rows(),B.cols()));
  complexSolveLinEq(int(A.rows()/2),int(B.cols()),C.data(),A.data(),B.data());
  return mxArray(SplitReal<T>(C),SplitImag<T>(C));
}

// Only double precision currently supported by umfpack
mxArray SparseSolveLinEq(const SparseMatrix<double> &A, const BasicArray<double> &B) {
#if HAVE_UMFPACK
  QVector<uint32> rowindx, colstart;
  QVector<double> accsdata;
  SparseToCCS(A,rowindx,colstart,accsdata);
  double *null = (double *) NULL ;
  void *Symbolic, *Numeric ;
  (void) umfpack_di_symbolic (int(A.cols()), int(A.cols()), (const int*) colstart.data(), 
			       (const int*) rowindx.data(), accsdata.data(), 
			      &Symbolic, null, null);
  (void) umfpack_di_numeric ( (const int*) colstart.data(),  (const int*) rowindx.data(), 
			      accsdata.data(), Symbolic, 
			     &Numeric, null, null) ;
  umfpack_di_free_symbolic (&Symbolic) ;
  BasicArray<double> x(NTuple(A.rows(),B.cols()));
  for (index_t i=1;i<=B.cols();i++) {
    (void) umfpack_di_solve (UMFPACK_A, (const int*) colstart.data(), 
			     (const int*) rowindx.data(), 
			     accsdata.data(), x.data()+int((i-1)*A.rows()), 
			     B.constData()+int((i-1)*B.rows()), 
			     Numeric, null, null) ;
  }
  umfpack_di_free_numeric (&Numeric);
  return mxArray(x);
#else
  throw Exception("Solving sparse systems of linear equations requires UMFPACK support, which was not available at compile time.  You must have UMFPACK installed at compile time for FreeMat to enable this functionality.");
#endif
}

mxArray SparseSolveLinEq(const SparseMatrix<double> &Ar, const SparseMatrix<double> &Ai,
		      const BasicArray<double> &Br, const BasicArray<double> &Bi) {
#if HAVE_UMFPACK
  QVector<uint32> rowindx, colstart;
  QVector<double> accsreal, accsimag;
  SparseToCCS(Ar,Ai,rowindx,colstart,accsreal,accsimag);
  double *null = (double *) NULL ;
  void *Symbolic, *Numeric ;
  (void) umfpack_zi_symbolic (int(Ar.cols()), int(Ar.cols()), 
			      (const int*) colstart.data(), 
			      (const int*) rowindx.data(), 
			      accsreal.data(), accsimag.data(), &Symbolic, null, null);
  (void) umfpack_zi_numeric ((const int*) colstart.data(), (const int*) rowindx.data(), 
			     accsreal.data(), accsimag.data(),
			     Symbolic, &Numeric, null, null) ;
  umfpack_zi_free_symbolic (&Symbolic) ;
  BasicArray<double> xr(NTuple(Ar.rows(),Br.cols()));
  BasicArray<double> xi(NTuple(Ar.rows(),Br.cols()));
  for (index_t i=1;i<=Br.cols();i++) {
    (void) umfpack_zi_solve (UMFPACK_A, (const int*) colstart.data(), 
			     (const int*) rowindx.data(), 
			     accsreal.data(), accsimag.data(), 
			     xr.data()+int((i-1)*Ar.rows()),
			     xi.data()+int((i-1)*Ar.rows()),
			     Br.constData()+int((i-1)*Br.rows()),
			     Bi.constData()+int((i-1)*Br.rows()),
			     Numeric, null, null) ;
  }
  umfpack_zi_free_numeric (&Numeric);
  return mxArray(xr,xi);  
#else
  throw Exception("Solving sparse systems of linear equations requires UMFPACK support, which was not available at compile time.  You must have UMFPACK installed at compile time for FreeMat to enable this functionality.");  
#endif
}
