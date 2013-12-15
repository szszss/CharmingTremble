#include "entity.h"
#include "memory.h"
#include "renderengine.h"
#include "SDL_opengl.h"
#include "util.h"

EntityPrototype entityPrototypePlayer;
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
	Entity* player = (Entity*)entity;
	//float x = player->posX;
	//float z = player->posY;
	glPushMatrix();
	//glTranslatef(-1,1,-5);
	RE_DrawCube(0,0,0,1,-1,1);
	glPopMatrix();
}

int InitEntities()
{
	entityPrototypePlayer.update = EntityPlayerUpdate;
	entityPrototypePlayer.render = EntityPlayerRender;
	entityPrototypePlayer.destroy = EntityDestroy;
	LoggerInfo("Entities initialized");
	return 0;
}
