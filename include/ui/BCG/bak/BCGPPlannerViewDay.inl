
//
// BCGPPlannerViewDay.cpp: implementation of the CBCGPPlannerViewDay class.
//
//////////////////////////////////////////////////////////////////////

#include "BCGPPlannerViewDay.h"

#ifndef BCGP_EXCLUDE_PLANNER

#include "BCGPPlannerManagerCtrl.h"
#include "BCGPDrawManager.h"
#include "BCGPAppointmentStorage.h"

#ifndef _BCGPCALENDAR_STANDALONE
	#include "BCGPVisualManager.h"
	#define visualManager	CBCGPVisualManager::GetInstance()
#else
	#include "BCGPCalendarVisualManager.h"
	#define visualManager	CBCGPCalendarVisualManager::GetInstance()
#endif

#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CBCGPPlannerViewDay, CBCGPPlannerView)

CBCGPPlannerViewDay::CBCGPPlannerViewDay()
	: CBCGPPlannerView        ()
	, m_Timer                  (NULL)
	, m_rectTimeBar            (0, 0, 0, 0)
	, m_nHeaderHeight          (0)
	, m_nHeaderAllDayHeight    (0)
	, m_CurrentTime           ()
{
	m_DateEnd = m_DateStart + COleDateTimeSpan (0, 23, 59, 59);
}

CBCGPPlannerViewDay::~CBCGPPlannerViewDay()
{
	StopTimer (FALSE);
}

#ifdef _DEBUG
void CBCGPPlannerViewDay::AssertValid() const
{
	CBCGPPlannerView::AssertValid();
}

void CBCGPPlannerViewDay::Dump(CDumpContext& dc) const
{
	CBCGPPlannerView::Dump(dc);
}
#endif

BOOL CBCGPPlannerViewDay::IsCurrentTimeVisible() const
{
	COleDateTime curTime (COleDateTime::GetCurrentTime());

	return GetDateStart() <= curTime && curTime <= GetDateEnd();
}

void CBCGPPlannerViewDay::StartTimer (BOOL bRedraw)
{
	CBCGPPlannerManagerCtrl* pPlanner = GetPlanner();
	ASSERT_VALID (pPlanner);

	if (pPlanner->GetSafeHwnd() == NULL)
	{
		return;
	}

	if (m_Timer == NULL)
	{
		m_CurrentTime = COleDateTime::GetCurrentTime();

		m_Timer = pPlanner->SetTimer (BCGP_PLANNER_TIMER_EVENT, 60000, NULL);
	}

	if (bRedraw)
	{
		OnTimer(BCGP_PLANNER_TIMER_EVENT);
	}
}

void CBCGPPlannerViewDay::StopTimer (BOOL bRedraw)
{
	if (!bRedraw && m_Timer == NULL)
	{
		return;
	}

	CBCGPPlannerManagerCtrl* pPlanner = GetPlanner();
	ASSERT_VALID (pPlanner);

	if (pPlanner->GetSafeHwnd() == NULL)
	{
		return;
	}

	if (m_Timer != NULL)
	{
		pPlanner->KillTimer (BCGP_PLANNER_TIMER_EVENT);
		m_Timer = NULL;
	}

	if (bRedraw)
	{
		OnTimer(BCGP_PLANNER_TIMER_EVENT);
	}
}

