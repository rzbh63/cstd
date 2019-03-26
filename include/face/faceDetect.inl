#ifndef _FACEDETECT_INL_
#define _FACEDETECT_INL_

#include "_faceDetect.h"
#include "init.inl"

#undef RECT_VAL2
#undef HAAR_FEAT_VAL
#undef LUT_VAL
#undef RECT_VAL2
// �� haar ����
#define RECT_VAL(A, p0, p1, p2, p3) (*(A + (p0)) - *(A + (p1)) - *(A + (p2)) + *(A + (p3)))
#define RECT_VAL2(_A, _a, _b)       (*(_A) - *(_A + (_a)) - *(_A + (_b)) + *(_A + (_a) + (_b)))
#define HAAR_RECT_VAL(A, rect)      (RECT_VAL(A, (rect).p0, (rect).p1, (rect).p2, (rect).p3) * (rect).dd)
#define HAAR_RECT_VAL3(A, rect)     (RECT_VAL(A, (rect).p0, (rect).p0+(rect).w, (rect).p0+(rect).h, (rect).p0+(rect).w+(rect).h) * (rect).dd)
#define HAAR_FEAT_VAL(A, vv, feat)  (vv * (HAAR_RECT_VAL(A, feat->ft0) + HAAR_RECT_VAL(A, feat->ft1)))
#define HAAR_FEAT_VAL3(A, vv, feat) (vv * (HAAR_RECT_VAL3(A, feat->ft0) + HAAR_RECT_VAL3(A, feat->ft1)))
#define HAAR_FEAT_VAL2(A, vv, feat) (vv * ((HAAR_RECT_VAL(A, feat->ft0) + HAAR_RECT_VAL(A, feat->ft1)) >> SHIFT))
#define LUT_BIN_1                   (31)
#define _SHIFT_2                     (_SHIFT * 2)
// LUT adaboost
#define LUT_VAL(x, lut)   ((lut)->val[(int) (((x) - (lut)->minval) * (lut)->binval) & (LUT_BIN_1)])
#define LUT_VAL2(x, lut)  ((lut)->val[(((((x)) - (lut)->minval) * (lut)->binval) >> (SHIFT_2)) & (LUT_BIN_1)]) 

#ifndef HAAR_CN
#define HAAR_CN (2)
#endif // HAAR_CN
#if 1
float invsqrtf( float x ) {
  float xhalf = 0.5f * x;
  suf32_t i;
  i.f = x;
  i.i = 0x5f3759df - (i.i >> 1);// �����һ�����Ƹ�
  x = i.f;
  x = x * (1.5f - xhalf * x * x); // ţ�ٵ�����
  return x;
}

int invsqrti( int n ) {
  int next_x, x;
  n = _SHIFT1 * _SHIFT1 / n;
  for ( x = n; x > ( next_x = n / x ); ) {
    x = ( x + next_x ) >> 1;
  }
  
  return x;
}

unsigned int InvSqrtInt( unsigned int d ) {
  unsigned int t, q, r, N;
  d = ( _SHIFT1 * _SHIFT1 ) / d;
  q = 0;
  r = d;
  N = 13;
  do {
    N--;
    t = 2 * q + ( 1 << N );
    if ( ( r >> N ) >= t ) {
      r -= ( t << N );
      q += ( 1 << N );
    }
  } while ( N );
  return q;
}

//#define INVSQRT(x) ( x > 0 ) ? invsqrti(x) : _SHIFT1
#ifdef USE_FIX_FLOAT  // ʹ�ö�����
#define INVSQRT(x)  (x > 0) ? InvSqrtInt(x) : _SHIFT1
#else
#define INVSQRT(x)  (x > 0.f) ? sqrt(1.f / (x)) : 1.f 
//#define INVSQRT(x)  (x > 0.f) ? invsqrtf(x) : 1.f
#endif

//#define INVSQRT(x) ( x > 0 ) ? XFLOOR(invsqrtf(x) * _SHIFT1) : _SHIFT1
#else
#ifdef __PC
#define INVSQRT(x)  (x > 0) ? XFLOOR(sqrt(1. / (x)) * _SHIFT1) : _SHIFT1
#else
#define INVSQRT(x)  (x > 0) ? XFLOOR(sqrtf(1.f / (x)) * _SHIFT1) : _SHIFT1
#endif
#endif

#if 1
//#pragma DATA_SECTION(invvar_table, ".ddr2")
static unsigned short invvar_table[ 2 << 13 ] = {
#include "table/invvar_table.inl"
    };
/*
int invvar_table_inited = 0;
int invvar_table_init( void ) {
  if ( 0 == invvar_table_inited ) {
    int ivar2;
    invvar_table_inited = 1;
    for ( ivar2 = 0; ivar2 < ( 2 << 13 ); ++ivar2 ) {
      invvar_table[ ivar2 ] = INVSQRT( ivar2 );
    }
  }
  return 0;
}
*/

#define INVSQRT2(_X)  invvar_table[(unsigned short) (_X)];
#else

#define INVSQRT2(_X)  INVSQRT(_X)
#endif

static double get_invvar_0( const FIX_FLOAT* tmpSamp, const FIX_FLOAT* tmpSamp1, int _AL, int _AI, int area ) {
  double var;
  var = RECT_VAL2( tmpSamp, ( _AL ), ( _AI ) ) / area;
  var = RECT_VAL2( tmpSamp1, ( _AL ), ( _AI ) ) / area - var * var;
  
  return ( var > (double)0.f ) ? (double)INVSQRT((uint)var) : (double)1.f;
}

