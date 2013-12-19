#include "oswork.h"
#include "memory.h"

#ifdef OS_WINDOWS
BOOL FindFirstFileExists(LPCTSTR lpPath, DWORD dwFilter);
BOOL FilePathExists(LPCTSTR lpPath);

char* OS_GetFontPath( char* fontName,char* backupFontName )
{
	char* fontFolder = getenv("WINDIR");
	int length = strlen(fontName)+strlen(fontFolder)+8;
	char* fontPath = (char*)malloc_s(length*sizeof(char));
	memset(fontPath,0,length);
	strcat(fontPath,fontFolder);
	strcat(fontPath,"\\fonts\\");
	strcat(fontPath,fontName);
	if(OS_PathExist(fontPath))
	{
		return fontPath;
	}
	else
	{
		if(backupFontName!=NULL)
		{
			return OS_GetFontPath(backupFontName,NULL);
		}
		else
		{
			GameCrash(0); //TODO:一个错误码
		}
	}
	return NULL;
}

//Author:冰点青蛙
BOOL FindFirstFileExists(LPCTSTR lpPath, DWORD dwFilter)
{
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(lpPath, &fd);
	BOOL bFilter = (FALSE == dwFilter) ? TRUE : fd.dwFileAttributes & dwFilter;
	BOOL RetValue = ((hFind != INVALID_HANDLE_VALUE) && bFilter) ? TRUE : FALSE;
	FindClose(hFind);
	return RetValue;
}

//Author:冰点青蛙
BOOL FilePathExists(LPCTSTR lpPath)
{
	return FindFirstFileExists(lpPath, FALSE);
}

BOOL OS_PathExist(char* path)
{
	return FilePathExists(path);
}
#endif