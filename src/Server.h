#ifndef MERC_SERVER_H
#define MERC_SERVER_H

#include "SNetwork.h"
#include "World_Server.h"
#include "DBConnector.h"
#include "ServerPlayerManager.h"

#define MAX_PLAYERS		40

Uint32 SendPing(Uint32 interval, void* param);
Uint32 CreatePlayers(Uint32 interval,void *param);
Uint32 SendPlayersStates(Uint32 interval,void *param);

class Server
{
private:
	SNetwork Net_Server;
	World_Server *SListWorlds;
	DBConnector DB;
	ServerPlayerManager SPManager;
	int NumWorlds;
	int Services;

	//Timing Variables
	int delta;
	int now,last;
	int accum;

	//New Connection Pool
	TCPsocket SocketPool[MAX_PLAYERS];
	int OffsetPool;

	//States Variables
	bool Running;
	bool UsingAcounts;
	bool bShutDown;

	int GetInitQueue();
	void InitWorldsUsingDB();
	void InitWorldsUsingLua();
public:
	Server();
	~Server();
	void Init();
	void ShutDown();
	void AddWorld(char *name);
	void CreateTimers();
	void AdvanceFrame();
	void GetIncomingData();
	void DispatchMessages();
	bool ShutDownPending();
	void HandleConnections();
	void AddClientUsingAccount(MsgLogin *MLogin,TCPsocket socket);
	void AddClient(MsgLogin *MLogin,TCPsocket socket);
	World_Server *GetWorld(char *name);
	void DeleteFromPool(TCPsocket socket);
};

#endif