#include "entity.h"


int CallbackDestroyEntity( void* entity )
{
	EntityPrototype* p = (EntityPrototype*)entity; //这个利用了ANSI C的结构体首字段内存对齐的原理
	p->destroy(entity);
	return 0;
}
