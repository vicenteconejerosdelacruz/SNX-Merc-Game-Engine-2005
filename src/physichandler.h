#ifndef MERC_PHYSICHANDLER_H
#define MERC_PHYSICHANDLER_H

#include <ode/ode.h>
#include "PhysicBody.h"
#include "PhysicObject.h"
#include "PhysicPlayer.h"
#include "Vector3.h"
#include "Quaternion.h"

#define GRAVITY_X					0.0
#define GRAVITY_Y					-75.8
#define GRAVITY_Z					0.0

#define PHYSIC_OBJECT				0x01
#define PHYSIC_STEP_SIZE			0.016f
#define MAX_CONTACTS				30

#define WALK_SPEED					50.0f
#define RUN_SPEED					80.0f

#define UPDATE_FACTOR				AVATAR_MASS/PHYSIC_STEP_SIZE

//Category Bits
#define TERRAIN_CATEGORY_BITS	1
#define STATIC_CATEGORY_BITS	2
#define PLAYER_CATEGORY_BITS	4
#define BODY_CATEGORY_BITS		8
#define RAY_CATEGORY_BITS		16
//Collide Bits
#define TERRAIN_COLLIDE_BITS	28
#define STATIC_COLLIDE_BITS		28
#define PLAYER_COLLIDE_BITS		15
#define BODY_COLLIDE_BITS		15
#define RAY_COLLIDE_BITS		3

#define DIMX					8
#define DIMY					8
#define DIMZ					8

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
	//dGeomID Terrain[1024/PATCH_SIZE*1024/PATCH_SIZE];
	dWorldID World;
	dSpaceID space;
	dJointGroupID contactgroup;
	dContact contact[MAX_CONTACTS];
	dGeomID CreateGeom(char *geomtype);
	//dBodyID CreateBody(char *bodytype);
	//dMassID *CreateMass(char *bodytype,char *totalmass);
	void GeomSetParams(dGeomID geom,char *params);
	void GeomSetParams(dGeomID geom,float X,float Y,float Z);
	void GetSpawnPosition(float *Position);
public:
	int GetNumPlayers(){return NumPlayers;}
	int GetNumBodies(){return NumBodies;}
	int GetNumObjects(){return NumObjects;}
	dGeomID GetTerrain(){return Terrain;}
	dWorldID GetWorld(){return World;}
	dJointGroupID GetContactGroup(){return contactgroup;}
	
	PhysicHandler();
	void Init(unsigned char *HeightMapPtr,int size,float scale);
	void SpawnPlayer(int id,float x,float y,float z);
	void AddPlayer(int id);
	void AddBody(char *modelname,float *Position,float *Rotation);
	void AddObject(char *modelname,float *Position,float *Rotation);
	void GetPlayerState(int id,Vector3 &Position,Vector3 &Velocity,float *Rotation,int *FeetState);
	void GetBodyState(int BodyNum,int *BodyID,Vector3 &Position,Vector3 &Velocity,Vector3 &AngularVel,Quaternion &Quater);
	void GetBodyData(int BodyNum,int *BodyID,char *ModelName);
	PhysicBody *GetBodyByNum(int Num);
	PhysicPlayer *GetPlayer(int id);
	PhysicPlayer *GetPlayerByNum(int Num);
	PhysicPlayer *GetPlayerByBodyID(dBodyID Body);
	PhysicPlayer *GetPlayerByRay(dGeomID Ray);
	PhysicObject *GetObjectByNum(int Num);
	void UpdateAvatarsStates();
	void UpdatePlayer(int id,int ContinuosAction,int InstantAction,int Action);
	void UpdatePlayerRotation(int id,float *Rotation);
	void AdvanceFrame();
	void DeleteObject();
	void DeleteBody();
	void DeletePlayer();
};

#endif