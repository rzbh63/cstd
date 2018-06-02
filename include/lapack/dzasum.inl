/* ../blas/src/dzasum.f -- translated by f2c (version 20061008).
   You must link the resulting object file with libf2c:
        on Microsoft Windows system, link with libf2c.lib;
        on Linux or Unix systems, link with .../path/to/libf2c.a -lm
        or, if you install libf2c.a in a standard place, with -lf2c -lm
        -- in that order, at the end of the command line, as in
                cc *.o -lf2c -lm
        Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

                http://www.netlib.org/f2c/libf2c.zip
*/


static doublereal dzasum_(integer *n, dcomplex *zx, integer *incx)
{
    /* System generated locals */
    integer i__1;
    doublereal ret_val;

    /* Local variables */
    static integer i__, ix;
    static doublereal stemp;
     doublereal dcabs1_(dcomplex *);


/*     takes the sum of the absolute values. */
/*     jack dongarra, 3/11/78. */
/*     modified 3/93 to return if incx .le. 0. */
/*     modified 12/3/93, array(1) declarations changed to array(*) */


    /* Parameter adjustments */
    --zx;

    /* Function Body */
    ret_val = 0.;
    stemp = 0.;
    if (*n <= 0 || *incx <= 0) {
        return ret_val;
    }
    if (*incx == 1) {
        goto L20;
    }

/*        code for increment not equal to 1 */

    ix = 1;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
        stemp += dcabs1_(&zx[ix]);
        ix += *incx;
/* L10: */
    }
    ret_val = stemp;
    return ret_val;

/*        code for increment equal to 1 */

L20:
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
        stemp += dcabs1_(&zx[i__]);
/* L30: */
    }
    ret_val = stemp;
    return ret_val;
} /* dzasum_ */

