
//
// BCGPCaptionMenuButton.cpp: implementation of the CBCGPCaptionMenuButton class.
//
/////////////////////////////////////////////////////////////////////////////////

#include "bcgcbpro.h"
#include "BCGGlobals.h"
#include "BCGPVisualManager.h"
#include "BCGPContextMenuManager.h"
#include "BCGPCaptionButton.h"
#include "BCGPCaptionMenuButton.h"

/////////////////////////////////////////////////////////////////////////////
// CBCGPCaptionMenuButton

CBCGPCaptionMenuButton::CBCGPCaptionMenuButton()
{
	m_nMenuResult = 0;
	m_bOSMenu = TRUE;
	m_bMenuIsActive = FALSE;
}

CBCGPCaptionMenuButton::CBCGPCaptionMenuButton(UINT nHit, BOOL bLeftAlign) 
	: CBCGPCaptionButton(nHit, bLeftAlign)
{
	m_nMenuResult = 0;
	m_bOSMenu = TRUE;
	m_bMenuIsActive = FALSE;
}

CBCGPCaptionMenuButton::~CBCGPCaptionMenuButton()
{
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPCaptionMenuButton message handlers

void CBCGPCaptionMenuButton::OnDraw (CDC* pDC, BOOL bActive,
	BOOL bHorz, BOOL bMaximized, BOOL bDisabled)
{
	if (m_bHidden)
	{
		return;
	}

	CBCGPVisualManager::GetInstance ()->OnDrawCaptionButton (
			pDC, this, bActive, bHorz, bMaximized, bDisabled);
}
//****
void CBCGPCaptionMenuButton::ShowMenu (HMENU hMenu, CWnd* pWndOwner)
{
	ASSERT_VALID (pWndOwner);

	CRect rectWnd;
	pWndOwner->GetWindowRect (&rectWnd);
	CSize size = GetSize ();
	CPoint point = m_ptOrg + rectWnd.TopLeft ();
	point.x += size.cx;
	point.y += size.cy;

	m_bMenuIsActive = TRUE;
	m_bPushed = TRUE;
	pWndOwner->InvalidateRect (GetRect ());
	pWndOwner->SendMessage(WM_NCPAINT);

	if (!m_bOSMenu && g_pContextMenuManager != NULL)
	{
		m_nMenuResult = g_pContextMenuManager->TrackPopupMenu (
			hMenu, point.x, point.y, pWndOwner, TRUE /* RightAlign */);
	}
	else
	{
		m_nMenuResult = ::TrackPopupMenu (hMenu, 
			TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, 
			point.x, point.y, 0, pWndOwner->GetSafeHwnd (), NULL);
	}

	m_bMenuIsActive = FALSE;
	m_bPushed = FALSE;
	pWndOwner->InvalidateRect (GetRect ());
	pWndOwner->SendMessage(WM_NCPAINT);
}