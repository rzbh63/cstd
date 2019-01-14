// HashSet
//�����µ�����������˵����Ҫ�Զ���HashCode������equal������
//���滹�����˼���������hashCode������equal������

#ifndef __HASHSET_C_H__
#define __HASHSET_C_H__

#include "hashcode_c.h"

#ifndef MYHASHSET_H_INCLUDED
#define MYHASHSET_H_INCLUDED


typedef struct myHashSet {
  int size; //��С
  int initialCapacity; //��ʼ����
  float loadFactor; //��������
  int (*hashCode)(void* data);
  int (*equal)(void* data1, void* data2);
  MyList** dataList;
} MyHashSet;

typedef struct myHashSetIterator {
  int index; //�ڼ�������
  MyHashSet* set;
  MyNode* current;
  int count; //�ڼ�������
} MyHashSetIterator;

//����HashSet
MyHashSet* createMyHashSet(int (*hashCode)(void* data), int (*equal)(void* data1, void* data2));

//ʹ��ȫ����������HashSet
MyHashSet* createMyHashSetForAll(int initialCapacity, float loadFactor, int (*hashCode)(void* data), int (*equal)(void* data1, void* data2));

//�ͷ�HashSet
void freeMyHashSet(MyHashSet* set);

//�Ƿ����ĳ��data
int myHashSetContains(MyHashSet* const set, void* const data);

//����һ������,�����Ƿ���ӳɹ�
int myHashSetAddData(MyHashSet* const set, void* const data);

//���ݵ�����
int myHashSetGetSize(const MyHashSet* const set);

//����������
MyHashSetIterator* createMyHashSetIterator(MyHashSet* const set);

//�ͷŵ�����
void freeMyHashSetIterator(MyHashSetIterator* iterator);

//�������Ƿ�����һ��
int myHashSetIteratorHasNext(MyHashSetIterator* iterator);

//������һ��Ԫ��
void* myHashSetIteratorNext(MyHashSetIterator* iterator);

//ɾ��һ�����ݣ������Ƿ�ɾ���ɹ�
int myHashSetRemoveData(MyHashSet* const set, void* const data);

//���ڶ���Set��ȫ��������뵽��һ�����������ӵĸ���
int myHashSetAddAllSet(MyHashSet* set, MyHashSet* other);

//����HashSet
MyHashSet* myHashSetCopy(MyHashSet* set);

//����
void myHashSetOutput(MyHashSet* set, void(*pt)(void*));

#endif // MYHASHSET_H_INCLUDED

#include <stdlib.h>

//����HashSet
MyHashSet* createMyHashSet(int(*hashCode)(void* data), int(*equal)(void* data1, void* data2))
{
  MyHashSet* re = (MyHashSet*)malloc(sizeof(MyHashSet));
  re->size = 0;
  re->loadFactor = DEFAULT_LOAD_FACTOR;
  re->initialCapacity = DEFAULT_INITIAL_CAPACITY;
  re->dataList = (MyList**) malloc(sizeof(MyList*) * re->initialCapacity);
  re->hashCode = hashCode;
  re->equal = equal;
  for (int i = 0; i < re->initialCapacity; i++) {
    re->dataList[i] = createMySearchList(equal);
  }
  return re;
}

//ʹ��ȫ����������HashSet
MyHashSet* createMyHashSetForAll(int initialCapacity, float loadFactor, int(*hashCode)(void* data), int(*equal)(void* data1, void* data2))
{
  MyHashSet* re = createMyHashSet(hashCode, equal);
  re->initialCapacity = initialCapacity;
  re->loadFactor = loadFactor;
  return re;
}

//�ͷ�HashSet
void freeMyHashSet(MyHashSet* set)
{
  for (int i = 0; i < set->initialCapacity; i++) {
    freeMyList(set->dataList[i]);
  }
  free(set->dataList);
  free(set);
}

//�Ƿ����ĳ��data
int myHashSetContains(MyHashSet* const set, void* const data)
{
  int hasCode = (*(set->hashCode))(data);
  hasCode %= set->initialCapacity;
  if (hasCode < 0) {
    hasCode += set->initialCapacity;
  }
  int re = myListFindDataIndex(set->dataList[hasCode], data);
  return re > -1;
}

void rebuildMyHashSet(MyHashSet* set)
{
  int newSize = set->initialCapacity * 2;
  MyList** newdataList = (MyList**) malloc(sizeof(MyList*) * newSize);
  for (int i = 0; i < newSize; i++) {
    newdataList[i] = createMyList();
  }
  MyHashSetIterator* it = createMyHashSetIterator(set);
  while (myHashSetIteratorHasNext(it)) {
    void* data = myHashSetIteratorNext(it);
    int hasCode = (*(set->hashCode))(data);
    hasCode %= newSize;
    myListInsertDataAtLast(newdataList[hasCode], data);
  }
  freeMyHashSetIterator(it);
  for (int i = 0; i < set->initialCapacity; i++) {
    freeMyList(set->dataList[i]);
  }
  free(set->dataList);
  set->dataList = newdataList;
  set->initialCapacity = newSize;
}

