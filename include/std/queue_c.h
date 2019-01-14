c����ʵ��ͨ�����ݽṹ��������ͨ�ö���
2014��03��30�� 23: 15: 08 swwlqw �Ķ�����1508 ��ǩ�� c���� ���ݽṹ ����  ����
���˷��ࣺ c����ʵ��ͨ�����ݽṹ
������ͨ������Ļ�����ʵ�ֵĶ��У����������ʵ�ֲμ���http://blog.csdn.net/swwlqw/article/details/22498833��

ע�������ֻ�洢��ָ�룬û�д���ʵ�ʵ����ݡ�



ͷ�ļ� myQueue.h


#ifndef MYQUEUE_H_INCLUDED
#define MYQUEUE_H_INCLUDED

#include "myList.h"

typedef MyList MyQueue;

//��������
MyQueue* createMyQueue();

//�ͷŶ���
void freeMyQueue(MyQueue* queue);

//����
void myQueueAdd(MyQueue* const queue, void* const data);

//ɾ��
void* myQueueRemove(MyQueue* const queue);

//ȡ�ö�ͷ
void* myQueueGetTop(const MyQueue* const queue);

#endif // MYQUEUE_H_INCLUDED


Դ�ļ� myQueue.c


#include "myQueue.h"

//��������
MyQueue* createMyQueue()
{
  return createMyList();
}

//�ͷŶ���
void freeMyQueue(MyQueue* queue)
{
  freeMyList(queue);
}

//����
void myQueueAdd(MyQueue* const queue, void* const data)
{
  myListInsertDataAtLast(queue, data);
}

//ɾ��
void* myQueueRemove(MyQueue* const queue)
{
  return myListRemoveDataAtFirst(queue);
}

//ȡ�ö�ͷ
void* myQueueGetTop(const MyQueue* const queue)
{
  return myListGetDataAtFirst(queue);
}



�����ļ� main.c

/*************************
*** File main.c
*** test for MyQueue
**************************/
#include <stdio.h>
#include <stdlib.h>
#include "myQueue.h"

typedef struct a {
  int i;
  char c;
} A;


int main()
{
  const int S = 10;
  //��������ʼ������
  A* data = malloc(sizeof(A) * S);
  for (int i = 0; i < S; i++) {
    data[i].i = i;
    data[i].c = (char)('A' + i);
  }
  //��������
  MyQueue* queue = createMyQueue();
  //��������
  myQueueAdd(queue, &data[0]);
  myQueueAdd(queue, &data[2]);
  myQueueAdd(queue, &data[6]);
  //����ɾ��
  while (myListGetSize(queue)) {
    A* pp = myQueueRemove(queue);
    printf("%d[%c] ", pp->i, pp->c);
  }
  puts("");
  //�ͷ�����
  freeMyQueue(queue);
  //�ͷ�����
  free(data);
  return 0;
}


