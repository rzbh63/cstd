
#include <math.h>
#include <tchar.h>
#include <windows.h>
#include "cstd.h"
#include ".\imdraw\color_space.inl"
#include "str.h"



COLORREF Tag_m_clrBase = (COLORREF) - 1;
COLORREF Tag_m_clrTarget = (COLORREF) - 1;

static LPCTSTR s_Point     = _T("POINT");
static LPCTSTR s_Size      = _T("SIZE");
static LPCTSTR s_Rect      = _T("RECT");
static LPCTSTR s_Offset    = _T("OFFSET");
static LPCTSTR s_X         = _T("X");
static LPCTSTR s_Y         = _T("Y");
static LPCTSTR s_Width     = _T("WIDTH");
static LPCTSTR s_Height    = _T("HEIGHT");

static LPCTSTR s_Left      = _T("LEFT");
static LPCTSTR s_Right     = _T("RIGHT");
static LPCTSTR s_Top       = _T("TOP");
static LPCTSTR s_Bottom    = _T("BOTTOM");

static LPCTSTR s_LT        = _T("LEFTTOP");
static LPCTSTR s_LB        = _T("LEFTBOTTOM");
static LPCTSTR s_RT        = _T("RIGHTTOP");
static LPCTSTR s_RB        = _T("RIGHTBOTTOM");

static LPCTSTR s_A         = _T("A");
static LPCTSTR s_R         = _T("R");
static LPCTSTR s_G         = _T("G");
static LPCTSTR s_B         = _T("B");

static LPCTSTR s_True      = _T("TRUE");
static LPCTSTR s_False     = _T("FALSE");

static LPCTSTR s_FaceName = _T("FACENAME");
static LPCTSTR s_Charset  = _T("CHARSET");
static LPCTSTR s_Quality  = _T("QUALITY");
static LPCTSTR s_Weight   = _T("WEIGHT");

static LPCTSTR s_Corners     = _T("CORNERS");
static LPCTSTR s_Sides       = _T("SIDES");
static LPCTSTR s_Interior    = _T("INTERIOR");
static LPCTSTR s_Transparent = _T("TRANSPARENT");
static LPCTSTR s_PreMltCheck = _T("PREMLTCHECK");

static LPCTSTR s_ColorBackground   = _T("COLOR_BACKGROUND");
static LPCTSTR s_ColorFill         = _T("COLOR_FILL");
static LPCTSTR s_ColorFillGradient = _T("COLOR_FILLGRADIENT");
static LPCTSTR s_ColorText         = _T("COLOR_TEXT");
static LPCTSTR s_ColorBorder       = _T("COLOR_BORDER");
static LPCTSTR s_GradientAngle     = _T("GRADIENT_ANGLE");

static LPCTSTR s_TTP_BallonTooltip     = _T("BALLON");
static LPCTSTR s_TTP_DrawIcon          = _T("DRAW_ICON");
static LPCTSTR s_TTP_DrawDescription   = _T("DRAW_DESCRIPTION");
static LPCTSTR s_TTP_DrawSeparator     = _T("DRAW_SEPARATOR");
static LPCTSTR s_TTP_MaxDescrWidth     = _T("MAX_DESC_WIDTH");
static LPCTSTR s_TTP_RoundedCorners    = _T("ROUNDED_CORNERS");
static LPCTSTR s_TTP_BoldLabel         = _T("BOLD_LABEL");

#ifndef BCGP_EXCLUDE_GRID_CTRL
static LPCTSTR s_Grid_EvenColors       = _T("EVEN_COLORS");
static LPCTSTR s_Grid_OddColors        = _T("ODD_COLORS");
static LPCTSTR s_Grid_GroupColors      = _T("GROUP_COLORS");
static LPCTSTR s_Grid_GroupSelColors   = _T("GROUP_SEL_COLORS");
static LPCTSTR s_Grid_SelColors        = _T("SEL_COLORS");
static LPCTSTR s_Grid_HeaderColors     = _T("HEADER_COLORS");
static LPCTSTR s_Grid_HeaderSelColors  = _T("HEADER_SEL_COLORS");
static LPCTSTR s_Grid_LeftOffsetColors = _T("OFFSET_COLORS");
static LPCTSTR s_Grid_ColorHeader      = _T("COLOR_HEADER");
static LPCTSTR s_Grid_ColorHorzLine    = _T("COLOR_H_LINE");
static LPCTSTR s_Grid_ColorVertLine    = _T("COLOR_V_LINE");
static LPCTSTR s_Grid_ColorPreviewText = _T("COLOR_PREVIEW_TEXT");
#endif