static int get_invvar_1( const FIX_FLOAT* tmpSamp, const FIX_FLOAT* tmpSamp1, int _AL, int _AI, int area2 ) {
  int invvar;
  int a, b, var, a1, b1;
  a1 = RECT_VAL2( tmpSamp, ( _AL ), ( _AI ) );
  b1 = RECT_VAL2( tmpSamp1, ( _AL ), ( _AI ) );
  //if (a>(1<<16)) {
  a = (a1<<5) / area2;//��������
  b = b1 / area2;
  var = b - ((a * a)>>10);
  //} else {
  //  var = (b - (a * a) / area2) / area2;
  //}
  if (var>(2 << 13)) {
    var = (2 << 13)-1;
  }
  //ASSERT(var<(2 << 13));
  invvar = INVSQRT2( var );

  return invvar;
}


#include "lutpass_0.inl"
#include "lutpass_1.inl"

// ����ͼ��ƫ����
static int setoffset1( int len, const fHaar* f, fHaarWork* p, int al, int ai, REAL_TYPE cs, REAL_TYPE iv ) {
  INT ar0, ar1;
  const fHaar * l = f + len;
  for ( ; f != l; ++f, ++p ) {
    int a, b, c, cx, cy;
    if ( f->ft0.w && 0 == f->ft0.h )
      continue;
      
    //SETOFFSET(f->ft0, p->ft0, ar0);
    cx = XROUND( ( f->ft0 ).w * cs );
    cy = XROUND( ( f->ft0 ).h * cs );
    a = XROUND( ( f->ft0 ).y * cs ) * al + XROUND( ( f->ft0 ).x * cs ) * ai;
    b = cx * ai, c = cy * al;
    ar0 = cx * cy;
    ( p->ft0 ).p0 = a;
    ( p->ft0 ).p1 = a + b;
    ( p->ft0 ).p2 = a + c;
    ( p->ft0 ).p3 = a + b + c;
    SETOFFSET( f->ft1, p->ft1, ar1 );
    p->ft1.dd = (float)( f->ft1.d * iv );
    if ( ar0 ) {
      p->ft0.dd = -( p->ft1.dd * ar1 / ar0 );
    }
  }
  
  return 0;
}

// ����ƫ����
#define SETOFFSET2(RECT, OUT, area) \
  { \
    int cx = _SHIFT_MUL((RECT).w, ics); \
    int cy = _SHIFT_MUL((RECT).h, ics); \
    int a = _SHIFT_MUL((RECT).y, ics) * al + _SHIFT_MUL((RECT).x, ics) * ai; \
    int b = cx * ai, c = cy * al; \
    area = cx * cy; \
    (OUT).p0 = a; \
    (OUT).p1 = a + b; \
    (OUT).p2 = a + c; \
    (OUT).p3 = a + b + c; \
  }
static int setoffset2( int len, const fHaar* f, iHaarWork2* p, int al, int ai, REAL_TYPE cs, REAL_TYPE iv0 ) {
  INT ar0, ar1, ics = (int)(cs * _SHIFT1), iv = (int)(iv0 * _SHIFT1);
  const fHaar * l = f + len;
  for ( ; f != l; ++f, ++p ) {
    if ( 0 == f->ft0.w || 0 == f->ft0.h || 0 == f->ft1.w || 0 == f->ft1.h )
      continue;
#if 0
      
    SETOFFSET2( f->ft0, p->ft0, ar0 );
    SETOFFSET2( f->ft1, p->ft1, ar1 );
#else
    
    {
      int a, b, c, cx, cy;
      cx = _SHIFT_MUL( ( f->ft0 ).w, ics );
      cy = _SHIFT_MUL( ( f->ft0 ).h, ics );
      a = _SHIFT_MUL( ( f->ft0 ).y, ics ) * al + _SHIFT_MUL( ( f->ft0 ).x, ics ) * ai;
      b = cx * ai;
      c = cy * al;
      ar0 = cx * cy;
      ( p->ft0 ).p0 = a;
      ( p->ft0 ).p1 = a + b;
      ( p->ft0 ).p2 = a + c;
      ( p->ft0 ).p3 = a + b + c;
    }
    {
      int a, b, c, cx, cy;
      cx = _SHIFT_MUL( ( f->ft1 ).w, ics );
      cy = _SHIFT_MUL( ( f->ft1 ).h, ics );
      a = _SHIFT_MUL( ( f->ft1 ).y, ics ) * al + _SHIFT_MUL( ( f->ft1 ).x, ics ) * ai;
      b = cx * ai;
      c = cy * al;
      ar1 = cx * cy;
      ( p->ft1 ).p0 = a;
      ( p->ft1 ).p1 = a + b;
      ( p->ft1 ).p2 = a + c;
      ( p->ft1 ).p3 = a + b + c;
    }
    
#endif
    p->ft1.dd = ( f->ft1.d * iv );
    if ( ar0 ) {
      p->ft0.dd = -( p->ft1.dd * ar1 / ar0 );
    }
  }
  
  return 0;
}

