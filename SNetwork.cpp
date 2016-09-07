#include "SNetwork.h"
#include "Server.h"

void SNetwork::Init()
{
	SDLNet_Init();

	Port = 6996;
	SDLNet_ResolveHost(&ServerIP,NULL,Port);
	ServerSocket = SDLNet_TCP_Open(&ServerIP);

	SockSet = SDLNet_AllocSocketSet(MAX_PLAYERS);
	SDLNet_TCP_AddSocket(SockSet,ServerSocket);
}

TCPsocket SNetwork::CheckNewConnection()
{
	int ready=SDLNet_CheckSockets(SockSet,(Uint32)0);
	TCPsocket new_client;

	if(SDLNet_SocketReady(ServerSocket)){
		new_client=SDLNet_TCP_Accept(ServerSocket);
		return new_client;
	}

	return NULL;
}

void SNetwork::AddSocket(TCPsocket socket)
{
	SDLNet_TCP_AddSocket(SockSet,socket);
}

int SNetwork::isSocketActive(TCPsocket socket){

	return SDLNet_SocketReady(socket);
}

int SNetwork::Recv(TCPsocket socket, MsgHeader *header, char *Data)
{
	int recv;

	recv=SDLNet_TCP_Recv(socket,header,sizeof(MsgHeader));
	
	if(header->size > 0 && recv > 0)
		SDLNet_TCP_Recv(socket,Data,header->size);

	return recv;
}

void SNetwork::Send(TCPsocket socket,char *Data,int size){

	MsgHeader *MHeader;
	MHeader=(MsgHeader *)Data;

	if(size>0)
		SDLNet_TCP_Send(socket,Data,size);
}

void SNetwork::DeleteSocket(TCPsocket socket){

	SDLNet_TCP_DelSocket(SockSet,socket);
	SDLNet_TCP_Close(socket);
}