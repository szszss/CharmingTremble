typedef struct
{
	int posX;
	int posY;
	int width;
	int height;
	char *text;
	int flags;
} GuiButton;

enum ScreenState {SS_MAINMENU,
				  SS_GAME};

void GuiUpdate();