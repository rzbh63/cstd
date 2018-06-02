
//
// BCGPDlgImpl.h: interface for the CBCGPDlgImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPDLGIMPL_H__18772215_4E74_4900_82E4_288CA46AB7E0__INCLUDED_)
#define AFX_BCGPDLGIMPL_H__18772215_4E74_4900_82E4_288CA46AB7E0__INCLUDED_

#include "BCGCBPro.h"
#include "BCGGlobals.h"
#include "BCGPVisualManager.h"

class CBCGPPopupMenu;
class CBCGPFrameCaptionButton;

class BCGCBPRODLLEXPORT CBCGPDlgImpl  
{
	friend class CBCGPDialog;
	friend class CBCGPPropertyPage;
	friend class CBCGPPropertySheet;
	friend class CBCGPDialogBar;
	friend class CBCGPFormView;

protected:
	CBCGPDlgImpl(CWnd& dlg);
	virtual ~CBCGPDlgImpl();

	static LRESULT CALLBACK BCGDlgMouseProc (int nCode, WPARAM wParam, LPARAM lParam);

	void SetActiveMenu (CBCGPPopupMenu* pMenu);

	BOOL ProcessMouseClick (POINT pt);
	BOOL ProcessMouseMove (POINT pt);

	BOOL PreTranslateMessage(MSG* pMsg);
	BOOL OnCommand (WPARAM wParam, LPARAM lParam);
	void OnNcActivate (BOOL& bActive);
	void OnActivate(UINT nState, CWnd* pWndOther);

	void EnableVisualManagerStyle (BOOL bEnable, BOOL bNCArea = FALSE, const CList<UINT,UINT>* plstNonSubclassedItems = NULL);

	void OnDestroy ();
	void OnDWMCompositionChanged ();

	BOOL EnableAero (BCGPMARGINS& margins);
	void GetAeroMargins (BCGPMARGINS& margins) const;
	BOOL HasAeroMargins () const;
	void ClearAeroAreas (CDC* pDC);

	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	BOOL OnNcPaint();
	BOOL OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	void UpdateCaption ();
	void UpdateCaptionButtons ();
	UINT OnNcHitTest (CPoint point);
	void OnNcMouseMove(UINT nHitTest, CPoint point);
	void OnLButtonDown(CPoint point);
	void OnLButtonUp(CPoint point);
	void OnMouseMove(CPoint point);

	CBCGPFrameCaptionButton* GetSysButton (UINT nHit);

	void SetHighlightedSysButton (UINT nHitTest);
	void OnTrackCaptionButtons (CPoint point);
	void StopCaptionButtonsTracking ();
	void RedrawCaptionButton (CBCGPFrameCaptionButton* pBtn);

	BOOL IsOwnerDrawCaption ()
	{
#if (!defined _BCGSUITE_) && (!defined _BCGSUITE_INC_)
		return m_bVisualManagerStyle && m_bVisualManagerNCArea && CBCGPVisualManager::GetInstance ()->IsOwnerDrawCaption ();
#else
		return m_bVisualManagerStyle && m_bVisualManagerNCArea && CMFCVisualManager::GetInstance ()->IsOwnerDrawCaption ();
#endif
	}

	CRect GetCaptionRect ();
	void OnChangeVisualManager ();
	void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	void OnGetMinMaxInfo (MINMAXINFO FAR* lpMMI);

	int GetCaptionHeight ();

	CWnd&					m_Dlg;
	static HHOOK			m_hookMouse;
	static CBCGPDlgImpl*	m_pMenuDlgImpl;
	BOOL					m_bVisualManagerStyle;
	BOOL					m_bVisualManagerNCArea;
	CArray<CWnd*, CWnd*>	m_arSubclassedCtrls;
	BCGPMARGINS				m_AeroMargins;
	BOOL					m_bTransparentStaticCtrls;
	CObList					m_lstCaptionSysButtons;
	UINT					m_nHotSysButton;
	UINT					m_nHitSysButton;
	CRect					m_rectRedraw;
	BOOL					m_bWindowPosChanging;
	BOOL					m_bIsWindowRgn;
	BOOL					m_bHasBorder;
};

extern BCGCBPRODLLEXPORT UINT BCGM_ONSETCONTROLAERO;
extern BCGCBPRODLLEXPORT UINT BCGM_ONSETCONTROLVMMODE;

#endif // !defined(AFX_BCGPDLGIMPL_H__18772215_4E74_4900_82E4_288CA46AB7E0__INCLUDED_)
