#include <stdarg.h>
#include <SDL/SDL.h>
#include <windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <math.h>
#include "PhysicHandler.h"
#include "ConfigFile.h"
#include "Log.h"
#include "Action.h"

static void nearCallback (void *data, dGeomID o1, dGeomID o2);

dGeomID laweaderayo;
bool seteaoabool;
float start[3];
float end[3];
float ColitionPos[3];

PhysicHandler::PhysicHandler()
{
	NumPlayers=0;
	NumObjects=0;
	NumBodies=0;
	PBlist=NULL;
	POlist=NULL;
	PPlist=NULL;
}

void PhysicHandler::Init(unsigned char *HeightMapPtr,int size,float scale)
{
	World=dWorldCreate();
	dWorldSetContactMaxCorrectingVel (World,0.1);
	dWorldSetContactSurfaceLayer (World,0.001);
	dWorldSetAutoDisableLinearThreshold(World,0.10);
	dWorldSetAutoDisableAngularThreshold(World,0.10);
	dWorldSetAutoDisableSteps(World,3);
	//space=dSimpleSpaceCreate(0);
	//space=dHashSpaceCreate(0);
	dWorldSetAutoDisableFlag (World,1);
	dVector3 Center;
	dVector3 Extend;

	Center[0]=512;
	Center[1]=0;
	Center[2]=512;

	Extend[0]=1024;
	Extend[1]=1024;
	Extend[2]=1024;

	space=dQuadTreeSpaceCreate(0,Center,Extend,10);

	dWorldSetGravity(World,(dReal)GRAVITY_X,(dReal)GRAVITY_Y,(dReal)GRAVITY_Z);
	dWorldSetERP(World, 0.5f);
	dWorldSetContactSurfaceLayer(World, 0.0001f);
	dWorldSetContactMaxCorrectingVel(World, 5.0);
	dWorldSetCFM(World, 0.001);

	dReal *HeightReal;
	HeightReal = new dReal[size*size];

	for(int i=0;i<size*size;i++)
		HeightReal[i]=HeightMapPtr[i];

	Terrain=dCreateTerrainY(space,HeightReal,(dReal)size*scale,size,1,1);

	dGeomSetCategoryBits(Terrain,TERRAIN_CATEGORY_BITS);
	dGeomSetCollideBits(Terrain,TERRAIN_COLLIDE_BITS);

	contactgroup = dJointGroupCreate(0);

}

