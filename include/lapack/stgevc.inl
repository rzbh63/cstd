/* ../../../dependencies/lapack/src/stgevc.f -- translated by f2c (version 20061008).
   You must link the resulting object file with libf2c:
        on Microsoft Windows system, link with libf2c.lib;
        on Linux or Unix systems, link with .../path/to/libf2c.a -lm
        or, if you install libf2c.a in a standard place, with -lf2c -lm
        -- in that order, at the end of the command line, as in
                cc *.o -lf2c -lm
        Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

                http://www.netlib.org/f2c/libf2c.zip
*/



/* Table of constant values */

static logical c_true = TRUE_;
static integer c__2 = 2;
static real c_b35 = 1.f;
static integer c__1 = 1;
static real c_b37 = 0.f;
static logical c_false = FALSE_;

/* Subroutine */ static int stgevc_(char *side, char *howmny, logical *select, 
        integer *n, real *a, integer *lda, real *b, integer *ldb, real *vl, 
        integer *ldvl, real *vr, integer *ldvr, integer *mm, integer *m, real 
        *work, integer *info, ftnlen side_len, ftnlen howmny_len)
{
    /* System generated locals */
    integer a_dim1, a_offset, b_dim1, b_offset, vl_dim1, vl_offset, vr_dim1, 
            vr_offset, i__1, i__2, i__3, i__4, i__5;
    real r__1, r__2, r__3, r__4, r__5, r__6;

    /* Local variables */
    static integer i__, j, ja, jc, je, na, im, jr, jw, nw;
    static real big;
    static logical lsa, lsb;
    static real ulp, sum[4]        /* was [2][2] */;
    static integer ibeg, ieig, iend;
    static real dmin__, temp, suma[4]        /* was [2][2] */, sumb[4]        /* 
            was [2][2] */, xmax, cim2a, cim2b, cre2a, cre2b;
     /* Subroutine */ int slag2_(real *, integer *, real *, integer *, 
            real *, real *, real *, real *, real *, real *);
    static real temp2, bdiag[2], acoef, scale;
    static logical ilall;
    static integer iside;
    static real sbeta;
     logical lsame_(char *, char *, ftnlen, ftnlen);
    static logical il2by2;
    static integer iinfo;
    static real small1;
    static logical compl;
    static real anorm, bnorm;
    static logical compr;
     /* Subroutine */ int sgemv_(char *, integer *, integer *, real *, 
            real *, integer *, real *, integer *, real *, real *, integer *, 
            ftnlen), slaln2_(logical *, integer *, integer *, real *, real *, 
            real *, integer *, real *, real *, real *, integer *, real *, 
            real *, real *, integer *, real *, real *, integer *);
    static real temp2i, temp2r;
    static logical ilabad, ilbbad;
    static real acoefa, bcoefa, cimaga, cimagb;
    static logical ilback;
     /* Subroutine */ int slabad_(real *, real *);
    static real bcoefi, ascale, bscale, creala, crealb, bcoefr;
     doublereal slamch_(char *, ftnlen);
    static real salfar, safmin;
     /* Subroutine */ int xerbla_(char *, integer *, ftnlen);
    static real xscale, bignum;
    static logical ilcomp, ilcplx;
     /* Subroutine */ int slacpy_(char *, integer *, integer *, real *, 
            integer *, real *, integer *, ftnlen);
    static integer ihwmny;


/*  -- LAPACK routine (version 3.0) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd., */
/*     Courant Institute, Argonne National Lab, and Rice University */
/*     June 30, 1999 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */


/*  Purpose */
/*  ======= */

/*  STGEVC computes some or all of the right and/or left generalized */
/*  eigenvectors of a pair of real upper triangular matrices (A,B). */

/*  The right generalized eigenvector x and the left generalized */
/*  eigenvector y of (A,B) corresponding to a generalized eigenvalue */
/*  w are defined by: */

/*          (A - wB) * x = 0  and  y**H * (A - wB) = 0 */

/*  where y**H denotes the conjugate tranpose of y. */

/*  If an eigenvalue w is determined by zero diagonal elements of both A */
/*  and B, a unit vector is returned as the corresponding eigenvector. */

/*  If all eigenvectors are requested, the routine may either return */
/*  the matrices X and/or Y of right or left eigenvectors of (A,B), or */
/*  the products Z*X and/or Q*Y, where Z and Q are input orthogonal */
/*  matrices.  If (A,B) was obtained from the generalized real-Schur */
/*  factorization of an original pair of matrices */
/*     (A0,B0) = (Q*A*Z**H,Q*B*Z**H), */
/*  then Z*X and Q*Y are the matrices of right or left eigenvectors of */
/*  A. */

/*  A must be block upper triangular, with 1-by-1 and 2-by-2 diagonal */
/*  blocks.  Corresponding to each 2-by-2 diagonal block is a scomplex */
/*  conjugate pair of eigenvalues and eigenvectors; only one */
/*  eigenvector of the pair is computed, namely the one corresponding */
/*  to the eigenvalue with positive imaginary part. */

/*  Arguments */
/*  ========= */

/*  SIDE    (input) CHARACTER*1 */
/*          = 'R': compute right eigenvectors only; */
/*          = 'L': compute left eigenvectors only; */
/*          = 'B': compute both right and left eigenvectors. */

/*  HOWMNY  (input) CHARACTER*1 */
/*          = 'A': compute all right and/or left eigenvectors; */
/*          = 'B': compute all right and/or left eigenvectors, and */
/*                 backtransform them using the input matrices supplied */
/*                 in VR and/or VL; */
/*          = 'S': compute selected right and/or left eigenvectors, */
/*                 specified by the logical array SELECT. */

/*  SELECT  (input) LOGICAL array, dimension (N) */
/*          If HOWMNY='S', SELECT specifies the eigenvectors to be */
/*          computed. */
/*          If HOWMNY='A' or 'B', SELECT is not referenced. */
/*          To select the real eigenvector corresponding to the real */
/*          eigenvalue w(j), SELECT(j) must be set to .TRUE.  To select */
/*          the scomplex eigenvector corresponding to a scomplex conjugate */
/*          pair w(j) and w(j+1), either SELECT(j) or SELECT(j+1) must */
/*          be set to .TRUE.. */

/*  N       (input) INTEGER */
/*          The order of the matrices A and B.  N >= 0. */

/*  A       (input) REAL array, dimension (LDA,N) */
/*          The upper quasi-triangular matrix A. */

/*  LDA     (input) INTEGER */
/*          The leading dimension of array A.  LDA >= MAX(1, N). */

/*  B       (input) REAL array, dimension (LDB,N) */
/*          The upper triangular matrix B.  If A has a 2-by-2 diagonal */
/*          block, then the corresponding 2-by-2 block of B must be */
/*          diagonal with positive elements. */

/*  LDB     (input) INTEGER */
/*          The leading dimension of array B.  LDB >= MAX(1,N). */

/*  VL      (input/output) REAL array, dimension (LDVL,MM) */
/*          On entry, if SIDE = 'L' or 'B' and HOWMNY = 'B', VL must */
/*          contain an N-by-N matrix Q (usually the orthogonal matrix Q */
/*          of left Schur vectors returned by SHGEQZ). */
/*          On exit, if SIDE = 'L' or 'B', VL contains: */
/*          if HOWMNY = 'A', the matrix Y of left eigenvectors of (A,B); */
/*          if HOWMNY = 'B', the matrix Q*Y; */
/*          if HOWMNY = 'S', the left eigenvectors of (A,B) specified by */
/*                      SELECT, stored consecutively in the columns of */
/*                      VL, in the same order as their eigenvalues. */
/*          If SIDE = 'R', VL is not referenced. */

/*          A scomplex eigenvector corresponding to a scomplex eigenvalue */
/*          is stored in two consecutive columns, the first holding the */
/*          real part, and the second the imaginary part. */

/*  LDVL    (input) INTEGER */
/*          The leading dimension of array VL. */
/*          LDVL >= MAX(1,N) if SIDE = 'L' or 'B'; LDVL >= 1 otherwise. */

/*  VR      (input/output) REAL array, dimension (LDVR,MM) */
/*          On entry, if SIDE = 'R' or 'B' and HOWMNY = 'B', VR must */
/*          contain an N-by-N matrix Q (usually the orthogonal matrix Z */
/*          of right Schur vectors returned by SHGEQZ). */
/*          On exit, if SIDE = 'R' or 'B', VR contains: */
/*          if HOWMNY = 'A', the matrix X of right eigenvectors of (A,B); */
/*          if HOWMNY = 'B', the matrix Z*X; */
/*          if HOWMNY = 'S', the right eigenvectors of (A,B) specified by */
/*                      SELECT, stored consecutively in the columns of */
/*                      VR, in the same order as their eigenvalues. */
/*          If SIDE = 'L', VR is not referenced. */

/*          A scomplex eigenvector corresponding to a scomplex eigenvalue */
/*          is stored in two consecutive columns, the first holding the */
/*          real part and the second the imaginary part. */

/*  LDVR    (input) INTEGER */
/*          The leading dimension of the array VR. */
/*          LDVR >= MAX(1,N) if SIDE = 'R' or 'B'; LDVR >= 1 otherwise. */

/*  MM      (input) INTEGER */
/*          The number of columns in the arrays VL and/or VR. MM >= M. */

/*  M       (output) INTEGER */
/*          The number of columns in the arrays VL and/or VR actually */
/*          used to store the eigenvectors.  If HOWMNY = 'A' or 'B', M */
/*          is set to N.  Each selected real eigenvector occupies one */
/*          column and each selected scomplex eigenvector occupies two */
/*          columns. */

/*  WORK    (workspace) REAL array, dimension (6*N) */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit. */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value. */
/*          > 0:  the 2-by-2 block (INFO:INFO+1) does not have a scomplex */
/*                eigenvalue. */

/*  Further Details */
/*  =============== */

/*  Allocation of workspace: */
/*  ---------- -- --------- */

/*     WORK( j ) = 1-norm of j-th column of A, above the diagonal */
/*     WORK( N+j ) = 1-norm of j-th column of B, above the diagonal */
/*     WORK( 2*N+1:3*N ) = real part of eigenvector */
/*     WORK( 3*N+1:4*N ) = imaginary part of eigenvector */
/*     WORK( 4*N+1:5*N ) = real part of back-transformed eigenvector */
/*     WORK( 5*N+1:6*N ) = imaginary part of back-transformed eigenvector */

/*  Rowwise vs. columnwise solution methods: */
/*  ------- --  ---------- -------- ------- */

/*  Finding a generalized eigenvector consists basically of solving the */
/*  singular triangular system */

/*   (A - w B) x = 0     (for right) or:   (A - w B)**H y = 0  (for left) */

/*  Consider finding the i-th right eigenvector (assume all eigenvalues */
/*  are real). The equation to be solved is: */
/*       n                   i */
/*  0 = sum  C(j,k) v(k)  = sum  C(j,k) v(k)     for j = i,. . .,1 */
/*      k=j                 k=j */

/*  where  C = (A - w B)  (The components v(i+1:n) are 0.) */

/*  The "rowwise" method is: */

/*  (1)  v(i) := 1 */
/*  for j = i-1,. . .,1: */
/*                          i */
/*      (2) compute  s = - sum C(j,k) v(k)   and */
/*                        k=j+1 */

/*      (3) v(j) := s / C(j,j) */

/*  Step 2 is sometimes called the "dot product" step, since it is an */
/*  inner product between the j-th row and the portion of the eigenvector */
/*  that has been computed so far. */

/*  The "columnwise" method consists basically in doing the sums */
/*  for all the rows in parallel.  As each v(j) is computed, the */
/*  contribution of v(j) times the j-th column of C is added to the */
/*  partial sums.  Since FORTRAN arrays are stored columnwise, this has */
/*  the advantage that at each step, the elements of C that are accessed */
/*  are adjacent to one another, whereas with the rowwise method, the */
/*  elements accessed at a step are spaced LDA (and LDB) words apart. */

/*  When finding left eigenvectors, the matrix in question is the */
/*  transpose of the one in storage, so the rowwise method then */
/*  actually accesses columns of A and B at each step, and so is the */
/*  preferred method. */

/*  ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. Local Arrays .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Decode and Test the input parameters */

    /* Parameter adjustments */
    --select;
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    b_dim1 = *ldb;
    b_offset = 1 + b_dim1;
    b -= b_offset;
    vl_dim1 = *ldvl;
    vl_offset = 1 + vl_dim1;
    vl -= vl_offset;
    vr_dim1 = *ldvr;
    vr_offset = 1 + vr_dim1;
    vr -= vr_offset;
    --work;

    /* Function Body */
    if (lsame_(howmny, "A", (ftnlen)1, (ftnlen)1)) {
        ihwmny = 1;
        ilall = TRUE_;
        ilback = FALSE_;
    } else if (lsame_(howmny, "S", (ftnlen)1, (ftnlen)1)) {
        ihwmny = 2;
        ilall = FALSE_;
        ilback = FALSE_;
    } else if (lsame_(howmny, "B", (ftnlen)1, (ftnlen)1) || lsame_(howmny, 
            "T", (ftnlen)1, (ftnlen)1)) {
        ihwmny = 3;
        ilall = TRUE_;
        ilback = TRUE_;
    } else {
        ihwmny = -1;
        ilall = TRUE_;
    }

    if (lsame_(side, "R", (ftnlen)1, (ftnlen)1)) {
        iside = 1;
        compl = FALSE_;
        compr = TRUE_;
    } else if (lsame_(side, "L", (ftnlen)1, (ftnlen)1)) {
        iside = 2;
        compl = TRUE_;
        compr = FALSE_;
    } else if (lsame_(side, "B", (ftnlen)1, (ftnlen)1)) {
        iside = 3;
        compl = TRUE_;
        compr = TRUE_;
    } else {
        iside = -1;
    }

    *info = 0;
    if (iside < 0) {
        *info = -1;
    } else if (ihwmny < 0) {
        *info = -2;
    } else if (*n < 0) {
        *info = -4;
    } else if (*lda < MAX(1,*n)) {
        *info = -6;
    } else if (*ldb < MAX(1,*n)) {
        *info = -8;
    }
    if (*info != 0) {
        i__1 = -(*info);
        xerbla_("STGEVC", &i__1, (ftnlen)6);
        return 0;
    }

/*     Count the number of eigenvectors to be computed */

    if (! ilall) {
        im = 0;
        ilcplx = FALSE_;
        i__1 = *n;
        for (j = 1; j <= i__1; ++j) {
            if (ilcplx) {
                ilcplx = FALSE_;
                goto L10;
            }
            if (j < *n) {
                if (a[j + 1 + j * a_dim1] != 0.f) {
                    ilcplx = TRUE_;
                }
            }
            if (ilcplx) {
                if (select[j] || select[j + 1]) {
                    im += 2;
                }
            } else {
                if (select[j]) {
                    ++im;
                }
            }
L10:
            ;
        }
    } else {
        im = *n;
    }

/*     Check 2-by-2 diagonal blocks of A, B */

    ilabad = FALSE_;
    ilbbad = FALSE_;
    i__1 = *n - 1;
    for (j = 1; j <= i__1; ++j) {
        if (a[j + 1 + j * a_dim1] != 0.f) {
            if (b[j + j * b_dim1] == 0.f || b[j + 1 + (j + 1) * b_dim1] == 
                    0.f || b[j + (j + 1) * b_dim1] != 0.f) {
                ilbbad = TRUE_;
            }
            if (j < *n - 1) {
                if (a[j + 2 + (j + 1) * a_dim1] != 0.f) {
                    ilabad = TRUE_;
                }
            }
        }
/* L20: */
    }

    if (ilabad) {
        *info = -5;
    } else if (ilbbad) {
        *info = -7;
    } else if (compl && *ldvl < *n || *ldvl < 1) {
        *info = -10;
    } else if (compr && *ldvr < *n || *ldvr < 1) {
        *info = -12;
    } else if (*mm < im) {
        *info = -13;
    }
    if (*info != 0) {
        i__1 = -(*info);
        xerbla_("STGEVC", &i__1, (ftnlen)6);
        return 0;
    }

/*     Quick return if possible */

    *m = im;
    if (*n == 0) {
        return 0;
    }

/*     Machine Constants */

    safmin = (real)slamch_("Safe minimum", (ftnlen)12);
    big = 1.f / safmin;
    slabad_(&safmin, &big);
    ulp = (real)slamch_("Epsilon", (ftnlen)7) * slamch_("Base", (ftnlen)4);
    small1 = safmin * *n / ulp;
    big = 1.f / small1;
    bignum = 1.f / (safmin * *n);

/*     Compute the 1-norm of each column of the strictly upper triangular */
/*     part (i.e., excluding all elements belonging to the diagonal */
/*     blocks) of A and B to check for possible overflow in the */
/*     triangular solver. */

    anorm = (r__1 = a[a_dim1 + 1], SABS(r__1));
    if (*n > 1) {
        anorm += (r__1 = a[a_dim1 + 2], SABS(r__1));
    }
    bnorm = (r__1 = b[b_dim1 + 1], SABS(r__1));
    work[1] = 0.f;
    work[*n + 1] = 0.f;

    i__1 = *n;
    for (j = 2; j <= i__1; ++j) {
        temp = 0.f;
        temp2 = 0.f;
        if (a[j + (j - 1) * a_dim1] == 0.f) {
            iend = j - 1;
        } else {
            iend = j - 2;
        }
        i__2 = iend;
        for (i__ = 1; i__ <= i__2; ++i__) {
            temp += (r__1 = a[i__ + j * a_dim1], SABS(r__1));
            temp2 += (r__1 = b[i__ + j * b_dim1], SABS(r__1));
/* L30: */
        }
        work[j] = temp;
        work[*n + j] = temp2;
/* Computing MIN */
        i__3 = j + 1;
        i__2 = MIN(i__3,*n);
        for (i__ = iend + 1; i__ <= i__2; ++i__) {
            temp += (r__1 = a[i__ + j * a_dim1], SABS(r__1));
            temp2 += (r__1 = b[i__ + j * b_dim1], SABS(r__1));
/* L40: */
        }
        anorm = dmax(anorm,temp);
        bnorm = dmax(bnorm,temp2);
/* L50: */
    }

    ascale = 1.f / dmax(anorm,safmin);
    bscale = 1.f / dmax(bnorm,safmin);

/*     Left eigenvectors */

    if (compl) {
        ieig = 0;

/*        Main loop over eigenvalues */

        ilcplx = FALSE_;
        i__1 = *n;
        for (je = 1; je <= i__1; ++je) {

/*           Skip this iteration if (a) HOWMNY='S' and SELECT=.FALSE., or */
/*           (b) this would be the second of a scomplex pair. */
/*           Check for scomplex eigenvalue, so as to be sure of which */
/*           entry(-ies) of SELECT to look at. */

            if (ilcplx) {
                ilcplx = FALSE_;
                goto L220;
            }
            nw = 1;
            if (je < *n) {
                if (a[je + 1 + je * a_dim1] != 0.f) {
                    ilcplx = TRUE_;
                    nw = 2;
                }
            }
            if (ilall) {
                ilcomp = TRUE_;
            } else if (ilcplx) {
                ilcomp = select[je] || select[je + 1];
            } else {
                ilcomp = select[je];
            }
            if (! ilcomp) {
                goto L220;
            }

/*           Decide if (a) singular pencil, (b) real eigenvalue, or */
/*           (c) scomplex eigenvalue. */

            if (! ilcplx) {
                if ((r__1 = a[je + je * a_dim1], SABS(r__1)) <= safmin && (
                        r__2 = b[je + je * b_dim1], SABS(r__2)) <= safmin) {

/*                 Singular matrix pencil -- return unit eigenvector */

                    ++ieig;
                    i__2 = *n;
                    for (jr = 1; jr <= i__2; ++jr) {
                        vl[jr + ieig * vl_dim1] = 0.f;
/* L60: */
                    }
                    vl[ieig + ieig * vl_dim1] = 1.f;
                    goto L220;
                }
            }

/*           Clear vector */

            i__2 = nw * *n;
            for (jr = 1; jr <= i__2; ++jr) {
                work[(*n << 1) + jr] = 0.f;
/* L70: */
            }
/*                                                 T */
/*           Compute coefficients in  ( a A - b B )  y = 0 */
/*              a  is  ACOEF */
/*              b  is  BCOEFR + i*BCOEFI */

            if (! ilcplx) {

/*              Real eigenvalue */

/* Computing MAX */
                r__3 = (r__1 = a[je + je * a_dim1], SABS(r__1)) * ascale, 
                        r__4 = (r__2 = b[je + je * b_dim1], SABS(r__2)) * 
                        bscale, r__3 = MAX(r__3,r__4);
                temp = 1.f / dmax(r__3,safmin);
                salfar = temp * a[je + je * a_dim1] * ascale;
                sbeta = temp * b[je + je * b_dim1] * bscale;
                acoef = sbeta * ascale;
                bcoefr = salfar * bscale;
                bcoefi = 0.f;

/*              Scale to avoid underflow */

                scale = 1.f;
                lsa = SABS(sbeta) >= safmin && SABS(acoef) < small1;
                lsb = SABS(salfar) >= safmin && SABS(bcoefr) < small1;
                if (lsa) {
                    scale = small1 / SABS(sbeta) * dmin(anorm,big);
                }
                if (lsb) {
/* Computing MAX */
                    r__1 = scale, r__2 = small1 / SABS(salfar) * dmin(bnorm,
                            big);
                    scale = dmax(r__1,r__2);
                }
                if (lsa || lsb) {
/* Computing MIN */
/* Computing MAX */
                    r__3 = 1.f, r__4 = SABS(acoef), r__3 = MAX(r__3,r__4), 
                            r__4 = SABS(bcoefr);
                    r__1 = scale, r__2 = 1.f / (safmin * dmax(r__3,r__4));
                    scale = dmin(r__1,r__2);
                    if (lsa) {
                        acoef = ascale * (scale * sbeta);
                    } else {
                        acoef = scale * acoef;
                    }
                    if (lsb) {
                        bcoefr = bscale * (scale * salfar);
                    } else {
                        bcoefr = scale * bcoefr;
                    }
                }
                acoefa = SABS(acoef);
                bcoefa = SABS(bcoefr);

/*              First component is 1 */

                work[(*n << 1) + je] = 1.f;
                xmax = 1.f;
            } else {

/*              Complex eigenvalue */

                r__1 = safmin * 100.f;
                slag2_(&a[je + je * a_dim1], lda, &b[je + je * b_dim1], ldb, &
                        r__1, &acoef, &temp, &bcoefr, &temp2, &bcoefi);
                bcoefi = -bcoefi;
                if (bcoefi == 0.f) {
                    *info = je;
                    return 0;
                }

/*              Scale to avoid over/underflow */

                acoefa = SABS(acoef);
                bcoefa = SABS(bcoefr) + SABS(bcoefi);
                scale = 1.f;
                if (acoefa * ulp < safmin && acoefa >= safmin) {
                    scale = safmin / ulp / acoefa;
                }
                if (bcoefa * ulp < safmin && bcoefa >= safmin) {
/* Computing MAX */
                    r__1 = scale, r__2 = safmin / ulp / bcoefa;
                    scale = dmax(r__1,r__2);
                }
                if (safmin * acoefa > ascale) {
                    scale = ascale / (safmin * acoefa);
                }
                if (safmin * bcoefa > bscale) {
/* Computing MIN */
                    r__1 = scale, r__2 = bscale / (safmin * bcoefa);
                    scale = dmin(r__1,r__2);
                }
                if (scale != 1.f) {
                    acoef = scale * acoef;
                    acoefa = SABS(acoef);
                    bcoefr = scale * bcoefr;
                    bcoefi = scale * bcoefi;
                    bcoefa = SABS(bcoefr) + SABS(bcoefi);
                }

/*              Compute first two components of eigenvector */

                temp = acoef * a[je + 1 + je * a_dim1];
                temp2r = acoef * a[je + je * a_dim1] - bcoefr * b[je + je * 
                        b_dim1];
                temp2i = -bcoefi * b[je + je * b_dim1];
                if (SABS(temp) > SABS(temp2r) + SABS(temp2i)) {
                    work[(*n << 1) + je] = 1.f;
                    work[*n * 3 + je] = 0.f;
                    work[(*n << 1) + je + 1] = -temp2r / temp;
                    work[*n * 3 + je + 1] = -temp2i / temp;
                } else {
                    work[(*n << 1) + je + 1] = 1.f;
                    work[*n * 3 + je + 1] = 0.f;
                    temp = acoef * a[je + (je + 1) * a_dim1];
                    work[(*n << 1) + je] = (bcoefr * b[je + 1 + (je + 1) * 
                            b_dim1] - acoef * a[je + 1 + (je + 1) * a_dim1]) /
                             temp;
                    work[*n * 3 + je] = bcoefi * b[je + 1 + (je + 1) * b_dim1]
                             / temp;
                }
/* Computing MAX */
                r__5 = (r__1 = work[(*n << 1) + je], SABS(r__1)) + (r__2 = 
                        work[*n * 3 + je], SABS(r__2)), r__6 = (r__3 = work[(*
                        n << 1) + je + 1], SABS(r__3)) + (r__4 = work[*n * 3 
                        + je + 1], SABS(r__4));
                xmax = dmax(r__5,r__6);
            }

/* Computing MAX */
            r__1 = ulp * acoefa * anorm, r__2 = ulp * bcoefa * bnorm, r__1 = 
                    MAX(r__1,r__2);
            dmin__ = dmax(r__1,safmin);

/*                                           T */
/*           Triangular solve of  (a A - b B)  y = 0 */

/*                                   T */
/*           (rowwise in  (a A - b B) , or columnwise in (a A - b B) ) */

            il2by2 = FALSE_;

            i__2 = *n;
            for (j = je + nw; j <= i__2; ++j) {
                if (il2by2) {
                    il2by2 = FALSE_;
                    goto L160;
                }

                na = 1;
                bdiag[0] = b[j + j * b_dim1];
                if (j < *n) {
                    if (a[j + 1 + j * a_dim1] != 0.f) {
                        il2by2 = TRUE_;
                        bdiag[1] = b[j + 1 + (j + 1) * b_dim1];
                        na = 2;
                    }
                }

/*              Check whether scaling is necessary for dot products */

                xscale = 1.f / dmax(1.f,xmax);
/* Computing MAX */
                r__1 = work[j], r__2 = work[*n + j], r__1 = MAX(r__1,r__2), 
                        r__2 = acoefa * work[j] + bcoefa * work[*n + j];
                temp = dmax(r__1,r__2);
                if (il2by2) {
/* Computing MAX */
                    r__1 = temp, r__2 = work[j + 1], r__1 = MAX(r__1,r__2), 
                            r__2 = work[*n + j + 1], r__1 = MAX(r__1,r__2), 
                            r__2 = acoefa * work[j + 1] + bcoefa * work[*n + 
                            j + 1];
                    temp = dmax(r__1,r__2);
                }
                if (temp > bignum * xscale) {
                    i__3 = nw - 1;
                    for (jw = 0; jw <= i__3; ++jw) {
                        i__4 = j - 1;
                        for (jr = je; jr <= i__4; ++jr) {
                            work[(jw + 2) * *n + jr] = xscale * work[(jw + 2) 
                                    * *n + jr];
/* L80: */
                        }
/* L90: */
                    }
                    xmax *= xscale;
                }

/*              Compute dot products */

/*                    j-1 */
/*              SUM = sum  conjg( a*A(k,j) - b*B(k,j) )*x(k) */
/*                    k=je */

/*              To reduce the op count, this is done as */

/*              _        j-1                  _        j-1 */
/*              a*conjg( sum  A(k,j)*x(k) ) - b*conjg( sum  B(k,j)*x(k) ) */
/*                       k=je                          k=je */

/*              which may cause underflow problems if A or B are close */
/*              to underflow.  (E.g., less than SMALL.) */


/*              A series of compiler directives to defeat vectorization */
/*              for the next loop */

/* $PL$ CMCHAR=' ' */
/* DIR$          NEXTSCALAR */
/* $DIR          SCALAR */
/* DIR$          NEXT SCALAR */
/* VD$L          NOVECTOR */
/* DEC$          NOVECTOR */
/* VD$           NOVECTOR */
/* VDIR          NOVECTOR */
/* VOCL          LOOP,SCALAR */
/* IBM           PREFER SCALAR */
/* $PL$ CMCHAR='*' */

                i__3 = nw;
                for (jw = 1; jw <= i__3; ++jw) {

/* $PL$ CMCHAR=' ' */
/* DIR$             NEXTSCALAR */
/* $DIR             SCALAR */
/* DIR$             NEXT SCALAR */
/* VD$L             NOVECTOR */
/* DEC$             NOVECTOR */
/* VD$              NOVECTOR */
/* VDIR             NOVECTOR */
/* VOCL             LOOP,SCALAR */
/* IBM              PREFER SCALAR */
/* $PL$ CMCHAR='*' */

                    i__4 = na;
                    for (ja = 1; ja <= i__4; ++ja) {
                        suma[ja + (jw << 1) - 3] = 0.f;
                        sumb[ja + (jw << 1) - 3] = 0.f;

                        i__5 = j - 1;
                        for (jr = je; jr <= i__5; ++jr) {
                            suma[ja + (jw << 1) - 3] += a[jr + (j + ja - 1) * 
                                    a_dim1] * work[(jw + 1) * *n + jr];
                            sumb[ja + (jw << 1) - 3] += b[jr + (j + ja - 1) * 
                                    b_dim1] * work[(jw + 1) * *n + jr];
/* L100: */
                        }
/* L110: */
                    }
/* L120: */
                }

/* $PL$ CMCHAR=' ' */
/* DIR$          NEXTSCALAR */
/* $DIR          SCALAR */
/* DIR$          NEXT SCALAR */
/* VD$L          NOVECTOR */
/* DEC$          NOVECTOR */
/* VD$           NOVECTOR */
/* VDIR          NOVECTOR */
/* VOCL          LOOP,SCALAR */
/* IBM           PREFER SCALAR */
/* $PL$ CMCHAR='*' */

                i__3 = na;
                for (ja = 1; ja <= i__3; ++ja) {
                    if (ilcplx) {
                        sum[ja - 1] = -acoef * suma[ja - 1] + bcoefr * sumb[
                                ja - 1] - bcoefi * sumb[ja + 1];
                        sum[ja + 1] = -acoef * suma[ja + 1] + bcoefr * sumb[
                                ja + 1] + bcoefi * sumb[ja - 1];
                    } else {
                        sum[ja - 1] = -acoef * suma[ja - 1] + bcoefr * sumb[
                                ja - 1];
                    }
/* L130: */
                }

/*                                  T */
/*              Solve  ( a A - b B )  y = SUM(,) */
/*              with scaling and perturbation of the denominator */

                slaln2_(&c_true, &na, &nw, &dmin__, &acoef, &a[j + j * a_dim1]
                        , lda, bdiag, &bdiag[1], sum, &c__2, &bcoefr, &bcoefi,
                         &work[(*n << 1) + j], n, &scale, &temp, &iinfo);
                if (scale < 1.f) {
                    i__3 = nw - 1;
                    for (jw = 0; jw <= i__3; ++jw) {
                        i__4 = j - 1;
                        for (jr = je; jr <= i__4; ++jr) {
                            work[(jw + 2) * *n + jr] = scale * work[(jw + 2) *
                                     *n + jr];
/* L140: */
                        }
/* L150: */
                    }
                    xmax = scale * xmax;
                }
                xmax = dmax(xmax,temp);
L160:
                ;
            }

/*           Copy eigenvector to VL, back transforming if */
/*           HOWMNY='B'. */

            ++ieig;
            if (ilback) {
                i__2 = nw - 1;
                for (jw = 0; jw <= i__2; ++jw) {
                    i__3 = *n + 1 - je;
                    sgemv_("N", n, &i__3, &c_b35, &vl[je * vl_dim1 + 1], ldvl,
                             &work[(jw + 2) * *n + je], &c__1, &c_b37, &work[(
                            jw + 4) * *n + 1], &c__1, (ftnlen)1);
/* L170: */
                }
                slacpy_(" ", n, &nw, &work[(*n << 2) + 1], n, &vl[je * 
                        vl_dim1 + 1], ldvl, (ftnlen)1);
                ibeg = 1;
            } else {
                slacpy_(" ", n, &nw, &work[(*n << 1) + 1], n, &vl[ieig * 
                        vl_dim1 + 1], ldvl, (ftnlen)1);
                ibeg = je;
            }

/*           Scale eigenvector */

            xmax = 0.f;
            if (ilcplx) {
                i__2 = *n;
                for (j = ibeg; j <= i__2; ++j) {
/* Computing MAX */
                    r__3 = xmax, r__4 = (r__1 = vl[j + ieig * vl_dim1], SABS(
                            r__1)) + (r__2 = vl[j + (ieig + 1) * vl_dim1], 
                            SABS(r__2));
                    xmax = dmax(r__3,r__4);
/* L180: */
                }
            } else {
                i__2 = *n;
                for (j = ibeg; j <= i__2; ++j) {
/* Computing MAX */
                    r__2 = xmax, r__3 = (r__1 = vl[j + ieig * vl_dim1], SABS(
                            r__1));
                    xmax = dmax(r__2,r__3);
/* L190: */
                }
            }

            if (xmax > safmin) {
                xscale = 1.f / xmax;

                i__2 = nw - 1;
                for (jw = 0; jw <= i__2; ++jw) {
                    i__3 = *n;
                    for (jr = ibeg; jr <= i__3; ++jr) {
                        vl[jr + (ieig + jw) * vl_dim1] = xscale * vl[jr + (
                                ieig + jw) * vl_dim1];
/* L200: */
                    }
/* L210: */
                }
            }
            ieig = ieig + nw - 1;

L220:
            ;
        }
    }

/*     Right eigenvectors */

    if (compr) {
        ieig = im + 1;

/*        Main loop over eigenvalues */

        ilcplx = FALSE_;
        for (je = *n; je >= 1; --je) {

/*           Skip this iteration if (a) HOWMNY='S' and SELECT=.FALSE., or */
/*           (b) this would be the second of a scomplex pair. */
/*           Check for scomplex eigenvalue, so as to be sure of which */
/*           entry(-ies) of SELECT to look at -- if scomplex, SELECT(JE) */
/*           or SELECT(JE-1). */
/*           If this is a scomplex pair, the 2-by-2 diagonal block */
/*           corresponding to the eigenvalue is in rows/columns JE-1:JE */

            if (ilcplx) {
                ilcplx = FALSE_;
                goto L500;
            }
            nw = 1;
            if (je > 1) {
                if (a[je + (je - 1) * a_dim1] != 0.f) {
                    ilcplx = TRUE_;
                    nw = 2;
                }
            }
            if (ilall) {
                ilcomp = TRUE_;
            } else if (ilcplx) {
                ilcomp = select[je] || select[je - 1];
            } else {
                ilcomp = select[je];
            }
            if (! ilcomp) {
                goto L500;
            }

/*           Decide if (a) singular pencil, (b) real eigenvalue, or */
/*           (c) scomplex eigenvalue. */

            if (! ilcplx) {
                if ((r__1 = a[je + je * a_dim1], SABS(r__1)) <= safmin && (
                        r__2 = b[je + je * b_dim1], SABS(r__2)) <= safmin) {

/*                 Singular matrix pencil -- unit eigenvector */

                    --ieig;
                    i__1 = *n;
                    for (jr = 1; jr <= i__1; ++jr) {
                        vr[jr + ieig * vr_dim1] = 0.f;
/* L230: */
                    }
                    vr[ieig + ieig * vr_dim1] = 1.f;
                    goto L500;
                }
            }

/*           Clear vector */

            i__1 = nw - 1;
            for (jw = 0; jw <= i__1; ++jw) {
                i__2 = *n;
                for (jr = 1; jr <= i__2; ++jr) {
                    work[(jw + 2) * *n + jr] = 0.f;
/* L240: */
                }
/* L250: */
            }

/*           Compute coefficients in  ( a A - b B ) x = 0 */
/*              a  is  ACOEF */
/*              b  is  BCOEFR + i*BCOEFI */

            if (! ilcplx) {

/*              Real eigenvalue */

/* Computing MAX */
                r__3 = (r__1 = a[je + je * a_dim1], SABS(r__1)) * ascale, 
                        r__4 = (r__2 = b[je + je * b_dim1], SABS(r__2)) * 
                        bscale, r__3 = MAX(r__3,r__4);
                temp = 1.f / dmax(r__3,safmin);
                salfar = temp * a[je + je * a_dim1] * ascale;
                sbeta = temp * b[je + je * b_dim1] * bscale;
                acoef = sbeta * ascale;
                bcoefr = salfar * bscale;
                bcoefi = 0.f;

/*              Scale to avoid underflow */

                scale = 1.f;
                lsa = SABS(sbeta) >= safmin && SABS(acoef) < small1;
                lsb = SABS(salfar) >= safmin && SABS(bcoefr) < small1;
                if (lsa) {
                    scale = small1 / SABS(sbeta) * dmin(anorm,big);
                }
                if (lsb) {
/* Computing MAX */
                    r__1 = scale, r__2 = small1 / SABS(salfar) * dmin(bnorm,
                            big);
                    scale = dmax(r__1,r__2);
                }
                if (lsa || lsb) {
/* Computing MIN */
/* Computing MAX */
                    r__3 = 1.f, r__4 = SABS(acoef), r__3 = MAX(r__3,r__4), 
                            r__4 = SABS(bcoefr);
                    r__1 = scale, r__2 = 1.f / (safmin * dmax(r__3,r__4));
                    scale = dmin(r__1,r__2);
                    if (lsa) {
                        acoef = ascale * (scale * sbeta);
                    } else {
                        acoef = scale * acoef;
                    }
                    if (lsb) {
                        bcoefr = bscale * (scale * salfar);
                    } else {
                        bcoefr = scale * bcoefr;
                    }
                }
                acoefa = SABS(acoef);
                bcoefa = SABS(bcoefr);

/*              First component is 1 */

                work[(*n << 1) + je] = 1.f;
                xmax = 1.f;

/*              Compute contribution from column JE of A and B to sum */
/*              (See "Further Details", above.) */

                i__1 = je - 1;
                for (jr = 1; jr <= i__1; ++jr) {
                    work[(*n << 1) + jr] = bcoefr * b[jr + je * b_dim1] - 
                            acoef * a[jr + je * a_dim1];
/* L260: */
                }
            } else {

/*              Complex eigenvalue */

                r__1 = safmin * 100.f;
                slag2_(&a[je - 1 + (je - 1) * a_dim1], lda, &b[je - 1 + (je - 
                        1) * b_dim1], ldb, &r__1, &acoef, &temp, &bcoefr, &
                        temp2, &bcoefi);
                if (bcoefi == 0.f) {
                    *info = je - 1;
                    return 0;
                }

/*              Scale to avoid over/underflow */

                acoefa = SABS(acoef);
                bcoefa = SABS(bcoefr) + SABS(bcoefi);
                scale = 1.f;
                if (acoefa * ulp < safmin && acoefa >= safmin) {
                    scale = safmin / ulp / acoefa;
                }
                if (bcoefa * ulp < safmin && bcoefa >= safmin) {
/* Computing MAX */
                    r__1 = scale, r__2 = safmin / ulp / bcoefa;
                    scale = dmax(r__1,r__2);
                }
                if (safmin * acoefa > ascale) {
                    scale = ascale / (safmin * acoefa);
                }
                if (safmin * bcoefa > bscale) {
/* Computing MIN */
                    r__1 = scale, r__2 = bscale / (safmin * bcoefa);
                    scale = dmin(r__1,r__2);
                }
                if (scale != 1.f) {
                    acoef = scale * acoef;
                    acoefa = SABS(acoef);
                    bcoefr = scale * bcoefr;
                    bcoefi = scale * bcoefi;
                    bcoefa = SABS(bcoefr) + SABS(bcoefi);
                }

/*              Compute first two components of eigenvector */
/*              and contribution to sums */

                temp = acoef * a[je + (je - 1) * a_dim1];
                temp2r = acoef * a[je + je * a_dim1] - bcoefr * b[je + je * 
                        b_dim1];
                temp2i = -bcoefi * b[je + je * b_dim1];
                if (SABS(temp) >= SABS(temp2r) + SABS(temp2i)) {
                    work[(*n << 1) + je] = 1.f;
                    work[*n * 3 + je] = 0.f;
                    work[(*n << 1) + je - 1] = -temp2r / temp;
                    work[*n * 3 + je - 1] = -temp2i / temp;
                } else {
                    work[(*n << 1) + je - 1] = 1.f;
                    work[*n * 3 + je - 1] = 0.f;
                    temp = acoef * a[je - 1 + je * a_dim1];
                    work[(*n << 1) + je] = (bcoefr * b[je - 1 + (je - 1) * 
                            b_dim1] - acoef * a[je - 1 + (je - 1) * a_dim1]) /
                             temp;
                    work[*n * 3 + je] = bcoefi * b[je - 1 + (je - 1) * b_dim1]
                             / temp;
                }

/* Computing MAX */
                r__5 = (r__1 = work[(*n << 1) + je], SABS(r__1)) + (r__2 = 
                        work[*n * 3 + je], SABS(r__2)), r__6 = (r__3 = work[(*
                        n << 1) + je - 1], SABS(r__3)) + (r__4 = work[*n * 3 
                        + je - 1], SABS(r__4));
                xmax = dmax(r__5,r__6);

/*              Compute contribution from columns JE and JE-1 */
/*              of A and B to the sums. */

                creala = acoef * work[(*n << 1) + je - 1];
                cimaga = acoef * work[*n * 3 + je - 1];
                crealb = bcoefr * work[(*n << 1) + je - 1] - bcoefi * work[*n 
                        * 3 + je - 1];
                cimagb = bcoefi * work[(*n << 1) + je - 1] + bcoefr * work[*n 
                        * 3 + je - 1];
                cre2a = acoef * work[(*n << 1) + je];
                cim2a = acoef * work[*n * 3 + je];
                cre2b = bcoefr * work[(*n << 1) + je] - bcoefi * work[*n * 3 
                        + je];
                cim2b = bcoefi * work[(*n << 1) + je] + bcoefr * work[*n * 3 
                        + je];
                i__1 = je - 2;
                for (jr = 1; jr <= i__1; ++jr) {
                    work[(*n << 1) + jr] = -creala * a[jr + (je - 1) * a_dim1]
                             + crealb * b[jr + (je - 1) * b_dim1] - cre2a * a[
                            jr + je * a_dim1] + cre2b * b[jr + je * b_dim1];
                    work[*n * 3 + jr] = -cimaga * a[jr + (je - 1) * a_dim1] + 
                            cimagb * b[jr + (je - 1) * b_dim1] - cim2a * a[jr 
                            + je * a_dim1] + cim2b * b[jr + je * b_dim1];
/* L270: */
                }
            }

/* Computing MAX */
            r__1 = ulp * acoefa * anorm, r__2 = ulp * bcoefa * bnorm, r__1 = 
                    MAX(r__1,r__2);
            dmin__ = dmax(r__1,safmin);

/*           Columnwise triangular solve of  (a A - b B)  x = 0 */

            il2by2 = FALSE_;
            for (j = je - nw; j >= 1; --j) {

/*              If a 2-by-2 block, is in position j-1:j, wait until */
/*              next iteration to process it (when it will be j:j+1) */

                if (! il2by2 && j > 1) {
                    if (a[j + (j - 1) * a_dim1] != 0.f) {
                        il2by2 = TRUE_;
                        goto L370;
                    }
                }
                bdiag[0] = b[j + j * b_dim1];
                if (il2by2) {
                    na = 2;
                    bdiag[1] = b[j + 1 + (j + 1) * b_dim1];
                } else {
                    na = 1;
                }

/*              Compute x(j) (and x(j+1), if 2-by-2 block) */

                slaln2_(&c_false, &na, &nw, &dmin__, &acoef, &a[j + j * 
                        a_dim1], lda, bdiag, &bdiag[1], &work[(*n << 1) + j], 
                        n, &bcoefr, &bcoefi, sum, &c__2, &scale, &temp, &
                        iinfo);
                if (scale < 1.f) {

                    i__1 = nw - 1;
                    for (jw = 0; jw <= i__1; ++jw) {
                        i__2 = je;
                        for (jr = 1; jr <= i__2; ++jr) {
                            work[(jw + 2) * *n + jr] = scale * work[(jw + 2) *
                                     *n + jr];
/* L280: */
                        }
/* L290: */
                    }
                }
/* Computing MAX */
                r__1 = scale * xmax;
                xmax = dmax(r__1,temp);

                i__1 = nw;
                for (jw = 1; jw <= i__1; ++jw) {
                    i__2 = na;
                    for (ja = 1; ja <= i__2; ++ja) {
                        work[(jw + 1) * *n + j + ja - 1] = sum[ja + (jw << 1) 
                                - 3];
/* L300: */
                    }
/* L310: */
                }

/*              w = w + x(j)*(a A(*,j) - b B(*,j) ) with scaling */

                if (j > 1) {

/*                 Check whether scaling is necessary for sum. */

                    xscale = 1.f / dmax(1.f,xmax);
                    temp = acoefa * work[j] + bcoefa * work[*n + j];
                    if (il2by2) {
/* Computing MAX */
                        r__1 = temp, r__2 = acoefa * work[j + 1] + bcoefa * 
                                work[*n + j + 1];
                        temp = dmax(r__1,r__2);
                    }
/* Computing MAX */
                    r__1 = MAX(temp,acoefa);
                    temp = dmax(r__1,bcoefa);
                    if (temp > bignum * xscale) {

                        i__1 = nw - 1;
                        for (jw = 0; jw <= i__1; ++jw) {
                            i__2 = je;
                            for (jr = 1; jr <= i__2; ++jr) {
                                work[(jw + 2) * *n + jr] = xscale * work[(jw 
                                        + 2) * *n + jr];
/* L320: */
                            }
/* L330: */
                        }
                        xmax *= xscale;
                    }

/*                 Compute the contributions of the off-diagonals of */
/*                 column j (and j+1, if 2-by-2 block) of A and B to the */
/*                 sums. */


                    i__1 = na;
                    for (ja = 1; ja <= i__1; ++ja) {
                        if (ilcplx) {
                            creala = acoef * work[(*n << 1) + j + ja - 1];
                            cimaga = acoef * work[*n * 3 + j + ja - 1];
                            crealb = bcoefr * work[(*n << 1) + j + ja - 1] - 
                                    bcoefi * work[*n * 3 + j + ja - 1];
                            cimagb = bcoefi * work[(*n << 1) + j + ja - 1] + 
                                    bcoefr * work[*n * 3 + j + ja - 1];
                            i__2 = j - 1;
                            for (jr = 1; jr <= i__2; ++jr) {
                                work[(*n << 1) + jr] = work[(*n << 1) + jr] - 
                                        creala * a[jr + (j + ja - 1) * a_dim1]
                                         + crealb * b[jr + (j + ja - 1) * 
                                        b_dim1];
                                work[*n * 3 + jr] = work[*n * 3 + jr] - 
                                        cimaga * a[jr + (j + ja - 1) * a_dim1]
                                         + cimagb * b[jr + (j + ja - 1) * 
                                        b_dim1];
/* L340: */
                            }
                        } else {
                            creala = acoef * work[(*n << 1) + j + ja - 1];
                            crealb = bcoefr * work[(*n << 1) + j + ja - 1];
                            i__2 = j - 1;
                            for (jr = 1; jr <= i__2; ++jr) {
                                work[(*n << 1) + jr] = work[(*n << 1) + jr] - 
                                        creala * a[jr + (j + ja - 1) * a_dim1]
                                         + crealb * b[jr + (j + ja - 1) * 
                                        b_dim1];
/* L350: */
                            }
                        }
/* L360: */
                    }
                }

                il2by2 = FALSE_;
L370:
                ;
            }

/*           Copy eigenvector to VR, back transforming if */
/*           HOWMNY='B'. */

            ieig -= nw;
            if (ilback) {

                i__1 = nw - 1;
                for (jw = 0; jw <= i__1; ++jw) {
                    i__2 = *n;
                    for (jr = 1; jr <= i__2; ++jr) {
                        work[(jw + 4) * *n + jr] = work[(jw + 2) * *n + 1] * 
                                vr[jr + vr_dim1];
/* L380: */
                    }

/*                 A series of compiler directives to defeat */
/*                 vectorization for the next loop */


                    i__2 = je;
                    for (jc = 2; jc <= i__2; ++jc) {
                        i__3 = *n;
                        for (jr = 1; jr <= i__3; ++jr) {
                            work[(jw + 4) * *n + jr] += work[(jw + 2) * *n + 
                                    jc] * vr[jr + jc * vr_dim1];
/* L390: */
                        }
/* L400: */
                    }
/* L410: */
                }

                i__1 = nw - 1;
                for (jw = 0; jw <= i__1; ++jw) {
                    i__2 = *n;
                    for (jr = 1; jr <= i__2; ++jr) {
                        vr[jr + (ieig + jw) * vr_dim1] = work[(jw + 4) * *n + 
                                jr];
/* L420: */
                    }
/* L430: */
                }

                iend = *n;
            } else {
                i__1 = nw - 1;
                for (jw = 0; jw <= i__1; ++jw) {
                    i__2 = *n;
                    for (jr = 1; jr <= i__2; ++jr) {
                        vr[jr + (ieig + jw) * vr_dim1] = work[(jw + 2) * *n + 
                                jr];
/* L440: */
                    }
/* L450: */
                }

                iend = je;
            }

/*           Scale eigenvector */

            xmax = 0.f;
            if (ilcplx) {
                i__1 = iend;
                for (j = 1; j <= i__1; ++j) {
/* Computing MAX */
                    r__3 = xmax, r__4 = (r__1 = vr[j + ieig * vr_dim1], SABS(
                            r__1)) + (r__2 = vr[j + (ieig + 1) * vr_dim1], 
                            SABS(r__2));
                    xmax = dmax(r__3,r__4);
/* L460: */
                }
            } else {
                i__1 = iend;
                for (j = 1; j <= i__1; ++j) {
/* Computing MAX */
                    r__2 = xmax, r__3 = (r__1 = vr[j + ieig * vr_dim1], SABS(
                            r__1));
                    xmax = dmax(r__2,r__3);
/* L470: */
                }
            }

            if (xmax > safmin) {
                xscale = 1.f / xmax;
                i__1 = nw - 1;
                for (jw = 0; jw <= i__1; ++jw) {
                    i__2 = iend;
                    for (jr = 1; jr <= i__2; ++jr) {
                        vr[jr + (ieig + jw) * vr_dim1] = xscale * vr[jr + (
                                ieig + jw) * vr_dim1];
/* L480: */
                    }
/* L490: */
                }
            }
L500:
            ;
        }
    }

    return 0;

/*     End of STGEVC */

} /* stgevc_ */

