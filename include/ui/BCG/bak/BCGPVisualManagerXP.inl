// BCGPVisualManagerXP.cpp: implementation of the CBCGPVisualManagerXP class.
//
//////////////////////////////////////////////////////////////////////


#include "BCGPVisualManagerXP.h"
#include "BCGPDrawManager.h"
#include "BCGPMenuBar.h"
#include "BCGPPopupMenu.h"
#include "BCGPToolbarMenuButton.h"
#include "BCGPOutlookBarPane.h"
#include "BCGPColorBar.h"
#include "BCGPBaseTabWnd.h"
#include "BCGPDockingControlBar.h"
#include "BCGPAutoHideDockBar.h"
#include "BCGGlobals.h"
#include "BCGPOutlookButton.h"
#include "BCGPMiniFrameWnd.h"
#include "BCGPTaskPaneMiniFrameWnd.h"
#include "BCGPToolbarEditBoxButton.h"
#include "BCGPTasksPane.h"
#include "BCGPCalendarBar.h"
#include "BCGPCalculator.h"
#include "BCGPToolBox.h"
#include "BCGPPopupWindow.h"
#include "BCGPPropList.h"
#include "CustomizeButton.h"
#include "BCGPRibbonButton.h"
#include "BCGPRibbonPanelMenu.h"
#include "BCGPRibbonComboBox.h"
#include "BCGPRibbonPanel.h"
#include "BCGPRibbonLabel.h"
#include "BCGPRibbonStatusBarPane.h"
#include "BCGPCaptionBar.h"
#include "BCGPRibbonColorButton.h"
#include "BCGPRibbonCategory.h"
#include "BCGPGridCtrl.h"

