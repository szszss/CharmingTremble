#include "world.h"
#include "memory.h"

extern EntityPrototype entityPrototypePlayer;

World* WorldNewGame( char* playerName,long seed,enum WorldType type,enum WorldDifficulty difficulty )
{
	World *world = (World*)malloc_s(sizeof(World));
	world->playerName=playerName;
	world->player = (EntityPlayer*)malloc_s(sizeof(EntityPlayer));
	world->player->base.prototype = &entityPrototypePlayer;
	world->score=0;
	world->seed=seed;
	world->type=type;
	world->difficulty=difficulty;
	world->powerupList=LinkedListCreate();
	world->blockList=LinkedListCreate();
	world->operateQueue=LinkedListCreate();
	return world;
}

void WorldStart(World* world)
{

}

void WorldEnd(World* world)
{

}

void WorldDestory(World* world)
{
	//free_s(world->player);
	LinkedListDestory(world->blockList,CallbackDestroyEntity);
	LinkedListDestory(world->powerupList,CallbackDestroyEntity);
	CallbackDestroyEntity((void*)world->player);
	//TODO:销毁操作队列
}

void UpdateEntityList(World* world,LinkedList *list)
{
	LinkedListIterator *iterator;
	void *entity;
	EntityPrototype* p;
	int result;
	for(iterator=LinkedListGetIterator(list);LinkedListIteratorHasNext(iterator);)
	{
		entity = LinkedListIteratorGetNext(iterator);
		p = ((Entity*)entity)->prototype;
		if((result=p->update(entity,world))<0)
		{
			p->destroy(entity,world,result);
			LinkedListIteratorDeleteCurrent(iterator);
		}
	}
}

void WorldUpdate( World* world )
{
	UpdateEntityList(world,world->blockList);
	UpdateEntityList(world,world->powerupList);
	((Entity*)(world->player))->prototype->update(world->player,world);
}

void RenderEntityList(World* world,LinkedList *list)
{
	LinkedListIterator *iterator;
	void *entity;
	EntityPrototype* p;
	for(iterator=LinkedListGetIterator(list);LinkedListIteratorHasNext(iterator);)
	{
		entity = LinkedListIteratorGetNext(iterator);
		p = ((Entity*)entity)->prototype;
		p->render(entity,world);
	}
}

void WorldRender( World* world )
{
	RenderEntityList(world,world->blockList);
	RenderEntityList(world,world->powerupList);
	((Entity*)(world->player))->prototype->render(world->player,world);
}