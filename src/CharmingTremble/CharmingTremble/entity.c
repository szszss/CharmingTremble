#include "entity.h"
#include "memory.h"
#include "renderengine.h"
#include "resourcemanager.h"
#include "SDL_opengl.h"
#include "util.h"

extern unsigned long long tickTime;

EntityPrototype entityPrototypePlayer;
EntityPrototype entityPrototypeBlock;
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
	/*static Texture *texture = NULL;
	Entity *player = (Entity*)entity;
	//float x = player->posX;
	//float z = player->posY;
	if(texture==NULL)
	{
		texture=RM_GetTexture("image/brick.png");
	}
	glPushMatrix();
	//glTranslatef(-4,3+(float)(tickTime)/10.0f,0);
	RE_BindTexture(texture);
	RE_RenderCube(0,0,0,3,-3,3);
	RE_BindTexture(NULL);
	glPopMatrix();*/
}

int EntityBlockUpdate(void* entity,World* world)
{
	EntityBlock *block = (EntityBlock*)entity;
	block->base.posY+=world->upSpeed;
	if(block->base.posY>20)
	{
		return -1;
	}
	return 0;
}

void EntityBlockRender(void* entity,World* world)
{
	EntityBlock *block = (EntityBlock*)entity;
	int width = block->width;
	float fWidth = (float)width/2.0f;;
	if(width<1)
	{
		LoggerDebug("A block has a wrong width:%d It won't be rendered",width);
		return;
	}
	glPushMatrix();

	RE_BindTexture(block->texture);
	glTranslatef(block->base.posX,block->base.posY,0);
	RE_RenderCubeQuick(width);
	RE_BindTexture(NULL);
	glPopMatrix();
}

int InitEntities()
{
	entityPrototypePlayer.update = EntityPlayerUpdate;
	entityPrototypePlayer.render = EntityPlayerRender;
	entityPrototypePlayer.destroy = EntityDestroy;
	entityPrototypeBlock.update = EntityBlockUpdate;
	entityPrototypeBlock.render = EntityBlockRender;
	entityPrototypeBlock.destroy = EntityDestroy;
	LoggerInfo("Entities initialized");
	return 0;
}
