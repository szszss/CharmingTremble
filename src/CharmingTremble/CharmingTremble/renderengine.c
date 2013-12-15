#pragma once
#include "renderengine.h"
#include "game.h"
#include "png.h"

#ifdef OS_WINDOWS //其实用WIN32也行...
#pragma comment( lib, "opengl32.lib")
#pragma comment( lib, "glu32.lib")
#endif

#include "SDL.h"
#include "SDL_opengl.h"

#ifdef OS_MAC
#include <OpenGL/glu.h> 
#else
#include <GL/glu.h>
#endif

#include "world.h"
#include "util.h"

SDL_Window* window = NULL;
SDL_GLContext* glContext = NULL;
static GLdouble aspect;

extern World* theWorld;
extern unsigned long long tickTime;

int RE_InitWindow(int width,int height)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
	window = SDL_CreateWindow(WINDOW_TITLE,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,width,height,SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if(window==NULL)
		GameCrash("Initialized window failed.");
	LoggerInfo("Window initialized");
	glContext = SDL_GL_CreateContext(window);
	if(glContext==NULL)
	{
		GameCrash("Initialized opengl(2.1) failed.");
	}
	LoggerInfo("OpenGL(2.1) initialized");
	SDL_GL_SetSwapInterval(1);
	RE_Reshape(width,height);
	return 0;
}

void RE_Reshape(int width,int height)
{
	glViewport(0,0,width,height);
	//aspect = (double)width/(double)height;
	aspect = (double)height/(double)width;
	//aspect=1;
}

void RE_DestroyWindow()
{
	if(glContext!=NULL)
		SDL_GL_DeleteContext(glContext);
	if(window!=NULL)
		SDL_DestroyWindow(window);
}

int RE_Render()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); //清理缓存
	glClearColor( RE_CLEAR_COLOR ); //静怡的天蓝色
	glMatrixMode(GL_PROJECTION); //重设定投影矩阵
	glLoadIdentity();
	//glOrtho(-100.0, 100.0, -100.0, 100.0, 1.0, 100.0);
	//gluOrtho2D(-1.0,1.0,-1.0,1.0);
	//gluPerspective( 70.0, aspect, 1.0, 1024.0 ); //将投影矩阵设为透视投影
	glFrustum(-0.35,0.65,-aspect/2,aspect/2,1,1024);
	glMatrixMode( GL_MODELVIEW ); //设定模型视角矩阵
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST); //不开深度测试的话毁三观啊
	glTranslatef(0.0f, 0.0f, -42);
	//glRotatef(tickTime,1,1,1);
	//RE_drawCube(-1,1,-1,1,-1,1);
	if(theWorld!=NULL)
	{
		WorldRender(theWorld);
	}
	SDL_GL_SwapWindow(window);
	return 0;
}

void RE_DrawCube( float lx,float ly,float lz,float rx,float ry,float rz )
{
	if(ly<ry)
	{
		float temp = ly;
		ly=ry;
		ry=temp;
	}
	glBegin(GL_QUADS);
		//绘制正面
		//glColor4f(1,0,0,1);
		glNormal3f(0,0,1);
		glVertex3f(rx,ly,rz);
		glVertex3f(rx,ry,rz);
		glVertex3f(lx,ry,rz);
		glVertex3f(lx,ly,rz);
		//绘制背面
		//glColor4f(1,0,0,1);
		glNormal3f(0,0,-1);
		glVertex3f(lx,ly,lz);
		glVertex3f(lx,ry,lz);
		glVertex3f(rx,ry,lz);
		glVertex3f(rx,ly,lz);
		//绘制左面
		//glColor4f(0,1,0,1);
		glNormal3f(-1,0,0);
		glVertex3f(lx,ly,rz);
		glVertex3f(lx,ry,rz);
		glVertex3f(lx,ry,lz);
		glVertex3f(lx,ly,lz);
		//绘制右面
		//glColor4f(0,1,0,1);
		glNormal3f(1,0,0);
		glVertex3f(rx,ly,lz);
		glVertex3f(rx,ry,lz);
		glVertex3f(rx,ry,rz);
		glVertex3f(rx,ly,rz);
		//绘制顶面
		//glColor4f(0,0,1,1);
		glNormal3f(0,1,0);
		glVertex3f(rx,ly,lz);
		glVertex3f(rx,ly,rz);
		glVertex3f(lx,ly,rz);
		glVertex3f(lx,ly,lz);
		//绘制底面
		//glColor4f(0,0,1,1);
		glNormal3f(0,-1,0);
		glVertex3f(lx,ry,lz);
		glVertex3f(lx,ry,rz);
		glVertex3f(rx,ry,rz);
		glVertex3f(rx,ry,lz);
	glEnd();
}

