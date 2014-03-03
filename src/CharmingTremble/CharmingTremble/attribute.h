#pragma once
#include "game.h"

struct implAttribute
{
	/*��һ���µ����Ա���ӵ�ʵ����ʱ����.����ֵΪ�Ƿ�ͬ�����.*/
	BOOL (*onAdd)(World*,Entity*,AttributeInstance*);
	/*��һ���������Ա�����ʱ����.����ֵΪ�Ƿ�ͬ������.*/
	void (*onExtend)(World*,Entity*,AttributeInstance*);
	/*ÿtick����һ��.����ֵΪ�Ƿ��������(����FALSE�����������ٺ�onExpire)*/
	BOOL (*onUpdate)(World*,Entity*,AttributeInstance*);
	/*ÿ��3D��Ⱦ����һ��.*/
	void (*onRender)(World*,Entity*,AttributeInstance*);
	/*��������ʱ����.*/
	void (*onExpire)(World*,Entity*,AttributeInstance*);
	unsigned long defaultLife;
};

struct implAttributeInstance
{
	Attribute *attribute;
	/*ʣ���Tick*/
	unsigned long lastLife;
	/*�����˶��(Tick)*/
	unsigned long elapseTime;
	long long dataBits;
};

int InitAttributes();

#define AttributeAdd(world,entity,attribute) AttributeAddOrExtend(world,entity,attribute)
BOOL AttributeAddOnly(World *world,Entity *entity,Attribute *attribute);
static BOOL AttributeAddDo(World *world,Entity *entity,Attribute *attribute);
BOOL AttributeAddOrExtend(World *world,Entity *entity,Attribute *attribute);
BOOL AttributeExtendOnly(World *world,Entity *entity,Attribute *attribute);
AttributeInstance* AttributeGet(Entity *entity,Attribute *attribute);
void AttributeUpdate(World *world,Entity *entity);
void AttributeRender(World *world,Entity *entity);
int AttributeDestroyCallback(void *attributeInstance);

static BOOL AttributeDefaultOnAdd(World *world,Entity *entity,AttributeInstance *attributeInstance);
static void AttributeDefaultOnExtend(World *world,Entity *entity,AttributeInstance *attributeInstance);
static BOOL AttributeDefaultOnUpdate(World *world,Entity *entity,AttributeInstance *attributeInstance);
static void AttributeDefaultOnRender(World *world,Entity *entity,AttributeInstance *attributeInstance);
static void AttributeDefaultOnExpire(World *world,Entity *entity,AttributeInstance *attributeInstance);
static BOOL AttributeMossySlowOnUpdate(World *world,Entity *entity,AttributeInstance *attributeInstance);