static COLORREF AddaptColor(COLORREF clrOrig, COLORREF clrBase, COLORREF clrTone)
{
  double dSrcH, dSrcS, dSrcL;
  double dDestH, dDestS, dDestL;
  double DH, DL, DS;
  double H, S, L;
  double HNew, SNew, LNew;

  if (clrBase == (COLORREF) - 1 || clrTone == (COLORREF) - 1 || clrOrig == (COLORREF) - 1) {
    return clrOrig;
  }

  RGBtoHSL(clrBase, &dSrcH, &dSrcS, &dSrcL);
  RGBtoHSL(clrTone, &dDestH, &dDestS, &dDestL);

  DH = dDestH - dSrcH;
  DL = dDestL - dSrcL;
  DS = dDestS - dSrcS;

  RGBtoHSL(clrOrig, &H, &S, &L);

  if (fabs(dSrcH - H) >= 0.2) {
    return clrOrig;
  }

  HNew = MAX(0, MIN(1., H + DH));
  SNew = MAX(0, MIN(1., S + DS));
  LNew = MAX(0, MIN(1., L + DL));

  return HLStoRGB_ONE(HNew, dDestH > 0.5 ? L : LNew, SNew);
}

BOOL Tag_ExcludeTag(str_t m_strBuffer, LPCTSTR lpszTag, str_t* strTag, BOOL bIsCharsList)
{
  int lpszTagLen = strlen(lpszTag);
  int iBufLen = m_strBuffer.l;
  int i, iStart = 0, nBalanse = 0;

  strTag->s = 0;

  for (i = 0; i < iBufLen - lpszTagLen + 2; ++i) {
    if (m_strBuffer.s[i] != ('<')) {
      continue;
    }

    if ('>' == m_strBuffer.s[i + lpszTagLen + 1] && 0 == _tcsncmp(m_strBuffer.s + i + 1, lpszTag, lpszTagLen)) {
      i += lpszTagLen + 2;
      nBalanse ++;

      if (0 == iStart) {
        iStart = i;
      }

      continue;
    }

    if ('/' == m_strBuffer.s[i + 1] && '>' == m_strBuffer.s[i + lpszTagLen + 2] && 0 == _tcsncmp(m_strBuffer.s + i + 2, lpszTag, lpszTagLen)) {
      nBalanse --;

      if (0 == nBalanse) {
        strTag->s = m_strBuffer.s + iStart;
        strTag->l = i - iStart;
        break;
      }

      i += lpszTagLen + 3;
    }
  }

  if (0 == strTag->s) {
    return FALSE;
  }

  *strTag = STRtrim(*strTag, " \t\r\n");

  if (bIsCharsList) {
    if (strTag->l > 1 && strTag->s[0] == ('\"')) {
      strTag->s++;
      strTag->l -= 2;
    }

    str_replace_str(strTag, STR1("\\t"), STR1("\t"));
    str_replace_str(strTag, STR1("\\n"), STR1("\n"));
    str_replace_str(strTag, STR1("\\r"), STR1("\r"));
    str_replace_str(strTag, STR1("\\b"), STR1("\b"));
    str_replace_str(strTag, STR1("LT"), STR1("<"));
    str_replace_str(strTag, STR1("GT"), STR1(">"));
    str_replace_str(strTag, STR1("AMP"), STR1("&"));
  }

  return TRUE;
}

