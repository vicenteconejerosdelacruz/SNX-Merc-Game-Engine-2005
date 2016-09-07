#ifndef MERC_PHYSICPLAYER_H
#define MERC_PHYSICPLAYER_H

#include <ode/ode.h>
#include "PhysicObject.h"

#define AVATAR_HEIGHT		10
#define AVATAR_RADIUS		3
#define AVATAR_MASS			80

class PhysicPlayer:public PhysicObject
{
public:
	int PlayerID;
	dBodyID Body;
	dJointID AMotor;
	dMass Mass;
	dGeomID Ray;
};

#endif