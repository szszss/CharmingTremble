#include "game.h"

#define LinkedListPush(linkedList,value) LinkedListAdd(linkedList,value)

struct implLinkedList{
	_LinkedListNode *headNode;
	_LinkedListNode *lastNode;
	unsigned long length;
};

struct implLinkedListIterator{
	LinkedList *host;
	_LinkedListNode *currentNode;
	_LinkedListNode *nextNode;
};

struct impl_LinkedListNode
{
	_LinkedListNode *last;
	_LinkedListNode *next;
	void *value;
};

LinkedList* LinkedListCreate();
unsigned long LinkedListAdd(LinkedList* linkedList,void* value);
unsigned long LinkedListInsert(LinkedList* linkedList,unsigned long index,void* value);
void* LinkedListGet(LinkedList* linkedList,unsigned long index,int* result);
void* LinkedListRemove(LinkedList* linkedList,void* value,int* result);
void* LinkedListRemoveByIndex(LinkedList* linkedList,unsigned long index,int* result);
void* LinkedListPop(LinkedList* linkedList,int* result);
unsigned long LinkedListDestory(LinkedList* linkedList,int (*callbackFunction)(void* ));
LinkedListIterator* LinkedListGetIterator(LinkedList* linkedList);
void* LinkedListIteratorGetNext(LinkedListIterator* iterator);
int LinkedListIteratorHasNext(LinkedListIterator* iterator);
void* LinkedListIteratorDeleteCurrent(LinkedListIterator* iterator);