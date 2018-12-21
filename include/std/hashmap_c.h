// HashMap
//������ͨ������Ļ�����ʵ�ֵ�ӳ�䣬���������ʵ�ֲμ���http://blog.csdn.net/swwlqw/article/details/22498833��
//ע��ӳ����ֻ�洢��key��value��ָ�룬û�д���ʵ�ʵ����ݡ�
//�����µ�key������˵����Ҫ�Զ���HashCode������equal������
//��HashSet��ʵ���и����˼���������hashCode������equal�������μ���http://blog.csdn.net/swwlqw/article/details/22664129��

#ifndef MYHASHMAP_H_INCLUDED
#define MYHASHMAP_H_INCLUDED
#include "list_c.h"
#include "hashset_c.h"
 
 
typedef struct entry
{
    void * key;
    void * value;
} Entry;
 
typedef struct myHashMap
{
    int size;   //��С
    int initialCapacity; //��ʼ����
    float loadFactor;   //��������
    int (*hashCode)(void *key);
    int (*equal)(void *key1,void *key2);
    MyList ** entryList;
} MyHashMap;
 
typedef struct myHashMapEntryIterator
{
    int index;       //�ڼ�������
    MyHashMap *map;
    MyNode *current;
    int count;        //�ڼ�������
} MyHashMapEntryIterator;
 
//����HashMap
MyHashMap *createMyHashMap(int (*hashCode)(void *key),int (*equal)(void *key1,void *key2));
 
//ʹ��ȫ����������HashMap
MyHashMap *createMyHashMapForAll(int initialCapacity,float loadFactor,int (*hashCode)(void *key),int (*equal)(void *key1,void *key2));
 
//�ͷ�HashMap
void freeMyHashMap(MyHashMap * map);
 
//�Ƿ����ĳ��key
int myHashMapContainsKey(MyHashMap *const map,void * const key);
 
//����һ��ӳ��
void myHashMapPutData(MyHashMap *const map,void * const key,void * const value);
 
//ͨ��key�õ����ݣ����û�������򷵻�null
void* myHashMapGetDataByKey(MyHashMap * const map,void *const key);
 
//���ݵ�����
int myHashMapGetSize(const MyHashMap * const map);
 
//����Entry������
MyHashMapEntryIterator* createMyHashMapEntryIterator( MyHashMap *const map);
 
//�ͷ�Entry������
void freeMyHashMapEntryIterator(MyHashMapEntryIterator* iterator);
 
//Entry�������Ƿ�����һ��
int myHashMapEntryIteratorHasNext(MyHashMapEntryIterator* iterator);
 
//������һ��EntryԪ��
Entry* myHashMapEntryIteratorNext(MyHashMapEntryIterator* iterator);
 
//ɾ��һ�����ݣ������Ƿ�ɾ���ɹ�
int myHashMapRemoveDataByKey(MyHashMap *const map,void * const key);
 
//����
void myHashMapOutput(MyHashMap *map, void(*pt)(Entry*));
 
#endif // MYHASHMAP_H_INCLUDED

#include <stdlib.h>
 
//ĳ��Entry�������Ƿ����ĳ��keyֵ��
Entry* listContainsEntry(MyList * list, void * key,
		int(*equal)(void *key1, void *key2)) {
	MyListIterator* it = createMyListIterator(list);
	while (myListIteratorHasNext(it)) {
		Entry * entry = (Entry *) (myListIteratorNext(it));
		if (entry->key == key || (equal != NULL && (*equal)(entry->key, key))) {
			return entry;
		}
	}
	freeMyListIterator(it);
	return NULL;
}
 
