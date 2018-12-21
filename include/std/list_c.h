// ͨ������

 
#ifndef MYLIST_H_INCLUDED
#define MYLIST_H_INCLUDED
#include <stdio.h>
 

#define DEFAULT_INITIAL_CAPACITY 16
#define DEFAULT_LOAD_FACTOR 0.75f

typedef struct myNode
{
    void * data;
    struct myNode *next;
} MyNode;
 
typedef struct myList
{
    MyNode * first;
    MyNode * last;
    int count;
    int (*equal)(void * a, void * b);
} MyList;
 
typedef struct myListIterator
{
    MyNode * p;
    int count;
    int allSize;
} MyListIterator;
 
//��������
MyList * createMyList();
 
//��������������Ȳ��������ڲ���
MyList * createMySearchList(int(*equal)(void * a, void * b));
 
//�ͷ�����
void freeMyList(MyList * list);
 
//������β��
void myListInsertDataAtLast(MyList* const list, void* const data);
 
//�������ײ�
void myListInsertDataAtFirst(MyList * const list, void* const data);
 
//����
void myListInsertDataAt(MyList * const list, void* const data, int index);
 
//ɾ����β��
void* myListRemoveDataAtLast(MyList* const list);
 
//ɾ�����ײ�
void* myListRemoveDataAtFirst(MyList * const list);
 
//ɾ��
void* myListRemoveDataAt(MyList* const list, int index);
 
//ɾ������,�����Ƿ�ɾ���ɹ�
int myListRemoveDataObject(MyList* const list, void * data);
 
//����
int myListGetSize(const MyList * const list);
 
//��ӡ
void myListOutput(const MyList * const list, void(*pt)(const void * const));
 
//ȡ������
void* myListGetDataAt(const MyList * const list, int index);
 
//ȡ�õ�һ������
void* myListGetDataAtFirst(const MyList * const list);
 
//ȡ�����һ������
void* myListGetDataAtLast(const MyList * const list);
 
//����ĳ�����ݵ�λ��,���equal����Ϊ�գ��Ƚϵ�ַ���������equal����
//��������ڷ���-1��������ڣ����س��ֵĵ�һ��λ��
int myListFindDataIndex(const MyList * const list, void * data);
 
//����������
MyListIterator* createMyListIterator(const MyList * const list);
 
//�ͷű�����
void freeMyListIterator(MyListIterator* iterator);
 
//�������Ƿ�����һ��Ԫ��
int myListIteratorHasNext(const MyListIterator* const iterator);
 
//���ر���������һ��Ԫ��
void * myListIteratorNext(MyListIterator* const iterator);
 
#endif // MYLIST_H_INCLUDED

/*************************
*** File myList.c
**************************/
 
#include <stdlib.h>
//��������
MyList * createMyList()
{
    MyList * re = (MyList *) malloc(sizeof(MyList));
    re->count = 0;
    re->first = NULL;
    re->last = NULL;
    re->equal = NULL;
    return re;
}
 
//�ͷ�����
void freeMyList(MyList * list)
{
    MyNode * p;
    while (list->first)
    {
        p = list->first->next;
        free(list->first);
        list->first = p;
    }
    free(list);
}
 
//������β��
void myListInsertDataAtLast(MyList * const list, void* const data)
{
    MyNode * node = (MyNode *) malloc(sizeof(MyNode));
    node->data = data;
    node->next = NULL;
    if (list->count)
    {
        list->last->next = node;
        list->last = node;
    }
    else
    {
        list->first = node;
        list->last = node;
    }
    (list->count)++;
}
 
//�������ײ�
void myListInsertDataAtFirst(MyList * const list, void* const data)
{
    MyNode * node = (MyNode *) malloc(sizeof(MyNode));
    node->data = data;
    node->next = NULL;
 
    if (list->count)
    {
        node->next = list->first;
        list->first = node;
    }
    else
    {
        list->first = node;
        list->last = node;
    }
    (list->count)++;
}
 
//����
int myListGetSize(const MyList * const list)
{
    return list->count;
}
 
//��ӡ
void myListOutput(const MyList * const list, void(*pt)(const void * const))
{
    MyNode * p = list->first;
    while (p)
    {
        (*pt)(p->data);
        p = p->next;
    }
}
 
//ɾ����β��
void* myListRemoveDataAtLast(MyList* const list)
{
    if (list->count == 1)
    {
        return myListRemoveDataAtFirst(list);
    }
    MyNode * p = list->first;
    while (p->next != list->last)
    {
        p = p->next;
    }
    void *re = list->last->data;
    free(list->last);
    p->next = NULL;
    list->last = p;
    (list->count)--;
    return re;
}
 
//ɾ�����ײ�
void* myListRemoveDataAtFirst(MyList * const list)
{
    MyNode *p = list->first;
    list->first = p->next;
    void * re = p->data;
    free(p);
    (list->count)--;
    if (list->count == 0)
    {
        list->last = NULL;
    }
    return re;
}
 
