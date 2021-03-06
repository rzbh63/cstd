

#ifndef _STDC_TYPES_H_
#define _STDC_TYPES_H_

#include "stddef_c.h"
#include "inttypes_c.h"
#include <math.h>
#include <float.h>

typedef union suf32_t {
  int32 i;
  uint32 u;
  float f;
  uchar c[4];
  struct _fp32Format {
    unsigned int significand : 23;
    unsigned int exponent : 8;
    unsigned int sign : 1;
  } fmt;
}
suf32_t;
typedef union suf64_t {
  int64 i;
  uint64 u;
  double f;
  uchar c[8];
}
suf64_t;


#if 1
#define TYPEDEF_DEF(TYPEDEF) \
  TYPEDEF(CC_8U, u8,1,uchar  ) \
  TYPEDEF(CC_8S, s8,1,char   ) \
  TYPEDEF(CC_16U,u16,2,ushort ) \
  TYPEDEF(CC_16S,s16,2,short  ) \
  TYPEDEF(CC_32S,s32,4,int    ) \
  TYPEDEF(CC_32F,f32,4,float  ) \
  TYPEDEF(CC_64F,f64,8,double ) \
  TYPEDEF(CC_32U,u32,4,uint   ) \
  TYPEDEF(CC_64U,u64,8,uint64 ) \
  TYPEDEF(CC_64S,s64,8,int64  )
#endif
#if 0
#define TYPEDEF_DEF(TYPEDEF) \
  TYPEDEF(CC_8U, u8,1,uchar  ) \
  TYPEDEF(CC_8S, s8,1,char   ) \
  TYPEDEF(CC_16U,u16,2,ushort ) \
  TYPEDEF(CC_16S,s16,2,short  ) \
  TYPEDEF(CC_32S,s32,4,int    ) \
  TYPEDEF(CC_32F,f32,4,float  ) \
  TYPEDEF(CC_64F,f64,8,double ) \
  TYPEDEF(CC_32U,u32,4,uint   ) \
  TYPEDEF(CC_64U,u64,8,uint64 ) \
  TYPEDEF(CC_64S,s64,8,int64  )
#endif
//typedef void* CFuncTableCn[32][5];
//typedef void* CFuncTable[32];

#define TYPESIZE(type)   ((type)>>16)


// Matrix type
typedef enum {
  CC_NUL = -1,
#define TYPEDEF(a,b,c,d)  a,
  TYPEDEF_DEF(TYPEDEF)
#undef TYPEDEF
  cc_uchar = CC_8U,
  cc_schar = CC_8S,
  cc_char = CC_8S,
  cc_ushort = CC_8U,
  cc_short = CC_8S,
  cc_uint = CC_32U,
  cc_int = CC_32S,
  cc_int32 = CC_32S,
  cc_float = CC_32F,
  cc_double = CC_64F,

  CC_UCHAR = CC_8U,
  CC_SCHAR = CC_8S,
  CC_CHAR = CC_8S,
  CC_USHORT = CC_8U,
  CC_SHORT = CC_8S,
  CC_UINT = CC_32U,
  CC_INT = CC_32S,
  CC_INT32 = CC_32S,
  CC_FLOAT = CC_32F,
  CC_DOUBLE = CC_64F,

} TypeId;

#define CC_CN_MAX     64
#define CC_CN_SHIFT   3
#define CC_CN_MASK    ((1<<CC_CN_SHIFT)-1)
#define _MAKETYPECN(depth,cn)   ((depth) + (((cn)-1) << CC_CN_SHIFT))
#define CC_TYPECN_CN(typecn)    (( (typecn)>>CC_CN_SHIFT ) + 1 )
#define CC_TYPECN_TYPE(typecn)  (TypeId)( ((typecn) & CC_CN_MASK) )
#define CC_TYPECN_DEPTH   CC_TYPECN_TYPE
typedef enum {
#define TDD(a,b,c)  a##C##c=_MAKETYPECN(a, c)
#define TYPEDEF(a,b,c,d)  TDD(a,b,1),TDD(a,b,2),TDD(a,b,3),TDD(a,b,4),
  TYPEDEF_DEF(TYPEDEF)
#undef TYPEDEF
} TypeCnId;


CC_INLINE int cvTypeSize(TypeId type)
{
  static int _type_size_tab[16] = { 0 };
  if (0 == _type_size_tab[CC_8U]) {
#define TYPEDEF(a,b,c,d)  _type_size_tab[a]=c;
    TYPEDEF_DEF(TYPEDEF);
#undef TYPEDEF
  }
  return _type_size_tab[type];
}

CC_INLINE char* cvTypeName(TypeId type)
{
  static char* _type_name_tab[16] = { 0 };
  if (NULL == _type_name_tab[CC_8U]) {
#define TYPEDEF(a,b,c,d)  _type_name_tab[a]= (char*)#a ;
    TYPEDEF_DEF(TYPEDEF);
#undef TYPEDEF
  }
  return _type_name_tab[type];
}

#define CC_TYPE_SIZE(type)      cvTypeSize(type)
#define CC_TYPE_NAME(type)      cvTypeName(type)


/////////////////////////////////////////////////////////

