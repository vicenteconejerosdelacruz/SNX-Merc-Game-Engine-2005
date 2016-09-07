#ifndef MERC_CLIENTPLAYERMANAGER_H
#define MERC_CLIENTPLAYERMANAGER_H

#include <stdio.h>
#include "ClientPlayer.h"
#include "Protocol.h"

class ClientPlayerManager
{
private:
	ClientPlayer *CPlist;
	int NumPlayers;
public:
	ClientPlayerManager(){CPlist=NULL;NumPlayers=0;}
	ClientPlayer *GetPlayerByNum(int PlayerNum);
	ClientPlayer *GetPlayer(int id);
	ClientPlayer *GetPlayer(char *name);
	void Add(char *name,int id,char *modelname,char *agency,char *race);
	void Delete(int id);
	void SetState(MsgClientState *state);
};

#endif