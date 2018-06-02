
//
// BCGPScrollBar.cpp : implementation file
//

#include "TrackMouse.h"
#include "BCGGlobals.h"
#include "BCGPScrollBar.h"

UINT BCGM_ON_DRAW_SCROLLBAR = ::RegisterWindowMessage (_T("BCGM_ON_DRAW_SCROLLBAR"));

/////////////////////////////////////////////////////////////////////////////
// CBCGPScrollBar

const UINT SB_TIMER_DOWN = 1111;
const UINT SB_TIMER      = 2222;

IMPLEMENT_DYNAMIC(CBCGPScrollBar, CScrollBar)

CBCGPScrollBar::CBCGPScrollBar()
	: m_Style		 (CBCGPScrollBar::BCGP_SBSTYLE_VISUAL_MANAGER) 
	, m_bTracked     (FALSE)
	, m_HitTest      (CBCGPScrollBar::BCGP_HT_NOWHERE)
	, m_HitPressed   (CBCGPScrollBar::BCGP_HT_NOWHERE)
	, m_pLastCapture (NULL)
	, m_TimerDown    (0)
	, m_Timer        (0)
	, m_ptTrackStart (0, 0)
	, m_bAutoDestoyWindow (FALSE)
	, m_dwDisabledArrows (0)
{
}

CBCGPScrollBar::~CBCGPScrollBar()
{
}


BEGIN_MESSAGE_MAP(CBCGPScrollBar, CScrollBar)
	//{{AFX_MSG_MAP(CBCGPScrollBar)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CANCELMODE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_NCDESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPScrollBar message handlers

const int c_ScrollMin      = 4;
const int c_ScrollThumbMin = 8;

CRect CBCGPScrollBar::GetThumbRect (const CRect& rect, BOOL bHorz) const
{
	CRect rectThumb (0, 0, 0, 0);

	if (m_HitPressed == CBCGPScrollBar::BCGP_HT_THUMB)
	{
		rectThumb = m_rectTrackThumb;
	}
	else
	{
		if (!IsWindowEnabled() || 
			(m_dwDisabledArrows & ESB_DISABLE_BOTH) == ESB_DISABLE_BOTH)
		{
			return rectThumb;
		}

		rectThumb = rect;

		int nClientSize = 0;
		int nThumbSize  = 0;
		if (bHorz)
		{
			nThumbSize = ::GetSystemMetrics (SM_CXHSCROLL);
			rectThumb.DeflateRect (nThumbSize, 0);
			nClientSize = rectThumb.Width();
		}
		else
		{
			nThumbSize = ::GetSystemMetrics (SM_CYVSCROLL);
			rectThumb.DeflateRect (0, nThumbSize);
			nClientSize = rectThumb.Height();
		}

		if (nClientSize <= c_ScrollMin)
		{
			rectThumb.SetRectEmpty();
			return rectThumb;
		}

		int nThumbPos = nThumbSize;

		SCROLLINFO si;
		si.cbSize = sizeof (SCROLLINFO);
		si.fMask  = SIF_RANGE | SIF_POS | SIF_PAGE;

		::GetScrollInfo(GetSafeHwnd(), SB_CTL, &si);

		if (si.nPage != 0)
		{
			nThumbSize = max (::MulDiv(nClientSize, si.nPage, si.nMax - si.nMin + 1), c_ScrollThumbMin);
		}

		if (nClientSize < nThumbSize || nThumbSize == 0)
		{
			rectThumb.SetRectEmpty();
			return rectThumb;
		}
		else
		{
			nClientSize -= nThumbSize;
			int nMax = si.nMax - max (si.nPage - 1, 0);
			if (si.nMin < nMax)
			{
				nThumbPos += ::MulDiv(nClientSize, si.nPos - si.nMin, nMax - si.nMin);
			}
		}

		if (bHorz)
		{
			rectThumb.l   = rect.l + nThumbPos;
			rectThumb.r  = rectThumb.l + nThumbSize;
		}
		else
		{
			rectThumb.t    = rect.t + nThumbPos;
			rectThumb.b = rectThumb.t + nThumbSize;
		}
	}

	return rectThumb;
}

