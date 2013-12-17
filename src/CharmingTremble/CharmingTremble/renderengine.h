//#define RE_INIT_SUCCESS_BUT_LOW_VERSION 1
//#define RE_INIT_GL_NO_SUPPORT 10
//#define RE_INIT_JUST_ERROR 255
#pragma once
#include "game.h"

#define RE_STAGE_BEFORE_DRAW_3D "Before drawing 3D"
#define RE_STAGE_AFTER_DRAW_3D "After drawn 3D"
#define RE_STAGE_FLUSH_3D "Flushing 3D"
#define RE_STAGE_BEFORE_DRAW_2D "Before drawing 2D"
#define RE_STAGE_AFTER_DRAW_2D "After drawn 2D"
#define RE_STAGE_FLUSH_2D "Flushing 2D"
#define RE_STAGE_SWAP "Swapping screen buffer"

#define RE_STAGE_BEFORE_PROCESS_TEXTURE "Before processing texture"
#define RE_STAGE_AFTER_PROCESS_TEXTURE "Processing texture"

#define RE_CLEAR_COLOR 0.345, 0.675, 1.0, 1.0

struct implTexture{
	unsigned long width;
	unsigned long height;
	unsigned int id;
};

int RE_InitWindow(int width,int height);
void RE_DestroyWindow();
void RE_Reshape(int width,int height);
int RE_Render();
/*lx,ly,lz为左上顶点,rx,ry,rz为右下顶点*/
void RE_RenderCubeQuick(int count);
void RE_RenderCube(float lx,float ly,float lz,float rx,float ry,float rz);
void RE_DrawRectWithTexture(float x,float y,float width,float height,float u,float v,float uw,float vh);
int RE_BindTexture(Texture* texture);
unsigned int RE_ProcessRawTexture(ImageData* rawData,int color,int format,unsigned long width,unsigned long height);
void RE_UnloadTexture(unsigned int texture);
int RE_CheckGLError(char* stage);