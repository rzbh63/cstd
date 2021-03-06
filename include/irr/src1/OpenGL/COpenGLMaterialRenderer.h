// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_OPENGL_MATERIAL_RENDERER_H_INCLUDED__
#define __C_OPENGL_MATERIAL_RENDERER_H_INCLUDED__
#ifdef _IRR_COMPILE_WITH_OPENGL_
//#include "COpenGLDriver.h"
//#include "IMaterialRenderer.h"
//! Base class for all internal OpenGL material renderers
class COpenGLMaterialRenderer : public IMaterialRenderer
{
public:
  //! Constructor
  COpenGLMaterialRenderer(COpenGLDriver* driver) : Driver(driver) {
  }
protected:
  COpenGLDriver* Driver;
};
//! Solid material renderer
class COpenGLMaterialRenderer_SOLID : public COpenGLMaterialRenderer
{
public:
  COpenGLMaterialRenderer_SOLID(COpenGLDriver* d)
    : COpenGLMaterialRenderer(d) {}
  void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
      bool resetAllRenderstates, IMaterialRendererServices* services) {
    Driver->disableTextures(1);
    Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);
    if (resetAllRenderstates || (material.MaterialType != lastMaterial.MaterialType)) {
      // thanks to Murphy, the following line removed some
      // bugs with several OpenGL implementations.
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }
  }
};
//! Generic Texture Blend
class COpenGLMaterialRenderer_ONETEXTURE_BLEND : public COpenGLMaterialRenderer
{
public:
  COpenGLMaterialRenderer_ONETEXTURE_BLEND(COpenGLDriver* d)
    : COpenGLMaterialRenderer(d) {}
  void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
      bool resetAllRenderstates, IMaterialRendererServices* services) {
    Driver->disableTextures(1);
    Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);
    //    if (material.MaterialType != lastMaterial.MaterialType ||
    //      material.MaterialTypeParam != lastMaterial.MaterialTypeParam ||
    //      resetAllRenderstates)
    {
      E_BLEND_FACTOR srcFact, dstFact;
      E_MODULATE_FUNC modulate;
      u32 alphaSource;
      unpack_textureBlendFunc(srcFact, dstFact, modulate, alphaSource, material.MaterialTypeParam);
#ifdef GL_ARB_texture_env_combine
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
      glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_MODULATE);
      glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
      glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);
      glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, (f32) modulate);
#else
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
      glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
      glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
      glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PREVIOUS_EXT);
      glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, (f32) modulate);
#endif
      glBlendFunc(Driver->getGLBlend(srcFact), Driver->getGLBlend(dstFact));
      glEnable(GL_ALPHA_TEST);
      glAlphaFunc(GL_GREATER, 0.f);
      glEnable(GL_BLEND);
      if (textureBlendFunc_hasAlpha(srcFact) || textureBlendFunc_hasAlpha(dstFact)) {
        if (alphaSource == EAS_VERTEX_COLOR) {
#ifdef GL_ARB_texture_env_combine
          glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_REPLACE);
          glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_PRIMARY_COLOR_ARB);
#else
          glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_REPLACE);
          glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_PRIMARY_COLOR_EXT);
#endif
        }
        else if (alphaSource == EAS_TEXTURE) {
#ifdef GL_ARB_texture_env_combine
          glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_REPLACE);
          glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_TEXTURE);
#else
          glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_REPLACE);
          glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_TEXTURE);
#endif
        }
        else {
#ifdef GL_ARB_texture_env_combine
          glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_MODULATE);
          glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_PRIMARY_COLOR_ARB);
          glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_ARB, GL_TEXTURE);
#else
          glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_MODULATE);
          glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_PRIMARY_COLOR_EXT);
          glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_EXT, GL_TEXTURE);
#endif
        }
      }
    }
  }
  void OnUnsetMaterial() {
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
#ifdef GL_ARB_texture_env_combine
    glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_MODULATE);
    glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 1.f);
    glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);
#else
    glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_MODULATE);
    glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, 1.f);
    glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PREVIOUS_EXT);
