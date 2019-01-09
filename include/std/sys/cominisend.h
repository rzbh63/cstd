
#ifndef _COMINISEND_H_
#define _COMINISEND_H_
#include "inifile.h"
#include "com.h"
/*
���ڲ����ʣ�9600
ָ��˵����
         UP����0x10
         down����0x11
         middle����0x12
�����ȷ���ָ��0x11
*/
#define FILENAME ".\\test_com.ini"
#define APPNAME "test"

com_t* pcom = NULL;

int hex2data(const char* input, int len, unsigned char* output, int maxoutputlen);
static double IniGetNumber(const char* lpFileName, const char* lpAppName, const char* key)
{
  char buf[256];
  double ret = 0.;
  IniGetString(lpFileName, lpAppName, key, "", buf, 256);

  if ('0' == buf[0] && ('x' == buf[1] || 'X' == buf[1])) {
    int i, len = strlen(buf);
    len = hex2data(buf + 2, len - 2, buf, 256);

    for (ret = 0., i = 0; i < len; ++i) {
      ret = ret * 256 + buf[i];
    }
  }
  else {
    sscanf(buf, "%lf", &ret);
  }

  return ret;
}

com_t* cominiopen()
{
  TCHAR pPort[256];
  int nBaudRate, nParity, nByteSize, nStopBits;
  IniGetString(FILENAME, APPNAME, "pPort", "", pPort, 256);
  nBaudRate = IniGetInt(FILENAME, APPNAME, "nBaudRate", 0);
  nParity = IniGetInt(FILENAME, APPNAME, "nParity", 0);
  nByteSize = IniGetInt(FILENAME, APPNAME, "nByteSize", 0);
  nStopBits = IniGetInt(FILENAME, APPNAME, "nStopBits", 0);

  pcom = ComOpen(pPort, nBaudRate, nParity, nByteSize, nStopBits);
  return pcom;
}

//#include "debug.h"
int comsend(const void* pData, int nLength)
{
  DWORD dwNum = 0;

  if (NULL == pcom) {
    pcom = cominiopen();
  }

  if (pcom) {
    dwNum = ComWrite(pcom, pData, nLength);
    //ComClose( pcom );
  }

  return dwNum;
}

int comrecv(void* pData, int nLength)
{
  DWORD dwNum = 0;

  if (NULL == pcom) {
    pcom = cominiopen();
  }

  if (pcom) {
    //utime_start(_start_time);
    dwNum = ComRead(pcom, pData, nLength);
    //dwNum = ComReadEx( pcom, pData, nLength );
    //printf("%f\n", utime_elapsed(_start_time));
    //ComClose( pcom );
  }

  //OutputDebugLastError();
  return dwNum;
}
//����ͷ�ɣ�˵����
//1 �����ڽ��յ�����֡��A2 01 10 00 00 00 00 00 11 2A����ʱ1������ͷ������2������ͷ�رա����ɼ���ͼƬ��Դ��1������ͷ��
//2 �����ڽ��յ�����֡��A2 01 20 00 00 00 00 00 21 2A����ʱ2������ͷ������1������ͷ�رա����ɼ���ͼƬ��Դ��2������ͷ��
//���ݷ���˵����
//1����鵽1������ͷ��⵽�����������������֡��A2 01 11 00 00 00 00 00 12 2A��
//2����鵽2������ͷ��⵽�����������������֡��A2 01 21 00 00 00 00 00 22 2A��
//3����������ͷ1��2����������ʶ��ɹ��������������֡��A2 01 30 00 00 00 00 00 31 2A��
#define CMDLEN 13
typedef struct COMDATAFMT {
  unsigned char buf[CMDLEN];
} COMDATAFMT;
static const COMDATAFMT d1 = {0xA2, 0x01, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x2A};
static const COMDATAFMT d2 = {0xA2, 0x01, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x2A};

static const COMDATAFMT d3 = {0xA2, 0x01, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x2A};
static const COMDATAFMT d4 = {0xA2, 0x01, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x2A};