dGeomID PhysicHandler::CreateGeom(char *geomtype)
{

	if(strcmp("Box",geomtype)==0)
		return dCreateBox(space,1,1,1);
	if(strcmp("Sphere",geomtype)==0)
		return dCreateSphere(space,1);
	if(strcmp("CCylinder",geomtype)==0)
		return dCreateCCylinder(space,1,3);

	return NULL;
		
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


void PhysicHandler::AddObject(char *modelname,float *Position,float *Rotation)
{
	PhysicObject *Obj;
	Obj=POlist;

	char PhysicModelFile[255];
	memset(PhysicModelFile,0,sizeof(PhysicModelFile[255]));
	strcat(PhysicModelFile,"Models/");
	strcat(PhysicModelFile,modelname);
	strcat(PhysicModelFile,"/");
	strcat(PhysicModelFile,modelname);
	strcat(PhysicModelFile,".p4m");

	ConfigFile CF(PhysicModelFile);

	int nObjs=CF.getInteger("NumGeoms");
	for(int i=0;i<nObjs;i++)
	{
/*
		Log::Output("%d: type %s\n",i,CF.getString("Geom_%d_Type",i));
		Log::Output("%d: dims %s\n",i,CF.getString("Geom_%d_Size",i));
		Log::Output("%d: pos  %s\n",i,CF.getString("Geom_%d_Position",i));
		Log::Output("%d: rot  %s\n",i,CF.getString("Geom_%d_Rotation",i));
*/
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

		Obj->Geom=CreateGeom(CF.getString("Geom_%d_Type",i));
		dGeomSetCategoryBits(Obj->Geom,STATIC_CATEGORY_BITS);
		dGeomSetCollideBits(Obj->Geom,STATIC_COLLIDE_BITS);
		GeomSetParams(Obj->Geom,CF.getString("Geom_%d_Size",i));
		float rot[3];
		float pos[3];
		dQuaternion Q;

		sscanf(CF.getString("Geom_%d_Position",i),"%f | %f | %f",&pos[0],&pos[1],&pos[2]);
		sscanf(CF.getString("Geom_%d_Quaternion",i),"%f | %f | %f | %f",&Q[0],&Q[1],&Q[2],&Q[3]);

		dGeomSetPosition(Obj->Geom,Position[0]+pos[0],Position[1]+pos[1],Position[2]+pos[2]);
		dGeomSetQuaternion(Obj->Geom,Q);		

		NumObjects++;
	}
}

void PhysicHandler::AddBody(char *modelname,float *Position,float *Rotation)
{
	PhysicBody *Obj;
	Obj=PBlist;

	if(Obj==NULL)
	{
		PBlist=new PhysicBody;
		PBlist->next=NULL;
		PBlist->prev=NULL;
		Obj=PBlist;
	}
	else
	{
		while(Obj->next!=NULL)
			Obj=(PhysicBody *)Obj->next;

		PhysicBody *NewBody;
		NewBody = new PhysicBody;

		Obj->next=NewBody;
		Obj->next->prev=Obj;
		Obj->next->next=NULL;
		Obj=(PhysicBody *)Obj->next;
	}


	Obj->Geom=CreateGeom("Box");
	GeomSetParams(Obj->Geom,"8.0 | 8.0 | 8.0");
	dGeomSetCategoryBits(Obj->Geom,BODY_CATEGORY_BITS);
	dGeomSetCollideBits(Obj->Geom,BODY_COLLIDE_BITS);
	Obj->Body=dBodyCreate(World);
	
	dMassSetBoxTotal(&Obj->Mass,0.1,DIMX,DIMY,DIMZ);
	dBodySetMass(Obj->Body,&Obj->Mass);
	dGeomSetBody(Obj->Geom,Obj->Body);

	dBodySetPosition(Obj->Body,Position[0],Position[1],Position[2]);

	dMatrix3 R;
	dRFromEulerAngles(R,Rotation[0],Rotation[1],Rotation[2]);
	dBodySetRotation(Obj->Body,R);

	//dBodyDisable(Obj->Body);

	Obj->BodyID=NumBodies;
	memset(Obj->modelname,0,sizeof(Obj->modelname));
	strcat(Obj->modelname,modelname);
	NumBodies++;
	Log::Output("add num bodies %d\n",NumBodies);
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
	dBodySetAutoDisableFlag(Player->Body,0);
	float Position[3];
	GetSpawnPosition(Position);
	dBodySetPosition(Player->Body,Position[0],Position[1],Position[2]);	
	dMassSetCappedCylinderTotal(&Player->Mass,AVATAR_MASS,2,AVATAR_RADIUS,AVATAR_HEIGHT);
	dBodySetMass(Player->Body,&Player->Mass);

	Player->Geom=CreateGeom("CCylinder");
	dGeomSetCategoryBits(Player->Geom,PLAYER_CATEGORY_BITS);
	dGeomSetCollideBits(Player->Geom,PLAYER_COLLIDE_BITS);
	
	GeomSetParams(Player->Geom,AVATAR_RADIUS,AVATAR_HEIGHT,0);
	dMatrix3 R;
    dRFromAxisAndAngle(R,1,0,0,M_PI/2);
    dBodySetRotation(Player->Body,R);
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
	
	Player->Ray[0]=dCreateRay(space,AVATAR_HEIGHT*6);
	dGeomSetCategoryBits(Player->Ray[0],RAY_CATEGORY_BITS);
	dGeomSetCollideBits(Player->Ray[0],RAY_COLLIDE_BITS);
	dGeomRaySet(Player->Ray[0],Position[0],Position[1],Position[2],0,-1,0);

	for(int i=0;i<8;i++)
	{
		Player->Ray[i+1]=dCreateRay(space,20);
		dGeomSetCategoryBits(Player->Ray[i+1],RAY_CATEGORY_BITS);
		dGeomSetCollideBits(Player->Ray[i+1],RAY_COLLIDE_BITS);
		float x,z;

		x=cosf((float)i*PI/4);
		z=sinf((float)i*PI/4);

		dGeomRaySet(Player->Ray[i+1],Position[0],Position[1],Position[2],x,0,z);
	
		Player->MotionKeys[i]=true;
	}
	
	Player->Rotation[0]=0.0f;
	Player->Rotation[1]=0.0f;
	Player->FeetState=FREE_FALLING;
	
	NumPlayers++;
}

/*
dBodyID PhysicHandler::CreateBody(char *bodytype,char *mass)
{
	ret
	dBodyID Body;
	
	dBpd
	if(strcmp("Box",geomtype)==0)
	{
		Body=dBodyCreate(World);		
	}
	if(strcmp("Sphere",geomtype)==0)
	{
		Body=dBodyCreate(World);
		dBodySetMass(Body,

	}
	if(strcmp("Cylinder",geomtype)==0)
	{
		Body=dBodyCreate(World);
	}

	return Body;
}
*/
void PhysicHandler::GetSpawnPosition(float *Position)
{
	//TODO:Implement a Super-Duper Spawn System
	Position[0]=600;
	Position[1]=1000;
	Position[2]=300;
}

void PhysicHandler::AdvanceFrame()
{
	dSpaceCollide(space,this,nearCallback);
	//dWorldStepFast1(World,PHYSIC_STEP_SIZE,20);
	dWorldQuickStep(World,PHYSIC_STEP_SIZE);
	//dWorldStep(World,PHYSIC_STEP_SIZE);
	dJointGroupEmpty(contactgroup);

	for(int i=0;i<NumPlayers;i++)
	{
		PhysicPlayer *PPptr=GetPlayerByNum(i);

		const dReal *Position=dBodyGetPosition(PPptr->Body);
		dGeomRaySet(PPptr->Ray[0],Position[0],Position[1],Position[2],0,-1,0);

		for(int j=0;j<8;j++)
			dGeomRaySet(PPptr->Ray[j+1],Position[0],Position[1],Position[2],-cos(j*PI/4+PPptr->Rotation[1]),0,sin(j*PI/4+PPptr->Rotation[1]));
	}
}

void PhysicHandler::GetPlayerState(int id,Vector3 &Position,Vector3 &Velocity,float *Rotation,int *FeetState)
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

	Rotation[0]=PPptr->Rotation[0];
	Rotation[1]=PPptr->Rotation[1];

	*FeetState=PPptr->FeetState;
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

PhysicPlayer *PhysicHandler::GetPlayerByNum(int Num)
{
	PhysicPlayer *PPptr=PPlist;

	for(int i=0;i<Num;i++)
		PPptr=(PhysicPlayer *)PPptr->next;

	if(PPptr!=NULL)
		return PPptr;
	
	return NULL;

}

PhysicPlayer *PhysicHandler::GetPlayerByBodyID(dBodyID Body)
{
	PhysicPlayer *PPptr=PPlist;

	for(int i=0;i<NumPlayers;i++)
	{
		if(PPptr->Body==Body)
			return PPptr;

		PPptr=(PhysicPlayer *)PPptr->next;
	}

	return NULL;
}

PhysicPlayer *PhysicHandler::GetPlayerByRay(dGeomID Ray)
{
	PhysicPlayer *PPptr=PPlist;

	for(int i=0;i<NumPlayers;i++)
	{
		for(int j=0;j<9;j++)
		{
			if(PPptr->Ray[j]==Ray)
				return PPptr;
		}

		PPptr=(PhysicPlayer *)PPptr->next;
	}

	return NULL;
}


PhysicBody *PhysicHandler::GetBodyByNum(int Num)
{
	PhysicBody *PBptr=PBlist;

	for(int i=0;i<Num;i++)
		PBptr=(PhysicBody *)PBptr->next;

	if(PBptr!=NULL)
		return PBptr;
	
	return NULL;

}

PhysicObject *PhysicHandler::GetObjectByNum(int Num)
{
	PhysicObject *POptr=POlist;

	for(int i=0;i<Num;i++)
		POptr=(PhysicObject *)POptr->next;

	if(POptr!=NULL)
		return POptr;
	
	return NULL;

}

void PhysicHandler::GetBodyData(int BodyNum,int *BodyID,char *ModelName)
{
	PhysicBody *PBptr=GetBodyByNum(BodyNum);

	*BodyID=PBptr->BodyID;

	memset(ModelName,0,sizeof(ModelName));
	strcat(ModelName,PBptr->modelname);
}


void PhysicHandler::GetBodyState(int BodyNum,int *BodyID,Vector3 &Position,Vector3 &Velocity,Vector3 &AngularVel,Quaternion &Q)
{
	PhysicBody *PBptr;

	PBptr=GetBodyByNum(BodyNum);
	const dReal *Pos;
	const dReal *Vel;
	const dReal *Quat;
	const dReal *AngVel;

	Pos=dBodyGetPosition(PBptr->Body);
	Vel=dBodyGetLinearVel(PBptr->Body);
	Quat=dBodyGetQuaternion(PBptr->Body);
	AngVel=dBodyGetAngularVel(PBptr->Body);

	Position.x(Pos[0]);
	Position.y(Pos[1]);
	Position.z(Pos[2]);

	Velocity.x(Vel[0]);
	Velocity.y(Vel[1]);
	Velocity.z(Vel[2]);

	AngularVel.x(AngVel[0]);
	AngularVel.y(AngVel[1]);
	AngularVel.z(AngVel[2]);
		
	Q.w(Quat[0]);
	Q.x(Quat[1]);
	Q.y(Quat[2]);
	Q.z(Quat[3]);

	*BodyID=PBptr->BodyID;
}

void PhysicHandler::UpdatePlayerRotation(int id, float *Rotation)
{
	PhysicPlayer *PPlayerPtr;

	PPlayerPtr=GetPlayer(id);

	PPlayerPtr->Rotation[0]=Rotation[0];
	PPlayerPtr->Rotation[1]=Rotation[1];

	UpdatePlayer(id,PPlayerPtr->AContinuos,PPlayerPtr->AInstant,PPlayerPtr->CurrentAction);

}

void PhysicHandler::UpdatePlayer(int id,int ContinuosAction,int InstantAction,int Action)
{
	PhysicPlayer *PPlayerPtr;

	PPlayerPtr=GetPlayer(id);

	seteaoabool=false;
	
	//this shit is for the client aproximation
	/*PPlayerPtr->AContinuos=ContinuosAction;
	PPlayerPtr->CurrentAction=Action;
	PPlayerPtr->AInstant=InstantAction;*/

	PPlayerPtr->AContinuos=0;
	PPlayerPtr->CurrentAction=0;
	PPlayerPtr->AInstant=0;

	Vector3 Direction;

	Direction.x(-cos(PPlayerPtr->Rotation[1]));
	Direction.y(0);
	Direction.z(sin(PPlayerPtr->Rotation[1]));
	
	Vector3 Dir2;
	Dir2.x(cos(PPlayerPtr->Rotation[0]));
	Dir2.y(0);
	Dir2.z(sin(PPlayerPtr->Rotation[0]));

	const dReal *lapos=dBodyGetPosition(PPlayerPtr->Body);
	start[0]=lapos[0];
	start[1]=lapos[1];
	start[2]=lapos[2];

	end[0]=lapos[0]+Direction.x()*10;
	end[1]=lapos[1];
	end[2]=lapos[2]+Direction.z()*10;

	const dReal *CurrentVel;
	CurrentVel=dBodyGetLinearVel(PPlayerPtr->Body);

	if(	(ContinuosAction&S2_WALK	) == S2_WALK || 
		(ContinuosAction&S2_RUN 	) == S2_RUN  ||
		(ContinuosAction&S2_STAND	) == S2_STAND)
	{
		float VelX,VelY,VelZ;
		Vector3 Cross;
		Vector3 UP(0.0f,1.0f,0.0f);

		VelX=0.0f;
		VelY=0.0f;
		VelZ=0.0f;

		Cross=Direction.Cross(UP);
		Cross.Normalize();

		if((ContinuosAction&S2_STAND)==S2_STAND){
			VelX=0.0f;
			VelY=0.0f;
			VelZ=0.0f;			
		}
		if(	(ContinuosAction&S2_RUN)==S2_RUN){

			if((Action&A_MOVING_FRONT)==A_MOVING_FRONT){
				if(PPlayerPtr->MotionKeys[0]==true)
				{
					VelX+=Direction.x();
					VelZ+=Direction.z();
				}
			}
			if((Action&A_MOVING_BACK)==A_MOVING_BACK){
				if(PPlayerPtr->MotionKeys[4]==true)
				{
					VelX-=Direction.x();
					VelZ-=Direction.z();
				}
			}
			if((Action&A_STRAFE_RIGHT)==A_STRAFE_RIGHT){
				if(PPlayerPtr->MotionKeys[6]==true)
				{
					VelX+=Cross.x();
					VelZ+=Cross.z();
				}
			}

			if((Action&A_STRAFE_LEFT)==A_STRAFE_LEFT){
				if(PPlayerPtr->MotionKeys[2]==true)
				{
					VelX-=Cross.x();
					VelZ-=Cross.z();
				}
			}
			if((ContinuosAction&S2_WALK)==S2_WALK)
			{
				if((ContinuosAction&A_SILENT_WALK)==A_SILENT_WALK){
					VelX*=WALK_SPEED;
					VelZ*=WALK_SPEED;
 				}
			}
			else if((ContinuosAction&S2_RUN)==S2_RUN){
				VelX*=RUN_SPEED;
				VelZ*=RUN_SPEED;
			}
		}

		Vector3 DesiredVel(VelX,VelY,VelZ);
		Vector3 Force(UPDATE_FACTOR*(VelX-CurrentVel[0]),0,UPDATE_FACTOR*(VelZ-CurrentVel[2]));

		dBodyAddForce(PPlayerPtr->Body,Force.x(),Force.y(),Force.z());
	}

	for(int i=0;i<8;i++)
		PPlayerPtr->MotionKeys[i]=true;
}

static void nearCallback (void *data, dGeomID o1, dGeomID o2)
{
	PhysicHandler *PHptr;
	dBodyID b1,b2;
	PHptr=(PhysicHandler *)data;
	int nCollides;

	dGeomID Rayo=0;

	if(dGeomGetCategoryBits(o1)==RAY_CATEGORY_BITS)
		Rayo=o1;
	if(dGeomGetCategoryBits(o2)==RAY_CATEGORY_BITS)
		Rayo=o2;

	if(Rayo!=0)
	{
		PhysicPlayer *PPptr;

		PPptr=PHptr->GetPlayerByRay(Rayo);

		for(int i=0;i<8;i++)
		{
			if(Rayo==PPptr->Ray[i])
			{
				if(i==0)
				{
					dContact contact[1];
					nCollides=dCollide(o1,o2,1,&contact[0].geom,sizeof(dContact));
					const dReal *Position;
					const dReal *CurrentVel;
					Position=dBodyGetPosition(PPptr->Body);
					CurrentVel=dBodyGetLinearVel(PPptr->Body);
					dVector3 one,two;

					dGeomRayGet(Rayo,one,two);

					float ThaForce;
					if(contact[0].geom.depth<15 && nCollides>0)					{
						PPptr->FeetState=GROUND_TOUCHING;
						ThaForce=-1000*(contact[0].geom.depth-15)-900*CurrentVel[1];
						dBodyAddForce(PPptr->Body,0,ThaForce,0);
					}
					else
					{
						PPptr->FeetState=FREE_FALLING;
					}

				}
				else
				{
					dContact contact[1];
					nCollides=dCollide(o1,o2,1,&contact[0].geom,sizeof(dContact));
					if(contact[0].geom.depth<5 && nCollides>0)					
						PPptr->MotionKeys[i-1]=false;
				}
			}
		}
	}
	else
	{
		dContact contact[MAX_CONTACTS];
		nCollides=dCollide(o1,o2,MAX_CONTACTS,&contact[0].geom,sizeof(dContact));

		if(nCollides<1)
			return;

		for(int j=0;j<nCollides;j++){
			contact[j].surface.mode =  dContactApprox1 | dContactSlip1 | dContactSlip2 | dContactSoftCFM|dContactSoftERP;
			contact[j].surface.soft_cfm = (dReal)0.000001;
			contact[j].surface.soft_erp = (dReal)0.8;
			contact[j].surface.slip1 = 0.4;
			contact[j].surface.slip2 = 0.4;
			contact[j].surface.mu=dInfinity;
			dJointID c = dJointCreateContact(PHptr->GetWorld(),PHptr->GetContactGroup(),&contact[j]);

			dJointAttach(c,dGeomGetBody(o1),dGeomGetBody(o2));
		}
	}
}

void PhysicObject::Render()
{
	const dReal *Pos=dGeomGetPosition(Geom);
	const dReal *R=dGeomGetRotation(Geom);

	GLfloat matrix[16];
	matrix[0]=R[0];
	matrix[1]=R[4];
	matrix[2]=R[8];
	matrix[3]=0;
	matrix[4]=R[1];
	matrix[5]=R[5];
	matrix[6]=R[9];
	matrix[7]=0;
	matrix[8]=R[2];
	matrix[9]=R[6];
	matrix[10]=R[10];
	matrix[11]=0;
	matrix[12]=Pos[0];
	matrix[13]=Pos[1];
	matrix[14]=Pos[2];
	matrix[15]=1;

	if(dGeomGetClass(Geom)==dSphereClass)
	{
		const dReal *Pos=dGeomGetPosition(Geom);
		float radius=dGeomSphereGetRadius(Geom);
		glPushMatrix();
			GLUquadricObj *pObj = gluNewQuadric();
			glTranslatef(Pos[0],Pos[1],Pos[2]);
			gluQuadricDrawStyle(pObj, GLU_LINE);
			gluSphere(pObj,radius,30,30);
			gluDeleteQuadric(pObj);
		glPopMatrix();
	}
	else if(dGeomGetClass(Geom)==dCCylinderClass)
	{


		float radius,length;
		dGeomCCylinderGetParams(Geom,&radius,&length);

		glPushMatrix();
			GLUquadricObj *pObj = gluNewQuadric();
			glMultMatrixf(matrix);
			gluQuadricDrawStyle(pObj, GLU_LINE);
			gluCylinder(pObj,radius,radius,length,30,30);
			gluDeleteQuadric(pObj);
		glPopMatrix();

	}
	else if(dGeomGetClass(Geom)==dBoxClass)
	{
		const dReal *Pos=dGeomGetPosition(Geom);
		const dReal *R=dGeomGetRotation(Geom);
		
		dVector3 Dims;
		dGeomBoxGetLengths(Geom,Dims);
	
		float dimx=Dims[0];
		float dimy=Dims[1];
		float dimz=Dims[2];

		glPushMatrix();

			glMultMatrixf(matrix);
			glBegin(GL_LINES);

			glVertex3f(dimx/2,dimy/2,dimz/2);
			glVertex3f(dimx/2,dimy/2,-dimz/2);

			glVertex3f(-dimx/2,dimy/2,dimz/2);
			glVertex3f(-dimx/2,dimy/2,-dimz/2);

			glVertex3f(dimx/2,dimy/2,dimz/2);
			glVertex3f(-dimx/2,dimy/2,dimz/2);

			glVertex3f(dimx/2,dimy/2,-dimz/2);
			glVertex3f(-dimx/2,dimy/2,-dimz/2);

			glVertex3f(dimx/2,-dimy/2,dimz/2);
			glVertex3f(dimx/2,-dimy/2,-dimz/2);

			glVertex3f(-dimx/2,-dimy/2,dimz/2);
			glVertex3f(-dimx/2,-dimy/2,-dimz/2);

			glVertex3f(dimx/2,-dimy/2,dimz/2);
			glVertex3f(-dimx/2,-dimy/2,dimz/2);

			glVertex3f(dimx/2,-dimy/2,-dimz/2);
			glVertex3f(-dimx/2,-dimy/2,-dimz/2);

			glVertex3f(dimx/2,dimy/2,dimz/2);
			glVertex3f(dimx/2,-dimy/2,dimz/2);

			glVertex3f(-dimx/2,dimy/2,dimz/2);
			glVertex3f(-dimx/2,-dimy/2,dimz/2);

			glVertex3f(dimx/2,dimy/2,-dimz/2);
			glVertex3f(dimx/2,-dimy/2,-dimz/2);

			glVertex3f(-dimx/2,dimy/2,-dimz/2);
			glVertex3f(-dimx/2,-dimy/2,-dimz/2);
			
			glEnd();	
		glPopMatrix();


	}

}