#endif
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
  }
  //! Returns if the material is transparent.
  /** Is not always transparent, but mostly. */
  bool isTransparent() const {
    return true;
  }
};
//! Solid 2 layer material renderer
class COpenGLMaterialRenderer_SOLID_2_LAYER : public COpenGLMaterialRenderer
{
public:
  COpenGLMaterialRenderer_SOLID_2_LAYER(COpenGLDriver* d)
    : COpenGLMaterialRenderer(d) {}
  void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
      bool resetAllRenderstates, IMaterialRendererServices* services) {
    Driver->disableTextures(2);
    Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);
    if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates) {
      if (Driver->queryFeature(EVDF_MULTITEXTURE)) {
        Driver->extGlActiveTexture(GL_TEXTURE1_ARB);
#ifdef GL_ARB_texture_env_combine
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
        glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_REPLACE);
        glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_PRIMARY_COLOR_ARB);
        glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_INTERPOLATE);
        glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
        glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);
        glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE2_RGB_ARB, GL_PRIMARY_COLOR);
        glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND2_RGB_ARB, GL_SRC_ALPHA);
#else
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
        glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_REPLACE);
        glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_PRIMARY_COLOR_EXT);
        glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_INTERPOLATE);
        glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
        glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PREVIOUS_EXT);
        glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE2_RGB_EXT, GL_PRIMARY_COLOR);
        glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND2_RGB_EXT, GL_SRC_ALPHA);
#endif
      }
    }
  }
  void OnUnsetMaterial() {
    if (Driver->queryFeature(EVDF_MULTITEXTURE)) {
      Driver->extGlActiveTexture(GL_TEXTURE1_ARB);
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
#ifdef GL_ARB_texture_env_combine
      glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND2_RGB_ARB, GL_SRC_COLOR);
#else
      glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND2_RGB_EXT, GL_SRC_COLOR);
#endif
      Driver->extGlActiveTexture(GL_TEXTURE0_ARB);
    }
  }
};
//! Transparent add color material renderer
class COpenGLMaterialRenderer_TRANSPARENT_ADD_COLOR : public COpenGLMaterialRenderer
{
public:
  COpenGLMaterialRenderer_TRANSPARENT_ADD_COLOR(COpenGLDriver* d)
    : COpenGLMaterialRenderer(d) {}
  void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
      bool resetAllRenderstates, IMaterialRendererServices* services) {
    Driver->disableTextures(1);
    Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);
    if ((material.MaterialType != lastMaterial.MaterialType) || resetAllRenderstates) {
      glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      glEnable(GL_BLEND);
    }
  }
  void OnUnsetMaterial() {
    glDisable(GL_BLEND);
  }
  //! Returns if the material is transparent.
  bool isTransparent() const {
    return true;
  }
};
//! Transparent vertex alpha material renderer
class COpenGLMaterialRenderer_TRANSPARENT_VERTEX_ALPHA : public COpenGLMaterialRenderer
{
public:
  COpenGLMaterialRenderer_TRANSPARENT_VERTEX_ALPHA(COpenGLDriver* d)
    : COpenGLMaterialRenderer(d) {}
  void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
      bool resetAllRenderstates, IMaterialRendererServices* services) {
    Driver->disableTextures(1);
    Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);
    if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates) {
#ifdef GL_ARB_texture_env_combine
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
      glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_REPLACE);
      glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_PRIMARY_COLOR_ARB);
      glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_MODULATE);
      glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
      glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);
#else
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
      glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_REPLACE);
      glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_PRIMARY_COLOR_EXT);
      glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
      glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
      glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PREVIOUS_EXT);
#endif
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_BLEND);
    }
  }
  void OnUnsetMaterial() {
    // default values
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
#ifdef GL_ARB_texture_env_combine
    glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_MODULATE);
    glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_TEXTURE);
#else
    glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_MODULATE);
    glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_TEXTURE);
#endif
    glDisable(GL_BLEND);
  }
  //! Returns if the material is transparent.
  bool isTransparent() const {
    return true;
  }
};
//! Transparent alpha channel material renderer
class COpenGLMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL : public COpenGLMaterialRenderer
{
public:
  COpenGLMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL(COpenGLDriver* d)
    : COpenGLMaterialRenderer(d) {}
  void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
      bool resetAllRenderstates, IMaterialRendererServices* services) {
    Driver->disableTextures(1);
    Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);
    if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates
        || material.MaterialTypeParam != lastMaterial.MaterialTypeParam) {
#ifdef GL_ARB_texture_env_combine
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
      glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_MODULATE);
      glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
      glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);
      glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_REPLACE);
      glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_TEXTURE);
#else
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
      glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
      glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
      glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PREVIOUS_EXT);
      glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_REPLACE);
      glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_TEXTURE);
#endif
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_BLEND);
      glEnable(GL_ALPHA_TEST);
      glAlphaFunc(GL_GREATER, material.MaterialTypeParam);
    }
  }
  void OnUnsetMaterial() {
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
#ifdef GL_ARB_texture_env_combine
    glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_MODULATE);