IMPLEMENT_DYNCREATE(CBCGPVisualManagerXP, CBCGPVisualManager)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPVisualManagerXP::CBCGPVisualManagerXP(BOOL bIsTemporary) :
		CBCGPVisualManager (bIsTemporary)
{
	m_bConnectMenuToParent = TRUE;

	m_nVertMargin = 4;
	m_nHorzMargin = 4;
	m_nGroupVertOffset = 4;
	m_nGroupCaptionHeight = 0;
	m_nGroupCaptionHorzOffset = 0;
	m_nGroupCaptionVertOffset = 0;
	m_nTasksHorzOffset = 12;
	m_nTasksIconHorzOffset = 5;
	m_nTasksIconVertOffset = 4;
	m_bActiveCaptions = FALSE;
	
	m_bMenuFlatLook = TRUE;
	m_bShadowHighlightedImage = TRUE;
	m_bEmbossDisabledImage = FALSE;
	m_bFadeInactiveImage = TRUE;
	m_bLook2000 = TRUE;
	m_nMenuShadowDepth = 4;
	m_nMenuBorderSize = 1;
	m_bShdowDroppedDownMenuButton = TRUE;

	m_bOfficeXPStyleMenus = TRUE;
	m_bDrawLastTabLine = TRUE;

	globalData.UpdateSysColors();
	OnUpdateSystemColors();
}
//****
CBCGPVisualManagerXP::~CBCGPVisualManagerXP()
{
}
//****
void CBCGPVisualManagerXP::OnUpdateSystemColors()
{
	CBCGPVisualManager::OnUpdateSystemColors();

	m_brBarBkgnd.DeleteObject();
	m_brMenuRarelyUsed.DeleteObject();
	m_brMenuLight.DeleteObject();
	m_brHighlight.DeleteObject();
	m_brHighlightDn.DeleteObject();
	m_brHighlightChecked.DeleteObject();

	m_brFloatToolBarBorder.DeleteObject();

	m_penSeparator.DeleteObject();
	m_brTabBack.DeleteObject();

	COLORREF clrTabBack;
	COLORREF clrFloatToolBarBorder;

	if (globalData.m_nBitsPerPixel > 8 && !globalData.IsHighContastMode())
	{
		COLORREF clrWindow = GetWindowColor();
		COLORREF clrFace = globalData.clrBarFace;

		m_clrMenuLight = _RGB (
			(219 * GetRV (clrWindow) + 36 * GetRV (clrFace)) / 255,
			(219 * GetGV (clrWindow) + 36 * GetGV (clrFace)) / 255,
			(219 * GetBV (clrWindow) + 36 * GetBV (clrFace)) / 255);

		double H, S, L;
		CBCGPDrawManager::RGBtoHSL (clrFace, &H, &S, &L);

		double S1;
		double L1;

		if (S < 0.1)
		{
			L1 = min (1., L + (1. - L) * .5);
			S1 = S == 0 ? 0 : min (1., S + .1);
		}
		else
		{
			L1 = min (1., 0.5 * L + 0.5);
			S1 = min (1., S * 2);
		}

		clrTabBack = CBCGPDrawManager::HLStoRGB_ONE (H, L1, S1);

		m_clrBarBkgnd = _RGB (
			(40 * GetRV (clrWindow) + 215 * GetRV (clrFace)) / 255,
			(40 * GetGV (clrWindow) + 215 * GetGV (clrFace)) / 255,
			(40 * GetBV (clrWindow) + 215 * GetBV (clrFace)) / 255);

		m_clrMenuRarelyUsed = CBCGPDrawManager::PixelAlpha (
			m_clrBarBkgnd, 94);

		m_clrInactiveTabText = CBCGPDrawManager::PixelAlpha (clrFace, 55);

		COLORREF clrHL = globalData.clrHilite;
		CBCGPDrawManager::RGBtoHSL (clrHL, &H, &S, &L);

		COLORREF clrMix = _RGB (
			(77 * GetRV (clrHL) + 178 * GetRV (m_clrMenuLight)) / 255,
			(77 * GetGV (clrHL) + 178 * GetGV (m_clrMenuLight)) / 255,
			(77 * GetBV (clrHL) + 178 * GetBV (m_clrMenuLight)) / 255);

		if (L > .8)	// The highlight color is very light
		{
			m_clrHighlight = CBCGPDrawManager::PixelAlpha (clrMix, 91);
			m_clrHighlightDn = CBCGPDrawManager::PixelAlpha (clrMix, 98);
			m_clrMenuItemBorder = CBCGPDrawManager::PixelAlpha (globalData.clrHilite, 84);
		}
		else
		{
			m_clrHighlight = CBCGPDrawManager::PixelAlpha (clrMix, 102);
			m_clrHighlightDn = CBCGPDrawManager::PixelAlpha (m_clrHighlight, 87);
			m_clrMenuItemBorder = globalData.clrHilite;
		}

		m_clrHighlightChecked = CBCGPDrawManager::PixelAlpha (_RGB (
			(GetRV (clrHL) + 5 * GetRV (m_clrMenuLight)) / 6,
			(GetGV (clrHL) + 5 * GetGV (m_clrMenuLight)) / 6,
			(GetBV (clrHL) + 5 * GetBV (m_clrMenuLight)) / 6), 
			100);

		m_clrSeparator = CBCGPDrawManager::PixelAlpha (
			globalData.clrBarFace, .86, .86, .86);

		m_clrPaneBorder = globalData.clrBarShadow;

		m_clrMenuBorder = CBCGPDrawManager::PixelAlpha (
			clrFace, 55);

		clrFloatToolBarBorder = CBCGPDrawManager::PixelAlpha (
			globalData.clrBarShadow, .85, .85, .85);

		m_clrGripper = CBCGPDrawManager::PixelAlpha (
			globalData.clrBarShadow, 110);
	}
	else
	{
		m_clrMenuLight = globalData.clrWindow;

		m_clrBarBkgnd = globalData.clrBtnFace;

		if (globalData.m_bIsBlackHighContrast)
		{
			m_clrHighlightChecked = m_clrHighlightDn = m_clrHighlight = globalData.clrHilite;
			m_clrMenuRarelyUsed = globalData.clrBtnFace;
		}
		else
		{
			m_clrHighlightDn = m_clrHighlight = globalData.clrBtnFace;
			m_clrHighlightChecked = globalData.clrWindow;
			m_clrMenuRarelyUsed = globalData.clrBarLight;
		}

		clrTabBack = globalData.clrBtnFace;
		m_clrInactiveTabText = globalData.clrBtnDkShadow;
		m_clrSeparator = globalData.clrBtnShadow;
		m_clrGripper = globalData.clrBtnShadow;
		m_clrPaneBorder = globalData.clrBtnShadow;
		m_clrMenuBorder = globalData.clrBtnDkShadow;
		clrFloatToolBarBorder = globalData.clrBtnShadow;

		m_clrMenuItemBorder = globalData.IsHighContastMode() ?
			globalData.clrBtnDkShadow : globalData.clrHilite;
	}

	m_brBarBkgnd.CreateSolidBrush (m_clrBarBkgnd);
	m_brMenuRarelyUsed.CreateSolidBrush (m_clrMenuRarelyUsed);
	m_brMenuLight.CreateSolidBrush (m_clrMenuLight);

	m_brHighlight.CreateSolidBrush (m_clrHighlight);
	m_brHighlightDn.CreateSolidBrush (m_clrHighlightDn);
	m_brHighlightChecked.CreateSolidBrush (m_clrHighlightChecked);
	m_brTabBack.CreateSolidBrush (clrTabBack);
	m_penSeparator.CreatePen (PS_SOLID, 1, m_clrSeparator);

	m_brFloatToolBarBorder.CreateSolidBrush (clrFloatToolBarBorder);

	m_clrPressedButtonBorder = (COLORREF)-1;	// Used in derived classes

	m_penMenuItemBorder.DeleteObject();
	m_penMenuItemBorder.CreatePen (PS_SOLID, 1, m_clrMenuItemBorder);

	m_clrTaskPaneHotText = globalData.clrHotText;
}
//****
void CBCGPVisualManagerXP::OnDrawBarGripper (CDC* pDC, CRect rectGripper, BOOL bHorz,
									   CBCGPBaseControlBar* pBar)
{
	ASSERT_VALID (pDC);

	if (pBar != NULL && pBar->IsDialogControl())
	{
		CBCGPVisualManager::OnDrawBarGripper (pDC, rectGripper, bHorz, pBar);
		return;
	}

	if (m_brGripperHorz.GetSafeHandle() == NULL)
	{
		CreateGripperBrush();
	}

	BOOL bSideBar = pBar != NULL && pBar->IsKindOf (RUNTIME_CLASS (CBCGPDockingControlBar));
	BOOL bMenuBar = pBar != NULL && pBar->IsKindOf (RUNTIME_CLASS (CBCGPMenuBar));

	CRect rectFill = rectGripper;

	if (!bSideBar)
	{
		if (bHorz)
		{
			int xCenter = rectFill.CenterPoint().x;
			rectFill.l = xCenter - 1;
			rectFill.r = xCenter + 2;
			rectFill.DeflateRect (0, 5);
		}
		else
		{
			int yCenter = rectFill.CenterPoint().y;
			rectFill.t = yCenter - 1;
			rectFill.b = yCenter + 2;
			rectFill.DeflateRect (5, 0);
		}
	}
	else
	{
		if (bHorz)
		{
			rectFill.DeflateRect (4, 0);
		}
		else
		{
			rectFill.DeflateRect (4, 0);
		}

		bHorz = !bHorz;
	}

	COLORREF clrTextOld = pDC->SetTextColor (m_clrGripper);
	COLORREF clrBkOld = pDC->SetBkColor (bSideBar || bMenuBar ? 
		globalData.clrBarFace : m_clrBarBkgnd);

	pDC->FillRect (rectFill, bHorz ? &m_brGripperHorz : &m_brGripperVert);

	if (bSideBar)
	{
		//------------------
		// Draw bar caption:
		//------------------
		int nOldBkMode = pDC->SetBkMode (OPAQUE);
		pDC->SetTextColor (globalData.clrBarText);

		const CFont& font = CBCGPMenuBar::GetMenuFont (bHorz);

		CFont* pOldFont = pDC->SelectObject ((CFont*) &font);

		CString strCaption;
		pBar->GetWindowText (strCaption);
		strCaption = _T(" ") + strCaption + _T(" ");

		CRect rectText = rectGripper;
		UINT uiTextFormat = 0;

		TEXTMETRIC tm;
		pDC->GetTextMetrics (&tm);

		CPoint ptTextOffset (0, 0);
		if (bHorz)
		{
			ptTextOffset.y = (rectGripper.Height() - tm.tmHeight - 1) / 2;
		}
		else
		{
			ptTextOffset.x = (rectGripper.Width() - tm.tmHeight + 1) / 2;
		}

		if (bHorz)
		{
			rectText.t += ptTextOffset.y;
			pDC->DrawText (strCaption, &rectText, uiTextFormat);
		}
		else
		{
			rectText.l = rectText.r - ptTextOffset.x;
			rectText.t = rectGripper.t + ptTextOffset.y;
			rectText.b = rectGripper.t + 3 * ptTextOffset.y;

			uiTextFormat |= DT_NOCLIP;

			pDC->DrawText (strCaption, &rectText, uiTextFormat);
		}

		pDC->SelectObject(pOldFont);
		pDC->SetBkMode(nOldBkMode);
	}

	pDC->SetTextColor (clrTextOld);
	pDC->SetBkColor (clrBkOld);
}
//****
void CBCGPVisualManagerXP::OnDrawMenuBorder (CDC* pDC, CBCGPPopupMenu* pMenu, CRect rect)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pMenu);

	const BOOL bRTL = pMenu->GetExStyle() & WS_EX_LAYOUTRTL;

	pDC->Draw3dRect (rect, m_clrMenuBorder, m_clrMenuBorder);
	rect.DeflateRect (1, 1);
	pDC->Draw3dRect (rect, m_clrMenuLight, m_clrMenuLight);

	CRect rectLeft (1, 1, 2, rect.b - 1);
	pDC->FillRect (rectLeft, &m_brBarBkgnd);

	//------------------------------------------------
	// Quick Customize Office XP like draw popup menu
	//------------------------------------------------
	CBCGPPopupMenu* pParentPopup = pMenu->GetParentPopupMenu();
	if (pParentPopup != NULL)
	{
		if (pParentPopup->IsQuickCustomize() && !bRTL)
		{
			CBCGPToolbarMenuButton* pParentBtn = pMenu->GetParentButton();
			if ((pParentBtn != NULL) && (pParentBtn->IsQuickMode()))
			{
				CBCGPPopupMenu* pParent = (CBCGPPopupMenu*)pMenu->GetParentPopupMenu();

				CRect rcParent;
				pParent->GetWindowRect(rcParent);

				CRect rcCurrent;
				pMenu->GetWindowRect(rcCurrent);

				CBCGPToolbarMenuButton* pBtn = pMenu->GetMenuItem(0);
				CRect rcButton = pBtn->Rect();

				CRect rectBorder;
				rectBorder.SetRectEmpty();

				if (rcParent.l > rcCurrent.l) 
				{
					if (rcParent.t <= rcCurrent.t)
					{
						rectBorder.SetRect (
							rect.r - 1,
							rect.t,
							rect.r + 1,
							rcButton.b);
					}
					else
					{
						// up
						rectBorder.SetRect (
							rect.r - 1,
							rect.b - rcButton.Height(), 
							rect.r + 1,
							rect.b);
					}
				}
				else
				{
					if (rcParent.t <= rcCurrent.t)
					{
						rectBorder.SetRect (
							rect.l - 1,
							rect.t, 
							rect.l + 1,
							rcButton.b);
					}
					else
					{
						// up
						rectBorder.SetRect (
							rect.l - 1, 
							rect.b - rcButton.Height(),
							rect.l + 1,
							rect.b);
					}
				}
				
				if (!rectBorder.IsRectEmpty())
				{
					pDC->FillRect (rectBorder, &m_brBarBkgnd);
				}
			}
		}
	}

	if (!CBCGPToolBar::IsCustomizeMode())
	{
		//-------------------------------------
		// "Connect" menu to the parent button:
		//-------------------------------------
		CBCGPToolbarMenuButton* pParentMenuBtn = pMenu->GetParentButton();
		if (m_bConnectMenuToParent &&
			pParentMenuBtn != NULL && pMenu->GetParentPopupMenu() == NULL && 
			pParentMenuBtn->IsBorder())
		{
			CRect rectConnect;
			rectConnect.SetRectEmpty();

			CRect rectParent = pParentMenuBtn->Rect();
			CWnd* pWnd = pParentMenuBtn->GetParentWnd();
			pWnd->ClientToScreen(rectParent);
			pMenu->ScreenToClient(&rectParent);

			switch (pMenu->GetDropDirection())
			{
			case CBCGPPopupMenu::DROP_DIRECTION_BOTTOM:
				rectConnect = CRect (rectParent.l + 1, rect.t - 1, rectParent.r - 1, rect.t);
				
				if (rectConnect.Width() > rect.Width() + 2)
				{
					return;
				}

				break;

			case CBCGPPopupMenu::DROP_DIRECTION_TOP:
				rectConnect = CRect (rectParent.l + 1, rect.b, rectParent.r - 1, rect.b + 1);

				if (rectConnect.Width() > rect.Width() + 2)
				{
					return;
				}

				break;

			case CBCGPPopupMenu::DROP_DIRECTION_RIGHT:
				rectConnect = CRect (rect.l - 1, rectParent.t + 1, rect.l, rectParent.b - 1);

				if (rectConnect.Height() > rect.Height() + 2)
				{
					return;
				}

				break;

			case CBCGPPopupMenu::DROP_DIRECTION_LEFT:
				rectConnect = CRect (rect.r, rectParent.t + 1, rect.r + 1, rectParent.b - 1);

				if (rectConnect.Height() > rect.Height() + 2)
				{
					return;
				}

				break;
			}

			CRect rectBorder = rect;
			rectBorder.InflateRect (1, 1);
			rectConnect.IntersectRect (&rectConnect, &rectBorder);
			rectParent.InflateRect (1, 1);
			rectConnect.IntersectRect (&rectConnect, &rectParent);

			pDC->FillRect (rectConnect, &m_brBarBkgnd);
		}
	}
}
//****
void CBCGPVisualManagerXP::OnDrawMenuShadow (CDC* pPaintDC, const CRect& rectClient, const CRect& rectExclude,
								int nDepth,  int iMinBrightness,  int iMaxBrightness,  
								CBitmap* pBmpSaveBottom,  CBitmap* pBmpSaveRight, BOOL bRTL)
{
	ASSERT_VALID (pPaintDC);
	ASSERT_VALID (pBmpSaveBottom);
	ASSERT_VALID (pBmpSaveRight);

	if (rectExclude.IsRectNull())
	{
		//------------------------
		// Simple draw the shadow:
		//------------------------
		CBCGPDrawManager dm (*pPaintDC);
		dm.DrawShadow (rectClient, nDepth, iMinBrightness, iMaxBrightness,
					pBmpSaveBottom, pBmpSaveRight, m_clrMenuShadowBase,
					!bRTL);
	}
	else
	{
		//--------------------------------------------
		// Copy screen content into the memory bitmap:
		//--------------------------------------------
		CDC dcMem;
		if (!dcMem.CreateCompatibleDC (pPaintDC))
		{
			ASSERT (FALSE);
			return;
		}

		//--------------------------------------------
		// Gets the whole menu and changes the shadow.
		//--------------------------------------------
		CRect rectBmp (0, 0, rectClient.Width(), rectClient.Height());
		int cx = rectBmp.Width() + nDepth;
		int cy = rectBmp.Height() + nDepth;
		CBitmap	bmpMem;
		if (!bmpMem.CreateCompatibleBitmap (pPaintDC, cx, cy))
		{
			ASSERT (FALSE);
			return;
		}

		CBitmap* pOldBmp = dcMem.SelectObject(&bmpMem);
		ASSERT (pOldBmp != NULL);

		dcMem.BitBlt (0, 0, cx, cy, pPaintDC, rectClient.l, rectClient.t, SRCCOPY);

		//-----------------
		// Draw the shadow:
		//-----------------
		CBCGPDrawManager dm (dcMem);
		dm.DrawShadow (rectBmp, nDepth, iMinBrightness, iMaxBrightness,
					pBmpSaveBottom, pBmpSaveRight, m_clrMenuShadowBase,
					!bRTL);

		//------------------------------------------
		// Do not cover rectExclude with the shadow:
		//------------------------------------------
		dcMem.BitBlt (rectExclude.l - rectClient.l, rectExclude.t - rectClient.t,
			rectExclude.Width(), rectExclude.Height(), 
			pPaintDC, rectExclude.l, rectExclude.t, SRCCOPY);

		//-----------------------------------------
		// Copy shadowed bitmap back to the screen:
		//-----------------------------------------
		pPaintDC->BitBlt (rectClient.l, rectClient.t, cx, cy, &dcMem, 0, 0, SRCCOPY);

		dcMem.SelectObject(pOldBmp);
	}
}
//****
void CBCGPVisualManagerXP::OnDrawBarBorder (CDC* pDC, CBCGPBaseControlBar* pBar, CRect& rect)
{
	ASSERT_VALID(pBar);
	ASSERT_VALID(pDC);

	if (pBar->IsDialogControl())
	{
		CBCGPVisualManager::OnDrawBarBorder (pDC, pBar, rect);
		return;
	}

	DWORD dwBarStyle = pBar->GetBarStyle();
	if (!(dwBarStyle & CBRS_BORDER_ANY))
	{
		return;
	}

	COLORREF clrBckOld = pDC->GetBkColor();	// FillSolidRect changes it

	COLORREF clrFill = pBar->IsOnGlass() ? _RGB (0, 0, 0) : globalData.clrBarFace;

	if (dwBarStyle & CBRS_BORDER_LEFT)
		pDC->FillSolidRect(0, 0, 1, rect.Height() - 1, clrFill);
	if (dwBarStyle & CBRS_BORDER_TOP)
		pDC->FillSolidRect(0, 0, rect.Width()-1 , 1, clrFill);
	if (dwBarStyle & CBRS_BORDER_RIGHT)
		pDC->FillSolidRect(rect.r, 0/*RGL~:1*/, -1,
			rect.Height()/*RGL-: - 1*/, clrFill);	
	if (dwBarStyle & CBRS_BORDER_BOTTOM)
		pDC->FillSolidRect(0, rect.b, rect.Width()-1, -1, clrFill);

	if (dwBarStyle & CBRS_BORDER_LEFT)
		++rect.l;
	if (dwBarStyle & CBRS_BORDER_TOP)
		++rect.t;
	if (dwBarStyle & CBRS_BORDER_RIGHT)
		--rect.r;
	if (dwBarStyle & CBRS_BORDER_BOTTOM)
		--rect.b;

	// Restore Bk color:
	pDC->SetBkColor (clrBckOld);
}
//****
void CBCGPVisualManagerXP::OnFillBarBackground (CDC* pDC, CBCGPBaseControlBar* pBar,
									CRect rectClient, CRect rectClip,
									BOOL /*bNCArea*/)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pBar);

	if (pBar->IsOnGlass())
	{
		pDC->FillSolidRect (rectClient, _RGB (0, 0, 0));
		return;
	}

	if (DYNAMIC_DOWNCAST (CReBar, pBar) != NULL ||
		DYNAMIC_DOWNCAST (CReBar, pBar->GetParent()))
	{
		FillRebarPane (pDC, pBar, rectClient);
		return;
	}

	if (rectClip.IsRectEmpty())
	{
		rectClip = rectClient;
	}

	CRuntimeClass* pBarClass = pBar->GetRuntimeClass();

	if (pBarClass == NULL || pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPMenuBar)))
	{
		CBCGPVisualManager::OnFillBarBackground (pDC, pBar, rectClient, rectClip);
		return;
	}

	if (pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPOutlookBarPane)))
	{
		CBCGPOutlookBarPane* pOlBar = DYNAMIC_DOWNCAST (CBCGPOutlookBarPane, pBar);
		ASSERT_VALID (pOlBar);

		if (pOlBar->IsBackgroundTexture())
		{
			CBCGPVisualManager::OnFillBarBackground (pDC, pBar, rectClient, rectClip);
			return;
		}
	}

	if (pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPColorBar)))
	{
		if (pBar->IsDialogControl())
		{
			CBCGPVisualManager::OnFillBarBackground (pDC, pBar, rectClient, rectClip);
		}
		else
		{
			pDC->FillRect (rectClip, 
				((CBCGPColorBar*) pBar)->IsTearOff() ?
					&m_brBarBkgnd : &m_brMenuLight);
		}

		return;
	}

	if (pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPCalculator)))
	{
		if (pBar->IsDialogControl())
		{
			CBCGPVisualManager::OnFillBarBackground (pDC, pBar, rectClient, rectClip);
		}
		else
		{
			pDC->FillRect (rectClip, &m_brMenuLight);
		}

		return;
	}

	if (pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPCalendarBar)))
	{
		pDC->FillRect (rectClip, &m_brMenuLight);

		CRect rectImages = rectClient;
		rectImages.r = rectImages.l + CBCGPToolBar::GetMenuImageSize().w + 
					2 * GetMenuImageMargin() + 1;
		return;
	}

    if (pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPPopupMenuBar)))
	{
		pDC->FillRect (rectClip, &m_brMenuLight);

		CBCGPPopupMenuBar* pMenuBar = DYNAMIC_DOWNCAST (CBCGPPopupMenuBar, pBar);
		if (!pMenuBar->m_bDisableSideBarInXPMode)
		{
			CRect rectImages = rectClient;

			rectImages.r = rectImages.l + pMenuBar->GetGutterWidth();
			rectImages.DeflateRect (0, 1);

			pDC->FillRect (rectImages, &m_brBarBkgnd);
		}

		return;
	}

	if (pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPToolBar)))
	{
		if (pBar->IsDialogControl())
		{
			CBCGPVisualManager::OnFillBarBackground (pDC, pBar, rectClient, rectClip);
		}
		else
		{
			pDC->FillRect (rectClip, &m_brBarBkgnd);
		}

		return;
	}

	if (pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPAutoHideDockBar)))
	{
		pDC->FillRect (rectClip, &m_brTabBack);
		return;
	}

	CBCGPVisualManager::OnFillBarBackground (pDC, pBar, rectClient, rectClip);
}
//****
void CBCGPVisualManagerXP::OnDrawSeparator (CDC* pDC, CBCGPBaseControlBar* pBar,
										 CRect rect, BOOL bHorz)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pBar);

	if (pBar->IsDialogControl())
	{
		CBCGPVisualManager::OnDrawSeparator (pDC, pBar, rect, bHorz);
		return;
	}

	CRect rectSeparator = rect;

	int x1, x2;
	int y1, y2;

	if (bHorz)
	{
		x1 = x2 = (rect.l + rect.r) / 2;
		y1 = rect.t;
		y2 = rect.b - 1;
	}
	else
	{
		y1 = y2 = (rect.t + rect.b) / 2;
		x1 = rect.l;
		x2 = rect.r;

		BOOL bIsRibbon = FALSE;

#ifndef BCGP_EXCLUDE_RIBBON
		bIsRibbon = pBar->IsKindOf (RUNTIME_CLASS (CBCGPRibbonPanelMenuBar));

		if (bIsRibbon && ((CBCGPRibbonPanelMenuBar*) pBar)->IsDefaultMenuLook())
		{
			bIsRibbon = FALSE;
		}
#endif

		if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar)) &&
			!bIsRibbon &&
			!pBar->IsKindOf (RUNTIME_CLASS (CBCGPColorBar)) && 
			!pBar->IsKindOf (RUNTIME_CLASS (CBCGPCalculator)) && 
			!pBar->IsKindOf (RUNTIME_CLASS (CBCGPCalendarBar)))
		{
			
			x1 = rect.l + CBCGPToolBar::GetMenuImageSize().w + 
						GetMenuImageMargin() + 1;

			CRect rectBar;
			pBar->GetClientRect (rectBar);

			if (rectBar.r - x2 < 50) // Last item in row
			{
				x2 = rectBar.r;
			}

			if (((CBCGPPopupMenuBar*) pBar)->m_bDisableSideBarInXPMode)
			{
				x1 = 0;
			}

			//---------------------------------
			//	Maybe Quick Customize separator
			//---------------------------------
			if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar)))
			{
				CWnd* pWnd = pBar->GetParent();
				if (pWnd != NULL && pWnd->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenu)))
				{
					CBCGPPopupMenu* pMenu = (CBCGPPopupMenu*)pWnd;
					if (pMenu->IsCustomizePane())
					{
						x1 = rect.l + 2*CBCGPToolBar::GetMenuImageSize().w + 
								3*GetMenuImageMargin() + 2;
					}
				}
			}
		}
	}

	if (CBCGPToolBarImages::m_bIsDrawOnGlass)
	{
		CBCGPDrawManager dm (*pDC);
		dm.DrawLine (x1, y1, x2, y2, m_clrSeparator);
	}
	else
	{
		CPen* pOldPen = pDC->SelectObject (&m_penSeparator);
		ASSERT (pOldPen != NULL);

		pDC->MoveTo (x1, y1);
		pDC->LineTo (x2, y2);
		
		pDC->SelectObject (pOldPen);
	}
}
//****
void CBCGPVisualManagerXP::OnDrawButtonBorder (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, BCGBUTTON_STATE state)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (state != ButtonsIsPressed && state != ButtonsIsHighlighted)
	{
		ASSERT ((pButton->m_nStyle & TBBS_CHECKED) == 0);
		return;
	}

	COLORREF clrBorder = m_clrMenuItemBorder;

	CBCGPToolbarMenuButton* pMenuButton = 
		DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton, pButton);
	BOOL bIsMenuButton = pMenuButton != NULL;

	BOOL bIsPopupMenu = bIsMenuButton &&
		pMenuButton->GetParentWnd() != NULL &&
		pMenuButton->GetParentWnd()->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar));

	BOOL bIsPressedBorder = !bIsPopupMenu;

	if (bIsMenuButton && !bIsPopupMenu && 
		pMenuButton->IsDroppedDown())
	{
		bIsPressedBorder = FALSE;

		CBCGPPopupMenu* pPopupMenu= pMenuButton->GetPopupMenu();
		if (pPopupMenu != NULL && 
			(pPopupMenu->IsWindowVisible() || pPopupMenu->IsShown()))
		{
			clrBorder = m_clrMenuBorder;
			ExtendMenuButton (pMenuButton, rect);

			BOOL bRTL = pPopupMenu->GetExStyle() & WS_EX_LAYOUTRTL;

			if (m_bShdowDroppedDownMenuButton && !bRTL && 
				CBCGPMenuBar::IsMenuShadows() &&
				!CBCGPToolBar::IsCustomizeMode() &&
				globalData.m_nBitsPerPixel > 8 &&
				!globalData.IsHighContastMode() &&
				!pPopupMenu->IsRightAlign())
			{
				CBCGPDrawManager dm (*pDC);

				dm.DrawShadow (rect, m_nMenuShadowDepth, 100, 75, NULL, NULL,
					m_clrMenuShadowBase);
			}
		}
	}

	const BOOL bIsChecked = (pButton->m_nStyle & TBBS_CHECKED);

	switch (state)
	{
	case ButtonsIsPressed:
		if (bIsPressedBorder && m_clrPressedButtonBorder != (COLORREF)-1 &&
			!bIsChecked &&
			rect.Width() > 5 && rect.Height() > 5)
		{
			clrBorder = m_clrPressedButtonBorder;
		}

	case ButtonsIsHighlighted:
		if (bIsPopupMenu && bIsChecked)
		{
			if (pButton->m_nStyle & TBBS_MARKED)
			{
				clrBorder = m_clrPressedButtonBorder;
			}

			rect.b ++;
		}

		if (CBCGPToolBarImages::m_bIsDrawOnGlass)
		{
			CBCGPDrawManager dm (*pDC);
			dm.DrawRect (rect, (COLORREF)-1, clrBorder);
		}
		else
		{
			pDC->Draw3dRect (rect, clrBorder, clrBorder);
		}
	}
}
//****
void CBCGPVisualManagerXP::OnFillButtonInterior (CDC* pDC,
	CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (state != ButtonsIsPressed && state != ButtonsIsHighlighted)
	{
		return;
	}

	if (CBCGPToolBar::IsCustomizeMode() && 
		!CBCGPToolBar::IsAltCustomizeMode() && !pButton->IsLocked())
	{
		return;
	}

	CBCGPToolbarMenuButton* pMenuButton = 
		DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton, pButton);
	BOOL bIsMenuButton = pMenuButton != NULL;

	BOOL bIsPopupMenu = bIsMenuButton &&
		pMenuButton->GetParentWnd() != NULL &&
		pMenuButton->GetParentWnd()->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar));

	if (!bIsPopupMenu && !m_bEnableToolbarButtonFill)
	{
		return;
	}

	CBrush* pBrush = ((pButton->m_nStyle & TBBS_PRESSED) && !bIsPopupMenu) ? 
		&m_brHighlightDn : &m_brHighlight;

	if (bIsMenuButton && !bIsPopupMenu && pMenuButton->IsDroppedDown())
	{
		ExtendMenuButton (pMenuButton, rect);
		pBrush = &m_brBarBkgnd;
	}
	
	if (pButton->m_nStyle & TBBS_CHECKED)
	{
		pBrush = (state == ButtonsIsHighlighted) ? 
				&m_brHighlightDn : &m_brHighlightChecked;
	}

	if (bIsMenuButton && (pButton->m_nStyle & TBBS_DISABLED))
	{
		pBrush = &m_brMenuLight;
	}

	switch (state)
	{
	case ButtonsIsPressed:
	case ButtonsIsHighlighted:
		if ((pButton->m_nStyle & TBBS_CHECKED) == 0)
		{
			rect.DeflateRect (1, 1);
		}

		OnFillHighlightedArea (pDC, rect, pBrush, pButton);
	}
}
//****
void CBCGPVisualManagerXP::OnHighlightMenuItem (CDC* pDC, CBCGPToolbarMenuButton* pButton,
											CRect rect, COLORREF& clrText)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	CBrush* pBrush = (pButton->m_nStyle & TBBS_DISABLED) ? 
					&m_brMenuLight : &m_brHighlight;

	rect.DeflateRect (1, 0);

	OnFillHighlightedArea (pDC, rect, pBrush, pButton);
	pDC->Draw3dRect (rect, m_clrMenuItemBorder, m_clrMenuItemBorder);

	clrText = GetHighlightedMenuItemTextColor (pButton);
}
//****
COLORREF CBCGPVisualManagerXP::GetHighlightedMenuItemTextColor (CBCGPToolbarMenuButton* pButton)
{
	ASSERT_VALID (pButton);

	if (pButton->m_nStyle & TBBS_DISABLED)
	{
		return globalData.clrGrayedText;
	}

	if (GetRV (m_clrHighlight) > 128 &&
		GetGV (m_clrHighlight) > 128 &&
		GetBV (m_clrHighlight) > 128)
	{
		return _RGB (0, 0, 0);
	}
	else
	{
		return _RGB (255, 255, 255);
	}
}
//****
void CBCGPVisualManagerXP::OnHighlightQuickCustomizeMenuButton (CDC* pDC, 
	CBCGPToolbarMenuButton* /*pButton*/, CRect rect)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &m_brBarBkgnd);
	pDC->Draw3dRect (rect, m_clrMenuBorder, m_clrMenuBorder);
}
//****
void CBCGPVisualManagerXP::OnHighlightRarelyUsedMenuItems (CDC* pDC, CRect rectRarelyUsed)
{
	ASSERT_VALID (pDC);

	rectRarelyUsed.l --;
	rectRarelyUsed.r = rectRarelyUsed.l + CBCGPToolBar::GetMenuImageSize().w + 
		2 * GetMenuImageMargin() + 2;

	pDC->FillRect (rectRarelyUsed, &m_brMenuRarelyUsed);
}
//****
void CBCGPVisualManagerXP::OnDrawTab (CDC* pDC, CRect rectTab,
						int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd)
{
	#define TEXT_MARGIN				4
	#define IMAGE_MARGIN			4

	ASSERT_VALID (pTabWnd);
	ASSERT_VALID (pDC);

	if (pTabWnd->IsFlatTab() || pTabWnd->IsOneNoteStyle() ||
		pTabWnd->IsColored() || pTabWnd->IsVS2005Style() ||
		pTabWnd->IsLeftRightRounded())
	{
		CBCGPVisualManager::OnDrawTab (pDC, rectTab, iTab, bIsActive, pTabWnd);
		return;
	}

	COLORREF	clrDark;
	COLORREF	clrBlack;
	COLORREF	clrHighlight;
	COLORREF	clrFace;
	COLORREF	clrDarkShadow;
	COLORREF	clrLight;
	CBrush*		pbrFace = NULL;
	CBrush*		pbrBlack = NULL;
				   
	GetTabFrameColors (
		pTabWnd, clrDark, clrBlack, clrHighlight, clrFace, clrDarkShadow, clrLight,
		pbrFace, pbrBlack);

	CPen penGray (PS_SOLID, 1, clrDark);
	CPen penDkGray (PS_SOLID, 1, clrBlack);
	CPen penHiLight (PS_SOLID, 1, clrHighlight);
	
	CPen* pOldPen = pDC->SelectObject (&penGray);
	ASSERT (pOldPen != NULL);
	
	if (iTab != pTabWnd->GetActiveTab() - 1)
	{
		if (iTab < pTabWnd->GetVisibleTabsNum() - 1 || m_bDrawLastTabLine)
		{
			pDC->MoveTo (rectTab.r, rectTab.t + 3);
			pDC->LineTo (rectTab.r, rectTab.b - 3);
		}
	}

	if (bIsActive)
	{
		if (pTabWnd->GetLocation() == CBCGPBaseTabWnd::LOCATION_BOTTOM)
		{
			CRect rectFace = rectTab;
			rectFace.t--;
			
			OnFillTab (pDC, rectFace, pbrFace, iTab, bIsActive, pTabWnd);
			
			pDC->SelectObject (&penDkGray);

			pDC->MoveTo (rectTab.r, rectTab.t);
			pDC->LineTo (rectTab.r, rectTab.b);
			pDC->LineTo (rectTab.l, rectTab.b);

			pDC->SelectObject (&penHiLight);
			pDC->LineTo (rectTab.l, rectTab.t - 2);
		}
		else
		{
			CPen penLight (PS_SOLID, 1, m_clrMenuLight);

			CRect rectFace = rectTab;
			rectFace.b++;
			rectFace.l++;
			
			OnFillTab (pDC, rectFace, pbrFace, iTab, bIsActive, pTabWnd);
			
			pDC->SelectObject (&penDkGray);
			pDC->MoveTo (rectTab.r, rectTab.b);
			pDC->LineTo (rectTab.r, rectTab.t);

			pDC->SelectObject (&penHiLight);

			pDC->LineTo (rectTab.r, rectTab.t);
			pDC->LineTo(rectTab.l, rectTab.t);
			pDC->LineTo(rectTab.l, rectTab.b);
		}
	}

	pDC->SelectObject (pOldPen);

	COLORREF clrText;

	if (pTabWnd->IsDialogControl())
	{
		clrText = globalData.clrBtnText;
	}
	else
	{
		clrText = bIsActive ? globalData.clrBarText : m_clrInactiveTabText;
	}

	OnDrawTabContent (pDC, rectTab, iTab, bIsActive, pTabWnd, clrText);
}
//****
void CBCGPVisualManagerXP::OnFillTab (CDC* pDC, CRect rectFill, CBrush* pbrFill,
									 int iTab, BOOL bIsActive, 
									 const CBCGPBaseTabWnd* pTabWnd)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pbrFill);
	ASSERT_VALID (pTabWnd);

	if (pTabWnd->GetTabBkColor (iTab) != (COLORREF)-1 && !bIsActive)
	{
		CBrush br (pTabWnd->GetTabBkColor (iTab));
		pDC->FillRect (rectFill, &br);
		return;
	}

	if (pTabWnd->IsOneNoteStyle() || pTabWnd->IsVS2005Style() ||
		pTabWnd->IsLeftRightRounded())
	{
		CBCGPVisualManager::OnFillTab (pDC, rectFill, pbrFill,
									 iTab, bIsActive, pTabWnd);
	}
	else if (bIsActive)
	{
		pDC->FillRect (rectFill, pbrFill);
	}
}
//****
void CBCGPVisualManagerXP::OnEraseTabsArea (CDC* pDC, CRect rect, 
										 const CBCGPBaseTabWnd* pTabWnd)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pTabWnd);

	if (pTabWnd->IsFlatTab())
	{
		CBCGPVisualManager::OnEraseTabsArea (pDC, rect, pTabWnd);
		return;
	}

	if (pTabWnd->IsDialogControl())
	{
		if (pTabWnd->IsVisualManagerStyle())
		{
			OnFillDialog (pDC, pTabWnd->GetParent(), rect);
		}
		else
		{
			pDC->FillRect (rect, &globalData.brBtnFace);
		}
		return;
	}

	pDC->FillRect (rect, &m_brTabBack);
}
//****
COLORREF CBCGPVisualManagerXP::OnDrawControlBarCaption (CDC* pDC, CBCGPDockingControlBar* /*pBar*/, 
			BOOL bActive, CRect rectCaption, CRect /*rectButtons*/)
{
	ASSERT_VALID (pDC);

	CPen pen (PS_SOLID, 1, 
		bActive ? globalData.clrBarLight : globalData.clrBarShadow);
	CPen* pOldPen = pDC->SelectObject (&pen);

	CBrush* pOldBrush = (CBrush*) pDC->SelectObject (
		bActive ? &globalData.brActiveCaption : &globalData.brBarFace);

	if (bActive)
	{
		rectCaption.InflateRect (1, 1);
	}

	pDC->RoundRect (rectCaption, CPoint (2, 2));

	pDC->SelectObject (pOldBrush);
	pDC->SelectObject (pOldPen);

    // get the text color
    COLORREF clrCptnText = bActive ?
        globalData.clrCaptionText :
        globalData.clrBarText;

	return clrCptnText;
}
//****
void CBCGPVisualManagerXP::OnDrawCaptionButton (CDC* pDC, CBCGPCaptionButton* pButton, 
											BOOL bActive,
											BOOL bHorz, BOOL bMaximized, BOOL bDisabled,
											int nImageID /*= -1*/)
{
	ASSERT_VALID (pDC);
	ASSERT (pButton != NULL);

    CRect rc = pButton->GetRect();

	if (pButton->m_bPushed && (pButton->m_bFocused || pButton->m_bDroppedDown) && !bDisabled)
	{
		OnFillHighlightedArea (pDC, rc, &m_brHighlightDn, NULL);
		bActive = TRUE;
	}
	else if (pButton->m_bPushed || pButton->m_bFocused || pButton->m_bDroppedDown)
	{
		if (!bDisabled)
		{
			OnFillHighlightedArea (pDC, rc, &m_brHighlight, NULL);
		}

		bActive = FALSE;
	}

	CBCGPMenuImages::IMAGES_IDS id = (CBCGPMenuImages::IMAGES_IDS)-1;
	
	if (nImageID != -1)
	{
		id = (CBCGPMenuImages::IMAGES_IDS)nImageID;
	}
	else
	{
		id = pButton->GetIconID (bHorz, bMaximized);
	}

	if (id != (CBCGPMenuImages::IMAGES_IDS)-1)
	{
		CSize sizeImage = CBCGPMenuImages::Size();
		CPoint ptImage (rc.l + (rc.Width() - sizeImage.w) / 2,
						rc.t + (rc.Height() - sizeImage.h) / 2);

		OnDrawCaptionButtonIcon (pDC, pButton, id, bActive, bDisabled, ptImage);
	}

	if ((pButton->m_bPushed || pButton->m_bFocused || pButton->m_bDroppedDown) && !bDisabled)
	{
		COLORREF clrDark = globalData.clrBarDkShadow;
		pDC->Draw3dRect (rc, clrDark, clrDark);
	}
}
//****
void CBCGPVisualManagerXP::OnDrawCaptionButtonIcon (CDC* pDC, 
													CBCGPCaptionButton* pButton,
													CBCGPMenuImages::IMAGES_IDS id,
													BOOL bActive, BOOL bDisabled,
													CPoint ptImage)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	CBCGPMenuImages::IMAGE_STATE imageState;
	
	if (bDisabled)
	{
		imageState = CBCGPMenuImages::ImageGray;
	}
	else if (pButton->m_bFocused || pButton->m_bPushed)
	{
		COLORREF clrBack = pButton->m_bPushed ? m_clrHighlightDn : m_clrHighlight;

		if (GetRV (clrBack) <= 192 &&
			GetGV (clrBack) <= 192 &&
			GetBV (clrBack) <= 192)
		{
			imageState = CBCGPMenuImages::ImageWhite;
		}
		else
		{
			imageState = CBCGPMenuImages::ImageBlack;
		}
	}
	else if (pButton->m_clrForeground == (COLORREF)-1)
	{
		imageState = bActive ? CBCGPMenuImages::ImageWhite : CBCGPMenuImages::ImageBlack;
	}
	else
	{
		if (GetRV (pButton->m_clrForeground) > 192 &&
			GetGV (pButton->m_clrForeground) > 192 &&
			GetBV (pButton->m_clrForeground) > 192)
		{
			imageState = CBCGPMenuImages::ImageWhite;
		}
		else
		{
			imageState = CBCGPMenuImages::ImageBlack;
		}
	}

	CBCGPMenuImages::Draw (pDC, id, ptImage, imageState);
}
//****
COLORREF CBCGPVisualManagerXP::OnFillCommandsListBackground (CDC* pDC, CRect rect, BOOL bIsSelected)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	COLORREF clrText = globalData.clrBarText;

	int iImageWidth = CBCGPToolBar::GetMenuImageSize().w + GetMenuImageMargin();

	if (bIsSelected)
	{
		if (m_bEnableToolbarButtonFill)
		{
			rect.l = 0;
		}

		OnFillHighlightedArea (pDC, rect, &m_brHighlight, NULL);

		pDC->Draw3dRect (rect, m_clrMenuItemBorder, m_clrMenuItemBorder);

		// Now, we should define a menu text color...
		if (GetRV (m_clrHighlight) > 128 &&
			GetGV (m_clrHighlight) > 128 &&
			GetBV (m_clrHighlight) > 128)
		{
			clrText = _RGB (0, 0, 0);
		}
		else
		{
			clrText = _RGB (255, 255, 255);
		}
	}
	else
	{
		pDC->FillRect (rect, &m_brMenuLight);

		CRect rectImages = rect;
		rectImages.r = rectImages.l + iImageWidth + MENU_IMAGE_MARGIN;

		pDC->FillRect (rectImages, &m_brBarBkgnd);

		clrText = globalData.clrBarText;
	}

	return clrText;
}
//****
void CBCGPVisualManagerXP::OnDrawMenuArrowOnCustomizeList (CDC* pDC, 
	CRect rectCommand, BOOL /*bSelected*/)
{
	ASSERT_VALID (pDC);

	CRect rectTriangle = rectCommand;
	rectTriangle.l = rectTriangle.r - CBCGPMenuImages::Size().w;

	CBCGPMenuImages::IMAGE_STATE state = CBCGPMenuImages::ImageBlack;
	COLORREF clrPen = _RGB (0, 0, 0);

	if (GetRV (m_clrHighlight) < 128 ||
		GetGV (m_clrHighlight) < 128 ||
		GetBV (m_clrHighlight) < 128)
	{
		state = CBCGPMenuImages::ImageWhite;
		clrPen = _RGB (255, 255, 255);
	}

	CBCGPMenuImages::Draw (pDC, CBCGPMenuImages::IdArowRightLarge, rectTriangle, state);

	CPen penLine (PS_SOLID, 1, clrPen);
	CPen* pOldPen = pDC->SelectObject (&penLine);
	ASSERT (pOldPen != NULL);

	pDC->MoveTo (rectTriangle.l - 1, rectCommand.t + 2);
	pDC->LineTo (rectTriangle.l - 1, rectCommand.b - 2);

	pDC->SelectObject (pOldPen);
}
//****
void CBCGPVisualManagerXP::OnDrawTearOffCaption (CDC* pDC, CRect rect, BOOL bIsActive)
{
	const int iBorderSize = 1;
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &m_brMenuLight);

	rect.DeflateRect (iBorderSize, iBorderSize);
	OnFillHighlightedArea (pDC, rect, bIsActive ? &m_brHighlight : &m_brBarBkgnd,
		NULL);
	
	// Draw gripper:
	int nGripperWidth = max (20, CBCGPToolBar::GetMenuImageSize().w * 2);

	CRect rectGripper = rect;
	rectGripper.DeflateRect ((rectGripper.Width() - nGripperWidth) / 2, 1);

	if (m_brGripperHorz.GetSafeHandle() == NULL)
	{
		CreateGripperBrush();
	}

	COLORREF clrTextOld = pDC->SetTextColor (bIsActive ?
		globalData.clrBarDkShadow : globalData.clrBarShadow);
	COLORREF clrBkOld = pDC->SetBkColor (
		bIsActive ? m_clrHighlight : m_clrBarBkgnd);

	if (bIsActive)
	{
		rectGripper.DeflateRect (0, 1);
	}

	pDC->FillRect (rectGripper, &m_brGripperHorz);

	pDC->SetTextColor (clrTextOld);
	pDC->SetBkColor (clrBkOld);

	if (bIsActive)
	{
		pDC->Draw3dRect (rect, globalData.clrBarDkShadow, globalData.clrBarDkShadow);
	}
}
//****
void CBCGPVisualManagerXP::OnDrawMenuResizeBar (CDC* pDC, CRect rect, int nResizeFlags)
{
	ASSERT_VALID (pDC);

	const int iBorderSize = 1;
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &m_brMenuLight);

	rect.DeflateRect (iBorderSize, iBorderSize);

	OnFillHighlightedArea (pDC, rect, &m_brBarBkgnd, NULL);
	
	CRect rectGripper = rect;

	if (nResizeFlags == (int) CBCGPPopupMenu::MENU_RESIZE_BOTTOM_RIGHT ||
		nResizeFlags == (int) CBCGPPopupMenu::MENU_RESIZE_TOP_RIGHT)
	{
		rectGripper.l = rectGripper.r - rectGripper.Height();
	}
	else
	{
		rectGripper.l = rectGripper.CenterPoint().x - rectGripper.Height() / 2;
		rectGripper.r = rectGripper.l + rectGripper.Height();
	}

	rectGripper.DeflateRect (2, 2);

	if (m_brGripperHorz.GetSafeHandle() == NULL)
	{
		CreateGripperBrush();
	}

	COLORREF clrTextOld = pDC->SetTextColor (globalData.clrBarShadow);
	COLORREF clrBkOld = pDC->SetBkColor (m_clrBarBkgnd);

	pDC->FillRect (rectGripper, &m_brGripperHorz);

	pDC->SetTextColor (clrTextOld);
	pDC->SetBkColor (clrBkOld);
}
//****
void CBCGPVisualManagerXP::OnDrawMenuScrollButton (CDC* pDC, CRect rect, BOOL bIsScrollDown, 
												 BOOL bIsHighlited, BOOL /*bIsPressed*/,
												 BOOL /*bIsDisabled*/)
{
	ASSERT_VALID (pDC);

	rect.t --;

	pDC->FillRect (rect, &globalData.brBarFace);

	CBCGPMenuImages::Draw (pDC, bIsScrollDown ? CBCGPMenuImages::IdArowDown : CBCGPMenuImages::IdArowUp, rect);

	if (bIsHighlited)
	{
		CPen pen (PS_SOLID, 1, globalData.clrBarShadow);
		CPen* pOldPen = pDC->SelectObject (&pen);

		CBrush* pOldBrush = (CBrush*) pDC->SelectStockObject (NULL_BRUSH);

		rect.DeflateRect (1, 1);
		pDC->RoundRect (rect, CPoint (2, 2));

		pDC->SelectObject (pOldBrush);
		pDC->SelectObject (pOldPen);
	}
}
//****
void CBCGPVisualManagerXP::CreateGripperBrush()
{
	ASSERT (m_brGripperHorz.GetSafeHandle() == NULL);
	ASSERT (m_brGripperVert.GetSafeHandle() == NULL);

	WORD horzHatchBits [8] = { 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00 };

	CBitmap bmpGripperHorz;
	bmpGripperHorz.CreateBitmap (8, 8, 1, 1, horzHatchBits);

	m_brGripperHorz.CreatePatternBrush (&bmpGripperHorz);

	WORD vertHatchBits[8] = { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA };

	CBitmap bmpGripperVert;
	bmpGripperVert.CreateBitmap (8, 8, 1, 1, vertHatchBits);

	m_brGripperVert.CreatePatternBrush (&bmpGripperVert);
}
//****
void CBCGPVisualManagerXP::ExtendMenuButton (CBCGPToolbarMenuButton* pMenuButton,
											CRect& rect)
{
	ASSERT_VALID (pMenuButton);

	CBCGPPopupMenu* pPopupMenu= pMenuButton->GetPopupMenu();
	if (pPopupMenu == NULL || pPopupMenu->GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectMenu;
	pPopupMenu->GetWindowRect (rectMenu);

	if (DYNAMIC_DOWNCAST (CCustomizeButton, pMenuButton) != NULL)
	{
		CBCGPBaseControlBar* pParentBar = DYNAMIC_DOWNCAST (
			CBCGPBaseControlBar, pMenuButton->GetParentWnd());

		if (pParentBar != NULL)
		{
			CRect rectScreen = rect;
			pParentBar->ClientToScreen (&rectScreen);

			if (pParentBar->IsHorizontal())
			{
				rectScreen.t = rectMenu.t;
				rectScreen.b = rectMenu.b;
			}
			else
			{
				rectScreen.l = rectMenu.l;
				rectScreen.r = rectMenu.r;
				rectScreen.b++;
			}

			CRect rectInter;
			if (!rectInter.IntersectRect (rectScreen, rectMenu))
			{
				return;
			}
		}
	}

	int nGrow = 4;

	switch (pPopupMenu->GetDropDirection())
	{
	case CBCGPPopupMenu::DROP_DIRECTION_BOTTOM:
		if (rectMenu.Width() < rect.Width())
		{
			nGrow = 1;
		}

		rect.b += nGrow;
		break;

	case CBCGPPopupMenu::DROP_DIRECTION_TOP:
		if (rectMenu.Width() < rect.Width())
		{
			nGrow = 1;
		}

		rect.t -= nGrow;
		break;

	case CBCGPPopupMenu::DROP_DIRECTION_RIGHT:
		if (rectMenu.Height() < rect.Height())
		{
			nGrow = 1;
		}

		rect.r += nGrow;
		break;

	case CBCGPPopupMenu::DROP_DIRECTION_LEFT:
		if (rectMenu.Height() < rect.Height())
		{
			nGrow = 1;
		}

		rect.l -= nGrow;
		break;
	}
}
//****
void CBCGPVisualManagerXP::OnDrawMenuSystemButton (CDC* pDC, CRect rect, UINT uiSystemCommand, 
										UINT nStyle, BOOL bHighlight)
{
	ASSERT_VALID (pDC);

	BOOL bIsDisabled = (nStyle & TBBS_DISABLED);
	BOOL bIsPressed = (nStyle & TBBS_PRESSED);

	CBCGPMenuImages::IMAGES_IDS imageID;

	switch (uiSystemCommand)
	{
	case SC_CLOSE:
		imageID = CBCGPMenuImages::IdClose;
		break;

	case SC_MINIMIZE:
		imageID = CBCGPMenuImages::IdMinimize;
		break;

	case SC_RESTORE:
		imageID = CBCGPMenuImages::IdRestore;
		break;

	default:
		return;
	}

	if (bHighlight && !bIsDisabled)
	{
		OnFillHighlightedArea (pDC, rect, 
			bIsPressed ? &m_brHighlightDn : &m_brHighlight, NULL);

		COLORREF clrBorder = m_clrMenuItemBorder;
		pDC->Draw3dRect (rect, clrBorder, clrBorder);
	}

	CBCGPMenuImages::Draw (pDC, imageID, rect,
		bIsDisabled ? CBCGPMenuImages::ImageGray : bHighlight ? 
			CBCGPMenuImages::ImageWhite : CBCGPMenuImages::ImageBlack);
}
//****
void CBCGPVisualManagerXP::OnDrawStatusBarPaneBorder (CDC* pDC, CBCGPStatusBar* /*pBar*/,
					CRect rectPane, UINT /*uiID*/, UINT nStyle)
{
	if (!(nStyle & SBPS_NOBORDERS))
	{
		if (nStyle & SBPS_POPOUT)
		{
			CBCGPDrawManager dm (*pDC);
			dm.HighlightRect (rectPane);
		}

		// Draw pane border:
		pDC->Draw3dRect (rectPane, m_clrPaneBorder, m_clrPaneBorder);
	}
}
//****
void CBCGPVisualManagerXP::OnDrawComboDropButton (CDC* pDC, CRect rect,
												BOOL bDisabled,
												BOOL bIsDropped,
												BOOL bIsHighlighted,
												CBCGPToolbarComboBoxButton* /*pButton*/)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID (this);

	COLORREF clrText = pDC->GetTextColor();

	if (bIsDropped || bIsHighlighted)
	{
		OnFillHighlightedArea (pDC, rect, 
			bIsDropped ? &m_brHighlightDn : &m_brHighlight,
			NULL);

		if (CBCGPToolBarImages::m_bIsDrawOnGlass)
		{
			CBCGPDrawManager dm (*pDC);
			dm.DrawLine (rect.l, rect.t, rect.l, rect.b, m_clrMenuItemBorder);
		}
		else
		{
			CPen* pOldPen = pDC->SelectObject (&m_penMenuItemBorder);
			ASSERT (pOldPen != NULL);

			pDC->MoveTo (rect.l, rect.t);
			pDC->LineTo (rect.l, rect.b);

			pDC->SelectObject (pOldPen);
		}
	}
	else
	{
		pDC->FillRect (rect, &globalData.brBarFace);

		if (CBCGPToolBarImages::m_bIsDrawOnGlass)
		{
			CBCGPDrawManager dm (*pDC);
			dm.DrawRect (rect, (COLORREF)-1, globalData.clrWindow);
		}
		else
		{
			pDC->Draw3dRect (rect, globalData.clrBarWindow, globalData.clrBarWindow);
		}
	}

	CBCGPMenuImages::Draw (pDC, CBCGPMenuImages::IdArowDown, rect,
		bDisabled ? CBCGPMenuImages::ImageGray : (bIsDropped && bIsHighlighted) ? 
			CBCGPMenuImages::ImageWhite : CBCGPMenuImages::ImageBlack);

	pDC->SetTextColor (clrText);
}
//****
void CBCGPVisualManagerXP::OnDrawComboBorder (CDC* pDC, CRect rect,
												BOOL /*bDisabled*/,
												BOOL bIsDropped,
												BOOL bIsHighlighted,
												CBCGPToolbarComboBoxButton* /*pButton*/)
{
	if (bIsHighlighted || bIsDropped)
	{
		rect.DeflateRect (1, 1);

		if (CBCGPToolBarImages::m_bIsDrawOnGlass)
		{
			CBCGPDrawManager dm (*pDC);
			dm.DrawRect (rect, (COLORREF)-1, m_clrMenuItemBorder);
		}
		else
		{
			pDC->Draw3dRect (&rect,  m_clrMenuItemBorder, m_clrMenuItemBorder);
		}
	}
}
//****
void CBCGPVisualManagerXP::OnDrawTabCloseButton (CDC* pDC, CRect rect, 
											   const CBCGPBaseTabWnd* /*pTabWnd*/,
											   BOOL bIsHighlighted,
											   BOOL bIsPressed,
											   BOOL /*bIsDisabled*/)
{
	if (bIsHighlighted)
	{
		OnFillHighlightedArea (pDC, rect, bIsPressed ? &m_brHighlightDn : &m_brHighlight, NULL);
	}

	CBCGPMenuImages::Draw (pDC, CBCGPMenuImages::IdClose, rect, CBCGPMenuImages::ImageBlack);

	if (bIsHighlighted)
	{
		pDC->Draw3dRect (rect, m_clrMenuItemBorder, m_clrMenuItemBorder);
	}
}
//****
void CBCGPVisualManagerXP::OnEraseTabsButton (CDC* pDC, CRect rect,
											  CBCGPButton* pButton,
											  CBCGPBaseTabWnd* pWndTab)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);
	ASSERT_VALID (pWndTab);

	if (pWndTab->IsFlatTab())
	{
		CBrush* pBrush = pButton->IsPressed() ? 
			&m_brHighlightDn : pButton->IsHighlighted() ? &m_brHighlight : &globalData.brBarFace;

		pDC->FillRect (rect, pBrush);
		OnFillHighlightedArea (pDC, rect, pBrush, NULL);
	}
	else if (pWndTab->IsDialogControl())
	{
		if (pWndTab->IsVisualManagerStyle())
		{
			OnFillDialog (pDC, pWndTab->GetParent(), rect);
		}
		else
		{
			pDC->FillRect (rect, &globalData.brBtnFace);
		}
	}
	else
	{
		pDC->FillRect (rect, &m_brTabBack);
	}
}
//****
void CBCGPVisualManagerXP::OnDrawTabsButtonBorder (CDC* pDC, CRect& rect, 
												 CBCGPButton* pButton, UINT /*uiState*/,
												 CBCGPBaseTabWnd* pWndTab)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);
	ASSERT_VALID (pWndTab);

	if (pWndTab->IsFlatTab())
	{
		if (pButton->IsPushed() || pButton->IsHighlighted())
		{
			COLORREF clrDark = globalData.clrBarDkShadow;
			pDC->Draw3dRect (rect, clrDark, clrDark);
		}
	}
	else
	{
		if (pButton->IsPushed() || pButton->IsHighlighted())
		{
			if (pButton->IsPressed())
			{
				pDC->Draw3dRect (rect, globalData.clrBarDkShadow, m_clrGripper);
			}
			else
			{
				pDC->Draw3dRect (rect, m_clrGripper, globalData.clrBarDkShadow);
			}
		}
	}
}
//****
COLORREF CBCGPVisualManagerXP::OnFillMiniFrameCaption (CDC* pDC, 
								CRect rectCaption, 
								CBCGPMiniFrameWnd* pFrameWnd,
								BOOL bActive)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pFrameWnd);

	BOOL bIsToolBar = FALSE;