void CBCGPScrollBar::DoPaint (CDC* pDC)
{
	CRect rect;
	GetClientRect (rect);

	DWORD dwStyle  = GetStyle();
	BOOL bDisabled = !IsWindowEnabled() || 
		(m_dwDisabledArrows & ESB_DISABLE_BOTH) == ESB_DISABLE_BOTH;

	if (!bDisabled)
	{
		SCROLLINFO si;
		si.cbSize = sizeof (SCROLLINFO);
		GetScrollInfo (&si, SIF_RANGE | SIF_PAGE);

		bDisabled = si.nMax <= si.nMin || si.nPage <= 0;
	}

	if (!rect.IsRectEmpty())
	{
		const BOOL bHorz = (dwStyle & SBS_VERT) == 0;

		CRect rectThumb (0, 0, 0, 0);
		if (!bDisabled)
		{
			rectThumb = GetThumbRect (rect, bHorz);
		}

		CRect rectBtn[2];
		rectBtn[0] = rect;

		if (bHorz)
		{
			int nSize  = ::GetSystemMetrics (SM_CXHSCROLL);
			if (rect.Width() < nSize * 2)
			{
				nSize = rect.Width() / 2;
			}

			rectBtn[0].r  = rect.l + nSize;
			rectBtn[1] = rectBtn[0];
			rectBtn[1].OffsetRect (rect.Width() - rectBtn[1].Width(), 0);

			rect.l += rectBtn[0].Width();
			rect.r -= rectBtn[1].Width();

			if (rectThumb.l < rectBtn[0].r || rectBtn[1].l < rectThumb.r)
			{
				rectThumb.SetRectEmpty();
			}
		}
		else
		{
			int nSize  = ::GetSystemMetrics (SM_CYVSCROLL);
			if (rect.Height() < nSize * 2)
			{
				nSize = rect.Height() / 2;
			}

			rectBtn[0].b = rect.t + nSize;
			rectBtn[1] = rectBtn[0];
			rectBtn[1].OffsetRect (0, rect.Height() - rectBtn[1].Height());

			rect.t += rectBtn[0].Height();
			rect.b -= rectBtn[1].Height();

			if (rectThumb.t < rectBtn[0].b || rectBtn[1].t < rectThumb.b)
			{
				rectThumb.SetRectEmpty();
			}
		}

		DoPaintButton (pDC, rectBtn[0], bHorz, 
			m_HitTest == CBCGPScrollBar::BCGP_HT_BUTTON_LTUP, 
			m_HitPressed == CBCGPScrollBar::BCGP_HT_BUTTON_LTUP, TRUE, 
			bDisabled || (m_dwDisabledArrows & ESB_DISABLE_LTUP) == ESB_DISABLE_LTUP);
		
		DoPaintButton (pDC, rectBtn[1], bHorz, 
			m_HitTest == CBCGPScrollBar::BCGP_HT_BUTTON_RTDN, 
			m_HitPressed == CBCGPScrollBar::BCGP_HT_BUTTON_RTDN, FALSE, 
			bDisabled || (m_dwDisabledArrows & ESB_DISABLE_RTDN) == ESB_DISABLE_RTDN);

		if (!bDisabled && !rectThumb.IsRectEmpty())
		{
			DoPaintThumb (pDC, rectThumb, bHorz, 
				m_HitTest == CBCGPScrollBar::BCGP_HT_THUMB || 
				m_HitPressed == CBCGPScrollBar::BCGP_HT_THUMB,
				m_HitPressed == CBCGPScrollBar::BCGP_HT_THUMB, bDisabled);

			BOOL bDrawBack = FALSE;
			CRect rectBack (rect);
			if (bHorz)
			{
				rectBack.r = rectThumb.l;
				bDrawBack = rectBack.l < rectBack.r;
			}
			else
			{
				rectBack.b = rectThumb.t;
				bDrawBack = rectBack.t < rectBack.b;
			}

			if (bDrawBack)
			{
				DoPaintBackground (pDC, rectBack, bHorz, 
					m_HitTest == CBCGPScrollBar::BCGP_HT_CLIENT_LTUP, 
					m_HitPressed == CBCGPScrollBar::BCGP_HT_CLIENT_LTUP, TRUE, bDisabled);
			}

			bDrawBack = FALSE;
			rectBack = rect;
			if (bHorz)
			{
				rectBack.l = rectThumb.r;
				bDrawBack = rectBack.l < rectBack.r;
			}
			else
			{
				rectBack.t = rectThumb.b;
				bDrawBack = rectBack.t < rectBack.b;
			}

			if (bDrawBack)
			{
				DoPaintBackground (pDC, rectBack, bHorz, 
					m_HitTest == CBCGPScrollBar::BCGP_HT_CLIENT_RTDN, 
					m_HitPressed == CBCGPScrollBar::BCGP_HT_CLIENT_RTDN, FALSE, bDisabled);
			}
		}
		else if (!rect.IsRectEmpty())
		{
			DoPaintBackground (pDC, rect, bHorz, FALSE, FALSE, TRUE, bDisabled);
		}
	}
}

int CBCGPScrollBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollBar::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	
	return 0;
}

void CBCGPScrollBar::PreSubclassWindow() 
{
	CScrollBar::PreSubclassWindow();
}

void CBCGPScrollBar::OnPaint(CDC* pDC) 
{
	if (!CBCGPVisualManager::GetInstance()->IsOwnerDrawScrollBar() ||
		m_Style == BCGP_SBSTYLE_DEFAULT)
	{
		Default();
		return;
	}

	CPaintDC dc(this); // device context for painting
	
	CBCGPMemDC memDC (dc, this);
	CDC* pDC = &memDC.GetDC();

	DoPaint (pDC);
}

void CBCGPScrollBar::OnCancelMode() 
{
	if (m_TimerDown != 0)
	{
		KillTimer (SB_TIMER_DOWN);
		m_TimerDown = 0;
	}
	if (m_Timer != 0)
	{
		KillTimer (SB_TIMER);
		m_Timer = 0;
	}

	CScrollBar::OnCancelMode();

	if (GetCapture() == this)
	{
		ReleaseCapture();
	}

	m_ptTrackStart = CPoint (0, 0);

	if (m_pLastCapture != NULL)
	{
		m_pLastCapture->SetCapture();
		m_pLastCapture = NULL;
	}

	CPoint point;
	::GetCursorPos (&point);
	CBCGPScrollBar::BCGPSB_HITTEST hit = HitTest (point);

	if (m_HitPressed != CBCGPScrollBar::BCGP_HT_NOWHERE ||
		m_HitTest != hit)
	{
		m_HitTest    = hit;
		m_HitPressed = CBCGPScrollBar::BCGP_HT_NOWHERE;

		RedrawWindow();
	}
}

