#ifndef _GEO_INL_
#define _GEO_INL_
// C���㼸�κ�����
// ���㼸��ѧ�⺯��
#include "cstd.h"
#include "str.h"
#include "cmath.h"
#define CC_INFINITY ((double)(1<<31))
#define EP 1e-10f
#define RADIAN (float)(CC_PI/180.0)
typedef struct affine_info_t {
  int op;
  double x, y;
} affine_info_t;
//�������ε����
/*�﷨��result = area3(float x1, float y1, float x2, float y2, float x3, float y3);
������
x1��3��������3������x����
y1��3��������3������y����
����ֵ�����������
*/
/*
����������-�ؾŹ�ʽ��֪����������a,b,c,��S����� ��[p(p - a)(p - b)(p - c)] �����׹�ʽ��
������p=(a+b+c)/2��

*/
static float area3(float x1, float y1, float x2, float y2, float x3, float y3) {
  float a, b, c, p, s;
  //�����߳���
  a = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
  b = sqrt((x1 - x3)*(x1 - x3) + (y1 - y3)*(y1 - y3));
  c = sqrt((x2 - x3)*(x2 - x3) + (y2 - y3)*(y2 - y3));
  
  p = (a + b + c) * 0.5;
  //�����
  s = sqrt(p*(p - a)*(p - b)*(p - c));
  
  return s;
}

//������㺯��
static double tri_area(float x1, float y1, float x2, float y2, float x3, float y3)
{
   double tarea ;
   tarea=0.5*(x1*y2+x2*y3+x3*y1-x1*y3-x2*y1-x3*y2);//�˴����Խ�������marray[i].mx/myֱ�Ӵ��룬����Ϊ�˼�࣬������
   return fabs(tarea);//ȡ����ֵ
}
CC_INLINE int pt_trans(int n, FPOINT* pt, const double* m3)
{
  double pt2[2];
  int i;
  for (i = 0; i < n; ++i) {
    M3MULV2(pt2, m3, &pt[i].x);
    V2SETV(&pt[i].x, pt2);
  }
  return 0;
}
CC_INLINE int pt_mul(int n, FPOINT* pt, double scaling_x, double scaling_y) {
  int i;
  for (i = 0; i < n; ++i) {
    pt[i].x *= scaling_x;
    pt[i].y *= scaling_y;
  }
  return 0;
}
CC_INLINE int pt_add(int n, FPOINT* pt, double x, double y) {
  int i;
  for (i = 0; i < n; ++i) {
    pt[i].x += x;
    pt[i].y += y;
  }
  return 0;
}
CC_INLINE int pt_scaling(int n, FPOINT* pt, double scaling_x, double scaling_y, double center_x, double center_y) {
  int i;
  for (i = 0; i < n; ++i) {
    pt[i].x = (pt[i].x-center_x)*scaling_x+center_x;
    pt[i].y = (pt[i].y-center_y)*scaling_y+center_y;
  }
  return 0;
}
CC_INLINE int pt_boundrc(int n, const FPOINT* pt, DRECT* boundrc) {
  if (n<=0) {
    return 0;
  } else {
    int i;
    *boundrc = dRECT(pt[0].x, pt[0].y, pt[0].x, pt[0].y);
    for (i=1; i<n; ++i) {
      boundrc->l = MIN(boundrc->l, pt[i].x);
      boundrc->t = MIN(boundrc->t, pt[i].y);
      boundrc->r = MAX(boundrc->r, pt[i].x);
      boundrc->b = MAX(boundrc->b, pt[i].y);
    }
  }
  return n;
}
CC_INLINE int ipt_boundrc(int n, const FPOINT* pt, IRECT* boundrc) {
  if (n<=0) {
    return 0;
  } else {
    int i;
    *boundrc = iRECT((int)pt[0].x, (int)pt[0].y, (int)pt[0].x+1, (int)pt[0].y+1);
    for (i=1; i<n; ++i) {
      boundrc->l = (int)MIN(boundrc->l, pt[i].x);
      boundrc->t = (int)MIN(boundrc->t, pt[i].y);
      boundrc->r = (int)MAX(boundrc->r, pt[i].x+1);
      boundrc->b = (int)MAX(boundrc->b, pt[i].y+1);
    }
  }
  return 0;
}
CC_INLINE int pt_center(int n, FPOINT* pt, double x, double y, double w, double h, double sc_x, double sc_y) {
  DRECT drc = {0};
  double sc;
  pt_boundrc(n, pt, &drc);
  sc = MIN(w/RCW(&drc), h/RCH(&drc));
  pt_add(n, pt, x+w/2 - RCX(&drc)/2, y+h/2 - RCY(&drc)/2);
  pt_scaling(n, pt, sc*sc_x, sc*sc_y, x+w/2, y+h/2);
  return 0;
}
CC_INLINE int affine_info_trans(int nai, affine_info_t* ai, double* m3c)
{
  double m3a[9];
  double m3b[9];
  int i;
  for (i = 0; i < nai; ++i) {
    double x = ai[i].x, y = ai[i].y;
    MEMCPY(m3b, m3c, 9);
    switch (ai[i].op) {
    case GEO_TRANS:
      V9SET(m3a, 1, 0, x, 0, 1, y, 0, 0, 1);
      M3MUL(m3c, m3a, m3b);
      break;
    case GEO_SCALING:
      V9SET(m3a, x, 0, 0, 0, y, 0, 0, 0, 1);
      M3MUL(m3c, m3a, m3b);
      break;
    default:
      V9SET(m3b, 1, 0, 0, 0, 1, 0, 0, 0, 1);
      ASSERT(0);
      break;
    }
  }
  return 0;
}
CC_INLINE int pt_trans_info(int n, FPOINT* pt, int nai, affine_info_t* ai)
{
  double m3c[9];
  M3ID(m3c);
  affine_info_trans(nai, ai, m3c);
  pt_trans(n, pt, m3c);
  return 0;
}
#undef CC_EPS
#define CC_EPS 0.000001
#define FISZERO(f) ((f)<CC_EPS&&(f)>(-CC_EPS))
#define FEQ(a, b) FISZERO((a)-(b))
#define pt_dist(p1, p2) sqrt(V2DIST2(p1, p2))
#define rand01() (1.*(rand() % RAND_MAX)/RAND_MAX)
CC_INLINE int pt_random(int n, int cx, int cy, double* pt)
{
  int i;
  for (i = 0; i < n; i++) {
#if 1
    pt[2 * i + 0] = rand() % cx;// + i * 1. / n;
    pt[2 * i + 1] = rand() % cy;// + i * 1. / n;
#else
    pt[2 * i + 0] = cx * rand01();
    pt[2 * i + 1] = cy * rand01();
#endif
  }
  return 0;
}
CC_INLINE int pt_random2(int n, int cx, int cy, double* pt)
{
  int i;
  for (i = 0; i < n; i++) {
    double ir = (9 * cx + rand() % cx) / 10.;
    pt[2 * i + 0] = (ir) * cos(i * 2 * CC_PI / n) + i * 1. / n;
    pt[2 * i + 1] = (1.*ir * cy / cx) * sin(i * 2 * CC_PI / n) + i * 1. / n;
  }
  return 0;
}
CC_INLINE int pt_random_4(int n, FPOINT* pt, double x, double y, double r1, int ir)
{
  int i;
  for (i = 0; i < n; i++) {
    double r = (rand() % ir) + r1;
    pt[i].x = x + r * cos(i * 2 * CC_PI / n);
    pt[i].y = y + r * sin(i * 2 * CC_PI / n);
  }
  return 0;
}
// ֱ�߽���pt �� ����ts
// ts[0] �ǽ����p1 p2�ı���
// ts[1] �ǽ����p3 p4�ı���
CC_INLINE double line_inter(const double* p1, const double* p2, const double* p3, const double* p4, inter_info_t* ii)
{
  double p21[2], p43[2], p31[2], d, d0, d1 = 0;
  V2SUBV(p21, p2, p1), V2SUBV(p43, p4, p3);
  //p21*ts[0]-p43*ts[1]==p31;
  ii->d = d = DET2(p21[0], p43[0], p21[1], p43[1]);
  if (!FISZERO(d)) {
    V2SUBV(p31, p3, p1);
    d0 = DET2(p31[0], p43[0], p31[1], p43[1]) / d;
    ii->ts[0] = d0;
    ii->ts[1] = DET2(p31[0], p21[0], p31[1], p21[1]) / d;
    ii->pt.x = p1[0] + p21[0] * d0;
    ii->pt.y = p1[1] + p21[1] * d0;
  }
  else {
    // ƽ��
    V2SUBV(p31, p3, p1);
    d0 = DET2(p21[0], p31[0], p21[1], p31[1]);
    if (FISZERO(d0)) {
      // �ĵ㹲��
      d1 = (fabs(p31[0]) + fabs(p31[1])) / (fabs(p21[0]) + fabs(p21[1]));
      if (d1 >= 0 && d1 <= 1) {
        ii->ts[0] = d1;
        ii->ts[1] = 0;
        ii->pt.x = p3[0];
        ii->pt.y = p3[1];
      }
      else {
        d1 = (fabs(p31[0]) + fabs(p31[1])) / (fabs(p43[0]) + fabs(p43[1]));
        ii->ts[0] = d1;
        ii->ts[1] = 0;
        ii->pt.x = p1[0];
        ii->pt.y = p1[1];
      }
    }
  }
  return d;
}
CC_INLINE int my_iline_inter(const int* p1, const int* p2, const int* p3, const int* p4, int* p5)
{
  int p21[2], p43[2], p31[2], d, d0;
  V2SUBV(p21, p2, p1), V2SUBV(p43, p4, p3);
  //p21*ts[0]-p43*ts[1]==p31;
  d = DET2(p21[0], p43[0], p21[1], p43[1]);
  if (0 != d) {
    V2SUBV(p31, p3, p1);
    d0 = DET2(p31[0], p43[0], p31[1], p43[1]);
    p5[0] = p1[0] + p21[0] * d0 / d;
    p5[1] = p1[1] + p21[1] * d0 / d;
  }
  return d;
}
CC_INLINE int iline_inter(const int* p1, const int* p2, const int* p3, const int* p4, iinter_info_t* ii)
{
  int p21[2], p43[2], p31[2], d, d0;
  V2SUBV(p21, p2, p1), V2SUBV(p43, p4, p3);
  //p21*ts[0]-p43*ts[1]==p31;
  ii->d = d = DET2(p21[0], p43[0], p21[1], p43[1]);
  if (0 != d) {
    V2SUBV(p31, p3, p1);
    d0 = DET2(p31[0], p43[0], p31[1], p43[1]);
    //ii->ts[0] = d0;
    //ii->ts[1] = DET2(p31[0], p21[0], p31[1], p21[1]) / d;
    ii->pt.x = p1[0] + p21[0] * d0 / d;
    ii->pt.y = p1[1] + p21[1] * d0 / d;
  }
  else {
    // ƽ��
    V2SUBV(p31, p3, p1);
    d0 = DET2(p21[0], p31[0], p21[1], p31[1]);
    if (0 == d0) {
      // �ĵ㹲��
      if ((abs(p31[0]) + abs(p31[1])) < (abs(p21[0]) + abs(p21[1]))) {
        ii->pt.x = p3[0];
        ii->pt.y = p3[1];
      }
      else {
        ii->pt.x = p1[0];
        ii->pt.y = p1[1];
      }
    }
  }
  return d;
}
#define V2CROSS(p1, p2, op) (((p1)[0] - (op)[0]) * ((p2)[1] - (op)[1]) - ((p2)[0] - (op)[0]) * ((p1)[1] - (op)[1]))
// �߶��ཻ����
// ���أ� 0 ���ཻ 1 �ཻ���>0 -1�ཻ���<0
CC_INLINE int seg_inter_test(const double* p1, const double* p2, const double* p3, const double* p4, inter_info_t* ii)
{
  int R = 1 //
      && (MAX(p1[0], p2[0]) >= MIN(p3[0], p4[0])) // �ų�ʵ��
      && (MAX(p3[0], p4[0]) >= MIN(p1[0], p2[0])) // �ų�ʵ��
      && (MAX(p1[1], p2[1]) >= MIN(p3[1], p4[1])) // �ų�ʵ��
      && (MAX(p3[1], p4[1]) >= MIN(p1[1], p2[1])) // �ų�ʵ��
      && (V2CROSS(p3, p2, p1) * V2CROSS(p4, p2, p1) <= 0) // ����ʵ��: ��34����12������
      && (V2CROSS(p1, p4, p3) * V2CROSS(p2, p4, p3) <= 0) // ����ʵ��: ��12����34������
      ;
  if (R) {
    double d = line_inter(p1, p2, p3, p4, ii);
    if (d < 0) {
      R = -R;
    }
  }
  ii->flag = R;
  return R;
}
CC_INLINE int iseg_inter_test(const int* p1, const int* p2, const int* p3, const int* p4, iinter_info_t* ii)
{
  int R = 1 //
      && (MAX(p1[0], p2[0]) >= MIN(p3[0], p4[0])) // �ų�ʵ��
      && (MAX(p3[0], p4[0]) >= MIN(p1[0], p2[0])) // �ų�ʵ��
      && (MAX(p1[1], p2[1]) >= MIN(p3[1], p4[1])) // �ų�ʵ��
      && (MAX(p3[1], p4[1]) >= MIN(p1[1], p2[1])) // �ų�ʵ��
      && (V2CROSS(p3, p2, p1) * V2CROSS(p4, p2, p1) <= 0) // ����ʵ��: ��34����12������
      && (V2CROSS(p1, p4, p3) * V2CROSS(p2, p4, p3) <= 0) // ����ʵ��: ��12����34������
      ;
  if (R) {
    int d = iline_inter(p1, p2, p3, p4, ii);
    if (d < 0) {
      R = -R;
    }
  }
  ii->flag = R;
  return R;
}
#define FV2EQ(A, B) (FEQ((A)[0], (B)[0]) && FEQ((A)[1], (B)[1]))
#define V2EQ(A, B) (((A)[0]==(B)[0]) && ((A)[1] == (B)[1]))
#define MAKE4BIT(b0,b1,b2,b3) ((b0)<<0 | (b1)<<1 | (b2)<<2 | (b3)<<3)
CC_INLINE int seg_inter_test2(const double* p1, const double* p2, const double* p3, const double* p4, inter_info_t* ii)
{
  int R0 = 0, R1 = 0, R2 = 0, R = 1 //
      && (MAX(p1[0], p2[0]) >= MIN(p3[0], p4[0])) // �ų�ʵ��
      && (MAX(p3[0], p4[0]) >= MIN(p1[0], p2[0])) // �ų�ʵ��
      && (MAX(p1[1], p2[1]) >= MIN(p3[1], p4[1])) // �ų�ʵ��
      && (MAX(p3[1], p4[1]) >= MIN(p1[1], p2[1])); // �ų�ʵ��
  if (R) {
    // ���ڵ���
    R0 = MAKE4BIT(FV2EQ(p1, p3), FV2EQ(p1, p4), FV2EQ(p2, p3), FV2EQ(p2, p4));
    ii->ts[0] = ii->ts[1] = 0;
    if (R0 & 1) {
      V2SETV(&ii->pt.x, p1);
    }
    else if (R0 & 2) {
      V2SETV(&ii->pt.x, p1);
    }
    else if (R0 & 2) {
      V2SETV(&ii->pt.x, p2);
    }
    else if (R0 & 4) {
      V2SETV(&ii->pt.x, p2);
    }
    else if (!R0) {
      // �������ϣ�����ʵ��
      // ���d1 * d2 < 0����12����34�����ߣ�d1==0��ʾp1����34��
      double d1 = V2CROSS(p1, p4, p3), d2 = V2CROSS(p2, p4, p3);
      // ��34����12������
      double d3 = V2CROSS(p3, p2, p1), d4 = V2CROSS(p4, p2, p1);
      R1 = (d3 * d4 <= 0) && (d1 * d2 <= 0);
      R2 = MAKE4BIT(FEQ(d1, 0), FEQ(d2, 0), FEQ(d3, 0), FEQ(d4, 0));
      if (R2 & 1) {
        line_inter(p1, p2, p3, p4, ii);
        V2SETV(&ii->pt.x, p1);
        ii->ts[0] = 0;
      }
      else if (R2 & 2) {
        line_inter(p1, p2, p3, p4, ii);
        V2SETV(&ii->pt.x, p2);
        ii->ts[0] = 1;
      }
      else if (R2 & 4) {
        line_inter(p1, p2, p3, p4, ii);
        V2SETV(&ii->pt.x, p3);
        ii->ts[1] = 0;
      }
      else if (R2 & 8) {
        line_inter(p1, p2, p3, p4, ii);
        V2SETV(&ii->pt.x, p4);
        ii->ts[1] = 1;
      }
      else if (!R2) {
        line_inter(p1, p2, p3, p4, ii);
      }
    }
  }
  return R | (R0 << 1) | (R1 << 5) | (R2 << 6);
}
#define EQ(a, b) ((a)==(b))
CC_INLINE int iseg_inter_test2(const int* p1, const int* p2, const int* p3, const int* p4, int* p5)
{
  int R = 0;
  if (V2EQ(p1, p3)) {
    V2SETV(p5, p1);
    R = 4;
  }
  else {
    R = 1 //
        && (MAX(p1[0], p2[0]) >= MIN(p3[0], p4[0])) // �ų�ʵ��
        && (MAX(p3[0], p4[0]) >= MIN(p1[0], p2[0])) // �ų�ʵ��
        && (MAX(p1[1], p2[1]) >= MIN(p3[1], p4[1])) // �ų�ʵ��
        && (MAX(p3[1], p4[1]) >= MIN(p1[1], p2[1])) // �ų�ʵ��
        //&& (V2CROSS(p3, p2, p1) * V2CROSS(p4, p2, p1) <= 0) // ����ʵ��: ��34����12������
        //&& (V2CROSS(p1, p4, p3) * V2CROSS(p2, p4, p3) <= 0) // ����ʵ��: ��12����34������
        ;
    if (R) {
      // �������ϣ�����ʵ��
      // ���d1 * d2 < 0����12����34�����ߣ�d1==0��ʾp1����34��
      int64 d1 = V2CROSS(p1, p4, p3), d2 = V2CROSS(p2, p4, p3);
      // ��34����12������
      int64 d3 = V2CROSS(p3, p2, p1), d4 = V2CROSS(p4, p2, p1);
      R = 0;
      if ((d1 * d2) <= 0 && (d3 * d4) <= 0) {
        if (0 == d2 || 0 == d4) {
          R = 0;
        }
        else {
          R = 1;
          if (0 == d1 || 0 == d3) {
            if (0 == d1) {
              V2SETV(p5, p1);
              R = 2;
            }
            if (0 == d3) {
              V2SETV(p5, p3);
              R = 3;
            }
          }
          else {
            my_iline_inter(p1, p2, p3, p4, p5);
            R = 1;
            if (V2EQ(p2, p5) || V2EQ(p4, p5)) {
              R = 0;
            }
          }
        }
      }
    }
  }
  return R;
}
//�ų�ʵ��
static BOOL IsRectCross(const IPOINT* p1, const IPOINT* p2, const IPOINT* q1, const IPOINT* q2)
{
  BOOL ret = MIN(p1->x, p2->x) <= MAX(q1->x, q2->x) &&
      MIN(q1->x, q2->x) <= MAX(p1->x, p2->x) &&
      MIN(p1->y, p2->y) <= MAX(q1->y, q2->y) &&
      MIN(q1->y, q2->y) <= MAX(p1->y, p2->y);
  return ret;
}
//�����ж�
static BOOL IsLineSegmentCross(const IPOINT* p1, const IPOINT* p2, const IPOINT* q1, const IPOINT* q2)
{
  long line1, line2;
  line1 = p1->x * (q1->y - p2->y) + p2->x * (p1->y - q1->y) + q1->x * (p2->y - p1->y);
  line2 = p1->x * (q2->y - p2->y) + p2->x * (p1->y - q2->y) + q2->x * (p2->y - p1->y);
  if (((line1 ^ line2) >= 0) && !(line1 == 0 && line2 == 0)) {
    return FALSE;
  }
  line1 = q1->x * (p1->y - q2->y) + q2->x * (q1->y - p1->y) + p1->x * (q2->y - q1->y);
  line2 = q1->x * (p2->y - q2->y) + q2->x * (q1->y - p2->y) + p2->x * (q2->y - q1->y);
  if (((line1 ^ line2) >= 0) && !(line1 == 0 && line2 == 0)) {
    return FALSE;
  }
  return TRUE;
}
static BOOL GetCrossPoint(const IPOINT* p1, const IPOINT* p2, const IPOINT* q1, const IPOINT* q2, IPOINT* out)
{
  if (IsRectCross(p1, p2, q1, q2)) {
    if (IsLineSegmentCross(p1, p2, q1, q2)) {
      //�󽻵�
      int tmpLeft, tmpRight;
      tmpLeft = (q2->x - q1->x) * (p1->y - p2->y) - (p2->x - p1->x) * (q1->y - q2->y);
      tmpRight = (p1->y - q1->y) * (p2->x - p1->x) * (q2->x - q1->x) + q1->x * (q2->y - q1->y) * (p2->x - p1->x) - p1->x * (p2->y - p1->y) * (q2->x - q1->x);
      out->x = (int)((double)tmpRight / (double)tmpLeft);
      tmpLeft = (p1->x - p2->x) * (q2->y - q1->y) - (p2->y - p1->y) * (q1->x - q2->x);
      tmpRight = p2->y * (p1->x - p2->x) * (q2->y - q1->y) + (q2->x - p2->x) * (q2->y - q1->y) * (p1->y - p2->y) - q2->y * (q1->x - q2->x) * (p2->y - p1->y);
      out->y = (int)((double)tmpRight / (double)tmpLeft);
      return TRUE;
    }
  }
  return FALSE;
}
CC_INLINE int pt_slope(const double* pa, const double* pb, const double* pc, double* pd)
{
  double pab[2], pcb[2], dba, dbc;
  V2SUBV(pab, pa, pb), V2SUBV(pcb, pc, pb);
  dba = sqrt(V2NORM(pab)), dbc = sqrt(V2NORM(pcb));
  V2MIX3(pd, pb, 1, pab, dbc, pcb, dba);
  return 0;
}
//���ؽ�bƽ������һ��
CC_INLINE double pt_slope2(const double* p1, const double* p2, const double* p3, const double* p4, double* p5, double* p6)
{
  double p21[2], p43[2], p31[2], d, d0;
  V2SUBV(p21, p2, p1), V2SUBV(p43, p4, p3);
  //p21*ts[0]-p43*ts[1]==p31;
  d = DET2(p21[0], p43[0], p21[1], p43[1]);
  if (!FISZERO(d)) {
    V2SUBV(p31, p3, p1);
    d0 = DET2(p31[0], p43[0], p31[1], p43[1]) / d;
    V2MIX2(p5, p1, 1, p21, d0);
    pt_slope(p2, p5, p4, p6);
  }
  else {
    V2MIX2(p5, p1, 0.5, p3, 0.5);
    V2MIX2(p6, p2, 0.5, p4, 0.5);
  }
  return 0;
}
// c��ֱ�ߴ��� pd, �;��� pdis
CC_INLINE int pt_pedal(const float* pa, const float* pb, const float* pc, float* pd, float* pdis, float* ts)
{
  double pba[2], pca[2], r, pd0[2];
  V2SUBV(pba, pb, pa), V2SUBV(pca, pc, pa);
  r = V2DOT(pba, pca) / V2DOT(pba, pba);
  V2MIX2(pd0, pa, 1, pba, r);
  if (ts) {
    *ts = r;
  }
  if (pd) {
    V2SETV(pd, pd0);
  }
  if (pdis) {
    *pdis = pt_dist(pc, pd0);
  }
  return 0;
}