int test_Tag_ExcludeTag()
{
  str_t ss[1] = {0};
  str_t st[1] = {0};
  const char* fn = "D:\\code\\c\\GUI\\bcgsoft\\BCG\\Bin\\BCGPStyle2007Luna1030\\BCGPStyle2007Luna1030_IDX_OFFICE2007_STYLE_STYLE_XML.xml";
  str_load(fn, ss);
  Tag_ExcludeTag(*ss, "DESCRIPTION", st, 0);
  st->s[st->l] = 0;
  str_free(ss);
  return 0;
}

#if 0
BOOL Tag_ParseString(const str_t* str, const str_t* sep, CStringArray& sa, BOOL bTrim, BOOL bIncludeEmpty)
{
  sa.RemoveAll();

  if (str.IsEmpty()) {
    return FALSE;
  }

  str_t s(str);

  if (bTrim) {
    s.TrimLeft();
    s.TrimRight();
  }

  if (s.IsEmpty()) {
    return FALSE;
  }

  if (sep.IsEmpty()) {
    return FALSE;
  }

  int pos = s.Find(sep);

  while (pos != -1) {
    str_t sp = s.Left(pos);
    s = s.Right(s.l - sep.l - pos);

    if (bTrim) {
      sp.TrimLeft();
      sp.TrimRight();
      s.TrimLeft();
    }

    if ((sp.IsEmpty() && bIncludeEmpty) || !sp.IsEmpty()) {
      sa.Add(sp);
    }

    pos = s.Find(sep);

    if (pos == -1 && ((s.IsEmpty() && bIncludeEmpty) || !s.IsEmpty())) {
      sa.Add(s);
    }
  }

  return sa.GetSize() > 0;
}

BOOL Tag_ParseColor(const str_t* strItem, COLORREF& value)
{
  CBCGPTagManager tm(strItem);

  CStringArray sa;

  str_t strA, strR, strG, strB;

  tm.ExcludeTag(s_A, strA);
  strA.TrimLeft();
  strA.TrimRight();
  tm.ExcludeTag(s_R, strR);
  strR.TrimLeft();
  strR.TrimRight();
  tm.ExcludeTag(s_G, strG);
  strG.TrimLeft();
  strG.TrimRight();
  tm.ExcludeTag(s_B, strB);
  strB.TrimLeft();
  strB.TrimRight();

  if (strR.IsEmpty() || strG.IsEmpty() || strB.IsEmpty()) {
    if (!ParseString(strItem, _T(","), sa, TRUE, FALSE)) {
      strR = tm.GetBuffer();
      strR.TrimLeft();
      strR.TrimRight();

      sa.Add(strR);
    }
  }
  else {
    sa.Add(strR);
    sa.Add(strG);
    sa.Add(strB);

    if (!strA.IsEmpty()) {
      sa.Add(strA);
    }
  }

  if (sa.GetSize() > 0) {
    const int count = (int) sa.GetSize();

    if (count == 3) {
      value = AddaptColor((COLORREF)RGB(_ttol(sa[0]), _ttol(sa[1]), _ttol(sa[2])),
          m_clrBase, m_clrTarget);
      return TRUE;
    }
    /*
        else if(count == 4)
        {
          value =(COLORREF)RGBA(_ttol(sa[0]), _ttol(sa[1]), _ttol(sa[2]),  _ttol(sa[3]));
          return TRUE;
        }
    */
    else if (count == 1) {
      value = (COLORREF)_ttol(sa[0]);
      return TRUE;
    }
  }

  return FALSE;
}

BOOL Tag_ParseColorHEX(const str_t* strItem, COLORREF& value)
{
  str_t str(strItem);

  str.MakeUpper();
  str.TrimLeft();
  str.TrimRight();

  const int len = str.l;

  if (len < 6) {
    return FALSE;
  }

  BOOL bRes = TRUE;
  BYTE clr[3] = {0, 0, 0};
  int nColor = 0;
  int nRead = 0;

  int val = 0;

  for (int i = 0; i < 6; i++) {
    TCHAR c = str[len - i - 1];

    if (TCHAR('A') <= c && c <= TCHAR('F')) {
      val = 10 + (c - TCHAR('A'));
    }
    else if (TCHAR('0') <= c && c <= TCHAR('9')) {
      val = c - TCHAR('0');
    }
    else {
      bRes = FALSE;
      break;
    }

    if (nRead == 0) {
      clr[nColor] = (BYTE)val;
    }
    else {
      clr[nColor] |= val << 4;
    }

    nRead++;

    if (nRead == 2) {
      nRead = 0;
      nColor++;
    }
  }

  if (bRes) {
    value = AddaptColor(RGB(clr[2], clr[1], clr[0]), m_clrBase, m_clrTarget);
  }

  return bRes;
}