#define SETOFFSET3(RECT, OUT, area) \
  { \
    (OUT).p0 = _SHIFT_MUL((RECT).y, ics) * al + _SHIFT_MUL((RECT).x, ics) * ai; \
    (OUT).w = _SHIFT_MUL((RECT).w, ics); \
    (OUT).h = _SHIFT_MUL((RECT).h, ics) * al; \
    area = (OUT).w * (OUT).h; \
  }

static int setoffset3( int len, const fHaar* f0, fHaarWork* p, int al, int ai, REAL_TYPE cs, REAL_TYPE iv0, int is_int ) {
  INT ar0=0, ar1=0, ics = (int)(cs * _SHIFT1), iv = (int)(iv0 * _SHIFT1);
  iHaarWork3 * p3 = ( iHaarWork3 * ) p;
  if (is_int) {
    const iHaar * f = (const iHaar *)f0;
    const iHaar * l = f + len;
    iv = (int)(iv0);
    //int i;
    //for ( i=0; i< len ;i++, ++f, ++p ) {
    //printf("%d %d %d %d \n", f->ft0.x, f->ft0.y, f->ft0.w, f->ft0.h);
    for ( ; f < l; ++f, ++p3 ) {
      // if ( 0 == f->ft0.w || 0 == f->ft0.h || 0 == f->ft1.w || 0 == f->ft1.h )
      //   continue;
      SETOFFSET3( f->ft0, p3->ft0, ar0 );
      SETOFFSET3( f->ft1, p3->ft1, ar1 );
      p3->ft1.dd = ( f->ft1.d * iv );
      p3->ft0.dd = ( ar0 ) ? ( -( p3->ft1.dd * ar1 / ar0 ) ) : ( -p3->ft1.dd );
      //printf("ft1=%d %d %d %d\n", f->ft1.x, f->ft1.y, f->ft1.w, f->ft1.h);
    }
  } else {
    const fHaar * f = f0;
    const fHaar * l = f + len;
    iv = (int)(iv0);
    //int i;
    //for ( i=0; i< len ;i++, ++f, ++p ) {
    for ( ; f < l; ++f, ++p3 ) {
      // if ( 0 == f->ft0.w || 0 == f->ft0.h || 0 == f->ft1.w || 0 == f->ft1.h )
      //   continue;
      SETOFFSET3( f->ft0, p3->ft0, ar0 );
      SETOFFSET3( f->ft1, p3->ft1, ar1 );
      p3->ft1.dd = ( f->ft1.d * iv );
      p3->ft0.dd = ( ar0 ) ? ( -( p3->ft1.dd * ar1 / ar0 ) ) : ( -p3->ft1.dd );
    }
  }
  
  return 0;
}

#define cast_sqr_macro(x) ((x) * (x))

static int cvIntegralImage_C1R( int size_height, int size_width, const unsigned char* src, int srcstep, FIX_FLOAT* sum, int sumstep, FIX_FLOAT* sqsum, int sqsumstep, FIX_FLOAT* tilted, int tiltedstep ) {
  int x, y;
  FIX_FLOAT s;
  FIX_FLOAT sq;
	UNUSED(tilted);
	UNUSED(tiltedstep);
  //memset( ( size_width + 1 ), 0, ( size_width + 1 ) * sizeof( sum[ 0 ] ) );
  FILL(sumstep, sum, 0);
  sum += sumstep + 1;
  FILL(( size_width + 1 ), sqsum, 0);
  //memset( sqsum, 0, ( size_width + 1 ) * sizeof( sqsum[ 0 ] ) );
  sqsum += sqsumstep + 1;
  for ( y = 0; y < size_height; y++, src += srcstep, sum += sumstep, sqsum += sqsumstep ) {
    sum[ -1 ] = 0;
    sqsum[ -1 ] = 0;
    for ( x = 0, s = 0, sq = 0; x < size_width; x++ ) {
      FIX_FLOAT it = src[ x ];
      FIX_FLOAT t = ( it );
      FIX_FLOAT tq = cast_sqr_macro( it );
      s += t;
      sq += tq;
      t = sum[ x - sumstep ] + s;
      tq = sqsum[ x - sqsumstep ] + sq;
      sum[ x ] = t;
      sqsum[ x ] = tq;
    }
  }
  
  return 0;
}
#undef cast_sqr_macro

#include "lut_detect_1.inl"

static void* detect_fix(void* p0, void* buf, int bufsize) {
  lut_detect_param_t* p = (lut_detect_param_t*)p0;
  //int buflen;
  //buflen = detect_memsize( pa );
  //if (NULL!=membuf && buflen < bufsize)
  {
    int i, h, w;
    XRECT r = p->roi;
#if 0
    p->sc    = SHIFT_TOFLOAT(p->isc    );
    p->ssmin = SHIFT_TOFLOAT(p->issmin );
    p->ssmax = SHIFT_TOFLOAT(p->issmax );
    p->ss    = SHIFT_TOFLOAT(p->iss    );
    p->thd   = SHIFT_TOFLOAT(p->ithd   );
#endif
    if (r.w) {
      p->height = r.h;
      p->width = r.w;
    }
    p->h = h = XFLOOR(p->height*p->sc);
    p->w = w = XFLOOR(p->width*p->sc);
    BUFUSEBEGIN(buf, p->buflen);
    detect_set_cas(p, p->cas, p->casnum);
    for (i=0; i<p->casnum; ++i) {
      HAARCASCADE * ca = ( HAARCASCADE* )(p->cas[i]);
      p->pWirk[i] = BUFMALLOC( fHaarWork, ca->ihlen );
    }
    p->tmpimg0 = BUFMALLOC( unsigned char, w * h );
    p->tmpmask0 = BUFMALLOC( unsigned char, w * h );
    p->tmpimg = BUFMALLOC( unsigned char, w * h );
    p->tmpmask = BUFMALLOC( unsigned char, w * h );
    p->inter = BUFMALLOC( FIX_FLOAT, ( h + 1 ) * ( w + 1 ) );
    p->inter1 = BUFMALLOC( FIX_FLOAT, ( h + 1 ) * ( w + 1 ) );
    p->rect_partition_buf = BUFMALLOC( unsigned char, SEQPARTITIONSIZE(sizeof(XRECT), p->maxoutlen) );
    BUFUSEEND();
  }
  return p;
}

