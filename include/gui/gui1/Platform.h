// Scintilla source code edit control
/** @file Platform.h
 ** Interface to platform facilities. Also includes some basic utilities.
 ** Implemented in PlatGTK.cxx for GTK+/Linux, PlatWin.cxx for Windows, and PlatWX.cxx for wxWindows.
 **/
// Copyright 1998-2009 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef PLATFORM_H
#define PLATFORM_H

// PLAT_GTK = GTK+ on Linux or Win32
// PLAT_GTK_WIN32 is defined additionally when running PLAT_GTK under Win32
// PLAT_WIN = Win32 API on Win32 OS
// PLAT_WX is wxWindows on any supported platform
// PLAT_TK = Tcl/TK on Linux or Win32

#define PLAT_GTK 0
#define PLAT_GTK_WIN32 0
#define PLAT_GTK_MACOSX 0
#define PLAT_MACOSX 0
#define PLAT_WIN 0
#define PLAT_WX  0
#define PLAT_QT 0
#define PLAT_FOX 0
#define PLAT_CURSES 0
#define PLAT_TK 0
#define PLAT_HAIKU 0

#if defined(FOX)
#undef PLAT_FOX
#define PLAT_FOX 1

#elif defined(__WX__)
#undef PLAT_WX
#define PLAT_WX  1

#elif defined(CURSES)
#undef PLAT_CURSES
#define PLAT_CURSES 1

#elif defined(__HAIKU__)
#undef PLAT_HAIKU
#define PLAT_HAIKU 1

#elif defined(SCINTILLA_QT)
#undef PLAT_QT
#define PLAT_QT 1

#elif defined(TK)
#undef PLAT_TK
#define PLAT_TK 1

#elif defined(GTK)
#undef PLAT_GTK
#define PLAT_GTK 1

#if defined(__WIN32__) || defined(_MSC_VER)
#undef PLAT_GTK_WIN32
#define PLAT_GTK_WIN32 1
#endif

#if defined(__APPLE__)
#undef PLAT_GTK_MACOSX
#define PLAT_GTK_MACOSX 1
#endif

#elif defined(__APPLE__)

#undef PLAT_MACOSX
#define PLAT_MACOSX 1

#else
#undef PLAT_WIN
#define PLAT_WIN 1

#endif

#include <vector>
#include "types.hpp"


#include "Scintilla/Scintilla.h"

// Underlying the implementation of the platform classes are platform specific types.
// Sometimes these need to be passed around by client code so they are defined here

NSBEGIN(Platform);

struct Font {
  FontID fid;
  Font() noexcept;
  // Deleted so Font objects can not be copied
  //Font(const Font &) = delete;
  //Font(Font &&) = delete;
  //Font &operator=(const Font &) = delete;
  //Font &operator=(Font &&) = delete;
  virtual ~Font();

  virtual void Create(const FontParameters & fp);
  virtual void Release();

  FontID GetID() const noexcept {
    return fid;
  }
  // Alias another font - caller guarantees not to Release
  void SetID(FontID fid_) noexcept { fid = fid_; }
};

struct IScreenLine {
  virtual std::string Text() const = 0;
  virtual size_t Length() const = 0;
  virtual size_t RepresentationCount() const = 0;
  virtual XYPOSITION Width() const = 0;
  virtual XYPOSITION Height() const = 0;
  virtual XYPOSITION TabWidth() const = 0;
  virtual XYPOSITION TabWidthMinimumPixels() const = 0;
  virtual const Font* FontOfPosition(size_t position) const = 0;
  virtual XYPOSITION RepresentationWidth(size_t position) const = 0;
  virtual XYPOSITION TabPositionAfter(XYPOSITION xPosition) const = 0;
};

struct Interval {
  XYPOSITION left;
  XYPOSITION right;
};

struct IScreenLineLayout {
public:
  //virtual ~IScreenLineLayout() = default;
  virtual size_t PositionFromX(XYPOSITION xDistance, bool charPosition) = 0;
  virtual XYPOSITION XFromPosition(size_t caretPosition) = 0;
  virtual std::vector<Interval> FindRangeIntervals(size_t start, size_t end) = 0;
};

/**
 * A surface abstracts a place to draw.
 */
struct Surface {
  //Surface() noexcept = default;
  //Surface(const Surface &) = delete;
  //Surface(Surface &&) = delete;
  //Surface &operator=(const Surface &) = delete;
  //Surface &operator=(Surface &&) = delete;
  virtual ~Surface() {}
  static Surface* Allocate(int technology);

  virtual void Init(WindowID wid) = 0;
  virtual void Init(SurfaceID sid, WindowID wid) = 0;
  virtual void InitPixMap(int width, int height, Surface* surface_, WindowID wid) = 0;

