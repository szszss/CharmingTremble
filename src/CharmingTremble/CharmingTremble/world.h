#pragma once
#include "game.h"
#include "collection.h"
#include "entity.h"

struct implWorld
{
	char *playerName;
	EntityPlayer *player;
	long seed;
	unsigned long tick;
	unsigned long long score; //�����浽40�ڷ���Щ��̫����,�����Ƕ�����ư�!���ں���4byte���ڴ���.
	LinkedList *blockList;
	LinkedList *powerupList;
	LinkedList *operateQueue;
	enum WorldType type;
	enum WorldDifficulty difficulty;
};

World* WorldNewGame(char* playerName,long seed,enum WorldType type,enum WorldDifficulty difficulty);
void WorldStart(World* world);
void WorldEnd(World* world);
void WorldUpdate(World* world);
void WorldRender(World* world);
void WorldDestory(World* world);