//����
void myListInsertDataAt(MyList * const list, void* const data, int index)
{
    if (index == 0)
    {
        myListInsertDataAtFirst(list, data);
        return;
    }
    if (index == list->count)
    {
        myListInsertDataAtLast(list, data);
        return;
    }
    MyNode * node = (MyNode *) malloc(sizeof(MyNode));
    node->data = data;
    node->next = NULL;
 
    MyNode * p = list->first;
    for (int i = 0; i < index - 1; i++)
    {
        p = p->next;
    }
    node->next = p->next;
    p->next = node;
 
    (list->count)++;
}
 
//ɾ��
void* myListRemoveDataAt(MyList* const list, int index)
{
    if (index == 0)
    {
        return myListRemoveDataAtFirst(list);
    }
    if (index == list->count - 1)
    {
        return myListRemoveDataAtLast(list);
    }
 
    MyNode * p = list->first;
    for (int i = 0; i < index - 1; i++)
    {
        p = p->next;
    }
    MyNode *tp = p->next;
    p->next = p->next->next;
    void * re = tp->data;
    free(tp);
    (list->count)--;
    return re;
}
 
//ȡ������
void* myListGetDataAt(const MyList * const list, int index)
{
    if (index == list->count - 1)
    {
        return myListGetDataAtLast(list);
    }
    MyNode * p = list->first;
    for (int i = 0; i < index; i++)
    {
        p = p->next;
    }
    return p->data;
}
 
//ȡ�õ�һ������
void* myListGetDataAtFirst(const MyList * const list)
{
    return list->first->data;
}
 
//ȡ�����һ������
void* myListGetDataAtLast(const MyList * const list)
{
    return list->last->data;
}
 
//����ĳ�����ݵ�λ��,���equal����Ϊ�գ��Ƚϵ�ַ���������equal����
//��������ڷ���-1��������ڣ����س��ֵĵ�һ��λ��
int myListFindDataIndex(const MyList * const list, void * data)
{
    MyNode * p = list->first;
    int re = 0;
    if (list->equal)
    {
        while (p)
        {
            if (p->data == data || (*(list->equal))(p->data, data))
            {
                return re;
            }
            re++;
            p = p->next;
        }
 
    }
    else
    {
        while (p)
        {
            if (p->data == data)
            {
                return re;
            }
            re++;
            p = p->next;
        }
    }
    return -1;
}
 
//��������������Ȳ��������ڲ���
MyList * createMySearchList(int(*equal)(void * a, void * b))
{
    MyList * re = createMyList();
    re->equal = equal;
    return re;
}
 
//����������
MyListIterator* createMyListIterator(const MyList * const list)
{
    MyListIterator * re = (MyListIterator *) malloc(sizeof(MyListIterator));
    re->p = list->first;
    re->allSize = list->count;
    re->count = 0;
    return re;
}
 
//�ͷű�����
void freeMyListIterator(MyListIterator* iterator)
{
    free(iterator);
}
 
//�������Ƿ�����һ��Ԫ��
int myListIteratorHasNext(const MyListIterator* const iterator)
{
    return iterator->count < iterator->allSize;
}
 
//���ر���������һ��Ԫ��
void * myListIteratorNext(MyListIterator* const iterator)
{
    void * re = iterator->p->data;
    iterator->p = iterator->p->next;
    (iterator->count)++;
    return re;
}
 
//ɾ������,�����Ƿ�ɾ���ɹ�
int myListRemoveDataObject(MyList* const list, void * data)
{
    MyListIterator * it = createMyListIterator(list);
    int a = 0;
    while (myListIteratorHasNext(it))
    {
        void * ld = myListIteratorNext(it);
        if (data == ld || (list->equal != NULL && (*(list->equal))(ld, data)))
        {
            a = 1;
            break;
        }
    }
    if (a)
    {
        myListRemoveDataAt(list, it->count - 1);
    }
    return a;
}

/*************************
*** File main.c
*** test for MyList
**************************/
#include <stdio.h>
#include <stdlib.h>
 
typedef struct a
{
    int i;
    char c;
} A;
 
void ppt(const void* const p)
{
    A * pp = (A*)p;
    printf("%d(%c) ", pp->i, pp->c);
}
 
 
int test_list_c()
{
    const int S =10;
 
    //��������ʼ������
    A * data= (A*)malloc(sizeof(A)*S);
    for (int i=0; i< S; i++)
    {
        data[i].i=i;
        data[i].c=(char)('A'+0);
    }
 
    //��������
    MyList * list= createMyList();
 
    //�������ֲ��뷽��
    myListInsertDataAtLast( list, &data[0]);
    myListInsertDataAtFirst( list, &data[4]);
    myListInsertDataAt(list, &data[1], 1 );
 
 
    //���Բ���
    int index = myListFindDataIndex(list, &data[2]);
    printf("%d\n", index);
    index = myListFindDataIndex(list, &data[4]);
    printf("%d\n", index);
 
    //���
    myListOutput(list, ppt );
    puts("");
 
    //����ʹ�õ��������
    MyListIterator * it = createMyListIterator(list);
    while(myListIteratorHasNext(it))
    {
        A * pp = (A*)myListIteratorNext(it);
        printf("%d[%c] ", pp->i, pp->c);
    }
    puts("");
    //�ͷŵ�����
    freeMyListIterator(it);
 
    //�ͷ�����
    freeMyList(list);
 
    //�ͷ�����
    free(data);
    return 0;
}


