// com.h: interface for the com_t class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_COM_H_)
#define _COM_H_


#define IsComOpen(_PCOM)   (NULL!=(_PCOM))
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef BOOL
#define BOOL int
#endif // BOOL

typedef struct com_t {
  OVERLAPPED ovWrite;
  OVERLAPPED ovRead;
  HANDLE hReadThread;
  HANDLE hThreadEnd;
  HANDLE hComm;
}
com_t;
static int WINAPI AsynComReadThread(void* p);


//����
com_t* Newcom_t(void)
{
  com_t com;
  com.hComm = INVALID_HANDLE_VALUE;
  com.hReadThread = NULL;
  ZeroMemory(&com.ovWrite, sizeof(OVERLAPPED));
  com.ovWrite.hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
  ZeroMemory(&com.ovRead, sizeof(OVERLAPPED));
  com.ovRead.hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
  com.hThreadEnd = CreateEvent(NULL, FALSE, TRUE, NULL);

  if (com.ovRead.hEvent == NULL || com.ovWrite.hEvent == NULL || com.hThreadEnd == NULL) {
    // "hEvent create fail";
    return 0;
  }
  else {
    com_t* pcom = (com_t*) malloc(sizeof(com_t));

    if (!pcom) {
      return 0;
    }

    *pcom = com;
    return pcom;
  }
}

