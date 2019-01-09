
#ifndef _DIBAPI_INL_
#define _DIBAPI_INL_

//  DIB(Independent Bitmap)API�����⣺��������й�DIB�Ĳ�������
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include "cfile.h"

#include "img/bitmap.inl"
// �����Ƕȵ����ȵ�ת��
//#define AngleToRadian(angle)  ((angle)*PI/180.0)
#undef  FWRITE
#undef  FREAD
#define FWRITE(file, _PTR, _N)     fwrite( _PTR, _N, 1, file)
#define FWRITEHUGE(file, _PTR, _N) fwrite( _PTR, _N, 1, file)
#define FREAD(file, _PTR, _N)      fread( _PTR, _N, 1, file)
#define FREADHUGE(file, _PTR, _N)  fread( _PTR, _N, 1, file)
// GetLength(file
//DWORD GetLength(FILE* file);
//#define GetPosition(file)  ftell(file)
//DECLARE_HANDLE(HDIB);
#define PALVERSION   0x300
// �ж��Ƿ���Win 3.0��DIB
#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))

// �ڼ���ͼ���Сʱ�����ù�ʽ��biSizeImage = biWidth' �� biHeight��
// ��biWidth'��������biWidth�������biWidth'������4������������ʾ
// ���ڻ����biWidth�ģ���4�������������WIDTHBYTES������������
// biWidth'
#define WIDTHBYTES(bits)    (((bits) + 31) / 32* 4)
// Dib�ļ�ͷ��־���ַ���"BM"��дDIBʱ�õ��ó�����
#define DIB_HEADER_MARKER   ((WORD)('M' << 8) | 'B')
// PCX�ļ�ͷ�ṹ
typedef struct PCXHEADER {
  BYTE bManufacturer;
  BYTE bVersion;
  BYTE bEncoding;
  BYTE bBpp;
  WORD wLeft;
  WORD wTop;
  WORD wRight;
  WORD wBottom;
  WORD wXResolution;
  WORD wYResolution;
  BYTE bPalette[ 48 ];
  BYTE bReserved;
  BYTE bPlanes;
  WORD wLineBytes;
  WORD wPaletteType;
  WORD wSrcWidth;
  WORD wSrcDepth;
  BYTE bFiller[ 54 ];
}
PCXHEADER;
// ��ָ����α��ɫ��������滻ָ����DIBͼ�����ɫ��
// ����:
//   LPRGBQUAD pDibQuad     - ָ��DIB����ɫ��
//   LPBYTE pColorsTable    - α��ɫ�����
//   int nColorEntries      - DIB����ɫ���еı�����
// ����ֵ:
//   BOOL            - �ɹ�����TRUE�����򷵻�FALSE
static BOOL ReplaceDIBColorTable(LPRGBQUAD pDibQuad, int nColorEntries, LPBYTE pColorsTable)
{
  int i; // ѭ������

  // ��ȡα��ɫ���룬����DIB��ɫ��
  for (i = 0; i < nColorEntries; i++) {
    // ����DIB��ɫ���ɫ����
    pDibQuad->rgbRed = pColorsTable[ i * 4 + 0 ];
    // ����DIB��ɫ����ɫ����
    pDibQuad->rgbGreen = pColorsTable[ i * 4 + 1 ];
    // ����DIB��ɫ����ɫ����
    pDibQuad->rgbBlue = pColorsTable[ i * 4 + 2 ];
    // ����DIB��ɫ�屣��λ
    pDibQuad->rgbReserved = 0;
    pDibQuad++;
  }

  // ����
  return TRUE;
}
// ����DIB�е�ɫ�����ɫ������
//   ���ڵ�ɫλͼ������2��
//   ����16ɫλͼ������16��
//   ����256ɫλͼ������256��
//   �������ɫλͼ��24λ����û�е�ɫ�壬����0��
// ����:
//   LPBYTE lpbi         - ָ��DIB�����ָ��
// ����ֵ:
//   WORD               - ���ص�ɫ������ɫ������
static WORD DIBNumColors(LPBYTE lpbi)
{
  WORD wBitCount;

  // ����Windows��DIB, ʵ����ɫ����Ŀ���Ա����ص�λ��Ҫ�١�
  // ��������������򷵻�һ�����Ƶ���ֵ��
  // �ж��Ƿ���WIN3.0 DIB
  if (IS_WIN30_DIB(lpbi)) {
    DWORD dwClrUsed;
    dwClrUsed = ((LPBITMAPINFOHEADER) lpbi) ->biClrUsed;     // ��ȡdwClrUsedֵ

    if (dwClrUsed != 0) {
      // ���dwClrUsed��ʵ���õ�����ɫ������Ϊ0��ֱ�ӷ��ظ�ֵ
      return (WORD) dwClrUsed;
    }
  }

  // ��ȡ���ص�λ��
  if (IS_WIN30_DIB(lpbi)) {
    wBitCount = ((LPBITMAPINFOHEADER) lpbi) ->biBitCount;     // ��ȡbiBitCountֵ
  }
  else {
    wBitCount = ((LPBITMAPCOREHEADER) lpbi) ->bcBitCount;     // ��ȡbiBitCountֵ
  }

  // �������ص�λ��������ɫ��Ŀ
  switch (wBitCount) {
  case 1:
    return 2;
    break;

  case 4:
    return 16;
    break;

  case 8:
    return 256;
    break;

  default:
    return 0;
    break;
  }
}
// ����DIB��ɫ��Ĵ�С
//   ����Windows 3.0 DIB��������ɫ��Ŀ��RGBQUAD�Ĵ�С��
//   ��������������ɫ��Ŀ��RGBTRIPLE�Ĵ�С��
// �������ƣ�
//   PaletteSize()
// ����:
//   LPBYTE lpbi         - ָ��DIB�����ָ��
// ����ֵ:
//   WORD               - DIB�е�ɫ��Ĵ�С
static WORD PaletteSize(LPBYTE lpbi)
{
  // ����DIB�е�ɫ��Ĵ�С
  if (IS_WIN30_DIB(lpbi)) {
    //������ɫ��Ŀ��RGBQUAD�Ĵ�С
    return (WORD)(DIBNumColors(lpbi) * sizeof(RGBQUAD));
  }
  else {
    //������ɫ��Ŀ��RGBTRIPLE�Ĵ�С
    return (WORD)(DIBNumColors(lpbi) * sizeof(RGBTRIPLE));
  }
}
// ����DIBͼ��������ʼλ��
//   ����DIB��ͼ�����ص���ʼλ�ã�������ָ������ָ�롣
//   LPBYTE lpbi  - ָ��DIB�����ָ��
// ����ֵ:
//   LPBYTE - ָ��DIBͼ��������ʼλ��
static LPBYTE DIBBits(LPBYTE lpbi)
{
  return (lpbi + * (LPDWORD) lpbi + PaletteSize(lpbi));
}
// ����DIBͼ���� ����Windows 3.0 DIB������BITMAPINFOHEADER�е�biWidthֵ��������������BITMAPCOREHEADER�е�bcWidthֵ��
// ����:
//   LPBYTE lpbi         - ָ��DIB�����ָ��
// ����ֵ:
//   DWORD              - DIB��ͼ��Ŀ��
#define DIBWidth(lpDIB)   (IS_WIN30_DIB(lpDIB) ? ((LPBITMAPINFOHEADER) lpDIB)->biWidth : ((LPBITMAPCOREHEADER) lpDIB)->bcWidth)