int detect_process(void* p0) {
  int ret;
  detect_info_t* p = (struct lut_detect_param_t *)p0;
  p = (struct lut_detect_param_t *)detect_fix(p, p->buf, p->buflen);
  if (NULL==p) {
    return 0;
  }
  //const HAARCASCADE* cas = p->cas;
  //ss = p->ss, ssmin = p->ssmin, ssmax = p->ssmax;
  //STS_set(&sts0, CLK_gethtime());
  //STS_delta(&sts0, CLK_gethtime());
#if 0
  if ( p->fastmode && 0 ) {
    double ss1, sss = 1.9, ssmin1;
    sss /= ssmin;
    //ssmax = MIN( ssmax, ( p->height / ( ( PHAARCASCADE ) cas ) ->h ) );
#if 0
    
    ss1 = pow( ss, 0.5 * ssmax / ssmin );
#else
    
    for ( ss1 = 1.f, ssmin1 = ssmin; ssmin1 < ssmax; ss1 += sss, ssmin1 *= ss1 ) {}
    
#endif
    
    ss1 = MAX( ss1, ss );
    for ( ssmin1 = ssmin; ssmin1 < ( ssmax / ss1 ); ssmin1 *= ss1 ) {}
    
    for ( ret = 0; !ret && ssmin1 >= ( ssmin - 0.00001 ); ssmax = ssmin1, ssmin1 /= ss1 ) {
      detect_info_t tmpalg = *p;
      //assert( ( 1.f / ssmin1 ) <= ( 1.1f / ssmin ) && "�ڴ����!" );
      tmpalg.sc = 1.f / ssmin1;
      tmpalg.ssmin = 1.f;
      tmpalg.ssmax = ssmax / ssmin1;
      //printf("%f  %f  %f  \n", ssmin, ssmin1, ssmax2);
      ret += lut_detect( &tmpalg );
    }
  } else 
#endif
  {
    //return 0;
  ret = lut_detect( p );
  }
  
  p->outlen = ret;
  return 1;
}

int detect_process2(lut_detect_param_t* p, int bufsize, XRECT* out, 
                    const unsigned char* data, const unsigned char* mask, 
                    const void* cas0, const void* cas1,
                    const void* cas2, const void* cas3) {
  int ret=0;
  //if (((int*)cas0)[0]==32) return 0;
  p->data = data;
  p->mask = mask;
  p->out = out;
  p->cas[0] = cas0;
  p->cas[1] = cas1;
  p->cas[2] = cas2;
  p->cas[3] = cas3;
  detect_set_cas(p, p->cas, 4);
  //if (((const HAARCASCADE*)(p->cas[0]))->w==32) return 0;
  detect_memsize(p);
  if (bufsize >= p->buflen) {
    p->buflen = bufsize;
    ret = detect_process(p);
  }
  return ret;
}

#define GET_EYE_RECT(_FACE, _DEYE, _LEYE, _REYE, _W, _DY, _CY, _DX, _CX) \
  ((_DEYE)->x = MAX(0, (_FACE)->x-(_DX)), \
  (_DEYE)->y  = (_FACE)->y+(_DY), \
  (_DEYE)->w = MIN(_W, (_FACE)->w+(_DX)+(_DX)), \
  (_DEYE)->h = (_CY), \
  (_LEYE)->x  = (_DEYE)->x, \
  (_LEYE)->y  = (_DEYE)->y, \
  (_LEYE)->w = (_CX), \
  (_LEYE)->h = (_DEYE)->h, \
  (_REYE)->x  = (_DEYE)->x+(_DEYE)->w-(_CX), \
  (_REYE)->y  = (_DEYE)->y, \
  (_REYE)->w = (_CX), \
  (_REYE)->h = (_DEYE)->h)
  

