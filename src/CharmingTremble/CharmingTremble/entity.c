#include "entity.h"
#include "memory.h"
#include "renderengine.h"
#include "resourcemanager.h"
#include "SDL_opengl.h"
#include "util.h"
#include "input.h"
#include <stdarg.h>

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

void* EntityPlayerCreate(World *world,float x,float y, ...)
{
	EntityPlayer *player = (EntityPlayer*)malloc_s(sizeof(EntityPlayer));
	player->base.posX=x;
	player->base.posY=y;
	player->base.prototype=&entityPrototypePlayer;
	player->left=FALSE;
	player->right=FALSE;
	player->up=FALSE;
	player->down=FALSE;
	player->jump=FALSE;
	player->landed=FALSE;
	player->vSpeed=0;
	player->life=5;
	return player;
}

int EntityPlayerUpdate(void* entity,World* world)
{
	unsigned char operate;
	EntityPlayer *player = (EntityPlayer*)entity;
	char hTempMove = 0;
	while((operate=IN_GetOperate())>200)
	{
		switch(operate)
		{
		case INPUT_OPERATE_LEFT_DOWN:
			hTempMove=(!hTempMove&&!player->left)?-1:0;
			player->left=TRUE;
			break;
		case INPUT_OPERATE_RIGHT_DOWN:
			hTempMove=(!hTempMove&&!player->right)?1:0;
			player->right=TRUE;
			break;
		case INPUT_OPERATE_UP_DOWN:
			player->up=TRUE;
			break;
		case INPUT_OPERATE_DOWN_DOWN:
			player->down=TRUE;
			break;
		case INPUT_OPERATE_SPACE_DOWN:
			player->jump=TRUE;
			break;
		case INPUT_OPERATE_LEFT_UP:
			player->left=FALSE;
			break;
		case INPUT_OPERATE_RIGHT_UP:
			player->right=FALSE;
			break;
		case INPUT_OPERATE_UP_UP:
			player->up=FALSE;
			break;
		case INPUT_OPERATE_DOWN_UP:
			player->down=FALSE;
			break;
		case INPUT_OPERATE_SPACE_UP:
			player->jump=FALSE;
			break;
		}
	}
	if(player->left||hTempMove<0)
	{
		player->base.posX-=0.2f;
	}
	else if(player->right||hTempMove>0)
	{
		player->base.posX+=0.2f;
	}
	if(player->landed)
	{
		//LoggerDebug("wwww");
		player->vSpeed=0.0f;
		if(player->jump)
		{
			player->vSpeed+=0.8f;
		}
	}
	else
	{
		player->base.posY+=player->vSpeed;
		player->vSpeed-=0.05f;
		
	}
	if(player->base.posY<-15)
	{
		player->vSpeed = 1.5f;
	}
	if(player->vSpeed<-1.0f)
	{
		player->vSpeed=-1.0f;
	}
	player->landed=FALSE;
	return 0;
}

void EntityPlayerRender(void* entity,World* world)
{
	static Texture *texture = NULL;
	Entity *player = (Entity*)entity;
	//float x = player->posX;
	//float z = player->posY;
	if(texture==NULL)
	{
		texture=RM_GetTexture("image/wood.png");
	}
	glPushMatrix();
	//glTranslatef(-4,3+(float)(tickTime)/10.0f,0);
	RE_BindTexture(texture);
	glTranslatef(player->posX,player->posY,0);
	RE_RenderCube(-0.5,2,-0.5,0.5,0,0.5);
	RE_BindTexture(NULL);
	glPopMatrix();
}

void* EntityBlockCreate(World *world,float x,float y, ...)
{
	va_list args;
	int width;
	EntityBlock *block = (EntityBlock*)malloc_s(sizeof(EntityBlock));
	va_start(args,y);
	block->base.posX=x;
	block->base.posY=y;
	block->base.prototype=&entityPrototypeBlock;
	block->texture=RM_GetTexture("image/brick.png");
	width=va_arg(args, int);
	block->width=width;
	va_end(args); 
	return block;
}

int EntityBlockUpdate(void* entity,World* world)
{
	EntityBlock *block = (EntityBlock*)entity;
	float widthLeft,widthRight,temp;
	block->base.posY+=world->upSpeed;
	if(block->base.posY>20)
	{
		return -1;
	}
	temp=(float)(block->width)/2;
	widthLeft=block->base.posX-temp;
	widthRight=block->base.posX+temp;
	if(world->player->base.posX>(widthLeft-0.2f) && world->player->base.posX<(widthRight+0.2f))
	{
		//LoggerDebug("yaya");
		if((world->player->base.posY > block->base.posY-1.0f) && (world->player->base.posY - block->base.posY < 0.7f) && (world->player->vSpeed<=0))
		{
			//LoggerDebug("yyyyy");
			world->player->landed=TRUE;
			world->player->base.posY = block->base.posY+0.5f;
			//world->player->vSpeed=0;
		}
		else if(world->player->base.posY > block->base.posY-2.5f)
		{
			//LoggerDebug("hehe");
			/*world->player->base.posY = block->base.posY-2.5f;
			if(world->player->base.posX>(widthLeft-0.4f))
				world->player->base.posX = widthLeft-0.4f;
			else if(world->player->base.posX<(widthRight+0.4f))
				world->player->base.posX = widthRight+0.4f;*/
		}
	}
	return 0;
}

void EntityBlockRender(void* entity,World* world)
{
	EntityBlock *block = (EntityBlock*)entity;
	int width = block->width;
	//float fWidth = (float)width/2.0f;;
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
	entityPrototypePlayer.create = EntityPlayerCreate;
	entityPrototypePlayer.update = EntityPlayerUpdate;
	entityPrototypePlayer.render = EntityPlayerRender;
	entityPrototypePlayer.destroy = EntityDestroy;
	entityPrototypeBlock.create = EntityBlockCreate;
	entityPrototypeBlock.update = EntityBlockUpdate;
	entityPrototypeBlock.render = EntityBlockRender;
	entityPrototypeBlock.destroy = EntityDestroy;
	LoggerInfo("Entities initialized");
	return 0;
}
