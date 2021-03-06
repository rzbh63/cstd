// Copyright (C) 2002-2012 Nikolaus Gebhardt / Thomas Alten
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
//#include "IBurningShader.h"
#ifdef _IRR_COMPILE_WITH_BURNINGSVIDEO_
// compile flag for this file
#undef USE_ZBUFFER
#undef IPOL_Z
#undef CMP_Z
#undef WRITE_Z
#undef IPOL_W
#undef CMP_W
#undef WRITE_W
#undef SUBTEXEL
#undef INVERSE_W
#undef IPOL_C0
#undef IPOL_T0
#undef IPOL_T1
#undef IPOL_T2
#undef IPOL_L0
// define render case
#define SUBTEXEL
#define INVERSE_W
#define USE_ZBUFFER
#define IPOL_W
#define CMP_W
#define WRITE_W
#define IPOL_C0
#define IPOL_T0
#define IPOL_T1
#define IPOL_L0
// apply global override
#ifndef SOFTWARE_DRIVER_2_PERSPECTIVE_CORRECT
#undef INVERSE_W
#endif
#ifndef SOFTWARE_DRIVER_2_SUBTEXEL
#undef SUBTEXEL
#endif
#ifndef SOFTWARE_DRIVER_2_USE_VERTEX_COLOR
#undef IPOL_C0
#endif
#if !defined ( SOFTWARE_DRIVER_2_USE_WBUFFER ) && defined ( USE_ZBUFFER )
#ifndef SOFTWARE_DRIVER_2_PERSPECTIVE_CORRECT
#undef IPOL_W
#endif
#define IPOL_Z
#ifdef CMP_W
#undef CMP_W
#define CMP_Z
#endif
#ifdef WRITE_W
#undef WRITE_W
#define WRITE_Z
#endif
#endif
class CTRNormalMap : public IBurningShader
{
public:
  //! constructor
  CTRNormalMap(CBurningVideoDriver* driver);
  //! draws an indexed triangle list
  void drawTriangle(const s4DVertex* a, const s4DVertex* b, const s4DVertex* c);
private:
  void scanline_bilinear(s);
  sScanConvertData scan[1] = {0};
  sScanLineData line;
};
//! constructor
CTRNormalMap_CTRNormalMap(CBurningVideoDriver* driver)
  : IBurningShader(driver)
{
#ifdef _DEBUG
  setDebugName("CTRNormalMap");
#endif
}
/*!
*/
void CTRNormalMap_scanline_bilinear(IBurningShader* s)
{
  tVideoSample* dst;
#ifdef USE_ZBUFFER
  fp24* z;
#endif
  s32 xStart;
  s32 xEnd;
  s32 dx;
#ifdef SUBTEXEL
  f32 subPixel;
#endif
#ifdef IPOL_Z
  f32 slopeZ;
#endif
#ifdef IPOL_W
  fp24 slopeW;
#endif
#ifdef IPOL_C0
  sVec4 slopeC[MATERIAL_MAX_COLORS];
#endif
#ifdef IPOL_T0
  sVec2 slopeT[BURNING_MATERIAL_MAX_TEXTURES];
#endif
#ifdef IPOL_L0
  sVec3 slopeL[BURNING_MATERIAL_MAX_TANGENT];
#endif
  // apply top-left fill-convention, left
  xStart = ceil32(line->x[0]);
  xEnd = ceil32(line->x[1]) - 1;
  dx = xEnd - xStart;
  if (dx < 0) {
    return;
  }
  // slopes
  f32 invDeltaX;
  invDeltaX = reciprocal_approxim(line->x[1] - line->x[0]);
#ifdef IPOL_Z
  slopeZ = (line->z[1] - line->z[0]) * invDeltaX;
#endif
#ifdef IPOL_W
  slopeW = (line->w[1] - line->w[0]) * invDeltaX;
#endif
#ifdef IPOL_C0
  VOP2a_4(slopeC[0], = , line->c[0][1], -, line->c[0][0], invDeltaX);
#endif
#ifdef IPOL_T0
  VOP2a_2(slopeT[0], = , line->t[0][1], -, line->t[0][0], invDeltaX);
#endif
#ifdef IPOL_T1
  VOP2a_2(slopeT[1], = , line->t[1][1], -, line->t[1][0], invDeltaX);
#endif
#ifdef IPOL_T2
  slopeT[2] = (line->t[2][1] - line->t[2][0]) * invDeltaX;
#endif
#ifdef IPOL_L0
  slopeL[0] = (line->l[0][1] - line->l[0][0]) * invDeltaX;
#endif
#ifdef SUBTEXEL
  subPixel = ((f32) xStart) - line->x[0];
#ifdef IPOL_Z
  line->z[0] += slopeZ * subPixel;
#endif
#ifdef IPOL_W
  line->w[0] += slopeW * subPixel;
#endif
#ifdef IPOL_C0
  VOP1a_4(line->c[0][0], += , slopeC[0], *, subPixel);
#endif
#ifdef IPOL_T0
  VOP1a_2(line->t[0][0], += , slopeT[0], *, subPixel);
#endif
#ifdef IPOL_T1
  VOP1a_2(line->t[1][0], += , slopeT[1], *, subPixel);
#endif
#ifdef IPOL_T2
  line->t[2][0] += slopeT[2] * subPixel;
#endif
#ifdef IPOL_L0
  line->l[0][0] += slopeL[0] * subPixel;
#endif
#endif
  dst = (tVideoSample*)(s->RenderTarget->Data + line->y * s->RenderTarget->Pitch) + xStart;
#ifdef USE_ZBUFFER
  z = (fp24*)(s->DepthBuffer->Data + line->y * s->DepthBuffer->Pitch) + xStart;
#endif
#ifdef INVERSE_W
  f32 inversew;
#endif
  tFixPoint tx0, tx1;
  tFixPoint ty0, ty1;
  tFixPoint r0, g0, b0;
  tFixPoint r1, g1, b1;
  tFixPoint r2, g2, b2;
  tFixPoint lx, ly, lz;
  tFixPoint ndotl;
  sVec3 light;
#ifdef IPOL_C0
  tFixPoint r3, g3, b3;
#endif
  s32 i;
  for (i = 0; i <= dx; i++) {
#ifdef CMP_Z
    if (line->z[0] < z[i])
#endif
#ifdef CMP_W
      if (line->w[0] >= z[i])
#endif
      {
#ifdef INVERSE_W
        inversew = fix_inverse32(line->w[0]);
        tx0 = tofix(line->t[0][0][0], inversew);
        ty0 = tofix(line->t[0][0][1], inversew);
        tx1 = tofix(line->t[1][0][0], inversew);
        ty1 = tofix(line->t[1][0][1], inversew);
#ifdef IPOL_C0
        r3 = tofix(line->c[0][0][1], inversew);
        g3 = tofix(line->c[0][0].z, inversew);
        b3 = tofix(line->c[0][0].w, inversew);
#endif
#else
        tx0 = tofix(line->t[0][0][0], FIX_POINT_F32_MUL);
        ty0 = tofix(line->t[0][0][1], FIX_POINT_F32_MUL);
        tx1 = tofix(line->t[1][0][0], FIX_POINT_F32_MUL);
        ty1 = tofix(line->t[1][0][1], FIX_POINT_F32_MUL);
#ifdef IPOL_C0
        r3 = tofix(line->c[0][0][1]);
        g3 = tofix(line->c[0][0].z);
        b3 = tofix(line->c[0][0].w);
#endif
#endif
        getSample_texture_rgb(&r0, &g0, &b0, &s->IT[0], tx0, ty0);
        // normal map
        getSample_texture_rgb(&r1, &g1, &b1, &s->IT[1], tx1, ty1);
        r1 = (r1 - FIX_POINT_HALF_COLOR) >> (COLOR_MAX_LOG2 - 1);
        g1 = (g1 - FIX_POINT_HALF_COLOR) >> (COLOR_MAX_LOG2 - 1);
        b1 = (b1 - FIX_POINT_HALF_COLOR) >> (COLOR_MAX_LOG2 - 1);
        /*
              sVec3 l = line->l[0][0] * inversew;
              l.setLength( 2.f );
              lx = tofix ( l.x - 0.5f );
              ly = tofix ( l.y - 0.5f );
              lz = tofix ( l.z - 0.5f );
        */
        lx = tofix(line->l[0][0][0], inversew);
        ly = tofix(line->l[0][0][1], inversew);
        lz = tofix(line->l[0][0].z, inversew);
        // DOT 3 Normal Map light in tangent space
        ndotl = saturateFix(FIX_POINT_HALF_COLOR + ((imulFix(r1, lx) + imulFix(g1, ly) + imulFix(b1, lz)) << (COLOR_MAX_LOG2 - 1)));
#ifdef IPOL_C0
        // N . L
        r2 = imulFix(imulFix_tex1(r0, ndotl), r3);
        g2 = imulFix(imulFix_tex1(g0, ndotl), g3);
        b2 = imulFix(imulFix_tex1(b0, ndotl), b3);
        /*
              // heightmap: (1 - neu ) + alt - 0.5, on_minus_srcalpha + add signed
              // emboss bump map
              a4 -= a1;
              r2 = clampfix_maxcolor ( clampfix_mincolor ( imulFix ( r0 + a4, r3 ) ) );
              g2 = clampfix_maxcolor ( clampfix_mincolor ( imulFix ( g0 + a4, g3 ) ) );
              b2 = clampfix_maxcolor ( clampfix_mincolor ( imulFix ( b0 + a4, b3 ) ) );
        */
        /*
              r2 = clampfix_maxcolor ( imulFix_tex1 ( r2, r1 ) );
              g2 = clampfix_maxcolor ( imulFix_tex1 ( g2, g1 ) );
              b2 = clampfix_maxcolor ( imulFix_tex1 ( b2, b1 ) );
        */
#else
        r2 = clampfix_maxcolor(imulFix_tex4(r0, r1));
        g2 = clampfix_maxcolor(imulFix_tex4(g0, g1));
        b2 = clampfix_maxcolor(imulFix_tex4(b0, b1));
#endif
        dst[i] = fix_to_color_rgb(r2, g2, b2);
#ifdef WRITE_Z
        z[i] = line->z[0];
#endif
#ifdef WRITE_W
        z[i] = line->w[0];
#endif
      }
#ifdef IPOL_Z
    line->z[0] += slopeZ;
#endif
#ifdef IPOL_W
    line->w[0] += slopeW;
#endif
#ifdef IPOL_C0
    VOP1_4(line->c[0][0], += , slopeC[0]);
#endif
#ifdef IPOL_T0
    VOP1_2(line->t[0][0], += , slopeT[0]);
#endif
#ifdef IPOL_T1
    VOP1_2(line->t[1][0], += , slopeT[1]);
#endif
#ifdef IPOL_T2
    line->t[2][0] += slopeT[2];
#endif
#ifdef IPOL_L0
    line->l[0][0] += slopeL[0];
#endif
  }
}
void CTRNormalMap_drawTriangle(IBurningShader* s, const s4DVertex* a, const s4DVertex* b, const s4DVertex* c)
{
  // sort on height, y
  if (F32_A_GREATER_B(a->Pos[1], b->Pos[1])) {
    swapVertexPointer(&a, &b);
  }
  if (F32_A_GREATER_B(b->Pos[1], c->Pos[1])) {
    swapVertexPointer(&b, &c);
  }
  if (F32_A_GREATER_B(a->Pos[1], b->Pos[1])) {
    swapVertexPointer(&a, &b);
  }
  ca = c->Pos[1] - a->Pos[1];
  ba = b->Pos[1] - a->Pos[1];
  cb = c->Pos[1] - b->Pos[1];
  // calculate delta y of the edges
  scan->invDeltaY[0] = reciprocal(ca);
  scan->invDeltaY[1] = reciprocal(ba);
  scan->invDeltaY[2] = reciprocal(cb);
  if (F32_LOWER_EQUAL_0(scan->invDeltaY[0])) {
    return;
  }
  // find if the major edge is left or right aligned
  temp[0] = a->Pos[0] - c->Pos[0];
  temp[1] = -ca;
  temp[2] = b->Pos[0] - a->Pos[0];
  temp[3] = ba;
  scan->left = (temp[0] * temp[3] - temp[1] * temp[2]) > 0.f ? 0 : 1;
  scan->right = 1 - scan->left;
  // calculate slopes for the major edge
  scan->slopeX[0] = (c->Pos[0] - a->Pos[0]) * scan->invDeltaY[0];
  scan->x[0] = a->Pos[0];
#ifdef IPOL_Z
  scan->slopeZ[0] = (c->Pos[2] - a->Pos[2]) * scan->invDeltaY[0];
  scan->z[0] = a->Pos[2];
#endif
#ifdef IPOL_W
  scan->slopeW[0] = (c->Pos[3] - a->Pos[3]) * scan->invDeltaY[0];
  scan->w[0] = a->Pos[3];
#endif
#ifdef IPOL_C0
  VOP2a_4(scan->slopeC[0][0], = , c->Color[0], -, a->Color[0], scan->invDeltaY[0]);
  VOP1_4(scan->c[0][0], = , a->Color[0]);
#endif
#ifdef IPOL_T0
  VOP2a_2(scan->slopeT[0][0], = , c->Tex[0], -, a->Tex[0], scan->invDeltaY[0]);
  VOP1_2(scan->t[0][0], = , a->Tex[0]);
#endif
#ifdef IPOL_T1
  VOP2a_2(scan->slopeT[1][0], = , c->Tex[1], -, a->Tex[1], scan->invDeltaY[0]);
  VOP1_2(scan->t[1][0], = , a->Tex[1]);
#endif
#ifdef IPOL_T2
  scan->slopeT[2][0] = (c->Tex[2] - a->Tex[2]) * scan->invDeltaY[0];
  scan->t[2][0] = a->Tex[2];
#endif
#ifdef IPOL_L0
  scan->slopeL[0][0] = (c->LightTangent[0] - a->LightTangent[0]) * scan->invDeltaY[0];
  scan->l[0][0] = a->LightTangent[0];
#endif
  // top left fill convention y run
  // rasterize upper sub-triangle
  //if ( (f32) 0.0 != scan->invDeltaY[1]  )
  if (F32_GREATER_0(scan->invDeltaY[1])) {
    // calculate slopes for top edge
    scan->slopeX[1] = (b->Pos[0] - a->Pos[0]) * scan->invDeltaY[1];
    scan->x[1] = a->Pos[0];
#ifdef IPOL_Z
    scan->slopeZ[1] = (b->Pos[2] - a->Pos[2]) * scan->invDeltaY[1];
    scan->z[1] = a->Pos[2];
#endif
#ifdef IPOL_W
    scan->slopeW[1] = (b->Pos[3] - a->Pos[3]) * scan->invDeltaY[1];
    scan->w[1] = a->Pos[3];
#endif
#ifdef IPOL_C0
    VOP2a_4(scan->slopeC[0][1], = , b->Color[0], -, a->Color[0], scan->invDeltaY[1]);
    VOP1_4(scan->c[0][1], = , a->Color[0]);
#endif
#ifdef IPOL_T0
    VOP2a_2(scan->slopeT[0][1], = , b->Tex[0], -, a->Tex[0], scan->invDeltaY[1]);
    VOP1_2(scan->t[0][1], = , a->Tex[0]);
#endif
#ifdef IPOL_T1
    VOP2a_2(scan->slopeT[1][1], = , b->Tex[1], -, a->Tex[1], scan->invDeltaY[1]);
    VOP1_2(scan->t[1][1], = , a->Tex[1]);
#endif
#ifdef IPOL_T2
    scan->slopeT[2][1] = (b->Tex[2] - a->Tex[2]) * scan->invDeltaY[1];
    scan->t[2][1] = a->Tex[2];
#endif
#ifdef IPOL_L0
    scan->slopeL[0][1] = (b->LightTangent[0] - a->LightTangent[0]) * scan->invDeltaY[1];
    scan->l[0][1] = a->LightTangent[0];
#endif
    // apply top-left fill convention, top part
    yStart = ceil32(a->Pos[1]);
    yEnd = ceil32(b->Pos[1]) - 1;
#ifdef SUBTEXEL
    subPixel = ((f32) yStart) - a->Pos[1];
    // correct to pixel center
    scan->x[0] += scan->slopeX[0] * subPixel;
    scan->x[1] += scan->slopeX[1] * subPixel;
#ifdef IPOL_Z
    scan->z[0] += scan->slopeZ[0] * subPixel;
    scan->z[1] += scan->slopeZ[1] * subPixel;
#endif
#ifdef IPOL_W
    scan->w[0] += scan->slopeW[0] * subPixel;
    scan->w[1] += scan->slopeW[1] * subPixel;
#endif
#ifdef IPOL_C0
    VOP1a_4(scan->c[0][0], += , scan->slopeC[0][0], *, subPixel);
    VOP1a_4(scan->c[0][1], += , scan->slopeC[0][1], *, subPixel);
#endif
#ifdef IPOL_T0
    VOP1a_2(scan->t[0][0], += , scan->slopeT[0][0], *, subPixel);
    VOP1a_2(scan->t[0][1], += , scan->slopeT[0][1], *, subPixel);
#endif
#ifdef IPOL_T1
    VOP1a_2(scan->t[1][0], += , scan->slopeT[1][0], *, subPixel);
    VOP1a_2(scan->t[1][1], += , scan->slopeT[1][1], *, subPixel);
#endif
#ifdef IPOL_T2
    scan->t[2][0] += scan->slopeT[2][0] * subPixel;
    scan->t[2][1] += scan->slopeT[2][1] * subPixel;
#endif
#ifdef IPOL_L0
    scan->l[0][0] += scan->slopeL[0][0] * subPixel;
    scan->l[0][1] += scan->slopeL[0][1] * subPixel;
#endif
#endif
    // rasterize the edge scanlines
    for (line->y = yStart; line->y <= yEnd; ++line->y) {
      line->x[scan->left] = scan->x[0];
      line->x[scan->right] = scan->x[1];
#ifdef IPOL_Z
      line->z[scan->left] = scan->z[0];
      line->z[scan->right] = scan->z[1];
#endif
#ifdef IPOL_W
      line->w[scan->left] = scan->w[0];
      line->w[scan->right] = scan->w[1];
#endif
#ifdef IPOL_C0
      VOP1_4(line->c[0][scan->left], = , scan->c[0][0]);
      VOP1_4(line->c[0][scan->right], = , scan->c[0][1]);
#endif
#ifdef IPOL_T0
      VOP1_2(line->t[0][scan->left], = , scan->t[0][0]);
      VOP1_2(line->t[0][scan->right], = , scan->t[0][1]);
#endif
#ifdef IPOL_T1
      VOP1_2(line->t[1][scan->left], = , scan->t[1][0]);
      VOP1_2(line->t[1][scan->right], = , scan->t[1][1]);
#endif
#ifdef IPOL_T2
      line->t[2][scan->left] = scan->t[2][0];
      line->t[2][scan->right] = scan->t[2][1];
#endif
#ifdef IPOL_L0
      line->l[0][scan->left] = scan->l[0][0];
      line->l[0][scan->right] = scan->l[0][1];
#endif
      // render a scanline
      scanline_bilinear(s);
      scan->x[0] += scan->slopeX[0];
      scan->x[1] += scan->slopeX[1];
#ifdef IPOL_Z
      scan->z[0] += scan->slopeZ[0];
      scan->z[1] += scan->slopeZ[1];
#endif
#ifdef IPOL_W
      scan->w[0] += scan->slopeW[0];
      scan->w[1] += scan->slopeW[1];
#endif
#ifdef IPOL_C0
      VOP1_4(scan->c[0][0], += , scan->slopeC[0][0]);
      VOP1_4(scan->c[0][1], += , scan->slopeC[0][1]);
#endif
#ifdef IPOL_T0
      VOP1_2(scan->t[0][0], += , scan->slopeT[0][0]);
      VOP1_2(scan->t[0][1], += , scan->slopeT[0][1]);
#endif
#ifdef IPOL_T1
      VOP1_2(scan->t[1][0], += , scan->slopeT[1][0]);
      VOP1_2(scan->t[1][1], += , scan->slopeT[1][1]);
#endif
#ifdef IPOL_T2
      scan->t[2][0] += scan->slopeT[2][0];
      scan->t[2][1] += scan->slopeT[2][1];
#endif
#ifdef IPOL_L0
      scan->l[0][0] += scan->slopeL[0][0];
      scan->l[0][1] += scan->slopeL[0][1];
#endif
    }
  }
  // rasterize lower sub-triangle
  //if ( (f32) 0.0 != scan->invDeltaY[2] )
  if (F32_GREATER_0(scan->invDeltaY[2])) {
    // advance to middle point
    //if( (f32) 0.0 != scan->invDeltaY[1] )
    if (F32_GREATER_0(scan->invDeltaY[1])) {
      temp[0] = b->Pos[1] - a->Pos[1];  // dy
      scan->x[0] = a->Pos[0] + scan->slopeX[0] * temp[0];
#ifdef IPOL_Z
      scan->z[0] = a->Pos[2] + scan->slopeZ[0] * temp[0];
#endif
#ifdef IPOL_W
      scan->w[0] = a->Pos[3] + scan->slopeW[0] * temp[0];
#endif
#ifdef IPOL_C0
      VOP2b_4(scan->c[0][0], = , a->Color[0], +, scan->slopeC[0][0], temp[0]);
#endif
#ifdef IPOL_T0
      VOP2b_2(scan->t[0][0], = , a->Tex[0], +, scan->slopeT[0][0], temp[0]);
#endif
#ifdef IPOL_T1
      VOP2b_2(scan->t[1][0], = , a->Tex[1], +, scan->slopeT[1][0], temp[0]);
#endif
#ifdef IPOL_T2
      scan->t[2][0] = a->Tex[2] + scan->slopeT[2][0] * temp[0];
#endif
#ifdef IPOL_L0
      scan->l[0][0] = a->LightTangent[0] + scan->slopeL[0][0] * temp[0];
#endif
    }
    // calculate slopes for bottom edge
    scan->slopeX[1] = (c->Pos[0] - b->Pos[0]) * scan->invDeltaY[2];
    scan->x[1] = b->Pos[0];
#ifdef IPOL_Z
    scan->slopeZ[1] = (c->Pos[2] - b->Pos[2]) * scan->invDeltaY[2];
    scan->z[1] = b->Pos[2];
#endif
#ifdef IPOL_W
    scan->slopeW[1] = (c->Pos[3] - b->Pos[3]) * scan->invDeltaY[2];
    scan->w[1] = b->Pos[3];
#endif
#ifdef IPOL_C0
    VOP2a_4(scan->slopeC[0][1], = , c->Color[0], -, b->Color[0], scan->invDeltaY[2]);
    VOP1_4(scan->c[0][1], = , b->Color[0]);
#endif
#ifdef IPOL_T0
    VOP2a_2(scan->slopeT[0][1], = , c->Tex[0], -, b->Tex[0], scan->invDeltaY[2]);
    VOP1_2(scan->t[0][1], = , b->Tex[0]);
#endif
#ifdef IPOL_T1
    VOP2a_2(scan->slopeT[1][1], = , c->Tex[1], -, b->Tex[1], scan->invDeltaY[2]);
    VOP1_2(scan->t[1][1], = , b->Tex[1]);
#endif
#ifdef IPOL_T2
    scan->slopeT[2][1] = (c->Tex[2] - b->Tex[2]) * scan->invDeltaY[2];
    scan->t[2][1] = b->Tex[2];
#endif
#ifdef IPOL_L0
    scan->slopeL[0][1] = (c->LightTangent[0] - b->LightTangent[0]) * scan->invDeltaY[2];
    scan->l[0][1] = b->LightTangent[0];
#endif
    // apply top-left fill convention, top part
    yStart = ceil32(b->Pos[1]);
    yEnd = ceil32(c->Pos[1]) - 1;
#ifdef SUBTEXEL
    subPixel = ((f32) yStart) - b->Pos[1];
    // correct to pixel center
    scan->x[0] += scan->slopeX[0] * subPixel;
    scan->x[1] += scan->slopeX[1] * subPixel;
#ifdef IPOL_Z
    scan->z[0] += scan->slopeZ[0] * subPixel;
    scan->z[1] += scan->slopeZ[1] * subPixel;
#endif
#ifdef IPOL_W
    scan->w[0] += scan->slopeW[0] * subPixel;
    scan->w[1] += scan->slopeW[1] * subPixel;
#endif
#ifdef IPOL_C0
    VOP1a_4(scan->c[0][0], += , scan->slopeC[0][0], *, subPixel);
    VOP1a_4(scan->c[0][1], += , scan->slopeC[0][1], *, subPixel);
#endif
#ifdef IPOL_T0
    VOP1a_2(scan->t[0][0], += , scan->slopeT[0][0], *, subPixel);
    VOP1a_2(scan->t[0][1], += , scan->slopeT[0][1], *, subPixel);
#endif
#ifdef IPOL_T1
    VOP1a_2(scan->t[1][0], += , scan->slopeT[1][0], *, subPixel);
    VOP1a_2(scan->t[1][1], += , scan->slopeT[1][1], *, subPixel);
#endif
#ifdef IPOL_T2
    scan->t[2][0] += scan->slopeT[2][0] * subPixel;
    scan->t[2][1] += scan->slopeT[2][1] * subPixel;
#endif
#ifdef IPOL_L0
    scan->l[0][0] += scan->slopeL[0][0] * subPixel;
    scan->l[0][1] += scan->slopeL[0][1] * subPixel;
#endif
#endif
    // rasterize the edge scanlines
    for (line->y = yStart; line->y <= yEnd; ++line->y) {
      line->x[scan->left] = scan->x[0];
      line->x[scan->right] = scan->x[1];
#ifdef IPOL_Z
      line->z[scan->left] = scan->z[0];
      line->z[scan->right] = scan->z[1];
#endif
#ifdef IPOL_W
      line->w[scan->left] = scan->w[0];
      line->w[scan->right] = scan->w[1];
#endif
#ifdef IPOL_C0
      VOP1_4(line->c[0][scan->left], = , scan->c[0][0]);
      VOP1_4(line->c[0][scan->right], = , scan->c[0][1]);
#endif
#ifdef IPOL_T0
      VOP1_2(line->t[0][scan->left], = , scan->t[0][0]);
      VOP1_2(line->t[0][scan->right], = , scan->t[0][1]);
#endif
#ifdef IPOL_T1
      VOP1_2(line->t[1][scan->left], = , scan->t[1][0]);
      VOP1_2(line->t[1][scan->right], = , scan->t[1][1]);
#endif
#ifdef IPOL_T2
      line->t[2][scan->left] = scan->t[2][0];
      line->t[2][scan->right] = scan->t[2][1];
#endif
#ifdef IPOL_L0
      line->l[0][scan->left] = scan->l[0][0];
      line->l[0][scan->right] = scan->l[0][1];
#endif
      // render a scanline
      scanline_bilinear(s);
      scan->x[0] += scan->slopeX[0];
      scan->x[1] += scan->slopeX[1];
#ifdef IPOL_Z
      scan->z[0] += scan->slopeZ[0];
      scan->z[1] += scan->slopeZ[1];
#endif
#ifdef IPOL_W
      scan->w[0] += scan->slopeW[0];
      scan->w[1] += scan->slopeW[1];
#endif
#ifdef IPOL_C0
      VOP1_4(scan->c[0][0], += , scan->slopeC[0][0]);
      VOP1_4(scan->c[0][1], += , scan->slopeC[0][1]);
#endif
#ifdef IPOL_T0
      VOP1_2(scan->t[0][0], += , scan->slopeT[0][0]);
      VOP1_2(scan->t[0][1], += , scan->slopeT[0][1]);
#endif
#ifdef IPOL_T1
      VOP1_2(scan->t[1][0], += , scan->slopeT[1][0]);
      VOP1_2(scan->t[1][1], += , scan->slopeT[1][1]);
#endif
#ifdef IPOL_T2
      scan->t[2][0] += scan->slopeT[2][0];
      scan->t[2][1] += scan->slopeT[2][1];
#endif
#ifdef IPOL_L0
      scan->l[0][0] += scan->slopeL[0][0];
      scan->l[0][1] += scan->slopeL[0][1];
#endif
    }
  }
}
#endif // _IRR_COMPILE_WITH_BURNINGSVIDEO_
//! creates a triangle renderer
IBurningShader* createTRNormalMap(CBurningVideoDriver* driver)
{
#ifdef _IRR_COMPILE_WITH_BURNINGSVIDEO_
  return new CTRNormalMap(driver);
#else
  return 0;
#endif // _IRR_COMPILE_WITH_BURNINGSVIDEO_
}