//static unsigned int NIRDoubleEYE[] = {
//#include "cas/NIRDoubleEYE.inl"
//                                      };
static int detect_eyes(int height, int width, const unsigned char* data, int datastep, void const* deyecas[], void const* leyecas0[], void const* reyecas0[],
                const XRECT* face, XRECT* deye, XRECT* leye, XRECT* reye, unsigned char* buf, int buflen, XRECT* out, int maxoutlen, int max_width, int is_trans, int is_flip, int is_flop) {
  float sc, mincx, kk=180.f;
  int ret, cy, dy, dx, cx, t;
  lut_detect_param_t pp _INIT0;
  XRECT face2=*face, ddeye, dleye, dreye;
  void const** leyecas=leyecas0;
  void const** reyecas=reyecas0;
  if (is_trans) {
    reyecas=leyecas0;
    leyecas=reyecas0;
  }
  //if (is_trans) { void const** t; CC_SWAP(leyecas, reyecas, t); }
  //deyecas[0] = leyecas[3];
  //leyecas[3]=0;
  if (deyecas[0]) {
    dx = (int)(face->w * ( 0.1 ));    // ˮƽƫ��
    cx = (int)(face->w * ( 0.4 ))+dx; // ��
    dy = (int)(face->h * ( 0.1 ));  // ��ֱƫ��
    cy = (int)(face->h * ( 0.3 ));    // ��
  } else {
    dx = (int)(face->w * ( 0.0 ));    // ˮƽƫ��
    cx = (int)(face->w * ( 0.5 ))+dx; // ��
    dy = (int)(face->h * ( 0.15 ));   // ��ֱƫ��
    cy = (int)(face->h * ( 0.3 ));    // ��
  }
  mincx = face->w / kk;
  if (is_trans) {
    RECT_TRANS(face2, t);
  }
  GET_EYE_RECT(&face2, &ddeye, &dleye, &dreye, max_width, dy, cy, dx, cx);
  if (is_trans) {
    RECT_TRANS(ddeye, t);
    RECT_TRANS(dleye, t);
    RECT_TRANS(dreye, t);
    CC_SWAP(dleye, dreye, face2);
  }
  sc = 1.f / mincx;
  pp.buf = (unsigned char *)buf;
  //pp=*p;
  if (deye) *deye = ddeye;
#if 1
  if (deyecas[0]) {
    // left
    //imwrite( "./leye.bmp", deye->h, deye->w/2, pos, srcstep, 1 );
    detect_init(&pp, 0, 0, deye->h, deye->w, data, datastep,
      0, 0, sc, 4.f*kk/180.f, 20.f*kk/180.f, 1.05f, 1, 0.8f, 0, 0, out, maxoutlen, is_trans, is_flip, is_flop );
    pp.buf = (unsigned char *)buf;
    ret = 1;
    if (ret) {
      //NIRDoubleEYE
      pp.roi = ddeye;
      pp.is_flop = 0;
      pp.outlen=0;
      RECT_SET(pp.out[0], 0, 0, 0, 0);
      ret = detect_process2(&pp, buflen, out, data, 0, deyecas[0], deyecas[1], deyecas[2], deyecas[3]);
      //ASSERT(ret && "buf�ռ�̫���ˣ�");
      if (pp.outlen && deye) {
        *deye = pp.out[0];
        if (1) {
          ddeye = pp.out[0];
          if (is_trans) {
            ddeye.y = face->y;
            ddeye.h = face->h;
            dreye = dleye = ddeye;
            dleye.h/=2;
            dreye.h/=2;
            dreye.y+=dleye.h;
            CC_SWAP(dleye, dreye, face2);
          } else {
            ddeye.x = face->x;
            ddeye.w = face->w;
            dreye = dleye = ddeye;
            dleye.w/=2;
            dreye.w/=2;
            dreye.x+=dleye.w;
          }
        }
#if 0
        if (0) {
          if (is_trans) {
            dleye.x=dleye.x=deye->x;
            //dleye.w=dleye.w=deye->w;
          } else {
            dleye.y=dleye.y=deye->y;
            //dleye.h=dleye.h=deye->h;
          }
        }
#endif
      } else {
        deye->x=deye->y=deye->w=deye->h=0;
        return 0;
      }
      ret = ret && pp.outlen && pp.out[0].w && pp.out[0].h;
    }
  }
#endif
  detect_init(&pp, leyecas, 4, dleye.h, dleye.w, data, datastep,
    0, 0, sc, 1.4f*kk/180.f, 2.f*kk/180.f, 1.05f, 1, 0.8f, 5, 0, out, maxoutlen, is_trans, is_flip, is_flop );
  pp.buf = (unsigned char *)buf;
  pp.buflen = buflen;
  ret = 1;
  if (ret) {
    pp.outlen=0;
    RECT_SET(pp.out[0], 0, 0, 0, 0);
    pp.roi = dleye;
    pp.is_flop=0;
    //imwriterect(".\\leyeroi.bmp", data, datastep, 1, dleye.x, dleye.y, dleye.w, dleye.h);
    ret = detect_process2(&pp, buflen, out, data, 0, leyecas[0], leyecas[1], leyecas[2], leyecas[3]);
    ASSERT(ret && "buf�ռ�̫���ˣ�");
    if (pp.outlen && leye) {
      *leye = pp.out[0];
    }
    ret = ret && pp.outlen && pp.out[0].w && pp.out[0].h;
#if 0
    {
      int i=0;
      for (i=0; i<pp.outlen; ++i) {
        DrawRectangle2(pp.width, pp.height, pp.data, pp.datastep, 1, pp.out[i], CC_RGB(255, 0, 0), 2);
      }
      cvShowImage("asdf", pp.width, pp.height, pp.data, pp.datastep, 1);
      cvWaitKey(-1);
    }
#endif
  }
  // right
  //imwrite( "./reye.bmp", deye->h, deye->w/2, pos, srcstep, 1 );
  if (ret) {
    pp.outlen=0;
    RECT_SET(pp.out[0], 0, 0, 0, 0);
    pp.roi = dreye;
    pp.is_flop=0;
    ret = detect_process2(&pp, buflen, out, data, 0, reyecas[0], reyecas[1], reyecas[2], reyecas[3]);
    ASSERT(ret && "buf�ռ�̫���ˣ�");
    if (pp.outlen && reye) {
      *reye = pp.out[0];
    }
    ret = ret && pp.outlen && pp.out[0].w && pp.out[0].h;
  }
  if (deyecas[0] && leye->w && reye->w) {
    deye->x = leye->x-5-(leye->x&15);
    deye->w = reye->x+reye->w-leye->x+10+(leye->y&15);
    deye->h = deye->w/4;
  }
  return ret;
}

