#undef min
#undef max
#define register
#include "f2c.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cabs.inl"
#include "F77_aloc.inl"
#include "r_cnjg.inl"
#include "z_abs.inl"
#include "z_div.inl"
#include "z_sqrt.inl"
#include "c_abs.inl"
#include "c_div.inl"
#include "d_cnjg.inl"
#include "d_imag.inl"
#include "d_lg10.inl"
#include "d_sign.inl"
#include "r_imag.inl"
#include "r_lg10.inl"
#include "r_sign.inl"
#include "s_cat.inl"
#include "s_cmp.inl"
#include "s_copy.inl"
#include "i_dnnt.inl"
#include "i_nint.inl"
#include "pow_di.inl"
#include "pow_ri.inl"

#include "lsame.inl"
#include "slamch.inl"
#include "dlamch.inl"

#include "slassq.inl"
#include "dlassq.inl"
#include "classq.inl"
#include "zlassq.inl"

#include "caxpy.inl"
#include "ccopy.inl"
#include "cdotc.inl"
#include "cdotu.inl"
#include "cgecon.inl"
#include "cgeequ.inl"
#include "cgerfs.inl"
#include "cgeru.inl"
#include "cgetrs.inl"
#include "sladiv.inl"
#include "clacon.inl"
#include "clacpy.inl"
#include "cladiv.inl"
#include "clange.inl"
#include "clantr.inl"
#include "claqge.inl"
#include "clatrs.inl"
#include "cscal.inl"
#include "csrscl.inl"
#include "csscal.inl"
#include "cswap.inl"
#include "ctrsv.inl"


#include "dasum.inl"
#include "daxpy.inl"
#include "dcabs1.inl"
#include "dcopy.inl"
#include "ddot.inl"
#include "dgecon.inl"
#include "dgeequ.inl"
#include "dger.inl"
#include "dgerfs.inl"
#include "dgetrs.inl"
#include "dlabad.inl"
#include "dlacon.inl"
#include "dlacpy.inl"
#include "dladiv.inl"
#include "dlange.inl"
#include "dlantr.inl"
#include "dlaqge.inl"
#include "dlatrs.inl"
#include "drscl.inl"
#include "dscal.inl"
#include "dswap.inl"
#include "dtrsv.inl"
#include "dzasum.inl"
#include "dzsum1.inl"
#include "icamax.inl"
#include "icmax1.inl"
#include "idamax.inl"
#include "ilaenv.inl"
#include "isamax.inl"
#include "izamax.inl"
#include "izmax1.inl"
#include "sasum.inl"
#include "saxpy.inl"
#include "scasum.inl"
#include "scopy.inl"
#include "scsum1.inl"
#include "sdot.inl"
#include "sgecon.inl"
#include "sgeequ.inl"
#include "sger.inl"
#include "sgerfs.inl"
#include "sgetrs.inl"
#include "slabad.inl"
#include "slacon.inl"
#include "slacpy.inl"
#include "slange.inl"
#include "slantr.inl"
#include "slaqge.inl"
#include "slatrs.inl"
#include "srscl.inl"
#include "sscal.inl"
#include "sswap.inl"
#include "strsv.inl"
#include "xerbla.inl"
#include "zaxpy.inl"
#include "zcopy.inl"
#include "zdotc.inl"
#include "zdotu.inl"
#include "zdrscl.inl"
#include "zdscal.inl"
#include "zgecon.inl"
#include "zgeequ.inl"
#include "zgerfs.inl"
#include "zgeru.inl"
#include "zgetrs.inl"
#include "zlacon.inl"
#include "zlacpy.inl"
#include "zladiv.inl"
#include "zlange.inl"
#include "zlantr.inl"
#include "zlaqge.inl"
#include "zlatrs.inl"
#include "zscal.inl"
#include "zswap.inl"
#include "ztrsv.inl"
#include "sormrz.inl"
#include "sormqr.inl"
#include "stzrzf.inl"
#include "slaic1.inl"
#include "sgeqp3.inl"
#include "slaset.inl"
#include "slascl.inl"
#include "dormrz.inl"
#include "dormqr.inl"
#include "dtzrzf.inl"
#include "dlaic1.inl"
#include "dgeqp3.inl"
#include "dlaset.inl"
#include "dlascl.inl"
#include "cunmrz.inl"
#include "cunmqr.inl"
#include "ctzrzf.inl"
#include "claic1.inl"
#include "cgeqp3.inl"
#include "claset.inl"
#include "clascl.inl"
#include "zunmrz.inl"
#include "zunmqr.inl"
#include "ztzrzf.inl"
#include "zlaic1.inl"
#include "zgeqp3.inl"
#include "zlaset.inl"
#include "zlascl.inl"
#include "slarzb.inl"
#include "slarzt.inl"
#include "sormr3.inl"
#include "slarfb.inl"
#include "slarft.inl"
#include "sorm2r.inl"
#include "slatrz.inl"
#include "slaqp2.inl"
#include "slaqps.inl"
#include "snrm2.inl"
#include "sgeqrf.inl"
#include "dlarzb.inl"
#include "dlarzt.inl"
#include "dormr3.inl"
#include "dlarfb.inl"
#include "dlarft.inl"
#include "dorm2r.inl"
#include "dlatrz.inl"
#include "dlaqp2.inl"
#include "dlaqps.inl"
#include "dnrm2.inl"
#include "dgeqrf.inl"
#include "clarzb.inl"
#include "clarzt.inl"
#include "cunmr3.inl"
#include "clarfb.inl"
#include "clarft.inl"
#include "cunm2r.inl"
#include "clatrz.inl"
#include "c_sqrt.inl"
#include "claqp2.inl"
#include "claqps.inl"
#include "scnrm2.inl"
#include "cgeqrf.inl"
#include "zlarzb.inl"
#include "zlarzt.inl"
#include "zunmr3.inl"
#include "zlarfb.inl"
#include "zlarft.inl"
#include "zunm2r.inl"
#include "zlatrz.inl"
#include "zlaqp2.inl"
#include "zlaqps.inl"
#include "dznrm2.inl"
#include "zgeqrf.inl"
#include "slarz.inl"
#include "slarf.inl"
#include "slarfg.inl"
#include "sgeqr2.inl"
#include "dlarz.inl"
#include "dlarf.inl"
#include "dlarfg.inl"
#include "dgeqr2.inl"
#include "clacgv.inl"
#include "clarz.inl"
#include "clarf.inl"
#include "clarfg.inl"
#include "cgeqr2.inl"
#include "zlacgv.inl"
#include "zlarz.inl"
#include "zlarf.inl"
#include "zlarfg.inl"
#include "zgeqr2.inl"
#include "slapy2.inl"
#include "dlapy2.inl"
#include "cgerc.inl"
#include "slapy3.inl"
#include "zgerc.inl"
#include "dlapy3.inl"


