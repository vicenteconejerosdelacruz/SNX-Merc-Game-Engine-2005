#include "Merc.h"
#include "Client.h"
#include "ConfigFile.h"
#include "Log.h"
#include "Action.h"

extern ConfigFile Config;//defined in Merc.cpp

Client::Client()
{
	memset(ClientName,0,sizeof(ClientName));
	memset(ClientPassword,0,sizeof(ClientPassword));
	memset(ServerDirection,0,sizeof(ServerDirection));
	ShutDown=false;
}

int Client::GetInitQueue(){

	return INIT_DISPLAY|INIT_SOUND|INIT_INPUT|INIT_NETWORK;
}

void Client::Init()
{
	int InitQueue;

	strcat(ClientName,Config.getString("user"));

	InitQueue=GetInitQueue();
	
	if((InitQueue&INIT_DISPLAY)==INIT_DISPLAY)
		CDisplay.Init();

	if((InitQueue&INIT_INPUT)==INIT_INPUT)
		IHandler.Init();

	if((InitQueue&INIT_NETWORK)==INIT_NETWORK)
	{
		Net_Client.Init();


		strcat(ServerDirection,
			(Config.getBoolean("server"))?"127.0.0.1":Config.getString("ServerIP"));

		strcat(ClientPassword,Config.getString("pass"));
	}
}

void Client::Authentificate()
{
	MsgHeader MHeader;
	MsgLogin MLogin;

	MHeader.type=MSG_LOGIN;
	MHeader.size=sizeof(MsgLogin);

	memset(&MLogin,0,sizeof(MsgLogin));
	strcat(MLogin.user,ClientName);
	strcat(MLogin.pass,ClientPassword);

	Net_Client.Send(MHeader,MLogin);
}

void Client::Connect()
{
	Net_Client.Connect(ServerDirection,MERC_NET_PORT);
}

void Client::AdvanceFrame()
{
	Input input;

	GetIncomingData();

	if(!isPlaying())
		return;

	CDisplay.Render();
	
	input=IHandler.CheckInput();

	UpdateGameState(input.GameState);
	
}

void Client::GetIncomingData()
{
	char Data[1024];
	memset(Data,0,sizeof(Data));
	MsgHeader MHeader;

	Net_Client.Recv(&MHeader,Data);

	if(MHeader.type==0)
		return;

	switch(MHeader.type){
	case MSG_LOAD_MAP:
		CWorld.Init(Data);
		CDisplay.AddObject(&CWorld);
	break;
	case MSG_CLIENT_DATA:
		MsgClientData *ClientData;

		ClientData=(MsgClientData *)Data;
		
		CPManager.Add(	ClientData->name,ClientData->id,ClientData->model,ClientData->agency,						ClientData->race);
		CDisplay.AddObject(CPManager.GetPlayer(ClientData->name));
	break;
	case MSG_CLIENT_STATE:
		MsgClientState *state;

		state=(MsgClientState *)Data;
		CPManager.SetState(state);

		if(Playing==false)
			if(CPManager.GetPlayer(ClientName)->id==state->uid)
				Playing=true;
	break;
	}
}

void Client::UpdateGameState(int GameState)
{
	if((GameState&GAME_QUIT)==GAME_QUIT)
		ShutDown=true;

}