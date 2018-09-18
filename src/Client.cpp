#include "Merc.h"
#include "Client.h"
#include "ConfigFile.h"
#include "Log.h"
#include "Action.h"
#include "Vector3.h"
#include "Network.h"

#include "AudioManager.h"

extern ConfigFile Config;//defined in Merc.cpp

Client::Client()
{
	memset(ClientName,0,sizeof(ClientName));
	memset(ClientPassword,0,sizeof(ClientPassword));
	memset(ServerDirection,0,sizeof(ServerDirection));
	ShutDown=false;
	PlayTime=-1;
	MovOffset=0;

}

int Client::GetInitQueue(){

	return INIT_DISPLAY|INIT_SOUND|INIT_INPUT|INIT_NETWORK;
}

void Client::Init()
{
	int InitQueue;

	strcat(ClientName,Config.getString("user"));
	CPManager.SetClientName(ClientName);

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

	if(SDL_WasInit(SDL_INIT_TIMER)==0)
		SDL_InitSubSystem(SDL_INIT_TIMER);

	SDL_AddTimer(50,SendPlayerRotation,this);
}

void Client::Authentificate()
{
	MsgLogin MLogin;

	memset(&MLogin,0,sizeof(MsgLogin));
	strcat(MLogin.user,ClientName);
	strcat(MLogin.pass,ClientPassword);
	strcat(MLogin.version,GAME_VERSION);

	Net_Client.Send(MSG_LOGIN,MLogin);
}

void Client::Connect()
{
	Net_Client.Connect(ServerDirection,PORT_SERVER);
}

void Client::AdvanceFrame()
{
	Input input;

	GetIncomingData();

	input=IHandler.CheckInput();
	
	UpdateGameState(input.GameState);

	if(!isPlaying())
		return;

	if(input.Rotation[0]!=0.0f||input.Rotation[1]!=0.0f)
		CPManager.ClientAddRotation(input.Rotation);

	ClientPlayer *PlayerPtr;
	PlayerPtr=CPManager.GetPlayer(ClientName);

	//RotationCorrection(PlayerPtr->Rotation);

	if(PlayerPtr->Rotation[0]	>=	1.0)
		PlayerPtr->Rotation[0]	=	1.0;
	if(PlayerPtr->Rotation[0]	<=	-1.0)
		PlayerPtr->Rotation[0]	=	-1.0;

	CWorld.PlaceCamera(	CPManager.GetPlayer(ClientName)->Position.x(),
						CPManager.GetPlayer(ClientName)->Position.y(),
						CPManager.GetPlayer(ClientName)->Position.z());
	CDisplay.PlaceCamera(CPManager.GetPlayer(ClientName));
	CPManager.AdvanceFrame();
	DManager.AdvanceFrame();
	CDisplay.Render();
	
	CheckInputForSending(input);

}

void Client::GetIncomingData()
{
	char Data[1024];
	memset(Data,0,sizeof(Data));
	Net_Client.Recv(Data);

	switch(Data[0])
	{
	case MSG_LOAD_MAP:
		CWorld.Init((Data+1));

		int numbuilds;
		numbuilds=CWorld.GetNumStructures();

		CDisplay.AddObject(&CWorld);
		for(int i=0;i<numbuilds;i++)
			CDisplay.AddObject(CWorld.GetStructureByNum(i));

		CDisplay.AddObject(CWorld.GetSkyDome());
	break;
	case MSG_CLIENT_DATA:
		MsgClientData *ClientData;
		ClientData=(MsgClientData *)(Data+1);

		CPManager.Add(	ClientData->name,ClientData->id,ClientData->model,ClientData->agency,						ClientData->race);
		CDisplay.AddObject(CPManager.GetPlayer(ClientData->name));
	break;
	case MSG_WAITING:
		Net_Client.Send(MSG_JOIN_GAME);
	break;
	case MSG_PING:
		Net_Client.Send(MSG_PING);
	break;
	case MSG_CLIENT_STATE:
		MsgClientState *state;

		state=(MsgClientState *)(Data+1);
		
		ClientPlayer *player;
		player=CPManager.GetPlayer(ClientName);
		if(player!=NULL)
		if(Playing==false)
			if(player->id==state->id)
			{
				if(player->FirstHierarchy==S1_DEATH&&state->FirstHierarchy!=S1_DEATH)
					Playing=true;

				if(state->FirstHierarchy==S1_DEATH)
				{
					//Log::Output("tenemo first como muerto :OO\n");
					memset(Movement,0,sizeof(Movement));
					MovOffset=0;
					PlayTime=-1;
				}
				else if(PlayTime==-1)
	 				PlayTime=SDL_GetTicks();
			}

		CPManager.SetState(state);
	break;
	}
}

void Client::UpdateGameState(int GameState)
{
	if((GameState&GAME_QUIT)==GAME_QUIT)
		ShutDown=true;

}

void Client::CheckInputForSending(Input input)
{
	bool SendState=false;
	if(PlayTime!=-1)
		if(input.KeyState!=Movement[MovOffset%MAX_MOVES].input.KeyState){
			SendState=true;
			MovOffset++;	
			Movement[MovOffset%MAX_MOVES].input=input;
			Movement[MovOffset%MAX_MOVES].state.Position=
				CPManager.GetPlayer(ClientName)->Position;
			Movement[MovOffset%MAX_MOVES].state.Velocity=
				CPManager.GetPlayer(ClientName)->Velocity;
			Movement[MovOffset%MAX_MOVES].time=SDL_GetTicks()-PlayTime;
		}

		
	if(SendState)
	{
		MsgClientPositionChange MChPos;

		MChPos.input=input.KeyState;
		Net_Client.Send(MSG_CLIENT_NEWPOS,MChPos);
	}
}

Uint32 SendPlayerRotation(Uint32 interval,void *param)
{

	Client *Cptr;
	ClientPlayerManager *CPMptr;
	ClientPlayer *CPptr;

	Cptr=(Client *)param;

	if(!Cptr->isPlaying())
		return interval;

	CPMptr=Cptr->GetPlayerManager();

	CPptr=CPMptr->GetPlayer(Cptr->GetClientName());

	MsgClientRotation MCRot;

	MCRot.Rotation[0]=CPptr->Rotation[0];
	MCRot.Rotation[1]=CPptr->Rotation[1];

	CNetwork *CNetptr;

	CNetptr=Cptr->GetNetwork();

	CNetptr->Send(MSG_CLIENT_ROT,MCRot);
	return interval;
}