#include "strmv.inl"
#include "dtrmv.inl"
#include "ctrmv.inl"
#include "ztrmv.inl"

#include "strmm.inl"
#include "dtrmm.inl"
#include "ctrmm.inl"
#include "ztrmm.inl"

#include "strti2.inl"
#include "dtrti2.inl"
#include "ctrti2.inl"
#include "ztrti2.inl"

#include "strtri.inl"
#include "dtrtri.inl"
#include "ctrtri.inl"
#include "ztrtri.inl"

#include "sgemv.inl"
#include "dgemv.inl"
#include "cgemv.inl"
#include "zgemv.inl"

#include "sgetf2.inl"
#include "dgetf2.inl"
#include "cgetf2.inl"
#include "zgetf2.inl"

#include "slaswp.inl"
#include "dlaswp.inl"
#include "claswp.inl"
#include "zlaswp.inl"

#include "strsm.inl"
#include "dtrsm.inl"
#include "ctrsm.inl"
#include "ztrsm.inl"

#include "sgemm.inl"
#include "dgemm.inl"
#include "cgemm.inl"
#include "zgemm.inl"

#include "sgetrf.inl"
#include "dgetrf.inl"
#include "cgetrf.inl"
#include "zgetrf.inl"

#include "sgetri.inl"
#include "dgetri.inl"
#include "cgetri.inl"
#include "zgetri.inl"

#include "sgesvx.inl"
#include "dgesvx.inl"
#include "cgesvx.inl"
#include "zgesvx.inl"

#include "sgelsy.inl"
#include "dgelsy.inl"
#include "cgelsy.inl"
#include "zgelsy.inl"

#include "ssyev.inl"
#include "dsyev.inl"

#include "ssteqr.inl"
#include "dsteqr.inl"

#include "sorgtr.inl"
#include "dorgtr.inl"

#include "ssterf.inl"
#include "dsterf.inl"

#include "ssytrd.inl"
#include "dsytrd.inl"

#include "slansy.inl"
#include "slasrt.inl"
#include "slartg.inl"
#include "slae2.inl"
#include "slasr.inl"
#include "slaev2.inl"
#include "slanst.inl"
#include "sorgqr.inl"
#include "sorgql.inl"
#include "ssytd2.inl"
#include "ssyr2k.inl"
#include "slatrd.inl"

