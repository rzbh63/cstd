#define GLUTCALLBACK
#include "sys/win32.inl"
//#include "ui/window.inl"
typedef struct TinyWGLContext {
  GLContext* gl_context;
  int do_convert; /* true if must do convertion to X11 format */
  int shm_use;
  int CompletionType;
  HWND hwnd;
  int x, y;
  img_t im[1];
  void (GLUTCALLBACK* idle)(void);
  void (GLUTCALLBACK* display)(void);
  void (GLUTCALLBACK* reshape)(int width, int height);
  void (GLUTCALLBACK* mouse)(int button, int state, int x, int y);
  void (GLUTCALLBACK* keyboard)(unsigned char key, int x, int y);
} TinyWGLContext;
static TinyWGLContext g_glut[1] = {0};
int gl_resize_viewport(GLContext* c, int h, int w)
{
  img_t* im = g_glut->im;
  imsetsize(im, h, w, 3, 1);
  ZB_Init(c->zb, h, w, im->tt.data, im->s, im->c << 3);
  return 0;
}
#define OutputDebugLastError() \
  { static int _time=0; int _err = GetLastError(); TCHAR buf[256]; TCHAR* lpMsgBuf; FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, \
        NULL, _err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL );\
    _snprintf(buf, 256, "%s(%d):%d:%d:%s", __FILE__, __LINE__, _err, _time++, lpMsgBuf); if (_err) OutputDebugString( buf ); LocalFree( lpMsgBuf ); }
