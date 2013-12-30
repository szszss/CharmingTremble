#pragma once
#include "game.h"
#include "world.h"

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
	int width;
	Texture *texture;
};

struct implEntityPlayer
{
	Entity base;
	int life;
	float vSpeed;
	BOOL left;
	BOOL right;
	BOOL up;
	BOOL down;
	BOOL jump;
	BOOL landed;
};

int InitEntities();

void EntityDestroy(void* entity,World* world,int cause);
void* EntityPlayerCreate(World* world,float x,float y,...);
int EntityPlayerUpdate(void* entity,World* world);
void EntityPlayerRender(void* entity,World* world);
int EntityPlayerLifeChange(void* entity,World* world,int value);
void* EntityBlockCreate(World* world,float x,float y,...);
int EntityBlockUpdate(void* entity,World* world);
void EntityBlockRender(void* entity,World* world);

int CallbackDestroyEntity(void* entity);