#ifndef BCGP_EXCLUDE_TASK_PANE
	BOOL bIsTasksPane = pFrameWnd->IsKindOf( RUNTIME_CLASS( CBCGPTaskPaneMiniFrameWnd ) );
#else
	BOOL bIsTasksPane = FALSE;
#endif

	if (DYNAMIC_DOWNCAST (CBCGPBaseToolBar, pFrameWnd->GetControlBar()) != NULL)
	{
		bActive = FALSE;
		bIsToolBar = TRUE;
	}

	if (bIsToolBar)
	{
		pDC->FillRect (rectCaption, &m_brFloatToolBarBorder);
		return globalData.clrCaptionText;
	}
	else if (bIsTasksPane)
	{
		pDC->FillRect (rectCaption, &globalData.brBarFace);
		return globalData.clrBarText;
	}

	pDC->FillRect (rectCaption, 
		bActive ? &globalData.brActiveCaption : &globalData.brInactiveCaption);

    // get the text color
	return globalData.clrCaptionText;
}
//****
void CBCGPVisualManagerXP::OnDrawMiniFrameBorder (
										CDC* pDC, CBCGPMiniFrameWnd* pFrameWnd,
										CRect rectBorder, CRect rectBorderSize)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pFrameWnd);

#ifndef BCGP_EXCLUDE_TASK_PANE
	BOOL bIsTasksPane = pFrameWnd->IsKindOf( RUNTIME_CLASS( CBCGPTaskPaneMiniFrameWnd ) );