// ��ά�ռ�㵽ֱ�ߵĴ���  
CC_INLINE FPOINT GetFootOfPerpendicular(FPOINT pt,     // ֱ����һ��  
                             FPOINT begin,  // ֱ�߿�ʼ��  
                             FPOINT end)   // ֱ�߽�����  
{  
  FPOINT retVal;  
  double u;
  double dx = begin.x - end.x;  
  double dy = begin.y - end.y;
  if(fabs(dx) < 0.00000001 && fabs(dy) < 0.00000001 )  
  {  
    retVal = begin;  
    return retVal;  
  }
  u = (pt.x - begin.x)*(begin.x - end.x) + (pt.y - begin.y)*(begin.y - end.y);  
  u = u/((dx*dx)+(dy*dy));  
  
  retVal.x = begin.x + u*dx;  
  retVal.y = begin.y + u*dy;  
  return retVal;  
}
// ��ά�ռ�㵽ֱ�ߵĴ���
CC_INLINE DPOINT3 GetFootOfPerpendicular3(DPOINT3 pt,     // ֱ����һ��  
                             DPOINT3 begin,  // ֱ�߿�ʼ��  
                             DPOINT3 end)   // ֱ�߽�����  
{  
  DPOINT3 retVal;  
  double u;
  double dx = begin.x - end.x;  
  double dy = begin.y - end.y;
  double dz = begin.z - end.z;  
  if(fabs(dx) < 0.00000001 && fabs(dy) < 0.00000001 && fabs(dz) < 0.00000001 )  
  {  
    retVal = begin;  
    return retVal;  
  }  
  
  u = (pt.x - begin.x)*(begin.x - end.x) +  
    (pt.y - begin.y)*(begin.y - end.y) + (pt.z - begin.z)*(begin.z - end.z);  
  u = u/((dx*dx)+(dy*dy)+(dz*dz));  
  
  retVal.x = begin.x + u*dx;  
  retVal.y = begin.y + u*dy;
  retVal.y = begin.z + u*dz;
  return retVal;  
}  
// ��p0������߶��յ�
CC_INLINE int line_nearest(int n, const FPOINT* pt, FPOINT p0, double mind)
{
  int i, j, minj = -1;
  float r, dis = 0, mindis = mind;
  for (i = 0; i < n; ++i) {
    j = (i + 1) % n;
    pt_pedal(&pt[i].x, &pt[j].x, &p0.x, 0, &dis, &r);
    if (r >= 0 && r < 1 && dis < mindis) {
      mindis = dis;
      minj = j;
    }
  }
  return minj;
}
CC_INLINE int dPOLYGON_line_nearest(const FPOLYGON* s, double x, double y, double mind)
{
  int k, i, j = 0, minj = -1;
  float r, dis = 0, mindis = mind;
  FPOINT p0 = fPOINT(x, y);
  const FPOINT* pt = s->pt;
  for (k = 0; k < s->n; ++k) {
    int npt = s->len[k];
    for (i = 0; i < npt; ++i) {
      j = (i + 1) % npt;
      pt_pedal(&pt[i].x, &pt[j].x, &p0.x, 0, &dis, &r);
      if (r >= 0 && r < 1 && dis < mindis) {
        mindis = dis;
        minj = j + (pt - s->pt);
      }
    }
    pt += npt;
  }
  return minj;
}
CC_INLINE IPOINT ipt2_add(IPOINT p1, IPOINT p2)
{
  return iPOINT(p1.x + p2.x, p1.y + p2.y);
}
CC_INLINE IPOINT ipt2_sub(IPOINT p1, IPOINT p2)
{
  return iPOINT(p1.x - p2.x, p1.y - p2.y);
}
#define euclidean_distance_sqr(X1, Y1, X2, Y2) (((X1)-(X2))*((X1)-(X2)) + ((Y1)-(Y2))*((Y1)-(Y2)))
// ����������
CC_INLINE int pt2grid(int n, FPOINT* pt, int g)
{
  int i = 0;
  for (i = 0; i < n; ++i) {
    pt[i].x = g * (int)(0.5 + pt[i].x / g);
    pt[i].y = g * (int)(0.5 + pt[i].y / g);
  }
  return 0;
}
//�任��ʽ:��PΪ�任ǰ�ĵ�,P1Ϊ�任��ĵ�,AΪ�任����,��
// P1 = P * A
//|P1.x| |P.x| |a00,a01,a02|
//|P1.y| = |P.y| * |a10,a11,a12|
//|P1.z| |P.z| |a20,a21,a22|
CC_INLINE int PtTransform(int n, FPOINT* out, const FPOINT* pt, DMATRIX33* m)
{
  int i;
  double x, y;
  for (i = 0; i < n; ++i) {
    x = pt[i].x, y = pt[i].y;
    M2TRANSFORM_3X3_D2(double, (*m), x, y, 1, out[i].x, out[i].y);
  }
  return 0;
}
CC_INLINE int PtRotate(int n, FPOINT* out, const FPOINT* pt, double angle, double cx, double cy, double sx, double sy, double tx, double ty)
{
  int i;
  double x, y, sa = sin(angle), ca = cos(angle);
  UNUSED(sy);
  for (i = 0; i < n; ++i) {
    x = sx * (pt[i].x - cx), y = sx * (pt[i].y - cy);
    out[i].x = (double)(x * ca - y * sa + cx + tx);
    out[i].y = (double)(x * sa + y * ca + cy + ty);
  }
  return 0;
}
CC_INLINE int RandPoints(FPOINT* pt, int len, double xmax, double ymax)
{
  int i;
  for (i = 0; i < len; ++i) {
    pt[i].x = (double)RANDX(xmax);
    pt[i].y = (double)RANDX(ymax);
  }
  return 0;
}
CC_INLINE FPOINT PtCenter(int n, const FPOINT* pt)
{
  FPOINT c = pt[0];
  int i;
  for (i = 1; i < n; ++i) {
    c.x += pt[i].x, c.y += pt[i].y;
  }
  c.x /= n, c.y /= n;
  return c;
}
CC_INLINE int point_in_polygon_point(const FPOINT* m_polygon, int m_num_points, double x, double y, double m_point_radius)
{
  int i;
  double sq = m_point_radius * m_point_radius;
  for (i = 0; i < m_num_points; i++) {
    double sqd = euclidean_distance_sqr(x, y, m_polygon[i].x, m_polygon[i].y);
    if (sqd < sq) {
      return i;
    }
  }
  return -1;
}
CC_INLINE int point_in_polygon_edge(const FPOINT* m_polygon, int m_num_points, double x, double y, double m_point_radius)
{
  int i;
  for (i = 0; i < m_num_points; i++) {
    unsigned n1 = i;
    unsigned n2 = NEXT_INDEX(i, m_num_points);
    double x1 = m_polygon[n1].x;
    double y1 = m_polygon[n1].y;
    double x2 = m_polygon[n2].x;
    double y2 = m_polygon[n2].y;
    double dx = x2 - x1;
    double dy = y2 - y1;
    if ((dx * dx + dy * dy) > 0.0000001) {
      double x3 = x;
      double y3 = y;
      double x4 = x3 - dy;
      double y4 = y3 + dx;
      double den = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
      double u1 = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / den;
      double xi = x1 + u1 * (x2 - x1);
      double yi = y1 + u1 * (y2 - y1);
      dx = xi - x;
      dy = yi - y;
      if (u1 > 0.0 && u1 < 1.0 && sqrt(dx * dx + dy * dy) <= m_point_radius) {
        return i;
      }
    }
  }
  return -1;
}

//======= Crossings Multiply algorithm of InsideTest ========================
//
// By Eric Haines, 3D/Eye Inc, erich@eye.com
//
// This version is usually somewhat faster than the original published in
// Graphics Gems IV; by turning the division for testing the X axis crossing
// into a tricky multiplication test this part of the test became faster,
// which had the additional effect of making the test for "both to left or
// both to right" a bit slower for triangles than simply computing the
// intersection each time. The main increase is in triangle testing speed,
// which was about 15% faster; all other polygon complexities were pretty much
// the same as before. On machines where division is very expensive (not the
// case on the HP 9000 series on which I tested) this test should be much
// faster overall than the old code. Your mileage may (in fact, will) vary,
// depending on the machine and the test data, but in general I believe this
// code is both shorter and faster. This test was inspired by unpublished
// Graphics Gems submitted by Joseph Samosky and Mark Haigh-Hutchinson.
// Related work by Samosky is in:
//
// Samosky, Joseph, "SectionView: A system for interactively specifying and
// visualizing sections through three-dimensional medical image data",
// M.S. Thesis, Department of Electrical Engineering and Computer Science,
// Massachusetts Institute of Technology, 1993.
//
// Shoot a test ray along +X axis. The strategy is to compare vertex Y values
// to the testing point's Y and quickly discard edges which are entirely to one
// side of the test ray. Note that CONVEX and WINDING code can be added as
// for the CrossingsTest() code; it is left out here for clarity.
//
// Input 2D polygon _pgon_ with _numverts_ number of vertices and test point
// _point_, returns 1 if inside, 0 if outside.
CC_INLINE BOOL point_in_polygon(const FPOINT* m_polygon, int m_num_points, double tx, double ty)
{
  int j, k;
  int yflag0, yflag1, inside_flag;
  double vtx0, vty0, vtx1, vty1;
  if (m_num_points < 3) {
    return FALSE;
  }
  vtx0 = m_polygon[m_num_points - 1].x;
  vty0 = m_polygon[m_num_points - 1].y;
  // get test bit for above/below X axis
  yflag0 = (vty0 >= ty);
  vtx1 = m_polygon[0].x;
  vty1 = m_polygon[0].y;
  inside_flag = 0;
  for (j = 1; j <= m_num_points; ++j) {
    yflag1 = (vty1 >= ty);
    if (yflag0 != yflag1) {
      if (((vty1 - ty) * (vtx0 - vtx1) >= (vtx1 - tx) * (vty0 - vty1)) == yflag1) {
        inside_flag ^= 1;
      }
    }
    // Move to the next pair of vertices, retaining info as possible.
    yflag0 = yflag1;
    vtx0 = vtx1;
    vty0 = vty1;
    k = (j >= m_num_points) ? j - m_num_points : j;
    vtx1 = m_polygon[k].x;
    vty1 = m_polygon[k].y;
  }
  return inside_flag != 0;
}

