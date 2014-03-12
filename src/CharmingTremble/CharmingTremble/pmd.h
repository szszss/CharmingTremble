#ifndef pmd_h__
#define pmd_h__

#include "game.h"

struct impl_PMD_Vertex
{
	float x;
	float y;
	float z;
	float nx;
	float nz;
	float ny;
	float u;
	float v;
	unsigned short bone0;
	unsigned short bone1;
	float weight;
	char edge;
};

struct impl_PMD_Model
{
	PMD_Vertex *vertexs;
	unsigned short *indexes;
	PMD_Material *materials;
	unsigned long vertexCount;
	unsigned long indexCount;
	unsigned long materialCount;
};

struct impl_PMD_ModelInstance
{
	PMD_Model *model;
	double posX;
	double posY;
	double posZ;
	double homogeneity;
	double scaleX;
	double scaleY;
	double scaleZ;
};

/*
struct impl_PMD_Texture
{
	enum PMD_Texture_Status status;
	enum PMD_Texture_Type   type;     //The texture type. Such as normal, toon...
	enum PMD_Texture_Format format;   //The image format. Such as jpg, bmp...
	char *textureName;
	byte *rawData;               //The raw image data of texture, such as bitmap or raw jpg data, depends on image format.
	int texturegid;
	unsigned int used;           //How many models (not model instances) use this texture.
};
*/

struct impl_PMD_Material
{
	float diffuseR;
	float diffuseG;
	float diffuseB;
	float diffuseA;
	float shininess;
	float specularR;
	float specularG;
	float specularB;
	float ambientR;
	float ambientG;
	float ambientB;
	byte toonNumber;
	byte edgeFlag;
	unsigned long faceAmount;
	Texture *texture;
};

void PMD_Init();
void PMD_Close();

PMD_Model* PMD_LoadModel(char *basePath,char *fileName);

#endif // pmd_h__
