#ifndef MERC_PHYSICPLAYER_H
#define MERC_PHYSICPLAYER_H

#include <ode/ode.h>
#include "PhysicObject.h"

#define AVATAR_HEIGHT		10
#define AVATAR_RADIUS		3
#define AVATAR_MASS			80

enum FEET_STATES{FREE_FALLING=1,GROUND_TOUCHING,UNDER_WATER};

class PhysicPlayer:public PhysicObject
{
public:
	int AContinuos;
	int AInstant;
	int CurrentAction;
	int PlayerID;
	dBodyID Body;
	dJointID AMotor;
	dMass Mass;
	dGeomID Ray[9];
	bool MotionKeys[8];
	float Rotation[2];
	int FeetState;
};

#endif