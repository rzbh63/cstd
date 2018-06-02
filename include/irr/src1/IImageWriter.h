// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef _I_IMAGE_WRITER_H_INCLUDED__
#define _I_IMAGE_WRITER_H_INCLUDED__
//#include "irrString.h"
class IWriteFile;
class IImage;
//! Interface for writing software image data.
class IImageWriter
{
public:
  //! Check if this writer can write a file with the given extension
  /** \param filename Name of the file to check.
  \return True if file extension specifies a writable type. */
  bool isAWriteableFileExtension(const char* filename) = 0;
  //! Write image to file
  /** \param file File handle to write to.
  \param image Image to write into file.
  \param param Writer specific parameter, influencing e.g. quality.
  \return True if image was successfully written. */
  bool writeImage(IWriteFile* file, IImage* image, u32 param = 0) = 0;
};
//  video
//  irr
#endif // _I_IMAGE_WRITER_H_INCLUDED__

