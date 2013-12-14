#include "world.h"
#include "memory.h"

World* WorldNewGame( char* playerName,long seed,enum WorldType type,enum WorldDifficulty difficulty )
{
	World *world = (World*)malloc_s(sizeof(World));
	world->playerName=playerName;
	world->player = (EntityPlayer*)malloc_s(sizeof(EntityPlayer));
	world->score=0;
	world->seed=seed;
	world->type=type;
	world->difficulty=difficulty;
	//world->powerupList=LinkedListCreate();
	world->blockList=LinkedListCreate();
	world->operateStack=LinkedListCreate();
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
	free_s(world->player);
	LinkedListDestory(world->blockList,CallbackDestroyEntity);
}

void WorldUpdate( World* world )
{

}
