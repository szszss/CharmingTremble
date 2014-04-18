#include "gui.h"
#include "memory.h"
#include "renderengine.h"
#include "resourcemanager.h"
#include "collection.h"
#include "SDL_opengl.h" //可去除
#include <stdlib.h> //可去除

static GuiScreen *currentScreen = NULL;
extern World* theWorld;

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

void Gui_MouseDown(long x,long y)
{
	LinkedListIterator *iterator;
	if(currentScreen==NULL)
		return;
	for(iterator = LinkedListGetIterator(currentScreen->controlList);LinkedListIteratorHasNext(iterator);)
	{
		GuiButton *button = (GuiButton*)LinkedListIteratorGetNext(iterator);
		if(x>=button->posX&&x<=(button->posX+button->width)&&y>=button->posY&&y<=(button->posY+button->height)&&button->flags!=2)
		{
			button->flags=1;
		}
	}
}

GuiButton* Gui_InitButton(int x, int y,int w,int h,char* text,int flags,void (*press)(GuiButton*,GuiScreen*,World*),void (*update)(GuiButton*,GuiScreen*,World*))
{
	GuiButton* button = (GuiButton*)malloc_s(sizeof(GuiButton));
	button->posX=x;
	button->posY=y;
	button->width=w;
	button->height=h;
	button->text=text;
	button->press=press;
	button->update=update;
	button->flags=flags;
}

void Gui_AddButton(GuiScreen* screen,GuiButton* button)
{
	LinkedListAdd(screen->controlList,button);
}

void Gui_RenderButton(GuiButton* button,GuiScreen* screen,World* world)
{
	//float x = button->flags==1?button->posX+0.01f:button->posX;
	//float y = button->flags==1?button->posY+0.01f:button->posY;
	RE_BindTexture(NULL);
	glColor3f(0.2,0.2,0.2);
	RE_DrawRectWithTexture(button->posX/WINDOW_WIDTH_FLOAT,button->posY/WINDOW_HEIGHT_FLOAT,
		button->width/WINDOW_WIDTH_FLOAT,button->height/WINDOW_HEIGHT_FLOAT,0,0,1,1);
	if(button->flags==2)
		glColor3f(0.3,0.3,0.3);
	else
		glColor3f(1,1,1);
	RE_DrawRectWithTexture(button->posX/WINDOW_WIDTH_FLOAT+0.003f,button->posY/WINDOW_HEIGHT_FLOAT+0.003f,
		button->width/WINDOW_WIDTH_FLOAT-0.006f,button->height/WINDOW_HEIGHT_FLOAT-0.006f,0,0,1,1);
	glColor3f(0,0,0);
	RE_DrawTextStatic(button->text, button->posX/WINDOW_WIDTH_FLOAT+0.02f, button->posY/WINDOW_HEIGHT_FLOAT+0.02f, button->width/WINDOW_WIDTH_FLOAT);
	glColor3f(1,1,1);
}

GuiScreen* GuiScreenGame()
{
	GuiScreen *screen = (GuiScreen*)malloc_s(sizeof(GuiScreen));
	GuiButton *btnStart = Gui_InitButton(500,400,70,40,"开始",0,GuiButtonStartPress,GuiButtonStartUpdate);
	GuiButton *btnStop = Gui_InitButton(600,400,70,40,"结束",0,GuiButtonStopPress,GuiButtonStopUpdate);
	GuiButton *btnRestart = Gui_InitButton(600,450,70,40,"重开",0,GuiButtonRestartPress,GuiButtonRestartUpdate);
	GuiButton *btnPause = Gui_InitButton(500,450,70,40,"暂停",0,GuiButtonPausePress,GuiButtonPauseUpdate);
	screen->update=GuiScreenGameUpdate;
	screen->render=GuiScreenGameRender;
	screen->close=GuiScreenGameClose;
	screen->controlList=LinkedListCreate();
	Gui_AddButton(screen,btnStart);
	Gui_AddButton(screen,btnStop);
	Gui_AddButton(screen,btnRestart);
	Gui_AddButton(screen,btnPause);
	return screen;
}