// ����DIBͼ��߶� ����Windows 3.0 DIB������BITMAPINFOHEADER�е�biHeightֵ�� ������������BITMAPCOREHEADER�е�bcHeightֵ��
// ����:
//   LPBYTE lpDIB        - ָ��DIB�����ָ��
// ����ֵ:
//   DWORD              - DIB��ͼ��ĸ߶�
#define DIBHeight(lpDIB)   (IS_WIN30_DIB(lpDIB) ? ((LPBITMAPINFOHEADER) lpDIB)->biHeight : ((LPBITMAPCOREHEADER) lpDIB)->bcHeight)

// ����DIB�����ɫ��
// ����DIB����һ���߼���ɫ�壬��DIB�ж�ȡ��ɫ���浽��ɫ���У�
// ����ո��߼���ɫ�崴��һ���µĵ�ɫ�壬�����ظõ�ɫ��ľ��������
// ��������õ���ɫ����ʾDIBͼ��
//   LPBYTE lpbi          - ָ��DIB��ָ��
//   HPALETTE pPal     - ָ��DIB�����ɫ���ָ��
// ����ֵ:
//   BOOL               - �����ɹ�����TRUE�����򷵻�FALSE��
static BOOL DIBCreatePalette(LPBYTE lpbi, HPALETTE pPal)
{
  LPLOGPALETTE lpPal;  // ָ���߼���ɫ���ָ��
  HANDLE hLogPal;   // �߼���ɫ��ľ��
  HPALETTE hPal = NULL; // ��ɫ��ľ��
  int i;     // ѭ������
  WORD wNumColors;  // ��ɫ���е���ɫ��Ŀ
  LPBITMAPINFO lpbmi;  // ָ��BITMAPINFO�ṹ��ָ�루Win3.0��
  LPBITMAPCOREINFO lpbmc; // ָ��BITMAPCOREINFO�ṹ��ָ��
  BOOL bWinStyleDIB;  // �����Ƿ���Win3.0 DIB�ı��
  BOOL bResult = FALSE; // �������

  if (lpbi == NULL) {
    return FALSE;
  }

  lpbmi = (LPBITMAPINFO) lpbi;    // ��ȡָ��BITMAPINFO�ṹ��ָ�루Win3.0��
  lpbmc = (LPBITMAPCOREINFO) lpbi;   // ��ȡָ��BITMAPCOREINFO�ṹ��ָ��
  wNumColors = DIBNumColors(lpbi);   // ��ȡDIB����ɫ���е���ɫ��Ŀ

  if (wNumColors != 0) {
    // ����Ϊ�߼���ɫ���ڴ�
    hLogPal = GMALLOC(BYTE, sizeof(LOGPALETTE)
        + sizeof(PALETTEENTRY)
        * wNumColors);

    // ����ڴ治�㣬�˳�
    if (hLogPal == 0) {
      return FALSE;
    }

    lpPal = (LPLOGPALETTE)(LPBYTE) GlobalLock((HGLOBAL) hLogPal);
    lpPal->palVersion = PALVERSION;   // ���ð汾��
    lpPal->palNumEntries = (WORD) wNumColors;   // ������ɫ��Ŀ
    bWinStyleDIB = IS_WIN30_DIB(lpbi);    // �ж��Ƿ���WIN3.0��DIB

    // ��ȡ��ɫ��
    for (i = 0; i < (int) wNumColors; i++) {
      if (bWinStyleDIB) {
        // ��ȡ��ɫ��ɫ��ɫ����
        lpPal->palPalEntry[ i ].peRed = lpbmi->bmiColors[ i ].rgbRed;
        lpPal->palPalEntry[ i ].peGreen = lpbmi->bmiColors[ i ].rgbGreen;
        lpPal->palPalEntry[ i ].peBlue = lpbmi->bmiColors[ i ].rgbBlue;
        // ����λ
        lpPal->palPalEntry[ i ].peFlags = 0;
      }
      else {
        // ��ȡ��ɫ��ɫ��ɫ����
        lpPal->palPalEntry[ i ].peRed = lpbmc->bmciColors[ i ].rgbtRed;
        lpPal->palPalEntry[ i ].peGreen = lpbmc->bmciColors[ i ].rgbtGreen;
        lpPal->palPalEntry[ i ].peBlue = lpbmc->bmciColors[ i ].rgbtBlue;
        // ����λ
        lpPal->palPalEntry[ i ].peFlags = 0;
      }
    }

    pPal = CreatePalette(lpPal);   // �����߼���ɫ�崴����ɫ�壬������ָ��
    bResult = pPal != 0;
    GlobalUnlock((HGLOBAL) hLogPal);     // �������
    GlobalFree((HGLOBAL) hLogPal);     // �ͷ��߼���ɫ��
  }

  return bResult;
}
//   ��ָ����α��ɫ��������滻ͼ��ĵ��԰壬����bpColorsTable
// ָ��Ҫ�滻��α��ɫ�����
// ����:
//   LPBYTE lpDIB   - ָ��ԴDIBͼ��ָ��
//   LPBYTE bpColorsTable - α��ɫ�����
// ����ֵ:
//   BOOL     - �ɹ�����TRUE�����򷵻�FALSE��
static BOOL ReplaceColorPal(LPBYTE lpDIB, LPBYTE bpColorsTable)
{
  int i;     // ѭ������
  WORD wNumColors;  // ��ɫ���е���ɫ��Ŀ
  LPBITMAPINFO lpbmi;  // ָ��BITMAPINFO�ṹ��ָ�루Win3.0��
  LPBITMAPCOREINFO lpbmc; // ָ��BITMAPCOREINFO�ṹ��ָ��
  BOOL bWinStyleDIB;  // �����Ƿ���Win3.0 DIB�ı��
  BOOL bResult = FALSE; // �������
  lpbmi = (LPBITMAPINFO) lpDIB;    // ��ȡָ��BITMAPINFO�ṹ��ָ�루Win3.0��
  lpbmc = (LPBITMAPCOREINFO) lpDIB;   // ��ȡָ��BITMAPCOREINFO�ṹ��ָ��
  wNumColors = DIBNumColors(lpDIB);   // ��ȡDIB����ɫ���е���ɫ��Ŀ

  if (wNumColors == 256) { // �ж���ɫ��Ŀ�Ƿ���256ɫ
    bWinStyleDIB = IS_WIN30_DIB(lpDIB);   // �ж��Ƿ���WIN3.0��DIB

    // ��ȡα��ɫ���룬����DIB��ɫ��
    for (i = 0; i < (int) wNumColors; i++) {
      if (bWinStyleDIB) {
        // ����DIB��ɫ���ɫ��ɫ��ɫ����
        lpbmi->bmiColors[ i ].rgbRed = bpColorsTable[ i * 4 ];
        lpbmi->bmiColors[ i ].rgbGreen = bpColorsTable[ i * 4 + 1 ];
        lpbmi->bmiColors[ i ].rgbBlue = bpColorsTable[ i * 4 + 2 ];
        // ����DIB��ɫ�屣��λ
        lpbmi->bmiColors[ i ].rgbReserved = 0;
      }
      else {
        // ����DIB��ɫ���ɫ��ɫ��ɫ����
        lpbmc->bmciColors[ i ].rgbtRed = bpColorsTable[ i * 4 ];
        lpbmc->bmciColors[ i ].rgbtGreen = bpColorsTable[ i * 4 + 1 ];
        lpbmc->bmciColors[ i ].rgbtBlue = bpColorsTable[ i * 4 + 2 ];
      }
    }
  }

  return bResult;
}
// �����ڴ��
//   ����ָ�����ڴ����򡣷��ظ��ƺ�����ڴ����򣬳���ʱ����0��
// ����:
//   HGLOBAL h          - Ҫ���Ƶ��ڴ�����
// ����ֵ:
//   HGLOBAL            - ���ƺ�����ڴ�����
static HGLOBAL CopyHandle(HGLOBAL h)
{
  if (h != NULL) {
    DWORD dwLen = GlobalSize((HGLOBAL) h);     // ��ȡָ���ڴ������С
    HGLOBAL hCopy = MALLOC(BYTE, dwLen);   // �������ڴ�ռ�

    if (hCopy != NULL) {   // �жϷ����Ƿ�ɹ�
      void* lpCopy = (LPBYTE) GlobalLock((HGLOBAL) hCopy);
      void* lp = (LPBYTE) GlobalLock((HGLOBAL) h);
      memcpy(lpCopy, lp, dwLen);
      GlobalUnlock(hCopy);
      GlobalUnlock(h);
    }

    return hCopy;
  }

  return NULL;
}
// ��DIB���浽ָ���ļ���
// ��ָ����DIB���󱣴浽ָ����CFile�С���CFile�ɵ��ó���򿪺͹رա�
//   SaveDIB()
// ����:
//   LPBYTE pDib          - Ҫ�����DIB
//   FILE* file        - �����ļ�CFile
// ����ֵ:
//   BOOL               - �ɹ�����TRUE�����򷵻�FALSE����CFileException
static BOOL SaveDIB(LPBYTE pDib, FILE* file)
{
  BITMAPFILEHEADER bmfHdr; // Bitmap�ļ�ͷ
  LPBITMAPINFOHEADER lpBI; // ָ��BITMAPINFOHEADER��ָ��
  DWORD dwDIBSize;   // DIB��С

  if (pDib == NULL) {
    return FALSE;
  }

  // ��ȡBITMAPINFO�ṹ��������
  lpBI = (LPBITMAPINFOHEADER)pDib;

  if (lpBI == NULL) {
    return FALSE;
  }

  // �ж��Ƿ���WIN3.0 DIB
  if (!IS_WIN30_DIB(lpBI)) {
    // ��֧���������͵�DIB����
    return FALSE;
  }

  // ����ļ�ͷ
  bmfHdr.bfType = DIB_HEADER_MARKER;  // �ļ�����"BM"
  // ����DIB��Сʱ����򵥵ķ����ǵ���GlobalSize()����������ȫ���ڴ��С��
  // ����DIB�����Ĵ�С�������Ƕ༸���ֽڡ���������Ҫ����һ��DIB����ʵ��С��
  // �ļ�ͷ��С����ɫ���С
  // ��BITMAPINFOHEADER��BITMAPCOREHEADER�ṹ�ĵ�һ��DWORD���Ǹýṹ�Ĵ�С��
  dwDIBSize = *(LPDWORD) lpBI + PaletteSize((LPBYTE) lpBI);

  // ����ͼ���С
  if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4)) {
    // ����RLEλͼ��û�������С��ֻ������biSizeImage�ڵ�ֵ
    dwDIBSize += lpBI->biSizeImage;
  }
  else {
    DWORD dwBmBitsSize;   // ���صĴ�С
    dwBmBitsSize = WIDTHBYTES((lpBI->biWidth) * ((DWORD) lpBI->biBitCount))
        * lpBI->biHeight;  // ��СΪWidth* Height
    dwDIBSize += dwBmBitsSize; // �����DIB�����Ĵ�С
    // ����biSizeImage���ܶ�BMP�ļ�ͷ��biSizeImage��ֵ�Ǵ���ģ�
    lpBI->biSizeImage = dwBmBitsSize;
  }

  // �����ļ���С��DIB��С��BITMAPFILEHEADER�ṹ��С
  bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
  // ����������
  bmfHdr.bfReserved1 = 0;
  bmfHdr.bfReserved2 = 0;
  // ����ƫ����bfOffBits�����Ĵ�СΪBitmap�ļ�ͷ��С��DIBͷ��С����ɫ���С
  bmfHdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + lpBI->biSize
      + PaletteSize((LPBYTE) lpBI);
  // ����д�ļ�
  FWRITE(file, (LPBYTE) & bmfHdr, sizeof(BITMAPFILEHEADER));       // д�ļ�ͷ
  FWRITEHUGE(file, lpBI, dwDIBSize);       // дDIBͷ������
  return TRUE;
}
// ��ָ���ļ��ж�ȡDIB����
//   ��ָ�����ļ��е�DIB�������ָ�����ڴ������С���BITMAPFILEHEADER
// ������ݶ����������ڴ档
// ����:
//   FILE* file        - Ҫ��ȡ���ļ��ļ�CFile
// ����ֵ:
//   LPBYTE               - �ɹ�����DIB�����򷵻�NULL��
static LPBYTE ReadDIBFile(FILE* file)
{
  BITMAPFILEHEADER bmfHeader;
  LPBYTE pDIB;
  DWORD dwBitsSize;
  dwBitsSize = (int)fsize(file);    // ��ȡDIB���ļ������ȣ��ֽڣ�

  // ���Զ�ȡDIB�ļ�ͷ
  if (FREAD(file, (LPBYTE) & bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader)) {
    return NULL;
  }

  // �ж��Ƿ���DIB���󣬼��ͷ�����ֽ��Ƿ���"BM"
  if (bmfHeader.bfType != DIB_HEADER_MARKER) {
    return NULL;
  }

  // ΪDIB�����ڴ�
  pDIB = MALLOC(BYTE, dwBitsSize);

  if (pDIB == 0) {
    return NULL;
  }

  if (FREADHUGE(file, pDIB, dwBitsSize - sizeof(BITMAPFILEHEADER)) !=
      dwBitsSize - sizeof(BITMAPFILEHEADER)) {  // ������
    free(pDIB);
    return NULL;
  }

  return pDIB;
}
// ����DIBBitCount��
// ����:
//   LPBYTE lpbi         - ָ��DIB�����ָ��
// ����ֵ:
//   WORD               - ���ص�ɫ������ɫ������
static WORD DIBBitCount(LPBYTE lpbi)
{
  WORD wBitCount;

  // ��ȡ���ص�λ��
  if (IS_WIN30_DIB(lpbi)) {
    // ��ȡbiBitCountֵ
    wBitCount = ((LPBITMAPINFOHEADER) lpbi) ->biBitCount;
  }
  else {
    // ��ȡbiBitCountֵ
    wBitCount = ((LPBITMAPCOREHEADER) lpbi) ->bcBitCount;
  }

  // ����wBitCount
  return wBitCount;
}
// ��ָ����256ɫDIB���󱣴�Ϊ256ɫPCX�ļ�
// ����:
//   LPBYTE lpDIB        - ָ��DIB�����ָ��
//   FILE* file        - Ҫ������ļ�
// ����ֵ:
//   BOOL               - �ɹ�����TRUE�����򷵻�False��
static BOOL DIBToPCX256(LPBYTE lpDIB, FILE* file)
{
  // ѭ������
  int i;
  int j;
  // DIB�߶�
  WORD wHeight;
  // DIB���
  WORD wWidth;
  // �м����
  BYTE bChar1;
  BYTE bChar2;
  // ָ��Դͼ�����ص�ָ��
  LPBYTE lpSrc;
  // ָ������ͼ�����ݵ�ָ��
  LPBYTE lpDst;
  // ͼ��ÿ�е��ֽ���
  int lLineBytes;
  // �ظ����ؼ���
  int iCount;
  // ��������ʹ�õ��ֽ���
  DWORD dwBuffUsed;
  // ָ��DIB����ָ��
  LPBYTE lpDIBBits;
  // PCX�ļ�ͷ
  PCXHEADER pcxHdr;
  // ָ��BITMAPINFO�ṹ��ָ�루Win3.0��
  LPBITMAPINFO lpbmi;
  // ָ��BITMAPCOREINFO�ṹ��ָ��
  LPBITMAPCOREINFO lpbmc;
  // �����Ƿ���Win3.0 DIB�ı��
  BOOL bWinStyleDIB;
  // ��ȡDIB�߶�
  wHeight = (WORD) DIBHeight(lpDIB);
  // ��ȡDIB���
  wWidth = (WORD) DIBWidth(lpDIB);
  // �ҵ�DIBͼ��������ʼλ��
  lpDIBBits = DIBBits(lpDIB);
  // ����ͼ��ÿ�е��ֽ���
  lLineBytes = WIDTHBYTES(wWidth * 8);
  // ���ļ�ͷ��ֵ
  // PCX��ʶ��
  pcxHdr.bManufacturer = 0x0A;
  // PCX�汾��
  pcxHdr.bVersion = 5;
  // PCX���뷽ʽ��1��ʾRLE���룩
  pcxHdr.bEncoding = 1;
  // ����λ����256ɫΪ8λ��
  pcxHdr.bBpp = 8;
  // ͼ���������Ļ�����Ͻ�X���꣨������Ϊ��λ��
  pcxHdr.wLeft = 0;
  // ͼ���������Ļ�����Ͻ�Y���꣨������Ϊ��λ��
  pcxHdr.wTop = 0;
  // ͼ���������Ļ�����½�X���꣨������Ϊ��λ��
  pcxHdr.wRight = wWidth - 1;
  // ͼ���������Ļ�����½�Y���꣨������Ϊ��λ��
  pcxHdr.wBottom = wHeight - 1;
  // ͼ���ˮƽ�ֱ���
  pcxHdr.wXResolution = wWidth;
  // ͼ��Ĵ�ֱ�ֱ���
  pcxHdr.wYResolution = wHeight;

  // ��ɫ�����ݣ�����256ɫPCX�����壬ֱ�Ӹ�ֵΪ0��
  for (i = 0; i < 48; i ++) {
    pcxHdr.bPalette[ i ] = 0;
  }

  // �������趨Ϊ0��
  pcxHdr.bReserved = 0;
  // ͼ��ɫ��ƽ����Ŀ������256ɫPCX�趨Ϊ1����
  pcxHdr.bPlanes = 1;
  // ͼ��Ŀ�ȣ��ֽ�Ϊ��λ��������Ϊż����
  // if ((wWidth & 1)== 0)
  // {
  pcxHdr.wLineBytes = wWidth;
  // }
  // else
  // {
  //  pcxHdr.wLineBytes = wWidth + 1;
  // }
  // ͼ���ɫ������ͣ�1��ʾ��ɫ���ߵ�ɫͼ��2��ʾͼ���ǻҶ�ͼ��
  pcxHdr.wPaletteType = 1;
  // ������ͼ�����Ļ��ȣ�����Ϊ��λ��
  pcxHdr.wSrcWidth = 0;
  // ������ͼ�����Ļ�߶ȣ�����Ϊ��λ��
  pcxHdr.wSrcDepth = 0;

  // ������ȡֵ�趨Ϊ0��
  for (i = 0; i < 54; i ++) {
    pcxHdr.bFiller[ i ] = 0;
  }

  // д���ļ�ͷ
  FWRITE(file, (LPBYTE) & pcxHdr, sizeof(PCXHEADER));
  // ��ʼ����
  // ����һƬ������(2��ԭʼͼ���С)�Ա��������
  lpDst = MALLOC(BYTE, wHeight * wWidth * 2);
  // ָ����ǰ�Ѿ����˶��ٻ��������ֽ�����
  dwBuffUsed = 0;

  // ÿ��
  for (i = 0; i < wHeight; i++) {
    // ָ��DIB��i�У���0�����ص�ָ��
    lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (wHeight - 1 - i);
    // ��bChar1��ֵ
    bChar1 = *lpSrc;
    // ����iCountΪ1
    iCount = 1;

    // ʣ����
    for (j = 1; j < wWidth; j ++) {
      // ָ��DIB��i�У���j�����ص�ָ��
      lpSrc++;
      // ��ȡ��һ������
      bChar2 = *lpSrc;

      // �ж��Ƿ��bChar1��ͬ����iCount < 63
      if ((bChar1 == bChar2) && (iCount < 63)) {
        // ��ͬ��������1
        iCount ++;
        // ��������һ��
      }
      else {
        // ��ͬ������iCount = 63
        // д�뻺����
        if ((iCount > 1) || (bChar1 >= 0xC0)) {
          // �����볤��Ϣ
          lpDst[ dwBuffUsed ] = iCount | 0xC0;
          // ����bChar1
          lpDst[ dwBuffUsed + 1 ] = bChar1;
          // ����dwBuffUsed
          dwBuffUsed += 2;
        }
        else {
          // ֱ�ӱ����ֵ
          lpDst[ dwBuffUsed ] = bChar1;
          // ����dwBuffUsed
          dwBuffUsed ++;
        }

        // ���¸�bChar1��ֵ
        bChar1 = bChar2;
        // ����iCountΪ1
        iCount = 1;
      }
    }

    // ����ÿ�����һ���ֱ���
    if ((iCount > 1) || (bChar1 >= 0xC0)) {
      // �����볤��Ϣ
      lpDst[ dwBuffUsed ] = iCount | 0xC0;
      // ����bChar1
      lpDst[ dwBuffUsed + 1 ] = bChar1;
      // ����dwBuffUsed
      dwBuffUsed += 2;
    }
    else {
      // ֱ�ӱ����ֵ
      lpDst[ dwBuffUsed ] = bChar1;
      // ����dwBuffUsed
      dwBuffUsed ++;
    }
  }

  // д�������
  FWRITEHUGE(file, (LPBYTE) lpDst, dwBuffUsed);
  // �ͷ��ڴ�
  FREE(lpDst);
  // д���ɫ����Ϣ
  // ����һƬ�������Ա����ɫ��
  lpDst = MALLOC(BYTE, 769);
  // ��ɫ����ʼ�ֽ�
  * lpDst = 0x0C;
  // ��ȡָ��BITMAPINFO�ṹ��ָ�루Win3.0��
  lpbmi = (LPBITMAPINFO) lpDIB;
  // ��ȡָ��BITMAPCOREINFO�ṹ��ָ��
  lpbmc = (LPBITMAPCOREINFO) lpDIB;
  // �ж��Ƿ���WIN3.0��DIB
  bWinStyleDIB = IS_WIN30_DIB(lpDIB);

  // ��ȡ��ǰDIB��ɫ��
  for (i = 0; i < 256; i ++) {
    if (bWinStyleDIB) {
      // ��ȡDIB��ɫ���ɫ����
      lpDst[ i * 3 + 1 ] = lpbmi->bmiColors[ i ].rgbRed;
      // ��ȡDIB��ɫ����ɫ����
      lpDst[ i * 3 + 2 ] = lpbmi->bmiColors[ i ].rgbGreen;
      // ��ȡDIB��ɫ����ɫ����
      lpDst[ i * 3 + 3 ] = lpbmi->bmiColors[ i ].rgbBlue;
    }
    else {
      // ��ȡDIB��ɫ���ɫ����
      lpDst[ i * 3 + 1 ] = lpbmc->bmciColors[ i ].rgbtRed;
      // ��ȡDIB��ɫ����ɫ����
      lpDst[ i * 3 + 2 ] = lpbmc->bmciColors[ i ].rgbtGreen;
      // ��ȡDIB��ɫ����ɫ����
      lpDst[ i * 3 + 3 ] = lpbmc->bmciColors[ i ].rgbtBlue;
    }
  }

  // д���ɫ����Ϣ
  FWRITE(file, (LPBYTE) lpDst, 769);
  // ����
  return TRUE;
}
// ��ȡ256ɫPCX�ļ�
//   ����ȡָ����256ɫPCX�ļ�������ȡ�Ľ��������һ��δѹ��
// �����DIB�����С�
// ����:
//   FILE* file        - Ҫ��ȡ���ļ�
// ����ֵ:
//   LPBYTE               - �ɹ�����DIB�ģ����򷵻�NULL��
static LPBYTE ReadPCX256(FILE* file)
{
  // PCX�ļ�ͷ
  PCXHEADER pcxHdr;
  // DIB��С���ֽ�����
  DWORD dwDIBSize;
  // DIBָ��
  LPBYTE pDIB;
  // ѭ������
  int i;
  int j;
  // �ظ����ؼ���
  int iCount;
  // DIB�߶�
  WORD wHeight;
  // DIB���
  WORD wWidth;
  // ͼ��ÿ�е��ֽ���
  int lLineBytes;
  // �м����
  BYTE bChar;
  // ָ��Դͼ�����ص�ָ��
  LPBYTE lpSrc;
  // ָ������ͼ�����ݵ�ָ��
  LPBYTE lpDst;
  // ��ʱָ��
  LPBYTE lpTemp;
  // ָ��BITMAPINFOHEADER��ָ��
  LPBITMAPINFOHEADER lpBI;

  // ���Զ�ȡPCX�ļ�ͷ
  if (FREAD(file, (LPBYTE) & pcxHdr, sizeof(PCXHEADER)) != sizeof(PCXHEADER)) {
    // ��С���ԣ�����NULL��
    return NULL;
  }

  // �ж��Ƿ���256ɫPCX�ļ�������һ���ֽ��Ƿ���0x0A��
  if ((pcxHdr.bManufacturer != 0x0A) || (pcxHdr.bBpp != 8) || (pcxHdr.bPlanes != 1)) {
    // ��256ɫPCX�ļ�������NULL��
    return NULL;
  }

  // ��ȡͼ��߶�
  wHeight = pcxHdr.wBottom - pcxHdr.wTop + 1;
  // ��ȡͼ����
  wWidth = pcxHdr.wRight - pcxHdr.wLeft + 1;
  // ����ͼ��ÿ�е��ֽ���
  lLineBytes = WIDTHBYTES(wWidth * 8);
  // ����DIB���ȣ��ֽڣ�
  dwDIBSize = sizeof(BITMAPINFOHEADER) + 1024 + wHeight * lLineBytes;
  // ΪDIB�����ڴ�
  pDIB = MALLOC(BYTE, dwDIBSize);

  if (pDIB == 0) {
    // �ڴ����ʧ�ܣ�����NULL��
    return NULL;
  }

  // ��ֵ
  lpBI = (LPBITMAPINFOHEADER) pDIB;
  // ��lpBI��Ա��ֵ
  lpBI->biSize = 40;
  lpBI->biWidth = wWidth;
  lpBI->biHeight = wHeight;
  lpBI->biPlanes = 1;
  lpBI->biBitCount = 8;
  lpBI->biCompression = BI_RGB;
  lpBI->biSizeImage = wHeight * lLineBytes;
  lpBI->biXPelsPerMeter = pcxHdr.wXResolution;
  lpBI->biYPelsPerMeter = pcxHdr.wYResolution;
  lpBI->biClrUsed = 0;
  lpBI->biClrImportant = 0;
  // �����ڴ��Զ�ȡ����������
  lpSrc = MALLOC(BYTE, GetLength(file) - sizeof(PCXHEADER) - 769);
  lpTemp = lpSrc;

  // ��ȡ����������
  if (FREADHUGE(file, lpSrc, GetLength(file) - sizeof(PCXHEADER) - 769) !=
      GetLength(file) - sizeof(PCXHEADER) - 769) {
    // ��С����
    // �ͷ��ڴ�
    free(lpSrc);
    free(pDIB);
    // ����NULL��
    return NULL;
  }

  // ����DIB������λ��
  lpDst = (LPBYTE) DIBBits(pDIB);

  // һ��һ�н���
  for (j = 0; j < wHeight; j++) {
    i = 0;

    while (i < wWidth) {
      // ��ȡһ���ֽ�
      bChar = *lpTemp;
      lpTemp++;

      if ((bChar & 0xC0) == 0xC0) {
        // �г�
        iCount = bChar & 0x3F;
        // ��ȡ��һ���ֽ�
        bChar = *lpTemp;
        lpTemp++;
        // bChar�ظ�iCount�α���
        memset(&lpDst[(wHeight - j - 1) * lLineBytes + i ], bChar, iCount);
        // �Ѿ���ȡ���صĸ�����iCount
        i += iCount;
      }
      else {
        // ���浱ǰ�ֽ�
        lpDst[(wHeight - j - 1) * lLineBytes + i ] = bChar;
        // �Ѿ���ȡ���صĸ�����1
        i += 1;
      }
    }
  }

  // �ͷ��ڴ�
  FREE(lpSrc);
  // ��ɫ��
  // ����ɫ���־λ
  FREAD(file, &bChar, 1);

  if (bChar != 0x0C) {
    // ����
    // �ͷ��ڴ�
    free(pDIB);
    // ����NULL��
    return NULL;
  }

  // �����ڴ��Զ�ȡ����������
  lpSrc = MALLOC(BYTE, 768);
  // ����DIB�е�ɫ���λ��
  lpDst = (LPBYTE) pDIB + sizeof(BITMAPINFOHEADER);

  // ��ȡ��ɫ��
  if (FREAD(file, lpSrc, 768) != 768) {
    // ��С���ԡ�
    // �ͷ��ڴ�
    free(pDIB);
    // ����NULL��
    return NULL;
  }

  // ����ɫ�帳ֵ
  for (i = 0; i < 256; i++) {
    lpDst[ i * 4 ] = lpSrc[ i * 3 + 2 ];
    lpDst[ i * 4 + 1 ] = lpSrc[ i * 3 + 1 ];
    lpDst[ i * 4 + 2 ] = lpSrc[ i * 3 ];
    lpDst[ i * 4 + 3 ] = 0;
  }

  // �ͷ��ڴ�
  FREE(lpSrc);
  // ����DIB���
  return pDIB;
}
// ��õ�ǰ����ʹ�õ�ϵͳ��ɫ��ľ��
// ����ֵ:
//   HPALETTE    - ϵͳ��ɫ����
static HPALETTE GetSystemPalette()
{
  // �豸������
  HDC hDC;
  // ������ɫ������ָ�����ʱ����
  static HPALETTE hPal = NULL;
  HANDLE hLogPal;
  LPLOGPALETTE lpLogPal;
  // ��ǰϵͳ��ɫ�����ɫ��
  int nColors;
  // ��õ�ǰϵͳ�豸������
  hDC = GetDC(NULL);

  if (!hDC) {
    return NULL;
  }

  // ��õ�ǰϵͳ��ɫ�����ɫ��Ŀ
  nColors = (1 << (GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES)));
  // ����ɫ������ڴ�
  hLogPal = MALLOC(BYTE, sizeof(LOGPALETTE) + nColors *
      sizeof(PALETTEENTRY));

  if (!hLogPal) {
    return NULL;
  }

  // �õ���ɫ���ڴ�ָ��
  lpLogPal = (LPLOGPALETTE) GlobalLock(hLogPal);
  // ���õ�ɫ����Ϣ
  lpLogPal->palVersion = 0x300;
  lpLogPal->palNumEntries = nColors;
  // ��ϵͳ�ĵ�ɫ�忽������ǰ���߼���ɫ����
  GetSystemPaletteEntries(hDC, 0, nColors,
      (LPPALETTEENTRY)(lpLogPal->palPalEntry));
  // ����Windows��ɫ��
  hPal = CreatePalette(lpLogPal);
  // �ͷ��ѷ����ڴ沢ɾ����ʱ����
  GlobalUnlock(hLogPal);
  GlobalFree(hLogPal);
  ReleaseDC(NULL, hDC);
  // ����
  return hPal;
}
// ������һ���µĵ�ɫ�壬����ָ���ĵ�ɫ�忽����ɫ������
// �������:
//   HPALETTE hPalSrc   - ��Ҫ������Դ��ɫ����
//
// ����ֵ:
//   HPALETTE    - ��������ɹ����򷵻ؿ����ĵ�ɫ����
static HPALETTE CopyPalette(HPALETTE hPalSrc)
{
  // ��ɫ��ָ�룬��ʱ����
  PLOGPALETTE plogPal;
  // ����һ����ɫ������һ����ʱ���
  HPALETTE hPalette;
  HANDLE hTemp;
  // ��ɫ�������
  int iNumEntries = 0;
  // ��ȡ��ɫ���еı�����
  iNumEntries = GetPaletteEntries(hPalSrc, 0, iNumEntries, NULL);

  if (iNumEntries == 0) {
    return (HPALETTE) NULL;
  }

  // �����ɫ���ڴ棬�õ����
  hTemp = MALLOC(BYTE, sizeof(DWORD) + sizeof(PALETTEENTRY) * iNumEntries);

  if (! hTemp) {
    return (HPALETTE) NULL;
  }

  // �õ���ɫ���ָ��
  plogPal = (PLOGPALETTE) GlobalLock(hTemp);

  if (! plogPal) {
    return (HPALETTE) NULL;
  }

  // ���õ�ɫ�����Ϣ
  plogPal->palVersion = 0x300;
  plogPal->palNumEntries = (WORD) iNumEntries;
  // ��ȡ�߼���ɫ����ָ����Χ�ĵ�ɫ�����
  GetPaletteEntries(hPalSrc, 0, iNumEntries, plogPal->palPalEntry);
  // ����һ��Windows��ɫ��
  hPalette = CreatePalette(plogPal);
  // �ͷ��Է�����ڴ�
  GlobalUnlock(hTemp);
  GlobalFree(hTemp);
  return hPalette;
}
// ����ȡ��ɫ��������һ��Windows��ɫ�壬�����ش˵�ɫ��ľ��
// �������:
//   LPVOID  lpvColorTable - ��ɫ��ָ��
//   LPBITMAPINFOHEADER lpBMIH  - DIB��Ϣͷָ��
// ����ֵ:
//   HPALETTE    - ����ɹ������ɫ����
static HPALETTE MakeDIBPalette(LPVOID lpvColorTable, LPBITMAPINFOHEADER lpBMIH)
{
  // ��ɫ����
  HPALETTE hPalette = NULL;
  LPLOGPALETTE pLogPal;
  LPRGBQUAD pDibQuad;
  // ��ɫ����ɫ��
  int i, nColorTableEntries;

  // ����DIB�еĵ�ɫ��ָ��
  // lpvColorTable = (LPBYTE )lpBMIH + sizeof(BITMAPINFOHEADER);
  // �����ɫ��ı�����
  if (lpBMIH->biClrUsed == 0) {
    switch (lpBMIH->biBitCount) {
    case 1:
      nColorTableEntries = 2;
      break;

    case 4:
      nColorTableEntries = 16;
      break;

    case 8:
      nColorTableEntries = 256;
      break;

    case 16:
    case 24:
    case 32:
      nColorTableEntries = 0;
      break;

    default:
      break;
    }
  }
  // �����ɫ��ı����������õ�����ɫ��Ŀ
  else {
    nColorTableEntries = lpBMIH->biClrUsed;
  }

  assert((nColorTableEntries >= 0) && (nColorTableEntries <= 256));

  // ��������ڵ�ɫ�壬�򷵻�FALSE
  if (nColorTableEntries == 0) {
    return 0;
  }

  // ���߼���ɫ������ڴ�
  pLogPal = (LPLOGPALETTE) MALLOC(BYTE, 2 * sizeof(WORD) +
      nColorTableEntries * sizeof(PALETTEENTRY));
  // �����߼���ɫ�����Ϣ
  pLogPal->palVersion = 0x300;
  pLogPal->palNumEntries = nColorTableEntries;
  // ����DIB�е���ɫ���߼���ɫ��
  pDibQuad = (LPRGBQUAD) lpvColorTable;

  for (i = 0; i < nColorTableEntries; i++) {
    pLogPal->palPalEntry[ i ].peRed = pDibQuad->rgbRed;
    pLogPal->palPalEntry[ i ].peGreen = pDibQuad->rgbGreen;
    pLogPal->palPalEntry[ i ].peBlue = pDibQuad->rgbBlue;
    pLogPal->palPalEntry[ i ].peFlags = 0;
    pDibQuad++;
  }

  // �����߼���ɫ��
  hPalette = CreatePalette(pLogPal);
  // ɾ����ʱ����
  FREE(pLogPal);
  // ���ص�ɫ����
  return hPalette;
}
// ����DIB����
// ���е�����StretchDIBits()����SetDIBitsToDevice()������DIB����
// ������豸���ɲ���hDCָ�������Ƶľ��������ɲ���lpDRECTָ����
// ���DIB�������ɲ���lpDIBRectָ����
//   HDC hDC            - ����豸DC
//   const IRECT* lpDRECT     - ���ƾ�������
//   const BITMAPINFO* lpDIBHdr    - BITMAPINFOHEADERָ��
//   LPBYTE lpBits;     - DIB����ָ��
//   const IRECT* lpDIBRect   - Ҫ�����DIB����
//   HPALETTE pPal      - ָ��DIB�����ɫ���ָ��
// ����ֵ:
//   BOOL               - ���Ƴɹ�����TRUE�����򷵻�FALSE��
static BOOL DrawDIB(HDC hDC, const IRECT* dRc, const BITMAPINFO* lpDIBHdr, const BYTE* lpBits, const IRECT* sRc, HPALETTE pPal)
{
  BOOL bSuccess = FALSE;   // �ɹ���־
  HPALETTE hPal = NULL;    // DIB��ɫ��
  HPALETTE hOldPal = NULL; // ��ǰ�ĵ�ɫ��

  if (lpDIBHdr == NULL) {
    return FALSE;
  }

  //lpBits = DIBBits(lpDIBHdr);   // �ҵ�DIBͼ��������ʼλ��

  if (pPal != NULL) {  // ��ȡDIB��ɫ�壬��ѡ����
    hPal = (HPALETTE) pPal;
    hOldPal = SelectPalette(hDC, hPal, TRUE);   // ѡ�е�ɫ��
  }

  SetStretchBltMode(hDC, COLORONCOLOR);     // ������ʾģʽ

  // �ж��ǵ���StretchDIBits()����SetDIBitsToDevice()������DIB����
  if ((RCW(dRc) == RCW(sRc)) && (RCH(dRc) == RCH(sRc))) {
    // ԭʼ��С���������졣
    bSuccess = SetDIBitsToDevice(hDC,
        dRc->l, dRc->t, RCW(dRc), RCH(dRc),
        sRc->l,  // SrcX
        DIBHeight(lpDIBHdr) - sRc->t - RCH(sRc),    // SrcY
        0,  // nStartScan
        DIBHeight(lpDIBHdr),      // nNumScans
        lpBits,  // lpBits
        lpDIBHdr,    // lpBitsInfo
        DIB_RGB_COLORS);             // wUsage
  }
  else {
    // ��ԭʼ��С�����졣
    bSuccess = StretchDIBits(hDC,   // hDC
        dRc->l, dRc->t, RCW(dRc), RCH(dRc),
        sRc->l, sRc->t, RCW(sRc), RCH(sRc),
        lpBits,  // lpBits
        lpDIBHdr,    // lpBitsInfo
        DIB_RGB_COLORS, SRCCOPY); // dwROP
  }

  if (hOldPal != NULL) {
    SelectPalette(hDC, hOldPal, TRUE);   // �ָ���ǰ�ĵ�ɫ��
  }

  return bSuccess;
}
//   ��ԴBITMAP��pDibSrc�е����ݿ�����pDibDst�У�������Ӧ�����ݳ�Ա��ֵ
// �������:
//   HBITMAP hBitmap   - ָ��Դ���ݵ�BITMAP���
//   CDib* pDibDst   - ָ��ת��Ŀ���CDib����ָ��
// ����ֵ:
//   LPBYTE    - ��������ɹ����򷵻�pDib
static LPBYTE DDBToDIB(HBITMAP hBitmap, HPALETTE hPal)
{
  // ����һ��BITMAP�ṹ
  LPBYTE pDib;
  BITMAP bm;
  // �豸������
  HDC hDC;
  // ����λ��
  WORD biBitCount;
  // ��ɫ�������
  int nColorTableEntries, lLineBytes;
  LPBITMAPINFOHEADER lpBMIH;
  LPBYTE lpImage;

  // ���hBitmap�����Ч���򷵻�
  if (!hBitmap) {
    return 0;
  }

  // ���ͼ�����ݵ�bm�У��������һ��������ʾ�������ָ���Ķ����ָ��
  if (!GetObject(hBitmap, sizeof(BITMAP), (LPBYTE) & bm)) {
    return 0;
  }

  // ��������λ��
  biBitCount = bm.bmPlanes * bm.bmBitsPixel;

  if (biBitCount <= 1) {
    biBitCount = 1;
  }
  else if (biBitCount <= 4) {
    biBitCount = 4;
  }
  else if (biBitCount <= 8) {
    biBitCount = 8;
  }
  else {
    biBitCount = 24;
  }

  // �����ɫ��ĳߴ�
  // ���biClrUsedΪ�㣬���õ�����ɫ��Ϊ2��biBitCount�η�
  switch (biBitCount) {
  case 1:
    nColorTableEntries = 2;
    break;

  case 4:
    nColorTableEntries = 16;
    break;

  case 8:
    nColorTableEntries = 256;
    break;

  case 16:
  case 24:
  case 32:
    nColorTableEntries = 0;
    break;

  default:
    ASSERT(FALSE);
  }

  lLineBytes = WIDTHBYTES(bm.bmWidth * biBitCount);
  ASSERT((nColorTableEntries >= 0) && (nColorTableEntries <= 256));
  // ����DIB��Ϣͷ�͵�ɫ����ڴ�
  pDib = MALLOC(BYTE, sizeof(BITMAPINFOHEADER) + 1024 + bm.bmHeight * lLineBytes);
  lpBMIH = (LPBITMAPINFOHEADER)pDib;
  lpBMIH->biSize = sizeof(BITMAPINFOHEADER);
  lpBMIH->biWidth = bm.bmWidth;
  lpBMIH->biHeight = bm.bmHeight;
  lpBMIH->biPlanes = 1;
  lpBMIH->biBitCount = biBitCount;
  lpBMIH->biCompression = BI_RGB;
  lpBMIH->biSizeImage = 0;
  lpBMIH->biXPelsPerMeter = 0;
  lpBMIH->biYPelsPerMeter = 0;
  lpBMIH->biClrUsed = nColorTableEntries;
  lpBMIH->biClrImportant = nColorTableEntries;
  // ����豸�����ľ��
  hDC = GetDC(NULL);

  // select and realize our palette
  // ���û��ָ����ɫ�壬���ϵͳ�л�õ�ǰ��ϵͳ��ɫ��
  if (hPal == NULL) {
    hPal = GetSystemPalette();
  }

  hPal = SelectPalette(hDC, hPal, FALSE);
  RealizePalette(hDC);
  // ����GetDIBits�����Ϣͷ�������ͼ�����ݵĳߴ硣ע������ͼ������ָ��ΪNULL
  GetDIBits(hDC, hBitmap, 0, (UINT) lpBMIH->biHeight, NULL, (LPBITMAPINFO) lpBMIH, DIB_RGB_COLORS);

  // ���û����ȷ�Ļ��ͼ�����ݳߴ磬�����¼���
  if (lpBMIH->biSizeImage == 0) {
    lpBMIH->biSizeImage = (((bm.bmWidth * biBitCount) + 31) / 32 * 4) * bm.bmHeight;
  }

  // ������ͼ�����ݵ��ڴ�
  lpImage = DIBBits(pDib);

  // ����GetDIBits����ͼ�����ݣ�ע�����������ͼ������ָ��
  // �������ͼ�����ݲ��ɹ������ͷ��Ѿ�������ڴ棬������FALSE
  if (GetDIBits(hDC, hBitmap, 0, (UINT) lpBMIH->biHeight, (LPBYTE) lpImage,
      (LPBITMAPINFO) lpBMIH, DIB_RGB_COLORS) == 0) {
    //clean up and return NULL
    FREE(pDib);
    SelectPalette(hDC, hPal, TRUE);
    RealizePalette(hDC);
    ReleaseDC(NULL, hDC);
    return 0;
  }

  // ɾ����ʱ����
  SelectPalette(hDC, hPal, TRUE);
  RealizePalette(hDC);
  ReleaseDC(NULL, hDC);
  return pDib;
}