#else
	BOOL bIsTasksPane = FALSE;
#endif

	if (bIsTasksPane)
	{
		CBrush* pOldBrush = pDC->SelectObject (&m_brFloatToolBarBorder);
		ASSERT (pOldBrush != NULL);

		pDC->PatBlt (rectBorder.l, rectBorder.t, rectBorderSize.l, rectBorder.Height(), PATCOPY);
		pDC->PatBlt (rectBorder.l, rectBorder.t, rectBorder.Width(), rectBorderSize.t, PATCOPY);
		pDC->PatBlt (rectBorder.r - rectBorderSize.r, rectBorder.t, rectBorderSize.r, rectBorder.Height(), PATCOPY);
		pDC->PatBlt (rectBorder.l, rectBorder.b - rectBorderSize.b, rectBorder.Width(), rectBorderSize.b, PATCOPY);

		rectBorderSize.DeflateRect (2, 2);
		rectBorder.DeflateRect (2, 2);

		pDC->SelectObject (bIsTasksPane ? &globalData.brLight : &globalData.brBarFace);

		pDC->PatBlt (rectBorder.l, rectBorder.t + 1, rectBorderSize.l, rectBorder.Height() - 2, PATCOPY);
		pDC->PatBlt (rectBorder.l + 1, rectBorder.t, rectBorder.Width() - 2, rectBorderSize.t, PATCOPY);
		pDC->PatBlt (rectBorder.r - rectBorderSize.r, rectBorder.t + 1, rectBorderSize.r, rectBorder.Height() - 2, PATCOPY);
		pDC->PatBlt (rectBorder.l + 1, rectBorder.b - rectBorderSize.b, rectBorder.Width() - 2, rectBorderSize.b, PATCOPY);

		pDC->SelectObject (pOldBrush);
	}
	else
	{
		CBCGPVisualManager::OnDrawMiniFrameBorder (pDC, pFrameWnd, rectBorder, rectBorderSize);
	}
}
//****
void CBCGPVisualManagerXP::OnDrawFloatingToolbarBorder (
												CDC* pDC, CBCGPBaseToolBar* /*pToolBar*/, 
												CRect rectBorder, CRect rectBorderSize)
{
	ASSERT_VALID (pDC);

	CBrush* pOldBrush = pDC->SelectObject (&m_brFloatToolBarBorder);
	ASSERT (pOldBrush != NULL);

	pDC->PatBlt (rectBorder.l, rectBorder.t, rectBorderSize.l, rectBorder.Height(), PATCOPY);
	pDC->PatBlt (rectBorder.l, rectBorder.t, rectBorder.Width(), rectBorderSize.t, PATCOPY);
	pDC->PatBlt (rectBorder.r - rectBorderSize.r, rectBorder.t, rectBorderSize.r, rectBorder.Height(), PATCOPY);
	pDC->PatBlt (rectBorder.l, rectBorder.b - rectBorderSize.b, rectBorder.Width(), rectBorderSize.b, PATCOPY);

	rectBorderSize.DeflateRect (2, 2);
	rectBorder.DeflateRect (2, 2);

	pDC->SelectObject (&globalData.brBarFace);

	pDC->PatBlt (rectBorder.l, rectBorder.t + 1, rectBorderSize.l, rectBorder.Height() - 2, PATCOPY);
	pDC->PatBlt (rectBorder.l + 1, rectBorder.t, rectBorder.Width() - 2, rectBorderSize.t, PATCOPY);
	pDC->PatBlt (rectBorder.r - rectBorderSize.r, rectBorder.t + 1, rectBorderSize.r, rectBorder.Height() - 2, PATCOPY);
	pDC->PatBlt (rectBorder.l + 1, rectBorder.b - rectBorderSize.b, rectBorder.Width() - 2, rectBorderSize.b, PATCOPY);

	pDC->SelectObject (pOldBrush);
}
//****
COLORREF CBCGPVisualManagerXP::GetToolbarButtonTextColor (CBCGPToolbarButton* pButton, 
														CBCGPVisualManager::BCGBUTTON_STATE state)
{
	ASSERT_VALID (pButton);

	if (!globalData.IsHighContastMode())
	{
		BOOL bDisabled = (CBCGPToolBar::IsCustomizeMode() && !pButton->IsEditable()) ||
			(!CBCGPToolBar::IsCustomizeMode() && (pButton->m_nStyle & TBBS_DISABLED));

		if (pButton->IsKindOf (RUNTIME_CLASS (CBCGPOutlookButton)))
		{
			if (bDisabled)
			{
				return globalData.clrGrayedText;
			}

			return globalData.IsHighContastMode() ? 
				globalData.clrWindowText : globalData.clrBarText;
		}

		if (state == ButtonsIsHighlighted && 
			(pButton->m_nStyle & (TBBS_PRESSED | TBBS_CHECKED)))
		{
			return globalData.clrTextHilite;
		}
	}

	return	CBCGPVisualManager::GetToolbarButtonTextColor (pButton, state);
}
//****
BOOL CBCGPVisualManagerXP::IsToolBarButtonDefaultBackground (CBCGPToolbarButton* pButton,
												CBCGPVisualManager::BCGBUTTON_STATE state)
{
	ASSERT_VALID (pButton);

	BOOL bIsChecked = pButton->m_nStyle & (TBBS_CHECKED | TBBS_INDETERMINATE);

	return !bIsChecked && state != ButtonsIsHighlighted && state != ButtonsIsPressed;
}
//****
void CBCGPVisualManagerXP::OnDrawEditBorder (CDC* pDC, CRect rect,
												BOOL bDisabled,
												BOOL bIsHighlighted,
												CBCGPToolbarEditBoxButton* pButton)
{
	if (!CBCGPToolbarEditBoxButton::IsFlatMode())
	{
		CBCGPVisualManager::OnDrawEditBorder (pDC, rect, bDisabled, bIsHighlighted, pButton);
		return;
	}

	if (bIsHighlighted)
	{
		pDC->Draw3dRect (&rect,  m_clrMenuItemBorder, m_clrMenuItemBorder);
	}
}