CBCGPScrollBar::BCGPSB_HITTEST CBCGPScrollBar::HitTest (const CPoint& point) const
{
	CRect rect;
	GetClientRect (rect);
	ClientToScreen (rect);

	CBCGPScrollBar::BCGPSB_HITTEST hit = CBCGPScrollBar::BCGP_HT_NOWHERE;

	if (rect.PtInRect (point))
	{
		hit = CBCGPScrollBar::BCGP_HT_CLIENT;

		BOOL bHorz = (GetStyle() & SBS_VERT) == 0;

		CRect rectThumb (GetThumbRect (rect, bHorz));

		CRect rectBtn[2];
		rectBtn[0] = rect;

		if (bHorz)
		{
			int nSize  = ::GetSystemMetrics (SM_CXHSCROLL);
			if (rect.Width() < nSize * 2)
			{
				nSize = rect.Width() / 2;
			}

			rectBtn[0].r  = rect.l + nSize;
			rectBtn[1] = rectBtn[0];
			rectBtn[1].OffsetRect (rect.Width() - rectBtn[1].Width(), 0);

			rect.l += rectBtn[0].Width();
			rect.r -= rectBtn[1].Width();

			if (rectThumb.l < rectBtn[0].r || rectBtn[1].l < rectThumb.r)
			{
				rectThumb.SetRectEmpty();
			}
		}
		else
		{
			int nSize  = ::GetSystemMetrics (SM_CYVSCROLL);
			if (rect.Height() < nSize * 2)
			{
				nSize = rect.Height() / 2;
			}

			rectBtn[0].b = rect.t + nSize;
			rectBtn[1] = rectBtn[0];
			rectBtn[1].OffsetRect (0, rect.Height() - rectBtn[1].Height());
			
			rect.t += rectBtn[0].Height();
			rect.b -= rectBtn[1].Height();

			if (rectThumb.t < rectBtn[0].b || rectBtn[1].t < rectThumb.b)
			{
				rectThumb.SetRectEmpty();
			}
		}

		if (rectBtn[0].PtInRect (point))
		{
			hit = bHorz ? CBCGPScrollBar::BCGP_HT_BUTTON_LEFT : CBCGPScrollBar::BCGP_HT_BUTTON_UP;
		}
		else
		{
			if (rectBtn[1].PtInRect (point))
			{
				hit = bHorz ? CBCGPScrollBar::BCGP_HT_BUTTON_RIGHT : CBCGPScrollBar::BCGP_HT_BUTTON_DOWN;
			}
			else if (!rectThumb.IsRectEmpty())
			{
				if (m_HitPressed == CBCGPScrollBar::BCGP_HT_THUMB)
				{
					ClientToScreen (&rectThumb);
				}

				if (!rectThumb.IsRectEmpty())
				{
					if (rectThumb.PtInRect (point))
					{
						hit = CBCGPScrollBar::BCGP_HT_THUMB;
					}
					else
					{
						if (bHorz)
						{
							if (point.x < rectThumb.l)
							{
								hit = CBCGPScrollBar::BCGP_HT_CLIENT_LEFT;
							}
							else
							{
								hit = CBCGPScrollBar::BCGP_HT_CLIENT_RIGHT;
							}
						}
						else
						{
							if (point.y < rectThumb.t)
							{
								hit = CBCGPScrollBar::BCGP_HT_CLIENT_UP;
							}
							else
							{
								hit = CBCGPScrollBar::BCGP_HT_CLIENT_DOWN;
							}
						}
					}
				}
			}
		}
	}

	return hit;
}

void CBCGPScrollBar::Process()
{
	CWnd* pWnd = GetParent();
	if (pWnd->GetSafeHwnd())
	{
		BOOL bProcess = FALSE;
		BOOL bHorz = (GetStyle() & SBS_VERT) == 0;

		UINT nMessage = bHorz ? WM_HSCROLL : WM_VSCROLL;
		WPARAM nWParam   = 0;
		LPARAM nLParam   = (LPARAM)GetSafeHwnd();

		if ((m_HitPressed == CBCGPScrollBar::BCGP_HT_BUTTON_LTUP ||
			 m_HitPressed == CBCGPScrollBar::BCGP_HT_BUTTON_RTDN) &&
			 m_HitPressed == m_HitTest)
		{
			BOOL bUp = m_HitPressed == CBCGPScrollBar::BCGP_HT_BUTTON_LTUP;

			if (bHorz)
			{
				nWParam = bUp ? SB_LINELEFT : SB_LINERIGHT;
			}
			else
			{
				nWParam = bUp ? SB_LINEUP : SB_LINEDOWN;
			}

			bProcess = TRUE;
		}
		else if ((m_HitPressed == CBCGPScrollBar::BCGP_HT_CLIENT_LTUP ||
				  m_HitPressed == CBCGPScrollBar::BCGP_HT_CLIENT_RTDN) &&
				  m_HitPressed == m_HitTest)
		{
			BOOL bUp = m_HitPressed == CBCGPScrollBar::BCGP_HT_CLIENT_LTUP;

			if (bHorz)
			{
				nWParam = bUp ? SB_PAGELEFT : SB_PAGERIGHT;
			}
			else
			{
				nWParam = bUp ? SB_PAGEUP : SB_PAGEDOWN;
			}

			bProcess = TRUE;
		}
		else if (m_HitPressed == CBCGPScrollBar::BCGP_HT_THUMB)
		{
		}

		if (bProcess)
		{
			pWnd->SendMessage (nMessage, nWParam, nLParam);
		}
	}
}

