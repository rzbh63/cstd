/*
 ���ļ������ڵ����ڷ����ڴ�й©����VC++������������
 (����Intel C++,��Ϊ��ʹ������ͬ�Ŀ�)
 MFC���ṩ��debug new��Ȼ������,������ʵ�ʲ��Ե�ʱ���ֶ��̲߳���
 ���õ�ʱ�����׳���ϵͳ����,���������������һ���̻߳�����.
 debug new��debug delete֮���費��Ҫ�����Ҳ���֪��,�������,��ͬ��
 �����̻߳�����.
 ����C++��׼�涨,��operator newʧ�ܺ�Ӧ������set_new_handler���õ�
 ����,����MSDN��ȴ˵"ͷ�ļ�new�е�set_new_handler��stub��,��Ӧ��ʹ
 ��ͷ�ļ�new.h�е�_set_new_handler",���ֱ�ǻ�����֮���.
 ������VC++6.0�е�set_new_handler����:
 new_handler __cdecl set_new_handler( new_handler new_p ) {
   assert( new_p == 0 ); // cannot use stub to register a new handler
   _set_new_handler( 0 );
   return 0;
 }
 ������Ҳ�޼ƿ�ʩ,ֻ������set_new_handler������.
//warning C4005: 'new' : macro redefinition
//#pragma warning(disable: 4005)
ʹ�÷���:
  ����������ڴ����
static void test_debug_new() {
  char* p = malloc(2);
  MEM_CHECK_BEGIN();
  p[0] = 'A';
  p[1] = 'B';
  MEM_CHECK_END();
  free(p);
}
MEM_CHECK_BEGIN();
MEM_CHECK_END();
*/
#ifndef _DEBUG_H_
#define _DEBUG_H_
#ifdef _DEBUG
#include <windows.h>
#include <malloc.h>
//#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h> // for vsprintf
#include <tchar.h>
//#include "tchar_x.h"
// �����ڴ�״̬�ṹ����
typedef _CrtMemState MemState[ 3 ];
static int _RegDebugNew_init = 0;
#define _RegDebugNew() (_CrtSetDbgFlag( _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF ), 1)
static void mem_recheck(MemState s)
{
  if (0 == _RegDebugNew_init) {
    _RegDebugNew_init = _RegDebugNew();
  }
  //memset(s, 0, 3*sizeof(_CrtMemState));
  // ��ȡ��һ���ڴ�״̬����
  _CrtMemCheckpoint(&s[ 0 ]);
}
static void mem_dump(MemState s)
{
  // ��ȡ�ڶ����ڴ�״̬����
  _CrtMemCheckpoint(&s[ 1 ]);
  // �Ƚ������ڴ���յĲ��죬 dump ������
  if (_CrtMemDifference(&s[ 2 ], &s[ 0 ], &s[ 1 ])) {
    _CrtMemDumpStatistics(&s[ 2 ]);
  }
  // ����ڴ�й©���棬��ʾ�� Output ���ڵ� Debug ҳ��
  _CrtDumpMemoryLeaks();
}
#ifndef _VT_BUF_LEN
#define _VT_BUF_LEN (4096)
#endif
static void _vtrace_dbg(const TCHAR* fmt, va_list argptr)
{
#if 0
  int i = 0, fmtlen, buflen;
  TCHAR* buf;
  TCHAR buf2[ _VT_BUF_LEN + 1 ];
  fmtlen = _tcslen(fmt);
  fmtlen *= 10;
  buf = (TCHAR*) malloc(fmtlen);
  memset(buf, 0, fmtlen);
  _vsntprintf(buf, fmtlen, fmt, argptr);
  buflen = _tcslen(buf);
  for (i = 0; i < buflen; i += _VT_BUF_LEN) {
    memcpy(buf2, buf + i, _VT_BUF_LEN * sizeof(TCHAR));
    buf2[ _VT_BUF_LEN ] = 0;
    OutputDebugString(buf2);
    if (buflen > _VT_BUF_LEN) {
      OutputDebugString(_T("\n"));
    }
  }
  free(buf);
#else
  TCHAR buf[ _VT_BUF_LEN ] = {0};
  _vsntprintf(buf, _VT_BUF_LEN, fmt, argptr);
  OutputDebugString(buf);
#endif
}
static int _trace_dbg(const TCHAR* fmt, ...)
{
  va_list argptr;
  va_start(argptr, fmt);
  _vtrace_dbg(fmt, argptr);
  va_end(argptr);
  return 0;
}
#define malloc(nSize) _malloc_dbg( nSize, _NORMAL_BLOCK, __FILE__, __LINE__ )
#define realloc(_p, nSize) _realloc_dbg( _p, nSize, _NORMAL_BLOCK, __FILE__, __LINE__ )
#define free(p)       _free_dbg( p, _NORMAL_BLOCK )
#define dprintf       _trace_dbg
#ifdef __cplusplus
inline void* operator new(size_t nSize, LPCSTR lpszFileName, int nLine, int pp)
{
  return _malloc_dbg(nSize, _NORMAL_BLOCK, lpszFileName, nLine);
}
inline void operator delete(void* pData, LPCSTR /* lpszFileName */, int /* nLine */, int pp)
{
  _free_dbg(pData, _NORMAL_BLOCK);
}
#define _DEBUG_NEW new(__FILE__, __LINE__, 0)
#define new _DEBUG_NEW
#define _DEBUG_DELETE delete(__FILE__, __LINE__, 0)
//#define delete _DEBUG_DELETE
#endif
#undef MEM_CHECK_BEGIN
#undef MEM_CHECK_END
#define MEM_CHECK_BEGIN  { MemState __mm; mem_recheck(__mm); {int __aux=0
#define MEM_CHECK_END    } mem_dump(__mm); }
#if 0
MEM_CHECK_BEGIN;
MEM_CHECK_END;
#endif
#ifdef _DEBUG
#define printf       _trace_dbg
#endif
#define __out_ecount_opt(x)
#include <dbghelp.h>
#include <stdio.h>
#pragma comment(lib,"dbghelp.lib")
static LPSTR GetSymbolSearchpath(LPSTR path)
{
  char   directory [MAX_PATH];
  char   drive [MAX_PATH];
  HMODULE module;
  memset(path, 0, MAX_PATH * sizeof(path[0]));
  // Oddly, the symbol handler ignores the link to the PDB embedded in the
  // executable image. So, we'll manually add the location of the executable
  // to the search path since that is often where the PDB will be located.
  path[0] = '\0';
  module = GetModuleHandle(NULL);
  GetModuleFileNameA(module, path, MAX_PATH);
  _splitpath(path, drive, directory, NULL, 0);
  strcpy(path, drive);
  strcat(path, directory);
  // Append the working directory.
  strcat(path, ";.\\");
  return path;
}
void uninit_sym(void)
{
  SymCleanup(GetCurrentProcess());
}
int init_sym(BOOL fInvadeProcess)
{
  char  symbolpath[MAX_PATH];
  GetSymbolSearchpath(symbolpath);
  SymInitialize(GetCurrentProcess(), symbolpath, fInvadeProcess);
  atexit(uninit_sym);
  return 0;
}
char* dbg_fuctionname(DWORD dwAddr)
{
  static char szFuctionName[32];
  BYTE  symbolBuffer[sizeof(SYMBOL_INFO) + 256 * sizeof(CHAR)] = { 0 };
  PSYMBOL_INFO pSymbol = (PSYMBOL_INFO) &symbolBuffer;
  static inited = 0;
  if (0 == inited) {
    inited = 1;
    init_sym(1);
  }
  memset(szFuctionName, 0, sizeof(szFuctionName));
  pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
  pSymbol->MaxNameLen = 256;
  if (SymFromAddr(GetCurrentProcess(), dwAddr, 0, pSymbol)) {
    strcpy(szFuctionName, pSymbol->Name);
  }
  else {
    printf("SymFromAddr Failed! %d\n", GetLastError());
  }
  return szFuctionName;
}
static DWORD GetEIP()
{
  DWORD dwCallerAddr;
  __asm {
    push DWORD  ptr[ebp+4]
    pop  DWORD  ptr[dwCallerAddr]
    sub  DWORD  ptr[dwCallerAddr], 5 //��ȥ call GetEIP() �ĳ���
  }
  return dwCallerAddr;
}
#define __func_for_vc6__   (dbg_fuctionname(GetEIP()))
#else // _DEBUG
/*
MEM_CHECK_BEGIN()
MEM_CHECK_END()
*/
#define MEM_CHECK_BEGIN()
#define MEM_CHECK_END()
#endif // _DEBUG
#endif // _CRTDBG_H_

