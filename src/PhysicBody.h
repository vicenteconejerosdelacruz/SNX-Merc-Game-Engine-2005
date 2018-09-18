#ifndef MERC_PHYSICBODY_H
#define MERC_PHYSICBODY_H

#include <ode/ode.h>
#include "PhysicObject.h"

class PhysicBody:public PhysicObject
{
public:
	int BodyID;
	dBodyID Body;
	dMass Mass;
	dJointID Joint;
	char modelname[32];
};

#endif