#ifndef _CHCSOCK_H_
#define _CHCSOCK_H_
#include <stdio.h>
#ifndef FAR
#define FAR
#endif // FAR
#ifdef __cplusplus
extern "C" {
#endif
#define CHCSOCK_ERR_TIMEOUT     (-2) /* ��ʱ���� */
#define CHCSOCK_ERR_DISCONNECT  (-3) /* ���Ӵ��� */
#define CHCSOCK_ERR_HEADER      (-4) /* ��ͷ���󣬶Ͽ����� */
#define CHCSOCK_ERR_LITTLEBUF   (-5) /* ���ջ������ȴ����յ�����С���Ͽ����� */
#define CHCSOCK_ERR_MAXTIMES    (-6) /* ���Գ��������� */
#define CHCSOCK_ERR_SUMMATION   (-7) /* ���Գ��������� */
#define DO_PRINT_FLAGS (0) // ���ڴ�ӡ���ͽ���
  typedef unsigned int CPM_SOCKET;
  struct sockaddr;
#define NET_INVALID(_SOCKET)  ((_SOCKET) == INVALID_SOCKET)
#define PAUSE  system("pause")
#define IF_ERR(_A, MSG)  if (!(_A))  { fprintf(stderr, MSG); }
  /*
   ʹ�����нӿ�ǰ�ȳ�ʼ�� CPM_InitSocket
   ʹ������ʼ�� CPM_InitSocket
  */
  int CPM_InitSocket(void);
  int CPM_UnInitSocket(void);
  /*
   �����׽���(AF_INET,SOCK_STREAM,IPPROTO_TCP)
   ʹ��ǰ����һ���µ��׽���
   af: ͨ�ŷ���������������ַ���ͣ�һ��ΪAF_INET����ʾ��Internet����ʹ��
   type: �׽�������
   procotol: ʹ�õ��ض�Э��
  */
  CPM_SOCKET CPM_OpenSocket(int af, int type, int protocol);
  /*
   �ر��׽��֣��ͷ���ռ�е���Դ�����������˺Ϳͻ��ˣ�
  */
  int CPM_CloseSocket(CPM_SOCKET s);
#define CPM_CLOSE(_SOCKET) CPM_CloseSocket(_SOCKET)
  /*
   �ж��׽������ӣ�֪ͨ�������˻�ͻ���ֹͣ���պͷ������ݡ����������˺Ϳͻ��ˣ�
   HowΪ������ֹ��Щ������ȡֵΪ��SD_RECEIVE��SD_SEND��SD_BOTH��
  */
  int CPM_Shutdown(CPM_SOCKET s, int how);
  /*
   �׽��ֵ����᣺�������׽�����������׼��ͨ�š����ͻ��ˣ�
   sΪ��������Ѵ������׽��֡�
   nameΪ�������socket��ַ��
   namelenΪsocket��ַ�Ľṹ�ĳ���
   StrAddrPort "192.168.1.1:35"
  */
  int CPM_Connect(CPM_SOCKET s, const struct sockaddr FAR* name, int namelen, const struct timeval FAR* timeout);
  int CPM_ConnectAddrStr(CPM_SOCKET s, const char* StrAddrPort, const struct timeval FAR* timeout);
  /*
   ���׽��֣��������˺Ϳͻ��ˣ�
   �����ص�ַ�󶨵����������׽����ϡ�
   s: ����socket���÷��صĲ���δ�����ӵ��׽������������׽��ֺţ���
   û�д���bind����0������SOCKET_ERROR
  */
  int CPM_Bind(CPM_SOCKET s, const struct sockaddr FAR* addr, int namelen);
  int CPM_BindAddrStr(CPM_SOCKET s, const char* StrAddrPort);
  /*
   �������ӣ��������ˣ�
   �����������ӷ�������������Ը���������
  */
  int CPM_Listen(CPM_SOCKET s, int backlog);
  /*
   �׽��ֵȴ����ӣ��������ˣ�
   �������ͻ���������������
   ����������ʱ, ���ر�����ͻ�����ַ�����׽���
   sΪ���ڼ���ģʽ���׽��֡�
   sockaddrΪ���ճɹ��󷵻ؿͻ��˵������ַ��
   addrlenΪ�����ַ�ĳ��ȡ�
   ����ֵ��
   ��ֵ��select����
   ��ֵ���ӵ���socket���
   0���ȴ���ʱ��û�пɶ�д�������ļ�
  */
  CPM_SOCKET CPM_Accept(CPM_SOCKET s, struct sockaddr FAR* addr, int FAR* addrlen);
  CPM_SOCKET CPM_AcceptAddrStr(CPM_SOCKET s, char* StrAddrPort, int slen);
  /*
   �׽��ַ������ݣ����������˺Ϳͻ��ˣ�
   sΪ�������˼������׽��֡�
   bufΪ���������ݻ�������ָ�롣
   lenΪ�������ݻ������ĳ��ȡ�
   flagsΪ���ݷ��ͱ�ǡ�
   ����ֵΪ�������ݵ��ַ�����
   ���ｲһ��������ͱ�ǣ�����8�����۵Ľ��ձ��Ҳһ����
   flagȡֵ����Ϊ0�������¶������ϣ�0��ʾû��������Ϊ��
   #define MSG_OOB 0x1 // process out-of-band data
   #define MSG_PEEK 0x2 // peek at incoming message
   #define MSG_DONTROUTE 0x4 // send without using routing tables
   MSG_OOB��ʾ����Ӧ�ô��ⷢ�ͣ���ν�������ݾ���TCP�������ݡ�
   MSG_PEEK��ʾʹ���õ����ݸ��Ƶ��������ڣ���������ϵͳ��������ɾ����
   MSG_DONTROUTE��ʾ��Ҫ����·�ɳ�ȥ��
  */
  int CPM_Send(CPM_SOCKET s, const void FAR* buf, int len, int flags, const struct timeval FAR* timeout);
  int CPM_SafeSend(CPM_SOCKET s, const void FAR* pbData, int dwDataLen, int flags, const struct timeval FAR* timeout);
  int CPM_SafeSendFile(CPM_SOCKET s, void* pf, int flags, const struct timeval FAR* timeout);
#define CPM_SAFESEND(_SOCKET, _BUF, _LEN) CPM_SafeSend(_SOCKET, (const char *)(_BUF), (_LEN)*sizeof(*(_BUF)), DO_PRINT_FLAGS, NULL)
#define CPM_SAFESENDFILE(_SOCKET, _PF) CPM_SafeSendFile(_SOCKET, _PF, DO_PRINT_FLAGS, NULL)
#define CPM_SEND(_SOCKET, _BUF, _LEN) CPM_SafeSend(_SOCKET, (const char *)(_BUF), (_LEN)*sizeof(*(_BUF)), DO_PRINT_FLAGS, NULL)
#define CPM_SEND2(_SOCKET, _VAL) CPM_SafeSend(_SOCKET, (const char *)&(_VAL), sizeof(_VAL), DO_PRINT_FLAGS, NULL)
  /*
   ��������
   sΪ׼���������ݵ��׽��֡�
   bufΪ׼���������ݵĻ�������
   lenΪ׼���������ݻ������Ĵ�С��
   flagsΪ���ݽ��ձ�ǡ�
   ����ֵ������Ϊ���յ����ݵ��ַ�����
   0��WSAECONNRESET���ӱ��ر�
  */
  int CPM_Recv(CPM_SOCKET s, void* buf, int len, int flags, const struct timeval FAR* timeout);
  int CPM_SafeRecv(CPM_SOCKET s, char FAR* pbData, int dwDataLen, int flags, const struct timeval FAR* timeout);
  int CPM_SafeRecvFile(CPM_SOCKET s, void* pf, int flags, const struct timeval FAR* timeout);
#define CPM_SAFERECV(_SOCKET, _BUF, _LEN) CPM_SafeRecv(_SOCKET, (char *)(_BUF), (_LEN)*sizeof(*(_BUF)), DO_PRINT_FLAGS, NULL)
#define CPM_SAFERECVFILE(_SOCKET, _PF) CPM_SafeRecvFile(_SOCKET, _PF, DO_PRINT_FLAGS, NULL)
#define CPM_RECV(_SOCKET, _BUF, _LEN) CPM_SafeRecv(_SOCKET, (char *)(_BUF), (_LEN)*sizeof(*(_BUF)), DO_PRINT_FLAGS, NULL)
#define CPM_RECV2(_SOCKET, _VAL) CPM_SafeRecv(_SOCKET, (char *)&(_VAL), sizeof(_VAL), DO_PRINT_FLAGS, NULL)
  /*
   ��·���á���select
   �������һ�������׽���״̬��
   readfds:ָ��Ҫ��������ָ��
   writefds:ָ��Ҫ��д����ָ��
   exceptfds:ָ��Ҫ����Ƿ�����ָ��
   timeout:���ȴ�ʱ��
   int PASCAL FAR select (int nfds,fd_set FAR * readfds,fd_set FAR * writefds,
   fd_set FAR * exceptfds,const struct timeval FAR * timeout);
   sendto (_SOCKET, buf, len, flags, to, tolen);
   ����������#define SO_DEBUG 0x0001 // turn on debugging info recording
   �Ƿ����ģʽ#define SO_ACCEPTCONN 0x0002 // socket has had listen()
   �׽����������׽��ֵĵ�ַ��#define SO_REUSEADDR 0x0004 // allow local address reuse
   ��������#define SO_KEEPALIVE 0x0008 // keep connections alive
   ��Ҫ·�ɳ�ȥ#define SO_DONTROUTE 0x0010 // just use interface addresses
   ����Ϊ�㲥#define SO_BROADCAST 0x0020 // permit sending of broadcast msgs
   ʹ�û��ز�ͨ��Ӳ��#define SO_USELOOPBACK 0x0040 // bypass hardware when possible
   ��ǰ����ֵ#define SO_LINGER 0x0080 // linger on close if data present
   �Ƿ�����������#define SO_OOBINLINE 0x0100 // leave received OOB data in line
   ����LINGERѡ��#define SO_DONTLINGER (int)(~SO_LINGER)
   ���ͻ���������#define SO_SNDBUF 0x1001 // send buffer size
   ���ջ���������#define SO_RCVBUF 0x1002 // receive buffer size
   ���ͳ�ʱʱ��#define SO_SNDTIMEO 0x1005 // send timeout
   ���ճ�ʱʱ��#define SO_RCVTIMEO 0x1006 // receive timeout
   ����״̬#define SO_ERROR 0x1007 // get error status and clear
   �׽�������#define SO_TYPE 0x1008 // get socket type
   ��ȡsocket����
   sΪ����ȡ���Ե��׽��֡�
   levelΪ�׽���ѡ��ļ��𣬴�������ض�Э����׽���ר�еġ���IPЭ��ӦΪ IPPROTO_IP��
   optnameΪ��ȡѡ�������
   optvalΪ���ѡ��ֵ�Ļ�����ָ�롣
   optlenΪ�������ĳ���
   getsockopt (_SOCKET, level, optname, optval, optlen)
   ����socket����
   sΪ���������Ե��׽��֡�
   levelΪ�׽���ѡ��ļ����÷�ͬ�ϡ�
   optnameΪ����ѡ�������
   optvalΪ���ѡ��ֵ�Ļ�����ָ�롣
   optlenΪ�������ĳ���
   setsockopt (_SOCKET, level, optname, optval, optlen)
   ��Ϊ������ģʽioctlsocket (FIONBIO, &flag)
   ioctlsocket (_SOCKET, cmd, argp);
  */
  int CPM_GetSockName(CPM_SOCKET s, struct sockaddr FAR* name, int FAR* namelen);
  int CPM_GetPeerName(CPM_SOCKET s, struct sockaddr FAR* name, int FAR* namelen);
#define CPM_GETSOCKNAME(_SOCKET, _ADDR, _NAMELEN) (CPM_GetSockName(_SOCKET, (struct sockaddr*)&(_ADDR), &_NAMELEN) != SOCKET_ERROR)
#define CPM_GETPEERNAME(_SOCKET, _ADDR, _NAMELEN) (CPM_GetPeerName(_SOCKET, (struct sockaddr*)&(_ADDR), &_NAMELEN) != SOCKET_ERROR)
  /*�ڲ�ʹ��*/
  int CPM_AcceptAll(CPM_SOCKET s, int nClientNum, CPM_SOCKET* pClient);
  CPM_SOCKET CPM_StartClient(const char* StrAddrPort);
  CPM_SOCKET CPM_StartServer(int port);
#ifdef __cplusplus
}
#endif
#endif // _CHCSOCK_H_

