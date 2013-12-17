#include "world.h"
#include "memory.h"
#include "resourcemanager.h"

extern EntityPrototype entityPrototypePlayer;
extern EntityPrototype entityPrototypeBlock;

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
	world->upSpeed = 0.01f;
	world->powerupList=LinkedListCreate();
	world->blockList=LinkedListCreate();
	world->operateQueue=LinkedListCreate();
	return world;
}

void WorldStart(World* world)
{
	Texture *texture = NULL;
	EntityBlock* block = (EntityBlock*)malloc_s(sizeof(EntityBlock));
	texture=RM_GetTexture("image/brick.png");
	block->width=5;
	block->base.posX=-5;
	block->base.posY=-10;
	block->texture=texture;
	block->base.prototype=&entityPrototypeBlock;
	LinkedListAdd(world->blockList,block);
	block = (EntityBlock*)malloc_s(sizeof(EntityBlock));
	block->width=7;
	block->base.posX=4;
	block->base.posY=2;
	block->texture=texture;
	block->base.prototype=&entityPrototypeBlock;
	LinkedListAdd(world->blockList,block);
	block = (EntityBlock*)malloc_s(sizeof(EntityBlock));
	block->width=1;
	block->base.posX=9;
	block->base.posY=8;
	block->texture=texture;
	block->base.prototype=&entityPrototypeBlock;
	LinkedListAdd(world->blockList,block);
	block = (EntityBlock*)malloc_s(sizeof(EntityBlock));
	block->width=20;
	block->base.posX=0.5;
	block->base.posY=-3;
	block->texture=texture;
	block->base.prototype=&entityPrototypeBlock;
	LinkedListAdd(world->blockList,block);
	block = (EntityBlock*)malloc_s(sizeof(EntityBlock));
	block->width=7;
	block->base.posX=-4;
	block->base.posY=5;
	block->texture=texture;
	block->base.prototype=&entityPrototypeBlock;
	LinkedListAdd(world->blockList,block);
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