#define POHIT_POINT (1<<28)
#define POHIT_EDGE (1<<29)
#define POHIT_FACE (1<<30)
#define POHIT_MARK (POHIT_POINT-1)
CC_INLINE int point_in_polygon_hittest(const FPOINT* m_polygon, int m_num_points, double x, double y, double m_point_radius, int hitmark)
{
  int i;
  if ((hitmark & POHIT_POINT) && (i = point_in_polygon_point(m_polygon, m_num_points, x, y, m_point_radius)) >= 0) {
    return i | POHIT_POINT;
  }
  if ((hitmark & POHIT_EDGE) && (i = point_in_polygon_edge(m_polygon, m_num_points, x, y, m_point_radius)) >= 0) {
    return i | POHIT_EDGE;
  }
  if ((hitmark & POHIT_FACE) && point_in_polygon(m_polygon, m_num_points, x, y)) {
    return POHIT_FACE;
  }
  return 0;
}
CC_INLINE int point_in_polygon_hittest2(const FPOINT* m_polygon, const int* lens, int n, double x, double y, double m_point_radius, int hitmark) {
  int i, ret = 0;
  for (i=0; i<n; ++i) {
  ret = point_in_polygon_hittest(m_polygon, lens[i], x, y, m_point_radius, hitmark);
    if (ret) {
      return ret;
    }
    m_polygon += lens[i];
  }
  return 0;
}
// �߶���struct LINETREE{};
// �������Ĵ���
CC_INLINE int dblcmp(double d)
{
  if (FABS(d) < CC_EPS) {
    return 0;
  }
  return (d > 0) ? 1 : - 1;
}
// <һ>��Ļ�������
// ��������֮��ŷ�Ͼ���
CC_INLINE double dist(FPOINT p1, FPOINT p2)
{
  return (sqrt(((p1).x - (p2).x) * ((p1).x - (p2).x) + ((p1).y - (p2).y) * ((p1).y - (p2).y)));
}
// �ж��������Ƿ��غ�
CC_INLINE BOOL equal_fpoint(FPOINT p1, FPOINT p2)
{
  return ((ABS(p1.x - p2.x) < CC_EPS) && (ABS(p1.y - p2.y) < CC_EPS));
}
/*(sp-op)*(ep-op)�Ĳ��
r=multiply(sp, ep, op), �õ�(sp-op)*(ep-op)�Ĳ��
r>0: sp��ʸ��op ep��˳ʱ�뷽��;
r=0��op sp ep���㹲��;
r<0: sp��ʸ��op ep����ʱ�뷽�� */
CC_INLINE double multiply(FPOINT sp, FPOINT ep, FPOINT op)
{
  return ((sp.x - op.x) * (ep.y - op.y) - (ep.x - op.x) * (sp.y - op.y));
}
CC_INLINE double amultiply(FPOINT sp, FPOINT ep, FPOINT op)
{
  double x = (sp.x - op.x) * (ep.y - op.y) - (ep.x - op.x) * (sp.y - op.y);
  return FABS(x);
}
/*ʸ��(p1-op)��(p2-op)�ĵ��
r=dotmultiply(p1, p2, op), �õ�ʸ��(p1-op)��(p2-op)�ĵ���������ʸ��������ʸ��
r < 0: ��ʸ���н�Ϊ���;
r = 0����ʸ���н�Ϊֱ��;
r > 0: ��ʸ���н�Ϊ�۽� */
CC_INLINE double dotmultiply(FPOINT p1, FPOINT p2, FPOINT p0)
{
  return ((p1.x - p0.x) * (p2.x - p0.x) + (p1.y - p0.y) * (p2.y - p0.y));
}
/* �жϵ�p�Ƿ����߶�l��
������(p���߶�l���ڵ�ֱ����)&& (��p�����߶�lΪ�Խ��ߵľ�����) */
CC_INLINE BOOL online(FLINESEG2 l, FPOINT p)
{
  return ((multiply(l.b, p, l.a) == 0) && (((p.x - l.a.x) * (p.x - l.b.x) <= 0) && ((p.y - l.a.y) * (p.y - l.b.y) <= 0)));
}
// ���ص�p�Ե�oΪԲ����ʱ����תalpha(��λ������)�����ڵ�λ��
CC_INLINE FPOINT rotate(FPOINT o, double alpha, FPOINT p)
{
  FPOINT tp;
  p.x -= o.x;
  p.y -= o.y;
  tp.x = (double)(p.x * cos(alpha) - p.y * sin(alpha) + o.x);
  tp.y = (double)(p.y * cos(alpha) + p.x * sin(alpha) + o.y);
  return tp;
}
/* ���ض�����o��, ��ʼ��Ϊos, ��ֹ��Ϊoe�ļн�(��λ������)
�Ƕ�С��pi, ������ֵ
�Ƕȴ���pi, ���ظ�ֵ
�����������߶�֮��ļн� */
CC_INLINE double angle(FPOINT o, FPOINT s, FPOINT e)
{
  double cosfi, fi, norm;
  double dsx = s.x - o.x;
  double dsy = s.y - o.y;
  double dex = e.x - o.x;
  double dey = e.y - o.y;
  cosfi = dsx * dex + dsy * dey;
  norm = (dsx * dsx + dey * dey) * (dex * dex + dey * dey);
  cosfi /= SQRT(norm);
  if (cosfi >= 1.f) {
    return 0;
  }
  if (cosfi <= - 1.f) {
    return - (double)CC_PI;
  }
  fi = ACOS(cosfi);
  if (dsx * dey - dsy * dex > 0) {
    return fi;
  }
  // ˵��ʸ��os ��ʸ�� oe��˳ʱ�뷽��
  return - fi;
}
// <��>�߶μ�ֱ�ߵĻ�������
// �жϵ�C���߶�AB���ڵ�ֱ��l�ϴ���P�����߶�AB�Ĺ�ϵ
// �������Ǹ�������Ĺ�ʽд��, P�ǵ�C���߶�AB����ֱ�ߵĴ���
// AC dot AB
// r = -----------
// ||AB||^2
// (Cx-Ax)(Bx-Ax) + (Cy-Ay)(By-Ay)
// = ---------------------------------
// L^2
// r has the following meaning:
// r=0 P = A
// r=1 P = B
// r<0 P is on the backward extension of AB
// r>1 P is on the forward extension of AB
// 0<r<1 P is interior to AB
CC_INLINE double relation(FPOINT c, FLINESEG2 l)
{
  FLINESEG2 tl;
  tl.a = l.a;
  tl.b = c;
  return dotmultiply(tl.b, l.b, l.a) / (dist(l.a, l.b) * dist(l.a, l.b));
}
// ���C���߶�AB����ֱ�ߵĴ��� P
CC_INLINE FPOINT perpendicular(FPOINT p, FLINESEG2 l)
{
  FPOINT tp;
  double r = relation(p, l);
  tp.x = (double)(l.a.x + r * (l.b.x - l.a.x));
  tp.y = (double)(l.a.y + r * (l.b.y - l.a.y));
  return tp;
}
/* ���p���߶�l����̾���
�����߶��Ͼ�õ�����ĵ�np
ע�⣺np���߶�l�ϵ���p����ĵ�, ��һ���Ǵ��� */
CC_INLINE double ptoTLineSegdist(FPOINT p, FLINESEG2 l, FPOINT* np)
{
  FPOINT n;
  double r = relation(p, l);
  if (r < 0) {
    n = l.a;
  }
  else if (r > 1) {
    n = l.b;
    return dist(p, l.b);
  }
  else {
    n = perpendicular(p, l);
  }
  if (np) {
    *np = n;
  }
  return dist(p, n);
}
// ���p���߶�l����ֱ�ߵľ���
// ��ע�Ȿ�������ϸ�����������
CC_INLINE double ptoldist(FPOINT p, FLINESEG2 l)
{
  double m = multiply(p, l.b, l.a);
  return ABS(m) / dist(l.a, l.b);
}
/* ����㵽���߼����������, �����������.
ע�⣺���õ���ptoTLineSeg()���� */
CC_INLINE double ptopointset(int vcount, FPOINT* pointset, FPOINT p, FPOINT* q)
{
  int i;
  double cd = CC_INFINITY, td;
  FLINESEG2 l;
  FPOINT tq = {0}, cq = {0};
  for (i = 0; i < vcount - 1; i++) {
    l.a = pointset[i];
    l.b = pointset[i + 1];
    td = ptoTLineSegdist(p, l, &tq);
    if (td < cd) {
      cd = td;
      cq = tq;
    }
  }
  if (q) {
    *q = cq;
  }
  return cd;
}
/* �ж�Բ�Ƿ��ڶ������*/
CC_INLINE BOOL circle_inside_polygon(int vcount, FPOINT* polygon, FPOINT center, double radius)
{
  FPOINT q;
  double d;
  q.x = 0;
  q.y = 0;
  d = ptopointset(vcount, polygon, center, &q);
  return (d < radius || FABS(d - radius) < CC_EPS);
}
/* ��������ʸ��l1��l2�ļнǵ����� (-1 ~ 1)
ע�⣺������������нǵĻ�, ע�ⷴ���Һ�����ֵ���Ǵ� 0��pi */
CC_INLINE double coSINe(FLINESEG2 l1, FLINESEG2 l2)
{
  double aa = (l1.b.x - l1.a.x) * (l2.b.x - l2.a.x) + (l1.b.y - l1.a.y) * (l2.b.y - l2.a.y);
  double dd = (dist(l1.b, l1.a) * dist(l2.b, l2.a));
  return (aa / dd);
}
// �����߶�l1��l2֮��ļн�
// ��λ������ ��Χ(-pi, pi)
CC_INLINE double lsangle(FLINESEG2 l1, FLINESEG2 l2)
{
  FPOINT o, s, e;
  o.x = o.y = 0;
  s.x = l1.b.x - l1.a.x;
  s.y = l1.b.y - l1.a.y;
  e.x = l2.b.x - l2.a.x;
  e.y = l2.b.y - l2.a.y;
  return angle(o, s, e);
}
// �ж��߶�u��v�ཻ(�����ཻ�ڶ˵㴦)
CC_INLINE BOOL intersect(FLINESEG2 u, FLINESEG2 v)
{
  return ((MAX(u.a.x, u.b.x) >= MIN(v.a.x, v.b.x)) && // �ų�ʵ��
      (MAX(v.a.x, v.b.x) >= MIN(u.a.x, u.b.x)) && (MAX(u.a.y, u.b.y) >= MIN(v.a.y, v.b.y)) && (MAX(v.a.y, v.b.y) >= MIN(u.a.y, u.b.y)) && (multiply(v.a, u.b, u.a) * multiply(u.b, v.b, u.a) >= 0) && // ����ʵ��
      (multiply(u.a, v.b, v.a) * multiply(v.b, u.b, v.a) >= 0));
}
//�ж��������Ƿ����
#define Euqal_Point(p1, p2) ((fabs(p1.x - p2.x) < EP) && (fabs(p1.y - p2.y) < EP))
//���ҽ���u,v�ཻ���ҽ��㲻��u,v�Ķ˵�ʱ����Ϊtrue;
CC_INLINE int intersect_Av2(FLINESEG2 u, FLINESEG2 v)
{
  return ((intersect(u, v)) && (!Euqal_Point(u.a, v.a)) && (!Euqal_Point(u.a, v.b)) && (!Euqal_Point(u.b, v.a)) && (!Euqal_Point(u.b, v.b)));
}
// �ж��߶�u��v�ཻ��������˫���Ķ˵㣩
CC_INLINE BOOL intersect_A(FLINESEG2 u, FLINESEG2 v)
{
  return ((intersect(u, v)) && (!online(u, v.a)) && (!online(u, v.b)) && (!online(v, u.b)) && (!online(v, u.a)));
}
// �ж��߶�v����ֱ�����߶�u�ཻ
// �������ж��߶�u�Ƿ�����߶�v
CC_INLINE BOOL intersect_l(FLINESEG2 u, FLINESEG2 v)
{
  return multiply(u.a, v.b, v.a) * multiply(v.b, u.b, v.a) >= 0;
}
// ������֪��������, ����������ֱ�߽������̣� a*x+b*y+c = 0 (a >= 0)
CC_INLINE DFLINE2 makeline(FPOINT p1, FPOINT p2)
{
  DFLINE2 tl;
  int sign = 1;
  tl.a = p2.y - p1.y;
  if (tl.a < 0) {
    sign = - 1;
    tl.a = sign * tl.a;
  }
  tl.b = sign * (p1.x - p2.x);
  tl.c = sign * (p1.y * p2.x - p1.x * p2.y);
  return tl;
}
// ����ֱ�߽������̷���ֱ�ߵ�б��k, ˮƽ�߷��� 0, ��ֱ�߷��� 1e200
CC_INLINE double slope(DFLINE2 l)
{
  if (ABS(l.a) < 1e-20) {
    return 0;
  }
  if (ABS(l.b) < 1e-20) {
    return CC_INFINITY;
  }
  return - (l.a / l.b);
}
// ����ֱ�ߵ���б��alpha ( 0 - pi)
// ע�⣺atan()���ص��� -CC_PI/2 ~ CC_PI/2
CC_INLINE double line_alpha(DFLINE2 l)
{
  double k;
  if (ABS(l.a) < CC_EPS) {
    return 0;
  }
  if (ABS(l.b) < CC_EPS) {
    return (double)CC_PI / 2.f;
  }
  k = slope(l);
  if (k > 0) {
    return ATAN(k);
  }
  else {
    return (double)CC_PI + ATAN(k);
  }
}
// ���p����ֱ��l�ĶԳƵ�
CC_INLINE FPOINT symmetry(DFLINE2 l, FPOINT p)
{
  FPOINT tp;
  tp.x = (double)(((l.b * l.b - l.a * l.a) * p.x - 2 * l.a * l.b * p.y - 2 * l.a * l.c) / (l.a * l.a + l.b * l.b));
  tp.y = (double)(((l.a * l.a - l.b * l.b) * p.y - 2 * l.a * l.b * p.x - 2 * l.b * l.c) / (l.a * l.a + l.b * l.b));
  return tp;
}
// �������ֱ�� l1(a1*x+b1*y+c1 = 0), l2(a2*x+b2*y+c2 = 0)�ཻ, ����TRUE, �ҷ��ؽ���p
CC_INLINE BOOL lineintersect(DFLINE2 l1, DFLINE2 l2, FPOINT* p)
{
  // �� L1, L2
  double d = l1.a * l2.b - l2.a * l1.b;
  if (ABS(d) < CC_EPS)
    // ���ཻ
  {
    return FALSE;
  }
  (*p).x = (double)((l2.c * l1.b - l1.c * l2.b) / d);
  (*p).y = (double)((l2.a * l1.c - l1.a * l2.c) / d);
  return TRUE;
}
// ����߶�l1��l2�ཻ, ����TRUE�ҽ�����(inter)����, ���򷵻�FALSE
CC_INLINE BOOL intersection(FLINESEG2 l1, FLINESEG2 l2, FPOINT* inter)
{
  DFLINE2 ll1, ll2;
  ll1 = makeline(l1.a, l1.b);
  ll2 = makeline(l2.a, l2.b);
  if (lineintersect(ll1, ll2, inter)) {
    return online(l1, *inter);
  }
  else {
    return FALSE;
  }
}
// ��ֱ�߽���
// ����0 ��ֱ��ƽ��
CC_INLINE double lineintersect2(FPOINT p1, FPOINT p2, FPOINT p3, FPOINT p4, FPOINT* pt)
{
  double x21 = p2.x - p1.x, y21 = p2.y - p1.y;
  double x43 = p4.x - p3.x, y43 = p4.y - p3.y;
  double t3 = x43 * y21 - x21 * y43;
  if (t3) {
    double t1 = x43 * p3.y - y43 * p3.x;
    double t2 = y21 * p1.x - x21 * p1.y;
    pt->x = (x21 * t1 + x43 * t2) / t3;
    pt->y = (y21 * t1 + y43 * t2) / t3;
  }
  return t3;
}
// <��> ����γ����㷨ģ��
// ������ر�˵��, �������ζ���Ҫ����ʱ������
// ����Ķ�����Ǽ򵥶���Σ�����TRUE
// Ҫ ��: ���붥�����а���ʱ������
// ˵ ��: �򵥶���ζ���:
// 1: ѭ�������������߶ζԵĽ�������֮�乲�еĵ�����
// 2: �����ڵ��߶β��ཻ
// ������Ĭ�ϵ�һ�������Ѿ�����
CC_INLINE BOOL issimple(int vcount, const FPOINT* polygon)
{
  int i, cn;
  FLINESEG2 l1, l2;
  for (i = 0; i < vcount; i++) {
    l1.a = polygon[i];
    l1.b = polygon[(i + 1) % vcount];
    cn = vcount - 3;
    while (cn) {
      l2.a = polygon[(i + 2) % vcount];
      l2.b = polygon[(i + 3) % vcount];
      if (intersect(l1, l2)) {
        break;
      }
      cn--;
    }
    if (cn) {
      return FALSE;
    }
  }
  return TRUE;
}
// �����polygon��͹�����ʱ������TRUE
// ������˳�򷵻ض���ζ����͹�����жϣ�bc[i]=1,iff:��i��������͹����
CC_INLINE BOOL isconvex(int vcount, const FPOINT* polygon, BOOL* bc)
{
  int i, count, index = 0, ret = 1;
  FPOINT tp = polygon[0];
  // Ѱ�ҵ�һ��͹����
  for (i = 1; i < vcount; ++i) {
    if (polygon[i].y < tp.y || (polygon[i].y == tp.y && polygon[i].x < tp.x)) {
      tp = polygon[i];
      index = i;
    }
  }
  count = vcount - 1;
  if (bc) {
    bc[index] = 1;
  }
  // �ж�͹����
  while (count) {
    if (multiply(polygon[(index + 1) % vcount], polygon[(index + 2) % vcount], polygon[index]) >= 0) {
      if (bc) {
        bc[(index + 1) % vcount] = 1;
      }
      else {
        if (bc) {
          bc[(index + 1) % vcount] = 0;
        }
        ret = 0;
      }
    }
    index++;
    count--;
  }
  return ret;
}
// �������ε����������
// Ҫ������ʱ�뷽���������εĶ���
// ������͹����λ򰼶����
CC_INLINE double poly_area(int n, FPOINT* pt)
{
  int i;
  double s;
  if (n < 3) {
    return 0;
  }
  s = pt[0].y * (pt[n - 1].x - pt[1].x);
  for (i = 1; i < n; i++) {
    s += pt[i].y * (pt[(i - 1)].x - pt[(i + 1) % n].x);
  }
  return s;
}
// ���ض�������(signed);
// ���붥�㰴��ʱ������ʱ, ������ֵ; ���򷵻ظ�ֵ
#define AREA_OF_POLYGON( vcount, polygon ) (0.5*poly_area(vcount, polygon))
// ������붥�㰴��ʱ������, ����TRUE
#define is_conter_clock( vcount, polygon ) (poly_area( vcount, polygon ) > 0)
// ��һ���ж϶���ζ������з���ķ���
CC_INLINE BOOL isccwize(int vcount, FPOINT* polygon)
{
  int i, index;
  FPOINT a, b, v;
  v = polygon[0];
  index = 0;
  for (i = 1; i < vcount; ++i) {
    // �ҵ���������󶥵㣬�϶���͹����
    if (polygon[i].y < v.y || (polygon[i].y == v.y && polygon[i].x < v.x)) {
      index = i;
    }
  }
  // ����v��ǰһ����
  a = polygon[(index - 1 + vcount) % vcount];
  // ����v�ĺ�һ����
  b = polygon[(index + 1) % vcount];
  return (multiply(v, b, a) > 0);
}
// ���߷��жϵ�q������polygon��λ�ù�ϵ
// Ҫ��polygonΪ�򵥶���Σ�������ʱ������
// ������ڶ������: ����0
// ������ڶ���α���: ����1
// ������ڶ������: ����2
CC_INLINE int inside_polygon(int vcount, const FPOINT* polygon, FPOINT q)
{
  int c = 0, i;
  FLINESEG2 l1, l2;
  l1.a = q;
  l1.b = q;
  l1.b.x = CC_INFINITY;
  for (i = 0; i < vcount; i++) {
    l2.a = polygon[i];
    l2.b = polygon[(i + 1) % vcount];
    if (online(l2, q)) {
      return 1;
    }
    if (intersect_A(l1, l2)) {
      c++;
    }
    // �ཻ�Ҳ��ڶ˵�
    if (online(l1, l2.b) && !online(l1, l2.a) && l2.b.y > l2.b.y) {
      c++;
    }
    // l2��һ���˵���l1���Ҹö˵������˵���������ϴ���Ǹ�
    if (!online(l1, l2.b) && online(l1, l2.a) && l2.b.y < l2.b.y) {
      c++;
    }
    // ����ƽ�б�
  }
  return (c % 2 == 1) ? 0 : 2;
}
CC_INLINE int inside_polygon2(int vcount, FPOINT* polygon, FPOINT q)
{
  int c = 0, i, n;
  FLINESEG2 l1, l2;
  double r1, r2;
  l1.a = q;
  l1.b = q;
  l1.b.x = CC_INFINITY;
  n = vcount;
  for (i = 0; i < vcount; ++i) {
    l2.a = polygon[i];
    l2.b = polygon[(i + 1) % n];
    if (online(l2, q)) {
      // ������ڱ��ϣ�����1
      return 1;
    }
    if ((intersect_A(l1, l2)) || // �ཻ�Ҳ��ڶ˵�
        (((online(l1, polygon[(i + 1) % n])) && // �ڶ����˵���������
            ((!(online(l1, polygon[(i + 2) % n]))) &&
                // ǰһ���˵�ͺ�һ���˵�����������
                (((r1 = multiply(polygon[i], polygon[(i + 1) % n], l1.a) * multiply(polygon[(i + 1) % n], polygon[(i + 2) % n], l1.a)) > 0) || 
				((online(l1, polygon[(i + 2) % n])) &&
                // ��һ������ˮƽ�ߣ�ǰһ���˵�ͺ�һ���˵�����������
                ((r2 = multiply(polygon[i], polygon[(i + 2) % n], l1.a) * multiply(polygon[(i + 2) % n], polygon[(i + 3) % n], l1.a)) > 0))))))) {
      c++;
    }
  }
  if (c % 2 == 1) {
    return 0;
  }
  else {
    return (2);
  }
}
// �жϵ�q��͹�����polygon��
// ��q��͹�����polygon��[��������]ʱ, ����TRUE
// ע�⣺�����polygonһ��Ҫ��͹�����
CC_INLINE BOOL inside_convex_polygon(int vcount, const FPOINT* polygon, FPOINT q)
{
  FPOINT p;
  FLINESEG2 l;
  int i;
  p.x = 0;
  p.y = 0;
  // Ѱ��һ���϶��ڶ����polygon�ڵĵ�p������ζ���ƽ��ֵ
  for (i = 0; i < vcount; i++) {
    p.x += polygon[i].x;
    p.y += polygon[i].y;
  }
  p.x /= vcount;
  p.y /= vcount;
  for (i = 0; i < vcount; i++) {
    l.a = polygon[i];
    l.b = polygon[(i + 1) % vcount];
    if (multiply(p, l.b, l.a) *multiply(q, l.b, l.a) < 0) {
      /* ��p�͵�q�ڱ�l������, ˵����q�϶��ڶ������ */
      return FALSE;
    }
  }
  return TRUE;
}