//����һ������,�����Ƿ���ӳɹ�
int myHashSetAddData(MyHashSet* const set, void* const data)
{
  int hasCode = (*(set->hashCode))(data);
  hasCode %= set->initialCapacity;
  if (hasCode < 0) {
    hasCode += set->initialCapacity;
  }
  int re = myListFindDataIndex(set->dataList[hasCode], data);
  if (re == -1) {
    myListInsertDataAtLast(set->dataList[hasCode], data);
    (set->size)++;
    if (set->size > set->initialCapacity * set->loadFactor) {
      rebuildMyHashSet(set);
    }
    return 1;
  }
  return 0;
}

//���ݵ�����
int myHashSetGetSize(const MyHashSet* const set)
{
  return set->size;
}

//����������
MyHashSetIterator* createMyHashSetIterator(MyHashSet* const set)
{
  MyHashSetIterator* re = (MyHashSetIterator*) malloc(
                            sizeof(MyHashSetIterator));
  re->count = 0;
  re->index = 0;
  re->set = set;
  re->current = set->dataList[0]->first;
  return re;
}

//�ͷŵ�����
void freeMyHashSetIterator(MyHashSetIterator* iterator)
{
  free(iterator);
}

//�������Ƿ�����һ��
int myHashSetIteratorHasNext(MyHashSetIterator* iterator)
{
  return iterator->count < iterator->set->size;
}

//������һ��Ԫ��
void* myHashSetIteratorNext(MyHashSetIterator* iterator)
{
  (iterator->count)++;
  while (!(iterator->current)) {
    (iterator->index)++;
    iterator->current = iterator->set->dataList[iterator->index]->first;
  }
  void* re = iterator->current->data;
  iterator->current = iterator->current->next;
  return re;
}

//ɾ��һ�����ݣ������Ƿ�ɾ���ɹ�
int myHashSetRemoveData(MyHashSet* const set, void* const data)
{
  int hasCode = (*(set->hashCode))(data);
  hasCode %= set->initialCapacity;
  if (hasCode < 0) {
    hasCode += set->initialCapacity;
  }
  int re = myListRemoveDataObject(set->dataList[hasCode], data);
  if (re) {
    (set->size)--;
  }
  return re;
}

//���ڶ���Set��ȫ��������뵽��һ�����������ӵĸ���
int myHashSetAddAllSet(MyHashSet* set, MyHashSet* other)
{
  int ssize = set->size;
  MyHashSetIterator* it = createMyHashSetIterator(other);
  while (myHashSetIteratorHasNext(it)) {
    myHashSetAddData(set, myHashSetIteratorNext(it));
  }
  freeMyHashSetIterator(it);
  int re = set->size - ssize;
  return re;
}

//����HashSet
MyHashSet* myHashSetCopy(MyHashSet* set)
{
  MyHashSet* re = createMyHashSetForAll(set->initialCapacity, set->loadFactor, set->hashCode, set->equal);
  myHashSetAddAllSet(re, set);
  return re;
}

//����
void myHashSetOutput(MyHashSet* set, void(*pt)(void*))
{
  MyHashSetIterator* it = createMyHashSetIterator(set);
  while (myHashSetIteratorHasNext(it)) {
    pt(myHashSetIteratorNext(it));
  }
  freeMyHashSetIterator(it);
}

//��4�������ļ�

/*************************
*** File main.c
*** test for MyHashSet
**************************/
#include <stdio.h>
#include <stdlib.h>


int test_hashset_c()
{
  char* strs[] = {
    "abc",
    "qq",
    "hello",
    "abc",
    "lmy",
    "ab",
    "qq",
    "lqw",
    "sww",
    "lqw"
  };
  int S = countof(strs);
  //����������Ҫָ������������hashCode������equal������
  MyHashSet* set = createMyHashSet(myHashCodeString, myEqualString);
  //��������
  for (int i = 0; i < S; i++) {
    myHashSetAddData(set, strs[i]);
  }
  //�����С
  printf("size=%d\n", myHashSetGetSize(set));
  //����ɾ��
  myHashSetRemoveData(set, "qq");
  myHashSetRemoveData(set, "ab");
  myHashSetRemoveData(set, "qwert");
  //�����С
  printf("after remove size=%d\n", myHashSetGetSize(set));
  //����
  MyHashSetIterator* it = createMyHashSetIterator(set);
  while (myHashSetIteratorHasNext(it)) {
    char* pp = (char*)myHashSetIteratorNext(it);
    puts(pp);
  }
  //�ͷű�����
  freeMyHashSetIterator(it);
  //�ͷż���
  freeMyHashSet(set);
  return 0;
}


#endif // __HASHSET_C_H__
