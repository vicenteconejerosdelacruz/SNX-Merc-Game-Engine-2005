#ifndef MERC_CLIENT_H
#define MERC_CLIENT_H

#define MAX_MOVES		1024

#include "CNetwork.h"
#include "World_Client.h"
#include "DynamicManager.h"
#include "ClientPlayerManager.h"
#include "Display.h"
#include "InputHandler.h"
#include "Prediction.h"

Uint32 SendPlayerRotation(Uint32 interval,void *param);

class Client
{
private:
	CNetwork Net_Client;
	World_Client CWorld;
	DynamicManager DManager;
	ClientPlayerManager CPManager;
	Display	CDisplay;
	char ClientName[32];
	char ClientPassword[32];
	char ServerDirection[32];
	InputHandler IHandler;
	int GetInitQueue();
	bool Playing;
	bool ShutDown;
	int PlayTime;
	ClientMovement Movement[MAX_MOVES];
	int MovOffset;
public:
	Client();
	void Init();
	void AdvanceFrame();
	void Connect();
	void Authentificate();
	bool ShutDownPending(){return ShutDown;}
	void GetIncomingData();
	bool isPlaying(){return Playing;}
	void UpdateGameState(int GameState);
	void CheckInputForSending(Input input);
	ClientPlayerManager *GetPlayerManager(){return &CPManager;}
	CNetwork *GetNetwork(){return &Net_Client;}
	char *GetClientName(){return ClientName;}
};

#endif