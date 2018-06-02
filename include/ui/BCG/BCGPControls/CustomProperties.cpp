//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a sample for BCGControlBar Library Professional Edition
// Copyright (C) 1998-2009 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
#include "stdafx.h"
#include "bcgpcontrols.h"
#include "CustomProperties.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////////////
// CDurationProp class

CDurationProp::CDurationProp(const CString& strName, const COleDateTimeSpan& duration, 
		LPCTSTR lpszDescr/* = NULL*/, DWORD dwData/* = 0*/) :
	CBCGPProp (strName, _variant_t((double)duration), lpszDescr, dwData)
{
}
//****************************************************************************************
void CDurationProp::OnDrawValue (CDC* pDC, CRect rect)
{
	ASSERT_VALID (m_pWndList);

	rect.DeflateRect (0, 2);

	rect.left = m_pWndList->GetListRect ().left + 
				m_pWndList->GetPropertyColumnWidth () + TEXT_MARGIN + 1;

	DRAWITEMSTRUCT dis;
	memset (&dis, 0, sizeof (DRAWITEMSTRUCT));

	dis.CtlType = ODT_BUTTON;
	dis.hDC = pDC->GetSafeHdc ();
	dis.rcItem = rect;

	COLORREF clrTextOld = pDC->GetTextColor ();

	m_wndDuration.SetTextColor (m_bEnabled ? m_pWndList->GetTextColor () : globalData.clrGrayedText, FALSE);
	m_wndDuration.SetBackgroundColor (m_pWndList->GetBkColor (), FALSE);

	m_wndDuration.SetFont (
		IsModified () && m_pWndList->IsMarkModifiedProperties () ? 
			&m_pWndList->GetBoldFont () : m_pWndList->GetFont ());

	m_wndDuration.DrawItem (&dis);
	pDC->SetTextColor (clrTextOld);
}
//*****************************************************************************
CWnd* CDurationProp::CreateInPlaceEdit (CRect rectEdit, BOOL& bDefaultFormat)
{
	ASSERT_VALID (m_pWndList);

	CBCGPDurationCtrl* pDurationCtrl = new CBCGPDurationCtrl;
	ASSERT_VALID (pDurationCtrl);

	pDurationCtrl->SetTextColor (m_bEnabled ? m_pWndList->GetTextColor () : globalData.clrGrayedText, FALSE);
	pDurationCtrl->SetBackgroundColor (m_pWndList->GetBkColor (), FALSE);

	pDurationCtrl->SetAutoResize (FALSE);

	CRect rectSpin;
	AdjustInPlaceEditRect (rectEdit, rectSpin);

	pDurationCtrl->Create (_T(""), WS_CHILD | WS_VISIBLE, rectEdit, 
		m_pWndList, BCGPROPLIST_ID_INPLACE);
	pDurationCtrl->SetFont (IsModified () && m_pWndList->IsMarkModifiedProperties () ? 
			&m_pWndList->GetBoldFont () : m_pWndList->GetFont ());

	pDurationCtrl->SetState (CBCGPDurationCtrl::DRTN_DAYS | CBCGPDurationCtrl::DRTN_HOURS_MINS, 
		CBCGPDurationCtrl::DRTN_DAYS | CBCGPDurationCtrl::DRTN_HOURS_MINS);
	pDurationCtrl->SetDuration (GetDuration ());

	bDefaultFormat = FALSE;

	return pDurationCtrl;
}
//*******************************************************************************
void CDurationProp::OnPosSizeChanged (CRect /*rectOld*/)
{
	ASSERT_VALID (m_pWndList);

	CRect rectEdit;
	CRect rectSpin;

	AdjustInPlaceEditRect (rectEdit, rectSpin);

	if (m_wndDuration.GetSafeHwnd () == NULL)
	{
		m_wndDuration.SetAutoResize (FALSE);
		m_wndDuration.Create (_T(""), WS_CHILD, rectEdit,
								m_pWndList, (UINT)-1);

		m_wndDuration.SetState (
			CBCGPDurationCtrl::DRTN_DAYS | CBCGPDurationCtrl::DRTN_HOURS_MINS, 
			CBCGPDurationCtrl::DRTN_DAYS | CBCGPDurationCtrl::DRTN_HOURS_MINS);
		m_wndDuration.SetFont (
			IsModified () && m_pWndList->IsMarkModifiedProperties () ? 
				&m_pWndList->GetBoldFont () : m_pWndList->GetFont ());
		m_wndDuration.SetDuration (GetDuration ());
	}
	else
	{
		m_wndDuration.SetFont (
			IsModified () && m_pWndList->IsMarkModifiedProperties () ? 
				&m_pWndList->GetBoldFont () : m_pWndList->GetFont ());

		m_wndDuration.SetWindowPos (NULL, rectEdit.left, rectEdit.top,
			rectEdit.Width (), rectEdit.Height (),
			SWP_NOZORDER | SWP_NOACTIVATE);
	}

	m_wndDuration.AdjustControl (rectEdit);
}
//********************************************************************************
void CDurationProp::AdjustInPlaceEditRect (CRect& rectEdit, CRect& rectSpin)
{
	rectSpin.SetRectEmpty ();

	rectEdit = m_Rect;
//	rectEdit.DeflateRect (0, 2);

	const int nMargin = 0;
//		m_bIsModified && m_pWndList->IsMarkModifiedProperties () ? 4 : 5;

	rectEdit.left = m_pWndList->GetListRect ().left + 
					m_pWndList->GetPropertyColumnWidth () + 
					nMargin;
	rectEdit.bottom++;
}
//******************************************************************************************
BOOL CDurationProp::OnUpdateValue ()
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndInPlace);
	ASSERT_VALID (m_pWndList);
	ASSERT (::IsWindow (m_pWndInPlace->GetSafeHwnd ()));

	CBCGPDurationCtrl* pDurationCtrl = (CBCGPDurationCtrl*) m_pWndInPlace;
	ASSERT_VALID (pDurationCtrl);

	COleDateTimeSpan durationOld = GetDuration ();
	COleDateTimeSpan durationNew = pDurationCtrl->GetDuration ();

	m_varValue = _variant_t ((double) durationNew);
	m_wndDuration.SetDuration (durationNew);

	if (durationOld != durationNew)
	{
		m_pWndList->OnPropertyChanged (this);
	}

	return TRUE;
}
//********************************************************************************
void CDurationProp::SetDuration (COleDateTimeSpan duration)
{
	ASSERT_VALID (this);
	SetValue (_variant_t ((double) duration));
}
//********************************************************************************
void CDurationProp::OnSetSelection (CBCGPProp* /*pOldSel*/)
{
	Redraw ();
}
//********************************************************************************
void CDurationProp::OnKillSelection (CBCGPProp* /*pNewSel*/)
{
	Redraw ();
}
//********************************************************************************
BOOL CDurationProp::PushChar (UINT nChar)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);
	ASSERT (m_pWndList->GetCurSel () == this);
	ASSERT_VALID (m_pWndInPlace);

	if (m_bEnabled && m_bAllowEdit)
	{
		CString str;
		str = (TCHAR) nChar;
		str.MakeUpper ();

		m_pWndInPlace->SendMessage (WM_KEYDOWN, (WPARAM) str [0]);
		return TRUE;
	}

	OnEndEdit ();
	
	if (::GetCapture () == m_pWndList->GetSafeHwnd ())
	{
		ReleaseCapture ();
	}

	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// CCheckBoxProp class