BOOL Tag_ParsePoint(const str_t* strItem, CPoint& value)
{
  CBCGPTagManager tm(strItem);

  CStringArray sa;

  str_t strX, strY;

  tm.ExcludeTag(s_X, strX);
  strX.TrimLeft();
  strX.TrimRight();
  tm.ExcludeTag(s_Y, strY);
  strY.TrimLeft();
  strY.TrimRight();

  if (strX.IsEmpty() || strY.IsEmpty()) {
    if (!ParseString(tm.GetBuffer(), _T(","), sa, TRUE, FALSE)) {
      return FALSE;
    }
  }
  else {
    sa.Add(strX);
    sa.Add(strY);
  }

  if (sa.GetSize() == 2) {
    value.x = _ttol(sa[0]);
    value.y = _ttol(sa[1]);
    return TRUE;
  }

  return FALSE;
}

BOOL Tag_ParseSize(const str_t* strItem, CSize& value)
{
  CBCGPTagManager tm(strItem);
  CStringArray sa;

  str_t strW, strH;

  tm.ExcludeTag(s_Width, strW);
  strW.TrimLeft();
  strW.TrimRight();
  tm.ExcludeTag(s_Height, strH);
  strH.TrimLeft();
  strH.TrimRight();

  if (strW.IsEmpty() || strH.IsEmpty()) {
    if (!ParseString(tm.GetBuffer(), _T(","), sa, TRUE, FALSE)) {
      return FALSE;
    }
  }
  else {
    sa.Add(strW);
    sa.Add(strH);
  }

  if (sa.GetSize() == 2) {
    value.w = _ttol(sa[0]);
    value.h = _ttol(sa[1]);
    return TRUE;
  }

  return FALSE;
}

BOOL Tag_ParseRect(const str_t* strItem, CRect& value)
{
  CBCGPTagManager tm(strItem);

  str_t str1;
  str_t str2;

  tm.ExcludeTag(s_Offset, str1);
  str1.TrimLeft();
  str1.TrimRight();
  tm.ExcludeTag(s_Size, str2);
  str2.TrimLeft();
  str2.TrimRight();

  CPoint pt(0, 0);
  CSize  sz(0, 0);

  if (ParsePoint(str1, pt) && ParseSize(str2, sz)) {
    value = CRect(pt, sz);
    return TRUE;
  }

  tm.SetBuffer(strItem);
  tm.ExcludeTag(s_LT, str1);
  str1.TrimLeft();
  str1.TrimRight();
  tm.ExcludeTag(s_RB, str2);
  str2.TrimLeft();
  str2.TrimRight();

  CPoint pt2(0, 0);

  if (ParsePoint(str1, pt) && ParsePoint(str2, pt2)) {
    value = CRect(pt, pt2);
    return TRUE;
  }

  CStringArray sa;

  str_t strL, strT, strR, strB;

  tm.SetBuffer(strItem);

  tm.ExcludeTag(s_Left, strL);
  strL.TrimLeft();
  strL.TrimRight();
  tm.ExcludeTag(s_Top, strT);
  strT.TrimLeft();
  strT.TrimRight();
  tm.ExcludeTag(s_Right, strR);
  strR.TrimLeft();
  strR.TrimRight();
  tm.ExcludeTag(s_Bottom, strB);
  strB.TrimLeft();
  strB.TrimRight();

  if (strL.IsEmpty() || strT.IsEmpty() || strR.IsEmpty() || strB.IsEmpty()) {
    if (!ParseString(tm.GetBuffer(), _T(","), sa, TRUE, FALSE)) {
      return FALSE;
    }
  }
  else {
    sa.Add(strL);
    sa.Add(strT);
    sa.Add(strR);
    sa.Add(strB);
  }

  if (sa.GetSize() == 4) {
    value.left   = _ttol(sa[0]);
    value.top    = _ttol(sa[1]);
    value.right  = _ttol(sa[2]);
    value.bottom = _ttol(sa[3]);
    return TRUE;
  }

  return FALSE;
}

