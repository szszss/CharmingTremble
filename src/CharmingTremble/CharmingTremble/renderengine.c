#include "renderengine.h"
#include "resourcemanager.h"
#include "game.h"
#include "memory.h"
#include "oswork.h"
#include "png.h"
#include "math.h"
#include "collection.h"
#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_BITMAP_H

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

int RE_InitQuicklyRender();
int RE_InitFontRenderer(int width,int height);
void RE_RenderCubeDoLeft(float lx,float ly,float lz,float rx,float ry,float rz);
void RE_RenderCubeDoCentre(float lx,float ly,float lz,float rx,float ry,float rz);
void RE_RenderCubeDoRight(float lx,float ly,float lz,float rx,float ry,float rz);
void RE_DestroyQuicklyRender();
void RE_DestroyFontRenderer();
BOOL RE_DestroyTextTexture(void *texture);
TextTexture* RE_ProcessTextTexture(unsigned long* unicodeText,float maxWidth);

SDL_Window* window = NULL;
SDL_GLContext glContext = NULL;
FT_Library library = NULL;
FT_Face face = NULL;
static LinkedList *textTextureCache = NULL;
static GLdouble aspect;
static GLuint quicklyRenderList[20]={0};
static int windowWidth;
static int windowHeight;

extern World* theWorld;
extern unsigned long long tickTime;

int RE_InitWindow(int width,int height)
{
	int result;
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24); //深度缓冲如果过大会有奇妙的效果,我想也许是因为超过硬件支持的范围后,OpenGL只能采用软件模拟了
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
	if(RE_InitQuicklyRender())
	{
		GameCrash("Initialized OpenGL quickly renderer failed.");
	}
	if(result=RE_InitFontRenderer(width,height))
	{
		LoggerFatal("Font renderer error:%d",result);
		GameCrash("Initialized Font renderer failed.");
	}
	LoggerInfo("Font renderer initialized");
	return 0;
}

void RE_Reshape(int width,int height)
{
	windowWidth=width;
	windowHeight=height;
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
	if(quicklyRenderList[0]!=0)
	{
		RE_DestroyQuicklyRender();
		LoggerInfo("OpenGL quickly renderer destroyed");
	}
	if(library!=NULL)
	{
		RE_DestroyFontRenderer();
		LoggerInfo("Font renderer destroyed");
	}
}

int RE_BindTexture(Texture* texture)
{
	if(texture==NULL)
	{
		glBindTexture(GL_TEXTURE_2D,0);
		return 0;
	}	
	else
	{
		glBindTexture(GL_TEXTURE_2D,texture->id);
		return texture->id;
	}
}

int RE_Render()
{
	static float amLight[] = {0.2,0.2,0.2,1};
	static Texture* texture = NULL;
	//-------------------绘制3D-------------------
	glClearColor( RE_CLEAR_COLOR ); //静怡的天蓝色
	glClearDepth(1.0f);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); //清理缓存
	RE_CheckGLError(RE_STAGE_BEFORE_DRAW_3D);
	glMatrixMode(GL_PROJECTION); //重设定投影矩阵
	glLoadIdentity();
	glFrustum(-0.35,0.65,-aspect/2,aspect/2,1,1024);
	glMatrixMode( GL_MODELVIEW ); //设定模型视角矩阵
	glLoadIdentity();
	glPushMatrix();
	glEnable(GL_DEPTH_TEST); //不开深度测试的话毁三观啊
	glEnable(GL_TEXTURE_2D);
	//glShadeModel(GL_SMOOTH);
	//glEnable(GL_LINE_SMOOTH);
	//glEnable(GL_POINT_SMOOTH);
	//glEnable(GL_LIGHTING);
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT,amLight);
	//glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	//glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glTranslatef(0.0f, 0.0f, -42);
	if(theWorld!=NULL)
	{
		WorldRender(theWorld);
	}
	RE_CheckGLError(RE_STAGE_AFTER_DRAW_3D);
	glPopMatrix();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_POINT_SMOOTH);
	//glDisable(GL_LIGHTING);
	glFlush();
	RE_CheckGLError(RE_STAGE_FLUSH_3D);
	//-------------------绘制2D-------------------
	RE_CheckGLError(RE_STAGE_BEFORE_DRAW_2D);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, 0.5, 10.0);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -1);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if(texture==NULL)
		texture=RM_GetTexture("image/bgGame.png");
	RE_BindTexture(texture);
	//glRotatef(tickTime,1,1,1);
	RE_DrawRectWithTexture(0,0,1,1,0,0,800.0/1024.0,600.0/1024.0);
	RE_BindTexture(NULL);

	//RE_DrawText(L"H",0,0,0);
	RE_DrawTextStatic(L"屌",0.01,0.01,1);

	RE_CheckGLError(RE_STAGE_AFTER_DRAW_2D);
	glPopMatrix();
	glFlush();
	RE_CheckGLError(RE_STAGE_FLUSH_2D);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	SDL_GL_SwapWindow(window);
	RE_CheckGLError(RE_STAGE_FINISH);
	return 0;
}