void CBCGPPlannerViewDay::DrawAppointment (CDC* pDC, CBCGPAppointment* pApp, CBCGPAppointmentDrawStructEx* pDS)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pApp);

	if (pDC == NULL || pDC == NULL || pApp == NULL || pDS == NULL)
	{
		return;
	}

	CBCGPPlannerManagerCtrl* pPlanner = GetPlanner();
	ASSERT_VALID (pPlanner);

	const DWORD dwDrawFlags = pPlanner->GetDrawFlags();
	const BOOL bIsGradientFill = dwDrawFlags & BCGP_PLANNER_DRAW_APP_GRADIENT_FILL;
	const BOOL bIsRoundedCorners = dwDrawFlags & BCGP_PLANNER_DRAW_APP_ROUNDED_CORNERS;
	const BOOL bDrawDuration = (dwDrawFlags & BCGP_PLANNER_DRAW_APP_NO_DURATION) == 0;
	const BOOL bDrawDurationShape = (dwDrawFlags & BCGP_PLANNER_DRAW_APP_DURATION_SHAPE) == 
		BCGP_PLANNER_DRAW_APP_DURATION_SHAPE && bDrawDuration;
	const BOOL bIsOverrideSelection = (dwDrawFlags & BCGP_PLANNER_DRAW_APP_OVERRIDE_SELECTION) ==
		BCGP_PLANNER_DRAW_APP_OVERRIDE_SELECTION || bDrawDurationShape;
	
	const CRect& rectDS = pDS->GetRect();
	CRect rect   (rectDS);

	COleDateTime dtStart  (pApp->GetStart());
	COleDateTime dtFinish (pApp->GetFinish());

	CString      strStart  (pApp->m_strStart);
	CString      strFinish (pApp->m_strFinish);

	const BOOL bAlternative = pApp->IsAllDay() || pApp->IsMultiDay();
	const BOOL bDrawShadow  = IsDrawAppsShadow() && !bAlternative;
	const BOOL bEmpty       = (dtStart == dtFinish) && !bAlternative;
	const BOOL bSelected    = pApp->IsSelected();

	const BOOL bAllBoders   = pDS->GetBorder() == CBCGPAppointmentDrawStruct::BCGP_APPOINTMENT_DS_BORDER_ALL;

	BOOL bDrawTime     = TRUE;
	BOOL bDurationFull = FALSE;
	CRect rectDuration (0, 0, 0, 0);

	if (!bAlternative && bDrawDuration)
	{
		const BOOL bNonSingleDay = !IsOneDay (dtStart, dtFinish);

		if (bNonSingleDay)
		{
			if (pDS->GetBorder() == CBCGPAppointmentDrawStruct::BCGP_APPOINTMENT_DS_BORDER_START)
			{
				dtFinish  = COleDateTime (dtFinish.GetYear(), dtFinish.GetMonth(), dtFinish.GetDay(),
					0, 0, 0);
			}
			else if (pDS->GetBorder() == CBCGPAppointmentDrawStruct::BCGP_APPOINTMENT_DS_BORDER_FINISH)
			{
				dtStart  = COleDateTime (dtFinish.GetYear(), dtFinish.GetMonth(), dtFinish.GetDay(),
					0, 0, 0);
			}
		}

		rectDuration = rect;
		rectDuration.r = rectDuration.l + CBCGPPlannerViewDay::BCGP_PLANNER_DURATION_BAR_WIDTH;

		const int nTimeDelta = CBCGPPlannerView::GetTimeDeltaInMinuts 
			(GetTimeDelta());

		double dStart = 
			(dtStart.GetHour() * 60 + dtStart.GetMinute()) / (double)nTimeDelta;
		double dEnd = IsOneDay (dtStart, dtFinish)
			? (dtFinish.GetHour() * 60 + dtFinish.GetMinute()) / (double)nTimeDelta
			: 24 * 60 / (double)nTimeDelta;

		int nStart = (int)dStart;
		int nEnd   = (int)ceil(dEnd);

		if (!bEmpty)
		{
			double dDelta = (double)rectDuration.Height() / (double)(nEnd - nStart);

			rectDuration.t    += long((dStart - nStart) * dDelta);
			rectDuration.b -= long((nEnd - dEnd) * dDelta);
		}

		if (bNonSingleDay)
		{
			dtStart  = pApp->GetStart();
			dtFinish = pApp->GetFinish();

			dStart = 
				(dtStart.GetHour() * 60 + dtStart.GetMinute()) / (double)nTimeDelta;
			dEnd = IsOneDay (dtStart, dtFinish)
				? (dtFinish.GetHour() * 60 + dtFinish.GetMinute()) / (double)nTimeDelta
				: 24 * 60 / (double)nTimeDelta;

			nStart = (int)dStart;
			nEnd   = (int)ceil(dEnd);
		}

		bDrawTime = dStart != nStart || dEnd != nEnd;

		bDurationFull = !bEmpty && (rectDuration.IsRectEmpty() ||
				(rectDuration.t == rect.t && rectDuration.b == rect.b));
	}

	const int durationMinHeight = 3;

	CRgn rgn;
	CRect rectClip (0, 0, 0, 0);
	pDC->GetClipBox (rectClip);

	if (bIsRoundedCorners)
	{
		int left  = 0;
		int right = 0;

		if (bAlternative)
		{
			left  = 4;
			right = 4;

			if (!bAllBoders)
			{
				if ((pDS->GetBorder() & CBCGPAppointmentDrawStruct::BCGP_APPOINTMENT_DS_BORDER_START) == 0)
				{
					left = 0;
				}

				if ((pDS->GetBorder() & CBCGPAppointmentDrawStruct::BCGP_APPOINTMENT_DS_BORDER_FINISH) == 0)
				{
					right = 0;
				}
			}

			if (bSelected)
			{
				rect.InflateRect (0, 1);
			}
		}
		else
		{
			if (bSelected && !bIsOverrideSelection)
			{
				rect.InflateRect (0, CBCGPPlannerViewDay::BCGP_PLANNER_DURATION_BAR_WIDTH - 1);
			}
		}

		rect.DeflateRect (left, 0, right, 0);

		if (bAlternative)
		{
			rgn.CreateRoundRectRgn (rect.l, rect.t, rect.r + 1, rect.b + 1, 5, 5);

			if (!bAllBoders)
			{
				if ((pDS->GetBorder() & CBCGPAppointmentDrawStruct::BCGP_APPOINTMENT_DS_BORDER_START) == 0)
				{
					CRgn rgnTmp;
					rgnTmp.CreateRectRgn (rect.l, rect.t, rect.l + 3, rect.b);
					rgn.CombineRgn (&rgn, &rgnTmp, RGN_OR);
				}

				if ((pDS->GetBorder() & CBCGPAppointmentDrawStruct::BCGP_APPOINTMENT_DS_BORDER_FINISH) == 0)
				{
					CRgn rgnTmp;
					rgnTmp.CreateRectRgn (rect.r - 3, rect.t, rect.r, rect.b);
					rgn.CombineRgn (&rgn, &rgnTmp, RGN_OR);
				}
			}

			rect.DeflateRect (0, bSelected ? 1 : 0);
		}
		else
		{
			BOOL bDuration = bDrawDurationShape && !bDurationFull;

			int left = rect.l;
			if (bDuration)
			{
				left += CBCGPPlannerViewDay::BCGP_PLANNER_DURATION_BAR_WIDTH;
			}

			rgn.CreateRoundRectRgn (left, rect.t, rect.r + 1, rect.b + 1, 5, 5);

			if (!bAllBoders)
			{
				if ((pDS->GetBorder() & CBCGPAppointmentDrawStruct::BCGP_APPOINTMENT_DS_BORDER_START) == 0)
				{
					CRgn rgnTmp;
					rgnTmp.CreateRectRgn (rect.l, rect.t, rect.r, rect.t + 3);
					rgn.CombineRgn (&rgn, &rgnTmp, RGN_OR);
				}

				if ((pDS->GetBorder() & CBCGPAppointmentDrawStruct::BCGP_APPOINTMENT_DS_BORDER_FINISH) == 0)
				{
					CRgn rgnTmp;
					rgnTmp.CreateRectRgn (rect.l, rect.b - 4, rect.r, rect.b + 1);
					rgn.CombineRgn (&rgn, &rgnTmp, RGN_OR);
				}
			}

			if (bDuration)
			{
				CRgn rgnTmp;

				if (bEmpty)
				{
					IPOINT points[3];
					points[0].x = rectDuration.l;
					points[0].y = rectDuration.t;
					points[1].x = rectDuration.r + 3;
					points[1].y = rectDuration.t;
					points[2].x = rectDuration.r + 3;
					points[2].y = rectDuration.t + rectDuration.Width() + 3;

					rgnTmp.CreatePolygonRgn (points, 3, WINDING);
				}
				else
				{
					if (rectDuration.Height() < durationMinHeight)
					{
						rectDuration.b = rectDuration.t + durationMinHeight;
						if (rect.b < rectDuration.b)
						{
							rectDuration.b = rect.b;
							rectDuration.t    = rectDuration.b - durationMinHeight;
						}
					}

					rgnTmp.CreateRectRgn (rectDuration.l, 
						rectDuration.t, rectDuration.r + 4, 
						rectDuration.b);
				}

				rgn.CombineRgn (&rgn, &rgnTmp, RGN_OR);
			}

			if (bSelected && !bIsOverrideSelection)
			{
				rect.DeflateRect (0, CBCGPPlannerViewDay::BCGP_PLANNER_DURATION_BAR_WIDTH - 1);
			}
		}

		pDC->SelectClipRgn (&rgn, RGN_AND);
	}
	else
	{
		if (bAlternative)
		{
			rect.DeflateRect (4, 0);
		}
		else
		{
			if (bDrawDurationShape)
			{
				int left = rect.l;
				if (!bDurationFull)
				{
					left += CBCGPPlannerViewDay::BCGP_PLANNER_DURATION_BAR_WIDTH;
				}

				rgn.CreateRectRgn (left, rect.t, rect.r, rect.b);

				if (!bDurationFull)
				{
					CRgn rgnTmp;

					if (bEmpty)
					{
						IPOINT points[3];
						points[0].x = rectDuration.l;
						points[0].y = rectDuration.t;
						points[1].x = rectDuration.r;
						points[1].y = rectDuration.t;
						points[2].x = rectDuration.r;
						points[2].y = rectDuration.t + rectDuration.Width();

						rgnTmp.CreatePolygonRgn (points, 3, WINDING);
					}
					else
					{
						if (rectDuration.Height() < durationMinHeight)
						{
							rectDuration.b = rectDuration.t + durationMinHeight;
							if (rect.b < rectDuration.b)
							{
								rectDuration.b = rect.b;
								rectDuration.t    = rectDuration.b - durationMinHeight;
							}
						}

						rgnTmp.CreateRectRgn (rectDuration.l, 
							rectDuration.t, rectDuration.r + 1, 
							rectDuration.b);
					}

					rgn.CombineRgn (&rgn, &rgnTmp, RGN_OR);
				}

				pDC->SelectClipRgn (&rgn, RGN_AND);
			}
		}
	}

	COLORREF clrBack1   = pApp->GetBackgroundColor();
	COLORREF clrBack2   = clrBack1;
	COLORREF clrText    = pApp->GetForegroundColor();
	COLORREF clrFrame1  = globalData.clrWindowFrame;
	COLORREF clrFrame2  = clrFrame1;
	COLORREF clrTextOld = CLR_DEFAULT;

	visualManager->GetPlannerAppointmentColors (this, bSelected, !bAlternative, 
		dwDrawFlags, clrBack1, clrBack2, clrFrame1, clrFrame2, clrText);

	if (bDrawShadow && (!bSelected || bIsOverrideSelection))
	{
		CRect rt (rect);
		if (!globalData.bIsOSAlphaBlendingSupport)
		{
			rt.b++;
		}

		int nShadow = CBCGPAppointment::BCGP_PLANNER_SHADOW_DEPTH;

		BOOL bShadowDepth = bIsRoundedCorners || bDrawDurationShape;

		if (bShadowDepth)
		{
			pDC->OffsetClipRgn (nShadow, nShadow);
			rt.OffsetRect (-3, -3);

			nShadow += 3;
		}

		{
			CBCGPDrawManager dm (*pDC);
			dm.DrawShadow (rt, nShadow, 100, bShadowDepth ? 50 : 70);
		}

		if (bShadowDepth)
		{
			nShadow -= 3;
			pDC->OffsetClipRgn (-nShadow, -nShadow);
		}
	}

	if (bSelected && bAlternative && !bIsOverrideSelection)
	{
		CBrush brBack (clrBack1);
		pDC->FillRect (rect, &brBack);
	}
	else
	{
		if (bIsGradientFill)
		{
			CRect rt (rect);
			rt.DeflateRect (1, 1);

			CBCGPDrawManager dm (*pDC);
			dm.FillGradient (rt, clrBack1, clrBack2);
		}
		else
		{
			CBrush brBack (clrBack1);
			pDC->FillRect (rect, &brBack);
		}
	}

	if (bIsRoundedCorners && bAlternative)
	{
		rect.DeflateRect (1, 0);
	}

	if (!rectDuration.IsRectNull())
	{
		CRect rt (rect);
		rt.r = rectDuration.r;

		if (bDrawDurationShape)
		{
			if (pApp->GetDurationColor() != CLR_DEFAULT)
			{
				CBrush br (pApp->GetDurationColor());

				CRect rt1 (rectDuration);
				rt1.r += 1;
				pDC->FillRect (rt1, &br);
			}

			rect.l = rt.r + 1;
		}
		else
		{
			pDC->FillRect (rt, &globalData.brWindow);

			CBrush br (pApp->GetDurationColor() == CLR_DEFAULT
				? globalData.clrWindow
				: pApp->GetDurationColor());

			CPen penBlack (PS_SOLID, 0, clrFrame1);
			CPen* pOldPen = pDC->SelectObject (&penBlack);

			if (!bEmpty)
			{
				CRect rt1 (rectDuration);
				pDC->FillRect (rt1, &br);
				
				rt1.r++;
				pDC->Draw3dRect (rt1, clrFrame1, clrFrame1);
			}
			else
			{
				IPOINT points[3];
				points[0].x = rt.l;
				points[0].y = rt.t;
				points[1].x = rt.r;
				points[1].y = rt.t;
				points[2].x = rt.r;
				points[2].y = rt.t + rt.r - rt.l;

				CRgn rgnP;
				rgnP.CreatePolygonRgn (points, 3, WINDING);

				pDC->FillRgn (&rgnP, &br);

				pDC->MoveTo (points[0].x, points[0].y);
				pDC->LineTo (points[2].x, points[2].y);
			}

			pDC->MoveTo (rt.r, rt.t);
			pDC->LineTo (rt.r, rt.b);

			pDC->SelectObject (pOldPen);

			rect.l = rt.r + 1;
		}
	}

	clrTextOld = pDC->SetTextColor (clrText);

	BOOL bCancelDraw = FALSE;
	BOOL bToolTipNeeded = FALSE;

	CSize szClock (GetClockIconSize());

	if (bAlternative && !pApp->IsAllDay())
	{
		if ((IsDrawTimeAsIcons() || 
			 (dwDrawFlags & BCGP_PLANNER_DRAW_APP_NO_MULTIDAY_CLOCKS) == 0) && 
			szClock != CSize (0, 0))
		{
			int top = (rect.Height() - szClock.h) / 2;

			if (pDS->GetBorder() & CBCGPAppointmentDrawStruct::BCGP_APPOINTMENT_DS_BORDER_START)
			{
				if (dtStart.GetHour() != 0 || dtStart.GetMinute() != 0 ||
					dtStart.GetSecond() != 0)
				{
					DrawClockIcon (pDC, CPoint (rect.l + 1, rect.t + top), 
						dtStart);

					rect.l += szClock.w + 3;
				}
			}

			if (pDS->GetBorder() & CBCGPAppointmentDrawStruct::BCGP_APPOINTMENT_DS_BORDER_FINISH)
			{
				if (dtFinish.GetHour() != 0 || dtFinish.GetMinute() != 0 ||
					dtFinish.GetSecond() != 0)
				{
					DrawClockIcon (pDC, CPoint (rect.r - szClock.w - 1, rect.t + top), 
						dtFinish);

					rect.r -= (szClock.w + 3);
				}
			}
		}
		else
		{
			COLORREF clrTime = visualManager->GetPlannerAppointmentTimeColor(this, 
				bSelected, !bAlternative, dwDrawFlags);
			if (clrTime != CLR_DEFAULT)
			{
				pDC->SetTextColor (clrTime);
			}

			COleDateTime dt (2005, 1, 1, 23, 59, 0);

			CString strFormat (_T("%H"));
			strFormat += GetTimeSeparator() + _T("%M");
			if (!Is24Hours())
			{
				strFormat += _T("%p");
			}
			CString strDT (dt.Format (strFormat));
			strDT.MakeLower();

			CSize szSpace (pDC->GetTextExtent (_T(" ")));
			CSize szDT (pDC->GetTextExtent (strDT));

			if (rect.Width() >= (szDT.w + szSpace.w) * 3)
			{
				if (pDS->GetBorder() & CBCGPAppointmentDrawStruct::BCGP_APPOINTMENT_DS_BORDER_START)
				{
					if (dtStart.GetHour() != 0 || dtStart.GetMinute() != 0 ||
						dtStart.GetSecond() != 0)
					{
						CRect rectText (rect);
						rectText.DeflateRect (2, 0, 3, 1);
						rectText.r = rectText.l + szDT.w;

						pDC->DrawText (strStart, rectText, 
							DT_NOPREFIX | DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
						rect.l = rectText.r + szSpace.w;
					}
				}

				if (pDS->GetBorder() & CBCGPAppointmentDrawStruct::BCGP_APPOINTMENT_DS_BORDER_FINISH)
				{
					if (dtFinish.GetHour() != 0 || dtFinish.GetMinute() != 0 ||
						dtFinish.GetSecond() != 0)
					{
						CRect rectText (rect);
						rectText.DeflateRect (2, 0, 3, 1);
						rectText.l = rectText.r - szDT.w;

						pDC->DrawText (strFinish, rectText, 
							DT_NOPREFIX | DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
						rect.r = rectText.l - szSpace.w;
					}
				}
			}
			else
			{
				bToolTipNeeded = TRUE;
			}

			if (clrTime != CLR_DEFAULT)
			{
				pDC->SetTextColor (clrText);
			}
		}
	}

	CSize szImage (CBCGPPlannerManagerCtrl::GetImageSize());

	CDWordArray dwIcons;

	CRect rtIcons (rect);
	rtIcons.l += (bAlternative ? 1 : 4);

	if (szImage != CSize (0, 0))
	{
		if (pApp->m_RecurrenceClone && 
			(dwDrawFlags & BCGP_PLANNER_DRAW_APP_NO_RECURRENCE_IMAGE) == 0)
		{
			if (rtIcons.Width() > szImage.w * 1.5)
			{
				dwIcons.Add (pApp->m_RecurrenceEcp ? 1 : 0);
				rtIcons.l += szImage.w;
			}
		}

		if (!pApp->m_lstImages.IsEmpty() && 
			(dwDrawFlags & BCGP_PLANNER_DRAW_APP_NO_IMAGES) == 0)
		{
			for (POSITION pos = pApp->m_lstImages.GetHeadPosition(); pos != NULL;)
			{
				if (rtIcons.Width() <= szImage.w)
				{
					bCancelDraw = TRUE;
					break;
				}

				rtIcons.l += szImage.w;

				dwIcons.Add (pApp->m_lstImages.GetNext (pos));
			}
		}
	}

	BOOL bDrawText = !pApp->m_strDescription.IsEmpty() && !bCancelDraw;

	CSize szText (0, 0);
	if (bDrawText)
	{
		szText = pDC->GetTextExtent (pApp->m_strDescription);
	}

	if (dwIcons.GetSize() > 0)
	{
		long nIconsWidth = rtIcons.l - rect.l;
		
		CPoint pt (rect.l + (bAlternative ? 1 : 4), 
			rect.t + (bAlternative ? (rect.Height() - szImage.h) / 2 : 1));

		if (bAlternative)
		{
			nIconsWidth += szText.w;

			if (nIconsWidth < rect.Width())
			{
				pt.x += (rect.Width() - nIconsWidth) / 2;
			}
		}

		for (int i = 0; i < dwIcons.GetSize(); i++)
		{
			CBCGPPlannerManagerCtrl::DrawImageIcon (pDC, pt, dwIcons[i]);

			pt.x += szImage.w;
		}

		rect.l = pt.x;
	}

	if (!bCancelDraw && !bAlternative)
	{
		CSize szClock (GetClockIconSize());

		if (IsDrawTimeAsIcons() && szClock != CSize (0, 0))
		{
			if (rect.Width() >= szClock.w * 3)
			{
				DrawClockIcon (pDC, CPoint (rect.l + 1, rect.t + 1), 
					dtStart);

				rect.l += szClock.w + 3;

				if (rect.Width() >= szClock.w * 4 && IsDrawTimeFinish())
				{
					DrawClockIcon (pDC, CPoint (rect.l + 1, rect.t + 1), 
						dtFinish);

					rect.l += szClock.w + 3;
				}
			}
		}
		else
		{
			if (bDrawTime)
			{
				CSize szSpace (pDC->GetTextExtent (_T(" ")));

				CString str;

				if (!strStart.IsEmpty())
				{
					str += strStart;

					if (!strFinish.IsEmpty() && !bEmpty)
					{
						str += _T("-") + strFinish;
					}
				}

				if (!str.IsEmpty())
				{
					COLORREF clrTime = visualManager->GetPlannerAppointmentTimeColor(this, 
						bSelected, !bAlternative, dwDrawFlags);
					if (clrTime != CLR_DEFAULT)
					{
						pDC->SetTextColor (clrTime);
					}

					CSize sz (pDC->GetTextExtent (str));

					CRect rectText (rect);
					rectText.DeflateRect (4, 1, 1, 0);

					pDC->DrawText (str, rectText, DT_NOPREFIX);	

					rect.l += sz.w + szSpace.w;

					if (clrTime != CLR_DEFAULT)
					{
						pDC->SetTextColor (clrText);
					}
				}
			}
		}
	}

	if (bDrawText)
	{
		CRect rectText (rect);

		if (bAlternative && dwIcons.GetSize() == 0 && szText.w < rectText.Width())
		{
			rectText.l += (rectText.Width() - szText.w) / 2;
		}
		else
		{
			// if time drawed and description not completely in view, then
			// move description down (if possible)
			if (bDrawTime)
			{
				if ((szText.w + 4) > rect.Width() && rect.Height() > szText.h * 2)
				{
					bCancelDraw = FALSE;
					rectText.l = rectDS.l;
					if (bDrawDuration)
					{
						rectText.l += CBCGPPlannerViewDay::BCGP_PLANNER_DURATION_BAR_WIDTH + 1;
					}
					rectText.OffsetRect (0, szText.h);
				}
			}
		}

		if (!bCancelDraw)
		{
			if (bAlternative)
			{
				rectText.DeflateRect (4, 0, 1, 1);
				pDC->DrawText (pApp->m_strDescription, rectText, 
					DT_NOPREFIX | DT_VCENTER | DT_SINGLELINE);
			}
			else
			{
				rectText.DeflateRect (4, 1, 1, 0);
				pDC->DrawText (pApp->m_strDescription, rectText, DT_NOPREFIX);
			}
		}

		if (rectText.Width() < szText.w)
		{
			bToolTipNeeded = TRUE;
		}
	}

	pDC->SetTextColor (clrTextOld);

	rect.l = rectDS.l;
	if (bAlternative)
	{
		rect.r = rectDS.r;
		rect.DeflateRect (4, 0);
	}

	CRect rectEdit (rect);

	if (!bAlternative)
	{
		rectEdit.l = rectDS.l + 1;
		if (bDrawDuration && (!bDrawDurationShape || !bDurationFull))
		{
			rectEdit.l += CBCGPPlannerViewDay::BCGP_PLANNER_DURATION_BAR_WIDTH;
			if (!bIsOverrideSelection)
			{
				rectEdit.l++;
			}
		}	

		if (bIsRoundedCorners || bIsOverrideSelection)
		{
			rectEdit.DeflateRect (0, 1, 1, 1);
		}

		rectEdit.IntersectRect (rectEdit, m_rectApps);
	}
	else
	{
		if (bIsRoundedCorners)
		{
			rectEdit.DeflateRect (1, 0);

			if ((pDS->GetBorder() & CBCGPAppointmentDrawStruct::BCGP_APPOINTMENT_DS_BORDER_START) == 0)
			{
				rectEdit.l = rectDS.l + 1;
			}

			if ((pDS->GetBorder() & CBCGPAppointmentDrawStruct::BCGP_APPOINTMENT_DS_BORDER_FINISH) == 0)
			{
				rectEdit.r = rectDS.r - 1;
			}
		}
		else
		{
			if ((pDS->GetBorder() & CBCGPAppointmentDrawStruct::BCGP_APPOINTMENT_DS_BORDER_START) == 0)
			{
				rectEdit.l--;
			}

			if ((pDS->GetBorder() & CBCGPAppointmentDrawStruct::BCGP_APPOINTMENT_DS_BORDER_FINISH) == 0)
			{
				rectEdit.r++;
			}

			rectEdit.t--;
			rectEdit.b++;
		}
	}

	if (rectEdit.t == m_rectApps.t)
	{
		rectEdit.DeflateRect (1, 0, 1, 1);
	}
	else
	{
		rectEdit.DeflateRect (1, 1);
	}

	pDS->SetRectEdit (rectEdit);

	if (bAlternative)
	{
		rectEdit.DeflateRect (0, 1);
	}
	pDS->SetRectEditHitTest (rectEdit);

	if (bAlternative)
	{
		if (!bIsRoundedCorners)
		{
			CPen pen (PS_SOLID, 0, clrFrame2);
			CPen* pOldPen = pDC->SelectObject (&pen);

			if(pDS->GetBorder() == CBCGPAppointmentDrawStruct::BCGP_APPOINTMENT_DS_BORDER_ALL)
			{
				pDC->Draw3dRect (rect, clrFrame2, clrFrame2);
			}
			else
			{
				pDC->MoveTo (rect.l , rect.t);
				pDC->LineTo (rect.r, rect.t);

				pDC->MoveTo (rect.l , rect.b - 1);
				pDC->LineTo (rect.r, rect.b - 1);

				if (pDS->GetBorder() & CBCGPAppointmentDrawStruct::BCGP_APPOINTMENT_DS_BORDER_START)
				{
					pDC->MoveTo (rect.l, rect.t);
					pDC->LineTo (rect.l, rect.b);
				}

				if (pDS->GetBorder() & CBCGPAppointmentDrawStruct::BCGP_APPOINTMENT_DS_BORDER_FINISH)
				{
					pDC->MoveTo (rect.r - 1, rect.t);
					pDC->LineTo (rect.r - 1, rect.b);
				}
			}

			if (bSelected)
			{
				CPoint pt (rect.l, rect.r);

				if ((pDS->GetBorder() & CBCGPAppointmentDrawStruct::BCGP_APPOINTMENT_DS_BORDER_START) == 0)
				{
					pt.x--;
				}

				if ((pDS->GetBorder() & CBCGPAppointmentDrawStruct::BCGP_APPOINTMENT_DS_BORDER_FINISH) == 0)
				{
					pt.y++;
				}

				pDC->MoveTo (pt.x, rect.t - 1);
				pDC->LineTo (pt.y, rect.t - 1);

				pDC->MoveTo (pt.x, rect.b);
				pDC->LineTo (pt.y, rect.b);
			}

			pDC->SelectObject (pOldPen);
		}
	}
	else
	{
		if (bSelected && bIsOverrideSelection)
		{
			pDC->Draw3dRect (rect, clrFrame2, clrFrame2);
		}
		else
		{
			pDC->Draw3dRect (rect, clrFrame1, clrFrame1);
		}

		if (bSelected)
		{
			if (!bIsOverrideSelection)
			{
				CRect rt (rect);
				rt.InflateRect (0, CBCGPPlannerViewDay::BCGP_PLANNER_DURATION_BAR_WIDTH - 1);

				CBrush br (pApp->GetDurationColor() == CLR_DEFAULT 
					? globalData.clrWindow
					: pApp->GetDurationColor());

				rt.DeflateRect (1, 1, 1, 0);
				rt.b = rt.t + CBCGPPlannerViewDay::BCGP_PLANNER_DURATION_BAR_WIDTH - 2;

				if (pDS->GetBorder() & CBCGPAppointmentDrawStruct::BCGP_APPOINTMENT_DS_BORDER_START)
				{
					pDC->FillRect (rt, &br);
					rect.t -= CBCGPPlannerViewDay::BCGP_PLANNER_DURATION_BAR_WIDTH - 1;
				}

				if (pDS->GetBorder() & CBCGPAppointmentDrawStruct::BCGP_APPOINTMENT_DS_BORDER_FINISH)
				{
					rt.OffsetRect (0, rectDS.Height() + rt.Height());
					pDC->FillRect (rt, &br);
					rect.b += CBCGPPlannerViewDay::BCGP_PLANNER_DURATION_BAR_WIDTH - 1;
				}
			}

			if (!bIsRoundedCorners)
			{
				CRect rt (rect);
				if (bIsOverrideSelection)
				{
					rt.DeflateRect (1, 1);
				}

				pDC->Draw3dRect (rt, clrFrame2, clrFrame2);
			}
		}
	}

	if (bIsRoundedCorners || bDrawDurationShape)
	{
		CRgn rgnClip;
		rgnClip.CreateRectRgn (rectClip.l, rectClip.t, rectClip.r, rectClip.b);

		pDC->SelectClipRgn (&rgnClip, RGN_COPY);

		int nWidth = 1;

		if (bSelected)
		{
			if (bIsOverrideSelection || bAlternative)
			{
				nWidth = 2;
			}
		}

		CBrush br (clrFrame2);
		pDC->FrameRgn (&rgn, &br, nWidth, nWidth);
	}

	if (bCancelDraw)
	{
		bToolTipNeeded = TRUE;
	}

	pDS->SetToolTipNeeded (bToolTipNeeded);
}

void CBCGPPlannerViewDay::OnDrawAppointmentsDuration (CDC* pDC)
{
	if ((GetPlanner()->GetDrawFlags() & BCGP_PLANNER_DRAW_VIEW_NO_DURATION) == 
			BCGP_PLANNER_DRAW_VIEW_NO_DURATION)
	{
		return;
	}

	XBCGPAppointmentArray& arQueryApps = GetQueryedAppointments();
	XBCGPAppointmentArray& arDragApps  = GetDragedAppointments();

	if (arQueryApps.GetSize() == 0 && arDragApps.GetSize() == 0)
	{
		return;
	}

	BOOL bDragDrop        = IsDragDrop();
	DROPEFFECT dragEffect = GetDragEffect();
	BOOL bDragMatch       = IsCaptureMatched();

	COleDateTime dtS = GetDateStart();
	COleDateTime dtE = GetDateEnd();

	const int nMinuts = CBCGPPlannerView::GetTimeDeltaInMinuts (GetTimeDelta());
	const int nCount = 24 * 60 / nMinuts;
	const int yOffset = m_nScrollOffset * m_nRowHeight;

	for (int nApp = 0; nApp < 2; nApp++)
	{
		XBCGPAppointmentArray& arApps = nApp == 1 ? arDragApps : arQueryApps;

		if (nApp == 1)
		{
			bDragDrop = bDragDrop && arDragApps.GetSize();
		}

		if (arApps.GetSize() == 0)
		{
			continue;
		}

		for (int i = 0; i < arApps.GetSize(); i++)
		{
			CBCGPAppointment* pApp = arApps [i];
			if (pApp == NULL || !(pApp->IsAllDay() || pApp->IsMultiDay()) || 
				pApp->GetDurationColor() == CLR_DEFAULT)
			{
				continue;
			}

			BOOL bDraw = FALSE;

			if (bDragDrop && dragEffect != DROPEFFECT_NONE && 
				pApp->IsSelected() && nApp == 0)
			{
				if ((dragEffect & DROPEFFECT_COPY) == DROPEFFECT_COPY || bDragMatch)
				{
					bDraw = TRUE;
				}
			}
			else
			{
				bDraw = TRUE;
			}

			if(!bDraw)
			{
				continue;
			}

			COleDateTime dtStart  = pApp->GetStart();
			COleDateTime dtFinish = pApp->GetFinish();

			dtStart.SetDate (dtStart.GetYear(), dtStart.GetMonth(), dtStart.GetDay());
			dtFinish.SetDate (dtFinish.GetYear(), dtFinish.GetMonth(), dtFinish.GetDay());

			BOOL bStart = FALSE;
			BOOL bEnd   = FALSE;

			if (dtStart < dtS)
			{
				dtStart = dtS;
				bStart  = TRUE;
			}

			if (dtE < dtFinish)
			{
				dtFinish = dtE;
				bEnd     = TRUE;
			}

			COleDateTimeSpan span (dtFinish - dtStart);

			int nStart = (dtStart - dtS).GetDays();
			int nEnd   = min(nStart + span.GetDays() + 1, GetViewDuration());

			CBrush br (pApp->GetDurationColor() == CLR_DEFAULT
				? globalData.clrWindow
				: pApp->GetDurationColor());

			for(int i = nStart; i < nEnd; i++)
			{
				CRect rt (m_ViewRects[i]);

				rt.r  = rt.l + 
					CBCGPPlannerViewDay::BCGP_PLANNER_DURATION_BAR_WIDTH + 1;		
				rt.l  -= (i == 0) ? 1 : 0;
				rt.t   -= 1;
				rt.b = rt.t + nCount * m_nRowHeight;
				rt.DeflateRect (1, 0);
				
				if (!pApp->IsAllDay())
				{
					rt.OffsetRect (0, -yOffset);

					if (i == (nEnd - 1) && !bEnd)
					{
						dtFinish = pApp->GetFinish();
						const double dDelta = (dtFinish.GetHour() * 60 + dtFinish.GetMinute()) / (double)nMinuts;

						rt.b = rt.t + CBCGPPlannerView::round(dDelta * m_nRowHeight);
					}

					if (i == nStart && !bStart)
					{
						dtStart  = pApp->GetStart();
						const double dDelta = 
							(dtStart.GetHour() * 60 + dtStart.GetMinute()) / (double)nMinuts;

						rt.t += CBCGPPlannerView::round(dDelta * m_nRowHeight);
					}

					rt.IntersectRect (rt, m_ViewRects[i]);
				}

				pDC->FillRect (rt, &br);
			}
		}
	}
}

void CBCGPPlannerViewDay::GetCaptionFormatStrings (CStringArray& sa)
{
	sa.RemoveAll();

	BOOL bCompact = (GetPlanner()->GetDrawFlags() & BCGP_PLANNER_DRAW_VIEW_CAPTION_DAY_COMPACT) ==
			BCGP_PLANNER_DRAW_VIEW_CAPTION_DAY_COMPACT;

	if (!bCompact)
	{
		CString strSep (CBCGPPlannerView::GetDateSeparator());

		if (CBCGPPlannerView::IsDateBeforeMonth())
		{
			sa.Add (_T("dddd d MMMM"));
			sa.Add (_T("dddd d MMM"));
			sa.Add (_T("ddd d MMM"));
			sa.Add (_T("d") + strSep + _T("M"));
		}
		else
		{
			sa.Add (_T("dddd MMMM d"));
			sa.Add (_T("dddd MMM d"));
			sa.Add (_T("ddd MMM d"));
			sa.Add (_T("M") + strSep + _T("d"));
		}
	}
	else
	{
		sa.Add (_T("d\ndddd"));
		sa.Add (_T("d\nddd"));
		sa.Add (_T("d"));
	}
}

void CBCGPPlannerViewDay::AdjustScrollSizes()
{
	long nCount = 24 * 60 / CBCGPPlannerView::GetTimeDeltaInMinuts (GetTimeDelta());

	m_nScrollPage  = m_rectApps.Height() / m_nRowHeight;
	m_nScrollTotal = nCount - 1;

	m_nScrollOffset = min (m_nScrollOffset, m_nScrollTotal - m_nScrollPage + 1);

	CBCGPPlannerView::AdjustScrollSizes();
}

void CBCGPPlannerViewDay::AdjustLayout (CDC* /*pDC*/, const CRect& rectClient)
{
	if (IsCurrentTimeVisible())
	{
		StartTimer (FALSE);
	}
	else
	{
		StopTimer (FALSE);
	}

	m_nHeaderHeight       = 1;
	m_nHeaderAllDayHeight = 1;

	const int nMinuts = CBCGPPlannerView::GetTimeDeltaInMinuts (GetTimeDelta());
	const int nCount = 24 * 60 / nMinuts;

	m_rectTimeBar = rectClient;
	m_rectTimeBar.r = m_rectTimeBar.l + m_nRowHeight * (nCount == 24 ? 2 : 3) + 5;

	m_rectApps.l = m_rectTimeBar.r;

	{
		// finding allday or multiday events
		const int nDays = GetViewDuration();

		CDWordArray arDays;
		arDays.SetSize (nDays);

		COleDateTime dtS = GetDateStart();

		XBCGPAppointmentArray& arQueryApps = GetQueryedAppointments();
		XBCGPAppointmentArray& arDragApps  = GetDragedAppointments();

		BOOL bDragDrop        = IsDragDrop();
		DROPEFFECT dragEffect = GetDragEffect();
		BOOL bDragMatch       = IsCaptureMatched();

		bDragDrop = !bDragDrop || 
			(bDragDrop && ((dragEffect & DROPEFFECT_COPY) == DROPEFFECT_COPY && bDragMatch) || 
			!bDragMatch);
		bDragDrop = bDragDrop && arDragApps.GetSize();

		for (int nApp = 0; nApp < 2; nApp++)
		{
			if (!bDragDrop && nApp == 0)
			{
				continue;
			}

			XBCGPAppointmentArray& arApps = nApp == 0 ? arDragApps : arQueryApps;

			for (int i = 0; i < arApps.GetSize(); i++)
			{
				const CBCGPAppointment* pApp = arApps[i];
				if (pApp == NULL)
				{
					continue;
				}

				ASSERT_VALID (pApp);

				if (pApp->IsAllDay() || pApp->IsMultiDay())
				{
					if (nDays > 1)
					{
						COleDateTime dtStart  = pApp->GetStart();
						COleDateTime dtFinish = pApp->GetFinish();

						dtStart = COleDateTime (dtStart.GetYear(), dtStart.GetMonth(), dtStart.GetDay(),
							0, 0, 0);

						if (pApp->IsAllDay())
						{
							dtFinish += COleDateTimeSpan (1, 0, 0, 0);
						}
						else if (pApp->GetFinish().GetHour() != 0 ||
							     pApp->GetFinish().GetMinute() != 0)
						{
							dtFinish = COleDateTime (dtFinish.GetYear(), dtFinish.GetMonth(), dtFinish.GetDay(),
								0, 0, 0);
							dtFinish += COleDateTimeSpan (1, 0, 0, 0);
						}

						if (dtStart < dtS)
						{
							dtStart = dtS;
						}

						COleDateTimeSpan span (dtFinish - dtStart);

						int nStart = (dtStart - dtS).GetDays();
						int nEnd   = nStart + span.GetDays();

						if (nEnd > (int) nDays)
						{
							nEnd = (int) nDays;
						}

						for (int j = nStart; j < nEnd; j++)
						{
							arDays[j] = arDays[j] + 1;
						}
					}
					else
					{
						arDays[0] = arDays[0] + 1;
					}
				}
			}
		}

		DWORD maxCount = 0;
		for (int i = 0; i < nDays; i++)
		{
			if (maxCount < arDays[i])
			{
				maxCount = arDays[i];
			}
		}

		if (maxCount > 0)
		{
			m_nHeaderAllDayHeight = maxCount;
		}
	}

	int nRow = rectClient.Height() / 
		(nCount + m_nHeaderHeight + m_nHeaderAllDayHeight);

	int nOldRowHeight = m_nRowHeight;

	if (nRow > m_nRowHeight)
	{
		m_nRowHeight = nRow;
	}

	m_nHeaderHeight       *= m_nRowHeight;
	m_nHeaderAllDayHeight *= (m_nRowHeight + 2);

	m_rectApps.t += m_nHeaderHeight;

	nRow = (m_rectApps.Height() - m_nHeaderAllDayHeight) / nCount;

	if (nRow > nOldRowHeight)
	{
		m_nRowHeight = nRow;
	}

	int delta = m_rectApps.Height() - m_nHeaderAllDayHeight;

	if (delta < 0)
	{
		m_nHeaderAllDayHeight = m_rectApps.Height();
	}
	else
	{
		int nc = (int)(delta / m_nRowHeight);

		if (nc >= nCount)
		{
			m_nHeaderAllDayHeight = m_rectApps.Height() - nCount * m_nRowHeight;
		}
		else
		{
			m_nHeaderAllDayHeight += delta - nc * m_nRowHeight;
		}
	}

	m_rectApps.t += m_nHeaderAllDayHeight;

	AdjustScrollSizes();
	
	// correct selection
	COleDateTime sel1 (GetSelectionStart());
	COleDateTime sel2 (GetSelectionEnd());

	SetSelection (sel1, sel2, FALSE);
}

void CBCGPPlannerViewDay::AdjustRects()
{
	const int nDays = GetViewDuration();
	const int dxColumn = CBCGPPlannerView::round (m_rectApps.Width() / (double)nDays);

	CRect rect (m_rectApps);
	rect.r = rect.l + dxColumn;

	m_SelectionAllDay.RemoveAll();

	m_ViewRects.SetSize (nDays);
	m_SelectionAllDay.SetSize (nDays);

	for (int nDay = 0; nDay < nDays; nDay++)
	{
		m_ViewRects[nDay] = rect;
		m_SelectionAllDay[nDay] = FALSE;

		rect.OffsetRect (dxColumn, 0);

		if (nDay == (nDays - 2))
		{
			rect.r = m_rectApps.r;
		}
	}
}

void CBCGPPlannerViewDay::AdjustAppointments()
{
	XBCGPAppointmentArray& arQueryApps = GetQueryedAppointments();
	XBCGPAppointmentArray& arDragApps = GetDragedAppointments();

	const int nDays = GetViewDuration();

	if ((arQueryApps.GetSize() == 0 && arDragApps.GetSize() == 0) || 
		m_ViewRects.GetSize() != nDays)
	{
		ClearVisibleUpDownIcons();
		return;
	}

	BOOL bDrawShadow = IsDrawAppsShadow();

	BOOL bDragDrop        = IsDragDrop();
	DROPEFFECT dragEffect = GetDragEffect();
	BOOL bDragMatch       = IsCaptureMatched();

	bDragDrop = !bDragDrop || 
		(bDragDrop && ((dragEffect & DROPEFFECT_COPY) == DROPEFFECT_COPY && bDragMatch) || 
		!bDragMatch);
	bDragDrop = bDragDrop && arDragApps.GetSize();

	const int nTimeDelta = CBCGPPlannerView::GetTimeDeltaInMinuts (GetTimeDelta());
	const int yOffset = m_nScrollOffset * m_nRowHeight;

	COleDateTime date (m_DateStart);

	for (int nApp = 0; nApp < 2; nApp++)
	{
		if (!bDragDrop && nApp == 0)
		{
			continue;
		}

		XBCGPAppointmentArray& arApps = nApp == 0 ? arDragApps : arQueryApps;

		for (int i = 0; i < arApps.GetSize(); i++)
		{
			CBCGPAppointment* pApp = arApps[i];
			ASSERT_VALID (pApp);

			pApp->ResetDraw();
		}
	}

	COleDateTimeSpan spanDay (1, 0, 0, 0);

	XBCGPAppointmentArray arAllDays;

	int nDay = 0;

	const BOOL bNoDuration = 
		(GetPlanner()->GetDrawFlags() & BCGP_PLANNER_DRAW_VIEW_NO_DURATION) == 
		BCGP_PLANNER_DRAW_VIEW_NO_DURATION;

	for (nDay = 0; nDay < nDays; nDay ++)
	{
		for (int nApp = 0; nApp < 2; nApp++)
		{
			if (!bDragDrop && nApp == 1)
			{
				continue;
			}

			XBCGPAppointmentArray& arApps = nApp == 1 ? arDragApps : arQueryApps;

			XBCGPAppointmentArray arByDate;

			int i;

			CRect rectFill (m_ViewRects [nDay]);
			rectFill.t -= yOffset;

			if (bNoDuration && nDay > 0)
			{
				rectFill.l++;
			}

			for (i = 0; i < arApps.GetSize(); i++)
			{
				CBCGPAppointment* pApp = arApps[i];
				ASSERT_VALID (pApp);

				if (CBCGPPlannerView::IsAppointmentInDate (*pApp, date))
				{
					CRect rtApp (rectFill);

					// trimming top and bottom edge, starting from TimeDelta and Appointment interval
					rtApp.t = rectFill.t + m_nRowHeight * 
						long((pApp->GetStart().GetHour() * 60 + 
							 pApp->GetStart().GetMinute()) / nTimeDelta) - 1;

					if (pApp->IsAllDay() || pApp->IsMultiDay())
					{
						BOOL bAdd = TRUE;

						for (int i = 0; i < arAllDays.GetSize(); i++)
						{
							if (arAllDays[i] == pApp)
							{
								bAdd = FALSE;
								break;
							}
						}

						if (bAdd)
						{
							arAllDays.Add (pApp);
						}
					}
					else
					{
						rtApp.r -= 2 * BCGP_PLANNER_APPOINTMENT_SPACE;

						if (pApp->GetStart() != pApp->GetFinish())
						{
							COleDateTime dtAF (pApp->GetFinish());
							int minutes = dtAF.GetHour() * 60 + dtAF.GetMinute();

							if (!IsOneDay (pApp->GetStart(), dtAF))
							{
								BOOL bStartDay   = IsOneDay (date, pApp->GetStart());
								BOOL bFinishNULL = dtAF.GetHour() == 0 && 
												   dtAF.GetMinute() == 0;

								if (bStartDay)
								{
									minutes = 24 * 60;
								}

								if (bFinishNULL)
								{
									if (!bStartDay)
									{
										if (pApp->GetDSDraw().GetCount() == 1)
										{
											pApp->GetDSDraw()[0].m_date2 = date;
											continue;
										}
									}
								}
								else
								{
									if (!bStartDay)
									{
										rtApp.t = rectFill.t - 1;
									}
								}
							}

							rtApp.b = rectFill.t + m_nRowHeight * 
								(long)ceil(minutes / (double)nTimeDelta) - 1;
						}
						else
						{
							rtApp.b = rtApp.t + m_nRowHeight;
						}

						pApp->SetRectDraw (rtApp, date);
						arByDate.Add (pApp);
					}
				}
			}

			// resort appointments in the view, if count of collection is great than 1
			if (arByDate.GetSize() > 1)
			{
				XBCGPAppointmentArray* ar = new XBCGPAppointmentArray;

				// array, that contains columns
				CArray<XBCGPAppointmentArray*, XBCGPAppointmentArray*> arColumns;
				arColumns.Add (ar);

				// initialize first column
				ar->Copy (arByDate);

				while (ar != NULL)
				{
					CBCGPAppointment* pApp1 = ar->GetAt (0);

					XBCGPAppointmentArray* arNew = NULL;

					i = 1;

					// remove appointments, that have collisions in rects, from previous column 
					// to the next column
					while (i < ar->GetSize())
					{
						CBCGPAppointment* pApp2 = ar->GetAt (i);

						CRect rtInter;
						if (rtInter.IntersectRect (pApp1->GetRectDraw (date), pApp2->GetRectDraw (date)))
						{
							if (arNew == NULL)
							{
								// add a new column
								arNew = new XBCGPAppointmentArray;
								arColumns.Add (arNew);
							}

							arNew->Add (pApp2);
							ar->RemoveAt (i);
						}
						else
						{
							pApp1 = pApp2;
							i++;
						}
					}

					ar = arNew;
				}

				int nCount = (int) arColumns.GetSize();

				// reinitialize drawing rects, if found great than 1 columns
				if (nCount > 1)
				{
					int nWidth = rectFill.Width() / nCount;
					int nL = rectFill.l;

					// left border of appointments, based on column order
					for (i = 0; i < nCount; i++)
					{
						ar = arColumns[i];

						for (int j = 0; j < ar->GetSize(); j++)
						{
							CBCGPAppointment* pApp = ar->GetAt (j);
							ASSERT_VALID (pApp);

							CRect rtApp (pApp->GetRectDraw (date));
							rtApp.l = nL;

							pApp->SetRectDraw (rtApp, date);
						}

						nL += nWidth;
					}

					// correcting right border of appointments
					for (i = 0; i < nCount; i++)
					{
						ar = arColumns[i];

						for (int j = 0; j < ar->GetSize(); j++)
						{
							CBCGPAppointment* pApp = ar->GetAt (j);
							CRect rtApp (pApp->GetRectDraw (date));

							for (int k = i + 1; k < nCount; k++)
							{
								XBCGPAppointmentArray* arNext = arColumns[k];

								for (int m = 0; m < arNext->GetSize(); m++)
								{
									CBCGPAppointment* pAppNext = arNext->GetAt (m);
								
									CRect rtInter;
									if (rtInter.IntersectRect (rtApp, pAppNext->GetRectDraw (date)))
									{
										rtApp.r = rectFill.l + nWidth * k - 
											(bDrawShadow ? BCGP_PLANNER_APPOINTMENT_SPACE : 0);
										pApp->SetRectDraw (rtApp, date);
										break;
									}
								}
							}
						}
					}
				}

				// clean up columns array
				for (i = 0; i < nCount; i++)
				{
					delete arColumns[i];
				}

				arColumns.RemoveAll();
			}
		}

		date += spanDay;
	}

	// adjust "all day" or "multi day" appointments
	if (arAllDays.GetSize() > 0)
	{
		const int c_Count = (int) arAllDays.GetSize();

		CRect rectFill (0, 0, 0, 0);

		rectFill.t    = m_nHeaderHeight + 1;
		rectFill.b = rectFill.t + m_nRowHeight;

		int i = 0;

		for (i = 0; i < c_Count; i++)
		{
			CBCGPAppointment* pApp = arAllDays[i];

			date = m_DateStart;

			for (nDay = 0; nDay < nDays; nDay ++)
			{
				rectFill.l  = m_ViewRects [nDay].l;
				rectFill.r = m_ViewRects [nDay].r;

				if (CBCGPPlannerView::IsAppointmentInDate (*pApp, date))
				{
					pApp->SetRectDraw (rectFill, date);
				}

				date += spanDay;
			}
		}

		for (i = 1; i < c_Count; i++)
		{
			CBCGPAppointment* pApp1 = arAllDays[i];

			CRect rtApp1;
			if (pApp1->GetDSDraw().IsEmpty())
			{
				rtApp1 = pApp1->GetRectDraw();
			}
			else
			{
				rtApp1 = pApp1->GetDSDraw().GetByIndex (0)->GetRect();
			}

			for (int j = 0; j < i; j++)
			{
				CBCGPAppointment* pApp2 = arAllDays[j];

				CRect rtApp2;
				if (pApp2->GetDSDraw().IsEmpty())
				{
					rtApp2 = pApp2->GetRectDraw();
				}
				else
				{
					rtApp2 = pApp2->GetDSDraw().GetByIndex (0)->GetRect();
				}

				CRect rtInter;
				if (rtInter.IntersectRect (rtApp1, rtApp2))
				{
					rtApp1.t    = rtApp2.t;
					rtApp1.b = rtApp2.b;
					rtApp1.OffsetRect (0, m_nRowHeight + 2);
					
					if (pApp1->GetDSDraw().IsEmpty())
					{
						pApp1->SetRectDraw (rtApp1);
					}
					else
					{
						pApp1->GetDSDraw().GetByIndex (0)->SetRect (rtApp1);
					}

					j = 0;
				}
			}
		}
	}

	date = m_DateStart;

	for (nDay = 0; nDay < nDays; nDay ++)
	{
		CheckVisibleAppointments (date, m_rectApps, FALSE);

		date += spanDay;
	}

	CheckVisibleUpDownIcons(FALSE);

	m_bUpdateToolTipInfo = TRUE;
}

void CBCGPPlannerViewDay::AddUpDownRect(BYTE nType, const CRect& /*rect*/)
{
	if ((nType & 0x01) == 0x01 && m_UpRects.GetSize() == 0)
	{
		CSize sz (GetPlanner()->GetUpDownIconSize());
		CRect rt (
			CPoint(
				m_rectTimeBar.r - sz.w, 
				m_rectTimeBar.t + m_nHeaderHeight + m_nHeaderAllDayHeight + 1),
			sz);
		m_UpRects.Add (rt);
	}

	if ((nType & 0x02) == 0x02 && m_DownRects.GetSize() == 0)
	{
		CSize sz (GetPlanner()->GetUpDownIconSize());
		CRect rt (CPoint (m_rectTimeBar.r - sz.w, m_rectTimeBar.b - sz.h - 1),
				sz);
		m_DownRects.Add (rt);
	}
}

void CBCGPPlannerViewDay::CheckVisibleAppointments(const COleDateTime& date, const CRect& rect, 
	BOOL bFullVisible)
{
	XBCGPAppointmentArray& arQueryApps = GetQueryedAppointments();
	XBCGPAppointmentArray& arDragApps  = GetDragedAppointments();

	if (arQueryApps.GetSize() == 0 && arDragApps.GetSize() == 0)
	{
		return;
	}

	BOOL bDragDrop        = IsDragDrop();
	DROPEFFECT dragEffect = GetDragEffect();
	BOOL bDragMatch       = IsCaptureMatched();	

	bDragDrop = !bDragDrop || 
		(bDragDrop && ((dragEffect & DROPEFFECT_COPY) == DROPEFFECT_COPY && bDragMatch) || 
		!bDragMatch);
	bDragDrop = bDragDrop && arDragApps.GetSize();

	BOOL bSelect = date != COleDateTime();

	for (int nApp = 0; nApp < 2; nApp++)
	{
		if (!bDragDrop && nApp == 0)
		{
			continue;
		}

		XBCGPAppointmentArray& arApps = nApp == 0 ? arDragApps : arQueryApps;

		for (int i = 0; i < arApps.GetSize(); i++)
		{
			CBCGPAppointment* pApp = arApps [i];
			if (pApp == NULL)
			{
				continue;
			}

			if (bSelect)
			{
				if (!IsAppointmentInDate (*pApp, date))
				{
					continue;
				}
			}

			CRect rt (rect);
			if (pApp->IsAllDay() || pApp->IsMultiDay())
			{
				rt.t    -= m_nHeaderAllDayHeight;
				rt.b = rt.t + m_nHeaderAllDayHeight;
			}

			CRect rtDraw (pApp->GetRectDraw (date));

			CRect rtInter;
			rtInter.IntersectRect (rtDraw, rt);

			pApp->SetVisibleDraw ((!bFullVisible && rtInter.Height() >= 2) || 
				(bFullVisible && rtInter.Height() == rtDraw.Height() && 
				 rtInter.b < rt.b), date);
		}
	}
}

void CBCGPPlannerViewDay::OnPaint (CDC* pDC, const CRect& rectClient)
{
	ASSERT_VALID (pDC);

	OnDrawClient (pDC, m_rectApps);

	OnDrawAppointmentsDuration (pDC);

	if (m_nHeaderAllDayHeight != 0)
	{
		CRect rtHeader (m_rectApps);
		rtHeader.t    -= m_nHeaderAllDayHeight;
		rtHeader.b  = rtHeader.t + m_nHeaderAllDayHeight;

		OnDrawHeaderAllDay (pDC, rtHeader);

		CRgn rgn;
		rgn.CreateRectRgn (rtHeader.l, rtHeader.t, rtHeader.r, rtHeader.b);

		pDC->SelectClipRgn (&rgn);

		OnDrawAppointments (pDC, rtHeader);

		pDC->SelectClipRgn (NULL);
	}

	{
		CRgn rgn;
		rgn.CreateRectRgn (m_rectApps.l, m_rectApps.t, m_rectApps.r, m_rectApps.b);

		pDC->SelectClipRgn (&rgn);
		
		OnDrawAppointments (pDC, m_rectApps);

		pDC->SelectClipRgn (NULL);
	}

	if (!m_rectTimeBar.IsRectEmpty())
	{
		OnDrawTimeBar (pDC, m_rectTimeBar, IsCurrentTimeVisible());
	}

	if (m_nHeaderHeight != 0)
	{
		CRect rtHeader (rectClient);
		rtHeader.l   = m_rectApps.l;
		rtHeader.b = rtHeader.t + m_nHeaderHeight;

		OnDrawHeader (pDC, rtHeader);
	}

	InitToolTipInfo();
}

void CBCGPPlannerViewDay::OnDrawClient (CDC* pDC, const CRect& rect)
{
	ASSERT_VALID (pDC);

	CRect rectFill (rect);

//	const int nWeekStart = CBCGPPlannerManagerCtrl::GetFirstDayOfWeek() + 1;
	const int yOffset = m_nScrollOffset * m_nRowHeight;

	int nFirstWorkingHour  = GetFirstWorkingHour();
	int nFirstWorkingMinute = GetFirstWorkingMinute();
	int nLastWorkingHour   = GetLastWorkingHour();
	int nLastWorkingMinute  = GetLastWorkingMinute();

	if (GetPlanner()->IsMultiResourceStorage())
	{
		CBCGPAppointmentBaseMultiStorage* pStorage = 
			DYNAMIC_DOWNCAST (CBCGPAppointmentBaseMultiStorage, GetPlanner()->GetStorage());
		if (pStorage != NULL)
		{
			const CBCGPAppointmentBaseResourceInfo* pInfo = 
				pStorage->GetResourceInfo (GetPlanner()->GetCurrentResourceID());

			if (pInfo != NULL)
			{
				COleDateTime dtStart (pInfo->GetWorkStart());
				COleDateTime dtEnd (pInfo->GetWorkEnd());

				if (dtStart != COleDateTime() && dtEnd != COleDateTime() &&
					dtStart != dtEnd)
				{
					nFirstWorkingHour = dtStart.GetHour();
					nFirstWorkingMinute = dtStart.GetMinute();
					nLastWorkingHour = dtEnd.GetHour();
					nLastWorkingMinute = dtEnd.GetMinute();
				}
			}
		}
	}

	const int nDays = GetViewDuration();

	rectFill.OffsetRect (0, -yOffset);

	const int nMinuts = CBCGPPlannerView::GetTimeDeltaInMinuts (GetTimeDelta());
	const int nCount = 60 / nMinuts;

	CPen     penHour[2];
	CPen     penHalfHour[2];

	for (int i = 0; i < 2; i++)
	{
		penHour[i].CreatePen (PS_SOLID, 0,
			GetHourLineColor (i == 0 /* Working */, TRUE));

		penHalfHour[i].CreatePen (PS_SOLID, 0, 
			GetHourLineColor (i == 0 /* Working */, FALSE));
	}

	OnFillPlanner (pDC, rect, FALSE /* Non-working */);

	CBrush brHilite (visualManager->GetPlannerSelectionColor (this));

	CPen penBlack (PS_SOLID, 0, visualManager->GetPlannerSeparatorColor (this));
	CPen* pOldPen = pDC->SelectObject (&penBlack);

	const int iStart = m_nScrollOffset;
	const int iEnd   = min (m_nScrollOffset + rect.Height() / m_nRowHeight, nCount * 24);

	const int iWorkStart = nFirstWorkingHour * nCount + (int)(nFirstWorkingMinute / nMinuts);
	const int iWorkEnd   = nLastWorkingHour * nCount + (int)(nLastWorkingMinute / nMinuts);

	COleDateTime dtStart (GetDateStart());

	BOOL bShowSelection = !((m_Selection[0].GetHour()   == 0 &&
							 m_Selection[0].GetMinute() == 0 &&
							 m_Selection[0].GetSecond() == 0 &&
							 m_Selection[1].GetHour()   == 23 &&
							 m_Selection[1].GetMinute() == 59 &&
							 m_Selection[1].GetSecond() == 59) ||
							(m_Selection[1].GetHour()   == 0 &&
							 m_Selection[1].GetMinute() == 0 &&
							 m_Selection[1].GetSecond() == 0 &&
							 m_Selection[0].GetHour()   == 23 &&
							 m_Selection[0].GetMinute() == 59 &&
							 m_Selection[0].GetSecond() == 59));

	BOOL bIsDrawDuration = 
		(GetPlanner()->GetDrawFlags() & BCGP_PLANNER_DRAW_VIEW_NO_DURATION) == 0;
	const int nDurationWidth = bIsDrawDuration ? BCGP_PLANNER_DURATION_BAR_WIDTH + 1 : 0;

	visualManager->PreparePlannerBackItem (FALSE, FALSE);

	int nDay = 0;
	for (nDay = 0; nDay < nDays; nDay++)
	{
		int nWD = dtStart.GetDayOfWeek();
		BOOL bWeekEnd = nWD == 1 || nWD == 7;

		rectFill = m_ViewRects [nDay];

		rectFill.l   += nDurationWidth;
		rectFill.b = rectFill.t + m_nRowHeight - 1;

		for (int iStep = iStart; iStep < iEnd; iStep++)
		{
			BOOL bIsWork = !bWeekEnd && (iWorkStart <= iStep && iStep < iWorkEnd);

			if (!IsDateInSelection (dtStart + 
				COleDateTimeSpan (0, (iStep * nMinuts) / 60, (iStep * nMinuts) % 60, 0)) ||
				!bShowSelection)
			{
				if (bIsWork)
				{
					OnFillPlanner (pDC, rectFill, TRUE /* Working */);
				}
			}
			else
			{
				pDC->FillRect (rectFill, &brHilite);
			}

			int nPenIndex = bIsWork ? 0 : 1;

			pDC->SelectObject (((iStep + 1) % nCount == 0) ? 
				&penHour [nPenIndex] : &penHalfHour [nPenIndex]);

			pDC->MoveTo (rectFill.l, rectFill.b);
			pDC->LineTo (rectFill.r, rectFill.b);

			rectFill.OffsetRect (0, m_nRowHeight);
		}

		dtStart += COleDateTimeSpan (1, 0, 0, 0);
	}

	pDC->SelectObject (&penBlack);

	if (bIsDrawDuration)
	{
		for (nDay = 0; nDay < nDays; nDay++)
		{
			CRect rectDurBar (m_ViewRects [nDay]);
			rectDurBar.r = rectDurBar.l + BCGP_PLANNER_DURATION_BAR_WIDTH;

			// Draw duration bar (at left):
			pDC->FillRect (rectDurBar, &globalData.brWindow);

			if (nDay > 0)
			{
				pDC->MoveTo (rectDurBar.l, rectDurBar.t);
				pDC->LineTo (rectDurBar.l, rectDurBar.b);
			}

			pDC->MoveTo (rectDurBar.r, rectDurBar.t);
			pDC->LineTo (rectDurBar.r, rectDurBar.b);
		}
	}
	else
	{
		for (nDay = 0; nDay < nDays; nDay++)
		{
			CRect rectDurBar (m_ViewRects [nDay]);

			if (nDay > 0)
			{
				pDC->MoveTo (rectDurBar.l, rectDurBar.t);
				pDC->LineTo (rectDurBar.l, rectDurBar.b);
			}
		}
	}

	pDC->SelectObject (pOldPen);
}

void CBCGPPlannerViewDay::OnDrawHeader (CDC* pDC, const CRect& rectHeader)
{
	ASSERT_VALID (pDC);

	const int nDays = GetViewDuration();

	DrawHeader (pDC, rectHeader, m_ViewRects [0].Width());

	CRect rectDayCaption (rectHeader);

	COleDateTime day (GetDateStart());

	COleDateTime dayCurrent = COleDateTime::GetCurrentTime();
	dayCurrent.SetDateTime (dayCurrent.GetYear(), dayCurrent.GetMonth(), 
		dayCurrent.GetDay(), 0, 0, 0);

	DWORD dwFlags = GetPlanner()->GetDrawFlags();
	BOOL bBold = (dwFlags & BCGP_PLANNER_DRAW_VIEW_CAPTION_DAY_BOLD) ==
			BCGP_PLANNER_DRAW_VIEW_CAPTION_DAY_BOLD;
	BOOL bCompact = (dwFlags & BCGP_PLANNER_DRAW_VIEW_CAPTION_DAY_COMPACT) ==
			BCGP_PLANNER_DRAW_VIEW_CAPTION_DAY_COMPACT;

	HFONT hOldFont = NULL;
	if (bBold)
	{
		hOldFont = SetCurrFont (pDC, bBold);
	}

	for (int nDay = 0; nDay < nDays; nDay++)
	{
		rectDayCaption.l = m_ViewRects [nDay].l;
		rectDayCaption.r = m_ViewRects [nDay].r;

		visualManager->PreparePlannerCaptionBackItem (TRUE);
		COLORREF clrText = OnFillPlannerCaption (
			pDC, rectDayCaption, day == dayCurrent, FALSE, FALSE);

		if (!bCompact)
		{
			DrawCaptionText (pDC, rectDayCaption, day, clrText);
		}
		else
		{
			CString strText;
			ConstructCaptionText (day, strText, GetCaptionFormat());

			if (strText.Find (TCHAR('\n')) != -1)
			{
				CString strDate;

				AfxExtractSubString (strDate, strText, 0, TCHAR('\n'));
				DrawCaptionText (pDC, rectDayCaption, strDate, clrText, DT_LEFT);

				AfxExtractSubString (strDate, strText, 1, TCHAR('\n'));
				DrawCaptionText (pDC, rectDayCaption, strDate, clrText, DT_CENTER);
			}
			else
			{
				DrawCaptionText (pDC, rectDayCaption, strText, clrText, DT_LEFT);
			}
		}

		day += COleDateTimeSpan (1, 0, 0, 0);
	}

	if (hOldFont != NULL)
	{
		::SelectObject (pDC, hOldFont);
	}
}

void CBCGPPlannerViewDay::OnDrawHeaderAllDay (CDC* pDC, const CRect& rectHeader)
{
	ASSERT_VALID (pDC);

	const int nDays = GetViewDuration();

	visualManager->OnFillPlannerHeaderAllDay (pDC, this, rectHeader);

	int nDay = 0;

	COleDateTime date (GetDateStart());
	COleDateTimeSpan span (1, 0, 0, 0);

	CRect rt (rectHeader);
	rt.b--;

	COleDateTime dayCurrent = COleDateTime::GetCurrentTime();
	dayCurrent.SetDateTime (dayCurrent.GetYear(), dayCurrent.GetMonth(), 
		dayCurrent.GetDay(), 0, 0, 0);

	if (nDays > 1)
	{
		CPen penBlack (PS_SOLID, 0, visualManager->GetPlannerSeparatorColor (this));
		CPen* pOldPen = pDC->SelectObject (&penBlack);

		for (nDay = 1; nDay < nDays; nDay++)
		{
			pDC->MoveTo (m_ViewRects [nDay].l, rectHeader.t);
			pDC->LineTo (m_ViewRects [nDay].l, rectHeader.b - 1);
		}

		pDC->SelectObject (pOldPen);
	}

	for (nDay = 0; nDay < nDays; nDay++)
	{
		rt.l  = m_ViewRects [nDay].l;
		rt.r = m_ViewRects [nDay].r;

		visualManager->OnDrawPlannerHeaderAllDayItem (pDC, this, rt, 
			date == dayCurrent, m_SelectionAllDay[nDay]);

		date += span;
	}
}

void CBCGPPlannerViewDay::OnDrawTimeLine (CDC* pDC, const CRect& rect)
{
	visualManager->OnDrawPlannerTimeLine (pDC, this, rect);
}

void CBCGPPlannerViewDay::OnDrawTimeBar (CDC* pDC, const CRect& rectBar, 
										 BOOL bDrawTimeLine)
{
	ASSERT_VALID (pDC);

	BOOL b24Hours = CBCGPPlannerView::Is24Hours();

	CString strAM;
	CString strPM;

	if (!b24Hours)
	{
		strAM = CBCGPPlannerView::GetTimeDesignator (TRUE);
		strAM.MakeLower();
		strPM = CBCGPPlannerView::GetTimeDesignator (FALSE);
		strPM.MakeLower();
	}

	COLORREF clrLine = globalData.clrBtnShadow;

	COLORREF clrText = visualManager->OnFillPlannerTimeBar (pDC, this, rectBar, clrLine);

	COLORREF clrTextOld = pDC->SetTextColor (clrText);

	const int nHeaderHeight = m_nHeaderHeight + m_nHeaderAllDayHeight;

	int y = rectBar.t + nHeaderHeight - m_nScrollOffset * m_nRowHeight - 1; 

	CPen pen (PS_SOLID, 0, clrLine);
	CPen* pOldPen = pDC->SelectObject (&pen);

	const long nCount = 60 / CBCGPPlannerView::GetTimeDeltaInMinuts (GetTimeDelta());

	UINT nFormat = DT_SINGLELINE | DT_RIGHT;

	if (nCount == 1)
	{
		nFormat |= DT_VCENTER;
	}

	CFont* pFont = CFont::FromHandle(GetFont());

	ASSERT (pFont != NULL);

	LOGFONT lf;
	pFont->GetLogFont (&lf);

	lf.lfHeight *= 2;

	CFont* pOldFont = pDC->SelectObject (pFont);

	CFont fontBold;
	fontBold.CreateFontIndirect (&lf);

	if (bDrawTimeLine)
	{
		int nTimeHeight = CBCGPPlannerView::round (
			(m_CurrentTime.GetHour() + m_CurrentTime.GetMinute() / 60.0) * 
			nCount * m_nRowHeight);

		CRect rectTime  = rectBar;
		rectTime.b = y + nTimeHeight;
		rectTime.t    = rectTime.b - m_nRowHeight / (nCount > 1 ? 2 : 4);
		rectTime.l   += 5;

		OnDrawTimeLine (pDC, rectTime);
	}

	int bDrawFirstAM_PM = 0;

	int right = 0;
	int nStartHour = 0;

	int nMinDelta = max (pDC->GetTextExtent (_T("000")).w, 18);
	CString strSeparator = CBCGPPlannerView::GetTimeSeparator();

	for (int i = nStartHour; i < nStartHour + 24; i++)
	{
		CRect rectHour  = rectBar;
		rectHour.t    = y;
		rectHour.b = y + m_nRowHeight * nCount;
		rectHour.l   += 5;
		rectHour.r  -= 5;

		if(rectHour.b < nHeaderHeight)
		{
			y = rectHour.b;
			continue;
		}

		if (nCount > 2)
		{
			long nd = y + m_nRowHeight;

			for (int j = 0; j < nCount - 1; j++)
			{
				if (nd >= nHeaderHeight)
				{
					pDC->MoveTo (rectHour.r - nMinDelta, nd);
					pDC->LineTo (rectHour.r, nd);
				}

				nd += m_nRowHeight;
			}
		}

		if (rectHour.b >= nHeaderHeight)
		{
			y += m_nRowHeight * nCount;

			pDC->MoveTo (rectHour.l , y);
			pDC->LineTo (rectHour.r + right, y);
		}

		if (rectHour.t >= nHeaderHeight || rectHour.b > nHeaderHeight)
		{
			if (rectHour.t >= nHeaderHeight - 1)
			{
				bDrawFirstAM_PM++;
			}

			CString str (_T("00"));

			int nHour = (i < 24) ? i : i - 24;

			BOOL bAM = nHour < 12; 

			if (!b24Hours)
			{
				if (nHour == 0 || nHour == 12)
				{
					nHour = 12;
				}
				else if (nHour > 12)
				{
					nHour -= 12;
				}
			}

			if (nCount == 1)
			{
				str.Format (_T("%2d%s00"), nHour, strSeparator);

				if (!b24Hours)
				{
					if (nHour == 12)
					{
						str.Format (_T("12 %s"), bAM ? strAM : strPM);
					}
				}

				pDC->DrawText (str, rectHour, nFormat);
			}
			else
			{
				y = rectHour.b;

				rectHour.b = rectHour.t + m_nRowHeight;

				if (!b24Hours)
				{
					if (nHour == 12 || bDrawFirstAM_PM == 1)
					{
						str = bAM ? strAM : strPM;
					}
				}

				CRect rectMin (rectHour);
				rectMin.l   = rectMin.r - nMinDelta;

				pDC->DrawText (str, rectMin, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

				pDC->SelectObject (&fontBold);

				rectHour.b = y;
				rectHour.r -= nMinDelta;

				str.Format (_T("%d"), nHour);
				pDC->DrawText (str, rectHour, nFormat);

				pDC->SelectObject (pFont);
			}
		}

		y = rectHour.b;
	}

	pDC->SelectObject (pOldFont);

	CRect rt (rectBar);
	rt.b = rt.t + nHeaderHeight;

	visualManager->OnFillPlannerTimeBar (pDC, this, rt, clrLine);

	pDC->MoveTo (rectBar.l, rt.b - 1);
	pDC->LineTo (rectBar.r - 5, rt.b - 1);

	pDC->MoveTo (rectBar.r - 1, rectBar.t);
	pDC->LineTo (rectBar.r - 1, rectBar.b);

	pDC->SelectObject (pOldPen);

	OnDrawUpDownIcons (pDC);

	pDC->SetTextColor (clrTextOld);
}

BYTE CBCGPPlannerViewDay::OnDrawAppointments (CDC* pDC, const CRect& rect, const COleDateTime& date)
{
	BYTE res = 0;

	XBCGPAppointmentArray& arQueryApps = GetQueryedAppointments();
	XBCGPAppointmentArray& arDragApps  = GetDragedAppointments();

	if (arQueryApps.GetSize() == 0 && arDragApps.GetSize() == 0)
	{
		return res;
	}

	BOOL bDragDrop        = IsDragDrop();
	DROPEFFECT dragEffect = GetDragEffect();
	BOOL bDragMatch       = IsCaptureMatched();

	BOOL bSelect = date != COleDateTime();

	XBCGPAppointmentArray arSelected;

	CRect rt;
	BOOL bClient = rt.IntersectRect (rect, m_rectApps);

	for (int nApp = 0; nApp < 2; nApp++)
	{
		XBCGPAppointmentArray& arApps = nApp == 1 ? arDragApps : arQueryApps;

		if (nApp == 1)
		{
			bDragDrop = bDragDrop && arDragApps.GetSize();
		}

		if (arApps.GetSize() == 0)
		{
			continue;
		}

		for (int i = 0; i < arApps.GetSize(); i++)
		{
			CBCGPAppointment* pApp = arApps [i];
			if (pApp == NULL)
			{
				continue;
			}

			if (bSelect)
			{
				if (!CBCGPPlannerView::IsAppointmentInDate (*pApp, date))
				{
					continue;
				}
			}

			BOOL bAllOrMulti = pApp->IsAllDay() || pApp->IsMultiDay();

			if ((bClient && bAllOrMulti) || (!bClient && !bAllOrMulti))
			{
				continue;
			}

			if (((!bDragDrop || dragEffect == DROPEFFECT_NONE) && nApp == 0) || nApp == 1)
			{
				if (pApp->IsSelected())
				{
					arSelected.Add (pApp);
				}
			}

			if (!bSelect || (bSelect && pApp->IsVisibleDraw (date)))
			{
				if (bDragDrop && dragEffect != DROPEFFECT_NONE && 
					pApp->IsSelected() && nApp == 0)
				{
					if ((dragEffect & DROPEFFECT_COPY) == DROPEFFECT_COPY || bDragMatch)
					{
						pApp->SetSelected (FALSE);
						pApp->OnDraw (pDC, this, date);
						pApp->SetSelected (TRUE);
					}
				}
				else if (!pApp->IsSelected())
				{
					pApp->OnDraw (pDC, this, date);
				}
			}
			else if (!bAllOrMulti)
			{
				if (!rect.IsRectNull())
				{
					if (pApp->GetRectDraw (date).b <= rect.t)
					{
						res |= 0x01;
					}
					else
					{
						res |= 0x02;
					}
				}
				else
				{
					res |= 0x01;
				}
			}
		}
	}

	if (arSelected.GetSize() > 0)
	{
		for (int i = (int) arSelected.GetSize() - 1; i >= 0; i--)
		{
			CBCGPAppointment* pApp = arSelected [i];

			if (bSelect)
			{
				if (!(pApp->IsAllDay() || pApp->IsMultiDay()))
				{
					CRect rt (pApp->GetRectDraw (date));
					rt.InflateRect (0, CBCGPPlannerViewDay::BCGP_PLANNER_DURATION_BAR_WIDTH - 1);

					CRect rtInter;

					if (pApp->IsSelected() && rtInter.IntersectRect (rect, rt))
					{
						pApp->OnDraw (pDC, this, date);
					}
				}
				else if (pApp->IsVisibleDraw (date))
				{
					pApp->OnDraw (pDC, this, date);
				}
			}
			else
			{
				pApp->OnDraw (pDC, this, date);
			}
		}
	}

	return res;
}

DROPEFFECT CBCGPPlannerViewDay::OnDragScroll(DWORD /*dwKeyState*/, CPoint /*point*/)
{
	return DROPEFFECT_SCROLL;
}

DROPEFFECT CBCGPPlannerViewDay::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	ASSERT_VALID (m_pPlanner);

	ASSERT (pDataObject != NULL);
	ASSERT (pDataObject->IsDataAvailable (CBCGPPlannerManagerCtrl::GetClipboardFormat()));

	DROPEFFECT dragEffect    = HitTestDrag (dwKeyState, point);
	DROPEFFECT dragEffectOld = GetDragEffect();

	BOOL bDateChanged       = FALSE;
	BOOL bAreaChanged       = FALSE;
	BOOL bResourceChanged   = FALSE;
	BOOL bDragEffectChanged = dragEffectOld != dragEffect;
	BOOL bDragMatchOld      = IsCaptureMatched();

	BCGP_PLANNER_HITTEST htAreaOld = m_htCaptureAreaCurrent;

	m_AdjustAction = BCGP_PLANNER_ADJUST_ACTION_NONE;

	if (dragEffect != DROPEFFECT_NONE)
	{
		COleDateTime dtCur  = GetDateFromPoint (point);
		BCGP_PLANNER_HITTEST htArea = HitTestArea (point);
		UINT htResource = GetResourceFromPoint (point);

		bDateChanged = dtCur != COleDateTime() && dtCur != m_dtCaptureCurrent;
		bAreaChanged     = m_htCaptureAreaCurrent != htArea;
		bResourceChanged = m_htCaptureResourceCurrent != htResource;

		if (bDateChanged)
		{
			m_dtCaptureCurrent = dtCur;
		}

		if (bAreaChanged)
		{
			if ((htArea == BCGP_PLANNER_HITTEST_HEADER ||
				 htArea == BCGP_PLANNER_HITTEST_HEADER_RESOURCE) && 
				m_htCaptureAreaCurrent == BCGP_PLANNER_HITTEST_HEADER_ALLDAY)
			{
				htArea = BCGP_PLANNER_HITTEST_HEADER_ALLDAY;
				bAreaChanged = FALSE;
			}

			m_htCaptureAreaCurrent = htArea;
		}

		if (bResourceChanged)
		{
			m_htCaptureResourceCurrent = htResource;
		}
	}

	BOOL bAllDay = m_htCaptureAreaCurrent == BCGP_PLANNER_HITTEST_HEADER_ALLDAY;

	BOOL bChanged = bDragEffectChanged || bDateChanged || bAreaChanged || bResourceChanged;

	if (bChanged)
	{
		if (m_pDragAppointments != NULL)
		{
			m_pDragAppointments->RemoveAll();
			m_arDragAppointments.RemoveAll();
		}
		else
		{
			m_pDragAppointments = 
				(CBCGPAppointmentStorage*)RUNTIME_CLASS(CBCGPAppointmentStorage)->CreateObject();
		}

		if (dragEffect == DROPEFFECT_NONE)
		{
			if (GetDragEffect() != dragEffect && 
				htAreaOld == BCGP_PLANNER_HITTEST_HEADER_ALLDAY)
			{
				m_AdjustAction = BCGP_PLANNER_ADJUST_ACTION_LAYOUT;
			}
			else
			{
				m_AdjustAction = BCGP_PLANNER_ADJUST_ACTION_APPOINTMENTS;
			}

			return dragEffect;
		}

		CFile* pFile = pDataObject->GetFileData (CBCGPPlannerManagerCtrl::GetClipboardFormat());
		if (pFile == NULL)
		{
			return FALSE;
		}

		XBCGPAppointmentArray ar;

		BOOL bRes = CBCGPPlannerManagerCtrl::SerializeFrom (*pFile, ar, 
			m_pPlanner->GetType(), 
			IsDragDrop() ? COleDateTime() : m_dtCaptureCurrent);

		delete pFile;

		if (bRes)
		{
			if (IsDragDrop())
			{
				COleDateTimeSpan spanTo (0, 0, 0, 0);

				BOOL bAdd = m_dtCaptureCurrent > m_dtCaptureStart;

				if (bAdd)
				{
					spanTo = m_dtCaptureCurrent - m_dtCaptureStart;
				}
				else
				{
					spanTo = m_dtCaptureStart - m_dtCaptureCurrent;
				}

				CBCGPPlannerManagerCtrl::MoveAppointments (ar, spanTo, bAdd);
			}

			for (int i = 0; i < ar.GetSize(); i++)
			{
				CBCGPAppointment* pApp = ar[i];

				pApp->SetSelected (TRUE);

				BOOL bAllOrMulti = pApp->IsAllDay() || pApp->IsMultiDay();

				if (bAllOrMulti)
				{
					if (!bAllDay)
					{
						pApp->SetAllDay (FALSE);
						pApp->SetInterval (m_dtCaptureCurrent, m_dtCaptureCurrent);
					}
				}
				else if (bAllDay)
				{
					pApp->SetAllDay (TRUE);
					pApp->SetInterval (m_dtCaptureCurrent, m_dtCaptureCurrent);
				}

				pApp->SetResourceID (m_htCaptureResourceCurrent);
				m_pDragAppointments->Add (pApp);
			}

			m_pDragAppointments->Query (m_arDragAppointments, m_DateStart, m_DateEnd);
		}
	}

	if (bChanged || bDragMatchOld != IsCaptureMatched())
	{
		if (
			(
				bAreaChanged && 
				(htAreaOld == BCGP_PLANNER_HITTEST_HEADER_ALLDAY ||
				 m_htCaptureAreaCurrent == BCGP_PLANNER_HITTEST_HEADER_ALLDAY)
			) ||
			(
				bDragEffectChanged && 
				((m_htCaptureAreaStart == BCGP_PLANNER_HITTEST_HEADER_ALLDAY &&
				  m_htCaptureAreaCurrent == BCGP_PLANNER_HITTEST_HEADER_ALLDAY) ||
				 (dragEffectOld == DROPEFFECT_NONE && 
				  m_htCaptureAreaCurrent == BCGP_PLANNER_HITTEST_HEADER_ALLDAY))
			) ||
			(
				(bDateChanged || bResourceChanged) && 
				m_htCaptureAreaCurrent == BCGP_PLANNER_HITTEST_HEADER_ALLDAY
			)
		   )
		{
			TRACE0("BCGP_PLANNER_ADJUST_ACTION_LAYOUT\n");
			m_AdjustAction = BCGP_PLANNER_ADJUST_ACTION_LAYOUT;
		}
		else
		{
			TRACE0("BCGP_PLANNER_ADJUST_ACTION_APPOINTMENTS\n");
			m_AdjustAction = BCGP_PLANNER_ADJUST_ACTION_APPOINTMENTS;
		}
	}

	return dragEffect;
}

CBCGPPlannerView::BCGP_PLANNER_HITTEST
CBCGPPlannerViewDay::HitTest (const CPoint& point) const
{
	BCGP_PLANNER_HITTEST hit = HitTestArea (point);

	if (hit == BCGP_PLANNER_HITTEST_CLIENT || 
		hit == BCGP_PLANNER_HITTEST_HEADER_ALLDAY)
	{
		BCGP_PLANNER_HITTEST hitA = HitTestAppointment (point);

		if (hitA != BCGP_PLANNER_HITTEST_NOWHERE)
		{
			hit = hitA;
		}
	}

	return hit;
}

CBCGPPlannerView::BCGP_PLANNER_HITTEST
CBCGPPlannerViewDay::HitTestArea (const CPoint& point) const
{
	BCGP_PLANNER_HITTEST hit = BCGP_PLANNER_HITTEST_NOWHERE;

	if (m_rectTimeBar.PtInRect (point))
	{
		if ((m_rectTimeBar.t + m_nHeaderHeight + m_nHeaderAllDayHeight) < point.y)
		{
			hit = BCGP_PLANNER_HITTEST_TIMEBAR;

			if (m_UpRects.GetSize() == 1 && m_UpRects[0].PtInRect (point))
			{
				hit = BCGP_PLANNER_HITTEST_ICON_UP;
			}
			else if (m_DownRects.GetSize() == 1 && m_DownRects[0].PtInRect (point))
			{
				hit = BCGP_PLANNER_HITTEST_ICON_DOWN;
			}
		}
		else
		{
			hit = BCGP_PLANNER_HITTEST_NOWHERE;
		}
	}
	else if (m_rectApps.l <= point.x && point.x <= m_rectApps.r)
	{
		if (0 <= point.y && point.y < m_nHeaderHeight)
		{
			hit = BCGP_PLANNER_HITTEST_HEADER;
		}
		else if (m_nHeaderHeight <= point.y && point.y < (m_nHeaderHeight + m_nHeaderAllDayHeight))
		{
			hit = BCGP_PLANNER_HITTEST_HEADER_ALLDAY;
		}
		else if (m_rectApps.PtInRect (point))
		{
			hit = BCGP_PLANNER_HITTEST_CLIENT;
		}
	}
		
	return hit;
}

BOOL CBCGPPlannerViewDay::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nPrevOffset = m_nScrollOffset;

	switch (nSBCode)
	{
	case SB_LINEUP:
		m_nScrollOffset -= 1;
		break;

	case SB_LINEDOWN:
		m_nScrollOffset += 1;
		break;

	case SB_TOP:
		m_nScrollOffset = 0;
		break;

	case SB_BOTTOM:
		m_nScrollOffset = m_nScrollTotal;
		break;

	case SB_PAGEUP:
		m_nScrollOffset -= m_nScrollPage;
		break;

	case SB_PAGEDOWN:
		m_nScrollOffset += m_nScrollPage;
		break;

	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		m_nScrollOffset = nPos;
		break;

	default:
		return FALSE;
	}

	m_nScrollOffset = max (0, m_nScrollOffset);

	if (m_nScrollTotal == m_nScrollPage)
	{
		if (m_nScrollOffset > 0)
		{
			m_nScrollOffset = 1;
		}
	}
	else
	{
		m_nScrollOffset = min (m_nScrollOffset, m_nScrollTotal - m_nScrollPage + 1);
	}

	if (m_nScrollOffset == nPrevOffset)
	{
		return FALSE;
	}

	return CBCGPPlannerView::OnVScroll (nSBCode, nPos, pScrollBar);
}