// Ѱ��͹���� graham ɨ�跨
// pointsetΪ����ĵ㼯;
// chΪ�����͹���ϵĵ㼯, ������ʱ�뷽������;
// setnumΪpointset�еĵ����Ŀ
// lenΪ�����͹���ϵĵ�ĸ���
CC_INLINE int graham_scan(int setnum, FPOINT* pointset, FPOINT* ch)
{
  int i, j, k = 0, top = 2, len;
  FPOINT tmp;
  // ѡȡpointset��y������С�ĵ�pointset[k], ��������ĵ��ж��, ��ȡ����ߵ�һ��
  for (i = 1; i < setnum; ++i) {
    if (pointset[i].y < pointset[k].y || ((pointset[i].y == pointset[k].y) && (pointset[i].x < pointset[k].x))) {
      k = i;
    }
  }
  tmp = pointset[0];
  pointset[0] = pointset[k];
  pointset[k] = tmp; // ����pointset��y������С�ĵ���pointset[0]
  // �Զ��㰴�����pointset[0]�ļ��Ǵ�С�����������,
  // ������ͬ�İ��վ���pointset[0]�ӽ���Զ��������
  for (i = 1; i < setnum - 1; ++i) {
    for (k = i, j = i + 1; j < setnum; ++j) {
      if (multiply(pointset[j], pointset[k], pointset[0]) > 0 || // ���Ǹ�С
          ((multiply(pointset[j], pointset[k], pointset[0]) == 0) && //�������, �������
          dist(pointset[0], pointset[j]) < dist(pointset[0], pointset[k]))) {
        k = j;
      }
    }
    tmp = pointset[i];
    pointset[i] = pointset[k];
    pointset[k] = tmp;
  }
  ch[0] = pointset[0];
  ch[1] = pointset[1];
  ch[2] = pointset[2];
  for (i = 3; i < setnum; i++) {
    while (multiply(pointset[i], ch[top], ch[top - 1]) >= 0) {
      top--;
    }
    ch[++top] = pointset[i];
  }
  len = top + 1;
  return len;
}
// ���������㼯͹��, ����˵��ͬ graham �㷨
CC_INLINE int convex_closure(int setnum, const FPOINT* pointset, FPOINT* ch)
{
  int top = 0, i, index, first, len;
  double curmax, curcos, curdis;
  FPOINT tmp;
  FLINESEG2 l1, l2;
  BOOL* use = (BOOL*)pmalloc(setnum * sizeof(BOOL));
  if (setnum <= 0) {
    return 0;
  }
  tmp = pointset[0];
  index = 0;
  // ѡȡy��С��, �������һ��, ��ѡȡ�����
  for (i = 0; i < setnum; ++i) {
    use[i] = FALSE;
    if ((pointset[i].y < tmp.y) || (pointset[i].y == tmp.y && pointset[i].x < tmp.x)) {
      index = i;
    }
    use[i] = FALSE;
  }
  tmp = pointset[index];
  first = index;
  use[index] = TRUE;
  index = - 1;
  ch[top++] = tmp;
  tmp.x -= 100;
  l1.a = tmp;
  l1.b = ch[0];
  l2.a = ch[0];
  while (index != first) {
    curmax = - 100;
    curdis = 0;
    index = - 1;
    // ѡȡ�����һ��ȷ���߼н���С�ĵ�, ������ֵ�����
    for (i = 0; i < setnum; ++i) {
      if (use[i]) {
        continue;
      }
      l2.b = pointset[i];
      curcos = coSINe(l1, l2); // ����cosֵ��н�����, ��Χ�� ��-1 -- 1 ��
      if (curcos > curmax || (FABS(curcos - curmax) < CC_EPS && dist(l2.a, l2.b) > curdis)) {
        curmax = curcos;
        index = i;
        curdis = dist(l2.a, l2.b);
      }
    }
    if (index < 0) {
      break;
    }
    use[first] = FALSE; // ��յ�first�������־, ʹ������γɷ�յ�hull
    use[index] = TRUE;
    ch[top++] = pointset[index];
    l1.a = ch[top - 2];
    l1.b = ch[top - 1];
    l2.a = ch[top - 1];
  }
  len = top;
  pfree(use);
  return len;
}
// ������򵥶����polygon������
// ��Ҫ�������漸������:
// void AddPosPart(); �����ұ���������
// void AddNegPart(); ���������������
// void AddRegion(); �����������
// ��ʹ�øó���ʱ�������xtr,ytr,wtr,xtl,ytl,wtl���ȫ�ֱ����Ϳ���ʹ
// ��Щ��������ʽ�õ�����,��Ҫע�⺯���������͵���Ҫ����Ӧ�仯
CC_INLINE void AddPosPart(double x, double y, double w, double* xtr, double* ytr, double* pwtr)
{
  if (fabs(*pwtr + w) < 1e-10) {
    return ;
  } // detect zero regions
  *xtr = (*pwtr** xtr + w * x) / (*pwtr + w);
  *ytr = (*pwtr** ytr + w * y) / (*pwtr + w);
  *pwtr = w + *pwtr;
  return ;
}
CC_INLINE void AddNegPart(double x, double y, double w, double* xtl, double* ytl, double* wtl)
{
  if (fabs(*wtl + w) < 1e-10) {
    return ;
  }
  // detect zero regions
  *xtl = (*wtl** xtl + w * x) / (*wtl + w);
  *ytl = (*wtl** ytl + w * y) / (*wtl + w);
  *wtl = w + *wtl;
  return ;
}
CC_INLINE void AddRegion(double x1, double y1, double x2, double y2, double* xtr, double* ytr, double* wtr, double* xtl, double* ytl, double* wtl)
{
  if (fabs(x1 - x2) < 1e-10) {
    return ;
  }
  if (x2 > x1) {
    // rectangle ȫ�ֱ����仯��
    AddPosPart((x2 + x1) / 2, y1 / 2, (x2 - x1) *y1, xtr, ytr, wtr);
    // triangle ȫ�ֱ����仯��
    AddPosPart((x1 + x2 + x2) / 3, (y1 + y1 + y2) / 3, (x2 - x1) * (y2 - y1) / 2, xtr, ytr, wtr);
  }
  else {
    // rectangle ȫ�ֱ����仯��
    AddNegPart((x2 + x1) / 2, y1 / 2, (x2 - x1) *y1, xtl, ytl, wtl);
    // triangle ȫ�ֱ����仯��
    AddNegPart((x1 + x2 + x2) / 3, (y1 + y1 + y2) / 3, (x2 - x1) * (y2 - y1) / 2, xtl, ytl, wtl);
  }
}
CC_INLINE FPOINT cg_simple(int vcount, FPOINT* polygon)
{
  int i;
  //ע��: �����xtr,ytr,wtr,xtl,ytl,wtl�ĳ�ȫ�ֱ���������Ҫɾȥ
  double xtr, ytr, wtr, xtl, ytl, wtl;
  FPOINT p1, p2, tp;
  xtr = ytr = wtr = 0.0;
  xtl = ytl = wtl = 0.0;
  for (i = 0; i < vcount; ++i) {
    p1 = polygon[i];
    p2 = polygon[(i + 1) % vcount];
    // ȫ�ֱ����仯��
    AddRegion(p1.x, p1.y, p2.x, p2.y, &xtr, &ytr, &wtr, &xtl, &ytl, &wtl);
  }
  tp.x = (double)((wtr * xtr + wtl * xtl) / (wtr + wtl));
  tp.y = (double)((wtr * ytr + wtl * ytl) / (wtr + wtl));
  return tp;
}
// ��͹����ε�����, Ҫ���������ΰ���ʱ������
CC_INLINE FPOINT gravity_center(int vcount, const FPOINT* polygon)
{
  FPOINT tp;
  double x, y, s, x0, y0, cs, k;
  int i;
  x = y = s = 0.f;
  for (i = 1; i < vcount - 1; i++) {
    x0 = (polygon[0].x + polygon[i].x + polygon[i + 1].x) / 3;
    y0 = (polygon[0].y + polygon[i].y + polygon[i + 1].y) / 3; // ��ǰ�����ε�����
    cs = multiply(polygon[i], polygon[i + 1], polygon[0]) / 2;
    // �������������ֱ�����øù�ʽ���
    if (ABS(s) < 1e-20) {
      x = x0;
      y = y0;
      s += cs;
      continue;
    }
    k = cs / s; // ���������
    x = (x + k * x0) / (1 + k);
    y = (y + k * y0) / (1 + k);
    s += cs;
  }
  tp.x = (double)(x);
  tp.y = (double)(y);
  return tp;
}
// �������߷����ж�һ�����Ƿ��ڶ���ε��ڲ�:
// ���߷����������Ҫ�жϵĵ���һ����(Ϊ�˷��㣬ֱ����һˮƽ���ҵ�����)����һ���߶���
// ����αߵĽ�����������ʱ�����ڶ�����ڣ�ż��ʱ�����ڶ�����⡣
// �ж��߶��Ƿ��ڼ򵥶������(ע��: ����������͹����Σ�������㷨���Ի���)
// ԭ��:
// ��Ҫ����һ: �߶ε������˵㶼�ڶ�����ڣ�
// ��Ҫ������: �߶κͶ���ε����б߶����ڽ���
// ��;: 1. �ж������Ƿ��ڼ򵥶������
// 2. �жϼ򵥶�����Ƿ�����һ���򵥶������
CC_INLINE BOOL TLineSeg_inside_polygon(int vcount, FPOINT* polygon, FLINESEG2 l)
{
  int top = 0, i, j;
  FPOINT* PointSet, tmp;
  FLINESEG2 s;
  // �ж��߶�l�Ķ˵��Ƿ񲻶��ڶ������
  if (!inside_polygon(vcount, polygon, l.a) || !inside_polygon(vcount, polygon, l.b)) {
    return FALSE;
  }
  PointSet = MALLOC(FPOINT, vcount);
  for (i = 0; i < vcount; ++i) {
    s.a = polygon[i];
    s.b = polygon[(i + 1) % vcount];
    if (online(s, l.a)) {
      // �߶�l����ʼ�˵����߶�s��
      PointSet[top++] = l.a;
    }
    else if (online(s, l.b)) {
      // �߶�l����ֹ�˵����߶�s��
      PointSet[top++] = l.b;
    }
    else {
      if (online(l, s.a)) {
        // �߶�s����ʼ�˵����߶�l��
        PointSet[top++] = s.a;
      }
      else if (online(l, s.b)) {
        // �߶�s����ֹ�˵����߶�l��
        PointSet[top++] = s.b;
      }
      else {
        if (intersect(l, s)) {
          // ���ʱ������ཻ���϶����ڽ�������FALSE
          pfree(PointSet);
          return FALSE;
        }
      }
    }
  }
  // ð������x����С������ǰ�棻x������ͬ�ߣ�
  // y����С������ǰ��
  for (i = 0; i < top - 1; ++i) {
    for (j = i + 1; j < top; ++j) {
      if (PointSet[i].x > PointSet[j].x || (fabs(PointSet[i].x - PointSet[j].x) < CC_EPS && PointSet[i].y > PointSet[j].y)) {
        tmp = PointSet[i];
        PointSet[i] = PointSet[j];
        PointSet[j] = tmp;
      }
    }
  }
  for (i = 0; i < top - 1; ++i) {
    // �õ��������ڽ�����е�
    tmp.x = (PointSet[i].x + PointSet[i + 1].x) / 2;
    tmp.y = (PointSet[i].y + PointSet[i + 1].y) / 2;
    if (!inside_polygon(vcount, polygon, tmp)) {
      FREE(PointSet);
      return FALSE;
    }
  }
  FREE(PointSet);
  return TRUE;
}
// ����һ�򵥶���Σ��ҳ�һ���϶��ڸö�����ڵĵ�
// ����1: ÿ�������������һ��͹����
// ����2: ������>=4�ļ򵥶����������һ���Խ���
// ����: x���������С�ĵ�϶���͹����
// y���������С�ĵ�϶���͹����
CC_INLINE FPOINT a_point_insidepoly(int vcount, FPOINT* polygon)
{
  FPOINT v = {0}, a = {0}, b = {0}, r = {0};
  FPOINT tri[3], q = {0};
  int i, index;
  double md;
  BOOL bin;
  v = polygon[0];
  index = 0;
  for (i = 1; i < vcount; ++i) {
    //Ѱ��һ��͹����
    if (polygon[i].y < v.y) {
      v = polygon[i];
      index = i;
    }
  }
  //�õ�v��ǰһ������
  a = polygon[(index - 1 + vcount) % vcount];
  //�õ�v�ĺ�һ������
  b = polygon[(index + 1) % vcount];
  tri[0] = a;
  tri[1] = v;
  tri[2] = b;
  md = CC_INFINITY;
  //in1 = index;
  bin = FALSE;
  //Ѱ����������avb�����붥��v����Ķ���q
  for (i = 0; i < vcount; ++i) {
    if (i == index) {
      continue;
    }
    if (i == (index - 1 + vcount) % vcount) {
      continue;
    }
    if (i == (index + 1) % vcount) {
      continue;
    }
    if (!inside_convex_polygon(3, tri, polygon[i])) {
      continue;
    }
    bin = TRUE;
    if (dist(v, polygon[i]) < md) {
      q = polygon[i];
      md = dist(v, q);
    }
  }
  //û�ж�����������avb�ڣ������߶�ab�е�
  if (!bin) {
    r.x = (a.x + b.x) / 2;
    r.y = (a.y + b.y) / 2;
    return r;
  }
  //�����߶�vq���е�
  r.x = (v.x + q.x) / 2;
  r.y = (v.y + q.y) / 2;
  return r;
}
// ��Ӷ������һ��p������һ���򵥶���ε�����,������ڷ����е�,����rp�������е�,lp�����е�
// ע��: p��һ��Ҫ�ڶ������
// ���붥����������ʱ������
// ԭ ��:������ڶ�����ڿ϶�������; ͹�������Ψһ�������е�,������ξͿ����ж����������е�;
// ���polygon��͹����Σ��е�ֻ������ֻҪ�ҵ��Ϳ���,���Ի�����㷨
// ����ǰ�����λ���һ���㷨�������:���󰼶���ε�͹��,Ȼ����͹��������
CC_INLINE void pointtangentpoly(int vcount, FPOINT* polygon, FPOINT p, FPOINT* rp, FPOINT* lp)
{
  FLINESEG2 ep, en;
  BOOL blp, bln;
  int i;
  *rp = polygon[0];
  *lp = polygon[0];
  for (i = 1; i < vcount; ++i) {
    ep.a = polygon[(i + vcount - 1) % vcount];
    ep.b = polygon[i];
    en.a = polygon[i];
    en.b = polygon[(i + 1) % vcount];
    blp = multiply(ep.b, p, ep.a) >= 0; // p is to the left of pre edge
    bln = multiply(en.b, p, en.a) >= 0; // p is to the left of next edge
    if (!blp && bln) {
      if (multiply(polygon[i], *rp, p) > 0) {
        // polygon[i] is above rp
        *rp = polygon[i];
      }
    }
    if (blp && !bln) {
      if (multiply(*lp, polygon[i], p) > 0) {
        // polygon[i] is below lp
        *lp = polygon[i];
      }
    }
  }
  return ;
}
// ��������polygon�ĺ˴��ڣ�����TRUE�����غ��ϵ�һ��p.���㰴��ʱ�뷽������
CC_INLINE BOOL core_exist(int vcount, FPOINT* polygon, FPOINT* p)
{
  int i, j, k;
  FLINESEG2 l;
  DFLINE2* lineset;
  lineset = MALLOC(DFLINE2, vcount);
  for (i = 0; i < vcount; ++i) {
    lineset[i] = makeline(polygon[i], polygon[(i + 1) % vcount]);
  }
  for (i = 0; i < vcount; ++i) {
    for (j = 0; j < vcount; ++j) {
      if (i == j) {
        continue;
      }
      if (lineintersect(lineset[i], lineset[j], p)) {
        for (k = 0; k < vcount; ++k) {
          l.a = polygon[k];
          l.b = polygon[(k + 1) % vcount];
          //����ζ��㰴��ʱ�뷽�����У��˿϶���ÿ���ߵ��������
          if (multiply(*p, l.b, l.a) > 0) {
            break;
          }
        }
        //�ҵ���һ�����ϵĵ�
        if (k == vcount) {
          break;
        }
      }
    }
    if (j < vcount) {
      break;
    }
  }
  FREE(lineset);
  return (i < vcount);
}
// ��ν͹����ε�ֱ��, ��͹���������������������롣������㷨
// ����ʱO(n), ��һ��������㷨�� ���������һ��͹�����, �Ҷ���
// ���밴˳��˳ʱ�롢��ʱ����ɣ��������롣�����벻��͹�����
// ����һ��㼯, ��Ҫ������͹���� ��������������Ŷ�, Ȼ�����ÿ
// ���ŶԵľ���, ȡ����ߡ�����Ҫ����5����
CC_INLINE double diameter(int n, const FPOINT* ch)
{
  int znum = 0, i, j, k = 1;
  int* zd = MALLOC(int, n * 2);
  double tmp, dia;
  while (amultiply(ch[0], ch[k + 1], ch[n - 1]) > amultiply(ch[0], ch[k], ch[n - 1]) - CC_EPS) {
    k++;
  }
  i = 0;
  j = k;
  while (i <= k && j < n) {
    zd[znum * 2] = i;
    zd[znum * 2 + 1] = j;
    ++znum;
    while (amultiply(ch[i + 1], ch[j + 1], ch[i]) > amultiply(ch[i + 1], ch[j], ch[i]) - CC_EPS && j < n - 1) {
      zd[znum * 2] = i;
      zd[znum * 2 + 1] = j;
      ++znum;
      j++;
    }
    i++;
  }
  dia = - 1.0;
  for (i = 0; i < znum; i++) {
    printf("%d %d\n", zd[i * 2], zd[i * 2 + 1]);
    tmp = dist(ch[zd[i * 2]], ch[zd[i * 2 + 1]]);
    if (dia < tmp) {
      dia = tmp;
    }
  }
  FREE(zd);
  return dia;
}
CC_INLINE int sklansky(FPOINT** array, int start, int end, int* stack, int nsign, int sign2)
{
  int incr = end > start ? 1 : - 1;
  /* prepare first triangle */
  int pprev = start, pcur = pprev + incr, pnext = pcur + incr;
  int stacksize = 3;
  if (start == end || (array[start]->x == array[end]->x && array[start]->y == array[end]->y)) {
    stack[0] = start;
    return 1;
  }
  stack[0] = pprev;
  stack[1] = pcur;
  stack[2] = pnext;
  end += incr; /* make end = afterend */
  while (pnext != end) {
    /* check the angle p1,p2,p3 */
    double cury = array[pcur]->y;
    double nexty = array[pnext]->y;
    double by = nexty - cury;
    if (CC_SIGN(by) != nsign) {
      double ax = array[pcur]->x - array[pprev]->x;
      double bx = array[pnext]->x - array[pcur]->x;
      double ay = cury - array[pprev]->y;
      double convexity = ay * bx - ax * by; /* if >0 then convex angle */
      if (CC_SIGN(convexity) == sign2 && (ax != 0 || ay != 0)) {
        pprev = pcur;
        pcur = pnext;
        pnext += incr;
        stack[stacksize] = pnext;
        stacksize++;
      }
      else {
        if (pprev == start) {
          pcur = pnext;
          stack[1] = pcur;
          pnext += incr;
          stack[2] = pnext;
        }
        else {
          stack[stacksize - 2] = pnext;
          pcur = pprev;
          pprev = stack[stacksize - 4];
          stacksize--;
        }
      }
    }
    else {
      pnext += incr;
      stack[stacksize - 1] = pnext;
    }
  }
  return --stacksize;
}
CC_INLINE int convex_hull2(int total, const FPOINT* array, FPOINT* hull, int orientation)
{
  FPOINT** pointer = 0;
  int* stack = 0;
  int* t_stack;
  int t_count, i, stop_idx;
  int hull_len = 0, miny_ind = 0, maxy_ind = 0;
  if (total == 0) {
    //if( array )
    // CC_ERROR( CC_StsBadSize,
    // "Point sequence can not be empty if the output is matrix" );
    return 0;
  }
  pointer = (FPOINT**)pmalloc(total * sizeof(pointer[0]));
  stack = (int*)pmalloc((total + 2) * sizeof(stack[0]));
  for (i = 0; i < total; i++) {
    pointer[i] = (FPOINT*)array + i;
  }
#define CMP_PTS( pt1, pt2 ) \
  ((pt1)->x < (pt2)->x || ((pt1)->x == (pt2)->x && (pt1)->y < (pt2)->y))
  // sort the point set by x-coordinate, find MIN and MAX y
  QSORT(pointer, pointer + total, CMP_PTS, FPOINT*);
#undef CMP_PTS
  //icvSortPointsByPointers_32s( pointer, total, 0 );
  for (i = 1; i < total; i++) {
    double y = pointer[i]->y;
    if (pointer[miny_ind]->y > y) {
      miny_ind = i;
    }
    if (pointer[maxy_ind]->y < y) {
      maxy_ind = i;
    }
  }
  if (pointer[0]->x == pointer[total - 1]->x && pointer[0]->y == pointer[total - 1]->y) {
    hull[0] = pointer[0][0];
    pfree(pointer);
    pfree(stack);
    return 1;
  }
  /*upper half */
  {
    int* tl_stack = stack;
    int tl_count = sklansky(pointer, 0, maxy_ind, tl_stack, - 1, 1);
    int* tr_stack = tl_stack + tl_count;
    int tr_count = sklansky(pointer, total - 1, maxy_ind, tr_stack, - 1, - 1);
    /* gather upper part of convex hull to output */
    if (orientation == CC_COUNTER_CLOCKWISE) {
      CC_SWAP(tl_stack, tr_stack, t_stack);
      CC_SWAP(tl_count, tr_count, t_count);
    }
    hull_len = 0;
    for (i = 0; i < tl_count - 1; i++) {
      hull[hull_len++] = pointer[tl_stack[i]][0];
    }
    for (i = tr_count - 1; i > 0; i--) {
      hull[hull_len++] = pointer[tr_stack[i]][0];
    }
    stop_idx = tr_count > 2 ? tr_stack[1] : tl_count > 2 ? tl_stack[tl_count - 2] : - 1;
  }
  /* lower half */
  {
    int* bl_stack = stack;
    int bl_count = sklansky(pointer, 0, miny_ind, bl_stack, 1, - 1);
    int* br_stack = stack + bl_count;
    int br_count = sklansky(pointer, total - 1, miny_ind, br_stack, 1, 1);
    if (orientation != CC_COUNTER_CLOCKWISE) {
      CC_SWAP(bl_stack, br_stack, t_stack);
      CC_SWAP(bl_count, br_count, t_count);
    }
    if (stop_idx >= 0) {
      int check_idx = bl_count > 2 ? bl_stack[1] : bl_count + br_count > 2 ? br_stack[2 - bl_count] : - 1;
      if (check_idx == stop_idx || (check_idx >= 0 && pointer[check_idx]->x == pointer[stop_idx]->x && pointer[check_idx]->y == pointer[stop_idx]->y)) {
        /* if all the points lie on the same line, then the bottom part of
        the convex hull is the mirrored top part (except the exteme points).*/
        bl_count = MIN(bl_count, 2);
        br_count = MIN(br_count, 2);
      }
    }
    for (i = 0; i < bl_count - 1; i++) {
      hull[hull_len++] = pointer[bl_stack[i]][0];
    }
    for (i = br_count - 1; i > 0; i--) {
      hull[hull_len++] = pointer[br_stack[i]][0];
    }
  }
  pfree(pointer);
  pfree(stack);
  // cvBoundingRect
  return hull_len;
}
// Բ�Ļ�������
// ��p��Բ��(�����߽�)ʱ������TRUE
// ��;: ��ΪԲΪ͹���������жϵ㼯�����ߣ�������Ƿ���Բ��ʱ��
// ֻ��Ҫ��һ�жϵ��Ƿ���Բ�ڼ��ɡ�
CC_INLINE BOOL point_in_circle1(FPOINT o, double r, FPOINT p)
{
  double d2 = (p.x - o.x) * (p.x - o.x) + (p.y - o.y) * (p.y - o.y);
  double r2 = r * r;
  return d2 < r2 || fabs(d2 - r2) < CC_EPS;
}
// �� ;: �󲻹��ߵ�����ȷ��һ��Բ
// �� ��: ������p1,p2,p3
// ������㹲�ߣ�����FALSE����֮������TRUE��Բ����q���أ��뾶��r����
CC_INLINE BOOL cocircle(FPOINT p1, FPOINT p2, FPOINT p3, FPOINT* q, double* r)
{
  double x12 = p2.x - p1.x, y12 = p2.y - p1.y;
  double x13 = p3.x - p1.x, y13 = p3.y - p1.y;
  double z2 = x12 * (p1.x + p2.x) + y12 * (p1.y + p2.y);
  double z3 = x13 * (p1.x + p3.x) + y13 * (p1.y + p3.y);
  double d = 2.f * (x12 * (p3.y - p2.y) - y12 * (p3.x - p2.x));
  //���ߣ�Բ������
  if (fabs(d) < CC_EPS) {
    return FALSE;
  }
  q->x = (double)((y13 * z2 - y12 * z3) / d);
  q->y = (double)((x12 * z3 - x13 * z2) / d);
  *r = dist(p1, *q);
  return TRUE;
}
CC_INLINE int line_circle(DFLINE2 l, FPOINT o, double r, FPOINT* p1, FPOINT* p2)
{
  return TRUE;
}
// ���εĻ�������
// ˵��: ��Ϊ���ε������ԣ������㷨���Ի���:
// 1.�жϾ����Ƿ������
// ֻҪ�жϸõ�ĺ�������������Ƿ���ھ��ε����ұߺ����±�֮�䡣
// 2.�ж��߶Ρ����ߡ�������Ƿ��ھ�����
// ��Ϊ�����Ǹ�͹��������ֻҪ�ж����ж˵��Ƿ��ھ����оͿ����ˡ�
// 3.�ж�Բ�Ƿ��ھ�����
// Բ�ھ����еĳ�Ҫ������: Բ���ھ�������Բ�İ뾶С�ڵ���Բ�ĵ������ıߵľ������Сֵ��
// ��֪���ε���������(a,b,c)��������ĸ�����d������. ע��: ��֪��������������������
CC_INLINE FPOINT rect4th(FPOINT a, FPOINT b, FPOINT c)
{
  FPOINT d;
  // ˵��c����ֱ�ǹսǴ�
  if (fabs(dotmultiply(a, b, c)) < CC_EPS) {
    d.x = a.x + b.x - c.x;
    d.y = a.y + b.y - c.y;
  }
  // ˵��b����ֱ�ǹսǴ�
  if (fabs(dotmultiply(a, c, b)) < CC_EPS) {
    d.x = a.x + c.x - b.x;
    d.y = a.y + c.y - b.x;
  }
  // ˵��a����ֱ�ǹսǴ�
  if (fabs(dotmultiply(c, b, a)) < CC_EPS) {
    d.x = c.x + b.x - a.x;
    d.y = c.y + b.y - a.y;
  }
  return d;
}
// �����㷨������
//
// ��δʵ�ֵ��㷨:
// 1. ������㼯����СԲ
// 2. �����εĽ�
// 3. �򵥶���ε������ʷ�
// 4. Ѱ�Ұ����㼯����С����
// 5. ���ߵĻ���
// 6. �жϾ����Ƿ��ھ�����
// 7. �жϾ����ܷ���ھ�����
// 8. ���β���������ܳ�
// 9. ���β�������
// 10.���β��ıհ�
// 11.���εĽ�
// 12.�㼯�е�������
// 13.����εĲ�
// 14.Բ�Ľ��벢
// 15.ֱ����Բ�Ĺ�ϵ
// 16.�߶���Բ�Ĺ�ϵ
// 17.�����εĺ˼��������
// 18.��㼯�в��ཻ��� railwai
// Ѱ�Ұ����㼯����С����
// ԭ��: �þ�������һ������㼯��͹�ǵ�ĳ���߹���
// First take the convex hull of the points. Let the resulting convex
// polygon be P. It has been known for some time that the minimum
// area rectangle encloSINg P must have one rectangle side flush with
// (i.b., collinear with and overlapping) one edge of P. This geometric
// fact was used by Godfried Toussaint to develop the "rotating calipers"
// algorithm in a hard-to-find 1983 paper, "Solving Geometric Problems
// with the Rotating Calipers" (Proc. IEEE MELECON). The algorithm
// rotates a surrounding rectangle from one flush edge to the next,
// keeping track of the minimum area for each edge. It achieves O(n)
// time (after hull computation). See the "Rotating Calipers Homepage"
// http://www.cs.mcgill.ca/~orm/rotcal.frame.html for a description
// and applet.
// ���ߵĻ��� α������:
// Input: tol = the approximation tolerance
// L = {V0,V1,...,Vn-1} is any n-vertex polyline
//
// Set start = 0;
// Set k = 0;
// Set W0 = V0;
// for each vertex Vi (i=1,n-1)
// {
// if Vi is within tol from Vstart
// then ignore it, and continue with the next vertex
//
// Vi is further than tol away from Vstart
// so add it as a new vertex of the reduced polyline
// Increment k++;
// Set Wk = Vi;
// Set start = i; as the new initial vertex
// }
//
// Output: W = {W0,W1,...,Wk-1} = the k-vertex simplified polyline
//
// ����
// ��Բ��ϵ:
// ����: return 1��
// ����: return 2��
// ����: return 3��
// �ں�: return 4��
// �ཻ: return 5��
CC_INLINE int CircleRelation(FPOINT p1, double r1, FPOINT p2, double r2)
{
  double d = SQRT((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
  // ���뱣֤ǰ����if�ȱ��ж���
  if (fabs(d - r1 - r2) < CC_EPS) {
    return 1;
  }
  if (fabs(d - fabs(r1 - r2)) < CC_EPS) {
    return 2;
  }
  if (d > r1 + r2) {
    return 3;
  }
  if (d < fabs(r1 - r2)) {
    return 4;
  }
  if (fabs(r1 - r2) < d && d < r1 + r2) {
    return 5;
  }
  return 0; // indicate an error!
}
// �ж�Բ�Ƿ��ھ�����:
// �ж�Բ�Ƿ��ھ����ڣ��Ǿͷ���TRUE�������ˮƽ�������ĸ����������Ͽ�ʼ��˳ʱ�����У�
// ����ptoldist�������ڵ�4ҳ
CC_INLINE BOOL CircleRecRelation(FPOINT pc, double r, FPOINT pr1, FPOINT pr2, FPOINT pr3, FPOINT pr4)
{
  if (pr1.x < pc.x && pc.x < pr2.x && pr3.y < pc.y && pc.y < pr2.y) {
    FLINESEG2 line1, line2, line3, line4;
    line1.a = pr1, line1.b = pr2;
    line2.a = pr2, line2.b = pr3;
    line3.a = pr3, line3.b = pr4;
    line4.a = pr4, line4.b = pr1;
    if (r < ptoldist(pc, line1) && r < ptoldist(pc, line2) && r < ptoldist(pc, line3) && r < ptoldist(pc, line4)) {
      return TRUE;
    }
  }
  return FALSE;
}
// �㵽ƽ��ľ���:
// ƽ����һ��ʽ��ʾax+by+cz+d=0
CC_INLINE double P2planeDist(double x, double y, double z, double a, double b, double c, double d)
{
  return (double)fabs(a * x + b * y + c * z + d) / SQRT(a * a + b * b + c * c);
}
//���Ƿ���ֱ��ͬ��:
// �������Ƿ���ֱ��ͬ�࣬���򷵻�TRUE
CC_INLINE BOOL SameSide(FPOINT p1, FPOINT p2, DFLINE2 line)
{
  return (line.a * p1.x + line.b * p1.y + line.c) * (line.a * p2.x + line.b * p2.y + line.c) > 0;
}
// ���淴����:
// ��֪�����ߡ����棬�����ߡ�
// a1,b1,c1Ϊ����ֱ�߷���(a1 x + b1 y + c1 = 0 ,��ͬ)ϵ��;
// a2,b2,c2Ϊ�����ֱ�߷���ϵ��;
// a,b,cΪ�����ֱ�߷���ϵ��.
// �����з���ģ�ʹ��ʱע��: ���������:<-b2,a2>�����������:<b,-a>.
// ��Ҫ���ǽ���п��ܻ���"negative zeros"
CC_INLINE void reflect(double a1, double b1, double c1, double a2, double b2, double c2, double* a, double* b, double* c)
{
  double n, m, tpb, tpa, xx, yy; //(xx,yy)���������뾵��Ľ��㡣
  tpb = b1 * b2 + a1 * a2;
  tpa = a2 * b1 - a1 * b2;
  m = (tpb * b1 + tpa * a1) / (b1 * b1 + a1 * a1);
  n = (tpa * b1 - tpb * a1) / (b1 * b1 + a1 * a1);
  if (fabs(a1 * b2 - a2 * b1) < 1e-20) {
    *a = a2;
    *b = b2;
    *c = c2;
    return ;
  }
  xx = (b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1);
  yy = (a2 * c1 - a1 * c2) / (a1 * b2 - a2 * b1);
  *a = n;
  *b = - m;
  *c = m * yy - xx * n;
}
// ���ΰ���
CC_INLINE BOOL rect2_in_rect1(double A, double B, double C, double D)
{
  // ����2��C��D���Ƿ���1��A��B����
  double X, Y, L, K, DMax, tmp;
  if (A < B) {
    tmp = A;
    A = B;
    B = tmp;
  }
  if (C < D) {
    tmp = C;
    C = D;
    D = tmp;
  }
  if (A > C && B > D) {
    // trivial case
    return TRUE;
  }
  else {
    if (D >= B) {
      return FALSE;
    }
    else {
      X = SQRT(A * A + B * B); // outer rectangle's diagonal
      Y = SQRT(C * C + D * D); // inner rectangle's diagonal
      if (Y < B) {
        // check for marginal conditions
        return TRUE;
      }
      else {
        // the inner rectangle can freely rotate inside
        if (Y > X) {
          return FALSE;
        }
        else {
          L = (B - SQRT(Y * Y - A * A)) / 2;
          K = (A - SQRT(Y * Y - B * B)) / 2;
          DMax = SQRT(L * L + K * K);
          return (D < DMax);
        }
      }
    }
  }
}
// ��Բ����:
// ��Բ�Ѿ��ཻ�����У�
CC_INLINE void circle2point(FPOINT p1, double r1, FPOINT p2, double r2, FPOINT* rp1, FPOINT* rp2)
{
  double a, b, r, delta;
  a = p2.x - p1.x;
  b = p2.y - p1.y;
  r = (a * a + b * b + r1 * r1 - r2 * r2) / 2;
  if (a == 0 && b != 0) {
    rp1->y = rp2->y = (double)(r / b);
    rp1->x = (double)(sqrt(r1 * r1 - rp1->y * rp1->y));
    rp2->x = - rp1->x;
  }
  else if (a != 0 && b == 0) {
    rp1->x = rp2->x = (double)(r / a);
    rp1->y = (double)(sqrt(r1 * r1 - rp1->x * rp2->x));
    rp2->y = - rp1->y;
  }
  else if (a != 0 && b != 0) {
    delta = b * b * r * r - (a * a + b * b) * (r * r - r1 * r1 * a * a);
    rp1->y = (double)((b * r + sqrt(delta)) / (a * a + b * b));
    rp2->y = (double)((b * r - sqrt(delta)) / (a * a + b * b));
    rp1->x = (double)((r - b * rp1->y) / a);
    rp2->x = (double)((r - b * rp2->y) / a);
  }
  rp1->x += p1.x;
  rp1->y += p1.y;
  rp2->x += p1.x;
  rp2->y += p1.y;
}
// ��Բ�������:
// ���뱣֤�ཻ
CC_INLINE double circle2area(FPOINT p1, double r1, FPOINT p2, double r2)
{
  double sita1, sita2, dx1, dy1, dx2, dy2, a, b, rr, s, tmp;
  FPOINT rp1, rp2, t;
  circle2point(p1, r1, p2, r2, &rp1, &rp2);
  if (r1 > r2) {
    // ��֤r2>r1
    CC_SWAP(p1, p2, t);
    CC_SWAP(r1, r2, tmp);
  }
  a = p1.x - p2.x;
  b = p1.y - p2.y;
  rr = SQRT(a * a + b * b);
  dx1 = rp1.x - p1.x;
  dy1 = rp1.y - p1.y;
  dx2 = rp2.x - p1.x;
  dy2 = rp2.y - p1.y;
  sita1 = ACOS((dx1 * dx2 + dy1 * dy2) / r1 / r1);
  dx1 = rp1.x - p2.x;
  dy1 = rp1.y - p2.y;
  dx2 = rp2.x - p2.x;
  dy2 = rp2.y - p2.y;
  sita2 = ACOS((dx1 * dx2 + dy1 * dy2) / r2 / r2);
  s = 0;
  if (rr < r2) {
    //�ཻ��Ϊ�Ż�
    s = r1 * r1 * ((double)CC_PI - sita1 / 2.f + SIN(sita1) / 2.f) + r2 * r2 * (sita2 - SIN(sita2)) / 2.f;
  }
  else {
    //�ཻ��Ϊ�ӻ�
    s = (r1 * r1 * (sita1 - SIN(sita1)) + r2 * r2 * (sita2 - SIN(sita2))) / 2;
  }
  return s;
}
// Բ��ֱ�߹�ϵ:
// 0----���� 1----���� 2----�ཻ
CC_INLINE int circle_line_point(FPOINT p, double r, double a, double b, double c, FPOINT* rp1, FPOINT* rp2)
{
  int res = 0;
  double tmp;
  c = c + a * p.x + b * p.y;
  if (a == 0 && b != 0) {
    tmp = - c / b;
    if (r * r < tmp * tmp) {
      res = 0;
    }
    else if (r * r == tmp * tmp) {
      res = 1;
      rp1->y = (double)(tmp);
      rp1->x = 0;
    }
    else {
      res = 2;
      rp1->y = rp2->y = (double)(tmp);
      rp1->x = (double)(sqrt(r * r - tmp * tmp));
      rp2->x = - rp1->x;
    }
  }
  else if (a != 0 && b == 0) {
    tmp = - c / a;
    if (r * r < tmp * tmp) {
      res = 0;
    }
    else if (r * r == tmp * tmp) {
      res = 1;
      rp1->x = (double)(tmp);
      rp1->y = 0;
    }
    else {
      res = 2;
      rp1->x = rp2->x = (double)(tmp);
      rp1->y = (double)(sqrt(r * r - tmp * tmp));
      rp2->y = - rp1->y;
    }
  }
  else if (a != 0 && b != 0) {
    double delta;
    delta = b * b * c * c - (a * a + b * b) * (c * c - a * a * r * r);
    if (delta < 0) {
      res = 0;
    }
    else if (delta == 0) {
      res = 1;
      rp1->y = (double)(- b * c / (a * a + b * b));
      rp1->x = (double)((- c - b * rp1->y) / a);
    }
    else {
      res = 2;
      rp1->y = (double)((- b * c + SQRT(delta)) / (a * a + b * b));
      rp2->y = (double)((- b * c - SQRT(delta)) / (a * a + b * b));
      rp1->x = (double)((- c - b * rp1->y) / a);
      rp2->x = (double)((- c - b * rp2->y) / a);
    }
  }
  rp1->x += p.x;
  rp1->y += p.y;
  rp2->x += p.x;
  rp2->y += p.y;
  return res;
}
// ����Բ:
CC_INLINE void incircle(FPOINT p1, FPOINT p2, FPOINT p3, FPOINT* rp, double* r)
{
  double dx31, dy31, dx21, dy21, d31, d21, a1, b1, c1;
  double dx32, dy32, dx12, dy12, d32, d12, a2, b2, c2;
  dx31 = p3.x - p1.x;
  dy31 = p3.y - p1.y;
  dx21 = p2.x - p1.x;
  dy21 = p2.y - p1.y;
  d31 = SQRT(dx31 * dx31 + dy31 * dy31);
  d21 = SQRT(dx21 * dx21 + dy21 * dy21);
  a1 = dx31 * d21 - dx21 * d31;
  b1 = dy31 * d21 - dy21 * d31;
  c1 = a1 * p1.x + b1 * p1.y;
  dx32 = p3.x - p2.x;
  dy32 = p3.y - p2.y;
  dx12 = - dx21;
  dy12 = - dy21;
  d32 = SQRT(dx32 * dx32 + dy32 * dy32);
  d12 = d21;
  a2 = dx12 * d32 - dx32 * d12;
  b2 = dy12 * d32 - dy32 * d12;
  c2 = a2 * p2.x + b2 * p2.y;
  rp->x = (double)((c1 * b2 - c2 * b1) / (a1 * b2 - a2 * b1));
  rp->y = (double)((c2 * a1 - c1 * a2) / (a1 * b2 - a2 * b1));
  *r = FABS(dy21 * rp->x - dx21 * rp->y + dx21 * p1.y - dy21 * p1.x) / d21;
}
// ���е�:
// p---Բ�����꣬ r---Բ�뾶�� sp---Բ��һ�㣬 rp1,rp2---�е�����
CC_INLINE void cut_point(FPOINT p, double r, FPOINT sp, FPOINT* rp1, FPOINT* rp2)
{
  double dx2, dy2, r2;
  FPOINT p2;
  p2.x = (p.x + sp.x) / 2;
  p2.y = (p.y + sp.y) / 2;
  dx2 = p2.x - p.x;
  dy2 = p2.y - p.y;
  r2 = SQRT(dx2 * dx2 + dy2 * dy2);
  circle2point(p, r, p2, r2, rp1, rp2);
}
// �߶ε�������:
// l2��l1����/�ҷ���l1Ϊ��׼�ߣ�
// ���� 0: �غϣ�
// ���� 1: ������
// ���� -1: ������
CC_INLINE int rotat(FLINESEG2 l1, FLINESEG2 l2)
{
  double dx1, dx2, dy1, dy2, d;
  dx1 = l1.a.x - l1.b.x;
  dy1 = l1.a.y - l1.b.y;
  dx2 = l2.a.x - l2.b.x;
  dy2 = l2.a.y - l2.b.y;
  d = dx1 * dy2 - dx2 * dy1;
  if (d == 0) {
    return 0;
  }
  else if (d > 0) {
    return - 1;
  }
  else {
    return 1;
  }
}
// ��ʽ:
// �����깫ʽ:
// ֱ������Ϊ P(x, y, z) ʱ����Ӧ����������(rSIN��cos��, rSIN��SIN��, rcos��),
// ���Ц�������OP��Z��ļнǣ���Χ[0����]����OP��XOY���ϵ�ͶӰ��X������ǣ���Χ[0��2��]
//
// ֱ�ߵ�һ�㷽��ת������������:
// ax+by+c=0
// x-x0 y-y0
// ------ = -------
// m n
// (x0,y0)Ϊֱ����һ�㣬m,nΪ����
// ת����ϵ:
// a=n��b=-m��c=m��y0-n��x0��
// m=-b; n=a;
//
// ����ƽ�淽��:
// ����ΪP1��P2��P3
// ������ M1=P2-P1; M2=P3-P1;
// ƽ�淨����: M=M1 x M2()
// ƽ�淽��: M.i(x-P1.x)+M.j(y-P1.y)+M.k(z-P1.z)=0
//��ƽ��������֮��ľ���
CC_INLINE double dpt_distance(FPOINT p1, FPOINT p2)
{
  return ((double)sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)));
}
CC_INLINE double dpt_linecross(FPOINT p1, FPOINT p2)
{
  return ((double)sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)));
}
//�жϵ�q�Ƿ��ڶ����Polygon�ڣ�
//���ж�����������͹�򰼶���Σ�
//Polygon�д�Ŷ���ε���ʱ�붥������
CC_INLINE int InsidePolygon(int vcount, FPOINT* Polygon, FPOINT q)
{
  int c = 0, i, n;
  FLINESEG2 l1, l2;
  l1.a = q;
  l1.b = q;
  l1.b.x = CC_INFINITY;
  n = vcount;
  for (i = 0; i < vcount; i++) {
    l2.a = Polygon[i];
    l2.b = Polygon[(i + 1) % n];
    if ((intersect_A(l1, l2)) ||
	((online(l1, Polygon[(i + 1) % n])) &&
	((!online(l1, Polygon[(i + 2) % n])) &&
	((multiply(Polygon[i], Polygon[(i + 1) % n], l1.a) * multiply(Polygon[(i + 1) % n], Polygon[(i + 2) % n], l1.a) > 0) ||
	((online(l1, Polygon[(i + 2) % n])) &&
	(multiply(Polygon[i], Polygon[(i + 2) % n], l1.a) *multiply(Polygon[(i + 2) % n], Polygon[(i + 3) % n], l1.a) > 0)))))) {
      c++;
    }
  }
  return (c % 2 != 0);
}
//Ѱ��͹�� graham ɨ�跨
//PointSetΪ����ĵ㼯��
//chΪ�����͹���ϵĵ㼯��������ʱ�뷽������;
//nΪPointSet�еĵ����Ŀ
//lenΪ�����͹���ϵĵ�ĸ�����
// ����һ��ɨ��ĵ�PointSet[i]=P2,��ǰջ����������ch[top]=P1,ch[top-1]=P0,
// ���P1P2�����P0P1�ڵ�P1������ת(����Ҳ����)����P0,P1һ����͹���ϵĵ㣻
// ����P1һ������͹���ϵĵ㣬Ӧ�ý����ջ��
// ����������ͼ�е�1��һ����͹���ϵĵ㣬��ͼ�е�1��һ������͹���ϵĵ㣬��Ϊ
// �������ӵ�0,2����͹���ı�
// 2
// |
// | _____2
// 1 1
// / /
// ____0/ ____0/
CC_INLINE int hull_Graham_scan(FPOINT* PointSet, FPOINT* ch, int n)
{
  int i, j, k = 0, top = 2;
  FPOINT tmp;
  //ѡȡPointSet��y������С�ĵ�PointSet[k]����������ĵ��Ҷ������ȡ����ߵ�һ��
  for (i = 1; i < n; i++) {
    if ((PointSet[i].y < PointSet[k].y) || ((PointSet[i].y == PointSet[k].y) && (PointSet[i].x < PointSet[k].x))) {
      k = i;
    }
  }
  tmp = PointSet[0];
  PointSet[0] = PointSet[k];
  PointSet[k] = tmp; //����PointSet��y������С�ĵ���PointSet[0]
  for (i = 1; i < n - 1; i++) {
    //�Զ��㰴�����PointSet[0]�ļ��Ǵ�С����������򣬼�����ͬ�İ��վ���PointSet[0]�ӽ���Զ��������
    k = i;
    for (j = i + 1; j < n; j++) {
      if ((multiply(PointSet[j], PointSet[k], PointSet[0]) > 0) || ((multiply(PointSet[j], PointSet[k], PointSet[0]) == 0) && (dpt_distance(PointSet[0], PointSet[j]) < dpt_distance(PointSet[0], PointSet[k])))) {
        k = j;
      }
    }
    tmp = PointSet[i];
    PointSet[i] = PointSet[k];
    PointSet[k] = tmp;
  }
  ch[0] = PointSet[0];
  ch[1] = PointSet[1];
  ch[2] = PointSet[2];
  for (i = 3; i < n; i++) {
    while (multiply(PointSet[i], ch[top], ch[top - 1]) >= 0) {
      top--;
    }
    ch[++top] = PointSet[i];
  }
  return top + 1;
}
CC_INLINE int test_geometry1()
{
  enum { MAX_points = 100 };
  int i, len;
  FPOINT pt1[MAX_points], pt2[MAX_points];
  //Ѱ��͹�� graham ɨ�跨
  // Graham_scan
  {
    RandPoints(pt1, MAX_points, 500, 500);
    len = hull_Graham_scan(pt1, pt2, MAX_points);
    printf("%d\n", len);
    for (i = 0; i < len; i++) {
      printf("%d %d\n", (int)pt2[i].x, (int)pt2[i].y);
    }
  }
  // intersect
  {
    int probN = 0;
    FLINESEG2 l1 = {0} , l2 = {0};
    printf("Case %d:\n", probN);
    printf("Line ((%0.2f,%0.2f),(%0.2f,%0.2f)) and ((%0.2f,%0.2f),(%0.2f,%0.2f)) are %sintersect.\n", l1.a.x, l1.a.y, l1.b.x, l1.b.y, l2.a.x, l2.a.y, l2.b.x, l2.b.y, (intersect(l1, l2) ? "" : "not "));
  }
  // dpolygon_area
  {
    double arr;
    arr = dpolygon_area(MAX_points, pt1);
    printf("The area of polygon is %f\n\n", arr);
  }
  // InsidePolygon
  {
    FPOINT p = {
      110, 110
    };
    int tt = InsidePolygon(MAX_points, pt1, p);
    printf("The point is %sin the polygon!\n", (tt ? "" : "not "));
  }
  return 0;
}
CC_INLINE int poly_tt(FPOLYGON* s)
{
  int i, off = 0;
  FPOINT t;
  for (i = 0; i < s->n; ++i) {
    double area = poly_area(s->len[i], s->pt + off);
    if (area < 0) {
      ARRREV(s->len[i], s->pt + off, t);
    }
    off += s->len[i];
  }
  return 0;
}
CC_INLINE int pt_in_poly(double x, double y, const FPOLYGON* s)
{
  int i, off = 0;
  for (i = 0; i < s->n; ++i) {
    if (point_in_polygon(s->pt + off, s->len[i], x, y)) {
      return TRUE;
    }
    off += s->len[i];
  }
  return FALSE;
}
#ifdef _WINDOWS_
CC_INLINE int poly_setstr(FPOLYGON* s, str_t ss)
{
  int i, n, npt = 0;
  str_t s1, s2;
  const char* nextline = "\r\n";
  const char* spacechr = " \t";
  s1 = STRsplit(ss, "\n", nextline, &ss);
  s->n = n = str_toi(s1);
  REALLOC(int, s->len, n);
  for (i = 0; i < n; ++i) {
    s1 = STRsplit(ss, "\n", nextline, &ss);
    s->len[i] = str_toi(s1);
    npt += s->len[i];
  }
  REALLOC(FPOINT, s->pt, npt);
  for (i = 0; i < npt; ++i) {
    s1 = STRsplit(ss, "\n", nextline, &ss);
    s2 = STRsplit(s1, ",", spacechr, &s1);
    s->pt[i].x = str_tof(s2);
    s2 = STRsplit(s1, ",", spacechr, &s1);
    s->pt[i].y = str_tof(s2);
  }
  return 0;
}
#include "cfile.h"
CC_INLINE int poly_load(FPOLYGON* pc, const char* fn)
{
  str_t bf[1] = {0};
  str_load(fn, bf);
  if (bf->l > 0) {
    poly_setstr(pc, bf[0]);
  }
  str_free(bf);
  return 0;
}
CC_INLINE int poly_save(FPOLYGON* s, const char* fn)
{
  FILE* pf = fopen(fn, "wb");
  if (pf) {
    int i, npt = 0;
    fprintf(pf, "%d\n", s->n);
    for (i = 0; i < s->n; ++i) {
      fprintf(pf, "%d\n", s->len[i]);
      npt += s->len[i];
    }
    for (i = 0; i < npt; ++i) {
      fprintf(pf, "%.1f,%.1f\n", s->pt[i].x, s->pt[i].y);
    }
    fclose(pf);
  }
  return 0;
}
#endif
////////////////////////////////////////////////////////
//rect
#if 0
CENTER ��ͼƬ��ԭ��size������ʾ����ͼƬ�� / ����View�ĳ� / �����ȡͼƬ�ľ��в�����ʾ
CENTER_CROP ����������ͼƬ��size������ʾ��ʹ��ͼƬ��(��)���ڻ����View�ĳ�(��)
CENTER_INSIDE ��ͼƬ����������������ʾ��ͨ����������С��ԭ����sizeʹ��ͼƬ�� / ����ڻ�С��View�ĳ� / ��
FIT_CENTER ��ͼƬ���������� / ��С��View�Ŀ�ȣ�������ʾ
FIT_START, FIT_END ��ͼƬ����Ч������ FIT_CENTER һ����ֻ����ʾ��λ�ò�ͬ��FIT_START�����ڶ�����FIT_CENTER���У�FIT_END���ڵײ���
FIT_XY ������������ͼƬ��Ŀ���ǰ�ͼƬ��������View��
#endif
CC_INLINE BOOL iPtInPolygon(const IPOINT* m_polygon, int m_num_points, int tx, int ty)
{
  int j, k;
  int yflag0, yflag1, inside_flag;
  int vtx0, vty0, vtx1, vty1;
  if (m_num_points < 3) {
    return FALSE;
  }
  vtx0 = m_polygon[m_num_points - 1].x;
  vty0 = m_polygon[m_num_points - 1].y;
  // get test bit for above/below X axis
  yflag0 = (vty0 >= ty);
  vtx1 = m_polygon[0].x;
  vty1 = m_polygon[0].y;
  inside_flag = 0;
  for (j = 1; j <= m_num_points; ++j) {
    yflag1 = (vty1 >= ty);
    if (yflag0 != yflag1) {
      if (((vty1 - ty) * (vtx0 - vtx1) >= (vtx1 - tx) * (vty0 - vty1)) == yflag1) {
        inside_flag ^= 1;
      }
    }
    // Move to the next pair of vertices, retaining info as possible.
    yflag0 = yflag1;
    vtx0 = vtx1;
    vty0 = vty1;
    k = (j >= m_num_points) ? j - m_num_points : j;
    vtx1 = m_polygon[k].x;
    vty1 = m_polygon[k].y;
  }
  return inside_flag != 0;
}
CC_INLINE int iPtInPolygon1(const IPOINT* ptPolygon, int nCount, int x, int y)
{
  int i, nCross = 0;
  for (i = 0; i < nCount; i++) {
    IPOINT p1 = ptPolygon[i];
    IPOINT p2 = ptPolygon[(i + 1) % nCount];
    // ��� y=p.y �� p1p2 �Ľ���
    if (p1.y == p2.y) { // p1p2 �� y=p0.yƽ��
      continue;
    }
    if (y < MIN(p1.y, p2.y)) { // ������p1p2�ӳ�����
      continue;
    }
    if (y >= MAX(p1.y, p2.y)) { // ������p1p2�ӳ�����
      continue;
    }
    if ((y - p1.y) * (p2.x - p1.x) > (x - p1.x) * (p2.y - p1.y)) {
      nCross++; // ֻͳ�Ƶ��߽���
    }
  }
  // ���߽���Ϊż�������ڶ����֮�� ---
  return (nCross % 2 == 1);
}
// ����ҿ�
typedef FPOINT point2d;
CC_INLINE int dRectSet(DRECT* prc, double l, double t, double r, double b)
{
  prc->l = l, prc->t = t, prc->r = r, prc->b = b;
  return 0;
}
#define rectd_setrect(prc, pr) rectd_set(prc, (pr)->l, (pr)->t, (pr)->r, (pr)->b)
#define rectd_torect(prc) iRECT((int)(prc)->l, (int)(prc)->t, (int)(prc)->r, (int)(prc)->b)
CC_INLINE DRECT dRectZoom(const DRECT* b, const DRECT* c, const DRECT* d)
{
  return dRECT(b->l + (c->l - d->l) * RCW(b) / RCW(d),
      b->t + (c->t - d->t) * RCH(b) / RCH(d),
      b->l + (c->r - d->l) * RCW(b) / RCW(d),
      b->t + (c->b - d->t) * RCH(b) / RCH(d));
}
/****************************************************************************************\
*** vcgen
\****************************************************************************************/
// ��άͼ�εļ���
// Cohen-Sutherland �����㷨
// ����:
// 0: �߶���ȫ�ھ���������
// 1: ��x1, y1���ü����滻
// 2: ��x2, y2���ü����滻
// 3: ���㶼���ü����滻
// 4: �߶���ȫ�ھ���������
CC_INLINE int TClipLine(const DRECT* edge, double* x1, double* y1, double* x2, double* y2)
{
  int result = 0;
  int c1, c2;
  if (edge->r < edge->l || edge->b < edge->t) {
    return (0); // ��ȫ�ų����û���
  }
  // c1 ��0 ��ʾ�ھ��ο���
  // 0101 | 0100 | 0110
  // 0001 | 0000 | 0010
  // 1001 | 1000 | 1010
  c1 = (*x1 < edge->l) + (*x1 > edge->r) * 2 + (*y1 < edge->t) * 4 + (*y1 > edge->b) * 8;
  c2 = (*x2 < edge->l) + (*x2 > edge->r) * 2 + (*y2 < edge->t) * 4 + (*y2 > edge->b) * 8;
  if (0 == (c1 | c2)) {
    result = 4;
  }
  if ((c1 & c2) == 0 && (c1 | c2) != 0) {
    double a, dx = *x2 - *x1, dy = *y2 - *y1;
    if (c1 & 12) {
      a = c1 < 8 ? edge->t : edge->b;
      *x1 += (a - *y1) * dx / dy;
      *y1 = a;
      c1 = (*x1 < edge->l) + (*x1 > edge->r) * 2;
      result |= 1;
    }
    if (c2 & 12) {
      a = c2 < 8 ? edge->t : edge->b;
      *x2 += (a - *y2) * dx / dy;
      *y2 = a;
      c2 = (*x2 < edge->l) + (*x2 > edge->r) * 2;
      result |= 2;
    }
    if ((c1 & c2) == 0 && (c1 | c2) != 0) {
      if (c1) {
        a = c1 == 1 ? edge->l : edge->r;
        *y1 += (a - *x1) * dy / dx;
        *x1 = a;
        c1 = 0;
        result |= 1;
      }
      if (c2) {
        a = c2 == 1 ? edge->l : edge->r;
        *y2 += (a - *x2) * dy / dx;
        *x2 = a;
        c2 = 0;
        result |= 2;
      }
    }
  }
  return result;
}
// Sutherland - Hodgman�����㷨
CC_INLINE int TClipPoly(const DRECT* edge, const FPOINT* pt, int n, FPOINT* pt0)
{
  double x1, y1, x2, y2;
  int i, ret, len = n;
  FPOINT* pt1 = pt0;
  int t = 0;
  x1 = pt[len - 1].x, y1 = pt[len - 1].y, x2 = pt[0].x, y2 = pt[0].y;
  //x1=1,y1=100,x2=2,y2=200;
  ret = TClipLine(edge, &x1, &y1, &x2, &y2);
  if (2 & ret) {
    //prex = x2, prey = y2; TRACE
  }
  for (i = 0; i < len; ++i) {
    int j = (i + 1) % len;
    x1 = pt[i].x, y1 = pt[i].y, x2 = pt[j].x, y2 = pt[j].y;
    ret = TClipLine(edge, &x1, &y1, &x2, &y2);
    if (4 == ret) {
      int asdf = 0;
    }
    if (3 == ret) {
      int asdf = 0;
    }
    if (ret) {
      if (4 == ret) {
        *pt1++ = fPOINT(x1, y1);
      }
      else {
        if (1 == ret) {
          *pt1++ = fPOINT(x1, y1);
          *pt1++ = fPOINT(x2, y2);
        }
        if (2 == ret) {
          *pt1++ = fPOINT(x1, y1);
          *pt1++ = fPOINT(x2, y2);
        }
        if (3 == ret) {
          *pt1++ = fPOINT(x1, y1);
          *pt1++ = fPOINT(x2, y2);
        }
      }
    }
  }
  return n;
}
#include "vcgen.inl"