void RE_RenderCubeDoLeft(float lx,float ly,float lz,float rx,float ry,float rz)
{
	//绘制左面
	//glColor4f(0,1,0,1);
	glNormal3f(-1,0,0);
	glTexCoord2f(1,1);glVertex3f(lx,ly,rz);
	glTexCoord2f(1,0);glVertex3f(lx,ry,rz);
	glTexCoord2f(0,0);glVertex3f(lx,ry,lz);
	glTexCoord2f(0,1);glVertex3f(lx,ly,lz);
}

void RE_RenderCubeDoCentre(float lx,float ly,float lz,float rx,float ry,float rz)
{
	//绘制正面
	//glColor4f(1,0,0,1);
	glNormal3f(0,0,1);
	glTexCoord2f(1,1);glVertex3f(rx,ly,rz);
	glTexCoord2f(1,0);glVertex3f(rx,ry,rz);
	glTexCoord2f(0,0);glVertex3f(lx,ry,rz);
	glTexCoord2f(0,1);glVertex3f(lx,ly,rz);
	//绘制背面
	//glColor4f(1,0,0,1);
	glNormal3f(0,0,-1);
	glTexCoord2f(1,1);glVertex3f(lx,ly,lz);
	glTexCoord2f(1,0);glVertex3f(lx,ry,lz);
	glTexCoord2f(0,0);glVertex3f(rx,ry,lz);
	glTexCoord2f(0,1);glVertex3f(rx,ly,lz);
	//绘制顶面
	//glColor4f(0,0,1,1);
	glNormal3f(0,1,0);
	glTexCoord2f(1,1);glVertex3f(rx,ly,lz);
	glTexCoord2f(1,0);glVertex3f(rx,ly,rz);
	glTexCoord2f(0,0);glVertex3f(lx,ly,rz);
	glTexCoord2f(0,1);glVertex3f(lx,ly,lz);
	//绘制底面
	//glColor4f(0,0,1,1);
	glNormal3f(0,-1,0);
	glTexCoord2f(1,1);glVertex3f(lx,ry,lz);
	glTexCoord2f(1,0);glVertex3f(lx,ry,rz);
	glTexCoord2f(0,0);glVertex3f(rx,ry,rz);
	glTexCoord2f(0,1);glVertex3f(rx,ry,lz);
}

void RE_RenderCubeDoRight(float lx,float ly,float lz,float rx,float ry,float rz)
{
	//绘制右面
	//glColor4f(0,1,0,1);
	glNormal3f(1,0,0);
	glTexCoord2f(1,1);glVertex3f(rx,ly,lz);
	glTexCoord2f(1,0);glVertex3f(rx,ry,lz);
	glTexCoord2f(0,0);glVertex3f(rx,ry,rz);
	glTexCoord2f(0,1);glVertex3f(rx,ly,rz);
}

void RE_RenderCube( float lx,float ly,float lz,float rx,float ry,float rz )
{
	if(ly<ry)
	{
		float temp = ly;
		ly=ry;
		ry=temp;
	}
	glBegin(GL_QUADS);
		RE_RenderCubeDoLeft(lx,ly,lz,rx,ry,rz);
		RE_RenderCubeDoCentre(lx,ly,lz,rx,ry,rz);
		RE_RenderCubeDoRight(lx,ly,lz,rx,ry,rz);
	glEnd();
}