// ��ָ������λ���ڵ���Ļ���ݿ�����DIB��ԴCDib��pDibSrc�е����ݿ�����pDibDst��
// �������:
//   const IRECT* lpRect   - ��Ҫ��������Ļ����
// ����ֵ:
//   BOOL    - ��������ɹ����򷵻�pDib����
static LPBYTE ScreenToDIB(const IRECT* lpRect)
{
  LPBYTE pDib = 0;
  // ��Ļ�豸�����ĺ��ڴ��豸�����ľ��
  HDC hScrDC, hMemDC;
  // ����BITMAP��ʱ�������ǰ��BITMAP���
  HBITMAP hBitmap, hOldBitmap;
  // ��ɫ����
  HPALETTE hPalette;
  // ��ȡ�������������
  int nX, nY, nX2, nY2;
  // DIBͼ��ĸ߶ȺͿ��
  int lWidth, lHeight;
  // ��Ļ�ֱ���
  int xScrn, yScrn;

  // ��������ľ�������Ϊ�գ��򲻽��н�һ���Ĵ���
  if (0==RCH(lpRect) || 0==RCW(lpRect)) {
    return pDib;
  }

  // �õ�һ����Ļ�豸������
  hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);
  // ��������Ļ�豸���ݵ��ڴ��豸������
  hMemDC = CreateCompatibleDC(hScrDC);
  // �õ����ε���������
  nX = lpRect->l;
  nY = lpRect->t;
  nX2 = lpRect->r;
  nY2 = lpRect->b;
  // �õ���Ļ�ķֱ��ʣ��Ա������жϴ���
  xScrn = GetDeviceCaps(hScrDC, HORZRES);
  yScrn = GetDeviceCaps(hScrDC, VERTRES);

  // �жϾ��������Ƿ񳬳���Ļ
  nX = MAX(nX, 0);
  nY = MAX(nY, 0);
  nX2 = MIN(nX2, xScrn);
  nY2 = MIN(nY2, xScrn);

  // ����DIBͼ��ĸ߶ȺͿ��
  lWidth = nX2 - nX;
  lHeight = nY2 - nY;
  // ����һ������Ļ�豸�����ļ��ݵ�DDBλͼ
  hBitmap = CreateCompatibleBitmap(hScrDC, lWidth, lHeight);
  // ��DDBλͼѡ���ڴ��豸������
  hOldBitmap = (HBITMAP) SelectObject(hMemDC, hBitmap);
  // ����Ļ��ָ�������ͼ���䵽�ڴ��豸��������
  BitBlt(hMemDC, 0, 0, lWidth, lHeight, hScrDC, nX, nY, SRCCOPY);
  // Ȼ����ǰ��ͼ��ѡ�룬���õ���Ļ�����DDBͼ����
  hBitmap = (HBITMAP) SelectObject(hMemDC, hOldBitmap);
  // ����ʱ���豸������ɾ��
  DeleteDC(hScrDC);
  DeleteDC(hMemDC);
  // �õ���ǰϵͳ��ɫ��
  hPalette = GetSystemPalette();
  // ��DDBͼ��ת��ΪDIBͼ��
  pDib = DDBToDIB(hBitmap, hPalette);

  // ɾ����ʱ����
  DeleteObject(hPalette);
  DeleteObject(hBitmap);
  return pDib;
}

#undef FWRITE
#undef FREAD

#endif // _DIBAPI_INL_