BOOL CBCGPPlannerViewDay::OnKeyDown(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/)
{
	if (m_pCapturedAppointment != NULL)
	{
		GetPlanner()->SendMessage (WM_CANCELMODE, 0, 0);
		return TRUE;
	}

	BOOL isShiftPressed = (0x8000 & GetKeyState(VK_SHIFT)) != 0;

	const int nMinuts = CBCGPPlannerView::GetTimeDeltaInMinuts (GetTimeDelta());

	COleDateTime sel1 = GetSelectionStart();
	COleDateTime sel2 = GetSelectionEnd();

	if (sel1.GetSecond() == 59)
	{
		sel1 = COleDateTime (sel1.GetYear(), sel1.GetMonth(), sel1.GetDay(),
							 sel1.GetHour(), sel1.GetMinute() - nMinuts + 1, 0);
	}

	if (sel2.GetSecond() == 59)
	{
		sel2 = COleDateTime (sel2.GetYear(), sel2.GetMonth(), sel2.GetDay(),
							 sel2.GetHour(), sel2.GetMinute() - nMinuts + 1, 0);
	}

	BOOL bHandled = FALSE;

	if (nChar == VK_PRIOR || nChar == VK_NEXT) // Page Up, Page Down
	{
		int nPrevScroll = m_nScrollOffset;

		OnVScroll (nChar == VK_PRIOR ? SB_PAGEUP : SB_PAGEDOWN, 0, NULL);

		if (nPrevScroll == m_nScrollOffset)
		{
			sel2 = COleDateTime (
				m_Date.GetYear(), m_Date.GetMonth(), m_Date.GetDay(), 
				nChar == VK_PRIOR ? 0 : 23, nChar == VK_PRIOR ? 0 : 60 - nMinuts, 0);
		}
		else
		{
			int n = m_nScrollPage * nMinuts;

			COleDateTimeSpan span (0, n / 60, n % 60, 0);

			if (nChar == VK_PRIOR)
			{
				sel2 -= span;
			}
			else
			{
				sel2 += span;
			}
		}

		if (!isShiftPressed)
		{
			sel1 = sel2;
		}

		SetSelection (sel1, sel2);

		bHandled = TRUE;
	}
	else if (nChar == VK_HOME || nChar == VK_END)
	{
		if (nChar == VK_HOME)
		{
			sel2 = COleDateTime (
				m_Date.GetYear(), m_Date.GetMonth(), m_Date.GetDay(), 
				GetFirstWorkingHour(), (int)(GetFirstWorkingMinute() / nMinuts) * nMinuts,
				0);
		}
		else
		{
			int nLastHour  = GetLastWorkingHour();
			int nLastMinut = (int)(GetLastWorkingMinute() / nMinuts) * nMinuts;
			if (nLastMinut < nMinuts)
			{
				if (nLastHour > 0)
				{
					nLastHour--;
				}
				nLastMinut = 60;
			}

			sel2 = COleDateTime (
				m_Date.GetYear(), m_Date.GetMonth(), m_Date.GetDay(), 
				nLastHour, nLastMinut - nMinuts, 0);
		}

		if (!isShiftPressed)
		{
			sel1 = sel2;
		}

		int nPos = (60 * sel2.GetHour() + sel2.GetMinute()) / nMinuts;
		if (nPos < m_nScrollOffset || (m_nScrollOffset + m_nScrollPage) < nPos)
		{
			OnVScroll (SB_THUMBPOSITION, 
				nChar == VK_HOME ? nPos : nPos - m_nScrollPage + 1, NULL);
		}

		SetSelection (sel1, sel2);

		bHandled = TRUE;
	}
	else if (nChar == VK_UP || nChar == VK_DOWN || nChar == VK_LEFT || nChar == VK_RIGHT)
	{
		int nDuration = GetViewDuration();

		if (nChar == VK_LEFT || nChar == VK_RIGHT)
		{
			BOOL bJump = FALSE;

			COleDateTime pSel1 (sel1 <= sel2 ? sel1 : sel2);
			COleDateTime pSel2 (sel1 <= sel2 ? sel2 : sel1);

			if (isShiftPressed)
			{
				pSel1 = pSel2 = sel2;
			}

			COleDateTimeSpan span (1, 0, 0, 0);

			if (nChar == VK_LEFT)
			{
				if (IsOneDay (pSel1, m_DateStart))
				{
					COleDateTime dt (GetPrevDateStart());
					span = (m_DateStart - dt);

					bJump = TRUE;
				}
			}
			else
			{
				if (IsOneDay (pSel2, m_DateEnd))
				{
					COleDateTime dt (GetNextDateStart());
					span = (dt - m_DateStart);

					bJump = TRUE;
				}
			}

			if (nChar == VK_LEFT)
			{
				if (!isShiftPressed || bJump)
				{
					sel1 -= span;
				}

				sel2 -= span;
			}
			else
			{
				if (!isShiftPressed || bJump)
				{
					sel1 += span;
				}

				sel2 += span;
			}
		}
		else
		{
			COleDateTimeSpan span (0, nMinuts / 60, nMinuts == 60 ? 0 : nMinuts, 0);

			BOOL bJump = FALSE;

			if (nChar == VK_UP)
			{
				sel2 -= span;
			}
			else
			{
				sel2 += span;
			}

			if (sel2 < m_DateStart && nDuration > 1)
			{
				COleDateTime dt (GetPrevDateStart());
				sel2 += span;
				sel2 -= (m_DateStart - dt);

				bJump = TRUE;
			}
			else if (sel2 < m_Date)
			{
				sel2 = m_Date;
			}

			if (sel2 > m_DateEnd && nDuration > 1)
			{
				COleDateTime dt (GetNextDateStart());
				sel2 -= span;
				sel2 += (dt - m_DateStart);

				bJump = TRUE;
			}
			else if (sel2 > m_Date + COleDateTimeSpan (0, 23, 60 - nMinuts, 0))
			{
				sel2 = m_Date + COleDateTimeSpan (0, 23, 60 - nMinuts, 0);
			}

			if (!isShiftPressed || bJump)
			{
				sel1 = sel2;
			}
		}

		BOOL bAdjust = FALSE;

		if (!IsOneDay (m_Date, sel2))
		{
			m_Date = COleDateTime (sel2.GetYear(), sel2.GetMonth(), sel2.GetDay(), 0, 0, 0);

			bAdjust = m_Date < m_DateStart || m_Date > m_DateEnd;

			if (bAdjust)
			{
				m_DateStart = CalculateDateStart (m_Date);
				m_DateEnd   = m_DateStart + COleDateTimeSpan (nDuration - 1, 23, 59, 59);
			}
		}

		SetSelection (sel1, sel2, !bAdjust);

		if (bAdjust)
		{
			QueryAppointments();

			CBCGPPlannerView::AdjustLayout (TRUE);
		}
		else if (m_nScrollTotal != 0)
		{
			BOOL bScroll = FALSE;

			COleDateTime dt1 (GetDateFromPoint (m_rectApps.TopLeft()));
			COleDateTime dt2 (GetDateFromPoint (m_rectApps.BottomRight()));

			if (nChar == VK_UP)
			{
				bScroll = (sel1.GetHour() * 60 + sel1.GetMinute()) < 
						  (dt1.GetHour() * 60 + dt1.GetMinute());
			}
			else
			{
				bScroll = (sel2.GetHour() * 60 + sel2.GetMinute()) >=
						  (dt2.GetHour() * 60 + dt2.GetMinute());
			}

			if (bScroll)
			{
				OnVScroll (nChar == VK_UP ? SB_LINEUP : SB_LINEDOWN, 0, NULL);
			}
		}

		bHandled = TRUE;
	}

	return bHandled;
}