#ifndef BCGP_EXCLUDE_TASK_PANE

void CBCGPVisualManagerXP::OnDrawTasksGroupCaption(CDC* pDC, CBCGPTasksGroup* pGroup, 
						BOOL bIsHighlighted, BOOL bIsSelected, BOOL bCanCollapse)
{
	ASSERT_VALID(pDC);
	ASSERT(pGroup != NULL);
	ASSERT_VALID (pGroup->m_pPage);

#ifndef BCGP_EXCLUDE_TOOLBOX
	BOOL bIsToolBox = pGroup->m_pPage->m_pTaskPane != NULL &&
		(pGroup->m_pPage->m_pTaskPane->IsKindOf (RUNTIME_CLASS (CBCGPToolBoxEx)));
#else
	BOOL bIsToolBox = FALSE;
#endif

	CRect rectGroup = pGroup->m_rect;

	if (bIsToolBox)
	{
		CRect rectFill = rectGroup;
		rectFill.DeflateRect (1, 0, 1, 1);

		CBrush brFill (globalData.IsHighContastMode() ?
			globalData.clrWindow : globalData.clrBarShadow);

		pDC->FillRect (rectFill, &brFill);

		if (bCanCollapse)
		{
			//--------------------
			// Draw expanding box:
			//--------------------
			int nBoxSize = 9;
			int nBoxOffset = 6;

			if (globalData.GetRibbonImageScale() != 1.)
			{
				nBoxSize = (int)(.5 + nBoxSize * globalData.GetRibbonImageScale());
			}

			CRect rectButton = rectFill;
			
			rectButton.l += nBoxOffset;
			rectButton.r = rectButton.l + nBoxSize;
			rectButton.t = rectButton.CenterPoint().y - nBoxSize / 2;
			rectButton.b = rectButton.t + nBoxSize;

			pDC->FillRect (rectButton, &globalData.brBarFace);

			OnDrawExpandingBox (pDC, rectButton, !pGroup->m_bIsCollapsed, 
				globalData.clrBarText);

			rectGroup.l = rectButton.r + nBoxOffset;
			bCanCollapse = FALSE;
		}
	}

	// ------------------------------
	// Draw group caption (Office XP)
	// ------------------------------
	
	// ---------------------------
	// Draw an icon if it presents
	// ---------------------------
	BOOL bShowIcon = (pGroup->m_hIcon != NULL 
		&& pGroup->m_sizeIcon.w < rectGroup.Width() - rectGroup.Height());
	if (bShowIcon)
	{
		OnDrawTasksGroupIcon(pDC, pGroup, 5, bIsHighlighted, bIsSelected, bCanCollapse);
	}
	
	// -----------------------
	// Draw group caption text
	// -----------------------
	CFont* pFontOld = pDC->SelectObject(globalData.fontBold);
	COLORREF clrTextOld = pDC->GetTextColor();

	if (bIsToolBox && !globalData.IsHighContastMode())
	{
		pDC->SetTextColor (globalData.clrWindow);
	}
	else
	{
		if (bCanCollapse && bIsHighlighted)
		{
			clrTextOld = pDC->SetTextColor (pGroup->m_clrTextHot == (COLORREF)-1 ?
				globalData.clrWindowText : pGroup->m_clrTextHot);
		}
		else
		{
			clrTextOld = pDC->SetTextColor (pGroup->m_clrText == (COLORREF)-1 ?
				globalData.clrWindowText : pGroup->m_clrText);
		}
	}

	int nBkModeOld = pDC->SetBkMode(TRANSPARENT);
	
	int nTaskPaneHOffset = pGroup->m_pPage->m_pTaskPane->GetGroupCaptionHorzOffset();
	int nTaskPaneVOffset = pGroup->m_pPage->m_pTaskPane->GetGroupCaptionVertOffset();
	int nCaptionHOffset = (nTaskPaneHOffset != -1 ? nTaskPaneHOffset : m_nGroupCaptionHorzOffset);
	
	CRect rectText = rectGroup;
	rectText.l += (bShowIcon ? pGroup->m_sizeIcon.w	+ 5: nCaptionHOffset);
	rectText.t += (nTaskPaneVOffset != -1 ? nTaskPaneVOffset : m_nGroupCaptionVertOffset);
	rectText.r = max(rectText.l, 
						rectText.r - (bCanCollapse ? rectGroup.Height() : nCaptionHOffset));

	pDC->DrawText (pGroup->m_strName, rectText, DT_SINGLELINE | DT_VCENTER);

	pDC->SetBkMode(nBkModeOld);
	pDC->SelectObject (pFontOld);
	pDC->SetTextColor (clrTextOld);

	// -------------------------
	// Draw group caption button
	// -------------------------
	if (bCanCollapse && !pGroup->m_strName.IsEmpty())
	{
		CSize sizeButton = CBCGPMenuImages::Size();
		CRect rectButton = rectGroup;
		rectButton.l = max(rectButton.l, rectButton.r - sizeButton.w);
		rectButton.t = max(rectButton.t, rectButton.b - sizeButton.h);
		
		if (rectButton.Width() >= sizeButton.w && rectButton.Height() >= sizeButton.h)
		{
			if (bIsHighlighted)
			{
				// Draw button frame
				CPen* pPenOld = (CPen*) pDC->SelectObject (&globalData.penHilite);
				CBrush* pBrushOld = (CBrush*) pDC->SelectObject (&m_brHighlight);
				COLORREF clrBckOld = pDC->GetBkColor();

				pDC->Rectangle(&rectButton);

				pDC->SetBkColor (clrBckOld);
				pDC->SelectObject (pPenOld);
				pDC->SelectObject (pBrushOld);
			}
			
			if (!pGroup->m_bIsCollapsed)
			{
				CBCGPMenuImages::Draw(pDC, CBCGPMenuImages::IdArowUp, rectButton.TopLeft());
			}
			else
			{
				CBCGPMenuImages::Draw(pDC, CBCGPMenuImages::IdArowDown, rectButton.TopLeft());
			}
		}
	}
}
//****
void CBCGPVisualManagerXP::OnFillTasksGroupInterior(CDC* pDC, CRect rect, BOOL /*bSpecial*/)
{
	ASSERT_VALID(pDC);

	// Draw underline
	CPen* pPenOld = (CPen*) pDC->SelectObject (&globalData.penBarShadow);
	pDC->MoveTo (rect.l, rect.t);
	pDC->LineTo (rect.r, rect.t);
	pDC->SelectObject (pPenOld);

}
//****
void CBCGPVisualManagerXP::OnDrawTasksGroupAreaBorder(CDC* /*pDC*/, CRect /*rect*/, 
													 BOOL /*bSpecial*/, BOOL /*bNoTitle*/)
{
}
//****
void CBCGPVisualManagerXP::OnDrawTask(CDC* pDC, CBCGPTask* pTask, CImageList* pIcons, BOOL bIsHighlighted, BOOL /*bIsSelected*/)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pIcons);
	ASSERT(pTask != NULL);

	CRect rectText = pTask->m_rect;

	if (pTask->m_bIsSeparator)
	{
		CPen* pPenOld = (CPen*) pDC->SelectObject (&globalData.penBarShadow);

		pDC->MoveTo (rectText.l, rectText.CenterPoint().y);
		pDC->LineTo (rectText.r, rectText.CenterPoint().y);

		pDC->SelectObject (pPenOld);
		return;
	}

	// ---------
	// Draw icon
	// ---------
	CSize sizeIcon(0, 0);
	::ImageList_GetIconSize (pIcons->m_hImageList, (int*) &sizeIcon.w, (int*) &sizeIcon.h);
	if (pTask->m_nIcon >= 0 && sizeIcon.w > 0)
	{
		pIcons->Draw (pDC, pTask->m_nIcon, rectText.TopLeft(), ILD_TRANSPARENT);
	}
	int nTaskPaneOffset = pTask->m_pGroup->m_pPage->m_pTaskPane->GetTasksIconHorzOffset();
	rectText.l += sizeIcon.w + (nTaskPaneOffset != -1 ? nTaskPaneOffset : m_nTasksIconHorzOffset);

	// ---------
	// Draw text
	// ---------
	BOOL bIsLabel = (pTask->m_uiCommandID == 0);

	CFont* pFontOld = NULL;
	COLORREF clrTextOld = pDC->GetTextColor();
	if (bIsLabel)
	{
		pFontOld = pDC->SelectObject (
			pTask->m_bIsBold ? &globalData.fontBold : &globalData.fontRegular);
		pDC->SetTextColor (pTask->m_clrText == (COLORREF)-1 ?
			globalData.clrWindowText : pTask->m_clrText);
	}
	else if (!pTask->m_bEnabled)
	{
		pDC->SetTextColor (globalData.clrGrayedText);
		pFontOld = pDC->SelectObject(globalData.fontRegular);
	}
	else if (bIsHighlighted)
	{
		pDC->SetTextColor (pTask->m_clrTextHot == (COLORREF)-1 ?
			m_clrTaskPaneHotText : pTask->m_clrTextHot);
		pFontOld = pDC->SelectObject(globalData.fontUnderline);
	}
	else
	{
		pDC->SetTextColor (pTask->m_clrText == (COLORREF)-1 ?
			globalData.clrWindowText : pTask->m_clrText);
		pFontOld = pDC->SelectObject(globalData.fontRegular);
	}
	int nBkModeOld = pDC->SetBkMode(TRANSPARENT);

	CBCGPTasksPane* pTaskPane = pTask->m_pGroup->m_pPage->m_pTaskPane;
	ASSERT_VALID (pTaskPane);

	BOOL bMultiline = bIsLabel ? 
		pTaskPane->IsWrapLabelsEnabled() : pTaskPane->IsWrapTasksEnabled();

	if (bMultiline)
	{
		pDC->DrawText (pTask->m_strName, rectText, DT_WORDBREAK);
	}
	else
	{
		CString strText = pTask->m_strName;
		strText.Remove (_T('\n'));
		strText.Remove (_T('\r'));
		pDC->DrawText (strText, rectText, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
	}

	pDC->SetBkMode(nBkModeOld);
	pDC->SelectObject (pFontOld);
	pDC->SetTextColor (clrTextOld);
}
//****
void CBCGPVisualManagerXP::OnDrawScrollButtons(CDC* pDC, const CRect& rect, const int nBorderSize,
									int iImage, BOOL bHilited)
{
	ASSERT_VALID (pDC);

	CRect rectFill = rect;
	rectFill.t -= nBorderSize;

	pDC->FillRect (rectFill, &globalData.brWindow);

	if (bHilited)
	{
		pDC->FillRect (rect, &m_brHighlight);
		pDC->Draw3dRect (rect, m_clrMenuItemBorder, m_clrMenuItemBorder);
	}

	CBCGPMenuImages::Draw (pDC, (CBCGPMenuImages::IMAGES_IDS) iImage, rect);
}

