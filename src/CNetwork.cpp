#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_Net.h>
#include "CNetwork.h"
#include "Log.h"

void CNetwork::Init()
{
	SDLNet_Init();
}

void CNetwork::Connect(const char *addr,int port)
{
	memset(Hostname,0,sizeof(Hostname));
 	strcat(Hostname,addr);
 
 	Port=port;
 
 	Log::Output("Connecting to %s\n",Hostname);
 
 	if(SDLNet_ResolveHost(&ServerIP,Hostname,Port) == -1){
 		Log::Output("Unable to resolve %s\n",Hostname);
 		Connected=false;
 		return;		
 	}
 
 	ServerSocket = SDLNet_TCP_Open(&ServerIP);
 	if(!ServerSocket){
 		Log::Output("Unable to create ServerSocket\n");
 		Connected=false;
 		return;
 	}

	SockSet = SDLNet_AllocSocketSet(1);
 	if(!SockSet){
 		Log::Output("Unable to create Socket set");
 		Connected=false;
 		return;
 	}
 
 	SDLNet_TCP_AddSocket(SockSet,ServerSocket);
 
 	Connected=true;

}

void CNetwork::Send(MsgHeader Header,...)
{
	va_list va;

	va_start(va,Header);

	switch(Header.type)
	{
	case MSG_LOGIN:
		MsgLogin Login;
		Login=va_arg(va,MsgLogin);

		SDLNet_TCP_Send(ServerSocket,&Header,sizeof(Header));
		SDLNet_TCP_Send(ServerSocket,&Login,sizeof(MsgLogin));
		Log::Output("sending Login %s %s\n",Login.user,Login.pass);
	break;
	default:
	break;
	}
}

void CNetwork::Recv(MsgHeader *header, char *Data)
{
	int ready = SDLNet_CheckSockets(SockSet,(Uint32)0);
	header->type=0;
	header->size=0;

	if(ready == -1){
		Log::Output("Disconnected");
		Connected=false;
		return;
	}

	if(SDLNet_SocketReady(ServerSocket)){

		int recv=SDLNet_TCP_Recv(ServerSocket,header,sizeof(MsgHeader));
		if(recv<=0){
			Log::Output("Disconnected");
			Connected=false;
			return;
		}
	}

	if(header->size>0)
		SDLNet_TCP_Recv(ServerSocket,Data,header->size);

}