// Spline Curve Horizontal and Vertical Extent (Span, Bounding Rectangle)
CC_INLINE double splineXMax(double Sx, double a, double Ex)
{
  double c = Sx - a, d = c + Ex - a, F, X, max1 = Sx ;
  if (Ex > Sx) {
    max1 = Ex;
  }
  if (d != 0.) {
    F = c / d;
    if (F > 0. && F < 1.) {
      X = Sx - c * F;
      if (X > max1) {
        max1 = X;
      }
    }
  }
  return max1;
}
CC_INLINE double splineXMin(double Sx, double a, double Ex)
{
  double c = Sx - a, d = c + Ex - a, F, X, min = Sx ;
  if (Ex < Sx) {
    min = Ex;
  }
  if (d != 0.) {
    F = c / d;
    if (F > 0. && F < 1.) {
      X = Sx - c * F;
      if (X < min) {
        min = X;
      }
    }
  }
  return min;
}
CC_INLINE double splineYMax(double Sy, double b, double Ey)
{
  double c = Sy - b, d = c + Ey - b, F, Y, max1 = Sy ;
  if (Ey > Sy) {
    max1 = Ey;
  }
  if (d != 0.) {
    F = c / d;
    if (F > 0. && F < 1.) {
      Y = Sy - c * F;
      if (Y > max1) {
        max1 = Y;
      }
    }
  }
  return max1;
}
CC_INLINE double splineYMin(double Sy, double b, double Ey)
{
  double c = Sy - b, d = c + Ey - b, F, Y, min = Sy ;
  if (Ey < Sy) {
    min = Ey;
  }
  if (d != 0.) {
    F = c / d;
    if (F > 0. && F < 1.) {
      Y = Sy - c * F;
      if (Y < min) {
        min = Y;
      }
    }
  }
  return min;
}
// public-domain code by Darel Rex Finley, January 2009
#define CIRCLE_RADIANS 6.283185307179586476925286766559
// Determines the radian angle of the specified point (as it relates to the origin).
//
// Warning: Do not pass zero in both parameters, as this will cause division-by-zero.
CC_INLINE double angleOf(double x, double y)
{
  double dist = sqrt(x * x + y * y) ;
  if (y >= 0.) {
    return acos(x / dist);
  }
  else {
    return acos(-x / dist) + .5 * CIRCLE_RADIANS;
  }
}
// Pass in a set of 2D points in x,y,points. Returns a polygon in polyX,polyY,polyCorners.
//
// To be safe, polyX and polyY should have enough space to store all the points passed in x,y,points.
CC_INLINE void findSmallestPolygon(double* x, double* y, long points, double* polyX, double* polyY, long* polyCorners)
{
  double newX = x[0], newY = y[0], xDif, yDif, oldAngle = .5 * CIRCLE_RADIANS, newAngle, angleDif, minAngleDif ;
  long i ;
  // Find a starting point.
  for (i = 0; i < points; i++) if (y[i] > newY || (y[i] == newY && x[i] < newX)) {
      newX = x[i];
      newY = y[i];
    }
  *polyCorners = 0;
  // Polygon-construction loop.
  while (!(*polyCorners) || newX != polyX[0] || newY != polyY[0]) {
    polyX[*polyCorners] = newX;
    polyY[*polyCorners] = newY;
    minAngleDif = CIRCLE_RADIANS;
    for (i = 0; i < points; i++) {
      xDif = x[i] - polyX[*polyCorners];
      yDif = y[i] - polyY[*polyCorners];
      if (xDif || yDif) {
        newAngle = angleOf(xDif, yDif);
        angleDif = oldAngle - newAngle;
        while (angleDif < 0.) {
          angleDif += CIRCLE_RADIANS;
        }
        while (angleDif >= CIRCLE_RADIANS) {
          angleDif -= CIRCLE_RADIANS;
        }
        if (angleDif < minAngleDif) {
          minAngleDif = angleDif;
          newX = x[i];
          newY = y[i];
        }
      }
    }
    (*polyCorners)++;
    oldAngle += .5 * CIRCLE_RADIANS - minAngleDif;
  }
}
// Point-In-Spline-Polygon Algorithm �� Testing Whether A Point Is Inside A Complex Polygon With Spline Curves
// http://alienryderflex.com/polyspline/
//This five-cornered polygon makes a simple house shape:
//1,0, 1,-1, -1,-1, -1,0, 0,1, SP_END polygon
//This is the same house shape, but with the top corner turned into a spline point:
//1,0, 1,-1, -1,-1, -1,0, SP_SPLINE,0,1, SP_END spline polygon
//Same house shape, but with the two roof��s-eve corners turned into spline points:
//SP_SPLINE,1,0, 1,-1, -1,-1, SP_SPLINE,-1,0, 0,1, SP_END
//Note that this case would not work without the division-by-zero protection, because the eve points are vertically at the midpoint between the top and bottom of the house. spline polygon
//Same house shape, with the two bottom corner points turned into spline points:
//1,0, SP_SPLINE,1,-1, SP_SPLINE,-1,-1, -1,0, 0,1, SP_END spline polygon
//Same house shape, with all five corners turned into spline points:
//SP_SPLINE,1,0, SP_SPLINE,1,-1, SP_SPLINE,-1,-1, SP_SPLINE,-1,0, SP_SPLINE,0,1, SP_END spline polygon
//Here��s an example of using the ��SP_NEW_LOOP�� tag to put two polygons in one. In this case, the smaller polygon is a hole in the larger one, since it is inside it:
//SP_SPLINE,1,1, SP_SPLINE,1,-1, SP_SPLINE,-1,-1, SP_SPLINE,-1,1, SP_NEW_LOOP, SP_SPLINE,.5,.5, SP_SPLINE,.5,-.5, SP_SPLINE,-.5,-.5, SP_SPLINE,-.5,.5, SP_END spline polygon with hole
//This uses the ��SP_NEW_LOOP�� tag to make two loops, but they don��t overlap each other:
//-.25,0, .125,0, .125,-.875, .25,-.875, .25,-1, -.25,-1, -.25,-.875, -.125,-.875, -.125,-.125, -.25,-.125, SP_NEW_LOOP, SP_SPLINE,-.125,.375, SP_SPLINE,.125,.375, SP_SPLINE,.125,.25, SP_SPLINE,-.125,.25, SP_END spline lower-case letter i
//You can��t do a real circle with splines, but this eight-point polygon makes a sweet fake:
//SP_SPLINE,.4142,1, SP_SPLINE,1,.4142, SP_SPLINE,1,-.4142, SP_SPLINE,.4142,-1, SP_SPLINE,-.4142,-1, SP_SPLINE,-1,-.4142, SP_SPLINE,-1,.4142, SP_SPLINE,-.4142,1, SP_END
//FYI, .4142 is sqrt(2)-1.
#define NO 0
#define YES 1
#define SP_END (20000000.)
#define SP_SPLINE (SP_END+1.)
#define SP_NEW_LOOP (SP_END+2.)
CC_INLINE BOOL pointInSplinePoly(double* poly, double X, double Y)
{
  double Sx, Sy, Ex, Ey, a, b, sRoot, F, plusOrMinus, topPart, bottomPart, xPart ;
  int i = 0, j, k, start = 0 ;
  BOOL oddNodes = NO ;
  Y += .000001; // prevent the need for special tests when F is exactly 0 or 1
  while (poly[i] != SP_END) {
    j = i + 2;
    if (poly[i] == SP_SPLINE) {
      j++;
    }
    if (poly[j] == SP_END || poly[j] == SP_NEW_LOOP) {
      j = start;
    }
    if (poly[i] != SP_SPLINE && poly[j] != SP_SPLINE) { // STRAIGHT LINE
      if ((poly[i + 1] < Y && poly[j + 1] >= Y) || (poly[j + 1] < Y && poly[i + 1] >= Y)) {
        if (poly[i] + (Y - poly[i + 1]) / (poly[j + 1] - poly[i + 1]) * (poly[j] - poly[i]) < X) {
          oddNodes = !oddNodes;
        }
      }
    }
    else if (poly[j] == SP_SPLINE) { // SP_SPLINE CURVE
      a = poly[j + 1];
      b = poly[j + 2];
      k = j + 3;
      if (poly[k] == SP_END || poly[k] == SP_NEW_LOOP) {
        k = start;
      }
      if (poly[i] != SP_SPLINE) {
        Sx = poly[i];
        Sy = poly[i + 1];
      }
      else { // interpolate hard corner
        Sx = (poly[i + 1] + poly[j + 1]) / 2.;
        Sy = (poly[i + 2] + poly[j + 2]) / 2.;
      }
      if (poly[k] != SP_SPLINE) {
        Ex = poly[k];
        Ey = poly[k + 1];
      }
      else { // interpolate hard corner
        Ex = (poly[j + 1] + poly[k + 1]) / 2.;
        Ey = (poly[j + 2] + poly[k + 2]) / 2.;
      }
      bottomPart = 2.*(Sy + Ey - b - b);
      if (bottomPart == 0.) { // prevent division-by-zero
        b += .0001;
        bottomPart = -.0004;
      }
      sRoot = 2.*(b - Sy);
      sRoot *= sRoot;
      sRoot -= 2.*bottomPart * (Sy - Y);
      if (sRoot >= 0.) {
        sRoot = sqrt(sRoot);
        topPart = 2.*(Sy - b);
        for (plusOrMinus = -1.; plusOrMinus < 1.1; plusOrMinus += 2.) {
          F = (topPart + plusOrMinus * sRoot) / bottomPart;
          if (F >= 0. && F <= 1.) {
            xPart = Sx + F * (a - Sx);
            if (xPart + F * (a + F * (Ex - a) - xPart) < X) {
              oddNodes = !oddNodes;
            }
          }
        }
      }
    }
    if (poly[i] == SP_SPLINE) {
      i++;
    }
    i += 2;
    if (poly[i] == SP_NEW_LOOP) {
      i++;
      start = i;
    }
  }
  return oddNodes;
}
// public domain function by Darel Rex Finley, 2009
CC_INLINE double splinePolyArea(double* poly)
{
  double area = 0., a, b, Sx, Sy, Ex, Ey ;
  int i = 0, j, k ;
  while (poly[i] != SP_END) {
    j = i + 2;
    if (poly[i] == SP_SPLINE) {
      j++;
    }
    if (poly[j] == SP_END) {
      j = 0;
    }
    if (poly[i] != SP_SPLINE && poly[j] != SP_SPLINE) { // STRAIGHT LINE
      area += (poly[i] + poly[j]) * (poly[i + 1] - poly[j - 1]);
    }
    else if (poly[j] == SP_SPLINE) { // SP_SPLINE CURVE
      a = poly[j + 1];
      b = poly[j + 2];
      k = j + 3;
      if (poly[k] == SP_END) {
        k = 0;
      }
      if (poly[i] != SP_SPLINE) {
        Sx = poly[i];
        Sy = poly[i + 1];
      }
      else { // interpolate hard corner
        Sx = (poly[i + 1] + poly[j + 1]) / 2.;
        Sy = (poly[i + 2] + poly[j + 2]) / 2.;
      }
      if (poly[k] != SP_SPLINE) {
        Ex = poly[k];
        Ey = poly[k + 1];
      }
      else { // interpolate hard corner
        Ex = (poly[j + 1] + poly[k + 1]) / 2.;
        Ey = (poly[j + 2] + poly[k + 2]) / 2.;
      }
      area += (Sx + Ex) * (Sy - Ey);
      area += ((Sx + a) * (Sy - b) + (a + Ex) * (b - Ey) + (Ex + Sx) * (Ey - Sy)) * (2. / 3.);
    }
    if (poly[i] == SP_SPLINE) {
      i++;
    }
    i += 2;
  }
  return area * .5;
}
typedef struct polySet {
  struct {
    int corners;
    double* y;
    double* x;
  }* poly;
  int count;
} polySet;
typedef struct sp_point {
  double x, y;
  int prev;
  double totalDist;
} sp_point;
// Public-domain code by Darel Rex Finley, 2006.
// (This function automatically knows that enclosed polygons are "no-go" areas.)
CC_INLINE BOOL pointInPolygonSet(double testX, double testY, polySet allPolys)
{
  BOOL oddNodes = NO;
  int polyI, i, j ;
  for (polyI = 0; polyI < allPolys.count; polyI++) {
    for (i = 0; i < allPolys.poly[polyI].corners; i++) {
      j = i + 1;
      if (j == allPolys.poly[polyI].corners) {
        j = 0;
      }
      if ((allPolys.poly[polyI].y[i] < testY && allPolys.poly[polyI].y[j] >= testY)
          || (allPolys.poly[polyI].y[j] < testY && allPolys.poly[polyI].y[i] >= testY)) {
        if (allPolys.poly[polyI].x[i] + (testY - allPolys.poly[polyI].y[i])
            / (allPolys.poly[polyI].y[j] - allPolys.poly[polyI].y[i])
            * (allPolys.poly[polyI].x[j] - allPolys.poly[polyI].x[i]) < testX) {
          oddNodes = !oddNodes;
        }
      }
    }
  }
  return oddNodes;
}
// Shortest Path Through A Concave Polygon With Holes
// This function should be called with the full set of *all* relevant polygons.
// (The algorithm automatically knows that enclosed polygons are ��no-go�� areas.)
//
// Note: As much as possible, this algorithm tries to return YES when the
// test line-segment is exactly on the border of the polygon, particularly
// if the test line-segment *is* a side of a polygon.
CC_INLINE BOOL lineInPolygonSet(double testSX, double testSY, double testEX, double testEY, polySet allPolys)
{
  double theCos, theSin, dist, sX, sY, eX, eY, rotSX, rotSY, rotEX, rotEY, crossX ;
  int i, j, polyI ;
  testEX -= testSX;
  testEY -= testSY;
  dist = sqrt(testEX * testEX + testEY * testEY);
  theCos = testEX / dist;
  theSin = testEY / dist;
  for (polyI = 0; polyI < allPolys.count; polyI++) {
    for (i = 0; i < allPolys.poly[polyI].corners; i++) {
      j = i + 1;
      if (j == allPolys.poly[polyI].corners) {
        j = 0;
      }
      sX = allPolys.poly[polyI].x[i] - testSX;
      sY = allPolys.poly[polyI].y[i] - testSY;
      eX = allPolys.poly[polyI].x[j] - testSX;
      eY = allPolys.poly[polyI].y[j] - testSY;
      if ((sX == 0. && sY == 0. && eX == testEX && eY == testEY)
          || (eX == 0. && eY == 0. && sX == testEX && sY == testEY)) {
        return YES;
      }
      rotSX = sX * theCos + sY * theSin;
      rotSY = sY * theCos - sX * theSin;
      rotEX = eX * theCos + eY * theSin;
      rotEY = eY * theCos - eX * theSin;
      if ((rotSY < 0. && rotEY > 0.) || (rotEY < 0. && rotSY > 0.)) {
        crossX = rotSX + (rotEX - rotSX) * (0. - rotSY) / (rotEY - rotSY);
        if (crossX >= 0. && crossX <= dist) {
          return NO;
        }
      }
      if (rotSY == 0. && rotEY == 0.
          && (rotSX >= 0. || rotEX >= 0.)
          && (rotSX <= dist || rotEX <= dist)
          && (rotSX < 0. || rotEX < 0.
              || rotSX > dist || rotEX > dist)) {
        return NO;
      }
    }
  }
  return pointInPolygonSet(testSX + testEX / 2., testSY + testEY / 2., allPolys);
}
CC_INLINE double calcDist(double sX, double sY, double eX, double eY)
{
  eX -= sX, eY -= sY;
  return sqrt(eX * eX + eY * eY);
}
CC_INLINE void swapPoints(sp_point* a, sp_point* b)
{
  sp_point swap = *a;
  *a = *b;
  *b = swap;
}
// Finds the shortest path from sX,sY to eX,eY that stays within the polygon set.
//
// Note: To be safe, the solutionX and solutionY arrays should be large enough
// to accommodate all the corners of your polygon set (although it is
// unlikely that anywhere near that many elements will ever be needed).
//
// Returns YES if the optimal solution was found, or NO if there is no solution.
// If a solution was found, solutionX and solutionY will contain the coordinates
// of the intermediate nodes of the path, in order. (The startpoint and endpoint
// are assumed, and will not be included in the solution.)
CC_INLINE BOOL shortestPath(int pointcount, double sX, double sY, double eX, double eY, polySet allPolys,
    double* solutionX, double* solutionY, int* solutionNodes)
{
  //#define CC_INF 9999999. // (larger than total solution dist could ever be)
  sp_point pointList[1000] ; // (enough for all polycorners plus two)
  int pointCount ;
  int treeCount, polyI, i, j, bestI, bestJ ;
  double bestDist, newDist ;
  // Fail if either the startpoint or endpoint is outside the polygon set.
  if (!pointInPolygonSet(sX, sY, allPolys)
      || !pointInPolygonSet(eX, eY, allPolys)) {
    return NO;
  }
  // If there is a straight-line solution, return with it immediately.
  if (lineInPolygonSet(sX, sY, eX, eY, allPolys)) {
    (*solutionNodes) = 0;
    return YES;
  }
  // Build a point list that refers to the corners of the
  // polygons, as well as to the startpoint and endpoint.
  pointList[0].x = sX;
  pointList[0].y = sY;
  pointCount = 1;
  for (polyI = 0; polyI < allPolys.count; polyI++) {
    for (i = 0; i < allPolys.poly[polyI].corners; i++) {
      pointList[pointCount].x = allPolys.poly[polyI].x[i];
      pointList[pointCount].y = allPolys.poly[polyI].y[i];
      pointCount++;
    }
  }
  pointList[pointCount].x = eX;
  pointList[pointCount].y = eY;
  pointCount++;
  // Initialize the shortest-path tree to include just the startpoint.
  treeCount = 1;
  pointList[0].totalDist = 0.;
  // Iteratively grow the shortest-path tree until it reaches the endpoint
  // -- or until it becomes unable to grow, in which case exit with failure.
  bestJ = 0;
  while (bestJ < pointcount - 1) {
    bestDist = CC_INF;
    for (i = 0; i < treeCount; i++) {
      for (j = treeCount; j < pointCount; j++) {
        if (lineInPolygonSet(
            pointList[i].x, pointList[i].y,
            pointList[j].x, pointList[j].y, allPolys)) {
          newDist = pointList[i].totalDist + calcDist(
              pointList[i].x, pointList[i].y,
              pointList[j].x, pointList[j].y);
          if (newDist < bestDist) {
            bestDist = newDist;
            bestI = i;
            bestJ = j;
          }
        }
      }
    }
    if (bestDist == CC_INF) {
      return NO; // (no solution)
    }
    pointList[bestJ].prev = bestI ;
    pointList[bestJ].totalDist = bestDist;
    swapPoints(&pointList[bestJ], &pointList[treeCount]);
    treeCount++;
  }
  // Load the solution arrays.
  (*solutionNodes) = -1;
  i = treeCount - 1;
  while (i > 0) {
    i = pointList[i].prev;
    (*solutionNodes)++;
  }
  j = (*solutionNodes) - 1;
  i = treeCount - 1;
  while (j >= 0) {
    i = pointList[i].prev;
    solutionX[j] = pointList[i].x;
    solutionY[j] = pointList[i].y;
    j--;
  }
  // Success.
  return YES;
}
// public-domain code by Darel Rex Finley,
// 2010. See diagrams at http://
// alienryderflex.com/point_left_of_ray
CC_INLINE BOOL isPointLeftOfRay(double x, double y, double raySx, double raySy, double rayEx, double rayEy)
{
  double theCos, theSin, dist ;
  // Translate the system so that the ray
  // starts on the origin.
  rayEx -= raySx;
  rayEy -= raySy;
  x -= raySx;
  y -= raySy;
  // Discover the ray��s length.
  dist = sqrt(rayEx * rayEx + rayEy * rayEy);
  // Rotate the system so that the ray
  // points along the positive X-axis.
  theCos = rayEx / dist;
  theSin = rayEy / dist;
  y = y * theCos - x * theSin;
  // Return the result.
  return y > 0.;
}
//Now here��s the same function, but stripped of unneeded calculations:
// public-domain code by Darel Rex Finley,
// 2010. See diagrams at http://alienryderflex.com/point_left_of_ray
CC_INLINE BOOL isPointLeftOfRay1(double x, double y, double raySx, double raySy, double rayEx, double rayEy)
{
  return (y - raySy) * (rayEx - raySx) > (x - raySx) * (rayEy - raySy);
}
CC_INLINE double polygonArea(double* X, double* Y, int points)
{
  double area = 0. ;
  int i, j = points - 1 ;
  for (i = 0; i < points; i++) {
    area += (X[j] + X[i]) * (Y[j] - Y[i]);
    j = i;
  }
  return area * .5;
}
// public-domain code by Darel Rex Finley, 2009
// See diagrams at http://alienryderflex.com/polygon_hatchline_fill
// This function returns NO if MAX_NODES is exceeded, in which case some
// of the stripes may have been drawn, but not all of them.
CC_INLINE BOOL drawDiagonalStripes(long polygonCount, long* polygonCorners, double** polygons, double spacing)
{
#define MAX_NODES 1000
#define FAR_FAR_AWAY 999999999.
  double spanMin = FAR_FAR_AWAY, theCos, theSin, nodeX[MAX_NODES] ;
  double spanMax = -FAR_FAR_AWAY, x, y, a, b, newX, stripeY, swap ;
  long i, j, k, spanStart, spanEnd, nodeCount, step ;
  // Create a 45-degree angle for stripes.
  theCos = sqrt(.5);
  theSin = sqrt(.5);
  // Loop to determine the span over which diagonal lines must be drawn.
  for (i = 0; i < polygonCount ; i++) {
    for (j = 0; j < polygonCorners[i]; j++) {
      x = polygons[i][j * 2 ];
      y = polygons[i][j * 2 + 1];
      // Rotate the point, since the stripes may be at an angle.
      y = y * theCos - x * theSin;
      // Adjust the span.
      if (spanMin > y) {
        spanMin = y;
      }
      if (spanMax < y) {
        spanMax = y;
      }
    }
  }
  // Turn the span into a discrete step range.
  spanStart = (long) floor(spanMin / spacing) - 1;
  spanEnd = (long) floor(spanMax / spacing) + 1;
  // Loop to create all stripes.
  for (step = spanStart; step <= spanEnd; step++) {
    nodeCount = 0;
    stripeY = spacing * (double) step;
    // Loop to build a node list for one row of stripes.
    for (i = 0; i < polygonCount ; i++) {
      k = polygonCorners[i] - 1;
      for (j = 0; j < polygonCorners[i]; j++) {
        a = polygons[i][k * 2 ];
        b = polygons[i][k * 2 + 1];
        x = polygons[i][j * 2 ];
        y = polygons[i][j * 2 + 1];
        // Rotate the points, since the stripes may be at an angle.
        newX = a * theCos + b * theSin;
        b = b * theCos - a * theSin;
        a = newX;
        newX = x * theCos + y * theSin;
        y = y * theCos - x * theSin;
        x = newX;
        // Find the node, if any.
        if ((b < stripeY && y >= stripeY)
            || (y < stripeY && b >= stripeY)) {
          if (nodeCount >= MAX_NODES) {
            return NO;
          }
          nodeX[nodeCount++] = a + (x - a) * (stripeY - b) / (y - b);
        }
        k = j;
      }
      // Sort the node list.
      i = 0;
      while (i < nodeCount - 1) {
        if (nodeX[i] <= nodeX[i + 1]) {
          i++;
        }
        else {
          swap = nodeX[i];
          nodeX[i] = nodeX[i + 1];
          nodeX[i + 1] = swap;
          if (i) {
            i--;
          }
        }
      }
      // Loop to draw one row of stripe segments.
      for (i = 0; i < nodeCount; i += 2) {
        // Rotate the points back to their original coordinate system.
        a = nodeX[i ] * theCos - stripeY * theSin;
        b = stripeY * theCos + nodeX[i ] * theSin;
        x = nodeX[i + 1] * theCos - stripeY * theSin;
        y = stripeY * theCos + nodeX[i + 1] * theSin;
#define drawLineSegmentABtoXY(a, b, x, y)
        // Draw a single stripe segment.
        drawLineSegmentABtoXY(a, b, x, y);
      }
    }
  }
  // Success.
  return YES;
}
//////////////////////////////////////////////////////////////////////////////////////////////
// public-domain code by Darel Rex Finley, 2007
// See examples at http://alienryderflex.com/polygon_perimeter
#define CIRCLE_RADIANS 6.283185307179586476925286766559
#define MAX_SEGS 1000
// public domain function by Darel Rex Finley, 2006
// Determines the intersection point of the line defined by points A and B with the
// line defined by points C and D.
//
// Returns YES if the intersection point was found, and stores that point in X,Y.
// Returns NO if there is no determinable intersection point, in which case X,Y will
// be unmodified.
CC_INLINE BOOL lineIntersection(double Ax, double Ay, double Bx, double By, double Cx, double Cy, double Dx, double Dy, double* X, double* Y)
{
  double distAB, theCos, theSin, newX, ABpos ;
  // Fail if either line is undefined.
  if ((Ax == Bx && Ay == By) || (Cx == Dx && Cy == Dy)) {
    return NO;
  }
  // (1) Translate the system so that point A is on the origin.
  Bx -= Ax;
  By -= Ay;
  Cx -= Ax;
  Cy -= Ay;
  Dx -= Ax;
  Dy -= Ay;
  // Discover the length of segment A-B.
  distAB = sqrt(Bx * Bx + By * By);
  // (2) Rotate the system so that point B is on the positive X axis.
  theCos = Bx / distAB;
  theSin = By / distAB;
  newX = Cx * theCos + Cy * theSin;
  Cy = Cy * theCos - Cx * theSin;
  Cx = newX;
  newX = Dx * theCos + Dy * theSin;
  Dy = Dy * theCos - Dx * theSin;
  Dx = newX;
  // Fail if the lines are parallel.
  if (Cy == Dy) {
    return NO;
  }
  // (3) Discover the position of the intersection point along line A-B.
  ABpos = Dx + (Cx - Dx) * Dy / (Dy - Cy);
  // (4) Apply the discovered position to line A-B in the original coordinate system.
  *X = Ax + ABpos * theCos;
  *Y = Ay + ABpos * theSin;
  // Success.
  return YES;
}
//If you need to find out only when (and where) the line segments intersect, you can modify the function as follows:
// public domain function by Darel Rex Finley, 2006
// Determines the intersection point of the line segment defined by points A and B
// with the line segment defined by points C and D.
//
// Returns YES if the intersection point was found, and stores that point in X,Y.
// Returns NO if there is no determinable intersection point, in which case X,Y will
// be unmodified.
CC_INLINE BOOL lineSegmentIntersection(double Ax, double Ay, double Bx, double By, double Cx, double Cy, double Dx, double Dy, double* X, double* Y)
{
  double distAB, theCos, theSin, newX, ABpos ;
  // Fail if either line segment is zero-length.
  if ((Ax == Bx && Ay == By) || (Cx == Dx && Cy == Dy)) {
    return NO;
  }
  // Fail if the segments share an end-point.
  if ((Ax == Cx && Ay == Cy) || (Bx == Cx && By == Cy)
      || (Ax == Dx && Ay == Dy) || (Bx == Dx && By == Dy)) {
    return NO;
  }
  // (1) Translate the system so that point A is on the origin.
  Bx -= Ax;
  By -= Ay;
  Cx -= Ax;
  Cy -= Ay;
  Dx -= Ax;
  Dy -= Ay;
  // Discover the length of segment A-B.
  distAB = sqrt(Bx * Bx + By * By);
  // (2) Rotate the system so that point B is on the positive X axis.
  theCos = Bx / distAB;
  theSin = By / distAB;
  newX = Cx * theCos + Cy * theSin;
  Cy = Cy * theCos - Cx * theSin;
  Cx = newX;
  newX = Dx * theCos + Dy * theSin;
  Dy = Dy * theCos - Dx * theSin;
  Dx = newX;
  // Fail if segment C-D doesn't cross line A-B.
  if ((Cy < 0. && Dy < 0.) || (Cy >= 0. && Dy >= 0.)) {
    return NO;
  }
  // (3) Discover the position of the intersection point along line A-B.
  ABpos = Dx + (Cx - Dx) * Dy / (Dy - Cy);
  // Fail if segment C-D crosses line A-B outside of segment A-B.
  if (ABpos < 0. || ABpos > distAB) {
    return NO;
  }
  // (4) Apply the discovered position to line A-B in the original coordinate system.
  *X = Ax + ABpos * theCos;
  *Y = Ay + ABpos * theSin;
  // Success.
  return YES;
}
// Returns the perimeter polygon in newX and newY (which must have at least
// MAX_SEGS elements each to prevent the possibility of overrun). "corners" is
// used to pass in the size of the original polygon, and to return the size of
// the new, perimeter polygon.
//
// If for any reason the procedure fails, it will return NO in its BOOL return
// value, in which case the values in "newX", "newY", and "corners" should not
// be trusted.
CC_INLINE BOOL polygonPerimeter(double* x, double* y, int* corners, double* newX, double* newY)
{
  double segSx[MAX_SEGS], segSy[MAX_SEGS], segEx[MAX_SEGS], segEy[MAX_SEGS] ;
  double segAngle[MAX_SEGS], intersectX, intersectY ;
  double startX = x[0], startY = y[0], lastAngle = .5 * CIRCLE_RADIANS, turnAngle = 0 ;
  double a, b, c, d, e, f, angleDif, bestAngleDif ;
  int i, j = (*corners) - 1, segs = 0 ;
  if ((*corners) > MAX_SEGS) {
    return NO;
  }
  // 1,3. Reformulate the polygon as a set of line segments, and choose a
  // starting point that must be on the perimeter.
  for (i = 0; i < (*corners); i++) {
    if (x[i] != x[j] || y[i] != y[j]) {
      segSx[segs] = x[i];
      segSy[segs] = y[i];
      segEx[segs] = x[j];
      segEy[segs++] = y[j];
    }
    j = i;
    if (y[i] > startY || (y[i] == startY && x[i] < startX)) {
      startX = x[i];
      startY = y[i];
    }
  }
  if (segs == 0) {
    return NO;
  }
  // 2. Break the segments up at their intersection points.
  for (i = 0; i < segs - 1; i++) {
    for (j = i + 1; j < segs ; j++) {
      if (lineSegmentIntersection(
          segSx[i], segSy[i], segEx[i], segEy[i],
          segSx[j], segSy[j], segEx[j], segEy[j], &intersectX, &intersectY)) {
        if ((intersectX != segSx[i] || intersectY != segSy[i])
            && (intersectX != segEx[i] || intersectY != segEy[i])) {
          if (segs == MAX_SEGS) {
            return NO;
          }
          segSx[segs] = segSx[i] ;
          segSy[segs ] = segSy[i] ;
          segEx[segs] = intersectX;
          segEy[segs++] = intersectY;
          segSx[i ] = intersectX;
          segSy[i ] = intersectY;
        }
        if ((intersectX != segSx[j] || intersectY != segSy[j])
            && (intersectX != segEx[j] || intersectY != segEy[j])) {
          if (segs == MAX_SEGS) {
            return NO;
          }
          segSx[segs] = segSx[j] ;
          segSy[segs ] = segSy[j] ;
          segEx[segs] = intersectX;
          segEy[segs++] = intersectY;
          segSx[j ] = intersectX;
          segSy[j ] = intersectY;
        }
      }
    }
  }
  // Calculate the angle of each segment.
  for (i = 0; i < segs; i++) {
    segAngle[i] = angleOf(segEx[i] - segSx[i], segEy[i] - segSy[i]);
  }
  // 4. Build the perimeter polygon.
  c = startX;
  d = startY;
  a = c - 1.;
  b = d;
  newX[0] = c;
  newY[0] = d;
  *corners = 1;
  while (YES) {
    bestAngleDif = CIRCLE_RADIANS;
    for (i = 0; i < segs; i++) {
      if (segSx[i] == c && segSy[i] == d && (segEx[i] != a || segEy[i] != b)) {
        angleDif = lastAngle - segAngle[i];
        while (angleDif >= CIRCLE_RADIANS) {
          angleDif -= CIRCLE_RADIANS;
        }
        while (angleDif < 0) {
          angleDif += CIRCLE_RADIANS;
        }
        if (angleDif < bestAngleDif) {
          bestAngleDif = angleDif;
          e = segEx[i];
          f = segEy[i];
        }
      }
      if (segEx[i] == c && segEy[i] == d && (segSx[i] != a || segSy[i] != b)) {
        angleDif = lastAngle - segAngle[i] + .5 * CIRCLE_RADIANS;
        while (angleDif >= CIRCLE_RADIANS) {
          angleDif -= CIRCLE_RADIANS;
        }
        while (angleDif < 0) {
          angleDif += CIRCLE_RADIANS;
        }
        if (angleDif < bestAngleDif) {
          bestAngleDif = angleDif;
          e = segSx[i];
          f = segSy[i];
        }
      }
    }
    if ((*corners) > 1 && c == newX[0] && d == newY[0] && e == newX[1] && f == newY[1]) {
      (*corners)--;
      return YES;
    }
    if (bestAngleDif == CIRCLE_RADIANS || (*corners) == MAX_SEGS) {
      return NO;
    }
    newX[ *corners ] = e;
    lastAngle -= bestAngleDif + .5 * CIRCLE_RADIANS;
    newY[(*corners)++] = f;
    a = c, b = d, c = e, d = f;
  }
}
// Given the sequentially connected points (a,b), (c,d), and (e,f), this
// function returns, in (C,D), a bevel-inset replacement for point (c,d).
//
// Note: If vectors (a,b)->(c,d) and (c,d)->(e,f) are exactly 180�� opposed,
// or if either segment is zero-length, this function will do
// nothing; i.e. point (C,D) will not be set.
CC_INLINE void insetCorner(
    double a, double b, // previous point
    double c, double d, // current point that needs to be inset
    double e, double f, // next point
    double* C, double* D, // storage location for new, inset point
    double insetDist) // amount of inset (perpendicular to each line segment)
{
  double c1 = c, d1 = d, c2 = c, d2 = d, dx1, dy1, dist1, dx2, dy2, dist2, insetX, insetY ;
  // Calculate length of line segments.
  dx1 = c - a, dy1 = d - b;
  dist1 = sqrt(dx1 * dx1 + dy1 * dy1);
  dx2 = e - c, dy2 = f - d;
  dist2 = sqrt(dx2 * dx2 + dy2 * dy2);
  // Exit if either segment is zero-length.
  if (dist1 == 0. || dist2 == 0.) {
    return;
  }
  // Inset each of the two line segments.
  insetX = dy1 / dist1 * insetDist;
  a += insetX;
  c1 += insetX;
  insetY = -dx1 / dist1 * insetDist;
  b += insetY;
  d1 += insetY;
  insetX = dy2 / dist2 * insetDist;
  e += insetX;
  c2 += insetX;
  insetY = -dx2 / dist2 * insetDist;
  f += insetY;
  d2 += insetY;
  // If inset segments connect perfectly, return the connection point.
  if (c1 == c2 && d1 == d2) {
    *C = c1, *D = d1;
    return;
  }
  // Return the intersection point of the two inset segments (if any).
  if (lineIntersection(a, b, c1, d1, c2, d2, e, f, &insetX, &insetY)) {
    *C = insetX, *D = insetY;
  }
}
// public-domain code by Darel Rex Finley, 2007
// See diagrams at http://alienryderflex.com/polygon_inset
CC_INLINE void insetPolygon(double* x, double* y, int corners, double insetDist)
{
  double startX = x[0], startY = y[0], a, b, c, d, e, f ;
  int i ;
  // Polygon must have at least three corners to be inset.
  if (corners < 3) {
    return;
  }
  // Inset the polygon.
  c = x[corners - 1], d = y[corners - 1];
  e = x[0], f = y[0];
  for (i = 0; i < corners - 1; i++) {
    a = c, b = d, c = e, d = f;
    e = x[i + 1], f = y[i + 1];
    insetCorner(a, b, c, d, e, f, &x[i], &y[i], insetDist);
  }
  insetCorner(c, d, e, f, startX, startY, &x[i], &y[i], insetDist);
}
#undef NO
#undef YES
#undef SP_END
#undef SP_SPLINE
#undef SP_NEW_LOOP
// http://baike.baidu.com/link?url=JJswACC_uCnTpRrM15IM4iEY-VuEw3IxvXoA1GxgnNBdfys2NFg9aa2IRb5bN6MvVIP-0mkTpCSx7Uxs7b7Z3a
// �㵽ֱ�߾��� (x, y) (a*x+b*y+c==0)
CC_INLINE double dist_pt2line(double x, double y, double a, double b, double c)
{
  return fabs(a * x + b * y + c) / sqrt(a * a + b * b);
}
// ƽ����֮��ļ��
CC_INLINE double dist_line2line(double a, double b, double c1, double c2)
{
  return fabs(c1 - c2) / sqrt(a * a + b * b);
}
// ֱ�ߵļн�
CC_INLINE double dist_line_angle(double a1, double b1, double a2, double b2)
{
  return fabs((a1 * b2 - a2 * b1) / (a1 * a2 + b1 * b2));
}