#endif	// BCGP_EXCLUDE_TASK_PANE

void CBCGPVisualManagerXP::OnDrawSpinButtons (CDC* pDC, CRect rectSpin, 
	int nState, BOOL bOrientation, CBCGPSpinButtonCtrl* pSpinCtrl)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID (this);

	CRect rect [2];
	rect[0] = rect[1] = rectSpin;

	if (!bOrientation) 
	{
		rect[0].DeflateRect(0, 0, 0, rect[0].Height() / 2);
		rect[1].t = rect[0].b ;
	}
	else
	{
		rect[0].DeflateRect(0, 0, rect[0].Width() / 2, 0);
		rect[1].l = rect[0].r ;
	}

	CBCGPMenuImages::IMAGES_IDS id[2][2] = {{CBCGPMenuImages::IdArowUp, CBCGPMenuImages::IdArowDown}, {CBCGPMenuImages::IdArowLeft, CBCGPMenuImages::IdArowRight}};

	int idxPressed = (nState & (SPIN_PRESSEDUP | SPIN_PRESSEDDOWN)) - 1;
	
	int idxHighlighted = -1;
	if (nState & SPIN_HIGHLIGHTEDUP)
	{
		idxHighlighted = 0;
	}
	else if (nState & SPIN_HIGHLIGHTEDDOWN)
	{
		idxHighlighted = 1;
	}

	BOOL bDisabled = nState & SPIN_DISABLED;

	for (int i = 0; i < 2; i ++)
	{
		if (idxPressed == i || idxHighlighted == i)
		{
			OnFillHighlightedArea (pDC, rect [i], 
				(idxPressed == i) ? &m_brHighlightDn : &m_brHighlight, NULL);
		}
		else
		{
			OnFillSpinButton (pDC, pSpinCtrl, rect[i], bDisabled);
		}

		CBCGPMenuImages::Draw (pDC, id[bOrientation ? 1 : 0][i], rect[i],
			bDisabled ? CBCGPMenuImages::ImageGray : CBCGPMenuImages::ImageBlack);
	}

	if (idxHighlighted >= 0)
	{
		CRect rectHot = rect [idxHighlighted];

		if (CBCGPToolBarImages::m_bIsDrawOnGlass)
		{
			CBCGPDrawManager dm (*pDC);
			dm.DrawRect (rectHot, (COLORREF)-1, m_clrMenuItemBorder);
		}
		else
		{
			pDC->Draw3dRect (rectHot, m_clrMenuItemBorder, m_clrMenuItemBorder);
		}
	}
}
//****
void CBCGPVisualManagerXP::OnFillSpinButton (CDC* pDC, CBCGPSpinButtonCtrl* /*pSpinCtrl*/, CRect rect, BOOL bDisabled)
{
	if (CBCGPToolBarImages::m_bIsDrawOnGlass)
	{
		CBCGPDrawManager dm (*pDC);
		dm.DrawRect (rect, 
			bDisabled ? globalData.clrBtnFace : globalData.clrBarFace, 
			globalData.clrBarHilite);
	}
	else
	{
		pDC->FillRect (rect, bDisabled ? &globalData.brBtnFace : &globalData.brBarFace);
		pDC->Draw3dRect (rect, globalData.clrBarHilite, globalData.clrBarHilite);
	}
}
//****
void CBCGPVisualManagerXP::OnFillHighlightedArea (CDC* pDC, CRect rect, 
		CBrush* pBrush, CBCGPToolbarButton* /*pButton*/)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pBrush);

	if (CBCGPToolBarImages::m_bIsDrawOnGlass)
	{
		LOGBRUSH br; 
        pBrush->GetLogBrush(&br);

		CBCGPDrawManager dm (*pDC);
		dm.DrawRect (rect, br.lbColor, (COLORREF)-1);
	}
	else
	{
		pDC->FillRect (rect, pBrush);
	}
}
//****
void CBCGPVisualManagerXP::OnDrawSplitterBorder (CDC* pDC, CBCGPSplitterWnd* /*pSplitterWnd*/, CRect rect)
{
	ASSERT_VALID(pDC);

	pDC->Draw3dRect (rect, globalData.clrBarShadow, globalData.clrBarShadow);
	rect.InflateRect(-CX_BORDER, -CY_BORDER);
	pDC->Draw3dRect (rect, globalData.clrBarFace, globalData.clrBarFace);
}
//****
void CBCGPVisualManagerXP::OnDrawSplitterBox (CDC* pDC, CBCGPSplitterWnd* /*pSplitterWnd*/, CRect& rect)
{
	ASSERT_VALID(pDC);
	pDC->Draw3dRect(rect, globalData.clrBarFace, globalData.clrBarFace);
}
//****
BOOL CBCGPVisualManagerXP::OnDrawCalculatorButton (CDC* pDC, 
	CRect rect, CBCGPToolbarButton* /*pButton*/, 
	CBCGPVisualManager::BCGBUTTON_STATE state, 
	int /*cmd*/ /* CBCGPCalculator::CalculatorCommands */,
	CBCGPCalculator* /*pCalculator*/)
{
	ASSERT_VALID (pDC);

	switch (state)
	{
	case ButtonsIsPressed:
		pDC->FillRect (rect, &m_brHighlightDn);
		pDC->SetTextColor (globalData.clrWindow);
		break;

	case ButtonsIsHighlighted:
		pDC->FillRect (rect, &m_brHighlight);
		break;
	}

	pDC->Draw3dRect (&rect, m_clrMenuItemBorder, m_clrMenuItemBorder);
	return TRUE;
}
//****
BOOL CBCGPVisualManagerXP::OnDrawCalculatorDisplay (CDC* pDC, CRect rect, 
												  const CString& /*strText*/, BOOL /*bMem*/,
												  CBCGPCalculator* /*pCalculator*/)
{
	pDC->FillRect (rect, &globalData.brWindow);
	pDC->Draw3dRect (&rect, m_clrMenuItemBorder, m_clrMenuItemBorder);

	return TRUE;
}
//****
BOOL CBCGPVisualManagerXP::OnDrawBrowseButton (CDC* pDC, CRect rect, 
	CBCGPEdit* /*pEdit*/, CBCGPVisualManager::BCGBUTTON_STATE state, COLORREF& /*clrText*/)
{
	ASSERT_VALID (pDC);

	CRect rectFrame = rect;
	rectFrame.InflateRect (0, 1, 1, 1);

	switch (state)
	{
	case ButtonsIsPressed:
		pDC->FillRect (rect, &m_brHighlightDn);
		pDC->Draw3dRect (&rectFrame, m_clrMenuItemBorder, m_clrMenuItemBorder);
		pDC->SetTextColor (globalData.clrWindow);
		break;

	case ButtonsIsHighlighted:
		pDC->FillRect (rect, &m_brHighlight);
		pDC->Draw3dRect (&rectFrame, m_clrMenuItemBorder, m_clrMenuItemBorder);
		break;

	default:
		pDC->FillRect (rect, &globalData.brBtnFace);
		pDC->Draw3dRect (rect, globalData.clrBarHilite, globalData.clrBarHilite);
		break;
	}

	return TRUE;
}
//****
COLORREF CBCGPVisualManagerXP::GetWindowColor() const
{
	return globalData.clrWindow;
}
//****
COLORREF CBCGPVisualManagerXP::GetAutoHideButtonTextColor (CBCGPAutoHideButton* /*pButton*/)
{
	return globalData.clrBtnDkShadow;
}
//****
void CBCGPVisualManagerXP::OnDrawAppBarBorder (
								CDC* pDC, CBCGPAppBarWnd* /*pAppBarWnd*/,
								CRect rectBorder, CRect rectBorderSize)
{
	ASSERT_VALID (pDC);

	CBrush* pOldBrush = pDC->SelectObject (&m_brFloatToolBarBorder);
	ASSERT (pOldBrush != NULL);

	pDC->PatBlt (rectBorder.l, rectBorder.t, rectBorderSize.l, rectBorder.Height(), PATCOPY);
	pDC->PatBlt (rectBorder.l, rectBorder.t, rectBorder.Width(), rectBorderSize.t, PATCOPY);
	pDC->PatBlt (rectBorder.r - rectBorderSize.r, rectBorder.t, rectBorderSize.r, rectBorder.Height(), PATCOPY);
	pDC->PatBlt (rectBorder.l, rectBorder.b - rectBorderSize.b, rectBorder.Width(), rectBorderSize.b, PATCOPY);

	rectBorderSize.DeflateRect (2, 2);
	rectBorder.DeflateRect (2, 2);

	pDC->SelectObject (&globalData.brLight);

	pDC->PatBlt (rectBorder.l, rectBorder.t + 1, rectBorderSize.l, rectBorder.Height() - 2, PATCOPY);
	pDC->PatBlt (rectBorder.l + 1, rectBorder.t, rectBorder.Width() - 2, rectBorderSize.t, PATCOPY);
	pDC->PatBlt (rectBorder.r - rectBorderSize.r, rectBorder.t + 1, rectBorderSize.r, rectBorder.Height() - 2, PATCOPY);
	pDC->PatBlt (rectBorder.l + 1, rectBorder.b - rectBorderSize.b, rectBorder.Width() - 2, rectBorderSize.b, PATCOPY);

	pDC->SelectObject (pOldBrush);
}
//****
void CBCGPVisualManagerXP::GetSmartDockingBaseMarkerColors (
		COLORREF& clrBaseGroupBackground,
		COLORREF& clrBaseGroupBorder)
{
	clrBaseGroupBackground = m_clrBarBkgnd;
	clrBaseGroupBorder = m_clrMenuBorder;
}
//****
void CBCGPVisualManagerXP::OnDrawButtonSeparator (CDC* pDC,
		CBCGPToolbarButton* /*pButton*/, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE /*state*/,
		BOOL bHorz)
{
	CPen* pOldPen = pDC->SelectObject (&m_penMenuItemBorder);
	ASSERT (pOldPen != NULL);

	if (bHorz)
	{
		pDC->MoveTo (rect.l, rect.t);
		pDC->LineTo (rect.l, rect.b);
	}
	else
	{
		pDC->MoveTo (rect.l, rect.t);
		pDC->LineTo (rect.r, rect.t);
	}

	pDC->SelectObject (pOldPen);
}

