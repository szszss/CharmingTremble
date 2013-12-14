#include "game.h"
#include <stdlib.h>
#include "renderengine.h"
#include "SDL.h"

void GameClose();
void GameMainLoop();
int Update();
int HandleEvent(SDL_Event sdlEvent);

static int shouldRun = 1;
unsigned long long tickTime = 0;

int main(int argc, char** argv)
{
	//int result;
	if(SDL_Init(SDL_INIT_EVERYTHING))
		GameCrash("Initialized SDL failed.");
	RE_InitWindow(WINDOW_WIDTH,WINDOW_HEIGHT);
	GameMainLoop();
	GameClose();
	return 0;
}

void GameMainLoop()
{
	while(shouldRun)
	{
		if(Update() || RE_Render())
			break;
		SDL_Delay(WINDOW_FRAME);
		tickTime++;
	}
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
	return 0;
}

int HandleEvent(SDL_Event sdlEvent)
{
	switch (sdlEvent.type)
	{
	case SDL_KEYDOWN:
		
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
	GameClose();
}

void GameClose()
{
	RE_DestroyWindow();
	SDL_Quit();
}

void GameExit()
{
	shouldRun = 0;
}