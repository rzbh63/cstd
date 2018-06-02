// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
//#include "CSceneNodeAnimatorRotation.h"
//! constructor
CSceneNodeAnimatorRotation::CSceneNodeAnimatorRotation(u32 time, const float3 rotation)
  : Rotation(rotation), StartTime(time)
{
#ifdef _DEBUG
  setDebugName("CSceneNodeAnimatorRotation");
#endif
}
//! animates a scene node
void CSceneNodeAnimatorRotation::animateNode(ISceneNode* node, u32 timeMs)
{
  if (node) { // thanks to warui for this fix
    const u32 diffTime = timeMs - StartTime;
    if (diffTime != 0) {
      // clip the rotation to small values, to avoid
      // precision problems with huge floats.
      float3 rot = node->getRotation() + Rotation * (diffTime * 0.1f);
      if (rot.X > 360.f) {
        rot.X = fmodf(rot.X, 360.f);
      }
      if (rot.Y > 360.f) {
        rot.Y = fmodf(rot.Y, 360.f);
      }
      if (rot.Z > 360.f) {
        rot.Z = fmodf(rot.Z, 360.f);
      }
      node->setRotation(rot);
      StartTime = timeMs;
    }
  }
}
//! Writes attributes of the scene node animator.
void CSceneNodeAnimatorRotation::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const
{
  out->addVector3d("Rotation", Rotation);
}
//! Reads attributes of the scene node animator.
void CSceneNodeAnimatorRotation::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options)
{
  Rotation = in->getAttributeAsVector3d("Rotation");
}
ISceneNodeAnimator* CSceneNodeAnimatorRotation::createClone(ISceneNode* node, ISceneManager* newManager)
{
  CSceneNodeAnimatorRotation* newAnimator =
      new CSceneNodeAnimatorRotation(StartTime, Rotation);
  return newAnimator;
}

