#include "game.h"
#include <stdlib.h>
#include "renderengine.h"
#include "SDL.h"
#include "util.h"
#include "entity.h"
#include "world.h"

void GameClose();
void GameMainLoop();
int Update();
int HandleEvent(SDL_Event sdlEvent);

static int shouldRun = 1;
unsigned long long tickTime = 0;
World* theWorld = NULL;

int main(int argc, char** argv)
{
	//int result;
	LoggerCreate(TRUE,"log.txt",LOGGER_APPEND,LOGGER_LEVEL_ALL,LOGGER_FORMAT_C);
	LoggerInfo("Initializing game");
	if(SDL_Init(SDL_INIT_EVERYTHING))
		GameCrash("Initialized SDL failed");
	LoggerInfo("SDL initialized");
	RE_InitWindow(WINDOW_WIDTH,WINDOW_HEIGHT);
	InitEntities();
	GameMainLoop();
	GameClose();
	return 0;
}

void GameMainLoop()
{
	LoggerInfo("Starting game main loop");
	theWorld = WorldNewGame("szszss",1000,TYPE_NORMAL,DIFF_NORMAL);
	WorldStart(theWorld);
	while(shouldRun)
	{
		if(Update() || RE_Render())
			break;
		SDL_Delay(WINDOW_FRAME);
		tickTime++;
	}
	LoggerInfo("Game main loop broke.");
	WorldEnd(theWorld);
	WorldDestory(theWorld);
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
	if(theWorld!=NULL)
	{
		WorldUpdate(theWorld);
	}
	return 0;
}

int HandleEvent(SDL_Event sdlEvent)
{
	switch (sdlEvent.type)
	{
	case SDL_KEYDOWN:
		
		break;
	case SDL_KEYUP:

		break;
	case SDL_TEXTINPUT:
		
		break;
	case SDL_TEXTEDITING:

		break;
	case SDL_QUIT:
		GameExit();
		return -1;
	}
	return 0;
}

void GameCrash(char* cause)
{
	LoggerFatal(cause);
	GameClose();
}

void GameClose()
{
	LoggerInfo("Closing game");
	RE_DestroyWindow();
	SDL_Quit();
	LoggerInfo("SDL closed");
	LoggerClose();
}

void GameExit()
{
	shouldRun = 0;
}