//unsigned char facebuf[ DETECT_MEMSIZE( 1000, 500, 500 ) ];
// 
// �������ܣ�������ķ��������Ҷ�ͼ����������λ��
// ����˵����
// ���룺
// cas - ���������� CPM_GetCascade ��ȡ
// [h, w, A, al] - �Ҷ�ͼƬ[��, ��, ���Ͻ�ָ��, ����]
// ����sc,ssmin,ssmax,ss,step���Ƽ����ƶ���ʽ��
// sc            - ͼ������ű��� �����0.25��ͼ������С��ԭ����0.25���ټ�⣬�������ֵ<0.00001�������Զ����Ų��ԡ�
// ����������ж����ԭʼ�����СΪ[20��20](����)����⺯�����[(20*ssmin)��(20*ssmin)]�Ĵ�С��ʼ(��ͼ���ϣ��������ң������ϵ���)����������ÿ����������ͼ���ٽ�����Ŵ�ss�����¿�ʼ������ֱ��[(20*ssmax)��(20*ssmax)]�Ĵ�СΪֹ��
// ssmin         - �������С����[0~INF]����ֵԽ��������ļ������С��Խ�󣬼���ٶ�Խ�죬��ֵԽС�������ļ������С��ԽС������ٶ�Խ�����ر�����ֵС��1��ʱ�򣩡�
// ssmax         - ����������[0~INF]����ֵԽ��������ļ��������Խ�󣬶��ٶ�Ӱ�첻��(����ֵ100)
// ss            - ����ÿ�ηŴ�ı���, ÿ�� ssmin*=ss;(����ֵ1.05��1.5)����ֵԽ�����ٶ�Խ�죬�����Խ�١�
// step          - ������������ƽ�ƵĲ����������Ϊ1��2��ÿ�ν������ƶ�1��2�����أ������Ϊ0����ÿ���ƶ�k������(k=��ǰ�����С/ԭʼ�����С)��
// ����mincnt,thd�����ϲ���ɸѡ�ϲ��������
// mincnt        - ��С����ϲ�����������ս���ļ�������5��ԭʼ���򰴸����ʷ���ϲ����ɵģ���ô���ĺϲ�������n��������ֻ���غϲ�������mincnt�Ľ��(mincnt һ����Ϊ5)����ֵԽ�������Խ�ͣ������Խ�͡�
// thd           - ����������򸲸��ʴ���thd����ô���������򽫻ᱻ�ϲ���һ��(����ֵ0.1 ~ 0.6)��ֱ�������������������ĸ����ʶ�������thdΪֹ��������ָ��������Ĺ����������ռ�����������֮�͵İٷֱȡ�
// ����mask,maskstepΪͼ�����룬������ֻ���������Ͻ����ض�Ӧ������ֵ��0�ļ���
// mask          - ͼ������ֻ��mask[y*mask_step+x]�ĵ�(x, y)��⣬maskΪ0������
// maskstep      - ���벽��
// ���:
// [B, B_len]    - �ⲿ����ľ��ο�����ָ�룬�ᰴ XRECT::count ֵ��������
// �������أ����B==NULL������Ҫ��B_len��С�����򷵻ؼ������Ŀ
// ʹ��˵����

int CPM_FaceDetect( const void* cas, int height, int width, const unsigned char* data, int datastep,
                   const unsigned char* mask, int maskstep, double sc, double ssmin, double ssmax, double ss, int stepxy,
                   double thd, int mincnt, XRECT* B, int B_len ) {
  //int fastmode = sc < 0.001; //����ģʽ
  lut_detect_param_t p={0};
  if (sc<0.01) {
    sc = 1./ssmin;
    ssmin = 1.;
  }
  detect_init(&p, &cas, 1, height, width, data, datastep, mask, maskstep, sc, ssmin, ssmax, ss, stepxy, thd, mincnt, 0, B, B_len, 0, 0, 0);
  p.buf = MALLOC(uchar, p.buflen);
  if (NULL!=p.buf) {
    detect_process(&p);
    FREE(p.buf);
  }
  return p.outlen;
}
typedef struct FaceDetectParams {
  double ssmin;
  double ssmax;
  double ss;
  int stepxy;
  double thd;
  int mincnt;
}FaceDetectParams;

FaceDetectParams* FaceDetectParamsDefault(FaceDetectParams* s) {
  s->ssmin = 1.;
  s->ssmax = 100.;
  s->ss = 1.1;
  s->stepxy = 1;
  s->thd = 0.81;
  s->mincnt = 5;
  return s;
}

