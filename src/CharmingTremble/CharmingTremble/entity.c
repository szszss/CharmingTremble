#include "entity.h"
#include "memory.h"

extern World *theWorld;

int CallbackDestroyEntity( void* entity )
{
	EntityPrototype* p = ((Entity*)entity)->prototype;
	p->destroy(entity,theWorld,0);
	return 0;
}

void EntityDestroy(void* entity,World* world,int cause)
{
	free_s(entity);
}

int EntityPlayerUpdate(void* entity,World* world)
{
	return 0;
}

void EntityPlayerRender(void* entity,World* world)
{

}