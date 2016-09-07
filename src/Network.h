#ifndef MERC_NETWORK_H
#define MERC_NETWORK_H

#include <SDL/SDL_net.h>

class Network
{
protected:
	int Port;
	SDLNet_SocketSet	SockSet;
public:
	virtual void Init(){};
};

#endif