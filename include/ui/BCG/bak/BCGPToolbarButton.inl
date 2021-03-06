
//
// BCGPToolbarButton.cpp: implementation of the CBCGPPToolbarButton class.
//
//////////////////////////////////////////////////////////////////////

#include "menuhash.h"
#include "BCGGlobals.h"
#include "BCGPCommandManager.h"
#include "BCGPVisualManager.h"  
#include "BCGPToolbarButton.h"
#include "BCGPToolBar.h"
#include "BCGPMDIFrameWnd.h"
#include "BCGPOleIPFrameWnd.h"
#include "BCGPFrameWnd.h"
#include "BCGPToolbarMenuButton.h"
#include "BCGPWorkspace.h"
#include "CustomizeButton.h"
#include "BCGPKeyboardManager.h"
#include "BCGPDlgImpl.h"

extern CBCGPWorkspace* g_pWorkspace;

IMPLEMENT_SERIAL(CBCGPToolbarButton, CObject, VERSIONABLE_SCHEMA | 1)

CLIPFORMAT CBCGPToolbarButton::m_cFormat = 0;
CString	 CBCGPToolbarButton::m_strClipboardFormatName;
BOOL CBCGPToolbarButton::m_bWrapText = TRUE;

static const int nTextMargin_tb = 3;
static const int nSeparatorWidth = 8;
#define strDummyAmpSeq  _T("\001\001")

