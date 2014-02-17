#pragma once
#include "game.h"

struct implAttribute
{
	/*��һ���µ����Ա���ӵ�ʵ����ʱ����.long long*Ϊ��������ֵ.����ֵΪ�Ƿ�ͬ�����.*/
	BOOL (*onAdd)(World*,Entity*,long long*);
	/*��һ���������Ա�����ʱ����.��һ��uint32Ϊʣ��ʱ��(Tick),�ڶ���uint32Ϊ�Ѵ���ʱ��(Tick),long long*Ϊ��������ֵ.����ֵΪ�Ƿ�ͬ������.*/
	BOOL (*onExtend)(World*,Entity*,unsigned long,unsigned long,long long*);
	/*ÿtick����һ��.��һ��uint32Ϊʣ��ʱ��(Tick),�ڶ���uint32Ϊ�Ѵ���ʱ��(Tick),long long*Ϊ��������ֵ.����ֵΪ�Ƿ��������(����FALSE�����������ٺ�onExpire)*/
	BOOL (*onUpdate)(World*,Entity*,unsigned long,unsigned long,long long*);
	/*ÿ����Ⱦ����һ��.��һ��uint32Ϊʣ��ʱ��(Tick),�ڶ���uint32Ϊ�Ѵ���ʱ��(Tick),long long*Ϊ��������ֵ.*/
	void (*onRender)(World*,Entity*,unsigned long,unsigned long,long long*);
	/*��������ʱ����.��һ��uint32Ϊʣ��ʱ��(Tick,������Ϊ���Կ��ܻ���ǰ��ʧ),�ڶ���uint32Ϊ�Ѵ���ʱ��(Tick),long long*Ϊ��������ֵ.*/
	void (*onExpire)(World*,Entity*,unsigned long,unsigned long,long long*);
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

void InitAttributes();
#define AttributeAdd(world,entity,attribute) AttributeAddOrExtend(world,entity,attribute)
BOOL AttributeAddOnly(World *world,Entity *entity,Attribute *attribute);
BOOL AttributeAddOrExtend(World *world,Entity *entity,Attribute *attribute);
BOOL AttributeExtendOnly(World *world,Entity *entity,Attribute *attribute);

static BOOL AttributeDefaultOnAdd(World *world,Entity *entity,long long *data);
static BOOL AttributeDefaultOnExtend(World *world,Entity *entity,unsigned long life,unsigned long elapse,long long *data);
static BOOL AttributeDefaultOnUpdate(World *world,Entity *entity,unsigned long life,unsigned long elapse,long long *data);
static void AttributeDefaultOnRender(World *world,Entity *entity,unsigned long life,unsigned long elapse,long long *data);
static void AttributeDefaultOnExpire(World *world,Entity *entity,unsigned long life,unsigned long elapse,long long *data);