//////////////////////////////////////////////////////////////////////////////////
typedef struct {
  FPOINT center;
  double r;
} circle_t;
static int double_equals(double const a, double const b)
{
  static const double ZERO = 1e-9;
  return fabs(a - b) < ZERO;
}
static double distance_sqr(FPOINT const* a, FPOINT const* b)
{
  return (a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y);
}
static double distance(FPOINT const* a, FPOINT const* b)
{
  return sqrt(distance_sqr(a, b));
}

static int circle_insect(circle_t circles[], FPOINT* points)
{
  double d, a, b, c, p, q, r;
  double cos_value[2], sin_value[2];
  if (double_equals(circles[0].center.x, circles[1].center.x)
      && double_equals(circles[0].center.y, circles[1].center.y)
      && double_equals(circles[0].r, circles[1].r)) {
    return -1;
  }
  d = distance(&circles[0].center, &circles[1].center);
  if (d > circles[0].r + circles[1].r
      || d < fabs(circles[0].r - circles[1].r)) {
    return 0;
  }
  a = 2.0 * circles[0].r * (circles[0].center.x - circles[1].center.x);
  b = 2.0 * circles[0].r * (circles[0].center.y - circles[1].center.y);
  c = circles[1].r * circles[1].r - circles[0].r * circles[0].r
      - distance_sqr(&circles[0].center, &circles[1].center);
  p = a * a + b * b;
  q = -2.0 * a * c;
  if (double_equals(d, circles[0].r + circles[1].r)
      || double_equals(d, fabs(circles[0].r - circles[1].r))) {
    cos_value[0] = -q / p / 2.0;
    sin_value[0] = sqrt(1 - cos_value[0] * cos_value[0]);
    points[0].x = circles[0].r * cos_value[0] + circles[0].center.x;
    points[0].y = circles[0].r * sin_value[0] + circles[0].center.y;
    if (!double_equals(distance_sqr(&points[0], &circles[1].center),
        circles[1].r * circles[1].r)) {
      points[0].y = circles[0].center.y - circles[0].r * sin_value[0];
    }
    return 1;
  }
  r = c * c - b * b;
  cos_value[0] = (sqrt(q * q - 4.0 * p * r) - q) / p / 2.0;
  cos_value[1] = (-sqrt(q * q - 4.0 * p * r) - q) / p / 2.0;
  sin_value[0] = sqrt(1 - cos_value[0] * cos_value[0]);
  sin_value[1] = sqrt(1 - cos_value[1] * cos_value[1]);
  points[0].x = circles[0].r * cos_value[0] + circles[0].center.x;
  points[1].x = circles[0].r * cos_value[1] + circles[0].center.x;
  points[0].y = circles[0].r * sin_value[0] + circles[0].center.y;
  points[1].y = circles[0].r * sin_value[1] + circles[0].center.y;
  if (!double_equals(distance_sqr(&points[0], &circles[1].center), circles[1].r * circles[1].r)) {
    points[0].y = circles[0].center.y - circles[0].r * sin_value[0];
  }
  if (!double_equals(distance_sqr(&points[1], &circles[1].center),
      circles[1].r * circles[1].r)) {
    points[1].y = circles[0].center.y - circles[0].r * sin_value[1];
  }
  if (double_equals(points[0].y, points[1].y) && double_equals(points[0].x, points[1].x)) {
    if (points[0].y > 0) {
      points[1].y = -points[1].y;
    }
    else {
      points[0].y = -points[0].y;
    }
  }
  return 2;
}
#ifdef TEST
static int test_circle_insect()
{
  circle_t circles[2];
  FPOINT points[2];
  printf("��������Բx��y���뾶(�Զ��ŷֿ�)��");
  while (EOF != scanf("%lf,%lf,%lf,%lf,%lf,%lf",
      &circles[0].center.x, &circles[0].center.y, &circles[0].r,
      &circles[1].center.x, &circles[1].center.y, &circles[1].r)) {
    switch (circle_insect(circles, points)) {
    case -1:
      printf("THE CIRCLES ARE THE SAME/n");
      break;
    case 0:
      printf("NO INTERSECTION/n");
      break;
    case 1:
      printf("(%.3lf %.3lf)\n", points[0].x, points[0].y);
      break;
    case 2:
      printf("(%.3lf %.3lf) (%.3lf %.3lf)\n",
          points[0].x, points[0].y,
          points[1].x, points[1].y);
    }
  }
  return 0;
}
#endif // TEST
#undef EQ
// �߶�ƽ��
static int fseg_offset(double x0, double y0, double x1, double y1, double width, FPOINT* pt) {
  double dx = x1 - x0, dy = y1 - y0;
  double len = sqrt(dx*dx + dy*dy);
  double cx = width*dy/len, cy = width*dx/len;
  pt[0].x = x0 + cx, pt[0].y = y0 - cy;
  pt[1].x = x1 + cx, pt[1].y = y1 - cy;
  return 0;
}
static FPOINT fseg_offset1(const FPOINT* p1, const FPOINT* p2, double width) {
  FPOINT pt[2] = {0};
  fseg_offset(p1->x, p1->y, p2->x, p2->y, width, pt);
  return *pt;
}
// ����ֱ�߽���
// ֱ�� 1 �ϵ����㣺(ax1, ay1), (ax2, ay2)
// ֱ�� 2 �ϵ����㣺(bx1, by1), (bx2, by2)
// ����ֵ��true: �н��㣬������ (ix, iy) ��
//    ��false: ��ֱ��ƽ��
static bool fseg_intersection(double ax1, double ay1, double ax2, double ay2, double bx1, double by1, double bx2, double by2, double *ix, double *iy)
{
  double d = (ay2-ay1) * (bx2-bx1) - (by2-by1) * (ax2-ax1);
  
  if (d==0) {
    return false; // ��ֱ��ƽ�о��˳����������Ϊ 0 �����
  }
  
  *ix = ((ax2-ax1)*(bx2-bx1)*(by1-ay1)+(ay2-ay1)*(bx2-bx1)*ax1-(by2-by1)*(ax2-ax1)*bx1) / d;
  *iy = ((ay2-ay1)*(by2-by1)*(bx1-ax1)+(ax2-ax1)*(by2-by1)*ay1-(bx2-bx1)*(ay2-ay1)*by1) / -d;
  
  return true;
}
static int fseg_joint(const FPOINT *p1, const FPOINT *p2, const FPOINT *p3, double distance, FPOINT* pt) {
  if ((p2->x - p1->x) * (p3->y - p1->y) > (p2->y - p1->y) * (p3->x - p1->x))
  {
    float length_0 = sqrtf((p2->x-p1->x)*(p2->x-p1->x)+(p2->y-p1->y)*(p2->y-p1->y));
    float length_1 = sqrtf((p3->x-p2->x)*(p3->x-p2->x)+(p3->y-p2->y)*(p3->y-p2->y));
    float dx0 = distance * (p2->x - p1->x) / length_0;
    float dy0 = distance * (p2->y - p1->y) / length_0;
    float dx1 = distance * (p3->x - p2->x) / length_1;
    float dy1 = distance * (p3->y - p2->y) / length_1;
    float det = (dy0*dx1 - dx0*dy1);
    float dx = (dx0*dx1*(dx0-dx1) + dy0*dy0*dx1 - dy1*dy1*dx0)/det;
    float dy = (dy0*dy1*(dy0-dy1) + dx0*dx0*dy1 - dx1*dx1*dy0)/det;
    if (dx*dx + dy*dy < distance*distance)
    {
    }
    pt->x = p2->x + dx;
    pt->y = p2->y + dy;
  }
  return 0;
}

