#include "attribute.h"
#include "entity.h"
#include "memory.h"
//#include "collection.h"

Attribute attributeTemplate;
Attribute attributeMossySlow;

void InitAttributes()
{
	attributeTemplate.onAdd = AttributeDefaultOnAdd;
	attributeTemplate.onExtend = AttributeDefaultOnExtend;
	attributeTemplate.onUpdate = AttributeDefaultOnUpdate;
	attributeTemplate.onRender = AttributeDefaultOnRender;
	attributeTemplate.onExpire = AttributeDefaultOnExpire;
	attributeMossySlow = attributeTemplate;
}

BOOL AttributeAddOnly(World *world,Entity *entity,Attribute *attribute);
BOOL AttributeAddOrExtend(World *world,Entity *entity,Attribute *attribute)
{

}

BOOL AttributeExtendOnly(World *world,Entity *entity,Attribute *attribute)
{
	AttributeInstance *attributeInstance = AttributeGet(entity,attribute);
	if(attributeInstance == NULL)
	{
		return FALSE;
	}

}

AttributeInstance* AttributeGet(Entity *entity,Attribute *attribute)
{
	LinkedList *linkedList = entity->attributeList;
	LinkedListIterator *iterator = NULL;
	for(iterator=LinkedListGetIterator(linkedList);LinkedListIteratorHasNext(iterator);)
	{
		AttributeInstance *attributeInstance = (AttributeInstance*)LinkedListIteratorGetNext(iterator);
		if(attributeInstance->attribute==attribute)
		{
			return attributeInstance;
		}
	}
	return NULL;
}

void AttributeUpdate(World *world,Entity *entity)
{
	LinkedList *linkedList = entity->attributeList;
	LinkedListIterator *iterator = NULL;
	for(iterator=LinkedListGetIterator(linkedList);LinkedListIteratorHasNext(iterator);)
	{
		BOOL keepLive;
		AttributeInstance *attributeInstance = (AttributeInstance*)LinkedListIteratorGetNext(iterator);
		if(attributeInstance->lastLife==0)
			keepLive=FALSE;
		else
			keepLive = attributeInstance->attribute->onUpdate(world,entity,attributeInstance->lastLife,attributeInstance->elapseTime,&(attributeInstance->dataBits));
		if(!keepLive)
		{
			attributeInstance->attribute->onExpire(world,entity,attributeInstance->lastLife,attributeInstance->elapseTime,&(attributeInstance->dataBits));
			LinkedListIteratorDeleteCurrent(iterator);
			free_s(attributeInstance);
		}
		else
		{
			attributeInstance->lastLife--;
			attributeInstance->elapseTime++;
		}
	}
}
int AttributeDestroyCallback(void *attributeInstance)
{
	free_s(attributeInstance);
	return TRUE;
}

BOOL AttributeDefaultOnAdd(World *world,Entity *entity,long long *data)
{
	return TRUE;
}
BOOL AttributeDefaultOnExtend(World *world,Entity *entity,unsigned long life,unsigned long elapse,long long *data)
{
	return TRUE;
}
BOOL AttributeDefaultOnUpdate(World *world,Entity *entity,unsigned long life,unsigned long elapse,long long *data)
{
	return TRUE;
}
void AttributeDefaultOnRender(World *world,Entity *entity,unsigned long life,unsigned long elapse,long long *data)
{

}
void AttributeDefaultOnExpire(World *world,Entity *entity,unsigned long life,unsigned long elapse,long long *data)
{

}