void CBCGPScrollBar::OnLButtonDown(UINT /*nFlags*/, CPoint point) 
{
	if (!CBCGPVisualManager::GetInstance()->IsOwnerDrawScrollBar() ||
		m_Style == BCGP_SBSTYLE_DEFAULT)
	{
		Default();
		return;
	}

	m_pLastCapture = SetCapture();

	ClientToScreen (&point);
	m_HitPressed = HitTest (point);

	RedrawWindow();

	if (m_HitPressed != CBCGPScrollBar::BCGP_HT_NOWHERE)
	{
		if (m_HitPressed != CBCGPScrollBar::BCGP_HT_THUMB)
		{
			Process();

			m_TimerDown = SetTimer (SB_TIMER_DOWN, 500, NULL);
		}
		else
		{
			m_ptTrackStart   = point;

			CRect rect;
			GetClientRect (rect);

			m_HitPressed = CBCGPScrollBar::BCGP_HT_NOWHERE;
			m_rectTrackThumbStart = GetThumbRect (rect, (GetStyle() & SBS_VERT) == 0);
			m_rectTrackThumb = m_rectTrackThumbStart;
			m_HitPressed = CBCGPScrollBar::BCGP_HT_THUMB;

			m_TrackPos = GetScrollPos();

			GetParent()->SendMessage ((GetStyle() & SB_VERT) == 0 ? WM_HSCROLL : WM_VSCROLL, 
					MAKEWPARAM (SB_THUMBTRACK, m_TrackPos), (LPARAM)GetSafeHwnd());

			RedrawWindow();
		}
	}
}

void CBCGPScrollBar::OnLButtonUp(UINT /*nFlags*/, CPoint /*point*/) 
{
	if (!CBCGPVisualManager::GetInstance()->IsOwnerDrawScrollBar() ||
		m_Style == BCGP_SBSTYLE_DEFAULT)
	{
		Default();
		return;
	}

	const BOOL bSendEvent = m_pLastCapture != NULL;

	OnCancelMode();

	CWnd* pWnd = GetParent();
	if (bSendEvent && pWnd->GetSafeHwnd() != NULL)
	{
		pWnd->SendMessage ((GetStyle() & SBS_VERT) == 0 ? WM_HSCROLL : WM_VSCROLL, 
			SB_ENDSCROLL, (LPARAM)GetSafeHwnd());
	}
}

void CBCGPScrollBar::OnLButtonDblClk(UINT /*nFlags*/, CPoint point) 
{
	if (!CBCGPVisualManager::GetInstance()->IsOwnerDrawScrollBar() ||
		m_Style == BCGP_SBSTYLE_DEFAULT)
	{
		Default();
	}
	else
	{
		ClientToScreen (&point);
		CBCGPScrollBar::BCGPSB_HITTEST hit = HitTest (point);
		if (hit == BCGP_HT_BUTTON_LTUP ||
			hit == BCGP_HT_BUTTON_RTDN)
		{
			GetParent()->PostMessage ((GetStyle() & SB_VERT) == 0 ? WM_HSCROLL : WM_VSCROLL, 
				(hit == BCGP_HT_BUTTON_LTUP ? SB_LINEUP : SB_LINEDOWN), (LPARAM)GetSafeHwnd());
		}
	}
}

void CBCGPScrollBar::OnRButtonUp(UINT /*nFlags*/, CPoint /*point*/)
{
	if (!CBCGPVisualManager::GetInstance()->IsOwnerDrawScrollBar() ||
		m_Style == BCGP_SBSTYLE_DEFAULT)
	{
		Default();
	}
}

void CBCGPScrollBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!CBCGPVisualManager::GetInstance()->IsOwnerDrawScrollBar() ||
		m_Style == BCGP_SBSTYLE_DEFAULT)
	{
		Default();
		return;
	}

	BOOL bDown = (nFlags & MK_LBUTTON) == MK_LBUTTON;

	if (!bDown)
	{
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

	CPoint ptWindow (point);
	ClientToScreen (&ptWindow);

	CBCGPScrollBar::BCGPSB_HITTEST hit = HitTest (ptWindow);
	if (m_HitTest != hit)
	{
		m_HitTest = hit;
		RedrawWindow();
	}

	if (bDown && m_HitPressed == CBCGPScrollBar::BCGP_HT_THUMB)
	{
		CRect rect;
		GetClientRect (rect);

		BOOL bHorz = (GetStyle() & SBS_VERT) == 0;

		CSize szClamp (0, 0);
		int nThumbLength = 0;

		if (bHorz)
		{
			szClamp.w = rect.l + ::GetSystemMetrics (SM_CXHSCROLL);
			szClamp.h = rect.r - szClamp.w - nThumbLength;
			nThumbLength = m_rectTrackThumbStart.Width();
		}
		else
		{
			szClamp.w = rect.t + ::GetSystemMetrics (SM_CYVSCROLL);
			szClamp.h = rect.b - szClamp.w;
			nThumbLength = m_rectTrackThumbStart.Height();
		}

		szClamp.h -= nThumbLength;

		CPoint ptOffset (ptWindow - m_ptTrackStart);
		CRect rectNew (m_rectTrackThumbStart);

		if (bHorz)
		{
			if (abs(ptOffset.y) < 150)
			{
				rectNew.OffsetRect (ptOffset.x, 0);

				if (rectNew.l < szClamp.w)
				{
					rectNew.l  = szClamp.w;
					rectNew.r = rectNew.l + nThumbLength;
				}
				else if (szClamp.h < rectNew.l)
				{
					rectNew.l  = szClamp.h;
					rectNew.r = rectNew.l + nThumbLength;
				}
			}
		}
		else
		{
			if (abs(ptOffset.x) < 150)
			{
				rectNew.OffsetRect (0, ptOffset.y);

				if (rectNew.t < szClamp.w)
				{
					rectNew.t    = szClamp.w;
					rectNew.b = rectNew.t + nThumbLength;
				}
				else if (szClamp.h < rectNew.t)
				{
					rectNew.t    = szClamp.h;
					rectNew.b = rectNew.t + nThumbLength;
				}
			}
		}

		if (rectNew != m_rectTrackThumb)
		{
			m_rectTrackThumb = rectNew;
			int nPosNew = PosFromThumb 
				((bHorz ? m_rectTrackThumb.l : m_rectTrackThumb.t) - szClamp.w, szClamp);

			if (m_TrackPos != nPosNew)
			{
				m_TrackPos = nPosNew;

				GetParent()->SendMessage (bHorz ? WM_HSCROLL : WM_VSCROLL, 
					MAKEWPARAM (SB_THUMBTRACK, m_TrackPos), (LPARAM)GetSafeHwnd());
			}

			RedrawWindow();
		}
	}
}

int CBCGPScrollBar::PosFromThumb (int thumb, const CSize& szClamp)
{
	SCROLLINFO si;
	si.cbSize = sizeof (SCROLLINFO);
	GetScrollInfo (&si, SIF_RANGE | SIF_PAGE);

	const int nRange1 = szClamp.h - szClamp.w;
	const int nMin    = si.nMin;
	const int nMax    = si.nMax - (si.nPage - 1);
	const int nRange2 = nMax - nMin;

	if (nRange2 < 0)
	{
		return 0;
	}

	int nPos = nMin + 
		(int)(((double)thumb) * ((double)nRange2) / (double)(nRange1) + 0.5);

	if (nPos < nMin)
	{
		nPos = nMin;
	}

	if (nMax < nPos)
	{
		nPos = nMax;
	}

	return nPos;
}

LRESULT CBCGPScrollBar::OnMouseLeave(WPARAM, LPARAM)
{
	if (!CBCGPVisualManager::GetInstance()->IsOwnerDrawScrollBar() ||
		m_Style == BCGP_SBSTYLE_DEFAULT)
	{
		return Default();
	}

	if (m_bTracked)// && m_HitPressed != CBCGPScrollBar::BCGP_HT_THUMB)
	{
		m_bTracked = FALSE;
		m_HitTest  = CBCGPScrollBar::BCGP_HT_NOWHERE;

		RedrawWindow();
	}

	return 0;
}


