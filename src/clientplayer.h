#ifndef MERC_CLIENTPLAYER_H
#define MERC_CLIENTPLAYER_H

#include "Player.h"
#include "Vector3.h"

class ClientPlayer:public Player
{
public:
	Vector3 TargetPos;
	Vector3 TargetVel;
	bool active;
	int FeetState;

	ClientPlayer(){active=false;}
	void Render();
};

#endif