void rebuildMyHashMap(MyHashMap * map) {
	int newSize = map->initialCapacity * 2;
	MyList **newentryList = (MyList **) malloc(sizeof(MyList*) * newSize);
	for (int i = 0; i < newSize; i++) {
		newentryList[i] = createMyList();
	}
	MyHashMapEntryIterator* it = createMyHashMapEntryIterator(map);
	while (myHashMapEntryIteratorHasNext(it)) {
		Entry * entry = myHashMapEntryIteratorNext(it);
		int hasCode = (*(map->hashCode))(entry->key);
		hasCode %= newSize;
		if (hasCode < 0)
			hasCode += newSize;
		myListInsertDataAtLast(newentryList[hasCode], entry);
	}
	freeMyHashMapEntryIterator(it);
	for (int i = 0; i < map->initialCapacity; i++) {
		freeMyList(map->entryList[i]);
	}
	free(map->entryList);
	map->entryList = newentryList;
	map->initialCapacity = newSize;
}
 
//����HashMap
MyHashMap *createMyHashMap(int(*hashCode)(void *key),
		int(*equal)(void *key1, void *key2)) {
	MyHashMap *re = (MyHashMap *) malloc(sizeof(MyHashMap));
	re->size = 0;
	re->loadFactor = DEFAULT_LOAD_FACTOR;
	re->initialCapacity = DEFAULT_INITIAL_CAPACITY;
	re->entryList = (MyList **) malloc(sizeof(MyList*) * re->initialCapacity);
	re->hashCode = hashCode;
	re->equal = equal;
	for (int i = 0; i < re->initialCapacity; i++) {
		re->entryList[i] = createMyList();
	}
	return re;
}
 
//ʹ��ȫ����������HashMap
MyHashMap *createMyHashMapForAll(int initialCapacity, float loadFactor,
		int(*hashCode)(void *key), int(*equal)(void *key1, void *key2)) {
	MyHashMap *re = createMyHashMap(hashCode, equal);
	re->initialCapacity = initialCapacity;
	re->loadFactor = loadFactor;
	return re;
}
 
//�Ƿ����ĳ��key
int myHashMapContainsKey(MyHashMap * const map, void * const key) {
	int hasCode = (*(map->hashCode))(key);
	hasCode %= map->initialCapacity;
	if (hasCode < 0)
		hasCode += map->initialCapacity;
	Entry * re = listContainsEntry(map->entryList[hasCode], key, map->equal);
	return re != NULL;
}
 
//����һ��ӳ��
void myHashMapPutData(MyHashMap * const map, void * const key,
		void * const value) {
	int hasCode = (*(map->hashCode))(key);
	hasCode %= map->initialCapacity;
	if (hasCode < 0)
		hasCode += map->initialCapacity;
	Entry * re = listContainsEntry(map->entryList[hasCode], key, map->equal);
	if (re == NULL) {
		Entry * entry = (Entry*) malloc(sizeof(Entry));
		entry->key = key;
		entry->value = value;
		myListInsertDataAtLast(map->entryList[hasCode], entry);
		(map->size)++;
		if (map->size > map->initialCapacity * map->loadFactor) {
			rebuildMyHashMap(map);
		}
	} else {
		re->value = value;
	}
}
 
//ͨ��key�õ����ݣ����û�������򷵻�null
void* myHashMapGetDataByKey(MyHashMap * const map, void * const key) {
	int hasCode = (*(map->hashCode))(key);
	hasCode %= map->initialCapacity;
	if (hasCode < 0)
		hasCode += map->initialCapacity;
	Entry * re = listContainsEntry(map->entryList[hasCode], key, map->equal);
	if (re == NULL) {
		return NULL;
	}
	return re->value;
}
 
//���ݵ�����
int myHashMapGetSize(const MyHashMap * const map) {
	return map->size;
}
 
//����Entry������
MyHashMapEntryIterator* createMyHashMapEntryIterator(MyHashMap * const map) {
	MyHashMapEntryIterator* re = (MyHashMapEntryIterator*) malloc(
			sizeof(MyHashMapEntryIterator));
	re->count = 0;
	re->index = 0;
	re->map = map;
	re->current = map->entryList[0]->first;
	return re;
}
 
