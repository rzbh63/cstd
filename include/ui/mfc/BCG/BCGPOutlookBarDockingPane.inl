
//
// BCGPOutlookBarDockingPane.cpp : implementation file
//

#include "bcgcbpro.h"
#include "BCGPOutlookBar.h"
#include "BCGPOutlookBarDockingPane.h"

IMPLEMENT_SERIAL(CBCGPOutlookBarDockingPane, CBCGPDockingCBWrapper, VERSIONABLE_SCHEMA | 2)

/////////////////////////////////////////////////////////////////////////////
// CBCGPOutlookBarDockingPane

CBCGPOutlookBarDockingPane::CBCGPOutlookBarDockingPane()
{
	m_pTabbedControlBarRTC = RUNTIME_CLASS (CBCGPOutlookBar);
}

CBCGPOutlookBarDockingPane::~CBCGPOutlookBarDockingPane()
{
}

BEGIN_MESSAGE_MAP(CBCGPOutlookBarDockingPane, CBCGPDockingCBWrapper)
	//{{AFX_MSG_MAP(CBCGPOutlookBarDockingPane)
	ON_WM_NCDESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBCGPOutlookBarDockingPane message handlers

void CBCGPOutlookBarDockingPane::OnNcDestroy() 
{
	CBCGPDockingCBWrapper::OnNcDestroy();
	delete this;
}
