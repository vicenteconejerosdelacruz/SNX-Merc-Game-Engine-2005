#include "ClientPlayerManager.h"
#include "Protocol.h"
#include "Log.h"
#include "PhysicHandler.h"
#include "Action.h"

void ClientPlayerManager::SetClientName(const char *CName)
{
	memset(ClientName,0,sizeof(ClientName));
	strcat(ClientName,CName);
}

void ClientPlayerManager::ClientAddRotation(float *Rotation)
{
	ClientPlayer *CPptr=GetPlayer(ClientName);
	CPptr->Rotation[0]-=Rotation[0];
	CPptr->Rotation[1]+=Rotation[1];
}

void ClientPlayerManager::Add(char *name,int id,char *modelname,char *agency,char *race)
{
	ClientPlayer *CPptr;
	CPptr=CPlist;

	if(CPptr==NULL)
	{
		CPlist=new ClientPlayer;
		CPlist->next=NULL;
		CPlist->prev=NULL;
		CPptr=CPlist;
	}
	else
	{
		while(CPptr->next!=NULL)
			CPptr=(ClientPlayer *)CPptr->next;

		ClientPlayer *NewSPlayer;
		NewSPlayer = new ClientPlayer;

		CPptr->next=NewSPlayer;
		CPptr->next->prev=CPptr;
		CPptr->next->next=NULL;
		CPptr=(ClientPlayer *)CPptr->next;
	}

	CPptr->id=id;
	
	strcat(CPptr->name,name);
	strcat(CPptr->modelname,modelname);
	strcat(CPptr->race,race);
	strcat(CPptr->agency,agency);
	CPptr->active=false;

	CPptr->LoadModel("Humans");
	
	Log::Output("Player %s\n",CPptr->name);

	if(NumPlayers==0)
		Last=SDL_GetTicks();


	NumPlayers++;

}

void ClientPlayerManager::AdvanceFrame()
{
	ClientPlayer *CPptr;

	if(NumPlayers==0)
		return;

	Now=SDL_GetTicks();
	for(int i=0;i<NumPlayers;i++)
	{
		CPptr=GetPlayerByNum(i);

		if(CPptr==NULL)
		{
			Log::Output("cpptr==NULL\n");
			continue;
		}

		float dt=((float)Now-Last)*0.001f;
/*
		if(CPptr->FeetState==FREE_FALLING)
			CPptr->TargetVel.y(CPptr->TargetVel.y()-9.8f*dt);

		CPptr->TargetPos.x(CPptr->TargetPos.x()+CPptr->TargetVel.x()*dt);
		CPptr->TargetPos.y(CPptr->TargetPos.y()+CPptr->TargetVel.y()*dt);
		CPptr->TargetPos.z(CPptr->TargetPos.z()+CPptr->TargetVel.z()*dt);

		CPptr->Position.x((CPptr->Position.x()+CPptr->TargetPos.x())/2.0f);
		CPptr->Position.y((CPptr->Position.y()+CPptr->TargetPos.y())/2.0f);
		CPptr->Position.z((CPptr->Position.z()+CPptr->TargetPos.z())/2.0f);*/
	}
		
	Last=Now;
}

void ClientPlayerManager::SetState(MsgClientState *state)
{
	ClientPlayer *CPptr=GetPlayer(state->id);
	if(CPptr==NULL)
		return;

	CPptr->FirstHierarchy=state->FirstHierarchy;
	CPptr->SecondHierarchy=state->SecondHierarchy;
	CPptr->UltraHierarchy=state->UltraHierarchy;
	CPptr->FeetState=state->FeetState;

	if(strcmp(CPptr->name,ClientName)!=0)
	{
		CPptr->Rotation[0]=state->Rotation[0];
		CPptr->Rotation[1]=state->Rotation[1];
	}

/*	if(CPptr->active==false)
	{*/
		CPptr->Position.x(state->Position[0]);
		CPptr->Position.y(state->Position[1]);
		CPptr->Position.z(state->Position[2]);

		CPptr->Velocity.x(state->Velocity[0]);
		CPptr->Velocity.y(state->Velocity[1]);
		CPptr->Velocity.z(state->Velocity[2]);

	/*	CPptr->TargetPos.x(state->Position[0]);
		CPptr->TargetPos.y(state->Position[1]);
		CPptr->TargetPos.z(state->Position[2]);

		CPptr->TargetVel.x(state->Velocity[0]);
		CPptr->TargetVel.y(state->Velocity[1]);
		CPptr->TargetVel.z(state->Velocity[2]);
*/
		CPptr->active=true;
	/*}
	else
	{
		CPptr->TargetPos.x(state->Position[0]);
		CPptr->TargetPos.y(state->Position[1]);
		CPptr->TargetPos.z(state->Position[2]);

		CPptr->TargetVel.x(state->Velocity[0]);
		CPptr->TargetVel.y(state->Velocity[1]);
		CPptr->TargetVel.z(state->Velocity[2]);
	}*/
	
}

ClientPlayer *ClientPlayerManager::GetPlayerByNum(int PlayerNum)
{
	ClientPlayer *CPptr=CPlist;
		
	for(int i=1;i<PlayerNum;i++)
		CPptr=(ClientPlayer *)CPptr->next;

	if(CPptr!=NULL)
		return CPptr;
	
	return NULL;
}

ClientPlayer *ClientPlayerManager::GetPlayer(int id)
{
	ClientPlayer *CPptr=CPlist;

	for(int i=0;i<NumPlayers;i++)
	{
		if(CPptr->id==id)
			return CPptr;

		CPptr=(ClientPlayer *)CPptr->next;
	}

	return NULL;
}

ClientPlayer *ClientPlayerManager::GetPlayer(char *name)
{
	ClientPlayer *CPptr=CPlist;

	for(int i=0;i<NumPlayers;i++)
	{
		if(strcmp(CPptr->name,name)==0)
			return CPptr;

		CPptr=(ClientPlayer *)CPptr->next;
	}

	return NULL;
}