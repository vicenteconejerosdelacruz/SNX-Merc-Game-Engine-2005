#ifndef MERC_SERVERPLAYER_H
#define MERC_SERVERPLAYER_H

#include <SDL/SDL.h>
#include <SDL/SDL_net.h>
#include "Player.h"
#include "SWeapon.h"
#include "Prediction.h"

class ServerPlayer:public Player
{
public:
	//member atributtes
	bool enabled;
	SWeapon *Wlist;
	IPaddress *ip;
	char Messages[4000];
	int offset;
	ClientMovement LastMovement;
	ClientMovement CurrentMovement;
	bool HasChange;
	int FeetState;
	
	bool PingFree;
	int ping;
	Uint32 PingTime;

	//member functions
	ServerPlayer(){
		PingFree=true;
		memset(Messages,0,sizeof(Messages));
		offset=0;
		enabled=false;
		memset(&LastMovement,0,sizeof(LastMovement));
		memset(&CurrentMovement,0,sizeof(CurrentMovement));
		HasChange=true;
		FeetState=0;
		}
	inline bool ChangeLastFrame(){return HasChange;}
	void Render();
};

#endif