void CBCGPPlannerViewDay::GetDragScrollRect (CRect& rect)
{
	CBCGPPlannerView::GetDragScrollRect (rect);

	rect.l = m_rectTimeBar.l;
}

BOOL CBCGPPlannerViewDay::IsDateInSelection (const COleDateTime& date) const
{
	const COleDateTime& pSel1 = m_Selection[0] < m_Selection [1] 
		? m_Selection [0]
		: m_Selection [1];
	const COleDateTime& pSel2 = m_Selection[0] < m_Selection [1] 
		? m_Selection [1]
		: m_Selection [0];

	if (IsOneDay (pSel1, date) && IsOneDay (date, pSel2) &&
		pSel1.GetHour() == date.GetHour() && 
		pSel1.GetMinute() == date.GetMinute() &&
		pSel1.GetSecond() == date.GetSecond())
	{
		return TRUE;
	}

	return CBCGPPlannerView::IsDateInSelection (date);
}

COleDateTime CBCGPPlannerViewDay::GetDateFromPoint (const CPoint& point) const
{
	COleDateTime date;

	CRect rectClient (m_rectApps);

	if (!m_rectTimeBar.IsRectEmpty())
	{
		rectClient.l = m_rectTimeBar.r;
	}

	CPoint pt (point - rectClient.TopLeft());

	date = m_Date;

	int nCount = GetViewDuration();

	if(nCount > 1)
	{
		const int dxColumn = CBCGPPlannerView::round (rectClient.Width () / (double)nCount);

		date = GetDateStart() + COleDateTimeSpan (pt.x / dxColumn, 0, 0, 0);

		if(date > m_DateEnd)
		{
			date = m_DateEnd;
		}
	}

	if (point.y <= rectClient.t && m_nScrollOffset == 0)
	{
		date.SetDate (date.GetYear(), date.GetMonth(), date.GetDay());
		return date;
	}

	const int y = pt.y + m_nScrollOffset * m_nRowHeight;

	const int nMinuts = y / m_nRowHeight * 
		CBCGPPlannerView::GetTimeDeltaInMinuts (GetTimeDelta());

	date += COleDateTimeSpan (0, nMinuts / 60, nMinuts % 60, 0);

	if (!m_rectTimeBar.IsRectEmpty())
	{
		if (m_rectTimeBar.PtInRect (point))
		{
			date = COleDateTime (m_Date.GetYear(), m_Date.GetMonth(), m_Date.GetDay(),
				date.GetHour(), date.GetMinute(), date.GetSecond());
		}
	}

	return date;
}

