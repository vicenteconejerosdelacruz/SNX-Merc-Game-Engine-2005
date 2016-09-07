#ifndef MERC_CLIENT_H
#define MERC_CLIENT_H

#include "CNetwork.h"
#include "World_Client.h"
#include "DynamicManager.h"
#include "ClientPlayerManager.h"
#include "Display.h"
#include "InputHandler.h"

class Client
{
private:
	CNetwork Net_Client;
	World_Client CWorld;
	DynamicManager DManager;
	ClientPlayerManager CPManager;
	Display	CDisplay;
	char ClientName[64];
	char ClientPassword[64];
	char ServerDirection[64];
	InputHandler IHandler;
	int GetInitQueue();
	bool Playing;
	bool ShutDown;
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
};

#endif