#ifndef MERC_SERVER_H
#define MERC_SERVER_H

#include "SNetwork.h"
#include "World_Server.h"
#include "DBConnector.h"
#include "ServerPlayerManager.h"
#include "Display.h"
#include "InputHandler.h"
#include "DynamicManager.h"

#define MAX_PLAYERS		40

#define OTIME_CONNECTION	10000

Uint32 SendPing(Uint32 interval, void* param);
Uint32 CreatePlayers(Uint32 interval,void *param);
Uint32 SendPlayersStates(Uint32 interval,void *param);
Uint32 SendBodyStates(Uint32 interval,void *param);
Uint32 MsgDispatcher(Uint32 interval,void *param);

typedef struct
{
	char name[32];
	char pass[32];
	IPaddress ip;
	Uint32 time;
}PoolElement;

class Server
{
private:
	World_Server *SListWorlds;
#ifdef MERC_DB
	DBConnector DB;
#endif
	
	int NumWorlds;
	int Services;

	//Timing Variables
	int delta;
	int now,last;
	int accum;

	//New Connection Pool
	PoolElement ConnectionPool[MAX_PLAYERS];
	int OffsetPool;

	//States Variables
	bool Running;
	bool UsingAcounts;
	bool bShutDown;
	int ServerType;//Dedicated or PlayerIsServer

	//Server Display
	Display SDisplay;

	//Client-if there is one data
	char PlayerName[20];
	InputHandler IHandler;
	DynamicManager DManager;

	int GetInitQueue();
	void InitWorldsUsingDB();
	void InitWorldsUsingLua();
public:
	SNetwork Net_Server;
	ServerPlayerManager SPManager;
	Server();
	~Server();
	bool IsInConnectionPool(IPaddress ip);
	PoolElement *GetPendingPlayer(IPaddress ip);
	void DeleteFromConnectionPool(IPaddress ip);
	void CheckZombies();
	int GetNumWorlds(){return NumWorlds;}
	void Init(int InitValue);
	void ShutDown();
	void AddWorld(char *name);
	void CreateTimers();
	void AdvanceFrame();
	void GetIncomingData();
	void DispatchMessages();
	bool ShutDownPending(){return bShutDown;}
	void HandleConnections();
	void AddClientUsingAccount(MsgLogin *MLogin,TCPsocket socket);
	World_Server *GetWorld(char *name);
	World_Server *GetWorldByNum(int num);
	void AddToConnectionPool(char *name, char *pass, IPaddress ip);
	void AddClient(char *name,char *pass,IPaddress ip);
	void HandleMessage(int PlayerId);
	void UpdateClientState(int PlayerID,MsgClientPositionChange *Change);
	void UpdateClientRotation(int PlayerID, MsgClientRotation *NewRot);
};

#endif