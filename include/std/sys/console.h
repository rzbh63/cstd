
#ifndef _CONSOLE_H_
#define _CONSOLE_H_
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include "macro.h"

#define CV_TS_NORMAL 0
#define CV_TS_BLUE   1
#define CV_TS_GREEN  2
#define CV_TS_RED    4

// �ı����̨������ɫ
void change_color(int color);

// ��ʽ����ӡ�������λ�ò���
int print_str(const char* fmt, ...)
{
  CONSOLE_SCREEN_BUFFER_INFO inf;
  HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
  GetConsoleScreenBufferInfo(hd, &inf);
  VPRINTF(fmt, arglist);
  SetConsoleCursorPosition(hd, inf.dwCursorPosition);
  return 0;
}
// ��ӡ������ rate - �ٷֱ� n - ����������
int print_pro(double rate, int n)
{
  char buf[ 256 ];
  CONSOLE_SCREEN_BUFFER_INFO inf;
  HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
  GetConsoleScreenBufferInfo(hd, &inf);

  if (rate < 0. || rate >= (double)(n - 1) / n) {
    memset(buf, ' ', 80);
    buf[ 80 ] = '\0';
    printf(buf);
  }
  else {
    int i = (int)(n * rate);
    printf("  ");
    buf[ 0 ] = '[';
    memset(buf + 1, '*', i);
    memset(buf + 1 + i, ' ', n - i);
    buf[ n ] = ']';
    buf[ n + 1 ] = '\0';
    printf(buf);
    i = (int)(100. * rate);
    printf("  [%3d%%]", i);
  }

  SetConsoleCursorPosition(hd, inf.dwCursorPosition);
  return 0;
}
#ifdef WIN32
void change_color(int color)
{
  static int normal_attributes = -1;
  HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
  fflush(stdout);

  if (normal_attributes < 0) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(hstdout, &info);
    normal_attributes = info.wAttributes;
  }

  SetConsoleTextAttribute(hstdout, (WORD)(color == CV_TS_NORMAL ? normal_attributes : ((color & CV_TS_BLUE ? FOREGROUND_BLUE : 0) | (color & CV_TS_GREEN ? FOREGROUND_GREEN : 0) | (color & CV_TS_RED ? FOREGROUND_RED : 0) | FOREGROUND_INTENSITY)));
}

#else
#include <signal.h>
void change_color(int color)
{

  static const unsigned char ansi_tab[] = {      //
    30, 34, 32, 36, 31, 35, 33, 37
  };
  char buf[ 16 ];
  int code = 0;
  fflush(stdout);

  if (color != CV_TS_NORMAL) {
    code = ansi_tab[ color & (CV_TS_BLUE | CV_TS_GREEN | CV_TS_RED) ];
  }

  sprintf(buf, "\x1b[%dm", code);
  printf(buf);
}

#endif

#ifndef PTTIME
static char _buffer[ 9 ];
#define STRDATE()  _strdate(_buffer)
#define STRTIME()  _strtime(_buffer)

#define PTTIME STRTIME()
#endif

// ����������ʾ���ļ���д
int fileread(void* ptr0, int size, int nitems, FILE* stream)
{
  int i, n;
  char* ptr = (char*)ptr0;

  for (i = 0, n = 0; i < nitems; ++i) {
    print_pro((double) i / nitems, 50);
    n += fread(ptr, size, 1, stream);
    ptr += size;
  }

  return n;
}
int filewrite(const void* ptr0, int size, int nitems, FILE* stream)
{
  int i, n;
  char* ptr = (char*)ptr0;

  for (i = 0, n = 0; i < nitems; ++i) {
    print_pro((double) i / nitems, 50);
    n += fwrite(ptr, size, 1, stream);
    ptr += size;
  }

  return n;
}
// ��־��ӡ pf - ��־�ļ� ���Ϊ����ֻ��ӡ�ڱ�׼��� stdout��
int print_log(FILE* pf, const char* fmt, ...)
{
  va_list ARGLIST;
  va_start(ARGLIST, fmt);

  if (pf) {
#ifdef PTTIME
    fprintf(pf, "%8s ", PTTIME);
#endif

    vfprintf(pf, fmt, ARGLIST);
    fflush(pf);
  }

#ifdef PTTIME
  //printf("%8s ", PTTIME);
#endif
  vprintf(fmt, ARGLIST);
  va_end(ARGLIST);
  return 0;
}

#define LOGPRINTF print_log

// ��Console������ı�


// width and height is the size of console window, if you specify fname,
// the output will also be writton to this file. The file pointer is automatically closed
// when you close the app
//void startConsoleWin(int width=80, int height=25, char* fname = NULL);
//����һ��Console���ڣ�ָ����Ⱥ͸߶ȣ����fname��Ϊ����ͬʱ�����д��һ���ļ���


#ifdef _DEBUG
#include <windows.h>
#include <stdio.h>
static FILE* __fStdOut = NULL;
static HANDLE __hStdOut = NULL;
#endif
void ExitConsoleWin(void)
{
#ifdef _DEBUG

  if (__fStdOut) {
    fclose(__fStdOut);
  }

  if (__hStdOut) {
    //fclose(__hStdOut);
  }

  FreeConsole();
#endif
}

void StartConsoleWin(int width, int height, char* fname)
{
#ifdef _DEBUG

  if (!__hStdOut) {
    AllocConsole(); //����
    SetConsoleTitle("Debug Window");
    __hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);    //ָ�����Ϊ��׼���HANDLE
    {
      COORD co = { width, height };
      SetConsoleScreenBufferSize(__hStdOut, co);    //ָ����������С
    }

    if (fname) {
      __fStdOut = fopen(fname, "w");
    }
  }

  atexit(ExitConsoleWin);
#endif
}

// Use wprintf like TRACE0, TRACE1, ... (The arguments are the same as printf)
//������printf�ĺ�������Consoleд���ı�
int cprintf(char* fmt, ...)
{
#ifdef _DEBUG
  char s[ 300 ];
  va_list argptr;
  int cnt;
  va_start(argptr, fmt);
  cnt = vsprintf(s, fmt, argptr);
  va_end(argptr);
  {
    DWORD cCharsWritten;

    if (__hStdOut) {         // дConsole
      WriteConsole(__hStdOut, s, strlen(s), &cCharsWritten, NULL);
    }
  }

  if (__fStdOut) {
    fprintf(__fStdOut, s);
  }

  return (cnt);
#else

  return 0;
#endif
}


#endif // _CONSOLE_H_
