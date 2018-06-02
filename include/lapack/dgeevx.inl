/* ../../../dependencies/lapack/src/dgeevx.f -- translated by f2c (version 20061008).
   You must link the resulting object file with libf2c:
        on Microsoft Windows system, link with libf2c.lib;
        on Linux or Unix systems, link with .../path/to/libf2c.a -lm
        or, if you install libf2c.a in a standard place, with -lf2c -lm
        -- in that order, at the end of the command line, as in
                cc *.o -lf2c -lm
        Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

                http://www.netlib.org/f2c/libf2c.zip
*/




/* Subroutine */ static int dgeevx_(char *balanc, char *jobvl, char *jobvr, char *
        sense, integer *n, doublereal *a, integer *lda, doublereal *wr, 
        doublereal *wi, doublereal *vl, integer *ldvl, doublereal *vr, 
        integer *ldvr, integer *ilo, integer *ihi, doublereal *scale, 
        doublereal *abnrm, doublereal *rconde, doublereal *rcondv, doublereal 
        *work, integer *lwork, integer *iwork, integer *info, ftnlen 
        balanc_len, ftnlen jobvl_len, ftnlen jobvr_len, ftnlen sense_len)
{
/* Table of constant values */

static integer c__1 = 1;
static integer c__0 = 0;
static integer c__8 = 8;
static integer c_n1 = -1;
static integer c__4 = 4;
    /* System generated locals */
    integer a_dim1, a_offset, vl_dim1, vl_offset, vr_dim1, vr_offset, i__1, 
            i__2, i__3, i__4;
    doublereal d__1, d__2;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer i__, k;
    static doublereal r__, cs, sn;
    static char job[1];
    static doublereal scl, dum[1], eps;
    static char side[1];
    static integer maxb;
    static doublereal anrm;
    static integer ierr, itau;
     /* Subroutine */ int drot_(integer *, doublereal *, integer *, 
            doublereal *, integer *, doublereal *, doublereal *);
    static integer iwrk, nout;
     doublereal dnrm2_(integer *, doublereal *, integer *);
     /* Subroutine */ int dscal_(integer *, doublereal *, doublereal *, 
            integer *);
    static integer icond;
     logical lsame_(char *, char *, ftnlen, ftnlen);
     doublereal dlapy2_(doublereal *, doublereal *);
     /* Subroutine */ int dlabad_(doublereal *, doublereal *), dgebak_(
            char *, char *, integer *, integer *, integer *, doublereal *, 
            integer *, doublereal *, integer *, integer *, ftnlen, ftnlen), 
            dgebal_(char *, integer *, doublereal *, integer *, integer *, 
            integer *, doublereal *, integer *, ftnlen);
    static logical scalea;
     doublereal dlamch_(char *, ftnlen);
    static doublereal cscale;
     doublereal dlange_(char *, integer *, integer *, doublereal *, 
            integer *, doublereal *, ftnlen);
     /* Subroutine */ int dgehrd_(integer *, integer *, integer *, 
            doublereal *, integer *, doublereal *, doublereal *, integer *, 
            integer *), dlascl_(char *, integer *, integer *, doublereal *, 
            doublereal *, integer *, integer *, doublereal *, integer *, 
            integer *, ftnlen);
     integer idamax_(integer *, doublereal *, integer *);
     /* Subroutine */ int dlacpy_(char *, integer *, integer *, 
            doublereal *, integer *, doublereal *, integer *, ftnlen), 
            dlartg_(doublereal *, doublereal *, doublereal *, doublereal *, 
            doublereal *), xerbla_(char *, integer *, ftnlen);
    static logical select[1];
     integer ilaenv_(integer *, char *, char *, integer *, integer *, 
            integer *, integer *, ftnlen, ftnlen);
    static doublereal bignum;
     /* Subroutine */ int dorghr_(integer *, integer *, integer *, 
            doublereal *, integer *, doublereal *, doublereal *, integer *, 
            integer *), dhseqr_(char *, char *, integer *, integer *, integer 
            *, doublereal *, integer *, doublereal *, doublereal *, 
            doublereal *, integer *, doublereal *, integer *, integer *, 
            ftnlen, ftnlen), dtrevc_(char *, char *, logical *, integer *, 
            doublereal *, integer *, doublereal *, integer *, doublereal *, 
            integer *, integer *, integer *, doublereal *, integer *, ftnlen, 
            ftnlen), dtrsna_(char *, char *, logical *, integer *, doublereal 
            *, integer *, doublereal *, integer *, doublereal *, integer *, 
            doublereal *, doublereal *, integer *, integer *, doublereal *, 
            integer *, integer *, integer *, ftnlen, ftnlen);
    static integer minwrk, maxwrk;
    static logical wantvl, wntsnb;
    static integer hswork;
    static logical wntsne;
    static doublereal smlnum;
    static logical lquery, wantvr, wntsnn, wntsnv;


/*  -- LAPACK driver routine (version 3.0) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd., */
/*     Courant Institute, Argonne National Lab, and Rice University */
/*     June 30, 1999 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  DGEEVX computes for an N-by-N real nonsymmetric matrix A, the */
/*  eigenvalues and, optionally, the left and/or right eigenvectors. */

/*  Optionally also, it computes a balancing transformation to improve */
/*  the conditioning of the eigenvalues and eigenvectors (ILO, IHI, */
/*  SCALE, and ABNRM), reciprocal condition numbers for the eigenvalues */
/*  (RCONDE), and reciprocal condition numbers for the right */
/*  eigenvectors (RCONDV). */

/*  The right eigenvector v(j) of A satisfies */
/*                   A * v(j) = lambda(j) * v(j) */
/*  where lambda(j) is its eigenvalue. */
/*  The left eigenvector u(j) of A satisfies */
/*                u(j)**H * A = lambda(j) * u(j)**H */
/*  where u(j)**H denotes the conjugate transpose of u(j). */

/*  The computed eigenvectors are normalized to have Euclidean norm */
/*  equal to 1 and largest component real. */

/*  Balancing a matrix means permuting the rows and columns to make it */
/*  more nearly upper triangular, and applying a diagonal similarity */
/*  transformation D * A * D**(-1), where D is a diagonal matrix, to */
/*  make its rows and columns closer in norm and the condition numbers */
/*  of its eigenvalues and eigenvectors smaller.  The computed */
/*  reciprocal condition numbers correspond to the balanced matrix. */
/*  Permuting rows and columns will not change the condition numbers */
/*  (in exact arithmetic) but diagonal scaling will.  For further */
/*  explanation of balancing, see section 4.10.2 of the LAPACK */
/*  Users' Guide. */

/*  Arguments */
/*  ========= */

/*  BALANC  (input) CHARACTER*1 */
/*          Indicates how the input matrix should be diagonally scaled */
/*          and/or permuted to improve the conditioning of its */
/*          eigenvalues. */
/*          = 'N': Do not diagonally scale or permute; */
/*          = 'P': Perform permutations to make the matrix more nearly */
/*                 upper triangular. Do not diagonally scale; */
/*          = 'S': Diagonally scale the matrix, i.e. replace A by */
/*                 D*A*D**(-1), where D is a diagonal matrix chosen */
/*                 to make the rows and columns of A more equal in */
/*                 norm. Do not permute; */
/*          = 'B': Both diagonally scale and permute A. */

/*          Computed reciprocal condition numbers will be for the matrix */
/*          after balancing and/or permuting. Permuting does not change */
/*          condition numbers (in exact arithmetic), but balancing does. */

/*  JOBVL   (input) CHARACTER*1 */
/*          = 'N': left eigenvectors of A are not computed; */
/*          = 'V': left eigenvectors of A are computed. */
/*          If SENSE = 'E' or 'B', JOBVL must = 'V'. */

/*  JOBVR   (input) CHARACTER*1 */
/*          = 'N': right eigenvectors of A are not computed; */
/*          = 'V': right eigenvectors of A are computed. */
/*          If SENSE = 'E' or 'B', JOBVR must = 'V'. */

/*  SENSE   (input) CHARACTER*1 */
/*          Determines which reciprocal condition numbers are computed. */
/*          = 'N': None are computed; */
/*          = 'E': Computed for eigenvalues only; */
/*          = 'V': Computed for right eigenvectors only; */
/*          = 'B': Computed for eigenvalues and right eigenvectors. */

/*          If SENSE = 'E' or 'B', both left and right eigenvectors */
/*          must also be computed (JOBVL = 'V' and JOBVR = 'V'). */

/*  N       (input) INTEGER */
/*          The order of the matrix A. N >= 0. */

/*  A       (input/output) DOUBLE PRECISION array, dimension (LDA,N) */
/*          On entry, the N-by-N matrix A. */
/*          On exit, A has been overwritten.  If JOBVL = 'V' or */
/*          JOBVR = 'V', A contains the real Schur form of the balanced */
/*          version of the input matrix A. */

/*  LDA     (input) INTEGER */
/*          The leading dimension of the array A.  LDA >= MAX(1,N). */

/*  WR      (output) DOUBLE PRECISION array, dimension (N) */
/*  WI      (output) DOUBLE PRECISION array, dimension (N) */
/*          WR and WI contain the real and imaginary parts, */
/*          respectively, of the computed eigenvalues.  Complex */
/*          conjugate pairs of eigenvalues will appear consecutively */
/*          with the eigenvalue having the positive imaginary part */
/*          first. */

/*  VL      (output) DOUBLE PRECISION array, dimension (LDVL,N) */
/*          If JOBVL = 'V', the left eigenvectors u(j) are stored one */
/*          after another in the columns of VL, in the same order */
/*          as their eigenvalues. */
/*          If JOBVL = 'N', VL is not referenced. */
/*          If the j-th eigenvalue is real, then u(j) = VL(:,j), */
/*          the j-th column of VL. */
/*          If the j-th and (j+1)-st eigenvalues form a scomplex */
/*          conjugate pair, then u(j) = VL(:,j) + i*VL(:,j+1) and */
/*          u(j+1) = VL(:,j) - i*VL(:,j+1). */

/*  LDVL    (input) INTEGER */
/*          The leading dimension of the array VL.  LDVL >= 1; if */
/*          JOBVL = 'V', LDVL >= N. */

/*  VR      (output) DOUBLE PRECISION array, dimension (LDVR,N) */
/*          If JOBVR = 'V', the right eigenvectors v(j) are stored one */
/*          after another in the columns of VR, in the same order */
/*          as their eigenvalues. */
/*          If JOBVR = 'N', VR is not referenced. */
/*          If the j-th eigenvalue is real, then v(j) = VR(:,j), */
/*          the j-th column of VR. */
/*          If the j-th and (j+1)-st eigenvalues form a scomplex */
/*          conjugate pair, then v(j) = VR(:,j) + i*VR(:,j+1) and */
/*          v(j+1) = VR(:,j) - i*VR(:,j+1). */

/*  LDVR    (input) INTEGER */
/*          The leading dimension of the array VR.  LDVR >= 1, and if */
/*          JOBVR = 'V', LDVR >= N. */

/*  ILO,IHI (output) INTEGER */
/*          ILO and IHI are integer values determined when A was */
/*          balanced.  The balanced A(i,j) = 0 if I > J and */
/*          J = 1,...,ILO-1 or I = IHI+1,...,N. */

/*  SCALE   (output) DOUBLE PRECISION array, dimension (N) */
/*          Details of the permutations and scaling factors applied */
/*          when balancing A.  If P(j) is the index of the row and column */
/*          interchanged with row and column j, and D(j) is the scaling */
/*          factor applied to row and column j, then */
/*          SCALE(J) = P(J),    for J = 1,...,ILO-1 */
/*                   = D(J),    for J = ILO,...,IHI */
/*                   = P(J)     for J = IHI+1,...,N. */
/*          The order in which the interchanges are made is N to IHI+1, */
/*          then 1 to ILO-1. */

/*  ABNRM   (output) DOUBLE PRECISION */
/*          The one-norm of the balanced matrix (the maximum */
/*          of the sum of absolute values of elements of any column). */

/*  RCONDE  (output) DOUBLE PRECISION array, dimension (N) */
/*          RCONDE(j) is the reciprocal condition number of the j-th */
/*          eigenvalue. */

/*  RCONDV  (output) DOUBLE PRECISION array, dimension (N) */
/*          RCONDV(j) is the reciprocal condition number of the j-th */
/*          right eigenvector. */

/*  WORK    (workspace/output) DOUBLE PRECISION array, dimension (LWORK) */
/*          On exit, if INFO = 0, WORK(1) returns the optimal LWORK. */

/*  LWORK   (input) INTEGER */
/*          The dimension of the array WORK.   If SENSE = 'N' or 'E', */
/*          LWORK >= MAX(1,2*N), and if JOBVL = 'V' or JOBVR = 'V', */
/*          LWORK >= 3*N.  If SENSE = 'V' or 'B', LWORK >= N*(N+6). */
/*          For good performance, LWORK must generally be larger. */

/*          If LWORK = -1, then a workspace query is assumed; the routine */
/*          only calculates the optimal size of the WORK array, returns */
/*          this value as the first entry of the WORK array, and no error */
/*          message related to LWORK is issued by XERBLA. */

/*  IWORK   (workspace) INTEGER array, dimension (2*N-2) */
/*          If SENSE = 'N' or 'E', not referenced. */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value. */
/*          > 0:  if INFO = i, the QR algorithm failed to compute all the */
/*                eigenvalues, and no eigenvectors or condition numbers */
/*                have been computed; elements 1:ILO-1 and i+1:N of WR */
/*                and WI contain eigenvalues which have converged. */

/*  ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. Local Arrays .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Test the input arguments */

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --wr;
    --wi;
    vl_dim1 = *ldvl;
    vl_offset = 1 + vl_dim1;
    vl -= vl_offset;
    vr_dim1 = *ldvr;
    vr_offset = 1 + vr_dim1;
    vr -= vr_offset;
    --scale;
    --rconde;
    --rcondv;
    --work;
    --iwork;

    /* Function Body */
    *info = 0;
    lquery = *lwork == -1;
    wantvl = lsame_(jobvl, "V", (ftnlen)1, (ftnlen)1);
    wantvr = lsame_(jobvr, "V", (ftnlen)1, (ftnlen)1);
    wntsnn = lsame_(sense, "N", (ftnlen)1, (ftnlen)1);
    wntsne = lsame_(sense, "E", (ftnlen)1, (ftnlen)1);
    wntsnv = lsame_(sense, "V", (ftnlen)1, (ftnlen)1);
    wntsnb = lsame_(sense, "B", (ftnlen)1, (ftnlen)1);
    if (! (lsame_(balanc, "N", (ftnlen)1, (ftnlen)1) || lsame_(balanc, "S", (
            ftnlen)1, (ftnlen)1) || lsame_(balanc, "P", (ftnlen)1, (ftnlen)1) 
            || lsame_(balanc, "B", (ftnlen)1, (ftnlen)1))) {
        *info = -1;
    } else if (! wantvl && ! lsame_(jobvl, "N", (ftnlen)1, (ftnlen)1)) {
        *info = -2;
    } else if (! wantvr && ! lsame_(jobvr, "N", (ftnlen)1, (ftnlen)1)) {
        *info = -3;
    } else if (! (wntsnn || wntsne || wntsnb || wntsnv) || (wntsne || wntsnb) 
            && ! (wantvl && wantvr)) {
        *info = -4;
    } else if (*n < 0) {
        *info = -5;
    } else if (*lda < MAX(1,*n)) {
        *info = -7;
    } else if (*ldvl < 1 || wantvl && *ldvl < *n) {
        *info = -11;
    } else if (*ldvr < 1 || wantvr && *ldvr < *n) {
        *info = -13;
    }

/*     Compute workspace */
/*      (Note: Comments in the code beginning "Workspace:" describe the */
/*       minimal amount of workspace needed at that point in the code, */
/*       as well as the preferred amount for good performance. */
/*       NB refers to the optimal block size for the immediately */
/*       following subroutine, as returned by ILAENV. */
/*       HSWORK refers to the workspace preferred by DHSEQR, as */
/*       calculated below. HSWORK is computed assuming ILO=1 and IHI=N, */
/*       the worst case.) */

    minwrk = 1;
    if (*info == 0 && (*lwork >= 1 || lquery)) {
        maxwrk = *n + *n * ilaenv_(&c__1, "DGEHRD", " ", n, &c__1, n, &c__0, (
                ftnlen)6, (ftnlen)1);
        if (! wantvl && ! wantvr) {
/* Computing MAX */
            i__1 = 1, i__2 = *n << 1;
            minwrk = MAX(i__1,i__2);
            if (! wntsnn) {
/* Computing MAX */
                i__1 = minwrk, i__2 = *n * *n + *n * 6;
                minwrk = MAX(i__1,i__2);
            }
/* Computing MAX */
            i__1 = ilaenv_(&c__8, "DHSEQR", "SN", n, &c__1, n, &c_n1, (ftnlen)
                    6, (ftnlen)2);
            maxb = MAX(i__1,2);
            if (wntsnn) {
/* Computing MIN */
/* Computing MAX */
                i__3 = 2, i__4 = ilaenv_(&c__4, "DHSEQR", "EN", n, &c__1, n, &
                        c_n1, (ftnlen)6, (ftnlen)2);
                i__1 = MIN(maxb,*n), i__2 = MAX(i__3,i__4);
                k = MIN(i__1,i__2);
            } else {
/* Computing MIN */
/* Computing MAX */
                i__3 = 2, i__4 = ilaenv_(&c__4, "DHSEQR", "SN", n, &c__1, n, &
                        c_n1, (ftnlen)6, (ftnlen)2);
                i__1 = MIN(maxb,*n), i__2 = MAX(i__3,i__4);
                k = MIN(i__1,i__2);
            }
/* Computing MAX */
            i__1 = k * (k + 2), i__2 = *n << 1;
            hswork = MAX(i__1,i__2);
/* Computing MAX */
            i__1 = MAX(maxwrk,1);
            maxwrk = MAX(i__1,hswork);
            if (! wntsnn) {
/* Computing MAX */
                i__1 = maxwrk, i__2 = *n * *n + *n * 6;
                maxwrk = MAX(i__1,i__2);
            }
        } else {
/* Computing MAX */
            i__1 = 1, i__2 = *n * 3;
            minwrk = MAX(i__1,i__2);
            if (! wntsnn && ! wntsne) {
/* Computing MAX */
                i__1 = minwrk, i__2 = *n * *n + *n * 6;
                minwrk = MAX(i__1,i__2);
            }
/* Computing MAX */
            i__1 = ilaenv_(&c__8, "DHSEQR", "SN", n, &c__1, n, &c_n1, (ftnlen)
                    6, (ftnlen)2);
            maxb = MAX(i__1,2);
/* Computing MIN */
/* Computing MAX */
            i__3 = 2, i__4 = ilaenv_(&c__4, "DHSEQR", "EN", n, &c__1, n, &
                    c_n1, (ftnlen)6, (ftnlen)2);
            i__1 = MIN(maxb,*n), i__2 = MAX(i__3,i__4);
            k = MIN(i__1,i__2);
/* Computing MAX */
            i__1 = k * (k + 2), i__2 = *n << 1;
            hswork = MAX(i__1,i__2);
/* Computing MAX */
            i__1 = MAX(maxwrk,1);
            maxwrk = MAX(i__1,hswork);
/* Computing MAX */
            i__1 = maxwrk, i__2 = *n + (*n - 1) * ilaenv_(&c__1, "DORGHR", 
                    " ", n, &c__1, n, &c_n1, (ftnlen)6, (ftnlen)1);
            maxwrk = MAX(i__1,i__2);
            if (! wntsnn && ! wntsne) {
/* Computing MAX */
                i__1 = maxwrk, i__2 = *n * *n + *n * 6;
                maxwrk = MAX(i__1,i__2);
            }
/* Computing MAX */
            i__1 = maxwrk, i__2 = *n * 3, i__1 = MAX(i__1,i__2);
            maxwrk = MAX(i__1,1);
        }
        work[1] = (doublereal) maxwrk;
    }
    if (*lwork < minwrk && ! lquery) {
        *info = -21;
    }
    if (*info != 0) {
        i__1 = -(*info);
        xerbla_("DGEEVX", &i__1, (ftnlen)6);
        return 0;
    } else if (lquery) {
        return 0;
    }

/*     Quick return if possible */

    if (*n == 0) {
        return 0;
    }

/*     Get machine constants */

    eps = dlamch_("P", (ftnlen)1);
    smlnum = dlamch_("S", (ftnlen)1);
    bignum = 1. / smlnum;
    dlabad_(&smlnum, &bignum);
    smlnum = sqrt(smlnum) / eps;
    bignum = 1. / smlnum;

/*     Scale A if MAX element outside range [SMLNUM,BIGNUM] */

    icond = 0;
    anrm = dlange_("M", n, n, &a[a_offset], lda, dum, (ftnlen)1);
    scalea = FALSE_;
    if (anrm > 0. && anrm < smlnum) {
        scalea = TRUE_;
        cscale = smlnum;
    } else if (anrm > bignum) {
        scalea = TRUE_;
        cscale = bignum;
    }
    if (scalea) {
        dlascl_("G", &c__0, &c__0, &anrm, &cscale, n, n, &a[a_offset], lda, &
                ierr, (ftnlen)1);
    }

/*     Balance the matrix and compute ABNRM */

    dgebal_(balanc, n, &a[a_offset], lda, ilo, ihi, &scale[1], &ierr, (ftnlen)
            1);
    *abnrm = dlange_("1", n, n, &a[a_offset], lda, dum, (ftnlen)1);
    if (scalea) {
        dum[0] = *abnrm;
        dlascl_("G", &c__0, &c__0, &cscale, &anrm, &c__1, &c__1, dum, &c__1, &
                ierr, (ftnlen)1);
        *abnrm = dum[0];
    }

/*     Reduce to upper Hessenberg form */
/*     (Workspace: need 2*N, prefer N+N*NB) */

    itau = 1;
    iwrk = itau + *n;
    i__1 = *lwork - iwrk + 1;
    dgehrd_(n, ilo, ihi, &a[a_offset], lda, &work[itau], &work[iwrk], &i__1, &
            ierr);

    if (wantvl) {

/*        Want left eigenvectors */
/*        Copy Householder vectors to VL */

        *(unsigned char *)side = 'L';
        dlacpy_("L", n, n, &a[a_offset], lda, &vl[vl_offset], ldvl, (ftnlen)1)
                ;

/*        Generate orthogonal matrix in VL */
/*        (Workspace: need 2*N-1, prefer N+(N-1)*NB) */

        i__1 = *lwork - iwrk + 1;
        dorghr_(n, ilo, ihi, &vl[vl_offset], ldvl, &work[itau], &work[iwrk], &
                i__1, &ierr);

/*        Perform QR iteration, accumulating Schur vectors in VL */
/*        (Workspace: need 1, prefer HSWORK (see comments) ) */

        iwrk = itau;
        i__1 = *lwork - iwrk + 1;
        dhseqr_("S", "V", n, ilo, ihi, &a[a_offset], lda, &wr[1], &wi[1], &vl[
                vl_offset], ldvl, &work[iwrk], &i__1, info, (ftnlen)1, (
                ftnlen)1);

        if (wantvr) {

/*           Want left and right eigenvectors */
/*           Copy Schur vectors to VR */

            *(unsigned char *)side = 'B';
            dlacpy_("F", n, n, &vl[vl_offset], ldvl, &vr[vr_offset], ldvr, (
                    ftnlen)1);
        }

    } else if (wantvr) {

/*        Want right eigenvectors */
/*        Copy Householder vectors to VR */

        *(unsigned char *)side = 'R';
        dlacpy_("L", n, n, &a[a_offset], lda, &vr[vr_offset], ldvr, (ftnlen)1)
                ;

/*        Generate orthogonal matrix in VR */
/*        (Workspace: need 2*N-1, prefer N+(N-1)*NB) */

        i__1 = *lwork - iwrk + 1;
        dorghr_(n, ilo, ihi, &vr[vr_offset], ldvr, &work[itau], &work[iwrk], &
                i__1, &ierr);

/*        Perform QR iteration, accumulating Schur vectors in VR */
/*        (Workspace: need 1, prefer HSWORK (see comments) ) */

        iwrk = itau;
        i__1 = *lwork - iwrk + 1;
        dhseqr_("S", "V", n, ilo, ihi, &a[a_offset], lda, &wr[1], &wi[1], &vr[
                vr_offset], ldvr, &work[iwrk], &i__1, info, (ftnlen)1, (
                ftnlen)1);

    } else {

/*        Compute eigenvalues only */
/*        If condition numbers desired, compute Schur form */

        if (wntsnn) {
            *(unsigned char *)job = 'E';
        } else {
            *(unsigned char *)job = 'S';
        }

/*        (Workspace: need 1, prefer HSWORK (see comments) ) */

        iwrk = itau;
        i__1 = *lwork - iwrk + 1;
        dhseqr_(job, "N", n, ilo, ihi, &a[a_offset], lda, &wr[1], &wi[1], &vr[
                vr_offset], ldvr, &work[iwrk], &i__1, info, (ftnlen)1, (
                ftnlen)1);
    }

/*     If INFO > 0 from DHSEQR, then quit */

    if (*info > 0) {
        goto L50;
    }

    if (wantvl || wantvr) {

/*        Compute left and/or right eigenvectors */
/*        (Workspace: need 3*N) */

        dtrevc_(side, "B", select, n, &a[a_offset], lda, &vl[vl_offset], ldvl,
                 &vr[vr_offset], ldvr, n, &nout, &work[iwrk], &ierr, (ftnlen)
                1, (ftnlen)1);
    }

/*     Compute condition numbers if desired */
/*     (Workspace: need N*N+6*N unless SENSE = 'E') */

    if (! wntsnn) {
        dtrsna_(sense, "A", select, n, &a[a_offset], lda, &vl[vl_offset], 
                ldvl, &vr[vr_offset], ldvr, &rconde[1], &rcondv[1], n, &nout, 
                &work[iwrk], n, &iwork[1], &icond, (ftnlen)1, (ftnlen)1);
    }

    if (wantvl) {

/*        Undo balancing of left eigenvectors */

        dgebak_(balanc, "L", n, ilo, ihi, &scale[1], n, &vl[vl_offset], ldvl, 
                &ierr, (ftnlen)1, (ftnlen)1);

/*        Normalize left eigenvectors and make largest component real */

        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            if (wi[i__] == 0.) {
                scl = 1. / dnrm2_(n, &vl[i__ * vl_dim1 + 1], &c__1);
                dscal_(n, &scl, &vl[i__ * vl_dim1 + 1], &c__1);
            } else if (wi[i__] > 0.) {
                d__1 = dnrm2_(n, &vl[i__ * vl_dim1 + 1], &c__1);
                d__2 = dnrm2_(n, &vl[(i__ + 1) * vl_dim1 + 1], &c__1);
                scl = 1. / dlapy2_(&d__1, &d__2);
                dscal_(n, &scl, &vl[i__ * vl_dim1 + 1], &c__1);
                dscal_(n, &scl, &vl[(i__ + 1) * vl_dim1 + 1], &c__1);
                i__2 = *n;
                for (k = 1; k <= i__2; ++k) {
/* Computing 2nd power */
                    d__1 = vl[k + i__ * vl_dim1];
/* Computing 2nd power */
                    d__2 = vl[k + (i__ + 1) * vl_dim1];
                    work[k] = d__1 * d__1 + d__2 * d__2;
/* L10: */
                }
                k = idamax_(n, &work[1], &c__1);
                dlartg_(&vl[k + i__ * vl_dim1], &vl[k + (i__ + 1) * vl_dim1], 
                        &cs, &sn, &r__);
                drot_(n, &vl[i__ * vl_dim1 + 1], &c__1, &vl[(i__ + 1) * 
                        vl_dim1 + 1], &c__1, &cs, &sn);
                vl[k + (i__ + 1) * vl_dim1] = 0.;
            }
/* L20: */
        }
    }

    if (wantvr) {

/*        Undo balancing of right eigenvectors */

        dgebak_(balanc, "R", n, ilo, ihi, &scale[1], n, &vr[vr_offset], ldvr, 
                &ierr, (ftnlen)1, (ftnlen)1);

/*        Normalize right eigenvectors and make largest component real */

        i__1 = *n;
        for (i__ = 1; i__ <= i__1; ++i__) {
            if (wi[i__] == 0.) {
                scl = 1. / dnrm2_(n, &vr[i__ * vr_dim1 + 1], &c__1);
                dscal_(n, &scl, &vr[i__ * vr_dim1 + 1], &c__1);
            } else if (wi[i__] > 0.) {
                d__1 = dnrm2_(n, &vr[i__ * vr_dim1 + 1], &c__1);
                d__2 = dnrm2_(n, &vr[(i__ + 1) * vr_dim1 + 1], &c__1);
                scl = 1. / dlapy2_(&d__1, &d__2);
                dscal_(n, &scl, &vr[i__ * vr_dim1 + 1], &c__1);
                dscal_(n, &scl, &vr[(i__ + 1) * vr_dim1 + 1], &c__1);
                i__2 = *n;
                for (k = 1; k <= i__2; ++k) {
/* Computing 2nd power */
                    d__1 = vr[k + i__ * vr_dim1];
/* Computing 2nd power */
                    d__2 = vr[k + (i__ + 1) * vr_dim1];
                    work[k] = d__1 * d__1 + d__2 * d__2;
/* L30: */
                }
                k = idamax_(n, &work[1], &c__1);
                dlartg_(&vr[k + i__ * vr_dim1], &vr[k + (i__ + 1) * vr_dim1], 
                        &cs, &sn, &r__);
                drot_(n, &vr[i__ * vr_dim1 + 1], &c__1, &vr[(i__ + 1) * 
                        vr_dim1 + 1], &c__1, &cs, &sn);
                vr[k + (i__ + 1) * vr_dim1] = 0.;
            }
/* L40: */
        }
    }