#include "sorg2r.inl"
#include "sorg2l.inl"
#include "ssyr2.inl"
#include "ssymv.inl"

#include "cheev.inl"
#include "zheev.inl"

#include "csteqr.inl"
#include "cungtr.inl"
#include "chetrd.inl"
#include "clanhe.inl"
#include "clasr.inl"
#include "cungqr.inl"
#include "cungql.inl"
#include "chetd2.inl"
#include "cher2k.inl"
#include "clatrd.inl"
#include "cung2r.inl"
#include "cung2l.inl"
#include "cher2.inl"
#include "chemv.inl"

#include "dlansy.inl"
#include "dlasrt.inl"
#include "dlartg.inl"
#include "dlae2.inl"
#include "dlasr.inl"
#include "dlaev2.inl"
#include "dlanst.inl"
#include "dorgqr.inl"
#include "dorgql.inl"
#include "dsytd2.inl"
#include "dsyr2k.inl"
#include "dlatrd.inl"
#include "zsteqr.inl"
#include "zungtr.inl"
#include "zhetrd.inl"
#include "zlanhe.inl"
#include "dorg2r.inl"
#include "dorg2l.inl"
#include "dsyr2.inl"
#include "dsymv.inl"
#include "zlasr.inl"
#include "zungqr.inl"
#include "zungql.inl"
#include "zhetd2.inl"
#include "zher2k.inl"
#include "zlatrd.inl"

#include "zung2r.inl"
#include "zung2l.inl"
#include "zher2.inl"
#include "zhemv.inl"


#include "sgeevx.inl"
#include "dgeevx.inl"

#include "srot.inl"
#include "sgebak.inl"
#include "strsna.inl"
#include "strevc.inl"
#include "shseqr.inl"
#include "sorghr.inl"
#include "sgehrd.inl"
#include "sgebal.inl"
#include "slaqtr.inl"
#include "strexc.inl"
#include "slaln2.inl"
#include "slarfx.inl"
#include "slanhs.inl"
#include "slahqr.inl"
#include "sgehd2.inl"
#include "slahrd.inl"
#include "slaexc.inl"
#include "slanv2.inl"
#include "slasy2.inl"


#include "cgeevx.inl"
#include "cgebak.inl"
#include "ctrsna.inl"
#include "ctrevc.inl"
#include "chseqr.inl"
#include "cunghr.inl"
#include "cgehrd.inl"
#include "cgebal.inl"
#include "ctrexc.inl"
#include "clarfx.inl"
#include "clanhs.inl"
#include "clahqr.inl"
#include "cgehd2.inl"
#include "clahrd.inl"
#include "crot.inl"
#include "clartg.inl"
#include "zgeevx.inl"
#include "drot.inl"
#include "dgebak.inl"
#include "dtrsna.inl"
#include "dtrevc.inl"
#include "dhseqr.inl"
#include "dorghr.inl"
#include "dgehrd.inl"
#include "dgebal.inl"
#include "zgebak.inl"
#include "ztrsna.inl"
#include "ztrevc.inl"
#include "zhseqr.inl"
#include "zunghr.inl"
#include "zgehrd.inl"
#include "zgebal.inl"
#include "dlaqtr.inl"
#include "dtrexc.inl"
#include "dlaln2.inl"
#include "dlarfx.inl"
#include "dlanhs.inl"
#include "dlahqr.inl"
#include "dgehd2.inl"
#include "dlahrd.inl"
#include "ztrexc.inl"
#include "zlarfx.inl"
#include "zlanhs.inl"
#include "zlahqr.inl"
#include "zgehd2.inl"
#include "zlahrd.inl"
#include "dlaexc.inl"
#include "dlanv2.inl"
#include "zrot.inl"
#include "zlartg.inl"
#include "dlasy2.inl"









static float seps() {
  char CMACH = 'E';
  return (float)slamch_(&CMACH,1);
}
static double deps() {
  char CMACH = 'E';
  return dlamch_(&CMACH,1);
}

static float slamch() {
  return seps();
}

static double dlamch() {
  return deps();
}

// Utility function used to handle stride changes between columns
static void changeStrideDouble(double*dst, int dstStride, double*src, int srcStride, int rowCount, int colCount){
  int i;
  for (i=0;i<colCount;i++) {
    memcpy(dst + i*dstStride, src + i*srcStride, rowCount*sizeof(double));
  }
}

static void changeStrideFloat(float*dst, int dstStride, float*src, int srcStride, int rowCount, int colCount){
  int i;
  for (i=0;i<colCount;i++) {
    memcpy(dst + i*dstStride, src + i*srcStride, rowCount*sizeof(float));
  }
}