void GuiScreenGameUpdate( GuiScreen* screen,World* world )
{
	LinkedListIterator *iterator;
	for(iterator = LinkedListGetIterator(screen->controlList);LinkedListIteratorHasNext(iterator);)
	{
		GuiButton *button = (GuiButton*)LinkedListIteratorGetNext(iterator);
		if(button->flags==1)
		{
			button->flags=0;
			button->press(button,screen,world);
		}
		button->update(button,screen,world);
	}
}

void GuiScreenGameRender(GuiScreen* screen,World* world)
{
	char* life = (char*)malloc_s(8*sizeof(char));
	char* score = (char*)malloc_s(24*sizeof(char));
	char* level = (char*)malloc_s(8*sizeof(char));
	LinkedListIterator *iterator;
	static Texture* texture = NULL;
	if(texture==NULL)
		texture=RM_GetTexture("image/bgGame.png");
	glColor3f(1,1,1);
	RE_BindTexture(texture);
	RE_DrawRectWithTexture(0,0,1,1,0,0,800.0/1024.0,600.0/1024.0);
	RE_BindTexture(NULL);
	if(world!=NULL)
	{
		sprintf(life,"%d",world->players[0]->life);
		sprintf(score,"%d",world->players[0]->score);
		sprintf(level,"%d",world->players[0]->maxDepthLevel);
		glColor3f(1,0,0);
		RE_DrawTextStatic("Life:", 0.65f, 0.1f, 1.0f);
		RE_DrawTextVolatile(life, 0.65f, 0.14f, 1.0f);
		RE_DrawTextStatic("Score:", 0.65f, 0.18f, 1.0f);
		RE_DrawTextVolatile(score, 0.65f, 0.22f, 1.0f);
		RE_DrawTextStatic("Level:", 0.65f, 0.26f, 1.0f);
		RE_DrawTextVolatile(level, 0.65f, 0.3f, 1.0f);
		if(world->state==WSTATE_GAMEOVERED)
		{
			RE_DrawTextStatic("很遗憾,你♂死♂了",0.2,0.5,1);
		}
	}
	
	glColor3f(1,1,1);
	free_s(life);
	free_s(score);
	free_s(level);
	for(iterator = LinkedListGetIterator(screen->controlList);LinkedListIteratorHasNext(iterator);)
	{
		GuiButton *button = (GuiButton*)LinkedListIteratorGetNext(iterator);
		Gui_RenderButton(button,screen,world);
	}
}

int CallbackGuiButtonDestroy(void *button)
{
	free_s(button);
	return 0;
}

void GuiScreenGameClose(GuiScreen* screen)
{
	LinkedListDestory(screen->controlList,CallbackGuiButtonDestroy);
	free_s(screen);
}

void GuiButtonStartPress(GuiButton* button,GuiScreen* screen,World* world)
{
	theWorld = WorldNewGame("szszss",1000,TYPE_NORMAL,DIFF_NORMAL);
	WorldStart(theWorld);
}

void GuiButtonPausePress(GuiButton* button,GuiScreen* screen,World* world)
{
	GameSetPause(!GameGetPause());
}

void GuiButtonStopPress(GuiButton* button,GuiScreen* screen,World* world)
{
	//GameSafelyKillWorld();
	WorldEnd(theWorld);
	WorldDestory(theWorld);
	theWorld=NULL;
}

void GuiButtonRestartPress(GuiButton* button,GuiScreen* screen,World* world)
{
	WorldEnd(theWorld);
	WorldStart(theWorld);
}

void GuiButtonStartUpdate(GuiButton* button,GuiScreen* screen,World* world)
{
	if(theWorld!=NULL)
		button->flags=2;
	else
		button->flags=0;
}

void GuiButtonPauseUpdate(GuiButton* button,GuiScreen* screen,World* world)
{
	if(theWorld==NULL)
		button->flags=2;
	else
		button->flags=0;
}

void GuiButtonStopUpdate(GuiButton* button,GuiScreen* screen,World* world)
{
	if(theWorld==NULL)
		button->flags=2;
	else
		button->flags=0;
}

void GuiButtonRestartUpdate(GuiButton* button,GuiScreen* screen,World* world)
{
	if(theWorld==NULL)
		button->flags=2;
	else
		button->flags=0;
}








