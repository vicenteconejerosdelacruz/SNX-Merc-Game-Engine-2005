#include <stdarg.h>
#include <string.h>
#include "ServerPlayerManager.h"
#include "Protocol.h"

ServerPlayerManager::ServerPlayerManager()
{
	NumPlayers=0;
	SPlist=NULL;
}

void ServerPlayerManager::Add(char *name,TCPsocket socket,int id,char *modelname,char *planetname)
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
	
	strcat(SPptr->name,name);
	SPptr->socket=socket;
	strcat(SPptr->modelname,modelname);
	strcat(SPptr->planetname,planetname);
	
	NumPlayers++;
}

void ServerPlayerManager::Delete(int id)
{

}

void ServerPlayerManager::AddMessage(int ID,int msgID,...)
{
	ServerPlayer *SPptr=(ServerPlayer*)GetPlayer(ID);
	ServerPlayer *SPInfo;

	va_list pa;

	va_start(pa,msgID);

	char *MessagePointer=SPptr->Messages+SPptr->offset;
	MsgHeader *header=(MsgHeader*)MessagePointer;
	header->type=msgID;

    switch(msgID)
    {
		case MSG_LOAD_MAP:
			header->size=20;
			MessagePointer+=sizeof(MsgHeader);
			strncpy(MessagePointer,va_arg(pa,char *),20);
			SPptr->offset+=sizeof(MsgHeader)+20;
		break;
		case MSG_CLIENT_DATA:
			MsgClientData cdata;
			memset(&cdata,0,sizeof(cdata));
			SPInfo=(ServerPlayer *)va_arg(pa,ServerPlayer *);

			strcat(cdata.name,SPInfo->name);
			cdata.id=SPInfo->id;
			strcat(cdata.agency,SPInfo->agency);
			strcat(cdata.race,SPInfo->race);
			strcat(cdata.model,SPInfo->modelname);
			header->size=sizeof(cdata);
			MessagePointer+=sizeof(MsgHeader);
			memcpy(MessagePointer,&cdata,sizeof(cdata));
			SPptr->offset+=sizeof(MsgHeader)+sizeof(cdata);
		break;
		case MSG_CLIENT_STATE:
			MsgClientState state;
			state.uid=ID;
			state.ping=0;
			state.FirstHierarchy=0;
			state.SecondHierarchy=0;
			state.time=0;
			state.UltraHierarchy=0;
			state.xpos=SPptr->Position.x();
			state.ypos=SPptr->Position.y();
			state.zpos=SPptr->Position.z();
			state.xvel=SPptr->Velocity.x();
			state.yvel=SPptr->Velocity.y();
			state.zvel=SPptr->Velocity.z();
			state.Rotation[0]=SPptr->Rotation[0];
			state.Rotation[1]=SPptr->Rotation[1];
			header->size=sizeof(state);
			MessagePointer+=sizeof(MsgHeader);
			memcpy(MessagePointer,&state,sizeof(state));
			SPptr->offset+=sizeof(MsgHeader)+sizeof(state);
		break;
		default:
		break;
    }

    va_end(pa);
	

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

void ServerPlayerManager::SetPlayerState(int id,Vector3 &Position,Vector3 &Velocity)
{
	ServerPlayer *SPptr=(ServerPlayer*)GetPlayer(id);
	SPptr->Position=Position;
	SPptr->Velocity=Velocity;
}