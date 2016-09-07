#ifndef MERC_SNETWORK_H
#define MERC_SNETWORK_H

#include <SDL/SDL_net.h>

#include "Network.h"
#include "Protocol.h"


class SNetwork : public Network
{
private:
	TCPsocket			ServerSocket;
	IPaddress			ServerIP;
public:
	void Init();
	TCPsocket CheckNewConnection();
	void AddSocket(TCPsocket socket);
	int isSocketActive(TCPsocket socket);
	int Recv(TCPsocket socket,MsgHeader *header,char *Data);
	void Send(TCPsocket socket,char *Data,int size);
	void DeleteSocket(TCPsocket socket);
};

#endif