/* ../../../dependencies/lapack/src/cgeqrf.f -- translated by f2c (version 20061008).
   You must link the resulting object file with libf2c:
        on Microsoft Windows system, link with libf2c.lib;
        on Linux or Unix systems, link with .../path/to/libf2c.a -lm
        or, if you install libf2c.a in a standard place, with -lf2c -lm
        -- in that order, at the end of the command line, as in
                cc *.o -lf2c -lm
        Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

                http://www.netlib.org/f2c/libf2c.zip
*/




/* Subroutine */ static int cgeqrf_(integer *m, integer *n, scomplex *a, integer *lda,
         scomplex *tau, scomplex *work, integer *lwork, integer *info)
{
/* Table of constant values */

static integer c__1 = 1;
static integer c_n1 = -1;
static integer c__3 = 3;
static integer c__2 = 2;
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3, i__4;

    /* Local variables */
    static integer i__, k, ib, nb, nx, iws, nbmin, iinfo;
     /* Subroutine */ int cgeqr2_(integer *, integer *, scomplex *, 
            integer *, scomplex *, scomplex *, integer *), clarfb_(char *, char 
            *, char *, char *, integer *, integer *, integer *, scomplex *, 
            integer *, scomplex *, integer *, scomplex *, integer *, scomplex *, 
            integer *, ftnlen, ftnlen, ftnlen, ftnlen), clarft_(char *, char *
            , integer *, integer *, scomplex *, integer *, scomplex *, scomplex *
            , integer *, ftnlen, ftnlen), xerbla_(char *, integer *, ftnlen);
     integer ilaenv_(integer *, char *, char *, integer *, integer *, 
            integer *, integer *, ftnlen, ftnlen);
    static integer ldwork, lwkopt;
    static logical lquery;


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

/*  CGEQRF computes a QR factorization of a scomplex M-by-N matrix A: */
/*  A = Q * R. */

/*  Arguments */
/*  ========= */

/*  M       (input) INTEGER */
/*          The number of rows of the matrix A.  M >= 0. */

/*  N       (input) INTEGER */
/*          The number of columns of the matrix A.  N >= 0. */

/*  A       (input/output) COMPLEX array, dimension (LDA,N) */
/*          On entry, the M-by-N matrix A. */
/*          On exit, the elements on and above the diagonal of the array */
/*          contain the MIN(M,N)-by-N upper trapezoidal matrix R (R is */
/*          upper triangular if m >= n); the elements below the diagonal, */
/*          with the array TAU, represent the unitary matrix Q as a */
/*          product of MIN(m,n) elementary reflectors (see Further */
/*          Details). */

/*  LDA     (input) INTEGER */
/*          The leading dimension of the array A.  LDA >= MAX(1,M). */

/*  TAU     (output) COMPLEX array, dimension (MIN(M,N)) */
/*          The scalar factors of the elementary reflectors (see Further */
/*          Details). */

/*  WORK    (workspace/output) COMPLEX array, dimension (LWORK) */
/*          On exit, if INFO = 0, WORK(1) returns the optimal LWORK. */

/*  LWORK   (input) INTEGER */
/*          The dimension of the array WORK.  LWORK >= MAX(1,N). */
/*          For optimum performance LWORK >= N*NB, where NB is */
/*          the optimal blocksize. */

/*          If LWORK = -1, then a workspace query is assumed; the routine */
/*          only calculates the optimal size of the WORK array, returns */
/*          this value as the first entry of the WORK array, and no error */
/*          message related to LWORK is issued by XERBLA. */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value */

/*  Further Details */
/*  =============== */

/*  The matrix Q is represented as a product of elementary reflectors */

/*     Q = H(1) H(2) . . . H(k), where k = MIN(m,n). */

/*  Each H(i) has the form */

/*     H(i) = I - tau * v * v' */

/*  where tau is a scomplex scalar, and v is a scomplex vector with */
/*  v(1:i-1) = 0 and v(i) = 1; v(i+1:m) is stored on exit in A(i+1:m,i), */
/*  and tau in TAU(i). */

/*  ===================================================================== */

/*     .. Local Scalars .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Test the input arguments */

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --tau;
    --work;

    /* Function Body */
    *info = 0;
    nb = ilaenv_(&c__1, "CGEQRF", " ", m, n, &c_n1, &c_n1, (ftnlen)6, (ftnlen)
            1);
    lwkopt = *n * nb;
    work[1].r = (real) lwkopt, work[1].i = 0.f;
    lquery = *lwork == -1;
    if (*m < 0) {
        *info = -1;
    } else if (*n < 0) {
        *info = -2;
    } else if (*lda < MAX(1,*m)) {
        *info = -4;
    } else if (*lwork < MAX(1,*n) && ! lquery) {
        *info = -7;
    }
    if (*info != 0) {
        i__1 = -(*info);
        xerbla_("CGEQRF", &i__1, (ftnlen)6);
        return 0;
    } else if (lquery) {
        return 0;
    }

/*     Quick return if possible */

    k = MIN(*m,*n);
    if (k == 0) {
        work[1].r = 1.f, work[1].i = 0.f;
        return 0;
    }

    nbmin = 2;
    nx = 0;
    iws = *n;
    if (nb > 1 && nb < k) {

/*        Determine when to cross over from blocked to unblocked code. */

/* Computing MAX */
        i__1 = 0, i__2 = ilaenv_(&c__3, "CGEQRF", " ", m, n, &c_n1, &c_n1, (
                ftnlen)6, (ftnlen)1);
        nx = MAX(i__1,i__2);
        if (nx < k) {

/*           Determine if workspace is large enough for blocked code. */

            ldwork = *n;
            iws = ldwork * nb;
            if (*lwork < iws) {

/*              Not enough workspace to use optimal NB:  reduce NB and */
/*              determine the minimum value of NB. */

                nb = *lwork / ldwork;
/* Computing MAX */
                i__1 = 2, i__2 = ilaenv_(&c__2, "CGEQRF", " ", m, n, &c_n1, &
                        c_n1, (ftnlen)6, (ftnlen)1);
                nbmin = MAX(i__1,i__2);
            }
        }
    }

    if (nb >= nbmin && nb < k && nx < k) {

/*        Use blocked code initially */

        i__1 = k - nx;
        i__2 = nb;
        for (i__ = 1; i__2 < 0 ? i__ >= i__1 : i__ <= i__1; i__ += i__2) {
/* Computing MIN */
            i__3 = k - i__ + 1;
            ib = MIN(i__3,nb);

/*           Compute the QR factorization of the current block */
/*           A(i:m,i:i+ib-1) */

            i__3 = *m - i__ + 1;
            cgeqr2_(&i__3, &ib, &a[i__ + i__ * a_dim1], lda, &tau[i__], &work[
                    1], &iinfo);
            if (i__ + ib <= *n) {

/*              Form the triangular factor of the block reflector */
/*              H = H(i) H(i+1) . . . H(i+ib-1) */

                i__3 = *m - i__ + 1;
                clarft_("Forward", "Columnwise", &i__3, &ib, &a[i__ + i__ * 
                        a_dim1], lda, &tau[i__], &work[1], &ldwork, (ftnlen)7,
                         (ftnlen)10);

/*              Apply H' to A(i:m,i+ib:n) from the left */

                i__3 = *m - i__ + 1;
                i__4 = *n - i__ - ib + 1;
                clarfb_("Left", "Conjugate transpose", "Forward", "Columnwise"
                        , &i__3, &i__4, &ib, &a[i__ + i__ * a_dim1], lda, &
                        work[1], &ldwork, &a[i__ + (i__ + ib) * a_dim1], lda, 
                        &work[ib + 1], &ldwork, (ftnlen)4, (ftnlen)19, (
                        ftnlen)7, (ftnlen)10);
            }
/* L10: */
        }
    } else {
        i__ = 1;
    }

/*     Use unblocked code to factor the last or only block. */

    if (i__ <= k) {
        i__2 = *m - i__ + 1;
        i__1 = *n - i__ + 1;
        cgeqr2_(&i__2, &i__1, &a[i__ + i__ * a_dim1], lda, &tau[i__], &work[1]
                , &iinfo);
    }

    work[1].r = (real) iws, work[1].i = 0.f;
    return 0;

/*     End of CGEQRF */

} /* cgeqrf_ */

