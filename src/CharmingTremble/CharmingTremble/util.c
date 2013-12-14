#include "util.h"
#include "memory.h"
#include <stdlib.h>

//梅森旋转算法随机数发生器

MTRandomGen* MTCreate(long seed)
{
	int i;
	MTRandomGen *randomGen = (MTRandomGen*)malloc_s(sizeof(MTRandomGen));
	randomGen->MT[0]=seed;
	randomGen->index=0;
	for (i = 1; i < 624; i++) {
		randomGen->MT[i] = 0xffffffff & (0x6c078965 * (randomGen->MT[i - 1] ^ (randomGen->MT[i - 1] >> 30)) + i);
	}
	return randomGen;
}

void MTRegenerate(MTRandomGen *randomGen)
{
	int i,y;
	for (i = 0; i < 624; i++)
	{
		y = (randomGen->MT[i] & 0x80000000) + (randomGen->MT[(i + 1) % 624] & 0x7fffffff);
		randomGen->MT[i] = randomGen->MT[(i + 397) % 624] ^ (y >> 1);
		if (y % 2 != 0)
		{
			randomGen->MT[i] ^= 0x9908b0df;
		}
	}
}

long MTNext(MTRandomGen *randomGen)
{
	int y;
	if (randomGen->index == 0)
	{
		MTRegenerate(randomGen);
	}
	y = randomGen->MT[randomGen->index];
	y ^= (y >> 11);
	y ^= ((y << 7) & 0x9d2c5680);
	y ^= ((y << 15) & 0xefc60000);
	y ^= (y >> 18);
	randomGen->index = (randomGen->index + 1) % 624;
	return y;
}

long MTNextInt(MTRandomGen *randomGen,long min,long max)
{
	if(max<min)
	{
		return MTNextInt(randomGen,max,min);
	}
	return MTNext(randomGen) % (max - min + 1) + min;
}

void MTDestroy(MTRandomGen *randomGen)
{
	free_s(randomGen);
}


Logger logger;

void LoggerCreate(BOOL logInFile,char* fileName,enum LoggerMode mode,int level,char* format)
{
	logger.fileAvailable = logInFile;
	logger.level=level;
	logger.mode=mode;
	logger.format=format;
	if(logger.fileAvailable)
	{
		switch(logger.mode)
		{
		case LOGGER_OVERRIDE:
			logger.loggerFile = fopen(fileName,"w");
			break;
		case LOGGER_APPEND:
			logger.loggerFile = fopen(fileName,"a+");
			break;
		default:
			logger.fileAvailable=FALSE;
			return;
		}
		//检查日志文件是否打开成功
		if(logger.loggerFile == NULL)
		{
			logger.fileAvailable=FALSE;
			LoggerError("Created/Opened log file failed. Logger will still run without log file");
		}
		LoggerInfo("Logger initialized. Hello Yooooorld!");
	}
}
int LoggerOutput(char* level,char* text)
{
	printf(logger.format,level,text);
	if(logger.fileAvailable)
	{
		fprintf(logger.loggerFile,logger.format,level,text);
		return 2;
	}
	return 1;
}

int LoggerDebug(char* text)
{
	if(logger.level&LOGGER_LEVEL_DEBUG)
		return LoggerOutput("Debug",text);
	return 0;
}
//int LoggerDebugln(char* text);
int LoggerInfo(char* text)
{
	if(logger.level&LOGGER_LEVEL_INFO)
		return LoggerOutput("Info",text);
	return 0;
}
//int LoggerInfoln(char* text);
int LoggerWarn(char* text)
{
	if(logger.level&LOGGER_LEVEL_WARN)
		return LoggerOutput("Warn",text);
	return 0;
}
//int LoggerWarnln(char* text);
int LoggerError(char* text)
{
	if(logger.level&LOGGER_LEVEL_ERROR)
		return LoggerOutput("Error",text);
	return 0;
}
//int LoggerErrorln(char* text);
int LoggerFatal(char* text)
{
	if(logger.level&LOGGER_LEVEL_FATAL)
		return LoggerOutput("Fatal",text);
	return 0;
}
//int LoggerFatalln(char* text);
void LoggerClose()
{
	LoggerInfo("Closing logger. Good-byte! Cruel world!\n");
	fclose(logger.loggerFile);
}

StringBuilder* SBCreate()
{
	StringBuilder *sb = (StringBuilder*)malloc_s(sizeof(StringBuilder));
	sb->bufferedString = (char**)malloc_s(sizeof(char*)*STRING_BUILDER_MAX_BUFFER);
	sb->bufferPointer = 0;
	return sb;
}

StringBuilder* SBAppend( StringBuilder *sb,char* str )
{
	if(sb->bufferPointer==STRING_BUILDER_MAX_BUFFER)
		return sb;
	sb->bufferedString[sb->bufferPointer]=str;
	sb->bufferPointer++;
	return sb;
}

char* SBBuild( StringBuilder *sb )
{
	int i,length=1;
	char* str=NULL;
	for(i=0;i<sb->bufferPointer;i++)
	{
		length+=strlen(sb->bufferedString[i]);
	}
	str = (char*)malloc_s(length*sizeof(char));
	memset(str,0,length*sizeof(char));
	for(i=0;i<sb->bufferPointer;i++)
	{
		strcat(str,sb->bufferedString[i]);
	}
	free(sb);
	return str;
}

void SBDestroy(StringBuilder *sb)
{
	free_s(sb);
}