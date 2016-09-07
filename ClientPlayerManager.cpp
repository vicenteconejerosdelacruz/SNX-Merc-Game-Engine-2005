#include "ClientPlayerManager.h"
#include "Protocol.h"
#include "Log.h"

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
	
	NumPlayers++;

}

void ClientPlayerManager::SetState(MsgClientState *state)
{
	ClientPlayer *CPptr=GetPlayer(state->uid);
	CPptr->Position.x(state->xpos);
	CPptr->Position.y(state->ypos);
	CPptr->Position.z(state->zpos);

	CPptr->Velocity.x(state->xvel);
	CPptr->Velocity.y(state->yvel);
	CPptr->Velocity.z(state->zvel);

	CPptr->Rotation[0]=state->Rotation[0];
	CPptr->Rotation[1]=state->Rotation[1];
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