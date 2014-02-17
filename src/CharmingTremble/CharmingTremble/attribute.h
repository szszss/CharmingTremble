#pragma once
#include "game.h"

struct implAttribute
{
	/*当一个新的属性被添加到实体上时触发.long long*为附加数据值.返回值为是否同意添加.*/
	BOOL (*onAdd)(World*,Entity*,long long*);
	/*当一个已有属性被延续时触发.第一个uint32为剩余时间(Tick),第二个uint32为已存在时间(Tick),long long*为附加数据值.返回值为是否同意延续.*/
	BOOL (*onExtend)(World*,Entity*,unsigned long,unsigned long,long long*);
	/*每tick触发一次.第一个uint32为剩余时间(Tick),第二个uint32为已存在时间(Tick),long long*为附加数据值.返回值为是否继续存在(返回FALSE立刻引发销毁和onExpire)*/
	BOOL (*onUpdate)(World*,Entity*,unsigned long,unsigned long,long long*);
	/*每次渲染触发一次.第一个uint32为剩余时间(Tick),第二个uint32为已存在时间(Tick),long long*为附加数据值.*/
	void (*onRender)(World*,Entity*,unsigned long,unsigned long,long long*);
	/*属性消除时引发.第一个uint32为剩余时间(Tick,这是因为属性可能会提前消失),第二个uint32为已存在时间(Tick),long long*为附加数据值.*/
	void (*onExpire)(World*,Entity*,unsigned long,unsigned long,long long*);
};

struct implAttributeInstance
{
	Attribute *attribute;
	/*剩余的Tick*/
	unsigned long lastLife;
	/*存在了多久(Tick)*/
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

