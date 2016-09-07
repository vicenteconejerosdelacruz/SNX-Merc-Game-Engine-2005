#ifndef MERC_PHYSICOBJECT_H
#define MERC_PHYSICOBJECT_H

#include <ode/ode.h>

class PhysicObject
{
public:
	PhysicObject *next;
	PhysicObject *prev;
	dGeomID Geom;
	PhysicObject(){Geom=NULL;next=NULL;prev=NULL;}
};

#endif