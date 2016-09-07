#ifndef MERC_SERVERPLAYERMANAGER_H
#define MERC_SERVERPLAYERMANAGER_H

#include "ServerPlayer.h"

class ServerPlayerManager
{
private:
	ServerPlayer *SPlist;
	int NumPlayers;
public:
	ServerPlayerManager();
	ServerPlayer *GetPlayer(int id);
	ServerPlayer *GetPlayer(char *name);
	void Add(char *name,TCPsocket socket,int id,char *modelname,char *planetname);
	void Delete(int id);
	int GetNumPlayers(){return NumPlayers;}
	void AddMessage(int ID,int msgID,...);
	ServerPlayer *GetPlayerByNum(int num);
	void SetPlayerState(int id,Vector3 &Position,Vector3 &Velocity);
};

#endif