  virtual void Release() = 0;
  virtual bool Initialised() = 0;
  virtual void PenColour(Color fore) = 0;
  virtual int LogPixelsY() = 0;
  virtual int DeviceHeightFont(int points) = 0;
  virtual void MoveTo(int x_, int y_) = 0;
  virtual void LineTo(int x_, int y_) = 0;
  virtual void Polygon(PointF* pts, size_t npts, Color fore, Color back) = 0;
  virtual void RectangleDraw(RectF rc, Color fore, Color back) = 0;
  virtual void FillRectangle(RectF rc, Color back) = 0;
  virtual void FillRectangle(RectF rc, Surface & surfacePattern) = 0;
  virtual void RoundedRectangle(RectF rc, Color fore, Color back) = 0;
  virtual void AlphaRectangle(RectF rc, int cornerSize, Color fill, int alphaFill,
                              Color outline, int alphaOutline, int flags) = 0;
  enum GradientOptions { leftToRight, topToBottom };
  virtual void GradientRectangle(RectF rc, const std::vector<ColourStop> & stops, GradientOptions options) = 0;
  virtual void DrawRGBAImage(RectF rc, int width, int height, const unsigned char* pixelsImage) = 0;
  virtual void Ellipse(RectF rc, Color fore, Color back) = 0;
  virtual void Copy(RectF rc, PointF from, Surface & surfaceSource) = 0;

  virtual IScreenLineLayout* Layout(const IScreenLine* screenLine) = 0;

  virtual void DrawTextNoClip(RectF rc, Font & font_, XYPOSITION ybase, std::string text, Color fore, Color back) = 0;
  virtual void DrawTextClipped(RectF rc, Font & font_, XYPOSITION ybase, std::string text, Color fore, Color back) = 0;
  virtual void DrawTextTransparent(RectF rc, Font & font_, XYPOSITION ybase, std::string text, Color fore) = 0;
  virtual void MeasureWidths(Font & font_, std::string text, XYPOSITION* positions) = 0;
  virtual XYPOSITION WidthText(Font & font_, std::string text) = 0;
  virtual XYPOSITION Ascent(Font & font_) = 0;
  virtual XYPOSITION Descent(Font & font_) = 0;
  virtual XYPOSITION InternalLeading(Font & font_) = 0;
  virtual XYPOSITION Height(Font & font_) = 0;
  virtual XYPOSITION AverageCharWidth(Font & font_) = 0;

  virtual void SetClip(RectF rc) = 0;
  virtual void FlushCachedState() = 0;

  virtual void SetUnicodeMode(bool unicodeMode_) = 0;
  virtual void SetDBCSMode(int codePage) = 0;
  virtual void SetBidiR2L(bool bidiR2L_) = 0;
};

struct Widget {
  Widget* _sub;
  Widget* next;
  Widget* prev;
  Widget() : _sub(NULL), next(NULL), prev(NULL) {}
  Widget* add(Widget* w) {
    list_add_head(w, &this->_sub);
    return this;
  }
  Slot* mouseSlot;
  Slot* paintSlot;
  
  virtual void OnPaint(Surface* s, RectF& rcPaint) {
    //printf("Widget OnPaint\n");
  }
  virtual void mouseEvent(const MouseEvent & e) {
    printf("Widget mousePressEvent\n");
  }
};


Widget* widget()
{
  Widget* w = new Widget;
  assert(w);
  return w;
}

void del_widget(Widget* w)
{
  if (w) {
    del_widget(w->_sub);
    del_widget(w->next);
    list_del(w);
    delete(w);
  }
  return;
}

/**
 * struct to hide the details of window manipulation.
 * Does not own the window which will normally have a longer life than this object.
 */