static int cvFaceDetect( const void* cas, const img_t* im, const img_t* mask, double scale, const FaceDetectParams* params, XRECT* B, int B_len ) {
  img_t gry[1] _INIT0;
  FaceDetectParams _params[1] _INIT0;
  int ret = 0;
  const unsigned char* mask_ptr = NULL;
  int mask_step = 0;
  ASSERT(im->c==3 ||im->c==2 ||im->c==4 ||im->c==1 );
  if (im->c!=1) {
    //im2gry(im, gry);    im = gry;
  }
  if (NULL==params) {
    params = FaceDetectParamsDefault(_params);
  }
  if (mask) {
    if (!CC_ARE_SIZES_EQ(im, mask)) {
      CC_ERROR(CC_StsBadSize, "");
    }
    mask_ptr = mask->tt.data;
    mask_step = mask->step;
  }
  ret = CPM_FaceDetect(cas, im->h, im->w, im->tt.data, im->s, mask_ptr, mask_step,
    scale, params->ssmin, params->ssmax, params->ss, params->stepxy, params->thd, params->mincnt, B, B_len);
  imfree(gry);
  return ret;
}

int objdetect(const void* cas, int height, int width, const unsigned char* data, int datastep,
	const unsigned char* mask, int maskstep, double sc, double ssmin, double ssmax, double ss, int stepxy,
	double thd, int mincnt, int* xywh, int xywh_len, int is_trans) {
	//int fastmode = sc < 0.001; //����ģʽ
	lut_detect_param_t p = { 0 };
	XRECT* B = (XRECT*)xywh;
	int B_len = xywh_len * sizeof(int) / sizeof(XRECT);
	if (sc<0.01) {
		sc = 1. / ssmin;
		ssmin = 1.;
	}
	detect_init(&p, &cas, 1, height, width, data, datastep, mask, maskstep, sc, ssmin, ssmax, ss, stepxy, thd, mincnt, 0, B, B_len, is_trans, 0, 0);
	MYREALLOC(p.buf, p.buflen);
	if (NULL != p.buf) {
		detect_process(&p);
		FREE(p.buf);
	}
	for (int i = 0; i < p.outlen; ++i) {
		int x = B[i].x, y = B[i].y, w = B[i].w, h = B[i].h;
		xywh[i * 4 + 0] = x;
		xywh[i * 4 + 1] = y;
		xywh[i * 4 + 2] = w;
		xywh[i * 4 + 3] = h;
	}
	return p.outlen;
}

int face_detect(int height, int width, const unsigned char* data, int datastep,
                const unsigned char* mask, int maskstep, double sc, double ssmin, double ssmax, double ss, int stepxy,
                double thd, int mincnt, int* xywh, int xywh_len, int is_trans ) {
  //int fastmode = sc < 0.001; //����ģʽ
  const void* cas = vis_nesting_face20110713;
  int ret = objdetect(cas, height, width, data, datastep,
	  mask, maskstep, sc, ssmin, ssmax, ss, stepxy,
	  thd, mincnt, xywh, xywh_len, is_trans);
  return ret;
}

int cas_ihlen(const void* cas[], int casnum);

// �������ܣ�������������ķ�������������Ҷ�ͼ�����۾���λ��
// ����˵����
// [A, al]     - ͼƬ[���Ͻ�ָ��, ����]
// face        - �������λ��
// deye        - ˫�۷���������������� CPM_GetCascade (NIR_DEYE_CASCADE) ��ȡ���ɼ��⴫ NULL
// leye        - ���۷���������������� CPM_GetCascade (NIR_LEYE_GLASS_CASCADE) ��ȡ���ɼ����� CPM_GetCascade (VIS_EYE_CASCADE) ��ȡ
// reye        - ���۷���������������� CPM_GetCascade (NIR_REYE_GLASS_CASCADE) ��ȡ���ɼ����� CPM_GetCascade (VIS_EYE_CASCADE) ��ȡ
// lalleye     - ���۴ּ����������������� CPM_GetCascade (NIR_EYE_CASCADE) ��ȡ���ɼ��⴫ NULL
// ralleye     - ���۴ּ����������������� CPM_GetCascade (NIR_EYE_CASCADE) ��ȡ���ɼ��⴫ NULL
// leye_without_glass  - ���۲����۾������������������ CPM_GetCascade (NIR_EYE_WITHOUT_GLASS_CASCADE) ��ȡ���ɼ��⴫ NULL
// reye_without_glass  - ���۲����۾������������������ CPM_GetCascade (NIR_EYE_WITHOUT_GLASS_CASCADE) ��ȡ���ɼ��⴫ NULL
// mincnt      - ������������������ XRECT::count ֵ��������ܽ�� �ٷ�֮ mincnt һ����Ϊ5
// [B, B_end]  - ��� buffer �������10
// �������أ��������Ŀ, 3����0, 3��ʾ��⵽���ۺ�����, 0��ʾ���ۻ�����������һ��û�м�⵽��
//          �����⵽ B[0] ���˫�۽����B[1] ������۽����B[2] ������۽����
// ʹ��˵��������������Խ������Խ�����Խ�󣬷�֮�����Խ�����ԽС
static int CPM_EyesDetect( const void* deye, const void* leye, const void* reye,
                    const void* lalleye, const void* ralleye, const void* leye_without_glass,
                    const void* reye_without_glass, const unsigned char* A, int al, int mincnt,
                    const XRECT* face, XRECT* B, int B_len ) {
  XRECT out[50]={0};
  int ret = 0, maxoutlen = 50, buflen = 0, ihlen;
  unsigned char* buf = 0;
  const void* deyecas[4], * leyecas[4], * reyecas[4];
  deyecas[0] = deye, deyecas[1] = 0, deyecas[2] = 0, deyecas[3] = 0;
  leyecas[0] = leye, leyecas[1] = lalleye, leyecas[2] = leye_without_glass, leyecas[3] = 0;
  reyecas[0] = reye, reyecas[1] = ralleye, reyecas[2] = reye_without_glass, reyecas[3] = 0;
  ihlen = MAX(cas_ihlen(leyecas, 4), cas_ihlen(reyecas, 4));
  buflen = DETECT_MEMSIZE(ihlen, MAX(128,face->h), MAX(128,face->w), maxoutlen );
  buf = (unsigned char*)malloc(buflen);
  if (buf) {
    ret = detect_eyes(0, 0, A, al, deyecas, leyecas, reyecas, (face), (B), (B+1), (B+2), buf, buflen, out, maxoutlen, al, 0, 0, 0);
    free(buf);
    if (ret) {
      ret = 3;
    }
  }
  return ret;
}

