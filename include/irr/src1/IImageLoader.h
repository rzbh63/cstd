// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_SURFACE_LOADER_H_INCLUDED__
#define __I_SURFACE_LOADER_H_INCLUDED__
//#include "IImage.h"
class IReadFile;
//! Class which is able to create a image from a file.
/** If you want the Irrlicht Engine be able to load textures of
currently unsupported file formats (e.g .gif), then implement
this and add your new Surface loader with
IVideoDriver::addExternalImageLoader() to the engine. */
class IImageLoader
{
public:
  //! Check if the file might be loaded by this class
  /** Check is based on the file extension (e.g. ".tga")
  \param filename Name of file to check.
  \return True if file seems to be loadable. */
  bool isALoadableFileExtension(const char* filename) = 0;
  //! Check if the file might be loaded by this class
  /** Check might look into the file.
  \param file File handle to check.
  \return True if file seems to be loadable. */
  bool isALoadableFileFormat(IReadFile* file) = 0;
  //! Creates a surface from the file
  /** \param file File handle to check.
  \return Pointer to newly created image, or 0 upon error. */
  IImage* loadImage(IReadFile* file) = 0;
};
#endif
