#ifndef MERC_PHYSICOBJECT_H
#define MERC_PHYSICOBJECT_H

#include <ode/ode.h>
#include "RenderAble.h"

class PhysicObject:public RenderAble
{
public:
	PhysicObject *next;
	PhysicObject *prev;
	dGeomID Geom;
	PhysicObject(){Geom=NULL;next=NULL;prev=NULL;}

	void Render();

};

#endif