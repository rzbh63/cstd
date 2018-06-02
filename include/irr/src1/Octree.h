// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_OCTREE_H_INCLUDED__
#define __C_OCTREE_H_INCLUDED__
//#include "SViewFrustum.h"
//#include "irrArray.h"
//#include "CMeshBuffer.h"
/**
  Flags for Octree
*/
//! use meshbuffer for drawing, enables VBO usage
#define OCTREE_USE_HARDWARE false
//! use visibility information together with VBOs
#define OCTREE_USE_VISIBILITY true
//! use bounding box or frustum for calculate polys
#define OCTREE_BOX_BASED true
//! bypass full invisible/visible test
#define OCTREE_PARENTTEST
//! template octree.
/** T must be a vertex type which has a member
called .Pos, which is a vertex3df position. */
template <class T>
class Octree
{
public:
  struct SMeshChunk : public CMeshBuffer<T> {
    SMeshChunk()
      : CMeshBuffer<T>(), MaterialId(0) {
      CMeshBuffer<T>& me = *this;
      me.grab();
    }
    ~SMeshChunk() {
      //removeAllHardwareBuffers
    }
    s32 MaterialId;
  };
  struct SIndexChunk {
    u16* Indices;
    s32 MaterialId;
  };
  struct SIndexData {
    u16* Indices;
    s32 CurrentSize;
    s32 MaxSize;
  };
  //! Constructor
  Octree(const SMeshChunk > & meshes, s32 minimalPolysPerNode = 128) :
    IndexData(0), IndexDataCount(meshes_size), NodeCount(0) {
    IndexData = new SIndexData[IndexDataCount];
    // construct array of all indices
    SIndexChunk** indexChunks = new SIndexChunk*;
    indexChunks->reallocate(meshes_size);
    int i;
    for (i = 0; i != meshes_size; ++i) {
      IndexData[i].CurrentSize = 0;
      IndexData[i].MaxSize = meshes[i].Indices_size;
      IndexData[i].Indices = new u16[IndexData[i].MaxSize];
      indexChunks->push_back(SIndexChunk());
      SIndexChunk& tic = indexChunks->getLast();
      tic.MaterialId = meshes[i].MaterialId;
      tic.Indices = meshes[i].Indices;
    }
    // create tree
    Root = new OctreeNode(NodeCount, 0, meshes, indexChunks, minimalPolysPerNode);
  }
  //! returns all ids of polygons partially or fully enclosed
  //! by this bounding box.
  void calculatePolys(const F3AABBOX& box) {
    int i;
    for (i = 0; i != IndexDataCount; ++i) {
      IndexData[i].CurrentSize = 0;
    }
    Root->getPolys(box, IndexData, 0);
  }
  //! returns all ids of polygons partially or fully enclosed
  //! by a view frustum.
  void calculatePolys(const SViewFrustum& frustum) {
    int i;
    for (i = 0; i != IndexDataCount; ++i) {
      IndexData[i].CurrentSize = 0;
    }
    Root->getPolys(frustum, IndexData, 0);
  }
  const SIndexData* getIndexData() const {
    return IndexData;
  }
  u32 getIndexDataCount() const {
    return IndexDataCount;
  }
  u32 getNodeCount() const {
    return NodeCount;
  }
  //! for debug purposes only, collects the bounding boxes of the tree
  void getBoundingBoxes(const F3AABBOX& box,
      const F3AABBOX** outBoxes) const {
    Root->getBoundingBoxes(box, outBoxes);
  }
  //! destructor
  ~Octree() {
    int i;
    for (i = 0; i < IndexDataCount; ++i) {
      delete [] IndexData[i].Indices;
    }
    delete [] IndexData;
    delete Root;
  }
private:
  // private inner class
  class OctreeNode
  {
  public:
    // constructor
    OctreeNode(u32& nodeCount, u32 currentdepth,
        const SMeshChunk > & allmeshdata,
        SIndexChunk** indices,
        s32 minimalPolysPerNode) : IndexData(0),
      Depth(currentdepth + 1) {
      ++nodeCount;
      u32 i; // new ISO for scoping problem with different compilers
      for (i = 0; i != 8; ++i) {
        Children[i] = 0;
      }
      if (indices->empty()) {
        delete indices;
        return;
      }
      bool found = false;
      // find first point for bounding box
      for (i = 0; i < indices->size(); ++i) {
        if (!(*indices)[i].Indices.empty()) {
          Box.reset(allmeshdata[i].Vertices[(*indices)[i].Indices[0]].Pos);
          found = true;
          break;
        }
      }
      if (!found) {
        delete indices;
        return;
      }
      s32 totalPrimitives = 0;
      // now lets calculate our bounding box
      for (i = 0; i < indices->size(); ++i) {
        totalPrimitives += (*indices)[i].Indices_size;
        for (int j = 0; j < (*indices)[i].Indices_size; ++j) {
          Box.addInternalPoint(allmeshdata[i].Vertices[(*indices)[i].Indices[j]].Pos);
        }
      }
      float3 middle = Box.getCenter();
      float3 edges[8];
      Box.getEdges(edges);
      // calculate all children
      F3AABBOX box;
      u16* keepIndices;
      if (totalPrimitives > minimalPolysPerNode && !Box.isEmpty())
        for (int ch = 0; ch != 8; ++ch) {
          box.reset(middle);
          box.addInternalPoint(edges[ch]);
          // create indices for child
          bool added = false;
          SIndexChunk** cindexChunks = new SIndexChunk*;
          cindexChunks->reallocate(allmeshdata_size);
          for (i = 0; i < allmeshdata_size; ++i) {
            cindexChunks->push_back(SIndexChunk());
            SIndexChunk& tic = cindexChunks->getLast();
            tic.MaterialId = allmeshdata[i].MaterialId;
            for (int t = 0; t < (*indices)[i].Indices_size; t += 3) {
              if (box.isPointInside(allmeshdata[i].Vertices[(*indices)[i].Indices[t]].Pos) &&
                  box.isPointInside(allmeshdata[i].Vertices[(*indices)[i].Indices[t + 1]].Pos) &&
                  box.isPointInside(allmeshdata[i].Vertices[(*indices)[i].Indices[t + 2]].Pos)) {
                tic.Indices.push_back((*indices)[i].Indices[t]);
                tic.Indices.push_back((*indices)[i].Indices[t + 1]);
                tic.Indices.push_back((*indices)[i].Indices[t + 2]);
                added = true;
              }
              else {
                keepIndices.push_back((*indices)[i].Indices[t]);
                keepIndices.push_back((*indices)[i].Indices[t + 1]);
                keepIndices.push_back((*indices)[i].Indices[t + 2]);
              }
            }
            (*indices)[i].Indices.set_used(keepIndices_size);
            memcpy((*indices)[i].Indices, keepIndices, keepIndices_size * sizeof(u16));
            keepIndices.set_used(0);
          }
          if (added)
            Children[ch] = new OctreeNode(nodeCount, Depth,
                allmeshdata, cindexChunks, minimalPolysPerNode);
          else {
            delete cindexChunks;
          }
        } // end for all possible children
      IndexData = indices;
    }
    // destructor
    ~OctreeNode() {
      delete IndexData;
      int i;
      for (i = 0; i < 8; ++i) {
        delete Children[i];
      }
    }
    // returns all ids of polygons partially or full enclosed
    // by this bounding box.
    void getPolys(const F3AABBOX& box, SIndexData* idxdata, u32 parentTest) const {
#if defined (OCTREE_PARENTTEST )
      // if not full inside
      if (parentTest != 2) {
        // partially inside ?
        if (!Box.intersectsWithBox(box)) {
          return;
        }
        // fully inside ?
        parentTest = Box.isFullInside(box) ? 2 : 1;
      }
#else
      if (Box.intersectsWithBox(box))
#endif
      {
        const u32 cnt = IndexData->size();
        u32 i; // new ISO for scoping problem in some compilers
        for (i = 0; i < cnt; ++i) {
          const s32 idxcnt = (*IndexData)[i].Indices_size;
          if (idxcnt) {
            memcpy(&idxdata[i].Indices[idxdata[i].CurrentSize],
                &(*IndexData)[i].Indices[0], idxcnt * sizeof(s16));
            idxdata[i].CurrentSize += idxcnt;
          }
        }
        for (i = 0; i != 8; ++i)
          if (Children[i]) {
            Children[i]->getPolys(box, idxdata, parentTest);
          }
      }
    }
    // returns all ids of polygons partially or full enclosed
    // by the view frustum.
    void getPolys(const SViewFrustum& frustum, SIndexData* idxdata, u32 parentTest) const {
      u32 i; // new ISO for scoping problem in some compilers
      // if parent is fully inside, no further check for the children is needed
#if defined (OCTREE_PARENTTEST )
      if (parentTest != 2)
#endif
      {
#if defined (OCTREE_PARENTTEST )
        parentTest = 2;
#endif
        for (i = 0; i != SViewFrustum::VF_PLANE_COUNT; ++i) {
          EIntersectionRelation3D r = Box.classifyPlaneRelation(frustum.planes[i]);
          if (r == ISREL3D_FRONT) {
            return;
          }
#if defined (OCTREE_PARENTTEST )
          if (r == ISREL3D_CLIPPED) {
            parentTest = 1;  // must still check children
          }
#endif
        }
      }
      const u32 cnt = IndexData->size();
      for (i = 0; i != cnt; ++i) {
        s32 idxcnt = (*IndexData)[i].Indices_size;
        if (idxcnt) {
          memcpy(&idxdata[i].Indices[idxdata[i].CurrentSize],
              &(*IndexData)[i].Indices[0], idxcnt * sizeof(s16));
          idxdata[i].CurrentSize += idxcnt;
        }
      }
      for (i = 0; i != 8; ++i)
        if (Children[i]) {
          Children[i]->getPolys(frustum, idxdata, parentTest);
        }
    }
    //! for debug purposes only, collects the bounding boxes of the node
    void getBoundingBoxes(const F3AABBOX& box,
        const F3AABBOX** outBoxes) const {
      if (Box.intersectsWithBox(box)) {
        outBoxes.push_back(&Box);
        int i;
        for (i = 0; i != 8; ++i)
          if (Children[i]) {
            Children[i]->getBoundingBoxes(box, outBoxes);
          }
      }
    }
  private:
    aabbox3df Box;
    SIndexChunk** IndexData;
    OctreeNode* Children[8];
    u32 Depth;
  };
  OctreeNode* Root;
  SIndexData* IndexData;
  u32 IndexDataCount;
  u32 NodeCount;
};
#endif