//�ͷ�Entry������
void freeMyHashMapEntryIterator(MyHashMapEntryIterator* iterator) {
	free(iterator);
}
 
//Entry�������Ƿ�����һ��
int myHashMapEntryIteratorHasNext(MyHashMapEntryIterator* iterator) {
	return iterator->count < iterator->map->size;
}
 
//������һ��EntryԪ��
Entry* myHashMapEntryIteratorNext(MyHashMapEntryIterator* iterator) {
	(iterator->count)++;
	while (!(iterator->current)) {
		(iterator->index)++;
		iterator->current = iterator->map->entryList[iterator->index]->first;
	}
	Entry * re = (Entry *) iterator->current->data;
	iterator->current = iterator->current->next;
	return re;
}
 
//ɾ��һ�����ݣ������Ƿ�ɾ���ɹ�
int myHashMapRemoveDataByKey(MyHashMap * const map, void * const key) {
	int hasCode = (*(map->hashCode))(key);
	hasCode %= map->initialCapacity;
	if (hasCode < 0)
		hasCode += map->initialCapacity;
	MyListIterator* it = createMyListIterator(map->entryList[hasCode]);
	int re = 0;
	while (myListIteratorHasNext(it)) {
		Entry * entry = (Entry *) (myListIteratorNext(it));
		if ((*(map->equal))(entry->key, key)) {
			myListRemoveDataAt(map->entryList[hasCode], it->count - 1);
			re = 1;
			(map->size)--;
			break;
		}
	}
	freeMyListIterator(it);
	return re;
}
 
void myFree(Entry * p){
    free(p);
}
 
//�ͷ�HashMap
void freeMyHashMap(MyHashMap * map) {
	myHashMapOutput(map, myFree);
	for (int i = 0; i < map->initialCapacity; i++) {
		freeMyList(map->entryList[i]);
	}
	free(map->entryList);
	free(map);
}
 
//����
void myHashMapOutput(MyHashMap *map, void(*pt)(Entry*)) {
	MyHashMapEntryIterator* iterator = createMyHashMapEntryIterator(map);
	while (myHashMapEntryIteratorHasNext(iterator)) {
		pt(myHashMapEntryIteratorNext(iterator));
	}
	freeMyHashMapEntryIterator(iterator);
}



/*************************
*** File main.c
*** test for MyHashMap
**************************/
#include <stdio.h>
#include <stdlib.h>
//#include "myEqual.h"
//#include "myHashCode.h"
//#include "myHashMap.h"
 
 
int test_hashmap()
{  
  char* strs[]=
  {
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
    int*  data = (int*)malloc(sizeof(int)* S);
    for (int i=0; i<S; i++)
    {
        data[i]=i;
    }
 
    //����ӳ����Ҫָ������������hashCode������equal������
    MyHashMap * map = createMyHashMap(myHashCodeString, myEqualString);
 
    //��������
    for (int i=0; i<S; i++)
    {
        myHashMapPutData(map, strs[i], &data[i]);
    }
 
    //�����С
    printf("size=%d\n",myHashMapGetSize(map));
 
    //����ɾ��
    myHashMapRemoveDataByKey(map,"qq");
    myHashMapRemoveDataByKey(map,"ab");
    myHashMapRemoveDataByKey(map,"qwert");
 
    //�����С
    printf("after remove size=%d\n",myHashMapGetSize(map));
 
    //����
    MyHashMapEntryIterator * it = createMyHashMapEntryIterator(map);
    while(myHashMapEntryIteratorHasNext(it))
    {
        Entry * pp= myHashMapEntryIteratorNext(it);
        char * key = (char*)pp->key;
        int* value = (int*)pp->value;
        printf("%s(%d)\n", key, *value);
    }
    //�ͷű�����
    freeMyHashMapEntryIterator(it);
 
    //�ͷ�ӳ��
    freeMyHashMap(map);
 
    //�ͷ�����
    free(data);
    return 0;
}

