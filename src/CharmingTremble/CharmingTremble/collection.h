typedef struct {
	struct _LinkedListNode *headNode;
	struct _LinkedListNode *lastNode;
	unsigned long length;
}LinkedList;

typedef struct {
	struct _LinkedListNode *currentNode;
}LinkedListIterator;

typedef struct _LinkedListNode
{
	struct _LinkedListNode *last;
	struct _LinkedListNode *next;
	void *value;
}_LinkedListNode;

LinkedList* LinkedListCreate();
unsigned long LinkedListAdd(LinkedList* linkedList,void* value);
unsigned long LinkedListInsert(LinkedList* linkedList,unsigned long index,void* value);
void* LinkedListGet(LinkedList* linkedList,unsigned long index,int* result);
void* LinkedListRemove(LinkedList* linkedList,void* value,int* result);
void* LinkedListRemoveByIndex(LinkedList* linkedList,unsigned long index,int* result);
void* LinkedListPop(LinkedList* linkedList,int* result);
unsigned long LinkedListDestory(LinkedList* linkedList,int (*callbackFunction)(void* ));