#define TYPECVTDEF_DEF(TYPECVTDEF) \
TYPECVTDEF(CC_8U,u1,1,uchar,CC_8U,u1,1,uchar) \
TYPECVTDEF(CC_8U,u1,1,uchar,CC_8S,s1,1,char) \
TYPECVTDEF(CC_8U,u1,1,uchar,CC_16U,u2,2,ushort) \
TYPECVTDEF(CC_8U,u1,1,uchar,CC_16S,s2,2,short) \
TYPECVTDEF(CC_8U,u1,1,uchar,CC_32U,u4,4,uint) \
TYPECVTDEF(CC_8U,u1,1,uchar,CC_32S,s4,4,int) \
TYPECVTDEF(CC_8U,u1,1,uchar,CC_64U,u8,8,uint64) \
TYPECVTDEF(CC_8U,u1,1,uchar,CC_64S,s8,8,int64) \
TYPECVTDEF(CC_8U,u1,1,uchar,CC_32F,f4,4,float) \
TYPECVTDEF(CC_8U,u1,1,uchar,CC_64F,f8,8,double) \
TYPECVTDEF(CC_8S,s1,1,char,CC_8U,u1,1,uchar) \
TYPECVTDEF(CC_8S,s1,1,char,CC_8S,s1,1,char) \
TYPECVTDEF(CC_8S,s1,1,char,CC_16U,u2,2,ushort) \
TYPECVTDEF(CC_8S,s1,1,char,CC_16S,s2,2,short) \
TYPECVTDEF(CC_8S,s1,1,char,CC_32U,u4,4,uint) \
TYPECVTDEF(CC_8S,s1,1,char,CC_32S,s4,4,int) \
TYPECVTDEF(CC_8S,s1,1,char,CC_64U,u8,8,uint64) \
TYPECVTDEF(CC_8S,s1,1,char,CC_64S,s8,8,int64) \
TYPECVTDEF(CC_8S,s1,1,char,CC_32F,f4,4,float) \
TYPECVTDEF(CC_8S,s1,1,char,CC_64F,f8,8,double) \
TYPECVTDEF(CC_16U,u2,2,ushort,CC_8U,u1,1,uchar) \
TYPECVTDEF(CC_16U,u2,2,ushort,CC_8S,s1,1,char) \
TYPECVTDEF(CC_16U,u2,2,ushort,CC_16U,u2,2,ushort) \
TYPECVTDEF(CC_16U,u2,2,ushort,CC_16S,s2,2,short) \
TYPECVTDEF(CC_16U,u2,2,ushort,CC_32U,u4,4,uint) \
TYPECVTDEF(CC_16U,u2,2,ushort,CC_32S,s4,4,int) \
TYPECVTDEF(CC_16U,u2,2,ushort,CC_64U,u8,8,uint64) \
TYPECVTDEF(CC_16U,u2,2,ushort,CC_64S,s8,8,int64) \
TYPECVTDEF(CC_16U,u2,2,ushort,CC_32F,f4,4,float) \
TYPECVTDEF(CC_16U,u2,2,ushort,CC_64F,f8,8,double) \
TYPECVTDEF(CC_16S,s2,2,short,CC_8U,u1,1,uchar) \
TYPECVTDEF(CC_16S,s2,2,short,CC_8S,s1,1,char) \
TYPECVTDEF(CC_16S,s2,2,short,CC_16U,u2,2,ushort) \
TYPECVTDEF(CC_16S,s2,2,short,CC_16S,s2,2,short) \
TYPECVTDEF(CC_16S,s2,2,short,CC_32U,u4,4,uint) \
TYPECVTDEF(CC_16S,s2,2,short,CC_32S,s4,4,int) \
TYPECVTDEF(CC_16S,s2,2,short,CC_64U,u8,8,uint64) \
TYPECVTDEF(CC_16S,s2,2,short,CC_64S,s8,8,int64) \
TYPECVTDEF(CC_16S,s2,2,short,CC_32F,f4,4,float) \
TYPECVTDEF(CC_16S,s2,2,short,CC_64F,f8,8,double) \
TYPECVTDEF(CC_32U,u4,4,uint,CC_8U,u1,1,uchar) \
TYPECVTDEF(CC_32U,u4,4,uint,CC_8S,s1,1,char) \
TYPECVTDEF(CC_32U,u4,4,uint,CC_16U,u2,2,ushort) \
TYPECVTDEF(CC_32U,u4,4,uint,CC_16S,s2,2,short) \
TYPECVTDEF(CC_32U,u4,4,uint,CC_32U,u4,4,uint) \
TYPECVTDEF(CC_32U,u4,4,uint,CC_32S,s4,4,int) \
TYPECVTDEF(CC_32U,u4,4,uint,CC_64U,u8,8,uint64) \
TYPECVTDEF(CC_32U,u4,4,uint,CC_64S,s8,8,int64) \
TYPECVTDEF(CC_32U,u4,4,uint,CC_32F,f4,4,float) \
TYPECVTDEF(CC_32U,u4,4,uint,CC_64F,f8,8,double) \
TYPECVTDEF(CC_32S,s4,4,int,CC_8U,u1,1,uchar) \
TYPECVTDEF(CC_32S,s4,4,int,CC_8S,s1,1,char) \
TYPECVTDEF(CC_32S,s4,4,int,CC_16U,u2,2,ushort) \
TYPECVTDEF(CC_32S,s4,4,int,CC_16S,s2,2,short) \
TYPECVTDEF(CC_32S,s4,4,int,CC_32U,u4,4,uint) \
TYPECVTDEF(CC_32S,s4,4,int,CC_32S,s4,4,int) \
TYPECVTDEF(CC_32S,s4,4,int,CC_64U,u8,8,uint64) \
TYPECVTDEF(CC_32S,s4,4,int,CC_64S,s8,8,int64) \
TYPECVTDEF(CC_32S,s4,4,int,CC_32F,f4,4,float) \
TYPECVTDEF(CC_32S,s4,4,int,CC_64F,f8,8,double) \
TYPECVTDEF(CC_64U,u8,8,uint64,CC_8U,u1,1,uchar) \
TYPECVTDEF(CC_64U,u8,8,uint64,CC_8S,s1,1,char) \
TYPECVTDEF(CC_64U,u8,8,uint64,CC_16U,u2,2,ushort) \
TYPECVTDEF(CC_64U,u8,8,uint64,CC_16S,s2,2,short) \
TYPECVTDEF(CC_64U,u8,8,uint64,CC_32U,u4,4,uint) \
TYPECVTDEF(CC_64U,u8,8,uint64,CC_32S,s4,4,int) \
TYPECVTDEF(CC_64U,u8,8,uint64,CC_64U,u8,8,uint64) \
TYPECVTDEF(CC_64U,u8,8,uint64,CC_64S,s8,8,int64) \
TYPECVTDEF(CC_64U,u8,8,uint64,CC_32F,f4,4,float) \
TYPECVTDEF(CC_64U,u8,8,uint64,CC_64F,f8,8,double) \
TYPECVTDEF(CC_64S,s8,8,int64,CC_8U,u1,1,uchar) \
TYPECVTDEF(CC_64S,s8,8,int64,CC_8S,s1,1,char) \
TYPECVTDEF(CC_64S,s8,8,int64,CC_16U,u2,2,ushort) \
TYPECVTDEF(CC_64S,s8,8,int64,CC_16S,s2,2,short) \
TYPECVTDEF(CC_64S,s8,8,int64,CC_32U,u4,4,uint) \
TYPECVTDEF(CC_64S,s8,8,int64,CC_32S,s4,4,int) \
TYPECVTDEF(CC_64S,s8,8,int64,CC_64U,u8,8,uint64) \
TYPECVTDEF(CC_64S,s8,8,int64,CC_64S,s8,8,int64) \
TYPECVTDEF(CC_64S,s8,8,int64,CC_32F,f4,4,float) \
TYPECVTDEF(CC_64S,s8,8,int64,CC_64F,f8,8,double) \
TYPECVTDEF(CC_32F,f4,4,float,CC_8U,u1,1,uchar) \
TYPECVTDEF(CC_32F,f4,4,float,CC_8S,s1,1,char) \
TYPECVTDEF(CC_32F,f4,4,float,CC_16U,u2,2,ushort) \
TYPECVTDEF(CC_32F,f4,4,float,CC_16S,s2,2,short) \
TYPECVTDEF(CC_32F,f4,4,float,CC_32U,u4,4,uint) \
TYPECVTDEF(CC_32F,f4,4,float,CC_32S,s4,4,int) \
TYPECVTDEF(CC_32F,f4,4,float,CC_64U,u8,8,uint64) \
TYPECVTDEF(CC_32F,f4,4,float,CC_64S,s8,8,int64) \
TYPECVTDEF(CC_32F,f4,4,float,CC_32F,f4,4,float) \
TYPECVTDEF(CC_32F,f4,4,float,CC_64F,f8,8,double) \
TYPECVTDEF(CC_64F,f8,8,double,CC_8U,u1,1,uchar) \
TYPECVTDEF(CC_64F,f8,8,double,CC_8S,s1,1,char) \
TYPECVTDEF(CC_64F,f8,8,double,CC_16U,u2,2,ushort) \
TYPECVTDEF(CC_64F,f8,8,double,CC_16S,s2,2,short) \
TYPECVTDEF(CC_64F,f8,8,double,CC_32U,u4,4,uint) \
TYPECVTDEF(CC_64F,f8,8,double,CC_32S,s4,4,int) \
TYPECVTDEF(CC_64F,f8,8,double,CC_64U,u8,8,uint64) \
TYPECVTDEF(CC_64F,f8,8,double,CC_64S,s8,8,int64) \
TYPECVTDEF(CC_64F,f8,8,double,CC_32F,f4,4,float) \
TYPECVTDEF(CC_64F,f8,8,double,CC_64F,f8,8,double)

