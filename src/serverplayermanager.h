#ifndef MERC_SERVERPLAYERMANAGER_H
#define MERC_SERVERPLAYERMANAGER_H

#include "Log.h"
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
	ServerPlayer *GetPlayer(IPaddress ip);	
	void Add(char *name,IPaddress *ip,int id,char *modelname,char *planetname);
	void Delete(int id);
	int GetNumPlayers(){
		return NumPlayers;
	}
	void AddMessage(int MessageID,...);
	ServerPlayer *GetPlayerByNum(int num);
	void SetPlayerState(int id,Vector3 &Position,Vector3 &Velocity,float *Rotation,int 
FeetState);
	void PlayerAddRotation(int id,float *Rotation);

};

#endif