
#ifndef _FILEIO_C_H_
#define _FILEIO_C_H_

#include "mem_c.h"

#ifdef _WIN32
#include <io.h>
#include <direct.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif // 


/*
For Windows
value	mode
00		Existence only
02		Write-only
04		Read-only
06		Read and write

For linux
F_OK	Existence only
R_OK	Read-only
W_OK	Write-only
X_OK	Executable
*/
static int CheckFileExist(const char* szFile)
{
#ifdef _WIN32
  return _access(szFile, 0) != -1;
#else
  return access(szFile, F_OK) != -1;
#endif
}

static long fsize(FILE* stream)
{
  long pos, size;
  pos = ftell(stream);
  fseek(stream, 0, SEEK_END);
  size = ftell(stream);
  fseek(stream, pos, SEEK_SET);
  return size;
}
static int64_t fsize64(FILE* stream)
{
  int64_t pos, size;
  pos = _ftelli64(stream);
  _fseeki64(stream, 0, SEEK_END);
  size = _ftelli64(stream);
  _fseeki64(stream, pos, SEEK_SET);
  return size;
}
static int buf_load(const char* fname, buf_t* bf)
{
  int len, readed_len;
  FILE* pf;
  pf = fname ? fopen(fname, "rb") : NULL;
  if (pf) {
    len = fsize(pf);
    bfsetsize(bf, len);
    readed_len = fread(bf->data, 1, len, pf);
    assert(readed_len == len);
    fclose(pf);
    return 1;
  }
  return 0;
}

CC_INLINE int savefile(const char* fname, const void* buf, int buflen)
{
  FILE* pf = fopen(fname, "wb");
  int n = 0;
  if (NULL == pf) {
    return 0;
  }
  n = fwrite(buf, 1, buflen, pf);
  fclose(pf);
  return n;
}
CC_INLINE int loadfile(const char* fname, void* buf, int buflen, int pos)
{
  FILE* pf = fopen(fname, "rb");
  int n = 0;
  if (NULL == pf) {
    return 0;
  }
  if (pos > 0) {
    fseek(pf, pos, SEEK_SET);
  }
  n = fread(buf, 1, buflen, pf);
  fclose(pf);
  return n;
}
static char* loaddata(const char* fname, int* pLen)
{
  int len, readed_len;
  FILE* pf;
  char* str;
  if (!fname || !fname[0]) {
    return 0;
  }
  pf = fopen(fname, "rb");
  if (NULL == pf) {
    return 0;
  }
  len = fsize(pf);
  str = (char*)malloc(len + 1);
  memset(str, 0, len + 1);
  readed_len = fread(str, 1, len, pf);
  assert(readed_len == len);
  fclose(pf);
  if (pLen) {
    *pLen = len;
  }
  return str;
}
//////////////////////////////////////////////////////

CC_INLINE int savedata2txt_impl(FILE* pf, int h, int w, const void* A, int al,
	const char* szBegL, const char* szFmt, const char* szEndL)
{
	int i, j;
	const uchar* A0 = (const uchar*)A;

	for (i = 0; i < h; ++i, A0 += al) {
		fprintf(pf, "%s", szBegL);

		for (j = 0; j < w; ++j) {
			fprintf(pf, szFmt, A0[j]);
		}

		fprintf(pf, "%s", szEndL);
	}

	return 0;
}
//"%02x" "\n"
CC_INLINE int savedata2txt(const char* fn, int h, int w, const void* A, int al, const char* szBegL, const char* szFmt, const char* szEndL)
{
	FILE* pf;
	pf = fopen(fn, "wb");

	if (NULL == pf) {
		return 0;
	}

	savedata2txt_impl(pf, h, w, A, al, szBegL, szFmt, szEndL);
	fclose(pf);
	return 0;
}

#endif // _FILEIO_C_H_