BOOL Tag_ParseFont(const str_t* strItem, LOGFONT& value)
{
  CBCGPTagManager tm(strItem);

  str_t strFontItem;

  if (tm.ExcludeTag(s_FaceName, strFontItem)) {
    ASSERT(!strFontItem.IsEmpty());

    if (!strFontItem.IsEmpty()) {
      memcpy(value.lfFaceName, (LPCTSTR)strFontItem, MIN(strFontItem.l, LF_FACESIZE) * sizeof(TCHAR));
    }
  }

  int val = 0;

  if (tm.ReadInt(s_Height, val)) {
    value.lfHeight = val;
  }

  if (value.lfHeight > 0) {
    if (tm.ReadInt(s_Width, val)) {
      value.lfWidth = val;
    }
  }

  if (tm.ExcludeTag(s_Weight, strFontItem)) {
    struct WEIGHT_FONT_TYPE {
      LPCTSTR name;
      LONG    weight;
    };

    const WEIGHT_FONT_TYPE WEIGHT_FONT_TYPES[] = {
      {_T("DONTCARE")  , FW_DONTCARE  },
      {_T("THIN")      , FW_THIN      },
      {_T("EXTRALIGHT"), FW_EXTRALIGHT},
      {_T("LIGHT")     , FW_LIGHT     },
      {_T("NORMAL")    , FW_NORMAL    },
      {_T("MEDIUM")    , FW_MEDIUM    },
      {_T("SEMIBOLD")  , FW_SEMIBOLD  },
      {_T("BOLD")      , FW_BOLD      },
      {_T("EXTRABOLD") , FW_EXTRABOLD },
      {_T("HEAVY")     , FW_HEAVY     },
      {_T("ULTRALIGHT"), FW_ULTRALIGHT},
      {_T("REGULAR")   , FW_REGULAR   },
      {_T("DEMIBOLD")  , FW_DEMIBOLD  },
      {_T("ULTRABOLD") , FW_ULTRABOLD },
      {_T("BLACK")     , FW_BLACK     }
    };

    for (long i = 0; i < sizeof(WEIGHT_FONT_TYPES) / sizeof(WEIGHT_FONT_TYPE); i++) {
      if (strFontItem.CompareNoCase(WEIGHT_FONT_TYPES[i].name) == 0) {
        value.lfWeight = WEIGHT_FONT_TYPES[i].weight;
        break;
      }
    }
  }

  if (tm.ExcludeTag(s_Quality, strFontItem)) {
    struct QUALITY_FONT_TYPE {
      LPCTSTR name;
      BYTE    quality;
    };

    const QUALITY_FONT_TYPE QUALITY_FONT_TYPES[] = {
      {_T("DEFAULT")          , DEFAULT_QUALITY          },
      {_T("DRAFT")            , DRAFT_QUALITY            },
      {_T("PROOF")            , PROOF_QUALITY            },
      {_T("NONANTIALIASED")   , NONANTIALIASED_QUALITY   },
      {_T("ANTIALIASED")      , ANTIALIASED_QUALITY      },
      {_T("CLEARTYPE")        , 5},//CLEARTYPE_QUALITY
      {_T("CLEARTYPE_NATURAL"), 6} //CLEARTYPE_NATURAL_QUALITY
    };

    for (long i = 0; i < sizeof(QUALITY_FONT_TYPES) / sizeof(QUALITY_FONT_TYPE); i++) {
      if (strFontItem.CompareNoCase(QUALITY_FONT_TYPES[i].name) == 0) {
        if (QUALITY_FONT_TYPES[i].quality <= ANTIALIASED_QUALITY) {
          value.lfQuality = QUALITY_FONT_TYPES[i].quality;
        }
        else {
          OSVERSIONINFO osvi;
          osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
          ::GetVersionEx(&osvi);

          if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT &&
              osvi.dwMajorVersion >= 5) {
            value.lfQuality = QUALITY_FONT_TYPES[i].quality;
          }
        }

        break;
      }
    }
  }

  return TRUE;
}

