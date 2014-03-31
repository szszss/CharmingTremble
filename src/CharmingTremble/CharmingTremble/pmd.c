#include "pmd.h"
#include "memory.h"
#include "collection.h"
#include "util.h"
#include "resourcemanager.h"
#include "renderengine.h"
#include "SDL_opengl.h"
#include <memory.h>

HashTree *modelMap;
HashTree *animationMap;

BOOL PMD_CheckMagicNumber(FILE *file);
BOOL PMD_ReadHeader(PMD_Model *model,FILE *file);
BOOL PMD_ReadVertext(PMD_Model *model,FILE *file);
BOOL PMD_ReadIndex(PMD_Model *model,FILE *file);
BOOL PMD_ReadMaterial(PMD_Model *model,FILE *file,char *basePath);
BOOL PMD_ReadBone(PMD_Model *model,FILE *file);
BOOL PMD_MixMatrix(Matrix *result,Matrix *m1,Matrix *m2,float weight);
BOOL VMD_CheckMagicNumber(FILE *file);
//BOOL VMD_ReadHeader(PMD_Animation *animation,FILE *file);
BOOL VMD_ReadBoneKeyFrame(PMD_Animation *animation,FILE *file);
void CallbackModelDestroy(void *_model);
void CallbackAnimationDestroy(void *_animation);
void CallbackBoneDestroy(void *_bone);

void PMD_Init()
{
	modelMap = HashTreeCreate();
	animationMap = HashTreeCreate();
	LoggerInfo("PMD initialized");
}

void PMD_Close()
{
	HashTreeDestroy(modelMap,CallbackModelDestroy);
	HashTreeDestroy(animationMap,CallbackAnimationDestroy);
	LoggerInfo("PMD closed");
}

void CallbackModelDestroy(void *_model)
{
	PMD_Model *model = (PMD_Model*)_model;
	free_s(model->name);
	free_s(model->indexes);
	free_s(model->materials);
	free_s(model->vertexs);
	HashTreeDestroy(model->boneMapping,CallbackBoneDestroy);
	/*for(i=0;i<model->boneCount;i++)
	{
		free_s(model->bones[i].name);
	}*/
	free_s(model->bones);
	free_s(model);
}

void CallbackAnimationDestroy(void *_animation)
{
	int i;
	PMD_Animation *animation = (PMD_Animation*)_animation;
	for(i=0;i<animation->frameLength;i++)
	{
		PMD_KeyFrame *keyFrame = animation->keyFrame+i;
		free_s(keyFrame->boneName);
	}
	free_s(animation->keyFrame);
}

