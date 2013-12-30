#include "gui.h"
#include "memory.h"
#include "renderengine.h"
#include "resourcemanager.h"
#include "collection.h"

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
	static Texture* texture = NULL;
	if(texture==NULL)
		texture=RM_GetTexture("image/bgGame.png");
	RE_BindTexture(texture);
	RE_DrawRectWithTexture(0,0,1,1,0,0,800.0/1024.0,600.0/1024.0);
	RE_BindTexture(NULL);
	if(world->state==WSTATE_GAMEOVERED)
	{
		RE_DrawTextStatic("很遗憾,你♂死♂了",0.2,0.5,1);
	}
}

void GuiScreenGameClose(GuiScreen* screen)
{
	free_s(screen);
}