static void* arrcvt(void* dst, TypeId dst_type, const void* src, TypeId src_type, int n)
{
  int i = 0;
  if (NULL == dst) {}
  switch ((dst_type << 8) | src_type) {
#define TYPECVTDEF(a, b, c, d, e, f, g, h)  case ((a<<8) | e): { d* _d = (d*)dst; const h* _s = (const h*) src; \
    for (; i<n-3; i+=4) {_d[i] = (d)_s[i];_d[i+1] = (d)_s[i+1];_d[i+2] = (d)_s[i+2];_d[i+3] = (d)_s[i+3]; } for (; i<n; ++i) {_d[i] = (d)_s[i]; } } break;
    TYPECVTDEF_DEF(TYPECVTDEF)
#undef TYPECVTDEF
  }
  return dst;
}
static void* arrcvt2(void* dst, TypeId dst_type, const void* src, TypeId src_type, int n, double alpha, double beta)
{
  BOOL noScale = fabs(alpha - 1) < DBL_EPSILON && fabs(beta) < DBL_EPSILON;
  if (noScale) {
    return arrcvt(dst, dst_type, src, src_type, n);
  } else {
    int i = 0;
    if (NULL == dst) {}
    switch ((dst_type << 8) | src_type) {
#define OP(a)  (alpha*(a) + beta)
#define TYPECVTDEF(a, b, c, d, e, f, g, h)  case ((a<<8) | e): { d* _d = (d*)dst; const h* _s = (const h*) src; \
    for (; i<n-3; i+=4) {_d[i] = (d)OP(_s[i]);_d[i+1] = (d)OP(_s[i+1]);_d[i+2] = (d)OP(_s[i+2]);_d[i+3] = (d)OP(_s[i+3]); } for (; i<n; ++i) {_d[i] = (d)OP(_s[i]); } } break;
      TYPECVTDEF_DEF(TYPECVTDEF)
#undef TYPECVTDEF
#undef OP
    }
    return dst;
  }
}
static void* arrcvt2d(void* dst, TypeId dsttype, int dl, const void* src, TypeId srctype, int sl, int h, int w, double alpha, double beta)
{
  char* d = (char*)(dst);
  const char* s = (const char*)(src);
  int dw = w * (dsttype >> 16);
  int sw = w * (srctype >> 16);
  dl = dl > 0 ? dl : dw;
  sl = sl > 0 ? sl : sw;
  if (dl == dw && sl == sw) {
    return arrcvt2(dst, dsttype, src, srctype, h * w, alpha, beta);
  }
  for (; h--; s += sl, d += dl) {
    arrcvt2(d, dsttype, s, srctype, w, alpha, beta);
  }
  return dst;
}

