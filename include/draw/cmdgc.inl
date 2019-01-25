
/////////////////////////////////////////////////////////////////////////////////
//#define countof(arr) (sizeof(arr)/sizeof(*arr))
#define VA_ARGN(T, N) ((T*)((va+=sizeof(T)*(N))-sizeof(T)*(N)))
#define VA_ARG(T) (*(T*)((va+=sizeof(T))-sizeof(T)))
#define VA_ARG_EQ(T, A) (VA_ARG(T) = A)
#define VA_ARG_EQN(T, A, N) memcpy(VA_ARGN(T), A, sizeof(T)*(N) )
//#define VA_ARG_EQ2(T, ap) (VA_ARG(T) = va_arg(ap, T))
#define VA_BEG(buflen, cmd) {char buf[buflen], *va = buf; VA_ARG_EQ(int, cmd);
#define VA_END() ASSERT((va-buf)<=countof(buf)); {char* p=(char*)myAllocPush(g->buf, g->len, va-buf);memcpy(p, buf, va-buf);}}
#define VA_BEG1(n1, cmd) {int _n = n1; char* buf=(char*)myAllocPush(g->buf, g->len, _n); char* va = buf; VA_ARG_EQ(int, cmd);
#define VA_END1() ASSERT((va-buf)<=_n); g->len += (va-buf)-_n; }
//x0 ���俪ʼ��� x ����
//y0 ���俪ʼ��� y ����
//x1 ���俪ʼ��� x ����
//y1 ���������� y ����
typedef int ImDrawStyle;
//ָʾ��ͼ���ɫ�� ��ɫֵ��Ĭ��ֵ�� #000000
//#define gcSolidBrush(g, clr) gcSolidBrush(g, 1, clr)
//#define gcSolidBrush(g, clr) gcSolidBrush(g, 1, clr)
CC_INLINE int cmdSolidBrush(gc_t* g, bool fill, COLOR clr)
{
  VA_BEG(16, ImDrawCmd_Solid);
  VA_ARG_EQ(int, fill);
  VA_ARG_EQ(COLOR, clr);
  VA_END();
  return GetAV(clr);
}
CC_INLINE int cmdHatchBrush(gc_t* g, bool fill, HatchStyle hatchstyle, COLOR forecol, COLOR backcol, int x, int y) {
  VA_BEG(32, ImDrawCmd_Hatch);
  VA_ARG_EQ(int, fill);
  VA_ARG_EQ(int, x);
  VA_ARG_EQ(int, y);
  VA_ARG_EQ(COLOR, forecol);
  VA_ARG_EQ(COLOR, backcol);
  VA_ARG_EQ(int, hatchstyle);
  VA_END();
  return GetAV(forecol || backcol);
}
//��������ͼ�Ľ���������Ի�����ԣ�
CC_INLINE BOOL cmdGradientBrush(gc_t* g, bool fill, float x0, float y0, float x1, float y1, const float* blendPositions, const COLOR* presetColors, int count)
{
  int i, ret = 0;
  if (0 == count) {
    return 0;
  }
  else if (1 == count) {
    cmdSolidBrush(g, fill, presetColors[0]);
  }
  else {
    VA_BEG(256, ImDrawCmd_Gradient);
    VA_ARG_EQ(int, fill);
    VA_ARG_EQ(float, x0);
    VA_ARG_EQ(float, y0);
    VA_ARG_EQ(float, x1);
    VA_ARG_EQ(float, y1);
    VA_ARG_EQ(int, count);
    for (i = 0; i < count; ++i) {
      float pos = blendPositions ? blendPositions[i] : (i * 1.f / (count - 1));
      VA_ARG_EQ(float, pos);
      VA_ARG_EQ(COLOR, presetColors[i]);
      ret += GetAV(presetColors[i]);
    }
    VA_END();
  }
  return ret;
}
#define gcGradientBrushColors(g, x0, y0, x1, y1, blendPositions, presetColors, count) gcGradientBrush(g, x0, y0, x1, y1, blendPositions, presetColors, count)

