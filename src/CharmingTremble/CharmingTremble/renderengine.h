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
#define RE_STAGE_FINISH "Rendering"

#define RE_STAGE_BEFORE_PROCESS_TEXTURE "Before processing texture"
#define RE_STAGE_AFTER_PROCESS_TEXTURE "Processing texture"

#define RE_CLEAR_COLOR 0.345, 0.675, 1.0, 1.0

struct implTexture{
	unsigned long width;
	unsigned long height;
	unsigned int id;
};

/*��ʼ������,OPENGL��ͼ����Դ.*/
int RE_InitWindow(int width,int height);
/*���ٴ��ں�OpenGL,���ͷ�ͼ����Դ*/
void RE_DestroyWindow();
/*��������*/
void RE_Reshape(int width,int height);
/*��~Ⱦ~!*/
int RE_Render();
/*������Ⱦһ��ש��.*/
void RE_RenderCubeQuick(int count);
/*��Ⱦһ��lx,ly,lzΪ���϶���,rx,ry,rzΪ���¶����ש��.*/
void RE_RenderCube(float lx,float ly,float lz,float rx,float ry,float rz);
/*����һ����������ľ���.
x,yΪ�������Ͻ�����
width,heightΪ���ο��
u,vΪ�����uv
uw,vhΪ�����н�ȡ�����*/
void RE_DrawRectWithTexture(float x,float y,float width,float height,float u,float v,float uw,float vh);
/*��һ������,��ΪNULL,��Ϊȡ����.����ֵΪ���󶨵�������������ID (����ΪNULL��Ϊ0)*/
int RE_BindTexture(Texture* texture);
/*����һ���������������*/
unsigned int RE_ProcessRawTexture(ImageData* rawData,int color,int format,unsigned long width,unsigned long height);
/*ж��һ���������,ע����ֻ����ж��OpenGL�ڲ����������,����(Texture)���ͷ�����Ҫ�ֶ����*/
void RE_UnloadTexture(unsigned int texture);
/*���OpenGL�ڲ�����,ע��ֻ����Debugģʽ�²Ż���ÿһ������,Releaseģʽ��ֻ��Rendering�׶βŻ���м��.������ΪglGetError�Ŀ����ܴ�.*/
int RE_CheckGLError(char* stage);
void RE_DrawText(char* text,float x,float y,float width);