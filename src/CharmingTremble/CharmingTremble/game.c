#include "game.h"
#include <stdlib.h>
#include "renderengine.h"
#include "resourcemanager.h"
#include "SDL.h"
#include "util.h"
#include "entity.h"
#include "attribute.h"
#include "world.h"
#include "input.h"
#include "math.h"
#include "oswork.h"
#include "gui.h"
#include "pmd.h"

void GameClose();
void GameMainLoop();
int Update();
int HandleEvent(SDL_Event sdlEvent);

static int shouldRun = 1;
static BOOL shouldKillWorld = FALSE;
unsigned long long tickTime = 0;
World* theWorld = NULL;
static BOOL gamePause = FALSE;

#define QUICK_START

int main(int argc, char** argv)
{
	//int result;
	OS_Init();
	LoggerCreate(TRUE,"log.txt",LOGGER_APPEND,LOGGER_LEVEL_ALL,LOGGER_FORMAT_C);
	LoggerInfo("Initializing game");
	if(SDL_Init(SDL_INIT_EVERYTHING))
		GameCrash("Initialized SDL failed");
	LoggerInfo("SDL initialized");
	MathInit();
	RM_InitResourceManager();
	PMD_Init();
	RE_InitWindow(WINDOW_WIDTH,WINDOW_HEIGHT);
	IN_InitInput();	
	InitEntities();
	InitAttributes();
	GameMainLoop();
	GameClose();
	return 0;
}

void GameMainLoop()
{
	long long lastTime=OS_GetMsTime();
	LoggerInfo("Starting game main loop");
	//theWorld = WorldNewGame("szszss",1000,TYPE_NORMAL,DIFF_NORMAL);
	//WorldStart(theWorld);
	Gui_Open(GuiScreenGame);
	IN_Clear();
	while(shouldRun)
	{
		if(OS_GetMsTime()-lastTime>WINDOW_FRAME)
		{
			if(Update() || RE_Render())
				break;
			/*if(shouldKillWorld)
			{
				shouldKillWorld=FALSE;
			}*/
			lastTime=OS_GetMsTime();
			tickTime++;
		}
		SDL_Delay(1);
	}
	LoggerInfo("Game main loop broke.");
	WorldEnd(theWorld);
	WorldDestory(theWorld);
	theWorld=NULL;
}

int Update()
{
	//处理事件
	SDL_Event sdlEvent;
	while(SDL_PollEvent(&sdlEvent))
	{
		if(HandleEvent(sdlEvent))
			return -1;
	}
	if(!gamePause)
	{
		IN_UpdateInput();
		if(theWorld!=NULL)
		{
			WorldUpdate(theWorld);
		}
	}
	Gui_Update(theWorld);
	return 0;
}

int HandleEvent(SDL_Event sdlEvent)
{
	switch (sdlEvent.type)
	{
	case SDL_KEYDOWN:
		if(IN_KeyDown(sdlEvent.key.keysym.sym))
			break;
		break;
	case SDL_KEYUP:
		if(IN_KeyUp(sdlEvent.key.keysym.sym))
			break;
		break;
	case SDL_TEXTINPUT:
		
		break;
	case SDL_TEXTEDITING:

		break;
	case SDL_MOUSEBUTTONDOWN:
		if(sdlEvent.button.button==SDL_BUTTON_LEFT)
			IN_MouseDown(sdlEvent.button.x,sdlEvent.button.y);
		break;
	case SDL_QUIT:
		GameExit();
		return -1;
	}
	return 0;
}

BOOL GameSetPause(BOOL pause)
{
	gamePause = pause;
	return TRUE;
}

BOOL GameGetPause()
{
	return gamePause;
}

void GameSafelyKillWorld()
{
	shouldKillWorld = TRUE;
}

void GameCrash(char* cause)
{
	LoggerFatal(cause);
	GameClose();
	exit(EXIT_FAILURE);
}

void GameClose()
{
	LoggerInfo("Closing game");
	Gui_Close();
	if(theWorld!=NULL)
		WorldDestory(theWorld);
	IN_DestroyInput();
	RE_DestroyWindow();
	PMD_Close();
	RM_Close();
	SDL_Quit();
	LoggerInfo("SDL closed");
	LoggerClose();
}

void GameExit()
{
	shouldRun = 0;
}