void gcGradientBrushS(gc_t* g, float x0, float y0, float x1, float y1, COLOR startColor, COLOR endColor) {
  float blendPositions[] = {0.f, 1.f};
  COLOR presetColors[] = {startColor, endColor};
  gcGradientBrush(g, x0, y0, x1, y1, blendPositions, presetColors, 2);
}
FPOINT GetGradientBrushRectPt(float x, float y, float w, float h, LinearGradientMode mode) {
  FPOINT pt = {x+w, y+h};
  switch (mode) {
  case LinearGradientModeHorizontal:
    pt = fPOINT(x+w, y);
    break;
  case LinearGradientModeVertical:
    pt = fPOINT(x, y+h);
    break;
  case LinearGradientModeForwardDiagonal:
    pt = GetFootOfPerpendicular(fPOINT(x, y), fPOINT(x, y+2*h), fPOINT(x+2*w, y));
    break;
  case LinearGradientModeBackwardDiagonal:
    x += w;
    pt = GetFootOfPerpendicular(fPOINT(x, y), fPOINT(x, y+2*h), fPOINT(x-2*w, y));
    break;
  }
  return pt;
}
#define gcGradientBrushRectS2(g, rc, startColor, endColor, mode)  gcGradientBrushRectS(g, rc.l, rc.t, RCW(&rc), RCH(&rc), startColor, endColor, mode)
void gcGradientBrushRectS(gc_t* g, float x, float y, float w, float h, COLOR startColor, COLOR endColor, LinearGradientMode mode) {
  FPOINT pt = GetGradientBrushRectPt(x, y, w, h, mode);
  gcGradientBrushS(g, x, y, pt.x, pt.y, startColor, endColor);
}
#define gcGradientBrushRect2(g, rc, pos, clrs, count, mode)  gcGradientBrushRect1(g, rc.l, rc.t, RCW(&rc), RCH(&rc), pos, clrs, count, mode)
void gcGradientBrushRect1(gc_t* g, float x, float y, float w, float h, const float* pos, const COLOR* clrs, int count, LinearGradientMode mode) {
  FPOINT pt = GetGradientBrushRectPt(x, y, w, h, mode);
  gcGradientBrush(g, x, y, pt.x, pt.y, pos, clrs, count);
}
void gcRadialBrushS(gc_t* g, float x, float y, float rx, float ry, COLOR startColor, COLOR endColor) {
  float blendPositions[] = {0.f, 1.f};
  COLOR presetColors[] = {startColor, endColor};
  gcRadialBrush(g, x, y, rx, ry, blendPositions, presetColors, 2);
}
void gcRadialBrushRectS(gc_t* g, float x, float y, float w, float h, COLOR startColor, COLOR endColor) {
  float w2 = w/2, h2=h/2;
  gcRadialBrushS(g, x+w2, y+h2, w2, h2, startColor, endColor);
}
void gcRadialBrushRectS2(gc_t* g, IRECT rc, COLOR startColor, COLOR endColor) {
  gcRadialBrushRectS(g, rc.l, rc.t, RCW(&rc), RCH(&rc), startColor, endColor);
}
void gcRadialBrushRect(gc_t* g, float x, float y, float w, float h, const float* blendPositions, const COLOR* presetColors, int count) {
  float w2 = w/2, h2=h/2;
  gcRadialBrush(g, x+w2, y+h2, w2, h2, blendPositions, presetColors, count);
}
void gcRadialBrushRect2(gc_t* g, IRECT rc, const float* blendPositions, const COLOR* presetColors, int count) {
  gcRadialBrushRect(g, rc.l, rc.t, RCW(&rc), RCH(&rc), blendPositions, presetColors, count);
}
void gcGradientBrushColorsRect(gc_t* g, float x, float y, float w, float h, LinearGradientMode mode, const float* blendPositions, const COLOR* presetColors, int count) {
  FPOINT pt = GetGradientBrushRectPt(x, y, w, h, mode);
  gcGradientBrushColors(g, x, y, pt.x, pt.y, blendPositions, presetColors, count);
}
#define gcGradientBrushRectAngle2(g, rc, startColor, endColor, angle)  gcGradientBrushRectAngle(g, rc.l, rc.t, RCW(&rc), RCH(&rc), startColor, endColor, angle, false)
void gcGradientBrushRectAngle(gc_t* g, float x, float y, float w, float h, COLOR startColor, COLOR endColor, float angle, bool isAngleScalable) {
  float width, height;
  float sin_angle = sinf(deg2rad(angle));
  float cos_angle = cosf(deg2rad(angle));
  float sin_cos_angle = sin_angle * cos_angle;
  float exofs, eyofs, x2, y2;
  
  if (isAngleScalable) {
    width = height = 1.0;
  } else {
    width = w;
    height = h;
  }
  
  
  if (sin_cos_angle >= 0) {
    exofs = width * sin_cos_angle + height * cos_angle * cos_angle;
    eyofs = width * sin_angle * sin_angle + height * sin_cos_angle;
  } else {
    exofs = width * cos_angle * cos_angle + height * sin_cos_angle;
    eyofs = -width * sin_cos_angle + height * sin_angle * sin_angle;
  }
  
  if (isAngleScalable) {
    exofs = exofs * w;
    eyofs = eyofs * h;
  }
  
  if (sin_angle >= 0) {
    x2 = x + exofs;
    y2 = y + eyofs;
  } else {
    x2 = x;
    y2 = y;
    x = x + exofs;
    x = x + eyofs;
  }
  
  gcGradientBrushS(g, x, y, x2, y2, startColor, endColor);
}
void gcPathRadialBrush3(gc_t* g, const Path* path, int nclrs, const COLOR* clrs, const float* pos, const FPOINT* ptcenter) {
  COLOR clrs1[256];
  fill_color_array2(256, clrs1, nclrs, clrs, pos);
  gcPathRadialBrush2(g, path, clrs1, ptcenter);
}
void gcPathRadialBrush4(gc_t* g, const Path* path, COLOR clr1, COLOR clr2, const FPOINT* ptcenter) {
  COLOR clrs[256];
  fill_color_array(256, clrs, clr1, clr2);
  gcPathRadialBrush2(g, path, clrs, ptcenter);
}