BOOL CBCGPPlannerViewDay::OnTimer(UINT_PTR nIDEvent)
{
	CBCGPPlannerManagerCtrl* pPlanner = GetPlanner();
	ASSERT_VALID (pPlanner);

	if (pPlanner->GetSafeHwnd() == NULL)
	{
		return FALSE;
	}

	if (nIDEvent == BCGP_PLANNER_TIMER_EVENT)
	{
		BOOL bDrawTimeLine = IsCurrentTimeVisible();

		if (!bDrawTimeLine)
		{
			StopTimer (FALSE);
		}

		m_CurrentTime = COleDateTime::GetCurrentTime();

		CDC* pDC = pPlanner->GetDC();

		if (pDC != NULL && pDC != NULL)
		{
			int nOldBack = pDC->SetBkMode (TRANSPARENT);

			OnDrawTimeBar (pDC, m_rectTimeBar, bDrawTimeLine);

			pDC->SetBkMode (nOldBack);

			pPlanner->ReleaseDC (pDC);
		}

		return TRUE;
	}

	return CBCGPPlannerView::OnTimer(nIDEvent);
}

void CBCGPPlannerViewDay::OnActivate(CBCGPPlannerManagerCtrl* pPlanner, const CBCGPPlannerView* pOldView)
{
	ASSERT_VALID(pPlanner);

	if (pOldView != NULL)
	{
		m_Date = pOldView->GetDate();
	}

	m_DateStart.SetDate(m_Date.GetYear(), m_Date.GetMonth(), m_Date.GetDay());
	m_DateEnd   = m_DateStart + COleDateTimeSpan (0, 23, 59, 59);

	COleDateTime sel1 (m_Date);
	COleDateTime sel2 (m_Date);

	if (pOldView != NULL)
	{
		sel1 = pOldView->GetSelectionStart();
		sel2 = pOldView->GetSelectionEnd();
	}

	const int nMinuts = CBCGPPlannerView::GetTimeDeltaInMinuts (pPlanner->GetTimeDelta());

	sel1 = COleDateTime (m_DateStart.GetYear(), m_DateStart.GetMonth(), m_DateStart.GetDay(),
		pPlanner->GetFirstWorkingHour(), (int)(pPlanner->GetFirstWorkingMinute() / nMinuts) * nMinuts, 0);
	sel2 = sel1 + COleDateTimeSpan (0, 0, nMinuts - 1, 59);

	//SetSelection (sel1, sel2, FALSE);

	CBCGPPlannerView::OnActivate (pPlanner, NULL);

	SetSelection (sel1, sel2);
}

