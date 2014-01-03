#include "gui.h"
#include "memory.h"
#include "renderengine.h"
#include "resourcemanager.h"
#include "collection.h"
#include "SDL_opengl.h" //可去除
#include <stdlib.h> //可去除

static GuiScreen *currentScreen = NULL;

void Gui_Update(World* world)
{
	if(currentScreen==NULL)
		return;
	currentScreen->update(currentScreen,world);
}

void Gui_Render(World* world)
{
	if(currentScreen==NULL)
		return;
	currentScreen->render(currentScreen,world);
}

void Gui_Destroy()
{
	while(Gui_Close()){};
}

void Gui_Open( GuiScreen* (*constructor)() )
{
	GuiScreen *screen = constructor();
	screen->parent=currentScreen;
	currentScreen=screen;
}

BOOL Gui_Close()
{
	GuiScreen *screen;
	if(currentScreen==NULL)
		return FALSE;
	screen = currentScreen->parent;
	currentScreen->close(currentScreen);
	currentScreen=screen;
	return TRUE;
}

GuiScreen* GuiScreenGame()
{
	GuiScreen *screen = (GuiScreen*)malloc_s(sizeof(GuiScreen));
	screen->update=GuiScreenGameUpdate;
	screen->render=GuiScreenGameRender;
	screen->close=GuiScreenGameClose;
	//screen->controlList=LinkedListCreate();
	return screen;
}

void GuiScreenGameUpdate( GuiScreen* screen,World* world )
{

}

void GuiScreenGameRender(GuiScreen* screen,World* world)
{
	char* life = (char*)malloc_s(8*sizeof(char));
	char* score = (char*)malloc_s(24*sizeof(char));
	static Texture* texture = NULL;
	if(texture==NULL)
		texture=RM_GetTexture("image/bgGame.png");
	RE_BindTexture(texture);
	RE_DrawRectWithTexture(0,0,1,1,0,0,800.0/1024.0,600.0/1024.0);
	RE_BindTexture(NULL);
	sprintf(life,"%d",world->players[0]->life);
	sprintf(score,"%d",world->players[0]->score);
	glColor3f(1,0,0);
	RE_DrawTextStatic("Life:",0.65,0.1,1);
	RE_DrawTextVolatile(life,0.65,0.14,1);
	RE_DrawTextStatic("Score:",0.65,0.18,1);
	RE_DrawTextVolatile(score,0.65,0.22,1);
	glColor3f(1,1,1);
	free_s(life);
	free_s(score);
	if(world->state==WSTATE_GAMEOVERED)
	{
		RE_DrawTextStatic("很遗憾,你♂死♂了",0.2,0.5,1);
	}
}

void GuiScreenGameClose(GuiScreen* screen)
{
	free_s(screen);
}