/*     Undo scaling if necessary */

L50:
    if (scalea) {
        i__1 = *n - *info;
/* Computing MAX */
        i__3 = *n - *info;
        i__2 = MAX(i__3,1);
        dlascl_("G", &c__0, &c__0, &cscale, &anrm, &i__1, &c__1, &wr[*info + 
                1], &i__2, &ierr, (ftnlen)1);
        i__1 = *n - *info;
/* Computing MAX */
        i__3 = *n - *info;
        i__2 = MAX(i__3,1);
        dlascl_("G", &c__0, &c__0, &cscale, &anrm, &i__1, &c__1, &wi[*info + 
                1], &i__2, &ierr, (ftnlen)1);
        if (*info == 0) {
            if ((wntsnv || wntsnb) && icond == 0) {
                dlascl_("G", &c__0, &c__0, &cscale, &anrm, n, &c__1, &rcondv[
                        1], n, &ierr, (ftnlen)1);
            }
        } else {
            i__1 = *ilo - 1;
            dlascl_("G", &c__0, &c__0, &cscale, &anrm, &i__1, &c__1, &wr[1], 
                    n, &ierr, (ftnlen)1);
            i__1 = *ilo - 1;
            dlascl_("G", &c__0, &c__0, &cscale, &anrm, &i__1, &c__1, &wi[1], 
                    n, &ierr, (ftnlen)1);
        }
    }

    work[1] = (doublereal) maxwrk;
    return 0;

/*     End of DGEEVX */

} /* dgeevx_ */

