// --
// -- You may wish to change these --
// --
// Colors (_RGB) - blue theme
#define TABBACKGROUNDCOLOR _RGB(205,224,240) // You'll see this on XP, and on Vista when Aero is off
#define TAB_OUTLINE 0,18,39
#define TAB_FILL_GRADIENTSTART 231,247,247
#define TAB_FILL_GRADIENTMID 173,189,206
#define TAB_FILL_GRADIENTEND 214,231,239
#define SUBTAB_OUTLINE 79,115,158
#define SUBTAB_FILL_TOPHALF 159,205,241
#define SUBTAB_FILL_BOTTOMHALF 203,232,255
// Margins and padding
#define RIBBONBARPADDING 5
#define TABINSET 15
#define TABPADDINGWIDTH 20
#define TABPADDINGHEIGHT 10
#define PADDINGBETWEENTABS 5
#define SUBTABPADDING 5
#define SUBTABPADDINGWIDTH 15
#define SUBTABPADDINGHEIGHT 8
#define PADDINGBETWEENSUBTABS 5
// Animation duration
#define TABCHANGEANIMATIONDURATION 250
// --
// -- You will not want to change these --
// --
// Tab states (RD: Ribbon Dialog, TST: Tab State)
#define RD_TST_UNSELECTED 0x0000
#define RD_TST_SELECTED 0x0001
#define RD_TST_HOT 0x0002
// DrawText flags
#define DT_FORMATFLAGS DT_SINGLELINE | DT_CENTER | DT_VCENTER
#define HLSMAX 240 /* H,L, and S vary over 0-HLSMAX */
#define RGBMAX 255 /* R,G, and B vary over 0-RGBMAX */
#define UNDEFINED (HLSMAX*2/3)
// --
#if 0
template <struct T>
struct ATL_NO_VTABLE CRibbonTabDialogImpl : public CThemeImpl< T >,
  public CDialogImpl< T >,
  public CBufferedPaintBase,
  public CMessageFilter {
private: // ---------------------------------------------------------------------------
  // Tab item structure
  struct RIBBONDLGTAB {
    UINT uID;
    UINT uIconResID;
    UINT uState;
    RECT rc;
    CAtlArray<struct RIBBONDLGTAB>* listSubtabs;
  };
  typedef CAtlArray<RIBBONDLGTAB> LISTSUBTABS;
  CAtlArray<RIBBONDLGTAB> m_listTabs;
  CRect m_rcRibbonBar; // in screen coordinates
  int m_iRibbonBarHeight, m_iTabHeight;
  CFont m_font, m_fontHot;
  int m_iCompositionEnabled;
  DTTOPTS m_dto;
  BP_BUFFERFORMAT m_BufferFormat;
  BP_PAINTPARAMS m_PaintParams;
  BP_ANIMATIONPARAMS m_AnimationParams;
  BOOL m_bRepaintBackground;
  CachedBitmap* m_pCachedBkBitmap;
  ImageAttributes m_imgAttributes;
  HACCEL m_hAccel;
  BOOL m_bShowPrefixes;
public:
  typedef CRibbonTabDialogImpl< T > thisClass;
  BEGIN_MSG_MAP_EX(thisClass)
  MSG_WM_INITDIALOG(OnInitDialog)
  MSG_WM_DESTROY(OnDestroy)
  MSG_WM_COMMAND(OnCommand)
  MSG_WM_SIZING(OnSizing)
  MSG_WM_ENTERSIZEMOVE(OnEnterSizeMove)
  MSG_WM_EXITSIZEMOVE(OnExitSizeMove)
  MSG_WM_NCCALCSIZE(OnNcCalcSize)
  MSG_WM_NCPAINT(OnNcPaint)
  MSG_WM_NCHITTEST(OnNcHitTest)
  MSG_WM_NCLBUTTONDOWN(OnNcLButtonDown)
  MSG_WM_NCMOUSEMOVE(OnNcMouseMove)
  END_MSG_MAP()
  CRibbonTabDialogImpl() : m_BufferFormat(BPBF_TOPDOWNDIB), m_iCompositionEnabled(-1), m_pCachedBkBitmap(NULL), m_hAccel(NULL),
    m_iRibbonBarHeight(0), m_iTabHeight(0), m_bShowPrefixes(FALSE), m_bRepaintBackground(FALSE) {
    m_rcRibbonBar.SetRectEmpty();
    // Paint operation parameters for BeginBufferedAnimation
    memset(&m_PaintParams, 0, sizeof(BP_PAINTPARAMS));
    m_PaintParams.cbSize = sizeof(BP_PAINTPARAMS);
    m_PaintParams.dwFlags = BPPF_NONCLIENT;
    // Animation parameters for BeginBufferedAnimation
    memset(&m_AnimationParams, 0, sizeof(BP_ANIMATIONPARAMS));
    m_AnimationParams.cbSize = sizeof(BP_ANIMATIONPARAMS);
    m_AnimationParams.style = BPAS_LINEAR;
    m_AnimationParams.dwDuration = 0;
    // For the DrawThemeTextEx function
    memset(&m_dto, 0, sizeof(DTTOPTS));
    m_dto.dwSize = sizeof(DTTOPTS);
    m_dto.dwFlags = DTT_COMPOSITED | DTT_GLOWSIZE | DTT_TEXTCOLOR | DTT_SHADOWTYPE;
    m_dto.iTextShadowType = TST_CONTINUOUS;
    m_dto.iGlowSize = 0;
    m_dto.crText = _RGB(0, 0, 0);
    // Figure out the color to colorwash inactive icons
    BYTE H, L, S = 0;
    RGBtoHLS(Color(TAB_FILL_GRADIENTEND).ToCOLORREF(), H, L, S);
    COLORREF clr = HLStoRGB(H, 36, S);
    // This matrix will grayscale and do colour component addition
    ColorMatrix colorwash = {
      0.213f, 0.213f, 0.213f, 0, 0,
      0.715f, 0.715f, 0.715f, 0, 0,
      0.072f, 0.072f, 0.072f, 0, 0,
      0 , 0 , 0 , 1, 0,
      GetRValue(clr) / 255.0f, GetGValue(clr) / 255.0f, GetBValue(clr) / 255.0f, 0, 1
    };
    m_imgAttributes.SetColorMatrix(&colorwash);
  }
  virtual ~CRibbonTabDialogImpl() {
    if (m_hAccel) {
      DestroyAcceleratorTable(m_hAccel);
      m_hAccel = NULL;
    }
    if (m_pCachedBkBitmap) {
      delete m_pCachedBkBitmap;
      m_pCachedBkBitmap = NULL;
    }
    for (int i = 0, iTabCount = m_listTabs.GetCount(); i < iTabCount; i++) {
      if (m_listTabs[i].listSubtabs) {
        delete m_listTabs[i].listSubtabs;
        m_listTabs[i].listSubtabs = NULL;
      }
    }
  }
  BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {
    SetMsgHandled(FALSE);
    // register object for message filtering and idle updates
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->AddMessageFilter(this);
    OpenThemeData(L"globals");
    // Create fonts
    CLogFont lf;
    lf.SetMessageBoxFont();
    lf.MakeLarger(2);
    m_font.CreateFontIndirect(&lf);
    lf.lfUnderline = TRUE;
    m_fontHot.CreateFontIndirect(&lf);
    // Get text extent, which will be the height of a tab item
    CDCHandle dc(GetWindowDC());
    HFONT hFontOld = dc.SelectFont(m_font);
    int iOldMapMode = dc.SetMapMode(MM_TEXT);
    TEXTMETRIC tm = {0};
    GetTextMetrics(dc, &tm);
    dc.SetMapMode(iOldMapMode);
    dc.SelectFont(hFontOld);
    ReleaseDC(dc);
    m_iTabHeight = tm.tmHeight;
    // Calculate height of entire ribbon bar
    m_iRibbonBarHeight = (RIBBONBARPADDING + SUBTABPADDING + m_iTabHeight) * 2 + TABPADDINGHEIGHT + SUBTABPADDINGHEIGHT;
    // Vista+ Only: Extend the frame (glass)
    if (IsCompositionEnabled()) {
      MARGINS margins = {0 /*Left*/, 0 /*Right*/, m_iRibbonBarHeight /*Top*/, 0};
      DwmExtendFrameIntoClientArea(m_hWnd, &margins);
    }
    // Must do this to get a WM_NCCALCSIZE
    SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOACTIVATE);
    CRect rc;
    GetWindowRect(rc);
    MoveWindow(rc.top, rc.left, rc.Width(), rc.Height() + m_iRibbonBarHeight, TRUE);
    return FALSE;
  }
  //
  //
  // Add a tab
  // uID: must be a string resource identifier; also serves as a command identifier
  //
  void AddTab(UINT uID) {
    static BOOL bFirstTab = TRUE;
    RIBBONDLGTAB tab = {0};
    tab.uID = uID;
    tab.listSubtabs = new LISTSUBTABS;
    // Default: The first tab added should be selected
    tab.uState = bFirstTab ? RD_TST_SELECTED : TST_NONE;
    bFirstTab = FALSE;
    m_listTabs.Add(tab);
  }
  //
  //
  // Add a sub-tab: Will get added to last tab
  // uID: as in AddTab
  // uIconResID: is an optional icon resource identifier
  //
  void AddSubTab(UINT uID, UINT uIconResID = 0) {
    int iTabCount = m_listTabs.GetCount();
    ATLENSURE(iTabCount > 0);
    int iTabId = iTabCount - 1;
    RIBBONDLGTAB subtab = {0};
    subtab.uID = uID;
    subtab.uIconResID = uIconResID;
    // Default: First sub-tab should be selected
    if (m_listTabs[iTabId].listSubtabs->GetCount() == 0) {
      subtab.uState = RD_TST_SELECTED;
    }
    m_listTabs[iTabId].listSubtabs->Add(subtab);
  }
  void OnCommand(UINT uNotifyCode, int uID, CWindow wndCtl) {
    // Only need to handle this message when source of the message is an accelerator
    if (uNotifyCode == 1) {
      UpdateSelectedTab(CPoint(0, 0), uID);
    }
    else {
      SetMsgHandled(FALSE);
    }
  }
  void OnEnterSizeMove() {
    SetMsgHandled(FALSE);
    // Don't want to animate when resizing dialog, because that would be silly.
    m_AnimationParams.dwDuration = 0;
  }
  void OnSizing(UINT fwSide, LPRECT pRect) {
    SetMsgHandled(FALSE);
    if (fwSide == WMSZ_BOTTOM || fwSide == WMSZ_TOP) {
      return;
    }
    // Necessary to repaint the dialog only when it's resized width-wise.
    m_bRepaintBackground = TRUE;
  }
  void OnExitSizeMove() {
    SetMsgHandled(FALSE);
    // Done sizing, so restore the animation duration
    m_AnimationParams.dwDuration = TABCHANGEANIMATIONDURATION;
  }
  // FYI: "Towards an even deeper understanding of the WM_NCCALCSIZE message"
  // http://blogs.msdn.com/oldnewthing/archive/2003/09/15/54925.aspx
  LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam) {
    // Need to do this first
    LRESULT lResult = ::DefWindowProc(m_hWnd, WM_NCCALCSIZE, (WPARAM)(bCalcValidRects), lParam);
    if (bCalcValidRects == TRUE) {
      NCCALCSIZE_PARAMS* pncsp = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
      pncsp->rgrc[0].top += m_iRibbonBarHeight;
      return 0;
    }
    return lResult;
  }
  UINT OnNcHitTest(CPoint point) {
    CRect rcWindow, rcRibbonBar(m_rcRibbonBar);
    GetWindowRect(rcWindow);
    rcRibbonBar.OffsetRect(rcWindow.left, rcWindow.top);
    // Use HTHSCROLL to indicate successful ribbon bar hit test.
    // I know that HTHSROLL normally indicates "in a horizontal scroll bar", but I decided
    // that this will -not- be an issue (for me, at least).
    if (rcRibbonBar.PtInRect(point)) {
      return HTHSCROLL;
    }
    SetMsgHandled(FALSE);
    return 0;
  }
  // Contrast this function with OnNcMouseMove.
  // There are always -two- selected items -> a tab AND a sub-tab.
  void OnNcLButtonDown(UINT nHitTest, CPoint point) {
    if (nHitTest != HTHSCROLL) {
      SetMsgHandled(FALSE);
      return;
    }
    // Mouse was used, so don't show the mnemonics
    m_bShowPrefixes = FALSE;
    // Map the screen coordinates to client coordinates...
    ::MapWindowPoints(NULL, m_hWnd, &point, 1);
    // ...and from client to window coordinates.
    point.x += m_rcRibbonBar.left;
    point.y += m_rcRibbonBar.top + m_rcRibbonBar.Height();
    UpdateSelectedTab(point);
  }
  // Contrast this function with OnNcLButtonDown.
  // There can only be -one- item that is hot -> a tab OR a sub-tab.
  void OnNcMouseMove(UINT nHitTest, CPoint point) {
    if (nHitTest != HTHSCROLL) {
      SetMsgHandled(FALSE);
      return;
    }
    // Map the screen coordinates to client coordinates...
    ::MapWindowPoints(NULL, m_hWnd, &point, 1);
    // ...and from client to window coordinates.
    point.x += m_rcRibbonBar.left;
    point.y += m_rcRibbonBar.top + m_rcRibbonBar.Height();
    // Hit test
    const UINT uState = RD_TST_HOT;
    static UINT uPrevHot = 0;
    UINT uCurHot = 0;
    for (int i = 0, iTabCount = m_listTabs.GetCount(); i < iTabCount; i++) {
      // Two parts to this if statement:
      // Part 1: Hit test
      // Part 2: If Hit test is successful (short-circuit evaluation), then set the state bit
      // and check that it is only in that one state (i.e., HOT and only HOT)
      if ((::PtInRect(&m_listTabs[i].rc, point) == TRUE) && ((m_listTabs[i].uState |= uState) == uState)) {
        uCurHot = m_listTabs[i].uID;
      }
      else {
        m_listTabs[i].uState &= ~uState; // clear bit
        // Only need to check the sub-tabs of a selected tab
        if (m_listTabs[i].uState & RD_TST_SELECTED) {
          LISTSUBTABS* lst = m_listTabs[i].listSubtabs;
          for (int j = 0, iSubtabCount = lst->GetCount(); j < iSubtabCount; j++) {
            RIBBONDLGTAB* pSubtab = &lst->GetAt(j);
            if ((::PtInRect(&pSubtab->rc, point) == TRUE) && ((pSubtab->uState |= uState) == uState)) {
              uCurHot = pSubtab->uID;
            }
            else {
              pSubtab->uState &= ~uState; // clear bit
            }
          }
        }
      }
    }
    if (uCurHot != uPrevHot) {
      uPrevHot = uCurHot;
      // Now do the painting
      DoRepaint(FALSE /* not necessary to repaint background */, 0 /* no animation for hot-tracking */);
    }
  }
  void OnNcPaint(CRgn rgn) {
    // Need to do this first!
    // Draw default window NC area
    LRESULT lResult = ::DefWindowProc(m_hWnd, WM_NCPAINT, (WPARAM)(HRGN)rgn, 0);
    CRect rcWindow, rcClient;
    // Get client rect in -screen- coordinates
    // FYI: Old way uses ClientToScreen: http://support.microsoft.com/kb/11570
    GetClientRect(rcClient);
    MapWindowPoints(NULL, (LPPOINT)&rcClient, 2);
    // Figure out the Ribbon Bar rect
    GetWindowRect(rcWindow);
    m_rcRibbonBar.left = (rcClient.left - rcWindow.left);
    m_rcRibbonBar.right = m_rcRibbonBar.left + rcClient.Width();
    m_rcRibbonBar.top = (rcClient.top - rcWindow.top) - m_iRibbonBarHeight;
    m_rcRibbonBar.bottom = m_rcRibbonBar.top + m_iRibbonBarHeight;
    // Set up the drawing
    HDC hDC = GetWindowDC();
    CDCHandle dcOut(hDC);
    // Do drawing an buffered/animated DC when available (Vista+), otherwise on Memory DC.
    CBufferedAnimation ba;
    CMemoryDC* memDC = NULL;
    if (IsBufferedPaintSupported()) {
      // Do not paint during an animation
      if (CBufferedAnimation::IsRendering(m_hWnd, hDC)) {
        ReleaseDC(hDC);
        return;
      }
      HDC hdcFrom = NULL, hdcTo = NULL;
      ba.Begin(m_hWnd, hDC, &m_rcRibbonBar, m_BufferFormat, &m_PaintParams, &m_AnimationParams, &hdcFrom, &hdcTo);
      if (ba.IsNull() == FALSE) {
        if (hdcFrom) {
          // Do this, and "magically", Windows will use the previous image to animate from.
          // It's the strangest thing!
          ::DeleteDC(hdcFrom);
        }
        if (hdcTo) {
          dcOut.m_hDC = hdcTo;
        }
      }
    }
    else {
      CRect rc(0, 0, m_rcRibbonBar.left + m_rcRibbonBar.Width(), m_rcRibbonBar.top + m_rcRibbonBar.Height());
      memDC = new CMemoryDC(hDC, rc);
      dcOut.m_hDC = memDC->m_hDC;
      // I do this to get around setting the viewport (i.e., SetViewportOrg) and dealing with that.
      // That's also why I made the rect (rc) bigger than it really needs to be.
      memDC->m_rcPaint = m_rcRibbonBar;
    }
    // Fill background, always, to start
    dcOut.FillSolidRect(m_rcRibbonBar, IsCompositionEnabled() ? 0 : TABBACKGROUNDCOLOR);
    // Create a bitmap based on the output DC
    Graphics grfx(dcOut);
    // Paint background
    if (m_bRepaintBackground || !m_pCachedBkBitmap) {
      m_bRepaintBackground = FALSE;
      Bitmap bmp(m_rcRibbonBar.left + m_rcRibbonBar.Width(), m_rcRibbonBar.top + m_rcRibbonBar.Height(), &grfx);
      Graphics memgfx(&bmp);
      PaintBackground(memgfx);
      if (m_pCachedBkBitmap) {
        delete m_pCachedBkBitmap;
      }
      // Cache the background image
      m_pCachedBkBitmap = new CachedBitmap(&bmp, &grfx);
    }
    // Put the background on the output canvas
    grfx.DrawCachedBitmap(m_pCachedBkBitmap, 0, 0);
    // Draw the tab text
    PaintForeground(dcOut);
    // Clean up!
    if (ba.IsNull() == FALSE) {
      ba.End();
    }
    if (memDC) {
      delete memDC;
    }
    ReleaseDC(hDC);
  }
