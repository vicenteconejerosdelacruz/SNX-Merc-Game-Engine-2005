#ifndef MERC_CLIENTPLAYERMANAGER_H
#define MERC_CLIENTPLAYERMANAGER_H

#include <stdio.h>
#include <string.h>
#include <SDL/SDL.h>
#include "ClientPlayer.h"
#include "Protocol.h"

class ClientPlayerManager
{
private:
	ClientPlayer *CPlist;
	int NumPlayers;
	char ClientName[64];
	Uint32 Now,Last;
public:
	ClientPlayerManager(){CPlist=NULL;NumPlayers=0;memset(ClientName,0,sizeof(ClientName));}
	ClientPlayer *GetPlayerByNum(int PlayerNum);
	ClientPlayer *GetPlayer(int id);
	ClientPlayer *GetPlayer(char *name);
	void Add(char *name,int id,char *modelname,char *agency,char *race);
	void Delete(int id);
	void SetState(MsgClientState *state);
	void ClientAddRotation(float *Rotation);
	void SetClientName(const char *CName);
	void AdvanceFrame();
};

#endif