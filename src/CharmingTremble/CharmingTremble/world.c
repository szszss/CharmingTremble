#include "world.h"
#include "memory.h"
#include "resourcemanager.h"

extern EntityPrototype entityPrototypePlayer;
extern EntityPrototype entityPrototypeBlock;

World* WorldNewGame( char* playerName,long seed,enum WorldType type,enum WorldDifficulty difficulty )
{
	World *world = (World*)malloc_s(sizeof(World));
	world->playerName=playerName;
	world->player = NULL;
	world->score=0;
	world->seed=seed;
	world->tick=0;
	world->type=type;
	world->difficulty=difficulty;
	world->upSpeed = 0.15f;
	world->powerupList=LinkedListCreate();
	world->blockList=LinkedListCreate();
	world->operateQueue=LinkedListCreate();
	world->randomGen=MTCreate(seed);
	LoggerInfo("A game world had been created");
	return world;
}

void WorldStart(World* world)
{
	EntityBlock* block = (EntityBlock*)entityPrototypeBlock.create(world,0,-1,5);
	LinkedListAdd(world->blockList,block);
	world->player = (EntityPlayer*)EntityPlayerCreate(world,0,0);
	LoggerInfo("World started");
}

void WorldEnd(World* world)
{
	LoggerInfo("World ended");
}

void WorldDestory(World* world)
{
	//free_s(world->player);
	LoggerInfo("Destroying world");
	LinkedListDestory(world->blockList,CallbackDestroyEntity);
	LinkedListDestory(world->powerupList,CallbackDestroyEntity);
	CallbackDestroyEntity((void*)world->player);
	MTDestroy(world->randomGen);
	free_s(world);
	LoggerInfo("World destroyed");
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
	if(world->tick%20==0)
	{
		int count = MTNextInt(world->randomGen,0,1);
		if(count==1)
		{
			int x = MTNextInt(world->randomGen,0,19);
			int length = MTNextInt(world->randomGen,3,10);
			EntityBlock* block = (EntityBlock*)entityPrototypeBlock.create(world,(float)x-9.5f,-16,5);
			LinkedListAdd(world->blockList,block);
		}
	}
	UpdateEntityList(world,world->blockList);
	UpdateEntityList(world,world->powerupList);
	((Entity*)(world->player))->prototype->update(world->player,world);
	world->tick++;
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