private: // ---------------------------------------------------------------------------
  void UpdateSelectedTab(CPoint& point, UINT uID = 0) {
    BOOL bSourceIsMouse = (point.x != 0) && (point.y != 0);
    BOOL bRepaint = FALSE;
    // Hit test: part 1 (tabs)
    const UINT uState = RD_TST_SELECTED;
    static int iPrevSelectedTab = 0;
    int iCurSelectedTab = -1;
    for (int i = 0, iTabCount = m_listTabs.GetCount(); i < iTabCount; i++) {
      BOOL bHit = bSourceIsMouse ? (::PtInRect(&m_listTabs[i].rc, point) == TRUE) : (m_listTabs[i].uID == uID);
      if (bHit) {
        m_listTabs[i].uState |= uState; // set bit
        iCurSelectedTab = i;
      }
      else {
        m_listTabs[i].uState &= ~uState; // clear bit
      }
    }
    // This means a tab was not selected. So keep the selection on the previous tab.
    if (iCurSelectedTab == -1) {
      m_listTabs[iPrevSelectedTab].uState |= uState;
      iCurSelectedTab = iPrevSelectedTab;
    }
    else if (iPrevSelectedTab != iCurSelectedTab) {
      iPrevSelectedTab = iCurSelectedTab;
      bRepaint = TRUE;
    }
    // Hit test: part 2 (subtabs)
    int iPrevSelectedItem = 0, iCurSelectedItem = -1;
    LISTSUBTABS* lst = m_listTabs[iCurSelectedTab].listSubtabs;
    for (int j = 0, iSubtabCount = lst->GetCount(); j < iSubtabCount; j++) {
      RIBBONDLGTAB* pSubtab = &lst->GetAt(j);
      if (pSubtab->uState & uState) {
        iPrevSelectedItem = j;
      }
      BOOL bHit = bSourceIsMouse ? (::PtInRect(&pSubtab->rc, point) == TRUE) : (pSubtab->uID == uID);
      if (bHit) {
        pSubtab->uState |= uState; // set bit
        iCurSelectedItem = j;
      }
      else {
        pSubtab->uState &= ~uState; // clear bit
      }
    }
    // This means a sub-tab was not selected. So keep the selection on the previous sub-tab.
    if (iCurSelectedItem == -1) {
      RIBBONDLGTAB* pSubtab = &lst->GetAt(iPrevSelectedItem);
      pSubtab->uState |= uState;
      iCurSelectedItem = iPrevSelectedItem;
    }
    else if (iPrevSelectedItem != iCurSelectedItem) {
      iPrevSelectedItem = iCurSelectedItem;
      bRepaint = TRUE;
    }
    if (bRepaint) {
      DoRepaint();
      // Generate WM_COMMAND message
      PostMessage(WM_COMMAND, MAKEWORD(lst->GetAt(iCurSelectedItem).uID, 0));
    }
  }
  void DoRepaint(BOOL bRepaintBackground = TRUE, DWORD dwAnimationDuration = TABCHANGEANIMATIONDURATION) {
    if (bRepaintBackground) {
      m_bRepaintBackground = TRUE;
      // This also means that we have to (re-)generate the accelerator table
      GetAcceleratorTable(TRUE);
    }
    m_AnimationParams.dwDuration = dwAnimationDuration; // I put this here, because I want dwDuration=0 on the first NCPAINT.
    // Otherwise, will get a fade-in from an (ugly) white box.
    RedrawWindow(&m_rcRibbonBar, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_ERASENOW);
  }
  // Paints the background: background fill, selected tab and selected sub-tab
  void PaintBackground(Graphics& graphics) {
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    // Fill background
    CDCHandle dc(graphics.GetHDC());
    if (IsCompositionEnabled()) {
      DrawThemeParentBackground(dc, &m_rcRibbonBar);
    }
    else {
      dc.FillSolidRect(m_rcRibbonBar, TABBACKGROUNDCOLOR);
    }
    graphics.ReleaseHDC(dc);
    CRect rcTabs(m_rcRibbonBar), rcRibbonBar(m_rcRibbonBar);
    rcTabs.left += RIBBONBARPADDING;
    rcTabs.top += RIBBONBARPADDING;
    rcRibbonBar.DeflateRect(IsCompositionEnabled() ? 2 : RIBBONBARPADDING, RIBBONBARPADDING);
    // Draw tabs
    int iTabCount = m_listTabs.GetCount();
    ATLENSURE(iTabCount > 0);
    for (int i = 0; i < iTabCount; i++) {
      // Calculate item rectangle, if necessary
      if (m_listTabs[i].rc.right == 0) {
        CRect rcTab(GetTextRect(m_listTabs[i].uID, graphics));
        // Add margins
        rcTab.MoveToXY(rcTabs.left + TABINSET, rcTabs.top);
        rcTab.InflateRect(0, 0, TABPADDINGWIDTH, TABPADDINGHEIGHT);
        // Save it
        m_listTabs[i].rc = rcTab;
        // Next tab starts at...
        rcTabs.left += rcTab.Width() + PADDINGBETWEENTABS;
      }
      // Draw selection, if necessary
      if (m_listTabs[i].uState & RD_TST_SELECTED) {
        CRect rcTab(m_listTabs[i].rc);
        // Figure out the path around the checked tab and its sub-tabs
        GraphicsPath path;
        path.AddArc(rcTab.left, rcTab.top, 10, 10, 180.0f, 90.0f);
        path.AddLine(rcTab.left + 10, rcTab.top, rcTab.right - 10, rcTab.top);
        path.AddArc(rcTab.right - 10, rcTab.top, 10, 10, 270.0f, 90.0f);
        path.AddLine(rcTab.right, rcTab.top + 10, rcTab.right, rcTab.bottom - 10);
        path.AddArc(rcTab.right, rcTab.bottom - 10, 10, 10, 180.0f, -90.0f);
        path.AddLine(rcTab.right + 10, rcTab.bottom, rcRibbonBar.right - 10, rcTab.bottom);
        path.AddArc(rcRibbonBar.right - 10, rcTab.bottom, 10, 10, 270.0f, 90.0f);
        path.AddLine(rcRibbonBar.right, rcTab.bottom + 10, rcRibbonBar.right, rcRibbonBar.bottom - 10);
        path.AddArc(rcRibbonBar.right - 10, rcRibbonBar.bottom - 10, 10, 10, 0.0f, 90.0f);
        path.AddLine(rcRibbonBar.right - 10, rcRibbonBar.bottom, rcRibbonBar.left + 10, rcRibbonBar.bottom);
        path.AddArc(rcRibbonBar.left, rcRibbonBar.bottom - 10, 10, 10, 90.0f, 90.0f);
        path.AddLine(rcRibbonBar.left, rcRibbonBar.bottom - 10, rcRibbonBar.left, rcTab.bottom + 10);
        path.AddArc(rcRibbonBar.left, rcTab.bottom, 10, 10, 180.0f, 90.0f);
        path.AddLine(rcRibbonBar.left + 10, rcTab.bottom, rcTab.left - 10, rcTab.bottom);
        path.AddArc(rcTab.left - 10, rcTab.bottom - 10, 10, 10, 90.0f, -90.0f);
        path.AddLine(rcTab.left, rcTab.bottom - 10, rcTab.left, rcTab.top + 5);
        // Fill the path
        Rect rect(rcRibbonBar.left, rcRibbonBar.top, rcRibbonBar.Width(), rcRibbonBar.Height() + 1);
        LinearGradientBrush gb(rect, Color::White, Color::Black, LinearGradientModeVertical);
        Color colours[5] = {Color(TAB_FILL_GRADIENTSTART), Color(TAB_FILL_GRADIENTMID), Color(TAB_FILL_GRADIENTEND), Color(TAB_FILL_GRADIENTMID), Color(TAB_FILL_GRADIENTEND)};
        REAL blend[5] = {0.0f, 0.2f, 0.45f, 0.5f, 1.0f};
        gb.SetInterpolationColors(colours, blend, 5);
        graphics.FillPath(&gb, &path);
        // Outline the path
        Pen pen1(Color::WhiteSmoke, 2.0f), pen2(Color(TAB_OUTLINE));
        graphics.DrawPath(&pen1, &path);
        graphics.DrawPath(&pen2, &path);
        // Now, draw sub-tabs
        LISTSUBTABS* lst = m_listTabs[i].listSubtabs;
        int iSubtabCount = lst->GetCount();
        ATLENSURE(iSubtabCount > 0);
        CRect rcSubtabs(rcRibbonBar);
        rcSubtabs.top = rcTab.bottom;
        rcSubtabs.DeflateRect(SUBTABPADDING, SUBTABPADDING);
        for (int j = 0; j < iSubtabCount; j++) {
          RIBBONDLGTAB* pSubtab = &lst->GetAt(j);
          // Calculate item rectangle, if necessary
          if (pSubtab->rc.right == 0) {
            CRect rcSubtab(GetTextRect(pSubtab->uID, graphics));
            // Make room for an icon, if necessary
            if (pSubtab->uIconResID) {
              rcSubtab.right += rcSubtab.Height();
            }
            // Add margins
            rcSubtab.MoveToXY(rcSubtabs.left, rcSubtabs.top);
            rcSubtab.InflateRect(0, 0, SUBTABPADDINGWIDTH, SUBTABPADDINGHEIGHT);
            // Save it
            pSubtab->rc = rcSubtab;
            // Next sub-tab starts at...
            rcSubtabs.left += rcSubtab.Width() + PADDINGBETWEENSUBTABS;
          }
          CRect rcSubtab(pSubtab->rc);
          if (pSubtab->uState & RD_TST_SELECTED) {
            // Figure out the path around the selected sub-tab
            GraphicsPath path;
            path.AddArc(rcSubtab.left, rcSubtab.top, 10, 10, 180.0f, 90.0f);
            path.AddLine(rcSubtab.left + 10, rcSubtab.top, rcSubtab.right - 10, rcSubtab.top);
            path.AddArc(rcSubtab.right - 10, rcSubtab.top, 10, 10, 270.0f, 90.0f);
            // Finish path around top-half of item
            GraphicsPath* pathCopy = path.Clone();
            pathCopy->AddLine(rcSubtab.right, rcSubtab.top + 10, rcSubtab.right, rcSubtab.Height() / 2);
            pathCopy->AddLine(rcSubtab.right, rcSubtab.top + rcSubtab.Height() / 2, rcSubtab.left, rcSubtab.top + rcSubtab.Height() / 2);
            pathCopy->AddLine(rcSubtab.left, rcSubtab.top + rcSubtab.Height() / 2, rcSubtab.left, rcSubtab.top + 10);
            // Finish path around entire item
            path.AddLine(rcSubtab.right, rcSubtab.top + 10, rcSubtab.right, rcSubtab.bottom - 10);
            path.AddArc(rcSubtab.right - 10, rcSubtab.bottom - 10, 10, 10, 0.0f, 90.0f);
            path.AddLine(rcSubtab.right - 10, rcSubtab.bottom, rcSubtab.left + 10, rcSubtab.bottom);
            path.AddArc(rcSubtab.left, rcSubtab.bottom - 10, 10, 10, 90.0f, 90.0f);
            path.AddLine(rcSubtab.left, rcSubtab.bottom - 10, rcSubtab.left, rcSubtab.top + 5);
            // Fills
            SolidBrush br1(Color(SUBTAB_FILL_TOPHALF)), br2(Color(SUBTAB_FILL_BOTTOMHALF));
            graphics.FillPath(&br1, &path);
            graphics.FillPath(&br2, pathCopy);
            // Outline
            Pen pen(Color(SUBTAB_OUTLINE));
            graphics.DrawPath(&pen, &path);
            // Clean up
            delete pathCopy;
          }
          // Draw icon, if necessary
          if (pSubtab->uIconResID) {
            int iIconSize = rcSubtab.Height() - SUBTABPADDING * 2;
            CIcon icon;
            icon.LoadIcon(pSubtab->uIconResID, 32, 32);
            Bitmap bmpIcon(icon);
            graphics.DrawImage(&bmpIcon, Rect(rcSubtab.left + SUBTABPADDING, rcSubtab.top + SUBTABPADDING, iIconSize, iIconSize),
                0, 0, bmpIcon.GetWidth(), bmpIcon.GetHeight(), UnitPixel, (pSubtab->uState & RD_TST_SELECTED) ? NULL : &m_imgAttributes);
          }
        }
      }
    }
  }
  RECT GetTextRect(UINT& uID, Graphics& graphics) {
    CString str;
    str.LoadString(uID);
    HDC hDC = graphics.GetHDC();
    RECT rcText = {0, 0, 0, 0};
    DrawText(hDC, str, -1, &rcText, DT_SINGLELINE | DT_CALCRECT);
    // All items are the same height, only the width can vary
    rcText.bottom = m_iTabHeight;
    graphics.ReleaseHDC(hDC);
    return rcText;
  }
  // Paints the text and icons
  void PaintForeground(CDCHandle& dc) {
    HFONT hFontOld = dc.SelectFont(m_font);
    dc.SetBkMode(TRANSPARENT);
    dc.SetTextColor(_RGB(0, 0, 0));
    // Draw tabs
    CString str;
    for (int i = 0, iTabCount = m_listTabs.GetCount(); i < iTabCount; i++) {
      str.LoadString(m_listTabs[i].uID);
      dc.SelectFont((m_listTabs[i].uState == RD_TST_HOT) ? m_fontHot : m_font);
      BOOL bShowPrefix = m_bShowPrefixes & ((m_listTabs[i].uState & RD_TST_SELECTED) == 0);
      DWORD dwTextFlags = DT_FORMATFLAGS | (bShowPrefix ? 0 : DT_HIDEPREFIX);
      // Draw text
      if (IsThemingSupported() && IsCompositionEnabled()) {
        m_dto.iGlowSize = 8;
        DrawThemeTextEx(dc, 0, 1, str, -1, dwTextFlags, &m_listTabs[i].rc, &m_dto);
        m_dto.iGlowSize = 0;
      }
      else {
        dc.DrawText(str, -1, &m_listTabs[i].rc, dwTextFlags);
      }
      // Draw subtabs, if necessary
      if (m_listTabs[i].uState & RD_TST_SELECTED) {
        LISTSUBTABS* lst = m_listTabs[i].listSubtabs;
        for (int j = 0, iSubtabCount = lst->GetCount(); j < iSubtabCount; j++) {
          RIBBONDLGTAB* pSubtab = &lst->GetAt(j);
          str.LoadString(pSubtab->uID);
          dc.SelectFont((pSubtab->uState == RD_TST_HOT) ? m_fontHot : m_font);
          BOOL bShowPrefix = m_bShowPrefixes && ((pSubtab->uState & RD_TST_SELECTED) == 0);
          DWORD dwTextFlags = DT_FORMATFLAGS | (bShowPrefix ? 0 : DT_HIDEPREFIX);
          CRect rcText(pSubtab->rc);
          // Adjust if there's an icon
          if (pSubtab->uIconResID) {
            rcText.left += rcText.Height() - SUBTABPADDING;
          }
          if (IsThemingSupported() && IsCompositionEnabled()) {
            DrawThemeTextEx(dc, 0, 1, str, -1, dwTextFlags, &rcText, &m_dto);
          }
          else {
            dc.DrawText(str, -1, &rcText, dwTextFlags);
          }
        }
      }
    }
    // Restore DC state
    dc.SelectFont(hFontOld);
  }
  // Helper for GetAcceleratorTable
  void GetACCELForStringResource(UINT& uID, CAtlArray<ACCEL>& listAccel) {
    CString str;
    str.LoadString(uID);
    int iAccelChar = str.Find(_T('&'));
    if (iAccelChar != -1) {
      CString strAccel = str.Mid(iAccelChar + 1, 1);
      // Check that there is a character after the ampersand (e.g., "Tab&" is bad!)
      if (strAccel != "\0") {
        // Want uppercase and lowercase letters
        CString strLower = strAccel.MakeLower(), strUpper = strAccel.MakeUpper();
        ACCEL accel = {FALT, strLower[0], uID};
        listAccel.Add(accel);
        if (strUpper != strLower) {
          accel.key = strUpper[0];
          listAccel.Add(accel);
        }
      }
    }
  }
  // Generates the accelerator table; called whenever the a new tab or sub-tab is checked
  // Why this way?
  // Say: Have "Proper&ties" and "Se&ttings"
  // Issue: Two "t" mnemonics!
  // So: To minimize mnemonic conflicts, only unchecked tabs and sub-tabs should be put in the accelerator tab.
  // Using the mnemonic for a checked tab or sub-tab doesn't mean anything anyway.
  HACCEL GetAcceleratorTable(BOOL bRegenerate = FALSE) {
    if (bRegenerate) {
      DestroyAcceleratorTable(m_hAccel);
      m_hAccel = NULL;
    }
    if (m_hAccel == NULL) {
      ACCEL accel = {0};
      CAtlArray<ACCEL> listAccel;
      for (int i = 0, iTabCount = m_listTabs.GetCount(); i < iTabCount; i++) {
        if ((m_listTabs[i].uState & RD_TST_SELECTED) == 0) {
          GetACCELForStringResource(m_listTabs[i].uID, listAccel);
        }
        else {
          LISTSUBTABS* lst = m_listTabs[i].listSubtabs;
          for (int j = 0, iSubtabCount = lst->GetCount(); j < iSubtabCount; j++) {
            if ((lst->GetAt(j).uState & RD_TST_SELECTED) == 0) {
              GetACCELForStringResource(lst->GetAt(j).uID, listAccel);
            }
          }
        }
      }
      m_hAccel = CreateAcceleratorTable(listAccel.GetData(), listAccel.GetCount());
    }
    return m_hAccel;
  }
  // Checks if Desktop Window Manager is enabled
  BOOL IsCompositionEnabled() {
    if (m_iCompositionEnabled == -1) {
      HRESULT hr = 0;
      BOOL bEnabled = FALSE;
      if (RunTimeHelper::IsVista()) {
        hr = DwmIsCompositionEnabled(&bEnabled);
      }
      m_iCompositionEnabled = SUCCEEDED(hr) && bEnabled;
    }
    return (m_iCompositionEnabled == 1);
  }
  // The next three functions are from Microsoft article ID 29240.
  // See: "How To Converting Colors Between _RGB and HLS"
  // http://support.microsoft.com/kb/29240
  void RGBtoHLS(COLORREF lRGBColor, BYTE& H, BYTE& L, BYTE& S) {
    WORD Rdelta, Gdelta, Bdelta; // intermediate value: % of spread from max
    // Get R, G, and B out of DWORD
    BYTE R = GetRValue(lRGBColor);
    BYTE G = GetGValue(lRGBColor);
    BYTE B = GetBValue(lRGBColor);
    // calculate lightness
    BYTE cMax = max(max(R, G), B); // max _RGB values
    BYTE cMin = min(min(R, G), B); // min _RGB values
    L = (((cMax + cMin) * HLSMAX) + RGBMAX) / (2 * RGBMAX);
    // r=g=b --> achromatic case
    if (cMax == cMin) {
      S = 0;
      H = UNDEFINED;
    }
    // chromatic case
    else {
      // saturation
      if (L <= (HLSMAX / 2)) {
        S = (((cMax - cMin) * HLSMAX) + ((cMax + cMin) / 2)) / (cMax + cMin);
      }
      else {
        S = (((cMax - cMin) * HLSMAX) + ((2 * RGBMAX - cMax - cMin) / 2)) / (2 * RGBMAX - cMax - cMin);
      }
      // hue
      Rdelta = (((cMax - R) * (HLSMAX / 6)) + ((cMax - cMin) / 2)) / (cMax - cMin);
      Gdelta = (((cMax - G) * (HLSMAX / 6)) + ((cMax - cMin) / 2)) / (cMax - cMin);
      Bdelta = (((cMax - B) * (HLSMAX / 6)) + ((cMax - cMin) / 2)) / (cMax - cMin);
      if (R == cMax) {
        H = Bdelta - Gdelta;
      }
      else if (G == cMax) {
        H = (HLSMAX / 3) + Rdelta - Bdelta;
      }
      else { // B == cMax
        H = ((2 * HLSMAX) / 3) + Gdelta - Rdelta;
      }
      if (H < 0) {
        H += HLSMAX;
      }
      if (H > HLSMAX) {
        H -= HLSMAX;
      }
    }
  }
  // utility routine for HLStoRGB
  WORD HueToRGB(WORD n1, WORD n2, WORD hue) {
    // range check: note values passed add/subtract thirds of range
    if (hue < 0) {
      hue += HLSMAX;
    }
    if (hue > HLSMAX) {
      hue -= HLSMAX;
    }
    // return r, g or b value from this tridrant
    if (hue < (HLSMAX / 6)) {
      return (n1 + (((n2 - n1) * hue + (HLSMAX / 12)) / (HLSMAX / 6)));
    }
    if (hue < (HLSMAX / 2)) {
      return (n2);
    }
    if (hue < ((HLSMAX * 2) / 3)) {
      return (n1 + (((n2 - n1) * (((HLSMAX * 2) / 3) - hue) + (HLSMAX / 12)) / (HLSMAX / 6)));
    }
    else {
      return (n1);
    }
  }
  DWORD HLStoRGB(WORD hue, WORD lum, WORD sat) {
    WORD R, G, B; // _RGB component values
    WORD Magic1, Magic2; // calculated magic numbers
    // achromatic case
    if (sat == 0) {
      R = G = B = (lum * RGBMAX) / HLSMAX;
      if (hue != UNDEFINED) {
        /* ERROR */
      }
    }
    // chromatic case
    else {
      // set up magic numbers
      if (lum <= (HLSMAX / 2)) {
        Magic2 = (lum * (HLSMAX + sat) + (HLSMAX / 2)) / HLSMAX;
      }
      else {
        Magic2 = lum + sat - ((lum * sat) + (HLSMAX / 2)) / HLSMAX;
      }
      Magic1 = 2 * lum - Magic2;
      // get _RGB, change units from HLSMAX to RGBMAX
      R = (HueToRGB(Magic1, Magic2, hue + (HLSMAX / 3)) * RGBMAX + (HLSMAX / 2)) / HLSMAX;
      G = (HueToRGB(Magic1, Magic2, hue) * RGBMAX + (HLSMAX / 2)) / HLSMAX;
      B = (HueToRGB(Magic1, Magic2, hue - (HLSMAX / 3)) * RGBMAX + (HLSMAX / 2)) / HLSMAX;
    }
    return (_RGB(R, G, B));
  }
};
#endif