// �������ڹ�һ���ķ���任����
CC_INLINE MATRIX3X2 get_std( IPOINT s1, IPOINT e1, IPOINT s2, IPOINT e2 ) {
  MATRIX3X2 m;
  double c, s;
  double dd = POINT_DIST( s1, e1 ) / ( POINT_DIST( s2, e2 ) );
  ANGLE_SIN_COS( e1.x - s1.x, e1.y - s1.y, e2.x - s2.x, e2.y - s2.y, s, c );
  c *= dd;
  s *= dd;
  MATRIX3X2_GET_STD_TOINT( m, c, s, s1, s2 );
  return m;
}
//  �������ܣ�������һ��
//  ����˵����
//  [ah, aw, A, al, ai] - ����ͼƬ
//  [bh, bw, B, bl, bi] - ���ͼƬ
//  leye, reye - ����������λ��
//  lx - ����x�������bw һ��ȡ 0.25
//  ly - ����y�������bh һ��ȡ 0.25
//  rx - ����x�������bw һ��ȡ 0.75
//  ry - ����y�������bh һ��ȡ 0.25
//  �������أ�������
//  ʹ��˵����
int CPM_FaceStd( int ah, int aw, const unsigned char* A, int al, int ai,
                int bh, int bw, unsigned char* B, int bl, int bi,
                IPOINT leye, IPOINT reye,
                double lx, double ly, double rx, double ry ) {
  IPOINT s2, e2;
  MATRIX3X2 m;
  s2.x = ( int ) ( bw * lx );
  s2.y = ( int ) ( bh * ly );
  e2.x = ( int ) ( bw * rx );
  e2.y = ( int ) ( bh * ry );
  m = get_std( leye, reye, s2, e2 );
  IMAFFINE( ah, aw, A, al, ai, bh, bw, B, bl, bi, m, INTER_BILINEARITY );
  return 0;
}

int CPM_FaceStd2( int ah, int aw, const unsigned char* A, int al, int ai,
                int bh, int bw, unsigned char* B, int bl, int bi,
                IPOINT leye, IPOINT reye,
                double lx, double ly, double rx, double ry ) {
  IPOINT s2, e2;
  MATRIX3X2 m;
  s2.x = ( int ) ( bw * lx );
  s2.y = ( int ) ( bh * ly );
  e2.x = ( int ) ( bw * rx );
  e2.y = ( int ) ( bh * ry );
  m = get_std( leye, reye, s2, e2 );
  IMAFFINE2( ah, aw, A, al, ai, bh, bw, B, bl, bi, m, INTER_BILINEARITY );
  return 0;
}

#if 0
// ��methodָ���ķ����жϾ����Ƿ��ھ����б��У��������С��thd����Ϊ����������ȣ�
// ����ھͷ��ر�ţ����򷵻�-1
int is_rect_in_rectlist( const XRECT* rect, const XRECT* rectlist, int listlen, REAL_TYPE thd, int method ) {
  int i;
  for ( i = 0; i < listlen; ++i ) {
    int cover = 0;
    IS_RECT_EQUAL_COVER_RETE( *rect, rectlist[ i ], thd, cover );
    if ( cover ) {
      return i;
    }
  }
  return -1;
}

static iRectW getEyesRect( int imgWidth, const iRectW face_rect, double h1, double h2, double w1 ) {
  iRectW ir;
  ir.x = face_rect.x - ( int ) ( 0.5 + 0.5 * ( w1 - 1.0 ) * ( face_rect.w ) );
  if ( ir.x < 0 ) {
    ir.x = 0;
  }
  
  ir.y = face_rect.y + XROUND( h1 * ( face_rect.h ) );
  ir.w = XROUND( w1 * ( face_rect.w ) );
  if ( ( ir.x + ir.w ) >= imgWidth ) {
    ir.x = imgWidth - 1 - ir.w;
  }
  
  ir.h = XROUND( h2 * ( face_rect.h ) );
  return ir;
}

static void* memdup( const void* p, size_t n ) {
  void * o = pmalloc( n );
  if ( NULL != o ) {
    memcpy( o, p, n );
  }
  return o;
}

#endif

#endif // _FACEDETECT_INL_