#ifndef BCGP_EXCLUDE_POPUP_WINDOW

void CBCGPVisualManagerXP::OnDrawPopupWindowBorder (CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);

	pDC->Draw3dRect (rect, m_clrMenuBorder, m_clrMenuBorder);
	rect.DeflateRect (1, 1);
	pDC->Draw3dRect (rect, m_clrMenuLight, m_clrMenuLight);
}
//****
COLORREF  CBCGPVisualManagerXP::OnDrawPopupWindowCaption (CDC* pDC, CRect rectCaption, CBCGPPopupWindow* /*pPopupWnd*/)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rectCaption, &m_brHighlight);

    // get the text color
	return globalData.clrBarText;
}
//****
void CBCGPVisualManagerXP::OnErasePopupWindowButton (CDC* pDC, CRect rc, CBCGPPopupWndButton* pButton)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (pButton->IsPressed())
	{
		CBrush br (m_clrHighlightDn);
		pDC->FillRect (&rc, &br);
		return;
	}
	else if (pButton->IsHighlighted() || pButton->IsPushed())
	{
		CBrush br (m_clrHighlight);
		pDC->FillRect (&rc, &br);
		return;
	}

	CRect rectParent;
	pButton->GetParent()->GetClientRect (rectParent);

	pButton->GetParent()->MapWindowPoints (pButton, rectParent);
	OnFillPopupWindowBackground (pDC, rectParent);
}
//****
void CBCGPVisualManagerXP::OnDrawPopupWindowButtonBorder (CDC* pDC, CRect rc, CBCGPPopupWndButton* pButton)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (pButton->IsHighlighted() || pButton->IsPushed() ||
		pButton->IsCaptionButton())
	{
		pDC->Draw3dRect (rc, m_clrMenuItemBorder, m_clrMenuItemBorder);
	}
}
//****
void CBCGPVisualManagerXP::OnFillPopupWindowBackground (CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);
	pDC->FillRect (rect, &m_brMenuLight);
}

#endif // BCGP_EXCLUDE_POPUP_WINDOW

#ifndef BCGP_EXCLUDE_PLANNER

COLORREF CBCGPVisualManagerXP::OnFillPlannerCaption (CDC* pDC, CBCGPPlannerView* pView,
		CRect rect, BOOL bIsToday, BOOL bIsSelected, BOOL bNoBorder/* = FALSE*/, BOOL bHorz/* = TRUE*/)
{
	ASSERT_VALID (pDC);

	if (bIsToday)
	{
		rect.DeflateRect (1, 1);

		pDC->FillRect (rect, &m_brHighlight);

		CPen pen (PS_SOLID, 1, m_clrMenuItemBorder);
		CPen* pOldPen = pDC->SelectObject (&pen);

		pDC->MoveTo (rect.l, rect.b);
		pDC->LineTo (rect.r, rect.b);

		pDC->SelectObject (pOldPen);

		return globalData.clrBtnText;
	}

	return CBCGPVisualManager::OnFillPlannerCaption (pDC, pView, rect, bIsToday, 
		bIsSelected, bNoBorder, bHorz);
}
//****
void CBCGPVisualManagerXP::OnDrawPlannerTimeLine (CDC* pDC, CBCGPPlannerView* /*pView*/, CRect rect)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &m_brHighlight);

	CPen* pOldPen = pDC->SelectObject (&m_penSeparator);

	pDC->MoveTo (rect.l, rect.b);
	pDC->LineTo (rect.r, rect.b);

	pDC->SelectObject (pOldPen);
}

#endif // BCGP_EXCLUDE_PLANNER

//****
void CBCGPVisualManagerXP::GetCalendarColors (const CBCGPCalendar* pCalendar,
				   CBCGPCalendarColors& colors)
{
	CBCGPVisualManager::GetCalendarColors (pCalendar, colors);
	colors.clrSelected = m_clrHighlight;
}

#ifndef BCGP_EXCLUDE_PROP_LIST

COLORREF CBCGPVisualManagerXP::GetPropListGroupColor (CBCGPPropList* pPropList)
{
	ASSERT_VALID (pPropList);

	if (globalData.m_nBitsPerPixel <= 8)
	{
		return CBCGPVisualManager::GetPropListGroupColor (pPropList);
	}

	return CBCGPDrawManager::PixelAlpha (
			pPropList->DrawControlBarColors() ? 
			globalData.clrBarFace : globalData.clrBtnFace, 94);
}

COLORREF CBCGPVisualManagerXP::GetPropListGroupTextColor (CBCGPPropList* pPropList)
{
	ASSERT_VALID (pPropList);

	return pPropList->DrawControlBarColors() ?
		globalData.clrBarShadow : globalData.clrBtnShadow;
}

#endif // BCGP_EXCLUDE_PROP_LIST

#ifndef BCGP_EXCLUDE_RIBBON

COLORREF CBCGPVisualManagerXP::OnFillRibbonButton (
					CDC* pDC, 
					CBCGPRibbonButton* pButton)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (pButton->IsDefaultPanelButton() && !pButton->IsQATMode())
	{
		return (COLORREF)-1;
	}

	CRect rect = pButton->GetRect();

	const BOOL bIsMenuMode = pButton->IsMenuMode() && !pButton->IsPaletteIcon();

	const BOOL bIsHighlighted = 
		((pButton->IsHighlighted() || pButton->IsDroppedDown()) && 
		!pButton->IsDisabled()) || pButton->IsFocused();

	if (pButton->IsKindOf (RUNTIME_CLASS (CBCGPRibbonEdit)))
	{
		COLORREF clrBorder = globalData.clrBarShadow;
		CRect rectCommand = pButton->GetCommandRect();

		if (pButton->GetLocationInGroup() != CBCGPBaseRibbonElement::RibbonElementNotInGroup)
		{
			rectCommand.r++;
		}

		if (CBCGPToolBarImages::m_bIsDrawOnGlass)
		{
			CBCGPDrawManager dm (*pDC);

			if (bIsHighlighted)
			{
				dm.DrawRect (rect, globalData.clrWindow, clrBorder);
			}
			else
			{
				dm.DrawRect (rect, globalData.clrBarFace, clrBorder);
			}
		}
		else
		{
			if (bIsHighlighted)
			{
				pDC->FillRect (rectCommand, &globalData.brWindow);
			}
			else 
			{
				pDC->FillRect (rectCommand, &globalData.brBarFace);
				
				CBCGPDrawManager dm (*pDC);
				dm.HighlightRect (rectCommand);
			}

			pDC->Draw3dRect (rect, clrBorder, clrBorder);
		}

		return (COLORREF)-1;
	}

	if (!pButton->IsChecked() && !bIsHighlighted)
	{
		return (COLORREF)-1;
	}

	if (pButton->IsChecked() && bIsMenuMode && !bIsHighlighted)
	{
		return (COLORREF)-1;
	}

	CRect rectMenu = pButton->GetMenuRect();
	
	if (pButton->GetLocationInGroup() != CBCGPBaseRibbonElement::RibbonElementNotInGroup)
	{
		rect.DeflateRect (1, 1);
	}

	CRect rectCommand (0, 0, 0, 0);
	if (!rectMenu.IsRectEmpty())
	{
		rectCommand = pButton->GetCommandRect();

		if (pButton->GetLocationInGroup() != CBCGPBaseRibbonElement::RibbonElementNotInGroup)
		{
			rectMenu.DeflateRect (0, 1, 1, 1);
			rectCommand.DeflateRect (1, 1, 0, 1);
		}
	}

	if (!rectMenu.IsRectEmpty() && bIsHighlighted)
	{
		if (pButton->IsCommandAreaHighlighted())
		{
			OnFillHighlightedArea (pDC, rectCommand, 
				(pButton->IsPressed() || pButton->IsDroppedDown()) && !bIsMenuMode ? 
				&m_brHighlightDn : &m_brHighlight, NULL);
		}
		else
		{
			OnFillHighlightedArea (pDC, rectCommand, 
				&m_brHighlight, NULL);

			CBCGPDrawManager dm (*pDC);
			dm.HighlightRect (rectCommand);
		}

		if (pButton->IsMenuAreaHighlighted())
		{
			OnFillHighlightedArea (pDC, rectMenu, 
				(pButton->IsPressed() || pButton->IsDroppedDown()) && !bIsMenuMode ? 
				&m_brHighlightDn : &m_brHighlight, NULL);
		}
		else
		{
			OnFillHighlightedArea (pDC, rectMenu, 
				&m_brHighlight, NULL);

			CBCGPDrawManager dm (*pDC);
			dm.HighlightRect (rectMenu);
		}
	}
	else
	{
		CBrush* pBrush = 
			(pButton->IsPressed() || pButton->IsDroppedDown()) && !bIsMenuMode ? 
				&m_brHighlightDn : &m_brHighlight;

		CRect rectFill = rect;

		if (pButton->IsChecked() && !bIsMenuMode)
		{
			pBrush = bIsHighlighted ? 
					&m_brHighlightDn : &m_brHighlightChecked;

			if (!bIsHighlighted && !rectCommand.IsRectEmpty())
			{
				rectFill = rectCommand;
			}
		}

		OnFillHighlightedArea (pDC, rectFill, pBrush, NULL);
	}

	return (COLORREF)-1;
}
//****
void CBCGPVisualManagerXP::OnDrawRibbonCategoryScroll (
					CDC* pDC, 
					CBCGPRibbonCategoryScroll* pScroll)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pScroll);

	CRect rect = pScroll->GetRect();
	rect.b--;

	OnFillHighlightedArea (pDC, rect, 
		pScroll->IsHighlighted() ? &m_brHighlight : &globalData.brBarFace, NULL);

	BOOL bIsLeft = pScroll->IsLeftScroll();
	if (globalData.m_bIsRTL)
	{
		bIsLeft = !bIsLeft;
	}

	CBCGPMenuImages::Draw (pDC,
		bIsLeft ? CBCGPMenuImages::IdArowLeftLarge : CBCGPMenuImages::IdArowRightLarge, 
		rect);

	pDC->Draw3dRect (rect, globalData.clrBarShadow, globalData.clrBarShadow);
}
//****
void CBCGPVisualManagerXP::OnDrawRibbonButtonBorder (
					CDC* pDC, 
					CBCGPRibbonButton* pButton)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (pButton->IsDefaultPanelButton() && !pButton->IsQATMode())
	{
		return;
	}

	if (pButton->IsKindOf (RUNTIME_CLASS (CBCGPRibbonEdit)))
	{
		return;
	}

	const BOOL bIsMenuMode = pButton->IsMenuMode();

	const BOOL bIsHighlighted = 
		((pButton->IsHighlighted() || pButton->IsDroppedDown()) && 
		!pButton->IsDisabled()) || pButton->IsFocused();

	if (pButton->IsChecked() && bIsMenuMode && !bIsHighlighted)
	{
		return;
	}

	CRect rect = pButton->GetRect();
	CRect rectMenu = pButton->GetMenuRect();

	if (pButton->GetLocationInGroup() != CBCGPBaseRibbonElement::RibbonElementNotInGroup)
	{
		rect.DeflateRect (1, 1);
	}

	if ((bIsHighlighted || pButton->IsChecked()) && 
		(!pButton->IsDisabled() || pButton->IsFocused() || pButton->IsChecked()))
	{
		COLORREF clrLine = 
			((pButton->IsPressed() || pButton->IsDroppedDown()) && !bIsMenuMode) ?
			m_clrPressedButtonBorder : m_clrMenuItemBorder;

		if (CBCGPToolBarImages::m_bIsDrawOnGlass)
		{
			CBCGPDrawManager dm (*pDC);
			dm.DrawRect (rect, (COLORREF)-1, m_clrMenuItemBorder);
		}
		else
		{
			pDC->Draw3dRect (rect, clrLine, clrLine);
		}

		if (!rectMenu.IsRectEmpty())
		{
			if (pButton->GetLocationInGroup() != CBCGPBaseRibbonElement::RibbonElementNotInGroup)
			{
				rectMenu.DeflateRect (0, 1, 1, 1);
			}

			if (CBCGPToolBarImages::m_bIsDrawOnGlass)
			{
				CBCGPDrawManager dm (*pDC);
				
				if (pButton->IsMenuOnBottom())
				{
					dm.DrawLine (rectMenu.l, rectMenu.t, rectMenu.r, rectMenu.t, m_clrMenuItemBorder);
				}
				else
				{
					dm.DrawLine (rectMenu.l, rectMenu.t, rectMenu.l, rectMenu.b - 1, m_clrMenuItemBorder);
				}
			}
			else
			{
				CPen* pOldPen = pDC->SelectObject (&m_penMenuItemBorder);
				ASSERT (pOldPen != NULL);

				if (pButton->IsMenuOnBottom())
				{
					pDC->MoveTo (rectMenu.l, rectMenu.t);
					pDC->LineTo (rectMenu.r, rectMenu.t);
				}
				else
				{
					pDC->MoveTo (rectMenu.l, rectMenu.t);
					pDC->LineTo (rectMenu.l, rectMenu.b);
				}

				pDC->SelectObject (pOldPen);
			}
		}
	}
}
//****
void CBCGPVisualManagerXP::OnDrawRibbonMenuCheckFrame (
					CDC* pDC,
					CBCGPRibbonButton* /*pButton*/, 
					CRect rect)
{
	ASSERT_VALID (pDC);

	OnFillHighlightedArea (pDC, rect, &m_brHighlight, NULL);
	pDC->Draw3dRect (rect, m_clrMenuItemBorder, m_clrMenuItemBorder);
}
//****
void CBCGPVisualManagerXP::OnFillRibbonMenuFrame (
					CDC* pDC, 
					CBCGPRibbonMainPanel* /*pPanel*/,
					CRect rect)
{
	ASSERT_VALID (pDC);
	pDC->FillRect (rect, &m_brMenuLight);
}
//****
void CBCGPVisualManagerXP::OnDrawRibbonRecentFilesFrame (
					CDC* pDC, 
					CBCGPRibbonMainPanel* /*pPanel*/,
					CRect rect)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &m_brBarBkgnd);

	CRect rectSeparator = rect;
	rectSeparator.r = rectSeparator.l + 2;

	pDC->Draw3dRect (rectSeparator, globalData.clrBarShadow,
									globalData.clrBarHilite);
}
//****
COLORREF CBCGPVisualManagerXP::OnDrawRibbonStatusBarPane (CDC* pDC, CBCGPRibbonStatusBar* /*pBar*/,
					CBCGPRibbonStatusBarPane* pPane)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pPane);

	CRect rect = pPane->GetRect();

	if (pPane->IsHighlighted())
	{
		CRect rectButton = rect;
		rectButton.DeflateRect (1, 1);

		OnFillHighlightedArea (pDC, rectButton, 
			pPane->IsPressed() ? 
			&m_brHighlightDn : &m_brHighlight, NULL);

		pDC->Draw3dRect (rectButton, m_clrMenuItemBorder, m_clrMenuItemBorder);
	}

	CRect rectSeparator = rect;
	rectSeparator.DeflateRect (0, 2);

	rectSeparator.l = rectSeparator.r - 1;

	pDC->Draw3dRect (rectSeparator, globalData.clrBarShadow,
									globalData.clrBarShadow);

	if (globalData.IsHighContastMode())
	{
		return globalData.clrBarText;
	}

	return (COLORREF)-1;
}
//****
void CBCGPVisualManagerXP::GetRibbonSliderColors (CBCGPRibbonSlider* /*pSlider*/,
							BOOL bIsHighlighted, 
							BOOL bIsPressed,
							BOOL bIsDisabled,
							COLORREF& clrLine,
							COLORREF& clrFill)
{
	clrLine = bIsDisabled ? globalData.clrBarShadow : 
		(bIsPressed || bIsHighlighted) ? m_clrMenuItemBorder : globalData.clrBarDkShadow;

	clrFill = bIsPressed && bIsHighlighted ?
		m_clrHighlightDn :
		bIsHighlighted ? m_clrHighlight : globalData.clrBarFace;
}
//****
void CBCGPVisualManagerXP::OnDrawRibbonQATSeparator (CDC* pDC, 
												   CBCGPRibbonSeparator* /*pSeparator*/, CRect rect)
{
	ASSERT_VALID (pDC);

	int x = rect.CenterPoint().x;
	int y1 = rect.t;
	int y2 = rect.b - 1;

	if (CBCGPToolBarImages::m_bIsDrawOnGlass)
	{
		CBCGPDrawManager dm (*pDC);
		dm.DrawLine (x, y1, x, y2, m_clrSeparator);
	}
	else
	{
		CPen* pOldPen = pDC->SelectObject (&m_penSeparator);
		ASSERT (pOldPen != NULL);

		pDC->MoveTo (x, y1);
		pDC->LineTo (x, y2);

		pDC->SelectObject (pOldPen);
	}
}
//****
void CBCGPVisualManagerXP::OnDrawRibbonColorPaletteBox (CDC* pDC, CBCGPRibbonColorButton* /*pColorButton*/,
		CBCGPRibbonPaletteIcon* /*pIcon*/,
		COLORREF color, CRect rect, BOOL bDrawTopEdge, BOOL bDrawBottomEdge,
		BOOL bIsHighlighted, BOOL bIsChecked, BOOL /*bIsDisabled*/)
{
	ASSERT_VALID (pDC);

	CRect rectFill = rect;
	rectFill.DeflateRect (1, 0);

	if (bIsHighlighted || bIsChecked)
	{
		OnFillHighlightedArea (pDC, rect, &m_brHighlight, NULL);
		rectFill.DeflateRect (1, 2);
	}

	if (color != (COLORREF)-1)
	{
		CBrush br (color);
		pDC->FillRect (rectFill, &br);
	}

	COLORREF clrBorder = _RGB (197, 197, 197);

	if (bDrawTopEdge && bDrawBottomEdge)
	{
		pDC->Draw3dRect (rect, clrBorder, clrBorder);
	}
	else
	{
		CPen penBorder (PS_SOLID, 1, clrBorder);

		CPen* pOldPen = pDC->SelectObject (&penBorder);
		ASSERT (pOldPen != NULL);

		pDC->MoveTo (rect.l, rect.t);
		pDC->LineTo (rect.l, rect.b);

		pDC->MoveTo (rect.r - 1, rect.t);
		pDC->LineTo (rect.r - 1, rect.b);

		if (bDrawTopEdge)
		{
			pDC->MoveTo (rect.l, rect.t);
			pDC->LineTo (rect.r, rect.t);
		}

		if (bDrawBottomEdge)
		{
			pDC->MoveTo (rect.l, rect.b - 1);
			pDC->LineTo (rect.r, rect.b - 1);
		}

		pDC->SelectObject (pOldPen);
	}

	if (bIsHighlighted || bIsChecked)
	{
		clrBorder = bIsChecked ? m_clrPressedButtonBorder : m_clrMenuItemBorder;
		pDC->Draw3dRect (rect, clrBorder, clrBorder);
	}
}
//****
COLORREF CBCGPVisualManagerXP::OnDrawRibbonPanel (
		CDC* pDC,
		CBCGPRibbonPanel* pPanel, 
		CRect rectPanel,
		CRect /*rectCaption*/)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pPanel);

	COLORREF clrText = globalData.clrBarText;

	if (pPanel->IsCollapsed() && pPanel->GetDefaultButton().IsFocused())
	{
		OnFillHighlightedArea (pDC, rectPanel, 
			pPanel->GetDefaultButton().IsDroppedDown() ? &m_brHighlightDn : &m_brHighlight, NULL);

		if (GetRV (m_clrHighlight) > 128 &&
			GetGV (m_clrHighlight) > 128 &&
			GetBV (m_clrHighlight) > 128)
		{
			clrText = _RGB (0, 0, 0);
		}
		else
		{
			clrText = _RGB (255, 255, 255);
		}
	}
	else if (pPanel->IsHighlighted())
	{
		CBCGPDrawManager dm (*pDC);
		dm.HighlightRect (rectPanel);
	}

	pDC->Draw3dRect (rectPanel, globalData.clrBarHilite, globalData.clrBarHilite);
	rectPanel.OffsetRect (-1, -1);
	pDC->Draw3dRect (rectPanel, globalData.clrBarShadow, globalData.clrBarShadow);

	return clrText;
}

