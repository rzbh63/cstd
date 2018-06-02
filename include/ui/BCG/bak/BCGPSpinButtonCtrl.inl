//
// BCGPSpinButtonCtrl.cpp : implementation file
//

//#include "BCGCBPro.h"
#include "trackmouse.h"
#include "BCGPDlgImpl.h"

#if defined (_BCGPCALENDAR_STANDALONE)
	#include "BCGPCalendarVisualManager.h"
	#define visualManager	CBCGPCalendarVisualManager::GetInstance()
#elif defined (_BCGPGRID_STANDALONE)
	#include "BCGPGridVisualManager.h"
	#define visualManager	CBCGPGridVisualManager::GetInstance()
#else
	#include "BCGPVisualManager.h"
	#define visualManager	CBCGPVisualManager::GetInstance()
#endif

#include "BCGGlobals.h"
#include "BCGPSpinButtonCtrl.h"

#ifndef _BCGSUITE_

#endif

#include "BCGPDrawManager.h"

/////////////////////////////////////////////////////////////////////////////
// CBCGPSpinButtonCtrl

IMPLEMENT_DYNAMIC(CBCGPSpinButtonCtrl, CSpinButtonCtrl)

CBCGPSpinButtonCtrl::CBCGPSpinButtonCtrl()
{
	m_bTracked = FALSE;

	m_bIsButtonPressedUp = FALSE;
	m_bIsButtonPressedDown = FALSE;

	m_bIsButtonHighligtedUp = FALSE;
	m_bIsButtonHighligtedDown = FALSE;

	m_bVisualManagerStyle = FALSE;
	m_bOnGlass = FALSE;
}
//****
CBCGPSpinButtonCtrl::~CBCGPSpinButtonCtrl()
{
}

BEGIN_MESSAGE_MAP(CBCGPSpinButtonCtrl, CSpinButtonCtrl)
	//{{AFX_MSG_MAP(CBCGPSpinButtonCtrl)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CANCELMODE()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_REGISTERED_MESSAGE(BCGM_ONSETCONTROLVMMODE, OnBCGSetControlVMMode)
	ON_REGISTERED_MESSAGE(BCGM_ONSETCONTROLAERO, OnBCGSetControlAero)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPSpinButtonCtrl message handlers

