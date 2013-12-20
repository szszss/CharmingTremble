#include "gui.h"
#include "game.h"

enum ScreenState screenState;

void GuiUpdate()
{
	switch(screenState)
	{
	case SS_GAME:

		break;
	default:
		GameCrash("Unknown screen state.");
		break;
	}
}

