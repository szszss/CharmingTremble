#include "pmd.h"
#include "memory.h"
#include "collection.h"
#include "util.h"
#include "resourcemanager.h"
#include <memory.h>

HashTree *modelMap;

BOOL PMD_CheckMagicNumber(FILE *file);
BOOL PMD_ReadHeader(PMD_Model *model,FILE *file);
BOOL PMD_ReadVertext(PMD_Model *model,FILE *file);
BOOL PMD_ReadIndex(PMD_Model *model,FILE *file);
BOOL PMD_ReadMaterial(PMD_Model *model,FILE *file,char *basePath);
void CallbackModelDestroy(void *_model);

void PMD_Init()
{
	modelMap = HashTreeCreate();
	LoggerInfo("PMD initialized");
}

void PMD_Close()
{
	HashTreeDestroy(modelMap,CallbackModelDestroy);
	LoggerInfo("PMD closed");
}

void CallbackModelDestroy(void *_model)
{
	PMD_Model *model = (PMD_Model*)_model;
	free_s(model->indexes);
	free_s(model->materials);
	free_s(model->vertexs);
	free_s(model);
}

PMD_Model* PMD_LoadModel(char *basePath,char *fileName)
{
	FILE *file;
	PMD_Model *model = NULL;
	StringBuilder *sb = NULL;
	char *filePath = NULL;
	char *newBasePath = NULL;
	BOOL result;
	if(basePath==NULL||fileName==NULL)
	{
		return NULL;
	}
	model = (PMD_Model*)HashTreeGet(modelMap,fileName,&result);
	if(result)
	{
		return model;
	}
	sb = SBCreate();
	SBAppend(sb,basePath);
	SBAppend(sb,"/");
	newBasePath = SBBuildWithoutDestroy(sb);
	SBAppend(sb,fileName);
	filePath = SBBuild(sb);
	file = fopen(filePath,"rb");
	if(file==NULL)
	{
		return NULL;
	}
	if(!PMD_CheckMagicNumber(file))
	{
		return NULL;
	}
	model = (PMD_Model*)malloc_s(sizeof(PMD_Model));
	PMD_ReadHeader(model,file);
	PMD_ReadVertext(model,file);
	PMD_ReadIndex(model,file);
	PMD_ReadMaterial(model,file,newBasePath);
	free_s(filePath);
	free_s(newBasePath);
	fclose(file);
	HashTreeAdd(modelMap,fileName,model);
	return model;
}

BOOL PMD_CheckMagicNumber(FILE *file)
{
	char magicNumber[3];
	magicNumber[0] = LESReadChar(file);
	magicNumber[1] = LESReadChar(file);
	magicNumber[2] = LESReadChar(file);
	if(magicNumber[0]=='P' && magicNumber[1]=='m' && magicNumber[2]=='d')
	{
		return TRUE;
	}
	return FALSE;
}

BOOL PMD_ReadHeader(PMD_Model *model,FILE *file)
{
	float modelVersion;
	byte *rubbish = (byte*)malloc_s(256*sizeof(byte));
	modelVersion = LESReadFloat(file);
	LESReadBytes(file,rubbish,20);
	LESReadBytes(file,rubbish,256);
	free_s(rubbish);
	return !feof(file);
}

BOOL PMD_ReadVertext(PMD_Model *model,FILE *file)
{
	int i;
	unsigned long count = LESReadUInt32(file);
	model->vertexCount = count;
	model->vertexs = (PMD_Vertex *)malloc_s(count*sizeof(PMD_Vertex));
	for(i=0;i<count;i++)
	{
		model->vertexs[i].x=LESReadFloat(file);
		model->vertexs[i].y=LESReadFloat(file);
		model->vertexs[i].z=-LESReadFloat(file);
		model->vertexs[i].nx=LESReadFloat(file);
		model->vertexs[i].ny=LESReadFloat(file);
		model->vertexs[i].nz=-LESReadFloat(file);
		model->vertexs[i].u=LESReadFloat(file);
		model->vertexs[i].v=LESReadFloat(file);
		model->vertexs[i].bone0=LESReadUInt16(file);
		model->vertexs[i].bone1=LESReadUInt16(file);
		model->vertexs[i].weight=LESReadByte(file)/100.0f;	
		model->vertexs[i].edge=LESReadChar(file);
		if(feof(file))
			return FALSE;
	}
	return !feof(file);
}

BOOL PMD_ReadIndex(PMD_Model *model,FILE *file)
{
	int i;
	unsigned long count = LESReadUInt32(file);
	model->indexCount = count;
	model->indexes = (unsigned short *)malloc_s(count*sizeof(unsigned short));
	for(i=0;i<count;i++)
	{
		model->indexes[i] = LESReadUInt16(file);
	}
	return !feof(file);
}

BOOL PMD_ReadMaterial(PMD_Model *model,FILE *file,char *basePath)
{
	int i;
	PMD_Texture *texture = NULL;
	unsigned long count = LESReadUInt32(file);
	StringBuilder *sb = NULL;
	char *buffer = (char *)malloc_s(20*sizeof(char));
	char *textureName = NULL;
	model->materialCount = count;
	model->materials = (PMD_Material *)malloc_s(count*sizeof(PMD_Material));
	for(i=0;i<count;i++)
	{
		model->materials[i].diffuseR=LESReadFloat(file);
		model->materials[i].diffuseG=LESReadFloat(file);
		model->materials[i].diffuseB=LESReadFloat(file);
		model->materials[i].diffuseA=LESReadFloat(file);
		model->materials[i].shininess=LESReadFloat(file);
		model->materials[i].specularR=LESReadFloat(file);
		model->materials[i].specularG=LESReadFloat(file);
		model->materials[i].specularB=LESReadFloat(file);
		model->materials[i].ambientR=LESReadFloat(file);
		model->materials[i].ambientG=LESReadFloat(file);
		model->materials[i].ambientB=LESReadFloat(file);
		model->materials[i].toonNumber=LESReadByte(file);
		model->materials[i].edgeFlag=LESReadByte(file);
		model->materials[i].faceAmount=LESReadUInt32(file)/3;
		memset(buffer,0,20);
		LESReadBytes(file,(byte*)buffer,20);
		sb = SBCreate();
		SBAppend(sb,basePath);
		SBAppend(sb,buffer);
		textureName = SBBuild(sb);
		model->materials[i].texture=RM_GetTexture(textureName);
		free_s(textureName);
	}
	free_s(buffer);
	return !feof(file);
}
