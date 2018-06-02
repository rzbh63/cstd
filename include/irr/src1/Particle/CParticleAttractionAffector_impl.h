// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
//#include "CParticleAttractionAffector.h"
//#include "IAttributes.h"
//! constructor
CParticleAttractionAffector::CParticleAttractionAffector(
    const float3 point, f32 speed, bool attract,
    bool affectX, bool affectY, bool affectZ)
  : Point(point), Speed(speed), AffectX(affectX), AffectY(affectY),
    AffectZ(affectZ), Attract(attract), LastTime(0)
{
#ifdef _DEBUG
  setDebugName("CParticleAttractionAffector");
#endif
}
//! Affects an array of particles.
void CParticleAttractionAffector::affect(u32 now, SParticle* particlearray, u32 count)
{
  if (LastTime == 0) {
    LastTime = now;
    return;
  }
  f32 timeDelta = (now - LastTime) / 1000.0f;
  LastTime = now;
  if (!Enabled) {
    return;
  }
  int i;
  for (i = 0; i < count; ++i) {
    float3 direction = (Point - particlearray[i].pos).normalize();
    direction *= Speed * timeDelta;
    if (!Attract) {
      direction *= -1.0f;
    }
    if (AffectX) {
      particlearray[i].pos.X += direction.X;
    }
    if (AffectY) {
      particlearray[i].pos.Y += direction.Y;
    }
    if (AffectZ) {
      particlearray[i].pos.Z += direction.Z;
    }
  }
}
//! Writes attributes of the object.
void CParticleAttractionAffector::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const
{
  out->addVector3d("Point", Point);
  out->addFloat("Speed", Speed);
  out->addBool("AffectX", AffectX);
  out->addBool("AffectY", AffectY);
  out->addBool("AffectZ", AffectZ);
  out->addBool("Attract", Attract);
}
//! Reads attributes of the object.
void CParticleAttractionAffector::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options)
{
  Point = in->getAttributeAsVector3d("Point");
  Speed = in->getAttributeAsFloat("Speed");
  AffectX = in->getAttributeAsBool("AffectX");
  AffectY = in->getAttributeAsBool("AffectY");
  AffectZ = in->getAttributeAsBool("AffectZ");
  Attract = in->getAttributeAsBool("Attract");
}