unsigned int RE_ProcessRawTexture( ImageData* rawData,int color,int format,unsigned long width,unsigned long height )
{
	GLuint texture;
	RE_CheckGLError(RE_STAGE_BEFORE_PROCESS_TEXTURE);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//GL_RGBA
	gluBuild2DMipmaps(GL_TEXTURE_2D,color,width,height,format,GL_UNSIGNED_BYTE, rawData);
	//glTexImage2D(GL_TEXTURE_2D, 0, color, width, height, 0, format, GL_UNSIGNED_BYTE, rawData);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_BLEND); //诡异的反色!
	RE_CheckGLError(RE_STAGE_AFTER_PROCESS_TEXTURE);
	return texture;
}

void RE_UnloadTexture( unsigned int texture )
{
	glDeleteTextures(1,&texture);
}

int RE_CheckGLError(char* stage)
{
	GLenum error;
	#ifndef DEBUG
		if(stage!=RE_STAGE_FINISH)
		{
			return 0;
		}
	#endif
	error = glGetError();
	if(error!=GL_NO_ERROR)
	{
		LoggerWarn("An OpenGL error happened in [%s] : %s",stage,gluErrorString(error));
	}
	return error;
}

void RE_DrawRectWithTexture( float x,float y,float width,float height,float u,float v,float uw,float vh )
{
	x=x*2.0f-1.0f;
	y=1.0f-(y*2.0f);
	v=1.0f-v;
	width*=2.0f;
	height*=2.0f;
	glBegin(GL_QUADS);
		glTexCoord2f(u+uw,v-vh);glVertex3f(x+width,y-height,0);
		glTexCoord2f(u+uw,v);glVertex3f(x+width, y,0);
		glTexCoord2f(u,v);glVertex3f(x,y,0);
		glTexCoord2f(u,v-vh);glVertex3f(x,y-height,0);
	glEnd();
}

int RE_InitQuicklyRender()
{
	int i,j;
	float x;
	quicklyRenderList[0] = glGenLists(20);
	if(quicklyRenderList[0]==0)
	{
		return 1;
	}
	for(i=1;i<20;i++)
	{
		quicklyRenderList[i]=quicklyRenderList[i-1]+1;
	}
	for(i=1;i<=20;i++)
	{
		glNewList(quicklyRenderList[i-1],GL_COMPILE);
		for(j=1;j<=i;j++)
		{
			if(j==1&&j==i)
			{
				RE_RenderCube(-0.5f,0.5f,-0.5f,0.5f,-0.5f,0.5f);
				break;
			}
			if(j==1)
			{
				x = - (float)(i-1)/2;
				//x = -0.5f - (float)(i-1)/2;
				glBegin(GL_QUADS);
				RE_RenderCubeDoLeft(-0.5f+x,0.5f,-0.5f,0.5f+x,-0.5f,0.5f);
				RE_RenderCubeDoCentre(-0.5f+x,0.5f,-0.5f,0.5f+x,-0.5f,0.5f);
				glEnd();
				x += 1.0f;
				continue;
			}
			if(j==i)
			{
				glBegin(GL_QUADS);
				RE_RenderCubeDoCentre(-0.5f+x,0.5f,-0.5f,0.5f+x,-0.5f,0.5f);
				RE_RenderCubeDoRight(-0.5f+x,0.5f,-0.5f,0.5f+x,-0.5f,0.5f);
				glEnd();
				x += 1.0f;
				continue;
			}
			glBegin(GL_QUADS);
			RE_RenderCubeDoCentre(-0.5f+x,0.5f,-0.5f,0.5f+x,-0.5f,0.5f);
			glEnd();
			x += 1.0f;
		}
		glEndList();
	}
	return 0;
}

void RE_DestroyQuicklyRender()
{
	glDeleteLists(quicklyRenderList[0],20);
}

void RE_RenderCubeQuick( int count )
{
	if(count<1||count>20)
	{
		LoggerWarn("Something is trying to quickly render %d cubes. Renderer ignored its request");
		return;
	}
	glCallList(quicklyRenderList[count-1]);
}

