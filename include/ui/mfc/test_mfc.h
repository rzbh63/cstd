// test_mfc.h : main header file for the TEST_MFC application
//

#if !defined(AFX_TEST_MFC_H__4EFCFA06_66C9_4CEB_B1BB_A02AB8DBB6AA__INCLUDED_)
#define AFX_TEST_MFC_H__4EFCFA06_66C9_4CEB_B1BB_A02AB8DBB6AA__INCLUDED_

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"   // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTest_mfcApp:
// See test_mfc.cpp for the implementation of this class
//

class CTest_mfcApp : public CWinApp
{
public:
  CTest_mfcApp();

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTest_mfcApp)
public:
  virtual BOOL InitInstance();
  //}}AFX_VIRTUAL

  // Implementation

  //{{AFX_MSG(CTest_mfcApp)
  // NOTE - the ClassWizard will add and remove member functions here.
  //    DO NOT EDIT what you see in these blocks of generated code !
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEST_MFC_H__4EFCFA06_66C9_4CEB_B1BB_A02AB8DBB6AA__INCLUDED_)