// Unix stuff
// Code from http://www.johndcook.com/blog/2009/01/19/stand-alone-error-function-erf/
CC_INLINE double erf1(double x)
{
    const float a1 =  0.254829592;
    const float a2 = -0.284496736;
    const float a3 =  1.421413741;
    const float a4 = -1.453152027;
    const float a5 =  1.061405429;
    const float p  =  0.3275911;
    float t, y, sign;

    /* Save the sign of x */
    sign = 1;
    if (x < 0)
        sign = -1;
    x = fabs(x);

    /* A & S 7.1.26 */
    t = 1.0/(1.0 + p*x);
    y = 1.0 - (((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*exp(-x*x);

    return sign*y;
}
//#include "brush2.inl"
#if 0
int gcGradientBrushSigmaBlend(gc_t* g, float x1, float y1, float x2, float y2, float focus, float scale)
{
    float factors[33];
    float positions[33];
    int num_points = 0;
    int i;
    const int precision = 16;
    float erf_range; /* we use values erf(-erf_range) through erf(+erf_range) */
    float min_erf;
    float scale_erf;

    TRACE("(%p, %0.2f, %0.2f)\n", clr, focus, scale);

    if(!clr || focus < 0.0 || focus > 1.0 || scale < 0.0 || scale > 1.0)
        return InvalidParameter;

    /* we want 2 standard deviations */
    erf_range = 2.0 / sqrt(2);

    /* calculate the constants we need to normalize the error function to be
        between 0.0 and scale over the range we need */
    min_erf = erf(-erf_range);
    scale_erf = scale / (-2.0 * min_erf);

    if (focus != 0.0)
    {
        positions[0] = 0.0;
        factors[0] = 0.0;
        for (i=1; i<precision; i++)
        {
            positions[i] = focus * i / precision;
            factors[i] = scale_erf * (erf(2 * erf_range * i / precision - erf_range) - min_erf);
        }
        num_points += precision;
    }

    positions[num_points] = focus;
    factors[num_points] = scale;
    num_points += 1;

    if (focus != 1.0)
    {
        for (i=1; i<precision; i++)
        {
            positions[i+num_points-1] = (focus + ((1.0-focus) * i / precision));
            factors[i+num_points-1] = scale_erf * (erf(erf_range - 2 * erf_range * i / precision) - min_erf);
        }
        num_points += precision;
        positions[num_points-1] = 1.0;
        factors[num_points-1] = 0.0;
    }

   return gcGradientBrush(line, factors, positions, num_points);
}
#endif

//��������ͼ�� pattern ����
//img_id �涨Ҫʹ�õ�ͼƬ����������ƵԪ��
//repeat Ĭ�ϡ���ģʽ��ˮƽ�ʹ�ֱ�����ظ�
//repeat-x ��ģʽֻ��ˮƽ�����ظ�
//repeat-y ��ģʽֻ�ڴ�ֱ�����ظ�
//no-repeat ��ģʽֻ��ʾһ�Σ����ظ���
CC_INLINE BOOL cmdPatternBrush(gc_t* g, bool fill, const texture_t* tex, const IRECT* prcSrc, COLOR bgclr, const COLOR* palette, const float* m23)
{
  VA_BEG(256, ImDrawCmd_Pattern);
  VA_ARG_EQ(int, fill);
  VA_ARG_EQ(COLOR, bgclr);
  VA_ARG_EQ(texture_t, *tex);
  VA_ARG_EQ(const IRECT*, prcSrc);
  VA_ARG_EQ(const COLOR*, palette);
  VA_ARG_EQ(const float*, m23);
  VA_END();
  return TRUE;
}
CC_INLINE void cmdLineDash(gc_t* g, float dashPhase, const float* dashPattern, int numdash)
{
  int i;
  VA_BEG(256, ImDrawCmd_LineDash);
  VA_ARG_EQ(int, numdash);
  for (i = 0; i < numdash; ++i) {
    VA_ARG_EQ(COLOR, dashPattern[i]);
  }
  VA_END();
}
CC_INLINE void cmdWrapMode(gc_t* g, WrapMode wrapX, WrapMode wrapY)
{
  VA_BEG(16, ImDrawCmd_WrapMode);
  VA_ARG_EQ(int, wrapX);
  VA_ARG_EQ(int, wrapY);
  VA_END();
}
CC_INLINE void cmdLineWidth(gc_t* g, float lineWidth)
{
  VA_BEG(16, ImDrawCmd_LineWidth);
  VA_ARG_EQ(float, lineWidth);
  VA_END();
}
CC_INLINE void cmdPenAlignment(gc_t* g, PenAlignment penAlignment)
{
  VA_BEG(16, ImDrawCmd_PenAlignment);
  VA_ARG_EQ(int, penAlignment);
  VA_END();
}
CC_INLINE void cmdInnerJoin(gc_t* g, InnerJoin innerJoin)
{
  VA_BEG(16, ImDrawCmd_InnerJoin);
  VA_ARG_EQ(int, innerJoin);
  VA_END();
}
CC_INLINE void cmdLineJoin(gc_t* g, LineJoin lineJoin)
{
  VA_BEG(16, ImDrawCmd_LineJoin);
  VA_ARG_EQ(int, lineJoin);
  VA_END();
}
CC_INLINE void cmdLineCap(gc_t* g, LineCap startCap, LineCap endCap)
{
  VA_BEG(16, ImDrawCmd_LineCap);
  VA_ARG_EQ(int, startCap);
  VA_ARG_EQ(int, endCap);
  VA_END();
}
CC_INLINE void cmdMiterLimit(gc_t* g, float miterLimit)
{
  VA_BEG(16, ImDrawCmd_MiterLimit);
  VA_ARG_EQ(float, miterLimit);
  VA_END();
}
CC_INLINE void cmdRect(gc_t* g, float x, float y, float w, float h)
{
  VA_BEG(32, ImDrawCmd_Rect);
  VA_ARG_EQ(float, x);
  VA_ARG_EQ(float, y);
  VA_ARG_EQ(float, w);
  VA_ARG_EQ(float, h);
  VA_END();
}
#define gcClipRectR(g, rc) gcPushClipRect(g, &rc)
#define gcSetClip(g, rc) gcClipRectR(g, rc)
#define gcClipReset(g) gcPopClipRect(g)
CC_INLINE void cmdPushClipRect(gc_t* g, float x, float y, float w, float h)
{
  VA_BEG(32, ImDrawCmd_ClipRect);
  VA_ARG_EQ(float, x);
  VA_ARG_EQ(float, y);
  VA_ARG_EQ(float, w);
  VA_ARG_EQ(float, h);
  VA_END();
}
CC_INLINE void cmdTextAlign(gc_t* g, int textAlign)
{
  VA_BEG(16, ImDrawCmd_TextAlign);
  VA_ARG_EQ(int, textAlign);
  VA_END();
}

#define gcFont(font, a, b, c, d, e, f)
#define gcTextOut(g, x, y, text, font) gcText(g, 1, text, NULL, font, 0, x, y, 1000, 1000, 0)

CC_INLINE void gcTextBaseline(gc_t* g, const char* str)
{
  ASSERT(0);
}
#define gcFillText(g, text, text_end, font_id, formatFlags, rc) gcText(g, 1, text, text_end, font_id, (rc)->l, (rc)->t, RCW(rc), RCH(rc), formatFlags)
#define gcStrokeText(g, text, text_end, font_id, formatFlags, rc) gcText(g, 0, text, text_end, font_id, (rc).l, (rc).t, RCW(rc), RCH(rc), formatFlags)
CC_INLINE void cmdText(gc_t* g, int opt, const char* text, const char* text_end, font_t* font, float size, float x, float y, float w, float h, int formatFlags)
{
  int textlen = text_end ? (text_end - text) : strlen(text);
  VA_BEG1(8 + 3 * sizeof(int) + textlen + 5 * sizeof(float), ImDrawCmd_Text);
  VA_ARG_EQ(int, opt);
  VA_ARG_EQ(int, textlen);
  VA_ARG_EQ(font_t*, font);
  VA_ARG_EQ(float, size);
  VA_ARG_EQ(float, x);
  VA_ARG_EQ(float, y);
  VA_ARG_EQ(float, w);
  VA_ARG_EQ(float, h);
  VA_ARG_EQ(int, formatFlags);
  memcpy(va, text, textlen);
  va += textlen;
  VA_END1();
}
CC_INLINE void gcSolidText(gc_t* g, IRECT rc, const char* text, const char* text_end, font_t* font, float size, int formatFlags, COLOR clrText)
{
  gcSolidBrush(g, clrText);
  gcText(g, 1, text, text_end, font, size, (float)(rc).l, (float)(rc).t, (float)RCW(&rc), (float)RCH(&rc), formatFlags);
}
CC_INLINE void gcSolidFormatTextV(gc_t* g, IRECT rc, font_t* font, float size, int formatFlags, COLOR clrText, const char* fmt, va_list arglist) {
  char* buf =  cstr_vsprintf_dup(fmt, arglist);
  gcSolidText(g, rc, buf, NULL, font, size, formatFlags, clrText);
  pfree(buf);
}
CC_INLINE void gcSolidFormatText(gc_t* g, IRECT rc, font_t* font, float size, int formatFlags, COLOR clrText, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  gcSolidFormatTextV(g, rc, font, size, formatFlags, clrText, fmt, args);
  va_end(args);
}
#if 0
typedef struct {
  drawImageMode_dstXY,
  drawImageMode_dstRect,
  drawImageMode_dstRectsrcRect,
} drawImageMode_;
// ͼ�����
CC_INLINE void gcDrawImage_(gc_t* g, int mode, const texture_t* tex, float dstx, float dsty, float dstw, float dsth, float srcx, float srcy, float srcw, float srch)
{
  PixelFormat fmt[8] = {0};
  fmt[1] = PixelFormat8bppIndexed;
  fmt[3] = PixelFormat24bppRGB;
  fmt[4] = PixelFormat32bppARGB;
  frame %= MAX(img->f, 1);
  Bitmap* bmp = new Bitmap(img->w, img->h, img->s, fmt[img->c], img->tt.data + frame * img->s * img->h);
  if (sz == NULL) {
    g->myg->DrawImage(bmp, x, y);
  }
  else {
    if (NULL == rcimg) {
      g->myg->DrawImage(bmp, x, y, (float)sz->w, (float)sz->h);
    }
    else {
      g->myg->DrawImage(bmp, RectF(x, y, (float)sz->w, (float)sz->h), rcimg->l, rcimg->t, RCW(rcimg), RCH(rcimg), UnitPixel);
    }
  }
  delete bmp;
}
#endif
CC_INLINE void cmdPath(gc_t* g, int opt, const FPOINT* Points, const BYTE* Types, int Count) {
  FPOINT* pt1;
  BYTE* by1;
  VA_BEG1(4 * 5 + Count * sizeof(FPOINT) + Count * sizeof(BYTE), ImDrawCmd_Path);
  VA_ARG_EQ(int, opt);
  VA_ARG_EQ(int, Count);
  pt1 = VA_ARGN(FPOINT, Count);
  by1 = VA_ARGN(BYTE, Count);
  MEMCPY(pt1, Points, Count);
  MEMCPY(by1, Types, Count);
  VA_END1();
}

CC_INLINE void cmdPrimitive(gc_t* g, info3d_t* info) {
  VA_BEG(4 + sizeof(info3d_t), ImDrawCmd_Primitive);
  VA_ARG_EQ(info3d_t*, info);
  VA_END();
}
CC_INLINE void cmdQuadImage(gc_t* g, const ImVertex* vtx4, const texture_t* tex)
{
  VA_BEG(8 + sizeof(ImVertex) * 4 + sizeof(texture_t), ImDrawCmd_QuadImage);
  VA_ARG_EQ(texture_t, *tex);
  VA_ARG_EQ(ImVertex, vtx4[0]);
  VA_ARG_EQ(ImVertex, vtx4[1]);
  VA_ARG_EQ(ImVertex, vtx4[2]);
  VA_ARG_EQ(ImVertex, vtx4[3]);
  VA_END();
}
CC_INLINE void cmdRectImage(gc_t* g, const ImVertex* vtx2, const texture_t* tex)
{
  VA_BEG(8 + sizeof(ImVertex) * 4 + sizeof(texture_t), ImDrawCmd_RectImage);
  VA_ARG_EQ(texture_t, *tex);
  VA_ARG_EQ(ImVertex, vtx2[0]);
  VA_ARG_EQ(ImVertex, vtx2[1]);
  VA_END();
}
#define SETPT(PT, X, Y)  ((PT).x = X, (PT).y = Y)
CC_INLINE void gcRectImageR(gc_t* g, IRECT rcDst, const texture_t* tex, const IRECT* rcSrc)
{
  if (tex && (tex->h>0 && tex->w>0)) {
    ImVertex vtx[4] = {0};
    IRECT rcd = rcDst;
    FRECT rcs = rcSrc ? fRECT(rcSrc->l * 1.f / tex->w, rcSrc->t * 1.f / tex->h, rcSrc->r * 1.f / tex->w, rcSrc->b * 1.f / tex->h) : fRECT(0.f, 0.f, 1.f, 1.f);
    SETPT(vtx[0].pos, (float)rcd.l, (float)rcd.t);
    SETPT(vtx[1].pos, (float)rcd.r, (float)rcd.b);
    vtx[0].uv = fVec2((float)rcs.l, (float)rcs.t);
    vtx[1].uv = fVec2((float)rcs.r, (float)rcs.b);
    gcRectImage(g, vtx, tex);
  }
}
CC_INLINE void gcRectImageRIM(gc_t* g, IRECT rcDst, const img_t* im, const IRECT* rcSrc) {
  texture_t tex[1] = {0};
  BMPINITIM(tex, im);
  tex->fmt = cn2PixFmt(im->c);
  gcRectImageR(g, rcDst, tex, rcSrc);
}
CC_INLINE void cmdPrimRectUV(gc_t* g, const PrimRectUV* pr, int npr, texture_t* tex)
{
  int sz = 1 + npr * sizeof(PrimRectUV) + 2 * sizeof(int);
  VA_BEG1(sz, ImDrawCmd_PrimRectUV);
  VA_ARG_EQ(int, npr);
  VA_ARG_EQ(texture_t*, tex);
  memcpy(va, pr, npr * sizeof(PrimRectUV));
  va += npr * sizeof(PrimRectUV);
  VA_END1();
}