int RE_InitFontRenderer(int width,int height)
{
	int result;
	char *font;
		//return 0;
	result = FT_Init_FreeType(&library);
	if(result)
		return result;
	font = OS_GetFontPath(FONT_DEFAULT,FONT_BACKUP);
	if(font==NULL)
	{
		LoggerFatal("Can't find font %s and %s",FONT_DEFAULT,FONT_BACKUP);
		return -1;
	}
	result = FT_New_Face(library,font,0,&face);
	free_s(font);
	if(result)
		return result;
	result = FT_Set_Char_Size(face,32<<6,28<<6,0,0);
	if(result)
		return result;
	result = FT_Select_Charmap(face,FT_ENCODING_UNICODE);
	if(result)
		return result;
	textTextureCache = LinkedListCreate();
	return 0;
}

BOOL RE_DestroyTextTexture(void *texture)
{
	TextTexture *textTexture=(TextTexture*)texture;
	if(!textTexture->isStatic)
	{
		free_s(textTexture->text);
	}
	RE_UnloadTexture(textTexture->texture.id);
	free_s(textTexture);
	return TRUE;
}

void RE_DestroyFontRenderer()
{
	if(face!=NULL)
		FT_Done_Face(face);
	FT_Done_FreeType(library);
	if(textTextureCache!=NULL)
	{
		LinkedListDestory(textTextureCache,RE_DestroyTextTexture);
	}
}

void RE_DrawTextStatic( unsigned long* text,float x,float y,float width )
{
	LinkedListIterator *iterator;
	TextTexture *texture = NULL;
	for(iterator=LinkedListGetIterator(textTextureCache);LinkedListIteratorHasNext(iterator);)
	{
		texture = (TextTexture*)LinkedListIteratorGetNext(iterator);
		if(texture->text==text && MathFloatEqual(texture->width,width))
		{
			LinkedListIteratorPullUpCurrent(iterator);
			texture->life=100;
			break;
		}
	}
	if(texture==NULL)
	{
		texture = RE_ProcessTextTexture(text,width);
		texture->width = width;
		texture->text=text;
		texture->isStatic=TRUE;
		texture->life=100;
		LinkedListOffer(textTextureCache,texture);
	}
	RE_BindTexture(&(texture->texture));
	RE_DrawRectWithTexture(x,y,texture->texture.width/(float)windowWidth,texture->texture.height/(float)windowHeight,0,0,1,1);
}

void RE_DrawTextVolatile( unsigned long* text,float x,float y,float width )
{

}

TextTexture* RE_ProcessTextTexture( unsigned long* unicodeText,float maxWidth )
{
	GLubyte *bytes;
	FT_GlyphSlot slot = face->glyph;
	FT_UInt glyph_index; 
	int i,j,w,h;
	TextTexture* texture;
	while(*unicodeText!=0)
	{
		glyph_index = FT_Get_Char_Index( face, *unicodeText ); 
		FT_Load_Glyph(face,glyph_index,FT_LOAD_TARGET_NORMAL  );  /* load flags, see below */
		FT_Render_Glyph( slot,FT_RENDER_MODE_NORMAL);
		//w = MathNextPower2(slot->bitmap.width);
		//h = MathNextPower2(slot->bitmap.rows);
		w = slot->bitmap.width;
		h = slot->bitmap.rows;
		bytes = (GLubyte *)malloc_s(w*h*sizeof(GLubyte));
		//memset(bytes,0,w*h*sizeof(GLubyte));
		for(j=0; j <h;j++) {
			for(i=0; i < w; i++){
				bytes[i+(h-j-1)*w]= i>=slot->bitmap.width||j>=slot->bitmap.rows?0:slot->bitmap.buffer[i + w*j];
			}
		}
		//memcpy(bytes,slot->bitmap.buffer,w*h);
		texture = (TextTexture*)malloc_s(sizeof(TextTexture));
		texture->texture.id = RE_ProcessRawTexture(bytes,GL_ALPHA8,GL_ALPHA,slot->bitmap.width,slot->bitmap.rows);
		texture->texture.width=w;
		texture->texture.height=h;
		free_s(bytes);
		return texture;
		unicodeText++;
	}
}
