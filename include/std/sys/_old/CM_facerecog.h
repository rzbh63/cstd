#ifndef _CM_FACERECOG_H_
#define _CM_FACERECOG_H_
#ifdef __cplusplus
extern "C" {
  ;
#endif
  typedef void* HFACELIB;
  typedef void* HCAM;
  // �û�id����
  typedef struct USERINFO {
    char id[24]; // �û���ʾ��������Ψһ
  } USERINFO;
#define RGBSTDH  (320) // ע����Ƭ���
#define RGBSTDW  (240) // ע����Ƭ�߶�
  // ����ʶ�𷵻����ݽṹ
  typedef struct RECOGINFO {
    USERINFO ui;
    int ispass;   // �Ƿ�ͨ�� 1��ʾͨ�� 0��ʾδͨ�� -1��ʾ���ˣ�ֻ��ͨ��ʱ������ui��������
    int score;
    int workmode; // ����ģʽ
    int register_pos; // �ɼ�����
    int max_register_num; // �ɼ�����
    unsigned char* facereg; // 320*240�Ĳ�ɫע����Ƭ(����320*240*3�ֽ�)
  } RECOGINFO;
  // ������ͷ
  // �ɹ���������ͷ���
  HCAM CM_StartCam(int id);
  // �ر�����ͷ
  // �ɹ�����1��ʧ�ܷ���0
  int CM_CloseCam(HCAM p);
  // ������������
  // �ɹ�������������
  HFACELIB CM_CreateFaceLib();
  // �ͷ���������
  // �ɹ�����1��ʧ�ܷ���0
  int CM_DeleteFaceLib(HFACELIB hLib);
  // ��ȡ����������������id���ɻ�ȡ���
  // ����ui�Ƿ�ΪNULL��������������������������
  int CM_GetFaceInfo(HFACELIB hLib, USERINFO* ui);
  // ����������ɾ��һ���������ݰ�
  // �ɹ�����1��ʧ�ܷ���0
  int CM_DeleteFacePack(HFACELIB hLib, USERINFO* ui);
  // �������������һ���������ݰ�
  // һ���������ݰ�ֻ����һ���˵���������
  // �����ͬһ���ˣ�������ĻḲ���������
  // �ɹ�����1��ʧ�ܷ���0
  int CM_AddFacePack(HFACELIB hLib, const void* buf, int buflen);
  // ��ȡ�����������С
  int CM_GetMaxFacePackLen(HFACELIB hLib);
  // ��ȡ����������
  // buf��������buflen��������С
  // ������������С
  int CM_GetFacePack(HFACELIB hLib, USERINFO* ui, void* buf, int maxbuflen);
  // �ص�����
  typedef int (*RecogCallback_t)(void* userdata, RECOGINFO* fi);
  enum { // type
    CMFR_VERSIONDATE,      // ��ȡ��̬��汾����
    CMFR_HFACELIB,          // ���������� CM_SetLong(h, CMFR_HFACELIB, (unsigned long)(HFACELIB)p);
    CMFR_WORKMODE,          // ����ģʽ 0��ʾֹͣ 1��ʾע�� 2��ʾʶ��CM_SetLong(h, CMFR_WORKMODE, 1);
    CMFR_RECOGCALLBACK,     // ����ʶ��ص�������ÿʶ��һ�� CM_SetLong(h, CMFR_REGISTERCALLBACK, (unsigned long)(RecogCallback_t)fun);
    CMFR_REGISTERUSERINFO,  // ����ע��ģʽ�������û�id CM_SetLong(h, CMFR_REGISTERUSERINFO, (unsigned long)(USERINFO*)userid);
    CMFR_LEVEL,             // �ϸ�̶� ��ȡ3������ 0: ���� 1: �е� 2: �ϸ�С��0��Ϊ0, ����2��Ϊ2��CM_SetLong(h, CMFR_LEVEL, 1);
    CMFR_USERDATA,          // �û��Զ�������  CM_SetLong(h, CMFR_USERDATA, (unsigned long)p);
    CMFR_SHOWVIDEOHWND,     // ��ʾ�ɼ�����Ƶ�Ĵ��ڻ�ؼ���� CM_SetLong(h, CMFR_SHOWVIDEOHWND, (unsigned long)(HWND)p);
    CMFR_SHOWRECT,          // ��ʾ��Ƶ�Ĵ��ڻ�ؼ���� CM_SetLong(h, CMFR_SHOWVIDEOHWND, (unsigned long)(HWND)p);
    CMFR_REGISTERTIMEDELIMS,     // �ɼ�һ�ε�ʱ��������λ���룬���Խ���ɻ�ø������̬�����ɼ�Խ�� CM_SetLong(h, CMFR_REGISTERTIMEDELIMS, (unsigned long)p);
    // ����ģʽ��ʹ��
    CMFR_DEBUG, //
    CMFR_SHOWVIDEOHWND2,   //
  };
  // ���ù�������
  unsigned long CM_SetLong(HCAM p, unsigned long type, unsigned long l);
  //unsigned long CM_GetLong(HCAM p, unsigned long type);
#ifdef __cplusplus
}
#endif
#endif // _CM_FACERECOG_H_

