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
};

void Gui_Update(World* world);
void Gui_Render(World* world);
void Gui_Destroy();
void Gui_Open(GuiScreen* (*constructor)());
BOOL Gui_Close();

GuiScreen* GuiScreenGame();
void GuiScreenGameUpdate(GuiScreen* screen,World* world);
void GuiScreenGameRender(GuiScreen* screen,World* world);
void GuiScreenGameClose(GuiScreen* screen);