void CBCGPSpinButtonCtrl::OnPaint(CDC* pDC) 
{
	CPaintDC dc(this); // device context for painting
	CBCGPMemDC memDC (dc, this);

	OnDraw (&memDC.GetDC());
}
//****
void CBCGPSpinButtonCtrl::OnDraw (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	CRect rectClient;
	GetClientRect (rectClient);

	if (GetBuddy()->GetSafeHwnd() != NULL && (m_bOnGlass || m_bVisualManagerStyle))
	{
		CRect rectBorder (0, 0, 0, 0);

		if (GetStyle() & UDS_ALIGNRIGHT)
		{
			rectBorder = rectClient;
			rectClient.DeflateRect (1, 1);
		}
		else if (GetStyle() & UDS_ALIGNLEFT)
		{
			rectBorder = rectClient;
			rectClient.DeflateRect (1, 1);
		}

		if (!rectBorder.IsRectEmpty())
		{
			visualManager->OnDrawControlBorder (
				pDC, rectBorder, this, CBCGPToolBarImages::m_bIsDrawOnGlass || m_bOnGlass);
		}
	}

	if (CBCGPToolBarImages::m_bIsDrawOnGlass || m_bOnGlass)
	{
		CBCGPDrawManager dm (*pDC);
		dm.DrawRect (rectClient, globalData.clrWindow, (COLORREF)-1);
	}
	else
	{
		pDC->FillRect (rectClient, &globalData.brWindow);
	}

	int nState = 0;

	if (m_bIsButtonPressedUp)
	{
		nState |= SPIN_PRESSEDUP;
	}

	if (m_bIsButtonPressedDown)
	{
		nState |= SPIN_PRESSEDDOWN;
	}

	if (m_bIsButtonHighligtedUp)
	{
		nState |= SPIN_HIGHLIGHTEDUP;
	}

	if (m_bIsButtonHighligtedDown)
	{
		nState |= SPIN_HIGHLIGHTEDDOWN;
	}

	if (!IsWindowEnabled())
	{
		nState |= SPIN_DISABLED;
	}

	CBCGPDrawOnGlass dog (m_bOnGlass);

	visualManager->OnDrawSpinButtons (
		pDC, rectClient, nState, FALSE, this);
}
//****
void CBCGPSpinButtonCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rect;
	GetClientRect (rect);

	CRect rectUp = rect;
	rectUp.b = rect.CenterPoint().y;

	CRect rectDown = rect;
	rectDown.t = rectUp.b;

	m_bIsButtonPressedUp = rectUp.PtInRect (point);
	m_bIsButtonPressedDown = rectDown.PtInRect (point);
	
	CSpinButtonCtrl::OnLButtonDown(nFlags, point);
}
//****
void CBCGPSpinButtonCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bIsButtonPressedUp = FALSE;
	m_bIsButtonPressedDown = FALSE;
	
	m_bIsButtonHighligtedUp = FALSE;
	m_bIsButtonHighligtedDown = FALSE;

	CSpinButtonCtrl::OnLButtonUp(nFlags, point);
}
//****
void CBCGPSpinButtonCtrl::OnCancelMode() 
{
	CSpinButtonCtrl::OnCancelMode();

	m_bIsButtonPressedUp = FALSE;
	m_bIsButtonPressedDown = FALSE;

	m_bIsButtonHighligtedUp = FALSE;
	m_bIsButtonHighligtedDown = FALSE;
}
//****
void CBCGPSpinButtonCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	BOOL bIsButtonHighligtedUp = m_bIsButtonHighligtedUp;
	BOOL bIsButtonHighligtedDown = m_bIsButtonHighligtedDown;

	CRect rect;
	GetClientRect (rect);

	CRect rectUp = rect;
	rectUp.b = rect.CenterPoint().y;

	CRect rectDown = rect;
	rectDown.t = rectUp.b;

	m_bIsButtonHighligtedUp = rectUp.PtInRect (point);
	m_bIsButtonHighligtedDown = rectDown.PtInRect (point);

	if (nFlags & MK_LBUTTON)
	{
		m_bIsButtonPressedUp = m_bIsButtonHighligtedUp;
		m_bIsButtonPressedDown = m_bIsButtonHighligtedDown;
	}
	
	CSpinButtonCtrl::OnMouseMove(nFlags, point);

	if (bIsButtonHighligtedUp != m_bIsButtonHighligtedUp ||
		bIsButtonHighligtedDown != m_bIsButtonHighligtedDown)
	{
		RedrawWindow();
	}

	if (!m_bTracked)
	{
		m_bTracked = TRUE;
		
		TRACKMOUSEEVENT trackmouseevent;
		trackmouseevent.cbSize = sizeof(trackmouseevent);
		trackmouseevent.dwFlags = TME_LEAVE;
		trackmouseevent.hwndTrack = GetSafeHwnd();
		trackmouseevent.dwHoverTime = HOVER_DEFAULT;
		::BCGPTrackMouse (&trackmouseevent);	
	}
}
//****
LRESULT CBCGPSpinButtonCtrl::OnMouseLeave(WPARAM,LPARAM)
{
	m_bTracked = FALSE;

	if (m_bIsButtonPressedUp || m_bIsButtonPressedDown ||
		m_bIsButtonHighligtedUp || m_bIsButtonHighligtedDown)
	{
		m_bIsButtonHighligtedUp = FALSE;
		m_bIsButtonHighligtedDown = FALSE;

		RedrawWindow();
	}

	return 0;
}
//****
BOOL CBCGPSpinButtonCtrl::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}
//****
LRESULT CBCGPSpinButtonCtrl::OnBCGSetControlVMMode (WPARAM wp, LPARAM)
{
	m_bVisualManagerStyle = (BOOL) wp;
	return 0;
}
//****
LRESULT CBCGPSpinButtonCtrl::OnBCGSetControlAero (WPARAM wp, LPARAM)
{
	m_bOnGlass = (BOOL) wp;
	return 0;
}
