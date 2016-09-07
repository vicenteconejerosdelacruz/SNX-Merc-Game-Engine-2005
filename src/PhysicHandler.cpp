#include <stdarg.h>

#include "PhysicHandler.h"
#include "ConfigFile.h"
#include "Log.h"


PhysicHandler::PhysicHandler()
{
	NumPlayers=0;
	NumObjects=0;
	NumBodies=0;
	PBlist=NULL;
	POlist=NULL;
	PPlist=NULL;
}

void PhysicHandler::Init(unsigned char *HeightMapPtr,int sizeX,int sizeY)
{
	World=dWorldCreate();
	space=dSimpleSpaceCreate(0);

	dWorldSetGravity(World,(dReal)GRAVITY_X,(dReal)GRAVITY_Y,(dReal)GRAVITY_Z);

	dReal *HeightReal;
	HeightReal = new dReal[sizeX*sizeY];

	for(int i=0;i<sizeX*sizeY;i++)
		HeightReal[i]=HeightMapPtr[i];

	Terrain=dCreateTerrainY(space,HeightReal,(dReal)sizeX,sizeY,8,8);

}

void PhysicHandler::AddObject(int Type, ...)
{
	va_list va;
	va_start(va,Type);

	switch(Type)
	{
	case PHYSIC_OBJECT:
		PhysicObject *Obj;
		Obj=POlist;

		if(Obj==NULL)
		{
			POlist=new PhysicObject;
			POlist->next=NULL;
			POlist->prev=NULL;
			Obj=POlist;
		}
		else
		{
			while(Obj->next!=NULL)
				Obj=Obj->next;

			PhysicObject *NewObject;
			NewObject = new PhysicObject;

			Obj->next=NewObject;
			Obj->next->prev=Obj;
			Obj->next->next=NULL;
			Obj=Obj->next;

		}
		
		char *modelname;
		modelname=va_arg(va,char *);
		ConfigFile CF("PhysicModels.cfg");
		
		Obj->Geom=CreateGeom(CF.getString("%s.type",modelname));
		GeomSetParams(Obj->Geom,CF.getString("%s.size",modelname));
		
		float *Position;
		Position=va_arg(va,float *);
		dGeomSetPosition(Obj->Geom,Position[0],Position[1],Position[2]);

		float *Rotation;
		Rotation=va_arg(va,float *);
		dMatrix3 R;
		dRFromEulerAngles(R,Rotation[0],Rotation[1],Rotation[2]);
		dGeomSetRotation(Obj->Geom,R);		
		NumObjects++;
	break;
	}
}

void PhysicHandler::AddPlayer(int id)
{
	PhysicPlayer *Player;
	Player=PPlist;

	if(Player==NULL)
	{
		PPlist=new PhysicPlayer;
		PPlist->next=NULL;
		PPlist->prev=NULL;
		Player=PPlist;
	}
	else
	{
		while(Player->next!=NULL)
			Player=(PhysicPlayer *)Player->next;

		PhysicPlayer *NewPlayer;
		NewPlayer = new PhysicPlayer;

		Player->next=NewPlayer;
		Player->next->prev=(PhysicPlayer *)Player;
		Player->next->next=NULL;
		Player=(PhysicPlayer *)Player->next;
	}

	Player->PlayerID=id;

	Player->Body=dBodyCreate(World);
	float Position[3];
	GetSpawnPosition(Position);
	dBodySetPosition(Player->Body,Position[0],Position[1],Position[2]);	
	dMassSetCappedCylinderTotal(&Player->Mass,AVATAR_MASS,2,AVATAR_RADIUS,AVATAR_HEIGHT);
	dBodySetMass(Player->Body,&Player->Mass);

	Player->Geom=CreateGeom("Cylinder");
	GeomSetParams(Player->Geom,AVATAR_RADIUS,AVATAR_HEIGHT,0);
	dMatrix3 R;
    dRFromAxisAndAngle(R,1,0,0,M_PI/2);
    dBodySetRotation(Player->Body,R);
	dGeomSetRotation(Player->Geom,R);
	dGeomSetBody(Player->Geom,Player->Body);

	Player->AMotor=dJointCreateAMotor(World,0);
	dJointAttach(Player->AMotor,Player->Body,0);
	dJointSetAMotorMode(Player->AMotor,dAMotorEuler);
    dJointSetAMotorNumAxes(Player->AMotor,3);
    dJointSetAMotorAxis(Player->AMotor,0,0,1,0,0);
    dJointSetAMotorAxis(Player->AMotor,1,0,0,1,0);
    dJointSetAMotorAxis(Player->AMotor,2,0,0,0,1);
    dJointSetAMotorAngle(Player->AMotor,0,0);
    dJointSetAMotorAngle(Player->AMotor,1,0);
    dJointSetAMotorAngle(Player->AMotor,2,0);
    dJointSetAMotorParam(Player->AMotor,dParamLoStop,-0);
    dJointSetAMotorParam(Player->AMotor,dParamLoStop3,-0);
    dJointSetAMotorParam(Player->AMotor,dParamLoStop2,-0);
    dJointSetAMotorParam(Player->AMotor,dParamHiStop,0);
    dJointSetAMotorParam(Player->AMotor,dParamHiStop3,0);
    dJointSetAMotorParam(Player->AMotor,dParamHiStop2,0);	
	
	Player->Ray=dCreateRay(space,AVATAR_HEIGHT*2);

	NumPlayers++;
}

