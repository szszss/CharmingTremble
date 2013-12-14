#include "collection.h"
#include "memory.h"


LinkedList* LinkedListCreate()
{
	LinkedList *linkedList = (LinkedList*)malloc_s(sizeof(LinkedList));
	_LinkedListNode *head = (_LinkedListNode*)malloc_s(sizeof(_LinkedListNode));
	LinkedListIterator *iterator = (LinkedListIterator *)malloc_s(sizeof(LinkedListIterator));
	head->value=(void*)iterator;
	linkedList->headNode=head;
	linkedList->lastNode=head;
	linkedList->length=0;
	return linkedList;
}

unsigned long LinkedListAdd(LinkedList* linkedList,void* value)
{
	_LinkedListNode *node = (_LinkedListNode*)malloc_s(sizeof(_LinkedListNode));
	node->value=value;
	linkedList->lastNode->next=node;
	node->last=linkedList->lastNode;
	linkedList->lastNode=node;
	node->next=NULL;
	return linkedList->length++;
}

unsigned long LinkedListInsert(LinkedList* linkedList,unsigned long index,void* value)
{
	int result;
	_LinkedListNode *node=NULL;
	_LinkedListNode *insertedNode = (_LinkedListNode*)LinkedListGet(linkedList,index,&result);
	if(result)
		return LinkedListAdd(linkedList,value);
	node = (_LinkedListNode*)malloc_s(sizeof(_LinkedListNode));
	node->value=value;
	node->next=insertedNode;
	insertedNode->last->next=node;
	node->last=insertedNode->last;
	insertedNode->last=node;
	linkedList->length++;
	return index;
}
void* LinkedListGet(LinkedList* linkedList,unsigned long index,int* result)
{
	//XXX:其实可以判断index与length的关系,然后选择性地做倒序搜索.
	_LinkedListNode *node;
	if(index>=linkedList->length)
	{
		if(result!=NULL)
			*result = 1;
		return NULL;
	}
	node=linkedList->headNode;
	for(;index>=0;index--) //因为headNode是不使用的,所以是>=
	{
		node=node->next;
	}
	if(result!=NULL)
		*result = 0;
	return node->value;
}

void* LinkedListRemoveNode(LinkedList *linkedList,_LinkedListNode *node)
{
	void* v;
	_LinkedListNode *temp;
	v=node->value;
	if(linkedList->lastNode==node)
	{
		linkedList->lastNode=node->last;
		node->last->next=NULL;
	}
	else
	{
		node->last->next=node->next;
		node->next->last=node->last;
	}
	free_s(node);
	return v;
}

void* LinkedListRemove(LinkedList* linkedList,void* value,int* result)
{
	_LinkedListNode *node=linkedList->headNode;
	while((node=node->next)!=NULL)
	{
		if(node->value==value)
		{
			if(result!=NULL)
				*result=0;
			return LinkedListRemoveNode(linkedList,node);;
		}
	}
	if(result!=NULL)
		*result=1;
	return NULL;
}

void* LinkedListRemoveByIndex(LinkedList* linkedList,unsigned long index,int* result)
{
	int r;
	_LinkedListNode *node=(_LinkedListNode*)LinkedListGet(linkedList,index,&r);
	if(r)
	{
		if(result!=NULL)
			*result=1;
		return NULL;
	}
	if(result!=NULL)
		*result=0;
	return LinkedListRemoveNode(linkedList,node);
}

void* LinkedListPop(LinkedList* linkedList,int* result)
{
	if(linkedList->length>0)
	{
		if(result!=NULL)
			*result=0;
		return LinkedListRemoveNode(linkedList,linkedList->lastNode);
	}
	else
	{
		if(result!=NULL)
			*result=1;
		return NULL;
	}
}

unsigned long LinkedListDestory(LinkedList* linkedList,int (*callbackFunction)(void* ))
{
	_LinkedListNode *deleted = NULL;
	_LinkedListNode *node = linkedList->headNode->next;
	while(node!=NULL)
	{
		if(callbackFunction(node->value))
		{
			linkedList->headNode->next=node;
			node->last=linkedList->headNode;
			return linkedList->length;
		}
		else
		{
			deleted=node;
			node=node->next;
			free_s(deleted);
			linkedList->length--;
		}
	}
	free_s(linkedList->headNode->value);
	free_s(linkedList->headNode);
	free_s(linkedList);
	return 0;
}