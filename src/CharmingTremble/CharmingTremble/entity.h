#pragma once
#include "game.h"
#include "world.h"

//#define FOREACH_PLAYERS FOREACH_PLAYERS(player)
#define FOREACH_PLAYERS(player) {EntityPlayer *player = NULL;int _loopVar_=0;while((player=world->players[_loopVar_++])!=NULL){
#define FOREACH_END     }}

struct implEntityPrototype 
{
	void* (*create)(World*,float,float,...);
	int (*update)(void*,World*);
	void (*render)(void*,World*);
	void (*destroy)(void*,World*,int);
};

struct implEntity 
{
	EntityPrototype* prototype;
	float posX;
	float posY;
};

struct implEntityBlock 
{
	Entity base;
	byte width;
	long depthLevel;
	unsigned long stepped;
	Texture *texture;
};

struct implEntityPlayer
{
	Entity base;
	byte id;
	int life;
	long long score; //尽管玩到40亿分有些不太可能,但还是多多益善吧!别在乎那4byte的内存了.
	float vSpeed;
	long maxDepthLevel;
	BOOL left;
	BOOL right;
	BOOL up;
	BOOL down;
	BOOL jump;
	BOOL landed;
};

int InitEntities();

void EntityDestroy(void* entity,World* world,int cause);
/*额外的附加参数:(byte)id - 玩家ID*/
void* EntityPlayerCreate(World* world,float x,float y,...);
int EntityPlayerUpdate(void* entity,World* world);
void EntityPlayerRender(void* entity,World* world);
int EntityPlayerLifeChange(void* entity,World* world,int value);
/*额外的附加参数:(byte)width - 宽度,(uint32)depth - 深度*/
void* EntityBlockCreate(World* world,float x,float y,...);
int EntityBlockUpdate(void* entity,World* world);
void EntityBlockRender(void* entity,World* world);

int CallbackDestroyEntity(void* entity);