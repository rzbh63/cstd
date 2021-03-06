//****
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2009 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
 //****


#include "BCGPDialog.h"

#ifndef _BCGSUITE_
#include "BCGPPopupMenu.h"
#include "BCGPToolbarMenuButton.h"
#include "BCGPPngImage.h"
#endif

#include "BCGPVisualManager.h"


IMPLEMENT_DYNAMIC(CBCGPDialog, CDialog)

/////////////////////////////////////////////////////////////////////////////
// CBCGPDialog dialog

#pragma warning (disable : 4355)

CBCGPDialog::CBCGPDialog() :
	m_Impl (*this)
{
	CommonConstruct();
}
//****
CBCGPDialog::CBCGPDialog (UINT nIDTemplate, CWnd *pParent/*= NULL*/) : 
				CDialog (nIDTemplate, pParent),
				m_Impl (*this)
{
	CommonConstruct();
}
//****
CBCGPDialog::CBCGPDialog (LPCTSTR lpszTemplateName, CWnd *pParentWnd/*= NULL*/) : 
				CDialog(lpszTemplateName, pParentWnd),
				m_Impl (*this)
{
	CommonConstruct();
}

#pragma warning (default : 4355)

//****
void CBCGPDialog::CommonConstruct()
{
	m_hBkgrBitmap = NULL;
	m_sizeBkgrBitmap = CSize (0, 0);
	m_BkgrLocation = (BackgroundLocation) -1;
	m_bAutoDestroyBmp = FALSE;
	m_bIsLocal = FALSE;
	m_pLocaRes = NULL;
	m_bWasMaximized = FALSE;
}
//****
void CBCGPDialog::SetBackgroundColor (COLORREF color, BOOL bRepaint)
{
	if (m_brBkgr.IsValid())
	{
		m_brBkgr.DeleteObject();
	}

	if (color != (COLORREF)-1)
	{
		m_brBkgr.CreateSolidBrush (color);
	}

	if (bRepaint && GetSafeHwnd() != NULL)
	{
		Invalidate();
		UpdateWindow();
	}
}
//****
void CBCGPDialog::SetBackgroundImage (CBitmap* hBitmap, 
								BackgroundLocation location,
								BOOL bAutoDestroy,
								BOOL bRepaint)
{
	if (m_bAutoDestroyBmp && m_hBkgrBitmap != NULL)
	{
		::DeleteObject (m_hBkgrBitmap);
	}

	m_hBkgrBitmap = hBitmap;
	m_BkgrLocation = location;
	m_bAutoDestroyBmp = bAutoDestroy;

	if (hBitmap != NULL)
	{
		BITMAP bmp;
		::GetObject (hBitmap, sizeof (BITMAP), (LPVOID) &bmp);

		m_sizeBkgrBitmap = CSize (bmp.bmWidth, bmp.bmHeight);
	}
	else
	{
		m_sizeBkgrBitmap = CSize (0, 0);
	}

	if (bRepaint && GetSafeHwnd() != NULL)
	{
		Invalidate();
		UpdateWindow();
	}
}
//****
BOOL CBCGPDialog::SetBackgroundImage (UINT uiBmpResId,
									BackgroundLocation location,
									BOOL bRepaint)
{
	CBitmap* hBitmap = NULL;

	if (uiBmpResId != 0)
	{
		//-----------------------------
		// Try to load PNG image first:
		//-----------------------------
		CBCGPPngImage pngImage;
		if (pngImage.Load (MAKEINTRESOURCE (uiBmpResId)))
		{
			hBitmap = (CBitmap*) pngImage.Detach();
		}
		else
		{
			hBitmap = ::LoadBitmap (AfxGetResourceHandle(), MAKEINTRESOURCE (uiBmpResId));
		}

		if (hBitmap == NULL)
		{
			ASSERT (FALSE);
			return FALSE;
		}
	}

	SetBackgroundImage (hBitmap, location, TRUE /* Autodestroy */, bRepaint);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CBCGPDialog, CDialog)
	//{{AFX_MSG_MAP(CBCGPDialog)
	ON_WM_ACTIVATE()
	ON_WM_NCACTIVATE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SETTINGCHANGE()
	ON_WM_SIZE()
	ON_WM_NCPAINT()
	ON_WM_NCMOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCHITTEST()
	ON_WM_NCCALCSIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DWMCOMPOSITIONCHANGED, OnDWMCompositionChanged)
	ON_REGISTERED_MESSAGE(BCGM_CHANGEVISUALMANAGER, OnChangeVisualManager)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPDialog message handlers

