// Copyright (C) 2009-2012 Christian Stehno
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __S_VERTEX_MANIPULATOR_H_INCLUDED__
#define __S_VERTEX_MANIPULATOR_H_INCLUDED__
class IMesh;
class IMeshBuffer;
struct SMesh;
//! Interface for vertex manipulators.
/** You should derive your manipulator from this class if it shall be called for every vertex, getting as parameter just the vertex.
*/
struct IVertexManipulator {
};
//! Vertex manipulator to set color to a fixed color for all vertices
class SVertexColorSetManipulator : public IVertexManipulator
{
public:
  SVertexColorSetManipulator(SColor color) : Color(color) {}
  void operator()(S3DVertex& vertex) const {
    vertex.Color = Color;
  }
private:
  SColor Color;
};
//! Vertex manipulator to set the alpha value of the vertex color to a fixed value
class SVertexColorSetAlphaManipulator : public IVertexManipulator
{
public:
  SVertexColorSetAlphaManipulator(u32 alpha) : Alpha(alpha) {}
  void operator()(S3DVertex& vertex) const {
    vertex.Color.setAlpha(Alpha);
  }
private:
  u32 Alpha;
};
//! Vertex manipulator which invertes the RGB values
class SVertexColorInvertManipulator : public IVertexManipulator
{
public:
  void operator()(S3DVertex& vertex) const {
    vertex.Color.setRed(255 - vertex.Color.getRed());
    vertex.Color.setGreen(255 - vertex.Color.getGreen());
    vertex.Color.setBlue(255 - vertex.Color.getBlue());
  }
};
//! Vertex manipulator to set vertex color to one of two values depending on a given threshold
/** If average of the color value is >Threshold the High color is chosen, else Low. */
class SVertexColorThresholdManipulator : public IVertexManipulator
{
public:
  SVertexColorThresholdManipulator(u8 threshold, SColor low,
      SColor high) : Threshold(threshold), Low(low), High(high) {}
  void operator()(S3DVertex& vertex) const {
    vertex.Color = ((u8)vertex.Color.getAverage() > Threshold) ? High : Low;
  }
private:
  u8 Threshold;
  SColor Low;
  SColor High;
};
//! Vertex manipulator which adjusts the brightness by the given amount
/** A positive value increases brightness, a negative value darkens the colors. */
class SVertexColorBrightnessManipulator : public IVertexManipulator
{
public:
  SVertexColorBrightnessManipulator(s32 amount) : Amount(amount) {}
  void operator()(S3DVertex& vertex) const {
    vertex.Color.setRed(clamp(vertex.Color.getRed() + Amount, 0u, 255u));
    vertex.Color.setGreen(clamp(vertex.Color.getGreen() + Amount, 0u, 255u));
    vertex.Color.setBlue(clamp(vertex.Color.getBlue() + Amount, 0u, 255u));
  }
private:
  s32 Amount;
};
//! Vertex manipulator which adjusts the contrast by the given factor
/** Factors over 1 increase contrast, below 1 reduce it. */
class SVertexColorContrastManipulator : public IVertexManipulator
{
public:
  SVertexColorContrastManipulator(f32 factor) : Factor(factor) {}
  void operator()(S3DVertex& vertex) const {
    vertex.Color.setRed(clamp(round32((vertex.Color.getRed() - 128)*Factor) + 128, 0, 255));
    vertex.Color.setGreen(clamp(round32((vertex.Color.getGreen() - 128)*Factor) + 128, 0, 255));
    vertex.Color.setBlue(clamp(round32((vertex.Color.getBlue() - 128)*Factor) + 128, 0, 255));
  }
private:
  f32 Factor;
};
//! Vertex manipulator which adjusts the contrast by the given factor and brightness by a signed amount.
/** Factors over 1 increase contrast, below 1 reduce it.
A positive amount increases brightness, a negative one darkens the colors. */
class SVertexColorContrastBrightnessManipulator : public IVertexManipulator
{
public:
  SVertexColorContrastBrightnessManipulator(f32 factor, s32 amount) : Factor(factor), Amount(amount + 128) {}
  void operator()(S3DVertex& vertex) const {
    vertex.Color.setRed(clamp(round32((vertex.Color.getRed() - 128)*Factor) + Amount, 0, 255));
    vertex.Color.setGreen(clamp(round32((vertex.Color.getGreen() - 128)*Factor) + Amount, 0, 255));
    vertex.Color.setBlue(clamp(round32((vertex.Color.getBlue() - 128)*Factor) + Amount, 0, 255));
  }
private:
  f32 Factor;
  s32 Amount;
};
//! Vertex manipulator which adjusts the brightness by a gamma operation
/** A value over one increases brightness, one below darkens the colors. */
class SVertexColorGammaManipulator : public IVertexManipulator
{
public:
  SVertexColorGammaManipulator(f32 gamma) : Gamma(1.f) {
    if (gamma != 0.f) {
      Gamma = 1.f / gamma;
    }
  }
  void operator()(S3DVertex& vertex) const {
    vertex.Color.setRed(clamp(round32(powf((f32)(vertex.Color.getRed()), Gamma)), 0, 255));
    vertex.Color.setGreen(clamp(round32(powf((f32)(vertex.Color.getGreen()), Gamma)), 0, 255));
    vertex.Color.setBlue(clamp(round32(powf((f32)(vertex.Color.getBlue()), Gamma)), 0, 255));
  }
private:
  f32 Gamma;
};
//! Vertex manipulator which scales the color values
/** Can e.g be used for white balance, factor would be 255.f/brightest color. */
class SVertexColorScaleManipulator : public IVertexManipulator
{
public:
  SVertexColorScaleManipulator(f32 factor) : Factor(factor) {}
  void operator()(S3DVertex& vertex) const {
    vertex.Color.setRed(clamp(round32(vertex.Color.getRed()*Factor), 0, 255));
    vertex.Color.setGreen(clamp(round32(vertex.Color.getGreen()*Factor), 0, 255));
    vertex.Color.setBlue(clamp(round32(vertex.Color.getBlue()*Factor), 0, 255));
  }
private:
  f32 Factor;
};
//! Vertex manipulator which desaturates the color values
/** Uses the lightness value of the color. */
class SVertexColorDesaturateToLightnessManipulator : public IVertexManipulator
{
public:
  void operator()(S3DVertex& vertex) const {
    vertex.Color = round32(vertex.Color.getLightness());
  }
};
//! Vertex manipulator which desaturates the color values
/** Uses the average value of the color. */
class SVertexColorDesaturateToAverageManipulator : public IVertexManipulator
{
public:
  void operator()(S3DVertex& vertex) const {
    vertex.Color = vertex.Color.getAverage();
  }
};
//! Vertex manipulator which desaturates the color values
/** Uses the luminance value of the color. */
class SVertexColorDesaturateToLuminanceManipulator : public IVertexManipulator
{
public:
  void operator()(S3DVertex& vertex) const {
    vertex.Color = round32(vertex.Color.getLuminance());
  }
};
//! Vertex manipulator which interpolates the color values
/** Uses linear interpolation. */
class SVertexColorInterpolateLinearManipulator : public IVertexManipulator
{
public:
  SVertexColorInterpolateLinearManipulator(SColor color, f32 factor) :
    Color(color), Factor(factor) {}
  void operator()(S3DVertex& vertex) const {
    vertex.Color = vertex.Color.getInterpolated(Color, Factor);
  }
private:
  SColor Color;
  f32 Factor;
};
//! Vertex manipulator which interpolates the color values
/** Uses linear interpolation. */
class SVertexColorInterpolateQuadraticManipulator : public IVertexManipulator
{
public:
  SVertexColorInterpolateQuadraticManipulator(SColor color1, SColor color2, f32 factor) :
    Color1(color1), Color2(color2), Factor(factor) {}
  void operator()(S3DVertex& vertex) const {
    vertex.Color = vertex.Color.getInterpolated_quadratic(Color1, Color2, Factor);
  }
private:
  SColor Color1;
  SColor Color2;
  f32 Factor;
};
//! Vertex manipulator which scales the position of the vertex
class SVertexPositionScaleManipulator : public IVertexManipulator
{
public:
  SVertexPositionScaleManipulator(const float3 factor) : Factor(factor) {}
  template <typename VType>
  void operator()(VType& vertex) const {
    vertex.Pos *= Factor;
  }
private:
  float3 Factor;
};
//! Vertex manipulator which scales the position of the vertex along the normals
/** This can look more pleasing than the usual Scale operator, but
depends on the mesh geometry.
*/
class SVertexPositionScaleAlongNormalsManipulator : public IVertexManipulator
{
public:
  SVertexPositionScaleAlongNormalsManipulator(const float3 factor) : Factor(factor) {}
  template <typename VType>
  void operator()(VType& vertex) const {
    vertex.Pos += vertex.Normal * Factor;
  }
private:
  float3 Factor;
};
//! Vertex manipulator which transforms the position of the vertex
class SVertexPositionTransformManipulator : public IVertexManipulator
{
public:
  SVertexPositionTransformManipulator(const matrix4& m) : Transformation(m) {}
  template <typename VType>
  void operator()(VType& vertex) const {
    Transformation.transformVect(vertex.Pos);
  }
private:
  matrix4 Transformation;
};
//! Vertex manipulator which scales the TCoords of the vertex
class SVertexTCoordsScaleManipulator : public IVertexManipulator
{
public:
  SVertexTCoordsScaleManipulator(const vector2df& factor, u32 uvSet = 1) : Factor(factor), UVSet(uvSet) {}
  void operator()(S3DVertex& vertex) const {
    if (1 == UVSet) {
      vertex.TCoords *= Factor;
    }
    else if (2 == UVSet) {
      vertex.TCoords2 *= Factor;
    }
  }
#if 0
  template <typename VType>
  void operator()(VType& vertex) const {
    if (1 == UVSet) {
      vertex.TCoords *= Factor;
    }
  }
#else
  void operator()(S3DVertex& vertex) const {
    if (1 == UVSet) {
      vertex.TCoords *= Factor;
    }
  }
  void operator()(S3DVertex& vertex) const {
    if (1 == UVSet) {
      vertex.TCoords *= Factor;
    }
  }
#endif
private:
  vector2df Factor;
  u32 UVSet;
};
#endif

