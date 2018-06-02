/* ../../../dependencies/lapack/src/zlarfg.f -- translated by f2c (version 20061008).
   You must link the resulting object file with libf2c:
        on Microsoft Windows system, link with libf2c.lib;
        on Linux or Unix systems, link with .../path/to/libf2c.a -lm
        or, if you install libf2c.a in a standard place, with -lf2c -lm
        -- in that order, at the end of the command line, as in
                cc *.o -lf2c -lm
        Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

                http://www.netlib.org/f2c/libf2c.zip
*/




/* Subroutine */ static int zlarfg_(integer *n, dcomplex *alpha, dcomplex *
        x, integer *incx, dcomplex *tau)
{
/* Table of constant values */

static dcomplex c_b5 = {1.,0.};
    /* System generated locals */
    integer i__1;
    doublereal d__1, d__2;
    dcomplex z__1, z__2;

    /* Builtin functions */
    double d_imag(dcomplex *), d_sign(doublereal *, doublereal *);

    /* Local variables */
    static integer j, knt;
    static doublereal beta, alphi, alphr;
     /* Subroutine */ int zscal_(integer *, dcomplex *, 
            dcomplex *, integer *);
    static doublereal xnorm;
     doublereal dlapy3_(doublereal *, doublereal *, doublereal *), 
            dznrm2_(integer *, dcomplex *, integer *), dlamch_(char *, 
            ftnlen);
    static doublereal safmin;
     /* Subroutine */ int zdscal_(integer *, doublereal *, 
            dcomplex *, integer *);
    static doublereal rsafmn;
     /* Double Complex */ VOID zladiv_(dcomplex *, dcomplex *,
             dcomplex *);


/*  -- LAPACK auxiliary routine (version 3.0) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd., */
/*     Courant Institute, Argonne National Lab, and Rice University */
/*     September 30, 1994 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  ZLARFG generates a scomplex elementary reflector H of order n, such */
/*  that */

/*        H' * ( alpha ) = ( beta ),   H' * H = I. */
/*             (   x   )   (   0  ) */

/*  where alpha and beta are scalars, with beta real, and x is an */
/*  (n-1)-element scomplex vector. H is represented in the form */

/*        H = I - tau * ( 1 ) * ( 1 v' ) , */
/*                      ( v ) */

/*  where tau is a scomplex scalar and v is a scomplex (n-1)-element */
/*  vector. Note that H is not hermitian. */

/*  If the elements of x are all zero and alpha is real, then tau = 0 */
/*  and H is taken to be the unit matrix. */

/*  Otherwise  1 <= real(tau) <= 2  and  ABS(tau-1) <= 1 . */

/*  Arguments */
/*  ========= */

/*  N       (input) INTEGER */
/*          The order of the elementary reflector. */

/*  ALPHA   (input/output) COMPLEX*16 */
/*          On entry, the value alpha. */
/*          On exit, it is overwritten with the value beta. */

/*  X       (input/output) COMPLEX*16 array, dimension */
/*                         (1+(N-2)*ABS(INCX)) */
/*          On entry, the vector x. */
/*          On exit, it is overwritten with the vector v. */

/*  INCX    (input) INTEGER */
/*          The increment between elements of X. INCX > 0. */

/*  TAU     (output) COMPLEX*16 */
/*          The value tau. */

/*  ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Executable Statements .. */

    /* Parameter adjustments */
    --x;

    /* Function Body */
    if (*n <= 0) {
        tau->r = 0., tau->i = 0.;
        return 0;
    }

    i__1 = *n - 1;
    xnorm = dznrm2_(&i__1, &x[1], incx);
    alphr = alpha->r;
    alphi = d_imag(alpha);

    if (xnorm == 0. && alphi == 0.) {

/*        H  =  I */

        tau->r = 0., tau->i = 0.;
    } else {

/*        general case */

        d__1 = dlapy3_(&alphr, &alphi, &xnorm);
        beta = -d_sign(&d__1, &alphr);
        safmin = dlamch_("S", (ftnlen)1) / dlamch_("E", (ftnlen)1);
        rsafmn = 1. / safmin;

        if (ABS(beta) < safmin) {

/*           XNORM, BETA may be inaccurate; scale X and recompute them */

            knt = 0;
L10:
            ++knt;
            i__1 = *n - 1;
            zdscal_(&i__1, &rsafmn, &x[1], incx);
            beta *= rsafmn;
            alphi *= rsafmn;
            alphr *= rsafmn;
            if (ABS(beta) < safmin) {
                goto L10;
            }

/*           New BETA is at most 1, at least SAFMIN */

            i__1 = *n - 1;
            xnorm = dznrm2_(&i__1, &x[1], incx);
            z__1.r = alphr, z__1.i = alphi;
            alpha->r = z__1.r, alpha->i = z__1.i;
            d__1 = dlapy3_(&alphr, &alphi, &xnorm);
            beta = -d_sign(&d__1, &alphr);
            d__1 = (beta - alphr) / beta;
            d__2 = -alphi / beta;
            z__1.r = d__1, z__1.i = d__2;
            tau->r = z__1.r, tau->i = z__1.i;
            z__2.r = alpha->r - beta, z__2.i = alpha->i;
            zladiv_(&z__1, &c_b5, &z__2);
            alpha->r = z__1.r, alpha->i = z__1.i;
            i__1 = *n - 1;
            zscal_(&i__1, alpha, &x[1], incx);

/*           If ALPHA is subnormal, it may lose relative accuracy */

            alpha->r = beta, alpha->i = 0.;
            i__1 = knt;
            for (j = 1; j <= i__1; ++j) {
                z__1.r = safmin * alpha->r, z__1.i = safmin * alpha->i;
                alpha->r = z__1.r, alpha->i = z__1.i;
/* L20: */
            }
        } else {
            d__1 = (beta - alphr) / beta;
            d__2 = -alphi / beta;
            z__1.r = d__1, z__1.i = d__2;
            tau->r = z__1.r, tau->i = z__1.i;
            z__2.r = alpha->r - beta, z__2.i = alpha->i;
            zladiv_(&z__1, &c_b5, &z__2);
            alpha->r = z__1.r, alpha->i = z__1.i;
            i__1 = *n - 1;
            zscal_(&i__1, alpha, &x[1], incx);
            alpha->r = beta, alpha->i = 0.;
        }
    }

    return 0;

/*     End of ZLARFG */

} /* zlarfg_ */

