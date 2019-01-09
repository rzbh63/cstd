#ifndef __EXCEPT_C_H
#define __EXCEPT_C_H
#include <setjmp.h>
/* ����ȫ���쳣���ͣ��ɰ���Ҫ�����κ��쳣���ͣ���ON_EXCEPT��ʹ�� */
#define EXCEPT_ALL        0
//
// ����C����ʹ�õ��쳣���͡������ͺ꣺
//
// 1��Raise(type, msg)���׳�type�쳣��msgΪ�쳣��Ϣ
// 2��RaiseMessage(msg)���׳��쳣���൱��Raise(EXCEPT_ALL, msg)
// 3��ReRaise()�������׳���ǰ���쳣
//
// 4���쳣��Ӧ���Կ��ܳ��ֵ��쳣���д���(���쳣ʱ��������벻ִ��)��
//
//    TRY
//        ��������
//    ON_EXCEPT(type)
//        ��ѡ�����type�쳣�Ĵ��룬����EXCEPTǰ����ʹ��
//    EXCEPT
//       ��ѡ������쳣������룬�൱��ON_EXCEPT(EXCEPT_ALL)
//    END_TRY
//
// 5���쳣�����������Ƿ�����쳣����ִ�еı������ʴ��룬����Դ�ͷţ�
//
//    TRY
//        ��������
//    FINALLY
//        �������ʴ���
//    END_TRY
//
// 6�����쳣��Ƕ��ʹ�ã������ܻ��ã��磺
//
//    TRY
//        �����1
//        TRY
//           �����2
//        FINALLY
//            �������ʴ���
//        END_TRY
//    EXCEPT
//        �쳣�������
//    END_TRY
//
#define TRY                except_Set(); if (!except_SetNum(setjmp(*except_Buf()))) {
#define Raise(type, msg)   except_Raise(type, msg, __FILE__, __LINE__)
#define RaiseMessage(msg)  Raise(EXCEPT_ALL, msg)
#define ReRaise()          except_ReRaise()
#define ON_EXCEPT(type)    } else if (except_On(type)) {
#define EXCEPT             ON_EXCEPT(EXCEPT_ALL)
#define FINALLY            } {
#define END_TRY            } except_end();
/* �쳣�ṹ */
typedef struct __Exception {
  int type;     /* �쳣���� */
  char* message; /* ��Ϣ */
  char* soufile; /* Դ�ļ� */
  int lineNum;  /* �����쳣���к� */
}
Exception;
// ��ȡ��ǰ�쳣��Ϣ
char* except_Message(void);
// ��ȡ��ǰ�쳣�ṹ
Exception* except_Exception(void);
// ���º���Ϊ�ڲ�ʹ��
void except_Set(void);
void except_Raise(int type, const char* message, char* file, int line);
void except_ReRaise(void);
int except_On(int type);
void except_end(void);
jmp_buf* except_Buf(void);
int except_SetNum(int Num);
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "except_c.h"
#define    MESSAGE_FORMAT    "%s(%d): "
enum {evEnter, evRaise, evExcept = 2};
typedef struct __Exception_Event {
  struct __Exception_Event* prev;
  jmp_buf eBuf;
  int evNum;
  Exception exception;
}
Exception_Event;
static Exception_Event* except_ptr = NULL;
static char* except_msg;
static char except_msg_size = 0;
static void except_Set(void)
{
  Exception_Event* ev;
  ev = (Exception_Event*) malloc(sizeof(Exception_Event));
  ev->prev = except_ptr;
  except_ptr = ev;
}
static void except_Clear(void)
{
  Exception_Event* ev;
  if (except_ptr) {
    ev = except_ptr;
    except_ptr = except_ptr->prev;
    free(ev);
  }
}
static void except_Raise(int type, const char* message, char* file, int line)
{
  int len = 0, size;
#ifndef    NDEBUG
  char buf[ 100 ];
  sprintf(buf, MESSAGE_FORMAT, file, line);
  len = strlen(buf);
#endif
  size = strlen(message) + len + 1;
  if (except_msg_size < size) {
    if (except_msg_size > 0) {
      free(except_msg);
    }
    except_msg_size = size;
    except_msg = (char*) malloc(except_msg_size);
  }
#ifndef    NDEBUG
  strcpy(except_msg, buf);
  strcat(except_msg, message);
#else
  strcpy(except_msg, message);
#endif
  if (except_ptr) {
    except_ptr->exception.type = type;
    except_ptr->exception.message = &except_msg[ len ];
    except_ptr->exception.soufile = file;
    except_ptr->exception.lineNum = line;
    longjmp(except_ptr->eBuf, evRaise);
  }
  else {
    fprintf(stderr, except_msg);
    abort();
  }
}
static void except_ReRaise(void)
{
  Exception e;
  if (except_ptr) {
    e = except_ptr->exception;
    if (except_ptr->prev) {
      except_Clear();
      except_ptr->exception = e;
      longjmp(except_ptr->eBuf, evRaise);
    }
    else {
      fprintf(stderr, except_msg);
      abort();
    }
  }
}
static int except_On(int type)
{
  if (except_ptr->evNum == evRaise &&
      (type == EXCEPT_ALL || type == except_ptr->exception.type)) {
    except_ptr->evNum = evExcept;
    return 1;
  }
  return 0;
}
static void except_end(void)
{
  if (except_ptr->evNum == evRaise) {
    except_ReRaise();
  }
  except_Clear();
}
static jmp_buf* except_Buf(void)
{
  return & except_ptr->eBuf;
}
static char* except_Message(void)
{
  return except_msg;
}
static Exception* except_Exception(void)
{
  return & except_ptr->exception;
}
static int except_SetNum(int Num)
{
  except_ptr->evNum = Num;
  return except_ptr->evNum;
}
#endif  /* __EXCEPT_C_H */

