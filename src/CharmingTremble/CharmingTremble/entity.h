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
	float posX;
	float posY;
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

int InitEntities();

void EntityDestroy(void* entity,World* world,int cause);
int EntityPlayerUpdate(void* entity,World* world);
void EntityPlayerRender(void* entity,World* world);

int CallbackDestroyEntity(void* entity);