LRESULT CBCGPScrollBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (!CBCGPVisualManager::GetInstance()->IsOwnerDrawScrollBar() ||
		m_Style == BCGP_SBSTYLE_DEFAULT)
	{
		return CScrollBar::WindowProc (message, wParam, lParam);
	}

	LRESULT lRes = 0L;

	if (GetSafeHwnd() != NULL)
	{
		if (message == SBM_SETPOS || message == SBM_SETRANGE ||
			message == SBM_SETRANGEREDRAW || message == SBM_SETSCROLLINFO)
		{
			BOOL bRedraw = FALSE;

			if (m_HitPressed != CBCGPScrollBar::BCGP_HT_THUMB)
			{
				SCROLLINFO si;
				si.cbSize = sizeof (SCROLLINFO);
				GetScrollInfo (&si);

				if (message == SBM_SETPOS)
				{
					if (lParam == TRUE)
					{
						bRedraw = (int)wParam != si.nPos;
					}
				}
				else if (message == SBM_SETRANGEREDRAW)
				{
					bRedraw = (int)wParam != si.nMin || (int)lParam != si.nMax;
				}
				else if (message == SBM_SETSCROLLINFO)
				{
					LPSCROLLINFO lpSI = (LPSCROLLINFO)lParam;

					if (wParam == TRUE && lpSI != NULL)
					{
						bRedraw = ((si.fMask & SIF_PAGE) == SIF_PAGE && si.nPage != lpSI->nPage) ||
							((si.fMask & SIF_POS) == SIF_POS && si.nPos != lpSI->nPos) ||
							((si.fMask & SIF_RANGE) == SIF_RANGE && (si.nMin != lpSI->nMin || si.nMax != lpSI->nMax)) ||
							((si.fMask & SIF_TRACKPOS) == SIF_TRACKPOS && si.nTrackPos != lpSI->nTrackPos);
					}
				}
			}

			CWnd::WindowProc(WM_SETREDRAW, (WPARAM)FALSE, 0L);
			lRes = CScrollBar::WindowProc(message, wParam, lParam);
			CWnd::WindowProc(WM_SETREDRAW, (WPARAM)TRUE, 0L);
			
			if (bRedraw)
			{
				RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			}
		}
		else
		{
			lRes = CScrollBar::WindowProc(message, wParam, lParam);

			if (message == SBM_ENABLE_ARROWS && m_dwDisabledArrows != (DWORD)wParam && lRes)
			{
				m_dwDisabledArrows = (DWORD)wParam;
				RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			}

			if (m_HitPressed == CBCGPScrollBar::BCGP_HT_THUMB)
			{
				if (message == SBM_GETSCROLLINFO && lParam != NULL &&
					(((SCROLLINFO*)lParam)->fMask & SIF_TRACKPOS) == SIF_TRACKPOS && lRes)
				{
					((SCROLLINFO*)lParam)->nTrackPos = m_TrackPos;
				}
			}	
		}
	}

	return lRes;
}

BOOL CBCGPScrollBar::PreTranslateMessage (MSG* pMsg)
{
	if (CBCGPVisualManager::GetInstance()->IsOwnerDrawScrollBar() &&
		m_Style != BCGP_SBSTYLE_DEFAULT)
	{
		if (pMsg->message == WM_MOUSEMOVE && m_HitPressed != BCGP_HT_NOWHERE)
		{
			OnMouseMove ((UINT) pMsg->wParam, CPoint ((DWORD)pMsg->lParam));
			return TRUE;
		}
	}

	return CScrollBar::PreTranslateMessage (pMsg);
}

void CBCGPScrollBar::OnTimer(UINT_PTR nIDEvent) 
{
	if (!CBCGPVisualManager::GetInstance()->IsOwnerDrawScrollBar() ||
		m_Style == BCGP_SBSTYLE_DEFAULT)
	{
		Default();
		return;
	}

	if (nIDEvent == SB_TIMER_DOWN && m_TimerDown != 0)
	{
		m_TimerDown = 0;

		KillTimer (SB_TIMER_DOWN);

		m_Timer = SetTimer (SB_TIMER, 50, NULL);
	}
	else if (nIDEvent == SB_TIMER && m_Timer != 0)
	{
		Process();
	}
	else
	{
		CScrollBar::OnTimer(nIDEvent);
	}
}

BOOL CBCGPScrollBar::OnEraseBkgnd(CDC* /*pDC*/) 
{
	if (!CBCGPVisualManager::GetInstance()->IsOwnerDrawScrollBar() ||
		m_Style == BCGP_SBSTYLE_DEFAULT)
	{
		return (BOOL) Default();
	}

	return TRUE;
}

void CBCGPScrollBar::OnNcDestroy() 
{
	CScrollBar::OnNcDestroy();

	if (m_bAutoDestoyWindow)
	{
		delete this;
	}
}
