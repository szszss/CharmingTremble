#include "input.h"

OSM mainOSM;
int keyState = 0;
int keyBuffer = 0;
static unsigned char operateCodes[256];

static BOOL _DummyDelete(void* v){return 0;}

int IN_InitInput()
{
	int i=0;
	mainOSM.linkedList = LinkedListCreate();
	mainOSM.delayTime = 0;
	for(i=0;i<256;i++)
	{
		operateCodes[i]=(unsigned char)i;
	}
	LoggerInfo("Input processor initialized");
}

void IN_DestroyInput()
{
	LinkedListDestory(mainOSM.linkedList,_DummyDelete);
	LoggerInfo("Input processor closed");
}

BOOL IN_KeyDown(SDL_Keycode keynum)
{
	switch(keynum)
	{
	case SDLK_UP:
		LinkedListOffer(mainOSM.linkedList,&operateCodes[INPUT_OPERATE_UP_DOWN]);
		break;
	case SDLK_DOWN:
		LinkedListOffer(mainOSM.linkedList,&operateCodes[INPUT_OPERATE_DOWN_DOWN]);
		break;
	case SDLK_LEFT:
		LinkedListOffer(mainOSM.linkedList,&operateCodes[INPUT_OPERATE_LEFT_DOWN]);
		break;
	case SDLK_RIGHT:
		LinkedListOffer(mainOSM.linkedList,&operateCodes[INPUT_OPERATE_RIGHT_DOWN]);
		break;
	case SDLK_SPACE:
		LinkedListOffer(mainOSM.linkedList,&operateCodes[INPUT_OPERATE_SPACE_DOWN]);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

BOOL IN_KeyUp(SDL_Keycode keynum)
{
	switch(keynum)
	{
	case SDLK_UP:
		LinkedListOffer(mainOSM.linkedList,&operateCodes[INPUT_OPERATE_UP_UP]);
		break;
	case SDLK_DOWN:
		LinkedListOffer(mainOSM.linkedList,&operateCodes[INPUT_OPERATE_DOWN_UP]);
		break;
	case SDLK_LEFT:
		LinkedListOffer(mainOSM.linkedList,&operateCodes[INPUT_OPERATE_LEFT_UP]);
		break;
	case SDLK_RIGHT:
		LinkedListOffer(mainOSM.linkedList,&operateCodes[INPUT_OPERATE_RIGHT_UP]);
		break;
	case SDLK_SPACE:
		LinkedListOffer(mainOSM.linkedList,&operateCodes[INPUT_OPERATE_SPACE_UP]);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

unsigned char IN_GetOperate()
{
	int operate;
	int result;
	unsigned char* value;
	if(mainOSM.delayTime>0)
		return 0;
	value = (unsigned char*)LinkedListPoll(mainOSM.linkedList,&result);
	if(result==FALSE || (operate=*value)<=200)
		return 0;;
	return operate;
}

void IN_UpdateInput()
{
	unsigned char operate;
	int result;
	if(mainOSM.delayTime>0)
	{
		mainOSM.delayTime--;
		return;
	}
}

void IN_Clear()
{
	keyState = 0;
	keyBuffer = 0;
}

void IN_MouseDown(long x,long y)
{
	Gui_MouseDown(x,y);
}
