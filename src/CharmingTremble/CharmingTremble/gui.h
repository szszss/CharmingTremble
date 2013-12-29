#include "game.h"

struct implGuiButton
{
	int posX;
	int posY;
	int width;
	int height;
	char *text;
	int flags;
};

void GuiUpdate();