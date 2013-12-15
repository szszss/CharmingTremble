#pragma once
#include "game.h"
#include "world.h"

struct implEntityPrototype 
{
	int (*update)(void*,World*);
	void (*render)(void*,World*);
	void (*destroy)(void*,World*,int);
};

struct implEntity 
{
	EntityPrototype* prototype;
	int posX;
	int posY;
};

struct implEntityBlock 
{
	Entity base;
	int width;
};

struct implEntityPlayer
{
	Entity base;
	int life;
};

int CallbackDestroyEntity(void* entity);
void EntityDestroy(void* entity,World* world,int cause);
int EntityPlayerUpdate(void* entity,World* world);
void EntityPlayerRender(void* entity,World* world);