#include "attribute.h"
#include "entity.h"

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