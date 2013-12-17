#include "resourcemanager.h"
#include "collection.h"
#include "memory.h"
#include "png.h"
#include "SDL_opengl.h"
#include <string.h>

HashTree *textureHT = NULL;

void RM_DestroyTexture(void* p);
Texture* RM_LoadPNG(char* imageFile);

int RM_InitResourceManager()
{
	textureHT = HashTreeCreate();
	LoggerInfo("Resource manager initialized");
	return 0;
}

void RM_DestroyTexture(void* p)
{
	Texture *texture = (Texture*)p;
	RE_UnloadTexture(texture->id);
	free_s(texture); //yeah, just♂so♂easy
}

int RM_Close()
{
	if(HashTreeDestroy(textureHT,RM_DestroyTexture)==FALSE)
	{
		LoggerWarn("Something is wrong when destroying texture HT. This is just a warnning since the game is closing");
	}
	LoggerInfo("Resource manager closed");
	return 0;
}

Texture* RM_GetTexture( char* imageFile )
{
	Texture* texture = NULL;
	BOOL result;
	texture = (Texture*)HashTreeGet(textureHT,imageFile,&result);
	if(result==FALSE)
		return RM_LoadTexture(imageFile);
	return texture;
}

Texture* RM_LoadTexture( char* imageFile )
{
	static char suffix[]={0,0,0,0,0};
	Texture* texture = NULL;
	int i;
	i=strlen(imageFile);
	suffix[0]=imageFile[i-4];
	suffix[1]=imageFile[i-3];
	suffix[2]=imageFile[i-2];
	suffix[3]=imageFile[i-1];
	if(strnicmp(suffix,".png",4)==0)
	{
		texture = RM_LoadPNG(imageFile);
	}
	else
	{
		LoggerWarn("A texture can't be load because of unknown suffix");
	}
	if (texture == NULL)
		return NULL; //TODO:A ERROR IMAGE.
	HashTreeAdd(textureHT,imageFile,texture);
	LoggerInfo("A texture has been loaded");
	return texture;
}

Texture* RM_LoadPNG( char* imageFile )
{
	Texture* texture = NULL;
	FILE* file;
	unsigned char header[8]; //byte
	png_struct *png = NULL;
	png_info *info = NULL;
	png_info *end_info = NULL;
	int bitDepth, colorType;
	png_uint_32 width, height;
	int rowbytes;
	png_byte *rawData;
	png_byte ** rowPointers;
	unsigned int i;

	file = fopen(imageFile,"rb");
	if(file==NULL)
	{
		LoggerWarn("Failed to open a texture file");
		return NULL;
	}
	fread(header,1,8,file);
	if(png_sig_cmp(header,0,8))
	{
		LoggerWarn("Failed to load a png texture. The magic number is wrong");
		fclose(file);
		return NULL;
	}
	png = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL); //XXX:如果可以的话应该使用_2版本,即允许自定义内存函数的版本.
	if(png==NULL)
	{
		LoggerWarn("Failed to load a png texture. Can't create png reading struct");
		fclose(file);
		return NULL;
	}
	info = png_create_info_struct(png);
	end_info = png_create_info_struct(png);
	if(info==NULL || end_info==NULL)
	{
		LoggerWarn("Failed to load a png texture. Can't create png info struct");
		png_destroy_read_struct(&png, info==NULL?NULL:&info, NULL);
		fclose(file);
		return NULL;
	}
	if (setjmp(png_jmpbuf(png))) {
		LoggerWarn("Failed to load a png texture. Error is from pnglib");
		png_destroy_read_struct(&png, &info, &end_info);
		fclose(file);
		return NULL;
	}
	png_init_io(png, file);
	png_set_sig_bytes(png, 8);
	png_read_info(png, info);
	png_get_IHDR(png, info, &width, &height, &bitDepth, &colorType,NULL, NULL, NULL);
	if (bitDepth != 8)
	{
		LoggerWarn("Failed to load a png texture.The bit depth must be 8");
		png_destroy_read_struct(&png, &info, &end_info);
		fclose(file);
		return NULL;
	}
	png_read_update_info(png, info);
	rowbytes = png_get_rowbytes(png, info);
	rowbytes += 3 - ((rowbytes-1) % 4);
	rawData = (png_byte *)malloc_s(rowbytes * height * sizeof(png_byte)+15);
	rowPointers = (png_byte **)malloc_s(height * sizeof(png_byte*));
	for (i = 0; i < height; i++)
	{
		rowPointers[height - 1 - i] = rawData + i * rowbytes;
	}
	png_read_image(png, rowPointers);
	texture = (Texture*)malloc_s(sizeof(Texture));
	texture->width=width;
	texture->height=height;
	texture->id=RE_ProcessRawTexture(rawData,
		colorType==PNG_COLOR_TYPE_RGB_ALPHA?GL_RGBA8:GL_RGB8,
		colorType==PNG_COLOR_TYPE_RGB_ALPHA?GL_RGBA:GL_RGB,
		width,height);
	png_destroy_read_struct(&png, &info, &end_info);
	free(rawData);
	free(rowPointers);
	fclose(file);
	return texture;
}
