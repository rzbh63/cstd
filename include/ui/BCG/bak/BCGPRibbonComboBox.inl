
//
// BCGPRibbonComboBox.cpp: implementation of the CBCGPRibbonComboBox class.
//
//////////////////////////////////////////////////////////////////////

//#include "BCGCBPro.h"
#include "BCGPRibbonComboBox.h"
#include "BCGPRibbonCategory.h"
#include "BCGPRibbonPanelMenu.h"
#include "BCGPVisualManager.h"
#include "BCGPDropDownList.h"
#include "BCGPRibbonBar.h"
#include "BCGPRibbonPanel.h"
#include "MenuImages.h"
#include "trackmouse.h"

#include "BCGPToolbarMenuButton.h"
#include "bcgprores.h"
#include "BCGPRibbonEdit.h"
#include "BCGPRibbonFloaty.h"

#ifndef BCGP_EXCLUDE_RIBBON

static const int iDefaultComboHeight = 150;
static const int iDefaultWidth = 108;

/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonComboBox

IMPLEMENT_DYNCREATE(CBCGPRibbonComboBox, CBCGPRibbonEdit)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPRibbonComboBox::CBCGPRibbonComboBox(
	UINT	uiID,
	BOOL	bHasEditBox,
	int		nWidth,
	LPCTSTR	lpszLabel,
	int		nImage) :
	CBCGPRibbonEdit (uiID, nWidth == -1 ? iDefaultWidth : nWidth, lpszLabel, nImage)
{
	CommonInit();

	m_bHasEditBox = bHasEditBox;
}
//****
CBCGPRibbonComboBox::CBCGPRibbonComboBox()
{
	CommonInit();
}
//****
void CBCGPRibbonComboBox::CommonInit()
{
	m_iSelIndex = -1;
	m_nDropDownHeight = iDefaultComboHeight;
	m_bHasEditBox = FALSE;
	m_bHasDropDownList = TRUE;
	m_nMenuArrowMargin = 3;
	m_bResizeDropDownList = TRUE;
}
//****
CBCGPRibbonComboBox::~CBCGPRibbonComboBox()
{
	ClearData();
}
//****
void CBCGPRibbonComboBox::ClearData()
{
}
//****
INT_PTR CBCGPRibbonComboBox::AddItem (LPCTSTR lpszItem, DWORD_PTR dwData)
{
	ASSERT_VALID (this);
	ASSERT (lpszItem != NULL);

	if (FindItem (lpszItem) < 0)
	{
		m_lstItems.AddTail (lpszItem);
		m_lstItemData.AddTail (dwData);
	}

	return m_lstItems.GetCount() - 1;
}
//****
LPCTSTR CBCGPRibbonComboBox::GetItem (int iIndex) const
{
	ASSERT_VALID (this);

	if (iIndex == -1)	// Current selection
	{
		if ((iIndex = m_iSelIndex) == -1)	
		{
			return NULL;
		}
	}

	POSITION pos = m_lstItems.FindIndex (iIndex);
	if (pos == NULL)
	{
		return NULL;
	}

	return m_lstItems.GetAt (pos);
}
//****
DWORD_PTR CBCGPRibbonComboBox::GetItemData (int iIndex) const
{
	ASSERT_VALID (this);

	if (iIndex == -1)	// Current selection
	{
		if ((iIndex = m_iSelIndex) == -1)	
		{
			return 0;
		}
	}

	POSITION pos = m_lstItemData.FindIndex (iIndex);
	if (pos == NULL)
	{
		return 0;
	}

	return m_lstItemData.GetAt (pos);
}
//****
void CBCGPRibbonComboBox::RemoveAllItems()
{
	ASSERT_VALID (this);

	ClearData();

	m_lstItems.RemoveAll();
	m_lstItemData.RemoveAll();
	m_strEdit.Empty();

	m_iSelIndex = -1;

	Redraw();
}
//****
BOOL CBCGPRibbonComboBox::SelectItem (int iIndex)
{
	ASSERT_VALID (this);

	if (iIndex >= m_lstItems.GetCount())
	{
		return FALSE;
	}

	m_iSelIndex = max (-1, iIndex);
	
	LPCTSTR lpszText = GetItem (m_iSelIndex);

	m_strEdit = lpszText == NULL ? _T("") : lpszText;

	if (m_pWndEdit->GetSafeHwnd() != NULL)
	{
		m_pWndEdit->SetWindowText (m_strEdit);
	}

	if (!m_bDontNotify)
	{
		CBCGPRibbonBar* pRibbonBar = GetTopLevelRibbonBar();
		if (pRibbonBar != NULL)
		{
			ASSERT_VALID (pRibbonBar);

			CArray<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*> arButtons;
			pRibbonBar->GetElementsByID (m_nID, arButtons, TRUE);

			for (int i = 0; i < arButtons.GetSize(); i++)
			{
				CBCGPRibbonComboBox* pOther =
					DYNAMIC_DOWNCAST (CBCGPRibbonComboBox, arButtons [i]);

				if (pOther != NULL && pOther != this)
				{
					ASSERT_VALID (pOther);

					pOther->m_bDontNotify = TRUE;
					pOther->SelectItem (iIndex);
					pOther->m_bDontNotify = FALSE;
				}
			}
		}
	}

	Redraw();
	return TRUE;
}
//****
BOOL CBCGPRibbonComboBox::SelectItem (DWORD_PTR dwData)
{
	ASSERT_VALID (this);

	int iIndex = 0;
	for (POSITION pos = m_lstItemData.GetHeadPosition(); pos != NULL; iIndex ++)
	{
		if (m_lstItemData.GetNext (pos) == dwData)
		{
			return SelectItem (iIndex);
		}
	}

	return FALSE;
}
//****
BOOL CBCGPRibbonComboBox::SelectItem (LPCTSTR lpszText)
{
	ASSERT_VALID (this);
	ASSERT (lpszText != NULL);

	int iIndex = FindItem (lpszText);
	if (iIndex < 0)
	{
		return FALSE;
	}

	return SelectItem (iIndex);
}
//****
BOOL CBCGPRibbonComboBox::DeleteItem (int iIndex)
{
	ASSERT_VALID (this);

	if (iIndex < 0 || iIndex >= m_lstItems.GetCount())
	{
		return FALSE;
	}

	POSITION pos = m_lstItems.FindIndex (iIndex);
	if (pos == NULL)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	m_lstItems.RemoveAt (pos);

	pos = m_lstItemData.FindIndex (iIndex);
	if (pos == NULL)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	m_lstItemData.RemoveAt (pos);

	if (iIndex == m_iSelIndex)
	{
		int iSelIndex = m_iSelIndex;
		if (iSelIndex >= m_lstItems.GetCount())
		{
			iSelIndex = (int) m_lstItems.GetCount() - 1;
		}

		SelectItem (iSelIndex);
	}

	return TRUE;
}
//****
BOOL CBCGPRibbonComboBox::DeleteItem (DWORD_PTR dwData)
{
	ASSERT_VALID (this);

	int iIndex = 0;
	for (POSITION pos = m_lstItemData.GetHeadPosition(); pos != NULL; iIndex ++)
	{
		if (m_lstItemData.GetNext (pos) == dwData)
		{
			return DeleteItem (iIndex);
		}
	}

	return FALSE;
}
//****
BOOL CBCGPRibbonComboBox::DeleteItem (LPCTSTR lpszText)
{
	ASSERT_VALID (this);
	ASSERT (lpszText != NULL);

	int iIndex = FindItem (lpszText);
	if (iIndex < 0)
	{
		return FALSE;
	}

	return DeleteItem (iIndex);
}
//****
int CBCGPRibbonComboBox::FindItem (LPCTSTR lpszText) const
{
	ASSERT_VALID (this);
	ASSERT (lpszText != NULL);

	int iIndex = 0;
	for (POSITION pos = m_lstItems.GetHeadPosition(); pos != NULL; iIndex++)
	{
		if (m_lstItems.GetNext (pos).CompareNoCase (lpszText) == 0)
		{
			return iIndex;
		}
	}

	return -1;
}
//****
void CBCGPRibbonComboBox::SetDropDownHeight (int nHeight)
{
	ASSERT_VALID (this);
	m_nDropDownHeight = nHeight;
}
//****
CSize CBCGPRibbonComboBox::GetIntermediateSize (CDC* pDC)
{
	ASSERT_VALID (this);

	CSize size = CBCGPRibbonEdit::GetIntermediateSize (pDC);

	size.w += GetDropDownImageWidth() + 2 * m_nMenuArrowMargin + 1;

	return size;
}
//****
void CBCGPRibbonComboBox::OnDraw (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	OnDrawLabelAndImage (pDC);

	BOOL bIsHighlighted = m_bIsHighlighted;

	if (m_bIsFocused)
	{
		m_bIsHighlighted = TRUE;
	}

	if (IsDisabled())
	{
		m_bIsHighlighted = FALSE;
	}

	CRect rectSaved = m_rect;
	m_rect.l += m_nLabelImageWidth;

	CBCGPVisualManager::GetInstance()->OnFillRibbonButton (pDC, this);

	if (m_pWndEdit->GetSafeHwnd() == NULL)
	{
		CRect rectText = m_rectCommand;
		rectText.DeflateRect (m_szMargin);

		DoDrawText (pDC, m_strEdit, rectText, DT_SINGLELINE | DT_VCENTER);
	}

	CBCGPVisualManager::GetInstance()->OnDrawRibbonButtonBorder 
			(pDC, this);

	CBCGPToolbarComboBoxButton buttonDummy;
	buttonDummy.m_bIsRibbon = TRUE;

	BOOL bIsDropDownHighlighted = IsMenuAreaHighlighted() || m_bIsFocused ||
		m_bIsEditFocused ||
		(bIsHighlighted && !m_bHasEditBox);

	CBCGPVisualManager::GetInstance()->OnDrawComboDropButton (
		pDC, m_rectMenu, IsDisabled(), IsDroppedDown(),
		bIsDropDownHighlighted,
		&buttonDummy);

	m_bIsHighlighted = bIsHighlighted;
	m_rect = rectSaved;
}
//****
void CBCGPRibbonComboBox::OnLButtonDown (CPoint point)
{
	ASSERT_VALID (this);

	CBCGPBaseRibbonElement::OnLButtonDown (point);

	if ((m_rectMenu.PtInRect (point) || !m_bHasEditBox) && !IsDroppedDown())
	{
		DropDownList();
	}
}
//****
void CBCGPRibbonComboBox::OnLButtonUp (CPoint /*point*/)
{
	ASSERT_VALID (this);
}
//****
void CBCGPRibbonComboBox::CopyFrom (const CBCGPBaseRibbonElement& s)
{
	ASSERT_VALID (this);

	CBCGPRibbonEdit::CopyFrom (s);

	CBCGPRibbonComboBox& src = (CBCGPRibbonComboBox&) s;

	m_strEdit = src.m_strEdit;
	m_bHasEditBox = src.m_bHasEditBox;

	if (m_pWndEdit != NULL)
	{
		m_pWndEdit->DestroyWindow();
		delete m_pWndEdit;
		m_pWndEdit = NULL;
	}

	m_lstItems.RemoveAll();
	m_lstItems.AddTail (&src.m_lstItems);

	m_lstItemData.RemoveAll();
	m_lstItemData.AddTail (&src.m_lstItemData);

	m_iSelIndex = src.m_iSelIndex;
	m_nDropDownHeight = src.m_nDropDownHeight;

	m_bResizeDropDownList = src.m_bResizeDropDownList;
}
//****
void CBCGPRibbonComboBox::DropDownList()
{
	ASSERT_VALID (this);

	if (IsDisabled())
	{
		return;
	}

	if (m_pWndEdit->GetSafeHwnd() != NULL && !m_pWndEdit->IsWindowVisible())
	{
		return;
	}

	if (CBCGPPopupMenu::GetActiveMenu() != NULL)
	{
		if (CBCGPPopupMenu::GetActiveMenu()->GetMenuBar() != m_pParentMenu)
		{
			CBCGPPopupMenu::GetActiveMenu()->SendMessage (WM_CLOSE);
			return;
		}
	}

	CBCGPBaseRibbonElement::OnShowPopupMenu();

	CBCGPDropDownList* pList = new CBCGPDropDownList (this);
	pList->SetParentRibbonElement (this);

	for (POSITION pos = m_lstItems.GetHeadPosition(); pos != NULL;)
	{
		pList->AddString (m_lstItems.GetNext (pos));
	}

	pList->SetCurSel (m_iSelIndex);
	pList->SetMaxHeight (m_nDropDownHeight);
	pList->SetMinWidth (m_rect.Width());

	CWnd* pWndParent = GetParentWnd();
	if (pWndParent == NULL)
	{
		ASSERT (FALSE);
		return;
	}

	const BOOL bIsRTL = (pWndParent->GetExStyle() & WS_EX_LAYOUTRTL);

	CRect rect = m_rectCommand.IsRectEmpty() ? m_rect : m_rectCommand;
	pWndParent->ClientToScreen (&rect);

	SetDroppedDown (pList);

	if (m_pParent != NULL)
	{
		ASSERT_VALID (m_pParent);
		m_pParent->HighlightPanel (NULL, CPoint (-1, -1));
	}

	if (m_pWndEdit->GetSafeHwnd() != NULL)
	{
		m_pWndEdit->SetFocus();
		m_pWndEdit->SetSel (0, -1);
	}

	if (m_bResizeDropDownList)
	{
		pList->EnableVertResize (2 * globalData.GetTextHeight());
	}

	pList->Track (CPoint (
		bIsRTL ? rect.r : rect.l, rect.b), pWndParent->GetOwner());
}
//****
void CBCGPRibbonComboBox::OnSelectItem (int nItem)
{
	ASSERT_VALID (this);

	SelectItem (nItem);

	NotifyCommand (TRUE);

	if (m_pParentMenu != NULL)
	{
		ASSERT_VALID (m_pParentMenu);

		if (m_pParentMenu->IsFloaty())
		{
			CBCGPRibbonFloaty* pFloaty = DYNAMIC_DOWNCAST (
				CBCGPRibbonFloaty, m_pParentMenu->GetParent());

			if (pFloaty != NULL && !pFloaty->IsContextMenuMode())
			{
				if (m_pWndEdit->GetSafeHwnd() != NULL && m_pWndEdit->GetTopLevelFrame() != NULL)
				{
					m_pWndEdit->GetTopLevelFrame()->SetFocus();
				}

				Redraw();
				return;
			}
		}

		CFrameWnd* pParentFrame = BCGPGetParentFrame (m_pParentMenu);
		ASSERT_VALID (pParentFrame);

		pParentFrame->PostMessage (WM_CLOSE);
	}
	else
	{
		if (m_pWndEdit->GetSafeHwnd() != NULL && m_pWndEdit->GetTopLevelFrame() != NULL)
		{
			m_bNotifyCommand = FALSE;
			m_pWndEdit->GetTopLevelFrame()->SetFocus();
		}

		Redraw();
	}
}
//****
void CBCGPRibbonComboBox::OnAfterChangeRect (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	CBCGPRibbonButton::OnAfterChangeRect (pDC);

	if (m_rect.IsRectEmpty())
	{
		if (m_pWndEdit->GetSafeHwnd() != NULL)
		{
			m_pWndEdit->ShowWindow (SW_HIDE);
		}
		return;
	}

	CRect rectCommandOld = m_rectCommand;

	m_Location = RibbonElementSingleInGroup;

	m_rectMenu = m_rect;
	m_rectMenu.l = m_rectMenu.r - GetDropDownImageWidth() - 2 * m_nMenuArrowMargin;
	
	m_rectCommand = m_rect;
	m_rectCommand.r = m_rectMenu.l;
	m_rectCommand.l += m_nLabelImageWidth;

	int cx = m_bFloatyMode ? m_nWidthFloaty : m_nWidth;
	if (globalData.GetRibbonImageScale() > 1.)
	{
		cx = (int)(.5 + globalData.GetRibbonImageScale() * cx);
	}

	if (m_rectCommand.Width() > cx)
	{
		m_rectCommand.l = m_rectCommand.r - cx;
	}

	m_rectMenu.DeflateRect (1, 1);

	m_bMenuOnBottom = FALSE;

	if (!m_bHasEditBox)
	{
		return;
	}

	if (m_pWndEdit == NULL)
	{
		DWORD dwEditStyle = WS_CHILD | ES_WANTRETURN | 
							ES_AUTOHSCROLL | WS_TABSTOP;

		CWnd* pWndParent = GetParentWnd();
		ASSERT_VALID (pWndParent);

		if ((m_pWndEdit = CreateEdit (pWndParent, dwEditStyle)) == NULL)
		{
			return;
		}

		m_pWndEdit->SendMessage (EM_SETTEXTMODE, TM_PLAINTEXT);
		m_pWndEdit->SetFont (GetTopLevelRibbonBar()->GetFont());
		m_pWndEdit->SetWindowText (m_strEdit);
	}

	if (rectCommandOld != m_rectCommand || !m_pWndEdit->IsWindowVisible())
	{
		CRect rectEdit = m_rectCommand;
		rectEdit.DeflateRect (m_szMargin.w, m_szMargin.h, 0, m_szMargin.h);

		m_pWndEdit->SetWindowPos (NULL, 
			rectEdit.l, rectEdit.t,
			rectEdit.Width(), rectEdit.Height(),
			SWP_NOZORDER | SWP_NOACTIVATE);

		m_pWndEdit->ShowWindow (SW_SHOWNOACTIVATE);
	}
}
//****
BOOL CBCGPRibbonComboBox::OnDrawDropListItem (CDC* /*pDC*/, 
											  int /*nIndex*/, 
											  CBCGPToolbarMenuButton* /*pItem*/,
											  BOOL /*bHighlight*/)
{
	ASSERT_VALID (this);
	return FALSE;
}
//****
CSize CBCGPRibbonComboBox::OnGetDropListItemSize (
		CDC* /*pDC*/, 
		int /*nIndex*/, 
		CBCGPToolbarMenuButton* /*pItem*/,
		CSize /*sizeDefault*/)
{
	ASSERT_VALID (this);
	return CSize (0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonFontComboBox

BOOL CBCGPRibbonFontComboBox::m_bDrawUsingFont = FALSE;

const int nImageHeight_rcb = 16;
const int nImageWidth = 16;
const int nImageMargin = 6;

IMPLEMENT_DYNCREATE(CBCGPRibbonFontComboBox, CBCGPRibbonComboBox)

CBCGPRibbonFontComboBox::CBCGPRibbonFontComboBox(
	UINT	uiID,
	int		nFontType,
	BYTE	nCharSet,
	BYTE	nPitchAndFamily,
	int		nWidth) :
	CBCGPRibbonComboBox (uiID, TRUE, nWidth),
	m_nFontType (DEVICE_FONTTYPE | RASTER_FONTTYPE | TRUETYPE_FONTTYPE),
	m_nCharSet (DEFAULT_CHARSET),
	m_nPitchAndFamily (DEFAULT_PITCH)
{
	BuildFonts (nFontType, nCharSet, nPitchAndFamily);
}
//****
CBCGPRibbonFontComboBox::CBCGPRibbonFontComboBox() :
	m_nFontType (DEVICE_FONTTYPE | RASTER_FONTTYPE | TRUETYPE_FONTTYPE),
	m_nCharSet (DEFAULT_CHARSET),
	m_nPitchAndFamily (DEFAULT_PITCH)
{
}
//****
CBCGPRibbonFontComboBox::~CBCGPRibbonFontComboBox()
{
	ClearData();
}
//****
void CBCGPRibbonFontComboBox::CopyFrom (const CBCGPBaseRibbonElement& s)
{
	ASSERT_VALID (this);

	CBCGPRibbonComboBox::CopyFrom (s);

	CBCGPRibbonFontComboBox& src = (CBCGPRibbonFontComboBox&) s;

	m_nFontType = src.m_nFontType;
	m_nCharSet = src.m_nCharSet;
	m_nPitchAndFamily = src.m_nPitchAndFamily;
}
//****
void CBCGPRibbonFontComboBox::DropDownList()
{
	ASSERT_VALID (this);

	BuildFonts (m_nFontType, m_nCharSet, m_nPitchAndFamily);

	CBCGPRibbonComboBox::DropDownList();
}
//****
void CBCGPRibbonFontComboBox::BuildFonts (
		int nFontType/* = DEVICE_FONTTYPE | RASTER_FONTTYPE | TRUETYPE_FONTTYPE*/,
		BYTE nCharSet/* = DEFAULT_CHARSET*/,
		BYTE nPitchAndFamily/* = DEFAULT_PITCH*/)
{
	if (m_lstItems.IsEmpty() || 
		m_nFontType       != nFontType || 
		m_nCharSet        != nCharSet || 
		m_nPitchAndFamily != nPitchAndFamily)
	{
		m_nFontType       = nFontType;
		m_nCharSet        = nCharSet;
		m_nPitchAndFamily = nPitchAndFamily;

		RebuildFonts();
	}
}
//****
void CBCGPRibbonFontComboBox::RebuildFonts()
{
	RemoveAllItems();

	CObList lstFonts;

	CBCGPToolbarFontCombo tlbFontCombo (
		&lstFonts, m_nFontType, m_nCharSet, m_nPitchAndFamily);

	POSITION pos = NULL;

	for (pos = lstFonts.GetHeadPosition(); pos != NULL;)
	{
		CBCGPFontDesc* pDesc = (CBCGPFontDesc*) lstFonts.GetNext (pos);
		ASSERT_VALID (pDesc);

		if ((m_nFontType & pDesc->m_nType) != 0)
		{
			BOOL bIsUnique = GetFontsCount (pDesc->m_strName, lstFonts) <= 1;
			AddItem (bIsUnique ? pDesc->m_strName : pDesc->GetFullName(), (DWORD_PTR) pDesc);
		}
	}

	// Delete unused items:
	for (pos = lstFonts.GetHeadPosition(); pos != NULL;)
	{
		CBCGPFontDesc* pDesc = (CBCGPFontDesc*) lstFonts.GetNext (pos);
		ASSERT_VALID (pDesc);

		if ((m_nFontType & pDesc->m_nType) == 0)
		{
			delete pDesc;
		}
	}
}
//****
int CBCGPRibbonFontComboBox::GetFontsCount (LPCTSTR lpszName,
											  const CObList& lstFonts)
{
	ASSERT (!lstFonts.IsEmpty());

	int nCount = 0;

	for (POSITION pos = lstFonts.GetHeadPosition(); pos != NULL;)
	{
		CBCGPFontDesc* pDesc = (CBCGPFontDesc*) lstFonts.GetNext (pos);
		ASSERT_VALID (pDesc);

		if (pDesc->m_strName == lpszName)
		{
			nCount++;
		}
	}

	return nCount;
}
//****
void CBCGPRibbonFontComboBox::ClearData()
{
	ASSERT_VALID (this);

	if (m_pOriginal != NULL)
	{
		return;
	}

	for (POSITION pos = m_lstItemData.GetHeadPosition(); pos != NULL;)
	{
		CBCGPFontDesc* pDesc = (CBCGPFontDesc*) m_lstItemData.GetNext (pos);
		ASSERT_VALID (pDesc);

		delete pDesc;
	}
}
//****
static BOOL CompareFonts (const CBCGPFontDesc* pDesc, LPCTSTR lpszName, BYTE nCharSet, BOOL bExact)
{
	ASSERT_VALID (pDesc);

	CString strName = pDesc->GetFullName();
	strName.MakeLower();

	if (bExact)
	{
		if (strName == lpszName ||
			(pDesc->m_strName.CompareNoCase(lpszName) == 0 && 
			(nCharSet == pDesc->m_nCharSet || nCharSet == DEFAULT_CHARSET)))
		{
			return TRUE;
		}
	}
	else if (strName.Find (lpszName) == 0 && 
		(nCharSet == DEFAULT_CHARSET || pDesc->m_nCharSet == nCharSet))
	{
		return TRUE;
	}

	return FALSE;
}
//****
BOOL CBCGPRibbonFontComboBox::SetFont (LPCTSTR lpszName, BYTE nCharSet, BOOL bExact)
{
	ASSERT_VALID (this);
	ASSERT (lpszName != NULL);

	CString strNameFind = lpszName;
	strNameFind.MakeLower();

	const CBCGPFontDesc* pCurrFont = GetFontDesc();
	if (pCurrFont != NULL && CompareFonts (pCurrFont, strNameFind, nCharSet, bExact))
	{
		// Font is already selected
		return TRUE;
	}

	for (POSITION pos = m_lstItemData.GetHeadPosition(); pos != NULL;)
	{
		CBCGPFontDesc* pDesc = (CBCGPFontDesc*) m_lstItemData.GetNext (pos);

		if (CompareFonts (pDesc, strNameFind, nCharSet, bExact))
		{
			SelectItem ((DWORD_PTR) pDesc);
			return TRUE;
		}
	}

	return FALSE;
}
//****
BOOL CBCGPRibbonFontComboBox::OnDrawDropListItem (CDC* pDC, 
											  int nIndex, 	
											  CBCGPToolbarMenuButton* pItem,
											  BOOL /*bHighlight*/)
{
	ASSERT_VALID (this);

	if (m_Images.GetSafeHandle() == NULL)
	{
		CBCGPLocalResource locaRes;
		m_Images.Create (IDB_BCGBARRES_FONT, nImageWidth, 0, _RGB (255, 255, 255));
	}

	CRect rc = pItem->Rect();

	CBCGPFontDesc* pDesc = (CBCGPFontDesc*) GetItemData (nIndex);
	LPCTSTR lpszText = GetItem (nIndex);

	if (pDesc == NULL || lpszText == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID (pDesc);

	CFont fontSelected;
	CFont* pOldFont = NULL;

	if (pDesc->m_nType & (DEVICE_FONTTYPE | TRUETYPE_FONTTYPE))
	{
		CPoint ptImage (rc.l, rc.t + (rc.Height() - nImageHeight_rcb) / 2);
		m_Images.Draw (pDC, (pDesc->m_nType & DEVICE_FONTTYPE) ? 0 : 1, 
			ptImage, ILD_NORMAL);
	}

	rc.l += nImageWidth + nImageMargin;
	
	if (m_bDrawUsingFont && pDesc->m_nCharSet != SYMBOL_CHARSET)
	{
		LOGFONT lf;
		globalData.fontRegular.GetLogFont (&lf);

		lstrcpy (lf.lfFaceName, pDesc->m_strName);

		if (pDesc->m_nCharSet != DEFAULT_CHARSET)
		{
			lf.lfCharSet = pDesc->m_nCharSet;
		}

		if (lf.lfHeight < 0)
		{
			lf.lfHeight -= 4;
		}
		else
		{
			lf.lfHeight += 4;
		}

		fontSelected.CreateFontIndirect (&lf);
		pOldFont = pDC->SelectObject (&fontSelected);
	}

	CString strText = lpszText;

	pDC->DrawText (strText, rc, DT_SINGLELINE | DT_VCENTER);

	if (pOldFont != NULL)
	{
		pDC->SelectObject (pOldFont);
	}

	return TRUE;
}
//****
CSize CBCGPRibbonFontComboBox::OnGetDropListItemSize (
		CDC* /*pDC*/, 
		int /*nIndex*/,
		CBCGPToolbarMenuButton* /*pItem*/,
		CSize sizeDefault)
{
	ASSERT_VALID (this);

	CSize size = sizeDefault;
	size.w += nImageWidth + nImageMargin;

	return size;
}

#endif // BCGP_EXCLUDE_RIBBON