#else
    glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_MODULATE);
#endif
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
  }
  //! Returns if the material is transparent.
  bool isTransparent() const {
    return true;
  }
};
//! Transparent alpha channel material renderer
class COpenGLMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL_REF : public COpenGLMaterialRenderer
{
public:
  COpenGLMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL_REF(COpenGLDriver* d)
    : COpenGLMaterialRenderer(d) {}
  void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
      bool resetAllRenderstates, IMaterialRendererServices* services) {
    Driver->disableTextures(1);
    Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);
    if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates) {
      glEnable(GL_ALPHA_TEST);
      glAlphaFunc(GL_GREATER, 0.5f);
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }
  }
  void OnUnsetMaterial() {
    glDisable(GL_ALPHA_TEST);
  }
  //! Returns if the material is transparent.
  bool isTransparent() const {
    return false;  // this material is not really transparent because it does no blending.
  }
};
//! material renderer for all kinds of lightmaps
class COpenGLMaterialRenderer_LIGHTMAP : public COpenGLMaterialRenderer
{
public:
  COpenGLMaterialRenderer_LIGHTMAP(COpenGLDriver* d)
    : COpenGLMaterialRenderer(d) {}
  void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
      bool resetAllRenderstates, IMaterialRendererServices* services) {
    Driver->disableTextures(2);
    Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);
    if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates) {
      // diffuse map
      switch (material.MaterialType) {
      case EMT_LIGHTMAP_LIGHTING:
      case EMT_LIGHTMAP_LIGHTING_M2:
      case EMT_LIGHTMAP_LIGHTING_M4:
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        break;
      case EMT_LIGHTMAP_ADD:
      case EMT_LIGHTMAP:
      case EMT_LIGHTMAP_M2:
      case EMT_LIGHTMAP_M4:
      default:
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        break;
      }
      if (Driver->queryFeature(EVDF_MULTITEXTURE)) {
        // lightmap
        Driver->extGlActiveTexture(GL_TEXTURE1_ARB);
#ifdef GL_ARB_texture_env_combine
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
        if (material.MaterialType == EMT_LIGHTMAP_ADD) {
          glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_ADD);
        }
        else {
          glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_MODULATE);
        }
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);
        glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_MODULATE);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_TEXTURE);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_ARB, GL_PREVIOUS_ARB);
#else
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
        if (material.MaterialType == EMT_LIGHTMAP_ADD) {
          glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_ADD);
        }
        else {
          glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
        }
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PREVIOUS_EXT);
        glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_MODULATE);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_TEXTURE);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_EXT, GL_PREVIOUS_EXT);
#endif
        switch (material.MaterialType) {
        case EMT_LIGHTMAP_M4:
        case EMT_LIGHTMAP_LIGHTING_M4:
#ifdef GL_ARB_texture_env_combine
          glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 4.0f);
#else
          glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, 4.0f);
#endif
          break;
        case EMT_LIGHTMAP_M2:
        case EMT_LIGHTMAP_LIGHTING_M2:
#ifdef GL_ARB_texture_env_combine
          glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 2.0f);
#else
          glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, 2.0f);
#endif
          break;
        default:
#ifdef GL_ARB_texture_env_combine
          glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 1.0f);
#else
          glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, 1.0f);
#endif
        }
        Driver->extGlActiveTexture(GL_TEXTURE0_ARB);
      }
    }
  }
  void OnUnsetMaterial() {
    if (Driver->queryFeature(EVDF_MULTITEXTURE)) {
      Driver->extGlActiveTexture(GL_TEXTURE1_ARB);
#ifdef GL_ARB_texture_env_combine
      glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 1.f);
#else
      glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, 1.f);
#endif
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      Driver->extGlActiveTexture(GL_TEXTURE0_ARB);
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }
  }
};
//! detail map  material renderer
class COpenGLMaterialRenderer_DETAIL_MAP : public COpenGLMaterialRenderer
{
public:
  COpenGLMaterialRenderer_DETAIL_MAP(COpenGLDriver* d)
    : COpenGLMaterialRenderer(d) {}
  void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
      bool resetAllRenderstates, IMaterialRendererServices* services) {
    Driver->disableTextures(2);
    Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);
    if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates) {
      // diffuse map is default modulated
      // detail map on second layer
      if (Driver->queryFeature(EVDF_MULTITEXTURE)) {
        Driver->extGlActiveTexture(GL_TEXTURE1_ARB);
#ifdef GL_ARB_texture_env_combine
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
        glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_ADD_SIGNED_ARB);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);
