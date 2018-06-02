/* ../../../dependencies/lapack/src/ztrsyl.f -- translated by f2c (version 20061008).
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

static integer c__1 = 1;

/* Subroutine */ static int ztrsyl_(char *trana, char *tranb, integer *isgn, integer 
        *m, integer *n, dcomplex *a, integer *lda, dcomplex *b, 
        integer *ldb, dcomplex *c__, integer *ldc, doublereal *scale, 
        integer *info, ftnlen trana_len, ftnlen tranb_len)
{
    /* System generated locals */
    integer a_dim1, a_offset, b_dim1, b_offset, c_dim1, c_offset, i__1, i__2, 
            i__3, i__4;
    doublereal d__1, d__2;
    dcomplex z__1, z__2, z__3, z__4;

    /* Builtin functions */
    double d_imag(dcomplex *);
    void d_cnjg(dcomplex *, dcomplex *);

    /* Local variables */
    static integer j, k, l;
    static dcomplex a11;
    static doublereal db;
    static dcomplex x11;
    static doublereal da11;
    static dcomplex vec;
    static doublereal dum[1], eps, sgn, smin;
    static dcomplex suml, sumr;
     logical lsame_(char *, char *, ftnlen, ftnlen);
     /* Double Complex */ VOID zdotc_(dcomplex *, integer *, 
            dcomplex *, integer *, dcomplex *, integer *), zdotu_(
            dcomplex *, integer *, dcomplex *, integer *, 
            dcomplex *, integer *);
     /* Subroutine */ int dlabad_(doublereal *, doublereal *);
     doublereal dlamch_(char *, ftnlen);
    static doublereal scaloc;
     /* Subroutine */ int xerbla_(char *, integer *, ftnlen);
     doublereal zlange_(char *, integer *, integer *, dcomplex *, 
            integer *, doublereal *, ftnlen);
    static doublereal bignum;
     /* Subroutine */ int zdscal_(integer *, doublereal *, 
            dcomplex *, integer *);
     /* Double Complex */ VOID zladiv_(dcomplex *, dcomplex *,
             dcomplex *);
    static logical notrna, notrnb;
    static doublereal smlnum;


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

/*  ZTRSYL solves the scomplex Sylvester matrix equation: */

/*     op(A)*X + X*op(B) = scale*C or */
/*     op(A)*X - X*op(B) = scale*C, */

/*  where op(A) = A or A**H, and A and B are both upper triangular. A is */
/*  M-by-M and B is N-by-N; the right hand side C and the solution X are */
/*  M-by-N; and scale is an output scale factor, set <= 1 to avoid */
/*  overflow in X. */

/*  Arguments */
/*  ========= */

/*  TRANA   (input) CHARACTER*1 */
/*          Specifies the option op(A): */
/*          = 'N': op(A) = A    (No transpose) */
/*          = 'C': op(A) = A**H (Conjugate transpose) */

/*  TRANB   (input) CHARACTER*1 */
/*          Specifies the option op(B): */
/*          = 'N': op(B) = B    (No transpose) */
/*          = 'C': op(B) = B**H (Conjugate transpose) */

/*  ISGN    (input) INTEGER */
/*          Specifies the sign in the equation: */
/*          = +1: solve op(A)*X + X*op(B) = scale*C */
/*          = -1: solve op(A)*X - X*op(B) = scale*C */

/*  M       (input) INTEGER */
/*          The order of the matrix A, and the number of rows in the */
/*          matrices X and C. M >= 0. */

/*  N       (input) INTEGER */
/*          The order of the matrix B, and the number of columns in the */
/*          matrices X and C. N >= 0. */

/*  A       (input) COMPLEX*16 array, dimension (LDA,M) */
/*          The upper triangular matrix A. */

/*  LDA     (input) INTEGER */
/*          The leading dimension of the array A. LDA >= MAX(1,M). */

/*  B       (input) COMPLEX*16 array, dimension (LDB,N) */
/*          The upper triangular matrix B. */

/*  LDB     (input) INTEGER */
/*          The leading dimension of the array B. LDB >= MAX(1,N). */

/*  C       (input/output) COMPLEX*16 array, dimension (LDC,N) */
/*          On entry, the M-by-N right hand side matrix C. */
/*          On exit, C is overwritten by the solution matrix X. */

/*  LDC     (input) INTEGER */
/*          The leading dimension of the array C. LDC >= MAX(1,M) */

/*  SCALE   (output) DOUBLE PRECISION */
/*          The scale factor, scale, set <= 1 to avoid overflow in X. */

/*  INFO    (output) INTEGER */
/*          = 0: successful exit */
/*          < 0: if INFO = -i, the i-th argument had an illegal value */
/*          = 1: A and B have common or very close eigenvalues; perturbed */
/*               values were used to solve the equation (but the matrices */
/*               A and B are unchanged). */

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

/*     Decode and Test input parameters */

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    b_dim1 = *ldb;
    b_offset = 1 + b_dim1;
    b -= b_offset;
    c_dim1 = *ldc;
    c_offset = 1 + c_dim1;
    c__ -= c_offset;

    /* Function Body */
    notrna = lsame_(trana, "N", (ftnlen)1, (ftnlen)1);
    notrnb = lsame_(tranb, "N", (ftnlen)1, (ftnlen)1);

    *info = 0;
    if (! notrna && ! lsame_(trana, "T", (ftnlen)1, (ftnlen)1) && ! lsame_(
            trana, "C", (ftnlen)1, (ftnlen)1)) {
        *info = -1;
    } else if (! notrnb && ! lsame_(tranb, "T", (ftnlen)1, (ftnlen)1) && ! 
            lsame_(tranb, "C", (ftnlen)1, (ftnlen)1)) {
        *info = -2;
    } else if (*isgn != 1 && *isgn != -1) {
        *info = -3;
    } else if (*m < 0) {
        *info = -4;
    } else if (*n < 0) {
        *info = -5;
    } else if (*lda < MAX(1,*m)) {
        *info = -7;
    } else if (*ldb < MAX(1,*n)) {
        *info = -9;
    } else if (*ldc < MAX(1,*m)) {
        *info = -11;
    }
    if (*info != 0) {
        i__1 = -(*info);
        xerbla_("ZTRSYL", &i__1, (ftnlen)6);
        return 0;
    }

/*     Quick return if possible */

    if (*m == 0 || *n == 0) {
        return 0;
    }

/*     Set constants to control overflow */

    eps = dlamch_("P", (ftnlen)1);
    smlnum = dlamch_("S", (ftnlen)1);
    bignum = 1. / smlnum;
    dlabad_(&smlnum, &bignum);
    smlnum = smlnum * (doublereal) (*m * *n) / eps;
    bignum = 1. / smlnum;
/* Computing MAX */
    d__1 = smlnum, d__2 = eps * zlange_("M", m, m, &a[a_offset], lda, dum, (
            ftnlen)1), d__1 = MAX(d__1,d__2), d__2 = eps * zlange_("M", n, n, 
            &b[b_offset], ldb, dum, (ftnlen)1);
    smin = MAX(d__1,d__2);
    *scale = 1.;
    sgn = (doublereal) (*isgn);

    if (notrna && notrnb) {

/*        Solve    A*X + ISGN*X*B = scale*C. */

/*        The (K,L)th block of X is determined starting from */
/*        bottom-left corner column by column by */

/*            A(K,K)*X(K,L) + ISGN*X(K,L)*B(L,L) = C(K,L) - R(K,L) */

/*        Where */
/*                    M                        L-1 */
/*          R(K,L) = SUM [A(K,I)*X(I,L)] +ISGN*SUM [X(K,J)*B(J,L)]. */
/*                  I=K+1                      J=1 */

        i__1 = *n;
        for (l = 1; l <= i__1; ++l) {
            for (k = *m; k >= 1; --k) {

                i__2 = *m - k;
/* Computing MIN */
                i__3 = k + 1;
/* Computing MIN */
                i__4 = k + 1;
                zdotu_(&z__1, &i__2, &a[k + MIN(i__3,*m) * a_dim1], lda, &c__[
                        MIN(i__4,*m) + l * c_dim1], &c__1);
                suml.r = z__1.r, suml.i = z__1.i;
                i__2 = l - 1;
                zdotu_(&z__1, &i__2, &c__[k + c_dim1], ldc, &b[l * b_dim1 + 1]
                        , &c__1);
                sumr.r = z__1.r, sumr.i = z__1.i;
                i__2 = k + l * c_dim1;
                z__3.r = sgn * sumr.r, z__3.i = sgn * sumr.i;
                z__2.r = suml.r + z__3.r, z__2.i = suml.i + z__3.i;
                z__1.r = c__[i__2].r - z__2.r, z__1.i = c__[i__2].i - z__2.i;
                vec.r = z__1.r, vec.i = z__1.i;

                scaloc = 1.;
                i__2 = k + k * a_dim1;
                i__3 = l + l * b_dim1;
                z__2.r = sgn * b[i__3].r, z__2.i = sgn * b[i__3].i;
                z__1.r = a[i__2].r + z__2.r, z__1.i = a[i__2].i + z__2.i;
                a11.r = z__1.r, a11.i = z__1.i;
                da11 = (d__1 = a11.r, ABS(d__1)) + (d__2 = d_imag(&a11), ABS(
                        d__2));
                if (da11 <= smin) {
                    a11.r = smin, a11.i = 0.;
                    da11 = smin;
                    *info = 1;
                }
                db = (d__1 = vec.r, ABS(d__1)) + (d__2 = d_imag(&vec), ABS(
                        d__2));
                if (da11 < 1. && db > 1.) {
                    if (db > bignum * da11) {
                        scaloc = 1. / db;
                    }
                }
                z__3.r = scaloc, z__3.i = 0.;
                z__2.r = vec.r * z__3.r - vec.i * z__3.i, z__2.i = vec.r * 
                        z__3.i + vec.i * z__3.r;
                zladiv_(&z__1, &z__2, &a11);
                x11.r = z__1.r, x11.i = z__1.i;

                if (scaloc != 1.) {
                    i__2 = *n;
                    for (j = 1; j <= i__2; ++j) {
                        zdscal_(m, &scaloc, &c__[j * c_dim1 + 1], &c__1);
/* L10: */
                    }
                    *scale *= scaloc;
                }
                i__2 = k + l * c_dim1;
                c__[i__2].r = x11.r, c__[i__2].i = x11.i;

/* L20: */
            }
/* L30: */
        }

    } else if (! notrna && notrnb) {

/*        Solve    A' *X + ISGN*X*B = scale*C. */

/*        The (K,L)th block of X is determined starting from */
/*        upper-left corner column by column by */

/*            A'(K,K)*X(K,L) + ISGN*X(K,L)*B(L,L) = C(K,L) - R(K,L) */

/*        Where */
/*                   K-1                         L-1 */
/*          R(K,L) = SUM [A'(I,K)*X(I,L)] + ISGN*SUM [X(K,J)*B(J,L)] */
/*                   I=1                         J=1 */

        i__1 = *n;
        for (l = 1; l <= i__1; ++l) {
            i__2 = *m;
            for (k = 1; k <= i__2; ++k) {

                i__3 = k - 1;
                zdotc_(&z__1, &i__3, &a[k * a_dim1 + 1], &c__1, &c__[l * 
                        c_dim1 + 1], &c__1);
                suml.r = z__1.r, suml.i = z__1.i;
                i__3 = l - 1;
                zdotu_(&z__1, &i__3, &c__[k + c_dim1], ldc, &b[l * b_dim1 + 1]
                        , &c__1);
                sumr.r = z__1.r, sumr.i = z__1.i;
                i__3 = k + l * c_dim1;
                z__3.r = sgn * sumr.r, z__3.i = sgn * sumr.i;
                z__2.r = suml.r + z__3.r, z__2.i = suml.i + z__3.i;
                z__1.r = c__[i__3].r - z__2.r, z__1.i = c__[i__3].i - z__2.i;
                vec.r = z__1.r, vec.i = z__1.i;

                scaloc = 1.;
                d_cnjg(&z__2, &a[k + k * a_dim1]);
                i__3 = l + l * b_dim1;
                z__3.r = sgn * b[i__3].r, z__3.i = sgn * b[i__3].i;
                z__1.r = z__2.r + z__3.r, z__1.i = z__2.i + z__3.i;
                a11.r = z__1.r, a11.i = z__1.i;
                da11 = (d__1 = a11.r, ABS(d__1)) + (d__2 = d_imag(&a11), ABS(
                        d__2));
                if (da11 <= smin) {
                    a11.r = smin, a11.i = 0.;
                    da11 = smin;
                    *info = 1;
                }
                db = (d__1 = vec.r, ABS(d__1)) + (d__2 = d_imag(&vec), ABS(
                        d__2));
                if (da11 < 1. && db > 1.) {
                    if (db > bignum * da11) {
                        scaloc = 1. / db;
                    }
                }

                z__3.r = scaloc, z__3.i = 0.;
                z__2.r = vec.r * z__3.r - vec.i * z__3.i, z__2.i = vec.r * 
                        z__3.i + vec.i * z__3.r;
                zladiv_(&z__1, &z__2, &a11);
                x11.r = z__1.r, x11.i = z__1.i;

                if (scaloc != 1.) {
                    i__3 = *n;
                    for (j = 1; j <= i__3; ++j) {
                        zdscal_(m, &scaloc, &c__[j * c_dim1 + 1], &c__1);
/* L40: */
                    }
                    *scale *= scaloc;
                }
                i__3 = k + l * c_dim1;
                c__[i__3].r = x11.r, c__[i__3].i = x11.i;

/* L50: */
            }
/* L60: */
        }

    } else if (! notrna && ! notrnb) {

/*        Solve    A'*X + ISGN*X*B' = C. */

/*        The (K,L)th block of X is determined starting from */
/*        upper-right corner column by column by */

/*            A'(K,K)*X(K,L) + ISGN*X(K,L)*B'(L,L) = C(K,L) - R(K,L) */

/*        Where */
/*                    K-1 */
/*           R(K,L) = SUM [A'(I,K)*X(I,L)] + */
/*                    I=1 */
/*                           N */
/*                     ISGN*SUM [X(K,J)*B'(L,J)]. */
/*                          J=L+1 */

        for (l = *n; l >= 1; --l) {
            i__1 = *m;
            for (k = 1; k <= i__1; ++k) {

                i__2 = k - 1;
                zdotc_(&z__1, &i__2, &a[k * a_dim1 + 1], &c__1, &c__[l * 
                        c_dim1 + 1], &c__1);
                suml.r = z__1.r, suml.i = z__1.i;
                i__2 = *n - l;
/* Computing MIN */
                i__3 = l + 1;
/* Computing MIN */
                i__4 = l + 1;
                zdotc_(&z__1, &i__2, &c__[k + MIN(i__3,*n) * c_dim1], ldc, &b[
                        l + MIN(i__4,*n) * b_dim1], ldb);
                sumr.r = z__1.r, sumr.i = z__1.i;
                i__2 = k + l * c_dim1;
                d_cnjg(&z__4, &sumr);
                z__3.r = sgn * z__4.r, z__3.i = sgn * z__4.i;
                z__2.r = suml.r + z__3.r, z__2.i = suml.i + z__3.i;
                z__1.r = c__[i__2].r - z__2.r, z__1.i = c__[i__2].i - z__2.i;
                vec.r = z__1.r, vec.i = z__1.i;

                scaloc = 1.;
                i__2 = k + k * a_dim1;
                i__3 = l + l * b_dim1;
                z__3.r = sgn * b[i__3].r, z__3.i = sgn * b[i__3].i;
                z__2.r = a[i__2].r + z__3.r, z__2.i = a[i__2].i + z__3.i;
                d_cnjg(&z__1, &z__2);
                a11.r = z__1.r, a11.i = z__1.i;
                da11 = (d__1 = a11.r, ABS(d__1)) + (d__2 = d_imag(&a11), ABS(
                        d__2));
                if (da11 <= smin) {
                    a11.r = smin, a11.i = 0.;
                    da11 = smin;
                    *info = 1;
                }
                db = (d__1 = vec.r, ABS(d__1)) + (d__2 = d_imag(&vec), ABS(
                        d__2));
                if (da11 < 1. && db > 1.) {
                    if (db > bignum * da11) {
                        scaloc = 1. / db;
                    }
                }

                z__3.r = scaloc, z__3.i = 0.;
                z__2.r = vec.r * z__3.r - vec.i * z__3.i, z__2.i = vec.r * 
                        z__3.i + vec.i * z__3.r;
                zladiv_(&z__1, &z__2, &a11);
                x11.r = z__1.r, x11.i = z__1.i;

                if (scaloc != 1.) {
                    i__2 = *n;
                    for (j = 1; j <= i__2; ++j) {
                        zdscal_(m, &scaloc, &c__[j * c_dim1 + 1], &c__1);
/* L70: */
                    }
                    *scale *= scaloc;
                }
                i__2 = k + l * c_dim1;
                c__[i__2].r = x11.r, c__[i__2].i = x11.i;

/* L80: */
            }
/* L90: */
        }

    } else if (notrna && ! notrnb) {

/*        Solve    A*X + ISGN*X*B' = C. */

/*        The (K,L)th block of X is determined starting from */
/*        bottom-left corner column by column by */

/*           A(K,K)*X(K,L) + ISGN*X(K,L)*B'(L,L) = C(K,L) - R(K,L) */

/*        Where */
/*                    M                          N */
/*          R(K,L) = SUM [A(K,I)*X(I,L)] + ISGN*SUM [X(K,J)*B'(L,J)] */
/*                  I=K+1                      J=L+1 */

        for (l = *n; l >= 1; --l) {
            for (k = *m; k >= 1; --k) {

                i__1 = *m - k;
/* Computing MIN */
                i__2 = k + 1;
/* Computing MIN */
                i__3 = k + 1;
                zdotu_(&z__1, &i__1, &a[k + MIN(i__2,*m) * a_dim1], lda, &c__[
                        MIN(i__3,*m) + l * c_dim1], &c__1);
                suml.r = z__1.r, suml.i = z__1.i;
                i__1 = *n - l;
/* Computing MIN */
                i__2 = l + 1;
/* Computing MIN */
                i__3 = l + 1;
                zdotc_(&z__1, &i__1, &c__[k + MIN(i__2,*n) * c_dim1], ldc, &b[
                        l + MIN(i__3,*n) * b_dim1], ldb);
                sumr.r = z__1.r, sumr.i = z__1.i;
                i__1 = k + l * c_dim1;
                d_cnjg(&z__4, &sumr);
                z__3.r = sgn * z__4.r, z__3.i = sgn * z__4.i;
                z__2.r = suml.r + z__3.r, z__2.i = suml.i + z__3.i;
                z__1.r = c__[i__1].r - z__2.r, z__1.i = c__[i__1].i - z__2.i;
                vec.r = z__1.r, vec.i = z__1.i;

                scaloc = 1.;
                i__1 = k + k * a_dim1;
                d_cnjg(&z__3, &b[l + l * b_dim1]);
                z__2.r = sgn * z__3.r, z__2.i = sgn * z__3.i;
                z__1.r = a[i__1].r + z__2.r, z__1.i = a[i__1].i + z__2.i;
                a11.r = z__1.r, a11.i = z__1.i;
                da11 = (d__1 = a11.r, ABS(d__1)) + (d__2 = d_imag(&a11), ABS(
                        d__2));
                if (da11 <= smin) {
                    a11.r = smin, a11.i = 0.;
                    da11 = smin;
                    *info = 1;
                }
                db = (d__1 = vec.r, ABS(d__1)) + (d__2 = d_imag(&vec), ABS(
                        d__2));
                if (da11 < 1. && db > 1.) {
                    if (db > bignum * da11) {
                        scaloc = 1. / db;
                    }
                }

                z__3.r = scaloc, z__3.i = 0.;
                z__2.r = vec.r * z__3.r - vec.i * z__3.i, z__2.i = vec.r * 
                        z__3.i + vec.i * z__3.r;
                zladiv_(&z__1, &z__2, &a11);
                x11.r = z__1.r, x11.i = z__1.i;

                if (scaloc != 1.) {
                    i__1 = *n;
                    for (j = 1; j <= i__1; ++j) {
                        zdscal_(m, &scaloc, &c__[j * c_dim1 + 1], &c__1);
/* L100: */
                    }
                    *scale *= scaloc;
                }
                i__1 = k + l * c_dim1;
                c__[i__1].r = x11.r, c__[i__1].i = x11.i;

/* L110: */
            }
/* L120: */
        }

    }

    return 0;

/*     End of ZTRSYL */

} /* ztrsyl_ */