static void bezier3_point2(double x1, double y1, double x2, double y2,
                   double x3, double y3, double x4, double y4, double mu,
                   double* x, double* y)
{
  double mum1, mum13, mu3;
  
  mum1 = 1 - mu;
  mum13 = mum1 * mum1 * mum1;
  mu3 = mu * mu * mu;
  
  *x = mum13*x1 + 3*mu*mum1*mum1*x2 + 3*mu*mu*mum1*x3 + mu3*x4;
  *y = mum13*y1 + 3*mu*mum1*mum1*y2 + 3*mu*mu*mum1*y3 + mu3*y4;
}

static void bezier3_point1(const FPOINT *p0, const FPOINT *p1, const FPOINT *p2, const FPOINT *p3, double t, FPOINT* pt)
{
  double x, y;
  bezier3_point2(p0->x, p0->y, p1->x, p1->y, p2->x, p2->y, p3->x, p3->y, t, &x, &y);
  pt->x = x;
  pt->y = y;
}
// <summary>
// ��ö��α�����������ĳ��ʱ�̵�λ��
// B(t) = P0(1-t)^2+ 2P1t(1-t) + P2t^2
// </summary>
// <param name="p0">��ʼ��</param>
// <param name="bs">���α���������</param>
// <param name="t">ʱ��t 0~1</param>
static FPOINT bezier2_point(const FPOINT* p0, const FPOINT* p1, const FPOINT* p2, double t) {
  FPOINT pr;
  double r = 1 - t;
  double rr = r*r, tr = t*r, tt = t*t;
  pr.x = p0->x * rr + 2 * p1->x * tr + p2->x * tt;
  pr.y = p0->y * rr + 2 * p1->y * tr + p2->y * tt;
  return pr;
}

