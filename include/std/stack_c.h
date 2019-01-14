c����ʵ��ͨ�����ݽṹ��������ͨ��׵ջ
2014��03��31�� 13: 25: 36 swwlqw �Ķ�����932
������ͨ������Ļ�����ʵ�ֵ�׵ջ�����������ʵ�ֲμ���http://blog.csdn.net/swwlqw/article/details/22498833��

������˵��׵ջ����ָ��ջ��

ע��׵ջ��ֻ�洢��ָ�룬û�д���ʵ�ʵ����ݡ�



ͷ�ļ���


/*************************
*** File myStack.h
**************************/
#ifndef MYSTACK_H_INCLUDED
#define MYSTACK_H_INCLUDED

#include "myList.h"
typedef MyList MyStack;

//������ջ
MyStack* createMyStack();

//�ͷŶ�ջ
void freeMyStack(MyStack* stack);

//����
void myStackAdd(MyStack* const stack, void* const data);

//ɾ��
void* myStackRemove(MyStack* const stack);

//ȡ�ö�ջͷ
void* myStackGetTop(const MyStack* const stack);

#endif // MYSTACK_H_INCLUDED


Դ�ļ�


/*************************
*** File myStack.c
**************************/
#include "myStack.h"

//������ջ
MyStack* createMyStack()
{
  return createMyList();
}

//�ͷŶ�ջ
void freeMyStack(MyStack* stack)
{
  freeMyList(stack);
}

//����
void myStackAdd(MyStack* const stack, void* const data)
{
  myListInsertDataAtFirst(stack, data);
}

//ɾ��
void* myStackRemove(MyStack* const stack)
{
  return myListRemoveDataAtFirst(stack);
}

//ȡ�ö�ջͷ
void* myStackGetTop(const MyStack* const stack)
{
  return myListGetDataAtFirst(stack);
}


�����ļ�


/*************************
*** File main.c
*** test for MyStack
**************************/
#include <stdio.h>
#include <stdlib.h>
#include "myStack.h"

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
  //����׵ջ
  MyStack* stack = createMyStack();
  //��������
  myStackAdd(stack, &data[0]);
  myStackAdd(stack, &data[2]);
  myStackAdd(stack, &data[6]);
  //����ɾ��
  while (myListGetSize(stack)) {
    A* pp = myStackRemove(stack);
    printf("%d[%c] ", pp->i, pp->c);
  }
  puts("");
  //�ͷ�׵ջ
  freeMyStack(stack);
  //�ͷ�����
  free(data);
  return 0;
}