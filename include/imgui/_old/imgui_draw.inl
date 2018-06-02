// dear imgui, v1.50 WIP
// (drawing and font code)
// Contains implementation for
// - ImDrawList
// - ImDrawData
// - ImFontAtlas
// - ImFont
// - Default font data
#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_PLACEMENT_NEW
#include "imgui_internal.h"
#include <stdio.h> // vsnprintf, sscanf, printf
#if !defined(alloca)
#ifdef _WIN32
#include <malloc.h> // alloca
#elif (defined(__FreeBSD__) || defined(FreeBSD_kernel) || defined(__DragonFly__)) && !defined(__GLIBC__)
#include <stdlib.h> // alloca. FreeBSD uses stdlib.h unless GLIBC
#else
#include <alloca.h> // alloca
#endif
#endif
#ifdef _MSC_VER
#pragma warning (disable: 4505) // unreferenced local function has been removed (stb stuff)
#pragma warning (disable: 4996) // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#define snprintf _snprintf
#endif
#ifdef __clang__
#pragma clang diagnostic ignored "-Wold-style-cast" // warning : use of old-style cast // yes, they are more terse.
#pragma clang diagnostic ignored "-Wfloat-equal" // warning : comparing floating point with == or != is unsafe // storing and comparing against same constants ok.
#pragma clang diagnostic ignored "-Wglobal-constructors" // warning : declaration requires a global destructor // similar to above, not sure what the exact difference it.
#pragma clang diagnostic ignored "-Wsign-conversion" // warning : implicit conversion changes signedness //
#pragma clang diagnostic ignored "-Wreserved-id-macro" // warning : macro name is a reserved identifier //
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-function" // warning: 'xxxx' defined but not used
#pragma GCC diagnostic ignored "-Wdouble-promotion" // warning: implicit conversion from 'float' to 'double' when passing argument to function
#pragma GCC diagnostic ignored "-Wconversion" // warning: conversion to 'xxxx' from 'xxxx' may alter its value
#endif
//-------------------------------------------------------------------------
// STB libraries implementation
//-------------------------------------------------------------------------
//#define IMGUI_STB_NAMESPACE ImGuiStb
//#define IMGUI_DISABLE_STB_RECT_PACK_IMPLEMENTATION
//#define IMGUI_DISABLE_STB_TRUETYPE_IMPLEMENTATION
#ifdef IMGUI_STB_NAMESPACE
namespace IMGUI_STB_NAMESPACE
{
#endif
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4456) // declaration of 'xx' hides previous local declaration
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast" // warning : use of old-style cast // yes, they are more terse.
#pragma clang diagnostic ignored "-Wunused-function"
#pragma clang diagnostic ignored "-Wmissing-prototypes"
#endif
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits" // warning: comparison is always true due to limited range of data type [-Wtype-limits]
#endif
#define STBRP_ASSERT(x) IM_ASSERT(x)
#ifndef IMGUI_DISABLE_STB_RECT_PACK_IMPLEMENTATION
#define STBRP_STATIC
#define STB_RECT_PACK_IMPLEMENTATION
#endif
#include "stb_rect_pack.h"
#define STBTT_malloc(x,u) ((void)(u), ImGui_MemAlloc(x))
#define STBTT_free(x,u) ((void)(u), ImGui_MemFree(x))
#define STBTT_assert(x) IM_ASSERT(x)
#ifndef IMGUI_DISABLE_STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#define STB_TRUETYPE_IMPLEMENTATION
#else
#define STBTT_DEF extern
#endif
#include "stb_truetype.h"
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef _MSC_VER
#pragma warning (pop)
#endif
#ifdef IMGUI_STB_NAMESPACE
} // namespace ImGuiStb
using namespace IMGUI_STB_NAMESPACE;
#endif
//-----------------------------------------------------------------------------
// ImDrawList
//-----------------------------------------------------------------------------
static const ImVec4 GNullClipRect = { -8192.0f, -8192.0f, +8192.0f, +8192.0f}; // Large values that are easy to encode in a few bits+shift
void ImDrawList_Clear(ImDrawList* g)
{
  g->CmdBuffer.resize(0);
  g->IdxBuffer.resize(0);
  g->VtxBuffer.resize(0);
  g->_VtxCurrentIdx = 0;
  g->_VtxWritePtr = NULL;
  g->_IdxWritePtr = NULL;
  g->_ClipRectStack.resize(0);
  g->_TextureIdStack.resize(0);
  g->_Path.resize(0);
  g->_ChannelsCurrent = 0;
  g->_ChannelsCount = 1;
  // NB: Do not clear channels so our allocations are re-used after the first frame.
}
void ImDrawList_Init(ImDrawList* g)
{
  g->_OwnerName = NULL;
  memset(g, 0, sizeof(*g));
  ImDrawList_Clear(g);
}
void ImDrawList_ClearFreeMemory(ImDrawList* g)
{
  int i;
  g->CmdBuffer.clear();
  g->IdxBuffer.clear();
  g->VtxBuffer.clear();
  g->_VtxCurrentIdx = 0;
  g->_VtxWritePtr = NULL;
  g->_IdxWritePtr = NULL;
  g->_ClipRectStack.clear();
  g->_TextureIdStack.clear();
  g->_Path.clear();
  g->_ChannelsCurrent = 0;
  g->_ChannelsCount = 1;
  for (i = 0; i < g->_Channels.Size; i++) {
    if (i == 0) {
      memset(&g->_Channels[0], 0, sizeof(g->_Channels[0])); // channel 0 is a copy of CmdBuffer/IdxBuffer, don't destruct again
    }
    g->_Channels[i].CmdBuffer.clear();
    g->_Channels[i].IdxBuffer.clear();
  }
  g->_Channels.clear();
}
// Use macros because C++ is a terrible language, we want guaranteed inline, no code in header, and no overhead in Debug mode
#define GetCurrentClipRect() (g->_ClipRectStack.Size ? g->_ClipRectStack.Data[g->_ClipRectStack.Size-1] : GNullClipRect)
#define GetCurrentTextureId() (g->_TextureIdStack.Size ? g->_TextureIdStack.Data[g->_TextureIdStack.Size-1] : NULL)
void ImDrawList_AddDrawCmd(ImDrawList* g)
{
  ImDrawCmd draw_cmd;//(0);// = {0};
  ImDrawCmd_init(&draw_cmd);
  draw_cmd.ClipRect = GetCurrentClipRect();
  draw_cmd.TextureId = GetCurrentTextureId();
  IM_ASSERT(draw_cmd.ClipRect.x <= draw_cmd.ClipRect.z && draw_cmd.ClipRect.y <= draw_cmd.ClipRect.w);
  g->CmdBuffer.push_back(draw_cmd);
}
void ImDrawList_AddCallback(ImDrawList* g, ImDrawCallback callback, void* callback_data)
{
  ImDrawCmd* current_cmd = g->CmdBuffer.Size ? &g->CmdBuffer.back() : NULL;
  if (!current_cmd || current_cmd->ElemCount != 0 || current_cmd->UserCallback != NULL) {
    ImDrawList_AddDrawCmd(g);
    current_cmd = &g->CmdBuffer.back();
  }
  current_cmd->UserCallback = callback;
  current_cmd->UserCallbackData = callback_data;
  ImDrawList_AddDrawCmd(g); // Force a new command after us (see comment below)
}
// Our scheme may appears a bit unusual, basically we want the most-common calls AddLine AddRect etc. to not have to perform any check so we always have a command ready in the stack.
// The cost of figuring out if a new command has to be added or if we can merge is paid in those Update** functions only.
void ImDrawList_UpdateClipRect(ImDrawList* g)
{
  // If current command is used with different settings we need to add a new command
  const ImVec4 curr_clip_rect = GetCurrentClipRect();
  ImDrawCmd* curr_cmd = g->CmdBuffer.Size > 0 ? &g->CmdBuffer.Data[g->CmdBuffer.Size - 1] : NULL;
  if (!curr_cmd || (curr_cmd->ElemCount != 0 && memcmp(&curr_cmd->ClipRect, &curr_clip_rect, sizeof(ImVec4)) != 0) || curr_cmd->UserCallback != NULL) {
    ImDrawList_AddDrawCmd(g);
    return;
  }
  // Try to merge with previous command if it matches, else use current command
  ImDrawCmd* prev_cmd = g->CmdBuffer.Size > 1 ? curr_cmd - 1 : NULL;
  if (curr_cmd->ElemCount == 0 && prev_cmd && memcmp(&prev_cmd->ClipRect, &curr_clip_rect, sizeof(ImVec4)) == 0 && prev_cmd->TextureId == GetCurrentTextureId() && prev_cmd->UserCallback == NULL) {
    g->CmdBuffer.pop_back();
  }
  else {
    curr_cmd->ClipRect = curr_clip_rect;
  }
}
void ImDrawList_UpdateTextureID(ImDrawList* g)
{
  // If current command is used with different settings we need to add a new command
  const ImTextureID curr_texture_id = GetCurrentTextureId();
  ImDrawCmd* curr_cmd = g->CmdBuffer.Size ? &g->CmdBuffer.back() : NULL;
  if (!curr_cmd || (curr_cmd->ElemCount != 0 && curr_cmd->TextureId != curr_texture_id) || curr_cmd->UserCallback != NULL) {
    ImDrawList_AddDrawCmd(g);
    return;
  }
  // Try to merge with previous command if it matches, else use current command
  ImDrawCmd* prev_cmd = g->CmdBuffer.Size > 1 ? curr_cmd - 1 : NULL;
  if (prev_cmd && prev_cmd->TextureId == curr_texture_id && memcmp(&prev_cmd->ClipRect, &GetCurrentClipRect(), sizeof(ImVec4)) == 0 && prev_cmd->UserCallback == NULL) {
    g->CmdBuffer.pop_back();
  }
  else {
    curr_cmd->TextureId = curr_texture_id;
  }
}
#undef GetCurrentClipRect
#undef GetCurrentTextureId
// Render-level scissoring. This is passed down to your render function but not used for CPU-side coarse clipping. Prefer using higher-level ImGui_PushClipRect() to affect logic (hit-testing and widget culling)
void ImDrawList_PushClipRect(ImDrawList* g, ImVec2 cr_min, ImVec2 cr_max, bool intersect_with_current_clip_rect)
{
  ImVec4 cr = fVec4(cr_min.x, cr_min.y, cr_max.x, cr_max.y);
  if (intersect_with_current_clip_rect && g->_ClipRectStack.Size) {
    ImVec4 current = g->_ClipRectStack.Data[g->_ClipRectStack.Size - 1];
    if (cr.x < current.x) {
      cr.x = current.x;
    }
    if (cr.y < current.y) {
      cr.y = current.y;
    }
    if (cr.z > current.z) {
      cr.z = current.z;
    }
    if (cr.w > current.w) {
      cr.w = current.w;
    }
  }
  cr.z = ImMax(cr.x, cr.z);
  cr.w = ImMax(cr.y, cr.w);
  g->_ClipRectStack.push_back(cr);
  ImDrawList_UpdateClipRect(g);
}
void ImDrawList_PushClipRectFullScreen(ImDrawList* g)
{
  ImDrawList_PushClipRect(g, fVec2(GNullClipRect.x, GNullClipRect.y), fVec2(GNullClipRect.z, GNullClipRect.w), NULL);
  //PushClipRect(GetVisibleRect()); // FIXME-OPT: This would be more correct but we're not supposed to access ImGuiContext from here?
}
void ImDrawList_PopClipRect(ImDrawList* g)
{
  IM_ASSERT(g->_ClipRectStack.Size > 0);
  g->_ClipRectStack.pop_back();
  ImDrawList_UpdateClipRect(g);
}
void ImDrawList_PushTextureID(ImDrawList* g, const ImTextureID& texture_id)
{
  g->_TextureIdStack.push_back(texture_id);
  ImDrawList_UpdateTextureID(g);
}
void ImDrawList_PopTextureID(ImDrawList* g)
{
  IM_ASSERT(g->_TextureIdStack.Size > 0);
  g->_TextureIdStack.pop_back();
  ImDrawList_UpdateTextureID(g);
}
void ImDrawList_ChannelsSplit(ImDrawList* g, int channels_count)
{
  int i;
  IM_ASSERT(g->_ChannelsCurrent == 0 && g->_ChannelsCount == 1);
  int old_channels_count = g->_Channels.Size;
  if (old_channels_count < channels_count) {
    g->_Channels.resize(channels_count);
  }
  g->_ChannelsCount = channels_count;
  // _Channels[] (24 bytes each) hold storage that we'll swap with this->_CmdBuffer/_IdxBuffer
  // The content of _Channels[0] at this point doesn't matter. We clear it to make state tidy in a debugger but we don't strictly need to.
  // When we switch to the next channel, we'll copy _CmdBuffer/_IdxBuffer into _Channels[0] and then _Channels[1] into _CmdBuffer/_IdxBuffer
  memset(&g->_Channels[0], 0, sizeof(ImDrawChannel));
  for (i = 1; i < channels_count; i++) {
    if (i >= old_channels_count) {
      IM_PLACEMENT_NEW(&g->_Channels[i]) ImDrawChannel();
    }
    else {
      g->_Channels[i].CmdBuffer.resize(0);
      g->_Channels[i].IdxBuffer.resize(0);
    }
    if (g->_Channels[i].CmdBuffer.Size == 0) {
      ImDrawCmd draw_cmd;
      ImDrawCmd_init(&draw_cmd);
      draw_cmd.ClipRect = g->_ClipRectStack.back();
      draw_cmd.TextureId = g->_TextureIdStack.back();
      g->_Channels[i].CmdBuffer.push_back(draw_cmd);
    }
  }
}
void ImDrawList_ChannelsSetCurrent(ImDrawList* g, int idx)
{
  IM_ASSERT(idx >= 0 && idx < g->_ChannelsCount);
  IM_ASSERT(g->_ChannelsCurrent >= 0 && g->_ChannelsCurrent < g->_ChannelsCount);
  if (g->_ChannelsCurrent == idx) {
    return;
  }
  memcpy(&g->_Channels.Data[g->_ChannelsCurrent].CmdBuffer, &g->CmdBuffer, sizeof(g->CmdBuffer)); // copy 12 bytes, four times
  memcpy(&g->_Channels.Data[g->_ChannelsCurrent].IdxBuffer, &g->IdxBuffer, sizeof(g->IdxBuffer));
  g->_ChannelsCurrent = idx;
  memcpy(&g->CmdBuffer, &g->_Channels.Data[g->_ChannelsCurrent].CmdBuffer, sizeof(g->CmdBuffer));
  memcpy(&g->IdxBuffer, &g->_Channels.Data[g->_ChannelsCurrent].IdxBuffer, sizeof(g->IdxBuffer));
  g->_IdxWritePtr = g->IdxBuffer.Data + g->IdxBuffer.Size;
}
void ImDrawList_ChannelsMerge(ImDrawList* g)
{
  int i;
  // Note that we never use or rely on channels.Size because it is merely a buffer that we never shrink back to 0 to keep all sub-buffers ready for use.
  if (g->_ChannelsCount <= 1) {
    return;
  }
  ImDrawList_ChannelsSetCurrent(g, 0);
  if (g->CmdBuffer.Size && g->CmdBuffer.back().ElemCount == 0) {
    g->CmdBuffer.pop_back();
  }
  int new_cmd_buffer_count = 0, new_idx_buffer_count = 0;
  for (i = 1; i < g->_ChannelsCount; i++) {
    ImDrawChannel& ch = g->_Channels[i];
    if (ch.CmdBuffer.Size && ch.CmdBuffer.back().ElemCount == 0) {
      ch.CmdBuffer.pop_back();
    }
    new_cmd_buffer_count += ch.CmdBuffer.Size;
    new_idx_buffer_count += ch.IdxBuffer.Size;
  }
  g->CmdBuffer.resize(g->CmdBuffer.Size + new_cmd_buffer_count);
  g->IdxBuffer.resize(g->IdxBuffer.Size + new_idx_buffer_count);
  ImDrawCmd* cmd_write = g->CmdBuffer.Data + g->CmdBuffer.Size - new_cmd_buffer_count;
  g->_IdxWritePtr = g->IdxBuffer.Data + g->IdxBuffer.Size - new_idx_buffer_count;
  for (i = 1; i < g->_ChannelsCount; i++) {
    ImDrawChannel& ch = g->_Channels[i];
    if (int sz = ch.CmdBuffer.Size) {
      memcpy(cmd_write, ch.CmdBuffer.Data, sz * sizeof(ImDrawCmd));
      cmd_write += sz;
    }
    if (int sz = ch.IdxBuffer.Size) {
      memcpy(g->_IdxWritePtr, ch.IdxBuffer.Data, sz * sizeof(ImDrawIdx));
      g->_IdxWritePtr += sz;
    }
  }
  ImDrawList_AddDrawCmd(g);
  g->_ChannelsCount = 1;
}
// NB: this can be called with negative count for removing primitives (as long as the result does not underflow)
void ImDrawList_PrimReserve(ImDrawList* g, int idx_count, int vtx_count)
{
  ImDrawCmd& draw_cmd = g->CmdBuffer.Data[g->CmdBuffer.Size - 1];
  draw_cmd.ElemCount += idx_count;
  int vtx_buffer_size = g->VtxBuffer.Size;
  g->VtxBuffer.resize(vtx_buffer_size + vtx_count);
  g->_VtxWritePtr = g->VtxBuffer.Data + vtx_buffer_size;
  int idx_buffer_size = g->IdxBuffer.Size;
  g->IdxBuffer.resize(idx_buffer_size + idx_count);
  g->_IdxWritePtr = g->IdxBuffer.Data + idx_buffer_size;
}
//-----------------------------------------------------------------------------
// DEFAULT FONT DATA
//-----------------------------------------------------------------------------
// Compressed with stb_compress() then converted to a C array.
// Use the program in extra_fonts/binary_to_compressed_c.cpp to create the array from a TTF file.
// Decompression from stb.h (public domain) by Sean Barrett https://github.com/nothings/stb/blob/master/stb.h
//-----------------------------------------------------------------------------
static unsigned int stb_decompress_length(unsigned char* input)
{
  return (input[8] << 24) + (input[9] << 16) + (input[10] << 8) + input[11];
}
static unsigned char* stb__barrier, *stb__barrier2, *stb__barrier3, *stb__barrier4;
static unsigned char* stb__dout;
static void stb__match(unsigned char* data, unsigned int length)
{
  // INVERSE of memmove... write each byte before copying the next...
  IM_ASSERT(stb__dout + length <= stb__barrier);
  if (stb__dout + length > stb__barrier) {
    stb__dout += length;
    return;
  }
  if (data < stb__barrier4) {
    stb__dout = stb__barrier + 1;
    return;
  }
  while (length--) {
    *stb__dout++ = *data++;
  }
}
static void stb__lit(unsigned char* data, unsigned int length)
{
  IM_ASSERT(stb__dout + length <= stb__barrier);
  if (stb__dout + length > stb__barrier) {
    stb__dout += length;
    return;
  }
  if (data < stb__barrier2) {
    stb__dout = stb__barrier + 1;
    return;
  }
  memcpy(stb__dout, data, length);
  stb__dout += length;
}
#define stb__in2(x) ((i[x] << 8) + i[(x)+1])
#define stb__in3(x) ((i[x] << 16) + stb__in2((x)+1))
#define stb__in4(x) ((i[x] << 24) + stb__in3((x)+1))
static unsigned char* stb_decompress_token(unsigned char* i)
{
  if (*i >= 0x20) { // use fewer if's for cases that expand small
    if (*i >= 0x80) {
      stb__match(stb__dout - i[1] - 1, i[0] - 0x80 + 1), i += 2;
    }
    else if (*i >= 0x40) {
      stb__match(stb__dout - (stb__in2(0) - 0x4000 + 1), i[2] + 1), i += 3;
    }
    else { /* *i >= 0x20 */
      stb__lit(i + 1, i[0] - 0x20 + 1), i += 1 + (i[0] - 0x20 + 1);
    }
  }
  else { // more ifs for cases that expand large, since overhead is amortized
    if (*i >= 0x18) {
      stb__match(stb__dout - (stb__in3(0) - 0x180000 + 1), i[3] + 1), i += 4;
    }
    else if (*i >= 0x10) {
      stb__match(stb__dout - (stb__in3(0) - 0x100000 + 1), stb__in2(3) + 1), i += 5;
    }
    else if (*i >= 0x08) {
      stb__lit(i + 2, stb__in2(0) - 0x0800 + 1), i += 2 + (stb__in2(0) - 0x0800 + 1);
    }
    else if (*i == 0x07) {
      stb__lit(i + 3, stb__in2(1) + 1), i += 3 + (stb__in2(1) + 1);
    }
    else if (*i == 0x06) {
      stb__match(stb__dout - (stb__in3(1) + 1), i[4] + 1), i += 5;
    }
    else if (*i == 0x04) {
      stb__match(stb__dout - (stb__in3(1) + 1), stb__in2(4) + 1), i += 6;
    }
  }
  return i;
}
static unsigned int stb_adler32(unsigned int adler32, unsigned char* buffer, unsigned int buflen)
{
  const unsigned long ADLER_MOD = 65521;
  unsigned long s1 = adler32 & 0xffff, s2 = adler32 >> 16;
  unsigned long blocklen, i;
  blocklen = buflen % 5552;
  while (buflen) {
    for (i = 0; i + 7 < blocklen; i += 8) {
      s1 += buffer[0], s2 += s1;
      s1 += buffer[1], s2 += s1;
      s1 += buffer[2], s2 += s1;
      s1 += buffer[3], s2 += s1;
      s1 += buffer[4], s2 += s1;
      s1 += buffer[5], s2 += s1;
      s1 += buffer[6], s2 += s1;
      s1 += buffer[7], s2 += s1;
      buffer += 8;
    }
    for (; i < blocklen; ++i) {
      s1 += *buffer++, s2 += s1;
    }
    s1 %= ADLER_MOD, s2 %= ADLER_MOD;
    buflen -= blocklen;
    blocklen = 5552;
  }
  return (unsigned int)(s2 << 16) + (unsigned int)s1;
}
static unsigned int stb_decompress(unsigned char* output, unsigned char* i, unsigned int length)
{
  unsigned int olen;
  if (stb__in4(0) != 0x57bC0000) {
    return 0;
  }
  if (stb__in4(4) != 0) {
    return 0; // error! stream is > 4GB
  }
  olen = stb_decompress_length(i);
  stb__barrier2 = i;
  stb__barrier3 = i + length;
  stb__barrier = output + olen;
  stb__barrier4 = output;
  i += 16;
  stb__dout = output;
  for (;;) {
    unsigned char* old_i = i;
    i = stb_decompress_token(i);
    if (i == old_i) {
      if (*i == 0x05 && i[1] == 0xfa) {
        IM_ASSERT(stb__dout == output + olen);
        if (stb__dout != output + olen) {
          return 0;
        }
        if (stb_adler32(1, output, olen) != (unsigned int) stb__in4(2)) {
          return 0;
        }
        return olen;
      }
      else {
        IM_ASSERT(0); /* NOTREACHED */
        return 0;
      }
    }
    IM_ASSERT(stb__dout <= output + olen);
    if (stb__dout > output + olen) {
      return 0;
    }
  }
}
//-----------------------------------------------------------------------------
// ProggyClean.ttf
// Copyright (c) 2004, 2005 Tristan Grimmer
// MIT license (see License.txt in http://www.upperbounds.net/download/ProggyClean.ttf.zip)
// Download and more information at http://upperbounds.net
//-----------------------------------------------------------------------------
// File: 'ProggyClean.ttf' (41208 bytes)
// Exported using binary_to_compressed_c.cpp
//-----------------------------------------------------------------------------
static const char* GetDefaultCompressedFontDataTTFBase85()
{
  static const char proggy_clean_ttf_compressed_data_base85[11980 + 1] =
      "7])#######hV0qs'/###[),##/l:$#Q6>##5[n42>c-TH`->>#/e>11NNV=Bv(*:.F?uu#(gRU.o0XGH`$vhLG1hxt9?W`#,5LsCp#-i>.r$<$6pD>Lb';9Crc6tgXmKVeU2cD4Eo3R/"
      "2*>]b(MC;$jPfY.;h^`IWM9<Lh2TlS+f-s$o6Q<BWH`YiU.xfLq$N;$0iR/GX:U(jcW2p/W*q?-qmnUCI;jHSAiFWM.R*kU@C=GH?a9wp8f$e.-4^Qg1)Q-GL(lf(r/7GrRgwV%MS=C#"
      "`8ND>Qo#t'X#(v#Y9w0#1D$CIf;W'#pWUPXOuxXuU(H9M(1<q-UE31#^-V'8IRUo7Qf./L>=Ke$$'5F%)]0^#0X@U.a<r:QLtFsLcL6##lOj)#.Y5<-R&KgLwqJfLgN&;Q?gI^#DY2uL"
      "i@^rMl9t=cWq6##weg>$FBjVQTSDgEKnIS7EM9>ZY9w0#L;>>#Mx&4Mvt//L[MkA#W@lK.N'[0#7RL_&#w+F%HtG9M#XL`N&.,GM4Pg;-<nLENhvx>-VsM.M0rJfLH2eTM`*oJMHRC`N"
      "kfimM2J,W-jXS:)r0wK#@Fge$U>`w'N7G#$#fB#$E^$#:9:hk+eOe--6x)F7*E%?76%^GMHePW-Z5l'&GiF#$956:rS?dA#fiK:)Yr+`&#0j@'DbG&#^$PG.Ll+DNa<XCMKEV*N)LN/N"
      "*b=%Q6pia-Xg8I$<MR&,VdJe$<(7G;Ckl'&hF;;$<_=X(b.RS%%)###MPBuuE1V:v&cX&#2m#(&cV]`k9OhLMbn%s$G2,B$BfD3X*sp5#l,$R#]x_X1xKX%b5U*[r5iMfUo9U`N99hG)"
      "tm+/Us9pG)XPu`<0s-)WTt(gCRxIg(%6sfh=ktMKn3j)<6<b5Sk_/0(^]AaN#(p/L>&VZ>1i%h1S9u5o@YaaW$e+b<TWFn/Z:Oh(Cx2$lNEoN^e)#CFY@@I;BOQ*sRwZtZxRcU7uW6CX"
      "ow0i(?$Q[cjOd[P4d)]>ROPOpxTO7Stwi1::iB1q)C_=dV26J;2,]7op$]uQr@_V7$q^%lQwtuHY]=DX,n3L#0PHDO4f9>dC@O>HBuKPpP*E,N+b3L#lpR/MrTEH.IAQk.a>D[.e;mc."
      "x]Ip.PH^'/aqUO/$1WxLoW0[iLA<QT;5HKD+@qQ'NQ(3_PLhE48R.qAPSwQ0/WK?Z,[x?-J;jQTWA0X@KJ(_Y8N-:/M74:/-ZpKrUss?d#dZq]DAbkU*JqkL+nwX@@47`5>w=4h(9.`G"
      "CRUxHPeR`5Mjol(dUWxZa(>STrPkrJiWx`5U7F#.g*jrohGg`cg:lSTvEY/EV_7H4Q9[Z%cnv;JQYZ5q.l7Zeas:HOIZOB?G<Nald$qs]@]L<J7bR*>gv:[7MI2k).'2($5FNP&EQ(,)"
      "U]W]+fh18.vsai00);D3@4ku5P?DP8aJt+;qUM]=+b'8@;mViBKx0DE[-auGl8:PJ&Dj+M6OC]O^((##]`0i)drT;-7X`=-H3[igUnPG-NZlo.#k@h#=Ork$m>a>$-?Tm$UV(?#P6YY#"
      "'/###xe7q.73rI3*pP/$1>s9)W,JrM7SN]'/4C#v$U`0#V.[0>xQsH$fEmPMgY2u7Kh(G%siIfLSoS+MK2eTM$=5,M8p`A.;_R%#u[K#$x4AG8.kK/HSB==-'Ie/QTtG?-.*^N-4B/ZM"
      "_3YlQC7(p7q)&](`6_c)$/*JL(L-^(]$wIM`dPtOdGA,U3:w2M-0<q-]L_?^)1vw'.,MRsqVr.L;aN&#/EgJ)PBc[-f>+WomX2u7lqM2iEumMTcsF?-aT=Z-97UEnXglEn1K-bnEO`gu"
      "Ft(c%=;Am_Qs@jLooI&NX;]0#j4#F14;gl8-GQpgwhrq8'=l_f-b49'UOqkLu7-##oDY2L(te+Mch&gLYtJ,MEtJfLh'x'M=$CS-ZZ%P]8bZ>#S?YY#%Q&q'3^Fw&?D)UDNrocM3A76/"
      "/oL?#h7gl85[qW/NDOk%16ij;+:1a'iNIdb-ou8.P*w,v5#EI$TWS>Pot-R*H'-SEpA:g)f+O$%%`kA#G=8RMmG1&O`>to8bC]T&$,n.LoO>29sp3dt-52U%VM#q7'DHpg+#Z9%H[K<L"
      "%a2E-grWVM3@2=-k22tL]4$##6We'8UJCKE[d_=%wI;'6X-GsLX4j^SgJ$##R*w,vP3wK#iiW&#*h^D&R?jp7+/u&#(AP##XU8c$fSYW-J95_-Dp[g9wcO&#M-h1OcJlc-*vpw0xUX&#"
      "OQFKNX@QI'IoPp7nb,QU//MQ&ZDkKP)X<WSVL(68uVl&#c'[0#(s1X&xm$Y%B7*K:eDA323j998GXbA#pwMs-jgD$9QISB-A_(aN4xoFM^@C58D0+Q+q3n0#3U1InDjF682-SjMXJK)("
      "h$hxua_K]ul92%'BOU&#BRRh-slg8KDlr:%L71Ka:.A;%YULjDPmL<LYs8i#XwJOYaKPKc1h:'9Ke,g)b),78=I39B;xiY$bgGw-&.Zi9InXDuYa%G*f2Bq7mn9^#p1vv%#(Wi-;/Z5h"
      "o;#2:;%d&#x9v68C5g?ntX0X)pT`;%pB3q7mgGN)3%(P8nTd5L7GeA-GL@+%J3u2:(Yf>et`e;)f#Km8&+DC$I46>#Kr]]u-[=99tts1.qb#q72g1WJO81q+eN'03'eM>&1XxY-caEnO"
      "j%2n8)),?ILR5^.Ibn<-X-Mq7[a82Lq:F&#ce+S9wsCK*x`569E8ew'He]h:sI[2LM$[guka3ZRd6:t%IG:;$%YiJ:Nq=?eAw;/:nnDq0(CYcMpG)qLN4$##&J<j$UpK<Q4a1]MupW^-"
      "sj_$%[HK%'F####QRZJ::Y3EGl4'@%FkiAOg#p[##O`gukTfBHagL<LHw%q&OV0##F=6/:chIm0@eCP8X]:kFI%hl8hgO@RcBhS-@Qb$%+m=hPDLg*%K8ln(wcf3/'DW-$.lR?n[nCH-"
      "eXOONTJlh:.RYF%3'p6sq:UIMA945&^HFS87@$EP2iG<-lCO$%c`uKGD3rC$x0BL8aFn--`ke%#HMP'vh1/R&O_J9'um,.<tx[@%wsJk&bUT2`0uMv7gg#qp/ij.L56'hl;.s5CUrxjO"
      "M7-##.l+Au'A&O:-T72L]P`&=;ctp'XScX*rU.>-XTt,%OVU4)S1+R-#dg0/Nn?Ku1^0f$B*P:Rowwm-`0PKjYDDM'3]d39VZHEl4,.j']Pk-M.h^&:0FACm$maq-&sgw0t7/6(^xtk%"
      "LuH88Fj-ekm>GA#_>568x6(OFRl-IZp`&b,_P'$M<Jnq79VsJW/mWS*PUiq76;]/NM_>hLbxfc$mj`,O;&%W2m`Zh:/)Uetw:aJ%]K9h:TcF]u_-Sj9,VK3M.*'&0D[Ca]J9gp8,kAW]"
      "%(?A%R$f<->Zts'^kn=-^@c4%-pY6qI%J%1IGxfLU9CP8cbPlXv);C=b),<2mOvP8up,UVf3839acAWAW-W?#ao/^#%KYo8fRULNd2.>%m]UK:n%r$'sw]J;5pAoO_#2mO3n,'=H5(et"
      "Hg*`+RLgv>=4U8guD$I%D:W>-r5V*%j*W:Kvej.Lp$<M-SGZ':+Q_k+uvOSLiEo(<aD/K<CCc`'Lx>'?;++O'>()jLR-^u68PHm8ZFWe+ej8h:9r6L*0//c&iH&R8pRbA#Kjm%upV1g:"
      "a_#Ur7FuA#(tRh#.Y5K+@?3<-8m0$PEn;J:rh6?I6uG<-`wMU'ircp0LaE_OtlMb&1#6T.#FDKu#1Lw%u%+GM+X'e?YLfjM[VO0MbuFp7;>Q&#WIo)0@F%q7c#4XAXN-U&VB<HFF*qL("
      "$/V,;(kXZejWO`<[5?\?ewY(*9=%wDc;,u<'9t3W-(H1th3+G]ucQ]kLs7df($/*JL]@*t7Bu_G3_7mp7<iaQjO@.kLg;x3B0lqp7Hf,^Ze7-##@/c58Mo(3;knp0%)A7?-W+eI'o8)b<"
      "nKnw'Ho8C=Y>pqB>0ie&jhZ[?iLR@@_AvA-iQC(=ksRZRVp7`.=+NpBC%rh&3]R:8XDmE5^V8O(x<<aG/1N$#FX$0V5Y6x'aErI3I$7x%E`v<-BY,)%-?Psf*l?%C3.mM(=/M0:JxG'?"
      "7WhH%o'a<-80g0NBxoO(GH<dM]n.+%q@jH?f.UsJ2Ggs&4<-e47&Kl+f//9@`b+?.TeN_&B8Ss?v;^Trk;f#YvJkl&w$]>-+k?'(<S:68tq*WoDfZu';mM?8X[ma8W%*`-=;D.(nc7/;"
      ")g:T1=^J$&BRV(-lTmNB6xqB[@0*o.erM*<SWF]u2=st-*(6v>^](H.aREZSi,#1:[IXaZFOm<-ui#qUq2$##Ri;u75OK#(RtaW-K-F`S+cF]uN`-KMQ%rP/Xri.LRcB##=YL3BgM/3M"
      "D?@f&1'BW-)Ju<L25gl8uhVm1hL$##*8###'A3/LkKW+(^rWX?5W_8g)a(m&K8P>#bmmWCMkk&#TR`C,5d>g)F;t,4:@_l8G/5h4vUd%&%950:VXD'QdWoY-F$BtUwmfe$YqL'8(PWX("
      "P?^@Po3$##`MSs?DWBZ/S>+4%>fX,VWv/w'KD`LP5IbH;rTV>n3cEK8U#bX]l-/V+^lj3;vlMb&[5YQ8#pekX9JP3XUC72L,,?+Ni&co7ApnO*5NK,((W-i:$,kp'UDAO(G0Sq7MVjJs"
      "bIu)'Z,*[>br5fX^:FPAWr-m2KgL<LUN098kTF&#lvo58=/vjDo;.;)Ka*hLR#/k=rKbxuV`>Q_nN6'8uTG&#1T5g)uLv:873UpTLgH+#FgpH'_o1780Ph8KmxQJ8#H72L4@768@Tm&Q"
      "h4CB/5OvmA&,Q&QbUoi$a_%3M01H)4x7I^&KQVgtFnV+;[Pc>[m4k//,]1?#`VY[Jr*3&&slRfLiVZJ:]?=K3Sw=[$=uRB?3xk48@aeg<Z'<$#4H)6,>e0jT6'N#(q%.O=?2S]u*(m<-"
      "V8J'(1)G][68hW$5'q[GC&5j`TE?m'esFGNRM)j,ffZ?-qx8;->g4t*:CIP/[Qap7/9'#(1sao7w-.qNUdkJ)tCF&#B^;xGvn2r9FEPFFFcL@.iFNkTve$m%#QvQS8U@)2Z+3K:AKM5i"
      "sZ88+dKQ)W6>J%CL<KE>`.d*(B`-n8D9oK<Up]c$X$(,)M8Zt7/[rdkqTgl-0cuGMv'?>-XV1q['-5k'cAZ69e;D_?$ZPP&s^+7])$*$#@QYi9,5P&#9r+$%CE=68>K8r0=dSC%%(@p7"
      ".m7jilQ02'0-VWAg<a/''3u.=4L$Y)6k/K:_[3=&jvL<L0C/2'v:^;-DIBW,B4E68:kZ;%?8(Q8BH=kO65BW?xSG&#@uU,DS*,?.+(o(#1vCS8#CHF>TlGW'b)Tq7VT9q^*^$$.:&N@@"
      "$&)WHtPm*5_rO0&e%K&#-30j(E4#'Zb.o/(Tpm$>K'f@[PvFl,hfINTNU6u'0pao7%XUp9]5.>%h`8_=VYbxuel.NTSsJfLacFu3B'lQSu/m6-Oqem8T+oE--$0a/k]uj9EwsG>%veR*"
      "hv^BFpQj:K'#SJ,sB-'#](j.Lg92rTw-*n%@/;39rrJF,l#qV%OrtBeC6/,;qB3ebNW[?,Hqj2L.1NP&GjUR=1D8QaS3Up&@*9wP?+lo7b?@%'k4`p0Z$22%K3+iCZj?XJN4Nm&+YF]u"
      "@-W$U%VEQ/,,>>#)D<h#`)h0:<Q6909ua+&VU%n2:cG3FJ-%@Bj-DgLr`Hw&HAKjKjseK</xKT*)B,N9X3]krc12t'pgTV(Lv-tL[xg_%=M_q7a^x?7Ubd>#%8cY#YZ?=,`Wdxu/ae&#"
      "w6)R89tI#6@s'(6Bf7a&?S=^ZI_kS&ai`&=tE72L_D,;^R)7[$s<Eh#c&)q.MXI%#v9ROa5FZO%sF7q7Nwb&#ptUJ:aqJe$Sl68%.D###EC><?-aF&#RNQv>o8lKN%5/$(vdfq7+ebA#"
      "u1p]ovUKW&Y%q]'>$1@-[xfn$7ZTp7mM,G,Ko7a&Gu%G[RMxJs[0MM%wci.LFDK)(<c`Q8N)jEIF*+?P2a8g%)$q]o2aH8C&<SibC/q,(e:v;-b#6[$NtDZ84Je2KNvB#$P5?tQ3nt(0"
      "d=j.LQf./Ll33+(;q3L-w=8dX$#WF&uIJ@-bfI>%:_i2B5CsR8&9Z&#=mPEnm0f`<&c)QL5uJ#%u%lJj+D-r;BoF&#4DoS97h5g)E#o:&S4weDF,9^Hoe`h*L+_a*NrLW-1pG_&2UdB8"
      "6e%B/:=>)N4xeW.*wft-;$'58-ESqr<b?UI(_%@[P46>#U`'6AQ]m&6/`Z>#S?YY#Vc;r7U2&326d=w&H####?TZ`*4?&.MK?LP8Vxg>$[QXc%QJv92.(Db*B)gb*BM9dM*hJMAo*c&#"
      "b0v=Pjer]$gG&JXDf->'StvU7505l9$AFvgYRI^&<^b68?j#q9QX4SM'RO#&sL1IM.rJfLUAj221]d##DW=m83u5;'bYx,*Sl0hL(W;;$doB&O/TQ:(Z^xBdLjL<Lni;''X.`$#8+1GD"
      ":k$YUWsbn8ogh6rxZ2Z9]%nd+>V#*8U_72Lh+2Q8Cj0i:6hp&$C/:p(HK>T8Y[gHQ4`4)'$Ab(Nof%V'8hL&#<NEdtg(n'=S1A(Q1/I&4([%dM`,Iu'1:_hL>SfD07&6D<fp8dHM7/g+"
      "tlPN9J*rKaPct&?'uBCem^jn%9_K)<,C5K3s=5g&GmJb*[SYq7K;TRLGCsM-$$;S%:Y@r7AK0pprpL<Lrh,q7e/%KWK:50I^+m'vi`3?%Zp+<-d+$L-Sv:@.o19n$s0&39;kn;S%BSq*"
      "$3WoJSCLweV[aZ'MQIjO<7;X-X;&+dMLvu#^UsGEC9WEc[X(wI7#2.(F0jV*eZf<-Qv3J-c+J5AlrB#$p(H68LvEA'q3n0#m,[`*8Ft)FcYgEud]CWfm68,(aLA$@EFTgLXoBq/UPlp7"
      ":d[/;r_ix=:TF`S5H-b<LI&HY(K=h#)]Lk$K14lVfm:x$H<3^Ql<M`$OhapBnkup'D#L$Pb_`N*g]2e;X/Dtg,bsj&K#2[-:iYr'_wgH)NUIR8a1n#S?Yej'h8^58UbZd+^FKD*T@;6A"
      "7aQC[K8d-(v6GI$x:T<&'Gp5Uf>@M.*J:;$-rv29'M]8qMv-tLp,'886iaC=Hb*YJoKJ,(j%K=H`K.v9HggqBIiZu'QvBT.#=)0ukruV&.)3=(^1`o*Pj4<-<aN((^7('#Z0wK#5GX@7"
      "u][`*S^43933A4rl][`*O4CgLEl]v$1Q3AeF37dbXk,.)vj#x'd`;qgbQR%FW,2(?LO=s%Sc68%NP'##Aotl8x=BE#j1UD([3$M(]UI2LX3RpKN@;/#f'f/&_mt&F)XdF<9t4)Qa.*kT"
      "LwQ'(TTB9.xH'>#MJ+gLq9-##@HuZPN0]u:h7.T..G:;$/Usj(T7`Q8tT72LnYl<-qx8;-HV7Q-&Xdx%1a,hC=0u+HlsV>nuIQL-5<N?)NBS)QN*_I,?&)2'IM%L3I)X((e/dl2&8'<M"
      ":^#M*Q+[T.Xri.LYS3v%fF`68h;b-X[/En'CR.q7E)p'/kle2HM,u;^%OKC-N+Ll%F9CF<Nf'^#t2L,;27W:0O@6##U6W7:$rJfLWHj$#)woqBefIZ.PK<b*t7ed;p*_m;4ExK#h@&]>"
      "_>@kXQtMacfD.m-VAb8;IReM3$wf0''hra*so568'Ip&vRs849'MRYSp%:t:h5qSgwpEr$B>Q,;s(C#$)`svQuF$##-D,##,g68@2[T;.XSdN9Qe)rpt._K-#5wF)sP'##p#C0c%-Gb%"
      "hd+<-j'Ai*x&&HMkT]C'OSl##5RG[JXaHN;d'uA#x._U;.`PU@(Z3dt4r152@:v,'R.Sj'w#0<-;kPI)FfJ&#AYJ&#//)>-k=m=*XnK$>=)72L]0I%>.G690a:$##<,);?;72#?x9+d;"
      "^V'9;jY@;)br#q^YQpx:X#Te$Z^'=-=bGhLf:D6&bNwZ9-ZD#n^9HhLMr5G;']d&6'wYmTFmL<LD)F^%[tC'8;+9E#C$g%#5Y>q9wI>P(9mI[>kC-ekLC/R&CH+s'B;K-M6$EB%is00:"
      "+A4[7xks.LrNk0&E)wILYF@2L'0Nb$+pv<(2.768/FrY&h$^3i&@+G%JT'<-,v`3;_)I9M^AE]CN?Cl2AZg+%4iTpT3<n-&%H%b<FDj2M<hH=&Eh<2Len$b*aTX=-8QxN)k11IM1c^j%"
      "9s<L<NFSo)B?+<-(GxsF,^-Eh@$4dXhN$+#rxK8'je'D7k`e;)2pYwPA'_p9&@^18ml1^[@g4t*[JOa*[=Qp7(qJ_oOL^('7fB&Hq-:sf,sNj8xq^>$U4O]GKx'm9)b@p7YsvK3w^YR-"
      "CdQ*:Ir<($u&)#(&?L9Rg3H)4fiEp^iI9O8KnTj,]H?D*r7'M;PwZ9K0E^k&-cpI;.p/6_vwoFMV<->#%Xi.LxVnrU(4&8/P+:hLSKj$#U%]49t'I:rgMi'FL@a:0Y-uA[39',(vbma*"
      "hU%<-SRF`Tt:542R_VV$p@[p8DV[A,?1839FWdF<TddF<9Ah-6&9tWoDlh]&1SpGMq>Ti1O*H&#(AL8[_P%.M>v^-))qOT*F5Cq0`Ye%+$B6i:7@0IX<N+T+0MlMBPQ*Vj>SsD<U4JHY"
      "8kD2)2fU/M#$e.)T4,_=8hLim[&);?UkK'-x?'(:siIfL<$pFM`i<?%W(mGDHM%>iWP,##P`%/L<eXi:@Z9C.7o=@(pXdAO/NLQ8lPl+HPOQa8wD8=^GlPa8TKI1CjhsCTSLJM'/Wl>-"
      "S(qw%sf/@%#B6;/U7K]uZbi^Oc^2n<bhPmUkMw>%t<)'mEVE''n`WnJra$^TKvX5B>;_aSEK',(hwa0:i4G?.Bci.(X[?b*($,=-n<.Q%`(X=?+@Am*Js0&=3bh8K]mL<LoNs'6,'85`"
      "0?t/'_U59@]ddF<#LdF<eWdF<OuN/45rY<-L@&#+fm>69=Lb,OcZV/);TTm8VI;?%OtJ<(b4mq7M6:u?KRdF<gR@2L=FNU-<b[(9c/ML3m;Z[$oF3g)GAWqpARc=<ROu7cL5l;-[A]%/"
      "+fsd;l#SafT/f*W]0=O'$(Tb<[)*@e775R-:Yob%g*>l*:xP?Yb.5)%w_I?7uk5JC+FS(m#i'k.'a0i)9<7b'fs'59hq$*5Uhv##pi^8+hIEBF`nvo`;'l0.^S1<-wUK2/Coh58KKhLj"
      "M=SO*rfO`+qC`W-On.=AJ56>>i2@2LH6A:&5q`?9I3@@'04&p2/LVa*T-4<-i3;M9UvZd+N7>b*eIwg:CC)c<>nO&#<IGe;__.thjZl<%w(Wk2xmp4Q@I#I9,DF]u7-P=.-_:YJ]aS@V"
      "?6*C()dOp7:WL,b&3Rg/.cmM9&r^>$(>.Z-I&J(Q0Hd5Q%7Co-b`-c<N(6r@ip+AurK<m86QIth*#v;-OBqi+L7wDE-Ir8K['m+DDSLwK&/.?-V%U_%3:qKNu$_b*B-kp7NaD'QdWQPK"
      "Yq[@>P)hI;*_F]u`Rb[.j8_Q/<&>uu+VsH$sM9TA%?)(vmJ80),P7E>)tjD%2L=-t#fK[%`v=Q8<FfNkgg^oIbah*#8/Qt$F&:K*-(N/'+1vMB,u()-a.VUU*#[e%gAAO(S>WlA2);Sa"
      ">gXm8YB`1d@K#n]76-a$U,mF<fX]idqd)<3,]J7JmW4`6]uks=4-72L(jEk+:bJ0M^q-8Dm_Z?0olP1C9Sa&H[d&c$ooQUj]Exd*3ZM@-WGW2%s',B-_M%>%Ul:#/'xoFM9QX-$.QN'>"
      "[%$Z$uF6pA6Ki2O5:8w*vP1<-1`[G,)-m#>0`P&#eb#.3i)rtB61(o'$?X3B</R90;eZ]%Ncq;-Tl]#F>2Qft^ae_5tKL9MUe9b*sLEQ95C&`=G?@Mj=wh*'3E>=-<)Gt*Iw)'QG:`@I"
      "wOf7&]1i'S01B+Ev/Nac#9S;=;YQpg_6U`*kVY39xK,[/6Aj7:'1Bm-_1EYfa1+o&o4hp7KN_Q(OlIo@S%;jVdn0'1<Vc52=u`3^o-n1'g4v58Hj&6_t7$##?M)c<$bgQ_'SY((-xkA#"
      "Y(,p'H9rIVY-b,'%bCPF7.J<Up^,(dU1VY*5#WkTU>h19w,WQhLI)3S#f$2(eb,jr*b;3Vw]*7NH%$c4Vs,eD9>XW8?N]o+(*pgC%/72LV-u<Hp,3@e^9UB1J+ak9-TN/mhKPg+AJYd$"
      "MlvAF_jCK*.O-^(63adMT->W%iewS8W6m2rtCpo'RS1R84=@paTKt)>=%&1[)*vp'u+x,VrwN;&]kuO9JDbg=pO$J*.jVe;u'm0dr9l,<*wMK*Oe=g8lV_KEBFkO'oU]^=[-792#ok,)"
      "i]lR8qQ2oA8wcRCZ^7w/Njh;?.stX?Q1>S1q4Bn$)K1<-rGdO'$Wr.Lc.CG)$/*JL4tNR/,SVO3,aUw'DJN:)Ss;wGn9A32ijw%FL+Z0Fn.U9;reSq)bmI32U==5ALuG&#Vf1398/pVo"
      "1*c-(aY168o<`JsSbk-,1N;$>0:OUas(3:8Z972LSfF8eb=c-;>SPw7.6hn3m`9^Xkn(r.qS[0;T%&Qc=+STRxX'q1BNk3&*eu2;&8q$&x>Q#Q7^Tf+6<(d%ZVmj2bDi%.3L2n+4W'$P"
      "iDDG)g,r%+?,$@?uou5tSe2aN_AQU*<h`e-GI7)?OK2A.d7_c)?wQ5AS@DL3r#7fSkgl6-++D:'A,uq7SvlB$pcpH'q3n0#_%dY#xCpr-l<F0NR@-##FEV6NTF6##$l84N1w?AO>'IAO"
      "URQ##V^Fv-XFbGM7Fl(N<3DhLGF%q.1rC$#:T__&Pi68%0xi_&[qFJ(77j_&JWoF.V735&T,[R*:xFR*K5>>#`bW-?4Ne_&6Ne_&6Ne_&n`kr-#GJcM6X;uM6X;uM(.a..^2TkL%oR(#"
      ";u.T%fAr%4tJ8&><1=GHZ_+m9/#H1F^R#SC#*N=BA9(D?v[UiFY>>^8p,KKF.W]L29uLkLlu/+4T<XoIB&hx=T1PcDaB&;HH+-AFr?(m9HZV)FKS8JCw;SD=6[^/DZUL`EUDf]GGlG&>"
      "w$)F./^n3+rlo+DB;5sIYGNk+i1t-69Jg--0pao7Sm#K)pdHW&;LuDNH@H>#/X-TI(;P>#,Gc>#0Su>#4`1?#8lC?#<xU?#@.i?#D:%@#HF7@#LRI@#P_[@#Tkn@#Xw*A#]-=A#a9OA#"
      "d<F&#*;G##.GY##2Sl##6`($#:l:$#>xL$#B.`$#F:r$#JF.%#NR@%#R_R%#Vke%#Zww%#_-4&#3^Rh%Sflr-k'MS.o?.5/sWel/wpEM0%3'/1)K^f1-d>G21&v(35>V`39V7A4=onx4"
      "A1OY5EI0;6Ibgr6M$HS7Q<)58C5w,;WoA*#[%T*#`1g*#d=#+#hI5+#lUG+#pbY+#tnl+#x$),#&1;,#*=M,#.I`,#2Ur,#6b.-#;w[H#iQtA#m^0B#qjBB#uvTB##-hB#'9$C#+E6C#"
      "/QHC#3^ZC#7jmC#;v)D#?,<D#C8ND#GDaD#KPsD#O]/E#g1A5#KA*1#gC17#MGd;#8(02#L-d3#rWM4#Hga1#,<w0#T.j<#O#'2#CYN1#qa^:#_4m3#o@/=#eG8=#t8J5#`+78#4uI-#"
      "m3B2#SB[8#Q0@8#i[*9#iOn8#1Nm;#^sN9#qh<9#:=x-#P;K2#$%X9#bC+.#Rg;<#mN=.#MTF.#RZO.#2?)4#Y#(/#[)1/#b;L/#dAU/#0Sv;#lY$0#n`-0#sf60#(F24#wrH0#%/e0#"
      "TmD<#%JSMFove:CTBEXI:<eh2g)B,3h2^G3i;#d3jD>)4kMYD4lVu`4m`:&5niUA5@(A5BA1]PBB:xlBCC=2CDLXMCEUtiCf&0g2'tN?PGT4CPGT4CPGT4CPGT4CPGT4CPGT4CPGT4CP"
      "GT4CPGT4CPGT4CPGT4CPGT4CPGT4CP-qekC`.9kEg^+F$kwViFJTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5o,^<-28ZI'O?;xp"
      "O?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xp;7q-#lLYI:xvD=#";
  return proggy_clean_ttf_compressed_data_base85;
}
#include "aaa.inl"
#include "bbb.inl"
#include "ccc.inl"