//�رմ��� TURE �ɹ�,FALSE ʧ��
BOOL ComClose(com_t* pcom)
{
  if (IsComOpen(pcom)) {
    if (CloseHandle(pcom->hComm)) {
      pcom->hComm = INVALID_HANDLE_VALUE;
      WaitForSingleObject(pcom->hThreadEnd, 6 * 1000);    //ֻҪ�����¼�������,����Ҫ���д���
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  if (!pcom) {
    return 0;
  }

  if (IsComOpen(pcom) == TRUE) {
    ComClose(pcom);
  }

  if (pcom->ovWrite.hEvent != NULL) {
    CloseHandle(pcom->ovWrite.hEvent);
    pcom->ovWrite.hEvent = NULL;
  }

  if (pcom->ovRead.hEvent) {
    CloseHandle(pcom->ovRead.hEvent);
    pcom->ovRead.hEvent = NULL;
  }

  if (pcom->hThreadEnd == NULL) {
    CloseHandle(pcom->hThreadEnd);
    pcom->hThreadEnd = NULL;
  }

  free(pcom);
  return TRUE;
}

//�ҿ�COMM��,������COMM�ڵ�һЩ����.
// pPort �˿ں�, nBaudRate ������, nParity ��żУ��, nByteSize ��λ, nStopBits ֹͣλ
//
// TURE �ɹ�,FALSE ʧ��
//

com_t* ComOpen(const TCHAR* pPort, int nBaudRate, int nParity, int nByteSize, int nStopBits)
{
  com_t* pcom;
  DCB dcb;  // ���ڿ��ƿ�
  COMMTIMEOUTS timeouts = { // ���ڳ�ʱ���Ʋ���
    100,   // ���ַ������ʱʱ��: 100 ms
    1,   // ������ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms)
    500,   // ������(�����)����ʱʱ��: 500 ms
    1,   // д����ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms)
    100
  };  // ������(�����)д��ʱʱ��: 100 ms
  pcom = Newcom_t();

  if (NULL == pcom) {
    return NULL;
  }

  pcom->hComm = CreateFile(pPort,    // ��������
      GENERIC_READ | GENERIC_WRITE,   // ��д��ʽ
      0,   // ����ʽ����ռ
      NULL,   // Ĭ�ϵİ�ȫ������
      OPEN_EXISTING,   // ������ʽ
      0,   // ���������ļ�����
      NULL);  // �������ģ���ļ�

  if (pcom->hComm == INVALID_HANDLE_VALUE) {
    return NULL;  // �򿪴���ʧ��
  }

  if (GetCommState(pcom->hComm, &dcb) == 0) {
    //ȡDCB
    if (pcom->hComm != INVALID_HANDLE_VALUE) {
      CloseHandle(pcom->hComm);
      pcom->hComm = INVALID_HANDLE_VALUE;
    }

    return NULL;
  }

  dcb.BaudRate = nBaudRate;
  dcb.ByteSize = nByteSize;
  dcb.Parity = nParity;
  dcb.StopBits = nStopBits;

  if (SetCommState(pcom->hComm, &dcb) == 0) {
    //����DCB
    if (pcom->hComm != INVALID_HANDLE_VALUE) {
      CloseHandle(pcom->hComm);
      pcom->hComm = INVALID_HANDLE_VALUE;
    }

    return NULL;
  }

  if (SetupComm(pcom->hComm, 4096, 1024) == 0) {
    //�������������������С
    if (pcom->hComm != INVALID_HANDLE_VALUE) {
      CloseHandle(pcom->hComm);
      pcom->hComm = INVALID_HANDLE_VALUE;
    }

    return NULL;
  }

  if (SetCommTimeouts(pcom->hComm, &timeouts) == 0) {
    //���ó�ʱ
    if (pcom->hComm != INVALID_HANDLE_VALUE) {
      CloseHandle(pcom->hComm);
      pcom->hComm = INVALID_HANDLE_VALUE;
    }

    return NULL;
  }

  //���COMM�Ľ��ջ�����������
  if (!PurgeComm(pcom->hComm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR)) {
    if (pcom->hComm != INVALID_HANDLE_VALUE) {
      CloseHandle(pcom->hComm);
      pcom->hComm = INVALID_HANDLE_VALUE;
    }

    return NULL;
  }

  return pcom;
}


//�򴮿�д����,�첽��ȡ.
// TURE �ɹ�, FALSE ʧ��
// ����Ӳ����ͣ���򴮿ڷ�������, ��������ж�ȡ,���Խ����ڶ���
BOOL ComAsynRead(com_t* pcom)
{
  pcom->hReadThread = CreateThread(NULL, 0, AsynComReadThread, pcom, 0, NULL);

  if (pcom->hReadThread == INVALID_HANDLE_VALUE) {
    return FALSE;
  }
  else {
    return TRUE;
  }
}


//�ҿ�COMM��,������COMM�ڵ�һЩ����.
// pPort �˿ں�, nBaudRate ������, nParity ��żУ��, nByteSize ��λ, nStopBits ֹͣλ
// TURE �ɹ�,FALSE ʧ��
com_t* ComOpenEx(const TCHAR* pPort, int nBaudRate, int nParity, int nByteSize, int nStopBits)
{
  com_t* pcom;
  DCB dcb;  // ���ڿ��ƿ�
  COMMTIMEOUTS timeouts = { // ���ڳ�ʱ���Ʋ���
    100,   // ���ַ������ʱʱ��: 100 ms
    1,   // ������ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms)
    500,   // ������(�����)����ʱʱ��: 500 ms
    1,   // д����ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms)
    100
  };  // ������(�����)д��ʱʱ��: 100 ms
  pcom = Newcom_t();

  if (NULL == pcom) {
    return NULL;
  }

  pcom->hComm = CreateFile(pPort,    // ��������
      GENERIC_READ | GENERIC_WRITE,   // ��д��ʽ
      0,   // ����ʽ����ռ
      NULL,   // Ĭ�ϵİ�ȫ������
      OPEN_EXISTING,   // ������ʽ
      FILE_FLAG_OVERLAPPED,   //�첽
      NULL);  // �������ģ���ļ�

  if (pcom->hComm == INVALID_HANDLE_VALUE) {
    return NULL;  // �򿪴���ʧ��
  }

  if (GetCommState(pcom->hComm, &dcb) == 0) {
    //ȡDCB
    if (pcom->hComm != INVALID_HANDLE_VALUE) {
      CloseHandle(pcom->hComm);
      pcom->hComm = INVALID_HANDLE_VALUE;
    }

    return NULL;
  }

  dcb.BaudRate = nBaudRate;
  dcb.ByteSize = nByteSize;
  dcb.Parity = nParity;
  dcb.StopBits = nStopBits;

  if (SetCommState(pcom->hComm, &dcb) == 0) {
    //����DCB
    if (pcom->hComm != INVALID_HANDLE_VALUE) {
      CloseHandle(pcom->hComm);
      pcom->hComm = INVALID_HANDLE_VALUE;
    }

    return NULL;
  }

  if (SetupComm(pcom->hComm, 4096, 1024) == 0) {
    //�������������������С
    if (pcom->hComm != INVALID_HANDLE_VALUE) {
      CloseHandle(pcom->hComm);
      pcom->hComm = INVALID_HANDLE_VALUE;
    }

    return NULL;
  }

  if (SetCommTimeouts(pcom->hComm, &timeouts) == 0) {
    //���ó�ʱ
    if (pcom->hComm != INVALID_HANDLE_VALUE) {
      CloseHandle(pcom->hComm);
      pcom->hComm = INVALID_HANDLE_VALUE;
    }

    return NULL;
  }

  //���COMM�Ľ��ջ�����������
  if (!PurgeComm(pcom->hComm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR)) {
    if (pcom->hComm != INVALID_HANDLE_VALUE) {
      CloseHandle(pcom->hComm);
      pcom->hComm = INVALID_HANDLE_VALUE;
    }

    return NULL;
  }

  //�������������߳�
  if (!ComAsynRead(pcom)) {
    if (pcom->hComm != INVALID_HANDLE_VALUE) {
      CloseHandle(pcom->hComm);
      pcom->hComm = INVALID_HANDLE_VALUE;
    }

    return NULL;
  }

  return pcom;
}

//�򴮿�д����,�첽��ȡ.
// ��ȡ�����߳�,��������ݹ����Ƚ���ͬ����ȡ,ͬ��ʧ���ڽ����첽
int WINAPI AsynComReadThread(void* p)
{
  com_t* pThis = (com_t*) p;
  DWORD dwEvtMask = 0;
  DWORD dwReadByte = 0;
  DWORD dwBytes = 0;
  char szInBuffer[ 5120 ] = { 0 };
  OVERLAPPED ovEvtMast;
  ZeroMemory(&ovEvtMast, sizeof(OVERLAPPED));
  ovEvtMast.hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);

  if (pThis == NULL) {
    if (ovEvtMast.hEvent != INVALID_HANDLE_VALUE) {
      CloseHandle(ovEvtMast.hEvent);
      ovEvtMast.hEvent = INVALID_HANDLE_VALUE;
    }

    ExitThread(0);
  }
  else {
    while (TRUE) {
      //�ر�com,�˳��߳�
      if (!IsComOpen(pThis)) {
        if (ovEvtMast.hEvent != INVALID_HANDLE_VALUE) {
          CloseHandle(ovEvtMast.hEvent);
          ovEvtMast.hEvent = INVALID_HANDLE_VALUE;

          //set event exit process
          SetEvent(pThis->hThreadEnd);
        }

        ExitThread(0);
      }

      //�����¼�
      if (ovEvtMast.hEvent != INVALID_HANDLE_VALUE) {
        ResetEvent(ovEvtMast.hEvent);
      }

      //�����¼�����,�ȴ��¼�����
      SetCommMask(pThis->hComm, EV_RXCHAR | EV_TXEMPTY);
      WaitCommEvent(pThis->hComm, &dwEvtMask, &ovEvtMast);

      if ((dwEvtMask & EV_RXCHAR) == EV_RXCHAR) {
        COMSTAT ComStat;
        DWORD dwLength = 0;
        DWORD dwErr = 0;
        ZeroMemory(&ComStat, sizeof(COMSTAT));
        ClearCommError(pThis->hComm, &dwErr, &ComStat);
        dwLength = ComStat.cbInQue;

        if (dwLength > 0) {
          BOOL bReadState = FALSE;
          bReadState = ReadFile(pThis->hComm, &szInBuffer, dwLength, &dwReadByte, &pThis->ovRead);

          if (bReadState == FALSE) {
            if (GetLastError() == ERROR_IO_INCOMPLETE) {
              DWORD dwErrCode = 0;

              while (TRUE) {
                if (!GetOverlappedResult(pThis->hComm, &pThis->ovRead, &dwReadByte, TRUE)) {
                  dwErrCode = GetLastError();

                  if (dwErrCode == ERROR_IO_INCOMPLETE) {
                    Sleep(1);
                    continue;
                  }
                  else {
                    if (ovEvtMast.hEvent != INVALID_HANDLE_VALUE) {
                      CloseHandle(ovEvtMast.hEvent);
                      ovEvtMast.hEvent = INVALID_HANDLE_VALUE;
                    }

                    ExitThread(0);
                  }
                }
                else {
                  dwBytes += dwReadByte;

                  if (dwBytes == dwLength || dwBytes == 5120) {
                    //MessageBox( NULL, "FULL", "msg", MB_OK );
                    ZeroMemory(&szInBuffer, 5120);
                    dwBytes = 0;
                    break;
                  }

                  Sleep(1);
                }
              }
            }
            else {
              if (ovEvtMast.hEvent != INVALID_HANDLE_VALUE) {
                CloseHandle(ovEvtMast.hEvent);
                ovEvtMast.hEvent = INVALID_HANDLE_VALUE;
              }

              ExitThread(0);
            }
          }
          else {
            //�Ժ�Ҫ��ȡ����,��Ӷ�ȡ�ص�
            //add function here
            ZeroMemory(&szInBuffer, 5120);

            //MessageBox( NULL, "not overlapped", "msg", MB_OK );
          }
        }
      }

      Sleep(1);
    }
  }

  return 0;
}


//�򴮿�д����,ͬ��д��.
// pData д�����ݵ��ڴ���ַ, nLength д�����ݵĳ���.
// -1 ʧ��, >0 д�����ݴ�С
int ComWrite(const com_t* pcom, const void* pData, int nLength)
{
  int ret = -1;
  int dwNumWrite = 0;

  if (IsComOpen(pcom)) {
    if (WriteFile(pcom->hComm, pData, nLength, &dwNumWrite, NULL) != 0 && dwNumWrite == nLength && dwNumWrite > 0) {
      ret = dwNumWrite;
    }
  }

  return -1;
}


//�򴮿�д����,�첽д��.
// pData д�����ݵ��ڴ���ַ, nLength д�����ݵĳ���.
// -1 ʧ��, >0 д�����ݴ�С
int ComWriteEx(com_t* pcom, const void* pData, int dwLength)
{
  int dwNumWrite = 0;
  BOOL bWriteState = FALSE;
  DWORD dwErrCode = 0;
  int dwWriteByte = 0;

  if (IsComOpen(pcom)) {
    bWriteState = WriteFile(pcom->hComm, pData, dwLength, &dwNumWrite, &pcom->ovWrite);

    if (bWriteState == FALSE) {
      if (GetLastError() == ERROR_IO_PENDING) {
        while (TRUE) {
          //��ȡ�첽���
          if (!GetOverlappedResult(pcom->hComm, &pcom->ovWrite, &dwNumWrite, TRUE)) {
            dwErrCode = GetLastError();

            if (dwErrCode == ERROR_IO_INCOMPLETE) {
              continue;
            }
            else {
              return -1;
            }
          }
          else {
            dwWriteByte += dwNumWrite;

            if (dwWriteByte == dwLength) {
              break;
            }
          }

          Sleep(1);
        }
      }
      else {
        return -1;
      }
    }
    else {
      if (dwLength > 0 && dwLength == dwNumWrite) {
        return dwNumWrite;
      }
      else {
        return -1;
      }
    }
  }

  return dwWriteByte;
}


//��ȡ���ڵ�����, ͬ����ȡ.
// pData ��ȡ����Ҫ��ȡ���ڴ���ַ, nLength ��ȡ���ݵ��ڴ泤��.
//
// -1 ʧ��, >0 ��ȡ���ݴ�С
int ComRead(const com_t* pcom, void* pData, int nLength)
{
  DWORD dwNumRead = 0;

  if (IsComOpen(pcom)) {
    OVERLAPPED Overlapped = {0};

    if (ReadFile(pcom->hComm, pData, nLength, &dwNumRead, NULL) != 0 && (int)dwNumRead == nLength && dwNumRead >= 0) {
      return dwNumRead;
    }
    else {
      return -1;
    }
  }

  return -1;
}


// ��ȡ���ڵ�����, ͬ����ȡ.
// pData ��ȡ����Ҫ��ȡ���ڴ���ַ, nLength ��ȡ���ݵ��ڴ泤��.
// -1 ʧ��, >0 ��ȡ���ݴ�С
int ComReadEx(const com_t* pcom, char* pData, int nLength)
{
  int iTime = 2;

  while (1) {
    //�������û�����ݷ���,ComReadһ������Ϊ-1.
    int len = ComRead(pcom, pData, nLength);

    if (len > 0) {
      return len;
    }

    iTime--;

    if (iTime <= 0) {
      return 0;
    }

    //Sleep(100);
  }
}


#endif // _COM_H_
