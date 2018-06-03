/* ../../../dependencies/lapack/src/clanhe.f -- translated by f2c (version 20061008).
   You must link the resulting object file with libf2c:
        on Microsoft Windows system, link with libf2c.lib;
        on Linux or Unix systems, link with .../path/to/libf2c.a -lm
        or, if you install libf2c.a in a standard place, with -lf2c -lm
        -- in that order, at the end of the command line, as in
                cc *.o -lf2c -lm
        Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

                http://www.netlib.org/f2c/libf2c.zip
*/



static doublereal clanhe_(char *norm, char *uplo, integer *n, scomplex *a, integer *
        lda, real *work, ftnlen norm_len, ftnlen uplo_len)
{
/* Table of constant values */

static integer c__1 = 1;
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2;
    real ret_val, r__1, r__2, r__3;

    /* Builtin functions */
    double c_abs(scomplex *), sqrt(doublereal);

    /* Local variables */
    static integer i__, j;
    static real sum, absa, scale;
     logical lsame_(char *, char *, ftnlen, ftnlen);
    static real value;
     /* Subroutine */ int classq_(integer *, scomplex *, integer *, real 
            *, real *);


/*  -- LAPACK auxiliary routine (version 3.0) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd., */
/*     Courant Institute, Argonne National Lab, and Rice University */
/*     October 31, 1992 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  CLANHE  returns the value of the one norm,  or the Frobenius norm, or */
/*  the  infinity norm,  or the  element of  largest absolute value  of a */
/*  scomplex hermitian matrix A. */

/*  Description */
/*  =========== */

/*  CLANHE returns the value */

/*     CLANHE = ( MAX(ABS(A(i,j))), NORM = 'M' or 'm' */
/*              ( */
/*              ( norm1(A),         NORM = '1', 'O' or 'o' */
/*              ( */
/*              ( normI(A),         NORM = 'I' or 'i' */
/*              ( */
/*              ( normF(A),         NORM = 'F', 'f', 'E' or 'e' */

/*  where  norm1  denotes the  one norm of a matrix (maximum column sum), */
/*  normI  denotes the  infinity norm  of a matrix  (maximum row sum) and */
/*  normF  denotes the  Frobenius norm of a matrix (square root of sum of */
/*  squares).  Note that  MAX(ABS(A(i,j)))  is not a  matrix norm. */

/*  Arguments */
/*  ========= */

/*  NORM    (input) CHARACTER*1 */
/*          Specifies the value to be returned in CLANHE as described */
/*          above. */

/*  UPLO    (input) CHARACTER*1 */
/*          Specifies whether the upper or lower triangular part of the */
/*          hermitian matrix A is to be referenced. */
/*          = 'U':  Upper triangular part of A is referenced */
/*          = 'L':  Lower triangular part of A is referenced */

/*  N       (input) INTEGER */
/*          The order of the matrix A.  N >= 0.  When N = 0, CLANHE is */
/*          set to zero. */

/*  A       (input) COMPLEX array, dimension (LDA,N) */
/*          The hermitian matrix A.  If UPLO = 'U', the leading n by n */
/*          upper triangular part of A contains the upper triangular part */
/*          of the matrix A, and the strictly lower triangular part of A */
/*          is not referenced.  If UPLO = 'L', the leading n by n lower */
/*          triangular part of A contains the lower triangular part of */
/*          the matrix A, and the strictly upper triangular part of A is */
/*          not referenced. Note that the imaginary parts of the diagonal */
/*          elements need not be set and are assumed to be zero. */

/*  LDA     (input) INTEGER */
/*          The leading dimension of the array A.  LDA >= MAX(N,1). */

/*  WORK    (workspace) REAL array, dimension (LWORK), */
/*          where LWORK >= N when NORM = 'I' or '1' or 'O'; otherwise, */
/*          WORK is not referenced. */

/* ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --work;

    /* Function Body */
    if (*n == 0) {
        value = 0.f;
    } else if (lsame_(norm, "M", (ftnlen)1, (ftnlen)1)) {

/*        Find MAX(ABS(A(i,j))). */

        value = 0.f;
        if (lsame_(uplo, "U", (ftnlen)1, (ftnlen)1)) {
            i__1 = *n;
            for (j = 1; j <= i__1; ++j) {
                i__2 = j - 1;
                for (i__ = 1; i__ <= i__2; ++i__) {
/* Computing MAX */
                    r__1 = value, r__2 = (real)c_abs(&a[i__ + j * a_dim1]);
                    value = dmax(r__1,r__2);
/* L10: */
                }
/* Computing MAX */
                i__2 = j + j * a_dim1;
                r__2 = value, r__3 = (r__1 = a[i__2].r, SABS(r__1));
                value = dmax(r__2,r__3);
/* L20: */
            }
        } else {
            i__1 = *n;
            for (j = 1; j <= i__1; ++j) {
/* Computing MAX */
                i__2 = j + j * a_dim1;
                r__2 = value, r__3 = (r__1 = a[i__2].r, SABS(r__1));
                value = dmax(r__2,r__3);
                i__2 = *n;
                for (i__ = j + 1; i__ <= i__2; ++i__) {
/* Computing MAX */
                    r__1 = value, r__2 = (real)c_abs(&a[i__ + j * a_dim1]);
                    value = dmax(r__1,r__2);
/* L30: */
                }
/* L40: */
            }
        }
    } else if (lsame_(norm, "I", (ftnlen)1, (ftnlen)1) || lsame_(norm, "O", (
            ftnlen)1, (ftnlen)1) || *(unsigned char *)norm == '1') {

/*        Find normI(A) ( = norm1(A), since A is hermitian). */

        value = 0.f;
        if (lsame_(uplo, "U", (ftnlen)1, (ftnlen)1)) {
            i__1 = *n;
            for (j = 1; j <= i__1; ++j) {
                sum = 0.f;
                i__2 = j - 1;
                for (i__ = 1; i__ <= i__2; ++i__) {
                    absa = (real)c_abs(&a[i__ + j * a_dim1]);
                    sum += absa;
                    work[i__] += absa;
/* L50: */
                }
                i__2 = j + j * a_dim1;
                work[j] = sum + (r__1 = a[i__2].r, SABS(r__1));
/* L60: */
            }
            i__1 = *n;
            for (i__ = 1; i__ <= i__1; ++i__) {
/* Computing MAX */
                r__1 = value, r__2 = work[i__];
                value = dmax(r__1,r__2);
/* L70: */
            }
        } else {
            i__1 = *n;
            for (i__ = 1; i__ <= i__1; ++i__) {
                work[i__] = 0.f;
/* L80: */
            }
            i__1 = *n;
            for (j = 1; j <= i__1; ++j) {
                i__2 = j + j * a_dim1;
                sum = work[j] + (r__1 = a[i__2].r, SABS(r__1));
                i__2 = *n;
                for (i__ = j + 1; i__ <= i__2; ++i__) {
                    absa = (real)c_abs(&a[i__ + j * a_dim1]);
                    sum += absa;
                    work[i__] += absa;
/* L90: */
                }
                value = dmax(value,sum);
/* L100: */
            }
        }
    } else if (lsame_(norm, "F", (ftnlen)1, (ftnlen)1) || lsame_(norm, "E", (
            ftnlen)1, (ftnlen)1)) {

/*        Find normF(A). */

        scale = 0.f;
        sum = 1.f;
        if (lsame_(uplo, "U", (ftnlen)1, (ftnlen)1)) {
            i__1 = *n;
            for (j = 2; j <= i__1; ++j) {
                i__2 = j - 1;
                classq_(&i__2, &a[j * a_dim1 + 1], &c__1, &scale, &sum);
/* L110: */
            }
        } else {
            i__1 = *n - 1;
            for (j = 1; j <= i__1; ++j) {
                i__2 = *n - j;
                classq_(&i__2, &a[j + 1 + j * a_dim1], &c__1, &scale, &sum);
/* L120: */
            }
        }
        sum *= 2;
        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            i__2 = i__ + i__ * a_dim1;
            if (a[i__2].r != 0.f) {
                i__2 = i__ + i__ * a_dim1;
                absa = (r__1 = a[i__2].r, SABS(r__1));
                if (scale < absa) {
/* Computing 2nd power */
                    r__1 = scale / absa;
                    sum = sum * (r__1 * r__1) + 1.f;
                    scale = absa;
                } else {
/* Computing 2nd power */
                    r__1 = absa / scale;
                    sum += r__1 * r__1;
                }
            }
/* L130: */
        }
        value = (real)(scale * sqrt(sum));
    }

    ret_val = value;
    return ret_val;

/*     End of CLANHE */

} /* clanhe_ */