CList<UINT, UINT> CBCGPToolbarButton::m_lstProtectedCommands;
BOOL CBCGPToolbarButton::m_bUpdateImages = TRUE;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPToolbarButton::CBCGPToolbarButton()
{
	Initialize();
}
//****
CBCGPToolbarButton::CBCGPToolbarButton(UINT uiID, int iImage, LPCTSTR lpszText, BOOL bUserButton,
									 BOOL bLocked)
{
	Initialize();

	m_bLocked = bLocked;

	m_nID = uiID;
	m_bUserButton = bUserButton;
	SetImage (iImage);

	m_strText = (lpszText == NULL) ? _T("") : lpszText;

	if (m_nID != 0 && !m_bLocked)
	{
		if (m_bUserButton)
		{
			if (m_iUserImage != -1)
			{
				BCGPCMD_MGR.SetCmdImage (m_nID, m_iUserImage, TRUE);
			}
			else
			{
				m_iUserImage = BCGPCMD_MGR.GetCmdImage (m_nID, TRUE);
			}
		}
		else
		{
			if (m_iImage != -1)
			{
				BCGPCMD_MGR.SetCmdImage (m_nID, m_iImage, FALSE);
			}
			else
			{
				m_iImage = BCGPCMD_MGR.GetCmdImage (m_nID, FALSE);
			}
		}
	}
}
//****
void CBCGPToolbarButton::Initialize()
{
	m_nID = 0;
	m_nStyle = TBBS_BUTTON;
	m_iImage = -1;
	m_iUserImage = -1;
	m_bUserButton = FALSE;
	m_bDragFromCollection = FALSE;
	m_bText = FALSE;
	m_bImage = TRUE;
	m_bWrap = FALSE;
	m_bWholeText = TRUE;
	m_bLocked = FALSE;
	m_bIsHidden = FALSE;
	m_bTextBelow = FALSE;
	m_dwdItemData = 0;

	m_rect.SetRectEmpty();
	m_sizeText = CSize (0, 0);
	m_bDisableFill = FALSE;
	m_bExtraSize = FALSE;
	m_bHorz = TRUE;
	m_bVisible = TRUE;
	m_pWndParent = NULL;
	m_bOnGlass = FALSE;
}
//****
CBCGPToolbarButton::~CBCGPToolbarButton()
{
}
//****
void CBCGPToolbarButton::CopyFrom (const CBCGPToolbarButton& src)
{
	m_nID			= src.m_nID;
	m_bLocked		= src.m_bLocked;
	m_bUserButton	= src.m_bUserButton;
	m_nStyle		= src.m_nStyle;
	SetImage (src.m_bUserButton ? src.m_iUserImage : src.m_iImage);
	m_strText		= src.m_strText;
	m_bText			= src.m_bText;
	m_bImage		= src.m_bImage;
	m_bWrap			= src.m_bWrap;
	m_strTextCustom	= src.m_strTextCustom;
	m_bVisible		= src.m_bVisible;
	m_dwdItemData	= src.m_dwdItemData;
					
	m_bDragFromCollection = FALSE;
}					
//****
void CBCGPToolbarButton::Serialize (CArchive& ar)
{
	CObject::Serialize (ar);

	if (ar.IsLoading())
	{
		int iImage;

		ar >> m_nID;
		ar >> m_nStyle;	
		ar >> iImage;	
		ar >> m_strText;	
		ar >> m_bUserButton;
		ar >> m_bDragFromCollection;
		ar >> m_bText;
		ar >> m_bImage;

		if (g_menuHash.IsActive() ||
			(g_pWorkspace != NULL && g_pWorkspace->GetDataVersion() >= 0x60410))
		{
			ar >> m_bVisible;
		}

		SetImage (iImage);
	}
	else
	{
		ar << m_nID;		
		ar << m_nStyle;	
		ar << GetImage();
		ar << m_strText;
		ar << m_bUserButton;
		ar << m_bDragFromCollection;
		ar << m_bText;
		ar << m_bImage;

		if (g_menuHash.IsActive() ||
			(g_pWorkspace != NULL && g_pWorkspace->GetDataVersion() >= 0x60410))
		{
			ar << m_bVisible;
		}
	}
}
//****
CLIPFORMAT CBCGPToolbarButton::GetClipboardFormat()
{
	if (m_cFormat == 0)	// Not registered yet
	{
		CString strFormat = m_strClipboardFormatName;

		if (strFormat.IsEmpty())
		{
			strFormat.Format (_T("BCGToolbarButton%lx"), AfxGetInstanceHandle());
						// Format should be unique per application
		}

		m_cFormat = (CLIPFORMAT)::RegisterClipboardFormat (strFormat);
		ASSERT (m_cFormat != NULL);
	}

	return m_cFormat;
}
//****
CBCGPToolbarButton* CBCGPToolbarButton::CreateFromOleData  (COleDataObject* pDataObject)
{
	ASSERT (pDataObject != NULL);
	ASSERT (pDataObject->IsDataAvailable (CBCGPToolbarButton::m_cFormat));

	CBCGPToolbarButton* pButton = NULL;

	try
	{
		//-------------------------------------
		// Get file refering to clipboard data:
		//-------------------------------------
		CFile* pFile = pDataObject->GetFileData (GetClipboardFormat());
		if (pFile == NULL)
		{
			return FALSE;
		}

		//-------------------------------------------------------
		// Connect the file to the archive and read the contents:
		//-------------------------------------------------------
		CArchive ar (pFile, CArchive::load);

		//----------------------------------------
		// First, read run-time class information:
		//----------------------------------------
		CRuntimeClass* pClass = ar.ReadClass();
		ASSERT (pClass != NULL);

		if (pClass != NULL)
		{
			pButton = (CBCGPToolbarButton*) pClass->CreateObject();
			ASSERT (pButton != NULL);

			if (pButton != NULL)
			{
				pButton->Serialize (ar);
			}
		}

		ar.Close();
		delete pFile;

		return pButton;
	}
	catch (COleException* pEx)
	{
		TRACE(_T("CBCGPToolbarButton::CreateFromOleData. OLE exception: %x\r\n"),
			pEx->m_sc);
		pEx->Delete();
	}
	catch (CArchiveException* pEx)
	{
		TRACE(_T("CBCGPToolbarButton::CreateFromOleData. Archive exception\r\n"));
		pEx->Delete();
	}
	catch (CNotSupportedException *pEx)
	{
		TRACE(_T("CBCGPToolbarButton::CreateFromOleData. \"Not Supported\" exception\r\n"));
		pEx->Delete();
	}

	if (pButton != NULL)
	{
		delete pButton;
	}

	return NULL;
}
//****
void CBCGPToolbarButton::OnDraw (CDC* pDC, const CRect& rect, CBCGPToolBarImages* pImages,
								BOOL bHorz, BOOL bCustomizeMode, BOOL bHighlight,
								BOOL bDrawBorder, BOOL bGrayDisabledButtons)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (this);

	m_bHorz = bHorz;

	//----------------------
	// Fill button interior:
	//----------------------
	FillInterior (pDC, rect, bHighlight);

	BOOL bHot = bHighlight;
	CSize sizeImage = (pImages == NULL) ? CSize (0, 0) : pImages->GetImageSize (TRUE);

	CBCGPUserTool* pUserTool = NULL;
	if (g_pUserToolsManager != NULL && !m_bUserButton)
	{
		pUserTool = g_pUserToolsManager->FindTool (m_nID);
	}

	CRect rectInternal = rect;
	CSize sizeExtra = m_bExtraSize ?
		CBCGPVisualManager::GetInstance()->GetButtonExtraBorder() : CSize (0, 0);
	rectInternal.DeflateRect (sizeExtra.w / 2, sizeExtra.h / 2);

	int x = rectInternal.l;
	int y = rectInternal.t;

	int iTextLen = 0;

	CString strWithoutAmp = m_strText;
	strWithoutAmp.Replace (_T("&&"), strDummyAmpSeq);
	strWithoutAmp.Remove (_T('&'));
	strWithoutAmp.Replace (strDummyAmpSeq, _T("&"));

	CSize sizeText = pDC->GetTextExtent (strWithoutAmp);

	if (IsDrawText() && !(m_bTextBelow && bHorz))
	{
		int nMargin = IsDrawImage() ? 0 : nTextMargin_tb;
		iTextLen = sizeText.w + nMargin;
	}

	int dx = 0;
	int dy = 0;

	if (m_bTextBelow && bHorz)
	{
		ASSERT (bHorz);

		dx = rectInternal.Width();
		dy = sizeImage.h + 2 * nTextMargin_tb;
	}
	else
	{
		dx = bHorz ? rectInternal.Width() - iTextLen : rectInternal.Width();
		dy = bHorz ? rectInternal.Height() : rectInternal.Height() - iTextLen;
	}

	// determine offset of bitmap (centered within button)
	CPoint ptImageOffset;
	ptImageOffset.x = (dx - sizeImage.w) / 2;
	ptImageOffset.y = (dy - sizeImage.h) / 2;

	CPoint ptTextOffset (nTextMargin_tb, nTextMargin_tb);

	if (IsDrawText() && !(m_bTextBelow && bHorz))
	{
		TEXTMETRIC tm;
		pDC->GetTextMetrics (&tm);

		if (bHorz)
		{
			ptImageOffset.x -= nTextMargin_tb;
			ptTextOffset.y = (dy - tm.tmHeight - 1) / 2;
		}
		else
		{
			ptImageOffset.y -= nTextMargin_tb;
			ptTextOffset.x = (dx - tm.tmHeight + 1) / 2;
		}
	}

	CPoint ptImageOffsetInButton (0, 0);
	BOOL bPressed = FALSE;

	BOOL bDrawImageShadow = 
		bHighlight && !bCustomizeMode &&
		!IsDroppedDown() &&
		CBCGPVisualManager::GetInstance()->IsShadowHighlightedImage() &&
		!globalData.IsHighContastMode() &&
		((m_nStyle & TBBS_PRESSED) == 0) &&
		((m_nStyle & TBBS_CHECKED) == 0) &&
		((m_nStyle & TBBS_DISABLED) == 0);

	if ((m_nStyle & (TBBS_PRESSED | TBBS_CHECKED)) && !bCustomizeMode &&
		!CBCGPVisualManager::GetInstance()->IsShadowHighlightedImage() &&
		CBCGPVisualManager::GetInstance()->IsOffsetPressedButton())
	{
		// pressed in or checked
		ptImageOffset.Offset (1, 1);
		bPressed = TRUE;

		ptTextOffset.y ++;

		if (bHorz)
		{
			ptTextOffset.x ++;
		}
		else
		{
			ptTextOffset.x --;
		}
	}

	BOOL bFadeImage = !bHighlight && CBCGPVisualManager::GetInstance()->IsFadeInactiveImage();
	BOOL bImageIsReady = FALSE;

	if ((m_nStyle & TBBS_PRESSED) || !(m_nStyle & TBBS_DISABLED) ||
		bCustomizeMode)
	{
		if (IsDrawImage() && pImages != NULL)
		{
			if (pUserTool != NULL)
			{
				pUserTool->DrawToolIcon (pDC,
					CRect (CPoint (x + ptImageOffset.x, y + ptImageOffset.y),
					sizeImage));
			}
			else
			{
				CPoint pt = ptImageOffset;

				if (bDrawImageShadow)
				{
					pt.Offset (1, 1);

					pImages->Draw (pDC, x + pt.x, 
										y + pt.y, GetImage(),
										FALSE, FALSE, FALSE, TRUE);
					pt.Offset (-2, -2);
				}

				pImages->Draw (pDC, x + pt.x, y + pt.y, GetImage(),
					FALSE, FALSE, FALSE, FALSE, bFadeImage);
			}
		}

		bImageIsReady = TRUE;
	}

	BOOL bDisabled = (bCustomizeMode && !IsEditable()) ||
		(!bCustomizeMode && (m_nStyle & TBBS_DISABLED));

	if (!bImageIsReady)
	{
		if (IsDrawImage() && pImages != NULL)
		{
			if (pUserTool != NULL)
			{
				pUserTool->DrawToolIcon (pDC,
					CRect (CPoint (x + ptImageOffset.x, y + ptImageOffset.y),
					sizeImage));
			}
			else
			{
				if (bDrawImageShadow)
				{
					ptImageOffset.Offset (1, 1);

					pImages->Draw (pDC, x + ptImageOffset.x, 
										y + ptImageOffset.y, GetImage(),
										FALSE, FALSE, FALSE, TRUE);
					ptImageOffset.Offset (-2, -2);
				}

				pImages->Draw (pDC, x + ptImageOffset.x, y + ptImageOffset.y, GetImage(),
								FALSE, bDisabled && bGrayDisabledButtons,
								FALSE, FALSE, bFadeImage);
			}
		}
	}

	if ((m_bTextBelow && bHorz) || IsDrawText())
	{
		//--------------------
		// Draw button's text:
		//--------------------
		CBCGPVisualManager::BCGBUTTON_STATE state = CBCGPVisualManager::ButtonsIsRegular;

		if (bHighlight)
		{
			state = CBCGPVisualManager::ButtonsIsHighlighted;
		}
		else if (m_nStyle & (TBBS_PRESSED | TBBS_CHECKED))
		{
			//-----------------------
			// Pressed in or checked:
			//-----------------------
			state = CBCGPVisualManager::ButtonsIsPressed;
		}

		COLORREF clrText = CBCGPVisualManager::GetInstance()->GetToolbarButtonTextColor (
			this, state);

		pDC->SetTextColor (clrText);
		CString strText = m_strText;
		CRect rectText = rectInternal;
		UINT uiTextFormat = 0;

		if (m_bTextBelow && bHorz)
		{
			ASSERT (bHorz);

			ptTextOffset.y += sizeImage.h + nTextMargin_tb;
			uiTextFormat = DT_CENTER;

			if (m_bWrapText)
			{
				uiTextFormat |= DT_WORDBREAK;
			}

			rectText.l = (rectInternal.l + rectInternal.r - m_sizeText.w) / 2 + ptTextOffset.x;
			rectText.r = (rectInternal.l + rectInternal.r + m_sizeText.w) / 2;
		}
		else
		{
			if (IsDrawImage())
			{
				const int nExtra = CBCGPToolBar::IsLargeIcons() ? 2 * nTextMargin_tb : 0;

				if (bHorz)
				{
					ptTextOffset.x += sizeImage.w + nExtra;
				}
				else
				{
					ptTextOffset.y += sizeImage.h + nExtra;
				}
			
				rectText.l = x + ptTextOffset.x + nTextMargin_tb;
			}
			else
			{
				rectText.l = x + nTextMargin_tb + 1;
			}

			uiTextFormat = DT_SINGLELINE;
		}

		if (bHorz)
		{
			rectText.t += ptTextOffset.y;

			if (m_bTextBelow && m_bExtraSize)
			{
				rectText.OffsetRect (0,
					CBCGPVisualManager::GetInstance()->GetButtonExtraBorder().h / 2);
			}

			if (!globalData.m_bUnderlineKeyboardShortcuts && !CBCGPToolBar::IsCustomizeMode())
			{
				strText = strWithoutAmp;
			}

			DrawButtonText (pDC, strText, &rectText, uiTextFormat, clrText, (int)state);
		}
		else
		{
			rectText = rectInternal;
			rectText.t += ptTextOffset.y;

			rectText.l = rectText.CenterPoint().x - sizeText.h / 2;
			rectText.r = rectText.l + sizeText.h;
			rectText.t += max (0, (rectText.Height() - sizeText.w) / 2);

			rectText.SwapLeftRight();

			uiTextFormat = DT_NOCLIP | DT_SINGLELINE;

			strText.Replace (_T("&&"), strDummyAmpSeq);
			int iAmpIndex = strText.Find (_T('&'));	// Find a SINGLE '&'
			strText.Remove (_T('&'));
			strText.Replace (strDummyAmpSeq, _T("&&"));

			if (iAmpIndex >= 0 && (globalData.m_bUnderlineKeyboardShortcuts && !CBCGPToolBar::IsCustomizeMode()))
			{
				//-----------------------------------------
				// Calculate underlined character position:
				//-----------------------------------------
				CRect rectSubText;
				rectSubText.SetRectEmpty();
				CString strSubText = strText.Left (iAmpIndex + 1);

				pDC->DrawText (strSubText, &rectSubText, uiTextFormat | DT_CALCRECT);
				int y1 = rectSubText.r;

				rectSubText.SetRectEmpty();
				strSubText = strText.Left (iAmpIndex);

				pDC->DrawText (strSubText, &rectSubText, uiTextFormat | DT_CALCRECT);
				int y2 = rectSubText.r;

				pDC->DrawText (strText, &rectText, uiTextFormat);

				int x = rect.CenterPoint().x - sizeText.h / 2;

				CPen* pOldPen = NULL;
				CPen pen (PS_SOLID, 1, pDC->GetTextColor());

				if (pDC->GetTextColor() != 0)
				{
					pOldPen = pDC->SelectObject (&pen);
				}

				pDC->MoveTo (x, rectText.t + y1);
				pDC->LineTo (x, rectText.t + y2);

				if (pOldPen != NULL)
				{
					pDC->SelectObject (pOldPen);
				}
			}
			else
			{
				pDC->DrawText (strText, &rectText, uiTextFormat);
			}
		}
	}

	//--------------------
	// Draw button border:
	//--------------------
	if (!bCustomizeMode && HaveHotBorder() && bDrawBorder)
	{
		if (m_nStyle & (TBBS_PRESSED | TBBS_CHECKED))
		{
			//-----------------------
			// Pressed in or checked:
			//-----------------------
			CBCGPVisualManager::GetInstance()->OnDrawButtonBorder (pDC,
				this, rect, CBCGPVisualManager::ButtonsIsPressed);
		}
		else if (bHot && !(m_nStyle & TBBS_DISABLED) &&
			!(m_nStyle & (TBBS_CHECKED | TBBS_INDETERMINATE)))
		{
			CBCGPVisualManager::GetInstance()->OnDrawButtonBorder (pDC,
				this, rect, CBCGPVisualManager::ButtonsIsHighlighted);
		}
	}
}
//****
ISIZE CBCGPToolbarButton::OnCalculateSize (
								CDC* pDC,
								const CSize& sizeDefault,
								BOOL bHorz)
{
	ASSERT_VALID (pDC);

	if(!IsVisible())
		return CSize(0,0);

	CSize size = sizeDefault;

	if (m_nStyle & TBBS_SEPARATOR)
	{
		if (bHorz)
		{
			size.w = m_iImage > 0 ? m_iImage : nSeparatorWidth;
		}
		else
		{
			size.h = nSeparatorWidth;
		}
	}
	else
	{
		BOOL bHasImage = TRUE;

		if (!IsDrawImage() || GetImage() < 0)
		{
			bHasImage = FALSE;

			CSize sizeExtra = m_bExtraSize ? 
				CBCGPVisualManager::GetInstance()->GetButtonExtraBorder() : CSize (0, 0);

			if (bHorz)
			{
				size.w = sizeExtra.w;
			}
			else
			{
				size.h = sizeExtra.h;
			}
		}

		m_sizeText = CSize (0, 0);

		if (!m_strText.IsEmpty())
		{
			if (m_bTextBelow && bHorz)
			{
				//----------------------------------------------------------
				// Try format text that it ocuppies no more tow lines an its
				// width less than 3 images:
				//----------------------------------------------------------
				CRect rectText (0, 0, 
					sizeDefault.w * 3, sizeDefault.h);

				UINT uiTextFormat = DT_CENTER | DT_CALCRECT;
				if (m_bWrapText)
				{
					uiTextFormat |= DT_WORDBREAK;
				}

				pDC->DrawText (	m_strText, rectText, uiTextFormat);
				m_sizeText = rectText.Size();
				m_sizeText.w += 2 * nTextMargin_tb;

				size.w = max (size.w, m_sizeText.w) + 4 * nTextMargin_tb;
				size.h += m_sizeText.h + CY_BORDER; 
			}
			else if (IsDrawText())
			{
				CString strWithoutAmp = m_strText;
				strWithoutAmp.Replace (_T("&&"), strDummyAmpSeq);
				strWithoutAmp.Remove (_T('&'));
				strWithoutAmp.Replace (strDummyAmpSeq, _T("&"));

				int nTextExtra = bHasImage ? 2 * nTextMargin_tb : 3 * nTextMargin_tb;
				int iTextLen = pDC->GetTextExtent (strWithoutAmp).w + nTextExtra;

				if (bHorz)
				{
					size.w += iTextLen;
				}
				else
				{
					size.h += iTextLen;
				}
			}
		}
	}

	return size;
}
//****
BOOL CBCGPToolbarButton::PrepareDrag (COleDataSource& srcItem)
{
	if (!CanBeStored())
	{
		return TRUE;
	}

	try
	{
		CSharedFile globFile;
		CArchive ar (&globFile,CArchive::store);

		//---------------------------------
		// Save run-time class information:
		//---------------------------------
		CRuntimeClass* pClass = GetRuntimeClass();
		ASSERT (pClass != NULL);

		ar.WriteClass (pClass);

		//---------------------
		// Save button context:
		//---------------------
		Serialize (ar);
		ar.Close();

		srcItem.CacheGlobalData (GetClipboardFormat(), globFile.Detach());
	}
	catch (COleException* pEx)
	{
		TRACE(_T("CBCGPToolbarButton::PrepareDrag. OLE exception: %x\r\n"),
			pEx->m_sc);
		pEx->Delete();
		return FALSE;
	}
	catch (CArchiveException* pEx)
	{
		TRACE(_T("CBCGPToolbarButton::PrepareDrag. Archive exception\r\n"));
		pEx->Delete();
		return FALSE;
	}

	return TRUE;
}
//****
void CBCGPToolbarButton::SetImage (int iImage)
{
	if (m_nStyle & TBBS_SEPARATOR)
	{
		m_iImage = iImage;	// Actualy, separator width!
		return;
	}

	if (m_bUserButton)
	{
		m_iUserImage = iImage;
	}
	else
	{
		m_iImage = iImage;
	}

	if (!m_bLocked)
	{
		if (m_nID != 0 && iImage != -1)
		{
			if (m_bUpdateImages || m_bUserButton)
			{
				BCGPCMD_MGR.SetCmdImage (m_nID, iImage, m_bUserButton);
			}
		}
		else if (m_nID != 0)
		{
			m_iImage = BCGPCMD_MGR.GetCmdImage (m_nID, FALSE);
			m_iUserImage = BCGPCMD_MGR.GetCmdImage (m_nID, TRUE);

			if (m_iImage == -1 && !m_bUserButton)
			{
				m_bUserButton = TRUE;
			}
			else if (m_iImage == -1 && m_bUserButton)
			{
				m_bUserButton = FALSE;
			}
		}
	}

    if ((!m_bUserButton && m_iImage < 0) ||
        (m_bUserButton && m_iUserImage < 0))
    {
        m_bImage = FALSE;
        m_bText  = TRUE;
    }
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPToolbarButton diagnostics

#ifdef _DEBUG
void CBCGPToolbarButton::AssertValid() const
{
	CObject::AssertValid();
}
//****
void CBCGPToolbarButton::Dump(CDumpContext& dc) const
{
	CObject::Dump (dc);

	CString strId;
	strId.Format (_T("%x"), m_nID);

	dc << "[" << strId << " " << m_strText << "]";
	dc << "\n";
}

#endif

int CBCGPToolbarButton::OnDrawOnCustomizeList (
	CDC* pDC, const CRect& rect, BOOL bSelected)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	BOOL bText = m_bText;
	m_bText = FALSE;

	int iWidth = 0;

	CBCGPToolBarImages* pImages = CBCGPToolBar::GetImages();
	if (m_bUserButton)
	{
		pImages = CBCGPToolBar::GetUserImages();
	}
	else
	{
		CBCGPToolBarImages* pMenuImages = CBCGPToolBar::GetMenuImages();
		if (pMenuImages != NULL && pMenuImages->GetCount() == pImages->GetCount())
		{
			pImages = pMenuImages;
		}
	}
	
	CBCGPUserTool* pUserTool = NULL;
	if (g_pUserToolsManager != NULL && !m_bUserButton)
	{
		pUserTool = g_pUserToolsManager->FindTool (m_nID);
	}

	CSize sizeMenuImage = CBCGPToolBar::GetMenuImageSize();

	int nMargin = 3;
	CSize sizeButton = CSize (	sizeMenuImage.w + 2 * nMargin,
								sizeMenuImage.h + 2 * nMargin);

	CRect rectFill = rect;

	if (bSelected && 
		!CBCGPVisualManager::GetInstance()->IsHighlightWholeMenuItem() &&
		GetImage() >= 0 && pImages != NULL)
	{
		rectFill.l += sizeButton.w;

		CRect rectLeftBtn = rect;
		rectLeftBtn.r = rectFill.l;

		CBCGPVisualManager::GetInstance()->OnFillButtonInterior (
			pDC, this, rectLeftBtn, CBCGPVisualManager::ButtonsIsHighlighted);

		CBCGPVisualManager::GetInstance()->OnDrawButtonBorder (pDC,
			this, rectLeftBtn, CBCGPVisualManager::ButtonsIsHighlighted);
	}

	COLORREF clrText = CBCGPVisualManager::GetInstance()->OnFillCommandsListBackground 
			(pDC, rectFill, bSelected);

	CRect rectText = rect;
	rectText.l += sizeMenuImage.w + 2 * IMAGE_MARGIN + 2;

	iWidth = sizeButton.w;

	//-------------------
	// Draw button image:
	//-------------------
	if (GetImage() >= 0 && pImages != NULL)
	{
		if (pUserTool != NULL)
		{
			CRect rectImage = rect;
			rectImage.r = rectImage.l + sizeButton.w;

			pUserTool->DrawToolIcon (pDC, rectImage);
		}
		else
		{
			BOOL bFadeImage = !bSelected && CBCGPVisualManager::GetInstance()->IsFadeInactiveImage();
			BOOL bDrawImageShadow = 
				bSelected && 
				CBCGPVisualManager::GetInstance()->IsShadowHighlightedImage() &&
				!globalData.IsHighContastMode();

			CSize sizeImageDest (0, 0);
			if (globalData.GetRibbonImageScale() != 1. && !CBCGPToolBar::m_bDontScaleImages)
			{
				sizeImageDest = sizeMenuImage;
			}

			CBCGPDrawState ds;
			pImages->PrepareDrawImage (ds,
				sizeImageDest, 
				bFadeImage);

			CPoint pt = rect.TopLeft();
			pt.x += nMargin;
			pt.y += nMargin;

			if (globalData.GetRibbonImageScale() != 1. && CBCGPToolBar::m_bDontScaleImages)
			{
				pt.x += max (0, (sizeMenuImage.w - pImages->GetImageSize().w) / 2);
				pt.y += max (0, (sizeMenuImage.h - pImages->GetImageSize().h) / 2);
			}

			if (bDrawImageShadow)
			{
				pt.Offset (1, 1);

				pImages->Draw (pDC, pt.x, 
									pt.y, GetImage(),
									FALSE, FALSE, FALSE, TRUE);
				pt.Offset (-2, -2);
			}

			pImages->Draw (pDC, pt.x, pt.y, GetImage(),
				FALSE, FALSE, FALSE, FALSE, bFadeImage);

			pImages->EndDrawImage (ds);
		}
	}

	//-------------------
	// Draw button text:
	//-------------------
	if (!m_strText.IsEmpty())
	{
		COLORREF clrTextOld = pDC->SetTextColor (clrText);

		pDC->SetBkMode (TRANSPARENT);
		pDC->DrawText (m_strText, rectText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		pDC->SetTextColor (clrTextOld);

		int iTextWidth = min (rectText.Width(), pDC->GetTextExtent (m_strText).w);
		iWidth += iTextWidth;
	}

	m_bText = bText;
	return iWidth;
}
//****
BOOL CBCGPToolbarButton::OnToolHitTest(const CWnd* pWnd, TOOLINFO* pTI)
{
	CFrameWnd* pTopFrame = (pWnd == NULL) ? 
		(CFrameWnd*) AfxGetMainWnd() : 
		BCGCBProGetTopLevelFrame (pWnd);

	CBCGPMDIFrameWnd* pMainFrame = DYNAMIC_DOWNCAST (CBCGPMDIFrameWnd, pTopFrame);
	if (pMainFrame != NULL)
	{
		return pMainFrame->OnMenuButtonToolHitTest (this, pTI);
	}
	else	// Maybe, SDI frame...
	{
		CBCGPFrameWnd* pFrame = DYNAMIC_DOWNCAST (CBCGPFrameWnd, pTopFrame);
		if (pFrame != NULL)
		{
			return pFrame->OnMenuButtonToolHitTest (this, pTI);
		}
		else	// Maybe, OLE frame...
		{
			CBCGPOleIPFrameWnd* pOleFrame = 
				DYNAMIC_DOWNCAST (CBCGPOleIPFrameWnd, pFrame);
			if (pOleFrame != NULL)
			{
				return pOleFrame->OnMenuButtonToolHitTest (this, pTI);
			}
		}
	}

	CBCGPFrameWnd* pFrame = DYNAMIC_DOWNCAST (CBCGPFrameWnd, pTopFrame);
	if (pFrame != NULL)
	{
		return pFrame->OnMenuButtonToolHitTest (this, pTI);
	}

	return FALSE;
}
//****
BOOL CBCGPToolbarButton::ExportToMenuButton (CBCGPToolbarMenuButton& menuButton) const
{
	//-----------------------------------------------------
	// Text may be undefined, bring it from the tooltip :-(
	//-----------------------------------------------------
	if (m_strText.IsEmpty() && m_nID != 0)
	{
		CString strMessage;
		int iOffset;

		if (strMessage.LoadString (m_nID) &&
			(iOffset = strMessage.Find (_T('\n'))) != -1)
		{
			menuButton.m_strText = strMessage.Mid (iOffset + 1);
		}
	}

	return TRUE;
}
//****
void CBCGPToolbarButton::SetProtectedCommands (const CList<UINT, UINT>& lstCmds)
{
	m_lstProtectedCommands.RemoveAll();
	m_lstProtectedCommands.AddTail ((CList<UINT,UINT>*) &lstCmds);
}
//****
void CBCGPToolbarButton::SetClipboardFormatName (LPCTSTR lpszName)
{
	ASSERT (lpszName != NULL);
	ASSERT (m_cFormat == 0);

	m_strClipboardFormatName = lpszName;
}
//****
void CBCGPToolbarButton::FillInterior (CDC* pDC, const CRect& rect,
									  BOOL bHighlight, BOOL bMenuImage/* = FALSE*/)
{
	if (m_bDisableFill)
	{
		return;
	}

	CBCGPVisualManager::BCGBUTTON_STATE state = CBCGPVisualManager::ButtonsIsRegular;

	if (!CBCGPToolBar::IsCustomizeMode() ||
		CBCGPToolBar::IsAltCustomizeMode() || m_bLocked)
	{
		if (bHighlight)
		{
			state = CBCGPVisualManager::ButtonsIsHighlighted;
		}
		else if (m_nStyle & (TBBS_PRESSED | TBBS_CHECKED))
		{
			//-----------------------
			// Pressed in or checked:
			//-----------------------
			state = CBCGPVisualManager::ButtonsIsPressed;
		}
	}

	if (bMenuImage)
	{
		CBCGPVisualManager::GetInstance()->OnFillMenuImageRect (pDC, this, rect, state);
	}
	else
	{
		CBCGPVisualManager::GetInstance()->OnFillButtonInterior (pDC, this, rect, state);
	}
}
//****
void CBCGPToolbarButton::ResetImageToDefault()
{
	if (m_bUserButton || (int) m_nID <= 0)
	{
		return;
	}

	if (g_pUserToolsManager != NULL &&
		g_pUserToolsManager->FindTool (m_nID) != NULL)
	{
		// User tool has its own image
		return;
	}

	BOOL bWasImage = m_bImage;

	int iImage = CBCGPToolBar::GetDefaultImage (m_nID);
	if (iImage >= 0)
	{
		SetImage (iImage);
	}
	else if (bWasImage)
	{
		m_bImage = FALSE;
		m_bText = TRUE;

		if (m_strText.IsEmpty())
		{
			CString strMessage;
			int iOffset;

			if (strMessage.LoadString (m_nID) &&
				(iOffset = strMessage.Find (_T('\n'))) != -1)
			{
				m_strText = strMessage.Mid (iOffset + 1);
			}
		}
	}
}
//****
BOOL CBCGPToolbarButton::CompareWith (const CBCGPToolbarButton& other) const
{
	return m_nID == other.m_nID;
}
//****
void CBCGPToolbarButton::OnChangeParentWnd (CWnd* pWndParent)
{
	m_bExtraSize = FALSE;
	m_pWndParent = pWndParent;

	if (pWndParent == NULL)
	{
		return;
	}

	CBCGPToolBar* pParentBar = DYNAMIC_DOWNCAST (CBCGPToolBar, pWndParent);
	if (pParentBar != NULL)
	{
		ASSERT_VALID (pParentBar);

		SetOnGlass (pParentBar->IsOnGlass());

		if (pParentBar->IsButtonExtraSizeAvailable())
		{
			m_bExtraSize = TRUE;
		}
	}
}
//****
BOOL CBCGPToolbarButton::IsFirstInGroup() const
{
	ASSERT_VALID (this);

	CBCGPToolBar* pToolBar = DYNAMIC_DOWNCAST (CBCGPToolBar, m_pWndParent);
	if (pToolBar == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID (pToolBar);

	CCustomizeButton* pCustomizeButton = pToolBar->GetCustomizeButton();
	if (this == pCustomizeButton)
	{
		return FALSE;
	}

	BOOL bIsFirstInGroup = TRUE;

	for (int i = 0; i < pToolBar->GetCount(); i++)
	{
		CBCGPToolbarButton* pButton = pToolBar->GetButton (i);
		ASSERT_VALID (pButton);

		if (pButton == this)
		{
			return bIsFirstInGroup;
		}

		if (pButton->IsVisible())
		{
			bIsFirstInGroup = 
				pButton->m_bWrap || (pButton->m_nStyle & TBBS_SEPARATOR) ||
				pButton->GetHwnd() != NULL;
		}
	}

	return FALSE;
}
//****
BOOL CBCGPToolbarButton::IsLastInGroup() const
{
	ASSERT_VALID (this);

	CBCGPToolBar* pToolBar = DYNAMIC_DOWNCAST (CBCGPToolBar, m_pWndParent);
	if (pToolBar == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID (pToolBar);

	CCustomizeButton* pCustomizeButton = pToolBar->GetCustomizeButton();
	if (this == pCustomizeButton)
	{
		return FALSE;
	}

	BOOL bIsLastInGroup = TRUE;

	int nCount = pToolBar->GetCount();
	if (pCustomizeButton != NULL)
	{
		nCount--;
	}

	for (int i = nCount - 1; i >= 0; i--)
	{
		CBCGPToolbarButton* pButton = pToolBar->GetButton (i);
		ASSERT_VALID (pButton);

		if (pButton == this)
		{
			return bIsLastInGroup || pButton->m_bWrap;
		}

		if (pButton->IsVisible())
		{
			bIsLastInGroup =	(pButton->m_nStyle & TBBS_SEPARATOR) ||
								pButton->GetHwnd() != NULL;
		}
	}

	return FALSE;
}
//****
BOOL CBCGPToolbarButton::SetACCData (CWnd* pParent, CBCGPAccessibilityData& data)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pParent);

	data.Clear();
	CString strText = m_strText;
	if (strText.IsEmpty() && m_nID != 0 && m_nID != (UINT) -1)
	{
		TCHAR szFullText [256];
		CString strTipText;

		if (AfxLoadString (m_nID, szFullText) &&
			AfxExtractSubString (strTipText, szFullText, 1, '\n'))
		{
			strText = strTipText;
		}
	}

	data.m_strAccName = strText;
	data.m_strAccName.Remove (_T('&'));
	data.m_strAccDefAction = _T("Press");
	
	CFrameWnd* pFrame  = pParent->GetParentFrame();
	if (pFrame != NULL && pFrame->GetSafeHwnd() != NULL)
	{
		CString strDescr;
		pFrame->GetMessageString (m_nID, strDescr);
		data.m_strDescription = strDescr;
	}

	CWnd* pWnd = GetParentWnd();
	CFrameWnd* pParentFrame = pWnd->GetParentFrame();
	CString strLabel;
	if (pParentFrame != NULL &&
		(CBCGPKeyboardManager::FindDefaultAccelerator (
			m_nID, strLabel, pParentFrame, TRUE) ||
		CBCGPKeyboardManager::FindDefaultAccelerator (
			m_nID, strLabel, pParentFrame->GetActiveFrame(), FALSE)))
	{
		data.m_strAccKeys = strLabel;
	}

	data.m_nAccHit = 1;
	data.m_nAccRole = ROLE_SYSTEM_PUSHBUTTON;
	data.m_bAccState = STATE_SYSTEM_FOCUSABLE;
	if (m_nStyle & TBBS_CHECKED)
	{
		data.m_bAccState |= STATE_SYSTEM_CHECKED; 
	}

	if (m_nStyle & TBBS_DISABLED)
	{
		data.m_bAccState |= STATE_SYSTEM_UNAVAILABLE;
	}

	if (m_nStyle & TBBS_PRESSED)
	{
	  data.m_bAccState |= STATE_SYSTEM_FOCUSED;
	}
	else
	{
		data.m_bAccState |= STATE_SYSTEM_HOTTRACKED;
	}
	
	data.m_rectAccLocation = m_rect;
	pParent->ClientToScreen (&data.m_rectAccLocation);

	return TRUE;
}
//****
void CBCGPToolbarButton::DrawButtonText (CDC* pDC, const CString& strText, CRect rectText, UINT uiDTFlags,
								 COLORREF clrText, int state)
{
	if (CBCGPToolBarImages::m_bIsDrawOnGlass)
	{
		if (!IsAlwaysOpaque())
		{
			const BOOL bIsZoomed = m_pWndParent->GetSafeHwnd() != NULL &&
				m_pWndParent->GetParentFrame()->GetSafeHwnd() != NULL &&
				m_pWndParent->GetParentFrame()->IsZoomed();

			if (bIsZoomed && CBCGPVisualManager::GetInstance()->IsToolBarButtonDefaultBackground (this, 
				(CBCGPVisualManager::BCGBUTTON_STATE)state))
			{
				clrText = _RGB (255, 255, 255);
			}
		}

		CBCGPVisualManager::GetInstance()->DrawTextOnGlass (pDC, strText, rectText, uiDTFlags, 0, clrText);
		return;
	}

	pDC->DrawText (strText, rectText, uiDTFlags);
}
//****
void CBCGPToolbarButton::SetOnGlass (BOOL bOnGlass)
{
	m_bOnGlass = bOnGlass;

	if (GetHwnd() != NULL)
	{
		::SendMessage (GetHwnd(), BCGM_ONSETCONTROLAERO, (WPARAM)m_bOnGlass, 0);
	}
}