void CallbackBoneDestroy(void *_bone)
{
	unsigned short *id = (unsigned short*)_bone;
	free_s(id);
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
	sb = SBCreate();
	SBAppend(sb,basePath);
	SBAppend(sb,"/");
	newBasePath = SBBuildWithoutDestroy(sb);
	SBAppend(sb,fileName);
	filePath = SBBuild(sb);
	model = (PMD_Model*)HashTreeGet(modelMap,filePath,&result);
	if(result)
	{
		free_s(filePath);
		free_s(newBasePath);
		return model;
	}
	file = fopen(filePath,"rb");
	if(file==NULL)
	{
		free_s(filePath);
		free_s(newBasePath);
		return NULL;
	}
	if(!PMD_CheckMagicNumber(file))
	{
		free_s(filePath);
		free_s(newBasePath);
		return NULL;
	}
	model = (PMD_Model*)malloc_s(sizeof(PMD_Model));
	model->name = filePath;
	PMD_ReadHeader(model,file);
	PMD_ReadVertext(model,file);
	PMD_ReadIndex(model,file);
	PMD_ReadMaterial(model,file,newBasePath);
	PMD_ReadBone(model,file);
	free_s(newBasePath);
	fclose(file);
	HashTreeAdd(modelMap,filePath,model);
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
	unsigned long count = LESReadUInt32(file);
	StringBuilder *sb = NULL;
	char *buffer = (char *)malloc_s(21*sizeof(char)); //防溢出
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
		memset(buffer, 0, 21*sizeof(char));
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

BOOL PMD_ReadBone(PMD_Model *model,FILE *file)
{
	unsigned short i;
	unsigned short count = LESReadUInt16(file);
	model->boneCount = count;
	model->bones = (PMD_Bone *)malloc_s(count*sizeof(PMD_Bone));
	model->boneMapping = HashTreeCreate(); //不要忘了销毁时释放字符串内存(已实现)
	for(i=0;i<count;i++)
	{
		unsigned short *number = (unsigned short*)malloc_s(sizeof(unsigned short));
		//char *buffer = (char *)malloc_s(21*sizeof(char)); //防溢出
		memset(model->bones[i].name, 0, 21*sizeof(char));
		LESReadBytes(file,(byte*)model->bones[i].name,20);
		*number = i;
		HashTreeAdd(model->boneMapping, (model->bones[i].name) ,number);
		model->bones[i].parent = LESReadInt16(file);
		model->bones[i].child = LESReadInt16(file);
		model->bones[i].boneType = LESReadByte(file);
		model->bones[i].targetBone = LESReadInt16(file);
		model->bones[i].posX = LESReadFloat(file);
		model->bones[i].posY = LESReadFloat(file);
		model->bones[i].posZ = LESReadFloat(file);
	}
}

PMD_ModelInstance* PMD_ModelInstanceCreate(PMD_Model *model)
{
	int i;
	PMD_ModelInstance *modelInstance;
	PMD_BoneInstance *boneInstance;
	if(model==NULL)
		return NULL;
	modelInstance = (PMD_ModelInstance*)malloc_s(sizeof(PMD_ModelInstance));
	modelInstance->model = model;
	modelInstance->boneHierarchy.boneInstaces = (PMD_BoneInstance *)malloc_s(model->boneCount*sizeof(PMD_BoneInstance));
	for(i=0;i<model->boneCount;i++)
	{
		boneInstance = modelInstance->boneHierarchy.boneInstaces+i;
		boneInstance->bone = &model->bones[i];
		boneInstance->pass = 0;
		boneInstance->transformMatrix = MathMatrixCreate(NULL);
	}
	return modelInstance;
}

void PMD_ModelInstanceDestroy(PMD_ModelInstance *modelInstance)
{
	free_s(modelInstance->boneHierarchy.boneInstaces);
	free_s(modelInstance);
}

/*
void CallbackBoneMappingDestroy(void* _boneMapping)
{
	unsigned short *boneNumber = (unsigned short *)_boneMapping;
	free_s(boneNumber);
}

PMD_Model* PMD_DestroyModel(PMD_Model *model)
{
	free_s(model->indexes);
	free_s(model->materials);
	free_s(model->name);
	free_s(model->vertexs);
	HashTreeDestroy(model->boneMapping,&CallbackBoneMappingDestroy);
	free_s(model->bones); //骨骼内存释放得在映射表之后进行,这一点很重要.
	free_s(model);
}*/

BOOL PMD_MixMatrix(Matrix *result,PMD_BoneInstance *b0,PMD_BoneInstance *b1,float weight)
{
	if(weight>0.99f)
	{
		if((b0->pass&48) == 0)
			return FALSE;
		*result = b0->transformMatrix;
	}
	else if(weight>0.01f)
	{
		int i;
		if(((b0->pass&48) == 0) && ((b1->pass&48) == 0))
			return FALSE;
		for(i=0;i<16;i++)
		{
			*(&(result->m00)+i) = *(&(b0->transformMatrix.m00)+i) * weight + *(&(b1->transformMatrix.m00)+i) * (1.0f - weight);
		}
	}
	else
	{
		if((b1->pass&48) == 0)
			return FALSE;
		*result = b1->transformMatrix;
	}
	return TRUE;
}

void PMD_ModelInstanceRender(PMD_ModelInstance *modelInstance)
{
	static float scale = 0.15f;
	float vbuffer[3];
	Matrix matrix;
	Matrix *m1;
	Matrix *m2;
	int i,j,face;
	long materialPointer=-1;
	long materialThreshold=2000000000;
	long vertex;
	PMD_Model *model = modelInstance->model;
	int indexCount = model->indexCount;
	if(model->materialCount>0)
	{
		materialThreshold=0;		
	}
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//glEnable(GL_LIGHTING);
	//glShadeModel(GL_SMOOTH);
	glDisable(GL_CULL_FACE);
	glScalef(scale,scale,scale);
	glColor3f(1,1,1);
	for(i=0,face=0;i<indexCount;i++,face++)
	{
		if(face==materialThreshold)
		{
			materialPointer++;
			materialThreshold+=model->materials[materialPointer].faceAmount;
			RE_BindTexture(model->materials[materialPointer].texture);
			RE_SetMaterial(&(model->materials[materialPointer].diffuseR),
				&(model->materials[materialPointer].ambientR),
				&(model->materials[materialPointer].specularR),
				&(model->materials[materialPointer].shininess));
		}
		glBegin(GL_TRIANGLES);
		for(j=2;j>=0;j--)
		{
			vertex = model->indexes[i+j];
			vbuffer[0]=model->vertexs[vertex].x;
			vbuffer[1]=model->vertexs[vertex].y;
			vbuffer[2]=model->vertexs[vertex].z;
			if(!PMD_MixMatrix(&matrix, 
				&modelInstance->boneHierarchy.boneInstaces[model->vertexs[vertex].bone0].transformMatrix,
				&modelInstance->boneHierarchy.boneInstaces[model->vertexs[vertex].bone1].transformMatrix,
				model->vertexs[vertex].weight))
			{
				vbuffer[0]=model->vertexs[vertex].x;
				vbuffer[1]=model->vertexs[vertex].y;
				vbuffer[2]=model->vertexs[vertex].z;
			}
			else
			{
				MathMatrixMultiplyVector3(&matrix,model->vertexs[vertex].x, model->vertexs[vertex].y, model->vertexs[vertex].z,vbuffer);
			}
			glNormal3f(model->vertexs[vertex].nx, model->vertexs[vertex].ny, model->vertexs[vertex].nz);
			glTexCoord2f(model->vertexs[vertex].u,model->vertexs[vertex].v);
			glVertex3f(vbuffer[0], vbuffer[1], vbuffer[2]);
		}
		i+=2;
		glEnd();
	}
	RE_BindTexture(NULL);
	glPopAttrib();
}

PMD_Animation* PMD_LoadAnimation(char *basePath,char *fileName)
{
	FILE *file;
	PMD_Animation *animation = NULL;
	StringBuilder *sb = NULL;
	char *filePath = NULL;
	char *newBasePath = NULL;
	BOOL result;
	if(basePath==NULL||fileName==NULL)
	{
		return NULL;
	}
	sb = SBCreate();
	SBAppend(sb,basePath);
	SBAppend(sb,"/");
	newBasePath = SBBuildWithoutDestroy(sb);
	SBAppend(sb,fileName);
	filePath = SBBuild(sb);
	animation = (PMD_Animation*)HashTreeGet(animationMap,filePath,&result);
	if(result)
	{
		free_s(filePath);
		free_s(newBasePath);
		return animation;
	}
	file = fopen(filePath,"rb");
	if(file==NULL)
	{
		free_s(filePath);
		free_s(newBasePath);
		return NULL;
	}
	if(!VMD_CheckMagicNumber(file))
	{
		free_s(filePath);
		free_s(newBasePath);
		return NULL;
	}
	animation = (PMD_Animation*)malloc_s(sizeof(PMD_Animation));
	animation->name = filePath;
	//VMD_ReadHeader(animation,file);
	VMD_ReadBoneKeyFrame(animation,file);
	free_s(newBasePath);
	fclose(file);
	HashTreeAdd(animationMap,filePath,animation);
	return animation;
}

BOOL VMD_CheckMagicNumber(FILE *file)
{
	char magicNumber[30];
	memset(magicNumber,0,30*sizeof(char));
	LESReadBytes(file,(byte*)magicNumber,30);
	if(strcmp(magicNumber,"Vocaloid Motion Data file"))
	{
		LESReadBytes(file,(byte*)magicNumber,10);
		return TRUE;
	}
	else if(strcmp(magicNumber,"Vocaloid Motion Data 0002"))
	{
		LESReadBytes(file,(byte*)magicNumber,20);
		return TRUE;
	}
	return FALSE;
}

typedef struct
{
	char *name;
	float posX;
	float posY;
	float posZ;
	Quaternion rotQuaternion;
} _TEMP_KeyFrame;

BOOL VMD_ReadBoneKeyFrame(PMD_Animation *animation,FILE *file)
{
	unsigned long i;
	unsigned long count = LESReadUInt32(file);
	unsigned long postFrame = 0;
	unsigned long nowFrame;
	_TEMP_KeyFrame *tempFrame;
	LinkedList *list = LinkedListCreate();
	for(i=0;i<count;i++)
	{
		char *name = (char*)malloc_s(15*sizeof(char));
		LESReadBytes(file,(byte*)name,15);
		nowFrame = LESReadUInt32(file);
		tempFrame = (_TEMP_KeyFrame *)malloc_s(sizeof(_TEMP_KeyFrame));
		tempFrame->name = name;
		tempFrame->posX=LESReadFloat(file);
		tempFrame->posY=LESReadFloat(file);
		tempFrame->posZ=LESReadFloat(file);
		tempFrame->rotQuaternion=MathQuaternionCreate(NULL);
		tempFrame->rotQuaternion.x=LESReadFloat(file);
		tempFrame->rotQuaternion.y=LESReadFloat(file);
		tempFrame->rotQuaternion.z=LESReadFloat(file);
		tempFrame->rotQuaternion.w=LESReadFloat(file);
		LinkedListAdd(list,tempFrame);
		if(nowFrame>postFrame)
		{
			int j=0;
			LinkedListIterator *iterator;
			PMD_KeyFrame *keyFrame = (PMD_KeyFrame *)malloc_s(sizeof(PMD_KeyFrame)); 
			postFrame = nowFrame;
			keyFrame->nextFrame=postFrame;
			keyFrame->count=list->length;
			keyFrame->boneName=(char**)malloc_s(keyFrame->count*sizeof(char*));
			keyFrame->posX=(float*)malloc_s(keyFrame->count*sizeof(float));
			keyFrame->posY=(float*)malloc_s(keyFrame->count*sizeof(float));
			keyFrame->posZ=(float*)malloc_s(keyFrame->count*sizeof(float));
			keyFrame->rot=(Quaternion*)malloc_s(keyFrame->count*sizeof(Quaternion));
			for(iterator=LinkedListGetIterator(list);LinkedListIteratorHasNext(iterator);)
			{
				tempFrame = (_TEMP_KeyFrame *)LinkedListIteratorDeleteCurrent(iterator);
				keyFrame->boneName[j]=tempFrame->name;
				keyFrame->posX[j]=tempFrame->posX;
				keyFrame->posY[j]=tempFrame->posY;
				keyFrame->posZ[j]=tempFrame->posZ;
				keyFrame->rot[j]=tempFrame->rotQuaternion;
			}
		}
	}

	return TRUE;
}
