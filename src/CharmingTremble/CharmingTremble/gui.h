#pragma once
#include "game.h"
#include "world.h"
#include "collection.h"

typedef struct implGuiScreen
{
	void (*update)(GuiScreen*,World*);
	void (*render)(GuiScreen*,World*);
	void (*close)(GuiScreen*);
	LinkedList *controlList;
	GuiScreen *parent;
};

struct implGuiButton
{
	int posX;
	int posY;
	int width;
	int height;
	char *text;
	int flags;
	void (*press)(GuiButton*,GuiScreen*,World*);
	void (*update)(GuiButton*,GuiScreen*,World*);
};

void Gui_Update(World* world);
void Gui_Render(World* world);
void Gui_Destroy();
void Gui_Open(GuiScreen* (*constructor)());
void Gui_MouseDown(long x,long y);
GuiButton* Gui_InitButton(int x, int y,int w,int h,char* text,int flags,void (*press)(GuiButton*,GuiScreen*,World*),void (*update)(GuiButton*,GuiScreen*,World*));
void Gui_AddButton(GuiScreen* screen,GuiButton* button);
void Gui_RenderButton(GuiButton* button,GuiScreen* screen,World* world);
BOOL Gui_Close();

GuiScreen* GuiScreenGame();
void GuiScreenGameUpdate(GuiScreen* screen,World* world);
void GuiScreenGameRender(GuiScreen* screen,World* world);
void GuiScreenGameClose(GuiScreen* screen);

void GuiButtonStartPress(GuiButton* button,GuiScreen* screen,World* world);
void GuiButtonPausePress(GuiButton* button,GuiScreen* screen,World* world);
void GuiButtonStopPress(GuiButton* button,GuiScreen* screen,World* world);
void GuiButtonRestartPress(GuiButton* button,GuiScreen* screen,World* world);
void GuiButtonStartUpdate(GuiButton* button,GuiScreen* screen,World* world);
void GuiButtonPauseUpdate(GuiButton* button,GuiScreen* screen,World* world);
void GuiButtonStopUpdate(GuiButton* button,GuiScreen* screen,World* world);
void GuiButtonRestartUpdate(GuiButton* button,GuiScreen* screen,World* world);