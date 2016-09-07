#ifndef MERC_SERVERPLAYER_H
#define MERC_SERVERPLAYER_H

#include <SDL/SDL_net.h>
#include "Player.h"
#include "SWeapon.h"

class ServerPlayer:public Player
{
public:
	bool enabled;
	SWeapon *Wlist;
	TCPsocket socket;
	bool PingFree;
	char Messages[4000];
	int offset;
	ServerPlayer(){PingFree=true;memset(Messages,0,sizeof(Messages));offset=0;enabled=false;}
	int GetMessages(char *Data);
};

#endif