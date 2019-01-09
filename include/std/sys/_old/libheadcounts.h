#ifndef _LIBHEADCOUNTS_H_
#define _LIBHEADCOUNTS_H_
#include <stdio.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
  ;
#endif
#define COUNTOF_PI  5
#define MAXOBJ  32
#define MAXPT 1000
#define SERVERPROCID 10
#define SERVERPROCPORT 4444
#if 1
#define _WW 640
#define _HH 480
#else
#define _WW 320
#define _HH 240
#endif
  typedef struct ptid_t {
    POINT pt[MAXOBJ];
    int id[MAXOBJ];
    int ptn;
  } ptid_t;
  typedef struct seid_t {
    POINT s, e;
    POINT pt[MAXPT];
    int ptlen;
    int id;
    int cnted_top;
    int cnted_bot;
  } seid_t;
  typedef struct headcounts_t {
    int h, w;
    int ImerodeThresh; // ��ʴ��ֵ
    int ImdilateThresh; // ������ֵ
    int AreaThresh; // �����ֵ
    int FilterLayerThresh; // ����˲�
    int FilterAreaThresh; // ����˲�
    int verson; // ѡ����и�ʴ
    int BRsize;
    int RN, RD;
    int idmax;
    int lables;
    int Order_verson;
    // ǰ������
    int beishu;
    int top2bot;
    int bot2top;
    int topIn;
    int topOut;
    int botIn;
    int botOut;
    int cnt_t1;
    int cnt_t2;
#define MAXSEID 100
    seid_t se[MAXSEID];
    ptid_t pi[COUNTOF_PI];
    unsigned short* depth;
    unsigned char* BI;
    unsigned char* FrontI;
    unsigned char* FAI;
    unsigned short* RDI;
    unsigned short* FRDI;
    unsigned short* g;
    unsigned char* buf;
    int buflen;
    POINT pt1, pt2;
  } headcounts_t;
  typedef unsigned int SOCKET_T;
  // �ͻ���������ͷ��������
  // ipaddrstr [in] ��ͷ��������IP���˿ڣ����� 192.168.1.7:4444
  // �����׽���
  SOCKET_T ClientProc_beg(const char* ipaddrstr);
  // �رս�
  // svi [in] �׽���
  int ClientProc_end(SOCKET_T svi);
  // �����־
  enum {
    NET_HEADCOUNTS_SETZRRO = 1 << 0, // ��������ͳ�ƽ����0
    NET_HEADCOUNTS_SETPARAM = 1 << 1, // ���÷�����������
    NET_HEADCOUNTS_GETCOLOR = 1 << 2, // �ӷ�������ȡ��ɫͼ�� ��СΪ _HH*_WW*3 �ֽ�
    NET_HEADCOUNTS_GETDEPTH = 1 << 3, // �ӷ�������ȡ���ͼ�� ��СΪ _HH*_WW �ֽ�
  };
  // ��������
  // cmd [in] �������� ����ʹ NET_HEADCOUNTS_SETZRRO�ȱ�־�������
  // ph0 [in out] ������������ͳ�ƽ��
  // BGRImg [out] ָ���ɫͼ���ַ
  // DepthImg [out] ָ�����ͼ���ַ
  int ClientProc_cmd(SOCKET_T svi, int cmd, headcounts_t* ph0,
      unsigned char* BGRImg, unsigned char* DepthImg);
#ifdef __cplusplus
}
#endif
#endif // _LIBHEADCOUNTS_H_

