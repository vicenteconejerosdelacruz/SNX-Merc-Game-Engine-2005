#ifndef MERC_PHYSICHANDLER_H
#define MERC_PHYSICHANDLER_H

#include <ode/ode.h>
#include "PhysicBody.h"
#include "PhysicObject.h"
#include "PhysicPlayer.h"
#include "Vector3.h"

#define GRAVITY_X					0.0
#define GRAVITY_Y					-75.0
#define GRAVITY_Z					0.0

#define PHYSIC_OBJECT				0x01
#define PHYSIC_STEP_SIZE			0.016f

class PhysicHandler
{
private:
	int NumPlayers;
	int NumBodies;
	int NumObjects;
	PhysicBody *PBlist;
	PhysicObject *POlist;
	PhysicPlayer *PPlist;
	dGeomID Terrain;
	dWorldID World;
	dSpaceID space;
	dGeomID CreateGeom(char *geomtype);
	void GeomSetParams(dGeomID geom,char *params);
	void GeomSetParams(dGeomID geom,float X,float Y,float Z);
	void GetSpawnPosition(float *Position);
public:
	PhysicHandler();
	void Init(unsigned char *HeightMapPtr,int sizeX,int sizeY);
	void SpawnPlayer(int id,float x,float y,float z);
	void AddPlayer(int id);
	void AddBody(int Type,...);
	void AddObject(int Type,...);
	void GetPlayerState(int id,Vector3 &Position,Vector3 &Velocity);
	void GetBodyState(int id,Vector3 *Position,Vector3 *Velocity,dQuaternion *quat);
	PhysicPlayer *GetPlayer(int id);
	void UpdatePlayer(int id,int Action,float *Rotation);
	void AdvanceFrame();
	void DeleteObject();
	void DeleteBody();
	void DeletePlayer();
};

#endif