#else
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
        glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_ADD_SIGNED_EXT);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PREVIOUS_EXT);
#endif
      }
    }
  }
  void OnUnsetMaterial() {
    if (Driver->queryFeature(EVDF_MULTITEXTURE)) {
      Driver->extGlActiveTexture(GL_TEXTURE1_ARB);
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      Driver->extGlActiveTexture(GL_TEXTURE0_ARB);
    }
  }
};
//! sphere map material renderer
class COpenGLMaterialRenderer_SPHERE_MAP : public COpenGLMaterialRenderer
{
public:
  COpenGLMaterialRenderer_SPHERE_MAP(COpenGLDriver* d)
    : COpenGLMaterialRenderer(d) {}
  void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
      bool resetAllRenderstates, IMaterialRendererServices* services) {
    Driver->disableTextures(1);
    // texture needs to be flipped for OpenGL
    matrix4 tmp = Driver->getTransform(ETS_TEXTURE_0);
    tmp[5] *= -1;
    Driver->setTransform(ETS_TEXTURE_0, tmp);
    Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);
    if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates) {
      glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
      glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
      glEnable(GL_TEXTURE_GEN_S);
      glEnable(GL_TEXTURE_GEN_T);
    }
  }
  void OnUnsetMaterial() {
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
  }
};
//! reflection 2 layer material renderer
class COpenGLMaterialRenderer_REFLECTION_2_LAYER : public COpenGLMaterialRenderer
{
public:
  COpenGLMaterialRenderer_REFLECTION_2_LAYER(COpenGLDriver* d)
    : COpenGLMaterialRenderer(d) {}
  void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
      bool resetAllRenderstates, IMaterialRendererServices* services) {
    Driver->disableTextures(2);
    Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);
    if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates) {
      if (Driver->queryFeature(EVDF_MULTITEXTURE)) {
        Driver->extGlActiveTexture(GL_TEXTURE1_ARB);
#ifdef GL_ARB_texture_env_combine
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
        glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_MODULATE);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);
#else
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
        glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PREVIOUS_EXT);
#endif
      }
      glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
      glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
      glEnable(GL_TEXTURE_GEN_S);
      glEnable(GL_TEXTURE_GEN_T);
    }
  }
  void OnUnsetMaterial() {
    if (Driver->queryFeature(EVDF_MULTITEXTURE)) {
      Driver->extGlActiveTexture(GL_TEXTURE1_ARB);
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    if (Driver->queryFeature(EVDF_MULTITEXTURE)) {
      Driver->extGlActiveTexture(GL_TEXTURE0_ARB);
    }
  }
};
//! reflection 2 layer material renderer
class COpenGLMaterialRenderer_TRANSPARENT_REFLECTION_2_LAYER : public COpenGLMaterialRenderer
{
public:
  COpenGLMaterialRenderer_TRANSPARENT_REFLECTION_2_LAYER(COpenGLDriver* d)
    : COpenGLMaterialRenderer(d) {}
  void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
      bool resetAllRenderstates, IMaterialRendererServices* services) {
    Driver->disableTextures(2);
    Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);
    if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates) {
#ifdef GL_ARB_texture_env_combine
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
      glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_MODULATE);
      glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
      glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);
      glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_REPLACE);
      glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_PREVIOUS_ARB);
#else
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
      glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
      glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
      glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PREVIOUS_EXT);
      glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_REPLACE);
      glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_PREVIOUS_ARB);
#endif
      if (Driver->queryFeature(EVDF_MULTITEXTURE)) {
        Driver->extGlActiveTexture(GL_TEXTURE1_ARB);
#ifdef GL_ARB_texture_env_combine
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
        glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_MODULATE);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);
        glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_REPLACE);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_PREVIOUS_ARB);
#else
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
        glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PREVIOUS_EXT);
        glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_REPLACE);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_PREVIOUS_ARB);
#endif
      }
      glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
      glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
      glEnable(GL_TEXTURE_GEN_S);
      glEnable(GL_TEXTURE_GEN_T);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_BLEND);
    }
  }
  void OnUnsetMaterial() {
    if (Driver->queryFeature(EVDF_MULTITEXTURE)) {
      Driver->extGlActiveTexture(GL_TEXTURE1_ARB);
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    if (Driver->queryFeature(EVDF_MULTITEXTURE)) {
      Driver->extGlActiveTexture(GL_TEXTURE0_ARB);
    }
    glDisable(GL_BLEND);
  }
  //! Returns if the material is transparent.
  bool isTransparent() const {
    return true;
  }
};
#endif
#endif

