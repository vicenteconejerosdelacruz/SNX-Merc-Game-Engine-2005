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
	
	ClientSocket = SDLNet_UDP_Open(0);
	if(!ClientSocket){
 		Log::Output("Unable to create ClientSocket\n");
 		return;
 	}

	out=SDLNet_AllocPacket(65535);
	in=SDLNet_AllocPacket(65535);
}

void CNetwork::Connect(const char *addr,int port)
{
	memset(Hostname,0,sizeof(Hostname));
 	strcat(Hostname,addr);
 
 	Port=port;
 
  	if(SDLNet_ResolveHost(&ServerIP,Hostname,Port) == -1){
 		Log::Output("Unable to resolve %s\n",Hostname);
 		return;		
 	}

	SDLNet_UDP_Bind(ClientSocket,0,&ServerIP);
}

void CNetwork::Send(int MsgType,...)
{
	va_list va;

	va_start(va,MsgType);
	
	switch(MsgType)
	{
	case MSG_LOGIN:
		MsgLogin Login;
		Login=va_arg(va,MsgLogin);

		out->data[0]=MSG_LOGIN;
		out->len=sizeof(MsgLogin)+1;
		memcpy(out->data+1,&Login,sizeof(MsgLogin));
		
		SDLNet_UDP_Send(ClientSocket,0,out);

		Log::Output("sending Login %s %s %s\n",Login.user,Login.pass,Login.version);
	break;
	case MSG_JOIN_GAME:
		out->data[0]=MSG_JOIN_GAME;
		out->len=1;
		SDLNet_UDP_Send(ClientSocket,0,out);
	break;
	case MSG_PING:
		out->data[0]=MSG_PING;
		out->len=1;
		SDLNet_UDP_Send(ClientSocket,0,out);
	break;
	case MSG_CLIENT_NEWPOS:
		MsgClientPositionChange MChPos;
		MChPos=va_arg(va,MsgClientPositionChange);

		out->data[0]=MSG_CLIENT_NEWPOS;
		out->len=sizeof(MChPos)+1;
		memcpy(out->data+1,&MChPos,sizeof(MChPos));

		SDLNet_UDP_Send(ClientSocket,0,out);
	break;
	case MSG_CLIENT_ROT:
		MsgClientRotation MCRot;

		MCRot=va_arg(va,MsgClientRotation);

		out->data[0]=MSG_CLIENT_ROT;
		out->len=sizeof(MCRot)+1;

		memcpy(out->data+1,&MCRot,sizeof(MCRot));

		SDLNet_UDP_Send(ClientSocket,0,out);
	break;
	default:
	break;
	}
}

void CNetwork::Recv(char *Data)
{
	in->data[0]=0;
	SDLNet_UDP_Recv(ClientSocket,in);

	if(in->data[0]==0)
	{
		Data[0]=0;
	}
	else
	{
		//memcpy(.,.,0)?
		memcpy(Data,in->data,in->len);
	}
}