BOOL Tag_ParseToolBarImages(const str_t* strItem, CBCGPToolBarImages& value, UINT ID)
{
  return ParseToolBarImages(strItem, value, MAKEINTRESOURCE(ID));
}

BOOL Tag_ParseToolBarImages(const str_t* strItem, CBCGPToolBarImages& value, LPCTSTR lpszID)
{
  CBCGPTagManager tm(strItem);

  CSize size(value.GetImageSize());

  value.Clear();
  value.SetTransparentColor((COLORREF)(-1));

  tm.ReadSize(s_Size, size);

  BOOL bPreMultiplyCheck = TRUE;
  tm.ReadBool(s_PreMltCheck, bPreMultiplyCheck);

  value.SetPreMultiplyAutoCheck(bPreMultiplyCheck);

  if (size != CSize(0, 0)) {
    value.SetImageSize(size);
  }

  value.LoadStr(lpszID);

  if (size == CSize(0, 0)) {
    value.SetSingleImage();
  }

  COLORREF clrTransparent = CLR_DEFAULT;

  if (tm.ReadColor(s_Transparent, clrTransparent)) {
    value.SetTransparentColor(clrTransparent);
  }

#ifndef _BCGSUITE_

  if (m_clrBase != (COLORREF) - 1 &&
      m_clrTarget != (COLORREF) - 1) {
    value.AddaptColors(m_clrBase, m_clrTarget);
  }

#endif

  if (CBCGPToolBarImages::IsRTL() && value.GetImageWell() != NULL &&
      clrTransparent == CLR_DEFAULT) {
    BITMAP bmp;

    if (::GetObject(value.GetImageWell(), sizeof(BITMAP), &bmp) != 0) {
      if (bmp.bmBitsPixel == 32) {
        value.Mirror();
      }
    }
  }

  return TRUE;
}

BOOL Tag_ParseControlRendererParams(const str_t* strItem, CBCGPControlRendererParams& value)
{
  CBCGPTagManager tm(strItem);

  CBCGPControlRendererParams params;
  params.SetResourceID(value.GetResourceID());

#ifndef _BCGSUITE_
  params.SetBaseColor(m_clrBase, m_clrTarget);
#endif

  if (!tm.ReadRect(s_Rect, params.m_rectImage)) {
    CSize size;

    if (tm.ReadSize(s_Size, size)) {
      params.m_rectImage = CRect(CPoint(0, 0), size);
    }
  }

  if (params.m_rectImage.IsRectEmpty()) {
    return FALSE;
  }

  tm.ReadRect(s_Corners, params.m_rectCorners);
  tm.ReadRect(s_Sides, params.m_rectSides);
  tm.ReadRect(s_Interior, params.m_rectInter);
  tm.ReadColor(s_Transparent, params.m_clrTransparent);
  tm.ReadBool(s_PreMltCheck, params.m_bPreMultiplyCheck);

  value = params;

  return TRUE;
}

BOOL Tag_ParseControlRenderer(const str_t* strItem, CBCGPControlRenderer& value, UINT ID)
{
  return ParseControlRenderer(strItem, value, MAKEINTRESOURCE(ID));
}

BOOL Tag_ParseControlRenderer(const str_t* strItem, CBCGPControlRenderer& value, LPCTSTR lpszID)
{
  value.CleanUp();

  CBCGPControlRendererParams params(lpszID, CRect(0, 0, 0, 0), CRect(0, 0, 0, 0));

  if (ParseControlRendererParams(strItem, params)) {
    return value.Create(params);
  }

  return FALSE;
}