struct Window {
  WindowID wid;
  enum Cursor { cursorInvalid, cursorText, cursorArrow, cursorUp, cursorWait, cursorHoriz, cursorVert, cursorReverseArrow, cursorHand };
  Cursor cursorLast;
  Signal2<Surface*, RectF&> paintSignel;
  Signal<const MouseEvent&> mouseSignel;

Window() noexcept :
  wid(nullptr), cursorLast(cursorInvalid) {
  }
  bool Create();
  //Window(const Window &source) = delete;
  //Window(Window &&) = delete;
  Window & operator=(WindowID wid_) noexcept {
    wid = wid_;
    cursorLast = cursorInvalid;
    return *this;
  }
  Window* widget(Widget* w) {
    if (w) {
      printf("asdf\n");
      w->mouseSlot = mouseSignel.connect2(&Widget::mouseEvent, w);
      w->paintSlot = paintSignel.connect2(&Widget::OnPaint, w);
      widget(w->_sub);
      widget(w->next);
    }
    return this;
  }
  //Window &operator=(const Window &) = delete;
  //Window &operator=(Window &&) = delete;
  virtual ~Window();
  WindowID GetID() const noexcept {
    return wid;
  }
  bool Created() const noexcept {
    return wid != nullptr;
  }
  void Destroy();
  RectF GetPosition() const;
  void SetPosition(RectF rc);
  void SetPositionRelative(RectF rc, const Window* relativeTo);
  RectF GetClientPosition() const;
  void Show(bool show = true);
  void InvalidateAll();
  void InvalidateRectangle(RectF rc);
  virtual void SetFont(Font & font);
  void SetCursor(Cursor curs);
  RectF GetMonitorRect(Point pt);
  int SetCaption(const char* title);
  std::string GetCaption() const;
  int Move(int x, int y, int width, int height);

};


/**
 * Listbox management.
 */

// ScintillaBase implements IListBoxDelegate to receive ListBoxEvents from a ListBox

struct ListBoxEvent {
  enum EventType { selectionChange, doubleClick } event;
ListBoxEvent(EventType event_) noexcept :
  event(event_) {
  }
};

struct IListBoxDelegate {
public:
  virtual void ListNotify(ListBoxEvent* plbe) = 0;
};

struct ListBox : public Window {
public:
  ListBox() noexcept;
  ~ListBox() override;
  static ListBox* Allocate();

  void SetFont(Font & font) override = 0;
  virtual void Create(Window & parent, int ctrlID, PointF location, int lineHeight_, bool unicodeMode_, int technology_) = 0;
  virtual void SetAverageCharWidth(int width) = 0;
  virtual void SetVisibleRows(int rows) = 0;
  virtual int GetVisibleRows() const = 0;
  virtual RectF GetDesiredRect() = 0;
  virtual int CaretFromEdge() = 0;
  virtual void Clear() = 0;
  virtual void Append(char* s, int type = -1) = 0;
  virtual int Length() = 0;
  virtual void Select(int n) = 0;
  virtual int GetSelection() = 0;
  virtual int Find(const char* prefix) = 0;
  virtual void GetValue(int n, char* value, int len) = 0;
  virtual void RegisterImage(int type, const char* xpm_data) = 0;
  virtual void RegisterRGBAImage(int type, int width, int height, const unsigned char* pixelsImage) = 0;
  virtual void ClearRegisteredImages() = 0;
  virtual void SetDelegate(IListBoxDelegate* lbDelegate) = 0;
  virtual void SetList(const char* list, char separator, char typesep) = 0;
};

/**
 * Menu management.
 */
struct Menu {
  MenuID mid;
public:
  Menu() noexcept;
  MenuID GetID() const noexcept {
    return mid;
  }
  void CreatePopUp();
  void Destroy();
  void Show(Point pt, Window & w);
};

/**
 * Dynamic Library (DLL/SO/...) loading
 */
struct DynamicLibrary {
public:
  //virtual ~DynamicLibrary() = default;

  /// @return Pointer to function "name", or NULL on failure.
  virtual Function FindFunction(const char* name) = 0;

  /// @return true if the library was loaded successfully.
  virtual bool IsValid() = 0;

  /// @return An instance of a DynamicLibrary subclass with "modulePath" loaded.
  static DynamicLibrary* Load(const char* modulePath);
};

#if defined(__clang__)
# if __has_feature(attribute_analyzer_noreturn)
#  define CLANG_ANALYZER_NORETURN __attribute__((analyzer_noreturn))
# else
#  define CLANG_ANALYZER_NORETURN
# endif
#else
# define CLANG_ANALYZER_NORETURN
#endif

/**
 * Platform struct used to retrieve system wide parameters such as double click speed
 * and chrome colour. Not a creatable object, more of a module with several functions.
 */
static Color Chrome();
static Color ChromeHighlight();
static const char* DefaultFont();
static int DefaultFontSize();
static unsigned int DoubleClickTime();
static void DebugDisplay(const char* s);
static constexpr long LongFromTwoShorts(short a, short b) noexcept {
  return (a) | ((b) << 16);
}

static void DebugPrintf(const char* format, ...);
static bool ShowAssertionPopUps(bool assertionPopUps_);
static void Assert(const char* c, const char* file, int line) CLANG_ANALYZER_NORETURN;


NSEND();


#ifdef  NDEBUG
#define PLATFORM_ASSERT(c) ((void)0)
#else
#define PLATFORM_ASSERT(c) ((c) ? (void)(0) : Platform::Assert(#c, __FILE__, __LINE__))
#endif


#endif
