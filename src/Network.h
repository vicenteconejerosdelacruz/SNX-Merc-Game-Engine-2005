#ifndef MERC_NETWORK_H
#define MERC_NETWORK_H

#include <SDL/SDL_net.h>

#define PORT_SERVER	6996
#define PORT_CLIENT	6997

class Network
{
protected:
	int Port;
	UDPpacket *in,*out;
public:
	virtual void Init(){};
};

#endif