
//
// BCGPPlannerViewWorkWeek.cpp: implementation of the CBCGPPlannerViewWorkWeek class.
//
//////////////////////////////////////////////////////////////////////

#include "BCGPPlannerViewWorkWeek.h"

#ifndef BCGP_EXCLUDE_PLANNER

#include "BCGPPlannerManagerCtrl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CBCGPPlannerViewWorkWeek, CBCGPPlannerViewDay)

CBCGPPlannerViewWorkWeek::CBCGPPlannerViewWorkWeek()
	: CBCGPPlannerViewDay ()
{
	m_DateEnd = m_DateStart + COleDateTimeSpan (4, 23, 59, 59);
}

CBCGPPlannerViewWorkWeek::~CBCGPPlannerViewWorkWeek()
{

}

#ifdef _DEBUG
void CBCGPPlannerViewWorkWeek::AssertValid() const
{
	CBCGPPlannerViewDay::AssertValid();
}

void CBCGPPlannerViewWorkWeek::Dump(CDumpContext& dc) const
{
	CBCGPPlannerViewDay::Dump(dc);
}
#endif

void CBCGPPlannerViewWorkWeek::OnActivate(CBCGPPlannerManagerCtrl* pPlanner, const CBCGPPlannerView* pOldView)
{
	ASSERT_VALID(pPlanner);

	if (pOldView != NULL)
	{
		m_Date = pOldView->GetDate ();
	}

	m_DateStart = CalculateDateStart (
		COleDateTime(m_Date.GetYear (), m_Date.GetMonth (), m_Date.GetDay (), 0, 0, 0));
	m_DateEnd   = m_DateStart + COleDateTimeSpan (4, 23, 59, 59);

	COleDateTime sel1 (m_Date);
	COleDateTime sel2 (m_Date);

	if (pOldView != NULL)
	{
		sel1 = pOldView->GetSelectionStart ();
		sel2 = pOldView->GetSelectionEnd ();
	}

	const int nMinuts = CBCGPPlannerView::GetTimeDeltaInMinuts (pPlanner->GetTimeDelta ());

	sel1 = COleDateTime (m_DateStart.GetYear (), m_DateStart.GetMonth (), m_DateStart.GetDay (),
		pPlanner->GetFirstWorkingHour (), (int)(pPlanner->GetFirstWorkingMinute () / nMinuts) * nMinuts, 0);
	sel2 = sel1 + COleDateTimeSpan (0, 0, nMinuts - 1, 59);

	//SetSelection (sel1, sel2, FALSE);

	CBCGPPlannerView::OnActivate (pPlanner, NULL);

	SetSelection (sel1, sel2);
}

void CBCGPPlannerViewWorkWeek::SetDate (const COleDateTime& date)
{
	COleDateTime dt (date.GetYear (), date.GetMonth (), date.GetDay (), 0, 0, 0);

	m_DateStart = CalculateDateStart (dt);
	m_DateEnd   = m_DateStart + COleDateTimeSpan (4, 23, 59, 59);

	CBCGPPlannerView::SetDate (dt);
}

COleDateTime CBCGPPlannerViewWorkWeek::CalculateDateStart (const COleDateTime& date) const
{
	return CBCGPPlannerView::GetFirstWeekDay (date, 2);//CBCGPPlannerManagerCtrl::GetFirstDayOfWeek () + 1);
}

COleDateTime CBCGPPlannerViewWorkWeek::GetNextDateStart () const
{
	return m_DateStart + COleDateTimeSpan (7, 0, 0, 0);
}

COleDateTime CBCGPPlannerViewWorkWeek::GetPrevDateStart () const
{
	return m_DateStart - COleDateTimeSpan (7, 0, 0, 0);
}

#endif // BCGP_EXCLUDE_PLANNER
