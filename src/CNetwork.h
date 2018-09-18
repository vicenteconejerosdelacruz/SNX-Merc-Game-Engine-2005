#ifndef MERC_CNETWORK_H
#define MERC_CNETWORK_H

#include <SDL/SDL_net.h>
#include "Network.h"
#include "Protocol.h"

class CNetwork:public Network
{
private:
	bool Connected;
	char Hostname[64];
	IPaddress ServerIP;
	UDPsocket ClientSocket;
public:
	void Init();
	void Recv(char *Data);
	void Send(int MsgType,...);
	bool isConnected();
	void Connect(const char *host,int port);
};

#endif