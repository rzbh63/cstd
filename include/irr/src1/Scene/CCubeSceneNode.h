// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_CUBE_SCENE_NODE_H_INCLUDED__
#define __C_CUBE_SCENE_NODE_H_INCLUDED__
//#include "IMeshSceneNode.h"
//#include "SMesh.h"
class CCubeSceneNode : public IMeshSceneNode
{
public:
  //! constructor
  CCubeSceneNode(f32 size, ISceneNode* parent, ISceneManager* mgr, s32 id,
      const float3 position = float3(0, 0, 0),
      const float3 rotation = float3(0, 0, 0),
      const float3 scale = float3(1.0f, 1.0f, 1.0f));
  ~CCubeSceneNode();
  void OnRegisterSceneNode();
  //! renders the node.
  void render();
  //! returns the axis aligned bounding box of this node
  const F3AABBOX& getBoundingBox() const;
  //! returns the material based on the zero based index i. To get the amount
  //! of materials used by this scene node, use getMaterialCount().
  //! This function is needed for inserting the node into the scene hirachy on a
  //! optimal position for minimizing renderstate changes, but can also be used
  //! to directly modify the material of a scene node.
  SMaterial& getMaterial(u32 i);
  //! returns amount of materials used by this scene node.
  u32 getMaterialCount() const;
  //! Returns type of the scene node
  ESCENE_NODE_TYPE getType() const {
    return ESNT_CUBE;
  }
  //! Creates shadow volume scene node as child of this node
  //! and returns a pointer to it.
  IShadowVolumeSceneNode* addShadowVolumeSceneNode(const IMesh* shadowMesh,
      s32 id, bool zfailmethod = true, f32 infinity = 10000.0f);
  //! Writes attributes of the scene node.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;
  //! Reads attributes of the scene node.
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);
  //! Creates a clone of this scene node and its children.
  ISceneNode* clone(ISceneNode* newParent = 0, ISceneManager* newManager = 0);
  //! Sets a new mesh to display
  void setMesh(IMesh* mesh) {}
  //! Returns the current mesh
  IMesh* getMesh(void) {
    return Mesh;
  }
  //! Sets if the scene node should not copy the materials of the mesh but use them in a read only style.
  /* In this way it is possible to change the materials a mesh causing all mesh scene nodes
  referencing this mesh to change too. */
  void setReadOnlyMaterials(bool readonly) {}
  //! Returns if the scene node should not copy the materials of the mesh but use them in a read only style
  bool isReadOnlyMaterials() const {
    return false;
  }
  //! Removes a child from this scene node.
  //! Implemented here, to be able to remove the shadow properly, if there is one,
  //! or to remove attached childs.
  bool removeChild(ISceneNode* child);
private:
  void setSize();
  IMesh* Mesh;
  IShadowVolumeSceneNode* Shadow;
  f32 Size;
};
#endif