BOOL Tag_ParseToolTipParams(const str_t* strItem, CBCGPToolTipParams& value)
{
  CBCGPTagManager tm(strItem);

  CBCGPToolTipParams params;

#ifndef _BCGSUITE_
  tm.ReadBool(s_TTP_BallonTooltip   , params.m_bBallonTooltip);
#else
  tm.ReadBool(s_TTP_BallonTooltip   , params.m_bBalloonTooltip);
#endif

  tm.ReadBool(s_TTP_DrawIcon        , params.m_bDrawIcon);
  tm.ReadBool(s_TTP_DrawDescription , params.m_bDrawDescription);
  tm.ReadInt(s_TTP_MaxDescrWidth   , params.m_nMaxDescrWidth);
  tm.ReadBool(s_TTP_RoundedCorners  , params.m_bRoundedCorners);
  tm.ReadBool(s_TTP_BoldLabel       , params.m_bBoldLabel);
  tm.ReadColor(s_ColorFill           , params.m_clrFill);
  tm.ReadColor(s_ColorFillGradient   , params.m_clrFillGradient);
  tm.ReadInt(s_GradientAngle       , params.m_nGradientAngle);
  tm.ReadColor(s_ColorText           , params.m_clrText);
  tm.ReadColor(s_ColorBorder         , params.m_clrBorder);
  tm.ReadBool(s_TTP_DrawSeparator   , params.m_bDrawSeparator);

  value = params;

  return TRUE;
}

BOOL Tag_ReadString(const str_t* strValue, str_t* value)
{
  BOOL bRes = FALSE;
  str_t strItem;

  if (ExcludeTag(strValue, strItem)) {
    value = strItem;
    bRes = TRUE;
  }

  return bRes;
}

BOOL Tag_ReadBool(const str_t* strValue, BOOL& value)
{
  BOOL bRes = FALSE;
  str_t strItem;

  if (ExcludeTag(strValue, strItem)) {
    value = strItem.CompareNoCase(s_True) == 0;
    bRes = TRUE;
  }

  return bRes;
}

BOOL Tag_ReadInt(const str_t* strValue, int& value)
{
  BOOL bRes = FALSE;
  str_t strItem;

  if (ExcludeTag(strValue, strItem)) {
    strItem.TrimLeft();
    strItem.TrimRight();

    value = _ttol(strItem);
    bRes = TRUE;
  }

  return bRes;
}

BOOL Tag_ReadPoint(const str_t* strValue, CPoint& value)
{
  str_t strItem;

  if (ExcludeTag(strValue, strItem)) {
    return ParsePoint(strItem, value);
  }

  return FALSE;
}

BOOL Tag_ReadSize(const str_t* strValue, CSize& value)
{
  str_t strItem;

  if (ExcludeTag(strValue, strItem)) {
    return ParseSize(strItem, value);
  }

  return FALSE;
}

BOOL Tag_ReadRect(const str_t* strValue, CRect& value)
{
  str_t strItem;

  if (ExcludeTag(strValue, strItem)) {
    return ParseRect(strItem, value);
  }

  return FALSE;
}

BOOL Tag_ReadColor(const str_t* strValue, COLORREF& value)
{
  str_t strItem;

  if (ExcludeTag(strValue, strItem)) {
    return ParseColor(strItem, value);
  }

  return FALSE;
}

BOOL Tag_ReadFont(const str_t* strValue, LOGFONT& value)
{
  str_t strItem;

  if (ExcludeTag(strValue, strItem)) {
    return ParseFont(strItem, value);
  }

  return FALSE;
}

BOOL Tag_ReadToolBarImages(const str_t* strValue, CBCGPToolBarImages& value, UINT ID)
{
  return ReadToolBarImages(strValue, value, MAKEINTRESOURCE(ID));
}

BOOL Tag_ReadToolBarImages(const str_t* strValue, CBCGPToolBarImages& value, LPCTSTR lpszID)
{
  str_t strItem;

  if (ExcludeTag(strValue, strItem)) {
    return ParseToolBarImages(strItem, value, lpszID);
  }

  return FALSE;
}

