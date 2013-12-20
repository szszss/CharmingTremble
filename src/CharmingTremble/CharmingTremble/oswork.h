#pragma once
#include "game.h"

#ifdef OS_WINDOWS
#include <Windows.h>
#endif

char* OS_GetFontPath(char* fontName,char* backupFontName);
BOOL OS_PathExist(char* path);