static const COMDATAFMT d5 = {0xA2, 0x01, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0x2A};
int curcap = 1;
int curcap2 = 0;
char recvdatastr[256];
int capcnt1 = 11;
int curcapnum = 1;
int dosend = 0;
int cansend = 0, canrecv = 0;
DWORD sendok(int curcap1, int isregok)
{
  //recvthd(0);
  {
    if (curcap1) {
      const COMDATAFMT* d = curcap1 == 1 ? &d3 : &d4;
      comsend(d, sizeof(COMDATAFMT));
    }

    if (isregok) {
      comsend(&d5, sizeof(COMDATAFMT));
    }
  }
  return 0;
}
int recvcmd(COMDATAFMT* c)
{
  return comrecv(c, CMDLEN);
}
// A2 0A 11 20 00 3A A9 3F 00 01 01 5E 2A
int recvcardID()
{
  COMDATAFMT c = {0};
  int ret = comrecv(&c, CMDLEN);

  if (ret > 0 && c.buf[3] == 0x20) {
    unsigned char* p = c.buf;
    int id = (p[4] << 24) + (p[5] << 16) + (p[6] << 8) + p[7];
    return id;
  }

  return -1;
}
int set_check_sum(const COMDATAFMT* c, COMDATAFMT* b)
{
  //У���=Ŀ�ĵ�ַ+Դ��ַ+������+����0+����1+����2+����3+����4+����5;
  int i, s = 0;

  for (i = 1; i < 10; ++i) {
    s += c->buf[i];
  }

  if (b) {
    b->buf[10] = 0xff & (s >> 8);
    b->buf[11] = 0xff & (s);
  }

  return s;
}
int setled(int ison)
{
  COMDATAFMT c = {0xA2, 0x11, 0x0A, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A};
  int ret;
  c.buf[4] = ison ? 0x01 : 0x00;
  set_check_sum(&c, &c);
  ret = comsend(&c, CMDLEN);
  return ret;
}
static const char hexmap[] = "0123456789ABCDEF";
#define bytehex(ch, hex)  ((char*)hex)[0] = hexmap[(ch)>>4], ((char*)hex)[1] = hexmap[(ch) & 0x0f]
#define hex2byte(hex, _I) (((hex[_I])>='A'?((hex[_I])-'A'+10):((hex[_I])-'0'))<<((1-_I)*4))
#define bin2byte(bin, _I) ((bin[_I])-'0')<<(7-_I)
int hex2data(const char* input, int len, unsigned char* output, int maxoutputlen)
{
  int i, j, outlen = 0;
  const char* ch;
  static const int bitlen = 2;
  ASSERT(maxoutputlen * bitlen >= len);
  ASSERT(len % bitlen == 0);

  for (i = 0; i < len; i += bitlen) {
    ch = input + i;
    output[outlen] = 0;

    for (j = 0; j < bitlen; ++j) {
      output[outlen] += hex2byte(ch, j);
    }

    outlen++;
  }

  return outlen;
}

int cominisend(void* key)
{
  char buf[256];
  int len;
  IniGetString(FILENAME, APPNAME, key, "", buf, 256);
  len = strlen(buf);
  printf("%s=%s\n", key, buf);

  if ('0' == buf[0] && ('x' == buf[1] || 'X' == buf[1])) {
    len = hex2data(buf + 2, len - 2, buf, 256);
    buf[len] = 0;
  }

  return comsend(buf, len);
}

int cominirecvbyte(int* recvlen)
{
  unsigned char buf[256];
  int len, ret = 0;
  len = comrecv(buf, 1);

  if (recvlen) {
    *recvlen = len;
  }

  if (len > 0) {
    ret = buf[0];
  }

  return ret;
}

double regthd()
{
  double thd = IniGetNumber(FILENAME, APPNAME, "ʶ����ֵ");
  return thd;
}

double change_speed()
{
  int by;
  double ret;

  return IniGetNumber(FILENAME, APPNAME, "Ĭ�ϼ��");
  by = cominirecvbyte(0);
  ret = IniGetNumber(FILENAME, APPNAME, "Ĭ�ϼ��");

  if (by == (int)IniGetNumber(FILENAME, APPNAME, "�ı�ɼ�Ƶ��ָ��")) {
    ret = IniGetNumber(FILENAME, APPNAME, "�ɼ����");
  }
  else if (by == (int)IniGetNumber(FILENAME, APPNAME, "�ı�ɼ�Ƶ��ָ��")) {
    ret = IniGetNumber(FILENAME, APPNAME, "Ĭ�ϼ��");
  }

  return ret;
}

double change_qiehuanpinlv()
{
  return IniGetNumber(FILENAME, APPNAME, "�л�Ƶ��");
}
#define qiehuanpinlv change_qiehuanpinlv()
#define timedelims change_speed()
// A2 01 10 00 00 00 00 00 11 2A
#endif // _COMINISEND_H_
