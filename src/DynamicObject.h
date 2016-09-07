#ifndef MERC_DYNAMICOBJECT_H
#define MERC_DYNAMICOBJECT_H

#include "RenderAble.h"
#include "Model.h"

class DynamicObject:public RenderAble
{
public:
	DynamicObject *next;
	DynamicObject *prev;
	int id;
	float Position[3];
	float Velocity[3];
	float Rotation[3];
	Model model;
public:
	void Render();
};

#endif