// <summary>
// ��ö��α�����������ĳ��ʱ�̵��ݶ�
// </summary>
// <param name ="p0">��ʼ��</param>
// <param name="bs">���α���������</param>
// <param name="t">ʱ��t 0~1</param>
// <returns></returns>
// ����x���Ա���ʱ��t ��x����ı仯��
static FPOINT bezier2_angle(const FPOINT* p0, const FPOINT* p1, const FPOINT* p2, double t) {
  FPOINT dr;
  double r = 1 - t;
  dr.x = p0->x * 2 * r * -1 + 2 * p1->x * (r + (-1) * t) + p2->x * 2 * t;
  dr.y = p0->y * 2 * r * -1 + 2 * p1->y * (r + (-1) * t) + p2->y * 2 * t;
  return dr;
}

// <summary>
// ������α�����������ĳ��ʱ�̵�λ��
// b(t) = p0(1-t)^3 + 3p1t(1-t^2) + 3p2t^2(1-t) + p3t^3
// </summary>
// <param name="p0">��ʼ��</param>
// <param name="bs">���α���������</param>
// <param name="t">ʱ��t 0~1</param>
static FPOINT bezier3_point(const FPOINT* p0, const FPOINT* p1, const FPOINT* p2, const FPOINT* p3, double t) {
  FPOINT pr;
  double r = 1 - t;
  double rr = r*r, tt = t*t;
  double rrr = r*rr, trr3 = t*rr*3, ttr3 = r*tt*3, ttt=t*tt;
  pr.x = p0->x * rrr + p1->x * trr3 + p2->x * ttr3 + p3->x * ttt;
  pr.y = p0->y * rrr + p1->y * trr3 + p2->y * ttr3 + p3->y * ttt;
  return pr;
}

// <summary>
// ������α�����������ĳ��ʱ�̵����߷���
// </summary>
// <param name ="p0">��ʼ��</param>
// <param name="bs">���α���������</param>
// <param name="t">ʱ��t 0~1</param>
// <returns></returns>
static FPOINT bezier3_angle(const FPOINT* p0, const FPOINT* p1, const FPOINT* p2, const FPOINT* p3, double t) {
  FPOINT dr;
  double r = 1 - t;
  double rr = r*r, tt = t*t, tr = t*r;
  double rrr = r*rr, trr3 = t*rr*3, ttr3 = r*tt*3, ttt=t*tt;
  // ����x���Ա���ʱ��t ��x����ı仯��
  dr.x = p0->x * 3 * rr * (-1) + 3 * p1->x * (rr + 2 * tr * (-1)) + 3 * p2->x * (2 * tr + tt * (-1)) + p3->x * 3 * tt;
  dr.y = p0->y * 3 * rr * (-1) + 3 * p1->y * (rr + 2 * tr * (-1)) + 3 * p2->y * (2 * tr + tt * (-1)) + p3->y * 3 * tt;
  return dr;
}

// [x, y] = solve('c1=a1*x + b1*y', 'c2=a2*x + b2*y')
//x = (-c2*b1+b2*c1)/(-a2*b1+a1*b2)
//y = -(a2*c1-a1*c2)/(-a2*b1+a1*b2)
static int fsolve2(double a1, double b1, double c1, double a2, double b2, double c2, float* x, float* y) {
  double d = (-a2*b1+a1*b2);
  *x = (-c2*b1+b2*c1)/d;
  *y = -(a2*c1-a1*c2)/d;
  return d!=0;
}
// solve('P1=(1-t1)^3*A + 3*t1*((1-t1)^2)*x + 3*(t1^2)*(1-t1)*y + t^3*D', 'P2=(1-t2)^3*A + 3*t2*((1-t2)^2)*x + 3*(t2^2)*(1-t2)*y + t2^3*D')
// a1 = 3*t1*((1-t1)^2)
// b1 = 3*(t1^2)*(1-t1)
// c1 = P1 - (1-t1)^3*A - t^3*D;
  //FPOINT pt3[4];
static int bezier3_offset(const FPOINT *a, const FPOINT *b, const FPOINT *c, const FPOINT *d, double distance, FPOINT* pt) {
  double t1 = 1./3, t2 = 2./3;
  FPOINT p1, p2, a1, a2;
  FPOINT ca, cb, cc, cd;
  p1 = bezier3_point(a,b,c,d,t1);
  a1 = bezier3_angle(a,b,c,d,t1);
  p2 = bezier3_point(a,b,c,d,t2);
  a2 = bezier3_angle(a,b,c,d,t2);
  ca = fseg_offset1(a, b, distance);
  cd = fseg_offset1(d, c, -distance);
  cb = fseg_offset1(&p1, &a1, distance);
  cc = fseg_offset1(&p2, &a2, distance);
  //pt3[0] = ca, pt3[1] = cb, pt3[2] = cc, pt3[3] = cd;
  {
    double r1 = (1-t1), r2 = 1-t2;
    fsolve2(3*t1*r1*r1, 3*t1*t1*r1, cb.x - r1*r1*r1*ca.x - t1*t1*t1*cd.x, 3*t2*r2*r2, 3*t2*t2*r2, cc.x - r2*r2*r2*ca.x - t2*t2*t2*cd.x, &pt[0].x, &pt[1].x);
    fsolve2(3*t1*r1*r1, 3*t1*t1*r1, cb.y - r1*r1*r1*ca.y - t1*t1*t1*cd.y, 3*t2*r2*r2, 3*t2*t2*r2, cc.y - r2*r2*r2*ca.y - t2*t2*t2*cd.y, &pt[0].y, &pt[1].y);
  }
  //fsolve2();
  return 0;
}
static int fpoly_offset(int n, const FPOINT *p1, bool closed, double distance, FPOINT* pt) {
  int i;
  if (n<=1) {
    return 0;
  }
  if (!closed) {
    fseg_offset(p1[0].x, p1[0].y, p1[1].x, p1[1].y, distance, pt);
    fseg_offset(p1[n-2].x, p1[n-2].y, p1[n-1].x, p1[n-1].y, distance, pt+n-2);
    for (i=1; i<n-1; ++i) {
      fseg_joint(p1+i-1, p1+i, p1+i+1, distance, pt+i);
    }
  } else {
    fseg_joint(p1+n-1, p1, p1+1, distance, pt);
    fseg_joint(p1+n-2, p1+n-1, p1, distance, pt+n-1);
    for (i=1; i<n-1; ++i) {
      fseg_joint(p1+i-1, p1+i, p1+i+1, distance, pt+i);
    }
  }
  return 0;
}

static int path_offset1(const FPOINT* Points, const BYTE* Types, int Count, bool closed, double distance, FPOINT* pt2) {
  int i, j, k, l, t;
  if (closed) {
    for (i=0; i<Count; ++i) {
      j = (i + Count - 1)%Count;
      k = (i + 1)%Count;
      l = (i + 2)%Count;
      t = Types[i]&PathPointTypePathTypeMask;
      switch (t) {
      case PathPointTypeStart:
      case PathPointTypeLine:
      case PathPointTypeBezier2:
        fseg_joint(Points+j, Points+i, Points+k, distance, pt2+i);
        break;
      case PathPointTypeBezier3:
        bezier3_offset(Points+j, Points+i, Points+k, Points+l, distance, pt2+i);
        ++i;
        break;
      }
    }
  } else {
    int n = Count;
    fseg_offset(Points[0].x, Points[0].y, Points[1].x, Points[1].y, distance, pt2);
    fseg_offset(Points[n-2].x, Points[n-2].y, Points[n-1].x, Points[n-1].y, distance, pt2+n-2);
    for (i=1; i<Count-1; ++i) {
      j = (i + Count - 1)%Count;
      k = (i + 1)%Count;
      l = (i + 2)%Count;
      t = Types[i]&PathPointTypePathTypeMask;
      switch (t) {
      case PathPointTypeStart:
      case PathPointTypeLine:
      case PathPointTypeBezier2:
        fseg_joint(Points+j, Points+i, Points+k, distance, pt2+i);
        break;
      case PathPointTypeBezier3:
        bezier3_offset(Points+j, Points+i, Points+k, Points+l, distance, pt2+i);
        ++i;
        break;
      }
    }
  }
  return 0;
}

//------------------------------------------------------------------------
static void trans_warp_magnifier(FPOINT* pt, int n, double m_xc, double m_yc, double m_radius, double m_magn, int inv)
{
  int i;
  if (inv) {
    // New version by Andrew Skalkin
    for (i=0; i<n; ++i) {
      double x = pt[i].x, y = pt[i].y;
      double dx = x - m_xc;
      double dy = y - m_yc;
      double r = sqrt(dx * dx + dy * dy);
      
      if(r < m_radius * m_magn) 
      {
        pt[i].x = m_xc + dx / m_magn;
        pt[i].y = m_yc + dy / m_magn;
      }
      else
      {
        double rnew = r - m_radius * (m_magn - 1.0);
        pt[i].x = m_xc + rnew * dx / r; 
        pt[i].y = m_yc + rnew * dy / r;
      }
    }
  } else {
    for (i=0; i<n; ++i) {
      double x = pt[i].x, y = pt[i].y;
      double dx = x - m_xc;
      double dy = y - m_yc;
      double r = sqrt(dx * dx + dy * dy);
      if(r < m_radius)
      {
        pt[i].x = m_xc + dx * m_magn;
        pt[i].y = m_yc + dy * m_magn;
      } else {
        double m = (r + m_radius * (m_magn - 1.0)) / r;
        pt[i].x = m_xc + dx * m;
        pt[i].y = m_yc + dy * m;
      }
    }
  }
}

#endif // _GEO_INL_

