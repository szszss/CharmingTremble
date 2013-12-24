#pragma once
#include "game.h"

long long OS_GetMsTime();
char* OS_GetFontPath(char* fontName,char* backupFontName);
BOOL OS_PathExist(char* path);