void CBCGPDialog::OnActivate(UINT nState, CWnd *pWndOther, BOOL /*bMinimized*/) 
{
	m_Impl.OnActivate (nState, pWndOther);
}
//****
BOOL CBCGPDialog::OnNcActivate(BOOL bActive) 
{
	m_Impl.OnNcActivate (bActive);

	//-----------------------------------------------------------
	// Do not call the base class because it will call Default()
	// and we may have changed bActive.
	//-----------------------------------------------------------
	return (BOOL) DefWindowProc (WM_NCACTIVATE, bActive, 0L);
}
//****
BOOL CBCGPDialog::OnEraseBkgnd(CDC* pDC) 
{
	BOOL bRes = TRUE;

	if (m_brBkgr.GetSafeHandle() == NULL && m_hBkgrBitmap == NULL &&
		!IsVisualManagerStyle())
	{
		bRes = CDialog::OnEraseBkgnd (pDC);
	}
	else
	{
		ASSERT_VALID (pDC);

		CRect rectClient;
		GetClientRect (rectClient);

		if (m_BkgrLocation != BACKGR_TILE || m_hBkgrBitmap == NULL)
		{
			if (m_brBkgr.IsValid())
			{
				pDC->FillRect (rectClient, &m_brBkgr);
			}
			else if (IsVisualManagerStyle())
			{
				if (!CBCGPVisualManager::GetInstance()->OnFillDialog (pDC, this, rectClient))
				{
					CDialog::OnEraseBkgnd (pDC);
				}
			}
			else
			{
				CDialog::OnEraseBkgnd (pDC);
			}
		}

		if (m_hBkgrBitmap != NULL)
		{
			ASSERT (m_sizeBkgrBitmap != CSize (0, 0));

			if (m_BkgrLocation != BACKGR_TILE)
			{
				CPoint ptImage = rectClient.TopLeft();

				switch (m_BkgrLocation)
				{
				case BACKGR_TOPLEFT:
					break;

				case BACKGR_TOPRIGHT:
					ptImage.x = rectClient.r - m_sizeBkgrBitmap.w;
					break;

				case BACKGR_BOTTOMLEFT:
					ptImage.y = rectClient.b - m_sizeBkgrBitmap.h;
					break;

				case BACKGR_BOTTOMRIGHT:
					ptImage.x = rectClient.r - m_sizeBkgrBitmap.w;
					ptImage.y = rectClient.b - m_sizeBkgrBitmap.h;
					break;
				}

				pDC->DrawState (ptImage, m_sizeBkgrBitmap, m_hBkgrBitmap, DSS_NORMAL);
			}
			else
			{
				// Tile background image:
				for (int x = rectClient.l; x < rectClient.Width(); x += m_sizeBkgrBitmap.w)
				{
					for (int y = rectClient.t; y < rectClient.Height(); y += m_sizeBkgrBitmap.h)
					{
						pDC->DrawState (CPoint (x, y), m_sizeBkgrBitmap, m_hBkgrBitmap, DSS_NORMAL);
					}
				}
			}
		}
	}

	m_Impl.ClearAeroAreas (pDC);
	return bRes;
}
//****
void CBCGPDialog::OnDestroy() 
{
	if (m_bAutoDestroyBmp && m_hBkgrBitmap != NULL)
	{
		::DeleteObject (m_hBkgrBitmap);
		m_hBkgrBitmap = NULL;
	}

	m_Impl.OnDestroy();

	CDialog::OnDestroy();
}
//****
HBRUSH CBCGPDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if (m_brBkgr.IsValid() || m_hBkgrBitmap != NULL ||
		IsVisualManagerStyle())
	{
		HBRUSH hbr = m_Impl.OnCtlColor (pDC, pWnd, nCtlColor);
		if (hbr != NULL)
		{
			return hbr;
		}
	}	

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}
//****
BOOL CBCGPDialog::PreTranslateMessage(MSG* pMsg) 
{
	if (m_Impl.PreTranslateMessage (pMsg))
	{
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}
//****
void CBCGPDialog::SetActiveMenu (CBCGPPopupMenu* pMenu)
{
	m_Impl.SetActiveMenu (pMenu);
}
//****
BOOL CBCGPDialog::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (m_Impl.OnCommand (wParam, lParam))
	{
		return TRUE;
	}

	return CDialog::OnCommand(wParam, lParam);
}
//****
INT_PTR CBCGPDialog::DoModal() 
{
	if (m_bIsLocal)
	{
		m_pLocaRes = new CBCGPLocalResource();
	}

	return CDialog::DoModal();
}
//****
void CBCGPDialog::PreInitDialog()
{
	if (m_pLocaRes != NULL)
	{
		delete m_pLocaRes;
		m_pLocaRes = NULL;
	}
}
//****
void CBCGPDialog::OnSysColorChange() 
{
	CDialog::OnSysColorChange();
	
	if (AfxGetMainWnd() == this)
	{
		globalData.UpdateSysColors();
	}
}
//****
void CBCGPDialog::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	CDialog::OnSettingChange(uFlags, lpszSection);
	
	if (AfxGetMainWnd() == this)
	{
		globalData.OnSettingChange();
	}
}
//****
void CBCGPDialog::EnableVisualManagerStyle (BOOL bEnable, BOOL bNCArea, const CList<UINT,UINT>* plstNonSubclassedItems)
{
	ASSERT_VALID (this);

	m_Impl.EnableVisualManagerStyle (bEnable, bEnable && bNCArea, plstNonSubclassedItems);

	if (bEnable && bNCArea)
	{
		m_Impl.OnChangeVisualManager();
	}
}
//****
BOOL CBCGPDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_Impl.m_bHasBorder = (GetStyle() & WS_BORDER) != 0;

	if (IsVisualManagerStyle())
	{
		m_Impl.EnableVisualManagerStyle (TRUE, IsVisualManagerNCArea());
	}

	if (m_Impl.HasAeroMargins())
	{
		m_Impl.EnableAero (m_Impl.m_AeroMargins);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//****
BOOL CBCGPDialog::EnableAero (BCGPMARGINS& margins)
{
	return m_Impl.EnableAero (margins);
}
//****
void CBCGPDialog::GetAeroMargins (BCGPMARGINS& margins) const
{
	m_Impl.GetAeroMargins (margins);
}
//****
LRESULT CBCGPDialog::OnDWMCompositionChanged(WPARAM,LPARAM)
{
	m_Impl.OnDWMCompositionChanged();
	return 0;
}
//****
void CBCGPDialog::OnSize(UINT nType, int cx, int cy) 
{
	BOOL bIsMinimized = (nType == SIZE_MINIMIZED);

	if (m_Impl.IsOwnerDrawCaption())
	{
		CRect rectWindow;
		GetWindowRect (rectWindow);

		WINDOWPOS wndpos;
		wndpos.flags = SWP_FRAMECHANGED;
		wndpos.x     = rectWindow.l;
		wndpos.y     = rectWindow.t;
		wndpos.w    = rectWindow.Width();
		wndpos.h    = rectWindow.Height();

		m_Impl.OnWindowPosChanged (&wndpos);
	}

	m_Impl.UpdateCaption();

	if (!bIsMinimized && nType != SIZE_MAXIMIZED && !m_bWasMaximized)
	{
		return;
	}

	CDialog::OnSize(nType, cx, cy);

	m_bWasMaximized = (nType == SIZE_MAXIMIZED);
}
//****
void CBCGPDialog::OnNcPaint() 
{
	if (!m_Impl.OnNcPaint())
	{
		Default();
	}
}
//****
void CBCGPDialog::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	m_Impl.OnNcMouseMove (nHitTest, point);
	CDialog::OnNcMouseMove(nHitTest, point);
}
//****
void CBCGPDialog::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_Impl.OnLButtonUp (point);
	CDialog::OnLButtonUp(nFlags, point);
}
//****
void CBCGPDialog::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_Impl.OnLButtonDown (point);
	CDialog::OnLButtonDown(nFlags, point);
}
//****
BCGNcHitTestType CBCGPDialog::OnNcHitTest(CPoint point) 
{
	BCGNcHitTestType nHit = m_Impl.OnNcHitTest (point);
	if (nHit != HTNOWHERE)
	{
		return nHit;
	}

	return CDialog::OnNcHitTest(point);
}
//****
void CBCGPDialog::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	if (!m_Impl.OnNcCalcSize (bCalcValidRects, lpncsp))
	{
		CDialog::OnNcCalcSize(bCalcValidRects, lpncsp);
	}
}
//****
void CBCGPDialog::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_Impl.OnMouseMove (point);
	CDialog::OnMouseMove(nFlags, point);
}
//****
void CBCGPDialog::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	if ((lpwndpos->flags & SWP_FRAMECHANGED) == SWP_FRAMECHANGED)
	{
		m_Impl.OnWindowPosChanged (lpwndpos);
	}

	CDialog::OnWindowPosChanged(lpwndpos);
}
//****
LRESULT CBCGPDialog::OnChangeVisualManager (WPARAM, LPARAM)
{
	m_Impl.OnChangeVisualManager();
	return 0;
}
//****
void CBCGPDialog::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	m_Impl.OnGetMinMaxInfo (lpMMI);
	CDialog::OnGetMinMaxInfo(lpMMI);
}
//****
LRESULT CBCGPDialog::OnSetText(WPARAM, LPARAM) 
{
	LRESULT	lRes = Default();

	if (lRes && IsVisualManagerStyle() && IsVisualManagerNCArea())
	{
		RedrawWindow (NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW);
	}

	return lRes;
}