void CBCGPPlannerViewDay::OnDeactivate(CBCGPPlannerManagerCtrl* pPlanner)
{
	StopTimer (FALSE);

	CBCGPPlannerView::OnDeactivate (pPlanner);
}

CBCGPPlannerView::BCGP_PLANNER_TIME_DELTA CBCGPPlannerViewDay::GetTimeDelta() const
{
	ASSERT_VALID(GetPlanner());
	return GetPlanner()->GetTimeDelta();
}

int CBCGPPlannerViewDay::GetFirstWorkingHour() const
{
	ASSERT_VALID(GetPlanner());
	return GetPlanner()->GetFirstWorkingHour();
}

int CBCGPPlannerViewDay::GetFirstWorkingMinute() const
{
	ASSERT_VALID(GetPlanner());
	return GetPlanner()->GetFirstWorkingMinute();
}

int CBCGPPlannerViewDay::GetLastWorkingHour() const
{
	ASSERT_VALID(GetPlanner());
	return GetPlanner()->GetLastWorkingHour();
}

int CBCGPPlannerViewDay::GetLastWorkingMinute() const
{
	ASSERT_VALID(GetPlanner());
	return GetPlanner()->GetLastWorkingMinute();
}

void CBCGPPlannerViewDay::SetDate (const COleDateTime& date)
{
	COleDateTime dt (date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0);

	int nDays = GetViewDuration();
	
	m_DateStart = dt;
	m_DateEnd   = m_DateStart + COleDateTimeSpan (nDays - 1, 23, 59, 59);

	CBCGPPlannerView::SetDate (dt);
}

