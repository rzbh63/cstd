// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
//#include "CSkyBoxSceneNode.h"
//#include "IVideoDriver.h"
//#include "ISceneManager.h"
//#include "ICameraSceneNode.h"
//#include "os.h"
//! constructor
CSkyBoxSceneNode::CSkyBoxSceneNode(ITexture* top, ITexture* bottom, ITexture* left,
    ITexture* right, ITexture* front, ITexture* back, ISceneNode* parent, ISceneManager* mgr, s32 id)
  : ISceneNode(parent, mgr, id)
{
#ifdef _DEBUG
  setDebugName("CSkyBoxSceneNode");
#endif
  setAutomaticCulling(EAC_OFF);
  Box.MaxEdge.set(0, 0, 0);
  Box.MinEdge.set(0, 0, 0);
  // create indices
  Indices[0] = 0;
  Indices[1] = 1;
  Indices[2] = 2;
  Indices[3] = 3;
  // create material
  SMaterial mat;
  mat.Lighting = false;
  mat.ZBuffer = ECFN_NEVER;
  mat.ZWriteEnable = false;
  mat.AntiAliasing = 0;
  mat.TextureLayer[0].TextureWrapU = ETC_CLAMP_TO_EDGE;
  mat.TextureLayer[0].TextureWrapV = ETC_CLAMP_TO_EDGE;
  /* Hey, I am no artist, but look at that
     cool ASCII art I made! ;)
       -111         111
          /6--------/5        y
         /  |      / |        ^  z
        /   |   11-1 |        | /
  -11-1 3---------2  |        |/
        |   7- - -| -4 1-11    *---->x
        | -1-11   |  /       3-------|2
        |/        | /         |    //|
        0---------1/          |  //  |
     -1-1-1     1-1-1         |//    |
                       0--------1
  */
  ITexture* tex = front;
  if (!tex) {
    tex = left;
  }
  if (!tex) {
    tex = back;
  }
  if (!tex) {
    tex = right;
  }
  if (!tex) {
    tex = top;
  }
  if (!tex) {
    tex = bottom;
  }
  const f32 onepixel = tex ? (1.0f / (tex->getSize().Width * 1.5f)) : 0.0f;
  const f32 t = 1.0f - onepixel;
  const f32 o = 0.0f + onepixel;
  // create front side
  Material[0] = mat;
  Material[0].setTexture(0, front);
  Vertices[0] = S3DVertex(-1, -1, -1, 0, 0, 1, SColor(255, 255, 255, 255), t, t);
  Vertices[1] = S3DVertex(1, -1, -1, 0, 0, 1, SColor(255, 255, 255, 255), o, t);
  Vertices[2] = S3DVertex(1, 1, -1, 0, 0, 1, SColor(255, 255, 255, 255), o, o);
  Vertices[3] = S3DVertex(-1, 1, -1, 0, 0, 1, SColor(255, 255, 255, 255), t, o);
  // create left side
  Material[1] = mat;
  Material[1].setTexture(0, left);
  Vertices[4] = S3DVertex(1, -1, -1, -1, 0, 0, SColor(255, 255, 255, 255), t, t);
  Vertices[5] = S3DVertex(1, -1, 1, -1, 0, 0, SColor(255, 255, 255, 255), o, t);
  Vertices[6] = S3DVertex(1, 1, 1, -1, 0, 0, SColor(255, 255, 255, 255), o, o);
  Vertices[7] = S3DVertex(1, 1, -1, -1, 0, 0, SColor(255, 255, 255, 255), t, o);
  // create back side
  Material[2] = mat;
  Material[2].setTexture(0, back);
  Vertices[8]  = S3DVertex(1, -1, 1, 0, 0, -1, SColor(255, 255, 255, 255), t, t);
  Vertices[9]  = S3DVertex(-1, -1, 1, 0, 0, -1, SColor(255, 255, 255, 255), o, t);
  Vertices[10] = S3DVertex(-1, 1, 1, 0, 0, -1, SColor(255, 255, 255, 255), o, o);
  Vertices[11] = S3DVertex(1, 1, 1, 0, 0, -1, SColor(255, 255, 255, 255), t, o);
  // create right side
  Material[3] = mat;
  Material[3].setTexture(0, right);
  Vertices[12] = S3DVertex(-1, -1, 1, 1, 0, 0, SColor(255, 255, 255, 255), t, t);
  Vertices[13] = S3DVertex(-1, -1, -1, 1, 0, 0, SColor(255, 255, 255, 255), o, t);
  Vertices[14] = S3DVertex(-1, 1, -1, 1, 0, 0, SColor(255, 255, 255, 255), o, o);
  Vertices[15] = S3DVertex(-1, 1, 1, 1, 0, 0, SColor(255, 255, 255, 255), t, o);
  // create top side
  Material[4] = mat;
  Material[4].setTexture(0, top);
  Vertices[16] = S3DVertex(1, 1, -1, 0, -1, 0, SColor(255, 255, 255, 255), t, t);
  Vertices[17] = S3DVertex(1, 1, 1, 0, -1, 0, SColor(255, 255, 255, 255), o, t);
  Vertices[18] = S3DVertex(-1, 1, 1, 0, -1, 0, SColor(255, 255, 255, 255), o, o);
  Vertices[19] = S3DVertex(-1, 1, -1, 0, -1, 0, SColor(255, 255, 255, 255), t, o);
  // create bottom side
  Material[5] = mat;
  Material[5].setTexture(0, bottom);
  Vertices[20] = S3DVertex(1, -1, 1, 0, 1, 0, SColor(255, 255, 255, 255), o, o);
  Vertices[21] = S3DVertex(1, -1, -1, 0, 1, 0, SColor(255, 255, 255, 255), t, o);
  Vertices[22] = S3DVertex(-1, -1, -1, 0, 1, 0, SColor(255, 255, 255, 255), t, t);
  Vertices[23] = S3DVertex(-1, -1, 1, 0, 1, 0, SColor(255, 255, 255, 255), o, t);
}
//! renders the node.
void CSkyBoxSceneNode::render()
{
  IVideoDriver* driver = SceneManager->getVideoDriver();
  ICameraSceneNode* camera = SceneManager->getActiveCamera();
  if (!camera || !driver) {
    return;
  }
  if (!camera->isOrthogonal()) {
    // draw perspective skybox
    matrix4 translate(AbsoluteTransformation);
    translate.setTranslation(camera->getAbsolutePosition());
    // Draw the sky box between the near and far clip plane
    const f32 viewDistance = (camera->getNearValue() + camera->getFarValue()) * 0.5f;
    matrix4 scale;
    scale.setScale(float3(viewDistance, viewDistance, viewDistance));
    driver->setTransform(ETS_WORLD, translate * scale);
    for (s32 i = 0; i < 6; ++i) {
      driver->setMaterial(Material[i]);
      driver->drawIndexedTriangleFan(&Vertices[i * 4], 4, Indices, 2);
    }
  }
  else {
    // draw orthogonal skybox,
    // simply choose one texture and draw it as 2d picture.
    // there could be better ways to do this, but currently I think this is ok.
    float3 lookVect = camera->getTarget() - camera->getAbsolutePosition();
    lookVect.normalize();
    float3 absVect(abs_(lookVect.X),
        abs_(lookVect.Y),
        abs_(lookVect.Z));
    int idx = 0;
    if (absVect.X >= absVect.Y && absVect.X >= absVect.Z) {
      // x direction
      idx = lookVect.X > 0 ? 0 : 2;
    }
    else if (absVect.Y >= absVect.X && absVect.Y >= absVect.Z) {
      // y direction
      idx = lookVect.Y > 0 ? 4 : 5;
    }
    else if (absVect.Z >= absVect.X && absVect.Z >= absVect.Y) {
      // z direction
      idx = lookVect.Z > 0 ? 1 : 3;
    }
    ITexture* tex = Material[idx].getTexture(0);
    if (tex) {
      IRECT rctDest(position2di(-1, 0),
          dimension2di(driver->getCurrentRenderTargetSize()));
      IRECT rctSrc(position2di(0, 0),
          dimension2di(tex->getSize()));
      driver->draw2DImage(tex, rctDest, rctSrc);
    }
  }
}
//! returns the axis aligned bounding box of this node
const F3AABBOX& CSkyBoxSceneNode::getBoundingBox() const
{
  return Box;
}
void CSkyBoxSceneNode::OnRegisterSceneNode()
{
  if (IsVisible) {
    SceneManager->registerNodeForRendering(this, ESNRP_SKY_BOX);
  }
  ISceneNode::OnRegisterSceneNode();
}
//! returns the material based on the zero based index i. To get the amount
//! of materials used by this scene node, use getMaterialCount().
//! This function is needed for inserting the node into the scene hirachy on a
//! optimal position for minimizing renderstate changes, but can also be used
//! to directly modify the material of a scene node.
SMaterial& CSkyBoxSceneNode::getMaterial(u32 i)
{
  return Material[i];
}
//! returns amount of materials used by this scene node.
u32 CSkyBoxSceneNode::getMaterialCount() const
{
  return 6;
}
//! Creates a clone of this scene node and its children.
ISceneNode* CSkyBoxSceneNode::clone(ISceneNode* newParent, ISceneManager* newManager)
{
  if (!newParent) {
    newParent = Parent;
  }
  if (!newManager) {
    newManager = SceneManager;
  }
  CSkyBoxSceneNode* nb = new CSkyBoxSceneNode(0, 0, 0, 0, 0, 0, newParent,
      newManager, ID);
  nb->cloneMembers(this, newManager);
  int i;
  for (i = 0; i < 6; ++i) {
    nb->Material[i] = Material[i];
  }
  if (newParent) {
    nb->drop();
  }
  return nb;
}