/////////////////////////////////////////////////////////

#define _POINT_DEF2(Tfix, tfix, type)  \
  struct Tfix##POINT { type x, y;\
  Tfix##POINT& operator += (const Tfix##POINT& b) { x += b.x, y += b.y; return *this; } \
  Tfix##POINT& operator -= (const Tfix##POINT& b) { x -= b.x, y -= b.y; return *this; } \
  type norm() {return x*x+y*y;} \
  };  \
  CC_INLINE Tfix##POINT operator - (const Tfix##POINT& a, const Tfix##POINT& b) { Tfix##POINT c; c.x = a.x - b.x, c.y = a.y - b.y; return c; } \
  CC_INLINE type norm(const Tfix##POINT& a) {return a.x*a.x+a.y*a.y;} \
  typedef struct Tfix##POINT3 { type x, y, z;\
  Tfix##POINT3& operator += (const Tfix##POINT3& b) { x += b.x, y += b.y, z += b.z; return *this; } \
  } Tfix##Point3;  \
  typedef struct Tfix##POINT4 { type x, y, z, w;\
  Tfix##POINT4& operator += (const Tfix##POINT4& b) { x += b.x, y += b.y, z += b.z, w += b.w; return *this; } \
  } Tfix##Point4;  \
  typedef struct Tfix##SIZE {  \
    union { type x; type cx; type w; type width; type c; }; \
    union { type y; type cy; type h; type height; type r; }; \
  } Tfix##Size;  \
  struct Tfix##RECT { \
    union { type x0; type l; type x; type left; }; \
    union { type y0; type t; type y; type top; }; \
    union { type x1; type r; type right; }; \
    union { type y1; type b; type bottom; }; \
};  \
  struct Tfix##Rect { \
    union { type l; type x; type left; }; \
    union { type t; type y; type top; }; \
    union { type cx; type w; type width; type c; }; \
    union { type cy; type h; type height; type r; }; \
    type area() {return w*h;} \
    Tfix##POINT tl() {Tfix##POINT p;p.x=x, p.y=y;return p;} \
    Tfix##POINT br() {Tfix##POINT p;p.x=x+w, p.y=y+h;return p;} \
}; \
  typedef struct Tfix##POINT _Point2##tfix; \
  typedef struct Tfix##SIZE _Size2##tfix; \
  typedef struct Tfix##POINT Tfix##Point; \
  typedef struct Tfix##POINT Tfix##Vec2; \
  typedef struct Tfix##POINT3 Tfix##Vec3; \
  typedef struct Tfix##POINT4 Tfix##Vec4; \
  typedef struct Tfix##POINT Tfix##POINT2; \
  typedef struct Tfix##RECT Tfix##BBox; \
  typedef Tfix##POINT Tfix##2VECTOR; \
  typedef Tfix##POINT3 Tfix##3VECTOR; \
  typedef Tfix##POINT4 Tfix##4VECTOR; \
  CC_INLINE Tfix##POINT tfix##POINT(type x, type y) { Tfix##POINT pt;  pt.x = x, pt.y = y; return pt; } \
  CC_INLINE Tfix##Point tfix##Point(type x, type y) { Tfix##Point pt;  pt.x = x, pt.y = y; return pt; } \
  CC_INLINE Tfix##Vec2 tfix##Vec2(type x, type y) { Tfix##Vec2 pt;  pt.x = x, pt.y = y; return pt; } \
  CC_INLINE Tfix##POINT3 tfix##POINT3(type x, type y, type z) { Tfix##POINT3 pt;  pt.x = x, pt.y = y, pt.z = z; return pt; } \
  CC_INLINE Tfix##Point3 tfix##Point3(type x, type y, type z) { Tfix##Point3 pt;  pt.x = x, pt.y = y, pt.z = z; return pt; } \
  CC_INLINE Tfix##Vec3 tfix##Vec3(type x, type y, type z) { Tfix##Vec3 pt;  pt.x = x, pt.y = y, pt.z = z; return pt; } \
  CC_INLINE Tfix##POINT4 tfix##POINT4(type x, type y, type z, type w) { Tfix##POINT4 pt;  pt.x = x, pt.y = y, pt.z = z, pt.w = w; return pt; } \
  CC_INLINE Tfix##Point4 tfix##Point4(type x, type y, type z, type w) { Tfix##Point4 pt;  pt.x = x, pt.y = y, pt.z = z, pt.w = w; return pt; } \
  CC_INLINE Tfix##Vec4 tfix##Vec4(type x, type y, type z, type w) { Tfix##Vec4 pt;  pt.x = x, pt.y = y, pt.z = z, pt.w = w; return pt; } \
  CC_INLINE Tfix##SIZE tfix##SIZE(type w, type h) { Tfix##SIZE o;  o.w = w, o.h = h; return o; } \
  CC_INLINE Tfix##Size tfix##Size(type w, type h) { Tfix##Size o;  o.w = w, o.h = h; return o; } \
  CC_INLINE Tfix##RECT tfix##RECT(type l, type t, type r, type b) { Tfix##RECT o;  o.l = l, o.t = t, o.r = r, o.b = b; return o; } \
  CC_INLINE Tfix##RECT tfix##RECT2(type l, type t, type w, type h) { Tfix##RECT o;  o.l = l, o.t = t, o.r = l+w, o.b = t+h; return o; } \
  CC_INLINE Tfix##Rect tfix##Rect(type x, type y, type w, type h) { Tfix##Rect o;  o.l = x, o.t = y, o.w = w, o.h = h; return o; } \
  CC_INLINE bool operator==(Tfix##POINT a, Tfix##POINT b) { return a.x==b.x && a.y==b.y; } \
  CC_INLINE bool operator!=(Tfix##POINT a, Tfix##POINT b) { return a.x!=b.x || a.y!=b.y; } \
  CC_INLINE bool contains(Tfix##RECT r, Tfix##POINT pt) { return r.x <= pt.x && pt.x < r.r && r.y <= pt.y && pt.y < r.b; } \
  CC_INLINE bool contains(Tfix##Rect r, Tfix##POINT pt) { return r.x <= pt.x && pt.x < r.x + r.width && r.y <= pt.y && pt.y < r.y + r.height; } \
  CC_INLINE bool inside(Tfix##Point pt, Tfix##Rect r) {  return r.x <= pt.x && pt.x < r.x + r.width && r.y <= pt.y && pt.y < r.y + r.height; } \
  CC_INLINE Tfix##POINT tfix##POINT(Tfix##Rect r) { Tfix##POINT pt;  pt.x = r.x, pt.y = r.y; return pt; } \
  CC_INLINE Tfix##SIZE tfix##SIZE(Tfix##Rect r) { Tfix##SIZE pt;  pt.w = r.w, pt.h = r.h; return pt; } \


_POINT_DEF2(I, i, int);
_POINT_DEF2(S, s, short);
_POINT_DEF2(L, l, int64);
_POINT_DEF2(F, f, float);
_POINT_DEF2(D, d, double);
#undef _POINT_DEF2
typedef struct {
  double a00, a01, a02, a10, a11, a12, a20, a21, a22;
} DMATRIX33;
typedef struct {
  IPOINT p0, p1;
} ISEGMENT;
typedef struct {
  LPOINT2 p0, p1;
} LSEGMENT;
typedef struct {
  DPOINT p0, p1;
} DSEGMENT;
typedef struct {
  DPOINT3 p0, p1, p2;
} DSEGMENT3;
typedef DPOINT DPOINT2;
//typedef DPOINT3 DPOINT3;
typedef DSEGMENT DSEGMENT2;
typedef struct {
  int x, y, cmd;
} vertex_i;
typedef struct {
  int x1, x2;
  uint32* ptr;
} row_info;

typedef struct CRange {
  int start;
  int end;
} CRange;
#define Data_2DSize(d) ((d).w * (d).h)
#define Data_3DSize(d) ((d).w * (d).h * (d).c)
#define Data_4DSize(d) ((d).w * (d).h * (d).c * (d).n)
#define Data_total(d)  Data_4DSize(d)
#define Data_count(d)  Data_4DSize(d)
#define Data_4DEqu(a, b) ((a).w == (b).w && (a).h == (b).h && (a).c == (b).c && (a).n == (b).n)
#define Data_4DNeq(a, b) (!Data_4DEqu(a, b))
#define Data_2DIndex(d, ih, iw) ((ih) * (d).w + (iw))
#define Data_3DIndex(d, ic, ih, iw) (((ic) * (d).h + ih) * (d).w + (iw))
#define Data_4DIndex(d, in, ic, ih, iw)   ((((in) * (d).c + ic) * (d).h + ih) * (d).w + (iw))
#define Data_4DIndex1(d, in)           Data_4DIndex(d, in, 0, 0, 0)
#define Data_4DIndex2(d, in, ic)       Data_4DIndex(d, in, ic, 0, 0)
#define Data_4DIndex3(d, in, ic, ih)   Data_4DIndex(d, in, ic, ih, 0)

typedef union DataSize {
  struct { int n, c, h, w; };
  int dim[4];
} DataSize;
static DataSize dataSize(int _number, int _channels, int _height, int _width)
{
  DataSize ret = { _number, _channels, _height, _width };
  ret.n = _number;
  ret.c = _channels;
  ret.h = _height;
  ret.w = _width;
  return ret;
}
static int dataCount(DataSize size, int start, int end)
{
  int count = 1;
  start = BOUND(start, 0, end);
  end = BOUND(end, start, 4);
  for (; start < end; ++start) {
    count *= size.dim[start];
  }
  return count;
}
typedef FPOINT2 CPoint2D32f;
typedef FPOINT3 CPoint3D32f;
typedef DPOINT2 CPoint2D64f;
typedef DPOINT3 CPoint3D64f;

typedef IPOINT2 CPoint2i;
typedef IPOINT3 CPoint3i;
typedef LPOINT2 CPoint2l;
typedef LPOINT3 CPoint3l;
typedef FPOINT2 CPoint2f;
typedef FPOINT3 CPoint3f;
typedef DPOINT2 CPoint2d;
typedef DPOINT3 CPoint3d;

typedef LSIZE CSize2l;
typedef ISIZE CSize;
typedef IPOINT CPoint;
typedef FPOINT CPoint2D32f;
#define CC_SIZE_AREA(sz)   ((sz).width * (sz).height)
#define CC_SIZE_ISEQ(sz1, sz2)   ( ((sz1).width==(sz1).width) && ((sz1).height==(sz2).height) )

#define SETPT2(pt, _x, _y)  ((pt).x=_x, (pt).y=_y)
#define SETPT3(pt, _x, _y, _z)  ((pt).x=_x, (pt).y=_y, (pt).z=_z)


#define PTSET(pt, _x, _y)   ((pt).x=_x, (pt).y=_y)

#define iPOINT_PT(pt)  iPOINT((int)(pt).x, (int)(pt).y)

CC_INLINE int iPOINT_cmp(IPOINT a, IPOINT b)
{
  return a.y == b.y ? CC_CMP(a.x, b.x) : CC_CMP(a.y, b.y);
}
CC_INLINE IRANGE cRANGE(int s, int e)
{
  IRANGE ra;
  ra.s = s, ra.e = e;
  return ra;
}
CC_INLINE FPOINT iPointTof(IPOINT point)
{
  return fPOINT((float)point.x, (float)point.y);
}
CC_INLINE IPOINT fPointToi(FPOINT point)
{
  IPOINT ipt;
  ipt.x = (int)(point.x + .5);
  ipt.y = (int)(point.y + .5);
  return ipt;
}
CC_INLINE DSEGMENT dSEGMENT(double x0, double y0, double x1, double y1)
{
  DSEGMENT d;
  d.p0 = dPOINT(x0, y0), d.p1 = dPOINT(x1, y1);
  return d;
}
CC_INLINE ISEGMENT iSEGMENT(int x0, int y0, int x1, int y1)
{
  ISEGMENT d;
  d.p0 = iPOINT(x0, y0), d.p1 = iPOINT(x1, y1);
  return d;
}

#define PT2OPDEF(p1, op, p2)  (p1).x op (p2).x, (p1).y op (p2).y
#define PT3OPDEF(p1, op, p2)   (p1).x op (p2).x, (p1).y op (p2).y, (p1).z op (p2).z
#define RAND01() (rand()*1./RAND_MAX)
#define RANDX(_X) (rand()*(_X*1.)/RAND_MAX)
#define iround(v) (int)((v < 0.0) ? v - 0.5 : v + 0.5)
#define uround(v) (unsigned)(v + 0.5)
#define ufloor(v) (unsigned)(v)
#define uceil(v) (unsigned)(ceil(v))
#define PREV_INDEX(i, n) (i-1+n)%(n)
#define NEXT_INDEX(i, n) (i+1 )%(n)
#define PTADD(c, a, b) (c.x=a.x+b.x, c.y=a.y+b.y)
#define PTSUB(c, a, b) (c.x=a.x-b.x, c.y=a.y-b.y)
#define PTCLIP(pt, l, t, r, b) iPOINT(BOUND(pt.x, l, r), BOUND(pt.y, t, b))
#define RCLT(rc) iPOINT((rc)->l, (rc)->t)
#define RCLB(rc) iPOINT((rc)->l, (rc)->b)
#define RCRB(rc) iPOINT((rc)->r, (rc)->b)
#define RCSZ(rc) iSIZE(RCW(rc), RCH(rc))
#define SZMAX(sz1, sz2) iSIZE(MAX(sz1.w, sz2.w), MAX(sz1.h, sz2.h))
#define SZADD(sz1, sz2) iSIZE(sz1.w+sz2.w, sz1.h+sz2.h)
#define SZADD2(sz1, CX, CY) iSIZE(sz1.w+CX, sz1.h+CY)
#define RCSET(prc, L, T, R, B) ((prc)->l=L, (prc)->t=T, (prc)->r=R, (prc)->b=B)
#define RCX(prc) ((prc)->r + (prc)->l)
#define RCY(prc) ((prc)->b + (prc)->t)
#define RCW(prc) ((prc)->r - (prc)->l)
#define RCH(prc) ((prc)->b - (prc)->t)
#define RCCENTER(rc)    iPOINT(RCX(rc)/2, RCY(rc)/2)
#define RCCENTERF(rc)   dPOINT(RCX(rc)/2., RCY(rc)/2.)
#define RCEQ(R1, R2)   (((R1)->l==(R2)->l) && ((R1)->t==(R2)->t) && ((R1)->r==(R2)->r) && ((R1)->b==(R2)->b))

CC_INLINE IRECT iRECT1(int x)
{
  IRECT rc;
  RCSET(&rc, x, x, x, x);
  return rc;
}
CC_INLINE IRECT iRECT_sz(ISIZE sz)
{
  IRECT rc;
  RCSET(&rc, 0, 0, sz.w, sz.h);
  return rc;
}
CC_INLINE IRECT iRECT_ps(IPOINT pt, ISIZE sz)
{
  IRECT rc;
  RCSET(&rc, pt.x, pt.y, pt.x + sz.w, pt.y + sz.h);
  return rc;
}
CC_INLINE IRECT iRECT_pp(IPOINT p1, IPOINT p2)
{
  IRECT rc;
  RCSET(&rc, p1.x, p1.y, p2.x, p2.y);
  return rc;
}
#define RECT2RECT(fun, T, rc) fun((T)((rc)->l), (T)((rc)->t), (T)((rc)->r), (T)((rc)->b));

CC_INLINE int iRectIsEmpty(const IRECT* rc)
{
  return rc->l >= rc->r || rc->t >= rc->b;
}
CC_INLINE int iRectNormalize(IRECT* prc)
{
  int nTemp;
  if (prc->l > prc->r) {
    nTemp = prc->l;
    prc->l = prc->r;
    prc->r = nTemp;
  }
  if (prc->t > prc->b) {
    nTemp = prc->t;
    prc->t = prc->b;
    prc->b = nTemp;
  }
  return 0;
}
CC_INLINE int fRectNormalize(FRECT* prc)
{
  float nTemp;
  if (prc->l > prc->r) {
    nTemp = prc->l;
    prc->l = prc->r;
    prc->r = nTemp;
  }
  if (prc->t > prc->b) {
    nTemp = prc->t;
    prc->t = prc->b;
    prc->b = nTemp;
  }
  return 0;
}
CC_INLINE int iRectInclude(IRECT r1, IRECT r2)
{
  iRectNormalize(&r1);
  iRectNormalize(&r2);
  return r1.l <= r2.l && r1.t <= r2.l && r1.r >= r2.r && r1.b >= r2.b;
}
CC_INLINE int iRectBoundUpdate(IRECT* prc, int x, int y)
{
  if (x < prc->l) {
    prc->l = x;
  } else if (x > prc->r) {
    prc->r = x;
  }
  if (y < prc->t) {
    prc->t = y;
  } else if (y > prc->b) {
    prc->b = y;
  }
  return 0;
}
CC_INLINE IRECT iRectInter(IRECT r1, IRECT r2)
{
  IRECT temp = { 0, 0, 0, 0 };
  iRectNormalize(&r1);
  iRectNormalize(&r2);
  // check for total exclusion
  if ((r1.r > r2.l) && (r1.l < r2.r) && (r1.b > r2.t) && (r1.t < r2.b)) {
    temp.l = (r1.l > r2.l) ? r1.l : r2.l;
    temp.r = (r1.r < r2.r) ? r1.r : r2.r;
    temp.t = (r1.t > r2.t) ? r1.t : r2.t;
    temp.b = (r1.b < r2.b) ? r1.b : r2.b;
  }
  return temp;
}
CC_INLINE FRECT fRectInter(FRECT r1, FRECT r2)
{
  FRECT temp = { 0, 0, 0, 0 };
  fRectNormalize(&r1);
  fRectNormalize(&r2);
  // check for total exclusion
  if ((r1.r > r2.l) && (r1.l < r2.r) && (r1.b > r2.t) && (r1.t < r2.b)) {
    temp.l = (r1.l > r2.l) ? r1.l : r2.l;
    temp.r = (r1.r < r2.r) ? r1.r : r2.r;
    temp.t = (r1.t > r2.t) ? r1.t : r2.t;
    temp.b = (r1.b < r2.b) ? r1.b : r2.b;
  }
  return temp;
}
CC_INLINE BOOL iRectIsNull(const IRECT* pRect)
{
  return (pRect->l >= pRect->r || pRect->t >= pRect->b);
}
CC_INLINE BOOL iRectSet(IRECT* pRect, int l, int t, int r, int b)
{
  pRect->l = l, pRect->r = r, pRect->t = t, pRect->b = b;
  return 0;
}
CC_INLINE BOOL iRectSetEmpty(IRECT* pRect)
{
  pRect->l = pRect->r = pRect->t = pRect->b = 0;
  return 0;
}

#define iRectIn(rc, d)  iRectOffset(rc, d, d, -(d), -(d))
#define iRectOff2(rc, x, y)  iRectOffset((rc), x, y, x, y)
#define iRectOff0(rc)  iRectOff2((rc), -(rc)->l, -(rc)->t)
CC_INLINE IRECT iRectOffset(IRECT rc, int l, int t, int r, int b)
{
  rc.l += l, rc.t += t;
  rc.r += r, rc.b += b;
  return rc;
}
CC_INLINE IRECT iRectOffset2(IRECT rc, int x, int y)
{
  rc.l += x, rc.t += y;
  rc.r += x, rc.b += y;
  return rc;
}
CC_INLINE IRECT iRectOffsetTo2(IRECT rc, int x, int y)
{
  rc.l += x, rc.t += y;
  rc.r += x, rc.b += y;
  return rc;
}
CC_INLINE IRECT iRectDeflate(IRECT rc, int l, int t, int r, int b)
{
  rc.l += l, rc.t += t;
  rc.r -= r, rc.b -= b;
  return rc;
}
CC_INLINE IRECT iRectDeflate2(IRECT rc, int x, int y)
{
  rc.l += x, rc.t += y;
  rc.r -= x, rc.b -= y;
  return rc;
}
CC_INLINE IRECT iRectInflate(IRECT rc, int l, int t, int r, int b)
{
  rc.l -= l, rc.t -= t;
  rc.r += r, rc.b += b;
  return rc;
}
CC_INLINE IRECT iRectOffsetXY(IRECT rc, int x, int y)
{
  rc.l = rc.l + x, rc.t = rc.t + y;
  rc.r = rc.r + x, rc.b = rc.b + y;
  return rc;
}
CC_INLINE IRECT iRectAdd(IRECT rc, const IRECT rc2)
{
  rc.l += rc2.l, rc.t += rc2.t;
  rc.r += rc2.r, rc.b += rc2.b;
  return rc;
}
CC_INLINE IRECT iRectDeflateR(IRECT rc, const IRECT rc2)
{
  rc.l += rc2.l, rc.t += rc2.t;
  rc.r -= rc2.r, rc.b -= rc2.b;
  return rc;
}
CC_INLINE IRECT iRectInflateR(IRECT rc, const IRECT rc2)
{
  rc.l -= rc2.l, rc.t -= rc2.t;
  rc.r += rc2.r, rc.b += rc2.b;
  return rc;
}
CC_INLINE IRECT iRectInflate2(IRECT rc, int x, int y)
{
  rc.l -= x, rc.t -= y;
  rc.r += x, rc.b += y;
  return rc;
}
CC_INLINE IRECT iRectInflate1(IRECT rc, int x)
{
  rc.l -= x, rc.t -= x;
  rc.r += x, rc.b += x;
  return rc;
}
#define iPtInRect2(rc, pt)  iPtInRect(rc, (int)(pt).x, (int)(pt).y)
CC_INLINE int iPtInRect(const IRECT* rc, int x, int y)
{
  return (rc->l <= x && x < rc->r) && (rc->t <= y && y < rc->b);
}

CC_INLINE int iPtInRects(int n, const IRECT* rc, int x, int y)
{
  int i;
  for (i = 0; i < n; ++i) {
    if (iPtInRect(rc + i, x, y)) {
      return i;
    }
  }
  return -1;
}
CC_INLINE int iPtInRectsBorder(IRECT rc, BOOL horz, int n, const float* v, int gap, int x, int y, int ex)
{
  int i;
  float l = (float)rc.l;
  float t = (float)rc.t;
  if (horz) {
    for (i = 0; i < (n - 1); ++i) {
      float r = l + v[i];
      float l1 = r + gap;
      if (x > (r - ex) && x < (l1 + ex) && y > rc.t && y < rc.b) {
        return i;
      }
      l = l1;
    }
  } else {
    for (i = 0; i < (n - 1); ++i) {
      float b = t + v[i];
      float t1 = b + gap;
      if (y > (b - ex) && y < (t1 + ex) && x > rc.l && x < rc.r) {
        return i;
      }
      t = t1;
    }
  }
  return -1;
}

CC_INLINE int iPtInRectBorder(const IRECT* prc, int x, int y, int l, int t, int r, int b)
{
  IRECT rc = iRectOffset(*prc, -l, -t, r, b);
  if (iPtInRect(&rc, x, y) && !iPtInRect(prc, x, y)) {
    return (x < prc->l) << 0 | (x > prc->r) << 1 | (y < prc->t) << 2 | (y > prc->b) << 3;
  }
  return 0;
}
CC_INLINE IRECT iRectToSquare(IRECT rc)
{
  IRECT rc1 = rc;
  int d = MIN(RCH(&rc), RCW(&rc));
  rc1.r = rc.l + d, rc1.b = rc.t + d;
  return rc1;
}

// 自定义函数，计算两矩形 IOU，传入为均为矩形对角线，（x,y）  坐标。·
CC_INLINE double iRectIOU(IRECT Reframe, IRECT GTframe)
{
  double ratio;
  int x1 = Reframe.l;
  int y1 = Reframe.t;
  int width1 = Reframe.r - Reframe.l;
  int height1 = Reframe.b - Reframe.t;
  int x2 = GTframe.l;
  int y2 = GTframe.t;
  int width2 = GTframe.r - GTframe.l;
  int height2 = GTframe.b - GTframe.t;
  int endx = MAX(x1 + width1, x2 + width2);
  int startx = MIN(x1, x2);
  int width = width1 + width2 - (endx - startx);
  int endy = MAX(y1 + height1, y2 + height2);
  int starty = MIN(y1, y2);
  int height = height1 + height2 - (endy - starty);
  if (width <= 0 || height <= 0) {
    ratio = 0; // 重叠率为 0
  } else {
    int Area = width * height; // 两矩形相交面积
    int Area1 = width1 * height1;
    int Area2 = width2 * height2;
    ratio = Area * 1. / (Area1 + Area2 - Area);
  }
  // return IOU
  return ratio;
}
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////

#define VECN_DEF(name, n, type) struct vec ## n ## name { type v[n]; };
#define VECN_DEF_DEF(n) VECN_DEF(b, n, uchar)VECN_DEF(s, n, short)VECN_DEF(i, n, int)VECN_DEF(f, n, float)
VECN_DEF_DEF(2);
VECN_DEF_DEF(3);
VECN_DEF_DEF(4);
VECN_DEF_DEF(6);
VECN_DEF_DEF(8);
VECN_DEF_DEF(16);
#undef VECN_DEF_DEF
#undef VECN_DEF
/////////////////////////////////////////////////////////

#include <malloc.h>
static size_t my_msize(const void* p)
{
#ifdef _WIN32
  return _msize((void*)p);
#else
  return malloc_usable_size((void*)p);
#endif
}
/////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
// Various wrap modes for brushes
//--------------------------------------------------------------------------
typedef enum {
  WrapModeSame, //普通平铺
  WrapModeTile, //普通平铺
  WrapModeTileFlip, //翻转并平铺
  WrapModeClamp, //不进行平铺，只在以（0，0）位置为原点显示一张图片
  WrapModeReflect = WrapModeTileFlip,
  WrapModeRepeat = WrapModeTile,
  WrapModeDefault = WrapModeSame,
  //WrapModeTileFlipX = WrapModeTileFlip, // 1
  //WrapModeTileFlipY = WrapModeTileFlip<<8, // 2
  //WrapModeTileFlipXY = WrapModeTileFlip|(WrapModeTileFlip<<8), // 3
} WrapMode;

CC_INLINE int WrapPix1D(int x, int w, WrapMode wrap) {
  switch (wrap) {
  case WrapModeSame:
    return BOUND(x, 0, w - 1);
    break;
  case WrapModeTile:
    return x % w;
    break;
  case WrapModeTileFlip:
  {
    int w2 = w * 2;
    x = x % (w2);
    if (x >= w) {
      x = w2 - 1 - x;
    }
    return x;
  }
  case WrapModeClamp:
    return (x >= 0 && x < w) ? x : -1;
    break;
  }
  return -1;
}
//
CC_INLINE int wrap_index(int* p, int n, int size, int idx, int step, int wrap_mode)
{
  int i, size2;
  switch (wrap_mode) {
  case WrapModeClamp:
    for (i = 0; i < n; ++i) {
      p[i] = (idx >= 0 && idx < size) ? idx * step : -1;
      ++idx;
    }
    break;
  case WrapModeTile:
    for (i = 0; i < n; ++i) {
      idx %= size;
      p[i] = idx * step;
      ++idx;
    }
    break;
  case WrapModeReflect:
    size2 = size * 2;
    for (i = 0; i < n; ++i) {
      idx %= size2;
      p[i] = ((idx >= size) ? size2 - idx - 1 : idx) * step;
      ++idx;
    }
    break;
  default:
    ASSERT(0);
    break;
  }
  return 0;
}
////////////////////////////////////////////////////////////////////////////////


#endif // _STDC_TYPES_H_