COleDateTime CBCGPPlannerViewDay::CalculateDateStart (const COleDateTime& /*date*/) const
{
	if (m_DateStart <= m_Date && m_Date <= m_DateEnd)
	{
		return m_DateStart;
	}
	else if (m_Date < m_DateStart)
	{
		return m_DateStart - COleDateTimeSpan (1, 0, 0, 0);
	}

	return m_DateStart + COleDateTimeSpan (1, 0, 0, 0);
}

COleDateTime CBCGPPlannerViewDay::GetNextDateStart() const
{
	return m_DateStart + COleDateTimeSpan (1, 0, 0, 0);
		//COleDateTimeSpan (GetViewDuration(), 0, 0, 0);
}

COleDateTime CBCGPPlannerViewDay::GetPrevDateStart() const
{
	return m_DateStart - COleDateTimeSpan (1, 0, 0, 0);
}

void CBCGPPlannerViewDay::SetSelection (const COleDateTime& sel1, const COleDateTime& sel2, BOOL bRedraw)
{
	if (!IsActive())
	{
		return;
	}

	COleDateTime s1 (sel1);
	COleDateTime s2 (sel2);

	const int nMinuts = CBCGPPlannerView::GetTimeDeltaInMinuts (GetTimeDelta());

	BOOL bEqual = s1 == s2; // only after change type

	// correct after change TimeDelta
	if (!bEqual)
	{
		if (s1.GetSecond() == 59)
		{
			s1 = COleDateTime (s1.GetYear(), s1.GetMonth(), s1.GetDay(), 
							   s1.GetHour(), (s1.GetMinute() / nMinuts) * nMinuts + nMinuts - 1, 59);
		}
		else
		{
			s1 = COleDateTime (s1.GetYear(), s1.GetMonth(), s1.GetDay(), 
							   s1.GetHour(), (s1.GetMinute() / nMinuts) * nMinuts, 0);
		}

		if (s2.GetSecond() == 59)
		{
			s2 = COleDateTime (s2.GetYear(), s2.GetMonth(), s2.GetDay(), 
							   s2.GetHour(), (s2.GetMinute() / nMinuts) * nMinuts + nMinuts - 1, 59);
		}
		else
		{
			s2 = COleDateTime (s2.GetYear(), s2.GetMonth(), s2.GetDay(), 
							   s2.GetHour(), (s2.GetMinute() / nMinuts) * nMinuts, 0);
		}
	}

	// correct after change direction of selection
	if (s1 < s2 || bEqual)
	{
		if (s1.GetSecond() == 59)
		{
			s1 = COleDateTime (s1.GetYear(), s1.GetMonth(), s1.GetDay(), 
							   s1.GetHour(), s1.GetMinute() - nMinuts + 1, 0);
		}

		if (s2.GetSecond() == 0)
		{
			s2 = COleDateTime (s2.GetYear(), s2.GetMonth(), s2.GetDay(),
				s2.GetHour(), (bEqual ? s1.GetMinute() : s2.GetMinute()) + nMinuts - 1, 59);
		}
	}
	else
	{
		if (s2.GetSecond() == 59)
		{
			s2 = COleDateTime (s2.GetYear(), s2.GetMonth(), s2.GetDay(),
							   s2.GetHour(), s2.GetMinute() - nMinuts + 1, 0);
		}

		if (s1.GetSecond() == 0)
		{
			s1 = COleDateTime (s1.GetYear(), s1.GetMonth(), s1.GetDay(), 
							   s1.GetHour(), s1.GetMinute() + nMinuts - 1, 59);
		}
	}

	const int nDays = GetViewDuration();

	if (m_SelectionAllDay.GetSize() == nDays)
	{
		COleDateTime date (GetDateStart());
		COleDateTimeSpan span1 (1, 0, 0, 0);
		COleDateTimeSpan span2 (0, 23, 59, 59);

		COleDateTime dtS1 (s1);
		COleDateTime dtS2 (s2);

		if (dtS2 < dtS1)
		{
			COleDateTime dt (dtS1);
			dtS1 = dtS2;
			dtS2 = dt;
		}

		for (int nDay = 0; nDay < nDays; nDay++)
		{
			m_SelectionAllDay[nDay] = date >= dtS1 && (date + span2) <= dtS2;

			date += span1;
		}
	}

	CBCGPPlannerView::SetSelection (s1, s2, bRedraw);
}

