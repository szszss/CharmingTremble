#include "entity.h"


int CallbackDestroyEntity( void* entity )
{
	EntityPrototype* p = (EntityPrototype*)entity; //���������ANSI C�Ľṹ�����ֶ��ڴ�����ԭ��
	p->destroy(entity);
	return 0;
}