BOOL Tag_ReadControlRendererParams(const str_t* strValue, CBCGPControlRendererParams& value)
{
  str_t strItem;

  if (ExcludeTag(strValue, strItem)) {
    return ParseControlRendererParams(strItem, value);
  }

  return FALSE;
}

BOOL Tag_ReadControlRenderer(const str_t* strValue, CBCGPControlRenderer& value, UINT ID)
{
  return ReadControlRenderer(strValue, value, MAKEINTRESOURCE(ID));
}

BOOL Tag_ReadControlRenderer(const str_t* strValue, CBCGPControlRenderer& value, LPCTSTR lpszID)
{
  str_t strItem;

  if (ExcludeTag(strValue, strItem)) {
    return ParseControlRenderer(strItem, value, lpszID);
  }

  return FALSE;
}

BOOL Tag_ReadToolTipParams(const str_t* strValue, CBCGPToolTipParams& value)
{
  str_t strItem;

  if (ExcludeTag(strValue, strItem)) {
    return ParseToolTipParams(strItem, value);
  }

  return FALSE;
}

#ifndef BCGP_EXCLUDE_GRID_CTRL
BOOL Tag_ParseGridColorData(const str_t* strItem, BCGP_GRID_COLOR_DATA::ColorData& value)
{
  CBCGPTagManager tm(strItem);

  value.InitColors();

  tm.ReadColor(s_ColorFill        , value.m_clrBackground);
  tm.ReadColor(s_ColorText        , value.m_clrText);
  tm.ReadColor(s_ColorFillGradient, value.m_clrGradient);
  tm.ReadInt(s_GradientAngle    , value.m_nGradientAngle);
  tm.ReadColor(s_ColorBorder      , value.m_clrBorder);

  return TRUE;
}

BOOL Tag_ParseGridColors(const str_t* strItem, BCGP_GRID_COLOR_DATA& value)
{
  CBCGPGridColors colors;

  CBCGPTagManager tm(strItem);

  tm.ReadGridColorData(s_Grid_EvenColors       , colors.m_EvenColors);
  tm.ReadGridColorData(s_Grid_OddColors        , colors.m_OddColors);
  tm.ReadGridColorData(s_Grid_GroupColors      , colors.m_GroupColors);
  tm.ReadGridColorData(s_Grid_GroupSelColors   , colors.m_GroupSelColors);
  tm.ReadGridColorData(s_Grid_SelColors        , colors.m_SelColors);
  tm.ReadGridColorData(s_Grid_HeaderColors     , colors.m_HeaderColors);
  tm.ReadGridColorData(s_Grid_HeaderSelColors  , colors.m_HeaderSelColors);
  tm.ReadGridColorData(s_Grid_LeftOffsetColors , colors.m_LeftOffsetColors);
  tm.ReadColor(s_ColorBackground      , colors.m_clrBackground);
  tm.ReadColor(s_ColorText            , colors.m_clrText);
  tm.ReadColor(s_Grid_ColorHeader     , colors.m_clrHeader);
  tm.ReadColor(s_Grid_ColorHorzLine   , colors.m_clrHorzLine);
  tm.ReadColor(s_Grid_ColorVertLine   , colors.m_clrVertLine);
  tm.ReadColor(s_Grid_ColorPreviewText, colors.m_clrPreviewText);

  value = colors;

  return TRUE;
}

BOOL Tag_ReadGridColorData(const str_t* strValue, BCGP_GRID_COLOR_DATA::ColorData& value)
{
  str_t strItem;

  if (ExcludeTag(strValue, strItem)) {
    return ParseGridColorData(strItem, value);
  }

  return FALSE;
}

BOOL Tag_ReadGridColors(const str_t* strValue, BCGP_GRID_COLOR_DATA& value)
{
  str_t strItem;

  if (ExcludeTag(strValue, strItem)) {
    return ParseGridColors(strItem, value);
  }

  return FALSE;
}
#endif

#endif