#endif // BCGP_EXCLUDE_RIBBON

COLORREF CBCGPVisualManagerXP::OnFillListBoxItem (CDC* pDC, CBCGPListBox* /*pListBox*/, int /*nItem*/, CRect rect, BOOL bIsHighlihted, BOOL bIsSelected)
{
	ASSERT_VALID (pDC);

	CBrush* pBrush = NULL; 

	if (bIsSelected)
	{
		pBrush = !bIsHighlihted ? &m_brHighlightChecked : &m_brHighlightDn;
	}
	else if (bIsHighlihted)
	{
		pBrush = &m_brHighlight;
	}

	OnFillHighlightedArea (pDC, rect, pBrush, NULL);
	pDC->Draw3dRect (rect, m_clrMenuItemBorder, m_clrMenuItemBorder);

	return globalData.clrBtnText;
}
//****
COLORREF CBCGPVisualManagerXP::OnDrawMenuLabel (CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &m_brBarBkgnd);

	CRect rectSeparator = rect;
	rectSeparator.t = rectSeparator.b - 2;

	pDC->Draw3dRect (rectSeparator, globalData.clrBarShadow,
									globalData.clrBarHilite);

	return globalData.clrBarText;
}
//****
COLORREF CBCGPVisualManagerXP::OnFillCaptionBarButton (CDC* pDC, CBCGPCaptionBar* pBar,
											CRect rect, BOOL bIsPressed, BOOL bIsHighlighted, 
											BOOL bIsDisabled, BOOL bHasDropDownArrow,
											BOOL bIsSysButton)
{
	ASSERT_VALID (pBar);

	if (!pBar->IsMessageBarMode())
	{
		return CBCGPVisualManager::OnFillCaptionBarButton (pDC, pBar,
											rect, bIsPressed, bIsHighlighted, 
											bIsDisabled, bHasDropDownArrow, bIsSysButton);
	}

	if (bIsDisabled)
	{
		return (COLORREF)-1;
	}

	COLORREF clrText = globalData.clrBarText;

	if (bIsHighlighted)
	{
		OnFillHighlightedArea (pDC, rect, &m_brHighlight, NULL);

		if (GetRV (m_clrHighlight) > 128 &&
			GetGV (m_clrHighlight) > 128 &&
			GetBV (m_clrHighlight) > 128)
		{
			clrText = _RGB (0, 0, 0);
		}
		else
		{
			clrText = _RGB (255, 255, 255);
		}
	}
	else if (!bIsSysButton)
	{
		pDC->FillRect (rect, &m_brMenuLight);
	}

	return clrText;
}
//****
void CBCGPVisualManagerXP::OnDrawCaptionBarButtonBorder (CDC* pDC, CBCGPCaptionBar* pBar,
											CRect rect, BOOL bIsPressed, BOOL bIsHighlighted, 
											BOOL bIsDisabled, BOOL bHasDropDownArrow,
											BOOL bIsSysButton)
{
	ASSERT_VALID (pBar);

	if (!pBar->IsMessageBarMode())
	{
		CBCGPVisualManager::OnDrawCaptionBarButtonBorder (pDC, pBar,
											rect, bIsPressed, bIsHighlighted, 
											bIsDisabled, bHasDropDownArrow, bIsSysButton);
		return;
	}

	ASSERT_VALID (pDC);

	if (bIsHighlighted)
	{
		if (bIsSysButton && bIsPressed && m_clrPressedButtonBorder != (COLORREF)-1)
		{
			pDC->Draw3dRect (rect, m_clrPressedButtonBorder, m_clrPressedButtonBorder);
		}
		else
		{
			pDC->Draw3dRect (rect, m_clrMenuItemBorder, m_clrMenuItemBorder);
		}
	}
	else if (!bIsSysButton)
	{
		pDC->Draw3dRect (rect, globalData.clrBarDkShadow, globalData.clrBarDkShadow);
	}
}
//****
void CBCGPVisualManagerXP::OnDrawCaptionBarInfoArea (CDC* pDC, CBCGPCaptionBar* /*pBar*/, CRect rect)
{
	ASSERT_VALID (pDC);

	::FillRect (pDC, rect, GetSysColorBrush (COLOR_INFOBK));
	pDC->Draw3dRect (rect, globalData.clrBarShadow, globalData.clrBarShadow);
}
//****
BOOL CBCGPVisualManagerXP::OnDrawPushButton (CDC* pDC, CRect rect, CBCGPButton* pButton, COLORREF& clrText)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	CBCGPDrawManager dm (*pDC);

	COLORREF clrFace = globalData.clrBarFace;
	CBCGPDrawOnGlass dog (pButton->m_bOnGlass);

	if (!pButton->IsWindowEnabled())
	{
		if (pButton->m_bOnGlass)
		{
			dm.DrawRect (rect, clrFace, globalData.clrBarShadow);
		}
		else
		{
			pDC->FillRect (rect, &globalData.brBarFace);
			pDC->Draw3dRect (rect, globalData.clrBarShadow, globalData.clrBarShadow);
		}
	}
	else if (pButton->IsPressed() || pButton->GetCheck())
	{
		OnFillHighlightedArea (pDC, rect, &m_brHighlightDn, NULL);

		if (pButton->m_bOnGlass)
		{
			dm.DrawRect (rect, (COLORREF)-1, m_clrMenuItemBorder);
		}
		else
		{
			pDC->Draw3dRect (rect, m_clrMenuItemBorder, m_clrMenuItemBorder);
		}

		clrFace = m_clrHighlightDn;
	}
	else if (pButton->IsHighlighted() || CWnd::GetFocus() == pButton)
	{
		OnFillHighlightedArea (pDC, rect, &m_brHighlight, NULL);

		if (pButton->m_bOnGlass)
		{
			dm.DrawRect (rect, (COLORREF)-1, m_clrMenuItemBorder);
		}
		else
		{
			pDC->Draw3dRect (rect, m_clrMenuItemBorder, m_clrMenuItemBorder);
		}

		clrFace = m_clrHighlight;
	}
	else
	{
		if (pButton->m_bOnGlass)
		{
			dm.DrawRect (rect, clrFace, globalData.clrBarDkShadow);
		}
		else
		{
			pDC->FillRect (rect, &globalData.brBarFace);
			pDC->Draw3dRect (rect, globalData.clrBarDkShadow, globalData.clrBarDkShadow);
		}
	}

	if (GetRV (clrFace) <= 128 ||
		GetGV (clrFace) <= 128 ||
		GetBV (clrFace) <= 128)
	{
		clrText = _RGB (255, 255, 255);
	}

	return TRUE;
}

#ifndef BCGP_EXCLUDE_GRID_CTRL

void CBCGPVisualManagerXP::OnDrawGridHeaderMenuButton (CBCGPGridCtrl* /*pCtrl*/, CDC* pDC, CRect rect, 
		BOOL bHighlighted, BOOL /*bPressed*/, BOOL /*bDisabled*/)
{
	ASSERT_VALID (pDC);

	rect.DeflateRect (1, 1);

	if (bHighlighted)
	{
		OnFillHighlightedArea (pDC, rect, &m_brHighlight, NULL);
		pDC->Draw3dRect (rect, m_clrMenuItemBorder, m_clrMenuItemBorder);
	}
}

BOOL CBCGPVisualManagerXP::OnSetGridColorTheme (CBCGPGridCtrl* pCtrl, BCGP_GRID_COLOR_DATA& theme)
{
	theme.m_EvenColors.m_clrBackground      = m_clrBarBkgnd;
	theme.m_EvenColors.m_clrText            = globalData.clrBarText;

	theme.m_OddColors.m_clrBackground       = m_clrMenuLight;
	theme.m_OddColors.m_clrText             = globalData.clrBarText;

	theme.m_SelColors.m_clrBackground       = m_clrHighlight;
	theme.m_SelColors.m_clrBorder			= m_clrMenuItemBorder;

	if (GetRV (m_clrHighlight) > 128 &&
		GetGV (m_clrHighlight) > 128 &&
		GetBV (m_clrHighlight) > 128)
	{
		theme.m_SelColors.m_clrText = _RGB (0, 0, 0);
	}
	else
	{
		theme.m_SelColors.m_clrText = _RGB (255, 255, 255);
	}

	theme.m_GroupColors.m_clrBackground     = GetGridLeftOffsetColor (pCtrl);
	theme.m_GroupColors.m_clrText           = globalData.clrBarText;

	return TRUE;
}

#endif