dGeomID PhysicHandler::CreateGeom(char *geomtype)
{

	if(strcmp("Box",geomtype)==0)
		return dCreateBox(space,1,1,1);
	if(strcmp("Sphere",geomtype)==0)
		return dCreateSphere(space,1);
	if(strcmp("Cylinder",geomtype)==0)
		return dCreateCCylinder(space,1,3);

	return NULL;
		
}

void PhysicHandler::GeomSetParams(dGeomID geom,char *params)
{
	int geomclass;
	geomclass=dGeomGetClass(geom);

	float Size[3];

	switch(geomclass)
	{
	case dBoxClass:
		sscanf(params,"%f | %f | %f",&Size[0],&Size[1],&Size[2]);
		dGeomBoxSetLengths(geom,Size[0],Size[1],Size[2]);
	break;
	case dSphereClass:
		sscanf(params,"%f",&Size[0]);
		dGeomSphereSetRadius(geom,Size[0]);
	break;
	case dCCylinderClass:
		sscanf(params,"%f | %f",&Size[0],&Size[1]);
		dGeomCCylinderSetParams(geom,Size[0],Size[1]);
	break;
	default:
		
	break;
	}	
}

void PhysicHandler::GeomSetParams(dGeomID geom,float X,float Y,float Z)
{
	int geomclass;
	geomclass=dGeomGetClass(geom);

	switch(geomclass)
	{
	case dBoxClass:
		dGeomBoxSetLengths(geom,X,Y,Z);
	break;
	case dSphereClass:
		dGeomSphereSetRadius(geom,X);
	break;
	case dCCylinderClass:
		dGeomCCylinderSetParams(geom,X,Y);
	break;
	default:
		
	break;
	}	

}

void PhysicHandler::GetSpawnPosition(float *Position)
{
	/*TODO:Implement a Super-Duper Spawn System*/
	Position[0]=500;
	Position[1]=500;
	Position[2]=500;
}

void PhysicHandler::AdvanceFrame()
{
	dWorldStepFast1(World,0.016f,4);
}

void PhysicHandler::GetPlayerState(int id,Vector3 &Position,Vector3 &Velocity)
{
	PhysicPlayer *PPptr=GetPlayer(id);
	const dReal *Pos;
	const dReal *Vel;

	Pos=dBodyGetPosition(PPptr->Body);
	Vel=dBodyGetLinearVel(PPptr->Body);

	Position.x(Pos[0]);
	Position.y(Pos[1]);
	Position.z(Pos[2]);

	Velocity.x(Vel[0]);
	Velocity.y(Vel[1]);
	Velocity.z(Vel[2]);
}

PhysicPlayer *PhysicHandler::GetPlayer(int id)
{
	PhysicPlayer *PPptr=PPlist;

	for(int i=0;i<NumPlayers;i++)
	{
		if(PPptr->PlayerID==id)
			return PPptr;

		PPptr=(PhysicPlayer *)PPptr->next;
	}

	return NULL;

}