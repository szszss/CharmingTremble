#include <stdio.h>

#ifndef BOOL
#define BOOL int
#define TRUE 1
#define FALSE 0
#endif

typedef struct
{
	long int MT[624];
	long int index ;
} MTRandomGen;

MTRandomGen* MTCreate(long seed);
long MTNext(MTRandomGen *randomGen);
long MTNextInt(MTRandomGen *randomGen,long min,long max);
void MTDestroy(MTRandomGen *randomGen);

#define LOGGER_LEVEL_DEBUG 1
#define LOGGER_LEVEL_INFO 2
#define LOGGER_LEVEL_WARN 4
#define LOGGER_LEVEL_ERROR 8
#define LOGGER_LEVEL_FATAL 16
#define LOGGER_LEVEL_ALL 31
#define LOGGER_LEVEL_ALL_EXCEPT_DEBUG 30
#define LOGGER_LEVEL_NONE 0
#define LOGGER_FORMAT_C "%s: %s\n"
#define LOGGER_FORMAT_JAVA "[%s] %s\n"
enum LoggerMode {LOGGER_OVERRIDE,LOGGER_APPEND};

typedef struct  
{
	FILE* loggerFile;
	BOOL fileAvailable;
	enum LoggerMode mode;
	int level;
	char* format;
} Logger;

// Logger采用单例模式,程序中只能存在一个Logger
void LoggerCreate(BOOL logInFile,char* fileName,enum LoggerMode mode,int level,char* format);
int LoggerDebug(char* text);
//int LoggerDebugln(char* text);
int LoggerInfo(char* text);
//int LoggerInfoln(char* text);
int LoggerWarn(char* text);
//int LoggerWarnln(char* text);
int LoggerError(char* text);
//int LoggerErrorln(char* text);
int LoggerFatal(char* text);
//int LoggerFatalln(char* text);
void LoggerClose();

/*StringBuilder是一个能将多个字符串拼接为一个的工具.*/
#define STRING_BUILDER_MAX_BUFFER 24

typedef struct {
	char** bufferedString;
	int bufferPointer;
} StringBuilder;

/*创建一个新的StringBuilder.*/
StringBuilder* SBCreate();
/*向StringBuilder后追加一个字符串.*/
StringBuilder* SBAppend(StringBuilder *sb,char* str);
/*生成一个新字符串,生成完毕后会自动销毁StringBuilder.*/
char* SBBuild(StringBuilder *sb);
/*手动销毁一个StringBuilder,已Build的StringBuilder无需此步骤.*/
void SBDestroy(StringBuilder *sb);