void glutSwapBuffers()
{
  return ;
}
void glutInit(int* argc, char** argv)
{
  glInit();
  glOrtho(-1, 1, -1, 1, -1, 1);
  return ;
}
/* GLUT initialization sub-API. */
void glutInitDisplayMode(unsigned int mode)
{
}
void glutInitDisplayString(const char* string);
void glutInitWindowPosition(int x, int y)
{
  g_glut->x = x;
  g_glut->y = y;
}
void glutInitWindowSize(int width, int height)
{
  imsetsize(g_glut->im, width, height, GLCN, 1);
  glViewport(0, 0, width, height);
}
void glDepthFunc(GLenum func)
{
  //ASSERT(0);
  return ;
}
BOOL glCopyContext(HGLRC h, HGLRC a, UINT b)
{
  ASSERT(0);
  return 0;
}
void glutDeleteContext(void)
{
  TinyWGLContext* ctx = g_glut;
  if (ctx->gl_context != NULL) {
    glClose();
  }
  imfree(ctx->im);
  ASSERT(0);
  return ;
}
HGLRC glGetCurrentContext(VOID)
{
  ASSERT(0);
  return 0;
}
PROC glGetProcAddress(LPCSTR str)
{
  ASSERT(0);
  return 0;
}
BOOL glShareLists(HGLRC h, HGLRC a)
{
  ASSERT(0);
  return 0;
}
BOOL glUseFontBitmapsA(HDC h, DWORD a, DWORD b, DWORD c)
{
  ASSERT(0);
  return 0;
}
BOOL glUseFontBitmapsW(img_t* im, DWORD a, DWORD b, DWORD c)
{
  ASSERT(0);
  return 0;
}
LRESULT CALLBACK gl_wndproc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  GLContext* c = gl_get_context();
  int k = 0;
  switch (uMsg) {
  case WM_SIZE: {
    int w = LOWORD(lParam), h = HIWORD(lParam);
    gl_resize_viewport(c, h, w);
    if (g_glut->reshape) {
      g_glut->reshape(w, h);
    }
  }
  break;
  case WM_PAINT:
    if (g_glut->display) {
      PAINTSTRUCT ps = {0};
      HDC hDC = BeginPaint(hwnd, &ps);
      //img_t im[1] = {0};
      //hdc_get_image(hDC, im);
      {
        //utime_start(_start_time);
        g_glut->display();
        //printf("%f\n", utime_elapsed(_start_time));
      }
      //cvShowImage("adsf", c->zb->h, c->zb->w, c->zb->pbuf, c->zb->linesize, 3);cvWaitKey(-1);
      {
        HBITMAP hbmp;
        uchar buffer[ sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD) ];
        BITMAPINFO* binfo = (BITMAPINFO*) buffer;
        uchar* dst_ptr;
        HDC hDCMem;
        ZBuffer* zb = c->zb;
        int dststep = (zb->w * 3 + 3) & -4;
        int cn = BPP2CN(zb->bpp);
        RECT rc = {0, 0, 100, 100};
        hDCMem = CreateCompatibleDC(hDC);
        FillBitmapInfo(binfo, zb->w, zb->h, zb->bpp, 1, NULL);
        hbmp = CreateDIBSection(hDCMem, binfo, DIB_RGB_COLORS, (void**)&dst_ptr, 0, 0);
        //memcpy2d(dst_ptr, dststep, g_glut->image, g_glut->linesize, im->h, im->w*PSZB);
        DeleteObject(SelectObject(hDCMem, hbmp));
        //FillSolidRect(hDCMem, &rc, RGB(255, 0, 0));
        memflip(dst_ptr, dststep, zb->pbuf, zb->step, zb->h, zb->w * cn);
        //cvShowImage("adsf", zb->zb->h, zb->zb->w, dst_ptr, dststep, 3);cvWaitKey(-1);
        BitBlt(hDC, 0, 0, zb->w, zb->h, hDCMem, 0, 0, SRCCOPY);
        DeleteObject(hbmp);
        DeleteDC(hDCMem);
      }
      //ZB_copyFrameBuffer(gl_context->zb, im->tt.data, im->s);
      EndPaint(hwnd, &ps);
    }
    else {
      return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    break;
  case WM_LBUTTONDOWN:
    break;
  case WM_KEYDOWN:
    if (g_glut->keyboard) {
      POINT pt[1];
      GetCursorPos(pt);
      ScreenToClient(hwnd, pt);
      //WindowToClient
      switch (wParam) {
#define KEY_MAP(a, b) case a: k = b; break
        KEY_MAP(VK_UP, GLUT_KEY_UP);
        KEY_MAP(VK_DOWN, GLUT_KEY_DOWN);
        KEY_MAP(VK_RIGHT, GLUT_KEY_RIGHT);
        KEY_MAP(VK_ESCAPE, GLUT_KEY_ESCAPE);
#undef KEY_MAP
      default:
        k = wParam;
      }
      g_glut->keyboard(k, pt->x, pt->y);
    }
    else {
      return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    break;
  case WM_CLOSE:
    if (1) {
      DestroyWindow(hwnd);
    }
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    glutDeleteContext();
    break;
  default:
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
  }
  return 0;
}
const char* g_title = NULL;
void glutMainLoop(void)
{
  GLContext* c = gl_get_context();
  img_t* im = g_glut->im;
  ZB_Init(c->zb, im->h, im->w, im->tt.data, im->s, im->c << 3);
  {
    DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    RECT prc[1] = {0};
    HINSTANCE hInstance = NULL;
    int x = CW_USEDEFAULT, y = CW_USEDEFAULT, w = CW_USEDEFAULT, h = CW_USEDEFAULT;
    x = g_glut->x;
    y = g_glut->y;
    w = im->w;
    h = im->h;
    SetRect(prc, x, y, x + w, y + h);
    AdjustWindowRectEx(prc, dwStyle, 0, 0);
    x = prc->left;
    y = prc->top;
    w = prc->right - prc->left;
    h = prc->bottom - prc->top;
    g_glut->hwnd = CreateWindow("opengl_win_class", g_title, dwStyle, x, y, w, h, NULL, NULL, hInstance, NULL);
  }
  if (g_glut->hwnd) {
    MSG Msg;
    //��Ϣѭ��
    while (GetMessage(&Msg, g_glut->hwnd, 0, 0)) {
      TranslateMessage(&Msg);
      DispatchMessage(&Msg);
    }
  }
  return ;
}
/* GLUT window sub-API. */
int glutCreateWindow(const char* title)
{
  static int inited = 0;
  HINSTANCE hInstance = NULL;
  if (!inited) {
    WNDCLASS wndcls;
    wndcls.cbClsExtra = 0;
    wndcls.cbWndExtra = 0;
    wndcls.hbrBackground = (HBRUSH)GetStockObject(WHITE_PEN);
    wndcls.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndcls.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndcls.hInstance = hInstance;
    wndcls.lpfnWndProc = gl_wndproc;
    wndcls.lpszClassName = "opengl_win_class";
    wndcls.lpszMenuName = NULL;
    wndcls.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wndcls);
    inited = 1;
  }
  g_title = title;
  return g_glut->hwnd != NULL;
}
int glutCreateSubWindow(int win, int x, int y, int width, int height);
void glutDestroyWindow(int win)
{
  return ;
}
void glutPostRedisplay(void)
{
  return ;
}
void glutPostWindowRedisplay(int win);
void glutSwapBuffers(void);
int glutGetWindow(void);
void glutSetWindow(int win);
void glutSetWindowTitle(const char* title);
void glutSetIconTitle(const char* title);
void glutPositionWindow(int x, int y);
void glutReshapeWindow(int width, int height);
void glutPopWindow(void);
void glutPushWindow(void);
void glutIconifyWindow(void);
void glutShowWindow(void);
void glutHideWindow(void);
void glutFullScreen(void);
void glutSetCursor(int cursor);
void glutWarpPointer(int x, int y);
/* GLUT overlay sub-API. */
void glutEstablishOverlay(void);
void glutRemoveOverlay(void);
void glutUseLayer(GLenum layer);
void glutPostOverlayRedisplay(void);
void glutPostWindowOverlayRedisplay(int win);
void glutShowOverlay(void);
void glutHideOverlay(void);
/* GLUT menu sub-API. */
void glutDestroyMenu(int menu);
int glutGetMenu(void);
void glutSetMenu(int menu);
void glutAddMenuEntry(const char* label, int value);
void glutAddSubMenu(const char* label, int submenu);
void glutChangeToMenuEntry(int item, const char* label, int value);
void glutChangeToSubMenu(int item, const char* label, int submenu);
void glutRemoveMenuItem(int item);
void glutAttachMenu(int button);
void glutDetachMenu(int button);
/* GLUT window callback sub-API. */
void glutDisplayFunc(void (GLUTCALLBACK* func)(void))
{
  g_glut->display = func;
}
void glutReshapeFunc(void (GLUTCALLBACK* func)(int width, int height))
{
  g_glut->reshape = func;
}
void glutKeyboardFunc(void (GLUTCALLBACK* func)(unsigned char key, int x, int y))
{
  g_glut->keyboard = func;
}
void glutKeyboardFunc(void (GLUTCALLBACK* func)(unsigned char key, int x, int y));
void glutMouseFunc(void (GLUTCALLBACK* func)(int button, int state, int x, int y))
{
  g_glut->mouse = func;
}
void glutMotionFunc(void (GLUTCALLBACK* func)(int x, int y));
void glutPassiveMotionFunc(void (GLUTCALLBACK* func)(int x, int y));
void glutEntryFunc(void (GLUTCALLBACK* func)(int state));
void glutVisibilityFunc(void (GLUTCALLBACK* func)(int state));
void glutIdleFunc(void (GLUTCALLBACK* func)(void))
{
  g_glut->idle = func;
}
void glutTimerFunc(unsigned int millis, void (GLUTCALLBACK* func)(int value), int value);
void glutMenuStateFunc(void (GLUTCALLBACK* func)(int state));
#if (GLUT_API_VERSION >= 2)
void glutSpecialFunc(void (GLUTCALLBACK* func)(int key, int x, int y));
void glutSpaceballMotionFunc(void (GLUTCALLBACK* func)(int x, int y, int z));
void glutSpaceballRotateFunc(void (GLUTCALLBACK* func)(int x, int y, int z));
void glutSpaceballButtonFunc(void (GLUTCALLBACK* func)(int button, int state));
void glutButtonBoxFunc(void (GLUTCALLBACK* func)(int button, int state));
void glutDialsFunc(void (GLUTCALLBACK* func)(int dial, int value));
void glutTabletMotionFunc(void (GLUTCALLBACK* func)(int x, int y));
void glutTabletButtonFunc(void (GLUTCALLBACK* func)(int button, int state, int x, int y));
#if (GLUT_API_VERSION >= 3)
void glutMenuStatusFunc(void (GLUTCALLBACK* func)(int status, int x, int y));
void glutOverlayDisplayFunc(void (GLUTCALLBACK* func)(void));
#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 9)
void glutWindowStatusFunc(void (GLUTCALLBACK* func)(int state));
#endif
#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 13)
void glutKeyboardUpFunc(void (GLUTCALLBACK* func)(unsigned char key, int x, int y));
void glutSpecialUpFunc(void (GLUTCALLBACK* func)(int key, int x, int y));
void glutJoystickFunc(void (GLUTCALLBACK* func)(unsigned int buttonMask, int x, int y, int z), int pollInterval);
#endif
#endif
#endif
/* GLUT color index sub-API. */
void glutSetColor(int, GLfloat red, GLfloat green, GLfloat blue);
GLfloat glutGetColor(int ndx, int component);
void glutCopyColormap(int win);
/* GLUT state retrieval sub-API. */
int glutGet(GLenum type);
int glutDeviceGet(GLenum type);
#if (GLUT_API_VERSION >= 2)
/* GLUT extension support sub-API */
int glutExtensionSupported(const char* name);
#endif
#if (GLUT_API_VERSION >= 3)
int glutGetModifiers(void);
int glutLayerGet(GLenum type);
#endif
/* GLUT font sub-API */
void glutBitmapCharacter(void* font, int character);
int glutBitmapWidth(void* font, int character);
void glutStrokeCharacter(void* font, int character);
int glutStrokeWidth(void* font, int character);
#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 9)
int glutBitmapLength(void* font, const unsigned char* string);
int glutStrokeLength(void* font, const unsigned char* string);
#endif
/* GLUT pre-built models sub-API */
void glutWireSphere(GLdouble radius, GLint slices, GLint stacks);
void glutSolidSphere(GLdouble radius, GLint slices, GLint stacks);
void glutWireCone(GLdouble base, GLdouble height, GLint slices, GLint stacks);
void glutSolidCone(GLdouble base, GLdouble height, GLint slices, GLint stacks);
void glutWireCube(GLdouble size);
void glutSolidCube(GLdouble size);
void glutWireTorus(GLdouble innerRadius, GLdouble outerRadius, GLint sides, GLint rings);
void glutSolidTorus(GLdouble innerRadius, GLdouble outerRadius, GLint sides, GLint rings);
void glutWireDodecahedron(void);
void glutSolidDodecahedron(void);
void glutWireTeapot(GLdouble size);
void glutSolidTeapot(GLdouble size);
void glutWireOctahedron(void);
void glutSolidOctahedron(void);
void glutWireTetrahedron(void);
void glutSolidTetrahedron(void);
void glutWireIcosahedron(void);
void glutSolidIcosahedron(void);
#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 9)
/* GLUT video resize sub-API. */
int glutVideoResizeGet(GLenum param);
void glutSetupVideoResizing(void);
void glutStopVideoResizing(void);
void glutVideoResize(int x, int y, int width, int height);
void glutVideoPan(int x, int y, int width, int height);
/* GLUT debugging sub-API. */
void glutReportErrors(void);
#endif
#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 13)
/* GLUT device control sub-API. */
/* glutSetKeyRepeat modes. */
#define GLUT_KEY_REPEAT_OFF 0
#define GLUT_KEY_REPEAT_ON 1
#define GLUT_KEY_REPEAT_DEFAULT 2
/* Joystick button masks. */
#define GLUT_JOYSTICK_BUTTON_A 1
#define GLUT_JOYSTICK_BUTTON_B 2
#define GLUT_JOYSTICK_BUTTON_C 4
#define GLUT_JOYSTICK_BUTTON_D 8
void glutIgnoreKeyRepeat(int ignore);
void glutSetKeyRepeat(int repeatMode);
void glutForceJoystickFunc(void);
/* GLUT game mode sub-API. */
/* glutGameModeGet. */
#define GLUT_GAME_MODE_ACTIVE  ((GLenum) 0)
#define GLUT_GAME_MODE_POSSIBLE  ((GLenum) 1)
#define GLUT_GAME_MODE_WIDTH  ((GLenum) 2)
#define GLUT_GAME_MODE_HEIGHT  ((GLenum) 3)
#define GLUT_GAME_MODE_PIXEL_DEPTH ((GLenum) 4)
#define GLUT_GAME_MODE_REFRESH_RATE ((GLenum) 5)
#define GLUT_GAME_MODE_DISPLAY_CHANGED ((GLenum) 6)
void glutGameModeString(const char* string);
int glutEnterGameMode(void);
void glutLeaveGameMode(void);
int glutGameModeGet(GLenum mode);
#endif