CCheckBoxProp::CCheckBoxProp(const CString& strName, BOOL bCheck, LPCTSTR lpszDescr, DWORD dwData)
	:	CBCGPProp (strName, _variant_t (bool (bCheck != 0)), lpszDescr, dwData)
{
	m_rectCheck.SetRectEmpty ();
}
//************************************************************************************
void CCheckBoxProp::OnDrawName(CDC* pDC, CRect rect)
{
	m_rectCheck = rect;
	m_rectCheck.DeflateRect (1, 1);

	m_rectCheck.right = m_rectCheck.left + m_rectCheck.Height();

	rect.left = m_rectCheck.right + 1;

	CBCGPProp::OnDrawName (pDC, rect);

	OnDrawCheckBox (pDC, m_rectCheck, bool (m_varValue));
}
//************************************************************************************
void CCheckBoxProp::OnClickName(CPoint point)
{
	if (m_bEnabled && m_rectCheck.PtInRect (point)) 
	{
		m_varValue = !bool(m_varValue);
		m_pWndList->InvalidateRect (m_rectCheck);
	}
}
//************************************************************************************
BOOL CCheckBoxProp::OnDblClick (CPoint point)
{
	if (m_bEnabled && m_rectCheck.PtInRect (point)) 
	{
		return TRUE;
	}

	m_varValue = !bool(m_varValue);
	m_pWndList->InvalidateRect (m_rectCheck);
	return TRUE;
}
//************************************************************************************
void CCheckBoxProp::OnDrawCheckBox (CDC * pDC, CRect rect, BOOL bChecked)
{
	COLORREF clrTextOld = pDC->GetTextColor ();

	CBCGPVisualManager::GetInstance ()->OnDrawCheckBox (pDC, rect,
										 FALSE, bChecked, m_bEnabled);

	pDC->SetTextColor (clrTextOld);
}
//************************************************************************************
BOOL CCheckBoxProp::PushChar (UINT nChar)
{
	if (nChar == VK_SPACE)
	{
		OnDblClick (CPoint (-1, -1));
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
// CPasswordProp class

const TCHAR cPassword = _T('*');

CPasswordProp::CPasswordProp(const CString& strName, const CString& strPassword, 
							 LPCTSTR lpszDescr, DWORD dwData)
	:	CBCGPProp (strName, (LPCTSTR) strPassword, lpszDescr, dwData)
{
}
//************************************************************************************
CWnd* CPasswordProp::CreateInPlaceEdit (CRect rectEdit, BOOL& bDefaultFormat)
{
	CEdit* pWndEdit = new CEdit;

	DWORD dwStyle = WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | ES_PASSWORD;

	if (!m_bEnabled || !m_bAllowEdit)
	{
		dwStyle |= ES_READONLY;
	}

	pWndEdit->Create (dwStyle, rectEdit, m_pWndList, BCGPROPLIST_ID_INPLACE);
	pWndEdit->SetPasswordChar (cPassword);

	bDefaultFormat = TRUE;
	return pWndEdit;
}
//************************************************************************************
CString CPasswordProp::FormatProperty ()
{
	CString strVal = (LPCTSTR)(_bstr_t)m_varValue;
	
	for (int i = 0; i < strVal.GetLength (); i++)
	{
		strVal.SetAt (i, cPassword);
	}

	return strVal;
}

/////////////////////////////////////////////////////////////////////////////
// CPropSliderCtrl

CPropSliderCtrl::CPropSliderCtrl(CSliderProp* pProp, COLORREF clrBack)
{
	m_clrBack = clrBack;
	m_brBackground.CreateSolidBrush (m_clrBack);
	m_pProp = pProp;
}

CPropSliderCtrl::~CPropSliderCtrl()
{
}


BEGIN_MESSAGE_MAP(CPropSliderCtrl, CSliderCtrl)
	//{{AFX_MSG_MAP(CPropSliderCtrl)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_HSCROLL_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropSliderCtrl message handlers

HBRUSH CPropSliderCtrl::CtlColor(CDC* pDC, UINT /*nCtlColor*/) 
{
	pDC->SetBkColor (m_clrBack);
	return m_brBackground;
}
//************************************************************************************
void CPropSliderCtrl::HScroll (UINT /*nSBCode*/, UINT /*nPos*/)
{
	ASSERT_VALID (m_pProp);

	m_pProp->OnUpdateValue ();
	m_pProp->Redraw ();
}

////////////////////////////////////////////////////////////////////////////////
// CSliderProp class

CSliderProp::CSliderProp(const CString& strName, long nValue, 
							 LPCTSTR lpszDescr, DWORD dwData)
	:	CBCGPProp (strName, nValue, lpszDescr, dwData)
{
}
//************************************************************************************
CWnd* CSliderProp::CreateInPlaceEdit (CRect rectEdit, BOOL& bDefaultFormat)
{
	CPropSliderCtrl* pWndSlider = new CPropSliderCtrl (this, m_pWndList->GetBkColor ());

	rectEdit.left += rectEdit.Height () + 5;

	pWndSlider->Create (WS_VISIBLE | WS_CHILD, rectEdit, m_pWndList, BCGPROPLIST_ID_INPLACE);
	pWndSlider->SetPos ((long) m_varValue);

	bDefaultFormat = TRUE;
	return pWndSlider;
}
//******************************************************************************************
BOOL CSliderProp::OnUpdateValue ()
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndInPlace);
	ASSERT_VALID (m_pWndList);
	ASSERT (::IsWindow (m_pWndInPlace->GetSafeHwnd ()));

	long lCurrValue = m_varValue;

	CSliderCtrl* pSlider = (CSliderCtrl*) m_pWndInPlace;

	m_varValue = (long) pSlider->GetPos ();

	if (lCurrValue != (long) m_varValue)
	{
		m_pWndList->OnPropertyChanged (this);
	}

	return TRUE;
}
//******************************************************************************************
BOOL CSliderProp::OnClickValue (UINT /*uiMsg*/, CPoint /*point*/)
{
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
// CIconListProp class

const int nIconMargin = 3;

CIconListProp::CIconListProp(const CString& strName, 
							const CImageList& imageListIcons,
							int nSelectedIcon,
							CStringList* plstIconNames,
							LPCTSTR lpszDescr, DWORD dwData)
	:	CBCGPProp (strName, (long) nSelectedIcon, lpszDescr, dwData)
{
	m_imageListIcons.CreateFromImageList (imageListIcons);
	m_imageListIcons.SetTransparentColor (::GetSysColor (COLOR_3DFACE));

	if (plstIconNames != NULL)
	{
		m_lstIconNames.AddTail (plstIconNames);
		ASSERT (m_lstIconNames.GetCount () == m_imageListIcons.GetCount ());
	}

	for (int i = 0; i < m_imageListIcons.GetCount (); i++)
	{
		CString strItem;
		strItem.Format (_T("%d"), i);

		AddOption (strItem);
	}

	AllowEdit (FALSE);
}

CComboBox* CIconListProp::CreateCombo (CWnd* pWndParent, CRect rect)
{
	rect.bottom = rect.top + 400;

	CIconCombo* pWndCombo = new CIconCombo (m_imageListIcons, m_lstIconNames);
	if (!pWndCombo->Create (WS_CHILD | CBS_NOINTEGRALHEIGHT | CBS_DROPDOWNLIST | WS_VSCROLL | CBS_OWNERDRAWFIXED | CBS_HASSTRINGS,
		rect, pWndParent, BCGPROPLIST_ID_INPLACE))
	{
		delete pWndCombo;
		return NULL;
	}

	return pWndCombo;
}

void CIconListProp::OnDrawValue (CDC* pDC, CRect rect)
{
	ASSERT_VALID (m_pWndList);
	ASSERT_VALID (pDC);

	CString strVal = (LPCTSTR)(_bstr_t) m_varValue;
	if (strVal.IsEmpty () || m_imageListIcons.GetCount () == 0)
	{
		return;
	}
	
	int nIndex = -1;
#if _MSC_VER < 1400
	_stscanf (strVal, _T("%d"), &nIndex);
#else
	_stscanf_s (strVal, _T("%d"), &nIndex);
#endif

	if (nIndex < 0)
	{
		return;
	}

	COLORREF clrTextOld = pDC->GetTextColor ();

	CFont* pOldFont = pDC->SelectObject (
			IsModified () && m_pWndList->IsMarkModifiedProperties () ? 
				&m_pWndList->GetBoldFont () : m_pWndList->GetFont ());

	CRect rectImage = rect;
	rectImage.right = rectImage.left + rectImage.Height ();
	rectImage.DeflateRect (1, 1);

	CBCGPDrawState ds;
	m_imageListIcons.PrepareDrawImage (ds, rectImage.Size ());
	m_imageListIcons.Draw (pDC, rectImage.left, rectImage.top, nIndex);
	m_imageListIcons.EndDrawImage (ds);

	if (!m_lstIconNames.IsEmpty ())
	{
		CString str = m_lstIconNames.GetAt (m_lstIconNames.FindIndex (nIndex));

		rect.left = rectImage.right + 2 * nIconMargin;

		pDC->DrawText (str, rect, DT_SINGLELINE | DT_VCENTER);
	}

	pDC->SetTextColor (clrTextOld);
	pDC->SelectObject (pOldFont);

	m_bValueIsTrancated = FALSE;
}

CWnd* CIconListProp::CreateInPlaceEdit (CRect rectEdit, BOOL& bDefaultFormat)
{
	CWnd* pWnd = CBCGPProp::CreateInPlaceEdit (rectEdit, bDefaultFormat);
	if (pWnd != NULL)
	{
		pWnd->ShowWindow (SW_HIDE);
	}

	return pWnd;
}

/////////////////////////////////////////////////////////////////////////////
// CIconCombo

CIconCombo::CIconCombo(CBCGPToolBarImages& imageListIcons, CStringList& lstIconNames) :
	m_imageListIcons (imageListIcons),
	m_lstIconNames (lstIconNames)
{
}

CIconCombo::~CIconCombo()
{
}

BEGIN_MESSAGE_MAP(CIconCombo, CComboBox)
	//{{AFX_MSG_MAP(CIconCombo)
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIconCombo message handlers

void CIconCombo::OnDrawItem(int /*nIDCtl*/, LPDRAWITEMSTRUCT lpDIS) 
{
	CDC* pDC = CDC::FromHandle (lpDIS->hDC);
	ASSERT_VALID (pDC);

	CRect rect = lpDIS->rcItem;
	int nIcon = lpDIS->itemID;

	HBRUSH		brBackground;
	COLORREF	clText; 

	if (lpDIS->itemState & ODS_SELECTED)  
	{
		brBackground = GetSysColorBrush (COLOR_HIGHLIGHT); 
		clText = globalData.clrTextHilite; 
	} 
	else 
	{
		brBackground = GetSysColorBrush (COLOR_WINDOW); 
		clText = globalData.clrWindowText;
	} 

	if (lpDIS->itemAction & (ODA_DRAWENTIRE | ODA_SELECT))
	{
		::FillRect (lpDIS->hDC, &rect, brBackground);
	}

	if (nIcon < 0)
	{
		return;
	}

	CBCGPDrawState ds;
	m_imageListIcons.PrepareDrawImage (ds);
	m_imageListIcons.Draw (pDC, rect.left + nIconMargin, rect.top + nIconMargin, nIcon);
	m_imageListIcons.EndDrawImage (ds);

	if (!m_lstIconNames.IsEmpty ())
	{
		CString str = m_lstIconNames.GetAt (m_lstIconNames.FindIndex (nIcon));
		CFont* pOldFont = pDC->SelectObject (&globalData.fontRegular);

		pDC->SetBkMode (TRANSPARENT);
		pDC->SetTextColor (clText);

		CRect rectText = rect;
		rectText.left += m_imageListIcons.GetImageSize ().cx + 2 * nIconMargin;

		pDC->DrawText (str, rectText, DT_SINGLELINE | DT_VCENTER);

		pDC->SelectObject (pOldFont);
	}
}

void CIconCombo::OnMeasureItem(int /*nIDCtl*/, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	int nTextWidth = 0;
	int nTextHeight = 0;

	if (!m_lstIconNames.IsEmpty ())
	{
		nTextHeight = globalData.GetTextHeight ();

		CClientDC dc (this);
		CFont* pOldFont = dc.SelectObject (&globalData.fontRegular);

		for (POSITION pos = m_lstIconNames.GetHeadPosition (); pos != NULL;)
		{
			CString str = m_lstIconNames.GetNext (pos);

			nTextWidth = max (nTextWidth, dc.GetTextExtent (str).cx + nIconMargin);
		}

		dc.SelectObject (pOldFont);
	}

	lpMeasureItemStruct->itemWidth = m_imageListIcons.GetImageSize ().cx + nTextWidth + 2 * nIconMargin;
	lpMeasureItemStruct->itemHeight = max (nTextHeight, m_imageListIcons.GetImageSize ().cy + 2 * nIconMargin);
}

////////////////////////////////////////////////////////////////////////////////
// CComboBoxExProp class

CComboBoxExProp::CComboBoxExProp(const CString& strName, 
							const CString& strValue,
							LPCTSTR lpszDescr, DWORD dwData,
							CImageList* pImageList)
	:	CBCGPProp (strName, (LPCTSTR) strValue, lpszDescr, dwData),
		m_pImageList (pImageList)
{
}

CComboBox* CComboBoxExProp::CreateCombo (CWnd* pWndParent, CRect rect)
{
	rect.bottom = rect.top + 400;

	CComboBoxEx* pWndCombo = new CComboBoxEx;

	if (!pWndCombo->Create (WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
		rect, pWndParent, BCGPROPLIST_ID_INPLACE))
	{
		delete pWndCombo;
		return NULL;
	}

	if (m_pImageList != NULL)
	{
		pWndCombo->SetImageList (m_pImageList);
	}

	return pWndCombo;
}

BOOL CComboBoxExProp::OnEdit (LPPOINT lptClick)
{
	if (!CBCGPProp::OnEdit (lptClick))
	{
		return FALSE;
	}

	CComboBoxEx* pWndComboEx = DYNAMIC_DOWNCAST (CComboBoxEx, m_pWndCombo);
	if (pWndComboEx == NULL)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	pWndComboEx->ResetContent ();

	int i = 0;

	COMBOBOXEXITEM item;
	memset (&item, 0, sizeof (item));

	item.mask = CBEIF_IMAGE | CBEIF_INDENT |
				CBEIF_SELECTEDIMAGE | CBEIF_TEXT;

	for (POSITION pos = m_lstOptions.GetHeadPosition (); pos != NULL; i++)
	{
		CString strItem = m_lstOptions.GetNext (pos);

		item.iItem = i;
		item.iSelectedImage = item.iImage = m_lstIcons [i];
		item.iIndent = m_lstIndents [i];
		item.pszText = (LPTSTR)(LPCTSTR) strItem;
		item.cchTextMax = strItem.GetLength ();

		pWndComboEx->InsertItem (&item);
	}

	return TRUE;
}

BOOL CComboBoxExProp::AddOption (LPCTSTR lpszOption, int nIcon, int nIndent)
{
	if (!CBCGPProp::AddOption (lpszOption))
	{
		return FALSE;
	}

	m_lstIcons.Add (nIcon);
	m_lstIndents.Add (nIndent);

	return TRUE;
}

COwnerDrawDescrProp::COwnerDrawDescrProp (const CString& strName,
										  const _variant_t& varValue) :
	CBCGPProp (strName, varValue)
{
}

void COwnerDrawDescrProp::OnDrawDescription (CDC* pDC, CRect rect)
{
	CBCGPDrawManager dm (*pDC);
	dm.FillGradient2 (rect, RGB (102, 200, 238), RGB (0, 129, 185), 45);

	CFont* pOldFont = pDC->SelectObject (&globalData.fontBold);

	CString strText = _T("Custom Text");

	pDC->SetTextColor (RGB (0, 65, 117));
	pDC->DrawText (strText, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	rect.OffsetRect (-2, -2);

	pDC->SetTextColor (RGB (155, 251, 255));
	pDC->DrawText (strText, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	pDC->SelectObject (pOldFont);
}

CTwoButtonsProp::CTwoButtonsProp (const CString& strName,
								  const _variant_t& varValue) :
	CBCGPProp (strName, varValue)
{
	CBitmap bmp;
	bmp.LoadBitmap (IDB_BUTTONS);

	m_images.Create (14, 14, ILC_MASK | ILC_COLOR8, 0, 0);
	m_images.Add (&bmp, RGB (255, 0, 255));
}

void CTwoButtonsProp::AdjustButtonRect ()
{
	CBCGPProp::AdjustButtonRect ();
	m_rectButton.left -= m_rectButton.Width ();
}

void CTwoButtonsProp::OnClickButton (CPoint point)
{
	BOOL bIsLeft = point.x < m_rectButton.CenterPoint ().x;
	AfxMessageBox (bIsLeft ? _T("Left button clicked") : _T("Right button clicked"));
}

void CTwoButtonsProp::OnDrawButton (CDC* pDC, CRect rectButton)
{
	IMAGEINFO imgInfo;
	memset (&imgInfo, 0, sizeof (IMAGEINFO));
	m_images.GetImageInfo  (0, &imgInfo);

	CSize sizeImage (
		imgInfo.rcImage.right - imgInfo.rcImage.left,
		imgInfo.rcImage.bottom - imgInfo.rcImage.top);

	for (int i = 0; i < 2; i++)
	{
		CBCGPToolbarButton button;

		CRect rect = rectButton;

		if (i == 0)
		{
			rect.right = rect.left + rect.Width () / 2;
		}
		else
		{
			rect.left = rect.right - rect.Width () / 2;
		}

		CBCGPVisualManager::BCGBUTTON_STATE state = 
			CBCGPVisualManager::ButtonsIsHighlighted;

		CBCGPVisualManager::GetInstance ()->OnFillButtonInterior (pDC,
			&button, rect, state);

		m_images.Draw (pDC, i, CPoint (
			rect.left + max (0, (rect.Width () - sizeImage.cx) / 2),
			rect.top + max (0, (rect.Height () - sizeImage.cy) / 2)), 
			ILD_NORMAL);

		CBCGPVisualManager::GetInstance ()->OnDrawButtonBorder (pDC, &button, rect, state);
	}
}

CCustomDlgProp::CCustomDlgProp (const CString& strName,
								const _variant_t& varValue) :
	CBCGPProp (strName, varValue)
{
}

void CCustomDlgProp::OnClickButton (CPoint /*point*/)
{
	AfxMessageBox (_T("Show your dialog here..."));
	SetValue (_T("New value"));
}
