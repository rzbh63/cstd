

#ifndef _CRITICAL_SECTION_
#define _CRITICAL_SECTION_

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif
#include <stdio.h>

class CriticalSection
{
#ifdef WIN32
  CRITICAL_SECTION m_sect;
#else
  pthread_mutex_t   m_sect;
  pthread_mutexattr_t m_attr;
#endif

#ifdef _DEBUG
  long m_Count;
#endif

public:
  CriticalSection() {
#ifdef _DEBUG
    m_Count = 0;
#endif
#ifdef WIN32
    ::InitializeCriticalSection(&m_sect);
#else
    pthread_mutexattr_init(&m_attr);
    pthread_mutexattr_settype(&m_attr, PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutex_init(&m_sect, &m_attr);
#endif
  }
  ~CriticalSection() {
#ifdef _DEBUG

    if (m_Count != 0) {
      printf("����������Ϊ\n");
    }

#endif
#ifdef WIN32
    ::DeleteCriticalSection(&m_sect);
#else
    pthread_mutexattr_destroy(&m_attr);
    pthread_mutex_destroy(&m_sect);
#endif
  }


  void Lock(void) {
#ifdef _DEBUG
    InterlockedIncrement(&m_Count);
#endif

#ifdef WIN32
    ::EnterCriticalSection(&m_sect);
#else
    pthread_mutex_lock(&m_sect);
#endif
  }

  void Unlock(void) {
#ifdef _DEBUG
    InterlockedDecrement(&m_Count);
#endif
#ifdef WIN32
    ::LeaveCriticalSection(&m_sect);
#else
    pthread_mutex_unlock(&m_sect);
#endif
  }
};


class AoutLock
{
  CriticalSection* m_pCritalSection;
public:
  AoutLock(CriticalSection* pCritalSection) {
    if (pCritalSection) {
      pCritalSection->Lock();
      m_pCritalSection = pCritalSection;
    }
  }
  ~AoutLock() {
    if (m_pCritalSection) {
      m_pCritalSection->Unlock();
    }
  }
};

#endif