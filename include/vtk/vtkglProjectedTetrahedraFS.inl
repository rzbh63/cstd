/* DO NOT EDIT.
 * Generated by ..\..\bin\Debug\vtkEncodeString-7.1.exe
 * 
 * Define the vtkglProjectedTetrahedraFS string.
 *
 * Generated from file: D:/code/lib/VTK-7.1.1/Rendering/VolumeOpenGL2/shaders/vtkglProjectedTetrahedraFS.glsl
 */
#include "vtkglProjectedTetrahedraFS.h"
const char *vtkglProjectedTetrahedraFS =
"//VTK::System::Dec\n"
"\n"
"/*=========================================================================\n"
"\n"
"  Program:   Visualization Toolkit\n"
"  Module:    vtkglprojectedTetrahdraFS.glsl\n"
"\n"
"  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen\n"
"  All rights reserved.\n"
"  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.\n"
"\n"
"     This software is distributed WITHOUT ANY WARRANTY; without even\n"
"     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR\n"
"     PURPOSE.  See the above copyright notice for more information.\n"
"\n"
"=========================================================================*/\n"
"\n"
"//VTK::Output::Dec\n"
"\n"
"varying vec3 fcolor;\n"
"varying float fdepth;\n"
"varying float fattenuation;\n"
"\n"
"void main()\n"
"{\n"
"  // the following exp is done in the fragment shader\n"
"  // because linear interpolation (from the VS) of the resulting\n"
"  // value would not match the exp of the interpolated\n"
"  // source values\n"
"  float opacity = 1.0 - exp(-1.0*fattenuation*fdepth);\n"
"\n"
"\n"
"  gl_FragData[0] =  vec4(fcolor,opacity);\n"
"\n"
"  if (gl_FragData[0].a <= 0.0)\n"
"    {\n"
"    discard;\n"
"    }\n"
"}\n"
"\n";

