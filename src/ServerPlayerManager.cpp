#include <stdarg.h>
#include <string.h>

#include <SDL/SDL.h>
#include <SDL/SDL_net.h>

#include "ServerPlayerManager.h"
#include "Protocol.h"
#include "Action.h"
#include "Log.h"
#include "Vector3.h"

ServerPlayerManager::ServerPlayerManager()
{
	NumPlayers=0;
	SPlist=NULL;
}

void ServerPlayerManager::Add(char *name,IPaddress *ip,int id,char *modelname,char *planetname)
{
	ServerPlayer *SPptr;
	SPptr=SPlist;
	
	if(SPptr==NULL)
	{
		SPlist=new ServerPlayer;
		SPlist->next=NULL;
		SPlist->prev=NULL;
		SPptr=SPlist;
	}
	else
	{
		while(SPptr->next!=NULL)
			SPptr=(ServerPlayer *)SPptr->next;

		ServerPlayer *NewSPlayer;
		NewSPlayer = new ServerPlayer;

		SPptr->next=NewSPlayer;
		SPptr->next->prev=SPptr;
		SPptr->next->next=NULL;
		SPptr=(ServerPlayer *)SPptr->next;
	}
	
	SPptr->id=id;
	SPptr->HasChange=true;

	strcat(SPptr->name,name);
	if(ip==NULL)
		SPptr->ip=ip;
	else
	{
		SPptr->ip=new IPaddress;
		SPptr->ip->host=ip->host;
		SPptr->ip->port=ip->port;
	}
	strcat(SPptr->modelname,modelname);
	strcat(SPptr->planetname,planetname);
	SPptr->FirstHierarchy=S1_LIVE;
	
	SPptr->LoadModel(modelname);

	NumPlayers++;

	Log::Output("Num Players %d\n",NumPlayers);
}

ServerPlayer *ServerPlayerManager::GetPlayer(int id)
{

	ServerPlayer *SPptr=SPlist;

	for(int i=0;i<NumPlayers;i++)
	{
		if(SPptr->id==id)
			return SPptr;

		SPptr=(ServerPlayer *)SPptr->next;
	}

	return NULL;
}

ServerPlayer *ServerPlayerManager::GetPlayer(IPaddress ip)
{

	ServerPlayer *SPptr=SPlist;

	for(int i=0;i<NumPlayers;i++)
	{
		if(SPptr->ip!=NULL)
			if(SPptr->ip->port==ip.port&&SPptr->ip->host==ip.host)
				return SPptr;

		SPptr=(ServerPlayer *)SPptr->next;
	}

	return NULL;
}


ServerPlayer *ServerPlayerManager::GetPlayer(char *name)
{
	ServerPlayer *SPptr=SPlist;

	for(int i=0;i<NumPlayers;i++)
	{
		if(strcmp(SPptr->name,name)==0)
			return SPptr;

		SPptr=(ServerPlayer *)SPptr->next;
	}

	return NULL;
}

ServerPlayer *ServerPlayerManager::GetPlayerByNum(int num)
{
	ServerPlayer *SPptr=SPlist;
	
	for(int i=0;i<num;i++)
		SPptr=(ServerPlayer *)SPptr->next;

	if(SPptr!=NULL)
		return SPptr;
	
	return NULL;
}

void ServerPlayerManager::SetPlayerState(int id,Vector3 &Position,Vector3 &Velocity,float *Rotation,int FeetState)
{
	ServerPlayer *SPptr=(ServerPlayer*)GetPlayer(id);

	if(	!(SPptr->CurrentMovement.state.Position==Position) ||
		!(SPptr->CurrentMovement.state.Velocity==Velocity))
	{
		SPptr->LastMovement.state.Position=
			SPptr->CurrentMovement.state.Position;

		SPptr->LastMovement.state.Velocity=
			SPptr->CurrentMovement.state.Velocity;

		SPptr->CurrentMovement.state.Position=Position;
		SPptr->CurrentMovement.state.Velocity=Velocity;

		SPptr->Position=Position;
		SPptr->Velocity=Velocity;

		SPptr->HasChange=true;
		SPptr->FeetState=FeetState;
	}
		
}

void ServerPlayerManager::PlayerAddRotation(int id,float *Rotation)
{
	ServerPlayer *SPptr ;

	SPptr=GetPlayer(id);

	//for(int i=0;i<2;i++)
	SPptr->Rotation[0]-=Rotation[0];
	SPptr->Rotation[1]+=Rotation[1];
}