void CBCGPPlannerViewDay::SetDateInterval (const COleDateTime& date1, const COleDateTime& date2)
{
	ASSERT (date1 <= date2);

	m_DateStart = date1;
	m_DateEnd   = date2;

	SetDate (date1);
}

void CBCGPPlannerViewDay::StartEditAppointment (CBCGPAppointment* pApp)
{
	if (pApp == NULL)
	{
		return;
	}

	ASSERT_VALID (m_pPlanner);

	if (m_pPlanner == NULL || m_pPlanner->GetSafeHwnd() == NULL)
	{
		return;
	}

	ASSERT_VALID(pApp);

	if (m_rectApps.t <= pApp->GetRectEdit().t || 
		(pApp->IsAllDay() || pApp->IsMultiDay()))
	{
		pApp->OnEdit (GetPlanner());
	}
}

COleDateTimeSpan CBCGPPlannerViewDay::GetMinimumSpan() const
{
	const int nMinuts = CBCGPPlannerView::GetTimeDeltaInMinuts (GetTimeDelta());
	return COleDateTimeSpan (0, nMinuts / 60, nMinuts % 60, 0);
}

BOOL CBCGPPlannerViewDay::OnLButtonDown(UINT nFlags, CPoint point)
{
	BCGP_PLANNER_HITTEST hit = HitTest (point);

	if (hit == BCGP_PLANNER_HITTEST_HEADER ||
		hit == BCGP_PLANNER_HITTEST_HEADER_RESOURCE ||
		hit == BCGP_PLANNER_HITTEST_HEADER_ALLDAY)
	{
		BOOL bRepaint = FALSE;

		if (GetSelectedAppointments().GetCount() > 0)
		{
			bRepaint = TRUE;
			ClearAppointmentSelection (FALSE);
		}

		UINT nResourceID = GetResourceFromPoint (point);
		COleDateTime date (GetDateFromPoint (point));
		COleDateTime dt (date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0);

		if (dt != COleDateTime() && (dt != m_Date || 
			date != GetSelectionStart() || date != GetSelectionEnd() ||
			nResourceID != GetCurrentResourceID()))
		{
			m_Date = dt;

			COleDateTimeSpan span (0, 23, 59, 59);

			COleDateTime dtS1 (date);
			COleDateTime dtS2 (date + span);

			if ((nFlags & MK_SHIFT) != 0 && hit < BCGP_PLANNER_HITTEST_APPOINTMENT)
			{
				dtS1.SetDate(m_Selection[0].GetYear(), m_Selection[0].GetMonth(),
					m_Selection[0].GetDay());

				if (m_Selection[0] >= date)
				{
					dtS1 += span;
					dtS2 = date;
				}
			}

			SetCurrentResourceID (nResourceID, FALSE, TRUE);

			SetSelection (dtS1, dtS2);
		}
		else
		{
			if (bRepaint)
			{
				GetPlanner()->RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			}
		}

		return TRUE;
	}

	return CBCGPPlannerView::OnLButtonDown (nFlags, point);
}

BOOL CBCGPPlannerViewDay::OnMouseMove(UINT nFlags, CPoint point)
{
	if (IsCaptured())
	{
		BCGP_PLANNER_HITTEST hit = HitTestArea (point);

		if (!IsDragDrop() &&
			(hit == BCGP_PLANNER_HITTEST_HEADER ||
			 hit == BCGP_PLANNER_HITTEST_HEADER_RESOURCE ||
			 hit == BCGP_PLANNER_HITTEST_HEADER_ALLDAY) &&
			m_pCapturedAppointment == NULL)
		{
			COleDateTime date = GetDateFromPoint (point);

			if (date != COleDateTime() && 
				(date != m_dtCaptureCurrent || (date == m_dtCaptureCurrent &&
				hit != m_htCaptureAreaCurrent)))
			{
				m_htCaptureAreaCurrent = hit;
				m_dtCaptureCurrent = date;

				COleDateTimeSpan span (0, 23, 59, 59);

				COleDateTime dtS1 (m_Selection[0]);
				COleDateTime dtS2 (date + span);

				dtS1.SetDate(m_Selection[0].GetYear(), m_Selection[0].GetMonth(),
					m_Selection[0].GetDay());

				if (m_Selection[0] >= date)
				{
					dtS1 += span;
					dtS2 = date;
				}

				SetSelection (dtS1, dtS2);
			}

			return TRUE;
		}
	}

	return CBCGPPlannerView::OnMouseMove (nFlags, point);
}

BOOL CBCGPPlannerViewDay::EnsureVisible(const CBCGPAppointment* pApp, BOOL bPartialOK)
{
	ASSERT_VALID (pApp);
	if (pApp == NULL)
	{
		return FALSE;
	}

	if (pApp->IsAllDay() || pApp->IsMultiDay())
	{
		return TRUE;
	}

	CRect rect;
	BOOL bVisible = FALSE;

	const CBCGPAppointmentDSExMap& ds = pApp->GetDSDraw();
	if (ds.IsEmpty())
	{
		rect     = pApp->GetRectDraw();
		bVisible = pApp->IsVisibleDraw();
	}
	else
	{
		for (int i = 0; i < ds.GetCount(); i++)
		{
			const CBCGPAppointmentDrawStructEx* pDS = 
				(CBCGPAppointmentDrawStructEx*)ds.GetByIndex (i);

			if (pDS != NULL)
			{
				rect     = pDS->GetRect();
				bVisible = pDS->IsVisible();
				break;
			}
		}

		if (bPartialOK && !bVisible)
		{
			BOOL bPartialVisible = FALSE;
			for (int i = 0; i < ds.GetCount(); i++)
			{
				const CBCGPAppointmentDrawStructEx* pDS = 
					(CBCGPAppointmentDrawStructEx*)ds.GetByIndex (i);

				if (pDS != NULL)
				{
					if (pDS->IsVisible())
					{
						bPartialVisible = TRUE;
						break;
					}
				}
			}

			if (bPartialVisible)
			{
				return TRUE;
			}
		}
	}

	if (rect.IsRectEmpty())
	{
		return FALSE;
	}

	rect.OffsetRect (0, 1);
	if (bVisible)
	{
		if (bPartialOK || 
			(rect.t >= m_ViewRects[0].t && 
			 rect.b <= m_ViewRects[0].b))
		{
			return TRUE;
		}
	}

	int nPos = 0;
	if (rect.t < m_ViewRects[0].t)
	{
		//scroll up
		nPos = (rect.t - m_ViewRects[0].t) / m_nRowHeight;
	}
	else
	{
		//scroll down
		if (rect.Height() > m_ViewRects[0].Height())
		{
			nPos = (rect.t - m_ViewRects[0].t) / m_nRowHeight;
		}
		else
		{
			nPos = (rect.b - m_ViewRects[0].t) / m_nRowHeight - m_nScrollPage;
		}
	}

	int nPrevOffset = m_nScrollOffset;
	
	m_nScrollOffset += nPos;
	m_nScrollOffset = max (0, m_nScrollOffset);

	if (m_nScrollTotal == m_nScrollPage)
	{
		if (m_nScrollOffset > 0)
		{
			m_nScrollOffset = 1;
		}
	}
	else
	{
		m_nScrollOffset = min (m_nScrollOffset, m_nScrollTotal - m_nScrollPage + 1);
	}

	if (m_nScrollOffset == nPrevOffset)
	{
		return FALSE;
	}

	return CBCGPPlannerView::OnVScroll (0, 0, NULL);
}

#endif // BCGP_EXCLUDE_PLANNER
