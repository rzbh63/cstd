
//
// BCGPRibbonHyperlink.cpp: implementation of the CBCGPRibbonHyperlink class.
//
//////////////////////////////////////////////////////////////////////

#include "BCGPRibbonHyperlink.h"
#include "BCGPLocalResource.h"
#include "bcgprores.h"
#include "bcgglobals.h"
#include "BCGPVisualManager.h"

#ifndef BCGP_EXCLUDE_RIBBON

static BOOL ShellExecute (LPCTSTR lpOperation, LPCTSTR lpFile, 
						  LPCTSTR lpParameters = NULL, LPCTSTR lpDirectory = NULL, 
						  UINT nShow = SW_SHOWNORMAL)
{
    SHELLEXECUTEINFO sei;
    ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));

    sei.cbSize       = sizeof(SHELLEXECUTEINFO);
    sei.fMask        = SEE_MASK_NOCLOSEPROCESS;
    sei.hwnd         = NULL;
    sei.lpVerb       = lpOperation;
    sei.lpFile       = lpFile;
    sei.lpParameters = lpParameters;
    sei.lpDirectory  = lpDirectory;
    sei.nShow        = nShow;
    sei.hInstApp     = NULL;

    return ::ShellExecuteEx(&sei);
}

IMPLEMENT_DYNCREATE(CBCGPRibbonHyperlink, CBCGPRibbonButton)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPRibbonHyperlink::CBCGPRibbonHyperlink()
{
}
//****
CBCGPRibbonHyperlink::CBCGPRibbonHyperlink(UINT nID, LPCTSTR lpszText, LPCTSTR lpszLink)
{
	ASSERT (lpszText != NULL);
	ASSERT (lpszLink != NULL);

	m_nID = nID;
	SetText (lpszText);
	SetLink (lpszLink);
}
//****
CBCGPRibbonHyperlink::~CBCGPRibbonHyperlink()
{
}

//////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////

CSize CBCGPRibbonHyperlink::GetRegularSize (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	CSize size (0, 0);

	OnSetIcon ();

	if (m_bQuickAccessMode || m_bFloatyMode)
	{
		size = GetImageSize (RibbonImageSmall);
	}
	else
	{
		size = pDC->GetTextExtent (m_strText);
	}

	return CSize (
		size.cx + 2 * m_szMargin.cx,
		size.cy + 2 * m_szMargin.cy);
}
//****
CSize CBCGPRibbonHyperlink::GetCompactSize (CDC* pDC)
{
	ASSERT_VALID (this);

	return GetRegularSize (pDC);
}
//****
void CBCGPRibbonHyperlink::OnDraw (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	if (m_rect.IsRectEmpty ())
	{
		return;
	}

	if (m_bQuickAccessMode || m_bFloatyMode)
	{
		OnSetIcon ();
		CBCGPRibbonButton::OnDraw (pDC);

		return;
	}

	// Set font:
	CFont* pOldFont = pDC->SelectObject (&globalData.fontUnderline);
	ASSERT (pOldFont != NULL);

	COLORREF clrTextOld = pDC->SetTextColor (
		CBCGPVisualManager::GetInstance ()->GetRibbonHyperlinkTextColor (this));

	CRect rectText = m_rect;
	rectText.DeflateRect (m_szMargin.cx, m_szMargin.cy);

	DoDrawText (pDC, m_strText, rectText, DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);

	pDC->SelectObject (pOldFont);
	pDC->SetTextColor (clrTextOld);

	if (IsFocused ())
	{
		CRect rectFocus = rectText;
		rectFocus.InflateRect (1, 1);

		pDC->DrawFocusRect (rectFocus);
	}
}
//****
void CBCGPRibbonHyperlink::CopyFrom (const CBCGPBaseRibbonElement& s)
{
	ASSERT_VALID (this);

	CBCGPRibbonButton::CopyFrom (s);

	CBCGPRibbonHyperlink& src = (CBCGPRibbonHyperlink&) s;

	m_strLink = src.m_strLink;
}
//****
BOOL CBCGPRibbonHyperlink::OpenLink ()
{
	ASSERT_VALID (this);

	if (m_strLink.IsEmpty ())
	{
		return FALSE;
	}

	return ShellExecute (NULL, m_strLink);
}
//****
void CBCGPRibbonHyperlink::OnMouseMove (CPoint point)
{
	ASSERT_VALID (this);

	CBCGPRibbonButton::OnMouseMove (point);

	::SetCursor (globalData.GetHandCursor ());
}
//****
void CBCGPRibbonHyperlink::OnSetIcon ()
{
	ASSERT_VALID (this);

	if (m_hIcon == NULL)
	{
		if (globalData.m_hiconLink == NULL)
		{
			CBCGPLocalResource locaRes;

			globalData.m_hiconLink = (HICON) ::LoadImage (
				AfxGetResourceHandle (),
				MAKEINTRESOURCE (IDI_BCGRES_LINK),
				IMAGE_ICON,
				16,
				16,
				LR_SHARED);
		}

		m_hIcon = globalData.m_hiconLink;
	}

}

void CBCGPRibbonHyperlink::SetLink (LPCTSTR lpszLink)
{
	m_strLink = lpszLink;
}

#endif // BCGP_EXCLUDE_RIBBON